/**
 * name
 */
'use strict'
require("webglPlus.js");
conch["platCallBack"]=function(ret)
{
    var objid,m,rs,c,rJSON;
    if(ret==null)return;
    objid=ret.objId;
    m=ret.mName;
    c=ret.cName;
    rs=ret.v;
    var platC:IPlatForm;
    if(objid!=-1){
        platC=PlatformObj.objMap[objid];
    }
    else{
        platC=PlatformClass.clsMap[c];
    }
    if(platC){
        var callback:Function= platC.callBackList[m];
        callback&&callback(rs);
    }
}
interface IPlatForm
{
    callBackList:Object;
    call(methodName,p1?:any,p2?:any,p3?:any,p4?:any,p5?:any,p6?:any,p7?:any,p8?:any,p9?:any);
    callWithBack(callback,methodName,p1?:any,p2?:any,p3?:any,p4?:any,p5?:any,p6?:any,p7?:any,p8?:any,p9?:any):void
    
}
class PlatformBase implements IPlatForm{
    callBackList:Object;
    className:string;
    objid:number;
    call(methodName,p1?:any,p2?:any,p3?:any,p4?:any,p5?:any,p6?:any,p7?:any,p8?:any,p9?:any)
    {
        var a:Array<any>= Array.prototype.slice.call(arguments);
        a.shift();
        try{
            var rs=JSON.parse(conch.callMethod(this.objid,true,this.className,methodName,JSON.stringify(a))||"{}");
            return rs.v;
        }
        catch(e)
        {
            return null;
        }
    }
    
    callWithBack(callback,methodName,p1?:any,p2?:any,p3?:any,p4?:any,p5?:any,p6?:any,p7?:any,p8?:any,p9?:any):void
    {
        this.callBackList[methodName]=callback;
        var a:Array<any>= Array.prototype.slice.call(arguments);
        a.splice(0,2)
        try{
            var rs=JSON.parse(conch.callMethod(this.objid,false,this.className,methodName,JSON.stringify(a))||"{}");
            return rs.v;
        }
        catch(e)
        {
            return null;
        }
    }
}

class PlatformClass extends PlatformBase {
    static clsMap:Object={};
    constructor(clsName:string)
    {
        super();
        this.objid=-1;
        this.className=clsName;
        this.callBackList={};
        PlatformClass.clsMap[clsName]=this;
    }
    static createClass(clsName:string):PlatformClass
    {
        return PlatformClass.clsMap[clsName]||new PlatformClass(clsName);
    }
    
    newObject(p1?:any,p2?:any,p3?:any,p4?:any,p5?:any,p6?:any,p7?:any,p8?:any,p9?:any):PlatformObj
    {
        var a:Array<any>= Array.prototype.slice.call(arguments);
        a.splice(0,0,"<init>");
        var rs=new PlatformObj(this);
        rs.init.apply(rs,a);
        return rs;
    }
}
class PlatformObj extends PlatformBase{
    static objMap:Object={};
    static objNum:number=0;
    cls:PlatformClass;
    callBackList:Object;
    constructor(cls:PlatformClass)
    {
        super();
        this.cls=cls;
        this.className=cls.className;
        this.callBackList={};
    }
    init()
    {
        this.objid=PlatformObj.objNum++;
        this.call.apply(this,arguments)||-1;
        PlatformObj.objMap[this.objid]=this;
    }
}
window["PlatformClass"]=PlatformClass;
window["PlatformObj"]=PlatformObj;

/**
 * 直接加载缓存中的某个文件
 * @param cache
 * @param relUrl  相对路径，从/开始。例如 '/test.html'
 */
