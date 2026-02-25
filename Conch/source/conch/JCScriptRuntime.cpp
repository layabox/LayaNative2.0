/**
@file			JCScriptRuntime.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_13
*/

#include "JCScriptRuntime.h"
#include <algorithm>
#include <cstring>
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
#include "JSWrapper/LayaWrap/JSPromiseRejectionEvent.h"
#ifdef JS_V8
    #include "JSWrapper/v8debug/debug-agent.h"
#elif JS_JSVM
    #include "JSWrapper/v8debug/debug-agent.h"
    #include "ark_runtime/jsvm.h"
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
#elif JS_JSVM
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
#if defined(JS_V8) || defined(JS_JSVM)
    // Promise rejection pipeline - engine-specific helpers

    void JCScriptRuntime::ReleaseRecord(PromiseRejectionRecord& record)
    {
#ifdef JS_V8
        record.promise.Reset();
        record.reason.Reset();
#elif JS_JSVM
        auto env = Javascript::getEnv();
        JSVM_Status status;
        if (record.promise) { JSVM_API_CALL(status, env, OH_JSVM_DeleteReference(env, record.promise)); }
        if (record.reason) { JSVM_API_CALL(status, env, OH_JSVM_DeleteReference(env, record.reason)); }
#endif
    }

    JCScriptRuntime::PromiseRejectionList::iterator JCScriptRuntime::FindPromiseRecord(
        PromiseRejectionList& list, JSValueAsParam promise)
    {
#ifdef JS_V8
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        for (auto it = list.begin(); it != list.end(); ++it)
        {
            v8::Local<v8::Value> p = v8::Local<v8::Value>::New(isolate, it->promise);
            if (p->StrictEquals(promise))
            {
                return it;
            }
        }
#elif JS_JSVM
        auto env = Javascript::getEnv();
        JSVM_Status status;
        for (auto it = list.begin(); it != list.end(); ++it)
        {
            JSVM_Value p;
            JSVM_API_CALL(status, env, OH_JSVM_GetReferenceValue(env, it->promise, &p));
            bool equals = false;
            JSVM_API_CALL(status, env, OH_JSVM_StrictEquals(env, p, promise, &equals));
            if (equals)
            {
                return it;
            }
        }
#endif
        return list.end();
    }

    bool JCScriptRuntime::RemovePromiseRecord(PromiseRejectionList& list, JSValueAsParam promise)
    {
        auto it = FindPromiseRecord(list, promise);
        if (it != list.end())
        {
            ReleaseRecord(*it);
            list.erase(it);
            return true;
        }
        return false;
    }

    void JCScriptRuntime::EmitPromiseRejectionEvent(const char* type, const PromiseRejectionRecord& record)
    {
        if (m_vJSOnUnhandledRejectionFunctions.empty())
            return;

#ifdef JS_V8
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::HandleScope scope(isolate);
        v8::Local<v8::Context> context = isolate->GetCurrentContext();

        v8::Local<v8::Object> eventObj = v8::Object::New(isolate);
        eventObj->Set(context,
            v8::String::NewFromUtf8(isolate, "promise").ToLocalChecked(),
            v8::Local<v8::Value>::New(isolate, record.promise)).Check();

        if (record.reasonType == PromiseRejectionRecord::ReasonType::ReasonTypeString)
        {
            eventObj->Set(context,
                v8::String::NewFromUtf8(isolate, "reason").ToLocalChecked(),
                v8::String::NewFromUtf8(isolate, record.reasonString.c_str()).ToLocalChecked()).Check();
        }
        else if (record.reasonType == PromiseRejectionRecord::ReasonType::ReasonTypeObject)
        {
            eventObj->Set(context,
                v8::String::NewFromUtf8(isolate, "reason").ToLocalChecked(),
                v8::Local<v8::Value>::New(isolate, record.reason)).Check();
        }

        eventObj->Set(context,
            v8::String::NewFromUtf8(isolate, "type").ToLocalChecked(),
            v8::String::NewFromUtf8(isolate, type).ToLocalChecked()).Check();

        for (auto& funcGlobal : m_vJSOnUnhandledRejectionFunctions)
        {
            v8::Local<v8::Function> func = v8::Local<v8::Function>::New(isolate, funcGlobal);
            v8::Local<v8::Value> argv[] = { eventObj };
            func->Call(context, context->Global(), 1, argv).FromMaybe(v8::Local<v8::Value>());
        }
#elif JS_JSVM
        auto env = Javascript::getEnv();
        JSVM_Status status;
        AutoHandleScope scope;

        JSVM_Value eventObj;
        JSVM_API_CALL(status, env, OH_JSVM_CreateObject(env, &eventObj));

        JSVM_Value promiseVal;
        JSVM_API_CALL(status, env, OH_JSVM_GetReferenceValue(env, record.promise, &promiseVal));
        JSVM_API_CALL(status, env, OH_JSVM_SetNamedProperty(env, eventObj, "promise", promiseVal));

        if (record.reasonType == PromiseRejectionRecord::ReasonType::ReasonTypeString)
        {
            JSVM_Value reasonVal;
            JSVM_API_CALL(status, env, OH_JSVM_CreateStringUtf8(env, record.reasonString.c_str(), record.reasonString.size(), &reasonVal));
            JSVM_API_CALL(status, env, OH_JSVM_SetNamedProperty(env, eventObj, "reason", reasonVal));
        }
        else if (record.reasonType == PromiseRejectionRecord::ReasonType::ReasonTypeObject)
        {
            JSVM_Value reasonVal;
            JSVM_API_CALL(status, env, OH_JSVM_GetReferenceValue(env, record.reason, &reasonVal));
            JSVM_API_CALL(status, env, OH_JSVM_SetNamedProperty(env, eventObj, "reason", reasonVal));
        }

        JSVM_Value typeVal;
        JSVM_API_CALL(status, env, OH_JSVM_CreateStringUtf8(env, type, strlen(type), &typeVal));
        JSVM_API_CALL(status, env, OH_JSVM_SetNamedProperty(env, eventObj, "type", typeVal));

        JSVM_Value global;
        JSVM_API_CALL(status, env, OH_JSVM_GetGlobal(env, &global));
        for (auto& funcRef : m_vJSOnUnhandledRejectionFunctions)
        {
            JSVM_Value func;
            JSVM_API_CALL(status, env, OH_JSVM_GetReferenceValue(env, funcRef, &func));
            JSVM_Value argv[] = { eventObj };
            JSVM_Value result;
            JSVM_API_CALL(status, env, OH_JSVM_CallFunction(env, global, func, 1, argv, &result));
        }
#endif
    }

    void JCScriptRuntime::EnqueueUnhandledRejection(JSValueAsParam promise, JSValueAsParam reason)
    {
        PromiseRejectionRecord record;
        record.uid = ++m_lastPromiseRejectionId;
#ifdef JS_V8
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        record.promise.Reset(isolate, promise);

        if (!reason.IsEmpty() && reason->IsString())
        {
            record.reasonType = PromiseRejectionRecord::ReasonType::ReasonTypeString;
            v8::String::Utf8Value str(isolate, reason);
            record.reasonString = *str ? *str : "";
        }
        else if (!reason.IsEmpty() && reason->IsObject())
        {
            record.reasonType = PromiseRejectionRecord::ReasonType::ReasonTypeObject;
            record.reasonString = "";
            record.reason.Reset(isolate, reason);
        }
        else
        {
            record.reasonType = PromiseRejectionRecord::ReasonType::ReasonTypeString;
            record.reasonString = "";
        }
#elif JS_JSVM
        auto env = Javascript::getEnv();
        JSVM_Status status;
        JSVM_API_CALL(status, env, OH_JSVM_CreateReference(env, promise, 1, &record.promise));

        bool isString = false;
        bool isObject = false;
        if (reason != nullptr)
        {
            JSVM_API_CALL(status, env, OH_JSVM_IsString(env, reason, &isString));
            if (!isString)
            {
                JSVM_API_CALL(status, env, OH_JSVM_IsObject(env, reason, &isObject));
            }
        }

        if (isString)
        {
            record.reasonType = PromiseRejectionRecord::ReasonType::ReasonTypeString;
            record.reasonString = __TransferToCpp<std::string>::ToCpp(reason);
        }
        else if (isObject)
        {
            record.reasonType = PromiseRejectionRecord::ReasonType::ReasonTypeObject;
            record.reasonString = "";
            JSVM_API_CALL(status, env, OH_JSVM_CreateReference(env, reason, 1, &record.reason));
        }
        else
        {
            record.reasonType = PromiseRejectionRecord::ReasonType::ReasonTypeString;
            record.reasonString = "";
        }
#endif
        m_pendingUnhandledRejections.emplace_back(std::move(record));
        SchedulePromiseRejectionProcessing();
    }

    // Promise rejection pipeline - unified methods (no engine-specific code)

    void JCScriptRuntime::SchedulePromiseRejectionProcessing()
    {
        if (m_isProcessingScheduled)
            return;
        m_isProcessingScheduled = true;
        m_pScriptThread->post([this]() {
            this->ProcessPromiseRejectionEvents();
        });
    }

    void JCScriptRuntime::ProcessPromiseRejectionEvents()
    {
        m_isProcessingScheduled = false;

        PromiseRejectionList pending;
        pending.swap(m_pendingUnhandledRejections);

        for (auto& record : pending)
        {
            record.warned = true;
            EmitPromiseRejectionEvent("unhandledrejection", record);
            m_maybeUnhandledRejections.emplace_back(std::move(record));
        }

        if (!m_asyncHandledRejections.empty())
        {
            std::vector<uint64_t> handled;
            handled.swap(m_asyncHandledRejections);

            for (const auto uid : handled)
            {
                auto it = std::find_if(
                    m_maybeUnhandledRejections.begin(),
                    m_maybeUnhandledRejections.end(),
                    [uid](const PromiseRejectionRecord& entry) { return entry.uid == uid; });
                if (it != m_maybeUnhandledRejections.end())
                {
                    EmitPromiseRejectionEvent("rejectionhandled", *it);
                    ReleaseRecord(*it);
                    m_maybeUnhandledRejections.erase(it);
                }
            }
        }
    }

    void JCScriptRuntime::EnqueueRejectionHandled(JSValueAsParam promise)
    {
        if (RemovePromiseRecord(m_pendingUnhandledRejections, promise))
            return;

        auto it = FindPromiseRecord(m_maybeUnhandledRejections, promise);
        if (it != m_maybeUnhandledRejections.end())
        {
            if (std::find(m_asyncHandledRejections.begin(),
                          m_asyncHandledRejections.end(),
                          it->uid) == m_asyncHandledRejections.end())
            {
                m_asyncHandledRejections.push_back(it->uid);
                SchedulePromiseRejectionProcessing();
            }
        }
    }

    void JCScriptRuntime::ClearPromiseRejectionTracking()
    {
        for (auto& record : m_pendingUnhandledRejections)
            ReleaseRecord(record);
        m_pendingUnhandledRejections.clear();

        for (auto& record : m_maybeUnhandledRejections)
            ReleaseRecord(record);
        m_maybeUnhandledRejections.clear();

        m_asyncHandledRejections.clear();
        m_lastPromiseRejectionId = 0;
        m_isProcessingScheduled = false;
    }

    void JCScriptRuntime::HandlePromiseRejectionEvent(JSValueAsParam promise, JSValueAsParam reason, const char* type)
    {
        if (type == nullptr)
            return;

        if (std::strcmp(type, "unhandledrejection") == 0)
        {
            EnqueueUnhandledRejection(promise, reason);
        }
        else if (std::strcmp(type, "rejectionhandled") == 0)
        {
            EnqueueRejectionHandled(promise);
        }
    }
