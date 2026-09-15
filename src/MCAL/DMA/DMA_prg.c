#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "DMA_prv.h"
#include "DMA_cfg.h"
#include "DMA_int.h"

static void (*G_FPTR[7])(void) = { NULL };

void MDMA2_vInit(u8 A_u8StreamID, DMA_DataDir_t A_u8DIR)
{
	// Disable the stream
	CLR_BIT(DMA1->Stream[A_u8StreamID].CCR, 0);

	// Choose data direction
	switch (A_u8DIR)
	{
		// Memory to Peripheral mode
		case 0:
			CLR_BIT(DMA1->Stream[A_u8StreamID].CCR, 14);
			SET_BIT(DMA1->Stream[A_u8StreamID].CCR, 4);
			break;

		// Memory to Memory mode
		case 1:
			SET_BIT(DMA1->Stream[A_u8StreamID].CCR, 4);
			SET_BIT(DMA1->Stream[A_u8StreamID].CCR, 14);
			break;

		// Peripheral To Memory mode
		case 2:
			CLR_BIT(DMA1->Stream[A_u8StreamID].CCR, 4);
			CLR_BIT(DMA1->Stream[A_u8StreamID].CCR, 14);
			break;
	}

	// Enable increment for both peripheral and memory
	SET_BIT(DMA1->Stream[A_u8StreamID].CCR, 6); // PINC
	SET_BIT(DMA1->Stream[A_u8StreamID].CCR, 7); // MINC

	// Enable Transfer Complete Interrupt
	SET_BIT(DMA1->Stream[A_u8StreamID].CCR, 1); // TCIE
}

void MDMA2_vSetStreamCfg(u8 A_u8StreamID, u32 *A_u32SourceAddr,
						 u32 *A_u32DestinationAddr, Size_t A_xPSize,
						 Size_t A_xMSize, u16 A_u16BlockSize)
{
	// Set Addresses of peripheral and memory (source and destination)
	DMA1->Stream[A_u8StreamID].CPAR = (u32)A_u32SourceAddr;
	DMA1->Stream[A_u8StreamID].CMAR = (u32)A_u32DestinationAddr;

	// Reset Peripheral and Memory size
	DMA1->Stream[A_u8StreamID].CCR &= ~(0b1111 << 8);

	// Set Peripheral and Memory size
	DMA1->Stream[A_u8StreamID].CCR |= (A_xPSize << 8);
	DMA1->Stream[A_u8StreamID].CCR |= (A_xMSize << 10);

	// Set Block Size
	DMA1->Stream[A_u8StreamID].CNDTR = A_u16BlockSize;
}

void MDMA2_vEnableSream(u8 A_u8StreamID)
{
	SET_BIT(DMA1->Stream[A_u8StreamID].CCR, 0);
}

void MDMA2_vCallBack(u8 A_u8StreamID, void (*A_xFPTR)(void))
{
	G_FPTR[A_u8StreamID] = A_xFPTR;
}

void DMA1_Channel1_IRQHandler(void)
{
	if (G_FPTR[0] != NULL)
		G_FPTR[0]();

	DMA1->IFCR |= 0xF;
}

void DMA1_Channel4_IRQHandler(void)
{
	if (G_FPTR[3] != NULL)
		G_FPTR[3]();

	DMA1->IFCR |= 0xF;
}
