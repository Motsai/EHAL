################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Omid\ Sarbishei/Documents/GitHub/EHAL/ARM/src/ResetEntry.c \
C:/Users/Omid\ Sarbishei/Documents/GitHub/EHAL/ARM/Microchip/SAM4E/src/system_sam4e.c \
C:/Users/Omid\ Sarbishei/Documents/GitHub/EHAL/ARM/Microchip/SAM4E/src/vectors_sam4e.c 

C_DEPS += \
./src/ResetEntry.d \
./src/system_sam4e.d \
./src/vectors_sam4e.d 

OBJS += \
./src/ResetEntry.o \
./src/system_sam4e.o \
./src/vectors_sam4e.o 


# Each subdirectory must supply rules for building sources it contributes
src/ResetEntry.o: C:/Users/Omid\ Sarbishei/Documents/GitHub/EHAL/ARM/src/ResetEntry.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -D__SAM4E16E__ -I"../include" -I"../../include" -I"../../../include" -I"../../../../../include" -I"../../../../../../include" -I"../../../../../../include/coredev" -I"../../../../../../../external/CMSIS/CMSIS/Core/Include" -std=gnu11 -MMD -MP -MF"src/ResetEntry.d" -MT"src/ResetEntry.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/system_sam4e.o: C:/Users/Omid\ Sarbishei/Documents/GitHub/EHAL/ARM/Microchip/SAM4E/src/system_sam4e.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -D__SAM4E16E__ -I"../include" -I"../../include" -I"../../../include" -I"../../../../../include" -I"../../../../../../include" -I"../../../../../../include/coredev" -I"../../../../../../../external/CMSIS/CMSIS/Core/Include" -std=gnu11 -MMD -MP -MF"src/system_sam4e.d" -MT"src/system_sam4e.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/vectors_sam4e.o: C:/Users/Omid\ Sarbishei/Documents/GitHub/EHAL/ARM/Microchip/SAM4E/src/vectors_sam4e.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -D__SAM4E16E__ -I"../include" -I"../../include" -I"../../../include" -I"../../../../../include" -I"../../../../../../include" -I"../../../../../../include/coredev" -I"../../../../../../../external/CMSIS/CMSIS/Core/Include" -std=gnu11 -MMD -MP -MF"src/vectors_sam4e.d" -MT"src/vectors_sam4e.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


