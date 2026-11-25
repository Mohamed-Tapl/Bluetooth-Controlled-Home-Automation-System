################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include RTOS/subdir.mk
-include Mcal/UART/subdir.mk
-include Mcal/Timer1/subdir.mk
-include Mcal/Timer0/subdir.mk
-include Mcal/TWI/subdir.mk
-include Mcal/SPI/subdir.mk
-include Mcal/GIE/subdir.mk
-include Mcal/EXTI/subdir.mk
-include Mcal/DIO/subdir.mk
-include Mcal/ADC/subdir.mk
-include Hal/TempSensor/subdir.mk
-include Hal/SevSeg/subdir.mk
-include Hal/Relay/subdir.mk
-include Hal/Led/subdir.mk
-include Hal/LM35/subdir.mk
-include Hal/LCD/subdir.mk
-include Hal/KPD/subdir.mk
-include Hal/EEPROM/subdir.mk
-include Hal/DcMotor/subdir.mk
-include Hal/Buzzer/subdir.mk
-include Hal/Button/subdir.mk
-include App/subdir.mk
-include .metadata/.plugins/org.eclipse.cdt.make.core/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Bluetooth-Controlled-Home-Automation-System
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
Bluetooth-Controlled-Home-Automation-System.lss \

FLASH_IMAGE += \
Bluetooth-Controlled-Home-Automation-System.hex \

SIZEDUMMY += \
sizedummy \


# All Target
all: main-build

# Main-build Target
main-build: Bluetooth-Controlled-Home-Automation-System.elf secondary-outputs

# Tool invocations
Bluetooth-Controlled-Home-Automation-System.elf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,Bluetooth-Controlled-Home-Automation-System.map -mmcu=atmega32 -o "Bluetooth-Controlled-Home-Automation-System.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Bluetooth-Controlled-Home-Automation-System.lss: Bluetooth-Controlled-Home-Automation-System.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S Bluetooth-Controlled-Home-Automation-System.elf  >"Bluetooth-Controlled-Home-Automation-System.lss"
	@echo 'Finished building: $@'
	@echo ' '

Bluetooth-Controlled-Home-Automation-System.hex: Bluetooth-Controlled-Home-Automation-System.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex Bluetooth-Controlled-Home-Automation-System.elf  "Bluetooth-Controlled-Home-Automation-System.hex"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: Bluetooth-Controlled-Home-Automation-System.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 Bluetooth-Controlled-Home-Automation-System.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) Bluetooth-Controlled-Home-Automation-System.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents main-build

-include ../makefile.targets
