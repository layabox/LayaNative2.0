///<reference path="./LayaConchRuntime.d.ts" />
class ColorUtil{
    static _COLOR_MAP:Object =  {"white": '#FFFFFF', "red": '#FF0000', "green": '#00FF00', "blue": '#0000FF', "black": '#000000', "yellow": '#FFFF00', 'gray': '#AAAAAA'};
    static getColorNum(str):number
    {
        if(!str)return 0;
        if(typeof(str)=="number")
        {
            if(str >= 0xFFFFFF ) return str;
            return str|0xFF000000;
        }
        var len:number;
        if(str[0]=="#")
        {
            var r:string=ColorUtil.getRGBByRGBStr(str);
            if(r){
                return parseInt(r,16);
            }
            else
                return NaN;
        }
        else if(str.indexOf("rgb")!=-1){
            return ColorUtil.getColNum(str,true);
        }
        else if(str.indexOf("hsl")!=-1){
            return ColorUtil.getColNum(str,false);
        }
        else
        {
            var c:string=ColorUtil._COLOR_MAP[str];
            return  c?ColorUtil.getColorNum(c):NaN;
        }
    }
    static RGBReg:RegExp=new RegExp("rgb([a]{0,1})\\s*[(]([\\d,.\\s-]*)?[)]");
    static HSLReg:RegExp=new RegExp("hsl([a]{0,1})\\s*[(]([\\d,.\\s-%]*)?[)]");
    static getColNum(c:string,isRgb:boolean):number
    {
        var  temp:string,i:number,s:number,result:Array<number>=[],st:number;
        var t:Array<string>=isRgb?ColorUtil.RGBReg.exec(c):ColorUtil.HSLReg.exec(c);
        if(t)
        {
            var r = t[2].split(",");
            i = 0; s = r.length;
            while (i < s) { result[i] =parseFloat(r[i]); i++;}
            var ta:Array<number>=result;
            if(!isRgb){
                ta=ColorUtil.hsl2rgb(result[0]/360,result[1]/100,result[2]/100);
            }
            if(i==3&&!t[1]){
                return 0xff000000+(ta[0]<<16)+(ta[1]<<8)+ta[2];
            }
            else if(i==4&&t[1]){
                return parseInt(Math.floor(0xff*result[3]).toString(16)+"000000",16)+(ta[0]<<16)+(ta[1]<<8)+ta[2];
            }
            return NaN;
        }
        else
        return NaN;
    }
    static  getRGBByRGBStr(str:string):string
	{
        str = str.substr(1);
        var i:number,len:number=str.length,result:string;
        switch (len) {
            case 3:
                result="ff";
                for(i=0;i<len;i++){
                    result+=str[i]+str[i];
                }
                break;
            case 6:
                result="ff"+str;
            default:
                break;
        }
        return result;
    }
    static toHexColor(color:number):string
    {
        if (color < 0 || isNaN(color))
            return null;
        if(color>0xff000000){
            color-=0xff000000;
        }
        else{
            return "rgba("+(color>>16&0xff)+","+(color>>8&0xff)+","+(color&0xff)+","+((color>>24&0xff)/0xff)+")";
        }
        var str:string = color.toString(16);
        while (str.length < 6)
            str = "0" + str;
        return "#" + str;
    }
    static  hsl2rgb(h:number, s:number, l:number):Array<number>
    {
        var r:number, g:number, b:number;
        if(s == 0){
            r = g = b = l; // achromatic
        }else{
            function hue2rgb(p, q, t){
                if(t < 0) t += 1;
                if(t > 1) t -= 1;
                if(t < 1/6) return p + (q - p) * 6 * t;
                if(t < 1/2) return q;
                if(t < 2/3) return p + (q - p) * (2/3 - t) * 6;
                return p;
            }
            var q = l < 0.5 ? l * (1 + s) : l + s - l * s;
            var p = 2 * l - q;
            r = hue2rgb(p, q, h + 1/3);
            g = hue2rgb(p, q, h);
            b = hue2rgb(p, q, h - 1/3);
        }
        return [Math.round(r * 255), Math.round(g * 255), Math.round(b * 255)];
    }
}

class ProgramLocationTable
{
    public _fakeLocationNum:number = 0;
    public _map:Object = {};
    
    getFakeLocation(fakeProgramID:number, name:string):number
    {
        var key:string = fakeProgramID + "-" + name;
        var fakeID:number = this._map[key];
        if (!fakeID)
        {
            fakeID = this._fakeLocationNum++;
            this._map[key] = fakeID;
        }
        return fakeID;
    }
}

enum FUNCTION_ID
{
    UNIFORMMATRIX2FVEX=0,
    UNIFORMMATRIX3FVEX,
    UNIFORMMATRIX4FVEX,
    ADDSHADERUNIFORM,
    UPLOADSHADERUNIFORMS,
    UPLOADSHADERUNIFORMS_BUFFER,
    USECOMMANDENCODER,
    LOADDATATOREG,
    LOADDATATOREGEX,
    IFLESS0,
    IFEQUAL0,
    IFGREATER0,
    IFLEQUAL0,
    IFGEQUAL0,
    IFGNOTEQUAL0,
    OPERATEREG,
    STORE,
    CREATEIMAGEONRENDERTHREAD=64,
    DELETE_IMAGE_ON_RENDER_THREAD,
    RELEASE_IMAGE_ON_RENDER_THREAD,
    SET_IMAGE_RELEASE_SPACE_TIME,
    SET_PREMULTIPLY_ALPHA,
    PERFADDDATA,
    PERFUPDATEDT,
    SET_MAIN_CONTEXT_SIZE,
    //-------------------------------
    GETCONTEXTATTRIBUTES=128,
    ISCONTEXTLOST,
    GETSUPPORTEDEXTENSIONS,
    GETEXTENSION,
    ACTIVETEXTURE,
    ATTACHSHADER,
    BINDATTRIBLOCATION,
    BINDBUFFER,
    BINDFRAMEBUFFER,
    BINDRENDERBUFFER,//第10个
    BINDTEXTURE,
    USETEXTURE,
    BLENDCOLOR,
    BLENDEQUATION,
    BLENDEQUATIONSEPARATE,
    BLENDFUNC,
    BLENDFUNCSEPARATE,
    BUFFERDATA_SIZE,
    BUFFERDATA_ARRAYBUFFER,
    BUFFERSUBDATA,//第20个
    CHECKFRAMEBUFFERSTATUS,
    CLEAR,
    CLEARCOLOR,
    CLEARDEPTH,
    CLEARSTENCIL,
    COLORMASK,
    COMPILESHADER,
    COPYTEXIMAGE2D,
    COPYTEXSUBIMAGE2D,
    CREATEBUFFER,//第30个
    CREATEFRAMEBUFFER,
    CREATEPROGRAM,
    CREATERENDERBUFFER,
    CREATESHADER,
    CREATETEXTURE,
    CULLFACE,
    DELETEBUFFER,
    DELETEFRAMEBUFFER,
    DELETEPROGRAM,
    DELETERENDERBUFFER,//第40个
    DELETESHADER,
    DELETETEXTURE,
    DEPTHFUNC,
    DEPTHMASK,
    DEPTHRANGE,
    DETACHSHADER,
    DISABLE,
    DISABLEVERTEXATTRIBARRAY,
    DRAWARRAYS,
    DRAWELEMENTS,//第50个
    ENABLE,
    ENABLEVERTEXATTRIBARRAY,
    FINISH,
    FLUSH,
    FRAMEBUFFERRENDERBUFFER,
    FRAMEBUFFERTEXTURE2D,
    FRONTFACE,
    GENERATEMIPMAP,
    GETACTIVEATTRIB,
    GETACTIVEUNIFORM,//第60个
    GETATTRIBLOCATION,
    GETPARAMETER,
    GETBUFFERPARAMETER,
    GETERROR,
    GETFRAMEBUFFERATTACHMENTPARAMETER,
    GETPROGRAMPARAMETER,
    GETPROGRAMINFOLOG,
    GETRENDERBUFFERPARAMETER,
    GETSHADERPRECISIONFORMAT,
    GETSHADERPARAMETER,//第70个
    GETSHADERINFOLOG,
    GETSHADERSOURCE,
    GETTEXPARAMETER,
    GETUNIFORM,
    GETUNIFORMLOCATION,
    GETVERTEXATTRIB,
    GETVERTEXATTRIBOFFSET,
    HINT,
    ISBUFFER,
    ISENABLED,//第80个
    ISFRAMEBUFFER,
    ISPROGRAM,
    ISRENDERBUFFER,
    ISSHADER,
    ISTEXTURE,
    LINEWIDTH,
    LINKPROGRAM,
    PIXELSTOREI,
    POLYGONOFFSET,
    READPIXELS,//第90个
    RENDERBUFFERSTORAGE,
    SAMPLECOVERAGE,
    SCISSOR,
    SHADERSOURCE,
    STENCILFUNC,
    STENCILFUNCSEPARATE,
    STENCILMASK,
    STENCILMASKSEPARATE,
    STENCILOP,
    STENCILOPSEPARATE,//第100个
    TEXIMAGE2D,
    TEXPARAMETERF,
    TEXPARAMETERI,
    TEXSUBIMAGE2D,
    UNIFORM1F,
    UNIFORM1FV,
    UNIFORM1I,
    UNIFORM1IV,
    UNIFORM2F,
    UNIFORM2FV,//第110个
    UNIFORM2I,
    UNIFORM2IV,
    UNIFORM3F,
    UNIFORM3FV,
    UNIFORM3I,
    UNIFORM3IV,
    UNIFORM4F,
    UNIFORM4FV,
    UNIFORM4I,
    UNIFORM4IV,//第120个
    UNIFORMMATRIX2FV,
    UNIFORMMATRIX3FV,
    UNIFORMMATRIX4FV,
    USEPROGRAM,
    VALIDATEPROGRAM,
    VERTEXATTRIB1F,
    VERTEXATTRIB1FV,
    VERTEXATTRIB2F,
    VERTEXATTRIB2FV,
    VERTEXATTRIB3F,//第130个
    VERTEXATTRIB3FV,
    VERTEXATTRIB4F,
    VERTEXATTRIB4FV,
    VERTEXATTRIBPOINTER,
    VIEWPORT,
    CONFIGUREBACKBUFFER,
    COMPRESSEDTEXIMAGE2D,
    TEXIMAGE2D_PIXEL,
    TEXSUBIMAGE2D_PIXEL,
    CREATEVERTEXARRAY,//第140个
    BINDVERTEXARRAY,
    DELETEVERTEXARRAYS,
    READPIXELS_ASYNC,
    COMPRESSEDTEXSUBIMAGE2D,
    VERTEXATTRIBDIVISOR,
    DRAWARRAYSINSTANCED,
    DRAWELEMENTSINSTANCED,
    TEXIMAGE2DCANVAS,
    TEXSTORAGE2D,
    RENDERBUFFERSTORAGEMUILTISAMPLE,
    CLEARBUFFERFV,
    BLITFRAMEBUFFER,
}
enum UNIFORM_TYPE
{
    //native用的shaderUniform的函数ID
    INTERIOR_UNIFORM1F = 0,
    INTERIOR_UNIFORM1FV,
    INTERIOR_UNIFORM1I,
    INTERIOR_UNIFORM1IV,
    INTERIOR_UNIFORM2F,
    INTERIOR_UNIFORM2FV,
    INTERIOR_UNIFORM2I,
    INTERIOR_UNIFORM2IV,
    INTERIOR_UNIFORM3F,
    INTERIOR_UNIFORM3FV,
    INTERIOR_UNIFORM3I,
    INTERIOR_UNIFORM3IV,
    INTERIOR_UNIFORM4F,
    INTERIOR_UNIFORM4FV,
    INTERIOR_UNIFORM4I,
    INTERIOR_UNIFORM4IV,
    INTERIOR_UNIFORMMATRIX2FV,
    INTERIOR_UNIFORMMATRIX3FV,
    INTERIOR_UNIFORMMATRIX4FV,
    INTERIOR_UNIFORMSAMPLER_2D,
    INTERIOR_UNIFORMSAMPLER_CUBE,
}

class BindInfo
{
    constructor()
    {
        this.m_bindMap = {}
    }

   private m_bindMap:Object;

   public setBind(pname:any, buffer:any):void
   {
       this.m_bindMap[pname] = buffer;
   }

   public getBind(pname:any):any
   {
        var buffer = this.m_bindMap[pname];
        if(!buffer)
            return null;

        if(buffer.id == -1) // delete
        {
            this.m_bindMap[pname] = null;
            return null;
        }


       return buffer;
   }
}
class ANGLEInstancedArrays
{
    constructor(encoder)
    {
        this.encoder = encoder;
    }
    vertexAttribDivisorANGLE(index:number, divisor:number):void
    {
        this.encoder.vertexAttribDivisor(index, divisor);
    }
    drawArraysInstancedANGLE(mode:number, first:number, count:number, instanceCount:number):void
    {
        this.encoder.drawArraysInstanced(mode, first, count, instanceCount);
    }
    drawElementsInstancedANGLE(mode:number, count:number, type:number, offset:number, instanceCount:number):void
    {
        this.encoder.drawElementsInstanced(mode, count, type, offset, instanceCount);
    }
    public encoder:GLCommandEncoder;
}
class OESVertexArrayObject
{
    constructor(encoder)
    {
        this.encoder = encoder;
    }
    createVertexArrayOES():any
    {
        return this.encoder.createVertexArray();
    }
    deleteVertexArrayOES(arrayObject:any):void
    {
        this.encoder.deleteVertexArray(arrayObject);
    }
    isVertexArrayOES(arrayObject:any):boolean
    {
        return this.encoder.isVertexArray(arrayObject);
    }
    bindVertexArrayOES(arrayObject:any):void
    {
        this.encoder.bindVertexArray(arrayObject);
    }
    public encoder:GLCommandEncoder;
}
class OESTextureHalfFloatLinear
{}
class EXTColorBufferFloat
{}
class WebGLBuffer
{
    constructor(id)
    {
        this.id = id;
    }
    public id:number;
}
class WebGLFrameBuffer
{
    constructor(id)
    {
        this.id = id;
        this.m_bindInfo = new BindInfo();
    }
    public id:number;
    private m_bindInfo:BindInfo;

    public setBind(pname:any, buffer:any):void
    {
        this.m_bindInfo.setBind(pname, buffer);
    }

    public getBind(pname:any):any
    {
        return this.m_bindInfo.getBind(pname);
    }

}
class WebGLRenderBuffer
{
    constructor(id)
    {
        this.id = id;
    }
    public id:number;
}
class WebGLShader
{
    constructor(id,type)
    {
        this.id = id;
        this.type = type;
        this.compileStatus = 0;
        this.src = "";
    }
    public id:number;
    public src:string;
    public type:number;
    public compileStatus:number;
}
class WebGLProgram
{
    constructor(id)
    {
        this.id = id;
        this.linkStatus = 0;
        this.vsShader = null;
        this.psShader = null;
        this.attachedShaderCount = 0;

        this.m_localMap = {};

    }
    public id:number;
    public vsShader:WebGLShader;
    public psShader:WebGLShader;
    public linkedVSSource:string;
    public linkedPSSource:string;
    public linkStatus:number;
    public attachedShaderCount;

    private m_localMap:Object;

    public addLocationPair(fakeLoc:number, name:string) {
        this.m_localMap[fakeLoc] = name;
    }

    public getLocationName(fakeLoc):string {
        return this.m_localMap[fakeLoc];
    }
}
class WebGLTexture
{
    constructor(id)
    {
        this.id = id;
    }
    public id:number;

}
class WebGLVertextArray
{
    constructor(id)
    {
        this.id = id;
    }
    public id:number;
}
class CallbackFuncObj
{
    public static s_nID:number=0;

    constructor()
    {
        this.id = CallbackFuncObj.s_nID++;
        this._nativeObj = new _callbackFuncObj(this.id);
    }
    addCallbackFunc(index:number,func:any)
    {
        this._vFunc[index] =func;
        this._nativeObj.addCallbackFunc(index,func);
    }
    public id:number;
    public _vFunc = new Map<number, any>();
    public _nativeObj:_callbackFuncObj;
}
class fakeIDObj
{
    constructor(id:number=0,frame:number=0)
    {
        this.id = id;
        this.frameCount = frame;
    }
    id:number;
    frameCount:number;
}
class GLCommandEncoder
{
    private static readonly INT = 0x1404;
    private static readonly FLOAT = 0x1406;
    /* Uniform Types */
    private static readonly FLOAT_VEC2 = 0x8B50;
    private static readonly FLOAT_VEC3 = 0x8B51;
    private static readonly FLOAT_VEC4 = 0x8B52;
    private static readonly INT_VEC2 = 0x8B53;
    private static readonly INT_VEC3 = 0x8B54;
    private static readonly INT_VEC4 = 0x8B55;
    private static readonly BOOL = 0x8B56;
    private static readonly BOOL_VEC2 = 0x8B57;
    private static readonly BOOL_VEC3 = 0x8B58;
    private static readonly BOOL_VEC4 = 0x8B59;
    private static readonly FLOAT_MAT2 = 0x8B5A;
    private static readonly FLOAT_MAT3 = 0x8B5B;
    private static readonly FLOAT_MAT4 = 0x8B5C;
    private static readonly SAMPLER_2D = 0x8B5E;
    private static readonly SAMPLER_CUBE = 0x8B60;
    private static _fakeIDCount:number = 0;
    private static _fakeArray=[new fakeIDObj(0,0)];
    private static _locTable:ProgramLocationTable = new ProgramLocationTable();
    public _idata:Int32Array;
    public _fdata:Float32Array;
    public _byteArray:Uint8Array;
    public _buffer:ArrayBuffer;
    private _adjustSize:number = 0;
    private _byteLen:number = 0;
    private _layagl:any;
    public _isSyncToRenderThread:Boolean = false;
    private _readPixelsAsyncCallbackFuncObj = new CallbackFuncObj();
    constructor(layagl:any, reserveSize:number, adjustSize:number, isSyncToRenderThread:boolean)
    {
        this._isSyncToRenderThread = isSyncToRenderThread;
        this._layagl = layagl;
        this._byteLen = reserveSize;
        this._adjustSize = adjustSize;
        this._init(isSyncToRenderThread);
    }
    
    _init(isSyncToRenderThread:boolean):void
    {
        this._buffer = new ArrayBuffer(this._byteLen);
        this._idata = new Int32Array(this._buffer);
        this._fdata = new Float32Array(this._buffer);
        this._byteArray = new Uint8Array(this._buffer);
        this._layagl.createArrayBufferRef(this._buffer, LayaGLContext.ARRAY_BUFFER_TYPE_CMD, isSyncToRenderThread);
        this._idata[0] = 1;
    }
    
    getPtrID():number
    {
        return this._buffer["_ptrID"];
    }
    
    beginEncoding():void
    {
        this._layagl.beginCommandEncoding(this);
    }
    
    endEncoding():void
    {
        this._layagl.endCommandEncoding();
    }
    
    clearEncoding():void
    {
        this._idata[0] = 1;
    }
    
    getCount():number
    {
        return this._idata[0];
    }

    _need(sz:number):void
    {
        if ((this._byteLen - (this._idata[0] << 2)) >= sz) return;
        this._byteLen += (sz > this._adjustSize) ? sz : this._adjustSize;
        var pre:Int32Array = this._idata;
        var preConchRef:any = this._buffer["conchRef"];
        var prePtrID:number = this._buffer["_ptrID"];
        this._buffer = new ArrayBuffer(this._byteLen);
        this._idata = new Int32Array(this._buffer);
        this._fdata = new Float32Array(this._buffer);
        this._byteArray = new Uint8Array(this._buffer);
        this._buffer["conchRef"] = preConchRef;
        this._buffer["_ptrID"] = prePtrID;
        pre && this._idata.set(pre, 0);
        webglPlus.updateArrayBufferRef(this._buffer["_ptrID"], preConchRef.isSyncToRender(), this._buffer);
    }
    
    add_i(i:number):void
    {
        this._need(4);
        this._idata[this._idata[0]++] = i;
    }
    
    add_f(a:number):void
    {
        this._need(4);
        this._fdata[this._idata[0]++] = a;
    }
    
    add_ff(a:number, b:number):void
    {
        this._need(8);
        var i:number = this._idata[0];
        this._fdata[i++] = a;
        this._fdata[i++] = b;
        this._idata[0] = i;
    }

    add_fff(a:number, b:number, c:number) {
        this._need(12);
        var i = this._idata[0];
        this._fdata[i++] = a;
        this._fdata[i++] = b;
        this._fdata[i++] = c;
        this._idata[0] = i;
    }        
    
    add_ii(a:number, b:number):void
    {
        this._need(8);
        var i:number = this._idata[0];
        this._idata[i++] = a;
        this._idata[i++] = b;
        this._idata[0] = i;
    }
    
    add_if(a:number, b:number):void
    {
        this._need(8);
        var i:number = this._idata[0];
        this._idata[i++] = a;
        this._fdata[i++] = b;
        this._idata[0] = i;
    }
    
    add_iii(a:number, b:number, c:number):void
    {
        this._need(12);
        var idata:Int32Array = this._idata;
        var i:number = this._idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        this._idata[0] = i;
    }
    
    add_iif(a:number, b:number, c:number):void
    {
        this._need(12);
        var idata:Int32Array = this._idata;
        var i:number = this._idata[0];
        idata[i++] = a;
        idata[i++] = b;
        this._fdata[i++] = c;
        this._idata[0] = i;
    }
    
    add_ifi(a:number, b:number, c:number):void
    {
        this._need(12);
        var idata:Int32Array = this._idata;
        var i:number = this._idata[0];
        idata[i++] = a;
        this._fdata[i++] = b;
        idata[i++] = c;
        this._idata[0] = i;
    }
    add_iifi(a:number, b:number, c:number, d:number):void
    {
        this._need(16);
        var idata:Int32Array = this._idata;
        var i:number = this._idata[0];
        idata[i++] = a;
        idata[i++] = b;
        this._fdata[i++] = c;
        idata[i++] = d;
        this._idata[0] = i;
    }
    add_iiii(a:number, b:number, c:number, d:number):void
    {
        this._need(16);
        var idata:Int32Array = this._idata;
        var i:number = this._idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        idata[0] = i;
    }
    
    add_iiif(a:number, b:number, c:number, d:number):void
    {
        this._need(16);
        var idata:Int32Array = this._idata;
        var i:number = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        this._fdata[i++] = d;
        idata[0] = i;
    }
    add_iiiif(a:number, b:number, c:number, d:number, e:number):void
    {
        this._need(20);
        var idata:Int32Array = this._idata;
        var i:number = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        this._fdata[i++] = e;
        idata[0] = i;
    }
    add_iiiff(a:number, b:number, c:number, d:number,e:number):void
    {
        this._need(20);
        var idata:Int32Array = this._idata;
        var i:number = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        this._fdata[i++] = d;
        this._fdata[i++] = e;
        idata[0] = i;
    }
    add_iiiiff(a:number, b:number, c:number, d:number,e:number,f:number):void
    {
        this._need(24);
        var idata:Int32Array = this._idata;
        var i:number = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        this._fdata[i++] = e;
        this._fdata[i++] = f;
        idata[0] = i;
    }
    add_iiifff(a:number, b:number, c:number, d:number,e:number,f:number):void
    {
        this._need(24);
        var idata:Int32Array = this._idata;
        var i:number = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        this._fdata[i++] = d;
        this._fdata[i++] = e;
        this._fdata[i++] = f;
        idata[0] = i;
    }
    add_iiiifff(a:number, b:number, c:number, d:number,e:number,f:number,g:number):void
    {
        this._need(28);
        var idata:Int32Array = this._idata;
        var i:number = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        this._fdata[i++] = e;
        this._fdata[i++] = f;
        this._fdata[i++] = g;
        idata[0] = i;
    }
    add_iiiffff(a:number, b:number, c:number, d:number,e:number,f:number,g:number):void
    {
        this._need(28);
        var idata:Int32Array = this._idata;
        var i:number = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        this._fdata[i++] = d;
        this._fdata[i++] = e;
        this._fdata[i++] = f;
        this._fdata[i++] = g;
        idata[0] = i;
    }
    add_iiiiffff(a:number, b:number, c:number, d:number,e:number,f:number,g:number,h:number):void
    {
        this._need(32);
        var idata:Int32Array = this._idata;
        var i:number = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        this._fdata[i++] = e;
        this._fdata[i++] = f;
        this._fdata[i++] = g;
        this._fdata[i++] = h;
        idata[0] = i;
    }
    add_iiff(a:number, b:number, c:number, d:number):void
    {
        this._need(16);
        var idata:Int32Array = this._idata;
        var fdata:Float32Array = this._fdata;
        var i:number = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        fdata[i++] = c;
        fdata[i++] = d;
        idata[0] = i;
    }
    
    add_iifff(a:number, b:number, c:number, d:number, e:number):void
    {
        this._need(20);
        var idata:Int32Array = this._idata;
        var fdata:Float32Array = this._fdata;
        var i:number = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        fdata[i++] = c;
        fdata[i++] = d;
        fdata[i++] = e;
        idata[0] = i;
    }
    
    add_iiffff(a:number, b:number, c:number, d:number, e:number, f:number):void
    {
        this._need(24);
        var idata:Int32Array = this._idata;
        var fdata:Float32Array = this._fdata;
        var i:number = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        fdata[i++] = c;
        fdata[i++] = d;
        fdata[i++] = e;
        fdata[i++] = f;
        idata[0] = i;
    }

