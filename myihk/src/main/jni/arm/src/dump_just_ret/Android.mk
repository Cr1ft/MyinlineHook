LOCAL_PATH := $(call my-dir)  


include $(CLEAR_VARS)

$(warning "abi: $(TARGET_ARCH_ABI)")

#ifeq "$(TARGET_ARCH_ABI)" "armeabi-v7a"
ifneq ($(findstring $(TARGET_ARCH_ABI), "armeabi-v7a"  "armeabi"),)

#LOCAL_CXXFLAGS +=  -g -O0
#LOCAL_ARM_MODE := arm
LOCAL_MODULE    			:= dump_just_ret
LOCAL_C_INCLUDES 			:= $(LOCAL_PATH)/../../../include $(LOCAL_PATH)/../../asm
LOCAL_SRC_FILES 			:= ../mhk.c dump.c ../fixPCOpcode.c ../../asm/dump_just_ret.s


include $(BUILD_STATIC_LIBRARY)

endif