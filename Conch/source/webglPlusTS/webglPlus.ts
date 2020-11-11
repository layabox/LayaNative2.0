///<reference path="./LayaConchRuntime.d.ts" />
///<reference path="../domsupport/ES6NoDOM.d.ts" />
/**
 * name
 */
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
//传入ArrayBufferType
enum ARRAY_BUFFER_PARAM_TYPE
{
    ARRAY_BUFFER_REF_REFERENCE = 0,
    ARRAY_BUFFER_REF_COPY = 1,
}
//uploadShaderUniformType
enum UPLOAD_SHADER_UNIFORM_TYPE
{
    UPLOAD_SHADER_UNIFORM_TYPE_ID = 0,       //data按照ID传入
    UPLOAD_SHADER_UNIFORM_TYPE_DATA = 1,     //data按照数据传入
}
enum ARRAY_BUFFER_TYPE
{
    ARRAY_BUFFER_TYPE_DATA = 0,             //创建ArrayBuffer时的类型为Data
    ARRAY_BUFFER_TYPE_CMD = 1,              //创建ArrayBuffer时的类型为Command
}

declare var window:Window;
class conchFloatArrayKeyframe
{
    _nativeObj:_conchFloatArrayKeyframe;
    _inTangent:Float32Array;
    _outTangent:Float32Array;
    _value:Float32Array;
    _data:Float32Array;
    constructor()
    {
        this._nativeObj = new _conchFloatArrayKeyframe();
    }
    set time(value:number)
    {
        this._nativeObj.setTime(value);
    }
    get time():number
    {
        return this._nativeObj.getTime();
    }
    set data(value:Float32Array)
    {
        this._data=value;
        this._nativeObj.setData(value);
    }
    get data()
    {
        return this._data;
    }
    set inTangent(value:Float32Array)
    {
        this._inTangent=value;
        this._nativeObj.setInTangent(this._inTangent);
    }
    get inTangent()
    {
        return this._inTangent;
    }
    set outTangent(value:Float32Array)
    {
        this._outTangent=value;
        this._nativeObj.setOutTangent(this._outTangent);
    }
    get outTangent()
    {
        return this._outTangent;
    }
    set value(v:Float32Array)
    {
        this._value=v;
        this._nativeObj.setValue(this._value);
    }
    get value()
    {
        return this._value;
    }
    clone()
    {
        let pDestObj:conchFloatArrayKeyframe = new conchFloatArrayKeyframe();
        this.cloneTo(pDestObj);
        return pDestObj;
    }
    cloneTo(destObj:conchFloatArrayKeyframe)
    {
        destObj.inTangent = this._inTangent.slice() as Float32Array;
        destObj.outTangent = this._outTangent.slice() as Float32Array;
        destObj.value = this._value.slice() as Float32Array;
        destObj.data = this._data.slice() as Float32Array;
    }
}
window["conchFloatArrayKeyframe"]=conchFloatArrayKeyframe;

