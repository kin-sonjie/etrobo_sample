mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_COBJS +=

APPL_CXXOBJS += \
util.o \
Locator.o \
DataLogger.o \
StateMachine.o \
Filter.o \
DataCollector.o \
DriveInfoTable.o \
DriveManager.o \
Driver.o \
DerivedDriver.o \
AccelController.o \
MotorController.o

SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3/Makefile

endif

APPL_DIRS += $(mkfile_path)app \
			 $(mkfile_path)unit \
			 $(mkfile_path)logger \
			 $(mkfile_path)inputs_data_manager \
			 $(mkfile_path)motion_controller


INCLUDES += \
	-I$(mkfile_path)app \
	-I$(mkfile_path)unit \
	-I$(mkfile_path)logger \
	-I$(mkfile_path)inputs_data_manager \
	-I$(mkfile_path)motion_controller

#COPTS += -fno-use-cxa-atexit
