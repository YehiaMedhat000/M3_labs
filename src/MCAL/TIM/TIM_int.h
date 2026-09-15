#ifndef MCAL_TIM_TIM_INT_H_
#define MCAL_TIM_TIM_INT_H_

#include "../../LIB/STD_TYPES.h"

#define MTIM_TIMER_2              2
#define MTIM_DEFAULT_INPUT_HZ     8000000UL
#define MTIM_DEFAULT_TICK_HZ      1000000UL

typedef struct
{
    u32 InputClockHz;
    u32 TickFrequencyHz;
} MTIM_Config_t;

void MTIM_vInit(const MTIM_Config_t *A_xConfig);
void MTIM_vStart(void);
void MTIM_vStop(void);
void MTIM_vReset(void);
u32 MTIM_u32GetCounter(void);
u32 MTIM_u32GetElapsed(u32 A_u32StartCounter);
void MTIM_vSetDelayUS(u32 A_u32DelayUS);
void MTIM_vSetDelayMS(u32 A_u32DelayMS);

#endif /* MCAL_TIM_TIM_INT_H_ */
