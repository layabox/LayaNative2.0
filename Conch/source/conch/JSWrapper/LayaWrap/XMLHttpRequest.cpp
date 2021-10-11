/**
@file			XMLHttpRequest.cpp
@brief			
@author			James
@version		1.0
@date			2017_11_28
*/

#include "XMLHttpRequest.h"
#include <util/JCCommonMethod.h>
#include <util/Log.h>
#include <downloadMgr/JCDownloadMgr.h>
#include "../../JCScriptRuntime.h"
#include <util/JCIThreadCmdMgr.h>
namespace laya
{
    ADDJSCLSINFO(XMLHttpRequest, JSObjNode);
    XMLHttpRequest::XMLHttpRequest() 
    {
        m_pCmdPoster = JCScriptRuntime::s_JSRT->m_pPoster;
        m_state = UNSENT;
        m_uploadComplete = false;
        m_error = false;
        m_ErrorCode = SUCCESS;
        m_async = false;
        m_responseTypeCode = ResponseTypeDefault;
        m_CallbackRef.reset(new int(1));
        AdjustAmountOfExternalAllocatedMemory(100000);
        JCMemorySurvey::GetInstance()->newClass("XMLHttpRequest", 100000, this);
        //mpJsOnReadyStateChange = mpJsPostComplete = mpJsPostError = NULL;
    }
    XMLHttpRequest::~XMLHttpRequest()
    {
        m_jsfunPostError.Reset();
        m_jsfunPostComplete.Reset();
        JCMemorySurvey::GetInstance()->releaseClass("XMLHttpRequest", this);
    }
    void XMLHttpRequest::open(const char* method, const char* p_url, bool async)
    {
        State previousState = m_state;
        m_state = UNSENT;
        m_error = false;
        m_uploadComplete = false;
        m_method = method;
        m_method = LayaStrlwr((char*)m_method.c_str());
        if (m_method != "post" && m_method != "get") 
        {
            m_ErrorCode = SYNTAX_ERR;
            return;
        }
        m_method = method;
        m_url = p_url;
        m_async = async;
        // Check previous state to avoid dispatching readyState event
        // when calling open several times in a row.
        if (previousState != OPENED)
            changeState(OPENED);
        else
            m_state = OPENED;
    }
    void XMLHttpRequest::_changeState(int p_nState) 
    {
        changeState((State)p_nState);
    }
    void XMLHttpRequest::changeState(State newState) 
    {
        if (m_state != newState) 
        {
            m_state = newState;
            callReadyStateChangeListener();
        }
    }
    // See RFC 7230, Section 3.2.3.
    bool isValidHTTPHeaderValue(const char* value) 
    {
        unsigned int len = strlen(value);
        char c = value[0];
        if (c == ' ' || c == '\t')
            return false;
        c = value[len - 1];
        if (c == ' ' || c == '\t')
            return false;
        for (unsigned int i = 0; i < len; ++i) {
            c = value[i];
            if (c == 0x7F || c > 0xFF || (c < 0x20 && c != '\t'))
                return false;
        }
        return true;
    }
    // See RFC 7230, Section 3.2.6.
    bool isValidHTTPToken(const char* value) 
    {
        if (!value || value[0] == 0)
            return false;
        unsigned int len = strlen(value);
        for (unsigned int i = 0; i < len; ++i) 
        {
            char c = value[i];
            if (c <= 0x20 || c >= 0x7F
                || c == '(' || c == ')' || c == '<' || c == '>' || c == '@'
                || c == ',' || c == ';' || c == ':' || c == '\\' || c == '"'
                || c == '/' || c == '[' || c == ']' || c == '?' || c == '='
                || c == '{' || c == '}')
                return false;
        }
        return true;
    }
    /*
    static bool isForbiddenRequestHeader(const String& name)
    {
    HTTPHeaderName headerName;
    if (!findHTTPHeaderName(name, headerName))
    return false;

    switch (headerName) {
    case HTTPHeaderName::AcceptCharset:
    case HTTPHeaderName::AcceptEncoding:
    case HTTPHeaderName::AccessControlRequestHeaders:
    case HTTPHeaderName::AccessControlRequestMethod:
    case HTTPHeaderName::Connection:
    case HTTPHeaderName::ContentLength:
    case HTTPHeaderName::ContentTransferEncoding:
    case HTTPHeaderName::Cookie:
    case HTTPHeaderName::Cookie2:
    case HTTPHeaderName::Date:
    case HTTPHeaderName::DNT:
    case HTTPHeaderName::Expect:
    case HTTPHeaderName::Host:
    case HTTPHeaderName::KeepAlive:
    case HTTPHeaderName::Origin:
    case HTTPHeaderName::Referer:
    case HTTPHeaderName::TE:
    case HTTPHeaderName::Trailer:
    case HTTPHeaderName::TransferEncoding:
    case HTTPHeaderName::Upgrade:
    case HTTPHeaderName::UserAgent:
    case HTTPHeaderName::Via:
    return true;

    default:
    return false;
    }
    }
    */
    void XMLHttpRequest::setRequestHeader(const char* name, const char* value) 
    {
        if (m_state != OPENED) 
        {
            return;
        }
        if (!isValidHTTPToken(name) || !isValidHTTPHeaderValue(value)) 
        {
            return;
        }
        setRequestHeaderInternal(name, value);
        return;
    }
    void XMLHttpRequest::callReadyStateChangeListener() 
    {
        if (!m_funcOnStateChg.Empty()) {
            m_funcOnStateChg.Call((int)m_state);
        }
    }
    void XMLHttpRequest::setRequestHeaderInternal(const std::string& name, const std::string& value) 
    {
        m_requestHeaders[name] = value;
    }
    void XMLHttpRequest::abort() 
    {
    }
    void XMLHttpRequest::set_onreadystatechange(JSValueAsParam pObj)
    {
        m_funcOnStateChg.set(onstatchgid, this, pObj);
    }
    void _onPostComplete_JSThread(XMLHttpRequest* pxhr, char* p_Buff, int p_nLen, bool p_bBin, std::weak_ptr<int> cbref) 
    {
        if (!cbref.lock())
            return;       
        //检查一下js环境
        if (!pxhr->IsMyJsEnv())
            return;
        if (p_Buff) 
        {
            if (p_bBin) 
            {
#ifdef JS_V8
                v8::HandleScope scope(v8::Isolate::GetCurrent());
#endif
                JsValue ab = createJSAB(p_Buff, p_nLen);
                pxhr->m_jsfunPostComplete.Call(ab, p_Buff);
            }
            else 
            {
                if (p_nLen > 0)
                {
                    std::string strBuff;
                    unsigned char* pBuff = (unsigned char*)p_Buff;
                    if (p_nLen >= 3 && pBuff[0] == 0xef && pBuff[1] == 0xbb && pBuff[2] == 0xbf) {
                        strBuff.append(p_Buff + 3);
                    }
                    else strBuff = p_Buff;
                    pxhr->m_jsfunPostComplete.Call(strBuff);
                }
                else
                {
                    pxhr->m_jsfunPostComplete.Call("");
                }
            }
            delete[] p_Buff;
        }
        else 
        {
            pxhr->m_jsfunPostError.Call(-1);
        }
        pxhr->makeWeak();
    }
    void _onPostError_JSThread(XMLHttpRequest* pxhr, int curle, int httpresponse, std::weak_ptr<int> cbref)
    {
        if (!cbref.lock())
            return;
        if (!pxhr->IsMyJsEnv())
            return;
       
        pxhr->m_jsfunPostError.Call(curle, httpresponse);
        pxhr->makeWeak();
    }
    void _onPostError(XMLHttpRequest* xhr, IConchThreadCmdMgr* pPoster, int curle, int httpresponse, std::weak_ptr<int> cbref) 
    {
        pPoster->postToJS(std::bind(_onPostError_JSThread, xhr, curle, httpresponse, cbref));
    }
    void _onPostComplete(XMLHttpRequest* pxhr, bool bBin, IConchThreadCmdMgr* pPoster, JCBuffer& p_Buff, 
            const std::string&, const std::string&,int curlr, int httpr, const std::string&,std::weak_ptr<int> callbackref) 
    {
        //bool bin = pxhr->isBin();
        char* pBuff = new char[p_Buff.m_nLen + 1];//(bin?0:1)];
        memcpy(pBuff, p_Buff.m_pPtr, p_Buff.m_nLen);
        //if(!bin){
        pBuff[p_Buff.m_nLen] = 0;	//不管是不是二进制都保护一下
        if (curlr != 0 || httpr<200 || httpr>300) 
        {
            //错误
            pPoster->postToJS(std::bind(_onPostError_JSThread, pxhr, curlr, httpr, callbackref));
        }
        else
        {
            int len = p_Buff.m_nLen;
            pPoster->postToJS(std::bind(_onPostComplete_JSThread, pxhr, pBuff, len, bBin, callbackref));
        }
    }
    /*
    void XMLHttpRequest::set_onreadystatechange1(JsValue pObj)
    {
        v8::HandleScope sc(mpJsIso);
        bool isfunc = pObj->IsFunction();
        //v8::Persistent<v8::Function>* pFunc = new v8::Persistent<v8::Function>(pIso, v8::Local<v8::Function>::Cast(pObj));
        v8::Persistent<v8::Object>* ppf = weakHoldJsObj(v8::Local<v8::Object>::Cast(pObj));
        mpJsOnReadyStateChange = (JsFunction*) ppf; //是否危险，需要测试 // v8::Persistent<v8::Function>::Cast(*ppf);
    }
    */
    /*
    void XMLHttpRequest::setPostCB1(JsValue p_onOK, JsValue p_onError) 
    {
        v8::HandleScope sc(mpJsIso);
        mpJsPostComplete = (JsFunction*)weakHoldJsObj(v8::Local<v8::Object>::Cast(p_onOK));
        mpJsPostError = (JsFunction*)weakHoldJsObj(v8::Local<v8::Object>::Cast(p_onError));
        std::weak_ptr<int> cbref(m_CallbackRef);
        m_funcPostComplete = std::bind(_onPostComplete, this, std::placeholders::_1, cbref);
        m_funcPostError = std::bind(_onPostError, this, std::placeholders::_1, cbref);
    }
    */
    void XMLHttpRequest::postString(const char* p_pszUrl, const char* p_pszString, JSValueAsParam p_funOnOK, JSValueAsParam p_funOnErr) 
    {
        JCDownloadMgr* pdmgr = JCDownloadMgr::getInstance();
        m_jsfunPostError.set(onerrid, this, p_funOnErr);
        m_jsfunPostComplete.set(oncompleteid, this, p_funOnOK);
        if (!pdmgr) 
        {
            //error
            m_jsfunPostError.Call(-1);
            //if(p_funOnErr)p_funOnErr->Call(-1);
            return;
        }
        else 
        {
			makeStrong();

            std::weak_ptr<int> cbref(m_CallbackRef);
            pdmgr->postData(p_pszUrl, p_pszString, strlen(p_pszString), 
                std::bind(_onPostComplete, this, isBin(), m_pCmdPoster, 
                    std::placeholders::_1, 
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4,
                    std::placeholders::_5,
                    std::placeholders::_6,
                    cbref)
                );
        }
    }
    /*
    void XMLHttpRequest::postString1(const char* p_pszUrl, const char* p_pszString, JsValue p_funOnOK, JsValue p_funOnErr) {
    downloadMgr* pdmgr = downloadMgr::getInstance();
    if (!pdmgr) {
    if (mpJsPostError)
    callJsFunc(*mpJsPostError, -1);
    }
    else {
    std::weak_ptr<int> cbref(m_CallbackRef);
    pdmgr->postData(p_pszUrl, p_pszString, strlen(p_pszString), std::bind(_onPostComplete, this, std::placeholders::_1, cbref),
    std::bind(_onPostError, this, std::placeholders::_1, cbref));
    }
    }
    */
    void XMLHttpRequest::setPostCB(JSValueAsParam p_onOK, JSValueAsParam p_onError) 
    {
        m_jsfunPostComplete.set(oncompleteid, this, p_onOK);
        m_jsfunPostError.set(onerrid, this, p_onError);
        std::weak_ptr<int> cbref(m_CallbackRef);
        m_funcPostComplete = std::bind(_onPostComplete, this, isBin(),m_pCmdPoster, 
            std::placeholders::_1, 
            std::placeholders::_2,
            std::placeholders::_3,
            std::placeholders::_4,
            std::placeholders::_5,
            std::placeholders::_6,
            cbref);
    }
    void XMLHttpRequest::postData(JCDownloadMgr* pDMgr, const char* p_pszURL, char* p_pData, int p_nLen) 
    {
        if (p_pData && p_nLen) 
        {
            if (m_requestHeaders.size()>0) 
            {
                std::vector<std::string> headers;
                HTTPHeaderMap::iterator it = m_requestHeaders.begin();
                while (it != m_requestHeaders.end())
                {
                    std::string head = (*it).first;
                    head += ": ";//标准是可以有任意个空格
                    head += (*it).second;
                    headers.push_back(head);
                    it++;
                }
                pDMgr->postData(p_pszURL, p_pData, p_nLen, m_funcPostComplete, headers);
            }
            else
                pDMgr->postData(p_pszURL, p_pData, p_nLen, m_funcPostComplete);

			makeStrong();
        }
    }
    void XMLHttpRequest::getData(const char* p_sUrl) 
    {
        JCDownloadMgr* pdmgr = JCDownloadMgr::getInstance();
        if (!pdmgr)
        {
            m_pCmdPoster->postToJS(std::bind(_onPostError_JSThread, this, -1, 0, std::weak_ptr<int>(m_CallbackRef)));
            return;
        }
        else 
        {
            std::vector<std::string> headers;
            HTTPHeaderMap::iterator it = m_requestHeaders.begin();
            while (it != m_requestHeaders.end()) 
            {
                std::string head = (*it).first;
                head += ": ";//标准是可以有任意个空格
                head += (*it).second;
                headers.push_back(head);
                it++;
            }
            pdmgr->download(p_sUrl, 0, JCDownloadMgr::defProgressFunc, m_funcPostComplete,0,headers);
        }
    }
    void XMLHttpRequest::JsPostData(const char* p_sUrl, JSValueAsParam arg1)
    {
        JCDownloadMgr* pdmgr = JCDownloadMgr::getInstance();
        char* pData = NULL;
        int nDataLen = 0;
        if (!pdmgr)
        {
            m_pCmdPoster->postToJS(std::bind(_onPostError_JSThread, this, -1,0, std::weak_ptr<int>(m_CallbackRef)));
            return;
        }
        else
        {
            //判断是否为string类型
            if (__TransferToCpp<char *>::is(arg1))
            {
                pData = JS_TO_CPP(char*, arg1);
                if (pData)
                {
                    nDataLen = pData ? strlen(pData) : 0;
                    postData(pdmgr, p_sUrl, pData, nDataLen);
                }
            }
            else
            {
                int nABLen = 0;
                bool bisab = extractJSAB(arg1, pData, nDataLen);
                if (bisab)
                {
                    postData(pdmgr, p_sUrl, pData, nDataLen);
                    // m_responseTypeCode = ResponseTypeArrayBuffer; 不能根据这个来绝对返回类型
                }
                else
                {
                    LOGE("XMLHttpRequest::JsPostData postData failed!! param 2 is not a valid type!");
                }
            }
        }
    }
    void XMLHttpRequest::exportJS()
    {
        JSP_CLASS("_XMLHttpRequest", XMLHttpRequest);
        JSP_ADD_METHOD("get_readyState", XMLHttpRequest::getReadyState);
        JSP_ADD_METHOD("get_status", XMLHttpRequest::getStatus);
        JSP_ADD_METHOD("get_statusText", XMLHttpRequest::getStatusText);
        JSP_ADD_METHOD("get_responseURL", XMLHttpRequest::getResponseURL);
        JSP_ADD_PROPERTY(responseTypeCode, XMLHttpRequest, get_responseTypeCode, set_responseTypeCode);
        JSP_ADD_METHOD("_open", XMLHttpRequest::open);
        JSP_ADD_METHOD("setRequestHeader", XMLHttpRequest::setRequestHeader);
        JSP_ADD_METHOD("abort", XMLHttpRequest::abort);
        JSP_ADD_METHOD("get_upload", XMLHttpRequest::getUpload);
        JSP_ADD_METHOD("set_onreadystatechange", XMLHttpRequest::set_onreadystatechange);
        JSP_ADD_METHOD("_changeState", XMLHttpRequest::_changeState);
        JSP_ADD_METHOD("setPostCB", XMLHttpRequest::setPostCB);
        JSP_ADD_METHOD("postString", XMLHttpRequest::postString);
        JSP_ADD_METHOD("postData", XMLHttpRequest::JsPostData);
        JSP_ADD_METHOD("getData", XMLHttpRequest::getData);
        JSP_INSTALL_CLASS("_XMLHttpRequest", XMLHttpRequest);
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
