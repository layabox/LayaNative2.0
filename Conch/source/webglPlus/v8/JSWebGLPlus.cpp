/**
@file			JSWebGLPlus.cpp
@brief			
@author			James
@version		1.0
@date			2019_8_24
*/

#include "JSWebGLPlus.h"
#include "../JCWebGLPlus.h"
#include "JSArrayBufferRef.h"
#include "JSKeyframeNodeList.h"
#include "../Log.h"
#define NDEBUG
#include <assert.h>
#include "V8Util.h"
//------------------------------------------------------------------------------
namespace laya 
{
    JSWebGLPlus* JSWebGLPlus::s_pWebGLPlus = NULL;
    JSWebGLPlus::JSWebGLPlus()
    {
        if (JSObjNode::s_pListJSObj)
        {
            JSObjNode::s_pListJSObj->push_back(this);
        }
        v8::Isolate::GetCurrent()->AdjustAmountOfExternalAllocatedMemory(8192);
        //JCMemorySurvey::GetInstance()->newClass("webglPlus", 8192, this);
    }
    JSWebGLPlus::~JSWebGLPlus()
    {
        if (JSObjNode::s_pListJSObj)
        {
            JSObjNode::s_pListJSObj->delNode(this);
        }
        persistentObject.Reset();
        s_pWebGLPlus = NULL;
        //JCMemorySurvey::GetInstance()->releaseClass("webglPlus", this);
    }
    JSWebGLPlus* JSWebGLPlus::getInstance()
    {
        if (s_pWebGLPlus == NULL)
        {
            s_pWebGLPlus = new JSWebGLPlus();
        }
        return s_pWebGLPlus;
    }
    

    static void Culling(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        TEST_WEBGLPLUS_LOG("webglplus Culling");

        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Object> pthis = args.This();
        int len = args.Length();
        if (len < 4)
        {
            args.GetReturnValue().Set(v8::Int32::New(isolate, 0));
            return;
        }

        v8::Local<v8::Value> boundFrustumBuffer = args[0];
        v8::Local<v8::Value> cullingBuffer = args[1];
        v8::Local<v8::Value> cullingBufferIndices = args[2];
        assert(args[3]->IsNumber());
        int cullingCount = args[3]->Int32Value();
        v8::Local<v8::Value> cullingBufferResult = args[4];

        char* pFrustumBuffer;
        char* pCullingBuffer;
        char* pCullingBufferIndices;
        char* pCullingBufferResult;
        int nFrustumLen = 0;
        int nCullingBufferLen = 0;
        int nCullingBufferIndicesLen = 0;
        int nCullingBufferResultLen = 0;

        if (V8Util::extractJSAB(boundFrustumBuffer, pFrustumBuffer, nFrustumLen) == false)
        {
            LOGE("culling culling frustum error");
            args.GetReturnValue().Set(v8::Int32::New(isolate, 0));
            return;
        }
        if (V8Util::extractJSAB(cullingBuffer, pCullingBuffer, nCullingBufferLen) == false)
        {
            LOGE("culling culling buffer error");
            args.GetReturnValue().Set(v8::Int32::New(isolate, 0));
            return;
        }
        if (V8Util::extractJSAB(cullingBufferIndices, pCullingBufferIndices, nCullingBufferIndicesLen) == false)
        {
            LOGE("culling culling buffer indices error");
            args.GetReturnValue().Set(v8::Int32::New(isolate, 0));
            return;
        }
        if (V8Util::extractJSAB(cullingBufferResult, pCullingBufferResult, nCullingBufferResultLen) == false)
        {
            LOGE("culling result error");
            args.GetReturnValue().Set(v8::Int32::New(isolate, 0));
            return;
        }
        int count = JCWebGLPlus::getInstance()->culling3D(cullingCount, pFrustumBuffer, nFrustumLen, pCullingBuffer, nCullingBufferLen, pCullingBufferIndices, nCullingBufferIndicesLen, pCullingBufferResult, nCullingBufferResultLen);
        args.GetReturnValue().Set(v8::Int32::New(isolate, count));
    }

