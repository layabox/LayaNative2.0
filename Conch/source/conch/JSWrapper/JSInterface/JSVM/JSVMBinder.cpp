#ifdef JS_JSVM
#include "JSVMBinder.h"
#include "../JSInterface.h"

namespace laya {

    JSClassMgr JSClassMgr::__Ins;

    std::set<JSObjBaseJSVM*> JSObjBaseJSVM::objBaseSet = {};
    bool JSObjBaseJSVM::restarting = false;

    JSObjBaseJSVM::JSObjBaseJSVM()
	{
    }

    JSObjBaseJSVM::~JSObjBaseJSVM() 
	{
		if (_ref == nullptr) {
			return;
		}
        if (restarting) {
            objBaseSet.insert(this);
        }
        JSVM_Status status;
        JSVM_API_CALL(status, _env, OH_JSVM_DeleteReference(_env, _ref));
        _ref = nullptr;
    }

    void JSObjBaseJSVM::makeStrong()
	{
        JSVM_Status status;
        uint32_t result = 0;
        JSVM_API_CALL(status, _env, OH_JSVM_ReferenceRef(_env, _ref, &result));
    }

    void JSObjBaseJSVM::makeWeak()
	{
        uint32_t result = 0;
        JSVM_Status status;
        JSVM_API_CALL(status, _env, OH_JSVM_ReferenceUnref(_env, _ref, &result));
        assert(result == 0);
    }

	void JSObjBaseJSVM::initialize(JSVM_Value obj)
	{
		this->_env = ENV;
        JSVM_Status status;
        JSVM_API_CALL(status, _env, OH_JSVM_CreateReference(_env, obj, 1, &_ref));
        JSVM_API_CALL(status, _env, 
                  OH_JSVM_Wrap(_env, obj, (void*)this, WeakCallback,
                            (void*)this /* finalize_hint */, nullptr));
	}

    JsValue JSObjBaseJSVM::callJsFunc(JsFunction& func) 
	{
        int argc = 0;
        JSVM_Value argv[1];
        return _callJsFunc(func, argc, argv);
    }

    bool JsObjHandle::Empty()
	{
        AutoHandleScope scope;
        return getJsObj() == nullptr;
    }

    
}
#endif