    add_iiiii(a:number, b:number, c:number, d:number, e:number):void
    {
        this._need(20);
        var idata:Int32Array = this._idata;
        var i:number = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        idata[i++] = e;
        idata[0] = i;
    }
    
    add_iiiiii(a:number, b:number, c:number, d:number, e:number, f:number):void
    {
        this._need(24);
        var idata:Int32Array = this._idata;
        var i:number = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        idata[i++] = e;
        idata[i++] = f;
        idata[0] = i;
    }
    
    add_iiiiiii(a:number, b:number, c:number, d:number, e:number, f:number, g:number):void
    {
        this._need(28);
        var idata:Int32Array = this._idata;
        var i:number = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        idata[i++] = e;
        idata[i++] = f;
        idata[i++] = g;
        idata[0] = i;
    }
    add_iiiiiiiiiiii(a:number, b:number, c:number, d:number, e:number, f:number, g:number,h:number,j:number,k:number,l:number,m:number):void
    {
        this._need(48);
        var idata:Int32Array = this._idata;
        var i:number = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        idata[i++] = e;
        idata[i++] = f;
        idata[i++] = g;
        idata[i++] = h;
        idata[i++] = j;
        idata[i++] = k;
        idata[i++] = l;
        idata[i++] = m;
        idata[0] = i;
    }
    add_iiiiiiiiiiiii(a:number, b:number, c:number, d:number, e:number, f:number, g:number,h:number,j:number,k:number,l:number,m:number,n:number):void
    {
        this._need(52);
        var idata:Int32Array = this._idata;
        var i:number = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        idata[i++] = e;
        idata[i++] = f;
        idata[i++] = g;
        idata[i++] = h;
        idata[i++] = j;
        idata[i++] = k;
        idata[i++] = l;
        idata[i++] = m;
        idata[i++] = n;
        idata[0] = i;
    }
    
    add_iff(a:number, b:number, c:number):void
    {
        this._need(12);
        var i:number = this._idata[0];
        this._idata[i++] = a;
        this._fdata[i++] = b;
        this._fdata[i++] = c;
        this._idata[0] = i;
    }
    
    add_iffi(a:number, b:number, c:number, d:number):void
    {
        this._need(16);
        var i:number = this._idata[0];
        this._idata[i++] = a;
        this._fdata[i++] = b;
        this._fdata[i++] = c;
        this._idata[i++] = d;
        this._idata[0] = i;
    }
    
    add_ifffi(a:number, b:number, c:number, d:number, e:number):void
    {
        this._need(20);
        var i:number = this._idata[0];
        this._idata[i++] = a;
        this._fdata[i++] = b;
        this._fdata[i++] = c;
        this._fdata[i++] = d;
        this._idata[i++] = e;
        this._idata[0] = i;
    }
    
    add_iffff(a:number, b:number, c:number, d:number, e:number):void
    {
        this._need(20);
        var i:number = this._idata[0];
        var fdata:Float32Array = this._fdata;
        this._idata[i++] = a;
        fdata[i++] = b;
        fdata[i++] = c;
        fdata[i++] = d;
        fdata[i++] = e;
        this._idata[0] = i;
    }
    
    add_iffffi(a:number, b:number, c:number, d:number, e:number, f:number):void
    {
        this._need(24);
        var i:number = this._idata[0];
        var fdata:Float32Array = this._fdata;
        this._idata[i++] = a;
        fdata[i++] = b;
        fdata[i++] = c;
        fdata[i++] = d;
        fdata[i++] = e;
        this._idata[i++] = f;
        this._idata[0] = i;
    }
    
    add_ifffffi(a:number, b:number, c:number, d:number, e:number, f:number, g:number):void
    {
        this._need(28);
        var i:number = this._idata[0];
        var fdata:Float32Array = this._fdata;
        this._idata[i++] = a;
        fdata[i++] = b;
        fdata[i++] = c;
        fdata[i++] = d;
        fdata[i++] = e;
        fdata[i++] = f;
        this._idata[i++] = g;
        this._idata[0] = i;
    }
    
    add_iffffiif(a:number, b:number, c:number, d:number, e:number, f:number, g:number, h:number):void
    {
        this._need(28);
        var i:number = this._idata[0];
        var fdata:Float32Array = this._fdata;
        this._idata[i++] = a;
        fdata[i++] = b;
        fdata[i++] = c;
        fdata[i++] = d;
        fdata[i++] = e;
        this._idata[i++] = f;
        this._idata[i++] = g;
        fdata[i++] = h;
        this._idata[0] = i;
    }
    
    add_iffffiii(a:number, b:number, c:number, d:number, e:number, f:number, g:number, h:number):void
    {
        this._need(32);
        var i:number = this._idata[0];
        var fdata:Float32Array = this._fdata;
        this._idata[i++] = a;
        fdata[i++] = b;
        fdata[i++] = c;
        fdata[i++] = d;
        fdata[i++] = e;
        this._idata[i++] = f;
        this._idata[i++] = g;
        this._idata[i++] = h;
        this._idata[0] = i;
    }
    
    add_iiiiiiii(a:number, b:number, c:number, d:number, e:number, f:number, g:number, h:number):void
    {
        this._need(32);
        var i:number = this._idata[0];
        var idata:Int32Array = this._idata;
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        idata[i++] = e;
        idata[i++] = f;
        idata[i++] = g;
        idata[i++] = h;
        idata[0] = i;
    }
    
    add_iiiiiiiii(a:number, b:number, c:number, d:number, e:number, f:number, g:number, h:number, j:number):void
    {
        this._need(36);
        var idata:Int32Array = this._idata;
        var i:number = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        idata[i++] = e;
        idata[i++] = f;
        idata[i++] = g;
        idata[i++] = h;
        idata[i++] = j;
        idata[0] = i;
    }
    add_iiiiiiiiii(a:number, b:number, c:number, d:number, e:number, f:number, g:number, h:number, j:number,k:number):void
    {
        this._need(40);
        var idata:Int32Array = this._idata;
        var i:number = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        idata[i++] = e;
        idata[i++] = f;
        idata[i++] = g;
        idata[i++] = h;
        idata[i++] = j;
        idata[i++] = k;
        idata[0] = i;
    }
    add_iiiiiiiiiii(a:number, b:number, c:number, d:number, e:number, f:number, g:number, h:number, j:number,k:number,l:number):void
    {
        this._need(44);
        var idata:Int32Array = this._idata;
        var i:number = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        idata[i++] = e;
        idata[i++] = f;
        idata[i++] = g;
        idata[i++] = h;
        idata[i++] = j;
        idata[i++] = k;
        idata[i++] = l;
        idata[0] = i;
    }
    
    add_String(str:string, len:number):void
    {
        var ab:ArrayBuffer = conch.strTobufer(str);
        this._need(len + 4);
        this._idata[this._idata[0]++] = len;
        if (len == 0) return;
        var uint8array:Uint8Array = new Uint8Array(ab);
        this._byteArray.set(uint8array, this._idata[0] * 4);
        this._idata[0] += len / 4;
    }

    wab(arraybuffer:any, length:number, nAlignLength:number, offset?:number):void
    {
        offset = offset ? offset : 0;
        this._need(nAlignLength + 4);
        //写入长度
        this._idata[this._idata[0]++] = length;
        //写入数据
        var uint8array:Uint8Array = null;
        if (arraybuffer instanceof Float32Array && offset ==0 )
        {
            this._fdata.set(arraybuffer, this._idata[0]);
        }
        else
        {
            if (arraybuffer instanceof ArrayBuffer)
            {
                uint8array = new Uint8Array(arraybuffer, offset, length);
            }
            else if (arraybuffer.buffer)
            {
                uint8array = new Uint8Array(arraybuffer.buffer, offset+arraybuffer.byteOffset, length);
            }
            else
            {
                console.log("not arraybuffer/dataview");
                return;
            }
            this._byteArray.set(uint8array, this._idata[0] * 4);
        }
        this._idata[0] += nAlignLength / 4;
    }
    
    copyBuffer(arraybuffer:any, length:number, offset?:number):void
    {
        offset = offset ? offset : 0;
        this._need(length);
        //写入数据
        var ab:any = null;
        if (arraybuffer instanceof ArrayBuffer)
        {
            ab = arraybuffer;
        }
        else if (arraybuffer.buffer)
        {
            ab = arraybuffer.buffer;
        }
        else
        {
            console.log("not arraybuffer/dataview ");
            return;
        }
        var uint8array:Uint8Array = new Uint8Array(ab, offset, length);
        this._byteArray.set(uint8array, this._idata[0] * 4);
        this._idata[0] += length / 4;
    }
    
    //------------------------------------------------------------------------------
    //--------以下是native-webgl函数------------------------------------------------
    //------------------------------------------------------------------------------
    getContextAttributes():any
    {
        return {alpha:false,antialias:false,depth:true,failIfMajorPerformanceCaveat:false,premultipliedAlpha:true,preserveDrawingBuffer:false,stencil:false};
    }
    
    isContextLost():boolean
    {
        console.log("isContextLost can't support");
        return false;
    }

    getSupportedExtensions():any
    {
        let extentions:string[] = this._layagl._nativeObj.getStringEx(0x1F03).split(' ');
        let version:string = this._layagl._nativeObj.getStringEx(0x1F02);
        function supports(extention:string)
        {
            for (let ext of extentions)
            {
                if (ext === extention)
                {
                    return true;
                }
            }
            return false;
        }
        let result:string[] = [];
        
        if (supports("GL_EXT_blend_minmax"))
            result.push("EXT_blend_minmax");
        if (supports("GL_EXT_sRGB"))
            result.push("EXT_sRGB");
        if (supports("GL_EXT_frag_depth"))
            result.push("EXT_frag_depth");
        if (supports("GL_OES_texture_float"))
            result.push("OES_texture_float");
        if (supports("GL_OES_texture_float_linear"))
            result.push("OES_texture_float_linear");
        if (supports("GL_OES_texture_half_float"))
            result.push("OES_texture_half_float");
        if (supports("GL_OES_texture_half_float_linear"))
            result.push("OES_texture_half_float_linear");
        if (supports("GL_OES_standard_derivatives"))
            result.push("OES_standard_derivatives");
        if (supports("GL_EXT_shader_texture_lod") || supports("GL_ARB_shader_texture_lod"))
            result.push("EXT_shader_texture_lod");
        if (supports("GL_EXT_texture_filter_anisotropic"))
            result.push("EXT_texture_filter_anisotropic");
        if (supports("GL_OES_vertex_array_object"))
            result.push("OES_vertex_array_object");
        if (supports("GL_OES_element_index_uint"))
            result.push("OES_element_index_uint");
        if (supports("GL_ANGLE_translated_shader_source"))
            result.push("WEBGL_debug_shaders");

        if (supports("GL_AMD_compressed_ATC_texture"))
            result.push("WEBGL_compressed_texture_atc");
        if (supports("GL_IMG_texture_compression_pvrtc"))
            result.push("WEBGL_compressed_texture_pvrtc");
        if (supports("GL_EXT_texture_compression_s3tc") || supports("GL_EXT_texture_compression_dxt1"))
            result.push("WEBGL_compressed_texture_s3tc");
        if (supports("GL_OES_depth_texture") || supports("GL_ARB_depth_texture") || supports("GL_ANGLE_depth_texture"))
            result.push("WEBGL_depth_texture");
        if (version.indexOf("OpenGL ES 3.") != -1)
        {
            //result.push("WEBGL_draw_buffers");
            if (conchConfig.getOS() == "Conch-android")
            {
                result.push("WEBGL_compressed_texture_etc");
            }
        }
        if (supports("ANGLE_instanced_arrays"))
            result.push("ANGLE_instanced_arrays");
   
        if (supports("GL_EXT_color_buffer_float"))
            result.push("EXT_color_buffer_float");
        if (supports("GL_EXT_color_buffer_half_float"))
            result.push("EXT_color_buffer_half_float");
        if (supports("GL_EXT_disjoint_timer_query"))
            result.push("EXT_disjoint_timer_query");
        if (supports("GL_OES_compressed_ETC1_RGB8_texture"))
            result.push("WEBGL_compressed_texture_etc1");
        if (supports("GL_EXT_texture_compression_s3tc"))
            result.push("WEBGL_compressed_texture_s3tc");
        if (supports("GL_EXT_texture_compression_s3tc_srgb"))
            result.push("WEBGL_compressed_texture_s3tc_srgb");
        if (supports("GL_OES_texture_compression_astc"))
            result.push("WEBGL_compressed_texture_astc");
        /*if (supports("???"))
            result.push("WEBGL_color_buffer_float");
            */
        result.push("WEBGL_debug_renderer_info");
        result.push("WEBGL_lose_context");
        return result;
    }
    
    getExtension(name:string):any
    {
        var extention:string = this._layagl._nativeObj.getStringEx(0x1F03);/*GL_EXTENSIONS*/
        let version:string = this._layagl._nativeObj.getStringEx(0x1F02);
        let extentions:string[] = extention.split(' ');
        function supports(extention:string)
        {
            for (let ext of extentions)
            {
                if (ext === extention)
                {
                    return true;
                }
            }
            return false;
        }

        if (name === 'EXT_blend_minmax' && extention.indexOf('GL_EXT_blend_minmax') != -1)
        {
            return  {
                        MAX_EXT : 32776,
                        MIN_EXT : 32775,
                    };
        }
        else if (name === 'EXT_color_buffer_float' && extention.indexOf('GL_EXT_color_buffer_float') != -1)
        {
            return  {};
        }
        else if (name === 'EXT_color_buffer_half_float' && extention.indexOf('GL_EXT_color_buffer_half_float') != -1)
        {
            return  {};
        }
        else if (name === 'EXT_shader_texture_lod' && 
        (extention.indexOf('GL_EXT_shader_texture_lod') != -1 || extention.indexOf('GL_ARB_shader_texture_lod') != -1))
        {
            return  {};
        }
        else if (name === 'OES_element_index_uint' && (extention.indexOf('GL_OES_element_index_uint') != -1))
        {
            return  {};
        }
        else if (name === 'EXT_sRGB' && extention.indexOf('GL_EXT_sRGB') != -1)
        {
            return  {
                        SRGB_EXT : 0x8C40,
                        SRGB_ALPHA_EXT : 0x8C42,
                        SRGB8_ALPHA8_EXT : 0x8C43,
                        FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING_EXT : 0x8210
                    };
        }
        else if (name === 'WEBGL_compressed_texture_pvrtc'
            && extention.indexOf('GL_IMG_texture_compression_pvrtc') != -1)
        {
            return { 
                        COMPRESSED_RGBA_PVRTC_2BPPV1_IMG : 35843,
                        COMPRESSED_RGBA_PVRTC_4BPPV1_IMG : 35842,
                        COMPRESSED_RGB_PVRTC_2BPPV1_IMG : 35841,
                        COMPRESSED_RGB_PVRTC_4BPPV1_IMG : 35840,
                    };
        }
        else if (name === 'WEBGL_compressed_texture_etc1'
                && extention.indexOf('GL_OES_compressed_ETC1_RGB8_texture') != -1)
        {
            return {COMPRESSED_RGB_ETC1_WEBGL : 36196};
        }
        else if (name === 'WEBGL_compressed_texture_s3tc' 
                && supports('GL_EXT_texture_compression_s3tc'))//和下面易混淆，所以用supports
        {
            return { 
                COMPRESSED_RGBA_S3TC_DXT1_EXT : 33777,
                COMPRESSED_RGBA_S3TC_DXT3_EXT : 33778,
                COMPRESSED_RGBA_S3TC_DXT5_EXT : 33779,
                COMPRESSED_RGB_S3TC_DXT1_EXT : 33776,
            };
        }
        else if (name === 'WEBGL_compressed_texture_s3tc_srgb'
                && extention.indexOf('GL_EXT_texture_compression_s3tc_srgb') != -1)
        {
            return {
                COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT : 35917,
                COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT : 35918,
                COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT : 35919,
                COMPRESSED_SRGB_S3TC_DXT1_EXT : 35916,
            };
        }
        else if (name === 'WEBGL_debug_renderer_info')
        {
            return {
                UNMASKED_RENDERER_WEBGL : 37446,
                UNMASKED_VENDOR_WEBGL : 37445,
            };
        }
        else if (name === 'EXT_texture_filter_anisotropic' 
                && extention.indexOf('GL_EXT_texture_filter_anisotropic') != -1)
        {
            return  {
                        MAX_TEXTURE_MAX_ANISOTROPY_EXT : 34047,
                        TEXTURE_MAX_ANISOTROPY_EXT : 34046,
                    };
        }
        else if (name.indexOf('ANGLE_instanced_arrays') != -1 && ((extention.indexOf('_instanced_arrays') != -1) || version.indexOf("OpenGL ES 3.") != -1))
        {
            return new ANGLEInstancedArrays(this);
        }
        else if (name.indexOf('OES_vertex_array_object') != -1 &&  ((extention.indexOf('GL_OES_vertex_array_object') != -1)|| version.indexOf("OpenGL ES 3.") != -1))
        {
            return new OESVertexArrayObject(this);
        }
        else if (name.indexOf('OES_texture_float') != -1 &&  extention.indexOf('GL_OES_texture_float') != -1)
        {
            return {};
        }
        else if (name.indexOf('OES_texture_half_float') != -1 &&  (extention.indexOf('GL_OES_texture_half_float') != -1))
        {
            return {HALF_FLOAT_OES: 36193};
        }
        else if (name.indexOf('OES_texture_half_float_linear') != -1 &&  extention.indexOf('GL_OES_texture_half_float_linear') != -1)
        {
            return new OESTextureHalfFloatLinear();
        }
        else if (name.indexOf('EXT_color_buffer_float') != -1 &&  extention.indexOf('GL_EXT_color_buffer_float') != -1)
        {
            return new EXTColorBufferFloat();
        }
        else if (name.indexOf('WEBGL_depth_texture') != -1 && ((supports("GL_OES_depth_texture") || supports("GL_ARB_depth_texture") || supports("GL_ANGLE_depth_texture"))))
        {
            return {UNSIGNED_INT_24_8_WEBGL:34042};
        }
        //>=3.1
        else if (name.indexOf('WEBGL_compressed_texture_astc') != -1 && (extention.indexOf('GL_OES_texture_compression_astc') != -1 || extention.indexOf('GL_KHR_texture_compression_astc') != -1 || (conchConfig.getOS() == "Conch-android" && version.indexOf("OpenGL ES 3.") != -1 && version.indexOf("OpenGL ES 3.0") == -1)))
        {
            return {
                        COMPRESSED_RGBA_ASTC_4x4_KHR : 0x93B0,
                        COMPRESSED_RGBA_ASTC_5x4_KHR : 0x93B1,
                        COMPRESSED_RGBA_ASTC_5x5_KHR : 0x93B2,
                        COMPRESSED_RGBA_ASTC_6x5_KHR : 0x93B3,
                        COMPRESSED_RGBA_ASTC_6x6_KHR : 0x93B4,
                        COMPRESSED_RGBA_ASTC_8x5_KHR : 0x93B5,
                        COMPRESSED_RGBA_ASTC_8x6_KHR :  0x93B6,
                        COMPRESSED_RGBA_ASTC_8x8_KHR : 0x93B7,
                        COMPRESSED_RGBA_ASTC_10x5_KHR : 0x93B8,
                        COMPRESSED_RGBA_ASTC_10x6_KHR : 0x93B9,
                        COMPRESSED_RGBA_ASTC_10x8_KHR :  0x93BA,
                        COMPRESSED_RGBA_ASTC_10x10_KHR :  0x93BB,
                        COMPRESSED_RGBA_ASTC_12x10_KHR :  0x93BC,
                        COMPRESSED_RGBA_ASTC_12x12_KHR : 0x93BD,
                        COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR : 0x93D0,
                        COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR : 0x93D1,
                        COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR : 0x93D2,
                        COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR : 0x93D3,
                        COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR : 0x93D4,
                        COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR : 0x93D5,
                        COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR : 0x93D6,
                        COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR : 0x93D7,
                        COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR : 0x93D8,
                        COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR : 0x93D9,
                        COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR : 0x93DA,
                        COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR :  0x93DB,
                        COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR : 0x93DC,
                        COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR : 0x93DD,
                    };
        }
        else if (name.indexOf('WEBGL_compressed_texture_etc') != -1 && conchConfig.getOS() == "Conch-android" && version.indexOf("OpenGL ES 3.") != -1 ) 
        {
            return {
                        COMPRESSED_R11_EAC : 0x9270,
                        COMPRESSED_SIGNED_R11_EAC : 0x9271,
                        COMPRESSED_RG11_EAC : 0x9272,
                        COMPRESSED_SIGNED_RG11_EAC : 0x9273,
                        COMPRESSED_RGB8_ETC2 : 0x9274,
                        COMPRESSED_SRGB8_ETC2 : 0x9275,
                        COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2 : 0x9276,
                        COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2 : 0x9277,
                        COMPRESSED_RGBA8_ETC2_EAC : 0x9278,
                        COMPRESSED_SRGB8_ALPHA8_ETC2_EAC : 0x9279
                    };
        }
        else
        {
            console.log("getExtension " + name + " 尚未支持");
        }
        return null;
    }
    
    activeTexture(texture:number):void
    {
        this.add_ii(FUNCTION_ID.ACTIVETEXTURE, texture);
    }
    
    attachShader(program:WebGLProgram, shader:WebGLShader):void
    {
        if(shader.type == LayaGLContext.VERTEX_SHADER)
        {
            if(program.vsShader)
                return;

            program.vsShader = shader;
            program.attachedShaderCount++;
        }
        else if(shader.type == LayaGLContext.FRAGMENT_SHADER)
        {
            if(program.psShader)
                return;

            program.psShader = shader;
            program.attachedShaderCount++;
        }
        this.add_iii(FUNCTION_ID.ATTACHSHADER, program.id, shader.id);
    }
    
    bindAttribLocation(program:WebGLProgram, index:number, name:string):void
    {
        var strLen = this.getStrLength(name);
        this.add_iii(FUNCTION_ID.BINDATTRIBLOCATION, program.id, index);
        this.add_String(name, strLen);
    }
    
    bindBuffer(target:number, buffer:WebGLBuffer):void
    {
        this.add_iii(FUNCTION_ID.BINDBUFFER, target, buffer?buffer.id:0);
    }
    
    bindFramebuffer(target:number, framebuffer:WebGLFrameBuffer):void
    {
        this.add_iii(FUNCTION_ID.BINDFRAMEBUFFER, target, framebuffer?framebuffer.id:0);
    }
    
    bindRenderbuffer(target:number, renderbuffer:WebGLRenderBuffer):void
    {
        this.add_iii(FUNCTION_ID.BINDRENDERBUFFER, target, renderbuffer?renderbuffer.id:0);
    }

