################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Users/dht11.cpp \
../Users/mpu9250.cpp 

OBJS += \
./Users/dht11.o \
./Users/mpu9250.o 

CPP_DEPS += \
./Users/dht11.d \
./Users/mpu9250.d 


# Each subdirectory must supply rules for building sources it contributes
Users/%.o Users/%.su Users/%.cyclo: ../Users/%.cpp Users/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/ASUS/STM32CubeIDE/workspace_1.16.0/encocam_workspace/quickproject/Users" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Users

clean-Users:
	-$(RM) ./Users/dht11.cyclo ./Users/dht11.d ./Users/dht11.o ./Users/dht11.su ./Users/mpu9250.cyclo ./Users/mpu9250.d ./Users/mpu9250.o ./Users/mpu9250.su

.PHONY: clean-Users

