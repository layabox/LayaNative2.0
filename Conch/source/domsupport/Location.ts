class Location {
       /* queries:Object;
    reload(force:boolean):void;
    hash: string;
    hostname: string;
    href: string;
    origin: string;
    pathname: string;
    port: string;
    assign(url: string): void;
    reload(forcedReload?: boolean): void;
    replace(url: string): void;
    toString(): string;*/
    
    // 	getHref():string;
	// setHref(url:string):void;
	// fullpath:string;
	// protocol:string;
	// host:string;
	// search:string;
	// appUrl:string;
    
    private _nativeObj = conch;
    private bk_setHref: Function;
    private _host:string;
    private _hostname:string;
    private _fullpath:string;
    private _pathname:string;
    private _protocol:string;
    private _port:string;
    private _search:string;
    private _href:string;
    private _origin:string;
    
    static __urlCache__:Object={};
    
    hash: string;
    get hostname():string
    {
        return this._hostname;
    }
    get host():string{
        return this._host;
    }
    get fullpath():string
    {
        return this._fullpath;
    }
    
    get pathname():string
    {
        return this._pathname;
    }
    
    get protocol():string
    {
        return this._protocol;
    }
    
    get search():string
    {
        return this._search;
    }
    
    get port():string
    {
        return this._port;
    }
    
    get origin():string
    {
        return this._origin;
    }
     
    get href(): string {
        return this._href;
    }
    set href(url: string) {
        url = this.normalizeUrl(url);
       // conch.showLoadingView(true);
        var oldhref = this._href;
        if (url != oldhref) {
            this.setHref(url);
            if (this._href != oldhref)
                reloadJS(true);
        }
    }
    //未实现
   // origin: string;
   // port: string;
    constructor() {
        //重载setHref
        this.bk_setHref = this._nativeObj.setHref.bind(this._nativeObj);
      //  conch.setLocationObj(this._nativeObj);

    }
    
    setBaseHref(basehref:string):void
    {
        //this._nativeObj.setBaseHref(basehref);
    } 
    
    getBaseHref():string
    {
        //return this._nativeObj.getBaseHref();
        return "";
    }
    
    assign(url: string): void {

    }
    reload(forcedReload?: boolean): void {
         reloadJS(forcedReload);
    }
    replace(url: string): void {

    }
    toString(): string {
        return this._href;
    }

    /**
     * 想在脚本里进行url的解析。所以封装了个setHref函数
     */
    setHref = (url:string) => {
        if (!url || url.length < 8) {
            alert("您的地址不符合要求");
            return;
        }
        var s=new RegExp("(http|file|https)://([^/:]*)(:(\\d+)|)([^?]+)(.*|)","g");
        var result;
        if((result=s.exec(url))!=null){
            this._href=result[0];
            this._hostname=result[2];
            this._host=result[2]+result[3];
            this._pathname=result[5];
            this._port=result[4];
            this._search=result[6];
            this._protocol=result[1]+":";
            this._origin=this._protocol+"//"+this._host;
            var i=this._pathname?this._pathname.lastIndexOf("/"):-1;
            if(i!=-1){
                var temp=this._pathname.substring(0,i);
                this._fullpath=this._origin+temp;
            }
            else{
                this._fullpath=this._origin+ this._pathname;
            }
        }
        else{
            alert("您的地址不符合要求");
        }
        //this._nativeObj.queries = this.parseQuery(url);
        this.bk_setHref(url);
        //对应的localStorage
        //注意必须在 bk_setHref， 因为需要解析结果。
        window.localStorage.create(this._fullpath + '/');

    };

    normalizeUrl(url): string {
        url = url.replace(/\\/g, '/');
        if (url[0] === '/')
            url = 'file://' + url;
        else if (url[1] === ':')
            url = 'file:///' + url;
        return url.trim();
    }

    parseQuery(url): Object {
        var ret = {};
        var p1 = url.indexOf('?');
        if (p1 < 0)
            return null;
        var q = url.substr(p1 + 1);
        q && q.split('&').forEach((v, i, a) => {
            var kv = v.split('=');
            if (kv.length === 2) {
                ret[kv[0].trim()] = kv[1].trim();
            }
        });
        return ret;
    }
    resolve(fileName:string):string{
        //从缓存里取出此文件名
        var urlcache = Location.__urlCache__[fileName];
        //如果已经缓存过，直接返回结果
        if (urlcache != null) return urlcache;

        if (fileName == null) {
            return "";
        }
        if(fileName.indexOf("//")==0)
        {
            return this.protocol+fileName;
        }
        if(fileName.indexOf("file:///")==0||fileName.indexOf("http://")==0||fileName.indexOf("https://")==0)return fileName;
        if ((fileName.charAt(1) == ':' && fileName.charAt(2) == '/'))
            fileName = "file://" + fileName;

        //绝对路径的话，不能从index.html所在路径开始，否则与浏览器不一致
        if (fileName.charAt(0) == "/") {
            return this._origin + fileName;
        }

        var basePath = this._fullpath;
        var urlfull = basePath + "/" + fileName;

        urlcache = Location.__urlCache__[urlfull];

        if (urlcache != null) return urlcache;

        if (fileName.indexOf("://") < 0)
            fileName = basePath + "/" + fileName;

        var urls = fileName.split("/");
        urls[1] = "";
        var str,i = 2,size = urls.length;
        while (i < size) {
            str = urls[i];
            if (str == null) break;
            if (str == '' || str == '.') {
                urls.splice(i, 1);
                continue;
            }
            if (str == "..") {
                if(i<=3&&this._protocol!="file:")
                {
                    urls.splice(i,1);
                }
                else{
                    urls.splice(i - 1, 2);
                    i -= 1;
                }
                continue;
            }
            i += 1;
        }
        fileName = urls.join("/");
        Location.__urlCache__[fileName] = fileName;
        Location.__urlCache__[urlfull] = fileName;
        return fileName;
    }
}