    static void UpdateAnimationNodeWorldMatix(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        TEST_WEBGLPLUS_LOG("webglplus UpdateAnimationNodeWorldMatix");

        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Object> pthis = args.This();
        int len = args.Length();
        if (len < 5)
        {
            args.GetReturnValue().Set(v8::Boolean::New(isolate, false));
            return;
        }


        v8::Local<v8::Value> locPosition = args[0];
        v8::Local<v8::Value> locRotation = args[1];
        v8::Local<v8::Value> locScaling = args[2];
        v8::Local<v8::Value> parentIndices = args[3];
        v8::Local<v8::Value> outWorldMatrix = args[4];


        char* pLocPosition;
        char* pLocRotation;
        char* pLocScaling;
        char* pParentIndices;
        char* pOutWorldMatrix;
        int nLocPosLen = 0;
        int nLocRotLen = 0;
        int nLocScaLen = 0;
        int nLocParentLen = 0;
        int nOutWorldLen = 0;
        if (V8Util::extractJSAB(locPosition, pLocPosition, nLocPosLen) == false)
        {
            LOGE("updateAnimationNodeWorldMatix postion error");
            args.GetReturnValue().Set(v8::Boolean::New(isolate, false));
            return;
        }
        if (V8Util::extractJSAB(locRotation, pLocRotation, nLocRotLen) == false)
        {
            LOGE("updateAnimationNodeWorldMatix rotateion error");
            args.GetReturnValue().Set(v8::Boolean::New(isolate, false));
            return;
        }
        if (V8Util::extractJSAB(locScaling, pLocScaling, nLocScaLen) == false)
        {
            LOGE("updateAnimationNodeWorldMatix scaling error");
            args.GetReturnValue().Set(v8::Boolean::New(isolate, false));
            return;
        }
        if (V8Util::extractJSAB(parentIndices, pParentIndices, nLocParentLen) == false)
        {
            LOGE("updateAnimationNodeWorldMatix parent index error");
            args.GetReturnValue().Set(v8::Boolean::New(isolate, false));
            return;
        }
        if (V8Util::extractJSAB(outWorldMatrix, pOutWorldMatrix, nOutWorldLen) == false)
        {
            LOGE("updateAnimationNodeWorldMatix world marix error");
            args.GetReturnValue().Set(v8::Boolean::New(isolate, false));
            return;
        }
        bool value = JCWebGLPlus::getInstance()->updateAnimationNodeWorldMatix3D(pLocPosition, nLocPosLen, pLocRotation, nLocRotLen, pLocScaling, nLocScaLen,
            pParentIndices, nLocParentLen, pOutWorldMatrix, nOutWorldLen);
        args.GetReturnValue().Set(v8::Boolean::New(isolate, value));
    }

    static void ComputeSubSkinnedData(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        TEST_WEBGLPLUS_LOG("webglplus ComputeSubSkinnedData");

        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Object> pthis = args.This();

        int len = args.Length();
        if (len < 6)
        {
            args.GetReturnValue().Set(v8::Boolean::New(isolate, false));
            return;
        }

        v8::Local<v8::Value> worldMatrixs = args[0];
        v8::Local<v8::Value> worldMatrixIndex = args[1];
        v8::Local<v8::Value> inverseBindPoses = args[2];
        v8::Local<v8::Value> boneIndices = args[3];
        v8::Local<v8::Value> bindPoseInices = args[4];
        v8::Local<v8::Value> resultData = args[5];


        char* pWorldMatrixs;
        char* pWorldMatrixIndex;
        char* pInverseBindPoses;
        char* pBoneIndices;
        char* pBindPoseIndices;
        char* pResultData;
        int nWorldMatLen = 0;
        int nWorldMatIndeLen = 0;
        int nInverseBindPosesLen = 0;
        int nBoneIndicesLen = 0;
        int nBindPoseIndicesLen = 0;
        int nResultDataLen = 0;
        if (V8Util::extractJSAB(worldMatrixs, pWorldMatrixs, nWorldMatLen) == false)
        {
            LOGE("computeSubSkinnedDataNative world matrix error");
            args.GetReturnValue().Set(v8::Boolean::New(isolate, false));
            return;
        }
        if (V8Util::extractJSAB(worldMatrixIndex, pWorldMatrixIndex, nWorldMatIndeLen) == false)
        {
            LOGE("computeSubSkinnedDataNative world matrix index error");
            args.GetReturnValue().Set(v8::Boolean::New(isolate, false));
            return;
        }
        if (V8Util::extractJSAB(inverseBindPoses, pInverseBindPoses, nInverseBindPosesLen) == false)
        {
            LOGE("computeSubSkinnedDataNative inverse bind poses error");
            args.GetReturnValue().Set(v8::Boolean::New(isolate, false));
            return;
        }
        if (V8Util::extractJSAB(boneIndices, pBoneIndices, nBoneIndicesLen) == false)
        {
            LOGE("computeSubSkinnedDataNative bone indices error");
            args.GetReturnValue().Set(v8::Boolean::New(isolate, false));
            return;
        }
        if (V8Util::extractJSAB(bindPoseInices, pBindPoseIndices, nBindPoseIndicesLen) == false)
        {
            LOGE("computeSubSkinnedDataNative bind pose indices error");
            args.GetReturnValue().Set(v8::Boolean::New(isolate, false));
            return;
        }
        if (V8Util::extractJSAB(resultData, pResultData, nResultDataLen) == false)
        {
            LOGE("computeSubSkinnedDataNative data error");
            args.GetReturnValue().Set(v8::Boolean::New(isolate, false));
            return;
        }
        bool value = JCWebGLPlus::getInstance()->computeSubSkinnedData3D(pWorldMatrixs, nWorldMatLen, pWorldMatrixIndex, nWorldMatIndeLen,
            pInverseBindPoses, nInverseBindPosesLen, pBoneIndices, nBoneIndicesLen,
            pBindPoseIndices, nBindPoseIndicesLen, pResultData, nResultDataLen);

        args.GetReturnValue().Set(v8::Boolean::New(isolate, value));
    }

