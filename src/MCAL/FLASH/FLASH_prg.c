#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "FLASH_prv.h"
#include "FLASH_int.h"


static void MFLASH_vUnlock(void)
{
	FLASH->KEYR = KEY1;
	FLASH->KEYR = KEY2;
}

void MFLASH_vMassErase()
{
	while (GET_BIT(FLASH->SR, BSY)){}

	if (GET_BIT(FLASH->CR, LOCK))
		MFLASH_vUnlock();

	SET_BIT(FLASH->CR, MER);
	SET_BIT(FLASH->CR, STRT);

	while (GET_BIT(FLASH->SR, BSY)){}

	CLR_BIT(FLASH->CR, MER);
}

void MFLASH_vPageErase(u32 A_u8PageAddr)
{
	while (GET_BIT(FLASH->SR, BSY)){}

	if (GET_BIT(FLASH->CR, LOCK))
		MFLASH_vUnlock();

	SET_BIT(FLASH->CR, PER);
	FLASH->AR = A_u8PageAddr;

	SET_BIT(FLASH->CR, STRT);

	while (GET_BIT(FLASH->SR, BSY)){}

	CLR_BIT(FLASH->CR, PER);
}

void MFLASH_vWriteProgram(u32 A_u32PageAddr, u16 *A_u16Data, u8 A_u8DataLength)
{
	// Make sure that the page is writable by erasing it
	MFLASH_vPageErase(A_u32PageAddr);

	while (GET_BIT(FLASH->SR, BSY)){}

	if (GET_BIT(FLASH->CR, LOCK))
		MFLASH_vUnlock();


	SET_BIT(FLASH->CR, PG);

	for (u8 i = 0; i < A_u8DataLength; i++)
	{
		*(volatile u16*)A_u32PageAddr = A_u16Data[i];
		A_u32PageAddr += 2;
		while (GET_BIT(FLASH->SR, BSY)){}
	}


	CLR_BIT(FLASH->CR, PG);
}

