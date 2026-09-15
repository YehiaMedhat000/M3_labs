/*
 * EXTI_int.h
 *
 *  Created on: Aug 23, 2026
 *      Author: LENOVO
 */

#ifndef MCAL_EXTI_EXTI_INT_H_
#define MCAL_EXTI_EXTI_INT_H_

void MEXTI_vInit();
void MEXTI_vEnableINT(u8 A_u8LineNo);
void MEXTI_vDisableINT(u8 A_u8LineNo);
void MEXTI_vSetTrigger(u8 A_u8LineNo, u8 A_u8Trigger);
void MEXTI_vSoftwareINT(u8 A_u8LineNo);
void MEXTI_vSetCallBack(void (*FPTR)(void), u8 A_u8LineNo);

typedef enum EXTI_Lines
{
	L0 = 0, L1, L2, L3, L4,
	L5, L6, L7, L8, L9,
	L10, L11, L12, L13, L14, L16
} EXTI_Line_t;

#define EXTI_Rising   0
#define EXTI_Falling  1
#define EXTI_OnChange 2

#endif /* MCAL_EXTI_EXTI_INT_H_ */
