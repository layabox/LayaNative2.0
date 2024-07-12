import display from '@ohos.display';
import vibrator from '@ohos.vibrator';
import deviceInfo from '@ohos.deviceInfo'
import hidebug from '@ohos.hidebug';
import window from '@ohos.window';
import { GlobalContext, GlobalContextConstants } from '../../common/GlobalContext';
import hilog from '@ohos.hilog'

export class DeviceUtils {
  static MODULE_NAME: string = 'DeviceUtils';

  static startVibration(time: number) {
    try {
      vibrator.startVibration({
        type: 'time',
        duration: time * 1000, // 秒转毫秒
      }, {
        id: 0,
        usage: 'unknown'
      }, (error) => {
        if (error) {
          // log.error('vibrate fail, error.code: %{public}d, error.message: %{public}s', error.code, error.message);
          return;
        }
      });
    } catch (err) {
      // log.error('error.code: %{public}d, error.message: %{public}s', err.code, err.message);
    }
  }

  static getDeviceInfo(): string {
    // TBD 鸿蒙系统字段和安卓系统字段非一一对应关系，可根据应用业务场景做字段选择，
    // 字段可参考：https://docs.openharmony.cn/pages/v4.0/zh-cn/application-dev/reference/apis/js-apis-device-info.md/
    var displayObj = display.getDefaultDisplaySync();
    const deviceInfoJsonStr: { [key: string]: any } = {
      "dpi":displayObj?.densityDPI,
      "resolution":displayObj?.width + "*" + displayObj?.height,
      "guid":"",
      "imei":"",
      "imsi":"",
      "os":"OpenHarmony",
      "osversion":deviceInfo.sdkApiVersion,
      "phonemodel":deviceInfo.softwareModel
    };
    return JSON.stringify(deviceInfoJsonStr);
  }

  // 获取窗口尺寸
  static getScreenInch(cb) {
    var displayObj = display.getDefaultDisplaySync();
    let windowClass = null;
    try {
      let context = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_ABILITY_CONTEXT);
      window.getLastWindow(context, (err, data) => { //获取窗口实例
        if (err.code) {
          hilog.error(0x0001, "DeviceUtils", 'Failed to obtain last window when initScreenInfo. Cause:%{public}s', JSON.stringify(err));
          cb(0);
          return;
        }
        windowClass = data;
        let windowProperties: window.WindowProperties = windowClass.getWindowProperties();
        let rect: window.Rect = windowProperties.windowRect;
        let nWidth = rect.width;
        let nHeight = rect.height;
        let nDpi = displayObj.densityDPI;
        let value = Math.sqrt(nWidth * nWidth + nHeight * nHeight) / nDpi;
        cb(value);
      });
    } catch (exception) {
      hilog.error(0x0001, "DeviceUtils", 'Failed to get or set the window when initScreenInfo, cause:%{public}s', JSON.stringify(exception));
      cb(0);
    }

  }

  // 获取获取进程的私有脏内存大小,单位为kB。
  static getPrivateDirty() {
    let privateDirty: bigint = hidebug.getPrivateDirty();
    return Number(privateDirty);
  }

  // 获取剩余内存空间
  static getAvalidMem() {
    console.log("aki getAvalidMem");
    let cpuUsage: number = hidebug.getCpuUsage();
    return Math.max(0, (1 - cpuUsage)) * DeviceUtils.getPrivateDirty();
  }

  // 设置屏幕常亮
  static setKeepScreenOn(value: boolean) {
    let windowClass = null;
    try {
      let context = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_ABILITY_CONTEXT);
      window.getLastWindow(context, (err, data) => { //获取窗口实例
        if (err.code) {
          hilog.error(0x0001, "DeviceUtils", 'Failed to obtain last window when setKeepScreenOn. Cause:%{public}s', JSON.stringify(err));
          return;
        }
        windowClass = data;
        // Sets whether the screen is always on.
        let keepScreenOnPromise = windowClass.setWindowKeepScreenOn(value);
        Promise.all([keepScreenOnPromise]).then(() => {
          hilog.info(0x0001, "DeviceUtils", 'Succeeded in setKeepScreenOn, value:%{public}s', value);
        }).catch((err) => {
          hilog.error(0x0001, "DeviceUtils", 'Failed to setKeepScreenOn, cause:%{public}s', JSON.stringify(err));
        });
      });
    } catch (exception) {
      hilog.error(0x0001, "DeviceUtils", 'Failed to get or set the window when setKeepScreenOn, cause:%{public}s', JSON.stringify(exception));
    }
  }

  static setPreferredOrientation(orientatio: number) {
    let windowClass = null;
    try {
      let context = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_ABILITY_CONTEXT);
      window.getLastWindow(context, (err, data) => { //获取窗口实例
        if (err.code) {
          hilog.error(0x0001, "DeviceUtils", 'Failed to obtain last window when setPreferredOrientation. Cause:%{public}s', JSON.stringify(err));
          return;
        }
        windowClass = data;
        windowClass.setPreferredOrientation(orientatio, (err) => {
          if (err.code) {
            hilog.error(0x0001, "DeviceUtils", 'Failed to set window orientation. Cause: ' + JSON.stringify(err));
            return;
          }
          hilog.info(0x0001, "DeviceUtils", 'Succeeded in setting window orientation.');
        });
      });
    } catch (exception) {
      hilog.error(0x0001, "DeviceUtils", 'Failed to get or set the window when setPreferredOrientation, cause:%{public}s', JSON.stringify(exception));
    }
  }
}