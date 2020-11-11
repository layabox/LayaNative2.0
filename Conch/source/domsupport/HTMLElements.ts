
class CSSStyleDeclaration {
    _transform = new Float32Array([1, 0, 0, 1, 0, 0]);
    _styleNumber = 0;
    _htmlEle: HTMLElement;
    constructor() {
    }
    get length(): number {
        return this._styleNumber;
    }
    getPropertyPriority(propertyName: string): string {
        return '';
    }
    getPropertyValue(propertyName: string): string {
        return '';
    }
    item(index: number): string {
        return '';
    }
    removeProperty(propertyName: string): string {
        return '';
    }
    setProperty(propertyName: string, value: string, priority?: string): void {

    }
    set left(l: string) {
        var nl = parseInt(l);
        this._htmlEle.clientLeft = nl;
    }
    set top(t: string) {
        var nt = parseInt(t);
        this._htmlEle.clientTop = nt;
    }
    set height(h: string) {
        var nh = parseInt(h);
        this._htmlEle.clientHeight = nh;
    }
    set width(w: string) {
        var nw = parseInt(w);
        this._htmlEle.clientWidth = nw;
    }

    set transform(trans: string) {
        var s1 = trans.split(/[\(,\)]/);
        if (s1 && s1.length > 2) {
            if (s1[0] === 'matrix') {
                //'matrix(1.45938, 0, 0, 1.45938, 0, 0)'
                if (s1.length >= 7) {
                    this._transform[0] = 1 * <any>s1[1];
                    this._transform[1] = 1 * <any>s1[2];
                    this._transform[2] = 1 * <any>s1[3];
                    this._transform[3] = 1 * <any>s1[4];
                    this._transform[4] = 1 * <any>s1[5];
                    this._transform[5] = 1 * <any>s1[6];
                    if (this._htmlEle && this._htmlEle.onCSS_Transform) {
                        this._htmlEle.onCSS_Transform(this._transform);
                    }
                } 
            }
            else if (s1[0] === 'scale') {
                console.log('css scale not implemented');
            }
        }
    }
}

class HTMLElement extends Element {
    style = new CSSStyleDeclaration();
    onCSS_Transform(mat: Float32Array): void { };
    constructor() {
        super();
        this.style._htmlEle = this;
    }
    onerror(ev: Event): any {

    }
    onload(ev: Event): any {

    }
}
window["HTMLElement"] = HTMLElement;

interface NativeObjWrapper {
    _nativeObj: any;
}
class HTMLDivElement extends HTMLElement {
    align: string;
    noWrap: boolean;
    constructor() {
        super();
        this.tagName = "DIV";
    }
}

class HTMLScriptElement extends HTMLElement {
    private _src: string;
    private _text: string;
    constructor() {
        super();
        this.tagName = "SCRIPT";
    }

    set src(url: string) {
        //url=encodeURI(url);
        var _t: HTMLScriptElement = this;
        this._src = location.resolve(url);
        console.log("HTMLScriptElement set src" + this._src);
        // document.downloadlist.push({"src":this._src,"obj":this});
        document.uploadScript({ "src": this._src, "obj": this });
        //document.dispatchEvent(new Event("listDownload"));

        /* window.downloadfile(this._src,false,function(data:string){
             var e:Event=new Event("load");
              e.target=e.currentTarget=_t;
              window.eval(data+"\n//@ sourceURL="+url);
              _t.onload&&_t.onload(e);
         },function(){
              var e:Event=new Event("error");
              e.target=e.currentTarget=_t;
              _t.onerror&&_t.onerror(e);
         })*/
    }

    get src(): string {
        return this._src;
    }

    set text(t: string) {
        this._text = t;
        document.uploadScript({ "text": this._text, "obj": this });
        //document.dispatchEvent(new Event("listDownload"));
    }

    get text(): string {
        return this._text;
    }
}
class HTMLCanvasElement extends HTMLElement {
    static defaultWidth = 300;
    static defaultHeight = 150;

