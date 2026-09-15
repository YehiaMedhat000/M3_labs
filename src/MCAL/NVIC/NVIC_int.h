/*
 * NVIC_int.h
 *
 *  Created on: Aug 22, 2026
 *      Author: LENOVO
 */

#ifndef MCAL_NVIC_NVIC_INT_H_
#define MCAL_NVIC_NVIC_INT_H_

typedef enum {
	Group16Sub0 = 3,
	Group8Sub2,
	Group4Sub4,
	Group2Sub8,
	Group0Sub16,
} NVIC_Group_t;

typedef enum {
	NVIC_EXTI0 = 6,
	NVIC_EXTI1,
	NVIC_EXTI2,
	NVIC_EXTI3,
	NVIC_EXTI4,
	NVIC_DMA1_CHANNEL1,
	NVIC_DMA1_CHANNEL2,
	NVIC_DMA1_CHANNEL3,
	NVIC_DMA1_CHANNEL4,
	NVIC_DMA1_CHANNEL5,
	NVIC_DMA1_CHANNEL6,
	NVIC_DMA1_CHANNEL7,
} NVIC_VecTableLine_t;

void MNVIC_vEnablePeripheralINT(u8 A_u8Position);
void MNVIC_vDisablePeripheralINT(u8 A_u8Position);
void MNVIC_vSetPendingFlag(u8 A_u8Position);
void MNVIC_vClrPendingFlag(u8 A_u8Position);
void MNVIC_vSetGroupPriority(NVIC_Group_t A_xGroupPriority);
void MNVIC_vSetPeripheralPriority(u8 A_u8Position, u8 A_u8Group, u8 A_u8SubGroup);

u8 MNVIC_u8GetFlagStatus(u8 A_u8Position);


#endif /* MCAL_NVIC_NVIC_INT_H_ */
