#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "../RCC/RCC_int.h"
#include "TIM_prv.h"
#include "TIM_int.h"

void MTIM_vInit(const MTIM_Config_t *A_xConfig)
{
    u32 L_u32Prescaler;

    MRCC_vEnableClk(RCC_APB1, RCC_TIM2);
    SET_BIT(DBGMCU_APB1_FZ, DBGMCU_APB1_FZ_DBG_TIM2_STOP);

    L_u32Prescaler = (A_xConfig->InputClockHz /
                      A_xConfig->TickFrequencyHz) - 1;

    TIM2->CR1 = 0;
    TIM2->PSC = L_u32Prescaler;
    TIM2->ARR = TIM_COUNTER_MAX;
    TIM2->CNT = 0;
    TIM2->EGR = (1UL << TIM_EGR_UG);
    TIM2->SR = 0;
}

void MTIM_vStart(void)
{
    SET_BIT(TIM2->CR1, TIM_CR1_CEN);
}

void MTIM_vStop(void)
{
    CLR_BIT(TIM2->CR1, TIM_CR1_CEN);
}

void MTIM_vReset(void)
{
    TIM2->CNT = 0;
    TIM2->SR = 0;
}

u32 MTIM_u32GetCounter(void)
{
    return TIM2->CNT;
}

u32 MTIM_u32GetElapsed(u32 A_u32StartCounter)
{
    u32 L_u32CurrentCounter = MTIM_u32GetCounter();

    if (L_u32CurrentCounter >= A_u32StartCounter)
        return L_u32CurrentCounter - A_u32StartCounter;

    return (TIM_COUNTER_MAX - A_u32StartCounter + 1) +
           L_u32CurrentCounter;
}

void MTIM_vSetDelayUS(u32 A_u32DelayUS)
{
    u32 L_u32StartCounter = MTIM_u32GetCounter();

    while (MTIM_u32GetElapsed(L_u32StartCounter) < A_u32DelayUS)
    {
    }
}

void MTIM_vSetDelayMS(u32 A_u32DelayMS)
{
    while (A_u32DelayMS > 0)
    {
        MTIM_vSetDelayUS(1000);
        A_u32DelayMS--;
    }
}
