#ifdef JS_V8
#include "JsBinder.h"
#include "../JSInterface.h"

using namespace v8;


namespace laya {

    JSClassMgr JSClassMgr::__Ins;

    Local<Function> createJSMethodRaw(FunctionCallback func, JsValue data)
	{
        Isolate* pIso = Isolate::GetCurrent();
        Local<FunctionTemplate> method = FunctionTemplate::New(pIso);
        method->SetCallHandler(func, data);
        return method->GetFunction(pIso->GetCurrentContext()).ToLocalChecked();
    }


    JSObjBaseV8::JSObjBaseV8()
	{
        //放在构造里面，容易导致没有js环境而非法。还是往后放放吧。
		m_isolate = NULL;
    }

    JSObjBaseV8::~JSObjBaseV8() 
	{
		if (!m_persistent_handle.IsEmpty())
		{
			m_persistent_handle.ClearWeak();
			m_persistent_handle.Reset();
		}
    }

    void JSObjBaseV8::makeStrong()
	{
		m_persistent_handle.ClearWeak();
    }

    void JSObjBaseV8::makeWeak()
	{
		m_persistent_handle.SetWeak(this, WeakCallback, v8::WeakCallbackType::kFinalizer);

    }

	void JSObjBaseV8::initialize(Isolate* isolate, v8::Local<v8::Object> object)
	{
		this->m_isolate = isolate;
		this->m_persistent_handle.Reset(isolate, object);
	}

    JsValue JSObjBaseV8::callJsFunc(JsFunction& func) 
	{
        int argc = 0;
        Local<Value> argv[1];
        return _callJsFunc(func, argc, argv);
    }

    bool JsObjHandle::Empty()
	{
        return getJsObj().IsEmpty();
    }

    
}
#endif