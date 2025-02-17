#include "../conch/JCConch.h"
#include <hilog/log.h>
#include "aki/jsbind.h"
#include <napi/native_api.h>
#include "NapiValueConverter.h"

using namespace laya;

struct AsyncCallParam {
    std::function<void(std::string)> cb;// 执行异步回调后的c++回调
    std::string paramStr;// 执行异步js方法的参数
    bool isSyn;
    const char* clsPath;
    const char* methodName;
    std::string method;
    std::string module_info;
    napi_ref executeFuncRef;// 安全函数调用方法的指针
};

class NapiHelper{
public:
    ~NapiHelper(){}
    static NapiHelper *GetInstance(){return &NapiHelper::help_;}
    std::string getDeviceInfo();
    int getNetworkType();
    void startVibration(float duration);
    void playBackgroundMusic(const char *p_sUrl,int p_nTimes,float nCurrentTime);
    void pauseBackgroundMusic();
    void stopBackgroundMusic();
    void setBackgroundMusicVolume(float p_nVolume);
    void resumeBackgroundMusic();
    void setCurrentTime(double nCurrentTime);
    double getCurrentTime();
    double getDuration();
    std::string getAppVersion();
    std::string getAppLocalVersion();

    void createVideoPlayer(int m_tag);
    void removeVideoPlayer(int m_tag);
    void setURL(int m_tag,std::string path,int isurl);
    void play(int m_tag);
    void pause(int m_tag);
    void stop(int m_tag);
    void setLooping(int m_tag,bool value);
    void setAutoplay(int m_tag,bool value);
    void setX(int m_tag,double value);
    void setY(int m_tag,double value);
    void setWidth(int m_tag,double value);
    void setHeight(int m_tag,double value);
    void setVideoCurrentTime(int m_tag,double value);
    void setVolume(int m_tag,double value);
    int getVolume(int m_tag);
    void showDialog(const char *p_sBuffer);
    void setKeepScreenOn(bool value);
    void setPreferredOrientation(int orientation);
    float getScreenInch();
    int getAvalidMem();
    int getUsedMem();
    void exitGame();
    std::string postMessageToUIThread(std::string eventName, std::string data);
    std::string postSyncMessageToUIThread(std::string eventName, std::string data);
    std::string postCmdToMain(std::string data);
    std::string callNativeMethod(bool isSyn, const char* clsPath, const char* methodName, const char* paramStr);
    void __pauseBackgroundMusic();
    void __resumeBackgroundMusic();
    void handleCloseWebview();
    void handleCreateWebview(const char* sUrl,int x, int y, int w, int h, bool bCloseWebview);
    void handleCallWebviewJS(const char* sFunctionName, const char* sJsonParam, const char* sCallbackFunction);
    void handleHideWebview();
    void handleShowWebview();

private:
    std::string __getDeviceInfo();
    int __getNetworkType();
    void __startVibration(float duration);
    void __playBackgroundMusic(const char *p_sUrl,int p_nTimes,float nCurrentTime);
    void __stopBackgroundMusic();
    void __setBackgroundMusicVolume(float p_nVolume);
    void __setCurrentTime(double nCurrentTime);
    double __getCurrentTime();
    double __getDuration();
    std::string __getAppVersion();
    std::string __getAppLocalVersion();

