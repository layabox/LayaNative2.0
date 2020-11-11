/**
@file			JCAudioMp3Media.cpp
@brief			
@author			dt
@version		1.0
@date			2014_12_24
*/


//包含头文件
#include "JCAudioMp3Player.h"
#include "util/Log.h"
#include "CToObjectC.h"

namespace laya
{
//------------------------------------------------------------------------------
JCAudioMp3Player::JCAudioMp3Player()
{
	m_nCurrentVolume = 1.0;
	m_pJSAudio = NULL;
}
//------------------------------------------------------------------------------
JCAudioMp3Player::~JCAudioMp3Player( void )
{
	m_pJSAudio = NULL;
}
//------------------------------------------------------------------------------
void JCAudioMp3Player::play( const char* p_sUrl,int p_nTimes,float nCurrentTime,JCAudioInterface* p_pJSAudio )
{
	m_pJSAudio = p_pJSAudio;
	CToObjectCPlayMp3Audio( p_sUrl,p_nTimes,nCurrentTime );
}
//------------------------------------------------------------------------------
void JCAudioMp3Player::delAudio( JCAudioInterface* p_pJSAudio )
{
	if( m_pJSAudio == p_pJSAudio)
	{
		m_pJSAudio = NULL;
	}
}
//------------------------------------------------------------------------------
void JCAudioMp3Player::pause()
{
	CToObjectCPauseMp3();
}
//------------------------------------------------------------------------------
void JCAudioMp3Player::stop()
{
	m_pJSAudio = NULL;
	CToObjectCStopMp3();
}
//------------------------------------------------------------------------------
void JCAudioMp3Player::resume()
{
	CToObjectCResumeMp3();
}
//------------------------------------------------------------------------------
void JCAudioMp3Player::setVolume( float p_nVolume )
{
	m_nCurrentVolume = p_nVolume;
    /*
    if( p_nVolume < -10000 ) p_nVolume = -10000;
    if( p_nVolume >0 ) p_nVolume = 0;
    //－10000  到  0 ，转换到 0 - 1
	float nVolume = (p_nVolume+10000)/10000.0;
    */
    CToObjectCSetMp3Volume( p_nVolume );
}
//------------------------------------------------------------------------------
void JCAudioMp3Player::setMute( bool p_bMute )
{
	if( p_bMute == true )
	{
		float nTemp = m_nCurrentVolume;
		setVolume( 0.0f );
		m_nCurrentVolume = nTemp;
	}
	else
	{
		setVolume( m_nCurrentVolume );
	}
}
//------------------------------------------------------------------------------
void JCAudioMp3Player::onPlayEnd()
{
	if( m_pJSAudio )
	{
		m_pJSAudio->onPlayEnd();
	}
}
void JCAudioMp3Player::setCurrentTime(double nCurrentTime)
{
    CToObjectCSetCurrentTime(nCurrentTime);
}
//------------------------------------------------------------------------------
double JCAudioMp3Player::getCurrentTime()
{
    return CToObjectCGetCurrentTime();
}
//------------------------------------------------------------------------------
double JCAudioMp3Player::getDuration()
{
    return CToObjectCGetDuration();
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
}

//-----------------------------END FILE--------------------------------
