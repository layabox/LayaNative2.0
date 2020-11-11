
class DOMParser
{
    private _parser:_DOMParser;
    private _src:string;
    private _onload:()=>void;
    private _onerror:()=>void;
    private _result:_jsXmlDocument;
    constructor() {
        this._parser=new _DOMParser();
    }
    set src(s)
    {
        this._src=location.resolve(s);
        //this._src = encodeURI(this._src);
        this._parser.src=this._src;
    }
    get src()
    {
        return this._src;
    }
    set onload(callback)
    {
        this._parser._onload=callback;
        this._parser.onload=this.nativeObjOnload;
    }
    private nativeObjOnload()
    {
        this._onload();
    }
    get onload()
    {
        return this._parser._onload;
    }
    set	onerror(callback)
    {
        this._parser._onerror=callback;
        this._parser.onerror=this.nativeObjOnerror;
    }
    private nativeObjOnerror()
    {
        this._onerror();
    }
    get	onerror()
    {
        return this._parser._onerror;
    }
    
    private static initXMl(xml:_XmlNode):_jsXmlNode
	{
		var result:_jsXmlNode;
	    if(!xml)
		{
		   var temp=new _jsXmlNode();
           temp.nodeName="parsererror";
		   temp.textContent="parsererror error";
           result=new _jsXmlNode();
           result.childNodes[0]=temp;
		   return result;
		}
		result=new _jsXmlNode();
		result.nodeName=xml.nodeName;
		result.nodeValue=xml.nodeValue;
        result.nodeType=1;
        if(result.nodeName=="#cdata-section")
        {
            result.nodeType=3;
            result.nodeName="#text";
        }
		result.textContent=xml.textContent;
		var attrs=xml.attributes;
		for(var i=0,sz1=attrs.length;i<sz1;i++)
		{
			var attr=attrs[i];
			var key=attr.nodeName;
			var tempAttr=new _jsXmlAttr(key,attr.nodeValue);
			result.attributes[i]=tempAttr;
			result.attributes[key]=tempAttr;
		}
		var childs=xml.childNodes;
		for(var i=0,sz1=childs.length;i<sz1;i++)
		{
			var chd=childs[i];
			var nodeName=chd.nodeName;
			result.childNodes[i]=DOMParser.initXMl(chd);
		} 
		return result;
	}
    
    parseFromString(s:string,t:string):_jsXmlDocument
	{
		var xml= this._parser.parseFromString(s,t);
		var root=new _jsXmlDocument();
		root.childNodes[0]=DOMParser.initXMl(xml.childNodes[0]);
		return root;
	}
    
	getResult():_jsXmlDocument
	{
		if(!this._result)
		{
		    this._result=new _jsXmlDocument();
		    this._result.childNodes[0]=DOMParser.initXMl(this._parser.getResult().childNodes[0]);
		}
		return this._result;
	}
}
window["DOMParser"] =DOMParser;
class _jsXmlAttr
{
    nodeName:string;
    nodeValue:string;
    textContent:string;
    constructor(key:string,value:string)
    {
        this.nodeName=key;
        this.nodeValue=this.textContent=value;
    }
    get value()
    {
        return this.nodeValue;
    }   
}

class _jsXmlNode extends _jsXmlAttr
{
    childNodes:Array<_jsXmlNode>;
    attributes:Array<_jsXmlAttr>;
    nodeType:number;
    get firstChild()
    {
        return this.childNodes?this.childNodes[0]:null;
    }
    constructor()
    {
        super("","");
        this.childNodes=[];
        this.childNodes["item"]=function(i:any)
        {
            return this[i];
        }
        this.attributes=[];
    }
    
    getElementsByTagName(name:string):Array<_jsXmlNode>
    {
        var result:Array<_jsXmlNode>=[];
        if(this.nodeName==name)
            result.push(this);
        else
        {
            for(var i=0,n=this.childNodes.length;i<n;i++)
            {
                var son=this.childNodes[i];
                result=result.concat(son.getElementsByTagName(name));
            }
        }
        return result;
    }
    
    getAttribute(name:string):string
    {
        var attr=this.attributes[name];
        return attr?attr["nodeValue"]:"";
    }  
}

class _jsXmlDocument extends _jsXmlNode
{
    
}