/*
 * LEDMAT_prg.c
 *
 *  Created on: Aug 25, 2026
 *      Author: LENOVO
 */
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "../../MCAL/GPIO/GPIO_int.h"
#include "LEDMAT_prv.h"
#include "LEDMAT_cfg.h"
#include "LEDMAT_int.h"
#include "../../MCAL/SYSTICK/SYSTICK_int.h"

static void HLEDMAT_vEnableCol(u8 A_u8Col);
static void HLEDMAT_vDisableAllCols();
static void HLEDMAT_vSetRow(u8 A_u8Row);

GPIOx_PinConfig_t *G_xRows = NULL;
GPIOx_PinConfig_t *G_xCols = NULL;
u8 G_u8Rows = 0;
u8 G_u8Cols = 0;

void HLEDMAT_vInit(GPIOx_PinConfig_t *A_xRows, GPIOx_PinConfig_t *A_xCols, u8 A_u8Rows, u8 A_u8Cols)
{
	G_xRows = A_xRows;
	G_xCols = A_xCols;
	G_u8Rows = A_u8Rows;
	G_u8Cols = A_u8Cols;

	for (u8 i = 0; i < A_u8Cols; i++)
		MGPIO_vPinInit(&A_xCols[i]);

	for (u8 i = 0; i < A_u8Rows; i++)
		MGPIO_vPinInit(&A_xRows[i]);

	MSYSTICK_CONFIG_t STK_cfg = {
			.InterruptEnable = ENABLE_TICKINT,
			.CLK_SRC = 0
	};

	MSYSTICK_vInit(&STK_cfg);
}

static void HLEDMAT_vEnableCol(u8 A_u8Col)
{
	MGPIO_vSetPinValue(G_xCols[A_u8Col].Port, G_xCols[A_u8Col].Pin, 0);
}

static void HLEDMAT_vDisableAllCols()
{
	for (u8 i = 0; i < G_u8Cols; i++)
		MGPIO_vSetPinValue(G_xCols[i].Port, G_xCols[i].Pin, 1);
}

static void HLEDMAT_vSetRow(u8 A_u8Row)
{
	for (u8 i = 0; i < G_u8Cols; i++)
		MGPIO_vSetPinValue(G_xRows[i].Port, G_xRows[i].Pin, GET_BIT(A_u8Row, i));
}

void HLEDMAT_vDispFrame(u8 A_u8Frame[], u32 A_u32FrameDelay)
{
	for (u8 j = 0; j < A_u32FrameDelay; j++)
	{
		for (u8 i = 0; i < G_u8Cols; i++)
		{
			HLEDMAT_vSetRow(A_u8Frame[i]);
			HLEDMAT_vEnableCol(i);
			MSYSTICK_vSetDelayUS(SCAN_TIME * 1000);
			HLEDMAT_vDisableAllCols();
		}
	}
}