class conchKeyframeNode
{
    _nativeObj:_conchKeyframeNode;
    _data:any;
    _keyFrameArray:Array<any> = [];
    _type:number = 0;
    constructor()
    {
        this._nativeObj = new _conchKeyframeNode();
    }
    set data(value:any)
    {
        this._data = value;
        this._nativeObj.setFloat32ArrayData(this._data.elements);//ConchVector3 or ConchQuaternion
    }
    get data()
    {
        //优化
        //return this._nativeObj.getDataType()==0 ? this._nativeObj.getFloatData() : this._data;
        return this._type == 0 ? this._nativeObj.getFloatData() : this._data;
    }
    get indexInList()
    {
        return this._nativeObj._indexInList;
    }
    set indexInList(value:number)
    {
        this._nativeObj._indexInList = value;
    }
    get type()
    {
        return this._type;
    }
    set type(type:number)
    {
        this._type = type;
        this._nativeObj.type = type;
    }
    get fullPath()
    {
        return this._nativeObj.fullPath;
    }
    set fullPath(path:string)
    {
        this._nativeObj.fullPath = path;
    }
    get propertyOwner()
    {
        return this._nativeObj.propertyOwner;
    }
    set propertyOwner(value:string)
    {
        this._nativeObj.propertyOwner = value;
    }
    get ownerPathCount()
    {
        return this._nativeObj.ownerPathCount;
    }
    set ownerPathCount(value:number)
    {
        this._nativeObj.ownerPathCount = value;
    }
    get propertyCount()
    {
        return this._nativeObj.propertyCount;
    }
    set propertyCount(value:number)
    {
        this._nativeObj.propertyCount = value;
    }
    get keyFramesCount()
    {
        return this._keyFrameArray.length;
    }
    set keyFramesCount(value:number)
    {
        this._keyFrameArray.length = value;
        this._nativeObj.keyFramesCount = value;
    }
    getOwnerPathCount()
    {
        return this._nativeObj.getOwnerPathCount();
    }
    _setOwnerPathCount(value:number)
    {
        this._nativeObj._setOwnerPathCount(value);
    }
    getPropertyCount()
    {
        return this._nativeObj.getPropertyCount();
    }
    _setPropertyCount(value:number)
    {
        this._nativeObj._setPropertyCount(value);
    }
    getKeyFramesCount()
    {
        return this._keyFrameArray.length;
    }
    _setKeyframeCount(value:number)
    {
        this._keyFrameArray.length = value;
        this._nativeObj._setKeyframeCount(value);
    }
    getOwnerPathByIndex(index:number)
    {
        return this._nativeObj.getOwnerPathByIndex(index)
    }
    _setOwnerPathByIndex(index:number, value:string)
    {
        this._nativeObj._setOwnerPathByIndex(index, value);
    }
    getPropertyByIndex(index:number)
    {
        return this._nativeObj.getPropertyByIndex(index);
    }
    _setPropertyByIndex(index:number, value:string)
    {
        this._nativeObj._setPropertyByIndex(index, value)
    }
    getKeyframeByIndex(index:number)
    {
        return this._nativeObj.getKeyframeByIndex(index);
    }
    _joinOwnerPath(sep:string)
    {
        return this._nativeObj._joinOwnerPath(sep);
    }
    _joinProperty(sep:string)
    {
        return this._nativeObj._joinProperty(sep);
    }
    _setKeyframeByIndex=function(index:number,keyframe:any)
    {
        this._keyFrameArray[index] = keyframe;
        if( keyframe instanceof(conchFloatArrayKeyframe) )
        {
            this._nativeObj._setKeyframeByIndex1(index,keyframe._nativeObj);
        }
        else
        {
            this._nativeObj._setKeyframeByIndex0(index,keyframe);
        }
    }
}
window["conchKeyframeNode"]=conchKeyframeNode;

class conchKeyframeNodeList
{
    _nativeObj:_conchKeyframeNodeList;
    _nodes:Array<conchKeyframeNode> = [];
    constructor()
    {
        this._nativeObj = new _conchKeyframeNodeList();
    }
    set count(value:number)
    {
        this._nodes.length = value;
        this._nativeObj.setCount(value);
    }
    get count()
    {
        return this._nodes.length;
    }
    getNodeByIndex(index:number)
    {
        return this._nodes[index];
    }
    setNodeByIndex(index:number,node:conchKeyframeNode)
    {
        this._nodes[index] = node;
        this._nativeObj.setNodeByIndex(index,node._nativeObj);
    }
}
window["conchKeyframeNodeList"] = conchKeyframeNodeList;

