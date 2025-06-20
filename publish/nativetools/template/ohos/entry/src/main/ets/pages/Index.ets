import laya from 'liblaya.so'
import { WorkerManager } from '../workers/WorkerManager'
import { LayaEditBox } from '../components/LayaEditBox'
import { LayaWebview } from '../components/LayaWebview'
import { LayaVideoPlayer } from '../components/LayaVideoPlayer'
import { TextInputDialog } from '../components/TextInputDialog'
import { GlobalContext, GlobalContextConstants, TextInputInfo, TextInputDialogEntity, ContextType,
  WebViewInfo, VideoPlayerInfo, WorkerMsgUtils, NapiHelper, Dialog } from "@ohos/libSysCapabilities"
import promptAction from '@ohos.promptAction'
import process from '@ohos.process';

const nativePageLifecycle: laya.CPPFunctions = laya.getContext(ContextType.JSPAGE_LIFECYCLE);
const napiContext: laya.CPPFunctions = laya.getContext(ContextType.NATIVE_API);
const  mouseWheelNapi: laya.CPPFunctions = laya.getContext(ContextType.MOUSE_WHEEL_NAPI);
NapiHelper.registerUIFunctions(napiContext.registerFunction);

let layaWorker = WorkerManager.getInstance().getWorker();
@Entry
@Component
struct Index {
  xcomponentController: XComponentController = new XComponentController();
  // EditBox
  @State editBoxArray: TextInputInfo[] = [];
  private editBoxIndexMap: Map<number, TextInputInfo> = new Map;
  // WebView
  @State webViewArray: WebViewInfo[] = [];
  private webViewIndexMap: Map<number, number> = new Map;
  // videoPlayer
  @State videoPlayerInfoArray: VideoPlayerInfo[] = [];
  private videoPlayerIndexMap: Map<number, VideoPlayerInfo> = new Map;

  private pro = new process.ProcessManager();
  private m_nBackPressTime = 0;
  // textInputDialog
  showMessage: TextInputDialogEntity = new TextInputDialogEntity('');
  dialogController: CustomDialogController = new CustomDialogController({
    builder: TextInputDialog({
      showMessage: this.showMessage
    }),
    autoCancel: true,
    alignment: DialogAlignment.Bottom,
    customStyle: true,
  })
  // PanGesture
  private panOption: PanGestureOptions = new PanGestureOptions({ direction: PanDirection.Up | PanDirection.Down });

  onPageShow() {
    console.log('[LIFECYCLE-Page] onPageShow');
    GlobalContext.storeGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_ARRAY, this.editBoxArray);
    GlobalContext.storeGlobalThis(GlobalContextConstants.LAYA_EDIT_BOX_INDEX_MAP, this.editBoxIndexMap);
    GlobalContext.storeGlobalThis(GlobalContextConstants.LAYA_WORKER, layaWorker);
    GlobalContext.storeGlobalThis(GlobalContextConstants.LAYA_WEB_VIEW_ARRAY, this.webViewArray);
    GlobalContext.storeGlobalThis(GlobalContextConstants.LAYA_WEB_VIEW_INDEX_MAP, this.webViewIndexMap);
    GlobalContext.storeGlobalThis(GlobalContextConstants.LAYA_VIDEO_PLAYER_ARRAY, this.videoPlayerInfoArray);
    GlobalContext.storeGlobalThis(GlobalContextConstants.LAYA_VIDEO_PLAYER_INDEX_MAP, this.videoPlayerIndexMap);
    GlobalContext.storeGlobalThis(GlobalContextConstants.LAYA_DIALOG_CONTROLLER, this.dialogController);
    GlobalContext.storeGlobalThis(GlobalContextConstants.LAYA_SHOW_MESSAGE, this.showMessage);
    nativePageLifecycle.onPageShow();
    Dialog.setTitle(getContext(this).resourceManager.getStringSync($r('app.string.Dialog_Title').id));
  }

  onPageHide() {
    console.log('[LIFECYCLE-Page] onPageHide');
    nativePageLifecycle.onPageHide();
  }

  onBackPress() {
    console.log('[LIFECYCLE-Page] onBackPress');
    layaWorker.postMessage({ type: "exit" });
    let curtm = Date.now();
    let MaxDelay = 3500;
    if (this.m_nBackPressTime == 0 || (this.m_nBackPressTime > 0 && curtm - this.m_nBackPressTime > MaxDelay)) {
      this.m_nBackPressTime = Date.now();
      promptAction.showToast({
        message: $r('app.string.text_backpress_toast'),
        duration: 1000
      });
    } else {
      this.pro.exit(0);
    }
    return true;
  }

  onMouseWheel(eventType: string, event: PanGestureEvent) {
    if (event.source == SourceType.Mouse) {
      const  validFinger = event.fingerList?.find(item => item.globalX && item.globalY);
      validFinger && mouseWheelNapi.onMouseWheel(eventType, validFinger.localX, validFinger.localY, event.offsetY);
    }
  }

  build() {
    // Flex({ direction: FlexDirection.Column, alignItems: ItemAlign.Center, justifyContent: FlexAlign.Center }) {
    Stack() {
      XComponent({
        id: 'xcomponentId',
        type: 'surface',
        libraryname: 'laya',
        controller: this.xcomponentController
      })
        .focusable(true)
        .defaultFocus(true)
        .gesture(
          PanGesture(this.panOption)
            .onActionStart((event: GestureEvent) => {
              this.onMouseWheel("actionStart", event);
            })
            .onActionUpdate((event: GestureEvent) => {
              this.onMouseWheel("actionUpdate", event);
            })
            .onActionEnd((event: GestureEvent) => {
              this.onMouseWheel("actionEnd", event);
            })
        )
        .onLoad((context) => {
          console.log('[laya] XComponent.onLoad Callback');
          layaWorker.postMessage({
            type: "abilityContextInit",
            context: GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_ABILITY_CONTEXT)
          });
          layaWorker.postMessage({ type: "onXCLoad", data: "XComponent" });
          layaWorker.onmessage = WorkerMsgUtils.recvWorkerThreadMessage;
        })
        .onDestroy(() => {
        })
      ForEach(this.editBoxArray, (item: TextInputInfo) => {
        LayaEditBox({ textInputInfo: item });
      }, (item: TextInputInfo) => item.viewTag.toString())

      ForEach(this.webViewArray, (item: WebViewInfo) => {
        LayaWebview({ viewInfo: item })
      }, (item: WebViewInfo) => item.uniqueId.toString())

      ForEach(this.videoPlayerInfoArray, (item: VideoPlayerInfo) => {
        LayaVideoPlayer({ videoPlayerInfo: item })
      }, (item: VideoPlayerInfo) => item.viewTag.toString())

    }
    .width('100%')
    .height('100%')
  }
}
