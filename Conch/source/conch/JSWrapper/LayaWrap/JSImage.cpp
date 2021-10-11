/**
@file			JSImage.cpp
@brief			
@author			James
@version		1.0
@date			2014_11_17
*/

//包含头文件
#include "JSImage.h"
#include <util/Log.h>
#ifndef WEBASM
    #include <util/JCMemorySurvey.h>
    #include "../JSInterface/JSInterface.h"
    #include "../../JCScriptRuntime.h"
    #include <downloadCache/JCFileSource.h>
    #include <resource/JCFileResManager.h>
    
    #include "JSRuntime.h"
    #include <fileSystem/JCFileSystem.h>
#else
    #include "../../JCScrpitRuntimeWASM.h"
#endif

#include "../../JCConch.h"
#include "../../JCSystemConfig.h"

#include <LayaGL/JCLayaGLDispatch.h>
#include <util/JCCrypto.h>

namespace laya
{
#ifndef WEBASM
    ADDJSCLSINFO(JSImage, JSObjNode);
#endif
    JSImage::JSImage()
    {
        m_pImage = new JCImage();
        m_nID = JCConch::s_pConchRender->m_pImageManager->getImageID();
#ifndef WEBASM
        m_pImage->setManager(JCConch::s_pConchRender->m_pFileResManager, JCConch::s_pConchRender->m_pImageManager);
        m_CallbackRef.reset(new int(1));
        m_bComplete = false;
        m_pClsInfo = &JSImage::JSCLSINFO;
        m_nDownloadState = 0;
#else
        m_pImage->setManager(JCConch::s_pConchRender->m_pAtlasManager, JCConch::s_pConchRender->m_pTextureManager, NULL, JCConch::s_pConchRender->m_pImageManager);
#endif
    }
    JSImage::~JSImage()
    {
#ifndef WEBASM
	    m_pOnLoad.Reset();
	    m_pOnError.Reset();
        m_pObj.Reset();
	    JCMemorySurvey::GetInstance()->releaseClass( "image",this );
#endif
        destroy();
    }
    void JSImage::destroy()
    {
        if (m_pImage)
        {
            //通知渲染线程
            deleteImageOnRenderThread(m_nID);
	    }
    }
    void JSImage::releaseTexture()
    {
        if (m_pImage)
        {
            releaseImageOnRenderThread(m_nID);
        }
    }
#ifndef WEBASM
    void JSImage::onLoaded(std::weak_ptr<int> callbackref)
    {
	    std::function<void(void)> pFunction = std::bind(&JSImage::onLoadedCallJSFunction,this, callbackref);
	    JCScriptRuntime::s_JSRT->m_pPoster->postToJS(pFunction);
    }
    void JSImage::onError( int p_nError,std::weak_ptr<int> callbackref )
    {
	    std::function<void(void)> pFunction = std::bind(&JSImage::onErrorCallJSFunction,this,p_nError, callbackref);
	    JCScriptRuntime::s_JSRT->m_pPoster->postToJS( pFunction );
    }
    void JSImage::onLoadedCallJSFunction(std::weak_ptr<int> callbackref)
    {
	    if (!callbackref.lock()) return;
        if (JCScriptRuntime::s_JSRT->m_bIsExit == true)return;
        if (!IsMyJsEnv()) return;

        if (GetWidth() <= 0 || GetHeight() <= 0|| m_pImage->m_kBitmapData.m_pImageData==NULL) {
            m_pOnError.Call(500);
        }
        else {
            int nMemSize = GetWidth() * GetHeight() * 4 + 272;
            AdjustAmountOfExternalAllocatedMemory(nMemSize);
            JCMemorySurvey::GetInstance()->newClass("image", 1024, this);
            m_pImage->m_sUrl = m_sUrl;
            //通知渲染线程
            createImageOnRenderThread(m_nID, m_pImage);
            m_bComplete = true;
            m_pOnLoad.Call();
        }
		makeWeak();
    }
    void JSImage::onErrorCallJSFunction( int p_nError,std::weak_ptr<int> callbackref )
    {
	    if (!callbackref.lock())return;
        if (JCScriptRuntime::s_JSRT->m_bIsExit == true)return;
	    if (!IsMyJsEnv())return;
        LOGW("download image file error! %s\n", m_sUrl.c_str());
        m_pOnError.Call(p_nError);
		makeWeak();
    }
    bool JSImage::getComplete()
    {
        return m_bComplete;
    }
    void JSImage::SetOnload(JSValueAsParam p_pFunction )
    {
	    m_pOnLoad.set(onloadid,this,p_pFunction);
    }
    JsValue JSImage::GetOnload()
    {
	    return m_pOnLoad.getJsObj();
    }
    void JSImage::SetOnError(JSValueAsParam p_pFunction )
    {
	    m_pOnError.set(onerrorid,this,p_pFunction);
    }
    JsValue JSImage::GetOnError()
    {
        return m_pOnError.getJsObj();
    }
    JsValue JSImage::getObj()
    {
        return m_pObj.getJsObj();
    }
    void JSImage::setObj(JSValueAsParam obj)
    {
        m_pObj.set(objid,this,obj);
    }
    const char* JSImage::getSrc()
    {
        return m_sUrl.c_str();
    }
    bool JSImage::syncRestoreResource()
    {
        //if (!m_pImage || !m_pImage->m_pImageFile ) return false;
        //return m_pImage->m_pImageFile->downloadImage(true);
        return false;
    }
    void JSImage::setSrc( const char* p_sSrc )
    {
	    if (!p_sSrc) return;
        m_sUrl = p_sSrc;
	    std::weak_ptr<int> cbref(m_CallbackRef);
        downloadImage( false );
    }
    void JSImage::onDecodeEnd(BitmapData& p_bmp, std::weak_ptr<int>& callbackref)
    {
        if (!callbackref.lock())
        {
            if (p_bmp.m_pImageData)
            {
                delete[] p_bmp.m_pImageData;
            }
            return;
        }
        if (m_pImage && m_pImage->m_kBitmapData.m_pImageData)
        {
            m_pImage->m_kBitmapData.releaseData();
            m_pImage->m_kBitmapData.m_pImageData = NULL;
        }
        m_pImage->m_kBitmapData = p_bmp;
        onLoaded(callbackref);
    }
    void JSImage::onDecodeEndDecThread(BitmapData p_bmp, std::weak_ptr<int>& callbackref)
    {
	    JCScriptRuntime::s_JSRT->m_pPoster->postToJS(std::bind(&JSImage::onDecodeEnd, this, p_bmp, callbackref));
    }
    void JSImage::onDownloadOK(JCResStateDispatcher* p_pRes, bool p_bDecodeSync, std::weak_ptr<int>& callbackref)
    {
        m_nDownloadState = 0;
        JCFileRes* pFileRes = (JCFileRes*)p_pRes;
        if (pFileRes->m_pBuffer.get())
        {
		
            //同步加载
            if (p_bDecodeSync)
            {
            }
            else
            {
                imgDecodeCB cb = std::bind(&JSImage::onDecodeEndDecThread, this, std::placeholders::_1, callbackref);
                loadImageMemASync(pFileRes->m_pBuffer, pFileRes->m_nLength, cb);
            }
        }
        else
        {
            onDownloadError(p_pRes, 1, callbackref);
        }
    }
    void JSImage::onDownloadError(JCResStateDispatcher*, int e, std::weak_ptr<int>& callbackref)
    {
        if (!callbackref.lock())return;
        m_nDownloadState = 0;
        onError(e,callbackref);
    }
    bool JSImage::downloadImage(bool p_bSyncDecode)
    {
        if (m_nDownloadState == 1)return false;
        m_nDownloadState = 1;
        //TODO 要处理本地文件的情况
        std::weak_ptr<int> cbref(m_CallbackRef);
        JCFileRes* pRes = JCScriptRuntime::s_JSRT->m_pFileResMgr->getRes(m_sUrl);
        pRes->setOnReadyCB(std::bind(&JSImage::onDownloadOK, this, std::placeholders::_1, false, cbref));
        pRes->setOnErrorCB(std::bind(&JSImage::onDownloadError, this, std::placeholders::_1, std::placeholders::_2, cbref));
		makeStrong();
        return true;
    }
    int JSImage::GetWidth()
    {
	    return m_pImage->getWidth();
    }
    int JSImage::GetHeight()
    {
	    return m_pImage->getHeight();
    }
    void JSImage::setPremultiplyAlpha(bool bPremultiplyAlpha)
    {
		if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_DOUBLE)
		{
			JCScriptRuntime::s_JSRT->flushSharedCmdBuffer();
            JCCommandEncoderBuffer* pCmd = JCScriptRuntime::s_JSRT->m_pRenderCmd;
			pCmd->append(LAYA_SET_PREMULTIPLY_ALPHA);
			pCmd->append(m_nID);
			pCmd->append((int)bPremultiplyAlpha);
		}
		else
		{
			JCImage* pImage = JCConch::s_pConchRender->m_pImageManager->getImage(m_nID);
            if (pImage)
            {
                pImage->setPremultiplyAlpha(bPremultiplyAlpha);
            }
		}
    }
    void JSImage::putBitmapDataJS(JSValueAsParam pArrayBuffer, int width, int height)
    {
        char* pArrayBufferPtr = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = extractJSAB(pArrayBuffer, pArrayBufferPtr, nABLen);
        if (bIsArrayBuffer)
        {
            if (nABLen >= width * height * 4)
            {
                putBitmapData(pArrayBufferPtr,width, height);
            }
            else
            {
                LOGE("JSImage::pushBitmapData array buffer size < width * height * 4");
            }
        }
        else 
        {
            LOGE("JSImage::pushBitmapData param is not an ArrayBuffer!");
        }
    }

    static void deleter(char* p)
    {
        //不删除 JS保证在onDecodeEndDecThread前pArrayBuffer不垃圾回收
    }

    void JSImage::putDataJS(JSValueAsParam pArrayBuffer)
    {
        char* pArrayBufferPtr = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = extractJSAB(pArrayBuffer, pArrayBufferPtr, nABLen);
        if (bIsArrayBuffer)
        {
            if (nABLen <= 0)
                return;
            //设置url的名字
            char sCachePath[1024];
            memset(sCachePath, 0, 1024);
            sprintf(sCachePath, "%s/%d.LayaBoxImg", JCConch::s_pConch->m_sCachePath.c_str(), m_nID);
            m_sUrl = sCachePath;
            std::weak_ptr<int> cbref(m_CallbackRef);
            imgDecodeCB cb = std::bind(&JSImage::onDecodeEndDecThread, this, std::placeholders::_1, cbref);
            std::shared_ptr<char> pBuffer(pArrayBufferPtr, deleter);
            loadImageMemASync(pBuffer, nABLen, cb);
        }
        else
        {
            LOGE("JSImage::putData param is not an ArrayBuffer!");
        }
    }
    void JSImage::setBase64(char* base64)
    {
        if (!base64)
            return;
        //设置url的名字
        char sCachePath[1024];
        memset(sCachePath, 0, 1024);
        sprintf(sCachePath, "%s/%d.LayaBoxImg", JCConch::s_pConch->m_sCachePath.c_str(), m_nID);
        m_sUrl = sCachePath;
        std::weak_ptr<int> cbref(m_CallbackRef);
        imgDecodeCB cb = std::bind(&JSImage::onDecodeEndDecThread, this, std::placeholders::_1, cbref);
        int length = 0;
        std::shared_ptr<char> pBuffer(base64_decode((const unsigned char*)base64, strlen(base64), &length));
        loadImageMemASync(pBuffer, length, cb);
    }
    void JSImage::putBitmapData(char* pData, int width, int height )
    {
        if (m_pImage && m_pImage->m_kBitmapData.m_pImageData) 
        {
            m_pImage->m_kBitmapData.releaseData();
            m_pImage->m_kBitmapData.m_pImageData = NULL;
        }
        m_pImage->m_kBitmapData.m_nWidth = width;
        m_pImage->m_kBitmapData.m_nHeight = height;
        m_pImage->m_kBitmapData.m_pImageData = new char[width * height * 4];
        memcpy(m_pImage->m_kBitmapData.m_pImageData, pData, width*height * 4);
        //设置url的名字
        char sCachePath[1024];
        memset(sCachePath, 0, 1024);
        sprintf(sCachePath,"%s/%d.LayaBoxImg", JCConch::s_pConch->m_sCachePath.c_str(),m_nID);
        //写入文件
        writeFileSync1(sCachePath, pData, width*height * 4);
        m_sUrl = sCachePath;
        m_pImage->m_bPushBitmapData = true;
        onLoaded(m_CallbackRef);
    }
    JsValue JSImage::getImageData( int p_nX,int p_nY,int p_nW,int p_nH )
    {
	    if( m_bComplete == false ) return JSP_TO_JS_NULL;
	    if( m_pImage == NULL ) return JSP_TO_JS_NULL;
	    BitmapData* pImg = &(m_pImage->m_kBitmapData);
	    if( pImg  )
	    {
		    if( p_nX < 0 || p_nY < 0 || p_nX >= pImg->m_nWidth || p_nY >= pImg->m_nHeight )return JSP_TO_JS_NULL;
		    if( ( p_nX + p_nW ) > pImg->m_nWidth || ( p_nY + p_nH ) > pImg->m_nHeight  )return JSP_TO_JS_NULL;

            if (pImg->m_pImageData != NULL || (pImg->m_pImageData == NULL && m_pImage->enableImage()))
            {
		        if( p_nX == 0 && p_nY == 0 && p_nW == pImg->m_nWidth && p_nH == pImg->m_nHeight )
		        {
			        return createJSAB( (char *)(pImg->m_pImageData),pImg->m_nWidth * pImg->m_nHeight * 4 );
		        }
		        else
		        {
			        unsigned char* pTemp = (unsigned char *)(pImg->m_pImageData);
			        int nSize = p_nH * p_nW * 4;
			        int nDstLine = p_nW*4;
			        int nSrcLine = pImg->m_nWidth*4;
			        unsigned char* pBuffer = new unsigned char[nSize];
			        for( int i = 0; i < p_nH; i++ )
			        {
				        memcpy(&pBuffer[nDstLine*i],&pTemp[nSrcLine*(i+p_nY)+p_nX*4],nDstLine);
			        }
			        return createJSAB( (char*)pBuffer,nSize );
		        }
            }
	    }
	    return JSP_TO_JS_NULL;
    }