class _GLCommandEncoder
{
    private static readonly INT = 0x1404;
    private static readonly FLOAT = 0x1406;
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
    public _idata:Int32Array;
    public _fdata:Float32Array;
    public _byteArray:Uint8Array;
    public _buffer:ArrayBuffer;
    private _adjustSize:number = 0;
    private _byteLen:number = 0;
    private _layagl:any;
    public _isSyncToRenderThread:Boolean = false;
    constructor(gl:any, reserveSize:number, adjustSize:number, isSyncToRenderThread:boolean)
    {
        this._isSyncToRenderThread = isSyncToRenderThread;
        this._layagl = gl;
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
        this._layagl.createArrayBufferRef(this._buffer, ARRAY_BUFFER_TYPE.ARRAY_BUFFER_TYPE_CMD, isSyncToRenderThread);
        this._idata[0] = 1;
    }
    getAlignLength(data:any):number{
        var byteLength = data.byteLength;
        return (byteLength + 3) & 0xfffffffc;
    }
    getPtrID():number
    {
        return this._buffer["_ptrID"];
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
    addShaderUniform(one:any):void
    {
        var funID:number = 0;
        var isArray:Boolean = one.isArray;
        switch (one.type)
        {
        case _GLCommandEncoder.INT: 
            funID = isArray ? UNIFORM_TYPE.INTERIOR_UNIFORM1IV : UNIFORM_TYPE.INTERIOR_UNIFORM1I;
            break;
        case _GLCommandEncoder.FLOAT: 
            funID = isArray ? UNIFORM_TYPE.INTERIOR_UNIFORM1FV : UNIFORM_TYPE.INTERIOR_UNIFORM1F;
            break;
        case _GLCommandEncoder.FLOAT_VEC2: 
            funID = isArray ? UNIFORM_TYPE.INTERIOR_UNIFORM2FV : UNIFORM_TYPE.INTERIOR_UNIFORM2F;
            break;
        case _GLCommandEncoder.FLOAT_VEC3: 
            funID = isArray ? UNIFORM_TYPE.INTERIOR_UNIFORM3FV : UNIFORM_TYPE.INTERIOR_UNIFORM3F;
            break;
        case _GLCommandEncoder.FLOAT_VEC4: 
            funID = isArray ? UNIFORM_TYPE.INTERIOR_UNIFORM4FV : UNIFORM_TYPE.INTERIOR_UNIFORM4F;
            break;
        case _GLCommandEncoder.SAMPLER_2D: 
            funID = UNIFORM_TYPE.INTERIOR_UNIFORMSAMPLER_2D;
            break;
        case _GLCommandEncoder.SAMPLER_CUBE: 
            funID = UNIFORM_TYPE.INTERIOR_UNIFORMSAMPLER_CUBE;
            break;
        case _GLCommandEncoder.FLOAT_MAT4: 
            funID = UNIFORM_TYPE.INTERIOR_UNIFORMMATRIX4FV;
            break;
        case _GLCommandEncoder.BOOL: 
            funID = UNIFORM_TYPE.INTERIOR_UNIFORM1I;
            break;
        case _GLCommandEncoder.FLOAT_MAT2: 
            funID = UNIFORM_TYPE.INTERIOR_UNIFORMMATRIX2FV;
            break;
        case _GLCommandEncoder.FLOAT_MAT3: 
            funID = UNIFORM_TYPE.INTERIOR_UNIFORMMATRIX3FV;
            break;
        default: 
            throw new Error("compile shader err!");
        }
        //同步ArrayBufferList
        this._layagl.syncBufferToRenderThread(this._buffer);
        //第二个参数3  就是函数ID = FUNCTION_ID.ADDSHADERUNIFORM 
        this.add_iiiiii(3, funID, one.location.id, one.type, one.dataOffset, one.textureID);
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
}

function extendWebGLPlusToWebGLContext(gl:any)
{
    gl.prototype.createArrayBufferRef=function(arrayBuffer:any, type:number, syncRender:boolean):any
    {
        var bufferConchRef:any = webglPlus.createArrayBufferRef(arrayBuffer, type, syncRender, ARRAY_BUFFER_PARAM_TYPE.ARRAY_BUFFER_REF_REFERENCE);
        arrayBuffer["conchRef"] = bufferConchRef;
        arrayBuffer["_ptrID"] = bufferConchRef.id;
        return bufferConchRef;
    }
    gl.prototype.createArrayBufferRefs=function(arrayBuffer:any, type:number, syncRender:boolean, refType:number):any
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
        if( refType == ARRAY_BUFFER_PARAM_TYPE.ARRAY_BUFFER_REF_REFERENCE)
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
    gl.prototype.updateArrayBufferRef=function(bufferID:number,isSyncToRender:boolean, buffer:any):any
    {
        return webglPlus.updateArrayBufferRef(bufferID,isSyncToRender,buffer);
    }
    gl.prototype.updateAnimationNodeWorldMatix=function(locPosition, locRotation, locScaling, parentIndices, outWorldMatrix)
    {
        return webglPlus.updateAnimationNodeWorldMatix(locPosition, locRotation, locScaling, parentIndices, outWorldMatrix);
    }
    gl.prototype.computeSubSkinnedData=function(worldMatrixs, worldMatrixIndex, inverseBindPoses,boneIndices, bindPoseInices, resultData)
    {
        return webglPlus.computeSubSkinnedData(worldMatrixs, worldMatrixIndex, inverseBindPoses,boneIndices, bindPoseInices, resultData);
    }
    gl.prototype.evaluateClipDatasRealTime=function(nodes:any, playCurTime:number, realTimeCurrentFrameIndexs:any, addtive:boolean, frontPlay:boolean)
    {
        webglPlus.evaluateClipDatasRealTime(nodes,playCurTime,realTimeCurrentFrameIndexs,addtive, frontPlay);
    }
    gl.prototype.culling=function(boundFrustumBuffer:Float32Array,cullingBuffer:Float32Array, cullingBufferIndices:Int32Array, cullingCount:number, cullingBufferResult:Int32Array):number
    {
        return webglPlus.culling(boundFrustumBuffer,cullingBuffer, cullingBufferIndices, cullingCount,cullingBufferResult);
    }
    if(!window["conch"])
    {
        gl.prototype.createCommandEncoder=function(reserveSize?:number, adjustSize?:number, isSyncToRenderThread?:boolean):any
        {
            reserveSize=reserveSize?reserveSize:128;
            adjustSize=adjustSize?adjustSize:64;
            isSyncToRenderThread=isSyncToRenderThread?isSyncToRenderThread:false;
            var cmd:_GLCommandEncoder = new _GLCommandEncoder(this, reserveSize, adjustSize, isSyncToRenderThread );
            if(isSyncToRenderThread)
            {
                this.syncBufferToRenderThread(cmd);
            }
            return cmd;
        }
        window["GLCommandEncoder"] = _GLCommandEncoder;
        gl.prototype.syncBufferToRenderThread=function(value:any,index:number=0)
        {
            //不用处理
        }
    }
    if(!window["conch"])
    {
        gl.prototype.uploadShaderUniforms=function(commandEncoder:any, data:any, type:number):number
        {
            if(type==UPLOAD_SHADER_UNIFORM_TYPE.UPLOAD_SHADER_UNIFORM_TYPE_ID)
            {
                var dataID:number = data["_ptrID"];
                this.syncBufferToRenderThread(data);
                //TODO   这个地方平台方自己处理，如果压流就需要按照压流的来，如果直接调用可以直接调用
                //压流
                //this.add_iii(FUNCTION_ID.UPLOADSHADERUNIFORMS, commandEncoder._buffer["_ptrID"], dataID);
                //直接调用
                //webglPlus.uploadShaderUniforms(commandEncoder._buffer["_ptrID"], dataID);

            }        
            else
            {
                var nAlignLength = this.getAlignLength(data);
                //TODO   这个地方平台方自己处理，如果压流就需要按照压流的来，如果直接调用可以直接调用
                //压流
                //this.add_ii(FUNCTION_ID.UPLOADSHADERUNIFORMS_BUFFER, commandEncoder._buffer["_ptrID"]);
                //this.wab(data, data.byteLength, nAlignLength);            
                //直接调用
				/*var copyAb = new ArrayBuffer(data.byteLength);
                var copyVi = new Uint8Array(copyAb);
                var vi = new Uint8Array(data);
                copyVi.set(vi);
				webglPlus.uploadShaderUniforms(commandEncoder._buffer["_ptrID"],copyAb);*/
            }
            return 0;
        }
        gl.prototype.uniformMatrix2fvEx=function(location:any, transpose:any, value:any):void
        {
            if (!value["_ptrID"])
            {
                this.createArrayBufferRef(value, ARRAY_BUFFER_TYPE.ARRAY_BUFFER_TYPE_DATA, true);
            }
            var nID:number = value["_ptrID"];
            this.syncBufferToRenderThread(value);
            //TODO   这个地方平台方自己处理，如果压流就需要按照压流的来，如果直接调用可以直接调用
            //压流
            //this.add_iiii(FUNCTION_ID.UNIFORMMATRIX2FVEX, location, transpose, nID);
            //直接调用
			//webglPlus.uniformMatrix2fvEx(location.id, transpose, nID);
        }
        gl.prototype.uniformMatrix3fvEx=function(location:any, transpose:any, value:any):void
        {
            if (!value["_ptrID"])
            {
                this.createArrayBufferRef(value, ARRAY_BUFFER_TYPE.ARRAY_BUFFER_TYPE_DATA, true);
            }
            var nID:number = value["_ptrID"];
            this.syncBufferToRenderThread(value); 
            
            //TODO   这个地方平台方自己处理，如果压流就需要按照压流的来，如果直接调用可以直接调用
            //压流
            //this.add_iiii(FUNCTION_ID.UNIFORMMATRIX3FVEX, location, transpose, nID);
            //直接调用
			//webglPlus.uniformMatrix3fvEx(location.id, transpose, nID);
        }
        gl.prototype.uniformMatrix4fvEx=function(location:any, transpose:any, value:any):void
        {
            if (!value["_ptrID"])
            {
                this.createArrayBufferRef(value, ARRAY_BUFFER_TYPE.ARRAY_BUFFER_TYPE_DATA, true);
            }
            var nID:number = value["_ptrID"];
            this.syncBufferToRenderThread(value);
            
            //TODO   这个地方平台方自己处理，如果压流就需要按照压流的来，如果直接调用可以直接调用
            //压流
            //this.add_iiii(FUNCTION_ID.UNIFORMMATRIX4FVEX, location, transpose, nID);
            //直接调用
			//webglPlus.uniformMatrix4fvEx(location.id, transpose, nID);
        }
    }
}
window["extendWebGLPlusToWebGLContext"]=extendWebGLPlusToWebGLContext;