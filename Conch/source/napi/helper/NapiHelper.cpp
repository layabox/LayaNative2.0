#include "NapiHelper.h"
#include "bundle/native_interface_bundle.h"

using namespace laya;

NapiHelper NapiHelper::help_;

std::string NapiHelper::getDeviceInfo()
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__getDeviceInfo, this));
    return deviceInfo;
}

std::string NapiHelper::__getDeviceInfo()
{
    if (auto getDeviceInfo = aki::JSBind::GetJSFunction("DeviceUtils.getDeviceInfo"))
    {
        deviceInfo = getDeviceInfo->Invoke<std::string>();
    }
    return deviceInfo;
}

int NapiHelper::getNetworkType()
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__getNetworkType, this));
    return networkType;
}

int NapiHelper::__getNetworkType()
{
    if (auto getNetworkType = aki::JSBind::GetJSFunction("NetworkUtils.getNetworkType"))
    {
        networkType = getNetworkType->Invoke<int>();
    }
    return networkType;
}

void NapiHelper::playBackgroundMusic(const char *p_sUrl, int p_nTimes, float nCurrentTime)
{
    JCConch::s_pConchRender->setInterruptFunc(
        std::bind(&NapiHelper::__playBackgroundMusic, this, p_sUrl, p_nTimes, nCurrentTime));
}

void NapiHelper::__playBackgroundMusic(const char *p_sUrl, int p_nTimes, float nCurrentTime)
{
    if (auto playBackgroundMusic = aki::JSBind::GetJSFunction("SoundUtils.playBackgroundMusic"))
    {
        playBackgroundMusic->Invoke<void>(p_sUrl, p_nTimes, (int)(nCurrentTime * 1000));
    }
}

void NapiHelper::pauseBackgroundMusic()
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__pauseBackgroundMusic, this));
}

void NapiHelper::__pauseBackgroundMusic()
{
    if (auto pauseBackgroundMusic = aki::JSBind::GetJSFunction("SoundUtils.pauseBackgroundMusic"))
    {
        pauseBackgroundMusic->Invoke<void>();
    }
}

void NapiHelper::stopBackgroundMusic()
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__stopBackgroundMusic, this));
}

void NapiHelper::__stopBackgroundMusic()
{
    if (auto stopBackgroundMusic = aki::JSBind::GetJSFunction("SoundUtils.stopBackgroundMusic"))
    {
        stopBackgroundMusic->Invoke<void>();
    }
}

void NapiHelper::resumeBackgroundMusic()
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__resumeBackgroundMusic, this));
}

void NapiHelper::__resumeBackgroundMusic()
{
    if (auto resumeBackgroundMusic = aki::JSBind::GetJSFunction("SoundUtils.resumeBackgroundMusic"))
    {
        resumeBackgroundMusic->Invoke<void>();
    }
}

void NapiHelper::setBackgroundMusicVolume(float p_nVolume)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__setBackgroundMusicVolume, this, p_nVolume));
}

void NapiHelper::__setBackgroundMusicVolume(float p_nVolume)
{
    if (auto setBackgroundMusicVolume = aki::JSBind::GetJSFunction("SoundUtils.setBackgroundMusicVolume"))
    {
        setBackgroundMusicVolume->Invoke<void>(p_nVolume);
    }
}

void NapiHelper::setCurrentTime(double nCurrentTime)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__setCurrentTime, this, nCurrentTime));
}

void NapiHelper::__setCurrentTime(double nCurrentTime)
{
    if (auto setCurrentTime = aki::JSBind::GetJSFunction("SoundUtils.setCurrentTime"))
    {
        setCurrentTime->Invoke<void>(nCurrentTime);
    }
}

double NapiHelper::getCurrentTime()
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__getCurrentTime, this));
    return currentTime;
}

double NapiHelper::__getCurrentTime()
{
    currentTime = 0.0f;
    if (auto getCurrentTime = aki::JSBind::GetJSFunction("SoundUtils.getCurrentTime"))
    {
        currentTime = getCurrentTime->Invoke<double>();
    }
    return currentTime;
}

double NapiHelper::getDuration()
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__getDuration, this));
    return duration;
}

