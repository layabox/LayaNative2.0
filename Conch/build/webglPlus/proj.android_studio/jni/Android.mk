
LOCAL_PATH:= $(call my-dir)

#libwebglPlus.a
include $(CLEAR_VARS)
LOCAL_MODULE := libwebglPlus
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
../../../../source/webglPlus/Animation/JCKeyframeNodeList.cpp \
../../../../source/webglPlus/JSObjBase.cpp \
../../../../source/webglPlus/math/JCBoundingBox.cpp \
../../../../source/webglPlus/math/JCBoundingFrustum.cpp \
../../../../source/webglPlus/math/JCBoundingSphere.cpp \
../../../../source/webglPlus/math/JCPlane.cpp \
../../../../source/webglPlus/buffer/JCCommandEncoderBuffer.cpp \
../../../../source/webglPlus/Manager/JCArrayBufferManager.cpp \
../../../../source/webglPlus/JCWebGLPlus.cpp \
../../../../source/webglPlus/v8/JCWebGLPlusV8.cpp \
../../../../source/webglPlus/v8/JSArrayBufferRef.cpp \
../../../../source/webglPlus/v8/JSFloatArrayKeyframe.cpp \
../../../../source/webglPlus/v8/JSFloatKeyframe.cpp \
../../../../source/webglPlus/v8/JSKeyframeNode.cpp \
../../../../source/webglPlus/v8/JSKeyframeNodeList.cpp \
../../../../source/webglPlus/v8/JSWebGLPlus.cpp \
../../../../source/webglPlus/v8/V8Util.cpp \

LOCAL_C_INCLUDES := \
        ../../../../include/common \
		../../../../include/webglPlus \
		../../../../../ThirdParty/v8/include \

include $(BUILD_STATIC_LIBRARY)

