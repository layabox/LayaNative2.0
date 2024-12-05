#include "./NAPIFun.h"
#include "helper/NapiHelper.h"
using namespace laya;

NAPIFun NAPIFun::fun_;
bool NAPIFun::m_bTakeScreenshot = false;

void NAPIFun::postCmdToMainThread(int p_nCmd, int p_nParam1, int p_nParam2)
{
    std::string data = to_string(p_nCmd);
    NapiHelper::GetInstance()->postCmdToMain(data);
}
void NAPIFun::ConchNAPI_configSetParamExt(std::string p_strParamExt)
{
    g_kSystemConfig.m_jsonparamExt = p_strParamExt;
}
void NAPIFun::ConchNAPI_configSetURL(std::string p_strUrl)
{
    g_kSystemConfig.m_strStartURL = p_strUrl;
}
void NAPIFun::ConchNAPI_configSetIsPlug(bool p_bIsPlug)
{
    JCSystemConfig::s_bIsPlug = p_bIsPlug;
}
void NAPIFun::ConchNAPI_InitDLib(napi_value assetManager, int nThreadNum, std::string p_strAssetRootPath, std::string p_strCachePath, int threadMode, int debugMode, int debugPort)
{
    auto fun = NAPIFun::GetInstance();
    LOGI("NAPI InitDLib");
    if (fun->g_pConch)
    {
        LOGI("NAPI has an old conch object! delete it");
        // 如果上次不正常退出，如果时间太短，可能有的线程还在创建过程中。所以等待一会儿。
        // 例如g_pConch突然为null，可能有人还在用。
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        // 必须走完整流程，例如有的线程还在创建，完了后就正常跑，然后这里g_pConch又被删了
        NAPIFun::ConchNAPI_ReleaseDLib();
        LOGI("NAPI del old end");
    }
    // 这个不要放到开始，以影响面上面的异常处理
    fun->g_nInitTime = tmGetCurms();

    gRedistPath = p_strCachePath;
    gRedistPath += "/";
    gAssetRootPath = p_strAssetRootPath;

    napi_env env = aki::JSBind::GetScopedEnv();
    g_pAssetManager = OH_ResourceManager_InitNativeResourceManager(env, assetManager);

    laya::JCOHOSFileSource *pAssets = new laya::JCOHOSFileSource();
    pAssets->Init(g_pAssetManager, "");
    JCConch::s_pAssetsFiles = pAssets;

    THREAD_MODE nMode = (THREAD_MODE)(threadMode);
    if (nMode == THREAD_MODE_SINGLE)
    {
        g_kSystemConfig.m_nThreadMODE = nMode;
        LOGI(">>>>>>Thread Mode = single");
    }
    else if (nMode == THREAD_MODE_DOUBLE)
    {
        g_kSystemConfig.m_nThreadMODE = nMode;
        LOGI(">>>>>>Thread Mode = double");
    }
    else
    {
        LOGI(">>>>>>Thread Mode = %d", g_kSystemConfig.m_nThreadMODE);
    }

    fun->g_pConch = new laya::JCConch(nThreadNum, (laya::JS_DEBUG_MODE)debugMode, debugPort);
    fun->g_pConch->m_funcPostMsgToMainThread = std::bind(NAPIFun::postCmdToMainThread, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

void NAPIFun::ConchNAPI_SetLocalStoragePath(std::string p_strLocalStorage)
{
    LOGI("NAPI localStoragePath=%{public}s", p_strLocalStorage.c_str());
}
void NAPIFun::ConchNAPI_ReleaseDLib()
{
    LOGI("NAPI del engine");
    JCAudioManager::GetInstance()->stopMp3();
    auto fun = NAPIFun::GetInstance();

    if (fun->g_pConch)
    {
        if (tmGetCurms() - fun->g_nInitTime < 2000)
        {
            LOGI("NAPI exit need a little wait");
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        if (fun->g_bInBKGround)
        {
            LOGI("NAPI App in background!");
            JCConch::s_pConchRender->willExit();
        }
        fun->g_pConch->onAppDestory();
        if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
        {
            JCScriptRuntime::s_JSRT->stop();
        }
        delete fun->g_pConch;
        fun->g_pConch = NULL;
    }
    fun->g_bEngineInited = false;
}
void NAPIFun::ConchNAPI_OnAppDestroy()
{
}
void NAPIFun::ConchNAPI_OnAppPause()
{
    LOGI("NAPI OnAppPause");
    auto fun = NAPIFun::GetInstance();
    fun->g_bInBKGround = true;
    if (laya::JCAudioManager::GetInstance()->getMp3Mute() == false && laya::JCAudioManager::GetInstance()->getMp3Stopped() == false)
    {
        JCAudioManager::GetInstance()->pauseMp3();
    }
    laya::JCAudioManager::GetInstance()->m_pWavPlayer->pause();
}
void NAPIFun::ConchNAPI_OnAppResume()
{
    LOGI("NAPI OnAppResume");
    auto fun = NAPIFun::GetInstance();

    if (!fun->g_pConch)
        return;
    fun->g_bInBKGround = false;
    // 继续声音
    if (laya::JCAudioManager::GetInstance()->getMp3Mute() == false && laya::JCAudioManager::GetInstance()->getMp3Stopped() == false)
    {
        laya::JCAudioManager::GetInstance()->resumeMp3();
    }
    laya::JCAudioManager::GetInstance()->m_pWavPlayer->resume();
}
void NAPIFun::ConchNAPI_OnGLReady(int width, int height)
{
    LOGI("NAPI onGLReady tid=%ld", gettidv1());
    auto pRender = JCConch::s_pConchRender;
    if (g_nInnerWidth != width || g_nInnerHeight != height)
    {
        LOGI("NAPI surface innersize changed : g_nInnerWidth=%d,g_nInnerHeight=%d", width, height);
        g_nInnerWidth = width;
        g_nInnerHeight = height;
        g_bGLCanvasSizeChanged = true;
    }
    LOGI("NAPI init dev w=%d,h=%d", width, height);
    pRender->onGLReady();
}
void NAPIFun::ConchNAPI_OnAppStart()
{
    auto fun = NAPIFun::GetInstance();
    if (fun->g_bEngineInited)
    {
        return;
    }
    fun->g_kReadyLock.lock();
    // 启动js线程
    fun->g_pConch->onAppStart();
    fun->g_bEngineInited = true;
    if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
    {
        JCScriptRuntime::s_JSRT->start(JCConch::s_pConch->m_strStartJS.c_str());
    }

    fun->g_kReadyLock.unlock();
}
void NAPIFun::ConchNAPI_onDrawFrame()
{
    auto pRender = JCConch::s_pConchRender;
    if (pRender)
    {
        pRender->renderFrame(0, false);
    }
}
void NAPIFun::ConchNAPI_onVSyncCallback(long VSynctm)
{
    double vsynctm = VSynctm / 1e6;
    if (JCScriptRuntime::s_JSRT)
    {
        JCScriptRuntime::s_JSRT->onVSyncEvent(vsynctm);
    }
}
void NAPIFun::ConchNAPI_audioMusicPlayEnd()
{
    laya::JCMp3Interface *pMp3Player = laya::JCAudioManager::GetInstance()->m_pMp3Player;
    if (pMp3Player)
    {
        pMp3Player->onPlayEnd();
    }
}
void NAPIFun::ConchNAPI_networkChanged(int nNetworkType)
{
    JCScriptRuntime::s_JSRT->onNetworkChanged(nNetworkType);
}
void NAPIFun::ConchNAPI_inputChange(int keycode)
{
    if (JCScriptRuntime::s_JSRT->m_pCurEditBox)
    {
        JCScriptRuntime::s_JSRT->m_pCurEditBox->onInput();
    }
}
void NAPIFun::ConchNAPI_setLocalizable(bool p_bIsLocalPackage)
{
    JCSystemConfig::s_bLocalizable = p_bIsLocalPackage;
    LOGI("setLocalizable：%d", p_bIsLocalPackage);
}
void NAPIFun::ConchNAPI_captureScreenCallBack(int w, int h, std::string byteArray)
{
}
void NAPIFun::ConchNAPI_RunJS(const std::string &js)
{
     JCScriptRuntime::s_JSRT->callJSString(js);
}
void NAPIFun::ConchNAPI_onRunCmd(std::string cmd)
{
    auto func = NAPIFun::GetInstance();

    if(func->g_pConch) {
        func->g_pConch->onRunCmdInMainThread(atoi(cmd.c_str()),0,0);
    }
}

void NAPIFun::ConchNAPI_gameMsgHandle(std::string key, std::string value)
{
    JCScriptRuntime::s_JSRT->onJsObjHandle(key,value);
}

JSBIND_GLOBAL()
{
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_configSetParamExt, "ConchNAPI_configSetParamExt");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_configSetURL, "ConchNAPI_configSetURL");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_configSetIsPlug, "ConchNAPI_configSetIsPlug");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_InitDLib, "ConchNAPI_InitDLib");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_SetLocalStoragePath, "ConchNAPI_SetLocalStoragePath");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_ReleaseDLib, "ConchNAPI_ReleaseDLib");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_OnAppDestroy, "ConchNAPI_OnAppDestroy");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_OnAppPause, "ConchNAPI_OnAppPause");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_OnAppResume, "ConchNAPI_OnAppResume");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_OnGLReady, "ConchNAPI_OnGLReady");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_OnAppStart, "ConchNAPI_OnAppStart");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_onDrawFrame, "ConchNAPI_onDrawFrame");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_onVSyncCallback, "ConchNAPI_onVSyncCallback");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_audioMusicPlayEnd, "ConchNAPI_audioMusicPlayEnd");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_networkChanged, "ConchNAPI_networkChanged");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_inputChange, "ConchNAPI_inputChange");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_setLocalizable, "ConchNAPI_setLocalizable");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_captureScreenCallBack, "ConchNAPI_captureScreenCallBack");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_RunJS, "ConchNAPI_RunJS");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_onRunCmd, "ConchNAPI_onRunCmd");
    JSBIND_FUNCTION(NAPIFun::ConchNAPI_gameMsgHandle, "ConchNAPI_gameMsgHandle");
}
