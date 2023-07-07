/**
@file			JSCEnv.h
@brief			
@author			James
@version		1.0
@date			2018_8_23
*/

#ifndef __JSCEnv_H__
#define __JSCEnv_H__

#include <JavaScriptCore/JavaScriptCore.h>
#include "JSCProxyTLS.h"
#include <misc/JCWorkerThread.h>

namespace laya
{
	void JSPrint( const char* p_sBuffer );
	void JSAlert( const char* p_sBuffer );
	void JSAlertExit( const char* p_sBuffer );
	void evalJS( const char* p_sSource );

    class Javascript
    {
    public:

        typedef void(*voidfun)(void* data);
        Javascript()
        {

        }
        void init(int nPort)
        {

        }
        void run(const char* sSource)
        {

        }
        void run(const char* sSource, std::function<void(void)> preRunFunc)
        {

        }
        void run(voidfun func, void* pdata)
        {

        }
        void uninit()
        {

        }
        
        void initJSEngine();

        void uninitJSEngine();

    public:

        JSGlobalContextRef  m_pGlobalContext;
        JSValueRef          m_pUndefined;
    };

    class JSThreadInterface
    {
    public:
        JSThreadInterface()
        {

        }
        virtual ~JSThreadInterface()
        {

        }
        virtual void post(std::function<void(void)> func) = 0;
        virtual void on(int nEvent, JCEventEmitter::EventHandler func, void* pInThread = 0) = 0;
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual void initialize(int nPort) = 0;
        virtual void uninitialize() = 0;
        virtual void setLoopFunc(std::function<void(void)> func) = 0;
        virtual void pushDbgFunc(std::function<void(void)> task) = 0;
        virtual void runDbgFuncs() = 0;
        virtual void waitAndRunDbgFuncs() = 0;
        virtual bool hasDbgFuncs() = 0;
        virtual JCWorkerThread* getWorker() = 0;
        virtual void run(Javascript::voidfun func, void* pData) = 0;
		virtual void clearFunc() = 0;
    };

    class JSCWorker : public JCWorkerThread
    {
    public:

        JSCWorker();

        ~JSCWorker();

        virtual void _defRunLoop();

        virtual void _runLoop();

        virtual void stop();

    public:

        Javascript* m_pJS;

    };

	class JSMulThread : public JSThreadInterface
	{
	public:
        JSMulThread()
        {
            m_kWorker.setThreadName("JavaScript Main");
            m_kWorker.m_pJS = &m_kJS;
        }
        ~JSMulThread()
        {

        }
        void post(std::function<void(void)> func)
        {
            if (m_kWorker.m_bStop)return;
            CToObjectCPostFunc(func);
        }
        void on(int nEvent, JCEventEmitter::EventHandler func, void* pInThread = 0)
        {
            m_kWorker.on(nEvent, func, (JCWorkerThread*)pInThread);
        }
        void start()
        {
            m_kWorker.start();
        }
        void stop()
        {
            m_kWorker.stop();
        }
        void initialize(int nPort)
        {
            m_kJS.init(nPort);
        }
        void uninitialize()
        {
            m_kJS.uninit();
        }
        void setLoopFunc(std::function<void(void)> func)
        {
            m_kWorker.setLoopFunc(func);
        }
        void pushDbgFunc(std::function<void(void)> task)
        {
        }
        void runDbgFuncs()
        {
        }
        void waitAndRunDbgFuncs()
        {
        }
        bool hasDbgFuncs()
        {
            return false;
        }
        JCWorkerThread* getWorker()
        {
            return &m_kWorker;
        }
        void run(Javascript::voidfun func, void* pData)
        {
            m_kWorker.runQueue();
            m_kWorker.m_funcLoop();
        }
		void clearFunc()
		{
		}
	public:

        Javascript          m_kJS;
        JSCWorker           m_kWorker;
	};

    class JSSingleThread : public JSThreadInterface
    {
    public:
        JSSingleThread()
        {
        }
        ~JSSingleThread()
        {

        }
        void post(std::function<void(void)> func)
        {
            //if (m_kWorker.m_bStop)return;
            //CToObjectCPostFunc(func);
            m_kQueueLock.lock();
            m_vFuncQueue.push_back(func);
            m_kQueueLock.unlock();
        }
        void on(int nEvent, JCEventEmitter::EventHandler func, void* pInThread = 0)
        {
            switch (nEvent)
            {
            case JCWorkerThread::Event_threadStart:
                m_kStartFunc.func = func;
                m_kStartFunc.bDel = false;
                break;
            case JCWorkerThread::Event_threadStop:
                m_kStopFunc.func = func;
                m_kStopFunc.bDel = false;
                break;
            default:
                LOGE("JSSingleThread on() event type error");
                break;
            }
        }
        void start()
        {
            m_kStartFunc.func(NULL);
        }
        void stop()
        {
            m_kStopFunc.func(NULL);
        }
        void initialize(int nPort)
        {
			clearFunc();
            m_kJS.init(nPort);
            m_kJS.initJSEngine();
        }
        void uninitialize()
        {
			clearFunc();
            m_kJS.uninitJSEngine();
            m_kJS.uninit();
        }
        void setLoopFunc(std::function<void(void)> func)
        {
            m_kLoopFunc = func;
        }
        void pushDbgFunc(std::function<void(void)> task)
        {
        }
        void runDbgFuncs()
        {
        }
        void waitAndRunDbgFuncs()
        {
        }
        bool hasDbgFuncs()
        {
            return false;
        }
        JCWorkerThread* getWorker()
        {
            return NULL;
        }
        void run(Javascript::voidfun func, void* pData)
        {
            runFunQueue();
            m_kLoopFunc();
        }
		void clearFunc()
		{
			m_kQueueLock.lock();
            m_vFuncQueue.clear();
            m_kQueueLock.unlock();
		}
    public:

        void runFunQueue()
        {
            std::vector<std::function<void(void)>> vWorkQueue;
            m_kQueueLock.lock();
            std::swap(vWorkQueue, m_vFuncQueue);
            m_kQueueLock.unlock();
            for (int i = 0; i < (int)vWorkQueue.size(); i++)
            {
                vWorkQueue[i]();
            }
        }

    public:

        Javascript                                  m_kJS;
        JCEventEmitter::EvtHandlerPack              m_kStartFunc;
        JCEventEmitter::EvtHandlerPack              m_kStopFunc;
        std::vector<std::function<void(void)>>      m_vFuncQueue;
        std::mutex				                    m_kQueueLock;
        std::function<void(void)>                   m_kLoopFunc;

    };
		
};

//------------------------------------------------------------------------------


#endif //__JSCEnv_H__

//-----------------------------END FILE--------------------------------
