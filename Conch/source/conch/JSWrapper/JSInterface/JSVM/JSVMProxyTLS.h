#ifndef _JSVMPROXYTLS_H_
#define _JSVMPROXYTLS_H_
#include "ark_runtime/jsvm.h"
#include "JSVMEnv.h"
#include <cassert>

namespace laya{

#define JSVM_API_CALL(status, env, the_call)                                        \
    status = the_call;                                                              \
    if (status != JSVM_OK) {                                                        \
        bool isPending = false;                                                     \
        if (JSVM_OK == OH_JSVM_IsExceptionPending((env), &isPending) && isPending) {  \
            JSVM_Value error;                                                       \
            if (JSVM_OK == OH_JSVM_GetAndClearLastException((env), &error)) {         \
                JSVM_Value stack;                                                   \
                OH_JSVM_GetNamedProperty((env), error, "stack", &stack);              \
                JSVM_Value message;                                                 \
                OH_JSVM_GetNamedProperty((env), error, "message", &message);          \
                char stackstr[256];                                                 \
                OH_JSVM_GetValueStringUtf8(env, stack, stackstr, 256, nullptr);     \
                char messagestr[256];                                               \
                OH_JSVM_GetValueStringUtf8(env, message, messagestr, 256, nullptr); \
                laya::__JSRun::ReportException(stackstr, messagestr);               \
            }                                                                       \
        }                                                                           \
    }

    extern void JSAlert(const char* p_sBuffer);
    class __JsThrow    {
    public:
        static void Throw( const char *p_pszInfo ) {
            OH_JSVM_ThrowError(Javascript::getEnv(), nullptr, p_pszInfo);
        }
    };
    extern bool gbAlertException;
    class __JSRun  {
    public:
        static void ReportException(const char* stackStr, const char* messageStr);

        static bool Run( const char *p_pszScript, const char * fileName)
		{
            AutoHandleScope scope;
            auto _env = Javascript::getEnv();
            ssize_t length = strlen(p_pszScript);
            JSVM_Status status;
            JSVM_Value jsVmStr;
			JSVM_API_CALL(status, _env, OH_JSVM_CreateStringUtf8(_env, p_pszScript, length, &jsVmStr));
            if (status != JSVM_OK) {
                LOGE("__JSRun::Run, create string failed, strScript = %{public}s", p_pszScript);
                return false;
            }
        
            uint8_t *cachedData = nullptr;
            size_t cacheLength = 0;
            fileName = fileName ? fileName : "";
            bool cacheRejected = false;
            JSVM_Script compiledScript;
            JSVM_ScriptOrigin scriptOrigin {
                .sourceMapUrl = nullptr,
                .resourceName = fileName,
                .resourceLineOffset = 0,
                .resourceColumnOffset = 0
            };
        
            JSVM_API_CALL(status, _env, OH_JSVM_CompileScriptWithOrigin(_env, jsVmStr, cachedData, cacheLength, false, &cacheRejected, &scriptOrigin, &compiledScript));
            if (status != JSVM_OK) {
                LOGE("__JSRun::Run, compile failed, strScript = %{public}s", p_pszScript);
                return false;
            }
        
            JSVM_Value result;
            JSVM_API_CALL(status, _env, OH_JSVM_RunScript(_env, compiledScript, &result));
            if (status != JSVM_OK) {
                return false;
            }
            return true;
        }
    };
}

#endif
