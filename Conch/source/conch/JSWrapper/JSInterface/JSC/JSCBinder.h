#ifndef _JSC_BINDER_H__
#define _JSC_BINDER_H__
#include <JavaScriptCore/JSObjectRef.h>
#include "JSCProxyTransfer.h"
#include "JSCProxyType.h"
#include "JSCProxyClass.h"
#include <pthread.h>
using namespace laya;

class JSClassMgr{
public:
    static pthread_key_t s_tls_curThread;
	typedef void (*RESETFUNC)();
	static JSClassMgr*   GetInstance(){
         JSClassMgr* pIns =(JSClassMgr*)pthread_getspecific(s_tls_curThread);
		if(!pIns){
            pIns = new JSClassMgr();
            pthread_setspecific(s_tls_curThread,(void*)pIns);
		}
		return pIns;
	}
	std::vector<RESETFUNC> allCls;
	void resetAllRegClass(){
		for( int i=0,sz=allCls.size(); i<sz; i++){
			allCls[i]();
		}
		allCls.clear();
	}
};

#define JSP_GLOBAL_OBJECT \
    JSContextGetGlobalObject(__TlsData::GetInstance()->GetCurContext())

#define JSP_GLOBAL_START1()

#define JSP_ADD_GLOBAL_FUNCTION(name, func, ...) \
    JSCGlobal::getInstance()->addFunction( #name, func );

#define JSP_ADD_GLOBAL_PROPERTY(name, v) \
    JSCGlobal::getInstance()->addProperty( #name, v );




#define JSP_CLASS(name, cls) \
    JSCClass<cls>* pJSCClass = JSCClass<cls>::getInstance();

#define JSP_GLOBAL_CLASS(name, cls, inst) \
    JSCClass<cls>* pJSCClass = JSCClass<cls>::getInstance();

#define JSP_ADD_FIXED_PROPERTY(name,cls,val) \
   pJSCClass->addFixedProperty(#name,val)
	
#define JSP_ADD_METHOD(name,fn) \
    pJSCClass->addMethod(name,&fn);

#define JSP_ADD_PROPERTY_RO(name,cls,getter) \
    pJSCClass->addProperty(#name,&cls::getter,nullptr);

#define JSP_ADD_PROPERTY(name,cls,getter,setter) \
    pJSCClass->addProperty(#name,&cls::getter,&cls::setter);

#define JSP_REG_CONSTRUCTOR(cls,...) \
    pJSCClass->addConstructor(laya::regConstructor<cls,##__VA_ARGS__>());
  
#define JSP_GLOBAL_ADD_METHOD(name,fn) \
    pJSCClass->addMethod(name,&fn);

#define JSP_GLOBAL_ADD_PROPERTY_RO(name,cls,getter) \
    pJSCClass->addProperty(#name,&cls::getter,nullptr);

#define JSP_GLOBAL_ADD_PROPERTY(name,cls,getter,setter) \
    pJSCClass->addProperty(#name,&cls::getter,&cls::setter);

#define JSP_INSTALL_CLASS(name,cls) \
    pJSCClass->finish(name); \
    JSClassMgr::GetInstance()->allCls.push_back(JSCClass<cls>::reset);

#define JSP_INSTALL_GLOBAL_CLASS(name,cls,inst) \
    pJSCClass->finishToGlobal(name,inst); \
    JSClassMgr::GetInstance()->allCls.push_back(JSCClass<cls>::reset);





class JSObjBaseJSC {
	friend class JsObjHandleJSC;
public:
	JSObjBaseJSC();
	virtual ~JSObjBaseJSC();

	void makeWeak();
	void makeStrong();

	bool IsMyJsEnv() {
		return true;
	}

	inline JSValueRef _callJsFunc(JSValueRef func, size_t argumentCount, const JSValueRef arguments[]) {
        JSContextRef ctx = __TlsData::GetInstance()->GetCurContext();
        JSValueRef exception = nullptr;
        JSObjectRef funcObj = JSValueToObject(ctx,func,&exception);
        if (exception != nullptr){
            __JSRun::OutputException(exception);
        }
        JSValueRef ret = JSObjectCallAsFunction(ctx,funcObj,mpJsThis,argumentCount,arguments,&exception);
        if (exception != nullptr){
            __JSRun::OutputException(exception);
        }
        return ret;
	}
    
	JSValueRef callJsFunc(JSValueRef func);

	template<class P1>
	JSValueRef callJsFunc(JSValueRef func, P1 p1) {
		int argc = 1;
		JSValueRef argv[1];
		argv[0] = __TransferToJs<P1>::ToJs(p1);
		return _callJsFunc(func, argc, argv);
	}

	template<class P1, class P2>
	JSValueRef callJsFunc(JSValueRef func, P1 p1, P2 p2) {
		int argc = 2;
		JSValueRef argv[2];
		argv[0] = __TransferToJs<P1>::ToJs(p1);
		argv[1] = __TransferToJs<P2>::ToJs(p2);
		return _callJsFunc(func, argc, argv);
	}
	template<class P1, class P2, class P3>
	JSValueRef callJsFunc(JSValueRef func, P1 p1, P2 p2, P3 p3) {
		int argc = 3;
		JSValueRef argv[3];
		argv[0] = __TransferToJs<P1>::ToJs(p1);
		argv[1] = __TransferToJs<P2>::ToJs(p2);
		argv[2] = __TransferToJs<P3>::ToJs(p3);
		return _callJsFunc(func, argc, argv);
	}
	template<class P1, class P2, class P3, class P4>
	JSValueRef callJsFunc(JSValueRef func, P1 p1, P2 p2, P3 p3, P4 p4) {
		int argc = 4;
		JSValueRef argv[4];
		argv[0] = __TransferToJs<P1>::ToJs(p1);
		argv[1] = __TransferToJs<P2>::ToJs(p2);
		argv[2] = __TransferToJs<P3>::ToJs(p3);
		argv[3] = __TransferToJs<P4>::ToJs(p4);
		return _callJsFunc(func, argc, argv);
	}
	template<class P1, class P2, class P3, class P4, class P5>
	JSValueRef callJsFunc(JSValueRef func, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) {
		int argc = 5;
		JSValueRef argv[5];
		argv[0] = __TransferToJs<P1>::ToJs(p1);
		argv[1] = __TransferToJs<P2>::ToJs(p2);
		argv[2] = __TransferToJs<P3>::ToJs(p3);
		argv[3] = __TransferToJs<P4>::ToJs(p4);
		argv[4] = __TransferToJs<P5>::ToJs(p5);
		return _callJsFunc(func, argc, argv);
	}
	template<class P1, class P2, class P3, class P4, class P5, class P6>
	JSValueRef callJsFunc(JSValueRef func, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) {
		int argc = 6;
		JSValueRef argv[6];
		argv[0] = __TransferToJs<P1>::ToJs(p1);
		argv[1] = __TransferToJs<P2>::ToJs(p2);
		argv[2] = __TransferToJs<P3>::ToJs(p3);
		argv[3] = __TransferToJs<P4>::ToJs(p4);
		argv[4] = __TransferToJs<P5>::ToJs(p5);
		argv[5] = __TransferToJs<P6>::ToJs(p6);
		return _callJsFunc(func, argc, argv);
	}
	template<class P1, class P2, class P3, class P4, class P5, class P6, class P7>
	JSValueRef callJsFunc(JSValueRef func, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) {
		int argc = 7;
		JSValueRef argv[7];
		argv[0] = __TransferToJs<P1>::ToJs(p1);
		argv[1] = __TransferToJs<P2>::ToJs(p2);
		argv[2] = __TransferToJs<P3>::ToJs(p3);
		argv[3] = __TransferToJs<P4>::ToJs(p4);
		argv[4] = __TransferToJs<P5>::ToJs(p5);
		argv[5] = __TransferToJs<P6>::ToJs(p6);
		argv[6] = __TransferToJs<P7>::ToJs(p7);
		return _callJsFunc(func, argc, argv);
	}
	template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
	JSValueRef callJsFunc(JSValueRef func, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8) {
		int argc = 8;
		JSValueRef argv[8];
		argv[0] = __TransferToJs<P1>::ToJs(p1);
		argv[1] = __TransferToJs<P2>::ToJs(p2);
		argv[2] = __TransferToJs<P3>::ToJs(p3);
		argv[3] = __TransferToJs<P4>::ToJs(p4);
		argv[4] = __TransferToJs<P5>::ToJs(p5);
		argv[5] = __TransferToJs<P6>::ToJs(p6);
		argv[6] = __TransferToJs<P7>::ToJs(p7);
		argv[7] = __TransferToJs<P8>::ToJs(p8);
		return _callJsFunc(func, argc, argv);
	}
	template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10, class P11, class P12>
	JSValueRef callJsFunc(JSValueRef func, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11, P12 p12) {
		int argc = 12;
		JSValueRef argv[12];
		argv[0] = __TransferToJs<P1>::ToJs(p1);
		argv[1] = __TransferToJs<P2>::ToJs(p2);
		argv[2] = __TransferToJs<P3>::ToJs(p3);
		argv[3] = __TransferToJs<P4>::ToJs(p4);
		argv[4] = __TransferToJs<P5>::ToJs(p5);
		argv[5] = __TransferToJs<P6>::ToJs(p6);
		argv[6] = __TransferToJs<P7>::ToJs(p7);
		argv[7] = __TransferToJs<P8>::ToJs(p8);
		argv[8] = __TransferToJs<P8>::ToJs(p9);
		argv[9] = __TransferToJs<P8>::ToJs(p10);
		argv[10] = __TransferToJs<P8>::ToJs(p11);
		argv[11] = __TransferToJs<P8>::ToJs(p12);
		return _callJsFunc(func, argc, argv);
	}
public:
	JSObjectRef			mpJsThis;
};

class JsObjHandleJSC
{
public:
	JsObjHandleJSC() {
		m_pObj = nullptr;
        m_pValue = nullptr;
        m_pReturn = nullptr;
	}
    ~JsObjHandleJSC(){
        JSContextRef ctx = __TlsData::GetInstance()->GetCurContext();
        if (m_pValue != nullptr && ctx != nullptr)
        {
            JSValueUnprotect(ctx, m_pValue);
        }
    }
	JSObjBaseJSC*	m_pObj;
    JSValueRef      m_pValue;
	JSValueRef		m_pReturn;
    bool Empty();

    JSValueRef getJsObj() {
        return m_pValue;
	}

	void set(int id, JSObjBaseJSC* pobj, JSValueRef v) {
		if (m_pValue != nullptr) {
			Reset();
		}
		m_pValue = v;
		JSValueProtect(__TlsData::GetInstance()->GetCurContext(), m_pValue);
		m_pObj = pobj;

	}
	void set(JSValueRef v) {
		if (m_pValue != nullptr) {
			Reset();
		}
		m_pValue = v;
		JSValueProtect(__TlsData::GetInstance()->GetCurContext(), m_pValue);

	}
	template <typename _Tp>
	static bool IsTypeof(JSValueRef val) {
		if (val == nullptr)
			return false;
        JSContextRef ctx = __TlsData::GetInstance()->GetCurContext();
		__InferType<_Tp> _it;
		switch (_it.iType) {
		case __VT_void:
			return false;
		case __VT_string:
			return JSValueIsString(ctx, val);
		case __VT_bool:
            return JSValueIsBoolean(ctx, val);
		case __VT_int:
		case __VT_long:
		case __VT_float:
		case __VT_double:
		case __VT_longlong:
			return JSValueIsNumber(ctx,val);
		default: {
            return __CheckClassType::IsTypeOf<_Tp>(val);
        }
        }
	}

	template <typename _Tp>
	bool IsTypeof() {
		return IsTypeof<_Tp>(m_pValue);
	}

	template <typename _R>
	static bool tryGet(JSValueRef val, _R **pRet) {
		if (val == nullptr) {
			*pRet = 0;
			return false;
		}
		if (!__TransferToCpp<_R>::is(val))
			return false;
		*pRet = __TransferToCpp<_R*>::ToCpp(val);
		return true;
	}

	static bool tryGetStr(JSValueRef val, char** ppRet) {
		if (JSValueIsString(__TlsData::GetInstance()->GetCurContext(), val)) {
            *ppRet = JsCharToC(val);
			return true;
		}
		return false;
	}
    
	void Reset() {
        JSContextRef ctx = __TlsData::GetInstance()->GetCurContext();
        if (m_pValue != nullptr && ctx != nullptr){
            JSValueUnprotect(ctx, m_pValue);
            m_pValue = nullptr;
        }
	}
	void __BindThis(JsObjHandleJSC &p_This) {}
	bool IsFunction() {
        if (!m_pObj)return false;
        JSContextRef ctx = __TlsData::GetInstance()->GetCurContext();
        JSObjectRef valueObj = JSValueToObject(ctx, m_pValue, nullptr);
        if (!JSObjectIsFunction(ctx, valueObj))
            return false;
        return true;
	}

	bool isValid() {
		return m_pValue != nullptr && m_pObj != nullptr && m_pObj->IsMyJsEnv();
	}

#define CALLJSPRE \
	if (!m_pObj)return false; \
    JSContextRef ctx = __TlsData::GetInstance()->GetCurContext(); \
    JSObjectRef func = JSValueToObject(ctx, m_pValue, nullptr); \
    if (!JSObjectIsFunction(ctx,func)) \
        return false;

	bool Call() {
		CALLJSPRE
			m_pReturn = m_pObj->callJsFunc(func);
		return true;
	}
	template <typename P1>
	bool Call(P1 p1) {
		CALLJSPRE
			m_pReturn = m_pObj->callJsFunc(func,p1);
		return true;
	}
	template <typename P1, typename R>
	bool CallWithReturn(P1 p1, R& r) {
		CALLJSPRE
		r = __TransferToCpp<R>::ToCpp(m_pObj->callJsFunc(func, p1));
		return true;
	}
    template <typename P1, typename P2>
    bool Call(JSObjectRef valThis, P1 p1, P2 p2) {
        if (!m_pObj)return false;
        JSContextRef ctx = __TlsData::GetInstance()->GetCurContext();
        JSObjectRef func = JSValueToObject(ctx, m_pValue, nullptr);
        if (!JSObjectIsFunction(ctx,func))
            return false;
        
        
        int argc = 2;
        JSValueRef argv[2];
        argv[0] = __TransferToJs<P1>::ToJs(p1);
        argv[1] = __TransferToJs<P2>::ToJs(p2);
        
        JSValueRef exception = nullptr;
        JSObjectRef funcObj = JSValueToObject(ctx,func,&exception);
        if (exception != nullptr){
            __JSRun::OutputException(exception);
        }
        //JSObjectRef thisObj = JSValueToObject(ctx,valThis,&exception);
        //if (exception != nullptr){
        //    __JSRun::OutputException(exception);
        //}
        JSValueRef ret = JSObjectCallAsFunction(ctx,funcObj,valThis,argc,argv,&exception);
        if (exception != nullptr){
            __JSRun::OutputException(exception);
        }
        return true;
    }
	template <typename P1, typename P2>
	bool Call(P1 p1, P2 p2) {
		CALLJSPRE
			m_pReturn = m_pObj->callJsFunc(func, p1, p2);
		return true;
	}
    
	template <typename P1, typename P2, typename P3>
	bool Call(P1 p1, P2 p2, P3 p3) {
		CALLJSPRE
			m_pReturn = m_pObj->callJsFunc(func, p1, p2, p3);
		return true;
	}
    template <typename P1, typename P2, typename P3, typename R>
    bool CallWithReturn(JSValueRef valThis,P1 p1, P2 p2, P3 p3, R& r) {
        if (!m_pObj)return false;
        JSContextRef ctx = __TlsData::GetInstance()->GetCurContext();
        JSObjectRef func = JSValueToObject(ctx, m_pValue, nullptr);
        if (!JSObjectIsFunction(ctx,func))
            return false;
        
        
        int argc = 3;
        JSValueRef argv[3];
        argv[0] = __TransferToJs<P1>::ToJs(p1);
        argv[1] = __TransferToJs<P2>::ToJs(p2);
        argv[2] = __TransferToJs<P3>::ToJs(p3);
        
        JSValueRef exception = nullptr;
        JSObjectRef funcObj = JSValueToObject(ctx,func,&exception);
        if (exception != nullptr){
            __JSRun::OutputException(exception);
        }
        JSObjectRef thisObj = JSValueToObject(ctx,valThis,&exception);
        if (exception != nullptr){
            __JSRun::OutputException(exception);
        }
        JSValueRef ret = JSObjectCallAsFunction(ctx,funcObj,thisObj,argc,argv,&exception);
        if (exception != nullptr){
            __JSRun::OutputException(exception);
        }
        r = __TransferToCpp<R>::ToCpp(ret);
        return true;
    }
	template <typename P1, typename P2, typename P3, typename P4>
	bool Call(P1 p1, P2 p2, P3 p3, P4 p4) {
		CALLJSPRE
			m_pReturn = m_pObj->callJsFunc(func, p1, p2, p3, p4);
		return true;
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5>
	bool Call(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) {
		CALLJSPRE
			m_pReturn = m_pObj->callJsFunc(func, p1, p2, p3, p4, p5);
		return true;
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	bool Call(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) {
		CALLJSPRE
			m_pReturn = m_pObj->callJsFunc(func, p1, p2, p3, p4, p5, p6);
		return true;
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	bool Call(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) {
		CALLJSPRE
			m_pReturn = m_pObj->callJsFunc(func, p1, p2, p3, p4, p5, p6, p7);
		return true;
	}
};
#endif