    clearBufferfv(buffer:number, drawbuffer:number, values:any, srcOffset?:number):void
    {
        let offset: number = srcOffset ? srcOffset : 0;
        this.add_iiiiiii(FUNCTION_ID.CLEARBUFFERFV, buffer, drawbuffer, values[0 + offset], values[1 + offset], values[2 + offset], values[3 + offset]);
    }
    blitFramebuffer(srcX0:number, srcY0:number, srcX1:number, srcY1:number, dstX0:number, dstY0:number, dstX1:number, dstY1:number, mask:number, filter:number):void
    {
        this.add_iiiiiiiiiii(FUNCTION_ID.BLITFRAMEBUFFER, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
    }
    bindTexture(target:number, texture:WebGLTexture):void
    {
        this.add_iii(FUNCTION_ID.BINDTEXTURE, target, texture?texture.id:0);
    }
    
    useTexture(texture:WebGLTexture):void
    {
        this.add_ii(FUNCTION_ID.USETEXTURE, texture?texture.id:0);
    }
    
    blendColor(red:number, green:number, blue:number, alpha:number):void
    {
        this.add_iffff(FUNCTION_ID.BLENDCOLOR, red, green, blue, alpha);
    }
    
    blendEquation(mode:number):void
    {
        this.add_ii(FUNCTION_ID.BLENDEQUATION, mode);
    }
    
    blendEquationSeparate(modeRGB:number, modeAlpha:number):void
    {
        this.add_iii(FUNCTION_ID.BLENDEQUATIONSEPARATE, modeRGB, modeAlpha);
    }
    
    blendFunc(sfactor:number, dfactor:number):void
    {
        this.add_iii(FUNCTION_ID.BLENDFUNC, sfactor, dfactor);
    }
    
    blendFuncSeparate(srcRGB:number, dstRGB:number, srcAlpha:number, dstAlpha:number):void
    {
        this.add_iiiii(FUNCTION_ID.BLENDFUNCSEPARATE, srcRGB, dstRGB, srcAlpha, dstAlpha);
    }
    
    bufferData(target:number, sizeOrArray:any, usage:number):void
    {
        if( ArrayBuffer.isView(sizeOrArray) || ( sizeOrArray instanceof ArrayBuffer) )
        {
            var nAlignLength = this.getAlignLength(sizeOrArray);
            this.add_iii(FUNCTION_ID.BUFFERDATA_ARRAYBUFFER, target, usage);
            this.wab(sizeOrArray, sizeOrArray.byteLength, nAlignLength);
        }
        else
        {
            var size:number = sizeOrArray;
            this.add_iiii(FUNCTION_ID.BUFFERDATA_SIZE, target, size, usage);
        }
    }
    
    bufferSubData(target:number, offset:number, data:any):void
    {
        var nAlignLength = this.getAlignLength(data);
        this.add_iii(FUNCTION_ID.BUFFERSUBDATA, target, offset);
        this.wab(data, data.byteLength, nAlignLength);
    }
    
    checkFramebufferStatus(target:any):any
    {
        return this._layagl._nativeObj.checkFramebufferStatusEx(target);
    }
    
    clear(mask:number):void
    {
        this.add_ii(FUNCTION_ID.CLEAR, mask);
    }
    
    clearColor(red:number, green:number, blue:number, alpha:number):void
    {
        this.add_iffff(FUNCTION_ID.CLEARCOLOR, red, green, blue, alpha);
    }
    
    clearDepth(depth:number):void
    {
        this.add_if(FUNCTION_ID.CLEARDEPTH, depth);
    }
    
    clearStencil(s:number):void
    {
        this.add_ii(FUNCTION_ID.CLEARSTENCIL, s);
    }
    
    colorMask(red:number, green:number, blue:number, alpha:number):void
    {
        this.add_iiiii(FUNCTION_ID.COLORMASK, red, green, blue, alpha);
    }
    
    compileShader(shader:WebGLShader):void
    {
        this.add_ii(FUNCTION_ID.COMPILESHADER, shader.id);
        shader.compileStatus = this._layagl._nativeObj.getShaderParameter(shader.src, shader.type, LayaGLContext.COMPILE_STATUS);
    }
    
    copyTexImage2D(target:number, level:number, internalformat:number, x:number, y:number, width:number, height:number, border:number):void
    {
        this.add_iiiiiiiii(FUNCTION_ID.COPYTEXIMAGE2D, target, level, internalformat, x, y, width, height, border);
    }
    
    copyTexSubImage2D(target:number, level:number, xoffset:number, yoffset:number, x:number, y:number, width:number, height:number):void
    {
        this.add_iiiiiiiii(FUNCTION_ID.COPYTEXSUBIMAGE2D, target, level, xoffset, yoffset, x, y, width, height);
    }
    
    createBuffer():WebGLBuffer
    {
        var fakeID:number = this.createFakeID();
        this.add_ii(FUNCTION_ID.CREATEBUFFER, fakeID);
        return new WebGLBuffer(fakeID);
    }
    
    createFramebuffer():WebGLFrameBuffer
    {
        var fakeID:number = this.createFakeID();
        this.add_ii(FUNCTION_ID.CREATEFRAMEBUFFER, fakeID);
        return new WebGLFrameBuffer(fakeID);
    }
    
    createProgram():WebGLProgram
    {
        var fakeID:number = this.createFakeID();
        this.add_ii(FUNCTION_ID.CREATEPROGRAM, fakeID);
        return new WebGLProgram(fakeID);
    }
    
    createRenderbuffer():WebGLRenderBuffer
    {
        var fakeID:number = this.createFakeID();
        this.add_ii(FUNCTION_ID.CREATERENDERBUFFER, fakeID);
        return new WebGLRenderBuffer(fakeID);
    }
    
    createShader(type:number):WebGLShader
    {
        var fakeID:number = this.createFakeID();
        this.add_iii(FUNCTION_ID.CREATESHADER, fakeID, type);
        return new WebGLShader(fakeID,type);
    }
    
    createTexture():WebGLTexture
    {
        var fakeID:number = this.createFakeID();
        this.add_ii(FUNCTION_ID.CREATETEXTURE, fakeID);
        return new WebGLTexture(fakeID);
    }
    
    cullFace(mode:number):void
    {
        this.add_ii(FUNCTION_ID.CULLFACE, mode);
    }
    
    deleteBuffer(buffer:WebGLBuffer):void
    {
        this.deleteFakeID(buffer.id);
        this.add_ii(FUNCTION_ID.DELETEBUFFER, buffer.id);
    }
    
    deleteFramebuffer(framebuffer:WebGLFrameBuffer):void
    {
        this.deleteFakeID(framebuffer.id);
        this.add_ii(FUNCTION_ID.DELETEFRAMEBUFFER, framebuffer.id);
    }
    
    deleteProgram(program:WebGLProgram):void
    {
        this.deleteFakeID(program.id);
        this.add_ii(FUNCTION_ID.DELETEPROGRAM, program.id);
    }
    
    deleteRenderbuffer(renderbuffer:WebGLRenderBuffer):void
    {
        this.deleteFakeID(renderbuffer.id);
        this.add_ii(FUNCTION_ID.DELETERENDERBUFFER, renderbuffer.id);
    }
    
    deleteShader(shader:WebGLShader):void
    {
        this.deleteFakeID(shader.id);
        this.add_ii(FUNCTION_ID.DELETESHADER, shader.id);
    }
    
    deleteTexture(texture:WebGLTexture):void
    {
        this.deleteFakeID(texture.id);
        this.add_ii(FUNCTION_ID.DELETETEXTURE, texture.id);
    }
    
    depthFunc(func:number):void
    {
        this.add_ii(FUNCTION_ID.DEPTHFUNC, func);
    }
    
    depthMask(flag:number):void
    {
        this.add_ii(FUNCTION_ID.DEPTHMASK, flag);
    }
    
    depthRange(zNear:number, zFar:number):void
    {
        this.add_iff(FUNCTION_ID.DEPTHRANGE, zNear, zFar);
    }
    
    detachShader(program:WebGLProgram, shader:WebGLShader):void
    {
        let shaderid = -1;
        if(program.vsShader && program.vsShader.id === shader.id)
        {
            shaderid = program.vsShader.id;
            program.vsShader = null;
        }
        else if (program.psShader && program.psShader.id === shader.id)
        {
            shaderid = program.psShader.id;
            program.psShader = null;
        }

        if(shaderid > 0)
        {
            program.attachedShaderCount--;
            this.add_iii(FUNCTION_ID.DETACHSHADER, program.id, shaderid);
        }
    }
    
    disable(cap:number):void
    {
        this.add_ii(FUNCTION_ID.DISABLE, cap);
    }
    
    disableVertexAttribArray(index:number):void
    {
        this.add_ii(FUNCTION_ID.DISABLEVERTEXATTRIBARRAY, index);
    }
    
    drawArrays(mode:number, first:number, count:number):void
    {
        this.add_iiii(FUNCTION_ID.DRAWARRAYS, mode, first, count);
    }
    
    drawElements(mode:number, count:number, type:number, offset:number):void
    {
        this.add_iiiii(FUNCTION_ID.DRAWELEMENTS, mode, count, type, offset);
    }
    
    enable(cap:number):void
    {
        this.add_ii(FUNCTION_ID.ENABLE, cap);
    }
    
    enableVertexAttribArray(index:number):void
    {
        this.add_ii(FUNCTION_ID.ENABLEVERTEXATTRIBARRAY, index);
    }
    
    finish():void
    {
        this.add_i(FUNCTION_ID.FINISH);
    }
    
    flush():void
    {
        this.add_i(FUNCTION_ID.FLUSH);
    }
    
    framebufferRenderbuffer(target:number, attachment:number, renderbuffertarget:number, renderbuffer:WebGLRenderBuffer):void
    {
        this.add_iiiii(FUNCTION_ID.FRAMEBUFFERRENDERBUFFER, target, attachment, renderbuffertarget, renderbuffer ? renderbuffer.id : 0);
    }
    
    framebufferTexture2D(target:number, attachment:number, textarget:number, texture:WebGLTexture, level:number):void
    {
        this.add_iiiiii(FUNCTION_ID.FRAMEBUFFERTEXTURE2D, target, attachment, textarget, texture ? texture.id : 0, level);
    }
    
    frontFace(mode:number):void
    {
        this.add_ii(FUNCTION_ID.FRONTFACE, mode);
    }
    
    generateMipmap(target:number):void
    {
        this.add_ii(FUNCTION_ID.GENERATEMIPMAP, target);
    }
    
    getActiveAttrib(program:WebGLProgram, index:number):any
    {
        return this._layagl._nativeObj.getActiveAttribEx(program.linkedVSSource,program.linkedPSSource,"",index);
    }
    
    getActiveUniform(program:WebGLProgram, index:number):any
    {
        return this._layagl._nativeObj.getActiveUniformEx(program.linkedVSSource,program.linkedPSSource,"",index);
    }
    
    getAttribLocation(program:WebGLProgram, name:string):number
    {
        return this._layagl._nativeObj.getAttribLocationEx(program.linkedVSSource,program.linkedPSSource,"",name);
    }
    
    getParameter(pname:number):any
    {
        var gl:LayaGLContext = this._layagl;
        var result:any = 0;

        switch(pname)
        {
        case LayaGLContext.ACTIVE_TEXTURE:     case LayaGLContext.ALPHA_BITS:        case LayaGLContext.BLEND_DST_ALPHA: 
        case LayaGLContext.BLEND_DST_RGB:      case LayaGLContext.BLEND_EQUATION:    case LayaGLContext.BLEND_EQUATION_ALPHA: 
        case LayaGLContext.BLEND_EQUATION_RGB: case LayaGLContext.BLEND_SRC_ALPHA:   case LayaGLContext.BLEND_SRC_RGB:        
        case LayaGLContext.BLUE_BITS:          case LayaGLContext.CULL_FACE_MODE:    case LayaGLContext.DEPTH_BITS:     
        case LayaGLContext.DEPTH_FUNC:         case LayaGLContext.FRONT_FACE:        case LayaGLContext.GENERATE_MIPMAP_HINT:
        case LayaGLContext.GREEN_BITS:         case LayaGLContext.IMPLEMENTATION_COLOR_READ_FORMAT:     case LayaGLContext.IMPLEMENTATION_COLOR_READ_TYPE:
        case LayaGLContext.MAX_COMBINED_TEXTURE_IMAGE_UNITS:     case LayaGLContext.MAX_CUBE_MAP_TEXTURE_SIZE:     case LayaGLContext.MAX_FRAGMENT_UNIFORM_VECTORS:
        case LayaGLContext.MAX_RENDERBUFFER_SIZE:     case LayaGLContext.MAX_TEXTURE_IMAGE_UNITS:     case LayaGLContext.MAX_TEXTURE_SIZE:
        case LayaGLContext.MAX_VARYING_VECTORS:     case LayaGLContext.MAX_VERTEX_ATTRIBS:    case LayaGLContext.MAX_VERTEX_TEXTURE_IMAGE_UNITS:
        case LayaGLContext.MAX_VERTEX_UNIFORM_VECTORS:    case LayaGLContext.SAMPLE_BUFFERS:    case LayaGLContext.SAMPLES:
        case LayaGLContext.STENCIL_BACK_FAIL:  case LayaGLContext.STENCIL_BACK_FUNC: case LayaGLContext.STENCIL_BACK_PASS_DEPTH_FAIL:
        case LayaGLContext.STENCIL_BACK_PASS_DEPTH_PASS:    case LayaGLContext.STENCIL_BACK_REF:    case LayaGLContext.STENCIL_BACK_VALUE_MASK:
        case LayaGLContext.STENCIL_BACK_WRITEMASK:    case LayaGLContext.STENCIL_BITS:    case LayaGLContext.STENCIL_CLEAR_VALUE:
        case LayaGLContext.STENCIL_FAIL:       case LayaGLContext.STENCIL_FUNC:      case LayaGLContext.STENCIL_PASS_DEPTH_FAIL:
        case LayaGLContext.STENCIL_PASS_DEPTH_PASS:    case LayaGLContext.STENCIL_REF:    case LayaGLContext.STENCIL_VALUE_MASK:
        case LayaGLContext.STENCIL_WRITEMASK:  case LayaGLContext.SUBPIXEL_BITS:    case LayaGLContext.UNPACK_ALIGNMENT:
            result = gl._nativeObj.getIntegerv(pname)
            break;
        case LayaGLContext.CURRENT_PROGRAM: 
            gl._nativeObj.flushCommand();
            result = this._layagl.getBind(pname);
            break;
        case LayaGLContext.ARRAY_BUFFER_BINDING:
            gl._nativeObj.flushCommand();
            result = this._layagl.getBind(LayaGLContext.ARRAY_BUFFER);
            break;
        case LayaGLContext.ELEMENT_ARRAY_BUFFER_BINDING:
            gl._nativeObj.flushCommand();
            result = this._layagl.getBind(LayaGLContext.ELEMENT_ARRAY_BUFFER)
            break;
        case LayaGLContext.RENDERBUFFER_BINDING:
            gl._nativeObj.flushCommand();
            result = this._layagl.getBind(LayaGLContext.RENDERBUFFER)
            break;
        case LayaGLContext.TEXTURE_BINDING_2D:
            gl._nativeObj.flushCommand();
            result = this._layagl.getBind(LayaGLContext.TEXTURE_2D);
            break;
        case LayaGLContext.TEXTURE_BINDING_CUBE_MAP:
            gl._nativeObj.flushCommand();
            result = this._layagl.getBind(LayaGLContext.TEXTURE_CUBE_MAP);
            break;
        case LayaGLContext.BLEND:
        case LayaGLContext.CULL_FACE:
        case LayaGLContext.DEPTH_TEST:
        case LayaGLContext.DEPTH_WRITEMASK:
        case LayaGLContext.DITHER:
        case LayaGLContext.SAMPLE_COVERAGE_INVERT:
        case LayaGLContext.SCISSOR_TEST:
        case LayaGLContext.STENCIL_TEST:
            var ret = gl._nativeObj.getBooleanv(pname);
            if(!ret) 
                result = false;
            else 
                result = true;
            break;
        case LayaGLContext.DEPTH_CLEAR_VALUE:
        case LayaGLContext.LINE_WIDTH:
        case LayaGLContext.SAMPLE_COVERAGE_VALUE:
            result = gl._nativeObj.getFloatv(pname);
            break;
        case LayaGLContext.ALIASED_LINE_WIDTH_RANGE:
        case LayaGLContext.ALIASED_POINT_SIZE_RANGE:
        case LayaGLContext.BLEND_COLOR:
        case LayaGLContext.COLOR_CLEAR_VALUE:
        case LayaGLContext.DEPTH_RANGE:
            result = gl._nativeObj.getFloatArrayv(pname);
            break;
        case LayaGLContext.MAX_VIEWPORT_DIMS:
        case LayaGLContext.SCISSOR_BOX:
        case LayaGLContext.VIEWPORT:
            result = gl._nativeObj.getIntegerArrayv(pname);
            break;
        case LayaGLContext.RENDERER:
        case LayaGLContext.SHADING_LANGUAGE_VERSION:
        case LayaGLContext.VENDOR:
        case LayaGLContext.VERSION:
            result = gl._nativeObj.getStringEx(pname);
            break;

        case LayaGLContext.MAX_TEXTURE_MAX_ANISOTROPY_EXT:
        case LayaGLContext.TEXTURE_MAX_ANISOTROPY_EXT:
            result =  gl._nativeObj.getParameter(pname);
            break;
        default:
            alert("getParameter 尚未支持");
            break;
        }
        return result;
    }
    
    getBufferParameter(target:number, pname:number):number
    {
        return this._layagl._nativeObj.getBufferParameterEx(target, pname);
    }
    
    getError():any
    {
        this.add_i(FUNCTION_ID.GETERROR);
        return 0;
    }
    
    getFramebufferAttachmentParameter(target:number, attachment:number, pname:number):any
    {
        var gl = this._layagl;
        var result:any = null;
        switch(pname)
        {
            case LayaGLContext.FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE:
                var frameBuffer:WebGLFrameBuffer = gl.getBind(target);
                if(frameBuffer)
                {
                    var attachBuffer = frameBuffer.getBind(attachment);
                    if(!attachBuffer)
                        result = LayaGLContext.NONE;
                    else if(attachBuffer instanceof WebGLTexture)
                        result = LayaGLContext.TEXTURE;
                    else if(attachBuffer instanceof WebGLRenderBuffer)
                        result = LayaGLContext.RENDERBUFFER;
                }
                break;
            case LayaGLContext.FRAMEBUFFER_ATTACHMENT_OBJECT_NAME:
                var frameBuffer:WebGLFrameBuffer = gl.getBind(target);
                if(frameBuffer)
                {
                    return frameBuffer.getBind(attachment);
                }
                break;
            case LayaGLContext.FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE:
            case LayaGLContext.FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL:
                result = gl._nativeObj.getFramebufferAttachmentParameter(target, attachment, pname);
                break;

            default:
                break;
        }


        return result;
    }
    
    getProgramParameter(program:WebGLProgram, pname:any):any
    {
        let ret:any;
        var gl:LayaGLContext = this._layagl;

        if(pname == LayaGLContext.ATTACHED_SHADERS)
            return program.attachedShaderCount;

        if(program.linkStatus == 0 || program.id == -1)
        {
            if(program.id == -1)
            {
                if(pname == LayaGLContext.DELETE_STATUS || pname == LayaGLContext.LINK_STATUS ||
                    pname == LayaGLContext.VALIDATE_STATUS)
                {
                    return null;
                }
            }
            else if(program.linkStatus == 0)
            {
                if(pname == LayaGLContext.DELETE_STATUS || pname == LayaGLContext.LINK_STATUS ||
                    pname == LayaGLContext.VALIDATE_STATUS)
                {
                    return false;
                }

            }

            return 0;
        }
        else
        {
            ret = this._layagl._nativeObj.getProgramParameterEx(program.linkedVSSource,program.linkedPSSource,"",pname);

            if(pname == LayaGLContext.DELETE_STATUS || pname == LayaGLContext.LINK_STATUS ||
               pname == LayaGLContext.VALIDATE_STATUS)
            {
                return ret == 1 ? true : false;
            }
            else
            {
                return ret;
            }
        }
    }
    
    getProgramInfoLog(program:WebGLProgram):any
    {
        if(program.id == -1)
            return null;

        if(program.linkStatus)
            return "";

        if(program.attachedShaderCount < 2)
        {
            return "missing shaders" + String.fromCharCode(0);
        }
        var ret =  this._layagl._nativeObj.getProgramInfoLogEx(program.vsShader.src,program.psShader.src,"");
        if(ret === "invalid shaders")
        {
            ret = ret + String.fromCharCode(0);
        }

        return ret;
    }
    
    getRenderbufferParameter(target:number, pname:number):number
    {
        return this._layagl._nativeObj.getRenderbufferParameterEx(target, pname);
    }
    
    getShaderPrecisionFormat(shaderType:number, precisionType:number):any
    {
        return this._layagl._nativeObj.getShaderPrecisionFormat(shaderType,precisionType);
    }
    
    getShaderParameter(shader:WebGLShader, pname:number):any
    {
        if(shader && shader.id == -1)
            return null;

        if(pname == LayaGLContext.COMPILE_STATUS)
        {
            return shader.compileStatus === 0 ? false : true;
        }

        return this._layagl._nativeObj.getShaderParameter(shader.src, shader.type,pname);
    }
    
    getShaderInfoLog(shader:WebGLShader):any
    {
        return this._layagl._nativeObj.getShaderInfoLogEx(shader.src,shader.type);
    }
    
    getShaderSource(shader:WebGLShader):any
    {
        if(!(shader instanceof WebGLShader))
            throw new TypeError("parameter 1 is not of type 'WebGLShader'");

        if(shader && shader.id !== -1)
            return shader.src;

        return null;
    }
    
    getTexParameter(target:any, pname:any):number
    {
        return this._layagl._nativeObj.getTexParameterEx(target, pname);
    }
    
    getUniform(program:WebGLProgram, location:number):any
    {
        if(!program.linkStatus || program.id == -1)
            return null;

        var gl = this._layagl;
        var curProgram = gl.getBind(LayaGLContext.CURRENT_PROGRAM);
        
        var result:any = null;

        this.useProgram(program);
        gl._nativeObj.flushCommand();

        var locationName = program.getLocationName(location);
        // ret: [type, ...data]
        var result =  this._layagl._nativeObj.getUniformEx(locationName);
        var type = result[0];
        result.splice(0, 1);
        
        if(type == LayaGLContext.BOOL)
        {
            for(var i = 0; i < result.length; i++)
            {
                if(result[i] === 0)
                    result[i] = false;
                else
                    result[i] = true;
            }
        }

        if(curProgram)
        {
            this.useProgram(curProgram);
            gl._nativeObj.flushCommand();
        }

        if(result.length == 1)
        {
            return result[0];
        }
        else
        {
            if(type == LayaGLContext.BOOL)
                return result;
            else if(type == LayaGLContext.FLOAT)
                return new Float32Array(result);
            else if(type == LayaGLContext.INT)
                return new Int32Array(result);
        }
        return null;
    }
    
    getUniformMutiThread(program:WebGLProgram, location:number):any
    {
        console.log("getUniformMutiThread can't support");
        return null;
    }


    getUniformLocation(program:WebGLProgram, name:string):any
    {
        var fakeLoc = GLCommandEncoder._locTable.getFakeLocation(program.id, name);
        var strLen = this.getStrLength(name);
        this.add_iii(FUNCTION_ID.GETUNIFORMLOCATION, program.id, fakeLoc);
        this.add_String(name, strLen);
        program.addLocationPair(fakeLoc, name);
        return fakeLoc;
    }
    
    getVertexAttrib(index:number, pname:any):any
    {
        var gl = this._layagl;
        var result:any = 0;
        switch(pname)
        {
            case LayaGLContext.VERTEX_ATTRIB_ARRAY_BUFFER_BINDING:
                gl._nativeObj.flushCommand();
                result = gl.getBind(LayaGLContext.ARRAY_BUFFER);
                break;

            case LayaGLContext.VERTEX_ATTRIB_ARRAY_ENABLED:
            case LayaGLContext.VERTEX_ATTRIB_ARRAY_NORMALIZED:
                var ret = gl._nativeObj.getVertexAttribEx(index, pname);
                if(ret === 0)
                    result = false;
                else
                    result = true;
                break;
            case LayaGLContext.VERTEX_ATTRIB_ARRAY_SIZE:
            case LayaGLContext.VERTEX_ATTRIB_ARRAY_STRIDE:
            case LayaGLContext.VERTEX_ATTRIB_ARRAY_TYPE:
                result = gl._nativeObj.getVertexAttribEx(index, pname);
                break;
            case LayaGLContext.CURRENT_VERTEX_ATTRIB:
                result = gl._nativeObj.getVertexAttribExfv(index, pname);
                break;
            default:
                result = 0;
                break;
        }

        return result;
    }
    
    getVertexAttribOffset(index:number, pname:any):any
    {
        return this._layagl._nativeObj.getVertexAttribOffset(index, pname);
    }
    
    hint(target:number, mode:number):void
    {
        this.add_iii(FUNCTION_ID.HINT, target, mode);
    }
    
    isBuffer(buffer:any):boolean
    {
        return (buffer instanceof WebGLBuffer);
    }
    
    isEnabled(cap:any):boolean
    {
        var ret = this._layagl._nativeObj.isEnabled(cap);
        if(!ret)
            return false;
        
        return true;
    }
    
    isFramebuffer(framebuffer:any):boolean
    {
        return (framebuffer instanceof WebGLFrameBuffer);
    }
    
    isProgram(program:any):boolean
    {
        return (program instanceof WebGLProgram);
    }
    
    isRenderbuffer(renderbuffer:any):boolean
    {
        return (renderbuffer instanceof WebGLRenderBuffer);
    }
    
    isShader(shader:any):boolean
    {
        return (shader instanceof WebGLShader);
    }
    
    isTexture(texture:any):boolean
    {
        return (texture instanceof WebGLTexture);
    }
    
    lineWidth(width:number):void
    {
        this.add_if(FUNCTION_ID.LINEWIDTH, width);
    }
    
    linkProgram(program:WebGLProgram):void
    {
        this.add_ii(FUNCTION_ID.LINKPROGRAM, program.id);
        if(program.attachedShaderCount >= 2)
        {
            var gl = this._layagl;
            program.linkStatus = gl._nativeObj.getProgramParameterEx(program.vsShader.src,program.psShader.src,"",LayaGLContext.LINK_STATUS);
            if(program.linkStatus)
            {
                program.linkedVSSource = program.vsShader.src;
                program.linkedPSSource = program.psShader.src;
            }
        }
    }
    
    pixelStorei(pname:number, param:number):void
    {
        this.add_iii(FUNCTION_ID.PIXELSTOREI, pname, param);
    }
    
    polygonOffset(factor:number, units:number):void
    {
        this.add_iff(FUNCTION_ID.POLYGONOFFSET, factor, units);
    }
    
    readPixels(x:number, y:number, width:number, height:number, format:any, type:any, pixels:any):void
    {
        if(!pixels && !(pixels instanceof Uint8Array) && !(pixels instanceof Uint16Array) &&
            !(pixels instanceof Float32Array))
            return;

        var ret = this._layagl._nativeObj.readPixels(x, y, width, height, format, type);
        //var len = Math.min(pixels.length, ret.length);
		pixels.set(new Uint8Array(ret));
    }

    readPixelsAsync(x:number, y:number, w:number, h:number, format:any, type:any, callBack:(data:ArrayBuffer)=>void):void
    {
        var fakeID:number = this.createFakeID();
        this._readPixelsAsyncCallbackFuncObj.addCallbackFunc(fakeID, callBack);
        this.add_iiiiiiiii(FUNCTION_ID.READPIXELS_ASYNC, x, y, w, h, format, type, this._readPixelsAsyncCallbackFuncObj.id, fakeID);
    }

    renderbufferStorage(target:number, internalformat:number, width:number, height:number):void
    {
        this.add_iiiii(FUNCTION_ID.RENDERBUFFERSTORAGE, target, internalformat, width, height);
    }

    renderbufferStorageMultisample(target:number, samples:number, internalformat:number, width:number, height:number):void
    {
        this.add_iiiiii(FUNCTION_ID.RENDERBUFFERSTORAGEMUILTISAMPLE, target, samples, internalformat, width, height);
    }

    sampleCoverage(value:number, invert:number):void
    {
        this.add_ifi(FUNCTION_ID.SAMPLECOVERAGE, value, invert);
    }
    
    scissor(x:number, y:number, width:number, height:number):void
    {
        this.add_iiiii(FUNCTION_ID.SCISSOR, x, y, width, height);
    }
    
    shaderSource(shader:WebGLShader, source:string):void
    {
        shader.src = source;
        var strLen = this.getStrLength(source);
        this.add_ii(FUNCTION_ID.SHADERSOURCE, shader.id);
        this.add_String(source, strLen);
    }
    
    stencilFunc(func:number, ref:number, mask:number):void
    {
        this.add_iiii(FUNCTION_ID.STENCILFUNC, func, ref, mask);
    }
    
    stencilFuncSeparate(face:number, func:number, ref:number, mask:number):void
    {
        this.add_iiiii(FUNCTION_ID.STENCILFUNCSEPARATE, face, func, ref, mask);
    }
    
    stencilMask(mask:number):void
    {
        this.add_ii(FUNCTION_ID.STENCILMASK, mask);
    }
    
    stencilMaskSeparate(face:number, mask:number):void
    {
        this.add_iii(FUNCTION_ID.STENCILMASKSEPARATE, face, mask);
    }
    
    stencilOp(fail:number, zfail:number, zpass:number):void
    {
        this.add_iiii(FUNCTION_ID.STENCILOP, fail, zfail, zpass);
    }
    
    stencilOpSeparate(face:number, fail:number, zfail:number, zpass:number):void
    {
        this.add_iiiii(FUNCTION_ID.STENCILOPSEPARATE, face, fail, zfail, zpass);
    }
    
    texImage2D(_args):void
    {
        var args = arguments;
        if (args.length == 6)
        {
            //目前只支持传入image
            if (args[5]._nativeObj)
            {
                //函数ID,target, level, internalformat, format, type, image
                this.add_iiiiiii(FUNCTION_ID.TEXIMAGE2D, args[0], args[1], args[2], args[3], args[4], args[5]._nativeObj.conchImgId);
            }
			else
            {
                this.add_iiiiiiiii(FUNCTION_ID.TEXIMAGE2DCANVAS, args[0], args[1], args[2], args[3], args[4], args[5]._ctx._targets._glTexture.id,  args[5].width, args[5].height);
            }
        }
        else if (args.length == 9)
        {
            if (args[8] == null)
            {
                this.add_iiiiiiiiii(FUNCTION_ID.TEXIMAGE2D_PIXEL, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7],0);
            }
            else if ( args[8] instanceof ArrayBuffer ||  ArrayBuffer.isView(args[8]) )
            {
                var ab:ArrayBuffer = args[8];
                var nAlignLength = this.getAlignLength(ab);
                //函数ID,target, level, internalformat, width, height, border, format, type, ArrayBufferView? pixels
                this.add_iiiiiiiii(FUNCTION_ID.TEXIMAGE2D_PIXEL, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]);
                this.wab(ab, ab.byteLength, nAlignLength);
            }
        }
    }
    texStorage2D(target:any, levels:any, internalformat:any, width:number, height:number):void
    {
        this.add_iiiiii(FUNCTION_ID.TEXSTORAGE2D, target, levels, internalformat, width, height);
    }
    texParameterf(target:number, pname:number, param:number):void
    {
        this.add_iiif(FUNCTION_ID.TEXPARAMETERF, target, pname, param);
    }
    
    texParameteri(target:number, pname:number, param:number):void
    {
        this.add_iiii(FUNCTION_ID.TEXPARAMETERI, target, pname, param);
    }
    
    texSubImage2D(_args):void
    {
        var args = arguments;
        if (args.length == 7)
        {
            //目前只支持传入image
            if (args[6]._nativeObj)
            {
                //函数ID,target,level,xoffset,yoffset,format,type,image
                this.add_iiiiiiii(FUNCTION_ID.TEXSUBIMAGE2D, args[0], args[1], args[2], args[3], args[4], args[5], args[6]._nativeObj.conchImgId);
            }
        }
        else if (args.length == 9)
        {
            if (args[8] == null)
            {
                this.add_iiiiiiiiii(FUNCTION_ID.TEXSUBIMAGE2D_PIXEL, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7],0);
            }
            else if ( args[8] instanceof ArrayBuffer ||  ArrayBuffer.isView(args[8]) )
            {
                var ab:ArrayBuffer = args[8];
                var nAlignLength = this.getAlignLength(ab);
                //函数ID,target,level,xoffset,yoffset,width,height,format,type,pixel
                this.add_iiiiiiiii(FUNCTION_ID.TEXSUBIMAGE2D_PIXEL, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]);
                this.wab(ab, ab.byteLength, nAlignLength);
            }
        }
    }
    
    uniform1f(location:any, x:number):void
    {
        this.add_iif(FUNCTION_ID.UNIFORM1F, location, x);
    }
    
    uniform1fv(location:any, values:any):void
    {
        values = (values instanceof Array )?new Float32Array(values):values;
        this.uniform1fv_laya(location,values);
    }

    uniform1fv_laya(location:any, values:any):void
    {
        var nAlignLength = this.getAlignLength(values);
        this.add_ii(FUNCTION_ID.UNIFORM1FV, location);
        this.wab(values, values.byteLength, nAlignLength);
    }
    
    uniform1i(location:any, x:number):void
    {
        this.add_iii(FUNCTION_ID.UNIFORM1I, location, x);
    }
    
    uniform1iv(location:any, values:any):void
    {
        values = (values instanceof Array )?new Int32Array(values):values;
        this.uniform1iv_laya(location,values);
    }

    uniform1iv_laya(location:any, values:any):void
    {
        var nAlignLength = this.getAlignLength(values);
        this.add_ii(FUNCTION_ID.UNIFORM1IV, location);
        this.wab(values, values.byteLength,nAlignLength);
    }
    
    uniform2f(location:any, x:number, y:number):void
    {
        this.add_iiff(FUNCTION_ID.UNIFORM2F, location, x, y);
    }
    
    uniform2fv(location:any, values:any):void
    {
        values = (values instanceof Array )?new Float32Array(values):values;
        this.uniform2fv_laya(location,values);
    }

    uniform2fv_laya(location:any, values:any):void
    {
        var nAlignLength = this.getAlignLength(values);
        this.add_ii(FUNCTION_ID.UNIFORM2FV, location);
        this.wab(values, values.byteLength, nAlignLength);
    }
    
    uniform2i(location:any, x:number, y:number):void
    {
        this.add_iiii(FUNCTION_ID.UNIFORM2I, location, x, y);
    }
    
    uniform2iv(location:any, values:any):void
    {
        values = (values instanceof Array )?new Int32Array(values):values;
        this.uniform2iv_laya(location,values);
    }

    uniform2iv_laya(location:any, values:any):void
    {
        var nAlignLength = this.getAlignLength(values);
        this.add_ii(FUNCTION_ID.UNIFORM2IV, location);
        this.wab(values, values.byteLength, nAlignLength);
    }
    
    uniform3f(location:any, x:number, y:number, z:number):void
    {
        this.add_iifff(FUNCTION_ID.UNIFORM3F, location, x, y, z);
    }
    
    uniform3fv(location:any, values:any):void
    {        
        values = (values instanceof Array )?new Float32Array(values):values;
        this.uniform3fv_laya(location,values);
    }
    
    uniform3fv_laya(location:any, values:any):void
    {        
        var nAlignLength = this.getAlignLength(values);
        this.add_ii(FUNCTION_ID.UNIFORM3FV, location);
        this.wab(values, values.byteLength, nAlignLength);
    }

    uniform3i(location:any, x:number, y:number, z:number):void
    {
        this.add_iiiii(FUNCTION_ID.UNIFORM3I, location, x, y, z);
    }
    
    uniform3iv(location:any, values:any):void
    {
        values = (values instanceof Array )?new Int32Array(values):values;
        this.uniform3iv_laya(location,values);
    }
    
    uniform3iv_laya(location:any, values:any):void
    {
        var nAlignLength = this.getAlignLength(values);
        this.add_ii(FUNCTION_ID.UNIFORM3IV, location);
        this.wab(values, values.byteLength, nAlignLength);
    }

    uniform4f(location:any, x:number, y:number, z:number, w:number):void
    {
        this.add_iiffff(FUNCTION_ID.UNIFORM4F, location, x, y, z, w);
    }
    
    uniform4fv(location:any, values:any):void
    {
        values = (values instanceof Array )?new Float32Array(values):values;
        this.uniform4fv_laya(location,values);
    }

    uniform4fv_laya(location:any, values:any):void
    {
        var nAlignLength = this.getAlignLength(values);
        this.add_ii(FUNCTION_ID.UNIFORM4FV, location);
        this.wab(values, values.byteLength, nAlignLength);
    }
    
    uniform4i(location:any, x:number, y:number, z:number, w:number):void
    {
        this.add_iiiiii(FUNCTION_ID.UNIFORM4I, location, x, y, z, w);
    }
    
    uniform4iv(location:any, values:any):void
    {
        values = (values instanceof Array )?new Int32Array(values):values;
        this.uniform4iv_laya(location,values);
    }

    uniform4iv_laya(location:any, values:any):void
    {
        var nAlignLength = this.getAlignLength(values);
        this.add_ii(FUNCTION_ID.UNIFORM4IV, location);
        this.wab(values, values.byteLength, nAlignLength);
    }
    
    uniformMatrix2fv(location:any, transpose:any, values:any):void
    {
        values = (values instanceof Array )?new Float32Array(values):values;
        this.uniformMatrix2fv_laya(location,transpose,values);
    }

    uniformMatrix2fv_laya(location:any, transpose:any, value:any):void
    {
        var nAlignLength = this.getAlignLength(value);
        this.add_iii(FUNCTION_ID.UNIFORMMATRIX2FV, location, transpose);
        this.wab(value, value.byteLength, nAlignLength);
    }

    uniformMatrix3fv(location:any, transpose:any, values:any):void
    {
        values = (values instanceof Array )?new Float32Array(values):values;
        this.uniformMatrix3fv_laya(location,transpose,values);
    }

    uniformMatrix3fv_laya(location:any, transpose:any, value:any):void
    {
        var nAlignLength = this.getAlignLength(value);
        this.add_iii(FUNCTION_ID.UNIFORMMATRIX3FV, location, transpose);
        this.wab(value, value.byteLength, nAlignLength);
    }
    
    uniformMatrix4fv(location:any, transpose:any, values:any):void
    {
        values = (values instanceof Array )?new Float32Array(values):values;
        this.uniformMatrix4fv_laya(location,transpose,values);
    }

    uniformMatrix4fv_laya(location:any, transpose:any, value:any):void
    {
        var nAlignLength = this.getAlignLength(value);
        this.add_iii(FUNCTION_ID.UNIFORMMATRIX4FV, location, transpose);
        this.wab(value, value.byteLength, nAlignLength);
    }
    
    useProgram(program:WebGLProgram):void
    {
        this.add_ii(FUNCTION_ID.USEPROGRAM, program ? program.id : 0);
    }
    
    validateProgram(program:WebGLProgram):void
    {
        this.add_ii(FUNCTION_ID.VALIDATEPROGRAM, program.id);
    }
    
    vertexAttrib1f(indx:any, x:number):void
    {
        this.add_iif(FUNCTION_ID.VERTEXATTRIB1F, indx, x);
    }
    
    vertexAttrib1fv(indx:any, values:any):void
    {
        var nAlignLength = this.getAlignLength(values);
        this.add_ii(FUNCTION_ID.VERTEXATTRIB1FV, indx);
        this.wab(values, values.byteLength, nAlignLength);
    }
    
    vertexAttrib2f(indx:any, x:number, y:number):void
    {
        this.add_iiff(FUNCTION_ID.VERTEXATTRIB2F, indx, x, y);
    }
    
    vertexAttrib2fv(indx:any, values:any):void
    {
        var nAlignLength = this.getAlignLength(values);
        this.add_ii(FUNCTION_ID.VERTEXATTRIB2FV, indx);
        this.wab(values, values.byteLength, nAlignLength);
    }
    
    vertexAttrib3f(indx:any, x:number, y:number, z:number):void
    {
        this.add_iifff(FUNCTION_ID.VERTEXATTRIB3F, indx, x, y, z);
    }
    
    vertexAttrib3fv(indx:any, values:any):void
    {
        var nAlignLength = this.getAlignLength(values);
        this.add_ii(FUNCTION_ID.VERTEXATTRIB3FV, indx);
        this.wab(values, values.byteLength, nAlignLength);
    }
    
    vertexAttrib4f(indx:any, x:number, y:number, z:number, w:number):void
    {
        this.add_iiffff(FUNCTION_ID.VERTEXATTRIB4F, indx, x, y, z, w);
    }
    
    vertexAttrib4fv(indx:any, values:any):void
    {
        var nAlignLength = this.getAlignLength(values);
        this.add_ii(FUNCTION_ID.VERTEXATTRIB4FV, indx);
        this.wab(values, values.byteLength, nAlignLength);
    }
    
    vertexAttribPointer(indx:any, size:any, type:any, normalized:any, stride:any, offset:number):void
    {
        this.add_iiiiiii(FUNCTION_ID.VERTEXATTRIBPOINTER, indx, size, type, normalized, stride, offset);
    }
    
    viewport(x:number, y:number, width:number, height:number):void
    {
        this.add_iiiii(FUNCTION_ID.VIEWPORT, x, y, width, height);
    }
    
    configureBackBuffer(width:number, height:number, antiAlias:number, enableDepthAndStencil?:Boolean, wantsBestResolution?:Boolean):void
    {
        enableDepthAndStencil=enableDepthAndStencil?enableDepthAndStencil:true;
        wantsBestResolution=wantsBestResolution?wantsBestResolution:false;
        console.log("configureBackBuffer can't support");
        //add_iiiiii(CONFIGUREBACKBUFFER, width, height, antiAlias, enableDepthAndStencil, wantsBestResolution);
    }
    compressedTexImage2D(_args):void
    {
        var args=arguments;
        var nAlignLength = this.getAlignLength(args[6]);
        this.add_iiiiiii(FUNCTION_ID.COMPRESSEDTEXIMAGE2D, args[0], args[1], args[2], args[3], args[4], args[5]);
        this.wab(args[6], args[6].byteLength, nAlignLength);
    }
    compressedTexSubImage2D(_args):void
    {
        var args=arguments;
        var nAlignLength = this.getAlignLength(args[7]);
        this.add_iiiiiiii(FUNCTION_ID.COMPRESSEDTEXSUBIMAGE2D, args[0], args[1], args[2], args[3], args[4], args[5], args[6]);
        this.wab(args[7], args[7].byteLength, nAlignLength);
    }
    createVertexArray():WebGLVertextArray
    {
        var fakeID:number = this.createFakeID();
        this.add_ii(FUNCTION_ID.CREATEVERTEXARRAY, fakeID);
        return new WebGLVertextArray(fakeID);
    }
    bindVertexArray(vao:WebGLVertextArray):void
    {
        this.add_ii(FUNCTION_ID.BINDVERTEXARRAY,vao?vao.id:0);
    }
    deleteVertexArray(vao:WebGLVertextArray):void
    {
        this.deleteFakeID(vao.id);
        this.add_ii(FUNCTION_ID.DELETEVERTEXARRAYS,vao?vao.id:0);
    }
    isVertexArray(vao:any):boolean
    {
        return vao.instanceof(WebGLVertextArray);
    }
    vertexAttribDivisor(index:number, divisor:number):void
    {
        this.add_iii(FUNCTION_ID.VERTEXATTRIBDIVISOR, index, divisor);
    }
    drawArraysInstanced(mode:number, first:number, count:number, instanceCount:number):void
    {
        this.add_iiiii(FUNCTION_ID.DRAWARRAYSINSTANCED, mode, first, count, instanceCount);
    }
    drawElementsInstanced(mode:number, count:number, type:number, offset:number, instanceCount:number):void
    {
        this.add_iiiiii(FUNCTION_ID.DRAWELEMENTSINSTANCED, mode, count, type, offset, instanceCount);
    }
    uniformMatrix2fvEx(location:any, transpose:any, value:any):void
    {
        if (!value["_ptrID"])
        {
            this._layagl.createArrayBufferRef(value, LayaGLContext.ARRAY_BUFFER_TYPE_DATA, true);
        }
        var nID:number = value["_ptrID"];
        this._layagl.syncBufferToRenderThread(value);
        this.add_iiii(FUNCTION_ID.UNIFORMMATRIX2FVEX, location, transpose, nID);
    }
    uniformMatrix3fvEx(location:any, transpose:any, value:any):void
    {
        if (!value["_ptrID"])
        {
            this._layagl.createArrayBufferRef(value, LayaGLContext.ARRAY_BUFFER_TYPE_DATA, true);
        }
        var nID:number = value["_ptrID"];
        this._layagl.syncBufferToRenderThread(value);
        this.add_iiii(FUNCTION_ID.UNIFORMMATRIX3FVEX, location, transpose, nID);
    }
    uniformMatrix4fvEx(location:any, transpose:any, value:any):void
    {
        if (!value["_ptrID"])
        {
            this._layagl.createArrayBufferRef(value, LayaGLContext.ARRAY_BUFFER_TYPE_DATA, true);
        }
        var nID:number = value["_ptrID"];
        this._layagl.syncBufferToRenderThread(value);
        this.add_iiii(FUNCTION_ID.UNIFORMMATRIX4FVEX, location, transpose, nID);
    }
    addShaderUniform(one:any):void
    {
        var funID:number = 0;
        var isArray:Boolean = one.isArray;
        switch (one.type)
        {
        case GLCommandEncoder.INT: 
            funID = isArray ? UNIFORM_TYPE.INTERIOR_UNIFORM1IV : UNIFORM_TYPE.INTERIOR_UNIFORM1I;
            break;
        case GLCommandEncoder.FLOAT: 
            funID = isArray ? UNIFORM_TYPE.INTERIOR_UNIFORM1FV : UNIFORM_TYPE.INTERIOR_UNIFORM1F;
            break;
        case GLCommandEncoder.FLOAT_VEC2: 
            funID = isArray ? UNIFORM_TYPE.INTERIOR_UNIFORM2FV : UNIFORM_TYPE.INTERIOR_UNIFORM2F;
            break;
        case GLCommandEncoder.FLOAT_VEC3: 
            funID = isArray ? UNIFORM_TYPE.INTERIOR_UNIFORM3FV : UNIFORM_TYPE.INTERIOR_UNIFORM3F;
            break;
        case GLCommandEncoder.FLOAT_VEC4: 
            funID = isArray ? UNIFORM_TYPE.INTERIOR_UNIFORM4FV : UNIFORM_TYPE.INTERIOR_UNIFORM4F;
            break;
        case GLCommandEncoder.SAMPLER_2D: 
            funID = UNIFORM_TYPE.INTERIOR_UNIFORMSAMPLER_2D;
            break;
        case GLCommandEncoder.SAMPLER_CUBE: 
            funID = UNIFORM_TYPE.INTERIOR_UNIFORMSAMPLER_CUBE;
            break;
        case GLCommandEncoder.FLOAT_MAT4: 
            funID = UNIFORM_TYPE.INTERIOR_UNIFORMMATRIX4FV;
            break;
        case GLCommandEncoder.BOOL: 
            funID = UNIFORM_TYPE.INTERIOR_UNIFORM1I;
            break;
        case GLCommandEncoder.FLOAT_MAT2: 
            funID = UNIFORM_TYPE.INTERIOR_UNIFORMMATRIX2FV;
            break;
        case GLCommandEncoder.FLOAT_MAT3: 
            funID = UNIFORM_TYPE.INTERIOR_UNIFORMMATRIX3FV;
            break;
        default: 
            throw new Error("compile shader err!");
        }
        //同步ArrayBufferList
        this._layagl.syncBufferToRenderThread(this._buffer);
        this.add_iiiiii(FUNCTION_ID.ADDSHADERUNIFORM, funID, one.location, one.type, one.dataOffset, one.textureID);
    }
    uploadShaderUniforms(commandEncoder:GLCommandEncoder, data:any, type:number):number
    {
        if(type==LayaGLContext.UPLOAD_SHADER_UNIFORM_TYPE_ID)
        {
            this._layagl.syncBufferToRenderThread(data);
            this.add_iii(FUNCTION_ID.UPLOADSHADERUNIFORMS, commandEncoder._buffer["_ptrID"], data._ptrID);
        }
        else
        {
            var nAlignLength = this.getAlignLength(data);
            this.add_ii(FUNCTION_ID.UPLOADSHADERUNIFORMS_BUFFER, commandEncoder._buffer["_ptrID"]);
            this.wab(data, data.byteLength, nAlignLength);            
        }
        return 0;
    }
    createFakeID():number
    {
        var frameCount:number = this._layagl.getFrameCount();
        var fakeArray:any = GLCommandEncoder._fakeArray;
        for( var i:number=0,n:number=fakeArray.length;i<n;i++)
        {
            var fakeObj:fakeIDObj = fakeArray[i];
            if(fakeObj.id == -1 && fakeObj.frameCount != frameCount)
            {
                fakeObj.id = i;
                fakeObj.frameCount = frameCount;
                return i;
            }
        }
        var nID:number = ++GLCommandEncoder._fakeIDCount;
        fakeArray.push( new fakeIDObj(nID,frameCount) );
        return nID;
    }
    deleteFakeID(nID:number)
    {
        var fakeObj:fakeIDObj = GLCommandEncoder._fakeArray[nID];
        fakeObj.id = -1;
        fakeObj.frameCount = this._layagl.getFrameCount();
    }
    useCommandEncoder(commandEncoder:GLCommandEncoder):number
    {
        if(commandEncoder._isSyncToRenderThread)
        {
            this._layagl.syncBufferToRenderThread(commandEncoder);
        }
        var loc = this._idata[0];
        this.add_ii(FUNCTION_ID.USECOMMANDENCODER, commandEncoder.getPtrID());
        return loc;
    }
    getProgramParameterEx(vs:string,ps:string,define:string,pname:number):number
    {
        return this._layagl.getProgramParameterEx(vs,ps,define,pname);
    }
    getActiveAttribEx(vs:string,ps:string,define:string,index:number):any
    {
        return this._layagl.getActiveAttribEx(vs,ps,define,index);
    }
    getActiveUniformEx(vs:string,ps:string,define:string,index:number):any
    {
        return this._layagl.getActiveUniformEx(vs,ps,define,index);
    }
    loadDataToReg(regNum:number,data:any,offset:number,size:number):void{
        var nAlignLength = this.getAlignLength(data);
        this.add_iiii(FUNCTION_ID.LOADDATATOREG, regNum, offset, size);
        this.wab(data,data.byteLength, nAlignLength);
    }
    loadDataToRegEx(regNum:number,dataID:number,offset:number,size:number):void{
        this.add_iiiii(FUNCTION_ID.LOADDATATOREGEX,regNum,dataID,offset,size);
    }
    ifLess0(regNum:number,statementNum:number):void{
        this.add_iii(FUNCTION_ID.IFLESS0,regNum,statementNum);
    }
    ifEqual0(regNum:number,statementNum:number):void{
        this.add_iii(FUNCTION_ID.IFEQUAL0,regNum,statementNum);
    }
    ifGreater0(regNum:number,statementNum:number):void{
        this.add_iii(FUNCTION_ID.IFGREATER0,regNum,statementNum);
    }
    ifLEqual0(regNum:number,statementNum:number):void{
        this.add_iii(FUNCTION_ID.IFLEQUAL0,regNum,statementNum);
    }
    ifGEqual0(regNum:number,statementNum:number):void{
        this.add_iii(FUNCTION_ID.IFGEQUAL0,regNum,statementNum);
    }
    ifGNotEqual0(regNum:number,statementNum:number):void{
        this.add_iii(FUNCTION_ID.IFGNOTEQUAL0,regNum,statementNum);
    }
    operateReg(regOut:number,reg1:number,reg2:number,size:number,operateType:number, dataType:number):void{
        this.add_iiiiiii(FUNCTION_ID.OPERATEREG,regOut,reg1,reg2,size,operateType, dataType);
    }
    store(dataID:number, offset:number, regID:number, size:number):void{
        this.add_iiiii(FUNCTION_ID.STORE, dataID, offset, regID, size);
    }
    getAlignLength(data:any):number{
        var byteLength = data.byteLength;
        return (byteLength + 3) & 0xfffffffc;
    }
    getStrLength(str:any):number{
        var ab:ArrayBuffer = conch.strTobufer(str); //现在这个函数返回的已经是对齐的了
        return ab.byteLength;
    }
}

