import { GlobalContext, GlobalContextConstants } from '../common/GlobalContext';
import { Logger } from '../utils/Logger'

let log: Logger = new Logger(0x0001, "LayaEditBox");

@Observed
export class TextInputInfo {
  public viewTag: number = 0;
  public x: number = 0;
  public y: number = 0;
  public w: number = 0;
  public h: number = 0;
  public opacity: number = 1;
  public value: string = '';
  public style: string = '';
  public visible: boolean = false;
  public color: string = '#ff00ffff';
  public fontSize: number = 12;
  // 光标位置
  public pos: number = 0;
  public maxLength: number = 100;
  public type = InputType.Normal;
  public regular: string = '';
  public multiAble: boolean = false;
  public forbidEdit: boolean = false;
  public textAreaController: TextAreaController = new TextAreaController();
  public textInputController: TextInputController = new TextInputController();

  constructor(viewTag: number) {
    this.viewTag = viewTag;
  }
}

export class EditBox {
  static MODULE_NAME : string = 'EditBox';

  static create(viewTag: number) : void {
    log.debug("call EditBox.create, viewTag: %{public}d", viewTag)
    let newTextInputInfo = new TextInputInfo(viewTag);
    GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_ARRAY).push(newTextInputInfo);
    GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_INDEX_MAP).set(viewTag, newTextInputInfo);
  }

  static remove(viewTag: number) : void {
    log.debug("call EditBox.remove, viewTag: %{public}d", viewTag)
    let removeIndex = -1;
    GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_ARRAY)
      .forEach((item: TextInputInfo, index: number) => {
        if (item.viewTag == viewTag) {
          removeIndex = index;
        }
      });
    GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_ARRAY).splice(removeIndex, 1);
    GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_INDEX_MAP).delete(viewTag);
  }

  static setPos(viewTag: number, x: number, y: number) : void {
    log.debug("call EditBox.setPos, viewTag: %{public}d, x: %{public}d, y: %{public}d", viewTag, x, y)
    let textInputInfo: TextInputInfo = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_INDEX_MAP).get(viewTag);
    textInputInfo.x = x;
    textInputInfo.y = y;
  }

  static setSize(viewTag: number, w: number, h: number) : void {
    log.debug("call EditBox.setSize, viewTag: %{public}d, w: %{public}d, h: %{public}d", viewTag, w, h)
    let textInputInfo: TextInputInfo = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_INDEX_MAP).get(viewTag);
    textInputInfo.w = w;
    textInputInfo.h = h;
  }

  static setOpacity(viewTag: number, opacity: number) : void {
    log.debug("call EditBox.setOpacity, viewTag: %{public}d, opacity: %{public}d", viewTag, opacity)
    let textInputInfo: TextInputInfo = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_INDEX_MAP).get(viewTag);
    textInputInfo.opacity = opacity;
  }

  static setValue(viewTag: number, value: string) : void {
    log.debug("call EditBox.setValue, viewTag: %{public}d, value: %{public}s", viewTag, value)
    let textInputInfo: TextInputInfo = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_INDEX_MAP).get(viewTag);
    textInputInfo.value = value;
  }

  static getValue(viewTag: number) : string {
    let textInputInfo: TextInputInfo = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_INDEX_MAP).get(viewTag);
    log.debug("call EditBox.getValue, viewTag: %{public}d, value: %{public}s", viewTag, textInputInfo.value);
    return textInputInfo.value;
  }

  static setStyle(viewTag: number, style: string) : void {
    log.debug("call EditBox.setStyle, viewTag: %{public}d, style: %{public}s", viewTag, style)
    let textInputInfo: TextInputInfo = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_INDEX_MAP).get(viewTag);
    textInputInfo.style = style;
  }

  static setVisible(viewTag: number, visible: boolean) : void {
    log.debug("call EditBox.setVisible, viewTag: %{public}d, visible: %{public}s", viewTag, visible)
    let textInputInfo: TextInputInfo = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_INDEX_MAP).get(viewTag);
    textInputInfo.visible = visible;
  }

  static focus(viewTag: number) : void {
    log.debug("call EditBox.focus, viewTag: %{public}d", viewTag)
    let textInputInfo: TextInputInfo = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_INDEX_MAP).get(viewTag);
    textInputInfo.visible = true;
    textInputInfo.pos = textInputInfo.value.length;
    if(textInputInfo.multiAble) {
      textInputInfo.textAreaController.caretPosition(textInputInfo.pos);
    } else {
      textInputInfo.textInputController.caretPosition(textInputInfo.pos);
    }
  }

  static setColor(viewTag: number, color: string) : void {
    log.debug("call EditBox.setColor, viewTag: %{public}d, color: %{public}s", viewTag, color)
    let textInputInfo: TextInputInfo = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_INDEX_MAP).get(viewTag);
    textInputInfo.color = color;
  }

  static setFontSize(viewTag: number, size: number) : void {
    log.debug("call EditBox.setFontSize, viewTag: %{public}d, size: %{public}d", viewTag, size)
    let textInputInfo: TextInputInfo = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_INDEX_MAP).get(viewTag);
    textInputInfo.fontSize = size;
  }

  static setCursorPosition(viewTag: number, pos: number) : void {
    log.debug("call EditBox.setCursorPosition, viewTag: %{public}d, pos: %{public}d", viewTag, pos)
    let textInputInfo: TextInputInfo = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_INDEX_MAP).get(viewTag);
    textInputInfo.pos = pos;
    if(textInputInfo.multiAble) {
      textInputInfo.textAreaController.caretPosition(pos);
    } else {
      textInputInfo.textInputController.caretPosition(pos);
    }
  }

  static setMaxLength(viewTag: number, maxLength: number) : void {
    log.debug("call EditBox.setMaxLength, viewTag: %{public}d, maxLength: %{public}d", viewTag, maxLength)
    let textInputInfo: TextInputInfo = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_INDEX_MAP).get(viewTag);
    textInputInfo.maxLength = maxLength;
  }

  static setType(viewTag: number, type: number) : void {
    log.debug("call EditBox.setType, viewTag: %{public}d, type: %{public}d", viewTag, type)
    let textInputInfo: TextInputInfo = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_INDEX_MAP).get(viewTag);
    switch (type) {
      case 0:
        textInputInfo.type = InputType.Normal;
        break;
      case 1:
        textInputInfo.type = InputType.Password;
        break;
      case 2:
        textInputInfo.type = InputType.Number;
        break;
      case 3:
        textInputInfo.type = InputType.NUMBER_PASSWORD;
        break;
      default:
        log.warn("undefined type value: %{public}", type)
    }
  }

  static setRegular(viewTag: number, regular: string) : void {
    log.debug("call EditBox.setRegular, viewTag: %{public}d, regular: %{public}s", viewTag, regular)
    let textInputInfo: TextInputInfo = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_INDEX_MAP).get(viewTag);
    textInputInfo.regular = regular;
  }

  static setMultiAble(viewTag: number, multiAble: boolean) : void {
    log.debug("call EditBox.setMultiAble, viewTag: %{public}d, multiAble: %{public}s", viewTag, multiAble)
    let textInputInfo: TextInputInfo = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_INDEX_MAP).get(viewTag);
    textInputInfo.multiAble = multiAble;
  }

  static setForbidEdit(viewTag: number, forbidEdit: boolean) : void {
    log.debug("call EditBox.setForbidEdit, viewTag: %{public}d, forbidEdit: %{public}s", viewTag, forbidEdit)
    let textInputInfo: TextInputInfo = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_INDEX_MAP).get(viewTag);
    textInputInfo.forbidEdit = forbidEdit;
  }
}