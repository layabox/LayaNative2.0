#include <js_native_api.h>
#include <js_native_api_types.h>
#include "napi/native_api.h"
/*
 * function for module exports
 */
extern  napi_value InitLaya(napi_env env, napi_value exports);

/*
 * Napi Module define
 */
static napi_module layaModule = {
    .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = nullptr,
    .nm_register_func = InitLaya,
    .nm_modname = "laya",
    .nm_priv = ((void *)0),
    .reserved = {0},
};
/*
 * Module register function
 */
extern "C" __attribute__((constructor)) void RegisterModule(void) { napi_module_register(&layaModule); }