    _isFirst = false;
    _tranform: Float32Array = null;
    _hasTransform = false;
    static RS:boolean;
    public _context: CanvasRenderingContext;
    _w:number;
    _h:number;
    private _width: number;
    private _height: number;
    private _clientRect = { left: 0, top: 0, width: 0, height: 0, right: 0, bottom: 0 };
    constructor() {
        super();
        this.clientLeft
        this.tagName = "CANVAS";
        this._width = HTMLCanvasElement.defaultWidth;
        this._height = HTMLCanvasElement.defaultHeight;
    }
    setFirst():void
    {
        this._isFirst = true;
        if(this._context)
        {
            //this._context.setCanvasType(0);
            LayaGLContext.instance.canvas = this;
        }
    }
    getBoundingClientRect() {
        var cr = this._clientRect;
        if (this._hasTransform) {
            cr.width = this._width * this._tranform[0];
            cr.height = this._height * this._tranform[3];
            cr.left = this._tranform[4];
            cr.top = this._tranform[5];
            cr.right = cr.left + cr.width;
            cr.bottom = cr.left + cr.height;
        }
        else {
            cr.right = cr.width = this._width;
            cr.bottom = cr.height = this._height;
        }
        return cr;
    }

    getContext(contextType: string/*, ...args: any[]*/): any{
        console.log("getContext type=" + contextType);
        if(!LayaGLContext.instance)
        {
            LayaGLContext.instance = new LayaGLContext(contextType);
        }
        if(!this._context)
        {
            this._context = new CanvasRenderingContext(this);
            if (this._width) this._context.setSize(this._width, this._height);
            if (this._isFirst) 
            {
                //this._context.setCanvasType(0);
                LayaGLContext.instance.canvas = this;
            }
        }
        let version:string = LayaGLContext.instance._nativeObj.getStringEx(0x1F02);
        if( contextType.indexOf("webgl2") >= 0 )
        {    
            if (version.indexOf("OpenGL ES 3.") >= 0)
            {
                LayaGLContext.instance.canvas = this;
                return LayaGLContext.instance;
            }
            else
            {
                return null;
            }
        }
        else if( contextType.indexOf("webgl") >= 0 )
        {
            if (version.indexOf("OpenGL ES 2.") >= 0 || version.indexOf("OpenGL ES 3.") >= 0)
            {
                LayaGLContext.instance.canvas = this;
                return LayaGLContext.instance;
            }
            else
            {
                return null;
            }
        }
        else
        {
            return this._context;
        }
    }
    onCSS_Transform(mat: Float32Array): void {
        this._tranform = mat;
        this._hasTransform = true;
        //由于c那边不知道哪个是主画布，所以还是在js这边处理webgl的
        var e = new Event('csstransform');
        this.dispatchEvent(e);
        //2d的走另外的流程
        var sw = mat[0];
        var sh = mat[3];
        conchConfig.setScreenScale(sw, sh, mat[4], mat[5]);
        //conch._canvPosX = mat[4];
        //conch._canvPosY = mat[5];
        // conch._canvScaledW = conch._canvWidth * sw;
        // conch._canvScaledH = conch._canvHeight * sh;
        if(sw==0||sh==0)
        {
            if(mat[4]>mat[5])
            {
                console.warn("设置横竖屏错误! 请在初始html页面meta中设置screenorientation为横屏landscape");
            }
            else
            {
                console.warn("设置横竖屏错误! 请在初始html页面meta中设置screenorientation为竖屏portrait");
            }
        }
    }
    /**
      * Returns the content of the current canvas as an image that you can use as a source for another canvas or an HTML element.
      * @param type The standard MIME type for the image format to return. If you do not specify this parameter, the default value is a PNG format image.
      */
    toDataURL(type?: string/*, ...args: any[]*/): string {
        return "";
    }
    /*
    toBlob(): Blob{
        return null;
    }
    */
    setSize(w: number, h: number): void {
        this._width = w;
        this._height = h;
        this._context && this._context.setSize(w,h);
        /*
        if (this._isFirst) {
            var sw = this._tranform ? this._tranform[0] : 1.0;
            var sh = this._tranform ? this._tranform[3] : 1.0;
            conch._canvWidth = w;
            conch._canvHeight = h;
            conch._canvScaledW = w * sw;
            conch._canvScaledH = h * sh;
        }
        */
    }

