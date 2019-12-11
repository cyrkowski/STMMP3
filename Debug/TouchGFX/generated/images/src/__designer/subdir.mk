################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../TouchGFX/generated/images/src/__designer/blue_buttons_round_edge_small.cpp \
../TouchGFX/generated/images/src/__designer/blue_buttons_round_edge_small_pressed.cpp 

OBJS += \
./TouchGFX/generated/images/src/__designer/blue_buttons_round_edge_small.o \
./TouchGFX/generated/images/src/__designer/blue_buttons_round_edge_small_pressed.o 

CPP_DEPS += \
./TouchGFX/generated/images/src/__designer/blue_buttons_round_edge_small.d \
./TouchGFX/generated/images/src/__designer/blue_buttons_round_edge_small_pressed.d 


# Each subdirectory must supply rules for building sources it contributes
TouchGFX/generated/images/src/__designer/blue_buttons_round_edge_small.o: ../TouchGFX/generated/images/src/__designer/blue_buttons_round_edge_small.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F746xx -DDEBUG -c -I../TouchGFX/gui/include -I"C:/Users/Michal/Documents/stm/f746g-disco-test/f746g-disco-test/Middlewares/ST/TouchGFX/touchgfx/framework/include/platform/hal/simulator/sdl2/vendor" -I../TouchGFX/generated/fonts/include -I"C:/Users/Michal/Documents/stm/f746g-disco-test/f746g-disco-test/Src" -I"C:/Users/Michal/Documents/stm/f746g-disco-test/f746g-disco-test/TouchGFX/generated/fonts/include" -I../TouchGFX/generated/texts/include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../TouchGFX/generated/images/include -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -I"C:/Users/Michal/Documents/stm/f746g-disco-test/f746g-disco-test/TouchGFX/generated/texts/include" -I../Middlewares/ST/TouchGFX/touchgfx/framework/include -I../TouchGFX/generated/gui_generated/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"C:/Users/Michal/Documents/stm/f746g-disco-test/f746g-disco-test/STM32746G-Discovery" -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I"C:/Users/Michal/Documents/stm/f746g-disco-test/f746g-disco-test/TouchGFX/gui/include" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I"C:/Users/Michal/Documents/stm/f746g-disco-test/f746g-disco-test/TouchGFX/generated/gui_generated/include" -I../Src -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Inc -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/Michal/Documents/stm/f746g-disco-test/f746g-disco-test/Middlewares/ST/TouchGFX/touchgfx/framework/include" -I../Middlewares/Third_Party/FatFs/src -I"C:/Users/Michal/Documents/stm/f746g-disco-test/f746g-disco-test/TouchGFX/generated/images/include" -Og -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"TouchGFX/generated/images/src/__designer/blue_buttons_round_edge_small.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
TouchGFX/generated/images/src/__designer/blue_buttons_round_edge_small_pressed.o: ../TouchGFX/generated/images/src/__designer/blue_buttons_round_edge_small_pressed.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F746xx -DDEBUG -c -I../TouchGFX/gui/include -I"C:/Users/Michal/Documents/stm/f746g-disco-test/f746g-disco-test/Middlewares/ST/TouchGFX/touchgfx/framework/include/platform/hal/simulator/sdl2/vendor" -I../TouchGFX/generated/fonts/include -I"C:/Users/Michal/Documents/stm/f746g-disco-test/f746g-disco-test/Src" -I"C:/Users/Michal/Documents/stm/f746g-disco-test/f746g-disco-test/TouchGFX/generated/fonts/include" -I../TouchGFX/generated/texts/include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../TouchGFX/generated/images/include -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -I"C:/Users/Michal/Documents/stm/f746g-disco-test/f746g-disco-test/TouchGFX/generated/texts/include" -I../Middlewares/ST/TouchGFX/touchgfx/framework/include -I../TouchGFX/generated/gui_generated/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"C:/Users/Michal/Documents/stm/f746g-disco-test/f746g-disco-test/STM32746G-Discovery" -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I"C:/Users/Michal/Documents/stm/f746g-disco-test/f746g-disco-test/TouchGFX/gui/include" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I"C:/Users/Michal/Documents/stm/f746g-disco-test/f746g-disco-test/TouchGFX/generated/gui_generated/include" -I../Src -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Inc -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/Michal/Documents/stm/f746g-disco-test/f746g-disco-test/Middlewares/ST/TouchGFX/touchgfx/framework/include" -I../Middlewares/Third_Party/FatFs/src -I"C:/Users/Michal/Documents/stm/f746g-disco-test/f746g-disco-test/TouchGFX/generated/images/include" -Og -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"TouchGFX/generated/images/src/__designer/blue_buttons_round_edge_small_pressed.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

