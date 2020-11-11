/**
@file			JCIDGenerator.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#include "JCIDGenerator.h"

namespace laya
{
    JCIDGenerator::JCIDGenerator()
    {
    }
    JCIDGenerator::~JCIDGenerator()
    {
    }
    bool JCIDGenerator::deleteID(GLuint nFakeID)
    {
        //只设置为无效，不从vector中删除
        if (nFakeID >= 0 && nFakeID < m_vIDTable.size())
        {
            m_vIDTable[nFakeID] = 0;
        }
        return true;
    }
    bool JCIDGenerator::setRealID(GLuint nFakeID,GLuint nRealID)
    {
        if (nFakeID == m_vIDTable.size())
        {
            m_vIDTable.push_back(nRealID);
        }
        else if (nFakeID > m_vIDTable.size())
        {
            int nLastSize = m_vIDTable.size();
            m_vIDTable.resize(nFakeID +1);
            //把中间的空的，全部填充成无效的
            for (int i = nLastSize,n=m_vIDTable.size(); i < n; i++)
            {
                m_vIDTable[i] = 0;
            }
            m_vIDTable[nFakeID] = nRealID;
        }
        else
        {
            m_vIDTable[nFakeID] = nRealID;
        }
        return true;
    }
    void JCIDGenerator::reset()
    {
        m_vIDTable.clear();
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------