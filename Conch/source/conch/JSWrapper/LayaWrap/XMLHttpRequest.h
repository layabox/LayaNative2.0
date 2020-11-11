/**
@file			XMLHttpRequest.h
@brief			
@author			James
@version		1.0
@date			2017_11_28
*/

#ifndef __XMLHttpRequest_H__
#define __XMLHttpRequest_H__

#include <string>
#include <map>
#include "buffer/JCBuffer.h"
#include "../JSInterface/JSInterface.h"

namespace laya
{
	class Curl;
	class JCDownloadMgr;
    class IConchThreadCmdMgr;
	class XMLHttpRequest:public JsObjBase , public JSObjNode
    {
	public:
		static JsObjClassInfo JSCLSINFO;
		typedef std::string String;
		enum ErrorCode
        {
			SUCCESS,
			SYNTAX_ERR,
			INVALID_STATE_ERR,
		};
	    //These exact numeric values are important because JS expects them.
		enum State 
        {
			UNSENT = 0,
			OPENED = 1,
			HEADERS_RECEIVED = 2,
			LOADING = 3,
			DONE = 4
		};
		enum ResponseTypeCode 
        {
			ResponseTypeDefault,
			ResponseTypeText,
			ResponseTypeJSON,
			ResponseTypeDocument,
			ResponseTypeBlob,// Binary format
			ResponseTypeArrayBuffer
		};
        enum { onstatchgid, thisid, oncompleteid, onerrid };
		static const ResponseTypeCode FirstBinaryResponseType = ResponseTypeBlob;
	public:

		XMLHttpRequest();

        ~XMLHttpRequest();

		State readyState() const;

		//创建
		void open(const char* method, const char* p_url, bool async);

		void changeState(State newState);

		void postString(const char* p_pszUrl, const char* p_pszString, JSValueAsParam p_funOnOK, JSValueAsParam p_funOnErr);

		void abort();

		void setRequestHeader(const char* name, const char* value );

		void setRequestHeaderInternal(const std::string& name, const std::string& value);

		std::string getRequestHeader(const std::string& name) const;

		void callReadyStateChangeListener();

		int getStatus()
        {
			return m_state;
		}

		int getReadyState()
        {
			return m_state;
		}

		char* getStatusText(){
			return "not implement";
		}

		char* getResponseURL()
        {
			return (char*)m_url.c_str();
		}

		int get_responseTypeCode() const 
        {
            return (int)m_responseTypeCode; 
        }

        void set_responseTypeCode(int c) 
        {
            m_responseTypeCode = (ResponseTypeCode)c; 
        }

		bool getUpload()
        {
			return false;
		}

		bool isBin()
        {
			return (m_responseTypeCode==ResponseTypeArrayBuffer || m_responseTypeCode==ResponseTypeBlob);
		}

		void set_onreadystatechange(JSValueAsParam pObj);

		void _changeState(int p_nState);

		inline void postData(JCDownloadMgr* p_pDownloadMgr, const char* p_pszURL, char* p_pData, int p_nLen );

		//因为不是用v8原生的方法把js函数转换JsObjHandle2所以分成两个函数
        void setPostCB(JSValueAsParam p_onOK, JSValueAsParam p_onError);

		void JsPostData( const char* p_sUrl, JSValueAsParam arg1 );

        //回调也是走 setPostCB设置的函数
        void getData(const char* p_sUrl);
		
        static void exportJS();

	private:
        typedef std::map<std::string, std::string> HTTPHeaderMap;
        std::function<void(JCBuffer&, const std::string&, const std::string&, int, int, const std::string&)>		m_funcPostComplete;
        State                   m_state;
        bool                    m_uploadComplete;
        bool                    m_error;
        ErrorCode	            m_ErrorCode;
        std::string             m_method;
        std::string             m_url;
		bool                    m_async;
		HTTPHeaderMap	        m_requestHeaders;
		ResponseTypeCode        m_responseTypeCode;
		
	public:
		
		JsObjHandle		        m_funcOnStateChg;
		JsObjHandle		        m_This;// this 的 JS 对象
		JsObjHandle		        m_jsfunPostComplete;
		JsObjHandle		        m_jsfunPostError;
		std::shared_ptr<int>	m_CallbackRef;
        IConchThreadCmdMgr*     m_pCmdPoster;
    };
}
//------------------------------------------------------------------------------


#endif //__XMLHttpRequest_H__

//-----------------------------END FILE--------------------------------