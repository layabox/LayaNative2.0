/**
@file			JCImage.h
@brief			
@author			James
@version		1.0
@date			2016_5_13
*/

#ifndef __JCImage_H__
#define __JCImage_H__

#include <resource/JCFileResManager.h>
#include <imageLib/JCImageRW.h>

namespace laya
{
    class JCImageManager;
    /**
        * @brief
    */
    class JCImage
    {
    public:

        /** @brief构造函数
        */
        JCImage();

        /** @brief析构函数
        */
        virtual ~JCImage();

        void setManager(void* pFileResManager, JCImageManager* pImageManager);

        int getWidth();

        int getHeight();

        void setImageID(int nID);

        char* getImageData();

        virtual bool enableImage();

        void releaseBitmapData();

        void setPremultiplyAlpha(bool bPremultiplyAlpha);

		virtual void updateTexImage() {}

		virtual bool isVideo() const { return false; }

    public:

        static bool premultiplyAlpha( BitmapData* pBitmapData );

    public:
        double             m_nTouchTime;
        BitmapData          m_kBitmapData;
        std::string         m_sUrl;
        JCFileResManager*   m_pFileResManager;
        JCImageManager*     m_pImageManager;
        bool                m_bPushBitmapData;      //是否为pushBitmapData的，正常的url为false，如果是js调用的pushBitmapData为true
    private:
        int                 m_nID;
        bool                m_bPremultiplyAlpha;
    };
}
#endif //__JCImage_H__

//-----------------------------END FILE--------------------------------
