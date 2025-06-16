import { ViewRect, WebViewMsgEntity } from '../../entity/WorkerMsgEntity';
import type { ThreadWorkerGlobalScope } from '@ohos.worker';

export class WebView {
  static MODULE_NAME: string = 'WebView';

  private static workerPort: ThreadWorkerGlobalScope;

  static init(workerPort: ThreadWorkerGlobalScope): void {
    WebView.workerPort = workerPort;
  }

  static createWebview(url: string, x: number, y: number, w: number, h: number, canclose: number): void {
    let webViewMsgEntity: WebViewMsgEntity = new WebViewMsgEntity(WebView.MODULE_NAME, 'createWebview');
    webViewMsgEntity.url = url;
    webViewMsgEntity.viewRect = new ViewRect(x, y, w, h);
    WebView.workerPort.postMessage(webViewMsgEntity);
  }

  static closeWebview(): void {
    let webViewMsgEntity: WebViewMsgEntity = new WebViewMsgEntity(WebView.MODULE_NAME, 'removeWebView');
    WebView.workerPort.postMessage(webViewMsgEntity);
  }
  static callWebviewJS(sFunctionName: string, sJsonParam:string, sCallbackFuncton:string): void {
    let sBuffer = "javascript: try{" + sFunctionName+"(\"" + sJsonParam +"\",\"" + sCallbackFuncton + "\"" + ");}catch(e){window.runtime.JSLog('CallJSFunction err ' + e.stack );}";
    let webViewMsgEntity: WebViewMsgEntity = new WebViewMsgEntity(WebView.MODULE_NAME, 'callWebviewJS');
    webViewMsgEntity.js = sBuffer;
    WebView.workerPort.postMessage(webViewMsgEntity);
  }

  static hideWebview(): void {
    let webViewMsgEntity: WebViewMsgEntity = new WebViewMsgEntity(WebView.MODULE_NAME, 'hideWebView');
    WebView.workerPort.postMessage(webViewMsgEntity);
  }

  static showWebView(): void {
    let webViewMsgEntity: WebViewMsgEntity = new WebViewMsgEntity(WebView.MODULE_NAME, 'showWebView');
    WebView.workerPort.postMessage(webViewMsgEntity);
  }
}