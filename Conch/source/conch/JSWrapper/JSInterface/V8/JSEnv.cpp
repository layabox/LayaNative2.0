/**
@file			JSEnv.cpp
@brief			
@author			James
@version		1.0
@date			2018_8_23
*/
#ifdef JS_V8
#include "JSEnv.h"
#include <v8-profiler.h>
#include "../../v8debug/debug-agent.h"
#include "util/Log.h"
#include "JSCProxyTLS.h"
#ifdef WIN32
    #include <windows.h>
    #include <process.h>
#endif
//#define V8PROFILE

namespace laya
{
	v8::Persistent<v8::Context> Javascript::m_DebugMessageContext;
	const char* ToCString(const v8::String::Utf8Value& value)
    {
		return *value ? *value : "<string conversion failed>";
	}
    class LayaHandleScope :public v8::HandleScope
    {
    public:
        LayaHandleScope(v8::Isolate* piso)
        {
            HandleScope::Initialize(piso);
        }
        ~LayaHandleScope()
        {
        }
    public:
        void Initialize(v8::Isolate* isolate)
        {
            HandleScope::Initialize(isolate);
        }
    };
	Javascript::Javascript() 
    {
		m_pIsolate = NULL;
		m_pPlatform = NULL;
        m_pScope = NULL;
        m_pHandleScope = NULL;
        m_pV8Locker = NULL;
        m_pABAlloc = NULL;
        m_nListenPort = 0;
	}
	void Javascript::init(int nPort) 
    {
        if (!m_pPlatform) 
        {
            m_pPlatform = v8::platform::CreateDefaultPlatform();
            v8::V8::InitializePlatform(m_pPlatform);
        }
		v8::V8::Initialize();
        m_nListenPort = 0;
		if(nPort > 0 && nPort <0xFFFF )
        {
			m_nListenPort = nPort;
		}
	}
    void Javascript::uninit() 
    {
        //不能在v8线程
        return;
        if (0 != m_pIsolate) 
        {
            m_pIsolate->Dispose();
            m_pIsolate = 0;
            ArrayBufferAllocator* pABAlloc = ArrayBufferAllocator::getInstance();
            //pABAlloc->FreeAllAlive();
            delete pABAlloc;
        }
    }
    void Javascript::initJSEngine()
    {
        v8::Isolate::CreateParams create_params;
        m_pABAlloc = ArrayBufferAllocator::getInstance();
        create_params.array_buffer_allocator = m_pABAlloc;
        m_pIsolate = v8::Isolate::New(create_params);
        static char* flags[] =
        {
            "--expose_gc",
            "--allow-natives-syntax",//导出内部的%函数
                                     //"--trace-gc-verbose",
                                     //"--use-strict",
                                     //"--harmony",
                                     //"--print-bytecode", //
                                     //"--print-bytecode-filter errf1"
                                     //"--trace-ignition-codegen",
                                     //"--debug-code",
                                     //"--trace-opt", 
                                     //"--trace-deopt",
                                     //"--prof",
                                     //"--code-comments", 
        };
        for (auto f : flags)
        {
            v8::V8::SetFlagsFromString(f, strlen(f));
        }
#ifdef V8PROFILE
        v8::V8::SetFlagsFromString("--prof", 6);
        char* pSingleLogFile = "--no-logfile_per_isolate";
        v8::V8::SetFlagsFromString(pSingleLogFile, strlen(pSingleLogFile));
#ifdef ANDROID
        char* profFile = "--logfile /sdcard/lr2perf/v8.log";
#elif WIN32
        char* profFile = "--logfile d:/temp/v8.log";
#endif
        v8::V8::SetFlagsFromString(profFile, strlen(profFile));
#endif
        //m_pLocker = new v8::Locker(m_pIsolate);

        //这个{}是为了能让 Scope起作用
        m_pScope = new v8::Isolate::Scope(m_pIsolate);
        m_pHandleScope = (v8::HandleScope*)malloc(sizeof(v8::HandleScope));
        new (m_pHandleScope) LayaHandleScope(m_pIsolate);
        v8::Handle<v8::ObjectTemplate> pGlobalTemplate = v8::ObjectTemplate::New(m_pIsolate);
        v8::Local<v8::Context> context = v8::Context::New(m_pIsolate, NULL, pGlobalTemplate, v8::Handle<v8::Value>());
        context->Enter();
        //v8::CpuProfiler* pCpuProf =  m_pIsolate->GetCpuProfiler();
        //pCpuProf->StartCpuProfiling(v8::String::New(""));
        //pCpuProf->StopCpuProfiling(v8::String::New(""));
        if (m_nListenPort > 0)
        {
            {
                std::unique_lock<std::recursive_mutex> __guard(m_Lock);
                if (m_DebugMessageContext.IsEmpty())
                {
                    m_DebugMessageContext.Reset(m_pIsolate, v8::Local<v8::Context>::New(m_pIsolate, context));
                }
            }
        }
    }
    void Javascript::uninitJSEngine()
    {
        //当js线程要结束的时候，关闭调试，否则调试器客户端在线程结束后再发送调试事件，会导致非法。
        if (m_nListenPort > 0)
        {
            //v8::Debug::DisableAgent();
        }
        if (!m_DebugMessageContext.IsEmpty())
        {
            m_DebugMessageContext.Reset();
        }
        //应该调用退出，但是context在scope中，先不退出了，发现也没有内存泄漏
        //context->Enter();
        delete m_pHandleScope;
        m_pHandleScope = NULL;
        delete m_pScope;
        m_pScope = NULL;
        m_pIsolate->Dispose();
        delete m_pABAlloc;
    }
	void Javascript::run(const char* sSource, std::function<void(void)> preRunFunc)
    {
	}
	void Javascript::run(const char* sSource)
    {
	}
	void Javascript::run(voidfun func, void* pdata) 
    {
        v8::TryCatch try_catch(m_pIsolate);
        func(pdata);
        if (try_catch.HasCaught())
        {
            v8::String::Utf8Value exceptioninfo(try_catch.Exception());
            printf("Exception info [%s]\n", *exceptioninfo);
        }
	}
    //------------------------------------------------------------------------------
    JSV8Worker::JSV8Worker()
    {

    }
    JSV8Worker::~JSV8Worker()
    {

    }
	void JSV8Worker::_defRunLoop()
    {
#ifdef WIN32
		{
			DWORD thid = GetCurrentThreadId();
			SetNameInternal(thid, m_strName.c_str());
			//threadInfoLog("start thread:%s,%d", m_strName.c_str(), thid);
		}
#elif ANDROID
		{
			//threadInfoLog("start thread:%s,%ld", m_strName.c_str(), gettidv1());
		}
#endif
		//开始事件
		JCEventEmitter::evtPtr startEvt(new JCEventBase);
		startEvt->m_nID = JCWorkerThread::Event_threadStart;
		emit(startEvt);
        JCWorkerThread::runObj task;
		while (!m_bStop)
        {
			v8::TryCatch trycatch(v8::Isolate::GetCurrent());
			if (!m_funcLoop)
            {
				//现在的waitdata返回false不再表示要退出。事件唤醒流程
				if (m_ThreadTasks.WaitData(&task))
					task();
			}
			else 
            {
				//固定循环流程
                runQueue();
				if (!m_funcLoop())
                {
					break;
				}
			}
			if (trycatch.HasCaught()) 
            {
				v8::Isolate* piso = v8::Isolate::GetCurrent();
				if( piso )__JSRun::ReportException(piso, &trycatch);
			}
		}
		//退出事件
		JCEventEmitter::evtPtr stopEvt(new JCEventBase);
		stopEvt->m_nID = JCWorkerThread::Event_threadStop;
        emit(stopEvt);
	}
	void call_JSThread__defRunLoop(void* pdata) 
    {
        JSV8Worker* pthis=(JSV8Worker*)pdata;
		pthis->_defRunLoop();
	}
	void JSV8Worker::_runLoop()
    {
        m_pJS->initJSEngine();
		m_pJS->run(call_JSThread__defRunLoop,this);
        m_pJS->uninitJSEngine();
	}
}
#endif
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------