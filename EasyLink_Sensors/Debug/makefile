################################################################################
# Automatically-generated file. Do not edit!
################################################################################

CG_TOOL_ROOT := /home/shanty/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS

GEN_OPTS += /home/shanty/workspace_v8/tirtos_builds_CC1310_LAUNCHXL_release_ccs/Debug/configPkg/compiler.opt
GEN_OPTS__FLAG := --cmd_file="/home/shanty/workspace_v8/tirtos_builds_CC1310_LAUNCHXL_release_ccs/Debug/configPkg/compiler.opt"
GEN_CMDS += /home/shanty/workspace_v8/tirtos_builds_CC1310_LAUNCHXL_release_ccs/Debug/configPkg/linker.cmd
GEN_CMDS__FLAG := -l"/home/shanty/workspace_v8/tirtos_builds_CC1310_LAUNCHXL_release_ccs/Debug/configPkg/linker.cmd"

ORDERED_OBJS += \
"./CC1310_LAUNCHXL.obj" \
"./CC1310_LAUNCHXL_fxns.obj" \
"./ccfg.obj" \
"./rfEasyLinkTx.obj" \
"./sensors.obj" \
"./easylink/EasyLink.obj" \
"./easylink/easylink_config.obj" \
"./smartrf_settings/smartrf_settings.obj" \
"./smartrf_settings/smartrf_settings_predefined.obj" \
"../CC1310_LAUNCHXL_TIRTOS.cmd" \
-l"ti/display/lib/display.aem3" \
-l"ti/grlib/lib/ccs/m3/grlib.a" \
-l"third_party/spiffs/lib/ccs/m3/spiffs_cc26xx.a" \
-l"ti/drivers/rf/lib/rf_multiMode_cc13x0.aem3" \
-l"ti/drivers/lib/drivers_cc13x0.aem3" \
-l"ti/dpl/lib/dpl_cc13x0.aem3" \
$(GEN_CMDS__FLAG) \
-l"ti/devices/cc13x0/driverlib/bin/ccs/driverlib.lib" \
-llibc.a \

-include ../makefile.init

RM := rm -rf
RMDIR := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include easylink/subdir_vars.mk
-include smartrf_settings/subdir_vars.mk
-include subdir_rules.mk
-include easylink/subdir_rules.mk
-include smartrf_settings/subdir_rules.mk
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
EasyLink_Sensors.out \

EXE_OUTPUTS__QUOTED += \
"EasyLink_Sensors.out" \

BIN_OUTPUTS += \
EasyLink_Sensors.hex \

BIN_OUTPUTS__QUOTED += \
"EasyLink_Sensors.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "EasyLink_Sensors.out"

dependents:
	-cd /home/shanty/workspace_v8/tirtos_builds_CC1310_LAUNCHXL_release_ccs/Debug && $(MAKE) all

# Tool invocations
EasyLink_Sensors.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS) /home/shanty/workspace_v8/tirtos_builds_CC1310_LAUNCHXL_release_ccs/Debug/tirtos_builds_CC1310_LAUNCHXL_release_ccs.lib
	@echo 'Building target: "$@"'
	@echo 'Invoking: ARM Linker'
	"/home/shanty/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --define=DeviceFamily_CC13X0 --define=CCFG_FORCE_VDDR_HH=0 --define=SUPPORT_PHY_CUSTOM --define=SUPPORT_PHY_50KBPS2GFSK --define=SUPPORT_PHY_625BPSLRM --define=SUPPORT_PHY_5KBPSSLLR -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on -z -m"EasyLink_Sensors.map" -i"/mnt/76269E6E269E2F5D/simplelink_cc13x0_sdk_2_40_00_20__win/simplelink_cc13x0_sdk_2_40_00_20/source" -i"/mnt/76269E6E269E2F5D/simplelink_cc13x0_sdk_2_40_00_20__win/simplelink_cc13x0_sdk_2_40_00_20/kernel/tirtos/packages" -i"/home/shanty/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/lib" --diag_wrap=off --display_error_number --warn_sections --xml_link_info="EasyLink_Sensors_linkInfo.xml" --rom_model -o "EasyLink_Sensors.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

EasyLink_Sensors.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: ARM Hex Utility'
	"/home/shanty/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/bin/armhex"  -o "EasyLink_Sensors.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "CC1310_LAUNCHXL.obj" "CC1310_LAUNCHXL_fxns.obj" "ccfg.obj" "rfEasyLinkTx.obj" "sensors.obj" "easylink/EasyLink.obj" "easylink/easylink_config.obj" "smartrf_settings/smartrf_settings.obj" "smartrf_settings/smartrf_settings_predefined.obj" 
	-$(RM) "CC1310_LAUNCHXL.d" "CC1310_LAUNCHXL_fxns.d" "ccfg.d" "rfEasyLinkTx.d" "sensors.d" "easylink/EasyLink.d" "easylink/easylink_config.d" "smartrf_settings/smartrf_settings.d" "smartrf_settings/smartrf_settings_predefined.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/shanty/workspace_v8/tirtos_builds_CC1310_LAUNCHXL_release_ccs/Debug/tirtos_builds_CC1310_LAUNCHXL_release_ccs.lib:

-include ../makefile.targets

