#include "../LIB/STD_TYPES.h"
#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/AFIO/AFIO_int.h"
#include "../MCAL/EXTI/EXTI_int.h"
#include "../MCAL/NVIC/NVIC_int.h"
#include "../HAL/TFT/TFT_int.h"
#include "../HAL/IR/IR_int.h"
#include "GAME/GAME_int.h"

static void IR_vCommandReceived(u8 A_u8Address, u8 A_u8Command)
{
    /*
     * Accept commands only from the configured NEC remote address.
     * The measured command values are handled by GAME_vHandleIRCommand().
     */
    if (A_u8Address == HIR_NEC_REMOTE_ADDRESS)
    {
        GAME_vHandleIRCommand(A_u8Command);
    }
}

int main(void)
{
    GPIOx_PinConfig_t L_xMOSI = {
        .Port = GPIO_PORTA,
        .Pin = GPIO_PIN7,
        .Mode = GPIO_MODE_ALF,
        .Speed = GPIO_SPEED_LOW,
        .OutputType = GPIO_OT_PUSHPULL,
        .PullType = GPIO_NO_PULL
    };

    GPIOx_PinConfig_t L_xSCK = {
        .Port = GPIO_PORTA,
        .Pin = GPIO_PIN5,
        .Mode = GPIO_MODE_ALF,
        .Speed = GPIO_SPEED_LOW,
        .OutputType = GPIO_OT_PUSHPULL,
        .PullType = GPIO_NO_PULL
    };

    GPIOx_PinConfig_t L_xIRPin = {
        .Port = GPIO_PORTA,
        .Pin = GPIO_PIN0,
        .Mode = GPIO_MODE_INPUT,
        .PullType = GPIO_PULL_UP
    };

    HIR_Config_t L_xIRConfig = {
        .ExtiLine = L0,
        .ExtiPort = AFIO_PORT_A,
        .GpioPort = GPIO_PORTA,
        .GpioPin = GPIO_PIN0,
        .NvicPosition = NVIC_EXTI0,
        .TicksPerUS = 1,
        .CommandCallback = IR_vCommandReceived
    };

    /*
     * Initialize the system clock and required peripheral clocks.
     */
    MRCC_vInit();
    MRCC_vEnableClk(RCC_APB2, RCC_GPIOA);
    MRCC_vEnableClk(RCC_APB2, RCC_GPIOB);
    MRCC_vEnableClk(RCC_APB2, RCC_AFIO);
    MRCC_vEnableClk(RCC_APB2, RCC_SPI1);

    /*
     * PA0 receives the demodulated IR signal.
     * PA5 and PA7 are the TFT SPI clock and MOSI pins.
     */
    MGPIO_vPinInit(&L_xIRPin);
    MGPIO_vPinInit(&L_xMOSI);
    MGPIO_vPinInit(&L_xSCK);

    /*
     * Initialize the TFT before the menu is rendered.
     */
    HTFT_vInit();

    /*
     * Initialize the NEC receiver.
     *
     * At an 8 MHz system clock with HCLK / 8 SysTick:
     * 8 MHz / 8 = 1 MHz, so one timer tick equals one microsecond.
     */
    HIR_vInit(&L_xIRConfig);

    /*
     * GAME_vTaskHandler() renders the menu and enters Connect 4
     * when the EQ/select command is received.
     */
    while (1)
    {
        GAME_vTaskHandler();
    }

    return 0;
}