#endif
    void JSImage::setImageInfo(const char* sUrl, int w, int h)
    {
#ifdef WEBASM
        if(sUrl)m_pImage->m_sUrl = sUrl;
        m_pImage->m_kBitmapData.m_nWidth = w;
        m_pImage->m_kBitmapData.m_nHeight = h;
        createImageOnRenderThread(m_nID, m_pImage);
#endif
    }
    int JSImage::getImageID()
    {
	    return m_nID;
    }
#ifndef WEBASM
    void JSImage::exportJS() 
    {
        JSP_CLASS("conchImage", JSImage);
        JSP_ADD_PROPERTY_RO(conchImgId, JSImage, getImageID);
        JSP_ADD_PROPERTY_RO(width, JSImage, GetWidth);
        JSP_ADD_PROPERTY_RO(height, JSImage, GetHeight);
        JSP_ADD_METHOD("setBase64", JSImage::setBase64);
        JSP_ADD_PROPERTY(src, JSImage, getSrc, setSrc);
        JSP_ADD_PROPERTY(_onload, JSImage, GetOnload, SetOnload);
        JSP_ADD_PROPERTY(onload, JSImage, GetOnload, SetOnload);
        JSP_ADD_PROPERTY(onerror, JSImage, GetOnError, SetOnError);
        JSP_ADD_PROPERTY(obj, JSImage, getObj, setObj);
        JSP_ADD_PROPERTY_RO(complete, JSImage, getComplete);
        JSP_ADD_METHOD("getImageID", JSImage::getImageID);
        JSP_ADD_METHOD("setSrc", JSImage::setSrc);
        JSP_ADD_METHOD("getImageData", JSImage::getImageData);
        JSP_ADD_METHOD("putBitmapData", JSImage::putBitmapDataJS);
        JSP_ADD_METHOD("putData", JSImage::putDataJS);
        JSP_ADD_METHOD("setPremultiplyAlpha", JSImage::setPremultiplyAlpha);
        JSP_ADD_METHOD("syncRestoreResource", JSImage::syncRestoreResource);
        JSP_ADD_METHOD("destroy", JSImage::destroy);
        JSP_INSTALL_CLASS("conchImage", JSImage);
    }
