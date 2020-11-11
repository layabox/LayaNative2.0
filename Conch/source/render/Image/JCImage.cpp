/**
@file			JCImage.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_13
*/

#include "JCImage.h"
#include <util/Log.h>
#include "JCImageManager.h"
#include <util/JCCommonMethod.h>

namespace laya
{
    JCImage::JCImage()
    {
        m_pFileResManager = NULL;
        m_pImageManager = NULL;
        m_nID = 0;
        m_nTouchTime = 0;
        m_bPushBitmapData = false;
        m_bPremultiplyAlpha = false;
    }
    JCImage::~JCImage()
    {
        m_kBitmapData.releaseData();
    }
    void JCImage::setManager(void* pFileResManager, JCImageManager* pImageManager)
    {
        m_pFileResManager = (JCFileResManager*)pFileResManager;
        m_pImageManager = pImageManager;
    }
    void JCImage::setPremultiplyAlpha(bool bPremultiplyAlpha)
    {
        m_bPremultiplyAlpha = bPremultiplyAlpha;
    }
    int JCImage::getWidth()
    {
        return m_kBitmapData.m_nWidth;
    }
    int JCImage::getHeight()
    {
        return m_kBitmapData.m_nHeight;
    }
    char* JCImage::getImageData()
    {
        return m_kBitmapData.m_pImageData;
    }
    void JCImage::setImageID(int nID)
    {
        m_nID = nID;
        m_kBitmapData.m_nImageID = nID;
    }
    bool JCImage::premultiplyAlpha(BitmapData* pBitmapData)
    {
        if (pBitmapData == NULL)return false;
        if (pBitmapData->m_nImageType != ImgType_png && pBitmapData->m_nImageType != ImgType_gif)return false;
        int w = pBitmapData->m_nWidth*4;
        int h = pBitmapData->m_nHeight;
        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < w; j+=4)
            {
                int n = i*w + j;
                unsigned char* base = (unsigned char*)(pBitmapData->m_pImageData+n);
                unsigned char alpha = base[3];
                if (alpha != 0xFF)
                {
                    unsigned int  red = base[0];
                    unsigned int  green = base[1];
                    unsigned int  blue = base[2];
                    red = red * alpha / 255;
					green = green * alpha / 255;
					blue = blue * alpha / 255;
                    base[0] = (unsigned char)red;
                    base[1] = (unsigned char)green;
                    base[2] = (unsigned char)blue;
                }
            }
        }
        return true;
    }
    void JCImage::releaseBitmapData()
    {
        m_kBitmapData.releaseData();
    }
    bool JCImage::enableImage()
    {
        if (m_kBitmapData.m_pImageData == NULL)
        {
            if (m_pFileResManager)
            {
                JCFileRes* pRes = m_pFileResManager->getRes(m_sUrl);
                JCBuffer kBuffer;
                if (pRes && pRes->loadFromCache(kBuffer, false))
                {
                    if (m_bPushBitmapData == false)
                    {
                        if (loadImageMemSync(kBuffer.m_pPtr, kBuffer.m_nLen, m_kBitmapData) == false)
                        {
                            LOGE("JCImage::gpuRestoreRes decode image error url=%s", m_sUrl.c_str());
                            m_kBitmapData.releaseData();
                            return false;
                        }
                    }
                    else
                    {
                        int nImageLenght = m_kBitmapData.m_nWidth * m_kBitmapData.m_nHeight * 4;
                        if (kBuffer.m_nLen == nImageLenght)
                        {
                            m_kBitmapData.m_pImageData = new char[nImageLenght];
                            memcpy(m_kBitmapData.m_pImageData, kBuffer.m_pPtr, nImageLenght);
                        }
                        else
                        {
                            LOGE("JCImage::gpuRestoreRes image lenght != buffer.lenght");
                            return false;
                        }
                    }
                }
                else
                {
                    LOGE("JCImage::gpuRestoreRes load file error url=%s", m_sUrl.c_str());
                    m_kBitmapData.releaseData();
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        if(m_bPremultiplyAlpha)
        {
            premultiplyAlpha(&m_kBitmapData);
        }
        return true;
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
