var UNIFORM_TYPE;
(function (UNIFORM_TYPE) {
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM1F"] = 0] = "INTERIOR_UNIFORM1F";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM1FV"] = 1] = "INTERIOR_UNIFORM1FV";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM1I"] = 2] = "INTERIOR_UNIFORM1I";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM1IV"] = 3] = "INTERIOR_UNIFORM1IV";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM2F"] = 4] = "INTERIOR_UNIFORM2F";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM2FV"] = 5] = "INTERIOR_UNIFORM2FV";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM2I"] = 6] = "INTERIOR_UNIFORM2I";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM2IV"] = 7] = "INTERIOR_UNIFORM2IV";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM3F"] = 8] = "INTERIOR_UNIFORM3F";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM3FV"] = 9] = "INTERIOR_UNIFORM3FV";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM3I"] = 10] = "INTERIOR_UNIFORM3I";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM3IV"] = 11] = "INTERIOR_UNIFORM3IV";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM4F"] = 12] = "INTERIOR_UNIFORM4F";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM4FV"] = 13] = "INTERIOR_UNIFORM4FV";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM4I"] = 14] = "INTERIOR_UNIFORM4I";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM4IV"] = 15] = "INTERIOR_UNIFORM4IV";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORMMATRIX2FV"] = 16] = "INTERIOR_UNIFORMMATRIX2FV";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORMMATRIX3FV"] = 17] = "INTERIOR_UNIFORMMATRIX3FV";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORMMATRIX4FV"] = 18] = "INTERIOR_UNIFORMMATRIX4FV";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORMSAMPLER_2D"] = 19] = "INTERIOR_UNIFORMSAMPLER_2D";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORMSAMPLER_CUBE"] = 20] = "INTERIOR_UNIFORMSAMPLER_CUBE";
})(UNIFORM_TYPE || (UNIFORM_TYPE = {}));
var ARRAY_BUFFER_PARAM_TYPE;
(function (ARRAY_BUFFER_PARAM_TYPE) {
    ARRAY_BUFFER_PARAM_TYPE[ARRAY_BUFFER_PARAM_TYPE["ARRAY_BUFFER_REF_REFERENCE"] = 0] = "ARRAY_BUFFER_REF_REFERENCE";
    ARRAY_BUFFER_PARAM_TYPE[ARRAY_BUFFER_PARAM_TYPE["ARRAY_BUFFER_REF_COPY"] = 1] = "ARRAY_BUFFER_REF_COPY";
})(ARRAY_BUFFER_PARAM_TYPE || (ARRAY_BUFFER_PARAM_TYPE = {}));
var UPLOAD_SHADER_UNIFORM_TYPE;
(function (UPLOAD_SHADER_UNIFORM_TYPE) {
    UPLOAD_SHADER_UNIFORM_TYPE[UPLOAD_SHADER_UNIFORM_TYPE["UPLOAD_SHADER_UNIFORM_TYPE_ID"] = 0] = "UPLOAD_SHADER_UNIFORM_TYPE_ID";
    UPLOAD_SHADER_UNIFORM_TYPE[UPLOAD_SHADER_UNIFORM_TYPE["UPLOAD_SHADER_UNIFORM_TYPE_DATA"] = 1] = "UPLOAD_SHADER_UNIFORM_TYPE_DATA";
})(UPLOAD_SHADER_UNIFORM_TYPE || (UPLOAD_SHADER_UNIFORM_TYPE = {}));
var ARRAY_BUFFER_TYPE;
(function (ARRAY_BUFFER_TYPE) {
    ARRAY_BUFFER_TYPE[ARRAY_BUFFER_TYPE["ARRAY_BUFFER_TYPE_DATA"] = 0] = "ARRAY_BUFFER_TYPE_DATA";
    ARRAY_BUFFER_TYPE[ARRAY_BUFFER_TYPE["ARRAY_BUFFER_TYPE_CMD"] = 1] = "ARRAY_BUFFER_TYPE_CMD";
})(ARRAY_BUFFER_TYPE || (ARRAY_BUFFER_TYPE = {}));
class conchFloatArrayKeyframe {
    constructor() {
        this._nativeObj = new _conchFloatArrayKeyframe();
    }
    set time(value) {
        this._nativeObj.setTime(value);
    }
    get time() {
        return this._nativeObj.getTime();
    }
    set data(value) {
        this._data = value;
        this._nativeObj.setData(value);
    }
    get data() {
        return this._data;
    }
    set inTangent(value) {
        this._inTangent = value;
        this._nativeObj.setInTangent(this._inTangent);
    }
    get inTangent() {
        return this._inTangent;
    }
    set outTangent(value) {
        this._outTangent = value;
        this._nativeObj.setOutTangent(this._outTangent);
    }
    get outTangent() {
        return this._outTangent;
    }
    set value(v) {
        this._value = v;
        this._nativeObj.setValue(this._value);
    }
    get value() {
        return this._value;
    }
    clone() {
        let pDestObj = new conchFloatArrayKeyframe();
        this.cloneTo(pDestObj);
        return pDestObj;
    }
    cloneTo(destObj) {
        destObj.inTangent = this._inTangent.slice();
        destObj.outTangent = this._outTangent.slice();
        destObj.value = this._value.slice();
        destObj.data = this._data.slice();
    }
}
window["conchFloatArrayKeyframe"] = conchFloatArrayKeyframe;
class conchKeyframeNode {
    constructor() {
        this._keyFrameArray = [];
        this._type = 0;
        this._setKeyframeByIndex = function (index, keyframe) {
            this._keyFrameArray[index] = keyframe;
            if (keyframe instanceof (conchFloatArrayKeyframe)) {
                this._nativeObj._setKeyframeByIndex1(index, keyframe._nativeObj);
            }
            else {
                this._nativeObj._setKeyframeByIndex0(index, keyframe);
            }
        };
        this._nativeObj = new _conchKeyframeNode();
    }
    set data(value) {
        this._data = value;
        this._nativeObj.setFloat32ArrayData(this._data.elements);
    }
    get data() {
        return this._type == 0 ? this._nativeObj.getFloatData() : this._data;
    }
    get indexInList() {
        return this._nativeObj._indexInList;
    }
    set indexInList(value) {
        this._nativeObj._indexInList = value;
    }
    get type() {
        return this._type;
    }
    set type(type) {
        this._type = type;
        this._nativeObj.type = type;
    }
    get fullPath() {
        return this._nativeObj.fullPath;
    }
    set fullPath(path) {
        this._nativeObj.fullPath = path;
    }
    get propertyOwner() {
        return this._nativeObj.propertyOwner;
    }
    set propertyOwner(value) {
        this._nativeObj.propertyOwner = value;
    }
    get ownerPathCount() {
        return this._nativeObj.ownerPathCount;
    }
    set ownerPathCount(value) {
        this._nativeObj.ownerPathCount = value;
    }
    get propertyCount() {
        return this._nativeObj.propertyCount;
    }
    set propertyCount(value) {
        this._nativeObj.propertyCount = value;
    }
    get keyFramesCount() {
        return this._keyFrameArray.length;
    }
    set keyFramesCount(value) {
        this._keyFrameArray.length = value;
        this._nativeObj.keyFramesCount = value;
    }
    getOwnerPathCount() {
        return this._nativeObj.getOwnerPathCount();
    }
    _setOwnerPathCount(value) {
        this._nativeObj._setOwnerPathCount(value);
    }
    getPropertyCount() {
        return this._nativeObj.getPropertyCount();
    }
    _setPropertyCount(value) {
        this._nativeObj._setPropertyCount(value);
    }
    getKeyFramesCount() {
        return this._keyFrameArray.length;
    }
    _setKeyframeCount(value) {
        this._keyFrameArray.length = value;
        this._nativeObj._setKeyframeCount(value);
    }
    getOwnerPathByIndex(index) {
        return this._nativeObj.getOwnerPathByIndex(index);
    }
    _setOwnerPathByIndex(index, value) {
        this._nativeObj._setOwnerPathByIndex(index, value);
    }
    getPropertyByIndex(index) {
        return this._nativeObj.getPropertyByIndex(index);
    }
    _setPropertyByIndex(index, value) {
        this._nativeObj._setPropertyByIndex(index, value);
    }
    getKeyframeByIndex(index) {
        return this._nativeObj.getKeyframeByIndex(index);
    }
    _joinOwnerPath(sep) {
        return this._nativeObj._joinOwnerPath(sep);
    }
    _joinProperty(sep) {
        return this._nativeObj._joinProperty(sep);
    }
}
window["conchKeyframeNode"] = conchKeyframeNode;
class conchKeyframeNodeList {
    constructor() {
        this._nodes = [];
        this._nativeObj = new _conchKeyframeNodeList();
    }
    set count(value) {
        this._nodes.length = value;
        this._nativeObj.setCount(value);
    }
    get count() {
        return this._nodes.length;
    }
    getNodeByIndex(index) {
        return this._nodes[index];
    }
    setNodeByIndex(index, node) {
        this._nodes[index] = node;
        this._nativeObj.setNodeByIndex(index, node._nativeObj);
    }
}
window["conchKeyframeNodeList"] = conchKeyframeNodeList;
class _GLCommandEncoder {
    constructor(gl, reserveSize, adjustSize, isSyncToRenderThread) {
        this._adjustSize = 0;
        this._byteLen = 0;
        this._isSyncToRenderThread = false;
        this._isSyncToRenderThread = isSyncToRenderThread;
        this._layagl = gl;
        this._byteLen = reserveSize;
        this._adjustSize = adjustSize;
        this._init(isSyncToRenderThread);
    }
    _init(isSyncToRenderThread) {
        this._buffer = new ArrayBuffer(this._byteLen);
        this._idata = new Int32Array(this._buffer);
        this._fdata = new Float32Array(this._buffer);
        this._byteArray = new Uint8Array(this._buffer);
        this._layagl.createArrayBufferRef(this._buffer, ARRAY_BUFFER_TYPE.ARRAY_BUFFER_TYPE_CMD, isSyncToRenderThread);
        this._idata[0] = 1;
    }
    getAlignLength(data) {
        var byteLength = data.byteLength;
        return (byteLength + 3) & 0xfffffffc;
    }
    getPtrID() {
        return this._buffer["_ptrID"];
    }
    clearEncoding() {
        this._idata[0] = 1;
    }
    getCount() {
        return this._idata[0];
    }
    _need(sz) {
        if ((this._byteLen - (this._idata[0] << 2)) >= sz)
            return;
        this._byteLen += (sz > this._adjustSize) ? sz : this._adjustSize;
        var pre = this._idata;
        var preConchRef = this._buffer["conchRef"];
        var prePtrID = this._buffer["_ptrID"];
        this._buffer = new ArrayBuffer(this._byteLen);
        this._idata = new Int32Array(this._buffer);
        this._fdata = new Float32Array(this._buffer);
        this._byteArray = new Uint8Array(this._buffer);
        this._buffer["conchRef"] = preConchRef;
        this._buffer["_ptrID"] = prePtrID;
        pre && this._idata.set(pre, 0);
        webglPlus.updateArrayBufferRef(this._buffer["_ptrID"], preConchRef.isSyncToRender(), this._buffer);
    }
    addShaderUniform(one) {
        var funID = 0;
        var isArray = one.isArray;
        switch (one.type) {
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
        this._layagl.syncBufferToRenderThread(this._buffer);
        this.add_iiiiii(3, funID, one.location.id, one.type, one.dataOffset, one.textureID);
    }
    add_iiiiii(a, b, c, d, e, f) {
        this._need(24);
        var idata = this._idata;
        var i = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        idata[i++] = e;
        idata[i++] = f;
        idata[0] = i;
    }
}
_GLCommandEncoder.INT = 0x1404;
_GLCommandEncoder.FLOAT = 0x1406;
_GLCommandEncoder.FLOAT_VEC2 = 0x8B50;
_GLCommandEncoder.FLOAT_VEC3 = 0x8B51;
_GLCommandEncoder.FLOAT_VEC4 = 0x8B52;
_GLCommandEncoder.INT_VEC2 = 0x8B53;
_GLCommandEncoder.INT_VEC3 = 0x8B54;
_GLCommandEncoder.INT_VEC4 = 0x8B55;
_GLCommandEncoder.BOOL = 0x8B56;
_GLCommandEncoder.BOOL_VEC2 = 0x8B57;
_GLCommandEncoder.BOOL_VEC3 = 0x8B58;
_GLCommandEncoder.BOOL_VEC4 = 0x8B59;
_GLCommandEncoder.FLOAT_MAT2 = 0x8B5A;
_GLCommandEncoder.FLOAT_MAT3 = 0x8B5B;
_GLCommandEncoder.FLOAT_MAT4 = 0x8B5C;
_GLCommandEncoder.SAMPLER_2D = 0x8B5E;
_GLCommandEncoder.SAMPLER_CUBE = 0x8B60;
function extendWebGLPlusToWebGLContext(gl) {
    gl.prototype.createArrayBufferRef = function (arrayBuffer, type, syncRender) {
        var bufferConchRef = webglPlus.createArrayBufferRef(arrayBuffer, type, syncRender, ARRAY_BUFFER_PARAM_TYPE.ARRAY_BUFFER_REF_REFERENCE);
        arrayBuffer["conchRef"] = bufferConchRef;
        arrayBuffer["_ptrID"] = bufferConchRef.id;
        return bufferConchRef;
    };
    gl.prototype.createArrayBufferRefs = function (arrayBuffer, type, syncRender, refType) {
        if (!arrayBuffer._refArray) {
            arrayBuffer._refArray = [];
            arrayBuffer._refNum = 1;
            arrayBuffer._refArray.length = 1;
            arrayBuffer.getRefNum = function () {
                return this._refNum;
            };
            arrayBuffer.clearRefNum = function () {
                this._refNum = 1;
            };
            arrayBuffer.getRefSize = function () {
                return this._refArray.length;
            };
            arrayBuffer.getPtrID = function (index) {
                index = index ? index : 0;
                return this._refArray[index].ptrID;
            };
        }
        var bufferConchRef = null;
        if (refType == ARRAY_BUFFER_PARAM_TYPE.ARRAY_BUFFER_REF_REFERENCE) {
            var refArray = arrayBuffer._refArray;
            if (!refArray[0]) {
                bufferConchRef = webglPlus.createArrayBufferRef(arrayBuffer, type, syncRender, refType);
                refArray[0] = { "ref": bufferConchRef, "ptrID": bufferConchRef.id };
            }
        }
        else {
            if (arrayBuffer._refNum < arrayBuffer._refArray.length) {
                bufferConchRef = arrayBuffer._refArray[arrayBuffer._refNum].ref;
                var nPtrID = arrayBuffer.getPtrID(arrayBuffer._refNum);
                webglPlus.syncArrayBufferDataToRuntime(nPtrID, bufferConchRef.isSyncToRender(), arrayBuffer);
            }
            else {
                bufferConchRef = webglPlus.createArrayBufferRef(arrayBuffer, type, syncRender, refType);
                arrayBuffer._refArray.push({ "ref": bufferConchRef, "ptrID": bufferConchRef.id });
            }
            arrayBuffer._refNum++;
        }
        return bufferConchRef;
    };
    gl.prototype.updateArrayBufferRef = function (bufferID, isSyncToRender, buffer) {
        return webglPlus.updateArrayBufferRef(bufferID, isSyncToRender, buffer);
    };
    gl.prototype.updateAnimationNodeWorldMatix = function (locPosition, locRotation, locScaling, parentIndices, outWorldMatrix) {
        return webglPlus.updateAnimationNodeWorldMatix(locPosition, locRotation, locScaling, parentIndices, outWorldMatrix);
    };
    gl.prototype.computeSubSkinnedData = function (worldMatrixs, worldMatrixIndex, inverseBindPoses, boneIndices, bindPoseInices, resultData) {
        return webglPlus.computeSubSkinnedData(worldMatrixs, worldMatrixIndex, inverseBindPoses, boneIndices, bindPoseInices, resultData);
    };
    gl.prototype.evaluateClipDatasRealTime = function (nodes, playCurTime, realTimeCurrentFrameIndexs, addtive, frontPlay) {
        webglPlus.evaluateClipDatasRealTime(nodes, playCurTime, realTimeCurrentFrameIndexs, addtive, frontPlay);
    };
    gl.prototype.culling = function (boundFrustumBuffer, cullingBuffer, cullingBufferIndices, cullingCount, cullingBufferResult) {
        return webglPlus.culling(boundFrustumBuffer, cullingBuffer, cullingBufferIndices, cullingCount, cullingBufferResult);
    };
    if (!window["conch"]) {
        gl.prototype.createCommandEncoder = function (reserveSize, adjustSize, isSyncToRenderThread) {
            reserveSize = reserveSize ? reserveSize : 128;
            adjustSize = adjustSize ? adjustSize : 64;
            isSyncToRenderThread = isSyncToRenderThread ? isSyncToRenderThread : false;
            var cmd = new _GLCommandEncoder(this, reserveSize, adjustSize, isSyncToRenderThread);
            if (isSyncToRenderThread) {
                this.syncBufferToRenderThread(cmd);
            }
            return cmd;
        };
        window["GLCommandEncoder"] = _GLCommandEncoder;
        gl.prototype.syncBufferToRenderThread = function (value, index = 0) {
        };
    }
    if (!window["conch"]) {
        gl.prototype.uploadShaderUniforms = function (commandEncoder, data, type) {
            if (type == UPLOAD_SHADER_UNIFORM_TYPE.UPLOAD_SHADER_UNIFORM_TYPE_ID) {
                var dataID = data["_ptrID"];
                this.syncBufferToRenderThread(data);
            }
            else {
                var nAlignLength = this.getAlignLength(data);
            }
            return 0;
        };
        gl.prototype.uniformMatrix2fvEx = function (location, transpose, value) {
            if (!value["_ptrID"]) {
                this.createArrayBufferRef(value, ARRAY_BUFFER_TYPE.ARRAY_BUFFER_TYPE_DATA, true);
            }
            var nID = value["_ptrID"];
            this.syncBufferToRenderThread(value);
        };
        gl.prototype.uniformMatrix3fvEx = function (location, transpose, value) {
            if (!value["_ptrID"]) {
                this.createArrayBufferRef(value, ARRAY_BUFFER_TYPE.ARRAY_BUFFER_TYPE_DATA, true);
            }
            var nID = value["_ptrID"];
            this.syncBufferToRenderThread(value);
        };
        gl.prototype.uniformMatrix4fvEx = function (location, transpose, value) {
            if (!value["_ptrID"]) {
                this.createArrayBufferRef(value, ARRAY_BUFFER_TYPE.ARRAY_BUFFER_TYPE_DATA, true);
            }
            var nID = value["_ptrID"];
            this.syncBufferToRenderThread(value);
        };
    }
}
window["extendWebGLPlusToWebGLContext"] = extendWebGLPlusToWebGLContext;
