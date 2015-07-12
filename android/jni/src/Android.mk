LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL
GLM_PATH := ../glm
SRC_PATH := ../../..

LOCAL_CFLAGS := -std=c++11
LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(GLM_PATH)/include \
					$(LOCAL_PATH)/$(SDL_PATH)/include \
					$(LOCAL_PATH)/$(SRC_PATH)/include \
					$(LOCAL_PATH)/$(SRC_PATH)/include/utils


FILE_LIST := $(wildcard $(LOCAL_PATH)/$(SRC_PATH)/src/*.cc) \
				$(wildcard $(LOCAL_PATH)/$(SRC_PATH)/utils/*.cc) \

# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
    $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_SHARED_LIBRARIES := SDL2

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)
