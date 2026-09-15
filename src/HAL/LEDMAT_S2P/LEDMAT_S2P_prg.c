/*
 * LEDMAT_S2P_prg.c
 *
 *  Created on: Sep 5, 2026
 *      Author: LENOVO
 */
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "../../MCAL/SYSTICK/SYSTICK_int.h"
#include "../S2P/S2P_int.h"
#include "LEDMAT_S2P_cfg.h"
#include "LEDMAT_S2P_int.h"

void HLEDMAT_S2P_vDispFrame(S2P_PinConfig_t *A_xS2P, u8 A_u8Frame[])
{
	u16 Data = 0;
	MSYSTICK_CONFIG_t timer = {ENABLE_TICKINT, 0};

	MSYSTICK_vInit(&timer);

	for (u8 i = 0; i < 8; i++)
	{
		Data = 0; // Reset all rows
		Data |= A_u8Frame[i]; // Set Rows
		Data |= 0xFF00; // Clear all columns
		CLR_BIT(Data, i+8); // Set the column

		HS2P_vSendData(A_xS2P, Data);
		MSYSTICK_vSetDelayUS(SCAN_TIME * 1000);
	}
}
