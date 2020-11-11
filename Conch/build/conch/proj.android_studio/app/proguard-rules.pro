# Add project specific ProGuard rules here.
# By default, the flags in this file are appended to flags specified
# in D:\sdk/tools/proguard/proguard-android.txt
# You can edit the include path and order by changing the proguardFiles
# directive in build.gradle.
#
# For more details, see
#   http://developer.android.com/guide/developing/tools/proguard.html

# Add any project specific keep options here:

# If your project uses WebView with JS, uncomment the following
# and specify the fully qualified class name to the JavaScript interface
# class:
#-keepclassmembers class fqcn.of.javascript.interface.for.webview {
#   public *;
#}
-dontusemixedcaseclassnames
-keepattributes *Annotation*

-keep public class * extends android.app.Activity
-keep public class * extends android.app.Application
-keep public class * extends android.app.Service
-keep public class * extends android.content.BroadcastReceiver
-keep public class * extends android.content.ContentProvider
-keep public class * extends android.app.backup.BackupAgentHelper
-keep public class * extends android.preference.Preference
-keep public class com.android.vending.licensing.ILicensingService


-keep class layaair.game.PlatformInterface.** {*;}
-keep class layaair.game.wrapper.** {*;}
-keep class layaair.game.device.DevID {*;}
-keep class layaair.game.browser.ConchJNI{*;}
-keep class layaair.game.browser.ExportJavaFunction {*;}
-keep class layaair.game.utility.ProcessInfo {*;}
-keep class layaair.game.utility.LayaAudioMusic {*;}
-keep class layaair.game.Notifycation.LayaNotifyManager {*;}
-keep class layaair.game.conch.ILayaEventListener {*;}
-keep class layaair.game.conch.ILayaGameEgine {*;}
-keep class layaair.game.conch.LayaConch5 {*;}


-keepclasseswithmembernames class *{
    native <methods>;
}

-keepclasseswithmembers class *{
    public <init>(android.content.Context,android.util.AttributeSet);
}

-keepclasseswithmembers class *{
    public <init>(android.content.Context,android.util.AttributeSet, int);
}

-keepclassmembers class * extends android.app.Activity{
   public void *(android.view.View);
}