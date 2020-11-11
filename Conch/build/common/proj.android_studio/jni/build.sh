#!/bin/sh
ndk-build NDK_PROJECT_PATH=. NDK_APP_APPLICATION_MK=./Application.mk APP_BUILD_SCRIPT=./Android.mk APP_ABI=armeabi-v7a obj/local/armeabi-v7a/libcommon.a LOCAL_ARM_MODE=arm
#LOCAL_ARM_NEON=true ARCH_ARM_HAVE_NEON=true
