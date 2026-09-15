/*
 * RCC_int.h
 *
 *  Created on: Aug 14, 2025
 *      Author: Nada Mamdouh
 *      Version: 0.1
 */

#ifndef MCAL_RCC_RCC_INT_H_
#define MCAL_RCC_RCC_INT_H_


/* Buses */
#define RCC_AHB     0
#define RCC_APB1    1
#define RCC_APB2    2

/* ------- Peripherals --------*/
// Bus : RCC_APB2 (Note: GPIOs on STM32F1 are on APB2)
typedef enum {
		RCC_AFIO,
		APB2_Res1,
		RCC_GPIOA,
		RCC_GPIOB,
		RCC_GPIOC,
		RCC_GPIOD,
		RCC_GPIOE,
		APB2_Res2,
		APB2_Res3,
		RCC_ADC1,
		RCC_ADC2,
		RCC_TIM1,
		RCC_SPI1,
		APB2_Res4,
		RCC_USART1
} RCCAPB2_PinMap_t;

typedef enum {
		RCC_TIM2,
		RCC_TIM3,
		RCC_TIM4,
		RCC_TIM5,
		RCC_TIM6,
		RCC_TIM7,
		APB1_Res1,
		APB1_Res2,
		APB1_Res3,
		APB1_Res4,
		APB1_Res5,
		RCC_WWDG,
		APB1_Res6,
		APB1_Res7,
		RCC_SPI2,
		RCC_SPI3,
		APB1_Res8,
		RCC_USART2,
		RCC_USART3,
		RCC_USART4,
		RCC_USART5,
		RCC_I2C1,
		RCC_I2C2,
		APB1_Res9,
		APB1_Res10,
		RCC_CAN1,
		RCC_CAN2,
		RCC_BKP,
		RCC_PWR,
		RCC_DAC,
} RCCAPB1_PinMap_t;

typedef enum {
	RCC_DMA1,
	RCC_DMA2,
	RCC_SRAM,
	AHB_Res1,
	RCC_FLITFEN,
	AHB_Res2,
	RCC_CRCEN,
	AHB_Res3,
	AHB_Res4,
	AHB_Res5,
	AHB_Res6,
	AHB_Res7,
	RCC_OTGFS,
	AHB_Res8,
	RCC_ETHMAC,
	RCC_ETHMACTX,
	RCC_ETHMACRX,
} RCCAHB_PinMap_t;

void MRCC_vInit(void);
void MRCC_vEnableClk(u8 A_u8BusID, u8 A_u8PeripheralID);
void MRCC_vDisableClk(u8 A_u8BusID, u8 A_u8PeripheralID);

#endif /* MCAL_RCC_RCC_INT_H_ */
