if( !window.runtime )
{
    window.runtime=
    {
        ScriptFrameParent:null,
        callObjectC:function(js)
        {
            if(!this.ScriptFrameParent)
            {
                this.ScriptFrameParent= document.createElement("div");
                this.ScriptFrameParent.cssText="position:absolute;left:0px;top:0;width:1px;height:1px;display:none";
                document.documentElement.appendChild(this.ScriptFrameParent);
            }
            var iframe = document.createElement("IFRAME");
            iframe.setAttribute("src", "runtime:"+js );
            this.ScriptFrameParent.appendChild(iframe);
            this.ScriptFrameParent.removeChild(iframe);
            iframe = null;
        },
        callConchJSFunction:function(name,json,callback)
        {
            this.callObjectC( "callConchJSFunction"+"\5" + name+"\5"+json+"\5"+callback );
        },
        JSLog:function( message )
        {
            this.callObjectC( "JSLog" + "\5" + message );
        }
    }
}
window.postRuntimeMessage=function(d,s){
    window.runtime.callConchJSFunction('window.postRuntimeMessage',encodeURIComponent( JSON.stringify(d)),'');
};
window.__getMessemage=function(d,s){
    d=decodeURIComponent(d);
    var e=document.createEvent("MessageEvent");
    e.initMessageEvent("message",false,false,JSON.parse(d),"","",window);
    e.target=window;
    window.dispatchEvent(e);
};