/**
@file			JCWorkerThread.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#include "JCWorkerThread.h"
//#include "../misc/conchDebugThread.h"
#include "../util/JCCommonMethod.h"
#include "../util/Log.h"
#ifdef WIN32
#include <windows.h>
#include <process.h>
//2013.3.5
//cswuyg
//参考 chromium源码: src\base\threading\platform_thread_win.cc

// The information on how to set the thread name comes from
// a MSDN article: http://msdn2.microsoft.com/en-us/library/xcb2z8hs.aspx
const DWORD kVCThreadNameException = 0x406D1388;

typedef struct tagTHREADNAME_INFO {
	DWORD dwType;  // Must be 0x1000.
	LPCSTR szName;  // Pointer to name (in user addr space).
	DWORD dwThreadID;  // Thread ID (-1=caller thread).
	DWORD dwFlags;  // Reserved for future use, must be zero.
} THREADNAME_INFO;

// This function has try handling, so it is separated out of its caller.
void SetNameInternal(unsigned int thread_id, const char* name)
{
	//只在调试的时候生效 
	if (!::IsDebuggerPresent()) 
		return;
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = name;
	info.dwThreadID = thread_id;
	info.dwFlags = 0;

	__try
	{
		RaiseException(kVCThreadNameException, 0, sizeof(info)/sizeof(DWORD), reinterpret_cast<DWORD_PTR*>(&info));
	}
	__except(EXCEPTION_CONTINUE_EXECUTION) 
	{
	}
}
#elif ANDROID
#include <sys/syscall.h>  
#define gettidv1() syscall(__NR_gettid)  
#define gettidv2() syscall(SYS_gettid)  
#endif

namespace laya{
#ifdef __APPLE__
    pthread_key_t	JCWorkerThread::s_tls_curThread;
#else
    thread_local long*	JCWorkerThread::s_tls_curThread = nullptr;	//这个虽然是静态的。但每个线程之间不会干扰
#endif

    JCWorkerThread* JCWorkerThread::getCurThread(){
#ifdef __APPLE__
        return (JCWorkerThread*)pthread_getspecific(s_tls_curThread);
#else
		return (JCWorkerThread*)s_tls_curThread;
#endif
	}

    JCWorkerThread::JCWorkerThread(bool p_bStart){
        m_bStop = false;
		m_bStopOnDestroy = true;
		m_AllListener.resize(JCWorkerThread::Event_Num);
		m_bdbgThreadStarted=false;
		m_pUserData=0;
		if(p_bStart)
			start();
	}

    JCWorkerThread::~JCWorkerThread(){
		if(m_bStopOnDestroy)
			stop();
	}

	void JCWorkerThread::setLoopFunc( loopRun loopFunc ){
		if(m_bdbgThreadStarted){
			printf("After the start of the thread,you can't set loopFunc again,Mainly afraid to call the first 'post',than call 'setLoopFunc' Cause tasks to spread between m_ThreadTasks and m_FuncQueue\n");
			throw -1;
		}
		m_funcLoop = loopFunc;
	}

	void	JCWorkerThread::start(){
		m_bStop = false;
		//m_pThread = new std::thread( workFunc );
		m_ThreadTasks.Start(std::bind(&JCWorkerThread::_threadProc,this));
		//m_ThreadTasks.CreateThread( std::bind(&workerThread::_threadProc,this));
		m_bdbgThreadStarted=true;
	}

	void JCWorkerThread::stop(){
		if(m_bStop)
			return;
		m_bStop = true;
		m_ThreadTasks.Stop();
		m_bdbgThreadStarted=false;
	}

	bool JCWorkerThread::tryStop(int timeout, JCDataThread<runObj>::tryStopWorker func, void* userdata) {
		if (m_bStop)
			return true;
		m_bStop = true;
		bool bret = m_ThreadTasks.tryStop(timeout, func, userdata);
		m_bdbgThreadStarted = false;
		return bret;
	}
	void JCWorkerThread::post(runObj func){
		if (m_bStop)
			return;
		if( !m_funcLoop){
			m_ThreadTasks.SendData(func);
		}else{
			m_QueueLock.lock();
			m_FuncQueue.push_back(func);
			m_QueueLock.unlock();
		}
	}

	void JCWorkerThread::runQueue(){
		std::vector<runObj> workQueue;
		m_QueueLock.lock();
		std::swap(workQueue, m_FuncQueue);
		m_QueueLock.unlock();
		for( int i=0; i<(int)workQueue.size(); i++){
			workQueue[i]();
		}
	}


	void JCWorkerThread::_runLoop(){
		_defRunLoop();
	}

	void JCWorkerThread::_defRunLoop(){
		//设置名字
#ifdef WIN32
		{
			DWORD thid = GetCurrentThreadId();
			SetNameInternal(thid, m_strName.c_str());
            LOGI("start thread:%s,%d", m_strName.c_str(), thid);
            //threadInfoLog("start thread:%s,%d", m_strName.c_str(), thid);
		}
#elif ANDROID
		{
            LOGI("start thread:%s,%d", m_strName.c_str(), (int)gettidv1());
			//threadInfoLog("start thread:%s,%ld", m_strName.c_str(), gettidv1());
		}
#endif
		//开始事件
		JCEventEmitter::evtPtr startEvt(new JCEventBase);
		startEvt->m_nID=JCWorkerThread::Event_threadStart;
		emit(startEvt);

		runObj task;
		while(!m_bStop){
			if( !m_funcLoop ){
				//现在的waitdata返回false不再表示要退出。
				//事件唤醒流程
				//if( !m_ThreadTasks.WaitData(&task) )
				//	break;
				if( m_ThreadTasks.WaitData(&task))
					task();
			}else{
				//固定循环流程
				runQueue();
				if(!m_funcLoop()){
					break;	//
				}
			}
		}	

		//退出事件
		JCEventEmitter::evtPtr stopEvt(new JCEventBase);
		stopEvt->m_nID=JCWorkerThread::Event_threadStop;
		emit(stopEvt);
	}

	void JCWorkerThread::_threadProc(){
        //设置tls
#ifdef __APPLE__
        pthread_setspecific(s_tls_curThread,(void*)this);
#else
        s_tls_curThread = (long*)this;
#endif
		//去掉try了，因为可能导致流程混乱，卡死
		_runLoop();
	}

	void JCWorkerThread::setThreadName(const char* p_pszName){
		m_strName = p_pszName;
	}
    bool JCWorkerThread::isStop()
    {
        return m_bStop;
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
