#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/final.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/final.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=main.c ProjectEventChecker.c Debug.c DebugService.c TopHSM.c NavigateHSM.c CombatHSM.c RecoveryHSM.c DriveService.c TapeService.c TrackWireService.c BumperService.c BeaconService.c LauncherService.c BatteryService.c ECE118/src/AD.c ECE118/src/BOARD.c ECE118/src/ES_CheckEvents.c ECE118/src/ES_Framework.c ECE118/src/ES_KeyboardInput.c ECE118/src/ES_PostList.c ECE118/src/ES_Queue.c ECE118/src/ES_TattleTale.c ECE118/src/ES_Timers.c ECE118/src/IO_Ports.c ECE118/src/LED.c ECE118/src/pwm.c ECE118/src/RC_Servo.c ECE118/src/serial.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/main.o ${OBJECTDIR}/ProjectEventChecker.o ${OBJECTDIR}/Debug.o ${OBJECTDIR}/DebugService.o ${OBJECTDIR}/TopHSM.o ${OBJECTDIR}/NavigateHSM.o ${OBJECTDIR}/CombatHSM.o ${OBJECTDIR}/RecoveryHSM.o ${OBJECTDIR}/DriveService.o ${OBJECTDIR}/TapeService.o ${OBJECTDIR}/TrackWireService.o ${OBJECTDIR}/BumperService.o ${OBJECTDIR}/BeaconService.o ${OBJECTDIR}/LauncherService.o ${OBJECTDIR}/BatteryService.o ${OBJECTDIR}/ECE118/src/AD.o ${OBJECTDIR}/ECE118/src/BOARD.o ${OBJECTDIR}/ECE118/src/ES_CheckEvents.o ${OBJECTDIR}/ECE118/src/ES_Framework.o ${OBJECTDIR}/ECE118/src/ES_KeyboardInput.o ${OBJECTDIR}/ECE118/src/ES_PostList.o ${OBJECTDIR}/ECE118/src/ES_Queue.o ${OBJECTDIR}/ECE118/src/ES_TattleTale.o ${OBJECTDIR}/ECE118/src/ES_Timers.o ${OBJECTDIR}/ECE118/src/IO_Ports.o ${OBJECTDIR}/ECE118/src/LED.o ${OBJECTDIR}/ECE118/src/pwm.o ${OBJECTDIR}/ECE118/src/RC_Servo.o ${OBJECTDIR}/ECE118/src/serial.o
POSSIBLE_DEPFILES=${OBJECTDIR}/main.o.d ${OBJECTDIR}/ProjectEventChecker.o.d ${OBJECTDIR}/Debug.o.d ${OBJECTDIR}/DebugService.o.d ${OBJECTDIR}/TopHSM.o.d ${OBJECTDIR}/NavigateHSM.o.d ${OBJECTDIR}/CombatHSM.o.d ${OBJECTDIR}/RecoveryHSM.o.d ${OBJECTDIR}/DriveService.o.d ${OBJECTDIR}/TapeService.o.d ${OBJECTDIR}/TrackWireService.o.d ${OBJECTDIR}/BumperService.o.d ${OBJECTDIR}/BeaconService.o.d ${OBJECTDIR}/LauncherService.o.d ${OBJECTDIR}/BatteryService.o.d ${OBJECTDIR}/ECE118/src/AD.o.d ${OBJECTDIR}/ECE118/src/BOARD.o.d ${OBJECTDIR}/ECE118/src/ES_CheckEvents.o.d ${OBJECTDIR}/ECE118/src/ES_Framework.o.d ${OBJECTDIR}/ECE118/src/ES_KeyboardInput.o.d ${OBJECTDIR}/ECE118/src/ES_PostList.o.d ${OBJECTDIR}/ECE118/src/ES_Queue.o.d ${OBJECTDIR}/ECE118/src/ES_TattleTale.o.d ${OBJECTDIR}/ECE118/src/ES_Timers.o.d ${OBJECTDIR}/ECE118/src/IO_Ports.o.d ${OBJECTDIR}/ECE118/src/LED.o.d ${OBJECTDIR}/ECE118/src/pwm.o.d ${OBJECTDIR}/ECE118/src/RC_Servo.o.d ${OBJECTDIR}/ECE118/src/serial.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/main.o ${OBJECTDIR}/ProjectEventChecker.o ${OBJECTDIR}/Debug.o ${OBJECTDIR}/DebugService.o ${OBJECTDIR}/TopHSM.o ${OBJECTDIR}/NavigateHSM.o ${OBJECTDIR}/CombatHSM.o ${OBJECTDIR}/RecoveryHSM.o ${OBJECTDIR}/DriveService.o ${OBJECTDIR}/TapeService.o ${OBJECTDIR}/TrackWireService.o ${OBJECTDIR}/BumperService.o ${OBJECTDIR}/BeaconService.o ${OBJECTDIR}/LauncherService.o ${OBJECTDIR}/BatteryService.o ${OBJECTDIR}/ECE118/src/AD.o ${OBJECTDIR}/ECE118/src/BOARD.o ${OBJECTDIR}/ECE118/src/ES_CheckEvents.o ${OBJECTDIR}/ECE118/src/ES_Framework.o ${OBJECTDIR}/ECE118/src/ES_KeyboardInput.o ${OBJECTDIR}/ECE118/src/ES_PostList.o ${OBJECTDIR}/ECE118/src/ES_Queue.o ${OBJECTDIR}/ECE118/src/ES_TattleTale.o ${OBJECTDIR}/ECE118/src/ES_Timers.o ${OBJECTDIR}/ECE118/src/IO_Ports.o ${OBJECTDIR}/ECE118/src/LED.o ${OBJECTDIR}/ECE118/src/pwm.o ${OBJECTDIR}/ECE118/src/RC_Servo.o ${OBJECTDIR}/ECE118/src/serial.o

