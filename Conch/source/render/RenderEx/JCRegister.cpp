/**
@file			JCRegister.cpp
@brief			
@author			James
@version		1.0
@date			2018_6_25
*/

#include "JCRegister.h"

namespace laya 
{
    JCRegister::JCRegister(int nRegNum)
    {
        m_nRegNum = nRegNum;
        m_vRegPool.resize(m_nRegNum);
        int nMaxSize = sizeof(float) * 16;
        for (int i = 0; i < m_nRegNum; i++)
        {
            m_vRegPool[i] = new char[nMaxSize];
        }
    }
    JCRegister::~JCRegister()
    {
        for (int i = 0; i < m_nRegNum; i++)
        {
            delete[] m_vRegPool[i];
        }
        m_vRegPool.clear();
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------