class LayaGLContext
{
    //-------------------------------------------------------------------------------------
    public static instance:LayaGLContext;
    public static readonly _SYNC_ARRAYBUFFER_SIZE_ = 4096;
    public static _syncBufferList:Int32Array;
    public static _frameAndSyncCountBuffer:Int32Array;
    private static _syncBufferSize =LayaGLContext._SYNC_ARRAYBUFFER_SIZE_;
    private static _tempGLEncoder:GLCommandEncoder = null;
    private static _shader_macro_id_ = 1;
    public _nativeObj:_layagl;
    public width:number = 0;
    public height:number = 0;
    public canvas:HTMLCanvasElement=null;
    private _defaultEncoder:GLCommandEncoder;
    private _currentCmdEncoder:GLCommandEncoder = null;
    private _saveCommandEncoder:GLCommandEncoder[] = new Array();
    private _currentContext:CanvasRenderingContext = null;
    private _threadMode:number = LayaGLContext.THREAD_MODE_DOUBLE;
    private _curBindInfo:BindInfo = new BindInfo();

    //-------------------------------------------------------------------------------------
    public static readonly THREAD_MODE_SINGLE = 1;                  //单线程模式
    public static readonly THREAD_MODE_DOUBLE = 2;                  //双线程模式
    //-------------------------------------------------------------------------------------
    public static readonly EXECUTE_JS_THREAD_BUFFER = 0;			//直接执行JS线程中的buffer
    public static readonly EXECUTE_RENDER_THREAD_BUFFER = 1;		//直接执行渲染线程的buffer
    public static readonly EXECUTE_COPY_TO_RENDER = 2;				//拷贝buffer到渲染线程
    public static readonly EXECUTE_COPY_TO_RENDER3D = 3;			//拷贝3Dbuffer到渲染线程
    