double NapiHelper::__getDuration()
{
    duration = 0.0f;
    if (auto getMp3Duration = aki::JSBind::GetJSFunction("SoundUtils.getDuration"))
    {
        duration = getMp3Duration->Invoke<double>();
    }
    return duration;
}

void NapiHelper::startVibration(float duration)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__startVibration, this, duration));
}

void NapiHelper::__startVibration(float duration)
{
    if (auto startVibration = aki::JSBind::GetJSFunction("DeviceUtils.startVibration"))
    {
        startVibration->Invoke<void>(duration);
    }
}

std::string NapiHelper::getAppVersion()
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__getAppVersion, this));
    return appVersion;
}

std::string NapiHelper::__getAppVersion()
{
    if (auto getAppVersion = aki::JSBind::GetJSFunction("ApplicationManager.getAppVersion"))
    {
        appVersion = getAppVersion->Invoke<std::string>();
    }
    return appVersion;
}

std::string NapiHelper::getAppLocalVersion()
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__getAppLocalVersion, this));
    return appLocalVersion;
}

std::string NapiHelper::__getAppLocalVersion()
{
    if (auto getAppLocalVersion = aki::JSBind::GetJSFunction("ApplicationManager.getAppLocalVersion"))
    {
        appLocalVersion = getAppLocalVersion->Invoke<std::string>();
    }
    return appLocalVersion;
}

void NapiHelper::createVideoPlayer(int m_tag)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__createVideoPlayer, this, m_tag));
}

void NapiHelper::__createVideoPlayer(int m_tag)
{
    if (auto createVideoPlayer = aki::JSBind::GetJSFunction("VideoPlayer.createVideoPlayer"))
    {
        createVideoPlayer->Invoke<void>(m_tag);
    }
}

void NapiHelper::removeVideoPlayer(int m_tag)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__removeVideoPlayer, this, m_tag));
}

void NapiHelper::__removeVideoPlayer(int m_tag)
{
    if (auto removeVideoPlayer = aki::JSBind::GetJSFunction("VideoPlayer.removeVideoPlayer"))
    {
        removeVideoPlayer->Invoke<void>(m_tag);
    }
}

void NapiHelper::setURL(int m_tag, std::string path, int isurl)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__setURL, this, m_tag, path, isurl));
}

void NapiHelper::__setURL(int m_tag, std::string path, int isurl)
{
    if (auto setURL = aki::JSBind::GetJSFunction("VideoPlayer.setURL"))
    {
        setURL->Invoke<void>(m_tag, path, isurl);
    }
}

void NapiHelper::play(int m_tag)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__play, this, m_tag));
}
void NapiHelper::__play(int m_tag)
{
    if (auto play = aki::JSBind::GetJSFunction("VideoPlayer.play"))
    {
        play->Invoke<void>(m_tag);
    }
}

void NapiHelper::pause(int m_tag)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__pause, this, m_tag));
}
void NapiHelper::__pause(int m_tag)
{
    if (auto pause = aki::JSBind::GetJSFunction("VideoPlayer.pause"))
    {
        pause->Invoke<void>(m_tag);
    }
}

void NapiHelper::stop(int m_tag)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__stop, this, m_tag));
}
void NapiHelper::__stop(int m_tag)
{
    if (auto stop = aki::JSBind::GetJSFunction("VideoPlayer.stop"))
    {
        stop->Invoke<void>(m_tag);
    }
}

void NapiHelper::setLooping(int m_tag, bool value)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__setLooping, this, m_tag, value));
}
void NapiHelper::__setLooping(int m_tag, bool value)
{
    if (auto setLooping = aki::JSBind::GetJSFunction("VideoPlayer.setLooping"))
    {
        setLooping->Invoke<void>(m_tag, value);
    }
}

void NapiHelper::setAutoplay(int m_tag, bool value)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__setAutoplay, this, m_tag, value));
}
void NapiHelper::__setAutoplay(int m_tag, bool value)
{
    if (auto setAutoplay = aki::JSBind::GetJSFunction("VideoPlayer.setAutoplay"))
    {
        setAutoplay->Invoke<void>(m_tag, value);
    }
}

