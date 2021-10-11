
LOCAL_PATH:= $(call my-dir)

#libcommon.a
include $(CLEAR_VARS)
LOCAL_MODULE := libcommon
LOCAL_CFLAGS := \
  -fexceptions \
  -Wno-multichar \
  -DANDROID \
  -DIN_LIBRARY \
  -D_GLIBCXX_PERMIT_BACKWARD_HASH \
  -DANDROID_ARM_NEON=TRUE \
  -DANDROID_STL=c++_shared \
  -DANDROID_TOOLCHAIN=clang \
  -frtti

LOCAL_CPPFLAGS += -std=c++11

ifeq ($(APP_PERFTEST),1)
LOCAL_CFLAGS += -DPERFTEST 
endif

ifeq ($(TARGET_ARCH),arm64)
   LOCAL_CFLAGS += -DV8_COMPRESS_POINTERS
   LOCAL_CPPFLAGS+= -DV8_COMPRESS_POINTERS
endif

LOCAL_CXXFLAGS := -O3

LOCAL_SRC_FILES := \
        ../../../../source/common/buffer/JCBuffer.cpp \
        ../../../../source/common/downloadCache/JCFileTable.cpp \
        ../../../../source/common/downloadCache/JCServerFileCache.cpp \
        ../../../../source/common/downloadMgr/JCCurlWrap.cpp \
        ../../../../source/common/downloadMgr/JCHttpHeader.cpp \
        ../../../../source/common/downloadMgr/JCDownloadMgr.cpp \
		../../../../source/common/manager/JCArrayBufferManager.cpp \
        ../../../../source/common/event/JCEmitter.cpp \
        ../../../../source/common/event/JCEventBase.cpp \
        ../../../../source/common/fileSystem/JCFileSystem.cpp \
        ../../../../source/common/fontMgr/JCFreeTypeRender.cpp \
        ../../../../source/common/imageLib/JCImageRW.cpp \
        ../../../../source/common/imageLib/JCJpegImg.cpp \
        ../../../../source/common/imageLib/JCPngImg.cpp \
        ../../../../source/common/imageLib/JCGifImg.cpp \
		../../../../source/common/math/Matrix32.cpp \
        ../../../../source/common/misc/JCGetClockExact.cpp \
        ../../../../source/common/misc/JCLayaThreadPool.cpp \
        ../../../../source/common/misc/JCWorkerThread.cpp \
        ../../../../source/common/misc/JCWorkSemaphore.cpp \
        ../../../../source/common/util/JCColor.cpp \
        ../../../../source/common/util/JCCommonMethed.cpp \
        ../../../../source/common/util/JCCrypto.cpp \
        ../../../../source/common/util/JCJson.cpp \
        ../../../../source/common/util/JCIniFile.cpp \
        ../../../../source/common/util/JCLayaUrl.cpp \
        ../../../../source/common/util/JCMemorySurvey.cpp \
        ../../../../source/common/util/JCXml.cpp \
        ../../../../source/common/util/JCZipFile.cpp \
        ../../../../source/common/util/JCZlib.cpp \
        ../../../../source/common/util/Log.cpp \
        ../../../../source/common/util/JCFlog.cpp \
		../../../../source/common/math/JCBoundingBox.cpp \
		../../../../source/common/math/JCBoundingFrustum.cpp \
		../../../../source/common/math/JCBoundingSphere.cpp \
		../../../../source/common/math/JCPlane.cpp \
		../../../../source/common/webglplus/JCWebGLPlus.cpp \
        ../../../../source/common/resource/Audio/JCAudioWavplayer.cpp \
        ../../../../source/common/resource/Audio/JCOggParser.cpp \
        ../../../../source/common/resource/Audio/JCWaveInfo.cpp \
        ../../../../source/common/resource/Audio/JCWaveParser.cpp \
        ../../../../source/common/fontMgr/JCFontInfo.cpp \
        ../../../../source/common/fontMgr/JCFontManager.cpp \
        ../../../../source/common/resource/JCFileResManager.cpp \
        ../../../../source/common/resource/JCResManager.cpp \
        ../../../../source/common/resource/JCResource.cpp \
        ../../../../source/common/OpenAL/OpenAL32/alAuxEffectSlot.c \
        ../../../../source/common/OpenAL/OpenAL32/alBuffer.c        \
        ../../../../source/common/OpenAL/OpenAL32/alDatabuffer.c    \
        ../../../../source/common/OpenAL/OpenAL32/alEffect.c        \
        ../../../../source/common/OpenAL/OpenAL32/alError.c         \
        ../../../../source/common/OpenAL/OpenAL32/alExtension.c     \
        ../../../../source/common/OpenAL/OpenAL32/alFilter.c        \
        ../../../../source/common/OpenAL/OpenAL32/alListener.c      \
        ../../../../source/common/OpenAL/OpenAL32/alSource.c        \
        ../../../../source/common/OpenAL/OpenAL32/alState.c         \
        ../../../../source/common/OpenAL/OpenAL32/alThunk.c         \
        ../../../../source/common/OpenAL/Alc/ALc.c                  \
        ../../../../source/common/OpenAL/Alc/alcConfig.c            \
        ../../../../source/common/OpenAL/Alc/alcEcho.c              \
        ../../../../source/common/OpenAL/Alc/alcModulator.c         \
        ../../../../source/common/OpenAL/Alc/alcReverb.c            \
        ../../../../source/common/OpenAL/Alc/alcRing.c              \
        ../../../../source/common/OpenAL/Alc/alcThread.c            \
        ../../../../source/common/OpenAL/Alc/ALu.c                  \
        ../../../../source/common/OpenAL/Alc/android.c              \
        ../../../../source/common/OpenAL/Alc/bs2b.c                 \
        ../../../../source/common/OpenAL/Alc/null.c                 \


LOCAL_C_INCLUDES := ../../../../../ThirdParty/curl/include/android \
        ../../../../../ThirdParty/png/include/android \
        ../../../../../ThirdParty/jpeg/include/android \
        ../../../../../ThirdParty/freetype/include/android \
        ../../../../../ThirdParty/zip/include/android \
        ../../../../../ThirdParty/ogg/include/android \
		../../../../../ThirdParty/zlib/include/android \
        ../../../../source/common/OpenAL/include \
        ../../../../source/common/OpenAL/OpenAL32/Include \
        ../../../../source/common/glm \

#LOCAL_IS64:=32
#LOCAL_LDLIBS    := -llog -lz -landroid			\
#		../../../../libs/android$(LOCAL_IS64)/libcurl.a \
#		../../../../libs/android$(LOCAL_IS64)/libgnustl_static.a \
#		../../../../libs/android$(LOCAL_IS64)/libjpeg.a \
#		../../../../libs/android$(LOCAL_IS64)/libpng.a \
#		../../../../libs/android$(LOCAL_IS64)/libfreetype.a \
#		../../../../libs/android$(LOCAL_IS64)/libzip.a \
#		../../../../libs/android$(LOCAL_IS64)/libogg.a \
#		../../../../libs/android$(LOCAL_IS64)/libvorbis.a \
#		../../../../libs/android$(LOCAL_IS64)/libvorbis-jni.a \


include $(BUILD_STATIC_LIBRARY)

