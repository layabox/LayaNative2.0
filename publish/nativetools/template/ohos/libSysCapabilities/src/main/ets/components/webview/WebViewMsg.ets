import web_webview from '@ohos.web.webview'
import { Logger } from '../../utils/Logger';
import { GlobalContext, GlobalContextConstants } from '../../common/GlobalContext';
import { WebViewMsgEntity } from '../../entity/WorkerMsgEntity';
import { BusinessError } from '@ohos.base';

let log: Logger = new Logger(0x0001, "WebViewMsg");

@Observed
export class WebViewInfo {
  public uniqueId: number = 0;
  // position
  public x: number = 0;
  public y: number = 0;
  // size
  public w: number = 0;
  public h: number = 0;
  // url
  public url?: string = '';
  public isUrl: boolean = true;
  public viewTag: number = 0
  public zoomAccess: boolean = true
  public visible: boolean = true
  public opacity: number = 1
  public backgroundColor: number = 0xffffff;
  public controller: web_webview.WebviewController = new web_webview.WebviewController()

  constructor(x: number, y: number, w: number, h: number, viewTag: number) {
    this.x = x;
    this.y = y;
    this.w = w;
    this.h = h;
    this.viewTag = viewTag
    this.uniqueId = viewTag;
  }
}

function waitUtilControllerAttached(): Promise<number> {
  return new Promise((resolve, reject) => {
    resolve(1);
  })
}

export function handleWebViewMsg(eventData: WebViewMsgEntity) {
  let index: number;
  switch (eventData.function) {
    case "createWebview": {
      let view = new WebViewInfo(0, 0, 0, 0, 0);
      let arr = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_WEB_VIEW_ARRAY) as WebViewInfo[];
      arr.push(view);
      arr[0].url = eventData.url;
      arr[0].isUrl = true;
      waitUtilControllerAttached().then(() => {
        arr[0].controller.loadUrl(eventData.url);
      })

      arr[0].x = px2vp(eventData.viewRect!.x);
      arr[0].y = px2vp(eventData.viewRect!.y);
      arr[0].w = px2vp(eventData.viewRect!.w);
      arr[0].h = px2vp(eventData.viewRect!.h);
      break;
    }
    case "removeWebView": {
      let arr = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_WEB_VIEW_ARRAY) as WebViewInfo[];
      if (arr.length > 0) {
        arr.splice(0, 1);
      }
      break;
    }
    case "showWebView": {
      let arr = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_WEB_VIEW_ARRAY) as WebViewInfo[];
      if (arr.length > 0) {
        arr[0].visible = true;
      }
      break;
    }
    case "hideWebView": {
      let arr = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_WEB_VIEW_ARRAY) as WebViewInfo[];
      if (arr.length > 0) {
        arr[0].visible = false;
      }
      break;
    }
    case "callWebviewJS": {
      let arr = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_WEB_VIEW_ARRAY) as WebViewInfo[];
      if (arr.length > 0) {
        arr[0]
          .controller
          .runJavaScript(eventData.js, (error: BusinessError, result: string) => {
            if (error) {
              log.warn("webView run JavaScript error:%{public}s", JSON.stringify(error));
              return;
            }
            if (result) {
              GlobalContext.storeGlobalThis(GlobalContextConstants.LAYA_WEB_RESULT, result)
              log.debug("webView run JavaScript result is %{public}s:", result);
            }
          })
      }
      break;
    }
    default:
      log.error('%s has not implement yet', eventData.function);
  }
}