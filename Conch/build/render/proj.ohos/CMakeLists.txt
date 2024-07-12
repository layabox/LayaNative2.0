cmake_minimum_required(VERSION 3.6)
project(render)
set(render_src
    ../../../source/render/RenderEx/JCGpuProgram.cpp
    ../../../source/render/RenderEx/JCGlobalValue.cpp
    ../../../source/render/RenderEx/JCRegister.cpp
    ../../../source/render/Image/JCImage.cpp
    ../../../source/render/Image/JCVideo.cpp
    ../../../source/render/Image/JCImageManager.cpp
    ../../../source/render/Performance/JCPerfHUD.cpp
    ../../../source/render/Performance/JCVBOManager.cpp
    ../../../source/render/LayaGL/JCIDGenerator.cpp
    ../../../source/render/LayaGL/JCLayaGL.cpp
    ../../../source/render/LayaGL/JCLayaGLDispatch.cpp
)
add_library(render STATIC ${render_src})

target_include_directories(render PUBLIC
    ../../../../ThirdParty/curl/include/ohos
    ../../../../ThirdParty/png/include/ohos
    ../../../../ThirdParty/jpeg-turbo/include/ohos
    ../../../../ThirdParty/freetype/include/ohos
    ../../../../ThirdParty/zip/include/ohos
    ../../../include/common
    ../../../include/webglPlus
    ../../../include/conch
)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11")

target_compile_options(${PROJECT_NAME} PRIVATE -DV8_COMPRESS_POINTERS)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -DV8_COMPRESS_POINTERS")