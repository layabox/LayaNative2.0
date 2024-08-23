#include <stdint.h>
#include <string>
#include <stdio.h>

#include <ace/xcomponent/native_interface_xcomponent.h>

#include "modules/VideoPlayerNapi.h"
#include "plugin_manager.h"
#include <aki/jsbind.h>
#include "util/Log.h"

enum ContextType
{
    APP_LIFECYCLE = 0,
    JS_PAGE_LIFECYCLE,
    WORKER_INIT,
    NATIVE_API,
    VIDEOPLAYER_NAPI,
};

JSBIND_ENUM(ContextType)
{
    JSBIND_ENUM_VALUE(APP_LIFECYCLE);
    JSBIND_ENUM_VALUE(JS_PAGE_LIFECYCLE);
    JSBIND_ENUM_VALUE(WORKER_INIT);
    JSBIND_ENUM_VALUE(NATIVE_API);
    JSBIND_ENUM_VALUE(VIDEOPLAYER_NAPI);
}

NapiManager NapiManager::manager_;

napi_value NapiManager::GetContext(long contextEnum)
{
    napi_env env = aki::JSBind::GetScopedEnv();
    napi_value exports;
    NAPI_CALL(env, napi_create_object(env, &exports));

    switch (contextEnum)
    {
    case APP_LIFECYCLE:
    {
        /****  application life cycle: onCreate, onShow, onHide, onDestroy ******/
        LOGI("GetContext APP_LIFECYCLE");
        napi_property_descriptor desc[] = {
            DECLARE_NAPI_FUNCTION("onCreate", NapiManager::NapiOnCreate),
            DECLARE_NAPI_FUNCTION("onShow", NapiManager::NapiOnShow),
            DECLARE_NAPI_FUNCTION("onHide", NapiManager::NapiOnHide),
            DECLARE_NAPI_FUNCTION("onBackPress", NapiManager::NapiOnBackPress),
            DECLARE_NAPI_FUNCTION("onDestroy", NapiManager::NapiOnDestroy),
        };
        NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
    }
    break;
    case JS_PAGE_LIFECYCLE:
    {
        /****************  JS Page Lifecycle ****************************/
        LOGI("GetContext JS_PAGE_LIFECYCLE");
        napi_property_descriptor desc[] = {
            DECLARE_NAPI_FUNCTION("onPageShow", NapiManager::NapiOnPageShow),
            DECLARE_NAPI_FUNCTION("onPageHide", NapiManager::NapiOnPageHide),
        };
        NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
    }
    break;
    case WORKER_INIT:
    {
        LOGI("NapiManager::GetContext WORKER_INIT");
        napi_property_descriptor desc[] = {
            DECLARE_NAPI_FUNCTION("workerInit", NapiManager::napiWorkerInit),
        };
        NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
    }
    break;
    case NATIVE_API:
    {
        LOGI("NapiManager::GetContext NATIVE_RENDER_API");
        napi_property_descriptor desc[] = {
            DECLARE_NAPI_FUNCTION("nativeEngineStart", NapiManager::napiNativeEngineStart),
        };
        NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
    }
    break;
    case VIDEOPLAYER_NAPI:
    {
        napi_property_descriptor desc[] = {
            DECLARE_NAPI_FUNCTION("onVideoCallBack", VideoPlayerNapi::onVideoCallBack),
            DECLARE_NAPI_FUNCTION("loadedmetadata", VideoPlayerNapi::loadedmetadata),
            DECLARE_NAPI_FUNCTION("onDurationBack", VideoPlayerNapi::onDurationBack),
            DECLARE_NAPI_FUNCTION("onCurrentTimeBack", VideoPlayerNapi::onCurrentTimeBack),
        };
        NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
    }
    break;
    default:
        LOGI("unknown type");
    }
    return exports;
}

