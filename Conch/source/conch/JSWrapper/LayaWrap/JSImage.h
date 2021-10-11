/**
@file			JSImage.h
@brief			
@author			James
@version		1.0
@date			2014_11_17
*/

#ifndef __JSImage_H__
#define __JSImage_H__

//包含头文件
#include <stdio.h>
#ifndef WEBASM
	#include "../JSInterface/JSObjBase.h"
    #include "../JSInterface/JSInterface.h"
#endif
#include <Image/JCImage.h>
#include <resource/JCResource.h>

/** 
 * @brief 
*/
namespace laya
{
    class IConchThreadCmdMgr;
#ifndef WEBASM
	class JSImage :public JsObjBase, public JSObjNode
#else
    class JSImage
#endif
	{
    public:

        JSImage();

        ~JSImage();

#ifndef WEBASM

	public:
		enum { onloadid, onerrorid, thisid, objid };
		static JsObjClassInfo JSCLSINFO;
        static void exportJS();

        void onLoaded(std::weak_ptr<int> callbackref);

		void onError( int p_nError,std::weak_ptr<int> callbackref );

		void onLoadedCallJSFunction(std::weak_ptr<int> callbackref);

		void onErrorCallJSFunction( int p_nError,std::weak_ptr<int> callbackref);
        
        void setObj(JSValueAsParam p_pFunction);
        
        JsValue getObj();

		void SetOnload(JSValueAsParam p_pFunction );

		JsValue GetOnload();

		void SetOnError(JSValueAsParam p_pFunction );

		JsValue GetOnError();

		int GetWidth();

		int GetHeight();
		
		const char* getSrc();

		void setSrc( const char* p_sSrc );

        bool getComplete();

		JsValue getImageData( int p_nX,int p_nY,int p_nW,int p_nH );

        bool syncRestoreResource();

        void putBitmapData( char* pData,int width, int height);

        void putBitmapDataJS( JSValueAsParam pArrayBuffer, int width, int height );

        void putDataJS(JSValueAsParam pArrayBuffer);

        void setPremultiplyAlpha(bool bPremultiplyAlpha);

        void setBase64(char* base64);

    private:

        void onDecodeEnd(BitmapData& p_bmp, std::weak_ptr<int>& callbackref);

        void onDecodeEndDecThread(BitmapData p_bmp, std::weak_ptr<int>& callbackref);

        void onDownloadOK(JCResStateDispatcher* p_pRes, bool p_bDecodeSync, std::weak_ptr<int>& callbackref);

        void onDownloadError(JCResStateDispatcher* p_pRes, int e, std::weak_ptr<int>& callbackref);

        bool downloadImage(bool p_bSyncDecode);

#endif

    public:

        int getImageID();

        void destroy();

        void releaseTexture();

        void setImageInfo(const char* sUrl,int w, int h);

        void createImageOnRenderThread(int nID,JCImage* pImage);

        void deleteImageOnRenderThread(int nID);

        void releaseImageOnRenderThread(int nID);


#ifndef WEBASM

	public:
        JsObjHandle		                m_pOnLoad;
        JsObjHandle		                m_pOnError;
        JsObjHandle                     m_pObj;
        bool			                m_bComplete;
        std::shared_ptr<int>	        m_CallbackRef;
        std::string                     m_sUrl;
        int                             m_nDownloadState;
#endif

    public:
        JCImage*		                m_pImage;
        int                             m_nID;

	};
}

#endif //__JSImage_H__

//-----------------------------END FILE--------------------------------