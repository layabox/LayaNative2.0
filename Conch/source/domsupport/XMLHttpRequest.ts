class XMLHttpRequest extends EventTarget{
	public static UNSENT:number = 0;
	public static OPENED:number = 1;
	public static HEADERS_RECEIVED:number = 2;
	public static LOADING:number = 3;
	public static DONE:number = 4;
    private xhr:_XMLHttpRequest;
    private _responseText:string;
    private _response:any;
    private _responseType:string;
    private _readyState:number;
    private _status:number;
    private _url:string;
    private _async:boolean;
    private _method:string;
    private _onloadcb:(evt:Event)=>any;
    private _onrchgcb:(evt:Event)=>any;
    private _head:any;
    private _hasReqHeader=false;
    onerror:(evt:Event)=>any;
    withCredentials:boolean=false;
    
    constructor(){
        super();
        this.xhr=new _XMLHttpRequest();
        this._readyState=0;
        this._responseText=this._response=this._responseType=this._url="";
        this._responseType="text";
        this._method="GET";
        this.xhr._t=this;
        this.xhr.set_onreadystatechange(function(r:number):void{
            var _t=this._t;
            if (r == 1) {
	           _t._readyState = 1;
            }
            if (_t._onrchgcb ) {
                //标准的回调是没有参数的。
                var e=new _lbEvent("readystatechange");
                e.target=_t;
                _t._onrchgcb(e);
            }
            //模拟事件
            var ev;
            if(_t._status==200){
                ev=new _lbEvent("load");
                ev.target=_t;
                _t.dispatchEvent(ev);
                
            }else if(_t._status==404){
                ev=new _lbEvent("error");
                ev.target=_t;
                _t.dispatchEvent(ev);
            }
                
        });
    }
    
    setRequestHeader(name:string,value:string):void{
        this.xhr.setRequestHeader(name,value);
        this._hasReqHeader=true;
    }
    
    getAllResponseHeaders(){
        return this._head;
    }
    
    get responseText()
    {
        return this._responseText;
    }
    
    get response()
    {
        return this._response;
    }
    
    get responseType()
    {
        return this._responseType;
    }
    
    set responseType(type:string)
    {
        this._responseType=type;
        if( type=='blob'){
            this.xhr.responseTypeCode=4;
        }
        else if(type=='arraybuffer'){
            this.xhr.responseTypeCode=5;
        }else{
            this.xhr.responseTypeCode= 1;
        }        
    }
    
    get url()
    {
        return this._url;
    }
    
    get async()
    {
        return this._async;
    }
    
    get readyState():number
    {
        return this._readyState;
    }
    
    get status():number
    {
        return this._status;
    }
    
    private _loadsus():void
    {
         var e=new _lbEvent("load");
         e.target=this;
         this._onloadcb(e);
    }
    
    set onreadystatechange(listen:(evt:Event)=>any)
    {
        this._onrchgcb=listen;
        if(listen==null)return;
        if(this._readyState!=0)
        {
            var e=new _lbEvent("readystatechange");
            e.target=this;
            this._onrchgcb(e);
        }
    }
    get onreadystatechange():(evt:Event)=>any
    {
        return this._onrchgcb;
    }
    
    set onload(listen:((evt:Event)=>any))
    {
        this._onloadcb=listen;
        if(listen==null)return;
        if(this._readyState==4&&this._status==200)
        {
            this._loadsus();
        }
    }
    get onload():(evt:Event)=>any
    {
        return this._onloadcb;
    }
    getResponseHeader()
	{
		return this._head;
	}
	setResponseHeader=function(name,value)
	{
		this._head=value;
	}
    
    open(type:string,url:string,async:boolean)
    {
        //url = encodeURI(url);
        console.log('xhr.'+type+' url='+url);
        if(!url)return;
		type=type.toUpperCase();
		async=true;
        url=location.resolve(url);
		this._method=(type==='POST'?'POST':'GET');
		this._url = url;
		this._async = (async == null || async == undefined || async == true) ? true : false;
		this.xhr._open(this._method, this._url, this._async);
    }
    
    overrideMimeType(mime:string):void
    {
        if(this._responseType=="text"||this._responseText=="")
                this._responseType="arraybuffer";
        this.xhr.mimeType="1";
    }
    
    send(body:any):void
    {
        if(body){
            if(body instanceof ArrayBuffer|| ArrayBuffer.isView(body) || body instanceof DataView )
                this._responseType='arraybuffer';
            else if( body instanceof Object){
                body = JSON.stringify(body);
            }
        }
        this.xhr._t=this;
        var onPostLoad:any=function(buf,strbuf)
        {
            var _t=this._t;
            if (_t.responseType == 'arraybuffer')
            {
                _t._response = buf;
                _t._responseText = strbuf;
            }
            else
            {
                _t._response=_t._responseText=buf;
            }
            //console.log('xhr post ret:'+_this.responseText);
            _t._readyState = 4;
            _t._status = 200;
            _t.xhr._changeState(4);
            if(_t._onloadcb)
            {
                _t._loadsus();
            }
            onPostLoad.ref=onPostError.ref=null;
        }
        var onPostError:any=function(e1:number, e2:number){
            var _t=this._t;//不用this是因为jsc有泄露
            _t._readyState = 4;
            _t._status = e2;
            _t.xhr._changeState(4);
            if(_t.onerror)
            {
                var ev=new _lbEvent("error");
                ev.target=_t;
                ev['ecode1']=e1;   //curl返回值
                ev['ecode2']=e2;   //http返回值。
                _t.onerror(ev);
            }
            onPostLoad.ref=onPostError.ref=null;
        }        
        if(this._method=='POST' && body){
            onPostLoad.ref=onPostError.ref=this.xhr;
		    this.xhr.setPostCB( onPostLoad,onPostError );
			this.xhr.postData(this.url,body);
		}else if( this._hasReqHeader){
            onPostLoad.ref=onPostError.ref=this.xhr;
		    this.xhr.setPostCB( onPostLoad,onPostError );
            this.xhr.getData(this.url)
        }
		else
		{
			var file = new conch_File(this.url);
			var fileRead = new FileReader();
			fileRead.sync = !this.async;
			if( this._responseType == "text" || this._responseType == "TEXT" ){
				fileRead.responseType = 0;
			}else if( this._responseType == "arraybuffer" ){
				fileRead.responseType = 1;
			}else{
				console.log("XMLhttpRequest 暂不支持的类型 responseType=" + this.responseType );
			}
            fileRead._t=this;
			fileRead.onload = function () {
                var _t=this._t;
				if( _t._responseType == "arraybuffer" ){
					_t._response = this.result;
                    //_t._responseText=?
				}else{
                    _t._response=_t._responseText = this.result;
                    if(_t._responseType=="json")
                    {
                        _t._response=JSON.parse(this.result);
                    }
				}
				if(_t.xhr.mimeType)
				{
				   var u8arr = new Uint8Array(_t._response);
				   var strret="";
					u8arr.forEach((v, i, arr) => {
						if (v >=0x80) {
							strret += String.fromCharCode(0xf700 | v);
						}
						else if( v==0 ){
							strret+='\0';
						}
						else {
							strret += String.fromCharCode(v);
						}
					});
					_t._responseText =  strret;
				}
				_t._readyState = 4;
				_t._status = 200;
				_t.xhr._changeState(4);
				if (_t._onloadcb) {
				    _t._loadsus();
				}
				fileRead.onload =null;
				fileRead.onerror = null;
				//_this.onreadystatechange=null;
			}
			fileRead.onerror = function () {
                var _t=this._t;
			    _t._readyState = 4;
			    _t._status = 404;
			    _t.xhr._changeState(4);
                if(_t.onerror)
                {
                    var ev=new _lbEvent("error");
                    ev.target=_t;
                    _t.onerror(ev);
                }
				fileRead.onload =null;
				fileRead.onerror = null;
			}
			if(this.onerror){
                fileRead.setIgnoreError(true);
            }
			if( this.responseType == "arraybuffer" )
				fileRead.readAsArrayBuffer(file);
			else
				fileRead.readAsText(file);
		}
        
    }

    //扩展的
   /* async:boolean;
    method:string;
    evtmap:Object;  //TODO 这个要去掉。*/
}
window.XMLHttpRequest=XMLHttpRequest;