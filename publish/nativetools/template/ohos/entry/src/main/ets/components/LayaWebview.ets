import worker from '@ohos.worker';
import { WorkerManager } from '../workers/WorkerManager';
import { Logger, WebViewInfo } from '@ohos/libSysCapabilities'

let log: Logger = new Logger(0x0001, "LayaWebViewComponent");
@Component
export struct LayaWebview {
  @ObjectLink viewInfo: WebViewInfo;
  layaWorker: worker.ThreadWorker = WorkerManager.getInstance().getWorker();

  build() {
    Web({
      src: this.viewInfo.isUrl ? this.viewInfo.url : $rawfile(this.viewInfo.url),
      controller: this.viewInfo.controller
    })
      .position({ x: this.viewInfo.x, y: this.viewInfo.y })
      .width(this.viewInfo.w)
      .height(this.viewInfo.h)
      .border({ width: 1 })
      .domStorageAccess(true)
      .databaseAccess(true)
      .imageAccess(true)
      .onlineImageAccess(true)
      .zoomAccess(this.viewInfo.zoomAccess)
      .javaScriptAccess(true)
      .visibility(this.viewInfo.visible ? Visibility.Visible : Visibility.None)
      .opacity(this.viewInfo.opacity)
      .backgroundColor(this.viewInfo.backgroundColor)
      .onControllerAttached(() => {
        if (this.viewInfo.url != '') {
          this.viewInfo.controller.loadUrl(this.viewInfo.url);
        }
      })
  }
}