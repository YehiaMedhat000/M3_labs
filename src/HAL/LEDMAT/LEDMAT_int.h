/*
 * LEDMAT_int.h
 *
 *  Created on: Aug 25, 2026
 *      Author: LENOVO
 */

#ifndef HAL_LEDMAT_LEDMAT_INT_H_
#define HAL_LEDMAT_LEDMAT_INT_H_

void HLEDMAT_vInit(GPIOx_PinConfig_t *A_xRows, GPIOx_PinConfig_t *A_xCols, u8 A_u8Rows, u8 A_u8Cols);
void HLEDMAT_vDispFrame(u8 A_u8Frame[], u32 A_u32FrameDelay);


#endif /* HAL_LEDMAT_LEDMAT_INT_H_ */
