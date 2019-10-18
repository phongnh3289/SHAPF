################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
DLOG4CHC.obj: ../DLOG4CHC.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_6.4.6/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -O4 --opt_for_speed=0 --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_6.4.6/include" --include_path="D:/Install/controlSUITE/libs/app_libs/solar/v1.2/float/include" --include_path="D:/Install/controlSUITE/device_support/f2833x/v132/DSP2833x_headers/include" --include_path="D:/Install/controlSUITE/device_support/f2833x/v132/DSP2833x_common/include" --include_path="D:/Install/controlSUITE/libs/math/IQmath/v15c/include" --include_path="D:/Install/controlSUITE/development_kits/~SupportFiles/F2833x_headers" --include_path="D:/Install/controlSUITE/libs/app_libs/motor_control/math_blocks/v4.0" --include_path="D:/Install/controlSUITE/libs/app_libs/motor_control/drivers/f2833x_v2.0" --advice:performance=all -g --define="_DEBUG" --define="LARGE_MODEL" --define="FLOATING_MATH" --quiet --diag_warning=225 --preproc_with_compile --preproc_dependency="DLOG4CHC.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

DSP2833x_ADC_cal.obj: ../DSP2833x_ADC_cal.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_6.4.6/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -O4 --opt_for_speed=0 --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_6.4.6/include" --include_path="D:/Install/controlSUITE/libs/app_libs/solar/v1.2/float/include" --include_path="D:/Install/controlSUITE/device_support/f2833x/v132/DSP2833x_headers/include" --include_path="D:/Install/controlSUITE/device_support/f2833x/v132/DSP2833x_common/include" --include_path="D:/Install/controlSUITE/libs/math/IQmath/v15c/include" --include_path="D:/Install/controlSUITE/development_kits/~SupportFiles/F2833x_headers" --include_path="D:/Install/controlSUITE/libs/app_libs/motor_control/math_blocks/v4.0" --include_path="D:/Install/controlSUITE/libs/app_libs/motor_control/drivers/f2833x_v2.0" --advice:performance=all -g --define="_DEBUG" --define="LARGE_MODEL" --define="FLOATING_MATH" --quiet --diag_warning=225 --preproc_with_compile --preproc_dependency="DSP2833x_ADC_cal.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

DSP2833x_CodeStartBranch.obj: ../DSP2833x_CodeStartBranch.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_6.4.6/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -O4 --opt_for_speed=0 --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_6.4.6/include" --include_path="D:/Install/controlSUITE/libs/app_libs/solar/v1.2/float/include" --include_path="D:/Install/controlSUITE/device_support/f2833x/v132/DSP2833x_headers/include" --include_path="D:/Install/controlSUITE/device_support/f2833x/v132/DSP2833x_common/include" --include_path="D:/Install/controlSUITE/libs/math/IQmath/v15c/include" --include_path="D:/Install/controlSUITE/development_kits/~SupportFiles/F2833x_headers" --include_path="D:/Install/controlSUITE/libs/app_libs/motor_control/math_blocks/v4.0" --include_path="D:/Install/controlSUITE/libs/app_libs/motor_control/drivers/f2833x_v2.0" --advice:performance=all -g --define="_DEBUG" --define="LARGE_MODEL" --define="FLOATING_MATH" --quiet --diag_warning=225 --preproc_with_compile --preproc_dependency="DSP2833x_CodeStartBranch.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

DSP2833x_GlobalVariableDefs.obj: ../DSP2833x_GlobalVariableDefs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_6.4.6/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -O4 --opt_for_speed=0 --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_6.4.6/include" --include_path="D:/Install/controlSUITE/libs/app_libs/solar/v1.2/float/include" --include_path="D:/Install/controlSUITE/device_support/f2833x/v132/DSP2833x_headers/include" --include_path="D:/Install/controlSUITE/device_support/f2833x/v132/DSP2833x_common/include" --include_path="D:/Install/controlSUITE/libs/math/IQmath/v15c/include" --include_path="D:/Install/controlSUITE/development_kits/~SupportFiles/F2833x_headers" --include_path="D:/Install/controlSUITE/libs/app_libs/motor_control/math_blocks/v4.0" --include_path="D:/Install/controlSUITE/libs/app_libs/motor_control/drivers/f2833x_v2.0" --advice:performance=all -g --define="_DEBUG" --define="LARGE_MODEL" --define="FLOATING_MATH" --quiet --diag_warning=225 --preproc_with_compile --preproc_dependency="DSP2833x_GlobalVariableDefs.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

