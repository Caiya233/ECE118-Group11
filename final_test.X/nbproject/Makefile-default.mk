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
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/final_test.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/final_test.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=main.c TestShell.c DebugPrint.c DriveTest.c LauncherTest.c ServoTest.c SensorTest.c BatteryTest.c ECE118/src/AD.c ECE118/src/BOARD.c ECE118/src/IO_Ports.c ECE118/src/pwm.c ECE118/src/RC_Servo.c ECE118/src/serial.c ECE118/src/timers.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/main.o ${OBJECTDIR}/TestShell.o ${OBJECTDIR}/DebugPrint.o ${OBJECTDIR}/DriveTest.o ${OBJECTDIR}/LauncherTest.o ${OBJECTDIR}/ServoTest.o ${OBJECTDIR}/SensorTest.o ${OBJECTDIR}/BatteryTest.o ${OBJECTDIR}/ECE118/src/AD.o ${OBJECTDIR}/ECE118/src/BOARD.o ${OBJECTDIR}/ECE118/src/IO_Ports.o ${OBJECTDIR}/ECE118/src/pwm.o ${OBJECTDIR}/ECE118/src/RC_Servo.o ${OBJECTDIR}/ECE118/src/serial.o ${OBJECTDIR}/ECE118/src/timers.o
POSSIBLE_DEPFILES=${OBJECTDIR}/main.o.d ${OBJECTDIR}/TestShell.o.d ${OBJECTDIR}/DebugPrint.o.d ${OBJECTDIR}/DriveTest.o.d ${OBJECTDIR}/LauncherTest.o.d ${OBJECTDIR}/ServoTest.o.d ${OBJECTDIR}/SensorTest.o.d ${OBJECTDIR}/BatteryTest.o.d ${OBJECTDIR}/ECE118/src/AD.o.d ${OBJECTDIR}/ECE118/src/BOARD.o.d ${OBJECTDIR}/ECE118/src/IO_Ports.o.d ${OBJECTDIR}/ECE118/src/pwm.o.d ${OBJECTDIR}/ECE118/src/RC_Servo.o.d ${OBJECTDIR}/ECE118/src/serial.o.d ${OBJECTDIR}/ECE118/src/timers.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/main.o ${OBJECTDIR}/TestShell.o ${OBJECTDIR}/DebugPrint.o ${OBJECTDIR}/DriveTest.o ${OBJECTDIR}/LauncherTest.o ${OBJECTDIR}/ServoTest.o ${OBJECTDIR}/SensorTest.o ${OBJECTDIR}/BatteryTest.o ${OBJECTDIR}/ECE118/src/AD.o ${OBJECTDIR}/ECE118/src/BOARD.o ${OBJECTDIR}/ECE118/src/IO_Ports.o ${OBJECTDIR}/ECE118/src/pwm.o ${OBJECTDIR}/ECE118/src/RC_Servo.o ${OBJECTDIR}/ECE118/src/serial.o ${OBJECTDIR}/ECE118/src/timers.o

