/**
@file			JCOrderResManager.h
@brief			
@author			James
@version		1.0
@date			2017_12_1
*/

#ifndef __JCOrderResManager_H__
#define __JCOrderResManager_H__


#include <stdio.h>
#include <vector>
#include <util/Log.h>

namespace laya
{
    template<class T>
    class JCOrderResManager
    {
    public:

        /** @brief构造函数
        */
        JCOrderResManager( bool bNeedDelRes )
        {
            m_bNeedDelRes = bNeedDelRes;
            m_nGlobalID = 0;
        }

        /** @brief析构函数
        */
        ~JCOrderResManager()
        {
            clearAllRes();
        }

        void setRes(int nID, T* pRes)
        {
            int nSize = (int)m_vRes.size();
            if (nID == nSize)
            {
                m_vRes.push_back(pRes);
            }
            else if (nID < nSize)
            {
                if (m_vRes[nID] == NULL)
                {
                    m_vRes[nID] = pRes;
                }
                else
                {
                    LOGE("JCOrderResManager::setRes error m_vRes[%d] != NULL", nID);
                }
            }
            else
            {
                int nOldSize = m_vRes.size();
                m_vRes.resize(nID + 1);
                m_vRes[nID] = pRes;
            }
        }

        T* getRes(int nID)
        {
            return m_vRes[nID];
        }

        void deleteRes(int nID)
        {
            if (nID < m_vRes.size())
            {
                T* pRes = m_vRes[nID];
                if (pRes)
                {
                    delete pRes;
                }
                m_vRes[nID] = NULL;
            }
        }

        void removeRes(int nID)
        {
            if (nID < m_vRes.size())
            {
                m_vRes[nID] = NULL;
            }
        }

        int getGlobalID()
        {
            return m_nGlobalID++;
        }

        void clearAllRes()
        {
            if (m_bNeedDelRes)
            {
                for (int i = 0, n = m_vRes.size(); i < n; i++)
                {
                    T* pRes = m_vRes[i];
                    if (pRes)delete pRes;
                }
            }
            m_vRes.clear();
        }

        void resetGlobalID()
        {
            m_nGlobalID = 0;
        }

    public:
        std::vector<T*>         m_vRes;
        int                     m_nGlobalID;
        bool                    m_bNeedDelRes;

    };
}
//------------------------------------------------------------------------------


#endif //__JCOrderResManager_H__

//-----------------------------END FILE--------------------------------