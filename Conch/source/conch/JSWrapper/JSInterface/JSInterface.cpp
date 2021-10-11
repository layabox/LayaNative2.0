/**
@file			JSInterface.cpp
@brief
@author			wyw
@version		1.0
@date			2014_7_29
*/

#include "JSInterface.h"
#ifdef JS_JSC
    //#include <JavaScriptCore/JSBasePrivate.h>
#endif

namespace laya 
{
    void AdjustAmountOfExternalAllocatedMemory(int p_nMemorySize)
    {
#ifdef JS_V8
        v8::Isolate::GetCurrent()->AdjustAmountOfExternalAllocatedMemory(p_nMemorySize);
#elif JS_JSC
        //JSReportExtraMemoryCost(__TlsData::GetInstance()->GetCurContext(), p_nMemorySize);
#endif
    }
    JsValue getNativeObj(JSValueAsParam p_pJsObj, char* p_strName) 
    {
#ifdef JS_V8
        if (p_pJsObj->IsObject()) {
			v8::Isolate* isolate = v8::Isolate::GetCurrent();
			v8::Local< v8::Context> context = isolate->GetCurrentContext();
            v8::Local<v8::Object> pobj = v8::Local<v8::Object>::Cast(p_pJsObj);
            JsValue nativeObj = pobj->Get(context, JSP_TO_JS(char*, p_strName)).ToLocalChecked();
            if (!nativeObj.IsEmpty() && nativeObj->IsObject()) {
                return nativeObj;
            }
        }
        return p_pJsObj;
#elif JS_JSC
        JSContextRef ctx = __TlsData::GetInstance()->GetCurContext();
        if (JSValueIsObject(ctx, p_pJsObj)) {
            JSObjectRef obj = JSValueToObject(ctx, p_pJsObj, nullptr);
            if (obj != nullptr) {
                JSStringRef name = JSStringCreateWithUTF8CString(p_strName);
                JSValueRef ret = JSObjectGetProperty(ctx, obj, name, nullptr);
                JSStringRelease(name);
                if (!JSValueIsNull(ctx, ret) && !JSValueIsUndefined(ctx, ret)) {
                    return ret;
                }
            }
        }
        return p_pJsObj;
#endif
    }
}



//-----------------------------END FILE--------------------------------