    static void CreateArrayBufferRef(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        TEST_WEBGLPLUS_LOG("webglplus CreateArrayBufferRef");

        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Object> pthis = args.This();

        int len = args.Length();
        if (len < 4)
        {
            args.GetReturnValue().Set(v8::Null(isolate));
            return;
        }

        v8::Local<v8::Value> pArrayBuffer = args[0];
        assert(args[1]->IsInt32());
        int nType = args[1]->Int32Value();
        assert(args[2]->IsBoolean());
        bool bSyncToRender = args[2]->BooleanValue();
        assert(args[3]->IsInt32());
        int nRefType = args[3]->Int32Value();
       

        char* pBuffer = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = V8Util::extractJSAB(pArrayBuffer, pBuffer, nABLen);
        if (bIsArrayBuffer)
        {
            JSArrayBufferRef* pArrayBufferRef = new JSArrayBufferRef();
            pArrayBufferRef->m_bSyncToRender = bSyncToRender;
            if (JCWebGLPlus::getInstance()->m_nThreadMODE == THREAD_MODE_DOUBLE)
            {
                if (bSyncToRender)
                {
                    pArrayBufferRef->m_nID = JCWebGLPlus::getInstance()->m_pJSABManagerSyncToRender->createArrayBuffer(pBuffer, nABLen, (JCArrayBufferManager::ARRAY_BUFFER_TYPE)nType, (JCArrayBufferManager::ARRAY_BUFFER_REF_TYPE)nRefType);
                }
                else
                {
                    pArrayBufferRef->m_nID = JCWebGLPlus::getInstance()->m_pJSArrayBufferManager->createArrayBuffer(pBuffer, nABLen, (JCArrayBufferManager::ARRAY_BUFFER_TYPE)nType, (JCArrayBufferManager::ARRAY_BUFFER_REF_TYPE)nRefType);
                }
            }
            else
            {
                pArrayBufferRef->m_nID = JCWebGLPlus::getInstance()->m_pJSArrayBufferManager->createArrayBuffer(pBuffer, nABLen, (JCArrayBufferManager::ARRAY_BUFFER_TYPE)nType, (JCArrayBufferManager::ARRAY_BUFFER_REF_TYPE)nRefType);
            }
            v8::Local<v8::ObjectTemplate> objectTemplate = v8::Local<v8::ObjectTemplate>::New(isolate, JSArrayBufferRef::persistentObjectTemplate);
            v8::Local<v8::Object> pNewIns = objectTemplate->NewInstance();
            pNewIns->SetAlignedPointerInInternalField(0, pArrayBufferRef);
            pArrayBufferRef->persistentObject.Reset(isolate, pNewIns);
            pArrayBufferRef->persistentObject.SetWeak(pArrayBufferRef, JSArrayBufferRef::WeakCallback, v8::WeakCallbackType::kParameter);
            args.GetReturnValue().Set(pNewIns);
            return;
        }
        LOGE("JSLayaGL::createArrayBufferRef type error");
        args.GetReturnValue().Set(v8::Null(isolate));
    }
    bool JSWebGLPlus::updateArrayBufferRef(int nID, bool bSyncToRender, v8::Local<v8::Value> pArrayBuffer)
    {
        char* pBuffer = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = V8Util::extractJSAB(pArrayBuffer, pBuffer, nABLen);
        if (bIsArrayBuffer)
        {
            if (JCWebGLPlus::getInstance()->m_nThreadMODE == THREAD_MODE_DOUBLE)
            {
                if (bSyncToRender)
                {
                    JCWebGLPlus::getInstance()->m_pJSABManagerSyncToRender->updateArrayBuffer(nID, pBuffer, nABLen);
                }
                else
                {
                    JCWebGLPlus::getInstance()->m_pJSArrayBufferManager->updateArrayBuffer(nID, pBuffer, nABLen);
                }
            }
            else
            {
                JCWebGLPlus::getInstance()->m_pJSArrayBufferManager->updateArrayBuffer(nID, pBuffer, nABLen);
            }
            return true;
        }
        LOGE("JSLayaGL::updateArrayBufferRef type error");
        return false;
    }
    static void UpdateArrayBufferRef(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        TEST_WEBGLPLUS_LOG("webglplus UpdateArrayBufferRef");

        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Object> pthis = args.This();
        JSWebGLPlus* self = (JSWebGLPlus*)(pthis->GetAlignedPointerFromInternalField(0));
        int len = args.Length();
        if (len < 3)
        {
            args.GetReturnValue().Set(v8::Boolean::New(isolate, false));
            return;
        }

        assert(args[0]->IsInt32());
        int nID = args[0]->Int32Value();
        assert(args[1]->IsBoolean());
        bool bSyncToRender = args[1]->BooleanValue();
        v8::Local<v8::Value> pArrayBuffer = args[2];
        bool value = self->updateArrayBufferRef(nID, bSyncToRender, pArrayBuffer);
        args.GetReturnValue().Set(v8::Boolean::New(isolate, value));
    }

