/**
@file			JCGetClockExact.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#include "JCGetClockExact.h"
#include <atomic>

#ifdef WIN32
#include <windows.h>
#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib")

HANDLE _l_hTimerEvent = 0;
UINT64 _l_u64_machine_cpu_freq = 0;
UINT _l_dwTimerRes = 0;
UINT64 _l_u64CurrentTime = 0;
UINT64 _l_u64StartupTime = 0;
DWORD _l_dwTimerID = 0;

inline void _GetCurrentTime()
{
	::QueryPerformanceCounter( (LARGE_INTEGER *)&_l_u64CurrentTime );
	_l_u64CurrentTime = _l_u64CurrentTime * 1000 / _l_u64_machine_cpu_freq;
}

static bool __InitWindowsTimer()
{
	TIMECAPS tc;
	if(::timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) 
	{
		return false;
	}

	if( tc.wPeriodMin > 1 )
		_l_dwTimerRes = tc.wPeriodMin;
	else
		_l_dwTimerRes = 1;

	if( ::timeBeginPeriod( _l_dwTimerRes ) != TIMERR_NOERROR )
	{
		_l_dwTimerRes = 0;
		return false;
	}

	::QueryPerformanceFrequency( (LARGE_INTEGER *)&_l_u64_machine_cpu_freq );

	if( NULL == (_l_hTimerEvent = ::CreateEvent( NULL, FALSE, FALSE, NULL )) )
	{
		return false;
	}

	_GetCurrentTime();
	_l_u64StartupTime = _l_u64CurrentTime;

	if( (_l_dwTimerID = ::timeSetEvent( 
			5,					// 延时
			_l_dwTimerRes,					// 分辨率
			(LPTIMECALLBACK)_l_hTimerEvent,	// 事件接收句柄
			(DWORD)0,
			TIME_PERIODIC|TIME_CALLBACK_EVENT_SET )) == NULL )
	{
		return false;
	}

	return true;
}

static void __ReleaseWindowsTimer()
{
	if( 0 != _l_dwTimerID )
	{
		::timeKillEvent( _l_dwTimerID );
		_l_dwTimerID = 0;
	}

	if( 0 != _l_hTimerEvent )
	{
		::CloseHandle( _l_hTimerEvent );
		_l_hTimerEvent = 0;
	}

	if( 0 != _l_dwTimerRes )
	{
		::timeEndPeriod( _l_dwTimerRes );
		_l_dwTimerRes = 0;
	}
}
#else	// WIN32
#include <unistd.h>
#endif	// WIN32

JCGetClockExact::JCGetClockExact()
{
	m_CurTime_ms = 0;
	m_pThread = 0;
	m_bWantStop = false;
}

JCGetClockExact::~JCGetClockExact()
{

}

bool JCGetClockExact::Init()
{
	if( 0 != m_pThread )
		return false;

#ifdef WIN32
	if( !__InitWindowsTimer() )
		return false;
#endif

	m_bWantStop = false;

	m_pThread = new std::thread( std::bind(&JCGetClockExact::__WorkThread, this) );

	return true;
}

void JCGetClockExact::Stop()
{
	m_bWantStop = true;

	if( 0 != m_pThread )
	{
		m_pThread->join();
		delete m_pThread;
		m_pThread = 0;
	}

#ifdef WIN32
	__ReleaseWindowsTimer();
#endif
}

void JCGetClockExact::__WorkThread()
{
#ifdef WIN32
	__WindowsTimeThread();
#else
	__LinuxTimeThread();
#endif
}

#ifdef WIN32
void JCGetClockExact::__WindowsTimeThread()
{
	std::uint32_t ulCur;

	for(;!m_bWantStop;)
	{
		::WaitForSingleObject( _l_hTimerEvent, INFINITE );

		_GetCurrentTime();
		ulCur = (std::uint32_t)(_l_u64CurrentTime - _l_u64StartupTime);

		m_CurTime_ms = ulCur;
	}
}
#else
void  JCGetClockExact::__LinuxTimeThread()
{
	std::uint32_t _TmpCurTime = m_CurTime_ms;
	for(;!m_bWantStop;)
	{
		usleep( 5000 );
		_TmpCurTime += 5;
//		std::interprocess::ipcdetail::atomic_inc32( &m_CurTime_ms );
		m_CurTime_ms = _TmpCurTime;
	}
}
#endif
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
