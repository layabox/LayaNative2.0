
#ifndef _JSCProxyTLS_h
#define _JSCProxyTLS_h

#include <JavaScriptCore/JSContextRef.h>
#include <JavaScriptCore/JSStringRef.h>
#include <JavaScriptCore/JSValueRef.h>
#include "../../../CToObjectC.h"
#include <util/JCCommonMethod.h>
#include <util/Log.h>
#include <pthread.h>
namespace laya
{
    class __TlsData
    {
        __TlsData(){}
        ~__TlsData(){}
    public:
        static pthread_key_t s_tls_curThread;
        static __TlsData *GetInstance()
        {
            static __TlsData _Ins;
            return &_Ins;
        }
        
        void *SetCurContext( JSContextRef p_pContext )
        {
            void *pRet = (void *)pthread_getspecific(s_tls_curThread);
            pthread_setspecific(s_tls_curThread,(void*)p_pContext);
            return pRet;
        }
        
        JSContextRef GetCurContext()
        {
            return (JSContextRef)pthread_getspecific(s_tls_curThread);
        }
    };
    
   
    
    class __JsThrow
    {
        JSValueRef *m_pException;
        
    public:
        __JsThrow()
        {
            m_pException = 0;
        }
        
        static __JsThrow *GetInstance()
        {
            static __JsThrow _Ins;
            return &_Ins;
        }
        
        static void Throw( JSValueRef* exception, const char *p_pszInfo=0 )
        {
            if( 0 != exception )
            {
                JSStringRef message = JSStringCreateWithUTF8CString((0==p_pszInfo)?"unknown error":p_pszInfo);
                *exception = JSValueMakeString(__TlsData::GetInstance()->GetCurContext(), message);
                JSStringRelease(message);
            }
        }
        
        void UpdateException( JSValueRef* exception )
        {
            m_pException = exception;
        }
        
        void RuntimeThrow( const char *p_pszInfo )
        {
            __JsThrow::Throw(m_pException,p_pszInfo);
        }
    };
    extern bool gbAlertException;
    class __JSRun
    {
        static char *__ToCppString( JSValueRef p_vl, JSContextRef p_pContext )
        {
            char *pRet = 0;
            JSStringRef _jsStr = JSValueToStringCopy(p_pContext, p_vl, 0);
            if( _jsStr )
            {
                size_t iSize = JSStringGetMaximumUTF8CStringSize(_jsStr);
                if(iSize>0)
                {
                    pRet=new char[iSize+1];
                    JSStringGetUTF8CString( _jsStr, pRet, iSize+1 );
                }
                JSStringRelease( _jsStr );
            }
            return pRet;
        }
        
    public:
        static void OutputException( JSValueRef exception )
        {
            if( NULL != exception )
            {
                JSContextRef pContext = __TlsData::GetInstance()->GetCurContext();
                
                JSStringRef jsLinePropertyName = JSStringCreateWithUTF8CString("line");
                JSStringRef jsColumnPropertyName = JSStringCreateWithUTF8CString("column");
                JSStringRef jsUrlPropertyName = JSStringCreateWithUTF8CString("sourceURL");
                JSObjectRef exObject = JSValueToObject( pContext, exception, NULL );
                JSValueRef line = JSObjectGetProperty( pContext, exObject, jsLinePropertyName, NULL );
                JSValueRef column = JSObjectGetProperty( pContext, exObject, jsColumnPropertyName, NULL );
                JSValueRef url = JSObjectGetProperty( pContext, exObject, jsUrlPropertyName, NULL );
                char *pEx = __ToCppString(exception,pContext);
                char *pLine = __ToCppString(line,pContext);
                char *pColumn = __ToCppString(column,pContext);
                char *pUrl = __ToCppString(url,pContext);
                
                //通知全局错误处理脚本
                
                std::string kBuf = "if(conch.onerror){conch.onerror('";
                kBuf += UrlEncode(pEx);
                kBuf += "','undefined','";
                kBuf += UrlEncode(pLine);
                kBuf += "','";
                kBuf += UrlEncode(pColumn);
                kBuf += "','";
                kBuf += UrlEncode(pEx);
                kBuf += "');};";
                __JSRun::Run(kBuf.c_str());
                
                static char sBuffer[10240]={0};
                sprintf( sBuffer,"exception info: [%s] at line %s\n", pEx, pLine );
                
                if (gbAlertException)
                    CToObjectCAlert( sBuffer );
                else {
                    LOGE("==JSERROR:\n%s", sBuffer);
                }
                
                delete[] pEx;
                delete[] pLine;
                delete[] pColumn;
                delete[] pUrl;
                JSStringRelease(jsLinePropertyName);
                JSStringRelease(jsColumnPropertyName);
                JSStringRelease(jsUrlPropertyName);
            }
        }
        
        static bool Run( const char *p_pszScript,JSValueRef* result = NULL )
        {
            bool bRet = true;
            
            JSContextRef pContext = __TlsData::GetInstance()->GetCurContext();
            JSStringRef pScript = JSStringCreateWithUTF8CString( p_pszScript );
            
            JSValueRef exception = NULL;
            JSValueRef ret = JSEvaluateScript(pContext, pScript, NULL, NULL, 0, &exception );
            if( 0 != exception )
            {
                OutputException( exception );
                bRet = false;
            }
            if( ret != NULL && result != NULL ){
                *result = ret;
            }
            JSStringRelease( pScript );
            
            return bRet;
        }
    };
}

#endif
