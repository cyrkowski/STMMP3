################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Startup/startup_stm32f746nghx.s 

OBJS += \
./Startup/startup_stm32f746nghx.o 


# Each subdirectory must supply rules for building sources it contributes
Startup/%.o: ../Startup/%.s
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -c -I"C:/Users/Mateusz/Desktop/stm32/STMMP3/Middlewares/ST/TouchGFX/touchgfx/framework/include/platform/hal/simulator/sdl2/vendor" -I../ -x assembler-with-cpp --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

