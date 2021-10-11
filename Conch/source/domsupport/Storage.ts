///<reference path="./ES6NoDOM.d.ts" />
/**
 * 问题：现在不支持 [] 方式的访问
 * 不支持length
 */
class Storage {
    private storagePath = conchConfig.getStoragePath();
    private filename = '';
    private db = {};//只保存了值，因为有根据idx索引的需求
    private _len:number=0;
    fileNamePre:string;
    get length():number
    {
        return this._len;
    }
    getItem(key: string): any {
        return this.db[key]||null;
    }
    key(index: number): string {
        var keys:Array<string>=Object.keys(this.db);
        keys.sort();
        return keys[index]||null;
    }
    removeItem(key: string): void {
        if(this.db[key])this._len--;
        delete this.db[key];
        this.savedb();
    }
    _setItem(key: string, data: string): void
    {
        if(this.db[key]==null)this._len++;
        this.db[key]=data;
        Object.defineProperty(this as any,key, {
            get: function(){
                return this.db[key];
            },
            enumerable: true,
            configurable: true
        });

    }
    setItem(key: string, data: string): void {
        this._setItem(key,data);
        this.savedb();
    }
    
    constructor() {
        //Object.observe(this,this.onChange.bind(this));
    }
    /**
     * 根据url来创建一个。在window初始化的时候做。
     */
    create(url: string):Storage {
        url=location.fullpath;
        if (location.protocol=="file:") {
            this.filename = url.substring(8).replace(/:/g, '_').replace(/[\\\/]/g, '__')
            this.fileNamePre=this.storagePath + '/'+this.filename;
        } else{
            this.fileNamePre=this.storagePath + '/'+url.split('/')[2].replace(':', '_');
        }
        this.filename =  this.fileNamePre + '.txt';
        var strdb = readFileSync(this.filename, 'utf8') || '{}';
        var db = JSON.parse(strdb);
        for(var v in db){
            this._setItem(v,db[v]);
        }
        return this;
    }
    onChange(changes) {
        if (changes && changes.length) {
            //save
        }
    }
    clear() {
        this.db = {};
        this.savedb();
    }
    savedb() {
        writeStrFileSync(this.filename, JSON.stringify(this.db));
    }
}
window["Storage"]=Storage;
class WindowLocalStorage {
    localStorage = new Storage();
}

class WindowSessionStorage  {
    getItem(i:string)
    {
        return this[i]||null;
    }
    setItem(i:string,b:Object)
    {
        this[i]=b;
    }
    
    //todo  sessionStorage 
}

window.loadLocalStorage = function( url ):Storage{
     return new Storage().create(url);
};


class _Cookie{
    key:string;
    value:string;
    domain:string;
    expires:Date;
    static cookies:Array<_Cookie>=[];
    static sreg:RegExp=new RegExp("([^=]*)\\s*=\\s*([^;]*)\\s*;\\s*(expires\\s*=\\s*(.{23,26}GMT)|)");
    
    constructor(){
        this.domain="";
    }

    static addCookie(s:string):_Cookie{
       var result= _Cookie.sreg.exec(s);
       if(result==null){
           console.warn("设置cookie无效");
           return null;
       }
       else{
           var temp:_Cookie=new _Cookie();
           temp.key=result[1];
           temp.value=result[2];
           if(result[4]){
             temp.expires=new Date(result[4]);
           }
           return temp;
       }
    }
      
    static pushCookie(c:_Cookie):boolean{
        //TO-DO
        if(!c.isValid())return false;
        for(var i:number=0,len:number=_Cookie.cookies.length;i<len;i++)
        {
            var temp:_Cookie=_Cookie.cookies[i];
            if(temp.key==c.key){
                _Cookie.cookies[i]=c;
                return true;
            }
        }
        _Cookie.cookies.push(c);
        return true;
    } 
    
    static flush():void
    {
        writeStrFileSync(document._cookiePath,_Cookie.getString("in","\""));
    }
    
    static getString(type:string,joinstr:string):string{
        var a:Array<string>=[];
        for(var i:number=0,len:number=_Cookie.cookies.length;i<len;i++)
        {
            var temp:_Cookie=_Cookie.cookies[i];
            if(temp.isValid()){
                a.push(temp.toLocalString(type));
            }
            else{
                _Cookie.cookies.slice(i,1);
                i--;
                len--;
            }
        }
        return a.join(joinstr);
    }
    
    static toLocalString():string{
       return _Cookie.getString("out","; ");
    }

  
    isValid():boolean{
       if(this.expires&&(this.expires.getTime()<Date.now())){
           return false;
       }
       return true;
    }
    
    static init(s:string){
        if(s)
        {
            var temp:Array<string>=s.split("\"");
            for(var i:number=0,len:number=temp.length;i<len;i++){
                 var t:_Cookie= _Cookie.addCookie(temp[i]);
                 console.warn(">>>>>>>>>>addCookie"+temp[i]);
                 if(t)
                 {
                     t.value=decodeURIComponent(t.value);
                     t.domain=decodeURIComponent(t.domain);
                 }
                 _Cookie.pushCookie(t);   
            }
        }
    }
    
    toLocalString(type:string):string{
        switch (type) {
            case "in":
                if (this.expires) {
                    return this.key+"="+encodeURIComponent(this.value)+"; expires="+this.expires.toGMTString()+"; domain="+encodeURIComponent(this.domain);
                }
                else {
                    return this.key+"="+encodeURIComponent(this.value) + "; domain="+encodeURIComponent(this.domain);
                }
            case "out":
                return this.key+"="+this.value;
            default:
                return null;
        }
    }
}