/**
@file			JCWorkerThread.h
@brief			
@author			hugao
@version		1.0
@date			2016_5_12
*/

#ifndef __JCWorkerThread_H__
#define __JCWorkerThread_H__

#include <thread>
#include <vector>
//#include <threadpool.h>
#include "../event/JCEmitter.h"
#include "JCLayaThreadPool.h"

#ifdef __APPLE__
#include <pthread.h>
#endif

namespace laya
{
    /** 
     * @brief 工作线程： 可以工作在两种模式下，
     *       1.没有任务就阻塞的模式
     *           如果没有调用 setLoopFunc 就是在这种模式下
     *       2.使劲循环的模式
     *           只要调用setLoopFunc 就是在这种模式下
     *       post 会把函数发送的worker线程执行
    */
	class JCWorkerThread:public JCEventEmitter
    {
	public:
		enum workerThreadEvt{
			Event_threadStart=0,	//线程开始了，产生地为工作线程
			Event_threadStop=1,		//线程结束了，产生地为工作线程
			Event_Num,
		};

		//返回false的时候，表示希望退出循环。
		typedef std::function<bool(void)> loopRun;
		typedef std::function<void(void)> runObj;

        JCWorkerThread(bool p_bStart=false);
		virtual ~JCWorkerThread();

		//在对象析构的时候是否要等待线程退出
		void setStopOnDestroy(bool b){m_bStopOnDestroy = b;};

		//开始线程。开始执行线程循环。注意在这之后不能再改变工作模式
		virtual void start();

		//停止工作线程。
		virtual void stop();
		bool tryStop(int timeout, JCDataThread<runObj>::tryStopWorker func, void* userdata);

		//会把func发送到worker线程执行。如果没有调用 setLoopFunc 则会激活worker线程
		virtual void post( runObj func );

		//调用这个之后，线程就不会再等待post的事件。
		void setLoopFunc( loopRun loopFunc );

		static JCWorkerThread* getCurThread();

		void setUserData(void* pData){ 
			m_pUserData = pData;
		}

		void* getUserData(){
			return m_pUserData;
		}

		void setThreadName(const char* p_pszName);

        bool isStop();

    public:

		void runQueue();

		void _threadProc();

		virtual void _defRunLoop();

		virtual void _runLoop();	//需要继承的。例如js线程，需要在_defRunLoop外面包点运行环境

    public:

        loopRun						m_funcLoop;

    public:

		std::vector< runObj >		m_FuncQueue;	//需要在此线程执行的函数的队列
		std::mutex				m_QueueLock;
		//JCThreadPoolShare<runObj>    m_ThreadTasks;
		JCDataThread<runObj>			m_ThreadTasks;
		bool						m_bdbgThreadStarted;
#ifdef __APPLE__
        static pthread_key_t s_tls_curThread;
#else
		static thread_local long*	s_tls_curThread;
#endif
		void*						m_pUserData;
		std::string					m_strName;
		bool						m_bStopOnDestroy;
		bool			            m_bStop;
        
	};
};
//------------------------------------------------------------------------------


#endif //__JCWorkerThread_H__

//-----------------------------END FILE--------------------------------