#endif
    void JSImage::createImageOnRenderThread(int nID, JCImage* pImage)
    {
		if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_DOUBLE)
		{
			JCScriptRuntime::s_JSRT->flushSharedCmdBuffer();
            JCCommandEncoderBuffer* pCmd = JCScriptRuntime::s_JSRT->m_pRenderCmd;
			pCmd->append(LAYA_CREATE_IMAGE_ON_RENDER_THREAD);
			pCmd->append(nID);
			pCmd->append((long)(pImage));
		}
		else
		{
			JCConch::s_pConchRender->m_pImageManager->setImage(nID, pImage);
		}
    }
    void JSImage::deleteImageOnRenderThread(int nID)
    {
        JCCommandEncoderBuffer* pCmd = JCScriptRuntime::s_JSRT->m_pGCCmd;
		pCmd->append(LAYA_DELETE_IMAGE_ON_RENDER_THREAD);
		pCmd->append(nID);
    }
    void JSImage::releaseImageOnRenderThread(int nID)
    {
        JCCommandEncoderBuffer* pCmd = JCScriptRuntime::s_JSRT->m_pGCCmd;
		pCmd->append(LAYA_RELEASE_IMAGE_ON_RENDER_THREAD);
		pCmd->append(nID);
    }
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//-----------------------------END FILE--------------------------------
