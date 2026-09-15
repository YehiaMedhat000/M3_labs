#ifndef HAL_IR_IR_INT_H_
#define HAL_IR_IR_INT_H_

#include "../../LIB/STD_TYPES.h"

#define HIR_NEC_FRAME_BYTES       4
#define HIR_NEC_FRAME_BITS        32
#define HIR_DEFAULT_TICKS_PER_US  1
#define HIR_NEC_REMOTE_ADDRESS    0x00

/* NEC command bytes for the supplied remote control. */
#define HIR_CMD_POWER              0x45
#define HIR_CMD_VOLUME_UP          0x46
#define HIR_CMD_MODE               0x46
#define HIR_CMD_FUNCTION_STOP      0x47
#define HIR_CMD_REWIND             0x40
#define HIR_CMD_PLAY_PAUSE         0x40
#define HIR_CMD_FAST_FORWARD       0x43
#define HIR_CMD_DOWN               0x07
#define HIR_CMD_VOLUME_DOWN        0x15
#define HIR_CMD_UP                 0x09
#define HIR_CMD_0                  0x16
#define HIR_CMD_EQ                 0x07
#define HIR_CMD_ST_REPT            0x0D
#define HIR_CMD_1                  0x0C
#define HIR_CMD_2                  0x18
#define HIR_CMD_3                  0x5E
#define HIR_CMD_4                  0x08
#define HIR_CMD_5                  0x1C
#define HIR_CMD_6                  0x5A
#define HIR_CMD_7                  0x42
#define HIR_CMD_8                  0x52
#define HIR_CMD_9                  0x4A

typedef void (*HIR_CommandCallback_t)(u8 A_u8Address, u8 A_u8Command);

typedef struct
{
	u8 ExtiLine;
	u8 ExtiPort;
	u8 GpioPort;
	u8 GpioPin;
	u8 NvicPosition;
	u32 TicksPerUS;
	HIR_CommandCallback_t CommandCallback;
} HIR_Config_t;

void HIR_vInit(const HIR_Config_t *A_xConfig);
void HIR_vReset(void);
void HIR_vResume(void);

/*
 * Measures the time since the previous IR edge. The returned value is in us.
 * This function is also useful when testing the timing layer independently.
 */
u32 HIR_u32MeasurePulse(void);
u32 HIR_u32GetLastPulse(void);

/*
 * Validates and decodes a complete NEC frame stored LSB-first in four bytes.
 * Returns 1 for a valid frame and writes the address and command values.
 */
u8 HIR_u8DecodeFrame(const u8 *A_pu8Frame, u8 *A_pu8Address, u8 *A_pu8Command);

#endif /* HAL_IR_IR_INT_H_ */