    //-------------------------------------------------------------------------------------
    public static readonly VALUE_OPERATE_ADD = 0;			
    public static readonly VALUE_OPERATE_SUB = 1;
    public static readonly VALUE_OPERATE_MUL = 2;
    public static readonly VALUE_OPERATE_DIV = 3;
    public static readonly VALUE_OPERATE_M2_MUL = 4;
    public static readonly VALUE_OPERATE_M3_MUL = 5;
    public static readonly VALUE_OPERATE_M4_MUL = 6;
    public static readonly VALUE_OPERATE_M32_MUL = 7;
    public static readonly VALUE_OPERATE_SET = 8;
    public static readonly VALUE_OPERATE_M32_TRANSLATE = 9;
    public static readonly VALUE_OPERATE_M32_SCALE = 10;
    public static readonly VALUE_OPERATE_M32_ROTATE = 11;
    public static readonly VALUE_OPERATE_M32_SCALE_PIVOT = 12;
    public static readonly VALUE_OPERATE_M32_ROTATE_PIVOT = 13;
    public static readonly VALUE_OPERATE_M32_TRANSFORM_PIVOT = 14;
    public static readonly VALUE_OPERATE_BYTE4_COLOR_MUL = 15;

    //-------------------------------------------------------------------------------------
	public static readonly ARRAY_BUFFER_TYPE_DATA = 0;          //创建ArrayBuffer时的类型为Data
    public static readonly ARRAY_BUFFER_TYPE_CMD = 1;           //创建ArrayBuffer时的类型为Command
    public static readonly ARRAY_BUFFER_REF_REFERENCE = 0;          //创建ArrayBuffer时的类型为引用
    public static readonly ARRAY_BUFFER_REF_COPY = 1;               //创建ArrayBuffer时的类型为拷贝

    public static readonly UPLOAD_SHADER_UNIFORM_TYPE_ID = 0;       //data按照ID传入
    public static readonly UPLOAD_SHADER_UNIFORM_TYPE_DATA = 1;     //data按照数据传入


    public static readonly DEPTH_BUFFER_BIT = 0x00000100;
	public readonly DEPTH_BUFFER_BIT = 0x00000100;
	
    public static readonly STENCIL_BUFFER_BIT = 0x00000400;
	public readonly STENCIL_BUFFER_BIT = 0x00000400;
	
    public static readonly COLOR_BUFFER_BIT = 0x00004000;
	public readonly COLOR_BUFFER_BIT = 0x00004000;
	
    public static readonly POINTS = 0x0000;
	public readonly POINTS = 0x0000;
	
    public static readonly LINES = 0x0001;
	public readonly LINES = 0x0001;
	
    public static readonly LINE_LOOP = 0x0002;
	public readonly LINE_LOOP = 0x0002;
	
    public static readonly LINE_STRIP = 0x0003;
	public readonly LINE_STRIP = 0x0003;
	 
    public static readonly TRIANGLES = 0x0004;
	public readonly TRIANGLES = 0x0004;
	
    public static readonly TRIANGLE_STRIP = 0x0005;
	public readonly TRIANGLE_STRIP = 0x0005;
	
    public static readonly TRIANGLE_FAN = 0x0006;
	public readonly TRIANGLE_FAN = 0x0006;
	
    public static readonly ZERO = 0;
	public readonly ZERO = 0;
	
    public static readonly ONE = 1;
	public readonly ONE = 1;
	
    public static readonly SRC_COLOR = 0x0300;
	public readonly SRC_COLOR = 0x0300;
	
    public static readonly ONE_MINUS_SRC_COLOR = 0x0301;
	public readonly ONE_MINUS_SRC_COLOR = 0x0301;
	
    public static readonly SRC_ALPHA = 0x0302;
	public readonly SRC_ALPHA = 0x0302;
	
    public static readonly ONE_MINUS_SRC_ALPHA = 0x0303;
	public readonly ONE_MINUS_SRC_ALPHA = 0x0303;
	
    public static readonly DST_ALPHA = 0x0304;
	public readonly DST_ALPHA = 0x0304;
	
    public static readonly ONE_MINUS_DST_ALPHA = 0x0305;
	public readonly ONE_MINUS_DST_ALPHA = 0x0305;
	
    public static readonly DST_COLOR = 0x0306;
	public readonly DST_COLOR = 0x0306;
	
    public static readonly ONE_MINUS_DST_COLOR = 0x0307;
	public readonly ONE_MINUS_DST_COLOR = 0x0307;
	
    public static readonly SRC_ALPHA_SATURATE = 0x0308;
	public readonly SRC_ALPHA_SATURATE = 0x0308;
	
    public static readonly FUNC_ADD = 0x8006;
	public readonly FUNC_ADD = 0x8006;
	
    public static readonly BLEND_EQUATION = 0x8009;
	public readonly BLEND_EQUATION = 0x8009;
	
    public static readonly BLEND_EQUATION_RGB = 0x8009;
	public readonly BLEND_EQUATION_RGB = 0x8009;
	
    public static readonly BLEND_EQUATION_ALPHA = 0x883D;
	public readonly BLEND_EQUATION_ALPHA = 0x883D;
	
    public static readonly FUNC_SUBTRACT = 0x800A;
	public readonly FUNC_SUBTRACT = 0x800A;
	
    public static readonly FUNC_REVERSE_SUBTRACT = 0x800B;
	public readonly FUNC_REVERSE_SUBTRACT = 0x800B;
	
    public static readonly BLEND_DST_RGB = 0x80C8;
	public readonly BLEND_DST_RGB = 0x80C8;
	
    public static readonly BLEND_SRC_RGB = 0x80C9;
	public readonly BLEND_SRC_RGB = 0x80C9;
	
    public static readonly BLEND_DST_ALPHA = 0x80CA;
	public readonly BLEND_DST_ALPHA = 0x80CA;
	
    public static readonly BLEND_SRC_ALPHA = 0x80CB;
	public readonly BLEND_SRC_ALPHA = 0x80CB;
	
    public static readonly CONSTANT_COLOR = 0x8001;
	public readonly CONSTANT_COLOR = 0x8001;
	
    public static readonly ONE_MINUS_CONSTANT_COLOR = 0x8002;
	public readonly ONE_MINUS_CONSTANT_COLOR = 0x8002;
	
    public static readonly CONSTANT_ALPHA = 0x8003;
	public readonly CONSTANT_ALPHA = 0x8003;
	
    public static readonly ONE_MINUS_CONSTANT_ALPHA = 0x8004;
	public readonly ONE_MINUS_CONSTANT_ALPHA = 0x8004;
	
    public static readonly BLEND_COLOR = 0x8005;
	public readonly BLEND_COLOR = 0x8005;
	
    public static readonly ARRAY_BUFFER = 0x8892;
	public readonly ARRAY_BUFFER = 0x8892;
	
    public static readonly ELEMENT_ARRAY_BUFFER = 0x8893;
	public readonly ELEMENT_ARRAY_BUFFER = 0x8893;
	
    public static readonly ARRAY_BUFFER_BINDING = 0x8894;
	public readonly ARRAY_BUFFER_BINDING = 0x8894;
	
    public static readonly ELEMENT_ARRAY_BUFFER_BINDING = 0x8895;
	public readonly ELEMENT_ARRAY_BUFFER_BINDING = 0x8895;
	
    public static readonly STREAM_DRAW = 0x88E0;
	public readonly STREAM_DRAW = 0x88E0;
	
    public static readonly STATIC_DRAW = 0x88E4;
	public readonly STATIC_DRAW = 0x88E4;
	
    public static readonly DYNAMIC_DRAW = 0x88E8;
	public readonly DYNAMIC_DRAW = 0x88E8;
	
    public static readonly BUFFER_SIZE = 0x8764;
	public readonly BUFFER_SIZE = 0x8764;
	
    public static readonly BUFFER_USAGE = 0x8765;
	public readonly BUFFER_USAGE = 0x8765;
	
    public static readonly CURRENT_VERTEX_ATTRIB = 0x8626;
	public readonly CURRENT_VERTEX_ATTRIB = 0x8626;
	
    public static readonly FRONT = 0x0404;
	public readonly FRONT = 0x0404;
	
    public static readonly BACK = 0x0405;
	public readonly BACK = 0x0405;
	
    public static readonly FRONT_AND_BACK = 0x0408;
	public readonly FRONT_AND_BACK = 0x0408;
	
    public static readonly CULL_FACE = 0x0B44;
	public readonly CULL_FACE = 0x0B44;
	
    public static readonly BLEND = 0x0BE2;
	public readonly BLEND = 0x0BE2;
	
    public static readonly DITHER = 0x0BD0;
	public readonly DITHER = 0x0BD0;
	
    public static readonly STENCIL_TEST = 0x0B90;
	public readonly STENCIL_TEST = 0x0B90;
	
    public static readonly DEPTH_TEST = 0x0B71;
	public readonly DEPTH_TEST = 0x0B71;
	
    public static readonly SCISSOR_TEST = 0x0C11;
	public readonly SCISSOR_TEST = 0x0C11;
	
    public static readonly POLYGON_OFFSET_FILL = 0x8037;
	public readonly POLYGON_OFFSET_FILL = 0x8037;
	
    public static readonly SAMPLE_ALPHA_TO_COVERAGE = 0x809E;
	public readonly SAMPLE_ALPHA_TO_COVERAGE = 0x809E;
	
    public static readonly SAMPLE_COVERAGE = 0x80A0;
	public readonly SAMPLE_COVERAGE = 0x80A0;
	
    public static readonly INVALID_ENUM = 0x0500;
	public readonly INVALID_ENUM = 0x0500;
	
    public static readonly INVALID_VALUE = 0x0501;
	public readonly INVALID_VALUE = 0x0501;
	
    public static readonly INVALID_OPERATION = 0x0502;
	public readonly INVALID_OPERATION = 0x0502;
	
    public static readonly OUT_OF_MEMORY = 0x0505;
	public readonly OUT_OF_MEMORY = 0x0505;
	
    public static readonly CW = 0x0900;
	public readonly CW = 0x0900;
	
    public static readonly CCW = 0x0901;
	public readonly CCW = 0x0901;
	
    public static readonly LINE_WIDTH = 0x0B21;
	public readonly LINE_WIDTH = 0x0B21;
	
    public static readonly ALIASED_POINT_SIZE_RANGE = 0x846D;
	public readonly ALIASED_POINT_SIZE_RANGE = 0x846D;
	
    public static readonly ALIASED_LINE_WIDTH_RANGE = 0x846E;
	public readonly ALIASED_LINE_WIDTH_RANGE = 0x846E;
	
    public static readonly CULL_FACE_MODE = 0x0B45;
	public readonly CULL_FACE_MODE = 0x0B45;
	
    public static readonly FRONT_FACE = 0x0B46;
	public readonly FRONT_FACE = 0x0B46;
	
    public static readonly DEPTH_RANGE = 0x0B70;
	public readonly DEPTH_RANGE = 0x0B70;
	
    public static readonly DEPTH_WRITEMASK = 0x0B72;
	public readonly DEPTH_WRITEMASK = 0x0B72;
	
    public static readonly DEPTH_CLEAR_VALUE = 0x0B73;
	public readonly DEPTH_CLEAR_VALUE = 0x0B73;
	
    public static readonly DEPTH_FUNC = 0x0B74;
	public readonly DEPTH_FUNC = 0x0B74;
	
    public static readonly STENCIL_CLEAR_VALUE = 0x0B91;
	public readonly STENCIL_CLEAR_VALUE = 0x0B91;
	
    public static readonly STENCIL_FUNC = 0x0B92;
	public readonly STENCIL_FUNC = 0x0B92;
	
    public static readonly STENCIL_FAIL = 0x0B94;
	public readonly STENCIL_FAIL = 0x0B94;
	
    public static readonly STENCIL_PASS_DEPTH_FAIL = 0x0B95;
	public readonly STENCIL_PASS_DEPTH_FAIL = 0x0B95;
	
    public static readonly STENCIL_PASS_DEPTH_PASS = 0x0B96;
	public readonly STENCIL_PASS_DEPTH_PASS = 0x0B96;
	
    public static readonly STENCIL_REF = 0x0B97;
	public readonly STENCIL_REF = 0x0B97;
	
    public static readonly STENCIL_VALUE_MASK = 0x0B93;
	public readonly STENCIL_VALUE_MASK = 0x0B93;
	
    public static readonly STENCIL_WRITEMASK = 0x0B98;
	public readonly STENCIL_WRITEMASK = 0x0B98;
	
    public static readonly STENCIL_BACK_FUNC = 0x8800;
	public readonly STENCIL_BACK_FUNC = 0x8800;
	
    public static readonly STENCIL_BACK_FAIL = 0x8801;
	public readonly STENCIL_BACK_FAIL = 0x8801;
	
    public static readonly STENCIL_BACK_PASS_DEPTH_FAIL = 0x8802;
	public readonly STENCIL_BACK_PASS_DEPTH_FAIL = 0x8802;
	
    public static readonly STENCIL_BACK_PASS_DEPTH_PASS = 0x8803;
	public readonly STENCIL_BACK_PASS_DEPTH_PASS = 0x8803;
	
    public static readonly STENCIL_BACK_REF = 0x8CA3;
	public readonly STENCIL_BACK_REF = 0x8CA3;
	
    public static readonly STENCIL_BACK_VALUE_MASK = 0x8CA4;
	public readonly STENCIL_BACK_VALUE_MASK = 0x8CA4;
	
    public static readonly STENCIL_BACK_WRITEMASK = 0x8CA5;
	public readonly STENCIL_BACK_WRITEMASK = 0x8CA5;
	
    public static readonly VIEWPORT = 0x0BA2;
	public readonly VIEWPORT = 0x0BA2;
	
    public static readonly SCISSOR_BOX = 0x0C10;
	public readonly SCISSOR_BOX = 0x0C10;
	
    public static readonly COLOR_CLEAR_VALUE = 0x0C22;
	public readonly COLOR_CLEAR_VALUE = 0x0C22;
	
    public static readonly COLOR_WRITEMASK = 0x0C23;
	public readonly COLOR_WRITEMASK = 0x0C23;
	
    public static readonly UNPACK_ALIGNMENT = 0x0CF5;
	public readonly UNPACK_ALIGNMENT = 0x0CF5;
	
    public static readonly PACK_ALIGNMENT = 0x0D05;
	public readonly PACK_ALIGNMENT = 0x0D05;
	
    public static readonly MAX_TEXTURE_SIZE = 0x0D33;
	public readonly MAX_TEXTURE_SIZE = 0x0D33;
	
    public static readonly MAX_VIEWPORT_DIMS = 0x0D3A;
	public readonly MAX_VIEWPORT_DIMS = 0x0D3A;
	
    public static readonly SUBPIXEL_BITS = 0x0D50;
	public readonly SUBPIXEL_BITS = 0x0D50;
	
    public static readonly RED_BITS = 0x0D52;
	public readonly RED_BITS = 0x0D52;
	
    public static readonly GREEN_BITS = 0x0D53;
	public readonly GREEN_BITS = 0x0D53;
	
    public static readonly BLUE_BITS = 0x0D54;
	public readonly BLUE_BITS = 0x0D54;
	
    public static readonly ALPHA_BITS = 0x0D55;
	public readonly ALPHA_BITS = 0x0D55;
	
    public static readonly DEPTH_BITS = 0x0D56;
	public readonly DEPTH_BITS = 0x0D56;
	
    public static readonly STENCIL_BITS = 0x0D57;
	public readonly STENCIL_BITS = 0x0D57;
	
    public static readonly POLYGON_OFFSET_UNITS = 0x2A00;
	public readonly POLYGON_OFFSET_UNITS = 0x2A00;
	
    public static readonly POLYGON_OFFSET_FACTOR = 0x8038;
	public readonly POLYGON_OFFSET_FACTOR = 0x8038;
	
    public static readonly TEXTURE_BINDING_2D = 0x8069;
	public readonly TEXTURE_BINDING_2D = 0x8069;
	
    public static readonly SAMPLE_BUFFERS = 0x80A8;
	public readonly SAMPLE_BUFFERS = 0x80A8;
	
    public static readonly SAMPLES = 0x80A9;
	public readonly SAMPLES = 0x80A9;
	
    public static readonly SAMPLE_COVERAGE_VALUE = 0x80AA;
	public readonly SAMPLE_COVERAGE_VALUE = 0x80AA;
	
    public static readonly SAMPLE_COVERAGE_INVERT = 0x80AB;
	public readonly SAMPLE_COVERAGE_INVERT = 0x80AB;
	
    public static readonly COMPRESSED_TEXTURE_FORMATS = 0x86A3;
	public readonly COMPRESSED_TEXTURE_FORMATS = 0x86A3;
	
    public static readonly DONT_CARE = 0x1100;
	public readonly DONT_CARE = 0x1100;
	
    public static readonly FASTEST = 0x1101;
	public readonly FASTEST = 0x1101;
	
    public static readonly NICEST = 0x1102;
	public readonly NICEST = 0x1102;
	
    public static readonly GENERATE_MIPMAP_HINT = 0x8192;
	public readonly GENERATE_MIPMAP_HINT = 0x8192;
	
    public static readonly BYTE = 0x1400;
	public readonly BYTE = 0x1400;
	
    public static readonly UNSIGNED_BYTE = 0x1401;
	public readonly UNSIGNED_BYTE = 0x1401;
	
    public static readonly SHORT = 0x1402;
	public readonly SHORT = 0x1402;
	
    public static readonly UNSIGNED_SHORT = 0x1403;
	public readonly UNSIGNED_SHORT = 0x1403;
	
    public static readonly INT = 0x1404;
	public readonly INT = 0x1404;
	
    public static readonly UNSIGNED_INT = 0x1405;
	public readonly UNSIGNED_INT = 0x1405;
	
    public static readonly FLOAT = 0x1406;
	public readonly FLOAT = 0x1406;
	
    public static readonly DEPTH_COMPONENT = 0x1902;
	public readonly DEPTH_COMPONENT = 0x1902;
	
    public static readonly ALPHA = 0x1906;
	public readonly ALPHA = 0x1906;
	
    public static readonly RGB = 0x1907;
	public readonly RGB = 0x1907;
	
    public static readonly RGBA = 0x1908;
	public readonly RGBA = 0x1908;
	
    public static readonly LUMINANCE = 0x1909;
	public readonly LUMINANCE = 0x1909;
	
    public static readonly LUMINANCE_ALPHA = 0x190A;
	public readonly LUMINANCE_ALPHA = 0x190A;
	
    public static readonly UNSIGNED_SHORT_4_4_4_4 = 0x8033;
	public readonly UNSIGNED_SHORT_4_4_4_4 = 0x8033;
	
    public static readonly UNSIGNED_SHORT_5_5_5_1 = 0x8034;
	public readonly UNSIGNED_SHORT_5_5_5_1 = 0x8034;
	
    public static readonly UNSIGNED_SHORT_5_6_5 = 0x8363;
	public readonly UNSIGNED_SHORT_5_6_5 = 0x8363;
	
    public static readonly FRAGMENT_SHADER = 0x8B30;
	public readonly FRAGMENT_SHADER = 0x8B30;
	
    public static readonly VERTEX_SHADER = 0x8B31;
	public readonly VERTEX_SHADER = 0x8B31;
	
    public static readonly MAX_VERTEX_ATTRIBS = 0x8869;
	public readonly MAX_VERTEX_ATTRIBS = 0x8869;
	
    public static readonly MAX_VERTEX_UNIFORM_VECTORS = 0x8DFB;
	public readonly MAX_VERTEX_UNIFORM_VECTORS = 0x8DFB;
	
    public static readonly MAX_VARYING_VECTORS = 0x8DFC;
	public readonly MAX_VARYING_VECTORS = 0x8DFC;
	
    public static readonly MAX_COMBINED_TEXTURE_IMAGE_UNITS = 0x8B4D;
	public readonly MAX_COMBINED_TEXTURE_IMAGE_UNITS = 0x8B4D;
	
    public static readonly MAX_VERTEX_TEXTURE_IMAGE_UNITS = 0x8B4C;
	public readonly MAX_VERTEX_TEXTURE_IMAGE_UNITS = 0x8B4C;
	
    public static readonly MAX_TEXTURE_IMAGE_UNITS = 0x8872;
	public readonly MAX_TEXTURE_IMAGE_UNITS = 0x8872;
	
    public static readonly MAX_FRAGMENT_UNIFORM_VECTORS = 0x8DFD;
	public readonly MAX_FRAGMENT_UNIFORM_VECTORS = 0x8DFD;
	
    public static readonly MAX_TEXTURE_MAX_ANISOTROPY_EXT = 34047;
	public readonly MAX_TEXTURE_MAX_ANISOTROPY_EXT = 34047;
	
    public static readonly TEXTURE_MAX_ANISOTROPY_EXT = 34046;
	public readonly TEXTURE_MAX_ANISOTROPY_EXT = 34046;
	
    public static readonly SHADER_TYPE = 0x8B4F;
	public readonly SHADER_TYPE = 0x8B4F;
	
    public static readonly DELETE_STATUS = 0x8B80;
	public readonly DELETE_STATUS = 0x8B80;
	
    public static readonly LINK_STATUS = 0x8B82;
	public readonly LINK_STATUS = 0x8B82;
	
    public static readonly VALIDATE_STATUS = 0x8B83;
	public readonly VALIDATE_STATUS = 0x8B83;
	
    public static readonly ATTACHED_SHADERS = 0x8B85;
	public readonly ATTACHED_SHADERS = 0x8B85;
	
    public static readonly ACTIVE_UNIFORMS = 0x8B86;
	public readonly ACTIVE_UNIFORMS = 0x8B86;
	
    public static readonly ACTIVE_ATTRIBUTES = 0x8B89;
	public readonly ACTIVE_ATTRIBUTES = 0x8B89;
	
    public static readonly SHADING_LANGUAGE_VERSION = 0x8B8C;
	public readonly SHADING_LANGUAGE_VERSION = 0x8B8C;
	
    public static readonly CURRENT_PROGRAM = 0x8B8D;
	public readonly CURRENT_PROGRAM = 0x8B8D;
	
    public static readonly NEVER = 0x0200;
	public readonly NEVER = 0x0200;
	
    public static readonly LESS = 0x0201;
	public readonly LESS = 0x0201;
	
    public static readonly EQUAL = 0x0202;
	public readonly EQUAL = 0x0202;
	
    public static readonly LEQUAL = 0x0203;
	public readonly LEQUAL = 0x0203;
	
    public static readonly GREATER = 0x0204;
	public readonly GREATER = 0x0204;
	
    public static readonly NOTEQUAL = 0x0205;
	public readonly NOTEQUAL = 0x0205;
	
    public static readonly GEQUAL = 0x0206;
	public readonly GEQUAL = 0x0206;
	
    public static readonly ALWAYS = 0x0207;
	public readonly ALWAYS = 0x0207;
	
    public static readonly KEEP = 0x1E00;
	public readonly KEEP = 0x1E00;
	
    public static readonly REPLACE = 0x1E01;
	public readonly REPLACE = 0x1E01;
	
    public static readonly INCR = 0x1E02;
	public readonly INCR = 0x1E02;
	
    public static readonly DECR = 0x1E03;
	public readonly DECR = 0x1E03;
	
    public static readonly INVERT = 0x150A;
	public readonly INVERT = 0x150A;
	
    public static readonly INCR_WRAP = 0x8507;
	public readonly INCR_WRAP = 0x8507;
	
    public static readonly DECR_WRAP = 0x8508;
	public readonly DECR_WRAP = 0x8508;
	
    public static readonly VENDOR = 0x1F00;
	public readonly VENDOR = 0x1F00;
	
    public static readonly RENDERER = 0x1F01;
	public readonly RENDERER = 0x1F01;
	
    public static readonly VERSION = 0x1F02;
	public readonly VERSION = 0x1F02;
	
    public static readonly NEAREST = 0x2600;
	public readonly NEAREST = 0x2600;
	
    public static readonly LINEAR = 0x2601;
	public readonly LINEAR = 0x2601;
	
    public static readonly NEAREST_MIPMAP_NEAREST = 0x2700;
	public readonly NEAREST_MIPMAP_NEAREST = 0x2700;
	
    public static readonly LINEAR_MIPMAP_NEAREST = 0x2701;
	public readonly LINEAR_MIPMAP_NEAREST = 0x2701;
	
    public static readonly NEAREST_MIPMAP_LINEAR = 0x2702;
	public readonly NEAREST_MIPMAP_LINEAR = 0x2702;
	
    public static readonly LINEAR_MIPMAP_LINEAR = 0x2703;
	public readonly LINEAR_MIPMAP_LINEAR = 0x2703;
	
    public static readonly TEXTURE_MAG_FILTER = 0x2800;
	public readonly TEXTURE_MAG_FILTER = 0x2800;
	