    set width(w: number) {
        if(this._isFirst&&HTMLCanvasElement.RS)
        {
            this._w=w;
            return;
        }
        //if (!this._isFirst) {
            if (this._width != w) {
                this._width = w;
                this._context && this._context.setSize(this._width, this._height);
            }
//        } else {
//            if(window.innerWidth<window.innerHeight)
//            {
//                this._ctx2d&&this._ctx2d.setSize(this._width, this._height);//更新一下c那边，
//            }
//        }
    }
    get width() {
        return this._width;
    }
    set height(h: number) {
        if(this._isFirst&&HTMLCanvasElement.RS)
        {
            this._h=h;
            return;
        }
        //if (!this._isFirst) {
            if (this._height != h) {
                this._height = h;
                this._context && this._context.setSize(this._width, this._height);
            }
        //}
        //else {
        //    var sh = this._tranform ? this._tranform[3] : 1.0;
        //}
    }
    get height() {
        return this._height;
    }

    get clientWidth() {
        return this._width;
    }

    get clientHeight() {
        return this._height;
    }

    set clientWidth(v) {
        this._width = v;
    }
    set clientHeight(v) {
        this._height = v;
    }
    toBase64(type:string, encoderOptions:number, callback:(data:string)=>void):void{
        if (this._context){
            this._context.toBase64(type, encoderOptions, callback);
        }
    }
    getImageData(x: number, y: number, w: number, h: number, callBack:(data:ArrayBuffer)=>void):void{
        if (this._context){
            this._context.getImageData(x, y, w, h, callBack);
        }
    }
}

(<any>window).HTMLCanvasElement = HTMLCanvasElement;

class HTMLImageElement extends HTMLElement implements NativeObjWrapper {
    _nativeObj: conchImage = null;
    complete: boolean = false;
    _enableMerageInAtlas:boolean = true;
    private srcs:number;
    private obj:any;
    constructor() {
        super();
        this.tagName = 'IMG';
        this._nativeObj = new conchImage();
        this._nativeObj.srcs=0;
        this._nativeObj.obj=this;
    }
    set src(url: string) {
        //url = encodeURI(url);
        this._nativeObj.onerror = this._nativeOnError;//.bind(this);
        this._nativeObj.onload = this._nativeOnload;//.bind(this);
        this._nativeObj.srcs=this._nativeObj.srcs||0;
        if(this._nativeObj.srcs==0)
        {
            this._nativeObj.obj=this;
        }
        this._nativeObj.srcs++;
		if (this.isBase64(url)) {
            let index:number = url.indexOf(';base64,');
            if (index != -1) {
                this._nativeObj.setBase64(url.substr(index + 8));
            }
        }
        else {
			url = location.resolve(url);
			this._nativeObj.setSrc(url);
		}
    }
	private isBase64(url: string):boolean {
        return url.startsWith('data:image/');
    }
    get src(): string {
        return this._nativeObj.src;
    }
    private _nativeOnload() {
        this.srcs=this.srcs||1;
        this.srcs--;
        var temp=this.obj;
        var e = new Event('load');
        temp.complete = true;
        e.target = this;
        temp.onload && temp.onload(e);
        temp.dispatchEvent(e);
        if(this.srcs==0)
        {
            this.obj=null;
        }
    }

    private _nativeOnError(e: number | Object) {
        this.srcs=this.srcs||1;
        this.srcs--;
        var temp=this.obj;
        var evt = new Event('error');
        evt.target = this;
        if (typeof (e) === 'object') {
            evt['extCode'] = (<{ target: Object, extCode: number }>e).extCode;
        } else
            evt['extCode'] = e;
        temp.onerror && temp.onerror(evt);
        temp.dispatchEvent(evt);
        if(this.srcs==0)
        {
           this.obj=null;
        }
    }
    get width(): number {
        if (this._nativeObj) return this._nativeObj.width;
        return 0;
    }
    get height(): number {
        if (this._nativeObj) return this._nativeObj.height;
        return 0;
    }

