/*
 * DMA_int.h
 *
 *  Created on: Sep 6, 2026
 *      Author: LENOVO
 */

#ifndef MCAL_DMA_DMA_INT_H_
#define MCAL_DMA_DMA_INT_H_

typedef enum {
	MEMORY_TO_PERIPHERAL,
	MEMORY_TO_MEMORY,
	PERIPHERAL_TO_MEMORY
} DMA_DataDir_t;

typedef enum {
	Byte,
	HalfWord,
	Word,
} Size_t;

typedef enum {
	Low,
	Medium,
	High,
	VeryHigh,
} Priority_t;

void MDMA2_vInit(u8 A_u8StreamID, DMA_DataDir_t A_u8DIR);
void MDMA2_vSetStreamCfg(u8 A_u8StreamID, u32 *A_u32SourceAddr, u32 *A_u32DestinationAddr, Size_t A_xPSize, Size_t A_xMSize, u16 A_u16BlockSize);
void MDMA2_vEnableSream(u8 A_u8StreamID);
void MDMA2_vCallBack(u8 A_u8StreamID, void (*A_xFPTR)(void));

#endif /* MCAL_DMA_DMA_INT_H_ */
