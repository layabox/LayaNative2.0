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
#include "JSCUtil.h"

//------------------------------------------------------------------------------
namespace laya 
{
    JSWebGLPlus* JSWebGLPlus::s_pWebGLPlus = NULL;
    //ADDJSCLSINFO(JSWebGLPlus, JSObjNode);
    JSWebGLPlus::JSWebGLPlus()
    {
        if (JSObjNode::s_pListJSObj)
        {
            JSObjNode::s_pListJSObj->push_back(this);
        }
        //AdjustAmountOfExternalAllocatedMemory(8192);
        //JCMemorySurvey::GetInstance()->newClass("webglPlus", 8192, this);
    }
    JSWebGLPlus::~JSWebGLPlus()
    {
        if (JSObjNode::s_pListJSObj)
        {
            JSObjNode::s_pListJSObj->delNode(this);
        }

        //JCMemorySurvey::GetInstance()->releaseClass("webglPlus", this);
        s_pWebGLPlus = NULL;
    }
    JSWebGLPlus* JSWebGLPlus::getInstance()
    {
        if (s_pWebGLPlus == NULL)
        {
            s_pWebGLPlus = new JSWebGLPlus();
        }
        return s_pWebGLPlus;
    }
    
    static JSValueRef Culling(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        TEST_WEBGLPLUS_LOG("webglplus Culling");
        if (argumentCount < 4)
        {
            return JSValueMakeNumber(ctx, 0);
        }

        JSValueRef boundFrustumBuffer = arguments[0];
        JSValueRef cullingBuffer = arguments[1];
        JSValueRef cullingBufferIndices = arguments[2];
        assert(JSValueIsNumber(ctx, arguments[3]));
        int cullingCount = JSValueToNumber(ctx, arguments[3], 0);
        JSValueRef cullingBufferResult = arguments[4];

        char* pFrustumBuffer;
        char* pCullingBuffer;
        char* pCullingBufferIndices;
        char* pCullingBufferResult;
        int nFrustumLen = 0;
        int nCullingBufferLen = 0;
        int nCullingBufferIndicesLen = 0;
        int nCullingBufferResultLen = 0;

        if (JSCUtil::extractJSAB(ctx, boundFrustumBuffer, pFrustumBuffer, nFrustumLen) == false)
        {
            LOGE("culling culling frustum error");
            return JSValueMakeNumber(ctx, 0);
        }
        if (JSCUtil::extractJSAB(ctx, cullingBuffer, pCullingBuffer, nCullingBufferLen) == false)
        {
            LOGE("culling culling buffer error");
            return JSValueMakeNumber(ctx, 0);
        }
        if (JSCUtil::extractJSAB(ctx, cullingBufferIndices, pCullingBufferIndices, nCullingBufferIndicesLen) == false)
        {
            LOGE("culling culling buffer indices error");
            return JSValueMakeNumber(ctx, 0);
        }
        if (JSCUtil::extractJSAB(ctx, cullingBufferResult, pCullingBufferResult, nCullingBufferResultLen) == false)
        {
            LOGE("culling result error");
            return JSValueMakeNumber(ctx, 0);
        }
        int count = JCWebGLPlus::getInstance()->culling3D(cullingCount, pFrustumBuffer, nFrustumLen, pCullingBuffer, nCullingBufferLen, pCullingBufferIndices, nCullingBufferIndicesLen, pCullingBufferResult, nCullingBufferResultLen);
        return JSValueMakeNumber(ctx, count);
    }
    static JSValueRef UpdateAnimationNodeWorldMatix(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        TEST_WEBGLPLUS_LOG("webglplus UpdateAnimationNodeWorldMatix");
        if (argumentCount < 5)
        {
            return JSValueMakeBoolean(ctx, false);
        }


        JSValueRef locPosition = arguments[0];
        JSValueRef locRotation = arguments[1];
        JSValueRef locScaling = arguments[2];
        JSValueRef parentIndices = arguments[3];
        JSValueRef outWorldMatrix = arguments[4];


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
        if (JSCUtil::extractJSAB(ctx, locPosition, pLocPosition, nLocPosLen) == false)
        {
            LOGE("updateAnimationNodeWorldMatix postion error");
            return JSValueMakeBoolean(ctx, false);
        }
        if (JSCUtil::extractJSAB(ctx, locRotation, pLocRotation, nLocRotLen) == false)
        {
            LOGE("updateAnimationNodeWorldMatix rotateion error");
            return JSValueMakeBoolean(ctx, false);
        }
        if (JSCUtil::extractJSAB(ctx, locScaling, pLocScaling, nLocScaLen) == false)
        {
            LOGE("updateAnimationNodeWorldMatix scaling error");
            return JSValueMakeBoolean(ctx, false);
        }
        if (JSCUtil::extractJSAB(ctx, parentIndices, pParentIndices, nLocParentLen) == false)
        {
            LOGE("updateAnimationNodeWorldMatix parent index error");
            return JSValueMakeBoolean(ctx, false);
        }
        if (JSCUtil::extractJSAB(ctx, outWorldMatrix, pOutWorldMatrix, nOutWorldLen) == false)
        {
            LOGE("updateAnimationNodeWorldMatix world marix error");
            return JSValueMakeBoolean(ctx, false);
        }
        bool value = JCWebGLPlus::getInstance()->updateAnimationNodeWorldMatix3D(pLocPosition, nLocPosLen, pLocRotation, nLocRotLen, pLocScaling, nLocScaLen,
            pParentIndices, nLocParentLen, pOutWorldMatrix, nOutWorldLen);
          return JSValueMakeBoolean(ctx, value);
    }
    static JSValueRef ComputeSubSkinnedData(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        TEST_WEBGLPLUS_LOG("webglplus ComputeSubSkinnedData");
        if (argumentCount < 6)
        {
            return JSValueMakeBoolean(ctx, false);
        }

        JSValueRef worldMatrixs = arguments[0];
        JSValueRef worldMatrixIndex = arguments[1];
        JSValueRef inverseBindPoses = arguments[2];
        JSValueRef boneIndices = arguments[3];
        JSValueRef bindPoseInices = arguments[4];
        JSValueRef resultData = arguments[5];

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
        if (JSCUtil::extractJSAB(ctx, worldMatrixs, pWorldMatrixs, nWorldMatLen) == false)
        {
            LOGE("computeSubSkinnedDataNative world matrix error");
            return JSValueMakeBoolean(ctx, false);
        }
        if (JSCUtil::extractJSAB(ctx, worldMatrixIndex, pWorldMatrixIndex, nWorldMatIndeLen) == false)
        {
            LOGE("computeSubSkinnedDataNative world matrix index error");
            return JSValueMakeBoolean(ctx, false);
        }
        if (JSCUtil::extractJSAB(ctx, inverseBindPoses, pInverseBindPoses, nInverseBindPosesLen) == false)
        {
            LOGE("computeSubSkinnedDataNative inverse bind poses error");
            return JSValueMakeBoolean(ctx, false);
        }
        if (JSCUtil::extractJSAB(ctx, boneIndices, pBoneIndices, nBoneIndicesLen) == false)
        {
            LOGE("computeSubSkinnedDataNative bone indices error");
            return JSValueMakeBoolean(ctx, false);
        }
        if (JSCUtil::extractJSAB(ctx, bindPoseInices, pBindPoseIndices, nBindPoseIndicesLen) == false)
        {
            LOGE("computeSubSkinnedDataNative bind pose indices error");
            return JSValueMakeBoolean(ctx, false);
        }
        if (JSCUtil::extractJSAB(ctx, resultData, pResultData, nResultDataLen) == false)
        {
            LOGE("computeSubSkinnedDataNative data error");
            return JSValueMakeBoolean(ctx, false);
        }
        bool value = JCWebGLPlus::getInstance()->computeSubSkinnedData3D(pWorldMatrixs, nWorldMatLen, pWorldMatrixIndex, nWorldMatIndeLen,
            pInverseBindPoses, nInverseBindPosesLen, pBoneIndices, nBoneIndicesLen,
            pBindPoseIndices, nBindPoseIndicesLen, pResultData, nResultDataLen);

        return JSValueMakeBoolean(ctx, value);
    }
    static JSValueRef CreateArrayBufferRef(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        TEST_WEBGLPLUS_LOG("webglplus CreateArrayBufferRef");
        if (argumentCount < 4)
        {
            return JSValueMakeNull(ctx);
        }

        JSValueRef pArrayBuffer = arguments[0];
        assert(JSValueIsNumber(ctx, arguments[1]));
        int nType = JSValueToNumber(ctx, arguments[1], 0);
        assert(JSValueIsBoolean(ctx, arguments[2]));
        bool bSyncToRender = JSValueToBoolean(ctx, arguments[2]);
        assert(JSValueIsNumber(ctx, arguments[3]));
        int nRefType = JSValueToNumber(ctx, arguments[3], 0);
       

        char* pBuffer = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = JSCUtil::extractJSAB(ctx, pArrayBuffer, pBuffer, nABLen);
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
            return JSCBinder<JSArrayBufferRef>::GetInstance()->transferObjPtrToJS(ctx, pArrayBufferRef);
        }
        LOGE("JSLayaGL::createArrayBufferRef type error");
        return JSValueMakeNull(ctx);
    }
    bool JSWebGLPlus::updateArrayBufferRef(int nID, bool bSyncToRender, JSValueRef pArrayBuffer)
    {
        char* pBuffer = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = JSCUtil::extractJSAB(JSCUtil::s_ctx, pArrayBuffer, pBuffer, nABLen);
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
    static JSValueRef UpdateArrayBufferRef(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        TEST_WEBGLPLUS_LOG("webglplus UpdateArrayBufferRef");
        JSWebGLPlus* self = (JSWebGLPlus*)JSObjectGetPrivate(thisObject);
        if (argumentCount < 3)
        {
            return JSValueMakeBoolean(ctx, false);
        }

        assert(JSValueIsNumber(ctx, arguments[0]));
        int nID = JSValueToNumber(ctx, arguments[0], 0);
        assert(JSValueIsBoolean(ctx, arguments[1]));
        bool bSyncToRender = JSValueToBoolean(ctx, arguments[1]);
        JSValueRef pArrayBuffer = arguments[2];

        bool value = self->updateArrayBufferRef(nID, bSyncToRender, pArrayBuffer);
        return JSValueMakeBoolean(ctx, value);
    }
    static JSValueRef SyncArrayBufferDataToRuntime(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        TEST_WEBGLPLUS_LOG("webglplus SyncArrayBufferDataToRuntime");
        if (argumentCount < 3)
        {
            return JSValueMakeBoolean(ctx, false);
        }

        assert(JSValueIsNumber(ctx, arguments[0]));
        int nID = JSValueToNumber(ctx, arguments[0], 0);
        assert(JSValueIsBoolean(ctx, arguments[1]));
        bool bSyncToRender = JSValueToBoolean(ctx, arguments[1]);
        JSValueRef pArrayBuffer = arguments[2];

        char* pBuffer = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = JSCUtil::extractJSAB(ctx, pArrayBuffer, pBuffer, nABLen);
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
            return JSValueMakeBoolean(ctx, true);
        }
        LOGE("JSLayaGL::syncArrayBufferDataToRuntime type error");
        return JSValueMakeBoolean(ctx, false);
    }
    static JSValueRef EvaluateClipDatasRealTime(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        TEST_WEBGLPLUS_LOG("webglplus EvaluateClipDatasRealTime");
        if (argumentCount < 5)
        {
            return JSValueMakeUndefined(ctx);
        }

        JSValueRef nodes = arguments[0];
        assert(JSValueIsNumber(ctx, arguments[1]));
        float playCurTime = JSValueToNumber(ctx, arguments[1], 0);
        JSValueRef realTimeCurrentFrameIndexs = arguments[2];
        assert(JSValueIsBoolean(ctx, arguments[3]));
        bool addtive = JSValueToBoolean(ctx, arguments[3]);
        assert(JSValueIsBoolean(ctx, arguments[4]));
        bool frontPlay = JSValueToBoolean(ctx, arguments[4]);

        char* pBuffer = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = JSCUtil::extractJSAB(ctx, realTimeCurrentFrameIndexs, pBuffer, nABLen);
        if (!bIsArrayBuffer)
        {
            LOGE("evaluateClipDatasRealTime index type error");
            return JSValueMakeUndefined(ctx);
        }
        
        if (JSValueIsObject(ctx, nodes))
        {
            JSKeyframeNodeList* pNodes = (JSKeyframeNodeList*)JSObjectGetPrivate(JSValueToObject(ctx, nodes, NULL));
            if (!pNodes)
            {
                LOGE("evaluateClipDatasRealTime nodes type error");
                return JSValueMakeUndefined(ctx);
            }
            JSKeyframeNodeList::evaluateClipDatasRealTime(pNodes, playCurTime, (short*)pBuffer, nABLen / sizeof(unsigned short), addtive, frontPlay);
        }
        return JSValueMakeUndefined(ctx);
    }
   
    static JSValueRef UniformMatrix2fvEx(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        TEST_WEBGLPLUS_LOG("webglplus UniformMatrix2fvEx");
        if (argumentCount < 3)
        {
            return JSValueMakeUndefined(ctx);
        }
        
        assert(JSValueIsNumber(ctx, arguments[0]));
        GLuint location = JSValueToNumber(ctx, arguments[0], 0);
        assert(JSValueIsBoolean(ctx, arguments[1]));
        GLboolean transpose = JSValueToBoolean(ctx, arguments[1]);
        assert(JSValueIsNumber(ctx, arguments[2]));
        int id = JSValueToNumber(ctx, arguments[2], 0);
        
        JCWebGLPlus::getInstance()->uniformMatrix2fvEx(location, transpose, id);
        return JSValueMakeUndefined(ctx);
    }
    
    static JSValueRef UniformMatrix3fvEx(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        TEST_WEBGLPLUS_LOG("webglplus UniformMatrix3fvEx");
        if (argumentCount < 3)
        {
            return JSValueMakeUndefined(ctx);
        }
        
        assert(JSValueIsNumber(ctx, arguments[0]));
        GLuint location = JSValueToNumber(ctx, arguments[0], 0);
        assert(JSValueIsBoolean(ctx, arguments[1]));
        GLboolean transpose = JSValueToBoolean(ctx, arguments[1]);
        assert(JSValueIsNumber(ctx, arguments[2]));
        int id = JSValueToNumber(ctx, arguments[2], 0);
        
        JCWebGLPlus::getInstance()->uniformMatrix3fvEx(location, transpose, id);
        return JSValueMakeUndefined(ctx);
    }
    static JSValueRef UniformMatrix4fvEx(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        TEST_WEBGLPLUS_LOG("webglplus UniformMatrix4fvEx");
        if (argumentCount < 3)
        {
            return JSValueMakeUndefined(ctx);
        }
        
        assert(JSValueIsNumber(ctx, arguments[0]));
        GLuint location = JSValueToNumber(ctx, arguments[0], 0);
        assert(JSValueIsBoolean(ctx, arguments[1]));
        GLboolean transpose = JSValueToBoolean(ctx, arguments[1]);
        assert(JSValueIsNumber(ctx, arguments[2]));
        int id = JSValueToNumber(ctx, arguments[2], 0);
        
        JCWebGLPlus::getInstance()->uniformMatrix4fvEx(location, transpose, id);
        return JSValueMakeUndefined(ctx);
    }
    
    static JSValueRef UploadShaderUniforms(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        TEST_WEBGLPLUS_LOG("webglplus UploadShaderUniforms");
        if (argumentCount < 2)
        {
            return JSValueMakeUndefined(ctx);
        }
        
        assert(JSValueIsNumber(ctx, arguments[0]));
        int nCmdID = JSValueToNumber(ctx, arguments[0], 0);
        
        assert(JSValueIsNumber(ctx, arguments[1]));
        int nDataID = JSValueToNumber(ctx, arguments[1], 0);
        
        JCWebGLPlus::getInstance()->uploadShaderUniforms(nCmdID, nDataID);
        return JSValueMakeUndefined(ctx);
    }
    static JSValueRef UploadShaderUniformsBuffer(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        TEST_WEBGLPLUS_LOG("webglplus UploadShaderUniformsBuffer");
        if (argumentCount < 2)
        {
            return JSValueMakeUndefined(ctx);
        }
        
        assert(JSValueIsNumber(ctx, arguments[0]));
        int nCmdID = JSValueToNumber(ctx, arguments[0], 0);
        
        char* pArrayBuffer = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = JSCUtil::extractJSAB(ctx, arguments[1], pArrayBuffer, nABLen);
        if (bIsArrayBuffer)
        {
            JCWebGLPlus::getInstance()->uploadShaderUniformsBuffer(nCmdID, pArrayBuffer, nABLen);
        }
        return JSValueMakeUndefined(ctx);
    }

    void JSWebGLPlus::exportJS(JSContextRef ctx, JSObjectRef object)
    {
        JSCBinder<JSWebGLPlus>* binder = JSCBinder<JSWebGLPlus>::GetInstance();
        binder->begin(ctx);
        binder->addMethod("uploadShaderUniforms", UploadShaderUniforms);
        binder->addMethod("uploadShaderUniformsBuffer", UploadShaderUniformsBuffer);
        binder->addMethod("uniformMatrix2fvEx", UniformMatrix2fvEx);
        binder->addMethod("uniformMatrix3fvEx", UniformMatrix3fvEx);
        binder->addMethod("uniformMatrix4fvEx", UniformMatrix4fvEx);
        binder->addMethod("createArrayBufferRef", CreateArrayBufferRef);
        binder->addMethod("updateArrayBufferRef", UpdateArrayBufferRef);
        binder->addMethod("syncArrayBufferDataToRuntime", SyncArrayBufferDataToRuntime);
        binder->addMethod("evaluateClipDatasRealTime", EvaluateClipDatasRealTime);
        binder->addMethod("updateAnimationNodeWorldMatix", UpdateAnimationNodeWorldMatix);
        binder->addMethod("computeSubSkinnedData", ComputeSubSkinnedData);
        binder->addMethod("culling", Culling);
        binder->endToGlobal(object, "webglPlus", JSWebGLPlus::getInstance());
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
