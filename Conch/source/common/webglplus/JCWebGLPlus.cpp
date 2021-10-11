/**
@file			JCWebGLPlus.cpp
@brief			
@author			James
@version		1.0
@date			2019_8_24
*/

#include "JCWebGLPlus.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../math/JCBoundingFrustum.h"
#include "../math/JCBoundingBox.h"
#include "../math/JCBoundingSphere.h"
#include "../util/Log.h"
#include "../util/JCCommonMethod.h"
namespace laya
{
    JCWebGLPlus* JCWebGLPlus::s_pWebGLPlus = NULL;
    JCWebGLPlus::JCWebGLPlus()
    {
        m_pJSArrayBufferManager = NULL;
        m_pJSABManagerSyncToRender = NULL;
        m_pRArrayBufferManager = NULL;
        m_nThreadMODE = THREAD_MODE_DOUBLE;
        s_pWebGLPlus = this;
    }
    JCWebGLPlus::~JCWebGLPlus()
    {
        s_pWebGLPlus = NULL;

        if (m_pJSArrayBufferManager)
        {
            delete m_pJSArrayBufferManager;
            m_pJSArrayBufferManager = NULL;
        }
        if (m_pJSABManagerSyncToRender)
        {
            delete m_pJSABManagerSyncToRender;
            m_pJSABManagerSyncToRender = NULL;
        }
        if (m_nThreadMODE == THREAD_MODE_DOUBLE)
        {
            if (m_pRArrayBufferManager)
            {
                delete m_pRArrayBufferManager;
                m_pRArrayBufferManager = NULL;
            }
        }
        else
        {
            m_pRArrayBufferManager = NULL;
        }
    }
    JCWebGLPlus* JCWebGLPlus::getInstance()
    {
        return s_pWebGLPlus ? s_pWebGLPlus : new JCWebGLPlus();
    }
    void JCWebGLPlus::releaseInstance()
    {
        if (s_pWebGLPlus)
        {
            delete s_pWebGLPlus;
            s_pWebGLPlus = NULL;
        }
    }
    void JCWebGLPlus::init(int nThreadMode)
    {
        if (m_pJSArrayBufferManager == NULL)
        {
            m_nThreadMODE = nThreadMode;
            m_pJSArrayBufferManager = new JCArrayBufferManager();
            m_pJSABManagerSyncToRender = new JCArrayBufferManager();

            if (nThreadMode == THREAD_MODE_SINGLE)
            {
                m_pRArrayBufferManager = m_pJSArrayBufferManager;
            }
            else if(nThreadMode == THREAD_MODE_DOUBLE)
            {
                m_pRArrayBufferManager = new JCArrayBufferManager();
            }
            else
            {
                LOGE("JCWebGLPlus::init thread mode error");
                m_nThreadMODE = THREAD_MODE_DOUBLE;
                m_pRArrayBufferManager = new JCArrayBufferManager();
            }
        }
    }
    void JCWebGLPlus::matrix4x4MultiplyFFF(float* a, float* b, float* e)
    {
        int i = 0;
        float ai0, ai1, ai2, ai3;
        float b0 = b[0], b1 = b[1], b2 = b[2], b3 = b[3];
        float b4 = b[4], b5 = b[5], b6 = b[6], b7 = b[7];
        float b8 = b[8], b9 = b[9], b10 = b[10], b11 = b[11];
        float b12 = b[12], b13 = b[13], b14 = b[14], b15 = b[15];
        for (i = 0; i < 4; i++)
        {
            ai0 = a[i];
            ai1 = a[i + 4];
            ai2 = a[i + 8];
            ai3 = a[i + 12];
            e[i] = ai0 * b0 + ai1 * b1 + ai2 * b2 + ai3 * b3;
            e[i + 4] = ai0 * b4 + ai1 * b5 + ai2 * b6 + ai3 * b7;
            e[i + 8] = ai0 * b8 + ai1 * b9 + ai2 * b10 + ai3 * b11;
            e[i + 12] = ai0 * b12 + ai1 * b13 + ai2 * b14 + ai3 * b15;
        }
    }
    void JCWebGLPlus::calcMatrix(float* pos, float* scale, float* quat, float* outE) {
        float x = quat[0], y = quat[1], z = quat[2], w = quat[3],
            x2 = x + x,
            y2 = y + y,
            z2 = z + z;
        float xx = x * x2,
            xy = x * y2,
            xz = x * z2,
            yy = y * y2,
            yz = y * z2,
            zz = z * z2;

        float wx = w * x2,
            wy = w * y2,
            wz = w * z2,
            sx = scale[0],
            sy = scale[1],
            sz = scale[2];

        outE[0] = (1 - (yy + zz)) * sx;
        outE[1] = (xy + wz) * sx;
        outE[2] = (xz - wy) * sx;
        outE[3] = 0;
        outE[4] = (xy - wz) * sy;
        outE[5] = (1 - (xx + zz)) * sy;
        outE[6] = (yz + wx) * sy;
        outE[7] = 0;
        outE[8] = (xz + wy) * sz;
        outE[9] = (yz - wx) * sz;
        outE[10] = (1 - (xx + yy)) * sz;
        outE[11] = 0;
        outE[12] = pos[0];
        outE[13] = pos[1];
        outE[14] = pos[2];
        outE[15] = 1;
    }

