
interface NavigatorID {
    appName: string;
    appVersion: string;
    platform: string;
    product: string;
    productSub: string;
    userAgent: string;
    vendor: string;
    vendorSub: string;
}

interface NavigatorOnLine {
    onLine: boolean;
}

interface PositionError {
    code: number;
    message: string;
    toString(): string;
    PERMISSION_DENIED: number;
    POSITION_UNAVAILABLE: number;
    TIMEOUT: number;
}

interface PositionCallback {
    (position: Position): void;
}

interface PositionErrorCallback {
    (error: PositionError): void;
}

interface Coordinates {
    accuracy: number;
    altitude: number;
    altitudeAccuracy: number;
    heading: number;
    latitude: number;
    longitude: number;
    speed: number;
}

interface Position {
    coords: Coordinates;
    timestamp: number;
}

interface PositionOptions {
    enableHighAccuracy?: boolean;
    timeout?: number;
    maximumAge?: number;
}

interface Geolocation {
    clearWatch(watchId: number): void;
    getCurrentPosition(successCallback: PositionCallback, errorCallback?: PositionErrorCallback, options?: PositionOptions): void;
    watchPosition(successCallback: PositionCallback, errorCallback?: PositionErrorCallback, options?: PositionOptions): number;
}

interface NavigatorGeolocation {
    geolocation: Geolocation;
}

class Navigator implements NavigatorID, NavigatorOnLine, NavigatorGeolocation {
    //NavigatorID
    get appName():string{ return 'Netscape';}
    get appVersion(): string{ return this.userAgent};
    get platform():string{return window.layabox.devinfo.os};
    product: string;
    productSub: string;
    get userAgent(): string{
        var os=window.layabox.devinfo.os;
        if(os=="ios")
        return "LayaBox(iPhone; CPU iPhone OS Mac OS X)";
        else if(os=="android")
        return "LayaBox Android";
        else
        return 'LayaBox/2.1';
    };
    vendor: string;
    vendorSub: string;
    get sv():number{
        var v:string=window.layabox.devinfo.osversion;
        var t:number=parseFloat(v);
        if(isNaN(t))
        {
          t=v.charCodeAt(0)-71;
        }
        return t;
    };
    //NavigatorOnLine
    onLine: boolean;
    //NavigatorGeolocation
    geolocation: Geolocation;
    
    get appCodeName(): string{ return 'Mozilla'; }
    appMinorVersion: string;
    browserLanguage: string;
    connectionSpeed: number;
    cookieEnabled: boolean;
    cpuClass: string;
    get language(): string{ return 'zh-CN';};
    maxTouchPoints: number;
    msManipulationViewsEnabled: boolean;
    msMaxTouchPoints: number;
    msPointerEnabled: boolean;
    pointerEnabled: boolean;
    systemLanguage: string;
    get userLanguage(): string{return 'zh-CN';}
    webdriver: boolean;
    constructor(){
    }
    getGamepads(): Gamepad[]{
        return null;
    }
    javaEnabled(): boolean{return false;}
    vibrate(pattern: number | number[]): boolean{
        return false;
    }
    addEventListener(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void{
        
    }
}