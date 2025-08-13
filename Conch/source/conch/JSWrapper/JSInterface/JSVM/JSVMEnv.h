#ifndef __JSVMEnv_H__
#define __JSVMEnv_H__

#include <misc/JCWorkerThread.h>
#include "JSVMArrayBuffer.h"
#include <util/Log.h>
#include "ark_runtime/jsvm.h"

using HandleScope = JSVM_HandleScope;
using EscapableHandleScope = JSVM_EscapableHandleScope;

namespace laya
{
    class AutoHandleScope {
        public:
            AutoHandleScope();
            ~AutoHandleScope();
    
        private:
        HandleScope _handleScope;
    };
    class AutoEscHandleScope {
        public:
            AutoEscHandleScope();
            ~AutoEscHandleScope();
            EscapableHandleScope getScope() {
                return _escapableHandleScope;
            }
    
        private:
        EscapableHandleScope _escapableHandleScope;
    };

	void JSPrint( const char* p_sBuffer );
	void JSAlert( const char* p_sBuffer );
	void evalJS( const char* p_sSource );
	class IsolateData;
	class Javascript
    {
	public:
        typedef std::recursive_mutex _MutexType;

		typedef void(*voidfun)(void* data);

		Javascript();

		~Javascript();

		void init(int nPort, std::function<void(JSVM_Value, JSVM_Value, const char*)> func);

		//在init之后，并且在js线程的情况下，可以添加js导出
		//执行一段脚本。并立即退出。
		void run(const char* sSource );

		//preRunFunc 是在执行脚本之前先执行的函数，例如可以做导出对象
		void run(const char* sSource, std::function<void(void)> preRunFunc );

		//执行一个函数。并立即退出
		void run(voidfun func,void* pdata);

        //不能在v8线程，在js退出后，可以进行js对象的清理
        void uninit();

        void initJSEngine();

        void uninitJSEngine();
    
        static JSVM_Env getEnv();
        static JSVM_Value getJsGlobal();

	public:
		int                                 m_nListenPort;
		IsolateData*						m_IsolateData;
        std::function<void(JSVM_Value, JSVM_Value, const char*)>           m_promiseRejectHandler;
        JSVM_Env      _env      = nullptr;
        JSVM_VM       _vm       = nullptr;
        JSVM_VMScope  _vmScope  = nullptr;
        JSVM_EnvScope _envScope = nullptr;
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
        virtual void on(int nEvent, JCEventEmitter::EventHandler func, void* pInThread=0) = 0;
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual void initialize(int nPort, std::function<void(JSVM_Value, JSVM_Value, const char* func)>) = 0;
        virtual void uninitialize() = 0;
        virtual void setLoopFunc(std::function<void(void)> func) = 0;
        virtual void pushDbgFunc(std::function<void(void)> task) = 0;
        virtual void runDbgFuncs() = 0;
        virtual void waitAndRunDbgFuncs() = 0;
        virtual bool hasDbgFuncs() = 0;
        virtual JCWorkerThread* getWorker() = 0;
        virtual void run(Javascript::voidfun func,void* pData) = 0;
        virtual void clearFunc() = 0;
        virtual JSVM_Env getEnv();
    };

    class JSVMWorker : public JCWorkerThread
    {
    public:

        JSVMWorker();

        ~JSVMWorker();

        //因为要trycatch，所以只好再抄一份了
        virtual void _defRunLoop();

        virtual void _runLoop();

    public:

        Javascript* m_pJS;

    };

	//把Javascript对象放到一个单独的线程中执行。
	class JSMulThread: public JSThreadInterface
    {
	public:

        JSMulThread()
        {
            m_kWorker.setThreadName("JavaScript Main");
            m_kWorker.m_pJS = &m_kJS;
        }

        ~JSMulThread()
        {
            //uninitialize();
        }