void NapiHelper::setX(int m_tag, double value)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__setX, this, m_tag, value));
}
void NapiHelper::__setX(int m_tag, double value)
{
    if (auto setX = aki::JSBind::GetJSFunction("VideoPlayer.setX"))
    {
        setX->Invoke<void>(m_tag, value);
    }
}

void NapiHelper::setY(int m_tag, double value)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__setY, this, m_tag, value));
}

void NapiHelper::__setY(int m_tag, double value)
{
    if (auto setY = aki::JSBind::GetJSFunction("VideoPlayer.setY"))
    {
        setY->Invoke<void>(m_tag, value);
    }
}

void NapiHelper::setWidth(int m_tag, double value)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__setWidth, this, m_tag, value));
}

void NapiHelper::__setWidth(int m_tag, double value)
{
    if (auto setWidth = aki::JSBind::GetJSFunction("VideoPlayer.setWidth"))
    {
        setWidth->Invoke<void>(m_tag, value);
    }
}

void NapiHelper::setHeight(int m_tag, double value)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__setHeight, this, m_tag, value));
}

void NapiHelper::__setHeight(int m_tag, double value)
{
    if (auto setHeight = aki::JSBind::GetJSFunction("VideoPlayer.setHeight"))
    {
        setHeight->Invoke<void>(m_tag, value);
    }
}

void NapiHelper::setVideoCurrentTime(int m_tag, double value)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__setVideoCurrentTime, this, m_tag, value));
}

void NapiHelper::__setVideoCurrentTime(int m_tag, double value)
{
    if (auto setCurrentTime = aki::JSBind::GetJSFunction("VideoPlayer.setCurrentTime"))
    {
        setCurrentTime->Invoke<void>(m_tag, value);
    }
}

void NapiHelper::setVolume(int m_tag, double value)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__setVolume, this, m_tag, value));
}

void NapiHelper::__setVolume(int m_tag, double value)
{
    if (auto setVolume = aki::JSBind::GetJSFunction("VideoPlayer.setVolume"))
    {
        setVolume->Invoke<void>(m_tag, value);
    }
}

int NapiHelper::getVolume(int m_tag)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__getVolume, this, m_tag));
    return volume;
}

int NapiHelper::__getVolume(int m_tag)
{
    if (auto getVolume = aki::JSBind::GetJSFunction("VideoPlayer.getVolume"))
    {
        volume = getVolume->Invoke<int>(m_tag);
    }
    return volume;
}

void NapiHelper::showDialog(const char *p_sBuffer)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__showDialog, this, p_sBuffer));
}

void NapiHelper::__showDialog(const char *p_sBuffer)
{
    std::string strBuffer = p_sBuffer;
    if (auto showDialog = aki::JSBind::GetJSFunction("Dialog.showDialog"))
    {
        showDialog->Invoke<void>(strBuffer.c_str());
    }
}

void NapiHelper::setKeepScreenOn(bool value)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__setKeepScreenOn, this, value));
}

void NapiHelper::__setKeepScreenOn(bool value)
{
    if (auto setKeepScreenOn = aki::JSBind::GetJSFunction("DeviceUtils.setKeepScreenOn"))
    {
        setKeepScreenOn->Invoke<void>(value);
    }
}

void NapiHelper::setPreferredOrientation(int orientation)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__setPreferredOrientation, this, orientation));
}

void NapiHelper::__setPreferredOrientation(int orientation)
{
    if (auto setPreferredOrientation = aki::JSBind::GetJSFunction("DeviceUtils.setPreferredOrientation"))
    {
        setPreferredOrientation->Invoke<void>(orientation);
    }
}

float NapiHelper::getScreenInch()
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__getScreenInch, this));
    return screenInch;
}

float NapiHelper::__getScreenInch()
{
    std::promise<float> promise;
    std::function<void(float)> cb = [&promise](float message)
    { promise.set_value(message); };
    if (auto getScreenInch = aki::JSBind::GetJSFunction("DeviceUtils.getScreenInch"))
    {
        getScreenInch->Invoke<void>(cb);
    }
    screenInch = promise.get_future().get();
    return screenInch;
}

int NapiHelper::getAvalidMem()
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__getAvalidMem, this));
    return avalidMem;
}

