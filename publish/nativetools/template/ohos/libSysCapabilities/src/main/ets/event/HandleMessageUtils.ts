import { GlobalContext, GlobalContextConstants } from "../common/GlobalContext";

export default class HandleMessageUtils {
    private static context = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_ABILITY_CONTEXT);

    /**
    * 异步事件
    * @param eventName 事件名称
    * @param data 数据
    */
    static handleMessage(eventName: string, data: string): string {
        return "true";
    }

    /**
    * 同步事件
    * @param eventName 事件名称
    * @param data 数据
    * @param cb callback
    */
    static handleSyncMessage(eventName: string, data: string, cb: Function): void {
        cb("");
    }
}