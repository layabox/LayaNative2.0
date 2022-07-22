/**
@file			JSPromiseRejectionEvent.h
@brief			
@author			James
@version		1.0
@date			2018_3_31
*/

#ifndef __JSPromiseRejectionEvent_H__
#define __JSPromiseRejectionEvent_H__


//包含头文件
#include <stdio.h>
#include <string>
#include <map>
#include "../JSInterface/JSInterface.h"
#include "../JSInterface/JSObjBase.h"

namespace laya
{
	enum { reasonid, promiseid};
    class JSPromiseRejectionEvent :public JsObjBase, public JSObjNode
    {
    public:
	    static JsObjClassInfo JSCLSINFO;
	    static void exportJS();

        JSPromiseRejectionEvent();

	    ~JSPromiseRejectionEvent();

		void setReason(JSValueAsParam pObj);
		
		JsValue getReason();
		
		void setPromise(JSValueAsParam pObj);
		
		JsValue getPromise();
		
		char* getType();
		
		void setType(const char* strType);
    public:
		std::string m_strType;
	    JsObjHandle m_pReason;
		JsObjHandle m_pPromise;
    };
}
//------------------------------------------------------------------------------


#endif //__JSPromiseRejectionEvent_H__

//-----------------------------END FILE--------------------------------