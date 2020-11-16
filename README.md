# LayanNative2.0 is an open-source LayaAir2.0 engine runtime

LayaNative is a complete set of development solutions for the development, testing and release of native mobile apps by LayaAir engine, but not limited to LayaAir engine. Based on LayaPlayer as the core runtime, LayaNative uses reflection mechanisms and channel docking solutions to provide developers with secondary opening and channel docking on native apps, and provides testers and construction tools to package and release html5 projects for developers Provide convenience as native App.

## Build

- Windows

1. Install Visual Studio 2015
2. Open Conch/build/conch/proj.win32/Conch6.sln
3. Change the configuation to Release/x86
4. Build the solution and then run the project

- iOS

1. Install Xcode
2. Open Conch/build/conch/proj.ios/conch6.xcworkspace
3. Set the active scheme to LayaBox
4. Build and then run the current scheme

- Android

1. On Mac or Linux, Install android studio and configure the Android SDK related environment variables 
2. cd Conch/build and then run ./buildAll-andoid.sh
3. Open Conch/build/conc/proj.android_studio with android_studio
4. Build and then run proj.android_studio-app
