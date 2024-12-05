#ifdef JS_JSVM
#include "util/Log.h"
#include <cassert>
#include <ark_runtime/jsvm.h>
#include "IsolateData.h"
#include "JSVMProxyTLS.h"
namespace laya
{

    std::vector<JSClass *> __allClasses;
    
    JSClass::JSClass() {
        __allClasses.push_back(this);
    }

    JSClass::~JSClass() {
    }

    JSClass *JSClass::create(const std::string &clsName, JSVM_Callback ctor) {
        JSClass *cls = new JSClass();
        cls->init(clsName, ctor);
        return cls;
    }

    void JSClass::init(const std::string &clsName, JSVM_Callback ctor) {
        _name = clsName;
        if (ctor) {
            _ctorFunc = ctor;
        }
    }

    bool JSClass::install(JSVM_Env env) {
        JSVM_Value  cons;
        JSVM_Status status;
        JSVM_API_CALL(status, env, OH_JSVM_DefineClass(env, _name.c_str(), - 1, _ctorFunc, _properties.size(), _properties.data(), &cons));
        JSVM_API_CALL(status, env, OH_JSVM_CreateReference(env, cons, 1, &_constructor));
        JSVM_Value gctx;
        JSVM_API_CALL(status, env, OH_JSVM_GetGlobal(env, &gctx));
        JSVM_API_CALL(status, env, OH_JSVM_SetNamedProperty(env, gctx, _name.c_str(),cons));
        return true;
    }

    void JSClass::defineFunction(const char* name, JSVM_Callback func) {
        _properties.push_back({name, nullptr, func, nullptr, nullptr, nullptr, JSVM_JSPROPERTY_NO_RECEIVER_CHECK});
    }

    void JSClass::defineProperty(const char* name, JSVM_Callback g, JSVM_Callback s, JSVM_Value value) {
        _properties.push_back({name, nullptr, nullptr, g, s, value, JSVM_DEFAULT_JSPROPERTY});
    }

    void JSClass::defineStaticFunction(const char* name, JSVM_Callback func) {
        _properties.push_back({name, nullptr, func, nullptr, nullptr, 0, JSVM_PropertyAttributes((int)JSVM_STATIC|(int)JSVM_WRITABLE)});
    }

    JSVM_Ref JSClass::getConRef() {
        return _constructor;
    }

    JSVM_Value JSClass::_defaultCtor(JSVM_Env env, JSVM_CallbackInfo info) {
        JSVM_Value thisArg;
        JSVM_Status status;
        JSVM_API_CALL(status, env, OH_JSVM_GetCbInfo(env, info, nullptr, nullptr, &thisArg, nullptr));
        return thisArg;
    }

	IsolateData::IsolateData(JSVM_Env env): _env(env)
	{
		IsolateData::instance = this;
	}

	IsolateData::~IsolateData()
	{
		IsolateData::instance = NULL;
	}

	IsolateData* IsolateData::getInstance()
	{
		return instance;
	}

    void IsolateData::_setNeedCallConstructor(bool need) { _isNeedCallConstructor = need; };
    
    bool IsolateData::_needCallConstructor() { return _isNeedCallConstructor;};

	void IsolateData::SetObjectTemplate(JsObjClassInfo* info, JSClass* cls)
	{
		m_class_map[info] = cls->getConRef();
	}
	JSVM_Value IsolateData::GetObjectTemplate(JsObjClassInfo* info)
	{
		ClsMap::iterator it = m_class_map.find(info);
		if (it == m_class_map.end())
			return nullptr;
    
        JSVM_Ref cons_ref = it->second;
        JSVM_Value result;
        JSVM_Status status;
		JSVM_API_CALL(status, _env, OH_JSVM_GetReferenceValue(_env, cons_ref, &result));
        return result;
	}

    IsolateData* IsolateData::instance = NULL;
}
#endif
