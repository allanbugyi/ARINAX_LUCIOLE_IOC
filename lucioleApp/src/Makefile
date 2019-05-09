TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS BELOW HERE

# Build the IOC application
PROD_IOC = luciole

DBD += luciole.dbd

luciole_DBD += base.dbd
luciole_DBD += asyn.dbd
luciole_DBD += drvAsynIPPort.dbd
luciole_DBD += luciole_functions.dbd

# Compile and add the code to the support library
lucioleSupport_SRCS += lucioledrv.cpp

# System libraries
luciole_SYS_LIBS    += Luciole
luciole_SYS_LIBS    += tinyDiaFrame

luciole_LIBS	    += asyn

luciole_SRCS += luciole_registerRecordDeviceDriver.cpp

# Build the main IOC entry point where needed
luciole_SRCS_DEFAULT += lucioleMain.cpp
luciole_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#luciole_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

lucioleSupport_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD EXTRA GNUMAKE RULES BELOW HERE

