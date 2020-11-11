/**
@file			JCGlobalValue.cpp
@brief			
@author			James
@version		1.0
@date			2018_5_30
*/

#include "JCGlobalValue.h"
#include <util/Log.h>
#include <math/Matrix32.h>
#include <memory.h>
#include <string.h>
#if __APPLE__
    #include <OpenGLES/ES3/gl.h>
    #include <OpenGLES/ES3/glext.h>
#else
    #include <GLES3/gl3.h>
    #include <EGL/egl.h>
    #include <EGL/eglext.h>
#endif

//------------------------------------------------------------------------------
namespace laya
{
    #define SAVE_DATA_NUM 20
    JCGlobalValue::JCGlobalValue()
    {
        m_nStrideSize = 0;
        m_pCurrentValue = NULL;
        m_pBuffer = NULL;
        m_nCurrentBufferPos = 0;
        m_nSaveDataSize = 0;
        m_pDefaultValue = NULL;
    }
    JCGlobalValue::~JCGlobalValue()
    {
        for (int i = 0, n = m_vValueDesc.size(); i < n; i++)
        {
            delete m_vValueDesc[i];
            m_vValueDesc[i] = NULL;
        }
        m_vValueDesc.clear();
        if (m_pBuffer)
        {
            delete[] m_pBuffer;
            m_pBuffer = NULL;
        }
        if (m_pDefaultValue)
        {
            delete m_pDefaultValue;
            m_pDefaultValue = NULL;
        }
        m_pCurrentValue = NULL;
    }
    ValueDesc* JCGlobalValue::getValueDesc(int nID)
    {
        return m_vValueDesc[nID];
    }
    void JCGlobalValue::createDefaultValue()
    {
        //计算总体的大小
        ValueDesc* pLast = m_vValueDesc[m_vValueDesc.size() - 1];
        m_nStrideSize = pLast->m_nOffset + pLast->m_nDataSize;

        //分配好默认的值
        m_pDefaultValue = new char[m_nStrideSize];
        for (int i = 0, n = m_vValueDesc.size(); i < n; i++)
        {
            ValueDesc* pValueDesc = m_vValueDesc[i];
            memcpy(m_pDefaultValue + pValueDesc->m_nOffset, pValueDesc->m_pDefaultValue, pValueDesc->m_nDataSize);
            //这段数据可以不要了
            delete pValueDesc->m_pDefaultValue;
            pValueDesc->m_pDefaultValue = NULL;
        }
    }
    void JCGlobalValue::clone(JCGlobalValue* pValueData)
    {
        m_nStrideSize = pValueData->m_nStrideSize;
        m_pDefaultValue = new char[m_nStrideSize];
        memcpy(m_pDefaultValue,pValueData->m_pDefaultValue,m_nStrideSize);
        int n = pValueData->m_vValueDesc.size();
        m_vValueDesc.resize(n);
        for (int i = VALUE_INTERIOR_TYPE_COUNT; i < n; i++)
        {
            ValueDesc* pTemp = pValueData->m_vValueDesc[i];
            ValueDesc* pValueDesc = new ValueDesc();
            pValueDesc->m_nModifyType = pTemp->m_nModifyType;
            pValueDesc->m_nDataType = pTemp->m_nDataType;
            pValueDesc->m_nSize = pTemp->m_nSize;
            pValueDesc->m_nDataSize = pTemp->m_nDataSize;
            pValueDesc->m_nOffset = pTemp->m_nOffset;
            pValueDesc->m_pDefaultValue = NULL;
            m_vValueDesc[i] = pValueDesc;
        }
        //分配整体的buffer
        m_nSaveDataSize = SAVE_DATA_NUM * m_nStrideSize;
        m_pBuffer = new char[m_nSaveDataSize];
        reset();
    }
    void JCGlobalValue::endValueDefine()
    {
        createDefaultValue();
        //分配整体的buffer
        m_nSaveDataSize = SAVE_DATA_NUM * m_nStrideSize;
        m_pBuffer = new char[m_nSaveDataSize];
        reset();
    }
    int JCGlobalValue::addValueDefine(VALUE_MODIFY_TYPE nModifyType, int nDataType, int nSize, char* pDefaultValue,int nDefaultValueSize)
    {
        ValueDesc* pValueDesc = new ValueDesc();
        pValueDesc->m_nModifyType = nModifyType;
        pValueDesc->m_nSize = nSize;
        pValueDesc->m_nDataType = nDataType;
        pValueDesc->m_nDataSize = nSize * 4;
        //new 默认值
        pValueDesc->m_pDefaultValue = new char[pValueDesc->m_nDataSize];
        memset(pValueDesc->m_pDefaultValue, 0, pValueDesc->m_nDataSize);
        if (nDefaultValueSize == pValueDesc->m_nDataSize)
        {
            memcpy(pValueDesc->m_pDefaultValue, pDefaultValue, pValueDesc->m_nDataSize);
        }
        int nVectorSize = m_vValueDesc.size();
        if (nVectorSize == 0)
        {
            pValueDesc->m_nOffset = 0;
        }
        else
        {
            ValueDesc* pLast = m_vValueDesc[nVectorSize - 1];
            pValueDesc->m_nOffset = pLast->m_nOffset + pLast->m_nDataSize;
        }
        m_vValueDesc.push_back(pValueDesc);
        return nVectorSize;
    }
    bool JCGlobalValue::reset()
    {
        memset(m_pBuffer, 0, m_nSaveDataSize);
        //放入一段默认的
        memcpy(m_pBuffer, m_pDefaultValue, m_nStrideSize);
        //当前的data指向默认
        m_pCurrentValue = m_pBuffer;
        m_nCurrentBufferPos = 0;
        return true;
    }
    bool JCGlobalValue::save()
    {
        m_nCurrentBufferPos += m_nStrideSize;
        if (m_nCurrentBufferPos >= m_nSaveDataSize )
        {
            int nEnlargeSize = SAVE_DATA_NUM * m_nStrideSize;
            char* pBuffer = new char[m_nSaveDataSize + nEnlargeSize];
            memset(pBuffer, 0, m_nSaveDataSize + nEnlargeSize);
            memcpy(pBuffer, m_pBuffer, m_nSaveDataSize);
            m_nSaveDataSize += nEnlargeSize;
            memcpy(pBuffer + m_nCurrentBufferPos, m_pCurrentValue, m_nStrideSize);
            if (m_pBuffer)
            {
                delete[] m_pBuffer;
                m_pBuffer = NULL;
            }
            m_pBuffer = pBuffer;
        }
        else
        {
            memcpy(m_pBuffer + m_nCurrentBufferPos, m_pCurrentValue, m_nStrideSize);
        }
        //重新指向新的位置
        m_pCurrentValue = m_pBuffer + m_nCurrentBufferPos;
        return true;
    }
    bool JCGlobalValue::restore()
    {
        if (m_nCurrentBufferPos > 0)
        {
            m_nCurrentBufferPos -= m_nStrideSize;
            m_pCurrentValue = m_pBuffer + m_nCurrentBufferPos;
            return true;
        }
        else
        {
            LOGE("JCContextSaveData::restore save and restore don't make a pair.");
            return false;
        }
    }
    ValueDesc* JCGlobalValue::setValue(int nValueID, char* pData, VALUE_OPERATE_TYPE nOpreateType)
    {
        ValueDesc* pValueDesc = m_vValueDesc[nValueID];
        if (pValueDesc->m_nDataType == GL_INT )
        {
            int* iCurData = (int*)(m_pCurrentValue + pValueDesc->m_nOffset);
            int* iData = (int*)pData;
            switch (nOpreateType)
            {
            case VALUE_OPERATE_ADD:
                for (int i = 0; i < pValueDesc->m_nSize; i++)iCurData[i] *= iData[i];
                break;
            case VALUE_OPERATE_SUB:
                for (int i = 0; i < pValueDesc->m_nSize; i++)iCurData[i] -= iData[i];
                break;
            case VALUE_OPERATE_MUL:
                for (int i = 0; i < pValueDesc->m_nSize; i++)iCurData[i] *= iData[i];
                break;
            case VALUE_OPERATE_DIV:
                for (int i = 0; i < pValueDesc->m_nSize; i++)iCurData[i] /= iData[i];
                break;
            case VALUE_OPERATE_SET:
                for (int i = 0; i < pValueDesc->m_nSize; i++)iCurData[i] = iData[i];
                break;
            case VALUE_OPERATE_BYTE4_COLOR_MUL:
                for (int i = 0; i < pValueDesc->m_nSize; i++)
                {
                    int color1 = iCurData[i];
                    int color2 = iData[i];
                    int b = color1 & 255;
                    int g = (color1 >> 8) & 255;
                    int r = (color1 >> 16) & 255;
                    int a = (color1 >> 24) & 255;
                    int b1 = color2 & 255;
                    int g1 = (color2 >> 8) & 255;
                    int r1 = (color2 >> 16) & 255;
                    int a1 = (color2 >> 24) & 255;
					b *= b1 / 255.0f;
					g *= g1 / 255.0f;
					r *= r1 / 255.0f;
					a *= a1 / 255.0f;
                    iCurData[i] = (a << 24) + (r << 16) + (g << 8) + b;
                }
                break;
            }
        }
        else if (pValueDesc->m_nDataType == GL_FLOAT)
        {
            float* fCurData = (float*)(m_pCurrentValue + pValueDesc->m_nOffset);
            float* fData = (float*)pData;
            switch (nOpreateType)
            {
            case VALUE_OPERATE_ADD:
                for (int i = 0; i < pValueDesc->m_nSize; i++)fCurData[i] *= fData[i];
                break;
            case VALUE_OPERATE_SUB:
                for (int i = 0; i < pValueDesc->m_nSize; i++)fCurData[i] -= fData[i];
                break;
            case VALUE_OPERATE_MUL:
                for (int i = 0; i < pValueDesc->m_nSize; i++)fCurData[i] *= fData[i];
                break;
            case VALUE_OPERATE_DIV:
                for (int i = 0; i < pValueDesc->m_nSize; i++)fCurData[i] /= fData[i];
                break;
            case VALUE_OPERATE_SET:
                for (int i = 0; i < pValueDesc->m_nSize; i++)fCurData[i] = fData[i];
                break;
            case VALUE_OPERATE_M2_MUL:
                {
                    //TODO
                }
                break;
            case VALUE_OPERATE_M3_MUL:
                {
                    //TODO
                }
                break;
            case VALUE_OPERATE_M4_MUL:
                {
                    //TODO
                }
                break;
            case VALUE_OPERATE_M32_MUL:
                Matrix32::mul(fCurData, fData,fCurData);
                break;
            case VALUE_OPERATE_M32_TRANSLATE:
                Matrix32::translate(fCurData, fData[0], fData[1]);
                break;
            case VALUE_OPERATE_M32_SCALE:
                Matrix32::scale(fCurData, fData[0], fData[1]);
                break;
            case VALUE_OPERATE_M32_ROTATE:
                Matrix32::rotate(fCurData, fData[0]);
                break;
            case VALUE_OPERATE_M32_SCALE_PIVOT:
                Matrix32::translate(fCurData,fData[2], fData[3]);
                Matrix32::scale(fCurData, fData[0], fData[1]);
                Matrix32::translate(fCurData, -fData[2], -fData[3]);
                break;
            case VALUE_OPERATE_M32_ROTATE_PIVOT:
                Matrix32::translate(fCurData, fData[1], fData[2]);
                Matrix32::rotate(fCurData, fData[0]);
                Matrix32::translate(fCurData, -fData[1], -fData[2]);
                break;
            case VALUE_OPERATE_M32_TRANSFORM_PIVOT:
                Matrix32::translate(fCurData, fData[6], fData[7]);
                Matrix32::mul(fCurData, fData, fCurData);
                Matrix32::translate(fCurData, -fData[6], -fData[7]);
                break;
            }
        }
        return pValueDesc;
    }
    void JCGlobalValue::copyValue(int nValueID, char* pData, int nDataLength)
    {
        ValueDesc* pValueDesc = m_vValueDesc[nValueID];
        memcpy(m_pCurrentValue + pValueDesc->m_nOffset, pData, nDataLength);
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
