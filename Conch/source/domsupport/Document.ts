///<reference path="./LayaConchRuntime.d.ts" />
class NodeList extends Array {
    constructor() {
        super();
    }
    item(index: number): Node {
        return this[index];
    }
}

interface NodeListOf<TNode extends Node> extends NodeList {
    length: number;
    item(index: number): TNode;
    [index: number]: TNode;
}

/**
 * 这个是为了生成的.d.ts更方便
 */
interface Document {
    createElement(tagName: "div"): HTMLDivElement;
    createElement(tagName: "img"): HTMLImageElement;
    createElement(tagName: "virtualBitmap"): any;
    createElement(tagName: "audio"): HTMLAudioElement;
    createElement(tagName: "input"): HTMLInputElement;
    createElement(tagName: "textarea"): HTMLInputElement;
    createElement(tagName: "canvas"): HTMLCanvasElement;
    createElement(tagName: "script"): HTMLScriptElement;
    createElement(tagName: "meta"): HTMLMetaElement;
    createElement(tagName: string): HTMLElement | any;
}

class HTMLHeadElement extends HTMLElement {
    profile: string;
    constructor() {
        super();
        this.tagName='head';
        this.__visible=false;
    }
}


class HTMLCollection extends Array {
    /**
      * Sets or retrieves the number of objects in a collection.
      */
    length: number;
    /**
      * Retrieves an object from various collections.
      */
    item(nameOrIndex?: any, optionalIndex?: any): Element {
        return this[nameOrIndex];
    }
    /**
      * Retrieves a select object or an object from an options collection.
      */
    namedItem(name: string): Element {
        return null; //TODO
    }
    //[index: number]: Element;
}

class Document extends Node implements GlobalEventHandlers, NodeSelector, DocumentEvent {
    private createMap = new Map<string, Function>();
    //private _inputSingleObj: HTMLInputElement;//editbox的话，只允许一个。
    private _frameEndEvt=new Event('frameend');
    _cookiePath:string;
    _topElement: HTMLElement;
    body: HTMLElement;
    documentElement: HTMLElement;
    head: HTMLHeadElement;
    location: Location;
    defaultView:Window;
    referrer:string;
    //downloadlist:Array<any>=[];
    scriptTextList:Array<any>=[];
    private _loading:number=0;
    private _evalNum:number=0;
   // private _isdownload:boolean;
    /**
     * 用cache来提高效率，在这个环境下事件的路径基本是不变的。
     */
    private _eventPathCache:Node[]=null;
    /**
      * Returns a reference to the collection of elements contained by the object.
      */
    all: HTMLCollection=new HTMLCollection() ;
    
    _elements:Array<any>=[];

    querySelector: (selectors: string) => Element; //接口的空函数
    querySelectorAll: (selectors: string) => NodeListOf<Element>;//接口的空函数
    createEvent: (eventInterface: string) => Event;//为了 DocumentEvent 接口的空函数tes
    onpointercancel: (ev: PointerEvent) => any;
    onpointerdown: (ev: PointerEvent) => any;
    onpointerenter: (ev: PointerEvent) => any;
    onpointerleave: (ev: PointerEvent) => any;
    onpointermove: (ev: PointerEvent) => any;
    onpointerout: (ev: PointerEvent) => any;
    onpointerover: (ev: PointerEvent) => any;
    onpointerup: (ev: PointerEvent) => any;
    
    constructor() {
        super();
        window.document=this;
        this.defaultView=window;
        var cm = this.createMap;
        cm.set('div', this.create_div);
        cm.set('img', this.create_img);
        cm.set('image', this.create_img);
        cm.set('canvas', this.create_canvas);
        cm.set('audio', this.create_audio);
        cm.set('input', this.create_input);
        cm.set('textarea', this.create_input);
        cm.set('video',this.create_video);
        cm.set('script',this.create_script);
        cm.set('meta',this.create_meta);
        var html = new HTMLElement();
        var ww = getInnerWidth();
        var wh = getInnerHeight();
        html.clientWidth = ww;
        html.clientHeight = wh;
        html.tagName = "HTML";
        html.ownerDocument = this;
        this.documentElement = html;
        this._topElement = html;
        this.appendChild(this.documentElement);
        var body = new HTMLBodyElement();
        body.ownerDocument = this;
        this.body = body;
        this.documentElement.appendChild(this.body);
        this.head = new HTMLHeadElement();
        this.documentElement.appendChild(this.head);
        this.dispatchEvent = this._dispatchEvent.bind(this);
        this._frameEndEvt.bubbles=false;
        this.nodeType=9;
        this.location=window.location;
        var _t:Document=this;
        var temp;
        /*this.addEventListener("listDownload",function(e){
            if(_t._isdownload)return;
            _t._isdownload=true;
            if(_t.downloadlist.length==0){
                _t._isdownload=false;
                return;
            }
            _t._downloadAysn();
        });*/
    }
    