    static void SyncArrayBufferDataToRuntime(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        TEST_WEBGLPLUS_LOG("webglplus SyncArrayBufferDataToRuntime");

        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Object> pthis = args.This();
        int len = args.Length();
        if (len < 3)
        {
            args.GetReturnValue().Set(v8::Boolean::New(isolate, false));
            return;
        }

        assert(args[0]->IsInt32());
        int nID = args[0]->Int32Value();
        assert(args[1]->IsBoolean());
        bool bSyncToRender = args[1]->BooleanValue();
        v8::Local<v8::Value> pArrayBuffer = args[2];

        char* pBuffer = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = V8Util::extractJSAB(pArrayBuffer, pBuffer, nABLen);
        if (bIsArrayBuffer)
        {
            if (JCWebGLPlus::getInstance()->m_nThreadMODE == THREAD_MODE_DOUBLE)
            {
                if (bSyncToRender)
                {
                    JCWebGLPlus::getInstance()->m_pJSABManagerSyncToRender->syncArrayBufferDataToRuntime(nID, pBuffer, nABLen);
                }
                else
                {
                    JCWebGLPlus::getInstance()->m_pJSArrayBufferManager->syncArrayBufferDataToRuntime(nID, pBuffer, nABLen);
                }
            }
            else
            {
                JCWebGLPlus::getInstance()->m_pJSArrayBufferManager->syncArrayBufferDataToRuntime(nID, pBuffer, nABLen);
            }
            args.GetReturnValue().Set(v8::Boolean::New(isolate, true));
            return;
        }
        LOGE("JSLayaGL::syncArrayBufferDataToRuntime type error");
        args.GetReturnValue().Set(v8::Boolean::New(isolate, false));
    }

    static void EvaluateClipDatasRealTime(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        TEST_WEBGLPLUS_LOG("webglplus EvaluateClipDatasRealTime");

        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Object> pthis = args.This();
        int len = args.Length();
        if (len < 5)
        {
            return;
        }

        v8::Local<v8::Value> nodes = args[0];
        assert(args[1]->IsNumber());
        float playCurTime = args[1]->NumberValue();
        v8::Local<v8::Value> realTimeCurrentFrameIndexs = args[2];
        assert(args[3]->IsBoolean());
        bool addtive = args[3]->BooleanValue();
        assert(args[4]->IsBoolean());
        bool frontPlay = args[4]->BooleanValue();

        char* pBuffer = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = V8Util::extractJSAB(realTimeCurrentFrameIndexs, pBuffer, nABLen);
        if (!bIsArrayBuffer)
        {
            LOGE("evaluateClipDatasRealTime index type error");
            return;
        }
        if (!nodes.IsEmpty() && nodes->IsObject())
        {
            JSKeyframeNodeList* pNodes = (JSKeyframeNodeList*)nodes.As<v8::Object>()->GetAlignedPointerFromInternalField(0);
            if (!pNodes)
            {
                LOGE("evaluateClipDatasRealTime nodes type error");
                return;
            }
            JSKeyframeNodeList::evaluateClipDatasRealTime(pNodes, playCurTime, (short*)pBuffer, nABLen / sizeof(unsigned short), addtive, frontPlay);
        }
    }
   
