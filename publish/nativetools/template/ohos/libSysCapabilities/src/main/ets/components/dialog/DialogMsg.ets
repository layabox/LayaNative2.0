import prompt from '@system.prompt'
import { Logger } from '../../utils/Logger'
import { GlobalContext, GlobalContextConstants } from "../../common/GlobalContext"
import { TextInputDialogEntity } from '../../entity/TextInputDialogEntity';
import { DialogMsgEntity } from '../../entity/WorkerMsgEntity';


let log: Logger = new Logger(0x0001, "DialogMsg");

export function handleDialogMsg(eventData: DialogMsgEntity): void {
  switch (eventData.function) {
    case "showDialog": {
      let title = eventData.title;
      let message = eventData.message;
      showDialog(title, message);
      break;
    }
    case "showTextInputDialog": {
      let tempShowMessage = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_SHOW_MESSAGE) as TextInputDialogEntity;
      tempShowMessage.message = eventData.message;
      let controller = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_DIALOG_CONTROLLER) as CustomDialogController;
      controller.open();
      break;
    }
    case "hideTextInputDialog": {
      let tempShowMessage = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_SHOW_MESSAGE) as TextInputDialogEntity;
      tempShowMessage.message = '';
      let controller = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_DIALOG_CONTROLLER) as CustomDialogController;
      controller.close();
      break;
    }
  }
}

function showDialog(dialogTitle: string | undefined, dialogMessage: string | undefined) {
  prompt.showDialog({
    title: dialogTitle,
    message: dialogMessage,
    buttons: [
      {
        text: 'OK',
        color: '#000000'
      },
    ],
    success: (data) => {
      log.debug("handling callback, data:%{public}s", data);
    }
  });
}