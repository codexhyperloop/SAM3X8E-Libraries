################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../Device_Startup/ \
../Include \
../Peripherals \
../Source


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../Device_Startup/startup_sam3xa.c \
../Device_Startup/system_sam3xa.c \
../libraries.cpp \
../main.cpp \
../Peripherals/LSM9DS0.cpp \
../Peripherals/NeoPixel.cpp \
../Source/can.cpp \
../Source/dacc.cpp \
../Source/delay.cpp \
../Source/gccPatch.cpp \
../Source/ioController.cpp \
../Source/pio.cpp \
../Source/pmc.cpp \
../Source/pwm.cpp \
../Source/ringbuffer.cpp \
../Source/rtc.cpp \
../Source/rtt.cpp \
../Source/spi.cpp \
../Source/tc.cpp \
../Source/timetick.cpp \
../Source/twi.cpp \
../Source/UART.cpp \
../Source/USART.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
Device_Startup/startup_sam3xa.o \
Device_Startup/system_sam3xa.o \
libraries.o \
main.o \
Peripherals/LSM9DS0.o \
Peripherals/NeoPixel.o \
Source/can.o \
Source/dacc.o \
Source/delay.o \
Source/gccPatch.o \
Source/ioController.o \
Source/pio.o \
Source/pmc.o \
Source/pwm.o \
Source/ringbuffer.o \
Source/rtc.o \
Source/rtt.o \
Source/spi.o \
Source/tc.o \
Source/timetick.o \
Source/twi.o \
Source/UART.o \
Source/USART.o

OBJS_AS_ARGS +=  \
Device_Startup/startup_sam3xa.o \
Device_Startup/system_sam3xa.o \
libraries.o \
main.o \
Peripherals/LSM9DS0.o \
Peripherals/NeoPixel.o \
Source/can.o \
Source/dacc.o \
Source/delay.o \
Source/gccPatch.o \
Source/ioController.o \
Source/pio.o \
Source/pmc.o \
Source/pwm.o \
Source/ringbuffer.o \
Source/rtc.o \
Source/rtt.o \
Source/spi.o \
Source/tc.o \
Source/timetick.o \
Source/twi.o \
Source/UART.o \
Source/USART.o

C_DEPS +=  \
Device_Startup/startup_sam3xa.d \
Device_Startup/system_sam3xa.d \
libraries.d \
main.d \
Peripherals/LSM9DS0.d \
Peripherals/NeoPixel.d \
Source/can.d \
Source/dacc.d \
Source/delay.d \
Source/gccPatch.d \
Source/ioController.d \
Source/pio.d \
Source/pmc.d \
Source/pwm.d \
Source/ringbuffer.d \
Source/rtc.d \
Source/rtt.d \
Source/spi.d \
Source/tc.d \
Source/timetick.d \
Source/twi.d \
Source/UART.d \
Source/USART.d

C_DEPS_AS_ARGS +=  \
Device_Startup/startup_sam3xa.d \
Device_Startup/system_sam3xa.d \
libraries.d \
main.d \
Peripherals/LSM9DS0.d \
Peripherals/NeoPixel.d \
Source/can.d \
Source/dacc.d \
Source/delay.d \
Source/gccPatch.d \
Source/ioController.d \
Source/pio.d \
Source/pmc.d \
Source/pwm.d \
Source/ringbuffer.d \
Source/rtc.d \
Source/rtt.d \
Source/spi.d \
Source/tc.d \
Source/timetick.d \
Source/twi.d \
Source/UART.d \
Source/USART.d

OUTPUT_FILE_PATH +=SAM3X8E_OfficialDevelopment.elf

OUTPUT_FILE_PATH_AS_ARGS +=SAM3X8E_OfficialDevelopment.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+=  \
../Device_Startup/sam3x8e_flash.ld \
../Device_Startup/sam3x8e_sram.ld \
../Device_Startup/sam3xa_flash.ld \
../Device_Startup/sam3xa_sram.ld


# AVR32/GNU C Compiler















































Device_Startup/%.o: ../Device_Startup/%.c
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 4.9.3
	$(QUOTE)F:\Atmel Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE)  -x c -mthumb -D__SAM3X8E__ -DDEBUG  -I"F:\Atmel Studio\7.0\Packs\atmel\SAM3X_DFP\1.0.35\include" -I"F:\Atmel Studio\7.0\Packs\arm\cmsis\4.2.0\CMSIS\Include"  -O1 -ffunction-sections -mlong-calls -g3 -Wall -mcpu=cortex-m3 -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./%.o: .././%.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 4.9.3
	$(QUOTE)F:\Atmel Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAM3X8E__ -DDEBUG  -I"F:\Atmel Studio\7.0\Packs\atmel\SAM3X_DFP\1.0.35\include" -I"F:\Atmel Studio\7.0\Packs\arm\cmsis\4.2.0\CMSIS\Include" -I"../include" -I"../Peripherals" -I"../source"  -O3 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m3 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

Peripherals/%.o: ../Peripherals/%.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 4.9.3
	$(QUOTE)F:\Atmel Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAM3X8E__ -DDEBUG  -I"F:\Atmel Studio\7.0\Packs\atmel\SAM3X_DFP\1.0.35\include" -I"F:\Atmel Studio\7.0\Packs\arm\cmsis\4.2.0\CMSIS\Include" -I"../include" -I"../Peripherals" -I"../source"  -O3 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m3 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

Source/%.o: ../Source/%.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 4.9.3
	$(QUOTE)F:\Atmel Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAM3X8E__ -DDEBUG  -I"F:\Atmel Studio\7.0\Packs\atmel\SAM3X_DFP\1.0.35\include" -I"F:\Atmel Studio\7.0\Packs\arm\cmsis\4.2.0\CMSIS\Include" -I"../include" -I"../Peripherals" -I"../source"  -O3 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m3 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: ARM/GNU Linker : 4.9.3
	$(QUOTE)F:\Atmel Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -mthumb -Wl,-Map="SAM3X8E_OfficialDevelopment.map" -Wl,--start-group -lm  -Wl,--end-group -L"..\\Device_Startup"  -Wl,--gc-sections -mcpu=cortex-m3 -Tsam3x8e_flash.ld  
	@echo Finished building target: $@
	"F:\Atmel Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O binary "SAM3X8E_OfficialDevelopment.elf" "SAM3X8E_OfficialDevelopment.bin"
	"F:\Atmel Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "SAM3X8E_OfficialDevelopment.elf" "SAM3X8E_OfficialDevelopment.hex"
	"F:\Atmel Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O binary "SAM3X8E_OfficialDevelopment.elf" "SAM3X8E_OfficialDevelopment.eep" || exit 0
	"F:\Atmel Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objdump.exe" -h -S "SAM3X8E_OfficialDevelopment.elf" > "SAM3X8E_OfficialDevelopment.lss"
	"F:\Atmel Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "SAM3X8E_OfficialDevelopment.elf" "SAM3X8E_OfficialDevelopment.srec"
	"F:\Atmel Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-size.exe" "SAM3X8E_OfficialDevelopment.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "SAM3X8E_OfficialDevelopment.elf" "SAM3X8E_OfficialDevelopment.a" "SAM3X8E_OfficialDevelopment.hex" "SAM3X8E_OfficialDevelopment.bin" "SAM3X8E_OfficialDevelopment.lss" "SAM3X8E_OfficialDevelopment.eep" "SAM3X8E_OfficialDevelopment.map" "SAM3X8E_OfficialDevelopment.srec"
	