#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "../../MCAL/GPIO/GPIO_int.h"
#include "../../MCAL/SYSTICK/SYSTICK_int.h"
#include "S2P_int.h"
#include "S2P_cfg.h"


void HS2P_vInit(S2P_PinConfig_t *A_xPinConfig)
{
	GPIOx_PinConfig_t data = {
			.Port = A_xPinConfig->Port,
			.Pin = A_xPinConfig->DataPin,
			.OutputType = GPIO_OT_PUSHPULL, .Speed = GPIO_SPEED_LOW,
			.PullType = GPIO_NO_PULL, .Mode = GPIO_MODE_OUTPUT
	};

	GPIOx_PinConfig_t shift = {
			.Port = A_xPinConfig->Port,
			.Pin = A_xPinConfig->ShiftCLKPin,
			.OutputType = GPIO_OT_PUSHPULL, .Speed = GPIO_SPEED_LOW,
			.PullType = GPIO_NO_PULL, .Mode = GPIO_MODE_OUTPUT
	};

	GPIOx_PinConfig_t latch = {
			.Port = A_xPinConfig->Port,
			.Pin = A_xPinConfig->LatchCLKPin,
			.OutputType = GPIO_OT_PUSHPULL, .Speed = GPIO_SPEED_LOW,
			.PullType = GPIO_NO_PULL, .Mode = GPIO_MODE_OUTPUT
	};

	GPIOx_PinConfig_t reset = {
			.Port = A_xPinConfig->Port,
			.Pin = A_xPinConfig->ResetPin,
			.OutputType = GPIO_OT_PUSHPULL, .Speed = GPIO_SPEED_LOW,
			.PullType = GPIO_NO_PULL, .Mode = GPIO_MODE_OUTPUT
	};

	MSYSTICK_CONFIG_t timer = {ENABLE_TICKINT, 0};
	MSYSTICK_vInit(&timer);

	MGPIO_vPinInit(&data);
	MGPIO_vPinInit(&shift);
	MGPIO_vPinInit(&latch);
	MGPIO_vPinInit(&reset);

	// Make sure the Reset pin doesn't start at 0 (Not to reset by mistake)
	MGPIO_vSetPinValue(A_xPinConfig->Port, A_xPinConfig->ShiftCLKPin, 0);
	MGPIO_vSetPinValue(A_xPinConfig->Port, A_xPinConfig->LatchCLKPin, 0);
	MGPIO_vSetPinValue(A_xPinConfig->Port, A_xPinConfig->ResetPin, 1);
}

static void HS2P_vShiftData(S2P_PinConfig_t *A_xPinConfig)
{
	MGPIO_vSetPinValue(A_xPinConfig->Port, A_xPinConfig->ShiftCLKPin, 1);
	MGPIO_vSetPinValue(A_xPinConfig->Port, A_xPinConfig->ShiftCLKPin, 0);
}

static void HS2P_vLatchData(S2P_PinConfig_t *A_xPinConfig)
{
	MGPIO_vSetPinValue(A_xPinConfig->Port, A_xPinConfig->LatchCLKPin, 1);
	MGPIO_vSetPinValue(A_xPinConfig->Port, A_xPinConfig->LatchCLKPin, 0);
}

void HS2P_vSendData(S2P_PinConfig_t *A_xPinConfig, u16 A_u32Data)
{
	for (u8 i = 0; i < (8 * S2P_NO); i++)
	{
		MGPIO_vSetPinValue(A_xPinConfig->Port, A_xPinConfig->DataPin, GET_BIT(A_u32Data, (8 * S2P_NO) - i - 1));
		HS2P_vShiftData(A_xPinConfig);
	}
	HS2P_vLatchData(A_xPinConfig);
}

void HS2P_vReset(S2P_PinConfig_t *A_xPinConfig)
{
	// Reset Shift Registers By driving the MR pin low
	MGPIO_vSetPinValue(A_xPinConfig->Port, A_xPinConfig->ResetPin, 0);

	// Pulse Latch clock A Falling edge to load those zeros and reset
	HS2P_vLatchData(A_xPinConfig);

	// Getting the MR pin back high not to be able to write to the registers again
	MGPIO_vSetPinValue(A_xPinConfig->Port, A_xPinConfig->ResetPin, 1);
}