DSP2833x_usDelay.obj: ../DSP2833x_usDelay.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_6.4.6/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -O4 --opt_for_speed=0 --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_6.4.6/include" --include_path="D:/Install/controlSUITE/libs/app_libs/solar/v1.2/float/include" --include_path="D:/Install/controlSUITE/device_support/f2833x/v132/DSP2833x_headers/include" --include_path="D:/Install/controlSUITE/device_support/f2833x/v132/DSP2833x_common/include" --include_path="D:/Install/controlSUITE/libs/math/IQmath/v15c/include" --include_path="D:/Install/controlSUITE/development_kits/~SupportFiles/F2833x_headers" --include_path="D:/Install/controlSUITE/libs/app_libs/motor_control/math_blocks/v4.0" --include_path="D:/Install/controlSUITE/libs/app_libs/motor_control/drivers/f2833x_v2.0" --advice:performance=all -g --define="_DEBUG" --define="LARGE_MODEL" --define="FLOATING_MATH" --quiet --diag_warning=225 --preproc_with_compile --preproc_dependency="DSP2833x_usDelay.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

SAHF-DevInit_F2833x.obj: ../SAHF-DevInit_F2833x.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_6.4.6/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -O4 --opt_for_speed=0 --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_6.4.6/include" --include_path="D:/Install/controlSUITE/libs/app_libs/solar/v1.2/float/include" --include_path="D:/Install/controlSUITE/device_support/f2833x/v132/DSP2833x_headers/include" --include_path="D:/Install/controlSUITE/device_support/f2833x/v132/DSP2833x_common/include" --include_path="D:/Install/controlSUITE/libs/math/IQmath/v15c/include" --include_path="D:/Install/controlSUITE/development_kits/~SupportFiles/F2833x_headers" --include_path="D:/Install/controlSUITE/libs/app_libs/motor_control/math_blocks/v4.0" --include_path="D:/Install/controlSUITE/libs/app_libs/motor_control/drivers/f2833x_v2.0" --advice:performance=all -g --define="_DEBUG" --define="LARGE_MODEL" --define="FLOATING_MATH" --quiet --diag_warning=225 --preproc_with_compile --preproc_dependency="SAHF-DevInit_F2833x.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

SAHF.obj: ../SAHF.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_6.4.6/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -O4 --opt_for_speed=0 --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_6.4.6/include" --include_path="D:/Install/controlSUITE/libs/app_libs/solar/v1.2/float/include" --include_path="D:/Install/controlSUITE/device_support/f2833x/v132/DSP2833x_headers/include" --include_path="D:/Install/controlSUITE/device_support/f2833x/v132/DSP2833x_common/include" --include_path="D:/Install/controlSUITE/libs/math/IQmath/v15c/include" --include_path="D:/Install/controlSUITE/development_kits/~SupportFiles/F2833x_headers" --include_path="D:/Install/controlSUITE/libs/app_libs/motor_control/math_blocks/v4.0" --include_path="D:/Install/controlSUITE/libs/app_libs/motor_control/drivers/f2833x_v2.0" --advice:performance=all -g --define="_DEBUG" --define="LARGE_MODEL" --define="FLOATING_MATH" --quiet --diag_warning=225 --preproc_with_compile --preproc_dependency="SAHF.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

SPLL_3PH_SRF_F.obj: ../SPLL_3PH_SRF_F.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_6.4.6/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -O4 --opt_for_speed=0 --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_6.4.6/include" --include_path="D:/Install/controlSUITE/libs/app_libs/solar/v1.2/float/include" --include_path="D:/Install/controlSUITE/device_support/f2833x/v132/DSP2833x_headers/include" --include_path="D:/Install/controlSUITE/device_support/f2833x/v132/DSP2833x_common/include" --include_path="D:/Install/controlSUITE/libs/math/IQmath/v15c/include" --include_path="D:/Install/controlSUITE/development_kits/~SupportFiles/F2833x_headers" --include_path="D:/Install/controlSUITE/libs/app_libs/motor_control/math_blocks/v4.0" --include_path="D:/Install/controlSUITE/libs/app_libs/motor_control/drivers/f2833x_v2.0" --advice:performance=all -g --define="_DEBUG" --define="LARGE_MODEL" --define="FLOATING_MATH" --quiet --diag_warning=225 --preproc_with_compile --preproc_dependency="SPLL_3PH_SRF_F.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


