import { TextInputInfo } from '@ohos/libSysCapabilities/src/main/ets/components/EditBox'
import { Logger } from '@ohos/libSysCapabilities'

let log: Logger = new Logger(0x0001, "LayaEditBoxComponent");

@Component
export struct LayaEditBox {
  @ObjectLink textInputInfo: TextInputInfo;

  build() {
    if (this.textInputInfo.multiAble) {
      TextArea({ text: this.textInputInfo.value, controller: this.textInputInfo.textAreaController })
        .id('TextArea' + this.textInputInfo.viewTag)
        .position({ x: px2vp(this.textInputInfo.x), y: px2vp(this.textInputInfo.y) })
        .size({ width: px2vp(this.textInputInfo.w), height: px2vp(this.textInputInfo.h) })
        .enabled(!this.textInputInfo.forbidEdit)
        .padding({ top: 0, left: 0, bottom: 0, right: 0 })
        .borderRadius(0)
        .maxLength(this.textInputInfo.maxLength)
        .align(Alignment.TopStart)
        .textAlign(TextAlign.Start)
        .fontSize(px2fp(this.textInputInfo.fontSize))
        .fontColor(this.textInputInfo.color)
        .backgroundColor(Color.Transparent)
        .visibility(this.textInputInfo.visible ? Visibility.Visible : Visibility.None)
        .inputFilter(this.textInputInfo.regular, (e) => {
          log.warn('input illegal, msg: %{public}s', JSON.stringify(e))
        })
        .onVisibleAreaChange([0.0, 1.0], (isVisible: boolean, currentRadio: number) => {
          focusControl.requestFocus('TextArea' + this.textInputInfo.viewTag);
        })
        .onChange((value: string) => {
          this.textInputInfo.value = value;
        })
    } else {
      TextInput({ text: this.textInputInfo.value, controller: this.textInputInfo.textInputController })
        .id('TextInput' + this.textInputInfo.viewTag)
        .position({ x: px2vp(this.textInputInfo.x), y: px2vp(this.textInputInfo.y) })
        .size({ width: px2vp(this.textInputInfo.w), height: px2vp(this.textInputInfo.h) })
        .enabled(!this.textInputInfo.forbidEdit)
        .padding({ top: 0, left: 0, bottom: 0, right: 0 })
        .borderRadius(0)
        .maxLength(this.textInputInfo.maxLength)
        .type(this.textInputInfo.type)
        .align(Alignment.Center)
        .textAlign(TextAlign.Start)
        .fontSize(px2fp(this.textInputInfo.fontSize))
        .fontColor(this.textInputInfo.color)
        .backgroundColor(Color.Transparent)
        .visibility(this.textInputInfo.visible ? Visibility.Visible : Visibility.None)
        .showPasswordIcon(false)
        .inputFilter(this.textInputInfo.regular, (e) => {
          log.warn('nput illegal, msg: %{public}s', JSON.stringify(e))
        })
        .onVisibleAreaChange([0.0, 1.0], (isVisible: boolean, currentRadio: number) => {
          focusControl.requestFocus('TextInput' + this.textInputInfo.viewTag);
        })
        .onChange((value: string) => {
          this.textInputInfo.value = value;
        })
    }
  }
}