# Source Files
SOURCEFILES=main.c ProjectEventChecker.c Debug.c DebugService.c TopHSM.c NavigateHSM.c CombatHSM.c RecoveryHSM.c DriveService.c TapeService.c TrackWireService.c BumperService.c BeaconService.c LauncherService.c BatteryService.c ECE118/src/AD.c ECE118/src/BOARD.c ECE118/src/ES_CheckEvents.c ECE118/src/ES_Framework.c ECE118/src/ES_KeyboardInput.c ECE118/src/ES_PostList.c ECE118/src/ES_Queue.c ECE118/src/ES_TattleTale.c ECE118/src/ES_Timers.c ECE118/src/IO_Ports.c ECE118/src/LED.c ECE118/src/pwm.c ECE118/src/RC_Servo.c ECE118/src/serial.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/final.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX320F128H
MP_LINKER_FILE_OPTION=,--script="ECE118/bootloader320.ld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/93c303a259ef07fd2906d95b1a8daaaf4451062d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ProjectEventChecker.o: ProjectEventChecker.c  .generated_files/flags/default/e375c20238755f56ea90b8ed45a416db9acad760 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ProjectEventChecker.o.d 
	@${RM} ${OBJECTDIR}/ProjectEventChecker.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ProjectEventChecker.o.d" -o ${OBJECTDIR}/ProjectEventChecker.o ProjectEventChecker.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/Debug.o: Debug.c  .generated_files/flags/default/cceac41d567a02849973c1de621e990ce2372895 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Debug.o.d 
	@${RM} ${OBJECTDIR}/Debug.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/Debug.o.d" -o ${OBJECTDIR}/Debug.o Debug.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/DebugService.o: DebugService.c  .generated_files/flags/default/b6da3e2b7b39f29ced7f8c8d92509d7f23f540b8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/DebugService.o.d 
	@${RM} ${OBJECTDIR}/DebugService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/DebugService.o.d" -o ${OBJECTDIR}/DebugService.o DebugService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TopHSM.o: TopHSM.c  .generated_files/flags/default/f80c8c180fcc7b3393772be8c27d1deab10dbd8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TopHSM.o.d 
	@${RM} ${OBJECTDIR}/TopHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/TopHSM.o.d" -o ${OBJECTDIR}/TopHSM.o TopHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/NavigateHSM.o: NavigateHSM.c  .generated_files/flags/default/23b7e018b538139bac35fddb6fc18f425095ab60 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/NavigateHSM.o.d 
	@${RM} ${OBJECTDIR}/NavigateHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/NavigateHSM.o.d" -o ${OBJECTDIR}/NavigateHSM.o NavigateHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/CombatHSM.o: CombatHSM.c  .generated_files/flags/default/5d9c3e91bba106146bf16b277005d380f92b68a8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/CombatHSM.o.d 
	@${RM} ${OBJECTDIR}/CombatHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/CombatHSM.o.d" -o ${OBJECTDIR}/CombatHSM.o CombatHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/RecoveryHSM.o: RecoveryHSM.c  .generated_files/flags/default/6ce483ef96ccafe352ca3a8c47029f778a29ca26 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/RecoveryHSM.o.d 
	@${RM} ${OBJECTDIR}/RecoveryHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/RecoveryHSM.o.d" -o ${OBJECTDIR}/RecoveryHSM.o RecoveryHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/DriveService.o: DriveService.c  .generated_files/flags/default/86e4eccdae86d05827e2e65b14870f070efb80b2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/DriveService.o.d 
	@${RM} ${OBJECTDIR}/DriveService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/DriveService.o.d" -o ${OBJECTDIR}/DriveService.o DriveService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TapeService.o: TapeService.c  .generated_files/flags/default/4f38d31f583463dbf1ac64667fc408afedb96e1f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TapeService.o.d 
	@${RM} ${OBJECTDIR}/TapeService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/TapeService.o.d" -o ${OBJECTDIR}/TapeService.o TapeService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TrackWireService.o: TrackWireService.c  .generated_files/flags/default/10255b80ac69cd59940d2af4b5e46e99c5153dec .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TrackWireService.o.d 
	@${RM} ${OBJECTDIR}/TrackWireService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/TrackWireService.o.d" -o ${OBJECTDIR}/TrackWireService.o TrackWireService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/BumperService.o: BumperService.c  .generated_files/flags/default/7f3a6ec885ca7ebef8352cfd7d3eea366af523b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/BumperService.o.d 
	@${RM} ${OBJECTDIR}/BumperService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/BumperService.o.d" -o ${OBJECTDIR}/BumperService.o BumperService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/BeaconService.o: BeaconService.c  .generated_files/flags/default/9e2826858f6c0f5846636bbce721dfe529bc1bbd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/BeaconService.o.d 
	@${RM} ${OBJECTDIR}/BeaconService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/BeaconService.o.d" -o ${OBJECTDIR}/BeaconService.o BeaconService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/LauncherService.o: LauncherService.c  .generated_files/flags/default/b6fcf81ef7e3aa8560328a95d7f6028ac35e6f3a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/LauncherService.o.d 
	@${RM} ${OBJECTDIR}/LauncherService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/LauncherService.o.d" -o ${OBJECTDIR}/LauncherService.o LauncherService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/BatteryService.o: BatteryService.c  .generated_files/flags/default/cab56c595a4e5b8b49e874e095f6758101a5dc6a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/BatteryService.o.d 
	@${RM} ${OBJECTDIR}/BatteryService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/BatteryService.o.d" -o ${OBJECTDIR}/BatteryService.o BatteryService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/AD.o: ECE118/src/AD.c  .generated_files/flags/default/1b1148b695d80b460ca828dc8b1a2bd90c2e16bc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/AD.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/AD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/AD.o.d" -o ${OBJECTDIR}/ECE118/src/AD.o ECE118/src/AD.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/BOARD.o: ECE118/src/BOARD.c  .generated_files/flags/default/be58f79477394de47ac6a0261e6ef31b059e5007 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/BOARD.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/BOARD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/BOARD.o.d" -o ${OBJECTDIR}/ECE118/src/BOARD.o ECE118/src/BOARD.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/ES_CheckEvents.o: ECE118/src/ES_CheckEvents.c  .generated_files/flags/default/a4409323ee4a75ce6e4f86b8ac02aace7c2022b5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_CheckEvents.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_CheckEvents.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/ES_CheckEvents.o.d" -o ${OBJECTDIR}/ECE118/src/ES_CheckEvents.o ECE118/src/ES_CheckEvents.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/ES_Framework.o: ECE118/src/ES_Framework.c  .generated_files/flags/default/d880482531f06bc6d60975e558eaa6eed405b306 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_Framework.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_Framework.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/ES_Framework.o.d" -o ${OBJECTDIR}/ECE118/src/ES_Framework.o ECE118/src/ES_Framework.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/ES_KeyboardInput.o: ECE118/src/ES_KeyboardInput.c  .generated_files/flags/default/f360f485e47f567bebcda124103d9d663698cf5a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_KeyboardInput.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_KeyboardInput.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/ES_KeyboardInput.o.d" -o ${OBJECTDIR}/ECE118/src/ES_KeyboardInput.o ECE118/src/ES_KeyboardInput.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/ES_PostList.o: ECE118/src/ES_PostList.c  .generated_files/flags/default/9fb8814df34b2a036cbc3b3c57060c6c6d504613 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_PostList.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_PostList.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/ES_PostList.o.d" -o ${OBJECTDIR}/ECE118/src/ES_PostList.o ECE118/src/ES_PostList.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/ES_Queue.o: ECE118/src/ES_Queue.c  .generated_files/flags/default/1efb0d7270cbb66dc20eda35b7980f2971f81df8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_Queue.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_Queue.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/ES_Queue.o.d" -o ${OBJECTDIR}/ECE118/src/ES_Queue.o ECE118/src/ES_Queue.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/ES_TattleTale.o: ECE118/src/ES_TattleTale.c  .generated_files/flags/default/cc67be953cfb676984cdf52933e6b61bb45c09e6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_TattleTale.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_TattleTale.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/ES_TattleTale.o.d" -o ${OBJECTDIR}/ECE118/src/ES_TattleTale.o ECE118/src/ES_TattleTale.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/ES_Timers.o: ECE118/src/ES_Timers.c  .generated_files/flags/default/8deb60344766013e652b179b5389523d67e1dae .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_Timers.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_Timers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/ES_Timers.o.d" -o ${OBJECTDIR}/ECE118/src/ES_Timers.o ECE118/src/ES_Timers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/IO_Ports.o: ECE118/src/IO_Ports.c  .generated_files/flags/default/5324d0e56fe10c7f03b15b0d1a2764a684a67919 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/IO_Ports.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/IO_Ports.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/IO_Ports.o.d" -o ${OBJECTDIR}/ECE118/src/IO_Ports.o ECE118/src/IO_Ports.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/LED.o: ECE118/src/LED.c  .generated_files/flags/default/162ea78e1e408353597760c36df436ad4b320a06 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/LED.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/LED.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/LED.o.d" -o ${OBJECTDIR}/ECE118/src/LED.o ECE118/src/LED.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/pwm.o: ECE118/src/pwm.c  .generated_files/flags/default/49faafcd1df7c98d3bff631301fa68da49665156 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/pwm.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/pwm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/pwm.o.d" -o ${OBJECTDIR}/ECE118/src/pwm.o ECE118/src/pwm.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/RC_Servo.o: ECE118/src/RC_Servo.c  .generated_files/flags/default/377db97c6504c629e020f7bfac91b781079a6890 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/RC_Servo.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/RC_Servo.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/RC_Servo.o.d" -o ${OBJECTDIR}/ECE118/src/RC_Servo.o ECE118/src/RC_Servo.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/serial.o: ECE118/src/serial.c  .generated_files/flags/default/5954f9411a706f23ca4edc72922a5846721ee0f9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/serial.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/serial.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/serial.o.d" -o ${OBJECTDIR}/ECE118/src/serial.o ECE118/src/serial.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/121257999e939e793ce1270aea919816f3c7b1e1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ProjectEventChecker.o: ProjectEventChecker.c  .generated_files/flags/default/5911ad723497869a5714e44067841c4c4b678a39 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ProjectEventChecker.o.d 
	@${RM} ${OBJECTDIR}/ProjectEventChecker.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ProjectEventChecker.o.d" -o ${OBJECTDIR}/ProjectEventChecker.o ProjectEventChecker.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/Debug.o: Debug.c  .generated_files/flags/default/ffd2af70869df7ef5521c92273f40501d523685d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Debug.o.d 
	@${RM} ${OBJECTDIR}/Debug.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/Debug.o.d" -o ${OBJECTDIR}/Debug.o Debug.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/DebugService.o: DebugService.c  .generated_files/flags/default/30079a9dd50cb254f9cef06d6a07b12b85d641aa .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/DebugService.o.d 
	@${RM} ${OBJECTDIR}/DebugService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/DebugService.o.d" -o ${OBJECTDIR}/DebugService.o DebugService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TopHSM.o: TopHSM.c  .generated_files/flags/default/1b40be7930cb4fb3a801ca4dd513bac08445f0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TopHSM.o.d 
	@${RM} ${OBJECTDIR}/TopHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/TopHSM.o.d" -o ${OBJECTDIR}/TopHSM.o TopHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/NavigateHSM.o: NavigateHSM.c  .generated_files/flags/default/c9e5bec5b41a2d15355e8df6f8055c8350c4ab02 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/NavigateHSM.o.d 
	@${RM} ${OBJECTDIR}/NavigateHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/NavigateHSM.o.d" -o ${OBJECTDIR}/NavigateHSM.o NavigateHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/CombatHSM.o: CombatHSM.c  .generated_files/flags/default/12f5837a44aa03ce69fd3f4bd9e580639d945256 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/CombatHSM.o.d 
	@${RM} ${OBJECTDIR}/CombatHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/CombatHSM.o.d" -o ${OBJECTDIR}/CombatHSM.o CombatHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/RecoveryHSM.o: RecoveryHSM.c  .generated_files/flags/default/b3d8a8bc11063dcf67ca8368d7c57cc0b7c541cc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/RecoveryHSM.o.d 
	@${RM} ${OBJECTDIR}/RecoveryHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/RecoveryHSM.o.d" -o ${OBJECTDIR}/RecoveryHSM.o RecoveryHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/DriveService.o: DriveService.c  .generated_files/flags/default/c34a9a42a51ccf2ea4a81f05d676b06a05ecede6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/DriveService.o.d 
	@${RM} ${OBJECTDIR}/DriveService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/DriveService.o.d" -o ${OBJECTDIR}/DriveService.o DriveService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TapeService.o: TapeService.c  .generated_files/flags/default/8ffdb811db642df08d9aefb666d0f46aca633602 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TapeService.o.d 
	@${RM} ${OBJECTDIR}/TapeService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/TapeService.o.d" -o ${OBJECTDIR}/TapeService.o TapeService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TrackWireService.o: TrackWireService.c  .generated_files/flags/default/570986324d0e9b987d6479089f44cc047dd9dd65 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TrackWireService.o.d 
	@${RM} ${OBJECTDIR}/TrackWireService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/TrackWireService.o.d" -o ${OBJECTDIR}/TrackWireService.o TrackWireService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/BumperService.o: BumperService.c  .generated_files/flags/default/6fdf7b693e859d70cf1579962600e8faf33369ec .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/BumperService.o.d 
	@${RM} ${OBJECTDIR}/BumperService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/BumperService.o.d" -o ${OBJECTDIR}/BumperService.o BumperService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/BeaconService.o: BeaconService.c  .generated_files/flags/default/a7933e2659caefb180cf28946fe75316a9671510 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/BeaconService.o.d 
	@${RM} ${OBJECTDIR}/BeaconService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/BeaconService.o.d" -o ${OBJECTDIR}/BeaconService.o BeaconService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/LauncherService.o: LauncherService.c  .generated_files/flags/default/86bdc912644e30d24d105178834b5fb8b5f5ce72 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/LauncherService.o.d 
	@${RM} ${OBJECTDIR}/LauncherService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/LauncherService.o.d" -o ${OBJECTDIR}/LauncherService.o LauncherService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/BatteryService.o: BatteryService.c  .generated_files/flags/default/da4e64618ccdeb627bb5d0a34b308c03ff69bff6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/BatteryService.o.d 
	@${RM} ${OBJECTDIR}/BatteryService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/BatteryService.o.d" -o ${OBJECTDIR}/BatteryService.o BatteryService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/AD.o: ECE118/src/AD.c  .generated_files/flags/default/325e229d8889d96a9fe11e3e02bf94a7199e72e5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/AD.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/AD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/AD.o.d" -o ${OBJECTDIR}/ECE118/src/AD.o ECE118/src/AD.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/BOARD.o: ECE118/src/BOARD.c  .generated_files/flags/default/268d09367c32f7a2b6ca1ecd94ab0e1dfa2666e5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/BOARD.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/BOARD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/BOARD.o.d" -o ${OBJECTDIR}/ECE118/src/BOARD.o ECE118/src/BOARD.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/ES_CheckEvents.o: ECE118/src/ES_CheckEvents.c  .generated_files/flags/default/faba959bfe4385e45535f539717aea885bf94678 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_CheckEvents.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_CheckEvents.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/ES_CheckEvents.o.d" -o ${OBJECTDIR}/ECE118/src/ES_CheckEvents.o ECE118/src/ES_CheckEvents.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/ES_Framework.o: ECE118/src/ES_Framework.c  .generated_files/flags/default/ce19afde3da322638453e3572c44182787db6267 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_Framework.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_Framework.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/ES_Framework.o.d" -o ${OBJECTDIR}/ECE118/src/ES_Framework.o ECE118/src/ES_Framework.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/ES_KeyboardInput.o: ECE118/src/ES_KeyboardInput.c  .generated_files/flags/default/afe184462b0d357f2e3cd6e25b6925b58f71f9bd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_KeyboardInput.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_KeyboardInput.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/ES_KeyboardInput.o.d" -o ${OBJECTDIR}/ECE118/src/ES_KeyboardInput.o ECE118/src/ES_KeyboardInput.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/ES_PostList.o: ECE118/src/ES_PostList.c  .generated_files/flags/default/46e93b04985d4542c1744591f2f1379eca6ffd1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_PostList.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_PostList.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/ES_PostList.o.d" -o ${OBJECTDIR}/ECE118/src/ES_PostList.o ECE118/src/ES_PostList.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/ES_Queue.o: ECE118/src/ES_Queue.c  .generated_files/flags/default/8b48b8e7fad59644ea5c2b1d50f2a08d6f2bd26e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_Queue.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_Queue.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/ES_Queue.o.d" -o ${OBJECTDIR}/ECE118/src/ES_Queue.o ECE118/src/ES_Queue.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/ES_TattleTale.o: ECE118/src/ES_TattleTale.c  .generated_files/flags/default/6251dde1222b54e2a2f706a8b027a60338ae69c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_TattleTale.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_TattleTale.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/ES_TattleTale.o.d" -o ${OBJECTDIR}/ECE118/src/ES_TattleTale.o ECE118/src/ES_TattleTale.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/ES_Timers.o: ECE118/src/ES_Timers.c  .generated_files/flags/default/9cbff8dd0d4e30d6297b7c06f00dec387287009d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_Timers.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/ES_Timers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/ES_Timers.o.d" -o ${OBJECTDIR}/ECE118/src/ES_Timers.o ECE118/src/ES_Timers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/IO_Ports.o: ECE118/src/IO_Ports.c  .generated_files/flags/default/55612f45d3043ebbcfc5e3583f3b8070106d1eb0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/IO_Ports.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/IO_Ports.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/IO_Ports.o.d" -o ${OBJECTDIR}/ECE118/src/IO_Ports.o ECE118/src/IO_Ports.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/LED.o: ECE118/src/LED.c  .generated_files/flags/default/961d1fbaa6deb87fbbc8ad9a2d6d0e4d0b085a41 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/LED.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/LED.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/LED.o.d" -o ${OBJECTDIR}/ECE118/src/LED.o ECE118/src/LED.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/pwm.o: ECE118/src/pwm.c  .generated_files/flags/default/dc4d51b465ad0ca443d6a30f7c40aaf88f46cf52 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/pwm.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/pwm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/pwm.o.d" -o ${OBJECTDIR}/ECE118/src/pwm.o ECE118/src/pwm.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/RC_Servo.o: ECE118/src/RC_Servo.c  .generated_files/flags/default/12f43badf24f0d864fce964839c35a8701b939ab .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/RC_Servo.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/RC_Servo.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/RC_Servo.o.d" -o ${OBJECTDIR}/ECE118/src/RC_Servo.o ECE118/src/RC_Servo.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/serial.o: ECE118/src/serial.c  .generated_files/flags/default/74afcfbbc05e4e27fd1653aaf106a5fe1d58a9cd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/serial.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/serial.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -I"." -I"ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ECE118/src/serial.o.d" -o ${OBJECTDIR}/ECE118/src/serial.o ECE118/src/serial.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/final.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ECE118/bootloader320.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -D__MPLAB_DEBUGGER_SIMULATOR=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/final.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)      -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_SIMULATOR=1,--defsym=_min_heap_size=10,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	
else
${DISTDIR}/final.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ECE118/bootloader320.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/final.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=10,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	${MP_CC_DIR}/xc32-bin2hex ${DISTDIR}/final.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
