#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "SYSTICK_prv.h"
#include "SYSTICK_int.h"

static void (*G_FPTR)(void) = NULL;
static u8 G_u8SingleFlag = 0;

void MSYSTICK_vInit(MSYSTICK_CONFIG_t *A_xCfg)
{
	// Stop the timer and reset it
	CLR_BIT(SYSTICK->CTRL, CTRL_ENABLE);

	if (A_xCfg->CLK_SRC)
		SET_BIT(SYSTICK->CTRL, CTRL_CLKSOURCE);
	else
		CLR_BIT(SYSTICK->CTRL, CTRL_CLKSOURCE);

	if (A_xCfg->InterruptEnable)
		SET_BIT(SYSTICK->CTRL, CTRL_TICKINT);
	else
		CLR_BIT(SYSTICK->CTRL, CTRL_TICKINT);
}

void MSYSTICK_vStartTimer(u32 A_u32LoadVal)
{
	if ((A_u32LoadVal > 0x00FFFFFF) | (A_u32LoadVal > 0x00000000))
		SYSTICK->LOAD = A_u32LoadVal;

	SYSTICK->VAL = 0;
	SET_BIT(SYSTICK->CTRL, CTRL_ENABLE);
}

void MSYSTICK_vStopTimer(void)
{
	CLR_BIT(SYSTICK->CTRL, CTRL_ENABLE);
	SYSTICK->VAL = 0;
}

u32 MSYSTICK_u32GetElapsedTime(void)
{
	return (SYSTICK->LOAD - SYSTICK->VAL);
}

u32 MSYSTICK_u32GetRemainingTime(void)
{
	return SYSTICK->VAL;
}

void MSYSTICK_vSetDelayMS(f64 A_f64Delay)
{
	u32 L_u32Ticks = (u32)(1000 * A_f64Delay);
	SYSTICK->VAL = 0;

	if ((L_u32Ticks < 0x00FFFFFF) | (L_u32Ticks > 0x00000000))
		MSYSTICK_vStartTimer(L_u32Ticks);

	while (!GET_BIT(SYSTICK->CTRL, CTRL_COUNTFLAG)){}
	MSYSTICK_vStopTimer();
}

void MSYSTICK_vSetDelayUS(f64 A_f64Delay)
{
	u32 L_u32Ticks = (u32)A_f64Delay;
	SYSTICK->VAL = 0;

	if ((L_u32Ticks < 0x00FFFFFF) | (L_u32Ticks > 0x00000000))
		MSYSTICK_vStartTimer(L_u32Ticks);

	while (!GET_BIT(SYSTICK->CTRL, CTRL_COUNTFLAG)){}
	MSYSTICK_vStopTimer();
}

void MSYSTICK_vSetIntervalSingleMS(u32 A_u32Delay_ms, void (*A_FPTR)(void))
{
	G_FPTR = A_FPTR;
	G_u8SingleFlag = 1;
	u32 L_u32Ticks = (u32)(1000 * A_u32Delay_ms);

	SYSTICK->VAL = 0;

	if ((L_u32Ticks < 0x00FFFFFF) | (L_u32Ticks > 0x00000000))
		MSYSTICK_vStartTimer(L_u32Ticks);
}

void MSYSTICK_vSetIntervalSingleUS(u32 A_u32Delay_us, void (*A_FPTR)(void))
{
	G_FPTR = A_FPTR;
	G_u8SingleFlag = 1;
	u32 L_u32Ticks = A_u32Delay_us;

	SYSTICK->VAL = 0;

	if ((L_u32Ticks < 0x00FFFFFF) | (L_u32Ticks > 0x00000000))
		MSYSTICK_vStartTimer(L_u32Ticks);
}

void MSYSTICK_vSetIntervalMultiMS(u32 A_u32Delay_ms, void (*A_FPTR)(void))
{
	G_u8SingleFlag = 0;
	G_FPTR = A_FPTR;
	u32 L_u32Ticks = (u32)(1000 * A_u32Delay_ms);

	SYSTICK->VAL = 0;

	if ((L_u32Ticks < 0x00FFFFFF) | (L_u32Ticks > 0x00000000))
		MSYSTICK_vStartTimer(L_u32Ticks - 1);
}

void MSYSTICK_vSetIntervalMultiUS(u32 A_u32Delay_us, void (*A_FPTR)(void))
{
	G_u8SingleFlag = 0;
	G_FPTR = A_FPTR;
	u32 L_u32Ticks = (u32)(A_u32Delay_us);

	SYSTICK->VAL = 0;

	if ((L_u32Ticks < 0x00FFFFFF) | (L_u32Ticks > 0x00000000))
		MSYSTICK_vStartTimer(L_u32Ticks - 1);
}

void SysTick_Handler (void)
{
	if(G_FPTR !=NULL)
	{
		G_FPTR();

		if(G_u8SingleFlag == 1)
			MSYSTICK_vStopTimer();
	}
}
