#ifndef MCAL_SYSTICK_SYSTICK_INT_H_
#define MCAL_SYSTICK_SYSTICK_INT_H_

typedef struct {
	u8 InterruptEnable;
	u8 CLK_SRC;
} MSYSTICK_CONFIG_t;

void MSYSTICK_vInit(MSYSTICK_CONFIG_t *A_xCfg);
void MSYSTICK_vStartTimer(u32 A_u32LoadVal);
void MSYSTICK_vStopTimer(void);

u32 MSYSTICK_u32GetElapsedTime(void);
u32 MSYSTICK_u32GetRemainingTime(void);

void MSYSTICK_vSetDelayMS(f64 A_f64Delay);
void MSYSTICK_vSetDelayUS(f64 A_f64Delay);

void MSYSTICK_vSetIntervalSingleMS(u32 A_u32Delay_ms, void (*A_FPTR)(void));
void MSYSTICK_vSetIntervalSingleUS(u32 A_u32Delay_us, void (*A_FPTR)(void));

void MSYSTICK_vSetIntervalMultiMS(u32 A_u32Delay_ms, void (*A_FPTR)(void));
void MSYSTICK_vSetIntervalMultiUS(u32 A_u32Delay_us, void (*A_FPTR)(void));

#define ENABLE_TICKINT	1
#define DISABLE_TICKINT 0
#define STK_AHB_8 		0
#define STK_PC 			0

#endif /* MCAL_SYSTICK_SYSTICK_INT_H_ */
