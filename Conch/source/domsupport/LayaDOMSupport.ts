///<reference path="LayaConchRuntime.d.ts" />
/**
 * 这个是模拟多重继承的函数
 */
function applyMixins(derivedCtor: any, baseCtors: any[]) {
    baseCtors.forEach(baseCtor => {
        Object.getOwnPropertyNames(baseCtor.prototype).forEach(name => {
             if (name !== 'constructor') {
                derivedCtor.prototype[name] = baseCtor.prototype[name];
            }
        });
    }); 
}

class IDBEnvironment implements IDBEnvironment{}

/**
 * 因为有些局部变量不希望直接暴露到全局环境下，所以做一个封装用的全局类
 */
class _Window extends EventTarget {
    crypto: Crypto;
    onerror: ErrorEventHandler;
    Audio:HTMLAudioElement;
    private timer = new WindowTimers();
    clearInterval:(handle: number)=>void=this.timer.clearInterval;
    clearTimeout:(handle: number)=>void = this.timer.clearTimeout;
    setInterval:(handler: any, timeout?: any/*, ...args: any[]*/)=>number=this.timer.setInterval;
    setTimeout:(handler: any, timeout?: any/*, ...args: any[]*/)=>number=this.timer.setTimeout;
    _removeEventListener:(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean)=>void;
    constructor(){
        super();
        this._removeEventListener=this.removeEventListener;
        this.removeEventListener=this.removeEventListenernew;
    }
    document:Document;
    //clearImmediate:(handle: number)=>void;
     addEventListener (type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void {
         if(type=="devicemotion"||type=="deviceorientation"){
             conch.setSensorAble(true);
         }
         super.addEventListener(type,listener,useCapture);
     }
     
    removeEventListenernew(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void
    {
        this._removeEventListener(type, listener, useCapture);
        if(type=="devicemotion"||type=="deviceorientation")
        {
           var de= this._evtMaps.get("devicemotion");
           var deo=this._evtMaps.get("deviceorientation");       
           if((!de||de.length==0)&&(!deo||deo.length==0))
           {
              conch.setSensorAble(false);
           }
        }
        
    }
    
}

//applyMixins(_Window,[EventTarget,WindowTimers,WindowSessionStorage, WindowLocalStorage,GlobalEventHandlers,IDBEnvironment,WindowBase64]);

