LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)



include $(CLEAR_VARS)

LOCAL_PACKAGE_NAME := HelloActivity1
LOCAL_CERTIFICATE := platform
LOCAL_PRIVILEGED_MODULE := true
LOCAL_MODULE_TAGS := optional
LOCAL_USE_AAPT2 := true

LOCAL_SRC_FILES := $(call all-java-files-under, src)






include $(BUILD_PACKAGE)


