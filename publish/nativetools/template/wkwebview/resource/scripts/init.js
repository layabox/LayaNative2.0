if (!window.wkbridge)
{
    window.onLayaWKWebview = true;
    var console = window.console;
    console.error = console.warn = console.log = function(message)
    {
        window.webkit.messageHandlers['log'].postMessage(message);
    }
    
    window.wkbridge =
    {
    responseCallbacks:{},
    callNative:function(name, sData, callback)
        {
            if (callback)
            {
                this.responseCallbacks[name] = callback;
            }
            window.webkit.messageHandlers[name].postMessage(sData);
        }
    }
    window.wkbridge.callback = function(name, sData)
    {
        var cb = this.responseCallbacks[name];
        if (cb)
        {
            cb(sData);
        }
    }
    window["conchPlatCallBack"] = function (ret) {
        var objid, m, rs, c, rJSON;
        if (ret == null)
            return;
        
        objid = ret.objId;
        m = ret.mName;
        c = ret.cName;
        rs = ret.v;
        var platC;
        if (objid != -1) {
            platC = PlatformObj.objMap[objid];
        }
        else {
            platC = PlatformClass.clsMap[c];
        }
        if (platC) {
            var callback = platC.callBackList[m];
            callback && callback(rs);
        }
    };
    class PlatformBase {
        call(methodName, p1, p2, p3, p4, p5, p6, p7, p8, p9) {
            var a = Array.prototype.slice.call(arguments);
            a.shift();
            try {
                var param =
                {
                    id:this.objid,
                    isStatic:true,
                    className:this.className,
                    methodName:methodName,
                    param:JSON.stringify(a) || "{}"
                };
                window.wkbridge.callNative('reflection', JSON.stringify(param), null);
            }
            catch (e) {
            }
        }
        callWithBack(callback, methodName, p1, p2, p3, p4, p5, p6, p7, p8, p9) {
            this.callBackList[methodName] = callback;
            var a = Array.prototype.slice.call(arguments);
            a.splice(0, 2);
            try {
                var param =
                {
                    id:this.objid,
                    isStatic:false,
                    className:this.className,
                    methodName:methodName,
                    param:JSON.stringify(a) || "{}"
                };
                window.wkbridge.callNative('reflection', JSON.stringify(param), null);
            }
            catch (e) {
            }
        }
    }
    class PlatformClass extends PlatformBase {
        constructor(clsName) {
            super();
            this.objid = -1;
            this.className = clsName;
            this.callBackList = {};
            PlatformClass.clsMap[clsName] = this;
        }
        static createClass(clsName) {
            return PlatformClass.clsMap[clsName] || new PlatformClass(clsName);
        }
        newObject(p1, p2, p3, p4, p5, p6, p7, p8, p9) {
            var a = Array.prototype.slice.call(arguments);
            a.splice(0, 0, "<init>");
            var rs = new PlatformObj(this);
            rs.init.apply(rs, a);
            return rs;
        }
    }
    PlatformClass.clsMap = {};
    class PlatformObj extends PlatformBase {
        constructor(cls) {
            super();
            this.cls = cls;
            this.className = cls.className;
            this.callBackList = {};
        }
        init() {
            this.objid = PlatformObj.objNum++;
            this.call.apply(this, arguments) || -1;
            PlatformObj.objMap[this.objid] = this;
        }
    }
    PlatformObj.objMap = {};
    PlatformObj.objNum = 0;
    window["PlatformClass"] = PlatformClass;
    window["PlatformObj"] = PlatformObj;
}

