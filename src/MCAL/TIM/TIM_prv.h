#ifndef MCAL_TIM_TIM_PRV_H_
#define MCAL_TIM_TIM_PRV_H_

#define TIM2_BASE_ADDR 0x40000000U
#define DBGMCU_APB1_FZ_ADDR 0xE0042008U
#define TIM_COUNTER_MAX 0x0000FFFFUL

typedef struct
{
    u32 CR1;
    u32 CR2;
    u32 SMCR;
    u32 DIER;
    u32 SR;
    u32 EGR;
    u32 CCMR1;
    u32 CCMR2;
    u32 CCER;
    u32 CNT;
    u32 PSC;
    u32 ARR;
    u32 RESERVED1;
    u32 CCR1;
    u32 CCR2;
    u32 CCR3;
    u32 CCR4;
    u32 RESERVED2;
    u32 DCR;
    u32 DMAR;
} TIM_MemMap_t;

#define TIM2 ((volatile TIM_MemMap_t *)TIM2_BASE_ADDR)
#define DBGMCU_APB1_FZ (*(volatile u32 *)DBGMCU_APB1_FZ_ADDR)

#define TIM_CR1_CEN  0
#define TIM_CR1_URS  2
#define TIM_EGR_UG   0
#define DBGMCU_APB1_FZ_DBG_TIM2_STOP 0

#endif /* MCAL_TIM_TIM_PRV_H_ */
