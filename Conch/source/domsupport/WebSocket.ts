class CloseEvent extends Event {
    code: number;
    reason: string;
    wasClean: boolean;
    constructor() {
        super('close');
    }
    initCloseEvent(typeArg: string, canBubbleArg: boolean, cancelableArg: boolean, wasCleanArg: boolean, codeArg: number, reasonArg: string): void {
    }
}

interface ErrorEvent extends Event {
    colno: number;
    error: any;
    filename: string;
    lineno: number;
    message: string;
    initErrorEvent(typeArg: string, canBubbleArg: boolean, cancelableArg: boolean, messageArg: string, filenameArg: string, linenoArg: number): void;
}

interface MessagePort extends EventTarget {
    onmessage: (ev: MessageEvent) => any;
    close(): void;
    postMessage(message?: any, ports?: any): void;
    start(): void;
    addEventListener(type: "message", listener: (ev: MessageEvent) => any, useCapture?: boolean): void;
    addEventListener(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void;
}

interface MessageEventInit extends EventInit {
    data?: any;
    origin?: string;
    lastEventId?: string;
    channel?: string;
    source?: any;
    ports?: MessagePort[];
}

class MessageEvent extends Event {
    data: any;
    origin: string;
    ports: any;
    source: Window;
    constructor(type: string, eventInitDict?: MessageEventInit) {
        super(type);
    }
    initMessageEvent(typeArg: string, canBubbleArg: boolean, cancelableArg: boolean, dataArg: any, originArg: string, lastEventIdArg: string, sourceArg: Window): void {
    }
}

//ES6 start
interface WebSocket {
    /*
    addEventListener(type: "close", listener: (ev: CloseEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "error", listener: (ev: ErrorEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "message", listener: (ev: MessageEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "open", listener: (ev: Event) => any, useCapture?: boolean): void;
    addEventListener(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void;
    */
}
/**
 * 注意必须从 ConchWebSocket 继承，必须要用es6.否则无法new出带两个field的对象。
 */
/*
class WebSocket extends ConchWebSocket implements _EventTarget {
    dispatchEvent: (evt: Event) => boolean;//接口
    removeEventListener: (type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean) => void;//接口

    onmessage: (data: MessageEvent) => void;
    _onmessage: (data: ArrayBuffer | string) => void;
    constructor(url: string) {
        super(url);
        this._onmessage = (data: ArrayBuffer | string) => {
            var evt = new MessageEvent('message');
            evt.data = data;
            this.onmessage && this.onmessage(evt);
        };
    }
    static CLOSED = 3;
    static CLOSING = 2;
    static CONNECTING = 0;
    static OPEN = 1;
}
applyMixins(WebSocket, [EventTarget]);
*/
//ES6 end


//ES5 start 
class WebSocket extends EventTarget {
    private _nativeObj:ConchWebSocket=null;

    onmessage: (data: MessageEvent) => void;
    constructor(url: string) {
        super();
        this._nativeObj = new ConchWebSocket(url);
        
        (<any>this._nativeObj)._onmessage = (data: ArrayBuffer | string) => {
            var evt = new MessageEvent('message');
            evt.data = data;
            evt.target=evt.currentTarget=this;
            this.onmessage && this.onmessage(evt);
        };
    }
    
    get binaryType(){
        return this._nativeObj.binaryType;
    }
    set binaryType(b:string){
        this._nativeObj.binaryType=b;
    }
    get timegap(){
        return this._nativeObj.timegap;
    }
    set onopen(f:(e)=>void){
         this._nativeObj.onopen=function(){
            var e=new Event("open");
            e.target=e.currentTarget=this;
            f(e);
        };
    }
    set onclose(f:()=>void){
        this._nativeObj.onclose=f;
    }
    set onerror(f:()=>void){
        this._nativeObj.onerror=f;
    }
    close(){
        this._nativeObj.close();
    }
  	send(msg:string|ArrayBuffer):void{
          this._nativeObj.send(msg);
    }
    
    get readyState():number
    {
        return this._nativeObj.readyState;
    }

    static CLOSED = 3;
    static CLOSING = 2;
    static CONNECTING = 0;
    static OPEN = 1;
}
//ES5 end

window.WebSocket = WebSocket;

interface Socket {
    addEventListener(type: "close", listener: (ev: CloseEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "error", listener: (ev: ErrorEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "message", listener: (ev: MessageEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "open", listener: (ev: Event) => any, useCapture?: boolean): void;
    addEventListener(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void;
}