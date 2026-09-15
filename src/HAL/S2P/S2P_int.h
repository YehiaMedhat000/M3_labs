/*
 * SHIFT_REGISTER_int.h
 *
 *  Created on: Aug 30, 2026
 *      Author: LENOVO
 */

#ifndef HAL_S2P_S2P_INT_H_
#define HAL_S2P_S2P_INT_H_

typedef struct {
	u8 Port;
	u8 DataPin;
	u8 ShiftCLKPin;
	u8 LatchCLKPin;
	u8 ResetPin;
} S2P_PinConfig_t;

void HS2P_vInit(S2P_PinConfig_t *A_xPinConfig);
void HS2P_vSendData(S2P_PinConfig_t *A_xPinConfig, u16 A_u32Data);
void HS2P_vReset(S2P_PinConfig_t *A_xPinConfig);

#endif /* HAL_S2P_S2P_INT_H_ */
