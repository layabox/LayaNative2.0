/**
@file			JSWebGLPlus.cpp
@brief			
@author			James
@version		1.0
@date			2019_8_24
*/

#include "JSWebGLPlus.h"
#include <webglplus/JCWebGLPlus.h>
#include "JSArrayBufferRef.h"
//#include "Animation/JSKeyframeNodeList.h"

//------------------------------------------------------------------------------
namespace laya 
{
    JSWebGLPlus* JSWebGLPlus::s_pWebGLPlus = NULL;
    ADDJSCLSINFO(JSWebGLPlus, JSObjNode);
    JSWebGLPlus::JSWebGLPlus()
    {
        AdjustAmountOfExternalAllocatedMemory(8192);
        JCMemorySurvey::GetInstance()->newClass("webglPlus", 8192, this);
    }
    JSWebGLPlus::~JSWebGLPlus()
    {
        JCMemorySurvey::GetInstance()->releaseClass("webglPlus", this);
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
    /*bool JSWebGLPlus::updateAnimationNodeWorldMatix(JSValueAsParam locPosition, JSValueAsParam locRotation, JSValueAsParam locScaling,
        JSValueAsParam parentIndices, JSValueAsParam outWorldMatrix)
    {
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
        if (extractJSAB(locPosition, pLocPosition, nLocPosLen) == false)
        {
            LOGE("updateAnimationNodeWorldMatix postion error");
            return false;
        }
        if (extractJSAB(locRotation, pLocRotation, nLocRotLen) == false)
        {
            LOGE("updateAnimationNodeWorldMatix rotateion error");
            return false;
        }
        if (extractJSAB(locScaling, pLocScaling, nLocScaLen) == false)
        {
            LOGE("updateAnimationNodeWorldMatix scaling error");
            return false;
        }
        if (extractJSAB(parentIndices, pParentIndices, nLocParentLen) == false)
        {
            LOGE("updateAnimationNodeWorldMatix parent index error");
            return false;
        }
        if (extractJSAB(outWorldMatrix, pOutWorldMatrix, nOutWorldLen) == false)
        {
            LOGE("updateAnimationNodeWorldMatix world marix error");
            return false;
        }
        return JCWebGLPlus::getInstance()->updateAnimationNodeWorldMatix3D(pLocPosition, nLocPosLen, pLocRotation, nLocRotLen, pLocScaling, nLocScaLen,
            pParentIndices, nLocParentLen, pOutWorldMatrix, nOutWorldLen);
    }
    bool JSWebGLPlus::computeSubSkinnedData(JSValueAsParam worldMatrixs, JSValueAsParam worldMatrixIndex, JSValueAsParam inverseBindPoses,
        JSValueAsParam boneIndices, JSValueAsParam bindPoseInices, JSValueAsParam resultData)
    {
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
        if (extractJSAB(worldMatrixs, pWorldMatrixs, nWorldMatLen) == false)
        {
            LOGE("computeSubSkinnedDataNative world matrix error");
            return false;
        }
        if (extractJSAB(worldMatrixIndex, pWorldMatrixIndex, nWorldMatIndeLen) == false)
        {
            LOGE("computeSubSkinnedDataNative world matrix index error");
            return false;
        }
        if (extractJSAB(inverseBindPoses, pInverseBindPoses, nInverseBindPosesLen) == false)
        {
            LOGE("computeSubSkinnedDataNative inverse bind poses error");
            return false;
        }
        if (extractJSAB(boneIndices, pBoneIndices, nBoneIndicesLen) == false)
        {
            LOGE("computeSubSkinnedDataNative bone indices error");
            return false;
        }
        if (extractJSAB(bindPoseInices, pBindPoseIndices, nBindPoseIndicesLen) == false)
        {
            LOGE("computeSubSkinnedDataNative bind pose indices error");
            return false;
        }
        if (extractJSAB(resultData, pResultData, nResultDataLen) == false)
        {
            LOGE("computeSubSkinnedDataNative data error");
            return false;
        }
        return JCWebGLPlus::getInstance()->computeSubSkinnedData3D(pWorldMatrixs, nWorldMatLen, pWorldMatrixIndex, nWorldMatIndeLen,
            pInverseBindPoses, nInverseBindPosesLen, pBoneIndices, nBoneIndicesLen,
            pBindPoseIndices, nBindPoseIndicesLen, pResultData, nResultDataLen);
    }
    int JSWebGLPlus::culling(JSValueAsParam boundFrustumBuffer, JSValueAsParam cullingBuffer, JSValueAsParam cullingBufferIndices, int cullingCount, JSValueAsParam cullingBufferResult)
    {
        char* pFrustumBuffer;
        char* pCullingBuffer;
        char* pCullingBufferIndices;
        char* pCullingBufferResult;
        int nFrustumLen = 0;
        int nCullingBufferLen = 0;
        int nCullingBufferIndicesLen = 0;
        int nCullingBufferResultLen = 0;
        if (extractJSAB(boundFrustumBuffer, pFrustumBuffer, nFrustumLen) == false)
        {
            LOGE("culling culling frustum error");
            return false;
        }
        if (extractJSAB(cullingBuffer, pCullingBuffer, nCullingBufferLen) == false)
        {
            LOGE("culling culling buffer error");
            return false;
        }
        if (extractJSAB(cullingBufferIndices, pCullingBufferIndices, nCullingBufferIndicesLen) == false)
        {
            LOGE("culling culling buffer indices error");
            return false;
        }
        if (extractJSAB(cullingBufferResult, pCullingBufferResult, nCullingBufferResultLen) == false)
        {
            LOGE("culling result error");
            return false;
        }
        return JCWebGLPlus::getInstance()->culling3D(cullingCount, pFrustumBuffer, nFrustumLen, pCullingBuffer, nCullingBufferLen, pCullingBufferIndices, nCullingBufferIndicesLen, pCullingBufferResult, nCullingBufferResultLen);
    }*/
    JsValue JSWebGLPlus::createArrayBufferRef(JSValueAsParam pArrayBuffer, int nType, bool bSyncToRender, int nRefType)
    {
        char* pBuffer = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = extractJSAB(pArrayBuffer, pBuffer, nABLen);
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
            return JSP_TO_JS(JSArrayBufferRef*, pArrayBufferRef);
        }
        LOGE("JSLayaGL::createArrayBufferRef type error");
        return JSP_TO_JS(JSArrayBufferRef*, NULL);
    }
    bool JSWebGLPlus::updateArrayBufferRef(int nID, bool bSyncToRender, JSValueAsParam pArrayBuffer)
    {
        char* pBuffer = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = extractJSAB(pArrayBuffer, pBuffer, nABLen);
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
    bool JSWebGLPlus::syncArrayBufferDataToRuntime(int nID, bool bSyncToRender, JSValueAsParam pArrayBuffer)
    {
        char* pBuffer = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = extractJSAB(pArrayBuffer, pBuffer, nABLen);
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
            return true;
        }
        LOGE("JSLayaGL::syncArrayBufferDataToRuntime type error");
        return false;
    }
    /*void JSWebGLPlus::evaluateClipDatasRealTime(JSValueAsParam nodes, float playCurTime, JSValueAsParam realTimeCurrentFrameIndexs, bool addtive, bool frontPlay)
    {
        char* pBuffer = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = extractJSAB(realTimeCurrentFrameIndexs, pBuffer, nABLen);
        if (!bIsArrayBuffer)
        {
            LOGE("evaluateClipDatasRealTime index type error");
            return;
        }
        JSKeyframeNodeList* pNodes;
        JsObjHandle::tryGet<JSKeyframeNodeList>(nodes, &pNodes);
        if (!pNodes)
        {
            LOGE("evaluateClipDatasRealTime nodes type error");
            return;
        }
        JSKeyframeNodeList::evaluateClipDatasRealTime(pNodes, playCurTime, (short*)pBuffer, nABLen / sizeof(unsigned short), addtive, frontPlay);
    }*/
    void JSWebGLPlus::exportJS()
    {
        JSP_GLOBAL_CLASS("webglPlus", JSWebGLPlus, this);
        JSP_GLOBAL_ADD_METHOD("createArrayBufferRef", JSWebGLPlus::createArrayBufferRef);
		JSP_GLOBAL_ADD_METHOD("updateArrayBufferRef", JSWebGLPlus::updateArrayBufferRef);
		JSP_GLOBAL_ADD_METHOD("syncArrayBufferDataToRuntime", JSWebGLPlus::syncArrayBufferDataToRuntime);
        //JSP_ADD_METHOD("evaluateClipDatasRealTime", JSWebGLPlus::evaluateClipDatasRealTime);
        //JSP_ADD_METHOD("updateAnimationNodeWorldMatix", JSWebGLPlus::updateAnimationNodeWorldMatix);
        //JSP_ADD_METHOD("computeSubSkinnedData", JSWebGLPlus::computeSubSkinnedData);
        //JSP_ADD_METHOD("culling", JSWebGLPlus::culling);
        JSP_INSTALL_GLOBAL_CLASS("webglPlus", JSWebGLPlus, this);
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