    public setReferrer(s):void
    {
        this.referrer=s;
    }
    
    public  uploadScript(d:any)
    {
        var _t:Document=this;
        d.i=this._loading;
        this._loading++;
        if(d.src){
              console.log("_downloadAysn:temp.src"+d.src);
                    window.downloadfile(d.src,false,function(data){
                         d._stext=data+"\n//@ sourceURL="+d.src;
                         _t._downloadOk(d);
                    },function(){
                        var e:Event=new Event("error");
                        e.target=e.currentTarget=d.obj;
                        d.obj.onerror&& d.obj.onerror(e);
                        //_t._evalNum++;
                        //_t._downloadOk(d);
                    });
         }
         else{
            d._stext=d.text;
            _t._downloadOk(d);
         }
    }
    
    private  _downloadOk(d:any):void
    {
        this.scriptTextList[d.i]=d;
        for(var i:number=this._evalNum,len:number=this.scriptTextList.length;i<len;i++)
        {
            var t:any=this.scriptTextList[i];
            if(!t) return;
            console.log(">>>>>>>>>>>>>>>eval"+t.src);
            var t1=Date.now();
            window.evalJS(t._stext);
            console.log(">>>>>>>>>>>>>>>>>eval take time:"+(Date.now()-t1));
            var e:Event=new Event("load");
            e.target=e.currentTarget=t.obj;
            t.obj.onload&& t.obj.onload(e);
            this._evalNum++;
        }
        if(this._loading==this._evalNum){
            this._loading=this._evalNum=0;
            this.scriptTextList.length=0;
        }
    }
    
   /* private  _downloadAysn():void
    {
        console.log(">>>>>>>>"+this.downloadlist.length);   
        var  temp=this.downloadlist.shift();
        var _t:Document=this;
         if(temp)
         {
                if(temp.src)
                {
                    console.log("_downloadAysn:temp.src"+temp.src);
                    window.downloadfile(temp.src,false,function(data){
                         var e:Event=new Event("load");
                         e.target=e.currentTarget=temp.obj;
                         window.eval(data+"\n//@ sourceURL="+temp.src);
                        temp.obj.onload&& temp.obj.onload(e);
                        if(_t.downloadlist.length!=0)
                            _t._downloadAysn();
                        else{
                           _t._isdownload=false;
                        }
                    },function(){
                        var e:Event=new Event("error");
                        e.target=e.currentTarget=temp.obj;
                        temp.obj.onerror&& temp.obj.onerror(e);
                    });
                }
                else
                {
                    window.eval(temp.text);
                    if(_t.downloadlist.length!=0){
                       _t._downloadAysn();
                    }
                    else{
                        _t._isdownload=false;
                    }
                }
            }
    }*/
    
    //addEventListener:(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean)=>void;
    pickElement(screenx: number, screeny: number): HTMLElement {
        //var top = this._topElement;
        //console.log(top.clientLeft+','+top.clientWidth);
        return this._topElement;
    }
    createElement(tagName: string): HTMLElement {
        tagName=tagName.toLowerCase();
        var f = this.createMap.get(tagName);
        var ret: HTMLElement = null;
        if (f) {
            ret = f.call(this);
        } else {
            //throw 'unsupported tagname:' + tagName;
            return new div();
        }
        //ret.ownerDocument = this;
        return ret;
    }
    createElementNS(tagName:string):HTMLElement{
        return this.createElement(tagName);
    }
    private create_div(): HTMLDivElement {
        var ret = new HTMLDivElement();
        ret.ownerDocument = this;
        return ret;
    }
    private create_img(): HTMLImageElement {
        var ret = new HTMLImageElement();
        ret.ownerDocument = this;
        return ret;
    }
    private create_canvas(): HTMLCanvasElement {
        var ret = new HTMLCanvasElement();
        ret.ownerDocument = this;
        return ret;
    }
    private create_audio(): HTMLAudioElement {
        var ret = new HTMLAudioElement();
        ret.ownerDocument = this;
        return ret;
    }
    private create_input(): HTMLInputElement {
        var rs=new HTMLInputElement();
        rs.ownerDocument = this;
        return rs;
    }
    private create_video():HTMLVideoElement{
        var ret=new HTMLVideoElement();
        ret.ownerDocument=this;
        return ret;
    }
    private create_script():HTMLScriptElement{
        var ret=new HTMLScriptElement();
        ret.ownerDocument=this;
        return ret;
    }
    private create_meta():HTMLMetaElement{
        var ret=new HTMLMetaElement();
        ret.ownerDocument=this;
        return ret;
    }
    
