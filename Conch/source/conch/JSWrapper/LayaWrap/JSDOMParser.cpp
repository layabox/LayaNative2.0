/**
@file			JSDOMParser.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_18
*/

//包含头文件
#include "JSDOMParser.h"
#include <downloadMgr/JCDownloadMgr.h>
#include "../../JCScriptRuntime.h"
#include <util/Log.h>
#include <util/JCIThreadCmdMgr.h>

#define OnLoadJSFunctionID 0
#define OnErrorJSFunctionID 1

namespace laya
{
    ADDJSCLSINFO(JSDOMParser, JSObjNode);
    JSDOMParser::JSDOMParser()
    {
        jsDOC = NULL;
        m_CallbackRef.reset(new int(1));
        AdjustAmountOfExternalAllocatedMemory(640000);
        JCMemorySurvey::GetInstance()->newClass("DOMParser", 640000, this);
    }
    JSDOMParser::~JSDOMParser()
    {
        JCMemorySurvey::GetInstance()->releaseClass("DOMParser", this);
        if (jsDOC != NULL)
        {
            jsDOC = NULL;
        }
    }
    JsValue JSDOMParser::parseFromString(const char *str, const char *type)
    {
        jsDOC = new JSXmlDocument();
        jsDOC->parse(str);
        return JSP_TO_JS(JSXmlDocument*, jsDOC);
    }
    const char* JSDOMParser::getSrc()
    {
        return m_sUrl.c_str();
    }
    //这个可能在下载线程，所以不能是成员对象。
    void JSDOM_onDownloadOK(JSDOMParser* pThis, JCResStateDispatcher* p_pRes, std::weak_ptr<int>& callbackref)
    {
        JCFileRes* pFileRes = (JCFileRes*)p_pRes;
        if (pFileRes->m_pBuffer != NULL)
        {
            int length = pFileRes->m_nLength;
            std::string xmlstr;
            xmlstr.assign(pFileRes->m_pBuffer.get(), length);
            auto pFunction = std::bind(&JSDOMParser::onLoadedCallJSFunction, pThis, xmlstr, callbackref);
            JCScriptRuntime::s_JSRT->m_pPoster->postToJS(pFunction);
        }
        else
        {
            auto pFunction = std::bind(&JSDOMParser::onErrorCallJSFunction, pThis, -1, callbackref);
            JCScriptRuntime::s_JSRT->m_pPoster->postToJS(pFunction);
        }
    }
    void JSDOM_onDownloadError(JSDOMParser* pThis, JCResStateDispatcher*, int e, std::weak_ptr<int>& callbackref)
    {
        auto pFunction = std::bind(&JSDOMParser::onErrorCallJSFunction, pThis, e, callbackref);
        JCScriptRuntime::s_JSRT->m_pPoster->postToJS(pFunction);
    }
    void JSDOMParser::setSrc(const char* p_sSrc)
    {
        m_sUrl = p_sSrc;
        std::weak_ptr<int> cbref(m_CallbackRef);
        JCFileRes* pRes =JCScriptRuntime::s_JSRT->m_pFileResMgr->getRes(m_sUrl);
        pRes->setOnReadyCB(std::bind(JSDOM_onDownloadOK, this, std::placeholders::_1, cbref));
        pRes->setOnErrorCB(std::bind(JSDOM_onDownloadError, this, std::placeholders::_1, std::placeholders::_2, cbref));
    }
    void JSDOMParser::SetOnload(JSValueAsParam p_pFunction)
    {
        m_pOnLoadJSFunction.set(OnLoadJSFunctionID, this, p_pFunction);
    }
    JsValue JSDOMParser::GetOnload()
    {
        return m_pOnLoadJSFunction.getJsObj();
    }
    void JSDOMParser::SetOnError(JSValueAsParam p_pFunction)
    {
        m_pOnErrorJSFunction.set(OnErrorJSFunctionID, this, p_pFunction);
    }
    JsValue JSDOMParser::GetOnError()
    {
        return m_pOnErrorJSFunction.getJsObj();
    }
    void JSDOMParser::onLoadedCallJSFunction(std::string& str, std::weak_ptr<int>& callbackref)
    {
        if (!callbackref.lock())return;
        LOGI("download xml file seccuss! %s\n", m_sUrl.c_str());
        jsDOC = new JSXmlDocument();
        jsDOC->parse(str.c_str());
        m_pOnLoadJSFunction.Call();
    }
    void JSDOMParser::onErrorCallJSFunction(int e, std::weak_ptr<int>& callbackref)
    {
        if (!callbackref.lock()) return;
        m_pOnErrorJSFunction.Call(e);
    }
    JsValue JSDOMParser::getXml()
    {
        return JSP_TO_JS(JSXmlDocument*, jsDOC);
    }
    void JSDOMParser::exportJS()
    {
        JSP_CLASS("_DOMParser", JSDOMParser);
        JSP_ADD_METHOD("parseFromString", JSDOMParser::parseFromString);
        JSP_ADD_PROPERTY(src, JSDOMParser, getSrc, setSrc);
        JSP_ADD_PROPERTY(onload, JSDOMParser, GetOnload, SetOnload);
        JSP_ADD_PROPERTY(onerror, JSDOMParser, GetOnError, SetOnError);
        JSP_ADD_METHOD("getResult", JSDOMParser::getXml);
        JSP_INSTALL_CLASS("_DOMParser", JSDOMParser);
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------