    void __createVideoPlayer(int m_tag);
    void __removeVideoPlayer(int m_tag);
    void __setURL(int m_tag,std::string path,int isurl);
    void __play(int m_tag);
    void __pause(int m_tag);
    void __stop(int m_tag);
    void __setLooping(int m_tag,bool value);
    void __setAutoplay(int m_tag,bool value);
    void __setX(int m_tag,double value);
    void __setY(int m_tag,double value);
    void __setWidth(int m_tag,double value);
    void __setHeight(int m_tag,double value);
    void __setVideoCurrentTime(int m_tag,double value);
    void __setVolume(int m_tag,double value);
    int __getVolume(int m_tag);
    void __showDialog(const char *p_sBuffer);
    void __setKeepScreenOn(bool value);
    void __setPreferredOrientation(int orientation);
    float __getScreenInch();
    int __getAvalidMem();
    int __getUsedMem();
    void __exitGame();
    std::string __postMessageToUIThread(std::string eventName, std::string data);
    std::string __postSyncMessageToUIThread(std::string eventName, std::string data);
    std::string __postCmdToMain(std::string data);
    std::string __callNativeMethod(bool isSyn, const char* clsPath, const char* methodName, const char* paramStr);
    std::string __getModuleInfo(const char* module_name);
    void __handleCloseWebview();
    void __handleCreateWebview(const char *sUrl, int x, int y, int w, int h, bool bCloseWebview);
    void __handleCallWebviewJS(const char *sFunctionName, const char *sJsonParam, const char *sCallbackFunction);
    void __handleHideWebview();
    void __handleShowWebview();
    static NapiHelper help_;

private:
    std::string deviceInfo;
    std::string appVersion;
    std::string appLocalVersion;
    std::string eventResult;
    std::string syncEventResult;
    int networkType;
    int volume;
    double currentTime;
    double duration;
    float screenInch;
    int avalidMem;
    int usedMem;
    std::string methodResult;
    std::string bundle_name;
};

class JSFunction {
public:
    napi_ref funcRef;
    napi_env env;
    char* name = nullptr;

public: 
    static std::unordered_map<std::string, JSFunction> FUNCTION_MAP;  

    explicit JSFunction(char* name, napi_env env, napi_ref funcRef)
        : name(name), env(env), funcRef(funcRef){}
    
    explicit JSFunction(char* name, napi_env env)
        : name(name), env(env){} 

    explicit JSFunction(char* name)
        : name(name){} 

    static JSFunction getFunction(std::string functionName)
    {
        return FUNCTION_MAP.at(functionName);
    }

    static void addFunction(std::string name, JSFunction* jsFunction) {
        FUNCTION_MAP.emplace(name, *jsFunction);
    }

    template<typename ReturnType, typename... Args>
    typename std::enable_if<!std::is_same<ReturnType, void>::value, ReturnType>::type
    invoke(Args... args) {
        LOGI("=========cocos-[NApiHelper]=========JSFunction::invoke =========");
        napi_value global;
        napi_status status = napi_get_global(env, &global);
        //if (status != napi_ok) return;
        
        napi_value func;
        status = napi_get_reference_value(env, funcRef, &func);
        
        napi_value jsArgs[sizeof...(Args)] = {NapiValueConverter::ToNapiValue(env, args)...};
        napi_value return_val;
        status = napi_call_function(env, global, func, sizeof...(Args), jsArgs, &return_val);
        
        ReturnType value;
        if (!NapiValueConverter::ToCppValue(env, return_val, value)) {
            // Handle error here
        }
        return value;
    }
    
    template<typename ReturnType, typename... Args>
    typename std::enable_if<std::is_same<ReturnType, void>::value, void>::type
    invoke(Args... args) {
        LOGI("=========cocos-[NApiHelper]=========JSFunction::invoke =========");
        napi_value global;
        napi_status status = napi_get_global(env, &global);
        if (status != napi_ok) return;
        
        napi_value func;
        status = napi_get_reference_value(env, funcRef, &func);
        
        napi_value jsArgs[sizeof...(Args)] = {NapiValueConverter::ToNapiValue(env, args)...};
        napi_value return_val;
        status = napi_call_function(env, global, func, sizeof...(Args), jsArgs, &return_val);
    }
    
