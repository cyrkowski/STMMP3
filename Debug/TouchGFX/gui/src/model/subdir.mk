################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../TouchGFX/gui/src/model/Model.cpp 

OBJS += \
./TouchGFX/gui/src/model/Model.o 

CPP_DEPS += \
./TouchGFX/gui/src/model/Model.d 


# Each subdirectory must supply rules for building sources it contributes
TouchGFX/gui/src/model/Model.o: ../TouchGFX/gui/src/model/Model.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F746xx -DDEBUG -c -I../TouchGFX/gui/include -I"C:/Users/Mateusz/Desktop/stm32/STMMP3/Middlewares/ST/TouchGFX/touchgfx/framework/include/platform/hal/simulator/sdl2/vendor" -I../TouchGFX/generated/fonts/include -I"C:/Users/Mateusz/Desktop/stm32/STMMP3/Src" -I"C:/Users/Mateusz/Desktop/stm32/STMMP3/TouchGFX/generated/fonts/include" -I../TouchGFX/generated/texts/include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../TouchGFX/generated/images/include -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -I"C:/Users/Mateusz/Desktop/stm32/STMMP3/TouchGFX/generated/texts/include" -I../Middlewares/ST/TouchGFX/touchgfx/framework/include -I../TouchGFX/generated/gui_generated/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"C:/Users/Mateusz/Desktop/stm32/STMMP3/STM32746G-Discovery" -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I"C:/Users/Mateusz/Desktop/stm32/STMMP3/TouchGFX/gui/include" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I"C:/Users/Mateusz/Desktop/stm32/STMMP3/TouchGFX/generated/gui_generated/include" -I../Src -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Inc -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/Mateusz/Desktop/stm32/STMMP3/Middlewares/ST/TouchGFX/touchgfx/framework/include" -I../Middlewares/Third_Party/FatFs/src -I"C:/Users/Mateusz/Desktop/stm32/STMMP3/TouchGFX/generated/images/include" -Og -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"TouchGFX/gui/src/model/Model.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

