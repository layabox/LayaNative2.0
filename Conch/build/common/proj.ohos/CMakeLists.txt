cmake_minimum_required(VERSION 3.6)
project(common)
set(common_src
    ../../../source/common/buffer/JCBuffer.cpp
    ../../../source/common/downloadCache/JCFileTable.cpp
    ../../../source/common/downloadCache/JCServerFileCache.cpp
    ../../../source/common/downloadMgr/JCCurlWrap.cpp
    ../../../source/common/downloadMgr/JCHttpHeader.cpp
    ../../../source/common/downloadMgr/JCDownloadMgr.cpp
    ../../../source/common/manager/JCArrayBufferManager.cpp
    ../../../source/common/event/JCEmitter.cpp
    ../../../source/common/event/JCEventBase.cpp
    ../../../source/common/fileSystem/JCFileSystem.cpp
    ../../../source/common/fontMgr/JCFreeTypeRender.cpp
    ../../../source/common/imageLib/JCImageRW.cpp
    ../../../source/common/imageLib/JCJpegImg.cpp
    ../../../source/common/imageLib/JCPngImg.cpp
    ../../../source/common/imageLib/JCGifImg.cpp
    ../../../source/common/math/Matrix32.cpp
    ../../../source/common/misc/JCGetClockExact.cpp
    ../../../source/common/misc/JCLayaThreadPool.cpp
    ../../../source/common/misc/JCWorkerThread.cpp
    ../../../source/common/misc/JCWorkSemaphore.cpp
    ../../../source/common/util/JCColor.cpp
    ../../../source/common/util/JCCommonMethed.cpp
    ../../../source/common/util/JCCrypto.cpp
    ../../../source/common/util/JCJson.cpp
    ../../../source/common/util/JCIniFile.cpp
    ../../../source/common/util/JCLayaUrl.cpp
    ../../../source/common/util/JCMemorySurvey.cpp
    ../../../source/common/util/JCXml.cpp
    ../../../source/common/util/JCZipFile.cpp
    ../../../source/common/util/JCZlib.cpp
    ../../../source/common/util/Log.cpp
    ../../../source/common/util/JCFlog.cpp
    ../../../source/common/math/JCBoundingBox.cpp
    ../../../source/common/math/JCBoundingFrustum.cpp
    ../../../source/common/math/JCBoundingSphere.cpp
    ../../../source/common/math/JCPlane.cpp
    ../../../source/common/webglplus/JCWebGLPlus.cpp
    ../../../source/common/resource/Audio/JCAudioWavplayer.cpp
    ../../../source/common/resource/Audio/JCOggParser.cpp
    ../../../source/common/resource/Audio/JCWaveInfo.cpp
    ../../../source/common/resource/Audio/JCWaveParser.cpp
    ../../../source/common/fontMgr/JCFontInfo.cpp
    ../../../source/common/fontMgr/JCFontManager.cpp
    ../../../source/common/resource/JCFileResManager.cpp
    ../../../source/common/resource/JCResManager.cpp
    ../../../source/common/resource/JCResource.cpp
)
add_library(common STATIC ${common_src})

target_include_directories(common PUBLIC
    ../../../../ThirdParty/curl/include/ohos
    ../../../../ThirdParty/png/include/ohos
    ../../../../ThirdParty/jpeg-turbo/include/ohos
    ../../../../ThirdParty/freetype/include/ohos
    ../../../../ThirdParty/zip/include/ohos
    ../../../../ThirdParty/ogg/include/ohos
    ../../../../ThirdParty/zlib/include/ohos
    ../../../../ThirdParty/openal/include/ohos
    ../../../../Conch/source
)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11")
target_compile_options(common PRIVATE -DV8_COMPRESS_POINTERS)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -DV8_COMPRESS_POINTERS")
target_compile_options(common PRIVATE -O3)