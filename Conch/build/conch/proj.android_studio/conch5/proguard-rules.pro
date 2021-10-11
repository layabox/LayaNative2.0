-libraryjars 'C:\Users\lvfulong\AppData\Local\Android\Sdk\platforms\android-22\android.jar'

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
-keep class layaair.game.config.config {*;}
-keep class layaair.game.browser.LayaVideoPlayer {*;}
-dontwarn layaair.game.browser.LayaVideoPlayer

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
