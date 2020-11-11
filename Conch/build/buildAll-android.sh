#!/bin/sh
cd common/proj.android_studio/jni/
ndk-build
cd ../../../

cd webglPlus/proj.android_studio/jni/
ndk-build
cd ../../../

cd render/proj.android_studio/jni/
ndk-build
cd ../../../

cp common/proj.android_studio/obj/local/armeabi-v7a/libcommon.a ../libs/android-armv7/
cp render/proj.android_studio/obj/local/armeabi-v7a/librender.a ../libs/android-armv7/
cp webglPlus/proj.android_studio/obj/local/armeabi-v7a/libwebglPlus.a ../libs/android-armv7/

cp common/proj.android_studio/obj/local/arm64-v8a/libcommon.a ../libs/android-arm64/
cp render/proj.android_studio/obj/local/arm64-v8a/librender.a ../libs/android-arm64/
cp webglPlus/proj.android_studio/obj/local/arm64-v8a/libwebglPlus.a ../libs/android-arm64/

cp common/proj.android_studio/obj/local/x86/libcommon.a ../libs/android-x86/
cp render/proj.android_studio/obj/local/x86/librender.a ../libs/android-x86/
cp webglPlus/proj.android_studio/obj/local/x86/libwebglPlus.a ../libs/android-x86/

#cp common/proj.android_studio/obj/local/armeabi/libcommon.a ../libs/android-arm/
#cp render/proj.android_studio/obj/local/armeabi/librender.a ../libs/android-arm/

touch ../source/conch/JCConch.cpp
cd conch/proj.android_studio/jni/
ndk-build
cd ../../../
cp conch/proj.android_studio/libs/arm64-v8a/liblayaair.so  conch/proj.android_studio/conch5/libs/arm64-v8a/liblayaair.so
cp conch/proj.android_studio/libs/armeabi-v7a/liblayaair.so  conch/proj.android_studio/conch5/libs/armeabi-v7a/liblayaair.so
cp conch/proj.android_studio/libs/x86/liblayaair.so  conch/proj.android_studio/conch5/libs/x86/liblayaair.so

cp conch/proj.android_studio/libs/  conch/proj.android_studio/conch5/ -f -R