        void post(std::function<void(void)> func)
        {
            m_kWorker.post(func);
        }
        void on(int nEvent, JCEventEmitter::EventHandler func, void* pInThread=0)
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
        void initialize(int nPort, std::function<void(JSVM_Value, JSVM_Value, const char*)> func)
        {
            m_kJS.init(nPort, func);
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
            m_DbgFuncLock.lock();
            m_DbgFunction.push_back(task);
            m_DbgFuncLock.unlock();
        }
        void runDbgFuncs()
        {
            m_DbgFuncLock.lock();
            for (std::function<void(void)>& task : m_DbgFunction)
            {
                task();
            }
            m_DbgFunction.clear();
            m_DbgFuncLock.unlock();
        }
        void waitAndRunDbgFuncs()
        {
            m_DbgFuncLock.lock();
            m_DbgFuncLock.unlock();
        }
        bool hasDbgFuncs()
        {
            bool bRet = false;
            m_DbgFuncLock.lock();
            bRet = m_DbgFunction.size() > 0;
            m_DbgFuncLock.unlock();
            return bRet;
        }
        JCWorkerThread* getWorker()
        {
            return &m_kWorker;
        }
        void run(Javascript::voidfun func, void* pData)
        {

        }
        void clearFunc()
        {

        }
    
        JSVM_Env getEnv(){
            return m_kJS._env;
        }

    protected:

        std::vector<std::function<void(void)>>	    m_DbgFunction;  //调试函数
        std::mutex				                    m_DbgFuncLock;
        JSVMWorker                                  m_kWorker;
		Javascript	                                m_kJS;

	};
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    //把Javascript对象放到一个单独的线程中执行。
    class JSSingleThread : public JSThreadInterface
    {
    public:

        JSSingleThread()
        {

        }
        ~JSSingleThread()
        {
            //uninitialize();
        }
        void post(std::function<void(void)> func)
        {
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
        void initialize(int nPort, std::function<void(JSVM_Value, JSVM_Value, const char*)> func)
        {
            clearFunc();
            m_kJS.init(nPort, func);
            m_kJS.initJSEngine();
        }
        void uninitialize()
        {
            clearFunc();
            m_kJS.uninit();
            m_kJS.uninitJSEngine();
        }
        void setLoopFunc(std::function<void(void)> func)
        {
            LOGE("JSSingleThread setLoopFunc error,You can't call this function on V8 Engine");
        }
        void pushDbgFunc(std::function<void(void)> task)
        {
            m_DbgFuncLock.lock();
            m_DbgFunction.push_back(task);
            m_DbgFuncLock.unlock();
        }
        void runDbgFuncs()
        {
            m_DbgFuncLock.lock();
            for (std::function<void(void)>& task : m_DbgFunction)
            {
                task();
            }
            m_DbgFunction.clear();
            m_DbgFuncLock.unlock();
        }
        void waitAndRunDbgFuncs()
        {
            m_DbgFuncLock.lock();
            m_DbgFuncLock.unlock();
        }
        bool hasDbgFuncs()
        {
            bool bRet = false;
            m_DbgFuncLock.lock();
            bRet = m_DbgFunction.size() > 0;
            m_DbgFuncLock.unlock();
            return bRet;
        }
        JCWorkerThread* getWorker()
        {
            return NULL;
        }
        void run(Javascript::voidfun func, void* pData)
        {
            runFunQueue();
            if (pData)func(pData);
        }
        void clearFunc()
        {
            m_kQueueLock.lock();
            m_vFuncQueue.clear();
            m_kQueueLock.unlock();
        }
    
        JSVM_Env getEnv() {
            return m_kJS._env;
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

    protected:

        std::vector<std::function<void(void)>>	    m_DbgFunction;  //调试函数
        std::mutex				                    m_DbgFuncLock;
        Javascript	                                m_kJS;
        JCEventEmitter::EvtHandlerPack              m_kStartFunc;
        JCEventEmitter::EvtHandlerPack              m_kStopFunc;
        std::vector<std::function<void(void)>>      m_vFuncQueue;	//需要在此线程执行的函数的队列
        std::mutex				                    m_kQueueLock;

    };
};
//------------------------------------------------------------------------------


#endif //__JSEnv_H__

//-----------------------------END FILE--------------------------------