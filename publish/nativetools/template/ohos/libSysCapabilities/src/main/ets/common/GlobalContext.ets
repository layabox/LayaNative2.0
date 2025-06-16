export class GlobalContext {
  private static instance: GlobalContext;
  private propMap = new Map<string, Object>();

  private static getInstance(): GlobalContext {
    if (!GlobalContext.instance) {
      GlobalContext.instance = new GlobalContext();
    }
    return GlobalContext.instance;
  }

  public static loadGlobalThis(name: string): Object {
    let ins = GlobalContext.getInstance();
    return ins.propMap.get(name)!;
  }

  public static storeGlobalThis(name: string, obj: Object) {
    let ins = GlobalContext.getInstance();
    ins.propMap.set(name, obj);
  }
}

export class GlobalContextConstants {
  static readonly LAYA_EDIT_BOX_ARRAY = "laya.editBoxArray";
  static readonly LAYA_EDIT_BOX_INDEX_MAP = "laya.editBoxIndexMap";
  static readonly LAYA_WORKER = "laya.layaWorker";
  static readonly LAYA_WEB_VIEW_ARRAY = "laya.WebViewArray";
  static readonly LAYA_WEB_VIEW_INDEX_MAP = "laya.WebViewIndexMap";
  static readonly LAYA_VIDEO_PLAYER_ARRAY = "laya.VideoPlayerArray";
  static readonly LAYA_VIDEO_PLAYER_INDEX_MAP = "laya.VideoPlayerIndexMap";
  static readonly LAYA_DIALOG_CONTROLLER = "laya.dialogController";
  static readonly LAYA_SHOW_MESSAGE = "laya.showMessage";

  static readonly LAYA_ABILITY_CONTEXT = "laya.abilityContext";
  static readonly LAYA_ABILITY_WANT = "laya.abilityWant";
  static readonly LAYA_WEB_RESULT = "laya.webResult";
}