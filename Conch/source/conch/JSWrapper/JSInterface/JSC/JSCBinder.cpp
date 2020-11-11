#ifdef JS_JSC
#include "JSCBinder.h"

pthread_key_t	JSClassMgr::s_tls_curThread;

JSObjBaseJSC::JSObjBaseJSC() {
    m_bWeakThis = true;
}

JSObjBaseJSC::~JSObjBaseJSC() {
    if (!m_bWeakThis){
        JSContextRef ctx = __TlsData::GetInstance()->GetCurContext();
        JSValueUnprotect(ctx, mpJsThis);
    }
}

void JSObjBaseJSC::retainThis() {
	JSContextRef ctx = __TlsData::GetInstance()->GetCurContext();
	JSValueProtect(ctx, mpJsThis);
}

void JSObjBaseJSC::releaseThis() {
	JSContextRef ctx = __TlsData::GetInstance()->GetCurContext();
	JSValueUnprotect(ctx, mpJsThis);
}


void JSObjBaseJSC::createJSObj(){
    JSContextRef ctx = __TlsData::GetInstance()->GetCurContext();
    mpJsThis = JSObjectMake(ctx,nullptr,nullptr);
    JSValueProtect(ctx, mpJsThis);
    m_bWeakThis = false;
}

JSValueRef JSObjBaseJSC::callJsFunc(JSValueRef func) {
	return _callJsFunc(func, 0, nullptr);
}

bool JsObjHandleJSC::Empty() {
	if (!m_pObj)
		return true;
    return m_pValue == nullptr;
}
#endif