#endif
    void JCScriptRuntime::start(const char* pStartJS) 
    {
        LOGI("Start js %s", pStartJS);
        if (pStartJS)m_strStartJS = pStartJS;
#if defined(JS_V8) || defined(JS_JSVM)
        m_pScriptThread->initialize(JCConch::s_pConch->m_nJSDebugPort,
            [](JSValueAsParam promise, JSValueAsParam reason, const char* type) {
                if (JCScriptRuntime::s_JSRT) {
                    JCScriptRuntime::s_JSRT->HandlePromiseRejectionEvent(promise, reason, type);
                }
            });
#else
        m_pScriptThread->initialize(JCConch::s_pConch->m_nJSDebugPort);
#endif
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
#elif JS_JSVM
        JSObjNode::s_pListJSObj = new JCSimpList();
//#ifdef JS_V8_DEBUGGER
        auto env = ENV;
        if (JCConch::s_pConch->m_nJSDebugMode != JS_DEBUG_MODE_OFF)
        {
            OH_JSVM_OpenInspector(env, "localhost", JCConch::s_pConch->m_nJSDebugPort);
            if (JCConch::s_pConch->m_nJSDebugMode == JS_DEBUG_MODE_WAIT) {
                OH_JSVM_WaitForDebugger(env, true);
            }
        }
//#endif
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
//#ifndef WIN32
        //JSP_RUN_SCRIPT((const char*)"function getExePath(){return null;}");
//#endif
        {
            char* sJSRuntime = NULL;
            int nSize = 0;
            if (m_pAssetsRes->loadFileContent("scripts/runtimeInit.js", sJSRuntime, nSize))
            {
//#ifdef JS_V8
//                JSP_RUN_SCRIPT(sJSRuntime);
//#elif JS_JSVM
                JSP_RUN_SCRIPT(sJSRuntime, "scripts/runtimeInit.js");
//#endif
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
                JSP_RUN_SCRIPT(kBuf.c_str(), NULL);
                if (try_catch.HasCaught())
                {
                    __JSRun::ReportException(isolate, &try_catch);
                }
            #else
//#ifdef JS_V8
//                JSP_RUN_SCRIPT(kBuf.c_str());
//#elif JS_JSVM
                JSP_RUN_SCRIPT(kBuf.c_str(),nullptr);
//#endif
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
//#ifdef JS_V8
//                JSP_RUN_SCRIPT("gc();gc();gc();");
//#elif JS_JSVM
                JSP_RUN_SCRIPT("gc();gc();gc();",nullptr);
//#endif
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
#if defined(JS_V8) || defined(JS_JSVM)
        ClearPromiseRejectionTracking();
#ifdef JS_JSVM
        {
            auto env = Javascript::getEnv();
            JSVM_Status status;
            for (auto& ref : m_vJSOnUnhandledRejectionFunctions)
            {
                if (ref) { JSVM_API_CALL(status, env, OH_JSVM_DeleteReference(env, ref)); }
            }
        }
#endif
        m_vJSOnUnhandledRejectionFunctions.clear();
#endif
#ifdef OHOS
        m_pGameJsOnMessage.Reset();
#endif
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
#elif JS_JSVM
        JSObjBaseJSVM::restarting = true;
        JSObjBaseJSVM::resetBaseSet();
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
//#ifdef JS_V8_DEBUGGER
        auto env = ENV;
        if (JCConch::s_pConch->m_nJSDebugMode != JS_DEBUG_MODE_OFF)
        {
            OH_JSVM_CloseInspector(env);
        }
//#endif
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
    void JCScriptRuntime::onUpdate()
    {
        PERF_INITVAR(nBenginTime);
#ifdef JS_V8
        m_pScriptThread->runDbgFuncs();
#elif JS_JSVM
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
        return;
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
//#ifdef JS_V8
//        JSP_RUN_SCRIPT("gc()");
//#elif JS_JSVM
        JSP_RUN_SCRIPT("gc()",nullptr);
//#endif
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
//#ifdef JS_V8
//        JSP_RUN_SCRIPT(sBuffer.c_str());
//#elif JS_JSVM
        JSP_RUN_SCRIPT(sBuffer.c_str(),nullptr);
//#endif
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
//#ifdef JS_V8
//        JSP_RUN_SCRIPT( sBuffer.c_str() );
//#elif JS_JSVM
        JSP_RUN_SCRIPT( sBuffer.c_str(),nullptr);
//#endif
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
//#ifdef JS_V8
//        JSP_RUN_SCRIPT("reloadJS(true)");
//#elif JS_JSVM
        JSP_RUN_SCRIPT("reloadJS(true)",nullptr);
//#endif
    }
    void JCScriptRuntime::jsUrlback()
    {
        std::function<void(void)> pFunction = std::bind(&JCScriptRuntime::jsUrlbackJSFunction, this);
        m_pScriptThread->post(pFunction);
    }
    void JCScriptRuntime::jsUrlbackJSFunction()
    {
//#ifdef JS_V8
//        JSP_RUN_SCRIPT("history.back()");
//#elif JS_JSVM
        JSP_RUN_SCRIPT("history.back()",nullptr);
//#endif
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

#if OHOS
    void JCScriptRuntime::onJsObjHandle(std::string key, std::string value)
    {
        std::function<void(void)> pFunction = std::bind(&JCScriptRuntime::onJsObjHandleCallJSFunction, this, key, value);
        m_pScriptThread->post(pFunction);
    }
    void JCScriptRuntime::onJsObjHandleCallJSFunction(std::string key, std::string value)
    {
        m_pGameJsOnMessage.Call(key,value);
    }
#endif

}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
