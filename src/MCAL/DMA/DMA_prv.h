/*
 * DMA_prv.h
 *
 *  Created on: Sep 6, 2026
 *      Author: LENOVO
 */

#ifndef MCAL_DMA_DMA_PRV_H_
#define MCAL_DMA_DMA_PRV_H_

#define DMA1_BASE_ADDR 0x40020000U

typedef struct {
	u32 CCR;
	u32 CNDTR;
	u32 CPAR;
	u32 CMAR;
	u32 Reserved;
} DMA_Stream_t;

typedef struct {
	u32 ISR;
	u32 IFCR;
	DMA_Stream_t Stream[7];
} DMA_MemMap_t;

#define DMA1 ((volatile DMA_MemMap_t*)(DMA1_BASE_ADDR))

#endif /* MCAL_DMA_DMA_PRV_H_ */