int NapiHelper::__getAvalidMem()
{
    if (auto getAvalidMem = aki::JSBind::GetJSFunction("DeviceUtils.getAvalidMem"))
    {
        avalidMem = getAvalidMem->Invoke<int>();
    }
    return avalidMem;
}

int NapiHelper::getUsedMem()
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__getUsedMem, this));
    return usedMem;
}

int NapiHelper::__getUsedMem()
{
    if (auto getUsedMem = aki::JSBind::GetJSFunction("DeviceUtils.getPrivateDirty"))
    {
        usedMem = getUsedMem->Invoke<int>();
    }
    return usedMem;
}

void NapiHelper::exitGame()
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__exitGame, this));
}

void NapiHelper::__exitGame()
{
    if (auto exit = aki::JSBind::GetJSFunction("ApplicationManager.exit"))
    {
        exit->Invoke<void>();
    }
}

std::string NapiHelper::postMessageToUIThread(std::string eventName, std::string data)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__postMessageToUIThread, this, eventName, data));
    return eventResult;
}

std::string NapiHelper::__postMessageToUIThread(std::string eventName, std::string data)
{
    if (auto post = aki::JSBind::GetJSFunction("HandleMessageUtils.handleMessage"))
    {
        eventResult = post->Invoke<std::string>(eventName, data);
    }
    return eventResult;
}

std::string NapiHelper::postSyncMessageToUIThread(std::string eventName, std::string data)
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__postSyncMessageToUIThread, this, eventName, data));
    return syncEventResult;
}

std::string NapiHelper::__postSyncMessageToUIThread(std::string eventName, std::string data)
{
    std::promise<std::string> promise;
    std::function<void(std::string)> cb = [&promise](std::string message){
        promise.set_value(message); 
    };
    if (auto post = aki::JSBind::GetJSFunction("HandleMessageUtils.handleSyncMessage"))
    {
        post->Invoke<void>(eventName, data, cb);
    }
    syncEventResult = promise.get_future().get();
    return syncEventResult;
}

/**
 * 调用本地方法，目前只支持调用ets/ts脚本类型中的静态方法 示例是Test.ets下的test方法
 * @param isSyn 是否是同步方法
 * @param clsPath 脚本路径 示例：entry/src/main/Tests/Test
 * @param methodName 模块名称/静态方法名称 示例：entry/test(模块名称可省略，省略为clsPath第一个字符串)
 * @param paramStr 方法入参 示例：json字符串
 * @return 方法出参 示例：json字符串
 */
std::string NapiHelper::callNativeMethod(bool isSyn, const char* clsPath, const char* methodName, const char* paramStr)
{
    JCConch::s_pConchRender->setInterruptFunc(
        std::bind(&NapiHelper::__callNativeMethod, this, isSyn, clsPath, methodName, paramStr)
    );
    return methodResult;
}

std::string NapiHelper::__callNativeMethod(bool isSyn, const char* clsPath, const char* methodName, const char* paramStr)
{
    const char* module_name;
    const char* method;
    
    std::string methodStr = methodName;
    std::string::size_type pos = methodStr.find("/");
    if (pos != std::string::npos) {
        std::string str1 = methodStr.substr(0, pos);
        module_name = str1.c_str();
        std::string str2 = methodStr.substr(pos + 1);
        method = str2.c_str();
    } else {
        method = methodName;
        std::string pathStr = clsPath;
        pos = pathStr.find("/");
        module_name = pos != std::string::npos ? pathStr.substr(0, pos).c_str() : "entry";
    }
    
    napi_env env = aki::JSBind::GetScopedEnv();
    
    napi_status status;
    napi_value result;
    char* module_info = __getModuleInfo(module_name);
    status = napi_load_module_with_info(env, clsPath, module_info, &result);
    if (status != napi_ok) {
        LOGW("callNativeMethod napi_load_module_with_info fail, status=%{public}d", status);
        return "";
    }
    
    napi_value func;
    status = napi_get_named_property(env, result, method, &func);
    if (status != napi_ok) {
        LOGW("callNativeMethod napi_get_named_property fail, status=%{public}d", status);
        return "";
    }
    
    // 同步方法直接调用napi_call_function
    if (isSyn) {
        napi_value jsArg = NapiValueConverter::ToNapiValue(env, paramStr);
        napi_value return_val;
        status = napi_call_function(env, result, func, 1, &jsArg, &return_val);
        if (status != napi_ok) {
            LOGW("callNativeMethod napi_call_function fail, status=%{public}d", status);
            return "";
        }
            
        if (!NapiValueConverter::ToCppValue(env, return_val, methodResult)) {
            // Handle erroe here
            LOGE("NapiValueConverter::ToCppValue Fail");
        }
        return methodResult;
    }
    
    std::promise<std::string> promise;
    std::function<void(std::string)> cb = [&promise](std::string message)
    {
        promise.set_value(message);
    };
    AsyncCallParam *callParam = new AsyncCallParam{cb, paramStr, func};
    JSFunction::getFunction("HandleMessageUtils.executeNativeMethod").invokeAsync(callParam);
    
    methodResult = promise.get_future().get();
    return methodResult;
}