# Source Files
SOURCEFILES=main.c TestShell.c DebugPrint.c DriveTest.c LauncherTest.c ServoTest.c SensorTest.c BatteryTest.c ECE118/src/AD.c ECE118/src/BOARD.c ECE118/src/IO_Ports.c ECE118/src/pwm.c ECE118/src/RC_Servo.c ECE118/src/serial.c ECE118/src/timers.c



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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/final_test.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

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
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/14dd2dbe52549e86bc23125779f187a1a64bcda3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TestShell.o: TestShell.c  .generated_files/flags/default/d826f4d4b7e330711cfee7e955be90e7c0920a14 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TestShell.o.d 
	@${RM} ${OBJECTDIR}/TestShell.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/TestShell.o.d" -o ${OBJECTDIR}/TestShell.o TestShell.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/DebugPrint.o: DebugPrint.c  .generated_files/flags/default/62737a48b50f69027fbef8646b48d15d0092023d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/DebugPrint.o.d 
	@${RM} ${OBJECTDIR}/DebugPrint.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/DebugPrint.o.d" -o ${OBJECTDIR}/DebugPrint.o DebugPrint.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/DriveTest.o: DriveTest.c  .generated_files/flags/default/b15181e2c41388fb8b58f800f70840d932e958a4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/DriveTest.o.d 
	@${RM} ${OBJECTDIR}/DriveTest.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/DriveTest.o.d" -o ${OBJECTDIR}/DriveTest.o DriveTest.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/LauncherTest.o: LauncherTest.c  .generated_files/flags/default/bfcbebec38d91c3090b4d0ebef3874801e6ca5d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/LauncherTest.o.d 
	@${RM} ${OBJECTDIR}/LauncherTest.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/LauncherTest.o.d" -o ${OBJECTDIR}/LauncherTest.o LauncherTest.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ServoTest.o: ServoTest.c  .generated_files/flags/default/fefefc989b57771489cb865ba5d89e34be616e87 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ServoTest.o.d 
	@${RM} ${OBJECTDIR}/ServoTest.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/ServoTest.o.d" -o ${OBJECTDIR}/ServoTest.o ServoTest.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/SensorTest.o: SensorTest.c  .generated_files/flags/default/6b6a505057ad5f1f68ece66354cfe089e6ccf669 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SensorTest.o.d 
	@${RM} ${OBJECTDIR}/SensorTest.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/SensorTest.o.d" -o ${OBJECTDIR}/SensorTest.o SensorTest.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/BatteryTest.o: BatteryTest.c  .generated_files/flags/default/25418dc5ec09ecd7a94c5a990c38b58b17a02326 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/BatteryTest.o.d 
	@${RM} ${OBJECTDIR}/BatteryTest.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/BatteryTest.o.d" -o ${OBJECTDIR}/BatteryTest.o BatteryTest.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/AD.o: ECE118/src/AD.c  .generated_files/flags/default/7433c49d9afcef11ccb40b25c7b9137de3f70736 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/AD.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/AD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/ECE118/src/AD.o.d" -o ${OBJECTDIR}/ECE118/src/AD.o ECE118/src/AD.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/BOARD.o: ECE118/src/BOARD.c  .generated_files/flags/default/66c3384baaaa0eee3c745542330cf061f360f89c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/BOARD.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/BOARD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/ECE118/src/BOARD.o.d" -o ${OBJECTDIR}/ECE118/src/BOARD.o ECE118/src/BOARD.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/IO_Ports.o: ECE118/src/IO_Ports.c  .generated_files/flags/default/540077084bbddfbb8cf51ad4b3468f3e9c043fde .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/IO_Ports.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/IO_Ports.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/ECE118/src/IO_Ports.o.d" -o ${OBJECTDIR}/ECE118/src/IO_Ports.o ECE118/src/IO_Ports.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/pwm.o: ECE118/src/pwm.c  .generated_files/flags/default/92d66b50d6c97ff1d98860f7fec82633ea5510d8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/pwm.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/pwm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/ECE118/src/pwm.o.d" -o ${OBJECTDIR}/ECE118/src/pwm.o ECE118/src/pwm.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/RC_Servo.o: ECE118/src/RC_Servo.c  .generated_files/flags/default/709eb4d478b995aa982a7fc6c19f448493b16bcc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/RC_Servo.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/RC_Servo.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/ECE118/src/RC_Servo.o.d" -o ${OBJECTDIR}/ECE118/src/RC_Servo.o ECE118/src/RC_Servo.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/serial.o: ECE118/src/serial.c  .generated_files/flags/default/129df0acc638fed4286f1059f2b113e20f464382 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/serial.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/serial.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/ECE118/src/serial.o.d" -o ${OBJECTDIR}/ECE118/src/serial.o ECE118/src/serial.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/timers.o: ECE118/src/timers.c  .generated_files/flags/default/43e57b22a9796f1267064a43d86a319d4512115b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/timers.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/timers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/ECE118/src/timers.o.d" -o ${OBJECTDIR}/ECE118/src/timers.o ECE118/src/timers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/2cf931f16bb0a6531bd05a6cd2c7a51d2a3de5f0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TestShell.o: TestShell.c  .generated_files/flags/default/82ff133879d9b7976cad818c9955de9f8f7ae4e4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TestShell.o.d 
	@${RM} ${OBJECTDIR}/TestShell.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/TestShell.o.d" -o ${OBJECTDIR}/TestShell.o TestShell.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/DebugPrint.o: DebugPrint.c  .generated_files/flags/default/3a3c77194b5a1e2ea8af78c6270e3e1feda463a7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/DebugPrint.o.d 
	@${RM} ${OBJECTDIR}/DebugPrint.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/DebugPrint.o.d" -o ${OBJECTDIR}/DebugPrint.o DebugPrint.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/DriveTest.o: DriveTest.c  .generated_files/flags/default/545ae7deb39f4763931f757856cc80113ba8cf81 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/DriveTest.o.d 
	@${RM} ${OBJECTDIR}/DriveTest.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/DriveTest.o.d" -o ${OBJECTDIR}/DriveTest.o DriveTest.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/LauncherTest.o: LauncherTest.c  .generated_files/flags/default/f85814695cdb650c203d9b156ad54db13c4c59e4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/LauncherTest.o.d 
	@${RM} ${OBJECTDIR}/LauncherTest.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/LauncherTest.o.d" -o ${OBJECTDIR}/LauncherTest.o LauncherTest.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ServoTest.o: ServoTest.c  .generated_files/flags/default/81b0e4ddd1ade9d9dccf87c1b8f05a6b65aef8c4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ServoTest.o.d 
	@${RM} ${OBJECTDIR}/ServoTest.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/ServoTest.o.d" -o ${OBJECTDIR}/ServoTest.o ServoTest.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/SensorTest.o: SensorTest.c  .generated_files/flags/default/d011f96ea2c5c7b26419aa616344d73709838990 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SensorTest.o.d 
	@${RM} ${OBJECTDIR}/SensorTest.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/SensorTest.o.d" -o ${OBJECTDIR}/SensorTest.o SensorTest.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/BatteryTest.o: BatteryTest.c  .generated_files/flags/default/2fcada6e3e4fa59186b816d29facb34fa89b433a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/BatteryTest.o.d 
	@${RM} ${OBJECTDIR}/BatteryTest.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/BatteryTest.o.d" -o ${OBJECTDIR}/BatteryTest.o BatteryTest.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/AD.o: ECE118/src/AD.c  .generated_files/flags/default/8cbbc722cc8a945464f926e1c15de66cfa26cccc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/AD.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/AD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/ECE118/src/AD.o.d" -o ${OBJECTDIR}/ECE118/src/AD.o ECE118/src/AD.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/BOARD.o: ECE118/src/BOARD.c  .generated_files/flags/default/5b943ef8174d8ce8bac0c74a6f4dc524982c8129 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/BOARD.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/BOARD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/ECE118/src/BOARD.o.d" -o ${OBJECTDIR}/ECE118/src/BOARD.o ECE118/src/BOARD.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/IO_Ports.o: ECE118/src/IO_Ports.c  .generated_files/flags/default/756c0f13e20f3005052178df9365dc10a792af3f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/IO_Ports.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/IO_Ports.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/ECE118/src/IO_Ports.o.d" -o ${OBJECTDIR}/ECE118/src/IO_Ports.o ECE118/src/IO_Ports.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/pwm.o: ECE118/src/pwm.c  .generated_files/flags/default/4b1db0fe3ea17a11a4d9fdf8f9b4a7ca894e0aeb .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/pwm.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/pwm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/ECE118/src/pwm.o.d" -o ${OBJECTDIR}/ECE118/src/pwm.o ECE118/src/pwm.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/RC_Servo.o: ECE118/src/RC_Servo.c  .generated_files/flags/default/f7cbc1fc9888437a4e7fe4b2958508e60d735793 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/RC_Servo.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/RC_Servo.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/ECE118/src/RC_Servo.o.d" -o ${OBJECTDIR}/ECE118/src/RC_Servo.o ECE118/src/RC_Servo.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/serial.o: ECE118/src/serial.c  .generated_files/flags/default/e4eeb4048af3eef591187ab97bc71cc2238c4dcf .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/serial.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/serial.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/ECE118/src/serial.o.d" -o ${OBJECTDIR}/ECE118/src/serial.o ECE118/src/serial.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ECE118/src/timers.o: ECE118/src/timers.c  .generated_files/flags/default/af35f0288499a7ee6c67856274f9cf002e74946a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/ECE118/src" 
	@${RM} ${OBJECTDIR}/ECE118/src/timers.o.d 
	@${RM} ${OBJECTDIR}/ECE118/src/timers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O0 -fno-common -MP -MMD -MF "${OBJECTDIR}/ECE118/src/timers.o.d" -o ${OBJECTDIR}/ECE118/src/timers.o ECE118/src/timers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/final_test.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ECE118/bootloader320.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -D__MPLAB_DEBUGGER_SIMULATOR=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/final_test.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)      -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_SIMULATOR=1,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	
else
${DISTDIR}/final_test.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ECE118/bootloader320.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/final_test.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	${MP_CC_DIR}/xc32-bin2hex ${DISTDIR}/final_test.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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
