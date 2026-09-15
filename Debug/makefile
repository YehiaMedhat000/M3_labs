################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include system/src/stm32f1-stdperiph/subdir.mk
-include system/src/newlib/subdir.mk
-include system/src/diag/subdir.mk
-include system/src/cortexm/subdir.mk
-include system/src/cmsis/subdir.mk
-include src/MCAL/USART/subdir.mk
-include src/MCAL/TIM/subdir.mk
-include src/MCAL/SYSTICK/subdir.mk
-include src/MCAL/SPI/subdir.mk
-include src/MCAL/RTOS/subdir.mk
-include src/MCAL/RCC/subdir.mk
-include src/MCAL/NVIC/subdir.mk
-include src/MCAL/GPIO/subdir.mk
-include src/MCAL/FLASH/subdir.mk
-include src/MCAL/EXTI/subdir.mk
-include src/MCAL/DMA/subdir.mk
-include src/MCAL/AFIO/subdir.mk
-include src/HAL/TFT/subdir.mk
-include src/HAL/SSD/subdir.mk
-include src/HAL/S2P/subdir.mk
-include src/HAL/LEDMAT_S2P/subdir.mk
-include src/HAL/LEDMAT/subdir.mk
-include src/HAL/IR/subdir.mk
-include src/HAL/DAC/subdir.mk
-include src/APP/TICTACTOE/subdir.mk
-include src/APP/SNAKE/subdir.mk
-include src/APP/HexParser/subdir.mk
-include src/APP/GAME/subdir.mk
-include src/APP/CONNECT4/subdir.mk
-include src/APP/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
M3_labs.hex \

SECONDARY_SIZE += \
M3_labs.siz \


# All Target
all: M3_labs.elf secondary-outputs

# Tool invocations
M3_labs.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM GNU C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -T mem.ld -T libs.ld -T sections.ld -nostartfiles -Xlinker --gc-sections -L"../ldscripts" -Wl,-Map,"M3_labs.map" --specs=nano.specs -o "M3_labs.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

M3_labs.hex: M3_labs.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "M3_labs.elf"  "M3_labs.hex"
	@echo 'Finished building: $@'
	@echo ' '

M3_labs.siz: M3_labs.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "M3_labs.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(CPP_DEPS) M3_labs.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
