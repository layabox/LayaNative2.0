/**
@file			JSInterface.h
@brief
@author			wyw
@version		1.0
@date			2013_12_2
*/

#ifndef __JSInterface_H__
#define __JSInterface_H__

#include "JSObjBase.h"
#include <util/JCMemorySurvey.h>
#ifdef JS_JSC
    #include "JSC/JSCBinder.h"
    #include "JSC/JSCEnv.h"
    #include "JSC/JSCArrayBuffer.h"
#elif JS_V8
    #include <v8.h>
    #include "V8/JSEnv.h"
    #include "V8/JSArrayBuffer.h"
    #include "V8/JsBinder.h"
#endif

namespace laya
{
#ifdef JS_JSC
    struct JsFuncArgs {};
    typedef JSValueRef  JSValueAsParam;
    typedef JSValueRef  JsValue;
    typedef JsObjHandleJSC  JsObjHandle;
    #define JSP_RESET_GLOBAL_FUNCTION JSCGlobal::getInstance()->reset()
    #define JS_TO_CPP(tp,v) laya::__TransferToCpp<tp>::ToCpp(v)
    #define JSP_TO_JS_BYTE_ARRAY(vl,sz) (laya::__JsByteArray::ToJsByteArray(vl,sz))
    #define JSP_TO_JS(tp,v) (laya::__TransferToJs<tp>::ToJs(v))
    #define JSP_TO_JS_NULL  JSP_TO_JS(void,1)
    #define JSP_TO_JS_UNDEFINE  JSP_TO_JS(void,0)
    #define JSP_THROW(str) (laya::__JsThrow::GetInstance()->RuntimeThrow(str))
    #define JSP_RUN_SCRIPT(script) (laya::__JSRun::Run(script))
    #define JSP_TO_JS_STR(str) (laya::__TransferToJs<const char*>::ToJs(str))
    class JsObjBase : public JSObjBaseJSC {};
    #define JS_TRY
    #define JS_CATCH
#elif JS_V8
    class JsObjBase :public JSObjBaseV8
    {
    };
    #define JSValueAsParam  JsValue
    #define JSP_THROW(str) 	__JsThrow::	Throw(str);
    #define JSP_RUN_SCRIPT(script)   laya::__JSRun::Run(script);
    #define JSP_TO_JS_NULL	((v8::Null(v8::Isolate::GetCurrent())))
    #define JSP_TO_JS_UNDEFINE ((v8::Undefined(v8::Isolate::GetCurrent())))
    #include "V8/JSCProxyTrnasfer.h"
    #define JSP_TO_JS_BYTE_ARRAY(vl,sz) (__JsByteArray::ToJsByteArray(vl,sz))
    #define JSP_TO_JS(tp,v) (__TransferToJs<tp>::ToJs(v))
    #define JS_TO_CPP(tp,v) (__TransferToCpp<tp>::ToCpp(v))
    #define JSP_TO_JS_STR(str) (v8::String::NewFromUtf8(v8::Isolate::GetCurrent(),str).ToLocalChecked())
    #define JS_TRY \
	    v8::Isolate* isolate = v8::Isolate::GetCurrent();\
	    v8::HandleScope handle_scope(isolate);\
	    v8::TryCatch try_catch(isolate);

    #define JS_CATCH \
	    if (try_catch.HasCaught()){\
	        LOGE("JS onFrame error\n");\
	        __JSRun::ReportException(isolate, &try_catch);\
	    }
#endif
    void AdjustAmountOfExternalAllocatedMemory(int p_nMemorySize);
    JsValue getNativeObj(JSValueAsParam p_pJsObj, char* p_strName);

}
#endif //__JSInterface_H__

//-----------------------------END FILE--------------------------------