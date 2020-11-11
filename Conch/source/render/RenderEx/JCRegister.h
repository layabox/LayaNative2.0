/**
@file			JCRegister.h
@brief			
@author			James
@version		1.0
@date			2018_6_25
*/

#ifndef __JCRegister_H__
#define __JCRegister_H__

#include <stdio.h>
#include <string>
#include <vector>
#include <math/Matrix32.h>
#include "JCGlobalValue.h"

#if __APPLE__
	#include <OpenGLES/ES2/gl.h>
	#include <OpenGLES/ES2/glext.h>
#else
	#include <GLES3/gl3.h>
	#include <EGL/egl.h>
	#include <EGL/eglext.h>
#endif

namespace laya
{
    enum RELATION_ZERO
    {
        RZ_LESS = 0,    //小于
        RZ_EQUAL,       //等于
        RZ_GREATER,     //大于
        RZ_LEQUAL,      //小于等于
        RZ_GEQUAL,      //大于等于
        RZ_NOTEQUAL     //不等于
    };
    class JCRegister
    {
    public:

        JCRegister(int nRegNum=32);

        virtual ~JCRegister();

        inline char* getReg(int nIndex)
        {
            return m_vRegPool[nIndex];
        }

        inline bool loadData(int nIndex, char* pData, int nDataSize)
        {
            char* pReg = m_vRegPool[nIndex];
            memcpy(pReg, pData, nDataSize);
            return true;
        }
		inline bool storeData(int nIndex, char* pData, int nDataSize)
		{
			if (nIndex >= m_nRegNum) return false;
			char* pReg = m_vRegPool[nIndex];
			memcpy(pData, pReg, nDataSize);
			return true;
		}
		inline bool operateReg(int nRegOut, int nReg1, int nReg2, int nSize, VALUE_OPERATE_TYPE nOperateType, int nDataType)
		{
			if (nRegOut >= m_nRegNum || nReg1 >= m_nRegNum || nReg2 >= m_nRegNum) return false;
			char* pReg1 = m_vRegPool[nReg1];
			char* pReg2 = m_vRegPool[nReg2];
			char* pRegOut = m_vRegPool[nRegOut];
			if (nDataType == GL_INT)
			{
				int* iReg1 = (int*)pReg1;
				int* iReg2 = (int*)pReg2;
				int* iRegOut = (int*)pRegOut;
				int size = nSize / sizeof(int);
				switch (nOperateType)
				{
				case VALUE_OPERATE_ADD:
					for (int i = 0; i < size; i++)iRegOut[i] = iReg1[i] + iReg2[i];
					break;
				case VALUE_OPERATE_SUB:
					for (int i = 0; i < size; i++)iRegOut[i] = iReg1[i] - iReg2[i];
					break;
				case VALUE_OPERATE_MUL:
					for (int i = 0; i < size; i++)iRegOut[i] = iReg1[i] / iReg2[i];
					break;
				case VALUE_OPERATE_DIV:
					for (int i = 0; i < size; i++)iRegOut[i] = iReg1[i] * iReg2[i];
					break;
				default:
					break;
				}
			}
			else if (nDataType == GL_FLOAT)
			{
				float* fReg1 = (float*)pReg1;
				float* fReg2 = (float*)pReg2;
				float* fRegOut = (float*)pRegOut;
				int size = nSize / sizeof(float);
				switch (nOperateType)
				{
				case VALUE_OPERATE_ADD:
					for (int i = 0; i < size; i++)fRegOut[i] = fReg1[i] + fReg2[i];
					break;
				case VALUE_OPERATE_SUB:
					for (int i = 0; i < size; i++)fRegOut[i] = fReg1[i] - fReg2[i];
					break;
				case VALUE_OPERATE_MUL:
					for (int i = 0; i < size; i++)fRegOut[i] = fReg1[i] * fReg2[i];
					break;
				case VALUE_OPERATE_DIV:
					for (int i = 0; i < size; i++)fRegOut[i] = fReg1[i] / fReg2[i];
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
					Matrix32::mul(fRegOut, fReg1, fReg2);
					break;
				}
			}
			return true;
		}

    private:

        int                 m_nRegNum;      ///<寄存器的个数
        std::vector<char*>  m_vRegPool;     ///<寄存器的池子

    };
}
//------------------------------------------------------------------------------


#endif //__JCRegister_H__

//-----------------------------END FILE--------------------------------