/**
@file			JCImage.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_13
*/

#include "JCVideo.h"
#include <util/Log.h>
#include "JCImageManager.h"
#include <util/JCCommonMethod.h>

namespace laya
{
	JCVideo::JCVideo() : JCImage()
    {
		m_kBitmapData.m_pImageData = 0;
		m_videoHandler = nullptr;
    }
	JCVideo::~JCVideo()
    {
        if(m_videoHandler)
        {
            delete m_videoHandler;
            m_videoHandler = nullptr;
        }
    }


	void JCVideo::setVideoHandler(IVideoHandler* handler)
	{
		m_videoHandler = handler;
	}

	void JCVideo::updateTexImage()
	{
        if (m_videoHandler && m_videoHandler->isFrameUpdated())
        {
            m_videoHandler->updateBitmapData(&m_kBitmapData);
        }

	}
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