char* NapiHelper::__getModuleInfo(const char* module_name)
{
    if (bundle_name == NULL) {
        OH_NativeBundle_ApplicationInfo info = OH_NativeBundle_GetCurrentApplicationInfo();
        bundle_name = info.bundleName;
    }
    
    char* module_info = (char*)malloc((strlen(bundle_name) + strlen(module_name) + 1) * sizeof(char*));
    strcpy(module_info, "");
    strcat(module_info, bundle_name);
    strcat(module_info, "/");
    strcat(module_info, module_name);
    
    return module_info;
}

std::string NapiHelper::postCmdToMain(std::string data) {
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__postCmdToMain, this, data));
    return eventResult;
}

std::string NapiHelper::__postCmdToMain(std::string data) {
    if (auto post = aki::JSBind::GetJSFunction("HandleMessageUtils.postCmdToMain")) {
        eventResult = post->Invoke<std::string>(data);
    }
    return eventResult;
}

void NapiHelper::handleCloseWebview()
{
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__handleCloseWebview, this));
}

void NapiHelper::__handleCloseWebview()
{
    if (auto handle = aki::JSBind::GetJSFunction("WebUtils.closeWebview")) {
        handle->Invoke<void>();
    }
}

void NapiHelper::handleCreateWebview(const char *sUrl, int x, int y, int w, int h, bool bCloseWebview) {
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__handleCreateWebview, this, sUrl, x, y, w, h, bCloseWebview));
}

void NapiHelper::__handleCreateWebview(const char *sUrl, int x, int y, int w, int h, bool bCloseWebview) {
    if (auto handle = aki::JSBind::GetJSFunction("WebUtils.createWebview")) {
        handle->Invoke<void>(sUrl, x, y, w, h, bCloseWebview);
    }
}

void NapiHelper::handleCallWebviewJS(const char *sFunctionName, const char *sJsonParam, const char *sCallbackFunction) {
    JCConch::s_pConchRender->setInterruptFunc(
        std::bind(&NapiHelper::__handleCallWebviewJS, this, sFunctionName, sJsonParam, sCallbackFunction));
}

void NapiHelper::__handleCallWebviewJS(const char *sFunctionName, const char *sJsonParam, const char *sCallbackFunction) {
    if (auto handle = aki::JSBind::GetJSFunction("WebUtils.callWebViewJS")) {
        handle->Invoke<void>(sFunctionName, sJsonParam, sCallbackFunction);
    }
}

void NapiHelper::handleHideWebview() {
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__handleHideWebview, this));
}

void NapiHelper::__handleHideWebview() {
    if (auto handle = aki::JSBind::GetJSFunction("WebUtils.hideWebview")) {
        handle->Invoke<void>();
    }
}

void NapiHelper::handleShowWebview() {
    JCConch::s_pConchRender->setInterruptFunc(std::bind(&NapiHelper::__handleShowWebview, this));
}

void NapiHelper::__handleShowWebview() {
    if (auto handle = aki::JSBind::GetJSFunction("WebUtils.showWebView")) {
        handle->Invoke<void>();
    }
}

std::unordered_map<std::string, JSFunction> JSFunction::FUNCTION_MAP;