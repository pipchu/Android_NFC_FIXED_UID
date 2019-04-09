MY_LOCAL_PATH := $(call my-dir)

# Build dynamic library
include $(MY_LOCAL_PATH)/UidCtrl/Android.mk

# Build Injector Module
include $(MY_LOCAL_PATH)/TinyInjector/Android.mk