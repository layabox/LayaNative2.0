/**
@file			JSConsole.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_18
*/

//包含头文件
#include "JSConsole.h"
#include "../JSInterface/JSInterface.h"
#include "util/Log.h"
#include "util/JCMemorySurvey.h"

namespace laya 
{
    ADDJSCLSINFO(JSConsole, JSObjNode);
    JSConsole* JSConsole::m_spConsole = NULL;
    //------------------------------------------------------------------------------
    JSConsole::JSConsole()
    {
	    AdjustAmountOfExternalAllocatedMemory( 4 );
	    JCMemorySurvey::GetInstance()->newClass( "console",4,this );
    }
    //------------------------------------------------------------------------------
    JSConsole::~JSConsole()
    {
        m_spConsole = NULL;
        JCMemorySurvey::GetInstance()->releaseClass( "console",this );
    }
    //------------------------------------------------------------------------------
    JSConsole* JSConsole::GetInstance()
    {
	    if( m_spConsole == NULL )
	    {
		    m_spConsole =  new JSConsole();
	    }
	    return m_spConsole;
    }
    void JSConsole::DelInstance()
    {
	    if( m_spConsole ){
		    delete m_spConsole;
		    m_spConsole = NULL;
	    }
    }
    //------------------------------------------------------------------------------
    void JSConsole::log(int p_nType,const char* p_sBuffer )
    {
        LogLevel logLevel = (LogLevel)p_nType;
    #ifdef WIN32
	    if( p_sBuffer==NULL)
		    return;
	    int nLen = strlen( p_sBuffer ) + 3;
        if (nLen>3) {
            unsigned short* ucStr = new unsigned short[nLen];
            int nlen = UTF8StrToUnicodeStr((unsigned char*)p_sBuffer, ucStr, nLen);
            switch (logLevel)
            {
            case Warn:
                wprintf(L"warn:%s\n", (wchar_t *)ucStr);
                break;
            case Error:
                wprintf(L"error:%s\n", (wchar_t *)ucStr);
                break;
            default:
                wprintf(L"%s\n", (wchar_t *)ucStr);
                break;
            }
            delete[] ucStr;
            ucStr = NULL;
        }
    #elif __APPLE__
        switch (logLevel)
        {
            case Warn:
                LOGIExt(p_sBuffer);
                break;
            case Error:
                LOGIExt(p_sBuffer);
                break;
            default:
                LOGIExt(p_sBuffer);
                break;
        }
    #else
        switch (logLevel)
        {
        case Warn:
            LOGI(" %s", p_sBuffer);
            break;
        case Error:
            LOGI(" %s", p_sBuffer);
            break;
        default:
            LOGI(" %s", p_sBuffer);
            break;
        }
    #endif
    #ifdef JS_V8
	    if (gLayaLogNoParam) 
        {
		    v8::HandleScope hs(mpJsIso);
		    int flags = v8::StackTrace::kLineNumber | v8::StackTrace::kScriptNameOrSourceURL | v8::StackTrace::kFunctionName;
		    int wantcount = 1;
		    v8::Local<StackTrace> curstack = v8::StackTrace::CurrentStackTrace(mpJsIso, wantcount, (v8::StackTrace::StackTraceOptions)flags);
		    int count = curstack->GetFrameCount();
		    for (int i = 0; i < count; i++ ){
			    v8::Local<StackFrame> curfrm = curstack->GetFrame(i);

			    v8::Local<v8::String> fname = curfrm->GetFunctionName();
			    std::string fnamestr = *v8::String::Utf8Value(fname->ToString());
			    int ln = curfrm->GetLineNumber();
			    v8::Local<v8::String> scname = curfrm->GetScriptName();
			    std::string srcfile;
			    if (!scname.IsEmpty()) {
				    srcfile = *v8::String::Utf8Value(scname->ToString());
			    }
			    v8::Local<v8::String> srcurl = curfrm->GetScriptNameOrSourceURL();
			    if (!srcurl.IsEmpty()) {
				    srcfile = *v8::String::Utf8Value(srcurl->ToString());
			    }
			    gLayaLogNoParam(Info, srcfile.c_str(), ln, p_sBuffer);
		    }
		    //gLayaLog(Info,
	    }
    #endif
    }
    void JSConsole::exportJS() 
    {
        JSP_GLOBAL_CLASS("_console", JSConsole);
        JSP_ADD_METHOD("log", JSConsole::log);
        JSP_INSTALL_GLOBAL_CLASS("_console", JSConsole, JSConsole::GetInstance());
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
