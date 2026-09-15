#include "../../LIB/BIT_MATH.h"
#include "../../LIB/STD_TYPES.h"
#include "EXTI_int.h"
#include "EXTI_prv.h"

static void (*G_FPTR[16])(void) = {NULL};

void MEXTI_vInit()
{
	EXTI->PR = 0xFFFFFFFF;
}
void MEXTI_vEnableINT(u8 A_u8LineNo)
{
	if (A_u8LineNo < 16)
		SET_BIT(EXTI->IMR, A_u8LineNo);
}
void MEXTI_vDisableINT(u8 A_u8LineNo)
{
	if (A_u8LineNo < 16)
		CLR_BIT(EXTI->IMR, A_u8LineNo);
}

void MEXTI_vSetTrigger(u8 A_u8LineNo, u8 A_u8Trigger)
{
	if (A_u8LineNo < 16)
	{
		switch (A_u8Trigger)
		{
			case EXTI_Rising:
				SET_BIT(EXTI->RTSR, A_u8LineNo);
				CLR_BIT(EXTI->FTSR, A_u8LineNo);
				break;

			case EXTI_Falling:
				SET_BIT(EXTI->FTSR, A_u8LineNo);
				CLR_BIT(EXTI->RTSR, A_u8LineNo);
				break;

			case EXTI_OnChange:
				SET_BIT(EXTI->RTSR, A_u8LineNo);
				SET_BIT(EXTI->FTSR, A_u8LineNo);
				break;

		}
	}
}

void MEXTI_vSoftwareINT(u8 A_u8LineNo)
{
	if (A_u8LineNo < 16)
		SET_BIT(EXTI->SWIER,A_u8LineNo);
}

void MEXTI_vSetCallBack(void (*FPTR)(void), u8 A_u8LineNo)
{
	if (A_u8LineNo < 16)
	{
		G_FPTR[A_u8LineNo] = FPTR;
	}
}

void EXTI0_IRQHandler(void)
{
	if (G_FPTR[L0] != NULL)
		G_FPTR[L0]();

	SET_BIT(EXTI->PR, L0);
}

void EXTI1_IRQHandler(void)
{
	if (G_FPTR[L1] != NULL)
		G_FPTR[L1]();

	SET_BIT(EXTI->PR, L1);
}

void EXTI2_IRQHandler(void)
{
	if (G_FPTR[L2] != NULL)
		G_FPTR[L2]();

	SET_BIT(EXTI->PR, L2);
}

void EXTI3_IRQHandler(void)
{
	if (G_FPTR[L3] != NULL)
		G_FPTR[L3]();

	SET_BIT(EXTI->PR, L3);
}
