#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
include Makefile

# Environment
# Adding MPLAB X bin directory to path
PATH:=/opt/microchip/mplabx/mplab_ide/mplab_ide/modules/../../bin/:$(PATH)
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/DesignGroupProject.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/DesignGroupProject.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/newmain.p1 ${OBJECTDIR}/lcd.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/newmain.p1.d ${OBJECTDIR}/lcd.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/newmain.p1 ${OBJECTDIR}/lcd.p1


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

# Path to java used to run MPLAB X when this makefile was created
MP_JAVA_PATH="/usr/lib/jvm/java-6-openjdk/jre/bin/"
OS_CURRENT="$(shell uname -s)"
############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
MP_CC="/usr/hitech/picc/9.82/bin/picc"
# MP_BC is not defined
MP_AS="/usr/hitech/picc/9.82/bin/picc"
MP_LD="/usr/hitech/picc/9.82/bin/picc"
MP_AR="/usr/hitech/picc/9.82/bin/picc"
DEP_GEN=${MP_JAVA_PATH}java -jar "/opt/microchip/mplabx/mplab_ide/mplab_ide/modules/../../bin/extractobjectdependencies.jar" 
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps
MP_CC_DIR="/usr/hitech/picc/9.82/bin"
# MP_BC_DIR is not defined
MP_AS_DIR="/usr/hitech/picc/9.82/bin"
MP_LD_DIR="/usr/hitech/picc/9.82/bin"
MP_AR_DIR="/usr/hitech/picc/9.82/bin"
# MP_BC_DIR is not defined

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/DesignGroupProject.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=16F917
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/newmain.p1: newmain.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	${MP_CC} --pass1 newmain.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=default,+asm,-asmfile,-speed,+space,-debug,9 -D__DEBUG --debugger=icd3  --double=24 --float=24 --addrqual=ignore --mode=lite -g --asmlist "--errformat=%f:%l: error: %s" "--msgformat=%f:%l: advisory: %s" "--warnformat=%f:%l warning: %s"
	@${MP_CC} --scandep  newmain.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --opt=default,+asm,-asmfile,-speed,+space,-debug,9 -D__DEBUG --debugger=icd3  --double=24 --float=24 --addrqual=ignore --mode=lite -g --asmlist "--errformat=%f:%l: error: %s" "--msgformat=%f:%l: advisory: %s" "--warnformat=%f:%l warning: %s"
	@echo ${OBJECTDIR}/newmain.p1: > ${OBJECTDIR}/newmain.p1.d
	@cat ${OBJECTDIR}/newmain.dep >> ${OBJECTDIR}/newmain.p1.d
	@${FIXDEPS} "${OBJECTDIR}/newmain.p1.d" $(SILENT) -ht 
	
${OBJECTDIR}/lcd.p1: lcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	${MP_CC} --pass1 lcd.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=default,+asm,-asmfile,-speed,+space,-debug,9 -D__DEBUG --debugger=icd3  --double=24 --float=24 --addrqual=ignore --mode=lite -g --asmlist "--errformat=%f:%l: error: %s" "--msgformat=%f:%l: advisory: %s" "--warnformat=%f:%l warning: %s"
	@${MP_CC} --scandep  lcd.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --opt=default,+asm,-asmfile,-speed,+space,-debug,9 -D__DEBUG --debugger=icd3  --double=24 --float=24 --addrqual=ignore --mode=lite -g --asmlist "--errformat=%f:%l: error: %s" "--msgformat=%f:%l: advisory: %s" "--warnformat=%f:%l warning: %s"
	@echo ${OBJECTDIR}/lcd.p1: > ${OBJECTDIR}/lcd.p1.d
	@cat ${OBJECTDIR}/lcd.dep >> ${OBJECTDIR}/lcd.p1.d
	@${FIXDEPS} "${OBJECTDIR}/lcd.p1.d" $(SILENT) -ht 
	
