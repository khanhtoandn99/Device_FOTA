################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../hw/Terminal/Terminal.cpp 

OBJS += \
./hw/Terminal/Terminal.o 

CPP_DEPS += \
./hw/Terminal/Terminal.d 


# Each subdirectory must supply rules for building sources it contributes
hw/Terminal/Terminal.o: ../hw/Terminal/Terminal.cpp hw/Terminal/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"D:/CODE ARM/NUCLEO-STM32L476RG/FOTA_Project/FOTA_FW/hw/Flash" -I"D:/CODE ARM/NUCLEO-STM32L476RG/FOTA_Project/FOTA_FW/hw/Sim7x00" -I"D:/CODE ARM/NUCLEO-STM32L476RG/FOTA_Project/FOTA_FW/hw/Terminal" -I"D:/CODE ARM/NUCLEO-STM32L476RG/FOTA_Project/FOTA_FW/hw" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"hw/Terminal/Terminal.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
