import worker, { MessageEvents } from '@ohos.worker';
import { Logger } from './Logger';
import { handleWebViewMsg } from '../components/webview/WebViewMsg'
import { handleDialogMsg } from '../components/dialog/DialogMsg'
import { handleJumpManagerMsg } from '../system/appJump/JumpManagerMsg'
import laya from "liblaya.so";

import {
  BaseWorkerMsgEntity,
  DialogMsgEntity,
  JumpMsgEntity,
  WebViewMsgEntity
} from '../entity/WorkerMsgEntity';

export class WorkerMsgUtils {
  static workPort = worker.workerPort;
  static log: Logger = new Logger(0x0001, 'WorkerMsgUtils')

  static async recvWorkerThreadMessage(event: MessageEvents) {
    let eventData: BaseWorkerMsgEntity = event.data;
    WorkerMsgUtils.log.debug('mainThread receiveMsg, module:%{public}s, function:%{public}s', eventData.module, eventData.function);

    switch (eventData.module) {
      case "Dialog":
        handleDialogMsg(eventData as DialogMsgEntity);
        break;
      case 'WebView':
        handleWebViewMsg(eventData as WebViewMsgEntity);
        break;
      case 'JumpManager':
        handleJumpManagerMsg(eventData as JumpMsgEntity);
        break;
      case 'reload':
        laya.ConchNAPI_onRunCmd(eventData.function);
        break;
      default:
        WorkerMsgUtils.log.error('%{public}s has not implement yet', eventData.module);
    }
  }
}
