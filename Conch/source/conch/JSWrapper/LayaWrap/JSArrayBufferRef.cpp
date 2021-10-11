/**
@file			JSArrayBufferRef.cpp
@brief			
@author			James
@version		1.0
@date			2017_11_29
*/

#include "JSArrayBufferRef.h"
#include <webglplus/JCWebGLPlus.h>
#include <util/JCCommonMethod.h>
#include <util/JCMemorySurvey.h>
#include "../JSInterface/JSInterface.h"
namespace laya
{
	ADDJSCLSINFO(JSArrayBufferRef, JSObjNode);
    JSArrayBufferRef::JSArrayBufferRef()
    {
        m_nID = 0;
        m_bSyncToRender = false;
		AdjustAmountOfExternalAllocatedMemory(128);
    }
    JSArrayBufferRef::~JSArrayBufferRef()
    {
        callManagerRemoveArrayBuffer();
    }
    void JSArrayBufferRef::callManagerRemoveArrayBuffer()
    {
        if (JCWebGLPlus::getInstance()->m_nThreadMODE == THREAD_MODE_DOUBLE)
        {
            if (m_bSyncToRender)
            {
                JCWebGLPlus::getInstance()->m_pJSABManagerSyncToRender->prepareRemoveArrayBuffer(m_nID);
            }
            else
            {
                JCWebGLPlus::getInstance()->m_pJSArrayBufferManager->removeArrayBuffer(m_nID);
            }
        }
        else
        {
            JCWebGLPlus::getInstance()->m_pJSArrayBufferManager->removeArrayBuffer(m_nID);
        }
    }
    int JSArrayBufferRef::getID()
    {
        return m_nID;
    }
    bool JSArrayBufferRef::getIsSyncToRender()
    {
        return m_bSyncToRender;
    }

    void JSArrayBufferRef::exportJS()
    {
		JSP_CLASS("ArrayBufferRef", JSArrayBufferRef);
		JSP_ADD_PROPERTY_RO(id, JSArrayBufferRef, getID);
		JSP_ADD_METHOD("isSyncToRender", JSArrayBufferRef::getIsSyncToRender);
		JSP_INSTALL_CLASS("ArrayBufferRef", JSArrayBufferRef);
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