    createDocumentFragment():HTMLDivElement
    {
        //TODO
        return new HTMLDivElement();
    }
    
    onframeend(){
        //this._dispatchEvent(this._frameEndEvt);
        this._frameEndEvt.eventPhase = Event.AT_TARGET;//只要at
        super._fireEventListeners(this._frameEndEvt);
    }
    _dispatchEvent(evt: Event): boolean{
        //
        var ancestores: Node[] = null;
        if (evt.target)
            ancestores = (<Node>evt.target).getAncestorsNode();
        if (ancestores == null || ancestores.length == 0) {
            //只有document，直接触发。
            return super._dispatchEvent(evt);
        }
        var ancLen = ancestores.length;

        //捕获阶段
        evt.eventPhase = Event.CAPTURING_PHASE;
        var stop = (function(): boolean {
            window.dispatchEvent(evt);
            if (evt._propagationStopped)
                return true;
            for (var i = ancLen - 1; i >= 0; i--) {
                var cnode = ancestores[i];
                evt.currentTarget = cnode;
                cnode.fireEventListeners(evt);
                if (evt._propagationStopped) {
                    return true;
                }
            }
            return false;
        })();

        //at 阶段
        if (!stop) {
            evt.eventPhase = Event.AT_TARGET;
            evt.currentTarget = evt.target;
            stop = (function(): boolean {
                evt.target.fireEventListeners(evt);
                if (evt._propagationStopped)
                    return true;
                return false;
            })();
        }

        //冒泡阶段
        if(!stop && !evt.cancelable){
            evt.eventPhase = Event.BUBBLING_PHASE;    
            for (var i = 0; i <ancLen; i++) {
                var cnode = ancestores[i];
                evt.currentTarget = cnode;
                cnode.fireEventListeners(evt);
                if (evt._propagationStopped) {
                    stop = true;
                    break;
                }
            }
            if( !stop ){
                evt.currentTarget=window;
                window.dispatchEvent(evt);
            }
        }
        
        //是否阻止缺省处理
        if( evt.preventDefault ){
            
        }
        return true;
    }
    //cookie:string;
    set cookie(v: string) {
        var t:_Cookie=_Cookie.addCookie(v);
        if(t&&_Cookie.pushCookie(t)){
            _Cookie.flush();
        }
    }
    get cookie(): string {
        return _Cookie.toLocalString();
    }
    loadCookie(): boolean {   
        this._cookiePath=window.localStorage.fileNamePre+"_cookie.txt";
        var temp=readFileSync(this._cookiePath,"utf8");
        //alert(temp);
        _Cookie.init(temp);
        return true;
    }
    open(url?: string, name?: string, features?: string, replace?: boolean): Document {
        throw 'not implements'
    }
    getElementsByTagName(tagname: string): NodeListOf<Element> {
        var d=new NodeList();
        if("body"==tagname)
           d.push(this.body);
        else if("head"==tagname)
           d.push(this.head);
        return <NodeListOf<Element>>d;
    }
    onkeydown(ev: KeyboardEvent): any {

    }
    onkeypress(ev: KeyboardEvent): any {

    }
    onkeyup(ev: KeyboardEvent): any {

    }

    onmousedown(ev: MouseEvent): any {

    }
    onmousemove(ev: MouseEvent): any { }
    onmouseout(ev: MouseEvent): any { }
    onmouseover(ev: MouseEvent): any { }
    onmouseup(ev: MouseEvent): any { }
    onmousewheel(ev: MouseWheelEvent): any { }

    ontouchcancel(ev: TouchEvent): any { }
    ontouchend(ev: TouchEvent): any { }
    ontouchmove(ev: TouchEvent): any { }
    ontouchstart(ev: TouchEvent): any { }
    
    getElementById(elementId: string): HTMLElement{
        for( var i=0,sz=this.all.length; i<sz; i++){
            var c:Element = this.all[i];
            if(c.id===elementId)
                return <HTMLElement>c;
        }
        //debugger;
       // throw 'not implemented';
        return null;
    }
    getElementsByClassName(classNames: string): NodeListOf<Element>{
        throw 'not implemented';
        //return null;
    }
    
    getElementsByName(name:string):Array<any>
    {
       return   document._elements[name]||[];
    }
    
    write(value):void
    {
        console.log("The document don't support write function!!!");
    }

}
applyMixins(Document, [Node, GlobalEventHandlers, NodeSelector, DocumentEvent]);