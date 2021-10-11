/**
@file			JCScriptRuntime.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_13
*/

#include "JCScriptRuntime.h"
#include <algorithm>
#include <util/Log.h>
#include "JSWrapper/JSInterface/JSInterface.h"
#include "JSWrapper/LayaWrap/JSFileReader.h"
#include "JSWrapper/LayaWrap/JSGlobalExportCFun.h"
#include "JSWrapper/LayaWrap/JSInput.h"
#include <downloadCache/JCFileSource.h>
#include <resource/JCFileResManager.h>
#include "Audio/JCAudioManager.h"
#include "JCSystemConfig.h"
#include "JCConch.h"
#include <Performance/JCPerfHUD.h>
#include <downloadMgr/JCDownloadMgr.h>
#include <inttypes.h>
#include "JSWrapper/LayaWrap/JSCallbackFuncObj.h"
#include "JSWrapper/LayaWrap/JSLayaGL.h"
#include <LayaGL/JCLayaGLDispatch.h>
#include <webglplus/JCWebGLPlus.h>
#ifdef JS_V8
    #include "JSWrapper/v8debug/debug-agent.h"
#endif
//#include "btBulletDynamicsCommon.h"

extern int g_nInnerWidth;
extern int g_nInnerHeight;
extern bool g_bGLCanvasSizeChanged;

#ifdef ANDROID
    #include <android/configuration.h>
    #include <dlfcn.h>
    // Declaration for native chreographer API.
    struct AChoreographer;
    typedef void(*AChoreographer_frameCallback)(long frameTimeNanos, void* data);
    typedef AChoreographer* (*func_AChoreographer_getInstance)();
    typedef void(*func_AChoreographer_postFrameCallback)(
        AChoreographer* choreographer, AChoreographer_frameCallback callback,
        void* data);

    // Function pointers for native Choreographer API.
    func_AChoreographer_getInstance AChoreographer_getInstance_;
    func_AChoreographer_postFrameCallback AChoreographer_postFrameCallback_;

    void choreographer_callback(long frameTimeNanos, void* data);
    double lastVSYNC = 0.0;
    void StartChoreographer() 
    {
        auto choreographer = AChoreographer_getInstance_();
        AChoreographer_postFrameCallback_(choreographer, choreographer_callback, nullptr);
    }

    void choreographer_callback(long frameTimeNanos, void* data) 
    {
        //LOGE("---TM:"PRIu64",d=%f", frameTimeNanos,(frameTimeNanos- lastVSYNC)/(1e6f));
        double vsynctm = ((unsigned long)frameTimeNanos) / 1e6;
        auto ctm = laya::tmGetCurms();
        //LOGE("---TM:%f,d=%f,cur=%f,d=%f", (float)vsynctm, (float)(vsynctm - lastVSYNC) , ctm,(ctm-vsynctm));
        laya::JCPerfHUD::m_tmVSYNC = vsynctm;
        if (laya::JCScriptRuntime::s_JSRT) {
            laya::JCScriptRuntime::s_JSRT->onVSyncEvent(vsynctm);
        }
        lastVSYNC = vsynctm;
        StartChoreographer();
    }

    void initChoreographer() 
    {
        return;//不用了，全部用java更方便一些。
               //>=24
        void* lib = dlopen("libandroid.so", RTLD_NOW | RTLD_LOCAL);
        if (lib != nullptr) 
        {
            LOGE("Run with Choreographer Native API.");
            //api_mode_ = kAPINativeChoreographer;

            // Retrieve function pointers from shared object.
            AChoreographer_getInstance_ = reinterpret_cast<func_AChoreographer_getInstance>(dlsym(lib, "AChoreographer_getInstance"));
            AChoreographer_postFrameCallback_ = reinterpret_cast<func_AChoreographer_postFrameCallback>(dlsym(lib, "AChoreographer_postFrameCallback"));
            //assert(AChoreographer_getInstance_);
            //assert(AChoreographer_postFrameCallback_);

            //开始
            if (AChoreographer_getInstance_)
                StartChoreographer();
        }
    }
