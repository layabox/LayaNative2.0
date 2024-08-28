import { Dialog } from '../components/dialog/DialogWorker'
import { JumpManager } from '../system/appJump/JumpManager'
import { DeviceUtils } from '../system/device/DeviceUtils'
import { NetworkUtils } from '../system/device/NetworkUtils'
import ApplicationManager from '../system/application/ApplicationManager'
import { WebView } from '../components/webview/WebView'
import { VideoPlayer }  from '../components/videoplayer/VideoPlayer'
import Accelerometer from '../system/sensor/AccelerometerUtils'
import Orientation from '../system/sensor/OrientationUtils'
import SoundUtils from '../system/sound/SoundUtils'
import laya from "liblaya.so"
import { EditBox } from '../components/EditBox'
import { HandleMessageUtils } from '../Event/HandleMessageUtils'

export class NapiHelper {
  // 需要将JS代码执行在worker线程的在此处注册方法
  static registerWorkerFunctions() {
    NapiHelper.registerLayaWorkerFunc();
    NapiHelper.registerSound();
    NapiHelper.registerWebView();
  }

  // 需要将JS代码执行在UI线程的在此处注册方法
  static registerUIFunctions() {
    NapiHelper.registerEditBox();
    NapiHelper.registerVideoPlay();
    laya.JSBind.bindFunction("DeviceUtils.getScreenInch", DeviceUtils.getScreenInch);
    laya.JSBind.bindFunction('DeviceUtils.setKeepScreenOn', DeviceUtils.setKeepScreenOn);
    laya.JSBind.bindFunction('DeviceUtils.setPreferredOrientation', DeviceUtils.setPreferredOrientation);
    laya.JSBind.bindFunction("ApplicationManager.exit", ApplicationManager.exit);
    laya.JSBind.bindFunction("HandleMessageUtils.handleMessage", HandleMessageUtils.handleMessage);
    laya.JSBind.bindFunction("HandleMessageUtils.handleSyncMessage", HandleMessageUtils.handleSyncMessage);
  }

  private static registerLayaWorkerFunc() {
    console.log("register getAvalidMem");
    laya.JSBind.bindFunction("DeviceUtils.startVibration", DeviceUtils.startVibration);
    laya.JSBind.bindFunction("DeviceUtils.getDeviceInfo", DeviceUtils.getDeviceInfo);
    laya.JSBind.bindFunction("ApplicationManager.getApplicationInfo", ApplicationManager.getApplicationInfo);
    laya.JSBind.bindFunction("DeviceUtils.getPrivateDirty", DeviceUtils.getPrivateDirty);
    laya.JSBind.bindFunction("DeviceUtils.getAvalidMem", DeviceUtils.getAvalidMem);
    laya.JSBind.bindFunction('Dialog.showDialog', Dialog.showDialog);
    laya.JSBind.bindFunction("NetworkUtils.getNetworkType", NetworkUtils.getNetworkType);
    laya.JSBind.bindFunction("Accelerometer.enable", Accelerometer.enable);
    laya.JSBind.bindFunction("Accelerometer.disable", Accelerometer.disable);
    laya.JSBind.bindFunction("Orientation.enable", Orientation.enable);
    laya.JSBind.bindFunction("Orientation.disable", Orientation.disable);
    laya.JSBind.bindFunction("HandleMessageUtils.postCmdToMain", HandleMessageUtils.postCmdToMain);
  }

