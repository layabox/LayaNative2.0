#include "VideoPlayerNapi.h"
#include "../plugin_manager.h"
#include "util/Log.h"
#include "conch/JSWrapper/LayaWrap/Video/JSVideo.h"
#include <js_native_api.h>
#include <js_native_api_types.h>

napi_value VideoPlayerNapi::onVideoCallBack(napi_env env, napi_callback_info info)
{

    napi_status status;
    size_t argc = 2;
    napi_value args[2];
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, nullptr, nullptr));
    if (argc != 2)
    {
        napi_throw_type_error(env, NULL, "Wrong number of arguments");
        return nullptr;
    }

    napi_valuetype valuetype;
    status = napi_typeof(env, args[0], &valuetype);
    if (status != napi_ok)
    {
        return nullptr;
    }
    if (valuetype != napi_number)
    {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return nullptr;
    }

    status = napi_typeof(env, args[1], &valuetype);
    if (status != napi_ok)
    {
        return nullptr;
    }
    if (valuetype != napi_number)
    {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return nullptr;
    }

    int32_t viewTag;
    NAPI_CALL(env, napi_get_value_int32(env, args[0], &viewTag));
    int32_t event;
    NAPI_CALL(env, napi_get_value_int32(env, args[1], &event));

    laya::JSVideo::executeVideoCallback(viewTag, event);
    return nullptr;
}

napi_value VideoPlayerNapi::loadedmetadata(napi_env env, napi_callback_info info)
{

    napi_status status;
    size_t argc = 1;
    napi_value args[1];
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, nullptr, nullptr));
    if (argc != 1)
    {
        napi_throw_type_error(env, NULL, "Wrong number of arguments");
        return nullptr;
    }

    napi_valuetype valuetype;
    status = napi_typeof(env, args[0], &valuetype);
    if (status != napi_ok)
    {
        return nullptr;
    }
    if (valuetype != napi_number)
    {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return nullptr;
    }

    int32_t viewTag;
    NAPI_CALL(env, napi_get_value_int32(env, args[0], &viewTag));

    laya::JSVideo::executeLoadedmetadata(viewTag, "loadedmetadata");
    return nullptr;
}

napi_value VideoPlayerNapi::onDurationBack(napi_env env, napi_callback_info info)
{

    napi_status status;
    size_t argc = 2;
    napi_value args[2];
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, nullptr, nullptr));
    if (argc != 2)
    {
        napi_throw_type_error(env, NULL, "Wrong number of arguments");
        return nullptr;
    }

    napi_valuetype valuetype;
    status = napi_typeof(env, args[0], &valuetype);
    if (status != napi_ok)
    {
        return nullptr;
    }
    if (valuetype != napi_number)
    {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return nullptr;
    }

    status = napi_typeof(env, args[1], &valuetype);
    if (status != napi_ok)
    {
        return nullptr;
    }
    if (valuetype != napi_number)
    {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return nullptr;
    }

    int32_t viewTag;
    NAPI_CALL(env, napi_get_value_int32(env, args[0], &viewTag));
    int32_t duration;
    NAPI_CALL(env, napi_get_value_int32(env, args[1], &duration));

    laya::JSVideo::executeDurationBack(viewTag, duration);
    return nullptr;
}

napi_value VideoPlayerNapi::onCurrentTimeBack(napi_env env, napi_callback_info info)
{
    napi_status status;
    size_t argc = 2;
    napi_value args[2];
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, nullptr, nullptr));
    if (argc != 2)
    {
        napi_throw_type_error(env, NULL, "Wrong number of arguments");
        return nullptr;
    }

    napi_valuetype valuetype;
    status = napi_typeof(env, args[0], &valuetype);
    if (status != napi_ok)
    {
        return nullptr;
    }
    if (valuetype != napi_number)
    {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return nullptr;
    }

    status = napi_typeof(env, args[1], &valuetype);
    if (status != napi_ok)
    {
        return nullptr;
    }
    if (valuetype != napi_number)
    {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return nullptr;
    }

    int32_t viewTag;
    NAPI_CALL(env, napi_get_value_int32(env, args[0], &viewTag));
    int32_t currentTime;
    NAPI_CALL(env, napi_get_value_int32(env, args[1], &currentTime));

    laya::JSVideo::executeCurrentTimeBack(viewTag, currentTime);
    return nullptr;
}
