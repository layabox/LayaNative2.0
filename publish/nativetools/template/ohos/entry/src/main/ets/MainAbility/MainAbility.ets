import window from '@ohos.window';
import UIAbility from '@ohos.app.ability.UIAbility';
import web_webview from '@ohos.web.webview';
import laya from "liblaya.so";
import { BusinessError } from '@ohos.base';
import { GlobalContext, GlobalContextConstants, ContextType } from "@ohos/libSysCapabilities"
import buffer from '@ohos.buffer';
import { Want, AbilityConstant } from '@kit.AbilityKit';

const nativeAppLifecycle: laya.CPPFunctions = laya.getContext(ContextType.APP_LIFECYCLE);

interface config {
  CheckNetwork: number;
  ThreadMode: number;
  JSDebugPort: number;
  JSDebugMode: number;
}

export default class MainAbility extends UIAbility {
  onCreate(want: Want, launchParam: AbilityConstant.LaunchParam) {
    nativeAppLifecycle.onCreate();
    GlobalContext.storeGlobalThis(GlobalContextConstants.LAYA_ABILITY_CONTEXT, this.context);
    // Initializes the webView kernel of the system. This parameter is optional if it is not used.
    web_webview.WebviewController.initializeWebEngine();
    GlobalContext.storeGlobalThis(GlobalContextConstants.LAYA_ABILITY_WANT, this.context);
    this.initEngine();
    console.info('[LIFECYCLE-App] onCreate')
  }

  onDestroy() {
    nativeAppLifecycle.onDestroy();
    console.info('[LIFECYCLE-App] onDestroy')
  }

  onWindowStageCreate(windowStage: window.WindowStage) {
    windowStage.loadContent('pages/Index', (err, data) => {
      if (err.code) {
        return;
      }
    });

    windowStage.getMainWindow().then((windowIns: window.Window) => {
      // Set whether to display the status bar and navigation bar. If they are not displayed, [] is displayed.
      let systemBarPromise = windowIns.setWindowSystemBarEnable([]);
      // Whether the window layout is displayed in full screen mode
      let fullScreenPromise = windowIns.setWindowLayoutFullScreen(true);
      // Sets whether the screen is always on.
      let keepScreenOnPromise = windowIns.setWindowKeepScreenOn(true);
      Promise.all([systemBarPromise, fullScreenPromise, keepScreenOnPromise]).then(() => {
        console.info('Succeeded in setting the window');
      }).catch((err: BusinessError) => {
        console.error('Failed to set the window, cause ' + JSON.stringify(err));
      });
    })

    windowStage.on("windowStageEvent", (data) => {
      let stageEventType: window.WindowStageEventType = data;
      switch (stageEventType) {
        case 5:
        case window.WindowStageEventType.RESUMED:
          nativeAppLifecycle.onShow();
          break;
        case 6:
        case window.WindowStageEventType.PAUSED:
          nativeAppLifecycle.onHide();
          break;
        default:
          break;
      }
    });
  }

  onWindowStageDestroy() {
    // Main window is destroyed, release UI related resources
  }

  onForeground() {
    // Ability has brought to foreground
    console.info('[LIFECYCLE-App] onShow')
    nativeAppLifecycle.onShow();
  }

  onBackground() {
    // Ability has back to background
    console.info('[LIFECYCLE-App] onDestroy')
    nativeAppLifecycle.onHide();
  }

  initConfig(fileName: string): config | undefined {
    try {
      let var1: Uint8Array = this.context.resourceManager.getRawFileContentSync(fileName);
      let bf = new Uint8Array(var1).buffer;
      let str = buffer.from(bf).toString();
      return JSON.parse(str);
    } catch (error) {
      let code = (error as BusinessError).code;
      let message = (error as BusinessError).message;
      console.error(`getRawFileContentSync failed, error code: ${code}, message: ${message}.`);
    }
    return undefined;
  }

  initEngine() {
    let var1 = this.context.filesDir + "/LayaCache";
    laya.ConchNAPI_configSetIsPlug(false);
    laya.ConchNAPI_configSetURL('http://nativetest.layabox.com/layaplayer2.0.1/index.js');
    laya.ConchNAPI_SetLocalStoragePath(var1 + "/localstorage");
    laya.ConchNAPI_setLocalizable(false);
    let config = this.initConfig("config.json");
    if (config === undefined) {
      return;
    }
    laya.ConchNAPI_InitDLib(this.context.resourceManager, 3, "cache", var1, config.ThreadMode, config.JSDebugMode, config.JSDebugPort);
  }
};
