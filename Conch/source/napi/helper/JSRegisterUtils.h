#ifndef CC_OH_JsRegister_H
#define CC_OH_JsRegister_H

#include <napi/native_api.h>
#include "NapiHelper.h"

napi_value registerFunction(napi_env env, napi_callback_info info) {
    LOGI("====begin to registerFunction!");
	napi_status status;
	napi_value exports;
	size_t argc = 2;
	napi_value args[2];

	napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);

	if (argc != 2) {
		napi_throw_type_error(env, NULL, "Wrong number of arguments");
		return nullptr;
	}

	napi_valuetype valuetype;
	status = napi_typeof(env, args[0], &valuetype);
	if (status != napi_ok) {
		return nullptr;
	}
	if (valuetype != napi_string) {
		napi_throw_type_error(env, NULL, "Wrong arguments");
		return nullptr;
	}
    
    auto jsArg = args[0];
    size_t len = 0;
    status = napi_get_value_string_utf8(env, jsArg, nullptr, 0, &len);    
    std::string functionName = "";
    functionName.resize(len, '\0');
    status = napi_get_value_string_utf8(env, jsArg, (char*)functionName.data(), functionName.size() + 1, &len);    

	napi_valuetype functionType;
	status = napi_typeof(env, args[1], &functionType);
	if (status != napi_ok) {
		return nullptr;
	}
	if (functionType != napi_function) {
		napi_throw_type_error(env, NULL, "Wrong arguments");
		return nullptr;
	}
    
    napi_value workName;
    status = napi_create_string_utf8(env, "Thread-safe call from async work", NAPI_AUTO_LENGTH, &workName);
    if (status != napi_ok) {
        LOGW("registerFunction napi_create_string_utf8 fail,status=%{public}d", status);
    }
    
    napi_threadsafe_function save_func;
    status = napi_create_threadsafe_function(env, args[1], nullptr, workName, 0, 1, nullptr, 
    [](napi_env env, void *raw, void *hint) {}, NULL, JSFunction::CallJS, &save_func);
    if (status != napi_ok) {
        LOGW("registerFunction napi_create_threadsafe_function fail,status=%{public}d", status);
    }
    
    JSFunction jsFunction(env, save_func);
    JSFunction::addFunction(functionName, jsFunction);
       
    LOGI("begin to return!");
    return nullptr;
}

#endif //CC_OH_JsRegister_H