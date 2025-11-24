#!/bin/sh
OHOS_NDK_CMAKE_PATH="F:/Ohayoo-native/huawei/ide6.0.1/DevEcoStudio/sdk/default/openharmony/native/build-tools/cmake/bin"
OHOS_NDK_CMAKE_TOOLCHAIN_PATH="F:/Ohayoo-native/huawei/ide6.0.1/DevEcoStudio/sdk/default/openharmony/native/build/cmake/ohos.toolchain.cmake"

#export PATH=${OHOS_NDK_CMAKE_PATH}:$PATH
mkdir build
cd build
${OHOS_NDK_CMAKE_PATH}/cmake.exe  -G "Ninja" -DCMAKE_BUILD_TYPE=Release  -DOHOS_STL=c++_shared -DCMAKE_TOOLCHAIN_FILE=${OHOS_NDK_CMAKE_TOOLCHAIN_PATH}  ../conch/proj.ohos 
${OHOS_NDK_CMAKE_PATH}/cmake.exe --build .
# 如果需要编译静态库则需要设置 cmake 变量 -DBUILD_SHARED_LIBS=OFF
# 可以通过-DCMAKE_INSTALL_PREFIX=xxx 设置库的安装目录
# 编译 arm32 -DOHOS_ARCH=armeabi-v7a