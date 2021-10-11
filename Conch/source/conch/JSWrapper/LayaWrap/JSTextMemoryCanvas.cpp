/**
@file			JSTextMemoryCanvas.cpp
@brief			
@author			James
@version		1.0
@date			2016.3.22
*/
#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <math.h>
#include "JSTextMemoryCanvas.h"
#include "../../JCScriptRuntime.h"
#include <util/Log.h>
#include "JSTextBitmapInfo.h"
#ifdef __APPLE__
#include "../../IOSFreetype/JCIOSFreeType.h"
#endif
#ifdef ANDROID
#include "../../JCSystemConfig.h"
#include "../../CToJavaBridge.h"
#endif   

namespace laya
{
    JSTextMemoryCanvas*  JSTextMemoryCanvas::ms_pTextMemoryCanvas = NULL;
	ADDJSCLSINFO(JSTextMemoryCanvas, JSObjNode);
    JSTextMemoryCanvas* JSTextMemoryCanvas::getInstance()
    {
        if (!ms_pTextMemoryCanvas)
        {
            ms_pTextMemoryCanvas = new JSTextMemoryCanvas();
        }
        return ms_pTextMemoryCanvas;
    }
    JSTextMemoryCanvas::JSTextMemoryCanvas()
    {
        AdjustAmountOfExternalAllocatedMemory(2048);
        m_pCurrentFontInfo = NULL;
        m_pFontManager = new JCFontManager();
        m_pFreeTypeRender = new JCFreeTypeFontRender();
#ifdef __APPLE__
        m_pIOSFreeType = new JCIOSFreeType();
        m_pFreeTypeRender->setIOSFTInterface(m_pIOSFreeType);
#endif
        //bitmapData
        m_kBitmapData.m_nBpp = 32;
        m_kBitmapData.m_nWidth = MAX_LINE_WIDTH;
        m_kBitmapData.m_nHeight = MAX_CHAR_HEIGHT;
        m_kBitmapData.m_nImageType = ImgType_unknow;
        m_kBitmapData.m_nImageID = -1;
        m_kBitmapData.m_pImageData = &(JCFreeTypeFontRender::m_pWordBuff[0]);
        m_fScaleX = 1.0f;
        m_fScaleY = 1.0f;
    }
    JSTextMemoryCanvas::~JSTextMemoryCanvas()
    {
        clearAllData();
#ifdef __APPLE__
        if (m_pIOSFreeType)
        {
            delete m_pIOSFreeType;
            m_pIOSFreeType = nullptr;
        }
#endif
        if (m_pFontManager)
        {
            delete m_pFontManager;
            m_pFontManager = NULL;
        }
        if (m_pFreeTypeRender)
        {
            delete m_pFreeTypeRender;
            m_pFreeTypeRender = NULL;
        }
        ms_pTextMemoryCanvas = NULL;
    }
    void JSTextMemoryCanvas::clearAllData()
    {
        m_pFontManager->clearAllData();
        m_pFreeTypeRender->clearCustomFont();
    }
    bool JSTextMemoryCanvas::initFreeTypeDefaultFontFromFile(const char* sDefaultTTFs)
    {
        return m_pFreeTypeRender->initDefaultFont(sDefaultTTFs);
    }
    bool JSTextMemoryCanvas::initFreeTypeDefaultFontFromBuffer(JSValueAsParam pArrayBufferArgs)
    {
        char* pArrayBuffer = NULL;
        int nArrayBufferSize = 0;
        bool bIsArrayBuffer = extractJSAB(pArrayBufferArgs, pArrayBuffer, nArrayBufferSize);
        if (bIsArrayBuffer)
        {
            return m_pFreeTypeRender->initDefaultFont(pArrayBuffer, nArrayBufferSize);
        }
        return false;
    }
    bool JSTextMemoryCanvas::setFontFaceFromUrl(const char* sFontFamily, const char* sTTFFileName)
    {
        return m_pFreeTypeRender->setFontFaceFromUrl(sFontFamily, sTTFFileName);
    }
    bool JSTextMemoryCanvas::setFontFaceFromBuffer(const char* sFontFamily, JSValueAsParam pArrayBufferArgs)
    {
        char* pArrayBuffer = NULL;
        int nArrayBufferSize = 0;
        bool bIsArrayBuffer = extractJSAB(pArrayBufferArgs, pArrayBuffer, nArrayBufferSize);
        if (bIsArrayBuffer)
        {
            return m_pFreeTypeRender->setFontFaceFromBuffer(sFontFamily, pArrayBuffer, nArrayBufferSize);
        }
        return false;
    }
    bool JSTextMemoryCanvas::removeFont(const char* sFontFamily)
    {
        return m_pFreeTypeRender->removeFont(sFontFamily);
    }
    void JSTextMemoryCanvas::setFontInfo(const char* sFontInfo)
    {
        m_sFont = sFontInfo;
        m_pCurrentFontInfo = m_pFontManager->getFontInfoFromText(sFontInfo);
    }
    const char* JSTextMemoryCanvas::getFontInfo()
    {
		return m_sFont.c_str();
    }
    JsValue JSTextMemoryCanvas::getTextBitmapData(const char* sText, int nColor, int nBorderSize, int nBorderColor)
    {
        if (m_pCurrentFontInfo)
        {
			//m_pMeasureTextmanager->measureText(m_pCurrentFontInfo, const_cast<char*>(sText), m_kBitmapData.m_nWidth, m_kBitmapData.m_nHeight);
			m_pFreeTypeRender->getTextLineBitmapData(const_cast<char*>(sText), &m_kBitmapData, nColor, m_pCurrentFontInfo, nBorderSize, nBorderColor, m_fScaleX, m_fScaleY);
        }
        else
        {
            m_kBitmapData.m_nWidth = 1;
            m_kBitmapData.m_nHeight = 1;
            memset(m_kBitmapData.m_pImageData, 0, m_kBitmapData.m_nWidth * m_kBitmapData.m_nHeight * sizeof(int));
        }
        JSTextBitmapInfo* pBitmapInfo = new JSTextBitmapInfo();
        pBitmapInfo->setInfo(m_kBitmapData.m_nWidth, m_kBitmapData.m_nHeight, m_kBitmapData.m_pImageData);
        return JSP_TO_JS(JSTextBitmapInfo*, pBitmapInfo);
    }
    JsValue JSTextMemoryCanvas::measureChar(int unicode)
    {
        int width = 0;
        int height = 0;
#ifdef JS_JSC
        
        JSContextRef ctx = laya::__TlsData::GetInstance()->GetCurContext();
        JSObjectRef obj = JSObjectMake(ctx, nullptr, nullptr);
        if (m_pCurrentFontInfo == NULL)
        {
            LOGW("JSMemoryCanvas::measureText 没有设置FontInfo");
            JSObjectSetProperty(ctx, obj, JSStringCreateWithUTF8CString("width"), JSValueMakeNumber(ctx, width), kJSPropertyAttributeNone, nullptr);
            //JSObjectSetProperty(ctx, obj, JSStringCreateWithUTF8CString("height"), JSValueMakeNumber(ctx, height), kJSPropertyAttributeNone, nullptr);
            return obj;
        }
        m_pFreeTypeRender->measureChar(unicode, m_pCurrentFontInfo, width, height);
        JSObjectSetProperty(ctx, obj, JSStringCreateWithUTF8CString("width"), JSValueMakeNumber(ctx, width), kJSPropertyAttributeNone, nullptr);
        //JSObjectSetProperty(ctx, obj, JSStringCreateWithUTF8CString("height"), JSValueMakeNumber(ctx, height), kJSPropertyAttributeNone, nullptr);
#else
        Isolate* iso = Isolate::GetCurrent();
        Local<Context> context = iso->GetCurrentContext();
        Local<Object> obj = Local<Object>::New(iso, Object::New(iso));

        if (m_pCurrentFontInfo == NULL)
        {
            LOGW("JSMemoryCanvas::measureText 没有设置FontInfo");
            obj->Set(context, String::NewFromUtf8(iso, "width").ToLocalChecked(), Number::New(iso, 0));
            //obj->Set(context, String::NewFromUtf8(iso, "height"), Number::New(iso, 0));
            return obj;
        }
        m_pFreeTypeRender->measureChar(unicode, m_pCurrentFontInfo, width, height);
        obj->Set(context, String::NewFromUtf8(iso, "width").ToLocalChecked(), Number::New(iso, width));
        //obj->Set(context, String::NewFromUtf8(iso, "height"), Number::New(iso, height));
#endif
        return obj;
    }
    void JSTextMemoryCanvas::scale(double x, double y)
    {
        m_fScaleX = x;
        m_fScaleY = y;
    }
	void JSTextMemoryCanvas::exportJS() 
    {
        JSP_GLOBAL_CLASS("_conchTextCanvas", JSTextMemoryCanvas, this);
        JSP_GLOBAL_ADD_PROPERTY(font, JSTextMemoryCanvas, getFontInfo, setFontInfo);
		JSP_GLOBAL_ADD_METHOD("setFontInfo", JSTextMemoryCanvas::setFontInfo);
        //JSP_GLOBAL_ADD_METHOD("measureText", JSTextMemoryCanvas::measureText);
		JSP_GLOBAL_ADD_METHOD("measureChar", JSTextMemoryCanvas::measureChar);
		JSP_GLOBAL_ADD_METHOD("initFreeTypeDefaultFontFromFile", JSTextMemoryCanvas::initFreeTypeDefaultFontFromFile);
		JSP_GLOBAL_ADD_METHOD("initFreeTypeDefaultFontFromBuffer", JSTextMemoryCanvas::initFreeTypeDefaultFontFromBuffer);
		JSP_GLOBAL_ADD_METHOD("setFontFaceFromUrl", JSTextMemoryCanvas::setFontFaceFromUrl);
		JSP_GLOBAL_ADD_METHOD("setFontFaceFromBuffer", JSTextMemoryCanvas::setFontFaceFromBuffer);
		JSP_GLOBAL_ADD_METHOD("removeFont", JSTextMemoryCanvas::removeFont);
		JSP_GLOBAL_ADD_METHOD("_getTextBitmapData", JSTextMemoryCanvas::getTextBitmapData);
		JSP_GLOBAL_ADD_METHOD("scale", JSTextMemoryCanvas::scale);
        JSP_INSTALL_GLOBAL_CLASS("_conchTextCanvas", JSTextMemoryCanvas, this);
	}
}
