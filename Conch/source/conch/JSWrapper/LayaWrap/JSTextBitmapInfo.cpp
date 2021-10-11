/**
@file			JSTextBitmapInfo.cpp
@brief			
@author			James
@version		1.0
@date			2018_3_31
*/

#include "JSTextBitmapInfo.h"
#include "../JSInterface/JSInterface.h"
#include "util/Log.h"
#include <util/JCMemorySurvey.h>
#include "../../JCScriptRuntime.h"

namespace laya
{
	ADDJSCLSINFO(JSTextBitmapInfo, JSObjNode);
    //------------------------------------------------------------------------------
    JSTextBitmapInfo::JSTextBitmapInfo()
    {
	    m_nWidth = 0;
	    m_nHeight = 0;
        m_pBitmapData = NULL;
	    AdjustAmountOfExternalAllocatedMemory(10240);
	    JCMemorySurvey::GetInstance()->newClass( "textBitmap",10240,this );
    }
    JSTextBitmapInfo::~JSTextBitmapInfo()
    {
	    JCMemorySurvey::GetInstance()->releaseClass( "textBitmap",this );
    }
    int JSTextBitmapInfo::getWidth()
    {
	    return m_nWidth;
    }
    int JSTextBitmapInfo::getHeight()
    {
	    return m_nHeight;
    }
    void JSTextBitmapInfo::setInfo(int w, int h, char* data)
    {
        m_nWidth = w;
        m_nHeight = h;
        m_pBitmapData = data;
    }
    JsValue JSTextBitmapInfo::getBitmapData()
    {
        return createJSAB(m_pBitmapData, m_nWidth*m_nHeight*sizeof(int));
    }
    void JSTextBitmapInfo::exportJS()
    {
        JSP_CLASS("_textBitmap", JSTextBitmapInfo);
        JSP_ADD_PROPERTY_RO(width, JSTextBitmapInfo, getWidth);
        JSP_ADD_PROPERTY_RO(height, JSTextBitmapInfo, getHeight);
        JSP_ADD_PROPERTY_RO(data, JSTextBitmapInfo, getBitmapData);
	    JSP_INSTALL_CLASS("_textBitmap", JSTextBitmapInfo);
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------