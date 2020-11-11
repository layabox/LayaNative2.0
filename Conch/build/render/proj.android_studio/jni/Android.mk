
LOCAL_PATH:= $(call my-dir)

#librender.a
include $(CLEAR_VARS)
LOCAL_MODULE := librender
LOCAL_CFLAGS := \
  -fexceptions \
  -Wno-multichar \
  -DANDROID \
  -DIN_LIBRARY \
  -D_GLIBCXX_PERMIT_BACKWARD_HASH \
  -frtti

LOCAL_CFLAGS += -std=c++11

ifeq ($(APP_PERFTEST),1)
LOCAL_CFLAGS += -DPERFTEST 
endif

LOCAL_CXXFLAGS := -O3

LOCAL_SRC_FILES := \
        ../../../../source/render/RenderEx/JCGpuProgram.cpp \
		../../../../source/render/RenderEx/JCGlobalValue.cpp \
		../../../../source/render/RenderEx/JCRegister.cpp \
        ../../../../source/render/Image/JCImage.cpp \
		../../../../source/render/Image/JCVideo.cpp \
        ../../../../source/render/Image/JCImageManager.cpp \
        ../../../../source/render/Performance/JCPerfHUD.cpp \
        ../../../../source/render/Performance/JCVBOManager.cpp \
		../../../../source/render/LayaGL/JCIDGenerator.cpp \
		../../../../source/render/LayaGL/JCLayaGL.cpp \
		../../../../source/render/LayaGL/JCLayaGLDispatch.cpp \
		   

LOCAL_C_INCLUDES := ../../../../../ThirdParty/curl/include/android \
        ../../../../../ThirdParty/png/include/android \
        ../../../../../ThirdParty/jpeg/include/android \
        ../../../../../ThirdParty/freetype/include/android \
        ../../../../../ThirdParty/zip/include/android \
        ../../../../include/common \
		../../../../include/webglPlus \
		../../../../include/conch \

include $(BUILD_STATIC_LIBRARY)

