################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hw/Flash/L4Flash.c 

C_DEPS += \
./hw/Flash/L4Flash.d 

OBJS += \
./hw/Flash/L4Flash.o 


# Each subdirectory must supply rules for building sources it contributes
hw/Flash/%.o: ../hw/Flash/%.c hw/Flash/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"D:/CODE ARM/NUCLEO-STM32L476RG/FOTA_Project/Bootloader_FW/hw" -I"D:/CODE ARM/NUCLEO-STM32L476RG/FOTA_Project/Bootloader_FW/hw/Sim7x00" -I"D:/CODE ARM/NUCLEO-STM32L476RG/FOTA_Project/Bootloader_FW/hw/Terminal" -I"D:/CODE ARM/NUCLEO-STM32L476RG/FOTA_Project/Bootloader_FW/hw/Flash" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