    void invokeAsync(AsyncCallParam *callParam) {
        LOGI("=========cocos-[NApiHelper]=========JSFunction::invokeAsync =========");
        
        // 传入执行方法的指针
        callParam->executeFuncRef = funcRef;
        
        napi_status status;
        napi_value func;
        status = napi_get_reference_value(env, funcRef, &func);
        if (status != napi_ok) {
            LOGW("invokeAsync napi_get_reference_value fail,status=%{public}d", status);
            return callParam->cb("napi_error");
        }
        
        napi_value workName;
        status = napi_create_string_utf8(env, "Thread-safe call from async work", NAPI_AUTO_LENGTH, &workName);
        if (status != napi_ok) {
            LOGW("invokeAsync napi_create_string_utf8 fail,status=%{public}d", status);
            return callParam->cb("napi_error");
        }
        
        napi_threadsafe_function save_func;
        status = napi_create_threadsafe_function(env, func, nullptr, workName, 0, 1, nullptr, 
        [](napi_env env, void *raw, void *hint) {}, callParam, CallJS, &save_func);
        if (status != napi_ok) {
            LOGW("invokeAsync napi_create_threadsafe_function fail,status=%{public}d", status);
            return callParam->cb("napi_error");
        }
        
        status = napi_acquire_threadsafe_function(save_func);
        if (status != napi_ok) {
            LOGW("invokeAsync napi_acquire_threadsafe_function fail,status=%{public}d", status);
            return callParam->cb("napi_error");
        }
        
        status = napi_call_threadsafe_function(save_func, NULL, napi_tsfn_blocking);
        if (status != napi_ok) {
            LOGW("invokeAsync napi_call_threadsafe_function fail,status=%{public}d", status);
            return callParam->cb("napi_error");
        }
    }
    
    static void CallJS(napi_env env, napi_value js_cb, void *context, void *data) {
        LOGI("napi_call_threadsafe_function CallJS");
        
        AsyncCallParam *callParam = (AsyncCallParam*) (context);
        if (callParam == nullptr) {
            LOGW("CallJS AsyncCallParam callParam is null");
            return callParam->cb("napi_error");
        }
        
        napi_status status;
        status = napi_get_reference_value(env, callParam->executeFuncRef, &js_cb);
        if (status != napi_ok) {
            LOGW("CallJS napi_get_reference_value fail,status=%{public}d", status);
            return callParam->cb("napi_error");
        }
        
        napi_value result;
        status = napi_load_module_with_info(env, callParam->clsPath, callParam->module_info.c_str(), &result);
        if (status != napi_ok) {
            LOGW("callNativeMethod napi_load_module_with_info fail, status=%{public}d", status);
            return callParam->cb("napi_error");
        }
        
        napi_value callFunc;
        status = napi_get_named_property(env, result, callParam->method.c_str(), &callFunc);
        if (status != napi_ok) {
            LOGW("callNativeMethod napi_get_named_property fail, status=%{public}d", status);
            return callParam->cb("napi_error");
        }
        
        napi_value isSyn = NapiValueConverter::ToNapiValue(env, callParam->isSyn);
        napi_value cName = NapiValueConverter::ToNapiValue(env, callParam->clsPath);
        napi_value mName = NapiValueConverter::ToNapiValue(env, callParam->methodName);
        napi_value paramStr = NapiValueConverter::ToNapiValue(env, callParam->paramStr);
        napi_value jsArgs[5] = {isSyn, cName, mName, callFunc, paramStr};
        napi_value return_val;
        napi_value global;
        status = napi_get_global(env, &global);
        if (status != napi_ok) {
            LOGW("CallJS napi_get_global fail,status=%{public}d", status);
            return callParam->cb("napi_error");
        }
        
        status = napi_call_function(env, global, js_cb, 5, jsArgs, &return_val);
        if (status != napi_ok) {
            LOGW("CallJS napi_call_function fail,status=%{public}d", status);
            return callParam->cb("napi_error");
        }
        
        std::string resultStr;
        NapiValueConverter::ToCppValue(env, return_val, resultStr);
        callParam->cb(resultStr);
    }
};