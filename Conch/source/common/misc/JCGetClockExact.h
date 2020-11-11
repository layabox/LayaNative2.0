/**
@file			JCGetClockExact.h
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#ifndef __JCGetClockExact_H__
#define __JCGetClockExact_H__


#include "JCSingleton.h"
#include <ctime>
#include <time.h>
#include <thread>
#include <atomic>

class JCGetClockExact : public laya::JCSingleton<JCGetClockExact>
{

#ifdef WIN32
	void __WindowsTimeThread();
#else
	void __LinuxTimeThread();
#endif

	bool m_bWantStop;

	std::thread *m_pThread;
    
#ifdef WIN32
    std::atomic_uint32_t m_CurTime_ms;
#else
    std::atomic_uint m_CurTime_ms;
#endif

	void __WorkThread();

public:
    JCGetClockExact();
	~JCGetClockExact();

	bool Init();
	void Stop();

	std::uint32_t GetTimeMs()
	{
		return m_CurTime_ms;
	}
};
//------------------------------------------------------------------------------


#endif //__JCGetClockExact_H__

//-----------------------------END FILE--------------------------------
