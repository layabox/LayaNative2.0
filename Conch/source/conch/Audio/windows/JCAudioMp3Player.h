/**
@file			JCAudioMp3Player.h
@brief			
@author			James
@version		1.0
@date			2014_11_26
*/

#ifndef __JCAudioMp3Player_H__
#define __JCAudioMp3Player_H__

//包含头文件
#include <windows.h>
#include <xaudio2.h>
#include <strsafe.h>
#include <shellapi.h>
#include <mmsystem.h>
#include <conio.h>
#include <stdio.h>
#include <dshow.h>
#include "resource/Audio/JCMp3Interface.h"
#include <string>

namespace laya
{
    class JCAudioMp3Player : public JCMp3Interface
    {
    public:

	    JCAudioMp3Player();

	    ~JCAudioMp3Player();

	    bool Mp3Init();

	    void release();

    public:

	    void play( const char* p_sUrl,int p_nTimes,float nCurrentTime,JCAudioInterface* p_pJSAudio );
	    void delAudio( JCAudioInterface* p_pJSAudio );

	    void setVolume( float p_nVolume );

	    void pause();

	    void stop();

	    void resume();

	    void setMute(bool p_bMute);

	    void onPlayEnd();

		void setCurrentTime(double nCurrentTime);

        double getCurrentTime();

        double getDuration();
		
    private:

	    friend DWORD WINAPI handleMp3ListenerThread( void* p_pParam  );

	    void releaseThread();

    public:

	    IGraphBuilder*		m_pGraphBuilder;
	    IMediaControl*		m_pMediaControl;
	    IMediaEvent*		m_pMediaEvent;
	    IMediaPosition*		m_pMediaPos;
	    IBasicAudio*		m_pBasicAudio;
	    IMediaSeeking*		m_pMediaSeeking;

    public:

	    DWORD				m_nNotifyThreadID;
	    HANDLE				m_hNotifyThread;

    private:

	    std::string			m_sUrl;
	    int					m_nPlayTimes;
	    int					m_nPlayCount;
	    double				m_fPos;
	    float				m_fCurrentVolume;

    };

}

#endif //__JCAudioMp3Player_H__

//-----------------------------END FILE--------------------------------
