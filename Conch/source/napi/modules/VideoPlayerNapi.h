#include <napi/native_api.h>

class VideoPlayerNapi
{
public:
    static napi_value onVideoCallBack(napi_env env, napi_callback_info info);
    static napi_value loadedmetadata(napi_env env, napi_callback_info info);
    static napi_value onDurationBack(napi_env env, napi_callback_info info);
    static napi_value onCurrentTimeBack(napi_env env, napi_callback_info info);
};