    public static readonly TEXTURE_MIN_FILTER = 0x2801;
	public readonly TEXTURE_MIN_FILTER = 0x2801;
	
    public static readonly TEXTURE_WRAP_S = 0x2802;
	public readonly TEXTURE_WRAP_S = 0x2802;
	
    public static readonly TEXTURE_WRAP_T = 0x2803;
	public readonly TEXTURE_WRAP_T = 0x2803;
	
    public static readonly TEXTURE_2D = 0x0DE1;
	public readonly TEXTURE_2D = 0x0DE1;
	
    public static readonly TEXTURE = 0x1702;
	public readonly TEXTURE = 0x1702;
	
    public static readonly TEXTURE_CUBE_MAP = 0x8513;
	public readonly TEXTURE_CUBE_MAP = 0x8513;
	
    public static readonly TEXTURE_BINDING_CUBE_MAP = 0x8514;
	public readonly TEXTURE_BINDING_CUBE_MAP = 0x8514;
	
    public static readonly TEXTURE_CUBE_MAP_POSITIVE_X = 0x8515;
	public readonly TEXTURE_CUBE_MAP_POSITIVE_X = 0x8515;
	
    public static readonly TEXTURE_CUBE_MAP_NEGATIVE_X = 0x8516;
	public readonly TEXTURE_CUBE_MAP_NEGATIVE_X = 0x8516;
	
    public static readonly TEXTURE_CUBE_MAP_POSITIVE_Y = 0x8517;
	public readonly TEXTURE_CUBE_MAP_POSITIVE_Y = 0x8517;
	
    public static readonly TEXTURE_CUBE_MAP_NEGATIVE_Y = 0x8518;
	public readonly TEXTURE_CUBE_MAP_NEGATIVE_Y = 0x8518;
	
    public static readonly TEXTURE_CUBE_MAP_POSITIVE_Z = 0x8519;
	public readonly TEXTURE_CUBE_MAP_POSITIVE_Z = 0x8519;
	
    public static readonly TEXTURE_CUBE_MAP_NEGATIVE_Z = 0x851A;
	public readonly TEXTURE_CUBE_MAP_NEGATIVE_Z = 0x851A;
	
    public static readonly MAX_CUBE_MAP_TEXTURE_SIZE = 0x851C;
	public readonly MAX_CUBE_MAP_TEXTURE_SIZE = 0x851C;
	
    public static readonly TEXTURE0 = 0x84C0;
	public readonly TEXTURE0 = 0x84C0;
	
    public static readonly TEXTURE1 = 0x84C1;
	public readonly TEXTURE1 = 0x84C1;
	
    public static readonly TEXTURE2 = 0x84C2;
	public readonly TEXTURE2 = 0x84C2;
	
    public static readonly TEXTURE3 = 0x84C3;
	public readonly TEXTURE3 = 0x84C3;
	
    public static readonly TEXTURE4 = 0x84C4;
	public readonly TEXTURE4 = 0x84C4;
	
    public static readonly TEXTURE5 = 0x84C5;
	public readonly TEXTURE5 = 0x84C5;
	
    public static readonly TEXTURE6 = 0x84C6;
	public readonly TEXTURE6 = 0x84C6;
	
    public static readonly TEXTURE7 = 0x84C7;
	public readonly TEXTURE7 = 0x84C7;
	
    public static readonly TEXTURE8 = 0x84C8;
	public readonly TEXTURE8 = 0x84C8;
	
    public static readonly TEXTURE9 = 0x84C9;
	public readonly TEXTURE9 = 0x84C9;
	
    public static readonly TEXTURE10 = 0x84CA;
	public readonly TEXTURE10 = 0x84CA;
	
    public static readonly TEXTURE11 = 0x84CB;
	public readonly TEXTURE11 = 0x84CB;
	
    public static readonly TEXTURE12 = 0x84CC;
	public readonly TEXTURE12 = 0x84CC;
	
    public static readonly TEXTURE13 = 0x84CD;
	public readonly TEXTURE13 = 0x84CD;
	
    public static readonly TEXTURE14 = 0x84CE;
	public readonly TEXTURE14 = 0x84CE;
	
    public static readonly TEXTURE15 = 0x84CF;
	public readonly TEXTURE15 = 0x84CF;
	
    public static readonly TEXTURE16 = 0x84D0;
	public readonly TEXTURE16 = 0x84D0;
	
    public static readonly TEXTURE17 = 0x84D1;
	public readonly TEXTURE17 = 0x84D1;
	
    public static readonly TEXTURE18 = 0x84D2;
	public readonly TEXTURE18 = 0x84D2;
	
    public static readonly TEXTURE19 = 0x84D3;
	public readonly TEXTURE19 = 0x84D3;
	
    public static readonly TEXTURE20 = 0x84D4;
	public readonly TEXTURE20 = 0x84D4;
	
    public static readonly TEXTURE21 = 0x84D5;
	public readonly TEXTURE21 = 0x84D5;
	
    public static readonly TEXTURE22 = 0x84D6;
	public readonly TEXTURE22 = 0x84D6;
	
    public static readonly TEXTURE23 = 0x84D7;
	public readonly TEXTURE23 = 0x84D7;
	
    public static readonly TEXTURE24 = 0x84D8;
	public readonly TEXTURE24 = 0x84D8;
	
    public static readonly TEXTURE25 = 0x84D9;
	public readonly TEXTURE25 = 0x84D9;
	
    public static readonly TEXTURE26 = 0x84DA;
	public readonly TEXTURE26 = 0x84DA;
	
    public static readonly TEXTURE27 = 0x84DB;
	public readonly TEXTURE27 = 0x84DB;
	
    public static readonly TEXTURE28 = 0x84DC;
	public readonly TEXTURE28 = 0x84DC;
	
    public static readonly TEXTURE29 = 0x84DD;
	public readonly TEXTURE29 = 0x84DD;
	
    public static readonly TEXTURE30 = 0x84DE;
	public readonly TEXTURE30 = 0x84DE;
	
    public static readonly TEXTURE31 = 0x84DF;
	public readonly TEXTURE31 = 0x84DF;
	
    public static readonly ACTIVE_TEXTURE = 0x84E0;
	public readonly ACTIVE_TEXTURE = 0x84E0;
	
    public static readonly REPEAT = 0x2901;
	public readonly REPEAT = 0x2901;
	
    public static readonly CLAMP_TO_EDGE = 0x812F;
	public readonly CLAMP_TO_EDGE = 0x812F;
	
    public static readonly MIRRORED_REPEAT = 0x8370;
	public readonly MIRRORED_REPEAT = 0x8370;
	
    public static readonly FLOAT_VEC2 = 0x8B50;
	public readonly FLOAT_VEC2 = 0x8B50;
	
    public static readonly FLOAT_VEC3 = 0x8B51;
	public readonly FLOAT_VEC3 = 0x8B51;
	
    public static readonly FLOAT_VEC4 = 0x8B52;
	public readonly FLOAT_VEC4 = 0x8B52;
	
    public static readonly INT_VEC2 = 0x8B53;
	public readonly INT_VEC2 = 0x8B53;
	
    public static readonly INT_VEC3 = 0x8B54;
	public readonly INT_VEC3 = 0x8B54;
	
    public static readonly INT_VEC4 = 0x8B55;
	public readonly INT_VEC4 = 0x8B55;
	
    public static readonly BOOL = 0x8B56;
	public readonly BOOL = 0x8B56;
	
    public static readonly BOOL_VEC2 = 0x8B57;
	public readonly BOOL_VEC2 = 0x8B57;
	
    public static readonly BOOL_VEC3 = 0x8B58;
	public readonly BOOL_VEC3 = 0x8B58;
	
    public static readonly BOOL_VEC4 = 0x8B59;
	public readonly BOOL_VEC4 = 0x8B59;
	
    public static readonly FLOAT_MAT2 = 0x8B5A;
	public readonly FLOAT_MAT2 = 0x8B5A;
	
    public static readonly FLOAT_MAT3 = 0x8B5B;
	public readonly FLOAT_MAT3 = 0x8B5B;
	
    public static readonly FLOAT_MAT4 = 0x8B5C;
	public readonly FLOAT_MAT4 = 0x8B5C;
	
    public static readonly SAMPLER_2D = 0x8B5E;
	public readonly SAMPLER_2D = 0x8B5E;
	
    public static readonly SAMPLER_CUBE = 0x8B60;
	public readonly SAMPLER_CUBE = 0x8B60;
	
    public static readonly VERTEX_ATTRIB_ARRAY_ENABLED = 0x8622;
	public readonly VERTEX_ATTRIB_ARRAY_ENABLED = 0x8622;
	
    public static readonly VERTEX_ATTRIB_ARRAY_SIZE = 0x8623;
	public readonly VERTEX_ATTRIB_ARRAY_SIZE = 0x8623;
	
    public static readonly VERTEX_ATTRIB_ARRAY_STRIDE = 0x8624;
	public readonly VERTEX_ATTRIB_ARRAY_STRIDE = 0x8624;
	
    public static readonly VERTEX_ATTRIB_ARRAY_TYPE = 0x8625;
	public readonly VERTEX_ATTRIB_ARRAY_TYPE = 0x8625;
	
    public static readonly VERTEX_ATTRIB_ARRAY_NORMALIZED = 0x886A;
	public readonly VERTEX_ATTRIB_ARRAY_NORMALIZED = 0x886A;
	
    public static readonly VERTEX_ATTRIB_ARRAY_POINTER = 0x8645;
	public readonly VERTEX_ATTRIB_ARRAY_POINTER = 0x8645;
	
    public static readonly VERTEX_ATTRIB_ARRAY_BUFFER_BINDING = 0x889F;
	public readonly VERTEX_ATTRIB_ARRAY_BUFFER_BINDING = 0x889F;
	
    public static readonly IMPLEMENTATION_COLOR_READ_TYPE = 0x8B9A;
	public readonly IMPLEMENTATION_COLOR_READ_TYPE = 0x8B9A;
	
    public static readonly IMPLEMENTATION_COLOR_READ_FORMAT = 0x8B9B;
	public readonly IMPLEMENTATION_COLOR_READ_FORMAT = 0x8B9B;
	
    public static readonly COMPILE_STATUS = 0x8B81;
	public readonly COMPILE_STATUS = 0x8B81;
	
    public static readonly LOW_FLOAT = 0x8DF0;
	public readonly LOW_FLOAT = 0x8DF0;
	
    public static readonly MEDIUM_FLOAT = 0x8DF1;
	public readonly MEDIUM_FLOAT = 0x8DF1;
	
    public static readonly HIGH_FLOAT = 0x8DF2;
	public readonly HIGH_FLOAT = 0x8DF2;
	
    public static readonly LOW_INT = 0x8DF3;
	public readonly LOW_INT = 0x8DF3;
	
    public static readonly MEDIUM_INT = 0x8DF4;
	public readonly MEDIUM_INT = 0x8DF4;
	
    public static readonly HIGH_INT = 0x8DF5;
	public readonly HIGH_INT = 0x8DF5;
	
    public static readonly FRAMEBUFFER = 0x8D40;
	public readonly FRAMEBUFFER = 0x8D40;
	
    public static readonly RENDERBUFFER = 0x8D41;
	public readonly RENDERBUFFER = 0x8D41;
	
    public static readonly RGBA4 = 0x8056;
	public readonly RGBA4 = 0x8056;
	
    public static readonly RGB5_A1 = 0x8057;
	public readonly RGB5_A1 = 0x8057;
	
    public static readonly RGB565 = 0x8D62;
	public readonly RGB565 = 0x8D62;
	
    public static readonly DEPTH_COMPONENT16 = 0x81A5;
	public readonly DEPTH_COMPONENT16 = 0x81A5;
	
    public static readonly STENCIL_INDEX = 0x1901;
	public readonly STENCIL_INDEX = 0x1901;
	
    public static readonly STENCIL_INDEX8 = 0x8D48;
	public readonly STENCIL_INDEX8 = 0x8D48;
	
    public static readonly DEPTH_STENCIL = 0x84F9;
	public readonly DEPTH_STENCIL = 0x84F9;
	
    public static readonly RENDERBUFFER_WIDTH = 0x8D42;
	public readonly RENDERBUFFER_WIDTH = 0x8D42;
	
    public static readonly RENDERBUFFER_HEIGHT = 0x8D43;
	public readonly RENDERBUFFER_HEIGHT = 0x8D43;
	
    public static readonly RENDERBUFFER_INTERNAL_FORMAT = 0x8D44;
	public readonly RENDERBUFFER_INTERNAL_FORMAT = 0x8D44;
	
    public static readonly RENDERBUFFER_RED_SIZE = 0x8D50;
	public readonly RENDERBUFFER_RED_SIZE = 0x8D50;
	
    public static readonly RENDERBUFFER_GREEN_SIZE = 0x8D51;
	public readonly RENDERBUFFER_GREEN_SIZE = 0x8D51;
	
    public static readonly RENDERBUFFER_BLUE_SIZE = 0x8D52;
	public readonly RENDERBUFFER_BLUE_SIZE = 0x8D52;
	
    public static readonly RENDERBUFFER_ALPHA_SIZE = 0x8D53;
	public readonly RENDERBUFFER_ALPHA_SIZE = 0x8D53;
	
    public static readonly RENDERBUFFER_DEPTH_SIZE = 0x8D54;
	public readonly RENDERBUFFER_DEPTH_SIZE = 0x8D54;
	
    public static readonly RENDERBUFFER_STENCIL_SIZE = 0x8D55;
	public readonly RENDERBUFFER_STENCIL_SIZE = 0x8D55;
	
    public static readonly FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE = 0x8CD0;
	public readonly FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE = 0x8CD0;
	
    public static readonly FRAMEBUFFER_ATTACHMENT_OBJECT_NAME = 0x8CD1;
	public readonly FRAMEBUFFER_ATTACHMENT_OBJECT_NAME = 0x8CD1;
	
    public static readonly FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL = 0x8CD2;
	public readonly FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL = 0x8CD2;
	
    public static readonly FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE = 0x8CD3;
	public readonly FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE = 0x8CD3;
	
    public static readonly COLOR_ATTACHMENT0 = 0x8CE0;
	public readonly COLOR_ATTACHMENT0 = 0x8CE0;
	
    public static readonly DEPTH_ATTACHMENT = 0x8D00;
	public readonly DEPTH_ATTACHMENT = 0x8D00;
	
    public static readonly STENCIL_ATTACHMENT = 0x8D20;
	public readonly STENCIL_ATTACHMENT = 0x8D20;
	
    public static readonly DEPTH_STENCIL_ATTACHMENT = 0x821A;
	public readonly DEPTH_STENCIL_ATTACHMENT = 0x821A;
	
    public static readonly NONE = 0;
	public readonly NONE = 0;
	
    public static readonly FRAMEBUFFER_COMPLETE = 0x8CD5;
	public readonly FRAMEBUFFER_COMPLETE = 0x8CD5;
	
    public static readonly FRAMEBUFFER_INCOMPLETE_ATTACHMENT = 0x8CD6;
	public readonly FRAMEBUFFER_INCOMPLETE_ATTACHMENT = 0x8CD6;
	
    public static readonly FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT = 0x8CD7;
	public readonly FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT = 0x8CD7;
	
    public static readonly FRAMEBUFFER_INCOMPLETE_DIMENSIONS = 0x8CD9;
	public readonly FRAMEBUFFER_INCOMPLETE_DIMENSIONS = 0x8CD9;
	
    public static readonly FRAMEBUFFER_UNSUPPORTED = 0x8CDD;
	public readonly FRAMEBUFFER_UNSUPPORTED = 0x8CDD;
	
    public static readonly FRAMEBUFFER_BINDING = 0x8CA6;
	public readonly FRAMEBUFFER_BINDING = 0x8CA6;
	
    public static readonly RENDERBUFFER_BINDING = 0x8CA7;
	public readonly RENDERBUFFER_BINDING = 0x8CA7;
	
    public static readonly MAX_RENDERBUFFER_SIZE = 0x84E8;
	public readonly MAX_RENDERBUFFER_SIZE = 0x84E8;
	
    public static readonly INVALID_FRAMEBUFFER_OPERATION = 0x0506;
	public readonly INVALID_FRAMEBUFFER_OPERATION = 0x0506;
	
    public static readonly UNPACK_FLIP_Y_WEBGL = 0x9240;
	public readonly UNPACK_FLIP_Y_WEBGL = 0x9240;
	
    public static readonly UNPACK_PREMULTIPLY_ALPHA_WEBGL = 0x9241;
	public readonly UNPACK_PREMULTIPLY_ALPHA_WEBGL = 0x9241;
	
    public static readonly CONTEXT_LOST_WEBGL = 0x9242;
	public readonly CONTEXT_LOST_WEBGL = 0x9242;
	
    public static readonly UNPACK_COLORSPACE_CONVERSION_WEBGL = 0x9243;
	public readonly UNPACK_COLORSPACE_CONVERSION_WEBGL = 0x9243;
	
