import laya from "liblaya.so";
import { ContextType } from "../../common/Constants"
import sensor from '@ohos.sensor';
import display from '@ohos.display';
import { Result } from "../../entity/Result"
import { Logger } from '../../utils/Logger'

let log: Logger = new Logger(0x0001, "AccelerometerUtils");

export default class Accelerometer {
  private static instance = new Accelerometer();

  static getInstance(): Accelerometer {
    return Accelerometer.instance;
  }

  static enable(): void {
    try {
      sensor.on(sensor.SensorId.ACCELEROMETER, (data) => {
        laya.ConchNAPI_handleDeviceMotionEvent(0, 0, 0, data.x, data.y, data.z, 0, 0, 0, 1);
      });
    } catch (err) {
      log.error('Accelerometer init fail,  err: %{public}s', JSON.stringify(Result.error(-1, JSON.stringify(err))) ?? '');
    }
  }

  static disable(): void {
    try {
      sensor.off(sensor.SensorId.ACCELEROMETER);
    } catch (err) {
      log.error('Accelerometer off fail,  err: %{public}s', JSON.stringify(Result.error(-1, JSON.stringify(err))) ?? '');
    }
  }
}