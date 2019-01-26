# Copyright (C) 2011 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

#variables for NFC_NXP_CHIP_TYPE
PN547C2 := 1
PN548C2 := 2
PN551   := 3
PN553   := 4
NQ110 := $PN547C2
NQ120 := $PN547C2
NQ210 := $PN548C2
NQ220 := $PN548C2
#NXP PN547 Enable
ifeq ($(PN547C2),1)
D_CFLAGS += -DPN547C2=1
endif
ifeq ($(PN548C2),2)
D_CFLAGS += -DPN548C2=2
endif
ifeq ($(PN551),3)
D_CFLAGS += -DPN551=3
endif
ifeq ($(PN553),4)
D_CFLAGS += -DPN553=4
endif

#### Select the CHIP ####
ifneq ($(filter pn547 nq110 nq120,$(BOARD_NFC_CHIPSET)),)
NXP_CHIP_TYPE := $(PN547C2)
else ifneq ($(filter pn548 nq210 nq220,$(BOARD_NFC_CHIPSET)),)
NXP_CHIP_TYPE := $(PN548C2)
else ifeq ($(BOARD_NFC_CHIPSET),pn551)
NXP_CHIP_TYPE := $(PN551)
else ifeq ($(BOARD_NFC_CHIPSET),pn553)
NXP_CHIP_TYPE := $(PN553)
endif

ifeq ($(NXP_CHIP_TYPE),$(PN547C2))
D_CFLAGS += -DNFC_NXP_CHIP_TYPE=PN547C2
else ifeq ($(NXP_CHIP_TYPE),$(PN548C2))
D_CFLAGS += -DNFC_NXP_CHIP_TYPE=PN548C2
else ifeq ($(NXP_CHIP_TYPE),$(PN551))
D_CFLAGS += -DNFC_NXP_CHIP_TYPE=PN551
else ifeq ($(NXP_CHIP_TYPE),$(PN553))
D_CFLAGS += -DNFC_NXP_CHIP_TYPE=PN553
else
D_CFLAGS += -DNFC_NXP_CHIP_TYPE=PN553
endif

ifneq ($(BOARD_NFC_HAL_SUFFIX),)
    HAL_SUFFIX := $(BOARD_NFC_HAL_SUFFIX)
else
    HAL_SUFFIX := $(TARGET_DEVICE)
endif

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := nfc_nci.$(HAL_SUFFIX)
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_SRC_FILES := \
    $(call all-c-files-under, common) \
    $(call all-c-files-under, dnld) \
    $(call all-c-files-under, hal) \
    $(call all-c-files-under, log) \
    $(call all-c-files-under, self-test) \
    $(call all-c-files-under, tml) \
    $(call all-c-files-under, utils) \
    $(call all-cpp-files-under, utils) \
    nfc_nci.c

LOCAL_SHARED_LIBRARIES := liblog libcutils libdl libhardware

LOCAL_CFLAGS := $(D_CFLAGS)
LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/utils \
    $(LOCAL_PATH)/inc \
    $(LOCAL_PATH)/common \
    $(LOCAL_PATH)/dnld \
    $(LOCAL_PATH)/hal \
    $(LOCAL_PATH)/log \
    $(LOCAL_PATH)/tml \
    $(LOCAL_PATH)/self-test \

LOCAL_CFLAGS += -DANDROID \
        -DNXP_UICC_ENABLE -DNXP_HW_SELF_TEST
LOCAL_CFLAGS += -DNFC_NXP_HFO_SETTINGS=FALSE
#LOCAL_CFLAGS += -DFELICA_CLT_ENABLE


#
# nfc_nci.$(HAL_SUFFIX)
#
include $(BUILD_SHARED_LIBRARY)
