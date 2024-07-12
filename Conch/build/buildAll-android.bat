@echo off


set ANDROID_SDK_TOOLS=D:/Program Files/androidStudioSDK/tools
set ANDROID_NDK_ROOT=D:/BaiduNetdiskDownload/android-ndk-r21e-windows-x86_64/android-ndk-r21e

set PATH=%ANDROID_SDK_TOOLS%;%ANDROID_NDK_ROOT%

REM 进入common模块的jni目录并执行ndk-build
cd /d common\proj.android_studio\jni
ndk-build
cd /d ..\..\..\

REM 进入render模块的jni目录并执行ndk-build
cd /d render\proj.android_studio\jni
ndk-build
cd /d ..\..\..\

REM 复制common模块的库文件到指定目录
xcopy common\proj.android_studio\obj\local\armeabi-v7a\libcommon.a ..\libs\android-armv7\ /Y
xcopy render\proj.android_studio\obj\local\armeabi-v7a\librender.a ..\libs\android-armv7\ /Y
REM xcopy webglPlus\proj.android_studio\obj\local\armeabi-v7a\libwebglPlus.a ..\libs\android-armv7\ /Y

REM 复制库文件到android-arm64目录
xcopy common\proj.android_studio\obj\local\arm64-v8a\libcommon.a ..\libs\android-arm64\ /Y
xcopy render\proj.android_studio\obj\local\arm64-v8a\librender.a ..\libs\android-arm64\ /Y
REM xcopy webglPlus\proj.android_studio\obj\local\arm64-v8a\libwebglPlus.a ..\libs\android-arm64\ /Y

REM 复制库文件到android-x86目录
xcopy common\proj.android_studio\obj\local\x86\libcommon.a ..\libs\android-x86\ /Y
xcopy render\proj.android_studio\obj\local\x86\librender.a ..\libs\android-x86\ /Y
REM xcopy webglPlus\proj.android_studio\obj\local\x86\libwebglPlus.a ..\libs\android-x86\ /Y

REM 触碰JCConch.cpp, 触发重建
REM echo. > ..\source\conch\JCConch.cpp

REM 编译conch模块
cd /d conch\proj.android_studio\jni
ndk-build
cd /d ..\..\..\

REM 复制获得的liblayaair.so文件到指定目录
xcopy conch\proj.android_studio\libs\arm64-v8a\liblayaair.so  conch\proj.android_studio\conch5\libs\arm64-v8a\ /Y
xcopy conch\proj.android_studio\libs\armeabi-v7a\liblayaair.so  conch\proj.android_studio\conch5\libs\armeabi-v7a\ /Y
xcopy conch\proj.android_studio\libs\x86\liblayaair.so  conch\proj.android_studio\conch5\libs\x86\ /Y

REM 递归复制libs目录到conch5
xcopy conch\proj.android_studio\libs\  conch\proj.android_studio\conch5\ /E /I /Y