    public static readonly BROWSER_DEFAULT_WEBGL = 0x9244;
    public readonly BROWSER_DEFAULT_WEBGL = 0x9244;
     //-------------------------------------------------------------------------------------
    /* WebGL2 constants */
    readonly READ_BUFFER: number = 0x0C02;
    readonly UNPACK_ROW_LENGTH: number = 0x0CF2;
    readonly UNPACK_SKIP_ROWS: number = 0x0CF3;
    readonly UNPACK_SKIP_PIXELS: number = 0x0CF4;
    readonly PACK_ROW_LENGTH: number = 0x0D02;
    readonly PACK_SKIP_ROWS: number = 0x0D03;
    readonly PACK_SKIP_PIXELS: number = 0x0D04;
    readonly COLOR: number = 0x1800;
    readonly DEPTH: number = 0x1801;
    readonly STENCIL: number = 0x1802;
    readonly RED: number = 0x1903;
    readonly RGB8: number = 0x8051;
    readonly RGBA8: number = 0x8058;
    readonly RGB10_A2: number = 0x8059;
    readonly TEXTURE_BINDING_3D: number = 0x806A;
    readonly UNPACK_SKIP_IMAGES: number = 0x806D;
    readonly UNPACK_IMAGE_HEIGHT: number = 0x806E;
    readonly TEXTURE_3D: number = 0x806F;
    readonly TEXTURE_WRAP_R: number = 0x8072;
    readonly MAX_3D_TEXTURE_SIZE: number = 0x8073;
    readonly UNSIGNED_INT_2_10_10_10_REV: number = 0x8368;
    readonly MAX_ELEMENTS_VERTICES: number = 0x80E8;
    readonly MAX_ELEMENTS_INDICES: number = 0x80E9;
    readonly TEXTURE_MIN_LOD: number = 0x813A;
    readonly TEXTURE_MAX_LOD: number = 0x813B;
    readonly TEXTURE_BASE_LEVEL: number = 0x813C;
    readonly TEXTURE_MAX_LEVEL: number = 0x813D;
    readonly MIN: number = 0x8007;
    readonly MAX: number = 0x8008;
    readonly DEPTH_COMPONENT24: number = 0x81A6;
    readonly MAX_TEXTURE_LOD_BIAS: number = 0x84FD;
    readonly TEXTURE_COMPARE_MODE: number = 0x884C;
    readonly TEXTURE_COMPARE_FUNC: number = 0x884D;
    readonly CURRENT_QUERY: number = 0x8865;
    readonly QUERY_RESULT: number = 0x8866;
    readonly QUERY_RESULT_AVAILABLE: number = 0x8867;
    readonly STREAM_READ: number = 0x88E1;
    readonly STREAM_COPY: number = 0x88E2;
    readonly STATIC_READ: number = 0x88E5;
    readonly STATIC_COPY: number = 0x88E6;
    readonly DYNAMIC_READ: number = 0x88E9;
    readonly DYNAMIC_COPY: number = 0x88EA;
    readonly MAX_DRAW_BUFFERS: number = 0x8824;
    readonly DRAW_BUFFER0: number = 0x8825;
    readonly DRAW_BUFFER1: number = 0x8826;
    readonly DRAW_BUFFER2: number = 0x8827;
    readonly DRAW_BUFFER3: number = 0x8828;
    readonly DRAW_BUFFER4: number = 0x8829;
    readonly DRAW_BUFFER5: number = 0x882A;
    readonly DRAW_BUFFER6: number = 0x882B;
    readonly DRAW_BUFFER7: number = 0x882C;
    readonly DRAW_BUFFER8: number = 0x882D;
    readonly DRAW_BUFFER9: number = 0x882E;
    readonly DRAW_BUFFER10: number = 0x882F;
    readonly DRAW_BUFFER11: number = 0x8830;
    readonly DRAW_BUFFER12: number = 0x8831;
    readonly DRAW_BUFFER13: number = 0x8832;
    readonly DRAW_BUFFER14: number = 0x8833;
    readonly DRAW_BUFFER15: number = 0x8834;
    readonly MAX_FRAGMENT_UNIFORM_COMPONENTS: number = 0x8B49;
    readonly MAX_VERTEX_UNIFORM_COMPONENTS: number = 0x8B4A;
    readonly SAMPLER_3D: number = 0x8B5F;
    readonly SAMPLER_2D_SHADOW: number = 0x8B62;
    readonly FRAGMENT_SHADER_DERIVATIVE_HINT: number = 0x8B8B;
    readonly PIXEL_PACK_BUFFER: number = 0x88EB;
    readonly PIXEL_UNPACK_BUFFER: number = 0x88EC;
    readonly PIXEL_PACK_BUFFER_BINDING: number = 0x88ED;
    readonly PIXEL_UNPACK_BUFFER_BINDING: number = 0x88EF;
    readonly FLOAT_MAT2x3: number = 0x8B65;
    readonly FLOAT_MAT2x4: number = 0x8B66;
    readonly FLOAT_MAT3x2: number = 0x8B67;
    readonly FLOAT_MAT3x4: number = 0x8B68;
    readonly FLOAT_MAT4x2: number = 0x8B69;
    readonly FLOAT_MAT4x3: number = 0x8B6A;
    readonly SRGB: number = 0x8C40;
    readonly SRGB8: number = 0x8C41;
    readonly SRGB8_ALPHA8: number = 0x8C43;
    readonly COMPARE_REF_TO_TEXTURE: number = 0x884E;
    readonly RGBA32F: number = 0x8814;
    readonly RGB32F: number = 0x8815;
    public readonly RGBA16F: number = 0x881A;
    public static readonly RGBA16F: number = 0x881A;
    readonly RGB16F: number = 0x881B;
    readonly VERTEX_ATTRIB_ARRAY_INTEGER: number = 0x88FD;
    readonly MAX_ARRAY_TEXTURE_LAYERS: number = 0x88FF;
    readonly MIN_PROGRAM_TEXEL_OFFSET: number = 0x8904;
    readonly MAX_PROGRAM_TEXEL_OFFSET: number = 0x8905;
    readonly MAX_VARYING_COMPONENTS: number = 0x8B4B;
    readonly TEXTURE_2D_ARRAY: number = 0x8C1A;
    readonly TEXTURE_BINDING_2D_ARRAY: number = 0x8C1D;
    readonly R11F_G11F_B10F: number = 0x8C3A;
    readonly UNSIGNED_INT_10F_11F_11F_REV: number = 0x8C3B;
    readonly RGB9_E5: number = 0x8C3D;
    readonly UNSIGNED_INT_5_9_9_9_REV: number = 0x8C3E;
    readonly TRANSFORM_FEEDBACK_BUFFER_MODE: number = 0x8C7F;
    readonly MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS: number = 0x8C80;
    readonly TRANSFORM_FEEDBACK_VARYINGS: number = 0x8C83;
    readonly TRANSFORM_FEEDBACK_BUFFER_START: number = 0x8C84;
    readonly TRANSFORM_FEEDBACK_BUFFER_SIZE: number = 0x8C85;
    readonly TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN: number = 0x8C88;
    readonly RASTERIZER_DISCARD: number = 0x8C89;
    readonly MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS: number = 0x8C8A;
    readonly MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS: number = 0x8C8B;
    readonly INTERLEAVED_ATTRIBS: number = 0x8C8C;
    readonly SEPARATE_ATTRIBS: number = 0x8C8D;
    readonly TRANSFORM_FEEDBACK_BUFFER: number = 0x8C8E;
    readonly TRANSFORM_FEEDBACK_BUFFER_BINDING: number = 0x8C8F;
    readonly RGBA32UI: number = 0x8D70;
    readonly RGB32UI: number = 0x8D71;
    readonly RGBA16UI: number = 0x8D76;
    readonly RGB16UI: number = 0x8D77;
    readonly RGBA8UI: number = 0x8D7C;
    readonly RGB8UI: number = 0x8D7D;
    readonly RGBA32I: number = 0x8D82;
    readonly RGB32I: number = 0x8D83;
    readonly RGBA16I: number = 0x8D88;
    readonly RGB16I: number = 0x8D89;
    readonly RGBA8I: number = 0x8D8E;
    readonly RGB8I: number = 0x8D8F;
    readonly RED_INTEGER: number = 0x8D94;
    readonly RGB_INTEGER: number = 0x8D98;
    readonly RGBA_INTEGER: number = 0x8D99;
    readonly SAMPLER_2D_ARRAY: number = 0x8DC1;
    readonly SAMPLER_2D_ARRAY_SHADOW: number = 0x8DC4;
    readonly SAMPLER_CUBE_SHADOW: number = 0x8DC5;
    readonly UNSIGNED_INT_VEC2: number = 0x8DC6;
    readonly UNSIGNED_INT_VEC3: number = 0x8DC7;
    readonly UNSIGNED_INT_VEC4: number = 0x8DC8;
    readonly INT_SAMPLER_2D: number = 0x8DCA;
    readonly INT_SAMPLER_3D: number = 0x8DCB;
    readonly INT_SAMPLER_CUBE: number = 0x8DCC;
    readonly INT_SAMPLER_2D_ARRAY: number = 0x8DCF;
    readonly UNSIGNED_INT_SAMPLER_2D: number = 0x8DD2;
    readonly UNSIGNED_INT_SAMPLER_3D: number = 0x8DD3;
    readonly UNSIGNED_INT_SAMPLER_CUBE: number = 0x8DD4;
    readonly UNSIGNED_INT_SAMPLER_2D_ARRAY: number = 0x8DD7;
    readonly DEPTH_COMPONENT32F: number = 0x8CAC;
    readonly DEPTH32F_STENCIL8: number = 0x8CAD;
    readonly FLOAT_32_UNSIGNED_INT_24_8_REV: number = 0x8DAD;
    readonly FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING: number = 0x8210;
    readonly FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE: number = 0x8211;
    readonly FRAMEBUFFER_ATTACHMENT_RED_SIZE: number = 0x8212;
    readonly FRAMEBUFFER_ATTACHMENT_GREEN_SIZE: number = 0x8213;
    readonly FRAMEBUFFER_ATTACHMENT_BLUE_SIZE: number = 0x8214;
    readonly FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE: number = 0x8215;
    readonly FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE: number = 0x8216;
    readonly FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE: number = 0x8217;
    readonly FRAMEBUFFER_DEFAULT: number = 0x8218;
    //readonly DEPTH_STENCIL_ATTACHMENT: number;                    // 0x821A /* Already defined in WebGL1 constants */;
    //readonly DEPTH_STENCIL: number;                               // 0x84F9 /* Already defined in WebGL1 constants */;
    readonly UNSIGNED_INT_24_8: number = 0x84FA;
    readonly DEPTH24_STENCIL8: number = 0x88F0;
    readonly UNSIGNED_NORMALIZED: number = 0x8C17;
    readonly DRAW_FRAMEBUFFER_BINDING: number = 0x8CA6 /* Same as FRAMEBUFFER_BINDING */;
    readonly READ_FRAMEBUFFER: number = 0x8CA8;
    readonly DRAW_FRAMEBUFFER: number = 0x8CA9;
    readonly READ_FRAMEBUFFER_BINDING: number = 0x8CAA;
    readonly RENDERBUFFER_SAMPLES: number = 0x8CAB;
    readonly FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER: number = 0x8CD4;
    readonly MAX_COLOR_ATTACHMENTS: number = 0x8CDF;
    readonly COLOR_ATTACHMENT1: number = 0x8CE1;
    readonly COLOR_ATTACHMENT2: number = 0x8CE2;
    readonly COLOR_ATTACHMENT3: number = 0x8CE3;
    readonly COLOR_ATTACHMENT4: number = 0x8CE4;
    readonly COLOR_ATTACHMENT5: number = 0x8CE5;
    readonly COLOR_ATTACHMENT6: number = 0x8CE6;
    readonly COLOR_ATTACHMENT7: number = 0x8CE7;
    readonly COLOR_ATTACHMENT8: number = 0x8CE8;
    readonly COLOR_ATTACHMENT9: number = 0x8CE9;
    readonly COLOR_ATTACHMENT10: number = 0x8CEA;
    readonly COLOR_ATTACHMENT11: number = 0x8CEB;
    readonly COLOR_ATTACHMENT12: number = 0x8CEC;
    readonly COLOR_ATTACHMENT13: number = 0x8CED;
    readonly COLOR_ATTACHMENT14: number = 0x8CEE;
    readonly COLOR_ATTACHMENT15: number = 0x8CEF;
    readonly FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: number = 0x8D56;
    readonly MAX_SAMPLES: number = 0x8D57;
    public readonly HALF_FLOAT: number = 0x140B;
    public static readonly HALF_FLOAT: number = 0x140B;
    readonly RG: number = 0x8227;
    readonly RG_INTEGER: number = 0x8228;
    readonly R8: number = 0x8229;
    readonly RG8: number = 0x822B;
    readonly R16F: number = 0x822D;
    readonly R32F: number = 0x822E;
    readonly RG16F: number = 0x822F;
    readonly RG32F: number = 0x8230;
    readonly R8I: number = 0x8231;
    readonly R8UI: number = 0x8232;
    readonly R16I: number = 0x8233;
    readonly R16UI: number = 0x8234;
    readonly R32I: number = 0x8235;
    readonly R32UI: number = 0x8236;
    readonly RG8I: number = 0x8237;
    readonly RG8UI: number = 0x8238;
    readonly RG16I: number = 0x8239;
    readonly RG16UI: number = 0x823A;
    readonly RG32I: number = 0x823B;
    readonly RG32UI: number = 0x823C;
    readonly VERTEX_ARRAY_BINDING: number = 0x85B5;
    readonly R8_SNORM: number = 0x8F94;
    readonly RG8_SNORM: number = 0x8F95;
    readonly RGB8_SNORM: number = 0x8F96;
    readonly RGBA8_SNORM: number = 0x8F97;
    readonly SIGNED_NORMALIZED: number = 0x8F9C;
    readonly COPY_READ_BUFFER: number = 0x8F36;
    readonly COPY_WRITE_BUFFER: number = 0x8F37;
    readonly COPY_READ_BUFFER_BINDING: number = 0x8F36 /* Same as COPY_READ_BUFFER */
    readonly COPY_WRITE_BUFFER_BINDING: number = 0x8F37 /* Same as COPY_WRITE_BUFFER */
    readonly UNIFORM_BUFFER: number = 0x8A11;
    readonly UNIFORM_BUFFER_BINDING: number = 0x8A28;
    readonly UNIFORM_BUFFER_START: number = 0x8A29;
    readonly UNIFORM_BUFFER_SIZE: number = 0x8A2A;
    readonly MAX_VERTEX_UNIFORM_BLOCKS: number = 0x8A2B;
    readonly MAX_FRAGMENT_UNIFORM_BLOCKS: number = 0x8A2D;
    readonly MAX_COMBINED_UNIFORM_BLOCKS: number = 0x8A2E;
    readonly MAX_UNIFORM_BUFFER_BINDINGS: number = 0x8A2F;
    readonly MAX_UNIFORM_BLOCK_SIZE: number = 0x8A30;
    readonly MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS: number = 0x8A31;
    readonly MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS: number = 0x8A33;
    readonly UNIFORM_BUFFER_OFFSET_ALIGNMENT: number = 0x8A34;
    readonly ACTIVE_UNIFORM_BLOCKS: number = 0x8A36;
    readonly UNIFORM_TYPE: number = 0x8A37;
    readonly UNIFORM_SIZE: number = 0x8A38;
    readonly UNIFORM_BLOCK_INDEX: number = 0x8A3A;
    readonly UNIFORM_OFFSET: number = 0x8A3B;
    readonly UNIFORM_ARRAY_STRIDE: number = 0x8A3C;
    readonly UNIFORM_MATRIX_STRIDE: number = 0x8A3D;
    readonly UNIFORM_IS_ROW_MAJOR: number = 0x8A3E;
    readonly UNIFORM_BLOCK_BINDING: number = 0x8A3F;
    readonly UNIFORM_BLOCK_DATA_SIZE: number = 0x8A40;
    readonly UNIFORM_BLOCK_ACTIVE_UNIFORMS: number = 0x8A42;
    readonly UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES: number = 0x8A43;
    readonly UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER: number = 0x8A44;
    readonly UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER: number = 0x8A46;
    readonly INVALID_INDEX: number = 0xFFFFFFFF;
    readonly MAX_VERTEX_OUTPUT_COMPONENTS: number = 0x9122;
    readonly MAX_FRAGMENT_INPUT_COMPONENTS: number = 0x9125;
    readonly MAX_SERVER_WAIT_TIMEOUT: number = 0x9111;
    readonly OBJECT_TYPE: number = 0x9112;
    readonly SYNC_CONDITION: number = 0x9113;
    readonly SYNC_STATUS: number = 0x9114;
    readonly SYNC_FLAGS: number = 0x9115;
    readonly SYNC_FENCE: number = 0x9116;
    readonly SYNC_GPU_COMMANDS_COMPLETE: number = 0x9117;
    readonly UNSIGNALED: number = 0x9118;
    readonly SIGNALED: number = 0x9119;
    readonly ALREADY_SIGNALED: number = 0x911A;
    readonly TIMEOUT_EXPIRED: number = 0x911B;
    readonly CONDITION_SATISFIED: number = 0x911C;
    readonly WAIT_FAILED: number = 0x911D;
    readonly SYNC_FLUSH_COMMANDS_BIT: number = 0x00000001;
    readonly VERTEX_ATTRIB_ARRAY_DIVISOR: number = 0x88FE;
    readonly ANY_SAMPLES_PASSED: number = 0x8C2F;
    readonly ANY_SAMPLES_PASSED_CONSERVATIVE: number = 0x8D6A;
    readonly SAMPLER_BINDING: number = 0x8919;
    readonly RGB10_A2UI: number = 0x906F;
    readonly INT_2_10_10_10_REV: number = 0x8D9F;
    readonly TRANSFORM_FEEDBACK: number = 0x8E22;
    readonly TRANSFORM_FEEDBACK_PAUSED: number = 0x8E23;
    readonly TRANSFORM_FEEDBACK_ACTIVE: number = 0x8E24;
    readonly TRANSFORM_FEEDBACK_BINDING: number = 0x8E25;
    readonly TEXTURE_IMMUTABLE_FORMAT: number = 0x912F;
    readonly MAX_ELEMENT_INDEX: number = 0x8D6B;
    readonly TEXTURE_IMMUTABLE_LEVELS: number = 0x82DF;

    readonly TIMEOUT_IGNORED: number = -1;

    /* WebGL-specific enums */
    readonly MAX_CLIENT_WAIT_TIMEOUT_WEBGL: number = 0x9247;

    //-------------------------------------------------------------------------------------
    public static __init__():void
    {
        LayaGLContext._syncBufferList = new Int32Array(LayaGLContext._syncBufferSize);
        LayaGLContext._syncBufferList["conchRef"] = webglPlus.createArrayBufferRef(LayaGLContext._syncBufferList, LayaGLContext.ARRAY_BUFFER_TYPE_DATA, false, LayaGLContext.ARRAY_BUFFER_REF_REFERENCE);
        LayaGLContext._syncBufferList["_ptrID"] = LayaGLContext._syncBufferList["conchRef"].id;

        //第0个值为frameCount  第1个值为需要同步data的count个数
        LayaGLContext._frameAndSyncCountBuffer = new Int32Array(2);
        LayaGLContext._frameAndSyncCountBuffer["conchRef"] = webglPlus.createArrayBufferRef(LayaGLContext._frameAndSyncCountBuffer, LayaGLContext.ARRAY_BUFFER_TYPE_DATA, false, LayaGLContext.ARRAY_BUFFER_REF_REFERENCE);
        LayaGLContext._frameAndSyncCountBuffer["_ptrID"] = LayaGLContext._frameAndSyncCountBuffer["conchRef"].id;
        LayaGLContext._frameAndSyncCountBuffer[0] = 1;//frameCont
        LayaGLContext._frameAndSyncCountBuffer[1] = 0;//同步data的个数

        if(layagl.getThreadMode() > 1)
        {
            LayaGLContext.prototype.bindBuffer = LayaGLContext.prototype.bindBufferMutiThread;
            LayaGLContext.prototype.useProgram = LayaGLContext.prototype.useProgramMutiThread;
            GLCommandEncoder.prototype.getUniform = GLCommandEncoder.prototype.getUniformMutiThread;
        }
    }
    
    public constructor(contextType:string)
    {
        if (!LayaGLContext.instance)
        {
            LayaGLContext.__init__();
            LayaGLContext.instance = this;
        }
        else
        {
            console.log("LayaGL is already exist");
            return LayaGLContext.instance;
        }
        this._nativeObj = layagl;
        this._threadMode = this._nativeObj.getThreadMode();
        
        contextType = contextType.toLowerCase();
        if(contextType.indexOf("layagl") >=0 )
        {
            // GLCommandEncoder.prototype.uniform1fv=GLCommandEncoder.prototype.uniform1fv_laya;
            // GLCommandEncoder.prototype.uniform1iv=GLCommandEncoder.prototype.uniform1iv_laya;
            // GLCommandEncoder.prototype.uniform2fv=GLCommandEncoder.prototype.uniform2fv_laya;
            // GLCommandEncoder.prototype.uniform2iv=GLCommandEncoder.prototype.uniform2iv_laya;
            // GLCommandEncoder.prototype.uniform3fv=GLCommandEncoder.prototype.uniform3fv_laya;
            // GLCommandEncoder.prototype.uniform3iv=GLCommandEncoder.prototype.uniform3iv_laya;
            // GLCommandEncoder.prototype.uniform4fv=GLCommandEncoder.prototype.uniform4fv_laya;
            // GLCommandEncoder.prototype.uniform4iv=GLCommandEncoder.prototype.uniform4iv_laya;
            // GLCommandEncoder.prototype.uniformMatrix2fv=GLCommandEncoder.prototype.uniformMatrix2fv_laya;
            // GLCommandEncoder.prototype.uniformMatrix3fv=GLCommandEncoder.prototype.uniformMatrix3fv_laya;
            // GLCommandEncoder.prototype.uniformMatrix4fv=GLCommandEncoder.prototype.uniformMatrix4fv_laya;

            this._nativeObj.setSyncArrayBufferID(LayaGLContext._syncBufferList["_ptrID"]);
            this._nativeObj.setFrameAndSyncCountArrayBufferID(LayaGLContext._frameAndSyncCountBuffer["_ptrID"]);
            this._defaultEncoder = new GLCommandEncoder(this, 102400, 1280, false);
            this._currentCmdEncoder = this._defaultEncoder;
            this._saveCommandEncoder.push(this._currentCmdEncoder);
            this._nativeObj.setRootCommandEncoder(this._currentCmdEncoder._buffer["_ptrID"]);
        }
        else
        {
            this._nativeObj.setSyncArrayBufferID(LayaGLContext._syncBufferList["_ptrID"]);
            this._nativeObj.setFrameAndSyncCountArrayBufferID(LayaGLContext._frameAndSyncCountBuffer["_ptrID"]);
            this._defaultEncoder = new GLCommandEncoder(this, 102400, 1280, false);
            this._currentCmdEncoder = this._defaultEncoder;
            this._nativeObj.setRootCommandEncoder(this._defaultEncoder._buffer["_ptrID"]);
        }
    }
    public static getFrameCount():number
    {
        return LayaGLContext._frameAndSyncCountBuffer[0];
    }
    public getFrameCount():number
    {
        return LayaGLContext._frameAndSyncCountBuffer[0];
    }
    public syncBufferToRenderThread(value:any,index:number=0)
    {
        if( LayaGLContext.instance._threadMode == LayaGLContext.THREAD_MODE_SINGLE )return;
        var bNeedSync:boolean = false;
        if( !value._refArray )
        {
            if( value.frameCount != LayaGLContext._frameAndSyncCountBuffer[0] )
            {
                value.frameCount = LayaGLContext._frameAndSyncCountBuffer[0];
                bNeedSync = true;
            }
        }
        else
        {
            var obj:any = value._refArray[index];
            if( obj.frameCount != LayaGLContext._frameAndSyncCountBuffer[0] )
            {
                obj.frameCount = LayaGLContext._frameAndSyncCountBuffer[0];
                bNeedSync = true;
            }
        }
        if(bNeedSync)
        {
            
            if ( (LayaGLContext._frameAndSyncCountBuffer[1] + 1) > LayaGLContext._syncBufferSize)
            {
                var pre:Int32Array = LayaGLContext._syncBufferList;
                var preConchRef:any = LayaGLContext._syncBufferList["conchRef"];
                var prePtrID:number = LayaGLContext._syncBufferList["_ptrID"];
                LayaGLContext._syncBufferSize += LayaGLContext._SYNC_ARRAYBUFFER_SIZE_;
                LayaGLContext._syncBufferList = new Int32Array(LayaGLContext._syncBufferSize);
                LayaGLContext._syncBufferList["conchRef"] = preConchRef;
                LayaGLContext._syncBufferList["_ptrID"] = prePtrID;
                pre && LayaGLContext._syncBufferList.set(pre, 0);
                webglPlus.updateArrayBufferRef(LayaGLContext._syncBufferList["_ptrID"], false, LayaGLContext._syncBufferList);
            }
            var nID = value.getPtrID?value.getPtrID(index):value["_ptrID"];
            if(!nID)
            {
                alert("syncBufferToRenderThread id error");
                debugger;
            }
            LayaGLContext._syncBufferList[LayaGLContext._frameAndSyncCountBuffer[1]++] = nID;
        }
    }

    public getDefaultCommandEncoder():GLCommandEncoder
    {
        return this._defaultEncoder;
    }
    
    public commit():void
    {
        this._saveCommandEncoder.length = 0;
        this._currentCmdEncoder = this._defaultEncoder;
        this._saveCommandEncoder.push(this._currentCmdEncoder);
    }

    public setBind(pname, buffer):void
    {
        this._curBindInfo.setBind(pname, buffer);
    }

    public getBind(pname):any
    {
        return this._curBindInfo.getBind(pname);
    }

    public addGlobalValueDefine(modifyType, type, size, defaultData):number
    {
        return this._nativeObj.addGlobalValueDefine(modifyType,type,size,defaultData);
    }
    
    public endGlobalValueDefine():void
    {
        this._nativeObj.endGlobalValueDefine();
    }
    
    public defineShaderMacro(macroString:string, uniformInfo:any):number
    {
        //uniformInfo  array[{uname:"u_filter1",id:14},{uname:"u_filter2",id:15},..];
        var nID:number = LayaGLContext._shader_macro_id_;
        var sUniformInfo:string = "";
        var len:number = uniformInfo.length;
        for (var i = 0; i < len; i++) 
        {
            sUniformInfo += uniformInfo[i].uname + "," + uniformInfo[i].id.toString() + ",";
        }
        this._nativeObj.defineShaderMacro(nID, macroString, sUniformInfo);
        LayaGLContext._shader_macro_id_ <<= 1;
        return nID;
    }

    public setCanvasType(type:number)
    {
        //TODO
    }

    public setSize(w:number,h:number)
    {
        if(!isNaN(w)&&!isNaN(h))
        {
            this.width = w;
            this.height = h;
            this._nativeObj.setSize(w,h);
        }
    }
    
    public createCommandEncoder(reserveSize?:number, adjustSize?:number, isSyncToRenderThread?:boolean):GLCommandEncoder
    {
        reserveSize=reserveSize?reserveSize:128;
        adjustSize=adjustSize?adjustSize:64;
        isSyncToRenderThread=isSyncToRenderThread?isSyncToRenderThread:false;
        var cmd:GLCommandEncoder = new GLCommandEncoder(this, reserveSize, adjustSize, isSyncToRenderThread );
        if(isSyncToRenderThread)
        {
            this.syncBufferToRenderThread(cmd);
        }
        return cmd;
    }
    
    public beginCommandEncoding(commandEncoder:GLCommandEncoder):void
    {
        commandEncoder = commandEncoder ? commandEncoder : this._defaultEncoder;
        this._saveCommandEncoder.push(commandEncoder);
        this._currentCmdEncoder = commandEncoder;
    }
    
    public endCommandEncoding():void
    {
        this._saveCommandEncoder.pop();
        this._currentCmdEncoder = this._saveCommandEncoder[this._saveCommandEncoder.length - 1];
    }
    
    public getContextAttributes():any
    {
        return this._currentCmdEncoder.getContextAttributes();
    }
    get drawingBufferWidth():number
    {
        return this.canvas.width;
    }
    get drawingBufferHeight():number
    {
        return this.canvas.height;
    }
    public isContextLost():void
    {
        this._currentCmdEncoder.isContextLost();
    }
    
    public getSupportedExtensions():any
    {
        return this._currentCmdEncoder.getSupportedExtensions();
    }
    
    public getExtension(name:string):any
    {
        return this._currentCmdEncoder.getExtension(name);
    }
    
    public activeTexture(texture:number):void
    {
        this._currentCmdEncoder.activeTexture(texture);
    }
    
    public attachShader(program:WebGLProgram, shader:WebGLShader):void
    {
        this._currentCmdEncoder.attachShader(program, shader);
    }
    
    public getAttachedShaders(program:WebGLProgram): WebGLShader[]
    {
        let ret = [];
        if(program.vsShader)
        {
            ret.push(program.vsShader);
        }

        if(program.psShader)
        {
            ret.push(program.psShader);
        }

        return ret;
    }

    public bindAttribLocation(program:WebGLProgram, index:number, name:string):void
    {
        this._currentCmdEncoder.bindAttribLocation(program, index, name);
    }
    
    public bindBuffer(target:any, buffer:WebGLBuffer):void
    {
        this.setBind(target, buffer);
        this._currentCmdEncoder.bindBuffer(target, buffer);
    }

    public bindBufferMutiThread(target:any, buffer:WebGLBuffer):void
    {
        this._currentCmdEncoder.bindBuffer(target, buffer);
    }
    
    public bindFramebuffer(target:any, framebuffer:WebGLFrameBuffer):void
    {
        this._currentCmdEncoder.bindFramebuffer(target, framebuffer);
        this.setBind(target, framebuffer);
    }
    
    public bindRenderbuffer(target:any, renderbuffer:WebGLRenderBuffer):void
    {
        this._currentCmdEncoder.bindRenderbuffer(target, renderbuffer);
        this.setBind(target, renderbuffer);
    }
    
    public clearBufferfv(buffer:number, drawbuffer:number, values:any, srcOffset?:number):void
    {
        this._currentCmdEncoder.clearBufferfv(buffer, drawbuffer, values, srcOffset);
    }
    public blitFramebuffer(srcX0:number, srcY0:number, srcX1:number, srcY1:number, dstX0:number, dstY0:number, dstX1:number, dstY1:number, mask:number, filter:number):void
    {
        this._currentCmdEncoder.blitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
    }
    public bindTexture(target:any, texture:WebGLTexture):void
    {
        this._currentCmdEncoder.bindTexture(target, texture);
        this.setBind(target, texture);
    }
    
    public useTexture(texture:WebGLTexture):void
    {
        this._currentCmdEncoder.useTexture(texture);
    }
    
    public blendColor(red:any, green:any, blue:any, alpha:number):void
    {
        this._currentCmdEncoder.blendColor(red, green, blue, alpha);
    }
    
    public blendEquation(mode:any):void
    {
        this._currentCmdEncoder.blendEquation(mode);
    }
    
    public blendEquationSeparate(modeRGB:any, modeAlpha:any):void
    {
        this._currentCmdEncoder.blendEquationSeparate(modeRGB, modeAlpha);
    }
    
    public blendFunc(sfactor:any, dfactor:any):void
    {
        this._currentCmdEncoder.blendFunc(sfactor, dfactor);
    }
    
    public blendFuncSeparate(srcRGB:any, dstRGB:any, srcAlpha:any, dstAlpha:any):void
    {
        this._currentCmdEncoder.blendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
    }
    
    public bufferData(target:any, size:any, usage:any):void
    {
        this._currentCmdEncoder.bufferData(target, size, usage);
    }
    
    public bufferSubData(target:any, offset:number, data:any):void
    {
        this._currentCmdEncoder.bufferSubData(target, offset, data);
    }
    
    public checkFramebufferStatus(target:any):any
    {
        return this._currentCmdEncoder.checkFramebufferStatus(target);
    }
    
    public clear(mask:number):void
    {
        this._currentCmdEncoder.clear(mask);
    }
    
    public clearColor(red:any, green:any, blue:any, alpha:number):void
    {
        this._currentCmdEncoder.clearColor(red, green, blue, alpha);
    }
    
    public clearDepth(depth:any):void
    {
        this._currentCmdEncoder.clearDepth(depth);
    }
    
    public clearStencil(s:any):void
    {
        this._currentCmdEncoder.clearStencil(s);
    }
    
    public colorMask(red:number, green:number, blue:number, alpha:number):void
    {
        this._currentCmdEncoder.colorMask(red, green, blue, alpha);
    }
    
    public compileShader(shader:WebGLShader):void
    {
        this._currentCmdEncoder.compileShader(shader);
    }
    
    public copyTexImage2D(target:any, level:any, internalformat:any, x:number, y:number, width:number, height:number, border:any):void
    {
        this._currentCmdEncoder.copyTexImage2D(target, level, internalformat, x, y, width, height, border);
    }
    
    public copyTexSubImage2D(target:any, level:any, xoffset:number, yoffset:number, x:number, y:number, width:number, height:number):void
    {
        this._currentCmdEncoder.copyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
    }
    
    public createBuffer():WebGLBuffer
    {
        return this._currentCmdEncoder.createBuffer();
    }
    
    public createFramebuffer():WebGLFrameBuffer
    {
        return this._currentCmdEncoder.createFramebuffer();
    }
    
    public createProgram():WebGLProgram
    {
        return this._currentCmdEncoder.createProgram();
    }
    
    public createRenderbuffer():WebGLRenderBuffer
    {
        return this._currentCmdEncoder.createRenderbuffer();
    }
    
    public createShader(type:any):WebGLShader
    {
        return this._currentCmdEncoder.createShader(type);
    }
    
    public createTexture():WebGLTexture
    {
        return this._currentCmdEncoder.createTexture();
    }
    
    public cullFace(mode:any):void
    {
        this._currentCmdEncoder.cullFace(mode);
    }
    
