import { GlobalContext, GlobalContextConstants } from "../common/GlobalContext";
import laya from "liblaya.so";

export class HandleMessageUtils {
    private static context = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_ABILITY_CONTEXT);
	private static workerPort;
    static RunJS(data: string): void {
        laya.ConchNAPI_RunJS(data);
    }
    

    static init(workerPort): void {
        HandleMessageUtils.workerPort = workerPort;
    }

    static postCmdToMain(data: string): string {
        HandleMessageUtils.workerPort.postMessage({module:"reload", function:data});
        return "true";
    }

    /**
    * 异步事件
    * @param eventName 事件名称
    * @param data 数据
    */
    static handleMessage(eventName: string, data: string): string {
        // 发送给游戏js在conch.setGameJsOnMessage((key,value)=>{});注册的函数中接受回调
        // let worker = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_WORKER);
        // worker.postMessage({type:"注册的key",text:"返回的value"});
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

    /**
     * aki执行本地自定义异步方法
     * @param nativeFunc 本地自定义异步方法
     * @param funcData 传输数据
     * @param funcCb 异步方法回调
     */
    static executeNativeMethod(nativeFunc: Function, funcData: string, funcCb: Function): void {
        nativeFunc && nativeFunc(funcData, funcCb);
    }
}