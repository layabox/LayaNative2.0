/**
@file			JCImageManager.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_13
*/

//包含头文件
#include "JCImageManager.h"
#include <util/Log.h>
#include <util/JCCommonMethod.h>

//------------------------------------------------------------------------------
namespace laya
{
    JCImageManager::JCImageManager( )
    {
        m_nDeleteMaxNum = 0;
        m_nCountImageID = 0;
        m_nReleaseSpaceTime = 20000;
    }
    JCImageManager::~JCImageManager()
    {
        clearAllImage();
        clearImageIDs();
    }
    void JCImageManager::setReleaseSpaceTime(int nTime)
    {
        m_nReleaseSpaceTime = nTime;
        LOGI("JCImageManager::setReleaseSpaceTime=%d",nTime);
    }
    void JCImageManager::setImage(int nID, JCImage* pImage)
    {
        bool bSucess = true;
        int nSize = (int)m_vImages.size();
        if (nID == nSize)
        {
            m_vImages.push_back( pImage );
        }
        else if( nID < nSize )
        {
            if (m_vImages[nID] == NULL)
            {
                m_vImages[nID] = pImage;
            }
            else
            {
                bSucess = false;
                LOGE("JCImageManager::setImage error m_vImages[%d] != NULL",nID );
            }
        }
        else
        {
            int nOldSize = m_vImages.size();
            m_vImages.resize( nID + 1 );
            m_vImages[nID] = pImage;
        }
        if (bSucess)
        {
            pImage->setImageID(nID);
            pImage->m_nTouchTime = tmGetCurms();
            m_vOccupiedMemImages[nID] = pImage;
        }
    }
    JCImage* JCImageManager::getImage(int nID)
    {
        if ((size_t)nID < m_vImages.size())
        {
            return m_vImages[nID];
        }
        return NULL;
    }
    bool JCImageManager::deleteImage(int nID)
    {
        if (nID == -1) return false;
        m_vDeleteList.push_back(nID);
        return true;
    }
    bool JCImageManager::_deleteImage(int nID)
    {
        if (nID == -1) return false;
        if (nID < (int)m_vImages.size())
        {
            if (m_vImages[nID])
            {
                removeImageFromMap(nID);
                delete m_vImages[nID];
                m_vImages[nID] = NULL;
            }
        }
        removeImageID(nID);
        return true;
    }
    void JCImageManager::removeImageFromMap(int nID)
    {
        MapImageIter iter = m_vOccupiedMemImages.find(nID);
        if (iter != m_vOccupiedMemImages.end())
        {
            iter = m_vOccupiedMemImages.erase(iter);
        }
    }
    void JCImageManager::clearAllImage()
    {
        for (VectorImageIter iter = m_vImages.begin(); iter != m_vImages.end(); iter++)
        {
            JCImage* pImage = *iter;
            if (pImage)
            {
                delete pImage;
                pImage = NULL;
            }
        }
        m_vImages.clear();
        m_vOccupiedMemImages.clear();
    }
    void JCImageManager::resetRenderThread()
    {
        clearAllImage();
    }
    void JCImageManager::removeImageID(int nID)
    {
        m_kMutex.lock();
        if (nID >= 0 && (size_t)nID < m_vImageIDs.size())
        {
            m_vImageIDs[nID] = -1;
        }
        m_kMutex.unlock();
    }
    void JCImageManager::clearImageIDs()
    {
        m_kMutex.lock();
        m_vImageIDs.clear();
        m_kMutex.unlock();
    }
    int JCImageManager::getImageID()
    {
        m_kMutex.lock();
        for (int i = 0,n = m_vImageIDs.size(); i < n; i++ )
        {
            if (m_vImageIDs[i] == -1)
            {
                m_vImageIDs[i] = i;
                m_kMutex.unlock();
                return i;
            }
        }
        m_vImageIDs.push_back(m_nCountImageID);
        m_kMutex.unlock();
        return m_nCountImageID++;
    }
    void JCImageManager::resetJSThread()
    {
        clearImageIDs();
        m_nCountImageID = 0;
    }
    void JCImageManager::_batchHandleDeleteImage()
    {
        if (m_vDeleteList.size() <= 0)return;
        int n = m_vDeleteList.size();
        for (int i = 0; i < n; i++)
        {
            _deleteImage(m_vDeleteList[i]);
        }
        if (n > m_nDeleteMaxNum)
        {
            m_nDeleteMaxNum = n;
            m_vDeleteList.reserve(m_nDeleteMaxNum);
        }
        m_vDeleteList.clear();
    }
    void JCImageManager::update(int nFrameCount)
    {
        _batchHandleDeleteImage();
        if (nFrameCount % 5 == 0)
        {
            if (m_vOccupiedMemImages.size() > 0)
            {
                double nCurentTime = tmGetCurms();
                for (MapImageIter iter = m_vOccupiedMemImages.begin(); iter != m_vOccupiedMemImages.end(); )
                {
                    JCImage* pImage = iter->second;
                    if (pImage && (nCurentTime - pImage->m_nTouchTime) > m_nReleaseSpaceTime)
                    {
                        pImage->releaseBitmapData();
                        iter = m_vOccupiedMemImages.erase(iter);
                    }
                    else
                    {
                        iter++;
                    }
                }
            }
        }
    }
    void JCImageManager::printCorpseImages(const char* sFileName)
    {
#ifdef WIN32
        FILE* fp = fopen(sFileName, "w");
        char sBuffer[2048] = { 0 };
        int nSizeCount = 0;
        for (int i = 0, n = m_vImages.size(); i < n; i++ )
        {
            JCImage* pImage = m_vImages[i];
            if (pImage && pImage->m_kBitmapData.m_pImageData )
            {
                int nCurrentSize = pImage->m_kBitmapData.m_nHeight*pImage->m_kBitmapData.m_nWidth * 4;
                nSizeCount += nCurrentSize;
                sprintf(sBuffer, "image id=%d,memorySize=%d,url=%s\n", i, nCurrentSize,pImage->m_sUrl.c_str());
                LOGI(sBuffer);
                if (fp)
                {
                    fputs(sBuffer, fp);
                }
            }
        }
        sprintf(sBuffer, "memory size count=%d", nSizeCount);
        LOGI(sBuffer);
        if (fp)
        {
            fputs(sBuffer, fp);
            fclose(fp);
        }
#else
        char sBuffer[2048] = { 0 };
        int nSizeCount = 0;
        for (int i = 0, n = m_vImages.size(); i < n; i++)
        {
            JCImage* pImage = m_vImages[i];
            if (pImage && pImage->m_kBitmapData.m_pImageData)
            {
                int nCurrentSize = pImage->m_kBitmapData.m_nHeight*pImage->m_kBitmapData.m_nWidth * 4;
                nSizeCount += nCurrentSize;
                LOGI("image id=%d,memorySize=%d,url=%s\n", i, nCurrentSize, pImage->m_sUrl.c_str());
            }
        }
        LOGI(sBuffer, "memory size count=%d", nSizeCount);
#endif
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------