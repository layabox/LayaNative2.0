/**
@file			JCLayaThreadPool.cpp
@brief			
@author			hugao
@version		1.0
@date			2016_5_12
*/

#include "JCLayaThreadPool.h"
#include "../util/Log.h"
#ifdef WIN32
#include <windows.h>
#endif

namespace laya{

#ifdef __APPLE__
    pthread_key_t s_tls_curDataThread;
#else
    thread_local long*	s_tls_curDataThread = nullptr;
#endif
	void ___datathread_onthreadstart(const char* threadname) {
#ifdef WIN32
	{
		DWORD thid = GetCurrentThreadId();
		SetNameInternal(thid, threadname);
        LOGI("start thread:%s,%d", threadname, thid);
		//threadInfoLog("start thread:%s,%d", threadname, thid);
	}
#elif ANDROID
	{
		//threadInfoLog("start thread:%s,%d", threadname, gettidv1());
	}
#elif __APPLE__
        pthread_setname_np(threadname);
#endif
	}
	void ___datathread_onthreadstop(const char* threadname) {
#ifdef WIN32
	{
		DWORD thid = GetCurrentThreadId();
		LOGI("thread end:%s,%d", threadname, thid);
		//threadInfoLog("thread end:%s,%d", threadname, thid);
	}
#elif ANDROID
	{
		LOGI("thread end:%s,%d", threadname, gettidv1());
		//threadInfoLog("thread end:%s,%ld", threadname, gettidv1());
	}
#endif
	}

}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
