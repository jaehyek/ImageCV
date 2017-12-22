LOCAL_PATH := $(call my-dir)
PROJECT_ROOT:= $(call my-dir)/../../../..

include $(CLEAR_VARS)

OPENCV_INSTALL_MODULES:=on
#OPENCV_LIB_TYPE:=STATIC
 
OPENCVROOT:= $(PROJECT_ROOT)/../OpenCV-android-sdk
include $(OPENCVROOT)/sdk/native/jni/OpenCV.mk


LOCAL_MODULE    := T2PASTE
LOCAL_SRC_FILES := T2PASTE.cpp guidedfilter.cpp globalmatting.cpp jpgexifinfo.cpp

LOCAL_LDLIBS    += -lm -llog -landroid -ljnigraphics
LOCAL_CPPFLAGS := -std=c++11 -pthread -frtti -fexceptions -marm -mfloat-abi=softfp 
TARGET_ARCH := arm
LOCAL_ARM_MODE := arm
LOCAL_ARM_NEON := true

#ifeq ($(TARGET_ARCH_ABI),$(filter $(TARGET_ARCH_ABI), armeabi-v7a x86))
#    LOCAL_CFLAGS := -DHAVE_NEON=1
#ifeq ($(TARGET_ARCH_ABI),x86)
#    LOCAL_CFLAGS += -mssse3
#endif
#endif
#ifeq ( $(TARGET_ARCH_ABI), armeabi-v7a)
	LOCAL_CFLAGS := -DHAVE_NEON=1
#endif
#LOCAL_STATIC_LIBRARIES := cpufeatures

#LOCAL_CFLAGS += -mhard-float -D_NDK_MATH_NO_SOFTFP=1
#LOCAL_LDFLAGS += -Wl,--no-warn-mismatch -lm_hard

#OpenMP
LOCAL_CFLAGS += -fopenmp
LOCAL_LDFLAGS += -fopenmp
include $(BUILD_SHARED_LIBRARY)
#$(call import-module,cpufeatures)