    static void UniformMatrix2fvEx(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        TEST_WEBGLPLUS_LOG("webglplus UniformMatrix2fvEx");

        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Object> pthis = args.This();
        int len = args.Length();
        if (len < 3)
        {
            return;
        }

        assert(args[0]->IsUint32());
        GLuint location = args[0]->Uint32Value();
        assert(args[1]->IsBoolean());
        GLboolean transpose = args[1]->BooleanValue();
        assert(args[2]->IsInt32());
        int id = args[2]->Int32Value();

        JCWebGLPlus::getInstance()->uniformMatrix2fvEx(location, transpose, id);
    }

    static void UniformMatrix3fvEx(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        TEST_WEBGLPLUS_LOG("webglplus UniformMatrix3fvEx");

        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Object> pthis = args.This();
        int len = args.Length();
        if (len < 3)
        {
            return;
        }

        assert(args[0]->IsUint32());
        GLuint location = args[0]->Uint32Value();
        assert(args[1]->IsBoolean());
        GLboolean transpose = args[1]->BooleanValue();
        assert(args[2]->IsInt32());
        int id = args[2]->Int32Value();

        JCWebGLPlus::getInstance()->uniformMatrix3fvEx(location, transpose, id);
    }

    static void UniformMatrix4fvEx(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        TEST_WEBGLPLUS_LOG("webglplus UniformMatrix4fvEx");

        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Object> pthis = args.This();
        int len = args.Length();
        if (len < 3)
        {
            return;
        }

        assert(args[0]->IsUint32());
        GLuint location = args[0]->Uint32Value();
        assert(args[1]->IsBoolean());
        GLboolean transpose = args[1]->BooleanValue();
        assert(args[2]->IsInt32());
        int id = args[2]->Int32Value();

        JCWebGLPlus::getInstance()->uniformMatrix4fvEx(location, transpose, id);
    }

    static void UploadShaderUniforms(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        TEST_WEBGLPLUS_LOG("webglplus UploadShaderUniforms");

        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Object> pthis = args.This();
        int len = args.Length();
        if (len < 2)
        {
            return;
        }

        assert(args[0]->IsInt32());
        int nCmdID = args[0]->Int32Value();

        assert(args[1]->IsInt32());
        int nDataID = args[1]->Int32Value();

        JCWebGLPlus::getInstance()->uploadShaderUniforms(nCmdID, nDataID);
    }

    static void UploadShaderUniformsBuffer(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        TEST_WEBGLPLUS_LOG("webglplus UploadShaderUniformsBuffer");

        v8::Local<v8::Object> pthis = args.This();
        int len = args.Length();
        if (len < 2)
        {
            return;
        }

        assert(args[0]->IsInt32());
        int nCmdID = args[0]->Int32Value();

        char* pArrayBuffer = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = V8Util::extractJSAB(args[1], pArrayBuffer, nABLen);
        if (bIsArrayBuffer)
        {
            JCWebGLPlus::getInstance()->uploadShaderUniformsBuffer(nCmdID, pArrayBuffer, nABLen);
        }
    }

    void JSWebGLPlus::exportJS(v8::Local<v8::Object> object)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::HandleScope sc(isolate);
        v8::Local<v8::FunctionTemplate> functionTemplate = v8::FunctionTemplate::New(isolate);
        functionTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "WebGLPlus"));
        v8::Local<v8::ObjectTemplate> instancTemp = functionTemplate->InstanceTemplate();
        v8::Local<v8::Context> pCtx = isolate->GetCurrentContext();

        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "uploadShaderUniforms", UploadShaderUniforms);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "uploadShaderUniformsBuffer", UploadShaderUniformsBuffer);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "uniformMatrix2fvEx", UniformMatrix2fvEx);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "uniformMatrix3fvEx", UniformMatrix3fvEx);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "uniformMatrix4fvEx", UniformMatrix4fvEx);

        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "createArrayBufferRef", CreateArrayBufferRef);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "updateArrayBufferRef", UpdateArrayBufferRef);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "syncArrayBufferDataToRuntime", SyncArrayBufferDataToRuntime);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "evaluateClipDatasRealTime", EvaluateClipDatasRealTime);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "updateAnimationNodeWorldMatix", UpdateAnimationNodeWorldMatix);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "computeSubSkinnedData", ComputeSubSkinnedData);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "culling", Culling);

        v8::EscapableHandleScope handle_scope(isolate);
        v8::Handle<v8::ObjectTemplate> pTempl = v8::Local<v8::ObjectTemplate>::New(isolate, instancTemp);
        v8::Handle<v8::Object> pNewIns = pTempl->NewInstance();

        persistentObject.Reset(isolate, pNewIns);
 

        object->Set(v8::String::NewFromUtf8(isolate, "webglPlus"), pNewIns);
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