    get imgId(): number {
        return this._nativeObj.conchImgId;
    }
    public putData(data:ArrayBuffer) {
		this._nativeObj.onerror = this._nativeOnError;
        this._nativeObj.onload = this._nativeOnload;
        this._nativeObj.putData(data);
    }
    putImageData(data:ArrayBuffer,w:number,h:number){
        this._nativeObj.onerror = this._nativeOnError;
        this._nativeObj.onload = this._nativeOnload;
		this._nativeObj.putBitmapData(data,w,h);
	}
    public setPremultiplyAlpha(b:Boolean)
    {
        this._nativeObj.setPremultiplyAlpha(b);
    }
    public conchDestroy(){
        this._nativeObj.destroy();
    }
    set enableMerageInAtlas(value:boolean)
    {
        this._enableMerageInAtlas = value;
        this._nativeObj.enableMerageInAtlas(value);
    }
    get enableMerageInAtlas():boolean
    {
        return this._enableMerageInAtlas;
    }
    public releaseTexture(){
        this._nativeObj.releaseTexture();
    }
}
window["HTMLImageElement"] = HTMLImageElement;

class HTMLInputElement extends HTMLElement implements NativeObjWrapper {

    // _nativeObj: ConchInput;
    // setLeft(l: number):void{}
    // setTop(t: number):void{}
    // setWidth(w: number):void{}
    // setHeight(h: number): void{}
    // setOpacity(o: number):void{}
    // setValue (v: string):void{}
    // getValue ():string{return "test123"};
    // setStyle(s: string):void{}
    // setVisible(b: boolean):void{}
    // focus():void{}
    // blur():void{}
    // setColor (c: string):void{}
    // setFontSize (s: number):void{}
    // setPos(x: number, y: number): void{}
    // setSize(w: number, h: number): void{}
    // setCursorPosition (p: number):void{}
    // setScale(w: number, h: number):void{}
    // setMaxLength(l: number):void{}
    // setType (t: string): void{}
    // setNumberOnly(b: boolean):void{}
    // setRegular(r: string): void{}
    // setFont (f: string) : void{}

    // constructor() {
    //     super();    
    //     this.tagName = "Input";
    // }
    // /*
    // addEventListener(type,fn){
    // 	this._nativeObj.addEventListener(type,fn);
    // }
    // */
    // set left(v) {
    //    // this._nativeObj.left = v;
    // }
    // get left() {
    //     return 0;
    // }
    // set top(v) {
    //    // this._nativeObj.top = v;
    // }
    // get top() {
    //     return 0;
    // }
    // set width(v) {
    //     //this._nativeObj.width = v;
    // }
    // get width() {
    //     return 100;
    // }
    // set height(v) {
    //    // this._nativeObj.height = v;
    // }
    // get height() {
    //     return 20;
    // }
    // set opacity(v) {
    //     //this._nativeObj.opacity = v;
    // }
    // get opacity() {
    //     return 100;
    // }
    // get clientLeft()
    // {
    //     return 0;
    // }
    // set clientLeft(v)
    // {
    //     //this._nativeObj.left = v;
    // }
    // get clientTop()
    // {
    //     return 0;
    // }
    // set clientTop(v)
    // {
    //    // this._nativeObj.top=v;
    // }
    // set clientWidth(v) {
    //     //this._nativeObj.width = v;
    // }
    // get clientWidth() {
    //     return 100;
    // }
    // set clientHeight(v) {
    //     //this._nativeObj.height = v;
    // }
    // get clientHeight() {
    //     return 20;
    // }

    // /**
    //  * 按理说，这个不应单独实现，应该在基类中
    //  * 
    //  * 这个如果定义了，则会导致获得style的时候不从基类取，而是调用这里的，然后没有get，就会返回undefined
    //  */
    // /*
    // set style(v) {
    //     //如果在基类的构造中设置style，则可能this._nativeObj为空。
    //     if( this._nativeObj)
    //         this._nativeObj.style = v.toString();
    // }
    // */

    // set value(v) {
    //     //this._nativeObj.value = v;
    // }
    // get value() {
    //     return "test";
    // }
    // set visible(v) {
    //     //this._nativeObj.visible = v;
    // }
    // get visible() {
    //     return true;
    // }

