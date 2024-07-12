cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 14)

project(layaair)

set(LAYA_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../..)

set(OHOS true)
add_definitions(-DOHOS)
add_definitions(-DJS_V8)
add_definitions(-fexceptions)
add_definitions(-DIN_LIBRARY)
add_definitions(-D_GLIBCXX_PERMIT_BACKWARD_HASH)
add_definitions(-DOHOS_ARM_NEON=TRUE)
add_definitions(-DOHOS_STL=c++_shared)
add_definitions(-DOHOS_TOOLCHAIN=clang)
add_definitions(-fno-rtti)
add_definitions(-DBT_USE_DOUBLE_PRECISION)
add_definitions(-DAL_ALEXT_PROTOTYPES)

add_subdirectory(../../common/proj.ohos common)
add_subdirectory(../../render/proj.ohos render)

set(${PROJECT_NAME}_src
    ../../../source/conch/JSWrapper/v8debug/debug-agent.cpp
    ../../../source/conch/JSWrapper/v8debug/V8WSSv.cpp
    ../../../source/conch/JSWrapper/JSInterface/JSInterface.cpp
    ../../../source/conch/JSWrapper/JSInterface/V8/JSEnv.cpp
    ../../../source/conch/JSWrapper/JSInterface/V8/JsBinder.cpp
    ../../../source/conch/JSWrapper/JSInterface/V8/JSArrayBuffer.cpp
    ../../../source/conch/JSWrapper/JSInterface/V8/IsolateData.cpp
    ../../../source/conch/JSWrapper/JSInterface/JSObjBase.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSArrayBufferRef.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSOhosEditBox.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSAppCache.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSAudio.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSConchConfig.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSConsole.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSDOMParser.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSFile.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSFileReader.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSFileSystem.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSGlobalExportCFun.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSHistory.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSImage.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSInput.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSLayaGL.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSNotify.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSRuntime.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSTextMemoryCanvas.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSWebSocket.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSXmlAttr.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSXmlNode.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSZip.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSShaderActiveInfo.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSTextBitmapInfo.cpp
    ../../../source/conch/JSWrapper/LayaWrap/XMLHttpRequest.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSShaderPrecisionFormat.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSCallbackFuncObj.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSWebGLPlus.cpp
    ../../../source/conch/JSWrapper/LayaWrap/JSPromiseRejectionEvent.cpp
    ../../../source/conch/Audio/ohos/JCAudioMp3Player.cpp
    ../../../source/conch/Audio/JCAudioManager.cpp
    ../../../source/conch/WebSocket/WebSocket.cpp
    ../../../source/conch/WebSocket/Uri.cpp
    ../../../source/conch/JCConch.cpp
    ../../../source/conch/JCConchRender.cpp
    ../../../source/conch/JCThreadCmdMgr.cpp
    ../../../source/conch/JCScriptRuntime.cpp
    ../../../source/conch/JCSystemConfig.cpp
    ../../../source/conch/JSWrapper/LayaWrap/Bullet/LayaBulletExport.cpp
    ../../../source/conch/JSWrapper/LayaWrap/Bullet/LayaBulletExportImp.cpp
    ../../../source/conch/JSWrapper/LayaWrap/Video/JSVideo_CommonImpl.cpp
    ../../../source/conch/JSWrapper/LayaWrap/Video/JSVideo_OHOSImpl.cpp
    ../../../source/conch/JSWrapper/LayaWrap/Video/VideoCache.cpp
    ../../../source/conch/Bridge/JCConchBridge.cpp
    
    ../../../source/napi/helper/NapiHelper.cpp
    ../../../source/napi/plugin_manager.cpp
    ../../../source/napi/WorkerMessageQueue.cpp
    ../../../source/napi/render/plugin_render.cpp
    ../../../source/napi/render/egl_core.cpp
    ../../../source/napi/modules/TouchesNapi.cpp
    ../../../source/napi/modules/VideoPlayerNapi.cpp
)
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_src})

include_directories(${LAYA_ROOT_PATH}/Conch/source)
include_directories(${LAYA_ROOT_PATH}/Conch/source/common)
include_directories(${LAYA_ROOT_PATH}/Conch/source/napi)
include_directories(${LAYA_ROOT_PATH}/Conch/source/resource)

add_subdirectory(${LAYA_ROOT_PATH}/ThirdParty thirdParty)
#target_link_libraries(${PROJECT_NAME} thirdParty)

find_library(EGL-lib EGL)
find_library(GLESv3-lib GLESv3)
find_library(hilog-lib hilog_ndk.z)
find_library(libace-lib ace_ndk.z)
find_library(libnapi-lib ace_napi.z)
find_library(libuv-lib uv)
find_library(rawfile-lib rawfile.z)
find_library(Drawing-lib native_drawing)

set(OHOS_ARM64_MACROS
    V8_31BIT_SMIS_ON_64BIT_ARCH
    V8_DEPRECATION_WARNINGS
    V8_IMMINENT_DEPRECATION_WARNINGS
    V8_TARGET_ARCH_ARM64
    V8_HAVE_TARGET_OS
    V8_TARGET_OS_LINUX
    V8_COMPRESS_POINTERS
)

set_property(TARGET v8_monolith
    APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS ${OHOS_ARM64_MACROS}
)

target_link_libraries(${PROJECT_NAME} ${EGL-lib} ${GLESv3-lib} ${hilog-lib} ${libuv-lib} ${Drawing-lib} ${libace-lib} ${libnapi-lib} ${rawfile-lib}
    libohaudio.so libavplayer.so common render v8_monolith thirdParty) 

set(LOCAL_IS64 arm64)

set(LOCAL_DISABLE_FATAL_LINKER_WARNINGS true)

target_include_directories(${PROJECT_NAME} PUBLIC
    ../../../include/common
    ../../../include/render
    ../../../include/webglPlus
    ../../../include/msgpack/include
)
target_compile_options(${PROJECT_NAME} PRIVATE -O3)

target_compile_options(${PROJECT_NAME} PRIVATE -DV8_COMPRESS_POINTERS)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -DV8_COMPRESS_POINTERS")