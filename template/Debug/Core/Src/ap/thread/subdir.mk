################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/ap/thread/info.c \
../Core/Src/ap/thread/status.c \
../Core/Src/ap/thread/system.c 

OBJS += \
./Core/Src/ap/thread/info.o \
./Core/Src/ap/thread/status.o \
./Core/Src/ap/thread/system.o 

C_DEPS += \
./Core/Src/ap/thread/info.d \
./Core/Src/ap/thread/status.d \
./Core/Src/ap/thread/system.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/ap/thread/%.o: ../Core/Src/ap/thread/%.c Core/Src/ap/thread/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"D:/study/personal_project/template/template/Core/Inc/hw/driver/core" -I"D:/study/personal_project/template/template/Core/Inc/hw" -I"D:/study/personal_project/template/template/Core/Inc/hw/driver" -I"D:/study/personal_project/template/template/Core/Inc/ap/thread" -I"D:/study/personal_project/template/template/Core/Src/ap/thread" -I"D:/study/personal_project/template/template/Core/Inc/Lib" -I"D:/study/personal_project/template/template/Core/Src/Lib" -I"D:/study/personal_project/template/template/Core/Src/hw/driver/device" -I"D:/study/personal_project/template/template/Core/Inc/hw/driver/device" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

