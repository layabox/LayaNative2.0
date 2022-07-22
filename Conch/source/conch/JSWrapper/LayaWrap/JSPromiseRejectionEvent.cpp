/**
@file			JSPromiseRejectionEvent.cpp
@brief			
@author			James
@version		1.0
@date			2018_3_31
*/

#include "JSPromiseRejectionEvent.h"
#include "../JSInterface/JSInterface.h"
#include "util/Log.h"
#include <util/JCMemorySurvey.h>
#include "../../JCScriptRuntime.h"

namespace laya
{
	ADDJSCLSINFO(JSPromiseRejectionEvent, JSObjNode);
    //------------------------------------------------------------------------------
    JSPromiseRejectionEvent::JSPromiseRejectionEvent()
    {
	    AdjustAmountOfExternalAllocatedMemory(128);
	    JCMemorySurvey::GetInstance()->newClass( "JSPromiseRejectionEvent",10240,this );
    }
    JSPromiseRejectionEvent::~JSPromiseRejectionEvent()
    {
		m_pReason.Reset();
		m_pPromise.Reset();
	    JCMemorySurvey::GetInstance()->releaseClass( "JSPromiseRejectionEvent",this );
    }
    JsValue JSPromiseRejectionEvent::getReason()
    {
	    return m_pReason.getJsObj();
    }
    JsValue JSPromiseRejectionEvent::getPromise()
    {
        return m_pPromise.getJsObj();
    }
	void JSPromiseRejectionEvent::setReason(JSValueAsParam pObj)
	{
		m_pReason.set(reasonid, this, pObj);
	}
	void JSPromiseRejectionEvent::setPromise(JSValueAsParam pObj)
	{
		m_pPromise.set(promiseid, this, pObj);
	}
	void JSPromiseRejectionEvent::setType(const char* strType)
	{
		m_strType = strType;
	}
	char* JSPromiseRejectionEvent::getType()
	{
		return (char*)m_strType.c_str();
	}
    void JSPromiseRejectionEvent::exportJS()
    {
        JSP_CLASS("PromiseRejectionEvent", JSPromiseRejectionEvent);
        JSP_ADD_PROPERTY_RO(reason, JSPromiseRejectionEvent, getReason);
        JSP_ADD_PROPERTY_RO(promise, JSPromiseRejectionEvent, getPromise);
        JSP_ADD_PROPERTY_RO(type, JSPromiseRejectionEvent, getType);
	    JSP_INSTALL_CLASS("PromiseRejectionEvent", JSPromiseRejectionEvent);
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------