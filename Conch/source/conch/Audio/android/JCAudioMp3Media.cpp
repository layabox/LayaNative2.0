/**
@file			JCAudioMp3Media.cpp
@brief			
@author			dt
@version		1.0
@date			2014_12_24
*/


//包含头文件
#include <vector>
#include "JCAudioMp3Media.h"
#include "util/Log.h"
#include "../../CToJavaBridge.h"

namespace laya
{
//------------------------------------------------------------------------------
JCAudioMp3Media::JCAudioMp3Media()
{
	m_nCurrentVolume = 1.0;
	m_pJSAudio = NULL;
}
//------------------------------------------------------------------------------
JCAudioMp3Media::~JCAudioMp3Media( void )
{
	m_pJSAudio = NULL;
}
//------------------------------------------------------------------------------
void JCAudioMp3Media::play( const char* p_sUrl,int p_nTimes,float nCurrentTime,JCAudioInterface* p_pJSAudio )
{
	m_pJSAudio = p_pJSAudio;
	CToJavaBridge::JavaRet ret;
    CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.LayaAudioMusic", "playBackgroundMusic", p_sUrl, p_nTimes,(int)(nCurrentTime*1000),ret);
}
//------------------------------------------------------------------------------
void JCAudioMp3Media::delAudio( JCAudioInterface* p_pJSAudio )
{
	if( m_pJSAudio == p_pJSAudio){
		m_pJSAudio = NULL;
	}
}
//------------------------------------------------------------------------------
void JCAudioMp3Media::pause()
{
	CToJavaBridge::JavaRet ret;
    CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.LayaAudioMusic", "pauseBackgroundMusic", ret);
}
//------------------------------------------------------------------------------
void JCAudioMp3Media::stop()
{
	CToJavaBridge::JavaRet ret;
    CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.LayaAudioMusic", "stopBackgroundMusic", ret);
	m_pJSAudio = NULL;
}
//------------------------------------------------------------------------------
void JCAudioMp3Media::resume()
{
	CToJavaBridge::JavaRet ret;
    CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.LayaAudioMusic", "resumeBackgroundMusic", ret);
}
//------------------------------------------------------------------------------
void JCAudioMp3Media::setVolume( float p_nVolume )
{
	m_nCurrentVolume = p_nVolume;
	/*if( p_nVolume < -10000 ) p_nVolume = -10000;
	if( p_nVolume > 0 ) p_nVolume = 0;
	p_nVolume = (p_nVolume+10000)/10000.0;*/
	CToJavaBridge::JavaRet ret;
    CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.LayaAudioMusic", "setBackgroundMusicVolume",p_nVolume, ret);
}
//------------------------------------------------------------------------------
void JCAudioMp3Media::setMute( bool p_bMute )
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
void JCAudioMp3Media::onPlayEnd()
{
	if( m_pJSAudio )
	{
		m_pJSAudio->onPlayEnd();
	}
}
void JCAudioMp3Media::setCurrentTime(double nCurrentTime)
{
	CToJavaBridge::JavaRet ret;
    CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.LayaAudioMusic", "setCurrentTime", (float)nCurrentTime, ret);
}
//------------------------------------------------------------------------------
double JCAudioMp3Media::getCurrentTime()
{
	CToJavaBridge::JavaRet ret;
    if (CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.LayaAudioMusic", "getCurrentTime", ret, CToJavaBridge::JavaRet::RT_Float))
	{
		return (double)ret.floatRet;
	}
	return 0.0f;
}
//------------------------------------------------------------------------------ 
double JCAudioMp3Media::getDuration()
{
	CToJavaBridge::JavaRet ret;
    if (CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.LayaAudioMusic", "getDuration", ret, CToJavaBridge::JavaRet::RT_Float))
	{
		return (double)ret.floatRet;
	}
	
	return 0.0f;
}
//------------------------------------------------------------------------------
}

//-----------------------------END FILE--------------------------------