    _nativeObj: ConchInput;
    setLeft: (l: number) => void;
    setTop: (t: number) => void;
    setWidth: (w: number) => void;
    setHeight: (h: number) => void;
    setOpacity: (o: number) => void;
    setValue: (v: string) => void;
    getValue: () => string;
    setStyle: (s: string) => void;
    setVisible: (b: boolean) => void;
    focus: () => void;
    blur: () => void;
    setColor: (c: string) => void;
    setBgColor: (c:string) => void;
    setFontSize: (s: number) => void;
    setPos: (x: number, y: number) => void;
    setSize: (w: number, h: number) => void;
    setCursorPosition: (p: number) => void;
    setScale: (w: number, h: number) => void;
    setMaxLength: (l: number) => void;
    setType: (t: string) => void;
    setNumberOnly: (b: boolean) => void;
    setRegular: (r: string) => void;
    setFont: (f: string) => void;
    setMultiAble:(b:boolean)=>void;
    setForbidEdit:(b:boolean)=>void;
    constructor() {
        super();
        this.tagName = "Input";
        this._nativeObj = new ConchInput();
        this.setLeft = this._nativeObj.setLeft.bind(this._nativeObj);
        this.setTop = this._nativeObj.setTop.bind(this._nativeObj);
        this.setWidth = this._nativeObj.setWidth.bind(this._nativeObj);
        this.setHeight = this._nativeObj.setHeight.bind(this._nativeObj);
        this.setOpacity = this._nativeObj.setOpacity.bind(this._nativeObj);
        this.setValue = this._nativeObj.setValue.bind(this._nativeObj);
        this.getValue = this._nativeObj.getValue.bind(this._nativeObj);
        this.setStyle = this._nativeObj.setStyle.bind(this._nativeObj);
        this.setVisible = this._nativeObj.setVisible.bind(this._nativeObj);
        this.focus = this._nativeObj.focus.bind(this._nativeObj);
        this.blur = this._nativeObj.blur.bind(this._nativeObj);
        this.setColor = this._nativeObj.setColor.bind(this._nativeObj);
        this.setFontSize = this._nativeObj.setFontSize.bind(this._nativeObj);
        this.setPos = this._nativeObj.setPos.bind(this._nativeObj);
        this.setSize = this._nativeObj.setSize.bind(this._nativeObj);
        this.setCursorPosition = this._nativeObj.setCursorPosition.bind(this._nativeObj);
        this.setScale = this._nativeObj.setScale.bind(this._nativeObj);
        this.setMaxLength = this._nativeObj.setMaxLength.bind(this._nativeObj);
        this.setType = this._nativeObj.setType.bind(this._nativeObj);
        this.setNumberOnly = this._nativeObj.setNumberOnly.bind(this._nativeObj);
        this.setRegular = this._nativeObj.setRegular.bind(this._nativeObj);
        this.setFont = this._nativeObj.setFont.bind(this._nativeObj);
        this.setMultiAble=this._nativeObj.setMultiAble.bind(this._nativeObj);
        this.setForbidEdit=this._nativeObj.setForbidEdit.bind(this._nativeObj);

        if(this._nativeObj.setBgColor)
            this.setBgColor = this._nativeObj.setBgColor.bind(this._nativeObj);
        else
            this.setBgColor = function(c:string) {}
    }
    /*
	addEventListener(type,fn){
		this._nativeObj.addEventListener(type,fn);
	}
    */
    set maxLength(v: number) {
        this.setMaxLength(v);
    }

    set left(v) {
        this._nativeObj.left = v;
    }
    get left() {
        return this._nativeObj.left;
    }
    set top(v) {
        this._nativeObj.top = v;
    }
    get top() {
        return this._nativeObj.top;
    }
    set width(v) {
        this._nativeObj.width = v;
    }
    get width() {
        return this._nativeObj.width;
    }
    set height(v) {
        this._nativeObj.height = v;
    }
    get height() {
        return this._nativeObj.height;
    }
    set opacity(v) {
        this._nativeObj.opacity = v;
    }
    get opacity() {
        return this._nativeObj.opacity;
    }
    get clientLeft() {
        return this._nativeObj.left;
    }
    set clientLeft(v) {
        this._nativeObj.left = v;
    }
    get clientTop() {
        return this._nativeObj.top;
    }
    set clientTop(v) {
        this._nativeObj.top = v;
    }
    set clientWidth(v) {
        this._nativeObj.width = v;
    }
    get clientWidth() {
        return this._nativeObj.width;
    }
    set clientHeight(v) {
        this._nativeObj.height = v;
    }
    get clientHeight() {
        return this._nativeObj.height;
    }

