/*
 * EFIO_prv.h
 *
 *  Created on: Aug 24, 2026
 *      Author: LENOVO
 */

#ifndef MCAL_AFIO_AFIO_PRV_H_
#define MCAL_AFIO_AFIO_PRV_H_

#define AFIO_BASE_ADDR 0x40010000U

typedef struct {
	u32 EVCR;
	u32 MAPR;
	u32 EXTICR[4];
	u32 Reserved;
	u32 MAPR2;
} AFIOx_MemMap_t ;


#define AFIO ((volatile AFIOx_MemMap_t*)(AFIO_BASE_ADDR))

#endif /* MCAL_AFIO_AFIO_PRV_H_ */