#endif

namespace laya 
{
    JCScriptRuntime* JCScriptRuntime::s_JSRT = NULL;
    JCScriptRuntime::JCScriptRuntime() 
    {
#ifdef ANDROID
        initChoreographer();
#endif
        s_JSRT = this;
        m_pPoster = NULL;
        m_bHasJSThread = false;
        m_pFileResMgr = NULL;
        m_pAssetsRes = NULL;
        m_bIsExit = false;
        m_pUrl = new JCUrl();
        m_nThreadState = 0;
        m_bJSOnBackPressedFunctionSet = false;
        m_bHasPostVsync = false;
        m_pArrayBufferManager = JCWebGLPlus::getInstance()->m_pJSArrayBufferManager;
        m_pABManagerSyncToRender = JCWebGLPlus::getInstance()->m_pJSABManagerSyncToRender;
        m_pCallbackFuncManager = new JCOrderResManager<JSCallbackFuncObj>(false);
        m_pRegister = new JCRegister();
        JCAudioManager::GetInstance();
        if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
        {
            m_pScriptThread = new JSSingleThread();
        }
        else
        {
            m_pScriptThread = new JSMulThread();
        }
        if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_DOUBLE)
        {
            m_pRenderCmd = new JCCommandEncoderBuffer(102400, 1280);
        }
        else
        {
            m_pRenderCmd = new JCCommandEncoderBuffer(0,0);
        }
        m_pGCCmd = new JCCommandEncoderBuffer(2048, 2048);
        //事件现在有问题，只能添加不能删除。所以不要调用多次。
        m_pScriptThread->on(JCWorkerThread::Event_threadStart, std::bind(&JCScriptRuntime::onThreadInit, this, std::placeholders::_1));
        m_pScriptThread->on(JCWorkerThread::Event_threadStop, std::bind(&JCScriptRuntime::onThreadExit, this, std::placeholders::_1));
        m_nFPS = 60;
        m_nDelayTime = 16;
        m_nUpdateCount = 0;
        m_bRunDraw = true;
        m_dbLastUsedVsync = 0;
        m_dbCurVsync = 0;
#ifdef JS_V8
        m_pDbgAgent = nullptr;  
#endif
#ifndef WIN32
		m_pCurEditBox = NULL;
#endif
        m_bReload = false;
    }
    JCScriptRuntime::~JCScriptRuntime() 
    {
        //在这时候js可能还在执行。需要先停止才能释放其运行环境。
#ifdef __APPLE__
        m_pScriptThread->stop();
#else
        if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_DOUBLE)
        {
            m_pScriptThread->stop();
        }
