################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/Install/CCS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"../rts2800_fpu32_fast_supplement.lib" \
"../IQmath_fpu32.lib" \
"./DLOG4CHC.obj" \
"./DSP2833x_ADC_cal.obj" \
"./DSP2833x_CodeStartBranch.obj" \
"./DSP2833x_GlobalVariableDefs.obj" \
"./DSP2833x_usDelay.obj" \
"./SAHF-DevInit_F2833x.obj" \
"./SAHF.obj" \
"./SPLL_3PH_SRF_F.obj" \
"../DSP2833x_Headers_nonBIOS.cmd" \
"../F28335_RAM_SAHF.CMD" \
$(GEN_CMDS__FLAG) \
-l"D:/Install/CCS/lib/rts2800_fpu32.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
SAHF.out \

EXE_OUTPUTS__QUOTED += \
"SAHF.out" \

BIN_OUTPUTS += \
SAHF.hex \

BIN_OUTPUTS__QUOTED += \
"SAHF.hex" \


# All Target
all: SAHF.out

# Tool invocations
SAHF.out: $(OBJS) $(CMD_SRCS) $(CMD_UPPER_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"D:/Install/CCS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -O4 --opt_for_speed=0 --advice:performance=all -g --define=_DEBUG --define=LARGE_MODEL --define=FLOATING_MATH --quiet --diag_warning=225 -z -m"SAHF.map" --stack_size=0x380 --warn_sections -i"D:/Install/CCS/lib" -i"D:/Install/CCS/include" -i"D:/OneDrive/OneDrive - Industrial University of HoChiMinh City/03_FIRMWARE/2018-2019/GIT/ACTIVE_FILTER" --reread_libs --xml_link_info="HVACI_Sensorless_2833x_linkInfo.xml" --rom_model -o "SAHF.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

SAHF.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: C2000 Hex Utility'
	"D:/Install/CCS/bin/hex2000"  -o "SAHF.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "DLOG4CHC.obj" "DSP2833x_ADC_cal.obj" "DSP2833x_CodeStartBranch.obj" "DSP2833x_GlobalVariableDefs.obj" "DSP2833x_usDelay.obj" "SAHF-DevInit_F2833x.obj" "SAHF.obj" "SPLL_3PH_SRF_F.obj" 
	-$(RM) "DSP2833x_GlobalVariableDefs.d" "SAHF-DevInit_F2833x.d" "SAHF.d" "SPLL_3PH_SRF_F.d" 
	-$(RM) "DLOG4CHC.d" "DSP2833x_ADC_cal.d" "DSP2833x_CodeStartBranch.d" "DSP2833x_usDelay.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

