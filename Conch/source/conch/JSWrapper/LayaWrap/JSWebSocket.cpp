/**
@file			JSWebSocket.cpp
@brief
@author			kuo.wang
@version		1.0
@date			2014_4_11
*/

//包含头文件
#include "JSWebSocket.h"
#include "../JSInterface/JSInterface.h"
#include <util/Log.h>
#include <util/JCCommonMethod.h>
#include <util/JCMemorySurvey.h>
#include "../../WebSocket/WebSocket.h"
#include "../../JCScriptRuntime.h"
#include <util/JCIThreadCmdMgr.h>

namespace laya
{
    ADDJSCLSINFO(JSWebSocket, JSObjNode);
    //------------------------------------------------------------------------------
    JSWebSocketDelegate::JSWebSocketDelegate(JSWebSocket* p_js_WebSocket) :jswsref(p_js_WebSocket->m_CallbackRef)
    {
        m_js_WebSocket = p_js_WebSocket;
        m_pCmdPoster = JCScriptRuntime::s_JSRT->m_pPoster;
    }
    void JSWebSocketDelegate::onOpen(WebSocket* ws)
    {
        LOGI("JSWebSocketDelegate::onOpen() this=%x ws=%x", (long)this,(long)m_js_WebSocket);
        std::string p_sEvent;
        m_js_WebSocket->closeTime = 0;
        m_pCmdPoster->postToJS(std::bind(&JSWebSocket::onSocketOpenCallJSFunction, m_js_WebSocket, p_sEvent, jswsref));
    }
    void JSWebSocketDelegate::onMessage(WebSocket* ws, const WebSocket::Data& data)
    {
        char* pDt = data.bytes;// new char[data.len];
        //memcpy(pDt, data.bytes, data.len);
        m_pCmdPoster->postToJS(std::bind(&JSWebSocket::onSocketMessageCallJSFunctionArrayBuffer, m_js_WebSocket, pDt, data.len, data.isBinary, jswsref));
    }
    void JSWebSocketDelegate::onClose(WebSocket* ws)
    {
        LOGI("JSWebSocketDelegate::onClose()this=%x ws=%x", (long)this, (long)m_js_WebSocket);
        std::string p_sEvent = "error";
        auto pFuncation = std::bind(&JSWebSocket::onSocketCloseCallJSFunction, m_js_WebSocket, p_sEvent,tmGetCurms(), jswsref);
        m_pCmdPoster->postToJS(pFuncation);
    }
    void JSWebSocketDelegate::onError(WebSocket* ws, const WebSocket::ErrorCode& error)
    {
        LOGW("JSWebSocketDelegate::onError( code=%d )this=%x ws=%x", error, (long)this, (long)m_js_WebSocket);
        if (m_js_WebSocket->m_nWebSocketState == WSS_OPEN)
        {
            LOGW("JSWebSocketDelegate::onError123( code=%d )this=%x ws=%x", error, (long)this, (long)m_js_WebSocket);
            std::string p_sEvent = "error";
            auto pFuncation = std::bind(&JSWebSocket::onSocketErrorCallJSFunction, m_js_WebSocket, p_sEvent, jswsref);
            m_pCmdPoster->postToJS(pFuncation);
        }
    }
    //------------------------------------------------------------------------------
    JSWebSocket::JSWebSocket()
    {
        m_CallbackRef.reset(new int(1));
        m_pCmdPoster = JCScriptRuntime::s_JSRT->m_pPoster;
        m_pWebSocket = NULL;
        m_pWebSocketDelegate = NULL;
        m_nBinaryType = Type_String;
        m_nWebSocketState = WSS_INIT;
        AdjustAmountOfExternalAllocatedMemory(1024);
        JCMemorySurvey::GetInstance()->newClass("webSocket", 1024, this);
    }
    //------------------------------------------------------------------------------
    JSWebSocket::JSWebSocket(const char* p_sUrl)
    {
        m_CallbackRef.reset(new int(1));
        m_pCmdPoster = JCScriptRuntime::s_JSRT->m_pPoster;
        m_pWebSocket = new WebSocket();
        closeTime = 0;
        m_pWebSocketDelegate = new JSWebSocketDelegate(this);
        LOGI("new JSWebSocket::this=%x deletgate=%x", (long)this, (long)m_pWebSocketDelegate);
        m_nBinaryType = Type_String;
        m_nWebSocketState = WSS_INIT;
        if (Init(p_sUrl))
        {
            m_nWebSocketState = WSS_OPEN;
        }
        else
        {
            m_nWebSocketState = WSS_CLOSE;
        }
        AdjustAmountOfExternalAllocatedMemory(1024);
        JCMemorySurvey::GetInstance()->newClass("webSocket", 1024, this);
    }
    //------------------------------------------------------------------------------
    JSWebSocket::~JSWebSocket()
    {
        LOGI("release JSWebSocket this=%x deletgate=%x", (long) this, (long)m_pWebSocketDelegate);
        if (m_pWebSocket) 
        {
            m_pWebSocket->clearDelegate();	//去掉delegate，防止发给js对象。因为跨线程之后，js对象已经删除了。
            m_pWebSocket->close();
        }
        //下面删除socket的时候会先同步关掉socket线程，所以不用考虑再有消息来的时候对象已经删除这种事情。
        LAYA_SAFE_DELETE(m_pWebSocket);
        LAYA_SAFE_DELETE(m_pWebSocketDelegate);
        JCMemorySurvey::GetInstance()->releaseClass("webSocket", this);
    }
    //------------------------------------------------------------------------------
    void JSWebSocket::onSocketOpenCallJSFunction(std::string p_sEvent, std::weak_ptr<int> cbref)
    {
        if (!cbref.lock())return;
        m_pJSFunctionOnOpen.Call(p_sEvent.c_str());
    }
    //------------------------------------------------------------------------------
    void JSWebSocket::onSocketCloseCallJSFunction(std::string p_sEvent, int64_t closetm, std::weak_ptr<int> cbref)
    {
        if (!cbref.lock())return;
        closeTime = closetm;
        LOGI("JSWebSocket::onSocketCloseCallJSFunction this=%x", (long) this);
        if (m_nWebSocketState == WSS_OPEN)
        {  
            m_nWebSocketState = WSS_CLOSE;
            m_pJSFunctionOnClose.Call(p_sEvent.c_str());
        }
        else
        {
            m_nWebSocketState = WSS_CLOSE;
        }
      
    }
    //------------------------------------------------------------------------------
    void JSWebSocket::onSocketErrorCallJSFunction(std::string p_sEvent, std::weak_ptr<int> cbref)
    {
        if (!cbref.lock())
            return;
        if (m_nWebSocketState == WSS_OPEN) 
        {
            m_nWebSocketState = WSS_CLOSE;
            m_pJSFunctionOnError.Call(p_sEvent.c_str());
        }
        else
        {
            m_nWebSocketState = WSS_CLOSE;
        }
    }
    void JSWebSocket::onSocketMessageCallJSFunctionArrayBuffer(const char* pBuf, int p_nLen, bool isBin, std::weak_ptr<int> cbref)
    {
        if (!cbref.lock()) return;
#ifdef JS_V8
        v8::HandleScope scope(v8::Isolate::GetCurrent());
#endif
        if (isBin)
        {
            JsValue ab = createJSAB((char*)pBuf, p_nLen);
            delete[] pBuf;
            m_pJSFunctionOnMessage.Call(ab);
        }
        else
        {
            std::string strMsg;
            strMsg.append(pBuf, p_nLen);	//怕有0的問題，就新建一個string了
            m_pJSFunctionOnMessage.Call(strMsg.c_str());
            delete[] pBuf;
        }
    }
    //------------------------------------------------------------------------------
    bool JSWebSocket::Init(const char* p_sUrl)
    {
        if (p_sUrl == NULL) return false;
        if (p_sUrl != NULL)
        {
            //ws->init(deleg, "ws://echo.websocket.org");
            m_pWebSocket->init(*m_pWebSocketDelegate, p_sUrl, NULL);
            return true;
        }
        return false;
    }
    //------------------------------------------------------------------------------
    void JSWebSocket::SetOnOpen(JSValueAsParam p_pFunction)
    {
        m_pJSFunctionOnOpen.set(onopenid, this, p_pFunction);
    }
    //------------------------------------------------------------------------------
    JsValue JSWebSocket::GetOnOpen()
    {
        return m_pJSFunctionOnOpen.getJsObj();
    }
    //------------------------------------------------------------------------------
    void JSWebSocket::SetOnMessage(JSValueAsParam p_pFunction)
    {
        m_pJSFunctionOnMessage.set(onmsgid, this, p_pFunction);
    }
    //------------------------------------------------------------------------------
    JsValue JSWebSocket::GetOnMessage()
    {
        return m_pJSFunctionOnMessage.getJsObj();
    }
    //------------------------------------------------------------------------------
    void JSWebSocket::SetOnClose(JSValueAsParam p_pFunction)
    {
        m_pJSFunctionOnClose.set(oncloseid, this, p_pFunction);
    }
    //------------------------------------------------------------------------------
    JsValue JSWebSocket::GetOnClose()
    {
        return m_pJSFunctionOnClose.getJsObj();
    }
    //------------------------------------------------------------------------------
    void JSWebSocket::SetOnError(JSValueAsParam p_pFunction)
    {
        m_pJSFunctionOnError.set(onerrid, this, p_pFunction);
    }
    //------------------------------------------------------------------------------
    JsValue JSWebSocket::GetOnError()
    {
        return m_pJSFunctionOnError.getJsObj();
    }
    //------------------------------------------------------------------------------
    void JSWebSocket::send(const char* p_sMessage)
    {
        if (m_nWebSocketState == WSS_OPEN)
        {
            if (m_pWebSocket)
            {
                m_pWebSocket->send(p_sMessage);
            }
        }
    }
    //------------------------------------------------------------------------------
    void JSWebSocket::close()
    {
        LOGI("JSWebSocket::close this=%x deletgate=%x", (long)this, (long)m_pWebSocketDelegate);
        if (m_nWebSocketState == WSS_OPEN)
        {
            if (m_pWebSocket)
            {
                m_nWebSocketState = WSS_CLOSEING;
                m_pWebSocket->close();
            }
        }
    }
    const char* JSWebSocket::getBinaryType() 
    {
        switch (m_nBinaryType) 
        {
        case Type_String:break;
        case Type_Blob:return "blob";
        case Type_ArrayBuffer:return "arraybuffer";
        }
        return NULL;
    }
    void JSWebSocket::setBinaryType(const char* p_pszBinaryType) 
    {
        if (!p_pszBinaryType)return;
        if (strcmp(p_pszBinaryType, "blob") == 0) 
        {
            m_nBinaryType = Type_Blob;
        }
        else if (strcmp(p_pszBinaryType, "arraybuffer") == 0)
        {
            m_nBinaryType = Type_ArrayBuffer;
        }
    }
    void JSWebSocket::JsSend(JSValueAsParam args)
    {
        enBinaryType type = m_nBinaryType;
        char* pABPtr = NULL;
        int nABLen = 0;
        if (__TransferToCpp<char *>::is(args))
        {
            type = Type_String;
        }
        else
        {
            bool bisab = extractJSAB(args, pABPtr, nABLen);
            type = bisab ? Type_ArrayBuffer : Type_Unknown;
        }
        switch (type)
        {
        case Type_ArrayBuffer:
            if (m_nWebSocketState == WSS_OPEN && m_pWebSocket)
            {
                m_pWebSocket->send((const unsigned char*)pABPtr, (unsigned int)nABLen);
            }
            break;
        case Type_String:
            if (m_nWebSocketState == WSS_OPEN && m_pWebSocket)
            {
                char* sColor = JS_TO_CPP(char*, args);
                if (sColor)
                {
                    m_pWebSocket->send(sColor);
                }
            }
            break;
        default:
            LOGW("JSWebSocket::JsSend send不支持的参数!");
            break;
        }
    }
    int JSWebSocket::getTimeGap()
    {
        if (closeTime == 0)
            return 0;
        else
            return int(tmGetCurms() - closeTime);
    }
    void JSWebSocket::exportJS()
    {
        JSP_CLASS("ConchWebSocket", JSWebSocket);
        JSP_ADD_PROPERTY(onopen, JSWebSocket, GetOnOpen, SetOnOpen);
        JSP_ADD_PROPERTY(_onmessage, JSWebSocket, GetOnMessage, SetOnMessage);
        JSP_ADD_PROPERTY(onclose, JSWebSocket, GetOnClose, SetOnClose);
        JSP_ADD_PROPERTY(onerror, JSWebSocket, GetOnError, SetOnError);
        JSP_ADD_PROPERTY(binaryType, JSWebSocket, getBinaryType, setBinaryType);
        JSP_ADD_PROPERTY_RO(timegap, JSWebSocket, getTimeGap);
        JSP_ADD_PROPERTY_RO(readyState, JSWebSocket, getReadState);
        JSP_ADD_METHOD("close", JSWebSocket::close);
        JSP_ADD_METHOD("send", JSWebSocket::JsSend);
        JSP_REG_CONSTRUCTOR(JSWebSocket, const char*);
        JSP_INSTALL_CLASS("ConchWebSocket", JSWebSocket);
    }
}
//-----------------------------END FILE--------------------------------