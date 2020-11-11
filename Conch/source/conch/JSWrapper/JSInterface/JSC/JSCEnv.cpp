/**
@file			JSCEnv.cpp
@brief			
@author			James
@version		1.0
@date			2018_8_23
*/

#include "JSCEnv.h"
#include "../../../CToObjectC.h"
#ifdef __APPLE__
#include <pthread.h>
#endif
namespace laya
{
    pthread_key_t __TlsData::s_tls_curThread;
	static JSValueRef gcCallback( JSContextRef ctx, JSObjectRef functionObject, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception )
    {
        JSGlobalContextRef pCtx = JSContextGetGlobalContext(ctx);
        JSGarbageCollect( pCtx );
        //JSSynchronousEdenCollectForDebugging(pCtx);
        //JSSynchronousGarbageCollectForDebugging(pCtx);
        return JSValueMakeUndefined(ctx);
    }
    void AlertOnJsException(bool b)
    {
        
    }
    void Javascript::initJSEngine()
    {
        m_pGlobalContext = JSGlobalContextCreateInGroup(NULL, NULL);
        m_pUndefined = JSValueMakeUndefined(m_pGlobalContext);
        JSValueProtect(m_pGlobalContext, m_pUndefined);
        __TlsData::GetInstance()->SetCurContext(m_pGlobalContext);

        // expose gc()
        JSStringRef pJSName = JSStringCreateWithUTF8CString("gc");
        JSObjectRef pFunc = JSObjectMakeFunctionWithCallback(m_pGlobalContext, pJSName, gcCallback);
        JSValueRef exception = 0;
        JSObjectSetProperty(m_pGlobalContext, JSContextGetGlobalObject(m_pGlobalContext), pJSName, pFunc, kJSPropertyAttributeDontDelete, &exception);
        if (0 != exception)
        {
            __JSRun::OutputException(exception);
        }
        JSStringRelease(pJSName);

    }
    void Javascript::uninitJSEngine()
    {
        __TlsData::GetInstance()->SetCurContext(NULL);
        JSValueUnprotect(m_pGlobalContext, m_pUndefined);
        JSGlobalContextRelease(m_pGlobalContext);
    }
    //------------------------------------------------------------------------------
    JSCWorker::JSCWorker()
    {

    }
    JSCWorker::~JSCWorker()
    {

    }
    void JSCWorker::stop()
    {
        if (m_bStop)return;
        m_bStop = true;
        m_ThreadTasks.Stop();
        m_bdbgThreadStarted = false;
    }
    void JSCWorker::_defRunLoop()
    {
        //设置tls
        pthread_setspecific(s_tls_curThread, (void*)this);

        //开始事件
        JCEventEmitter::evtPtr startEvt(new JCEventBase);
        startEvt->m_nID = JCWorkerThread::Event_threadStart;
        emit(startEvt);

        CToObjectCRunJSLoop();
        
        //退出事件
        JCEventEmitter::evtPtr stopEvt(new JCEventBase);
        stopEvt->m_nID = JCWorkerThread::Event_threadStop;
        emit(stopEvt);
    }
	void JSCWorker::_runLoop()
	{
        m_pJS->initJSEngine();
		_defRunLoop();
        m_pJS->uninitJSEngine();	
	}
};
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
