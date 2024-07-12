import laya from "liblaya.so";
import sensor from '@ohos.sensor';
import { Result } from "../../entity/Result"
import { Logger } from '../../utils/Logger'

let log: Logger = new Logger(0x0001, "OrientationUtils");

export default class Orientation {
  private static instance = new Orientation();

  static getInstance(): Orientation {
    return Orientation.instance;
  }

  static enable(): void {
    try {
      sensor.on(sensor.SensorId.ORIENTATION, (data) => {
        laya.ConchNAPI_handleDeviceOrientationEvent(data.alpha, data.beta, data.gamma);
      });
    } catch (err) {
      log.error('Orientation init fail,  err: %{public}s', JSON.stringify(Result.error(-1, JSON.stringify(err))) ?? '');
    }
  }

  static disable(): void {
    try {
      sensor.off(sensor.SensorId.ORIENTATION);
    } catch (err) {
      log.error('Orientation off fail,  err: %{public}s', JSON.stringify(Result.error(-1, JSON.stringify(err))) ?? '');
    }
  }
}