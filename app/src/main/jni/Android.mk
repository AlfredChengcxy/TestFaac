##################################

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := vfaac

LOCAL_C_INCLUDES := $(LOCAL_PATH)/.
LOCAL_C_INCLUDES += $(NDK_PATH)/platforms/$(TARGET_PLATFORM)/arch-$(TARGET_ARCH)/usr/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/libfaac
LOCAL_C_INCLUDES += $(LOCAL_PATH)/libfaac/kiss_fft

#jni
LOCAL_SRC_FILES += AudioFaac.c \
                   util.c \
                   javamethod.c

#libfaac
LOCAL_SRC_FILES += libfaac/aacquant.c \
                   libfaac/backpred.c \
                   libfaac/bitstream.c \
                   libfaac/channels.c \
                   libfaac/fft.c \
                   libfaac/filtbank.c \
                   libfaac/frame.c \
                   libfaac/huffman.c \
                   libfaac/ltp.c \
                   libfaac/midside.c \
                   libfaac/psychkni.c \
                   libfaac/tns.c \
                   libfaac/util.c

#libfaac/kiss_fft
LOCAL_SRC_FILES += libfaac/kiss_fft/kiss_fft.c \
                   libfaac/kiss_fft/kiss_fftr.c



#支持for循环什么的需要这个标准
LOCAL_CFLAGS += -std=c99
LOCAL_CFLAGS += -O3

LOCAL_LDFLAGS := -O3

LOCAL_LDLIBS := -llog


LOCAL_ARM_NEON := true

include $(BUILD_SHARED_LIBRARY)

