
/**
 * 全局变量
 */
enum GL_CAPS
{
    NONE = 0,
    TEXTURE_COMPRESSION_PVR = 1 << 1,
    TEXTURE_COMPRESSION_ETC1 = 1 << 2,
    TEXTURE_COMPRESSION_ETC2 = 1 << 3,
    TEXTURE_TPG = 1 << 5,
    INSTANCEING = 1 << 6,
}

interface ErrorEventHandler {
    (message: string, filename?: string, lineno?: number, colno?: number, error?:Error): void;
}

interface LayaBox{
    devinfo:{resolution:string,mac:string,imei:string,imsi:string,os:string,osversion:string,phonemodel:string}
    getDeviceInfo():any;
}
interface Window extends EventTarget, WindowTimers{
    layaHtml5:any;
    screen:Screen;
    SetupWebglContext();
    downloadfile(url:string, force:boolean, onok:(data:string)=>void, onerr:()=>void);
    loadLocalStorage(url:string):Storage;
    XMLHttpRequest:typeof XMLHttpRequest;
    innerHeight: number;
    innerWidth: number;
    outerWidth: number;
    outerHeight:number;
    performance:any;    
    localStorage:Storage;
    sessionStorage:WindowSessionStorage;
    location:Location;
    document:Document;
    Audio:typeof HTMLAudioElement;
    crypto:Crypto;
    console:Console;
    pageXOffset: number;
    pageYOffset: number;
    Image:typeof HTMLImageElement;
    layabox:LayaBox;
    layaDoc:any;
    ConchVirtualBitmap:any;    //TODO 临时
    alert:(message?: any)=> void
    onerror:(message: string, filename?: string, lineno?: number, colno?: number, error?:Error)=>void;
    onunhandledrejection:(ev: any)=>void;
    onload: (ev: Event) => any;
    open(url?: string, target?: string, features?: string, replace?: boolean): Window;
    onresize: (ev: UIEvent) => any;
    requestAnimationFrame(callback: FrameRequestCallback): number;
    cancelAnimationFrame(handle: number): void;
    process:any;
    WebSocket:typeof WebSocket;
    parent: Window;
    frames: Window;
    eval(s:any);
    evalJS(s:any);
    focus():void;
    navigator: Navigator;
    devicePixelRatio: number;
    getComputedStyle(ele:Element,parm:any):any
    addEventListener(type: "devicemotion", listener: (ev: DeviceMotionEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "deviceorientation", listener: (ev: DeviceOrientationEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "message", listener: (ev: MessageEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "mousedown", listener: (ev: MouseEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "mouseenter", listener: (ev: MouseEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "mouseleave", listener: (ev: MouseEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "mousemove", listener: (ev: MouseEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "mouseout", listener: (ev: MouseEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "mouseover", listener: (ev: MouseEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "mouseup", listener: (ev: MouseEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "mousewheel", listener: (ev: MouseWheelEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "offline", listener: (ev: Event) => any, useCapture?: boolean): void;
    addEventListener(type: "online", listener: (ev: Event) => any, useCapture?: boolean): void;
    addEventListener(type: "orientationchange", listener: (ev: Event) => any, useCapture?: boolean): void;
    addEventListener(type: "pause", listener: (ev: Event) => any, useCapture?: boolean): void;
    addEventListener(type: "pointercancel", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointerdown", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointerenter", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointerleave", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointermove", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointerout", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointerover", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointerup", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "load", listener: (ev: Event) => any, useCapture?: boolean): void;
    addEventListener(type: "resize", listener: (ev: UIEvent) => any, useCapture?: boolean): void;
    addEventListener(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void;
    _conchInfo:{version:string};
    appcache:AppCache;
    loadConchUrl:any;
    postMessage:any;
    postRuntimeMessage:any;    
}
declare var window:Window;
var _window = new _Window();
window.getComputedStyle=function(ele:Element,parm:any):any
{
    return null;
}
window.pageXOffset=window.pageYOffset=0;
window.localStorage = new Storage();       //这时候还没有url。
//window.sessionStorage = new Storage();  有的项目会使用这个，但是如果没有的话，一般都会再用localStorage,为了避免出错，先不要这个。
window.sessionStorage=new WindowSessionStorage();
var location = window.location = new Location;
window.console = new Console();
window.addEventListener = _window.addEventListener.bind(_window);
window.removeEventListener = _window.removeEventListener.bind(_window);
window.dispatchEvent = _window.dispatchEvent.bind(_window);
window.document = new Document();  //是不是应该在sethref之后呢
window.layaDoc = window.document;
window.crypto = new Crypto();
window.devicePixelRatio=1.0;
var Image = window.Image = HTMLImageElement;
var Audio = window.Audio = HTMLAudioElement;
window.requestAnimationFrame = requestAnimationFrame;
window.cancelAnimationFrame = cancelAnimationFrame;
var parent = window.parent = window;
var frames = window.frames = null;
var navigator = window.navigator = new Navigator();
window.open=(url?: string, target?: string, features?: string, replace?: boolean):Window=>{
    createProcess('scripts/index.js',url);
    return window;
}
var onload = window.onload = null;

function printstack(){
	var e = new Error();
	alert((<any>e).stack);
}

var div = HTMLDivElement;
window.layabox={
	devinfo:(function getDevInfo()
	{
		var devi = conchConfig.getDeviceInfo();
		window.console.log(devi);
		return JSON.parse(devi);
	})(),
	getDeviceInfo:function(){return this.devinfo;}
};
window.layaHtml5={File:File};

var _$innerWidth=getInnerWidth();
var _$innerHeight=getInnerHeight();
var _$devicePixelRatio=getDevicePixelRatio();
Object.defineProperty(window,'innerWidth',{get:function(){return _$innerWidth}});
Object.defineProperty(window,'innerHeight',{get:function(){return _$innerHeight}});
Object.defineProperty(window,'outerWidth',{get:function(){return _$innerWidth}});
Object.defineProperty(window,'outerHeight',{get:function(){return _$innerHeight}});
Object.defineProperty(window,'devicePixelRatio',{get:function(){return _$devicePixelRatio}});
/*conch.setOnBlur && conch.setOnBlur(function(){
    var evt = new UIEvent('blur');
    evt.view=window;
    document._dispatchEvent(evt);
    window.dispatchEvent(evt);
});
conch.setOnFocus && conch.setOnFocus(function(){
    var evt = new UIEvent('focus');
    evt.view=window;
    document._dispatchEvent(evt);
    window.dispatchEvent(evt);
});*/
conch.setOnResize(function(w,h){
    _$innerWidth=w;
    _$innerHeight=h;
    window.console.log(">>>>>>>>>>>>>>>>innerWidth:"+_$innerWidth+"innerHeight:"+_$innerHeight);
    var evt = new UIEvent('resize');
    evt.view=window;
    //evt.eventPhase=Event.AT_TARGET;
    //TODO 还不好使，需要最终能调用到window.onresize
    document._dispatchEvent(evt);    //需要完整流程
    window.dispatchEvent(evt);
    //TODO 
    //window.onresize&&window.onresize(null);
});

conch.config=conchConfig;
class Screen{
    get width():number{
        return _$innerWidth;
    }
    get height():number{
        return _$innerHeight;
    }
}
window.screen=new Screen();

window.onresize=function(e:UIEvent){
    //alert('kkkkk')
}

conch.onerror = function (message, filename, lineno, colno, error) {
    if (window.onerror)
    {
        var ln=decodeTemp(lineno);
        var cn=decodeTemp(colno);
        var er=decodeTemp(error);
        var mg=decodeTemp(message);
        var fn=decodeTemp(filename);
        var e={
            message:decodeTemp(message),
            stack:er,
            name:""
        };
        window.onerror(mg=="undefined"?undefined:mg,fn=="undefined"?undefined:fn,ln!="undefined"?parseInt(ln):undefined,cn!="undefined"?parseInt(cn):undefined,e);
    }
};
/**
 * 全局错误处理
 */
Object.defineProperty(window,'onerror',{set:function(fun){
     conch.__onerror=fun;
     showAlertOnJsException(false);

},get:function(){
    return conch.__onerror;
}});


Object.defineProperty(window,'onunhandledrejection',{set:function(fun){
    conch.onunhandledrejection = fun;
},get:function(){
   return conch.onunhandledrejection;
}});

/*window.onerror = function(message: string, filename?: string, lineno?: number, colno?: number, error?:Error): void{
    alert('window.onerror');
}*/
//var console = window.console;
var document = window.document;

/**
 * 全局函数
 */
interface FrameRequestCallback {
    (time: number): void;
}
var addEventListener:(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean)=>void=window.addEventListener.bind(this);
var dispatchEvent:(evt: Event)=>boolean=window.dispatchEvent.bind(this);
var removeEventListener:(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean)=>void=window.removeEventListener.bind(this);
var clearInterval:(handle: number)=>void=window.clearInterval = _window.clearInterval;
var clearTimeout:(handle: number)=>void=window.clearTimeout= _window.clearTimeout;
var setInterval:(handler: any, timeout?: any/*, ...args: any[]*/)=>number=window.setInterval=_window.setInterval;
var setTimeout:(handler: any, timeout?: any/*, ...args: any[]*/)=>number=window.setTimeout=_window.setTimeout;

//onload
Object.defineProperty(window,'runtime',{get:function(){return true}});//给window.runtime赋值
/*window.postMessage=function(data,d) {
    if(typeof(data)=="object")
        data=JSON.stringify(data);
    conch.callWebviewJS( "window.__getMessemage",encodeURIComponent(data),"" );
}*/

window.postRuntimeMessage=function(d)
{
   if(typeof(d)=="object")
        d=JSON.stringify(d);
   d=decodeURIComponent(d);
   var e=new MessageEvent('message');
   e.data=JSON.parse(d);
   e.target=window;
   var s=new RegExp("(http|file|https)://([^/:]*)(:(\\d+)|)([^?]+)(.*|)");
   var rs=s.exec(document.referrer);
   if(rs){
    e.origin=rs[1]+"://"+rs[2]+rs[3];
   }
   window.dispatchEvent(e);
}

//为了兼容性而保留的
window.SetupWebglContext = function(){};
//document.createElement("canvas").getContext("conch");
window.downloadfile = function (url, force, onok, onerr) {
    //url = encodeURI(url);
    if (force) {
        url = (function (url) {
            var ret = url;
            if (url.indexOf('?') < 0) {
                ret = url + '?rnd=' + Math.random();
            }
            else {
                ret = url + '&downloadrnd' + Math.random().toString().substr(2) + '=1';
            }
            return ret;
        })(url);
    }
    var file =new window.layaHtml5.File(url);
    var filereader:any = new FileReader();
    filereader.onload = function () { onok && onok(filereader.result); };
    filereader.onerror = function () { onerr && onerr(); };
    filereader.readAsText(file);
};
if(window.navigator.platform!="windows")window["ontouchstart"]=null;
window['GL_CAPS'] = GL_CAPS;
window.focus=function() {}
//根据内存设置大图合集参数
var nMem=conchConfig.getTotalMem();//单位是KB
if( nMem <= 524288 )
{
    //设置引擎的大图集的个数
    conchConfig.atlasNum=10;
    //贴图资源的cache的大小。单位是byte
    conchConfig.maxTextureMemSize=64*1024*1024;
}
else if( nMem > 524288 && nMem <= 1048576 )
{
    //设置引擎的大图集的个数
    conchConfig.atlasNum=16;
    //贴图资源的cache的大小。单位是byte
    conchConfig.maxTextureMemSize=84*1024*1024;
}
else if( nMem > 1048576 )
{
    //设置引擎的大图集的个数
    conchConfig.atlasNum=20;
    //贴图资源的cache的大小。单位是byte
    conchConfig.maxTextureMemSize=128*1024*1024;
}

class Performance{
    now(){
        return tmGetCurms();
    }
}
window["Performance"] = Performance;
window.performance = new Performance();
