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
#include "IsolateData.h"
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
	Javascript::Javascript() 
    {
		m_pIsolate = NULL;
		m_pPlatform = NULL;
        m_nListenPort = 0;


		m_pPlatform = v8::platform::NewDefaultPlatform().release();
		v8::V8::InitializePlatform(m_pPlatform);
		v8::V8::Initialize();
		static char* flags[] =
		{
#if __APPLE__
            " --jitless",
#endif
			"--expose_gc",
			"--no-flush-bytecode",
			"--no-lazy",
		};
		for (auto f : flags)
		{
			v8::V8::SetFlagsFromString(f, strlen(f));
		}
		
	}
	Javascript::~Javascript()
	{

		v8::V8::Dispose();
		v8::V8::ShutdownPlatform();
		delete m_pPlatform;
	}
	void Javascript::init(int nPort) 
    {
		m_nListenPort = 0;
		if (nPort > 0 && nPort <0xFFFF)
		{
			m_nListenPort = nPort;
		}
	}
    void Javascript::uninit() 
    {
       
    }
    void Javascript::initJSEngine()
    {
		v8::Isolate::CreateParams create_params;
		create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
		m_pIsolate = v8::Isolate::New(create_params);
		m_pIsolate->Enter();
		v8::HandleScope handle_scope(m_pIsolate);
		v8::Local<v8::Context> context = v8::Context::New(m_pIsolate);
		m_context.Reset(m_pIsolate, context);
		m_IsolateData = new IsolateData(m_pIsolate, NULL);
		context->Enter();
    }
    void Javascript::uninitJSEngine()
    {
		{
			v8::HandleScope handle_scope(m_pIsolate);
			v8::Local<v8::Context> context = m_context.Get(m_pIsolate);
			context->Exit();
			m_context.Reset();
			delete m_IsolateData;
			m_pIsolate->Exit();
			
		}
		m_pIsolate->Dispose();

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
            v8::String::Utf8Value exceptioninfo(m_pIsolate, try_catch.Exception());
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
			v8::HandleScope handle_scope(v8::Isolate::GetCurrent());
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
				if (piso)__JSRun::ReportException(piso, &trycatch);
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