  private static registerVideoPlay() {
    laya.JSBind.bindFunction('VideoPlayer.createVideoPlayer', VideoPlayer.createVideoPlayer);
    laya.JSBind.bindFunction('VideoPlayer.removeVideoPlayer', VideoPlayer.removeVideoPlayer);
    laya.JSBind.bindFunction('VideoPlayer.setURL', VideoPlayer.setURL);
    laya.JSBind.bindFunction('VideoPlayer.play', VideoPlayer.play);
    laya.JSBind.bindFunction('VideoPlayer.pause', VideoPlayer.pause);
    laya.JSBind.bindFunction('VideoPlayer.stop', VideoPlayer.stop);
    laya.JSBind.bindFunction('VideoPlayer.setLooping', VideoPlayer.setLooping);

    laya.JSBind.bindFunction('VideoPlayer.setAutoplay', VideoPlayer.setAutoplay);
    laya.JSBind.bindFunction('VideoPlayer.setCurrentTime', VideoPlayer.setCurrentTime);

    laya.JSBind.bindFunction('VideoPlayer.getVolume', VideoPlayer.getVolume);
    laya.JSBind.bindFunction('VideoPlayer.setVolume', VideoPlayer.setVolume);

    // setVideoPlayerRect
    laya.JSBind.bindFunction('VideoPlayer.setX', VideoPlayer.setX);
    laya.JSBind.bindFunction('VideoPlayer.setY', VideoPlayer.setY);
    laya.JSBind.bindFunction('VideoPlayer.setWidth', VideoPlayer.setWidth);
    laya.JSBind.bindFunction('VideoPlayer.setHeight', VideoPlayer.setHeight);
  }

  private static registerSound() {
    laya.JSBind.bindFunction('SoundUtils.playBackgroundMusic', SoundUtils.playBackgroundMusic);
    laya.JSBind.bindFunction('SoundUtils.pauseBackgroundMusic', SoundUtils.pauseBackgroundMusic);
    laya.JSBind.bindFunction('SoundUtils.stopBackgroundMusic', SoundUtils.stopBackgroundMusic);
    laya.JSBind.bindFunction('SoundUtils.resumeBackgroundMusic', SoundUtils.resumeBackgroundMusic);
    laya.JSBind.bindFunction('SoundUtils.setBackgroundMusicVolume', SoundUtils.setBackgroundMusicVolume);
    laya.JSBind.bindFunction('SoundUtils.setCurrentTime', SoundUtils.setCurrentTime);
    laya.JSBind.bindFunction('SoundUtils.getCurrentTime', SoundUtils.getCurrentTime);
    laya.JSBind.bindFunction('SoundUtils.getDuration', SoundUtils.getDuration);
  }

  private static registerEditBox() {
    laya.JSBind.bindFunction('EditBox.create', EditBox.create);
    laya.JSBind.bindFunction('EditBox.remove', EditBox.remove);
    laya.JSBind.bindFunction('EditBox.setPos', EditBox.setPos);
    laya.JSBind.bindFunction('EditBox.setSize', EditBox.setSize);
    laya.JSBind.bindFunction('EditBox.setOpacity', EditBox.setOpacity);
    laya.JSBind.bindFunction('EditBox.setValue', EditBox.setValue);
    laya.JSBind.bindFunction('EditBox.getValue', EditBox.getValue);
    laya.JSBind.bindFunction('EditBox.setStyle', EditBox.setStyle);
    laya.JSBind.bindFunction('EditBox.setVisible', EditBox.setVisible);
    laya.JSBind.bindFunction('EditBox.focus', EditBox.focus);
    laya.JSBind.bindFunction('EditBox.setColor', EditBox.setColor);
    laya.JSBind.bindFunction('EditBox.setFontSize', EditBox.setFontSize);
    laya.JSBind.bindFunction('EditBox.setCursorPosition', EditBox.setCursorPosition);
    laya.JSBind.bindFunction('EditBox.setMaxLength', EditBox.setMaxLength);
    laya.JSBind.bindFunction('EditBox.setType', EditBox.setType);
    laya.JSBind.bindFunction('EditBox.setRegular', EditBox.setRegular);
    laya.JSBind.bindFunction('EditBox.setMultiAble', EditBox.setMultiAble);
    laya.JSBind.bindFunction('EditBox.setForbidEdit', EditBox.setForbidEdit);
  }

  private static registerWebView() {
    laya.JSBind.bindFunction('WebUtils.createWebview', WebView.createWebview);
    laya.JSBind.bindFunction('WebUtils.closeWebview', WebView.closeWebview);
    laya.JSBind.bindFunction('WebUtils.callWebviewJS', WebView.callWebviewJS);
    laya.JSBind.bindFunction('WebUtils.hideWebview', WebView.hideWebview);
    laya.JSBind.bindFunction('WebUtils.showWebView', WebView.showWebView);
  }
}