    /**
     * 按理说，这个不应单独实现，应该在基类中
     * 
     * 这个如果定义了，则会导致获得style的时候不从基类取，而是调用这里的，然后没有get，就会返回undefined
     */
    /*
    set style(v) {
        //如果在基类的构造中设置style，则可能this._nativeObj为空。
        if( this._nativeObj)
            this._nativeObj.style = v.toString();
    }
    */

    set value(v) {
        this._nativeObj.value = v;
    }
    get value() {
        return this._nativeObj.value;
    }
    set visible(v) {
        this._nativeObj.visible = v;
    }
    get visible() {
        return this._nativeObj.visible;
    }

    addEventListener(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void {
        super.addEventListener(type, listener, useCapture);
        var _t = this;
        this._nativeObj.addEventListener(type, function (e) {
            var ev = new _lbEvent(type);
            ev.target = this;
            _t.dispatchEvent(ev);
        });
    }
}

let CanPlayTypeResult = {
    Empty : "",
    Maybe : "maybe",
    Probably : "probably"
};

class HTMLMediaElement<ConchMedia extends any> extends HTMLElement implements NativeObjWrapper {
    _nativeObj:ConchMedia;
    _src:string = "";

    constructor(NativeMediaCreator : {new() : ConchMedia;}) {
        super();
        this._nativeObj = new NativeMediaCreator();
    }

    // network state
    set src(val) {
        this._src = val;
        //this._src = encodeURI(this._src);
        this._nativeObj.src = window.location.resolve(this._src);
    }

    get src() {
        return this._nativeObj.src;
    }

    get currentSrc():string {
        return this._src;
    }

    canPlayType(type: string): string {
        return CanPlayTypeResult.Empty;
    }

    load(): void {
    }

    // playback state
    set currentTime(v:number){
        this._nativeObj.currentTime = v;
    }

    get currentTime():number{
        return this._nativeObj.currentTime;
    }

    play():void { 
        this._nativeObj.play(); 
    }

    pause():void { 
        this._nativeObj.pause(); 
    }

    set autoplay(v:boolean) {
        this._nativeObj.autoplay = v;
    }
    get autoplay() {
        return this._nativeObj.autoplay;
    }

    set loop(v:boolean) {
        this._nativeObj.loop = v;
    }

    get loop():boolean {
        return this._nativeObj.loop;
    }

    // controls
    set volume(v:number) {
        this._nativeObj.volume = v;
    }
    get volume():number {
        return this._nativeObj.volume;
    }

    set muted(v:boolean) {
        this._nativeObj.muted = v;
    }

    get muted() {
        return this._nativeObj.muted;
    }

}

class HTMLMetaElement extends HTMLElement {
    private _name: string;
    /**
    * Sets or retrieves the character set used to encode the object.
    */
    charset: string;
    /**
      * Gets or sets meta-information to associate with httpEquiv or name.
      */
    content: string;
    /**
      * Gets or sets information used to bind the value of a content attribute of a meta element to an HTTP response header.
      */

    get httpEquiv(): string {
        return this["http-equiv"];
    }
    /**
      * Sets or retrieves the value specified in the content attribute of the meta object.
      */
    // name: string;

    set name(n: string) {
        this._name = n;
        document._elements[n] = document._elements[n] || [];
        document._elements[n].push(this);
    }

    get name(): string {
        return this._name;
    }
    /**
      * Sets or retrieves a scheme to be used in interpreting the value of a property specified for the object.
      */
    scheme: string;
    /**
      * Sets or retrieves the URL property that will be loaded after the specified time has elapsed.
      */
    url: string;