bool NapiManager::Export(napi_env env, napi_value exports)
{
    LOGI("NapiManager::Export");
    napi_status status;
    napi_value exportInstance = nullptr;
    OH_NativeXComponent *nativeXComponent = nullptr;
    int32_t ret;
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;

    status = napi_get_named_property(env, exports, OH_NATIVE_XCOMPONENT_OBJ, &exportInstance);
    if (status != napi_ok)
    {
        return false;
    }

    status = napi_unwrap(env, exportInstance, reinterpret_cast<void **>(&nativeXComponent));
    if (status != napi_ok)
    {
        return false;
    }

    auto context = NapiManager::GetInstance();
    if (context)
    {
        context->SetNativeXComponent(nativeXComponent);
        PluginRender::GetInstance()->SetNativeXComponent(nativeXComponent);
        PluginRender::GetInstance()->Export(env, exports);
        return true;
    }
    return false;
}

void NapiManager::SetNativeXComponent(OH_NativeXComponent *nativeXComponent)
{
    nativeXComponent_ = nativeXComponent;
}

OH_NativeXComponent *NapiManager::GetNativeXComponent()
{
    return nativeXComponent_;
}

void NapiManager::MainOnMessage(const uv_async_t *req)
{
    LOGI("MainOnMessage Triggered");
}

napi_value NapiManager::NapiOnCreate(napi_env env, napi_callback_info info)
{
    return nullptr;
}

napi_value NapiManager::NapiOnShow(napi_env env, napi_callback_info info)
{
    LOGI("NapiManager::NapiOnShow");
    WorkerMessageData data{MessageType::WM_APP_SHOW, nullptr, nullptr};
    PluginRender::GetInstance()->enqueue(data);
    return nullptr;
}

napi_value NapiManager::NapiOnHide(napi_env env, napi_callback_info info)
{
    LOGI("NapiManager::NapiOnHide");
    WorkerMessageData data{MessageType::WM_APP_HIDE, nullptr, nullptr};
    PluginRender::GetInstance()->enqueue(data);
    return nullptr;
}

napi_value NapiManager::NapiOnBackPress(napi_env env, napi_callback_info info)
{
    LOGI("NapiManager::NapiOnBackPress");
    return nullptr;
}

napi_value NapiManager::NapiOnDestroy(napi_env env, napi_callback_info info)
{
    WorkerMessageData data{MessageType::WM_APP_DESTROY, nullptr, nullptr};
    PluginRender::GetInstance()->enqueue(data);
    return nullptr;
}

napi_value NapiManager::napiWorkerInit(napi_env env, napi_callback_info info)
{
    LOGI("NapiManager::napiWorkerInit");
    uv_loop_t *loop = nullptr;
    NAPI_CALL(env, napi_get_uv_event_loop(env, &loop));
    PluginRender::GetInstance()->workerInit(env, loop);
    return nullptr;
}

napi_value NapiManager::napiNativeEngineStart(napi_env env, napi_callback_info info)
{
    LOGI("NapiManager::napiNativeEngineStart");
    PluginRender::GetInstance()->run();
    return nullptr;
}

napi_value NapiManager::NapiOnPageShow(napi_env env, napi_callback_info info)
{
    LOGI("NapiManager::NapiOnPageShow");
    return nullptr;
}

napi_value NapiManager::NapiOnPageHide(napi_env env, napi_callback_info info)
{
    LOGI("NapiManager::NapiOnPageHide");
    return nullptr;
}

void NapiManager::OnPageShowNative()
{
    LOGI("NapiManager::OnPageShowNative");
}

void NapiManager::OnPageHideNative()
{
    LOGI("NapiManager::OnPageHideNative");
}

JSBIND_GLOBAL()
{
    JSBIND_FUNCTION(NapiManager::GetContext, "getContext");
}

/*
 * function for module exports
 */
napi_value InitLaya(napi_env env, napi_value exports) {
    aki::JSBind::BindSymbols(env, exports);
    bool ret = NapiManager::GetInstance()->Export(env, exports);
    if (!ret) {
        LOGE("napi init failed");
    }
    return exports;
}
