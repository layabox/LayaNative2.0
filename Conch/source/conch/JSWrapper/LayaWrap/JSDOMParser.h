/**
@file			JSXml.h
@brief			
@author			James
@version		1.0
@date			2015_5_5
*/

#ifndef __JSDOMParser_H__
#define __JSDOMParser_H__

#include <stdio.h>
#include "../JSInterface/JSInterface.h"
#include "resource/JCFileResManager.h"
#include "JSXmlNode.h"

/** 
 * @brief 
*/
namespace laya
{
	class JSDOMParser : public JsObjBase, public JSObjNode
	{
	public:
		static JsObjClassInfo JSCLSINFO;
		static void exportJS();
		JSDOMParser();
		~JSDOMParser();
		JsValue parseFromString(const char * str,const char *type);
		const char* getSrc();
		void setSrc( const char* p_sSrc );
		void SetOnload( JSValueAsParam p_pFunction );
		JsValue GetOnload();
		void SetOnError( JSValueAsParam p_pFunction );
		JsValue GetOnError();
		JsValue getXml();
	public:
		void onLoadedCallJSFunction(std::string& str,std::weak_ptr<int>& callbackref);
		void onErrorCallJSFunction( int e , std::weak_ptr<int>& callbackref);
	public:

		std::shared_ptr<int> m_CallbackRef;

	public:
		std::string		m_sUrl;
		JsObjHandle		m_pOnLoadJSFunction;
		JsObjHandle		m_pOnErrorJSFunction;
	private:
		JSXmlDocument* jsDOC;
	};
}

#endif //__JSDOMParser_H__

//-----------------------------END FILE--------------------------------