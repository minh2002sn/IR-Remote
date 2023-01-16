################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libs/Src/IR_Remote.c \
../Libs/Src/NEC_Protocol.c 

OBJS += \
./Libs/Src/IR_Remote.o \
./Libs/Src/NEC_Protocol.o 

C_DEPS += \
./Libs/Src/IR_Remote.d \
./Libs/Src/NEC_Protocol.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/Src/%.o Libs/Src/%.su: ../Libs/Src/%.c Libs/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/A_Minh/B_Big Projects/IR Remote/Code/IR_Remote_1.0/Libs/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Libs-2f-Src

clean-Libs-2f-Src:
	-$(RM) ./Libs/Src/IR_Remote.d ./Libs/Src/IR_Remote.o ./Libs/Src/IR_Remote.su ./Libs/Src/NEC_Protocol.d ./Libs/Src/NEC_Protocol.o ./Libs/Src/NEC_Protocol.su

.PHONY: clean-Libs-2f-Src

