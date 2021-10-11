/**
@file			JCArrayBufferManager.cpp
@brief			
@author			James
@version		1.0
@date			2017_11_29
*/

#include "JCArrayBufferManager.h"
#include "../util/Log.h"

namespace laya
{
    JCArrayBufferManager::JCArrayBufferManager()
    {
        m_nGlobalID=1;
        m_vPrepareDelIDs.reserve(1024);
    }
    JCArrayBufferManager::~JCArrayBufferManager()
    {
        clearAll();
    }
    void JCArrayBufferManager::clearAll()
    {
        for (int i = 0, n = m_vBuffers.size(); i < n; i++)
        {
            if (m_vBuffers[i])
            {
                delete m_vBuffers[i];
                m_vBuffers[i] = NULL;
            }
        }
        m_vBuffers.clear();
        m_vPrepareDelIDs.clear();
        m_nGlobalID = 1;
    }
    int JCArrayBufferManager::getID()
    {
        for (int i = 1, n = m_vBuffers.size(); i < n; i++)
        {
            if (m_vBuffers[i] == NULL)
            {
                return i;
            }
        }
        return m_nGlobalID++;
    }
    void JCArrayBufferManager::removeArrayBuffer(int nID)
    {
        if ((size_t)nID < m_vBuffers.size())
        {
            if (m_vBuffers[nID])
            {
                delete m_vBuffers[nID];
                m_vBuffers[nID] = NULL;
            }
        }
    }
    int JCArrayBufferManager::createArrayBuffer(char* pBuffer, int nLength,ARRAY_BUFFER_TYPE nType, ARRAY_BUFFER_REF_TYPE nRefType)
    {
        int nID = getID();
        int nSize = m_vBuffers.size();

        JCArrayBufferManager::ArrayBufferContent* pBufferContent = NULL;
        if (nRefType == ARRAY_BUFFER_REF_TYPE_REFERENCE)
        {
            pBufferContent = new JCArrayBufferManager::ArrayBufferContent(pBuffer, nLength, nType, false);
        }
        else
        {
            pBufferContent = new JCArrayBufferManager::ArrayBufferContent(nType, true);
            pBufferContent->mallocBuffer(pBuffer, nLength, nType);
        }
        if (nID == nSize)
        {
            m_vBuffers.push_back(pBufferContent);
        }
        else if (nID < nSize)
        {
            if (m_vBuffers[nID] != NULL)
            {
                LOGE("JCArrayBufferManager::createArrayBuffer error");
            }
            m_vBuffers[nID] = pBufferContent;
        }
        else
        {
            m_vBuffers.resize(nID + 1);
            m_vBuffers[nID] = pBufferContent;
        }
        return nID;
    }
    bool JCArrayBufferManager::updateArrayBuffer(int nID, char* pBuffer, int nLength)
    {
        if ((size_t)nID >= m_vBuffers.size()) return false;
        if (m_vBuffers[nID])
        {
            m_vBuffers[nID]->setBuffer(pBuffer, nLength);
            return true;
        }
        else
        {
            LOGE("JCArrayBufferManager::updateArrayBuffer error");
            return false;
        }
        return true;
    }
    bool JCArrayBufferManager::syncArrayBufferDataToRuntime(int nID, char* pBuffer, int nLength)
    {
        if ((size_t)nID >= m_vBuffers.size()) return false;
        if (m_vBuffers[nID])
        {
            JCArrayBufferManager::ArrayBufferContent* pBufferContext = m_vBuffers[nID];
            if (nLength == pBufferContext->m_nLength)
            {
                memcpy(pBufferContext->m_pBuffer, pBuffer, nLength);
            }
            else
            {
                LOGE("JCArrayBufferManager::syncArrayBufferDataToRuntime length error");
                return false;
            }
        }
        else
        {
            LOGE("JCArrayBufferManager::syncArrayBufferDataToRuntime error");
            return false;
        }
        return true;
    }
    void JCArrayBufferManager::prepareRemoveArrayBuffer(int nID)
    {
        m_vPrepareDelIDs.push_back(nID);
    }
}

//-----------------------------END FILE--------------------------------