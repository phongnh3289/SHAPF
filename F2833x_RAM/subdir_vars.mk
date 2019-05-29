################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../DSP2833x_Headers_nonBIOS.cmd 

LIB_SRCS += \
../IQmath_fpu32.lib \
../rts2800_fpu32_fast_supplement.lib 

ASM_SRCS += \
../DLOG4CHC.asm \
../DSP2833x_ADC_cal.asm \
../DSP2833x_CodeStartBranch.asm \
../DSP2833x_usDelay.asm 

CMD_UPPER_SRCS += \
../F28335_RAM_SAHF.CMD 

C_SRCS += \
../DSP2833x_GlobalVariableDefs.c \
../SAHF-DevInit_F2833x.c \
../SAHF.c 

C_DEPS += \
./DSP2833x_GlobalVariableDefs.d \
./SAHF-DevInit_F2833x.d \
./SAHF.d 

OBJS += \
./DLOG4CHC.obj \
./DSP2833x_ADC_cal.obj \
./DSP2833x_CodeStartBranch.obj \
./DSP2833x_GlobalVariableDefs.obj \
./DSP2833x_usDelay.obj \
./SAHF-DevInit_F2833x.obj \
./SAHF.obj 

ASM_DEPS += \
./DLOG4CHC.d \
./DSP2833x_ADC_cal.d \
./DSP2833x_CodeStartBranch.d \
./DSP2833x_usDelay.d 

OBJS__QUOTED += \
"DLOG4CHC.obj" \
"DSP2833x_ADC_cal.obj" \
"DSP2833x_CodeStartBranch.obj" \
"DSP2833x_GlobalVariableDefs.obj" \
"DSP2833x_usDelay.obj" \
"SAHF-DevInit_F2833x.obj" \
"SAHF.obj" 

C_DEPS__QUOTED += \
"DSP2833x_GlobalVariableDefs.d" \
"SAHF-DevInit_F2833x.d" \
"SAHF.d" 

ASM_DEPS__QUOTED += \
"DLOG4CHC.d" \
"DSP2833x_ADC_cal.d" \
"DSP2833x_CodeStartBranch.d" \
"DSP2833x_usDelay.d" 

ASM_SRCS__QUOTED += \
"../DLOG4CHC.asm" \
"../DSP2833x_ADC_cal.asm" \
"../DSP2833x_CodeStartBranch.asm" \
"../DSP2833x_usDelay.asm" 

C_SRCS__QUOTED += \
"../DSP2833x_GlobalVariableDefs.c" \
"../SAHF-DevInit_F2833x.c" \
"../SAHF.c" 