    constructor() {
        super();
        this.tagName = "META";
    }

}

class HTMLAudioElement extends HTMLMediaElement<ConchAudio> {
    constructor() {
        super(ConchAudio);
        this.tagName = "AUDIO";
    }
    readyState: number = 0;
    setLoop(loop) {
        this._nativeObj.setLoop(loop);
    }

    stop() {
        this._nativeObj.stop();
    }
    //TODO 去掉c中的，只要能触发就行
    /*
	addEventListener( name,func )
	{
		this.conchAudio.addEventListener( name,func );
	}
    */

    set muted(v) {
        this._nativeObj.muted = v;
    }
    get muted() {
        return this._nativeObj.muted;
    }
	get duration(){
		return this._nativeObj.duration;
    }
    addEventListener(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void {
        super.addEventListener(type, listener, useCapture);
        var _t = this;
        this._nativeObj.addEventListener(type, function (e) {
            var ev = new _lbEvent(type);
            ev.target = this;
            if(type=="canplaythrough")
            _t.readyState=4;
            _t.dispatchEvent(ev);
        });
    }
}

class HTMLVideoElement extends HTMLMediaElement<ConchVideo> {
    _src:string = "";
    _removeEventListener:(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean)=>void;

    constructor() {
        super(ConchVideo);
        this.tagName = "VIDEO";
        this._nativeObj._setDispatchEventFunc(this._dispatchEventByType.bind(this));

        this._removeEventListener = this.removeEventListener;
        this.removeEventListener = this.removeEventListenerNew;
    }

    appendChild(node: any):Node
    {
        if(node.src)
        {
            this.src = node.src;
        }
        return super.appendChild(node);
    }

    // media
    get readyState():number {
        return this._nativeObj.readyState;
    }

    canPlayType(val:string):string {
        let result:boolean = this._nativeObj.canPlayType(val);
        return result ? CanPlayTypeResult.Maybe : CanPlayTypeResult.Empty;
    }
    
    get paused(): boolean { return this._nativeObj.paused; }

    get duration():number { return this._nativeObj.duration; }

    //override
    get src() { return this._src; }
    set src(val:string) { super.src = val; }
    load() : void {
        this._nativeObj.load();
    }


    // video
    set width(val: number) { this._nativeObj.width = val; }
    get width(): number { return this._nativeObj.width; }

    set height(val:number) { this._nativeObj.height = val; }
    get height():number { return this._nativeObj.height; }

    set videoWidth(val:number) { this._nativeObj.videoWidth = val; }
    get videoWidth():number { return this._nativeObj.videoWidth; }

    set videoHeight(val:number) { this._nativeObj.videoHeight = val; }
    get videoHeight():number { return this._nativeObj.videoHeight; }


    // internal
    get clientLeft()
    {
        return this._nativeObj.x;
    }
    
    set clientLeft(val:number)
    {
        this._nativeObj.x = val;
    }
    
    get clientTop()
    {
        return this._nativeObj.y;
    }
    
    set clientTop(val:number)
    {
        this._nativeObj.y = val; 
    }

    addEventListener(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void {
        super.addEventListener(type, listener, useCapture);
        this._nativeObj.addEvent(type);
    }

    removeEventListenerNew(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void {
        this._removeEventListener(type, listener, useCapture);
        this._nativeObj.removeEvent(type);
    }

    _dispatchEventByType(type:string) {
        var ev = new _lbEvent(type);
        ev.target = this;
        this._dispatchEvent(ev);
    }

    _destroy()
    {
        this._nativeObj._releaseHandler();
    }
}
window["HTMLVideoElement"] = HTMLVideoElement;

class HTMLBodyElement extends HTMLElement {
    constructor() {
        super();
        this.tagName = "BODY";
    }
    
    get clientHeight()
    {
        return window.innerHeight;
    }
    
    set clientHeight(h:number)
    {
        
    }
    
    get clientWidth()
    {
        return window.innerWidth;
    }
    
    set clientWidth(w:number)
    {
        
    }
    appendChild(newChild:Node):Node {
        if(newChild instanceof HTMLCanvasElement){
            newChild.setFirst();
        }
        return super.appendChild(newChild);
    }
}