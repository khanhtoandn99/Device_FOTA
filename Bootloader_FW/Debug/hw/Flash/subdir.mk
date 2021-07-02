################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hw/Flash/Flash.c \
../hw/Flash/FlashL4.c 

C_DEPS += \
./hw/Flash/Flash.d \
./hw/Flash/FlashL4.d 

OBJS += \
./hw/Flash/Flash.o \
./hw/Flash/FlashL4.o 


# Each subdirectory must supply rules for building sources it contributes
hw/Flash/Flash.o: ../hw/Flash/Flash.c hw/Flash/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"D:/CODE ARM/NUCLEO-STM32L476RG/FOTA_Project/Bootloader_FW/hw" -I"D:/CODE ARM/NUCLEO-STM32L476RG/FOTA_Project/Bootloader_FW/hw/Flash" -I"D:/CODE ARM/NUCLEO-STM32L476RG/FOTA_Project/Bootloader_FW/hw/Sim7x00" -I"D:/CODE ARM/NUCLEO-STM32L476RG/FOTA_Project/Bootloader_FW/hw/Terminal" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"hw/Flash/Flash.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
hw/Flash/FlashL4.o: ../hw/Flash/FlashL4.c hw/Flash/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"D:/CODE ARM/NUCLEO-STM32L476RG/FOTA_Project/Bootloader_FW/hw" -I"D:/CODE ARM/NUCLEO-STM32L476RG/FOTA_Project/Bootloader_FW/hw/Flash" -I"D:/CODE ARM/NUCLEO-STM32L476RG/FOTA_Project/Bootloader_FW/hw/Sim7x00" -I"D:/CODE ARM/NUCLEO-STM32L476RG/FOTA_Project/Bootloader_FW/hw/Terminal" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"hw/Flash/FlashL4.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

