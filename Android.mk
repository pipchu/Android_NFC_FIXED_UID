LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE           := uidctrl
LOCAL_SRC_FILES        := main.cpp
LOCAL_SRC_FILES        += xHook/libxhook/jni/xhook.c \
                          xHook/libxhook/jni/xh_core.c \
                          xHook/libxhook/jni/xh_elf.c \
                          xHook/libxhook/jni/xh_jni.c \
                          xHook/libxhook/jni/xh_log.c \
                          xHook/libxhook/jni/xh_util.c \
                          xHook/libxhook/jni/xh_version.c

LOCAL_C_INCLUDES       += $(LOCAL_PATH)/android_system_nfc/halimpl/bcm2079x/include
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/android_system_nfc/halimpl/bcm2079x/gki/ulinux
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/android_system_nfc/halimpl/bcm2079x/gki/common
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/android_system_nfc/halimpl/bcm2079x/adaptation
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/android_system_nfc/src/hal/include
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/android_system_nfc/src/hal/int
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/android_system_nfc/src/nfc/include
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/android_system_nfc/src/nfa/include
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/android_system_nfc/src/nfa/int
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/android_system_nfc/src/udrv/include
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/android_system_nfc/src/include
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/xHook/libxhook/jni/

LOCAL_CFLAGS           := -Wno-unused-parameter -fno-rtti -fno-exceptions -Wno-deprecated-register \
                          -fpermissive

LOCAL_LDLIBS           := -llog

include $(BUILD_SHARED_LIBRARY)

# For Injector Module
include $(LOCAL_PATH)/TinyInjector/Android.mk