function loadRawCache(cache:AppCache, relUrl:string, encode:'raw'|'utf8'):string|ArrayBuffer{
    var cpath = cache.getCachePath();
    var relFile = relUrl; 
    var id =  new Uint32Array([cache.hashstr(relFile)])[0].toString(16);
    var fn = cpath+'/files/'+id.substr(0,2)+'/'+id.substr(2);
    var ab = fs_readFileSync(fn);
    if(ab){
        var content = new Uint8Array(ab,48);
        //content就是文件内容
        //如果需要字符串
        if(encode==='utf8'){
            var strCont = String.fromCharCode.apply(null,content);
            return strCont
        }
        else
            return content.buffer;
    }  
    return null;
}
class textBitmap
{
    _nativeObj:_textBitmap;
    _data:ArrayBuffer;
    constructor(obj:any)
    {
        this._nativeObj = obj;
        this._data = null;
    }
    get width():number
    {
        return this._nativeObj.width;
    }
    get height():number
    {
        return this._nativeObj.height;
    }
    get data():ArrayBuffer
    {
        return this._data;
    }
    set data(value:ArrayBuffer)
    {
        this._data=value;
    }
}
window["textBitmap"] = textBitmap;

class measureText
{
    width:number = 0;;
    height:number = 0;;
    constructor()
    {
    }
}
class conchTextCanvas
{    
    private charSizeMap = new Map<string, Map<string, any>>();
    _nativeObj:_conchTextCanvas;
    _currentFont:string = null;
    constructor()
    {
        this._nativeObj = window["_conchTextCanvas"];
    }
    scale(x:number,y:number):void
    {
        this._nativeObj.scale(x, y);
    }
    get font():string
    {
        this._currentFont = this._nativeObj.font;
        return this._currentFont;
    }
    set font(value:string)
    {
        this._currentFont = value;
        this._nativeObj.font  = value;
    }
    setFontInfo(font:string):void
    {
        this._nativeObj.setFontInfo(font);
    }
    measureText(text:string):measureText
    {
        if (!this._currentFont)
        {
            return null;
        }
        var measure:measureText = new measureText();
        var widthCount:number = 0;
        var maxHeight:number = 0;
        var charCode:number = 0;
        var char:string = "";
        var size:any = null;
        var dic:Map<string, any> = null;
        if (!text)
        {
            //兼容老版本undefined报错
            return  this.measureText('undefined');
        }
        for (var i:number = 0; i < text.length; i++)
        {
            char = text.charAt(i);
            charCode = text.charCodeAt(i);
            dic = this.charSizeMap.get(this._currentFont);
            if (!dic)
            {
                dic = new Map<string, any>();
                this.charSizeMap.set(this._currentFont.slice(0), dic);
            }
            if (charCode >= 0x4E00 && charCode <= 0x9FFF)
            {
                size = dic.get("国");
                if (!size)
                {
                    size = this._nativeObj.measureChar("国".charCodeAt(0));
                    dic.set("国", size);
                }
                    
            } 
            else
            {
                size = dic.get(char);
                if (!size)
                {
                    size = this._nativeObj.measureChar(charCode);
                    dic.set(char.slice(0), size);
                }
            }      
            widthCount += size.width;
            maxHeight = size.height > maxHeight ? size.height : maxHeight; 
        }
        measure.width = widthCount;
        measure.height = maxHeight;
        return measure;
    }
    initFreeTypeDefaultFontFromFile(defaultTTFs:string):boolean
    {
        return this._nativeObj.initFreeTypeDefaultFontFromFile(defaultTTFs);
    }
    initFreeTypeDefaultFontFromBuffer(ab:ArrayBuffer):boolean
    {
        return this._nativeObj.initFreeTypeDefaultFontFromBuffer(ab);
    }
    setFontFaceFromUrl(fontFamily:string, TTFFileName:string):boolean
    {
        return this._nativeObj.setFontFaceFromUrl(fontFamily, TTFFileName);
    }
    setFontFaceFromBuffer(fontFamily:string, ab:ArrayBuffer):boolean
    {
        return this._nativeObj.setFontFaceFromBuffer(fontFamily, ab);
    }
    removeFont(fontFamily:string):boolean
    {
        return this._nativeObj.removeFont(fontFamily);
    }
    getTextBitmapData(sText:string,nColor:number, nBorderSize:number,nBorderColor:number)
    {
        var nativeObj:_textBitmap = this._nativeObj._getTextBitmapData(sText,nColor,nBorderSize,nBorderColor);
        var pTextBitmap = new textBitmap(nativeObj);
        pTextBitmap.data = nativeObj.data;
        return pTextBitmap;
    }
}
window["conchTextCanvas"] = new conchTextCanvas;