    public deleteBuffer(buffer:WebGLBuffer):void
    {
        this._currentCmdEncoder.deleteBuffer(buffer);
        buffer.id = -1;
    }
    
    public deleteFramebuffer(framebuffer:WebGLFrameBuffer):void
    {
        this._currentCmdEncoder.deleteFramebuffer(framebuffer);
        framebuffer.id = -1;
    }
    
    public deleteProgram(program:WebGLProgram):void
    {
        this._currentCmdEncoder.deleteProgram(program);
        program.id = -1;
    }
    
    public deleteRenderbuffer(renderbuffer:WebGLRenderBuffer):void
    {
        this._currentCmdEncoder.deleteRenderbuffer(renderbuffer);
        renderbuffer.id = -1;
    }
    
    public deleteShader(shader:WebGLShader):void
    {
        this._currentCmdEncoder.deleteShader(shader);
        shader.id = -1;
        shader.src = null;
    }
    
    public deleteTexture(texture:WebGLTexture):void
    {
        this._currentCmdEncoder.deleteTexture(texture);
        texture.id = -1;
    }
    
    public depthFunc(func:any):void
    {
        this._currentCmdEncoder.depthFunc(func);
    }
    
    public depthMask(flag:any):void
    {
        this._currentCmdEncoder.depthMask(flag);
    }
    
    public depthRange(zNear:any, zFar:any):void
    {
        this._currentCmdEncoder.depthRange(zNear, zFar);
    }
    
    public detachShader(program:WebGLProgram, shader:WebGLShader):void
    {
        this._currentCmdEncoder.detachShader(program, shader);
    }
    
    public disable(cap:any):void
    {
        this._currentCmdEncoder.disable(cap);
    }
    
    public disableVertexAttribArray(index:number):void
    {
        this._currentCmdEncoder.disableVertexAttribArray(index);
    }
    
    public drawArrays(mode:any, first:number, count:number):void
    {
        this._currentCmdEncoder.drawArrays(mode, first, count);
    }
    
    public drawElements(mode:any, count:number, type:any, offset:number):void
    {
        this._currentCmdEncoder.drawElements(mode, count, type, offset);
    }
    
    public enable(cap:any):void
    {
        this._currentCmdEncoder.enable(cap);
    }
    
    public enableVertexAttribArray(index:number):void
    {
        this._currentCmdEncoder.enableVertexAttribArray(index);
    }
    
    public finish():void
    {
        this._currentCmdEncoder.finish();
    }
    
    public flush():void
    {
        this._currentCmdEncoder.flush();
    }
    
    public framebufferRenderbuffer(target:any, attachment:any, renderbuffertarget:any, renderbuffer:WebGLRenderBuffer):void
    {
        this._currentCmdEncoder.framebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
        var frameBuffer:WebGLFrameBuffer = this.getBind(target);
        if(frameBuffer)
        {
            frameBuffer.setBind(attachment, renderbuffer);
        }
    }
    
    public framebufferTexture2D(target:any, attachment:any, textarget:any, texture:WebGLTexture, level:any):void
    {
        this._currentCmdEncoder.framebufferTexture2D(target, attachment, textarget, texture, level);
        var frameBuffer:WebGLFrameBuffer = this.getBind(target);
        if(frameBuffer)
        {
            frameBuffer.setBind(attachment, texture);
        }
    }
    
    public frontFace(mode:any):any
    {
        return this._currentCmdEncoder.frontFace(mode);
    }
    
    public generateMipmap(target:any):any
    {
        return this._currentCmdEncoder.generateMipmap(target);
    }
    
    public getActiveAttrib(program:WebGLProgram, index:number):any
    {
        return this._currentCmdEncoder.getActiveAttrib(program, index);
    }
    
    public getActiveUniform(program:WebGLProgram, index:number):any
    {
        return this._currentCmdEncoder.getActiveUniform(program, index);
    }
    
    public getAttribLocation(program:WebGLProgram, name:string):any
    {
        return this._currentCmdEncoder.getAttribLocation(program, name);
    }
    
    public getParameter(pname:any):any
    {
        return this._currentCmdEncoder.getParameter(pname);
    }
    
    public getBufferParameter(target:any, pname:any):any
    {
        return this._currentCmdEncoder.getBufferParameter(target, pname);
    }
    
    public getError():any
    {
        return this._currentCmdEncoder.getError();
    }
    
    public getFramebufferAttachmentParameter(target:any, attachment:any, pname:any):any
    {
        return this._currentCmdEncoder.getFramebufferAttachmentParameter(target, attachment, pname);
    }
    
    public getProgramParameter(program:WebGLProgram, pname:any):number
    {
        return this._currentCmdEncoder.getProgramParameter(program, pname);
    }
    
    public getProgramInfoLog(program:WebGLProgram):any
    {
        return this._currentCmdEncoder.getProgramInfoLog(program);
    }
    
    public getRenderbufferParameter(target:any, pname:any):any
    {
        return this._currentCmdEncoder.getRenderbufferParameter(target, pname);
    }
    
    public getShaderPrecisionFormat(shaderType:number, precisionType:number):any
    {
        return this._currentCmdEncoder.getShaderPrecisionFormat(shaderType,precisionType);
    }
    
    public getShaderParameter(shader:WebGLShader, pname:any):any
    {
        return this._currentCmdEncoder.getShaderParameter(shader, pname);
    }
    
    public getShaderInfoLog(shader:WebGLShader):any
    {
        return this._currentCmdEncoder.getShaderInfoLog(shader);
    }
    
    public getShaderSource(shader:WebGLShader):any
    {
        return this._currentCmdEncoder.getShaderSource(shader);
    }
    
    public getTexParameter(target:any, pname:any):any
    {
        return this._currentCmdEncoder.getTexParameter(target, pname);
    }
    
    public getUniform(program:WebGLProgram, location:number):any
    {
        return this._currentCmdEncoder.getUniform(program, location);
    }
    
    public getUniformLocation(program:WebGLProgram, name:string):any
    {
        return this._currentCmdEncoder.getUniformLocation(program, name);
    }
    
    public getVertexAttrib(index:number, pname:any):any
    {
        return this._currentCmdEncoder.getVertexAttrib(index, pname);
    }
    
    public getVertexAttribOffset(index:number, pname:any):any
    {
        return this._currentCmdEncoder.getVertexAttribOffset(index, pname);
    }
    
    public hnumber(target:any, mode:any):void
    {
        this._currentCmdEncoder.hint(target, mode);
    }

    public hint(target:any, mode:any):void
    {
        this._currentCmdEncoder.hint(target, mode);
    }
    
    public isBuffer(buffer:any):void
    {
        this._currentCmdEncoder.isBuffer(buffer);
    }
    
    public isEnabled(cap:any):boolean
    {
        return this._currentCmdEncoder.isEnabled(cap);
    }
    
    public isFramebuffer(framebuffer:any):void
    {
        this._currentCmdEncoder.isFramebuffer(framebuffer);
    }
    
    public isProgram(program:any):void
    {
        this._currentCmdEncoder.isProgram(program);
    }
    
    public isRenderbuffer(renderbuffer:any):void
    {
        this._currentCmdEncoder.isRenderbuffer(renderbuffer);
    }
    
    public isShader(shader:any):void
    {
        this._currentCmdEncoder.isShader(shader);
    }
    
    public isTexture(texture:any):void
    {
        this._currentCmdEncoder.isTexture(texture);
    }
    
    public lineWidth(width:number):void
    {
        this._currentCmdEncoder.lineWidth(width);
    }
    
    public linkProgram(program:WebGLProgram):void
    {
        this._currentCmdEncoder.linkProgram(program);
    }
    
    public pixelStorei(pname:any, param:any):void
    {
        this._currentCmdEncoder.pixelStorei(pname, param);
    }
    
    public polygonOffset(factor:any, units:any):void
    {
        this._currentCmdEncoder.polygonOffset(factor, units);
    }
    
    public readPixels(x:number, y:number, width:number, height:number, format:any, type:any, pixels:any):void
    {
        this._currentCmdEncoder.readPixels(x, y, width, height, format, type, pixels);
    }
    
    public readPixelsAsync(x:number, y:number, w:number, h:number, format:any, type:any, callBack:(data:ArrayBuffer)=>void):void
    {
        this._currentCmdEncoder.readPixelsAsync(x, y, w, h, format, type, callBack);
    }
    
    public renderbufferStorage(target:any, internalformat:any, width:number, height:number):void
    {
        this._currentCmdEncoder.renderbufferStorage(target, internalformat, width, height);
    }
    
    public renderbufferStorageMultisample(target:any, samples:number, internalformat:any, width:number, height:number):void
    {
        this._currentCmdEncoder.renderbufferStorageMultisample(target, samples, internalformat, width, height);
    }

    public sampleCoverage(value:any, invert:any):void
    {
        this._currentCmdEncoder.sampleCoverage(value, invert);
    }
    
    public scissor(x:number, y:number, width:number, height:number):void
    {
        this._currentCmdEncoder.scissor(x, y, width, height);
    }
    
    public shaderSource(shader:WebGLShader, source:any):void
    {
        this._currentCmdEncoder.shaderSource(shader, source);
    }
    
    public stencilFunc(func:number, ref:number, mask:number):void
    {
        this._currentCmdEncoder.stencilFunc(func, ref, mask);
    }
    
    public stencilFuncSeparate(face:number, func:number, ref:number, mask:number):void
    {
        this._currentCmdEncoder.stencilFuncSeparate(face, func, ref, mask);
    }
    
    public stencilMask(mask:any):void
    {
        this._currentCmdEncoder.stencilMask(mask);
    }
    
    public stencilMaskSeparate(face:any, mask:any):void
    {
        this._currentCmdEncoder.stencilMaskSeparate(face, mask);
    }
    
    public stencilOp(fail:number, zfail:number, zpass:number):void
    {
        this._currentCmdEncoder.stencilOp(fail, zfail, zpass);
    }
    
    public stencilOpSeparate(face:number, fail:number, zfail:number, zpass:number):void
    {
        this._currentCmdEncoder.stencilOpSeparate(face, fail, zfail, zpass);
    }
    
    public texImage2D(_args):void
    {
        var args = arguments;
        this._currentCmdEncoder.texImage2D.apply(this._currentCmdEncoder, args);
    }
    public texStorage2D(target:any, levels:any, internalformat:any, width:number, height:number):void
    {
        this._currentCmdEncoder.texStorage2D(target, levels, internalformat, width, height);
    }
    public texParameterf(target:any, pname:any, param:any):void
    {
        this._currentCmdEncoder.texParameterf(target, pname, param);
    }
    
    public texParameteri(target:any, pname:any, param:any):void
    {
        this._currentCmdEncoder.texParameteri(target, pname, param);
    }
    
    public texSubImage2D(_args):void
    {
        var args = arguments;
        this._currentCmdEncoder.texSubImage2D.apply(this._currentCmdEncoder, args);
    }
    
    public uniform1f(location:any, x:number):void
    {
        this._currentCmdEncoder.uniform1f(location, x);
    }
    
    public uniform1fv(location:any, v:any):void
    {
        this._currentCmdEncoder.uniform1fv(location, v);
    }
    
    public uniform1i(location:any, x:number):void
    {
        this._currentCmdEncoder.uniform1i(location, x);
    }
    
    public uniform1iv(location:any, v:any):void
    {
        this._currentCmdEncoder.uniform1iv(location, v);
    }
    
    public uniform2f(location:any, x:number, y:number):void
    {
        this._currentCmdEncoder.uniform2f(location, x, y);
    }
    
    public uniform2fv(location:any, v:any):void
    {
        this._currentCmdEncoder.uniform2fv(location, v);
    }
    
    public uniform2i(location:any, x:number, y:number):void
    {
        this._currentCmdEncoder.uniform2i(location, x, y);
    }
    
    public uniform2iv(location:any, v:any):void
    {
        this._currentCmdEncoder.uniform2iv(location, v);
    }
    
    public uniform3f(location:any, x:number, y:number, z:number):void
    {
        this._currentCmdEncoder.uniform3f(location, x, y, z);
    }
    
    public uniform3fv(location:any, v:any):void
    {
        this._currentCmdEncoder.uniform3fv(location, v);
    }
    
    public uniform3i(location:any, x:number, y:number, z:number):void
    {
        this._currentCmdEncoder.uniform3i(location, x, y, z);
    }
    
    public uniform3iv(location:any, v:any):void
    {
        this._currentCmdEncoder.uniform3iv(location, v);
    }
    
    public uniform4f(location:any, x:number, y:number, z:number, w:number):void
    {
        this._currentCmdEncoder.uniform4f(location, x, y, z, w);
    }
    
    public uniform4fv(location:any, v:any):void
    {
        this._currentCmdEncoder.uniform4fv(location, v);
    }
    
    public uniform4i(location:any, x:number, y:number, z:number, w:number):void
    {
        this._currentCmdEncoder.uniform4i(location, x, y, z, w);
    }
    
    public uniform4iv(location:any, v:any):void
    {
        this._currentCmdEncoder.uniform4iv(location, v);
    }
    
    public uniformMatrix2fv(location:any, transpose:any, value:any):void
    {
        this._currentCmdEncoder.uniformMatrix2fv(location, transpose, value);
    }
    
    public uniformMatrix3fv(location:any, transpose:any, value:any):void
    {
        this._currentCmdEncoder.uniformMatrix3fv(location, transpose, value);
    }
    
    public uniformMatrix4fv(location:any, transpose:any, value:any):void
    {
        this._currentCmdEncoder.uniformMatrix4fv(location, transpose, value);
    }
    
    public useProgram(program:WebGLProgram):void
    {
        this.setBind(LayaGLContext.CURRENT_PROGRAM, program);
        this._currentCmdEncoder.useProgram(program);
    }

    public useProgramMutiThread(program:WebGLProgram):void
    {
        this._currentCmdEncoder.useProgram(program);
    }
    
    public validateProgram(program:WebGLProgram):void
    {
        this._currentCmdEncoder.validateProgram(program);
    }
    
    public vertexAttrib1f(indx:any, x:number):void
    {
        this._currentCmdEncoder.vertexAttrib1f(indx, x);
    }
    
    public vertexAttrib1fv(indx:any, values:any):void
    {
        this._currentCmdEncoder.vertexAttrib1fv(indx, values);
    }
    
    public vertexAttrib2f(indx:any, x:number, y:number):void
    {
        this._currentCmdEncoder.vertexAttrib2f(indx, x, y);
    }
    
    public vertexAttrib2fv(indx:any, values:any):void
    {
        this._currentCmdEncoder.vertexAttrib2fv(indx, values);
    }
    
    public vertexAttrib3f(indx:any, x:number, y:number, z:number):void
    {
        this._currentCmdEncoder.vertexAttrib3f(indx, x, y, z);
    }
    
    public vertexAttrib3fv(indx:any, values:any):void
    {
        this._currentCmdEncoder.vertexAttrib3fv(indx, values);
    }
    
    public vertexAttrib4f(indx:any, x:number, y:number, z:number, w:number):void
    {
        this._currentCmdEncoder.vertexAttrib4f(indx, x, y, z, w);
    }
    
    public vertexAttrib4fv(indx:any, values:any):void
    {
        this._currentCmdEncoder.vertexAttrib4fv(indx, values);
    }
    
    public vertexAttribPointer(indx:any, size:any, type:any, normalized:any, stride:any, offset:number):void
    {
        this._currentCmdEncoder.vertexAttribPointer(indx, size, type, normalized, stride, offset);
    }
    
    public viewport(x:number, y:number, width:number, height:number):void
    {
        this._currentCmdEncoder.viewport(x, y, width, height);
    }
    
    public configureBackBuffer(width:number, height:number, antiAlias:number, enableDepthAndStencil?:Boolean, wantsBestResolution?:Boolean):void
    {
        enableDepthAndStencil=enableDepthAndStencil?enableDepthAndStencil:true;
        wantsBestResolution=wantsBestResolution?wantsBestResolution:false;
        this._currentCmdEncoder.configureBackBuffer(width, height, antiAlias, enableDepthAndStencil, wantsBestResolution);
    }
    
    public compressedTexImage2D(_args):void
    {
        var args=arguments;
        this._currentCmdEncoder.compressedTexImage2D.apply(this._currentCmdEncoder, args);
    }
    public compressedTexSubImage2D(_args):void
    {
        var args=arguments;
        this._currentCmdEncoder.compressedTexSubImage2D.apply(this._currentCmdEncoder, args);
    }
    public createVertexArray():WebGLVertextArray
    {
        return this._currentCmdEncoder.createVertexArray();
    }
    public bindVertexArray(vao:WebGLVertextArray):void
    {
        this._currentCmdEncoder.bindVertexArray(vao);
    }
    public deleteVertexArray(vao:WebGLVertextArray):void
    {
        this._currentCmdEncoder.deleteVertexArray(vao);
    }
    public isVertexArray(vao:any):boolean
    {
        return this._currentCmdEncoder.isVertexArray(vao);
    }
    public vertexAttribDivisor(index:number, divisor:number):void
    {
        this._currentCmdEncoder.vertexAttribDivisor(index, divisor);
    }
    public drawArraysInstanced(mode:number, first:number, count:number, instanceCount:number):void
    {
        this._currentCmdEncoder.drawArraysInstanced(mode, first, count, instanceCount);
    }
    public drawElementsInstanced(mode:number, count:number, type:number, offset:number, instanceCount:number):void
    {
        this._currentCmdEncoder.drawElementsInstanced(mode, count, type, offset, instanceCount);
    }
    public uniformMatrix2fvEx(location:any, transpose:any, value:any):void
    {
        this._currentCmdEncoder.uniformMatrix2fvEx(location,transpose,value);
    }
    public uniformMatrix3fvEx(location:any, transpose:any, value:any):void
    {
        this._currentCmdEncoder.uniformMatrix3fvEx(location,transpose,value);
    }    
    public uniformMatrix4fvEx(location:any, transpose:any, value:any):void
    {
        this._currentCmdEncoder.uniformMatrix4fvEx(location,transpose,value);
    }
    public addShaderUniform(one:any):void
    {
        this._currentCmdEncoder.addShaderUniform(one);
    }
    public uploadShaderUniforms(commandEncoder:GLCommandEncoder, data:any,type:number):number
    {
        this._currentCmdEncoder.uploadShaderUniforms(commandEncoder, data,type);
        return 0;
    }
    public useCommandEncoder(commandEncoder:GLCommandEncoder):void
    {
        this._currentCmdEncoder.useCommandEncoder(commandEncoder);
    }
    public loadDataToReg(regNum:number,data:any,offset:number,size:number):void{
        this._currentCmdEncoder.loadDataToReg(regNum, data, offset, size);
    }
    public loadDataToRegEx(regNum:number,dataID:number,offset:number,size:number):void{
        this._currentCmdEncoder.loadDataToRegEx(regNum, dataID, offset, size);
    }
    public ifLess0(regNum:number,statementNum:number):void{
        this._currentCmdEncoder.ifLess0(regNum, statementNum);
    }
    public ifEqual0(regNum:number,statementNum:number):void{
        this._currentCmdEncoder.ifEqual0(regNum, statementNum);
    }
    public ifGreater0(regNum:number,statementNum:number):void{
        this._currentCmdEncoder.ifGreater0(regNum, statementNum);
    }
    public ifLEqual0(regNum:number,statementNum:number):void{
        this._currentCmdEncoder.ifLEqual0(regNum, statementNum);
    }
    public ifGEqual0(regNum:number,statementNum:number):void{
        this._currentCmdEncoder.ifGEqual0(regNum, statementNum);
    }
    public ifGNotEqual0(regNum:number,statementNum:number):void{
        this._currentCmdEncoder.ifGNotEqual0(regNum, statementNum);
    }
    public operateReg(regOut:number,reg1:number,reg2:number,size:number,operateType:number,dataType:number):void{
        this._currentCmdEncoder.operateReg(regOut, reg1, reg2, size, operateType, dataType);
    }
    public store(dataID:number, offset:number, regID:number, size:number):void{
        this._currentCmdEncoder.store(dataID, offset, regID, size);
    }
    public setMainContextSize(width: number, height: number):void {
        this._nativeObj.setMainContextSize(width, height);
    }

    public getCurrentContext()
    {
        return this._currentContext;
    }
    public getProgramParameterEx(vs:string,ps:string,define:string,pname:number):number
    {
        return this._nativeObj.getProgramParameterEx(vs,ps,define,pname);
    }
    public getActiveAttribEx(vs:string,ps:string,define:string,index:number):any
    {
        return this._nativeObj.getActiveAttribEx(vs,ps,define,index);
    }
    public getActiveUniformEx(vs:string,ps:string,define:string,index:number):any
    {
        return this._nativeObj.getActiveUniformEx(vs,ps,define,index);
    }
    //为了兼容老版本用的，回头删除掉，现在都不用静态函数了
    public static createArrayBufferRef(arrayBuffer:any, type:number, syncRender:boolean):any
    {
        var bufferConchRef:any = webglPlus.createArrayBufferRef(arrayBuffer, type, syncRender, LayaGLContext.ARRAY_BUFFER_REF_REFERENCE);
        arrayBuffer["conchRef"] = bufferConchRef;
        arrayBuffer["_ptrID"] = bufferConchRef.id;
        return bufferConchRef;
    }
    //为了兼容老版本用的，回头删除掉，现在都不用静态函数了
    public static createArrayBufferRefs(arrayBuffer:any, type:number, syncRender:boolean, refType:number):any
    {
        if( !arrayBuffer._refArray )
        {
            arrayBuffer._refArray = [];
            arrayBuffer._refNum = 1;
            arrayBuffer._refArray.length = 1;
            arrayBuffer.getRefNum=function()
            {
                return this._refNum;
            }
            arrayBuffer.clearRefNum=function()
            {
                this._refNum = 1;
            }
            arrayBuffer.getRefSize=function()
            {
                return this._refArray.length;
            }
            arrayBuffer.getPtrID=function(index)
            {
                index = index ? index : 0;
                return this._refArray[index].ptrID;
            }
        }
        var bufferConchRef:any = null;
        if( refType == LayaGLContext.ARRAY_BUFFER_REF_REFERENCE)
        {
            var refArray:any = arrayBuffer._refArray;
            if( !refArray[0] )
            {
                bufferConchRef = webglPlus.createArrayBufferRef(arrayBuffer, type, syncRender, refType);
                refArray[0] =  {"ref":bufferConchRef,"ptrID":bufferConchRef.id};
            }
        }
        else
        {
            if( arrayBuffer._refNum < arrayBuffer._refArray.length )
            {
                bufferConchRef = arrayBuffer._refArray[arrayBuffer._refNum].ref;
                var nPtrID:number = arrayBuffer.getPtrID(arrayBuffer._refNum);
                webglPlus.syncArrayBufferDataToRuntime(nPtrID,bufferConchRef.isSyncToRender(),arrayBuffer);
            }
            else
            {
                bufferConchRef = webglPlus.createArrayBufferRef(arrayBuffer, type, syncRender, refType);
                arrayBuffer._refArray.push( {"ref":bufferConchRef,"ptrID":bufferConchRef.id});
            }
            arrayBuffer._refNum++;
        }
        return bufferConchRef;
    }
    //为了兼容老版本用的，回头删除掉，现在都不用静态函数了
    public static syncBufferToRenderThread(value:any,index:number=0)
    {
        if( LayaGLContext.instance._threadMode == LayaGLContext.THREAD_MODE_SINGLE )return;
        var bNeedSync:boolean = false;
        if( !value._refArray )
        {
            if( value.frameCount != LayaGLContext._frameAndSyncCountBuffer[0] )
            {
                value.frameCount = LayaGLContext._frameAndSyncCountBuffer[0];
                bNeedSync = true;
            }
        }
        else
        {
            var obj:any = value._refArray[index];
            if( obj.frameCount != LayaGLContext._frameAndSyncCountBuffer[0] )
            {
                obj.frameCount = LayaGLContext._frameAndSyncCountBuffer[0];
                bNeedSync = true;
            }
        }
        if(bNeedSync)
        {
            
            if ( (LayaGLContext._frameAndSyncCountBuffer[1] + 1) > LayaGLContext._syncBufferSize)
            {
                var pre:Int32Array = LayaGLContext._syncBufferList;
                var preConchRef:any = LayaGLContext._syncBufferList["conchRef"];
                var prePtrID:number = LayaGLContext._syncBufferList["_ptrID"];
                LayaGLContext._syncBufferSize += LayaGLContext._SYNC_ARRAYBUFFER_SIZE_;
                LayaGLContext._syncBufferList = new Int32Array(LayaGLContext._syncBufferSize);
                LayaGLContext._syncBufferList["conchRef"] = preConchRef;
                LayaGLContext._syncBufferList["_ptrID"] = prePtrID;
                pre && LayaGLContext._syncBufferList.set(pre, 0);
                webglPlus.updateArrayBufferRef(LayaGLContext._syncBufferList["_ptrID"], false, LayaGLContext._syncBufferList);
            }
            var nID = value.getPtrID?value.getPtrID(index):value["_ptrID"];
            if(!nID)
            {
                alert("syncBufferToRenderThread id error");
                debugger;
            }
            LayaGLContext._syncBufferList[LayaGLContext._frameAndSyncCountBuffer[1]++] = nID;
        }
    }
}
window["WebGLRenderingContext"]=LayaGLContext;
window["WebGL2RenderingContext"]=LayaGLContext;
window["LayaGLContext"]=LayaGLContext;
window["ProgramLocationTable"]=ProgramLocationTable;
window["GLCommandEncoder"]=GLCommandEncoder;
window["CallbackFuncObj"]=CallbackFuncObj;