#endif
        if (m_pScriptThread)
        {
            delete m_pScriptThread;
            m_pScriptThread = NULL;
        }
        s_JSRT = NULL;
        m_pFileResMgr = NULL;
        m_pAssetsRes = NULL;
        if (m_pUrl)
        {
            delete m_pUrl;
            m_pUrl = NULL;
        }
        if (m_pCallbackFuncManager)
        {
            delete m_pCallbackFuncManager;
            m_pCallbackFuncManager = NULL;
        }
        if (m_pRegister)
        {
            delete m_pRegister;
            m_pRegister = NULL;
        }
        if (m_pRenderCmd)
        {
            delete m_pRenderCmd;
            m_pRenderCmd = NULL;
        }
        if (m_pGCCmd)
        {
            delete m_pGCCmd;
            m_pGCCmd = NULL;
        }

        JCWebGLPlus::releaseInstance();

    }
    void  JCScriptRuntime::init(JCFileResManager* pFileMgr, JCFileSource* pAssetRes, IConchThreadCmdMgr* pThreadCmdSender) 
    {
        m_pFileResMgr = pFileMgr;
        m_pAssetsRes = pAssetRes;
        m_pPoster = pThreadCmdSender;
    }
    void JCScriptRuntime::start(const char* pStartJS) 
    {
        LOGI("Start js %s", pStartJS);
        if (pStartJS)m_strStartJS = pStartJS;
        m_pScriptThread->initialize(JCConch::s_pConch->m_nJSDebugPort);
#ifdef __APPLE__
        m_pScriptThread->setLoopFunc(std::bind(&JCScriptRuntime::onUpdate, this));
#endif
        m_nThreadState = 1;
        m_pScriptThread->start();
    }
    void JCScriptRuntime::stop()
    {
        LOGI("Stop js start...");
        while (m_nThreadState==1) 
        {
            LOGI("stop: wait for thread to start...");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
#ifdef __APPLE__
        if (JCConch::s_pConchRender)
        {
            while (JCConch::s_pConchRender->m_kRenderSem.getDataNum() == 1)
            {
                JCConch::s_pConchRender->renderFrame(0, false);
            }
        }
#endif
        m_pScriptThread->stop();
        m_pScriptThread->uninitialize();
        LOGI("Stop js end.");
    }
    void JCScriptRuntime::reload() 
    {
        m_bReload = false;
#ifdef __APPLE__
        if(JCConch::s_pConchRender)
        {
            JCConch::s_pConchRender->m_bStopRender = true;
        }
#else
        if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
        {
            JCConch::s_pConchRender->m_bStopRender = true;
        }
#endif
        
        stop();
        //为了避免上个js的下载的影响，去掉下载任务
        //这个必须在stop之后做，且保证stop确实会停止线程，包括还没有启动的线程。
        //如果线程还没有启动，stop会等待线程先启动。这样就可能会执行脚本，所以，清理必须放在stop之后。
        JCDownloadMgr* pNetLoader = JCDownloadMgr::getInstance();
        pNetLoader->stopCurTask();
        pNetLoader->clearAllAsyncTask();
        pNetLoader->resetDownloadTail();	//防止第二次进入的时候，下载错误的dcc等
        pNetLoader->resetFinalReplacePath();
        pNetLoader->resetDownloadReplaceExt();
        //文件资源不能跨js环境使用，所以必须clear
        // 例如一个资源正在下载，则可能的问题：1.可能会上个线程取消了，不会再回调， 2. 自己希望回调的是上个js环境，也无法传给新的js环境。
        // 所以需要clear。
        m_pFileResMgr->clear();
        start(m_strStartJS.c_str());
#ifndef __APPLE_
        if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
        {
            if (JCConch::s_pConch)
            {
                JCConch::s_pConch->postCmdToMainThread(JCConch::CMD_MgrStartThread, 0, 0);
            }
        }
#endif
    }
    void JCScriptRuntime::onThreadInit(JCEventEmitter::evtPtr evt) 
    {
        LOGI("js thread started.");
        m_nThreadState = 2;
        JCPerfHUD::resetFrame();
#ifdef JS_V8
        JSObjNode::s_pListJSObj = new JCSimpList();
#ifdef JS_V8_DEBUGGER
        if (m_pDbgAgent) 
        {
            m_pDbgAgent->onJSStart(m_pScriptThread,(JCConch::s_pConch->m_nJSDebugMode == JS_DEBUG_MODE_WAIT) ? true : false);
            LOGI("js debug open mode: %d port %d", JCConch::s_pConch->m_nJSDebugMode, JCConch::s_pConch->m_nJSDebugPort);
        }
        else
        {
            LOGI("js debug closed");
        }
#endif
#endif
        JCConch::s_pConchRender->m_pImageManager->resetJSThread();

        //JS线程的数据清空一下
        m_pCallbackFuncManager->clearAllRes();
        m_pCallbackFuncManager->resetGlobalID();
        m_pArrayBufferManager->clearAll();
        m_pABManagerSyncToRender->clearAll();

        //给渲染线程发开始指令
        startScriptOnRenderThread();
        JsFile::RegisterToJS();
        JsFileReader::RegisterToJS();
        JSGlobalExportC();
        //设置js的一些环境。必须在所有导出之后，执行其他脚本之前。
#ifndef WIN32
        JSP_RUN_SCRIPT((const char*)"function getExePath(){return null;}");
#endif
        {
            char* sJSRuntime = NULL;
            int nSize = 0;
            if (m_pAssetsRes->loadFileContent("scripts/runtimeInit.js", sJSRuntime, nSize))
            {
                JSP_RUN_SCRIPT(sJSRuntime);
                delete[] sJSRuntime;
            }
        }
        char* sJCBuffer = NULL;
        int nJSSize = 0;
        if (m_pAssetsRes->loadFileContent(m_strStartJS.c_str(), sJCBuffer, nJSSize))
        {
            std::string kBuf = "(function(window){\n'use strict'\n";
            kBuf += sJCBuffer;
            kBuf += "\n})(window);\n//@ sourceURL=apploader.js";
            #ifdef JS_V8
                v8::Isolate* isolate = v8::Isolate::GetCurrent();
                v8::HandleScope handle_scope(isolate);
                v8::TryCatch try_catch(isolate);
                JSP_RUN_SCRIPT(kBuf.c_str());
                if (try_catch.HasCaught())
                {
                    __JSRun::ReportException(isolate, &try_catch);
                }
            #else
                JSP_RUN_SCRIPT(kBuf.c_str());
            #endif	
            delete[] sJCBuffer;
            sJCBuffer = NULL;
        }
#ifndef __APPLE__
        if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_DOUBLE)
        {
            m_pScriptThread->post(std::bind(&JCScriptRuntime::onUpdate, this));
        }
#endif
        JSP_RUN_SCRIPT("gc();gc();gc();");
    }
    void JCScriptRuntime::onThreadExit(JCEventEmitter::evtPtr evt)
    {
        if (m_nThreadState == 0)
        {
            return;
        }
        LOGI("js thread exiting...");
        m_nThreadState = 0;
        m_pJSOnFrameFunction.Reset();
        m_pJSOnResizeFunction.Reset();
        m_pJSOnFocusFunction.Reset();
        m_pJSOnBlurFunction.Reset();
        m_pJSMouseEvtFunction.Reset();
        m_pJSKeyEvtFunction.Reset();
        m_pJSTouchEvtFunction.Reset();
        m_pJSDeviceMotionEvtFunction.Reset();
        m_pJSNetworkEvtFunction.Reset();
        m_pJSOnBackPressedFunction.Reset();
        m_pJSOnceOtherEvtFuction.Reset();
        m_pJSOnDrawFunction.Reset();
        m_bJSBulletGetWorldTransformHandle.Reset();
        m_bJSBulletSetWorldTransformHandle.Reset();
#ifndef WIN32
        m_pCurEditBox = NULL;
#endif
        JSClassMgr::GetInstance()->resetAllRegClass();
        
#ifdef JS_V8
        JCSimpList* pNodeLists = JSObjNode::s_pListJSObj;
        if (pNodeLists != NULL)
        {
            JCListNode* pCur = pNodeLists->begin();
            JCListNode* pEnd = pNodeLists->end();
            while (pCur != pEnd)
            {
                JSObjNode* pJsCur = (JSObjNode*)pCur;
                pCur = pNodeLists->delNode(pCur);
                delete pJsCur;
            }
            delete JSObjNode::s_pListJSObj;
            JSObjNode::s_pListJSObj = nullptr;
        }
#ifdef JS_V8_DEBUGGER
        if (m_pDbgAgent)
        {
            m_pDbgAgent->onJSExit();
        }
#endif
#elif JS_JSC
        JSP_RESET_GLOBAL_FUNCTION;
#endif

        JCAudioManager::ClearAllWork();
        JCAudioManager::GetInstance()->stopMp3();
        JCAudioManager::GetInstance()->pauseMp3();
        m_pCallbackFuncManager->clearAllRes();
        m_pCallbackFuncManager->resetGlobalID();
        JCWebGLPlus::getInstance()->clearAll();
    }
    bool JCScriptRuntime::onUpdate() 
    {
        PERF_INITVAR(nBenginTime);
#ifdef JS_V8
        m_pScriptThread->runDbgFuncs();
#endif
        m_nUpdateCount++;
        bool bRunOnDraw = false;
        double nTime = tmGetCurms();
#ifdef ANDROID1
        if (m_bRunDraw) 
        {
            m_bRunDraw = false;
            bRunOnDraw = true;
            m_dbLastUsedVsync = m_dbCurVsync;
            onUpdateDraw(m_dbCurVsync);
        }
#else
        if (!g_kSystemConfig.m_bUseChoreographer) 
        {
            JCPerfHUD::m_tmVSYNC = nTime;    
        }
#endif
        onUpdateDraw(JCPerfHUD::m_tmVSYNC);
        JSInput* pInput = JSInput::getInstance();
        if ( pInput->m_bTouchMode )
        {
            pInput->swapCurrentTouchEvent();
            if( pInput->m_vInputEventsJS.size() > 0 )
            {
                pInput->m_nTouchFrame = 120;
                for (int i = 0, nSize = (int)pInput->m_vInputEventsJS.size(); i < nSize; i++ )
                {
                    TouchEventInfo* touchEvent = &pInput->m_vInputEventsJS[i];
                    m_pJSTouchEvtFunction.Call(touchEvent->nType, touchEvent->nID,"type",touchEvent->x, touchEvent->y);
                }
            }
            if( pInput->m_nTouchFrame > 0 )
            {
                pInput->m_nTouchFrame--;
            }
        }
        if (g_bGLCanvasSizeChanged)
        {
            m_pJSOnResizeFunction.Call(g_nInnerWidth, g_nInnerHeight);
            //m_pRootCanvas->size( g_nInnerWidth,g_nInnerHeight );
            g_bGLCanvasSizeChanged = false;
        }
        int nUpdateNum = m_nUpdateCount % 3;
        switch (nUpdateNum)
        {
        case 0:
            JCAudioManager::GetInstance()->update();
            break;
        case 1:
            //如果有需要清理的或者update可以放到这 
            JCAudioManager::GetInstance()->m_pWavPlayer->autoGarbageCollection();
            break;
        case 2:
            //如果有需要清理的或者update可以放到这
            break;
        }
        JS_TRY;
        m_pJSOnFrameFunction.Call();
        JS_CATCH;
        float dt = tmGetCurms() - nBenginTime;
        PERF_UPDATE_DATA(JCPerfHUD::PHUD_JS_DELAY, (float)dt);
        return true;
    }
    void JCScriptRuntime::onUpdateDraw(double vsyncTime) 
    {
        m_bHasPostVsync = false;
        if (!m_pJSOnDrawFunction.Empty()) 
        {
            JS_TRY;
            m_pJSOnDrawFunction.Call(vsyncTime);
            JS_CATCH;
            runLayaGL();
        }
    }
    void JCScriptRuntime::runLayaGL()
    {
        JSLayaGL* pLayaGL = JSLayaGL::getInstance();
        if (pLayaGL->m_nFrameAndSyncCountABListID != -1)
        {
            //恢复frameCount和syncCount
            JCArrayBufferManager::ArrayBufferContent* pBuffer = m_pArrayBufferManager->getArrayBuffer(pLayaGL->m_nFrameAndSyncCountABListID);
            if (pBuffer)
            {
                int* pData = (int*)pBuffer->m_pBuffer;
                pLayaGL->m_nSyncArrayBufferCount=pData[1];
                pData[0]++;
                pData[1]=0;
                pLayaGL->m_nFrameCount = pData[0];
            }
        }
        if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_DOUBLE)
        {
            flushSharedCmdBuffer();
            if (m_pGCCmd->getDataSize() > 0)
            {
                m_pRenderCmd->append(m_pGCCmd->getBuffer(), m_pGCCmd->getDataSize());
                m_pGCCmd->clearData();
            }
            if (pLayaGL->m_nSyncToRenderABListID != -1)
            {
                JCArrayBufferManager::ArrayBufferContent* pSyncBufferList = m_pArrayBufferManager->getArrayBuffer(pLayaGL->m_nSyncToRenderABListID);
                JCConch::s_pConchRender->setRenderData(m_pABManagerSyncToRender, pSyncBufferList, pLayaGL->m_nSyncArrayBufferCount, m_pRenderCmd, m_nDelayTime, m_nFPS);
            }
            else
            {
                JCConch::s_pConchRender->setRenderData(NULL, NULL, 0, m_pRenderCmd, m_nDelayTime, m_nFPS);
            }
        }
        else
        {
            dispatchLayaGLBuffer(true);
        }
    }
    void JCScriptRuntime::dispatchLayaGLBuffer(bool bDispatchGC)
    {
        JCArrayBufferManager::ArrayBufferContent* pRootCommandEncoder = JSLayaGL::getInstance()->m_pRootCommandEncoder;
        if (!pRootCommandEncoder)return;
        char* pBuffer = pRootCommandEncoder->m_pBuffer;
        int nLen = (*(int*)pBuffer - 1) * 4;
        m_pRenderCmd->setShareBuffer(pBuffer + 4, nLen);
        ((int*)pBuffer)[0] = 1;
        JCLayaGLDispatch::dispatchAllCmds(m_pRenderCmd);
        m_pRenderCmd->clearData();
        if (bDispatchGC && m_pGCCmd->getDataSize() > 0)
        {
            JCLayaGLDispatch::dispatchAllCmds(m_pGCCmd);
            m_pGCCmd->clearData();
        }
    }
    void JCScriptRuntime::flushSharedCmdBuffer()
    {
        JCArrayBufferManager::ArrayBufferContent* pRootCommandEncoder = JSLayaGL::getInstance()->m_pRootCommandEncoder;
        if (!pRootCommandEncoder)return;
        char* pBuffer = pRootCommandEncoder->m_pBuffer;
        int nLen = (*(int*)pBuffer - 1)*4;
        if (nLen > 0)
        {
            //TODO想办法不拷贝，改成share
            m_pRenderCmd->append(pBuffer + 4, nLen);
            ((int*)pBuffer)[0] = 1;
        }
    }
    void JCScriptRuntime::onVSyncEvent(double vsyncTime) 
    {
        JCPerfHUD::m_tmVSYNC = vsyncTime;
        m_dbCurVsync = vsyncTime;
        //m_bRunDraw = true;
        //这个事件的速度始终固定，但是js可能会非常卡，导致大量积累，所以要保护 一下。
        if (!m_bHasPostVsync)
        {
            m_bHasPostVsync = true;
            m_pScriptThread->post(std::bind(&JCScriptRuntime::onUpdate, this));
        }
    }
    void JCScriptRuntime::dispatchInputEvent(inputEvent e)
    {
        JSInput::getInstance()->activeCall(e);
    }
    void JCScriptRuntime::dispatchInputEvent(DeviceOrientationEvent e)
    {
        JSInput::getInstance()->activeCall(e);
    }
    void JCScriptRuntime::dispatchInputEvent(DeviceMotionEvent e)
    {
        JSInput::getInstance()->activeCall(e);
    }
    void JCScriptRuntime::onNetworkChanged(int nType)
    {
        std::function<void(void)> pFunction = std::bind(&JCScriptRuntime::onNetworkChangedCallJSFunction, this, nType);
        m_pScriptThread->post(pFunction);
    }
    void JCScriptRuntime::onNetworkChangedCallJSFunction(int nType)
    {
        m_pJSNetworkEvtFunction.Call(nType);
    }
    void JCScriptRuntime::jsGC()
    {
        std::function<void(void)> pFunction = std::bind(&JCScriptRuntime::jsGCCallJSFunction, this);
        m_pScriptThread->post(pFunction);
    }
    void JCScriptRuntime::jsGCCallJSFunction()
    {
        JSP_RUN_SCRIPT("gc()");
    }
    void JCScriptRuntime::callJC(std::string sFunctionName, std::string sJsonParam, std::string sCallbackFunction)
    {
        std::function<void(void)> pFunction = std::bind(&JCScriptRuntime::callJSFuncton, this, sFunctionName, sJsonParam, sCallbackFunction );
        m_pScriptThread->post(pFunction);
    }
    void JCScriptRuntime::callJSString( std::string sBuffer )
    {
        std::function<void(void)> pFunction = std::bind(&JCScriptRuntime::callJSStringFunction, this,sBuffer);
        m_pScriptThread->post(pFunction);
    }
    void JCScriptRuntime::callJSStringFunction( std::string sBuffer )
    {
        JSP_RUN_SCRIPT(sBuffer.c_str());
    }
    void JCScriptRuntime::callJSFuncton(std::string sFunctionName, std::string sJsonParam, std::string sCallbackFunction)
    {
        std::string sBuffer = sFunctionName;
        sBuffer += "(\"";
        sBuffer += sJsonParam;
        sBuffer += "\",\"";
        sBuffer += sCallbackFunction;
        sBuffer += "\");";
        LOGI("JCScriptRuntime::callJSFuncton buffer=%s",sBuffer.c_str() );
        JSP_RUN_SCRIPT( sBuffer.c_str() );
    }
    void JCScriptRuntime::restoreAudio()
    {
        std::function<void(void)> pFunction = std::bind(&JCScriptRuntime::jsRestoreAudioFunction, this);
        m_pScriptThread->post(pFunction);
    }
    void JCScriptRuntime::jsRestoreAudioFunction()
    {
        if(JCAudioManager::GetInstance()->getMp3Mute() == false && JCAudioManager::GetInstance()->getMp3Stopped() == false)
        {
            JCAudioManager::GetInstance()->resumeMp3();
        }
    }
    void JCScriptRuntime::jsReloadUrl()
    {
        std::function<void(void)> pFunction = std::bind(&JCScriptRuntime::jsReloadUrlJSFunction, this);
        m_pScriptThread->post(pFunction);
    }
    void JCScriptRuntime::jsReloadUrlJSFunction()
    {
        JSP_RUN_SCRIPT("reloadJS(true)");
    }
    void JCScriptRuntime::jsUrlback()
    {
        std::function<void(void)> pFunction = std::bind(&JCScriptRuntime::jsUrlbackJSFunction, this);
        m_pScriptThread->post(pFunction);
    }
    void JCScriptRuntime::jsUrlbackJSFunction()
    {
        JSP_RUN_SCRIPT("history.back()");
    }
    void JCScriptRuntime::startScriptOnRenderThread()
    {
        m_pRenderCmd->clearData();
        m_pGCCmd->clearData();
        if (JCConch::s_pConch)
        {
#ifdef __APPLE__
            JCConch::s_pConch->postCmdToMainThread(JCConch::CMD_ClearRender, 0, 0);
#else
			if (JCConch::s_pConchRender){
				JCConch::s_pConchRender->clearAllData();
			}
#endif
        }
    }
	bool JCScriptRuntime::onBackPressed()
	{
		std::lock_guard<std::mutex> lock(m_OnBackPressedMutex);
		if (!m_bJSOnBackPressedFunctionSet)
        {
			return false;
		}
        if (JCScriptRuntime::s_JSRT->m_pPoster)
        {
            JCScriptRuntime::s_JSRT->m_pPoster->postToJS([this]() {
                this->m_pJSOnBackPressedFunction.Call();
            });
        }
		return true;
	}
    void JCScriptRuntime::postToJS(const std::function<void(void)>& func)
    {
        m_pScriptThread->post(func);
    }
    void JCScriptRuntime::postToDownload(const std::function<void(void)>& funcf)
    {

    }
    void JCScriptRuntime::postToDecoder(const std::function<void(void)>& func)
    {

    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
