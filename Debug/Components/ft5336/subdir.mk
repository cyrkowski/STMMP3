################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Components/ft5336/ft5336.c 

OBJS += \
./Components/ft5336/ft5336.o 

C_DEPS += \
./Components/ft5336/ft5336.d 


# Each subdirectory must supply rules for building sources it contributes
Components/ft5336/ft5336.o: ../Components/ft5336/ft5336.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F746xx -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../TouchGFX/gui/include -I"C:/Users/Michal/Documents/stm/f746g-disco-test/f746g-disco-test/Middlewares/ST/TouchGFX/touchgfx/framework/include/platform/hal/simulator/sdl2/vendor" -I../TouchGFX/generated/fonts/include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TouchGFX/generated/texts/include -I../Src -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../TouchGFX/generated/images/include -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I"C:/Users/Michal/Documents/stm/f746g-disco-test/f746g-disco-test/Components/ft5336" -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -I../Inc -I../Drivers/CMSIS/Include -I../Middlewares/ST/TouchGFX/touchgfx/framework/include -I../TouchGFX/generated/gui_generated/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"C:/Users/Michal/Documents/stm/f746g-disco-test/f746g-disco-test/STM32746G-Discovery" -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Components/ft5336/ft5336.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