    int JCWebGLPlus::culling3D(int cullingCount, char* pFrustumBuffer, int nFrustumLen, char* pCullingBuffer, int nCullingBufferLen,
        char* pCullingBufferIndices, int nCullingBufferIndicesLen, char* pCullingBufferResult, int nCullingBufferResultLen)
    {
        float* pF32FrustumBuffer = (float*)pFrustumBuffer;
        float* pF32CullingBuffer = (float*)pCullingBuffer;
        int* pI32CullingBufferIndices = (int*)pCullingBufferIndices;
        int* pI32CullingBufferResult = (int*)pCullingBufferResult;
        int nResultCount = cullingCount;
        JCBoundingFrustum kFrustum;
        kFrustum.setValues(pF32FrustumBuffer);
        JCBoundingBox kBox;
        JCBoundingSphere kSphere;
        for (int i = 0; i < cullingCount; i++)
        {
            int nIndex = pI32CullingBufferIndices[i];
            int nType = (int)(pF32CullingBuffer[nIndex]);
            bool bSucess = true;
            switch (nType)
            {
            case 0:
                bSucess = true;
                break;
            case 1:
                kSphere.setValues(pF32CullingBuffer + nIndex + 1);
                bSucess = kFrustum.containsBoundingSphere(kSphere) != CT_DISJOINT;
                break;
            case 2:
                kBox.setValues(pF32CullingBuffer + nIndex + 1);
                //bSucess = kFrustum.containsAxisAlignedBouningBox(kBox) != CT_DISJOINT;
                bSucess = kFrustum.intersects(kBox);
                break;
            default:
                break;
            }
            if (bSucess)
            {
                pI32CullingBufferResult[i] = 1;
            }
            else
            {
                pI32CullingBufferResult[i] = 0;
            }
        }
        return nResultCount;
    }
    bool JCWebGLPlus::updateAnimationNodeWorldMatix3D(char* pLocPosition, int nLocPosLen, char* pLocRotation, int nLocRotLen, char* pLocScaling, int nLocScaLen,
        char* pParentIndices, int nLocParentLen, char* pOutWorldMatrix, int nOutWorldLen)
    {
        float* pfLocPosition = (float*)pLocPosition;    //vec3
        float* pfLocRotation = (float*)pLocRotation;    //
        float* pfLocScaling = (float*)pLocScaling;
        signed short* pfParentIndices = (signed short*)pParentIndices;
        float* pfOutWorldMatrix = (float*)pOutWorldMatrix;

        int boneNum = nLocParentLen / sizeof(signed short);
        int matOutNum = nOutWorldLen / (sizeof(float) * 16);
        if (boneNum > matOutNum)
        {
            //错误了
            LOGE("输出矩阵空间太小，结果会不正确！");
            boneNum = matOutNum;
        }
        float* pCurPos = pfLocPosition;
        float* pCurScale = pfLocScaling;
        float* pCurQuat = pfLocRotation;
        float* pCurMat = pfOutWorldMatrix;
        signed short* pCurPar = pfParentIndices;
        float tmpMat[16];
        for (int i = 0; i < boneNum; i++) {
            calcMatrix(pCurPos, pCurScale, pCurQuat, pCurMat);
            int par = *(pCurPar++);
            if (par >= 0) {
                if (par >= boneNum) {
                    LOGE("boneParent too big!");
                }
                else {
                    float* pParMat = pfOutWorldMatrix + par * 16;
                    matrix4x4MultiplyFFF(pParMat, pCurMat, tmpMat);
                    memcpy(pCurMat, tmpMat, 16 * sizeof(float));//用一个一个赋值吗
                }
            }
            else {
                pCurMat[1] = pCurMat[2] = pCurMat[3] = pCurMat[4] = pCurMat[6] = pCurMat[7] = pCurMat[8] = pCurMat[9] = pCurMat[11] = pCurMat[12] = pCurMat[13] = pCurMat[14] = 0;
                pCurMat[0] = pCurMat[5] = pCurMat[10] = pCurMat[15] = 1;
            }
            pCurPos += 3;
            pCurScale += 3;
            pCurQuat += 4;
            pCurMat += 16;
        }
        return true;
    }
    bool JCWebGLPlus::computeSubSkinnedData3D(char* pWorldMatrixs, int nWorldMatLen, char* pWorldMatrixIndex, int nWorldMatIndeLen,
        char* pInverseBindPoses, int nInverseBindPosesLen, char* pBoneIndices, int nBoneIndicesLen,
        char* pBindPoseIndices, int nBindPoseIndicesLen, char* pResultData, int nResultDataLen)
    {
        float* pF32WorldMatrixs = (float*)pWorldMatrixs;
        unsigned short* pU16WorldMatrixIndex = (unsigned short*)pWorldMatrixIndex;
        float* pF32InverseBindPoses = (float*)pInverseBindPoses;
        unsigned short* pU16BoneIndices = (unsigned short*)pBoneIndices;
        nBoneIndicesLen /= sizeof(unsigned short);
        unsigned short* pU16BindPoseIndices = (unsigned short*)pBindPoseIndices;
        float* pf32ResultData = (float*)pResultData;
        //当前的骨头数量
        for (int i = 0; i < nBoneIndicesLen; i++)
        {
            int nBoneIndex = pU16BoneIndices[i];//骨头的ID
                                                //worldMartix的ID
            int nWorldMatrixID = pU16WorldMatrixIndex[nBoneIndex];
            //当前world Matrix
            float* pCurWorldMatrix = pF32WorldMatrixs + nWorldMatrixID * 16;
            //当前bindPoseID
            int nBindPoseID = pU16BindPoseIndices[nBoneIndex];
            //当前bind Pose Matrix
            float* pCurBindMatrix = pF32InverseBindPoses + nBindPoseID * 16;

            //计算
            float* pResultMatrix = pf32ResultData + i * 16;
            matrix4x4MultiplyFFF(pCurWorldMatrix, pCurBindMatrix, pResultMatrix);
        }
        return true;
    }
    void JCWebGLPlus::uniformMatrix2fvEx(GLuint location, GLboolean transpose, int id)
    {
        JCArrayBufferManager::ArrayBufferContent* pData = m_pRArrayBufferManager->getArrayBuffer(id);
        if (pData)
        {
            uniformMatrix2fv(location, pData->m_nLength / sizeof(float) / 4, transpose, (float*)pData->m_pBuffer);
        }
    }
    void JCWebGLPlus::uniformMatrix3fvEx(GLuint location, GLboolean transpose, int id)
    {
        JCArrayBufferManager::ArrayBufferContent* pData = m_pRArrayBufferManager->getArrayBuffer(id);
        if (pData)
        {
            uniformMatrix3fv(location, pData->m_nLength / sizeof(float) / 9, transpose, (float*)pData->m_pBuffer);
        }
    }
    void JCWebGLPlus::uniformMatrix4fvEx(GLuint location, GLboolean transpose, int id)
    {
        JCArrayBufferManager::ArrayBufferContent* pData = m_pRArrayBufferManager->getArrayBuffer(id);
        if (pData)
        {
            uniformMatrix4fv(location, pData->m_nLength / sizeof(float) / 16, transpose, (float*)pData->m_pBuffer);
        }
    }
    void JCWebGLPlus::uploadShaderUniforms(int nCmdID, int nDataID)
    {
        JCArrayBufferManager::ArrayBufferContent* pCmd = m_pRArrayBufferManager->getArrayBuffer(nCmdID);
        JCArrayBufferManager::ArrayBufferContent* pData = m_pRArrayBufferManager->getArrayBuffer(nDataID);
        if (pCmd && pData)
        {
            _uploadShaderUniforms(pCmd->getCommandEncoderBuffer(), pData->m_pBuffer, pData->m_nLength);
        }
    }
    void JCWebGLPlus::uploadShaderUniformsBuffer(int nCmdID, char* pData, int nDataSize)
    {
        JCArrayBufferManager::ArrayBufferContent* pCmd = m_pRArrayBufferManager->getArrayBuffer(nCmdID);
        if (pCmd && pData)
        {
            _uploadShaderUniforms(pCmd->getCommandEncoderBuffer(), pData, nDataSize);
        }
    }
    void JCWebGLPlus::_uploadShaderUniforms(JCCommandEncoderBuffer* pCmd, char* data, int dataSize)
    {
        if (pCmd == NULL)return;
        pCmd->setReadPos(0);
        int nLen = pCmd->getDataSize();
        while (pCmd->getReadPos() < (size_t)nLen)
        {
            //i=funID  j=uniformFunID  k=location  l=type  m=offset
            WEBGLPLUS_iiiiii* pParam = pCmd->popp<WEBGLPLUS_iiiiii>();
            UNIFORM_FUN nFun = (UNIFORM_FUN)pParam->c;
            int nLocation = pParam->d;
            int nType = pParam->e;
            int nOffset = pParam->f * 4;
            int nTextureLocation = pParam->g;
            switch (nFun)
            {
            case laya::UNIFORM1F:
            {
                float value = *(float*)(data + nOffset);
                this->uniform1f(nLocation, value);
                break;
            }
            case laya::UNIFORM1FV:
            {
                int value = *(int*)(data + nOffset);
                JCArrayBufferManager::ArrayBufferContent* pBuffer = m_pRArrayBufferManager->getArrayBuffer(value);
                if (pBuffer)
                {
                    this->uniform1fv(nLocation, pBuffer->m_nLength / sizeof(float), (GLfloat*)pBuffer->m_pBuffer);
                }
                break;
            }
            case laya::UNIFORM1I:
            {
                int value = *(int*)(data + nOffset);
                this->uniform1i(nLocation, value);
                break;
            }
            case laya::UNIFORM1IV:
            {
                int value = *(int*)(data + nOffset);
                JCArrayBufferManager::ArrayBufferContent* pBuffer = m_pRArrayBufferManager->getArrayBuffer(value);
                if (pBuffer)
                {
                    this->uniform1iv(nLocation, pBuffer->m_nLength / sizeof(int), (GLint*)pBuffer->m_pBuffer);
                }
                break;
            }
            case laya::UNIFORM2F:
            case laya::UNIFORM2FV:
            {
                int value = *(int*)(data + nOffset);
                JCArrayBufferManager::ArrayBufferContent* pBuffer = m_pRArrayBufferManager->getArrayBuffer(value);
                if (pBuffer)
                {
                    this->uniform2fv(nLocation, pBuffer->m_nLength / sizeof(float) / 2, (GLfloat*)pBuffer->m_pBuffer);
                }
                break;
            }
            case laya::UNIFORM2I:
            case laya::UNIFORM2IV:
            {
                int value = *(int*)(data + nOffset);
                JCArrayBufferManager::ArrayBufferContent* pBuffer = m_pRArrayBufferManager->getArrayBuffer(value);
                if (pBuffer)
                {
                    this->uniform2iv(nLocation, pBuffer->m_nLength / sizeof(int) / 2, (GLint*)pBuffer->m_pBuffer);
                }
                break;
            }
            case laya::UNIFORM3F:
            case laya::UNIFORM3FV:
            {
                int value = *(int*)(data + nOffset);
                JCArrayBufferManager::ArrayBufferContent* pBuffer = m_pRArrayBufferManager->getArrayBuffer(value);
                if (pBuffer)
                {
                    this->uniform3fv(nLocation, pBuffer->m_nLength / sizeof(float) / 3, (GLfloat*)pBuffer->m_pBuffer);
                }
                break;
            }
            case laya::UNIFORM3I:
            case laya::UNIFORM3IV:
            {
                int value = *(int*)(data + nOffset);
                JCArrayBufferManager::ArrayBufferContent* pBuffer = m_pRArrayBufferManager->getArrayBuffer(value);
                if (pBuffer)
                {
                    this->uniform3iv(nLocation, pBuffer->m_nLength / sizeof(int) / 3, (GLint*)pBuffer->m_pBuffer);
                }
                break;
            }
            case laya::UNIFORM4F:
            case laya::UNIFORM4FV:
            {
                int value = *(int*)(data + nOffset);
                JCArrayBufferManager::ArrayBufferContent* pBuffer = m_pRArrayBufferManager->getArrayBuffer(value);
                if (pBuffer)
                {
                    int nLength = pBuffer->m_nLength > 16 ? pBuffer->m_nLength : 16;
                    this->uniform4fv(nLocation, nLength / sizeof(float) / 4, (GLfloat*)pBuffer->m_pBuffer);
                }
                break;
            }
            case laya::UNIFORM4I:
            case laya::UNIFORM4IV:
            {
                int value = *(int*)(data + nOffset);
                JCArrayBufferManager::ArrayBufferContent* pBuffer = m_pRArrayBufferManager->getArrayBuffer(value);
                if (pBuffer)
                {
                    int nLength = pBuffer->m_nLength > 16 ? pBuffer->m_nLength : 16;
                    this->uniform4iv(nLocation, nLength / sizeof(int) / 4, (GLint*)pBuffer->m_pBuffer);
                }
                break;
            }
            case laya::UNIFORMMATRIX2FV:
            {
                int value = *(int*)(data + nOffset);
                JCArrayBufferManager::ArrayBufferContent* pBuffer = m_pRArrayBufferManager->getArrayBuffer(value);
                if (pBuffer)
                {
                    this->uniformMatrix2fv(nLocation, pBuffer->m_nLength / sizeof(float) / 4, false, (GLfloat*)pBuffer->m_pBuffer);
                }
                break;
            }
            case laya::UNIFORMMATRIX3FV:
            {
                int value = *(int*)(data + nOffset);
                JCArrayBufferManager::ArrayBufferContent* pBuffer = m_pRArrayBufferManager->getArrayBuffer(value);
                if (pBuffer)
                {
                    this->uniformMatrix3fv(nLocation, pBuffer->m_nLength / sizeof(float) / 9, false, (GLfloat*)pBuffer->m_pBuffer);
                }
                break;
            }
            case laya::UNIFORMMATRIX4FV:
            {
                int value = *(int*)(data + nOffset);
                JCArrayBufferManager::ArrayBufferContent* pBuffer = m_pRArrayBufferManager->getArrayBuffer(value);
                if (pBuffer)
                {
                    this->uniformMatrix4fv(nLocation, pBuffer->m_nLength / sizeof(float) / 16, false, (GLfloat*)pBuffer->m_pBuffer);
                }
                break;
            }
            case laya::UNIFORMSAMPLER_2D:
            {
                int nGpuTextureID = *(int*)(data + nOffset);
                this->activeTexture(nTextureLocation);
                this->bindTexture(GL_TEXTURE_2D, nGpuTextureID);
                this->uniform1i(nLocation, nTextureLocation - GL_TEXTURE0);
                break;
            }
            case laya::UNIFORMSAMPLER_CUBE:
            {
                int nGpuTextureID = *(int*)(data + nOffset);
                this->activeTexture(nTextureLocation);
                this->bindTexture(GL_TEXTURE_CUBE_MAP, nGpuTextureID);
                this->uniform1i(nLocation, nTextureLocation - GL_TEXTURE0);
                break;
            }
            default:
                break;
            }
        }
    }
	void JCWebGLPlus::clearAll()
	{
		m_pJSArrayBufferManager->clearAll();
		m_pJSABManagerSyncToRender->clearAll();
		m_pRArrayBufferManager->clearAll();
	}
}

//-----------------------------END FILE--------------------------------
