#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "../../MCAL/AFIO/AFIO_int.h"
#include "../../MCAL/EXTI/EXTI_int.h"
#include "../../MCAL/GPIO/GPIO_int.h"
#include "../../MCAL/NVIC/NVIC_int.h"
#include "../../MCAL/SYSTICK/SYSTICK_int.h"
#include "IR_int.h"
#include "IR_prv.h"

static HIR_Config_t G_xConfig;
static HIR_State_t G_xState = HIR_WAIT_LEADER_START;
static u32 G_u32LastTimestamp;
static u8 G_u8Frame[HIR_NEC_FRAME_BYTES];
static u8 G_u8BitCount;

static u32 HIR_u32ToMicroseconds(u32 A_u32Ticks)
{
	if (G_xConfig.TicksPerUS == 0)
		return 0;

	return (A_u32Ticks + (G_xConfig.TicksPerUS / 2)) /
		   G_xConfig.TicksPerUS;
}

static u8 HIR_u8InRange(u32 A_u32Value, u32 A_u32Min, u32 A_u32Max)
{
	return (A_u32Value >= A_u32Min && A_u32Value <= A_u32Max);
}

u32 HIR_u32MeasurePulse(void)
{
	u32 L_u32Now = MSYSTICK_u32GetElapsedTime();
	u32 L_u32Delta;

	if (L_u32Now >= G_u32LastTimestamp)
		L_u32Delta = L_u32Now - G_u32LastTimestamp;
	else
		L_u32Delta = (HIR_TIMER_PERIOD_TICKS - G_u32LastTimestamp) + L_u32Now;

	G_u32LastTimestamp = L_u32Now;
	return HIR_u32ToMicroseconds(L_u32Delta);
}

u8 HIR_u8DecodeFrame(const u8 *A_pu8Frame, u8 *A_pu8Address, u8 *A_pu8Command)
{
	if (A_pu8Frame == NULL || A_pu8Address == NULL || A_pu8Command == NULL)
		return 0;

	if ((u8)(A_pu8Frame[0] ^ A_pu8Frame[1]) != 0xFF ||
		(u8)(A_pu8Frame[2] ^ A_pu8Frame[3]) != 0xFF)
		return 0;

	*A_pu8Address = A_pu8Frame[0];
	*A_pu8Command = A_pu8Frame[2];
	return 1;
}

static void HIR_vResetFrame(void)
{
	G_xState = HIR_WAIT_LEADER_START;
	G_u8BitCount = 0;
	G_u8Frame[0] = 0;
	G_u8Frame[1] = 0;
	G_u8Frame[2] = 0;
	G_u8Frame[3] = 0;
}

static void HIR_vOnEdge(void)
{
	u32 L_u32Pulse;
	u8 L_u8Address;
	u8 L_u8Command;

	/*
	 * With falling-edge EXTI, intervals between falling edges are used:
	 * approximately 13.5 ms for the leader, 1.12 ms for zero, and
	 * 2.25 ms for one.
	 */
	if (G_xState == HIR_WAIT_LEADER_START)
	{
		G_u32LastTimestamp = MSYSTICK_u32GetElapsedTime();
		G_xState = HIR_WAIT_LEADER_INTERVAL;
		return;
	}

	L_u32Pulse = HIR_u32MeasurePulse();

	switch (G_xState)
	{
		case HIR_WAIT_LEADER_INTERVAL:
			if (!HIR_u8InRange(L_u32Pulse, HIR_LEADER_INTERVAL_MIN_US,
							  HIR_LEADER_INTERVAL_MAX_US))
			{
				HIR_vResetFrame();
			}
			else
			{
				G_xState = HIR_RECEIVING_DATA;
			}
			break;

		case HIR_RECEIVING_DATA:
			if (HIR_u8InRange(L_u32Pulse, HIR_ZERO_INTERVAL_MIN_US,
							 HIR_ZERO_INTERVAL_MAX_US))
			{
				CLR_BIT(G_u8Frame[G_u8BitCount / 8], G_u8BitCount % 8);
			}
			else if (HIR_u8InRange(L_u32Pulse, HIR_ONE_INTERVAL_MIN_US,
								  HIR_ONE_INTERVAL_MAX_US))
			{
				SET_BIT(G_u8Frame[G_u8BitCount / 8], G_u8BitCount % 8);
			}
			else
			{
				HIR_vResetFrame();
				break;
			}

			G_u8BitCount++;
			if (G_u8BitCount == HIR_NEC_FRAME_BITS)
			{
				if (HIR_u8DecodeFrame(G_u8Frame, &L_u8Address, &L_u8Command) &&
					G_xConfig.CommandCallback != NULL)
				{
					G_xConfig.CommandCallback(L_u8Address, L_u8Command);
				}
				HIR_vResetFrame();
			}
			else
			{
				G_xState = HIR_RECEIVING_DATA;
			}
			break;

		default:
			HIR_vResetFrame();
			break;
	}
}

void HIR_vReset(void)
{
	G_u32LastTimestamp = MSYSTICK_u32GetElapsedTime();
	HIR_vResetFrame();
}

void HIR_vResume(void)
{
	MSYSTICK_vStartTimer(HIR_TIMER_PERIOD_TICKS - 1);
	HIR_vReset();
}

void HIR_vInit(const HIR_Config_t *A_xConfig)
{
	if (A_xConfig == NULL)
		return;

	G_xConfig = *A_xConfig;
	if (G_xConfig.TicksPerUS == 0)
		G_xConfig.TicksPerUS = HIR_DEFAULT_TICKS_PER_US;

	MSYSTICK_CONFIG_t L_xTimer = {DISABLE_TICKINT, STK_AHB_8};
	MSYSTICK_vInit(&L_xTimer);
	MSYSTICK_vStartTimer(HIR_TIMER_PERIOD_TICKS - 1);
	HIR_vReset();

	MEXTI_vInit();
	MAFIO_vSetLinePort(G_xConfig.ExtiLine, G_xConfig.ExtiPort);
	MEXTI_vSetTrigger(G_xConfig.ExtiLine, EXTI_Falling);
	MEXTI_vSetCallBack(HIR_vOnEdge, G_xConfig.ExtiLine);
	MEXTI_vEnableINT(G_xConfig.ExtiLine);
	MNVIC_vEnablePeripheralINT(G_xConfig.NvicPosition);
}