else
${OBJECTDIR}/newmain.p1: newmain.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	${MP_CC} --pass1 newmain.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=default,+asm,-asmfile,-speed,+space,-debug,9  --double=24 --float=24 --addrqual=ignore --mode=lite -g --asmlist "--errformat=%f:%l: error: %s" "--msgformat=%f:%l: advisory: %s" "--warnformat=%f:%l warning: %s"
	@${MP_CC} --scandep  newmain.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --opt=default,+asm,-asmfile,-speed,+space,-debug,9  --double=24 --float=24 --addrqual=ignore --mode=lite -g --asmlist "--errformat=%f:%l: error: %s" "--msgformat=%f:%l: advisory: %s" "--warnformat=%f:%l warning: %s"
	@echo ${OBJECTDIR}/newmain.p1: > ${OBJECTDIR}/newmain.p1.d
	@cat ${OBJECTDIR}/newmain.dep >> ${OBJECTDIR}/newmain.p1.d
	@${FIXDEPS} "${OBJECTDIR}/newmain.p1.d" $(SILENT) -ht 
	
${OBJECTDIR}/lcd.p1: lcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	${MP_CC} --pass1 lcd.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=default,+asm,-asmfile,-speed,+space,-debug,9  --double=24 --float=24 --addrqual=ignore --mode=lite -g --asmlist "--errformat=%f:%l: error: %s" "--msgformat=%f:%l: advisory: %s" "--warnformat=%f:%l warning: %s"
	@${MP_CC} --scandep  lcd.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --opt=default,+asm,-asmfile,-speed,+space,-debug,9  --double=24 --float=24 --addrqual=ignore --mode=lite -g --asmlist "--errformat=%f:%l: error: %s" "--msgformat=%f:%l: advisory: %s" "--warnformat=%f:%l warning: %s"
	@echo ${OBJECTDIR}/lcd.p1: > ${OBJECTDIR}/lcd.p1.d
	@cat ${OBJECTDIR}/lcd.dep >> ${OBJECTDIR}/lcd.p1.d
	@${FIXDEPS} "${OBJECTDIR}/lcd.p1.d" $(SILENT) -ht 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/DesignGroupProject.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) -odist/${CND_CONF}/${IMAGE_TYPE}/DesignGroupProject.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  -mdist/${CND_CONF}/${IMAGE_TYPE}/DesignGroupProject.X.${IMAGE_TYPE}.map --summary=default,-psect,-class,+mem,-hex --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=default,+asm,-asmfile,-speed,+space,-debug,9 -D__DEBUG --debugger=icd3 -N31 --warn=0  --double=24 --float=24 --addrqual=ignore --mode=lite --output=default,-inhx032 -g --asmlist "--errformat=%f:%l: error: %s" "--msgformat=%f:%l: advisory: %s" "--warnformat=%f:%l warning: %s" ${OBJECTFILES_QUOTED_IF_SPACED}  
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/DesignGroupProject.X.${IMAGE_TYPE}.hex
else
dist/${CND_CONF}/${IMAGE_TYPE}/DesignGroupProject.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) -odist/${CND_CONF}/${IMAGE_TYPE}/DesignGroupProject.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -mdist/${CND_CONF}/${IMAGE_TYPE}/DesignGroupProject.X.${IMAGE_TYPE}.map --summary=default,-psect,-class,+mem,-hex --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=default,+asm,-asmfile,-speed,+space,-debug,9 -N31 --warn=0  --double=24 --float=24 --addrqual=ignore --mode=lite --output=default,-inhx032 -g --asmlist "--errformat=%f:%l: error: %s" "--msgformat=%f:%l: advisory: %s" "--warnformat=%f:%l warning: %s" ${OBJECTFILES_QUOTED_IF_SPACED}  
endif


# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "/opt/microchip/mplabx/mplab_ide/mplab_ide/modules/../../bin/"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
