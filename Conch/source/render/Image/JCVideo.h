/**
@file			JCImage.h
@brief			
@author			James
@version		1.0
@date			2016_5_13
*/

#ifndef __JCVideo_H__
#define __JCVideo_H__

#include <resource/JCFileResManager.h>
#include <imageLib/JCImageRW.h>
#include "JCImage.h"

namespace laya
{
    class JCImageManager;
    /**
        * @brief
    */

	class IVideoHandler
	{
	public:
        virtual ~IVideoHandler() {}
        
		virtual bool isFrameUpdated() = 0;
		virtual void updateBitmapData(BitmapData* bitmapData) = 0;
	};


    class JCVideo : public JCImage
    {
    public:

        /** @brief构造函数
        */
		JCVideo();

        /** @brief析构函数
        */
        ~JCVideo();

		void setVideoHandler(IVideoHandler* handler);

		virtual bool isVideo() const override {
			return true;
		}

		virtual void updateTexImage();
		virtual bool enableImage() override { return true; }

    private:
		IVideoHandler*		m_videoHandler;
    };
}
#endif //__JCVideo_H__

//-----------------------------END FILE--------------------------------
