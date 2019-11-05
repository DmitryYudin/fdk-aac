LOCAL_PATH:= $(call my-dir)

aacdec_sources := $(sort $(wildcard $(LOCAL_PATH)/libAACdec/src/*.cpp))
aacdec_sources := $(aacdec_sources:$(LOCAL_PATH)/libAACdec/src/%=%)

aacenc_sources := $(sort $(wildcard $(LOCAL_PATH)/libAACenc/src/*.cpp))
aacenc_sources := $(aacenc_sources:$(LOCAL_PATH)/libAACenc/src/%=%)

pcmutils_sources := $(sort $(wildcard $(LOCAL_PATH)/libPCMutils/src/*.cpp))
pcmutils_sources := $(pcmutils_sources:$(LOCAL_PATH)/libPCMutils/src/%=%)

fdk_sources := $(sort $(wildcard $(LOCAL_PATH)/libFDK/src/*.cpp))
fdk_sources := $(fdk_sources:$(LOCAL_PATH)/libFDK/src/%=%)

sys_sources := $(sort $(wildcard $(LOCAL_PATH)/libSYS/src/*.cpp))
sys_sources := $(sys_sources:$(LOCAL_PATH)/libSYS/src/%=%)

mpegtpdec_sources := $(sort $(wildcard $(LOCAL_PATH)/libMpegTPDec/src/*.cpp))
mpegtpdec_sources := $(mpegtpdec_sources:$(LOCAL_PATH)/libMpegTPDec/src/%=%)

mpegtpenc_sources := $(sort $(wildcard $(LOCAL_PATH)/libMpegTPEnc/src/*.cpp))
mpegtpenc_sources := $(mpegtpenc_sources:$(LOCAL_PATH)/libMpegTPEnc/src/%=%)

sbrdec_sources := $(sort $(wildcard $(LOCAL_PATH)/libSBRdec/src/*.cpp))
sbrdec_sources := $(sbrdec_sources:$(LOCAL_PATH)/libSBRdec/src/%=%)

sbrenc_sources := $(sort $(wildcard $(LOCAL_PATH)/libSBRenc/src/*.cpp))
sbrenc_sources := $(sbrenc_sources:$(LOCAL_PATH)/libSBRenc/src/%=%)

arithcoding_sources := $(sort $(wildcard $(LOCAL_PATH)/libArithCoding/src/*.cpp))
arithcoding_sources := $(arithcoding_sources:$(LOCAL_PATH)/libArithCoding/src/%=%)

drcdec_sources := $(sort $(wildcard $(LOCAL_PATH)/libDRCdec/src/*.cpp))
drcdec_sources := $(drcdec_sources:$(LOCAL_PATH)/libDRCdec/src/%=%)

sacdec_sources := $(sort $(wildcard $(LOCAL_PATH)/libSACdec/src/*.cpp))
sacdec_sources := $(sacdec_sources:$(LOCAL_PATH)/libSACdec/src/%=%)

sacenc_sources := $(sort $(wildcard $(LOCAL_PATH)/libSACenc/src/*.cpp))
sacenc_sources := $(sacenc_sources:$(LOCAL_PATH)/libSACenc/src/%=%)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
        $(aacdec_sources:%=libAACdec/src/%) \
        $(aacenc_sources:%=libAACenc/src/%) \
        $(pcmutils_sources:%=libPCMutils/src/%) \
        $(fdk_sources:%=libFDK/src/%) \
        $(sys_sources:%=libSYS/src/%) \
        $(mpegtpdec_sources:%=libMpegTPDec/src/%) \
        $(mpegtpenc_sources:%=libMpegTPEnc/src/%) \
        $(sbrdec_sources:%=libSBRdec/src/%) \
        $(sbrenc_sources:%=libSBRenc/src/%) \
        $(arithcoding_sources:%=libArithCoding/src/%) \
        $(drcdec_sources:%=libDRCdec/src/%) \
        $(sacdec_sources:%=libSACdec/src/%) \
        $(sacenc_sources:%=libSACenc/src/%)

LOCAL_CFLAGS += \
       -Werror \
       -Wno-constant-conversion \
       "-Wno-\#warnings" \
       -Wuninitialized \
       -Wno-self-assign \
       -Wno-implicit-fallthrough

LOCAL_C_INCLUDES := \
        $(LOCAL_PATH)/libAACdec/include \
        $(LOCAL_PATH)/libAACenc/include \
        $(LOCAL_PATH)/libPCMutils/include \
        $(LOCAL_PATH)/libFDK/include \
        $(LOCAL_PATH)/libSYS/include \
        $(LOCAL_PATH)/libMpegTPDec/include \
        $(LOCAL_PATH)/libMpegTPEnc/include \
        $(LOCAL_PATH)/libSBRdec/include \
        $(LOCAL_PATH)/libSBRenc/include \
        $(LOCAL_PATH)/libArithCoding/include \
        $(LOCAL_PATH)/libDRCdec/include \
        $(LOCAL_PATH)/libSACdec/include \
        $(LOCAL_PATH)/libSACenc/include

LOCAL_C_INCLUDES += \
        $(LOCAL_PATH)/android/liblog/inlcude 

LOCAL_EXPORT_C_INCLUDES := \
        $(LOCAL_PATH)/libAACenc/include \
        $(LOCAL_PATH)/libAACdec/include \
        $(LOCAL_PATH)/libSYS/include

LOCAL_EXPORT_LDLIBS := -llog

LOCAL_MODULE := libFraunhoferAAC

include $(BUILD_STATIC_LIBRARY)

ifeq (1,$(strip $(BUILD_TEST_libFraunhoferAAC)))
    aac-enc_sources := aac-enc.c wavreader.c
    aac-dec_sources := aac-dec.c wavwriter.c
    test-encode-decode_sources := test-encode-decode.c wavreader.c
    m4a-dec_sources := m4a-dec.c wavwriter.c
    m4a-enc_sources := m4a-enc.c wavreader.c

    include $(CLEAR_VARS)
    LOCAL_MODULE := aac-enc
    LOCAL_SRC_FILES := $(aac-enc_sources)
    LOCAL_STATIC_LIBRARIES := libFraunhoferAAC
    include $(BUILD_EXECUTABLE)

    include $(CLEAR_VARS)
    LOCAL_MODULE := aac-dec
    LOCAL_SRC_FILES := $(aac-dec_sources)
    LOCAL_STATIC_LIBRARIES := libFraunhoferAAC
    include $(BUILD_EXECUTABLE)

    include $(CLEAR_VARS)
    LOCAL_MODULE := test-encode-decode
    LOCAL_SRC_FILES := $(test-encode-decode_sources)
    LOCAL_STATIC_LIBRARIES := libFraunhoferAAC
    include $(BUILD_EXECUTABLE)

  ifeq (1,1) # ffmpeg dependant apps
    include $(CLEAR_VARS)
    LOCAL_MODULE := m4a-dec
    LOCAL_SRC_FILES := $(m4a-dec_sources)
    LOCAL_STATIC_LIBRARIES := libFraunhoferAAC libavformat libavcodec libavutil
    include $(BUILD_EXECUTABLE)

    include $(CLEAR_VARS)
    LOCAL_MODULE := m4a-enc
    LOCAL_SRC_FILES := $(m4a-enc_sources)
    LOCAL_STATIC_LIBRARIES := libFraunhoferAAC libavformat libavcodec libavutil
    include $(BUILD_EXECUTABLE)

    $(call import-add-path,$(LOCAL_PATH))
    $(call import-module,../ffmpeg-builder)
  endif
endif
