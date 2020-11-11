/**
@file			JCAudioMp3Player.cpp
@brief			
@author			wyw
@version		1.0
@date			2012_11_14
*/


//包含头文件
#include "JCAudioMp3Player.h"
#include <android/log.h>
#include "../../../util/Log.h"
#include "../JCAudioManager.h"

namespace laya
{
//------------------------------------------------------------------------------
JCAudioMp3Player::JCAudioMp3Player( const char* p_sFilePath )
{
	m_pPlayerObject=NULL;
	m_pPlayerPlay=NULL;
	m_pPlayerSeek=NULL;
	m_pPlayerMuteSolo=NULL;
	m_pPlayerVolume=NULL;
	m_sUrlName = p_sFilePath;
	m_nCurrentVolume = 0;
	m_bInit = false;
}
//------------------------------------------------------------------------------
JCAudioMp3Player::~JCAudioMp3Player( void )
{
	shutdown();
}
//------------------------------------------------------------------------------
void JCAudioMp3Player::shutdown( void )
{
    if( m_pPlayerObject != NULL )
    {
        (*m_pPlayerObject)->Destroy(m_pPlayerObject);
        m_pPlayerObject = NULL;
        m_pPlayerPlay = NULL;
        m_pPlayerSeek = NULL;
        m_pPlayerMuteSolo = NULL;
        m_pPlayerVolume = NULL;
    }
}
//------------------------------------------------------------------------------
bool JCAudioMp3Player::createAudioPlayer( std::string p_sUrl )
{
	if( m_bInit == true ) return true;
	m_bInit = true;
	SLresult pResult;

	//设置数据来源
	SLchar* sUrl = (SLchar*)p_sUrl.c_str();
	SLDataLocator_URI kLocUri = { SL_DATALOCATOR_URI, sUrl };
	SLDataFormat_MIME kFormatMime = { SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED };
	SLDataSource kAudioSrc = { &kLocUri, &kFormatMime };

	//sink
#ifdef ANDROIDSLES
	SLDataLocator_OutputMix loc_outmix = { SL_DATALOCATOR_OUTPUTMIX, JCAudioManager::GetInstance()->getMixObject() };
	SLDataSink audioSnk = {&loc_outmix, NULL};
#endif
	//创建声音
	const SLInterfaceID ids[3] = {SL_IID_SEEK, SL_IID_MUTESOLO, SL_IID_VOLUME};
	const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
#ifdef ANDROIDSLES
	pResult = (*JCAudioManager::GetInstance()->getEngine())->CreateAudioPlayer( JCAudioManager::GetInstance()->getEngine(), &m_pPlayerObject, &kAudioSrc,&audioSnk, 3, ids, req );
#endif
	if( pResult != SL_RESULT_SUCCESS )
	{
		LOGE("createAudioPlayer err=CreateAudioPlayer");
		return false;
	}

	//realize播放对象
	pResult = (*m_pPlayerObject)->Realize(m_pPlayerObject, SL_BOOLEAN_FALSE);
	if( pResult != SL_RESULT_SUCCESS )
	{
		(*m_pPlayerObject)->Destroy( m_pPlayerObject );
		m_pPlayerObject = NULL;
		LOGE("createAudioPlayer err=RealizePlayer");
		return false;
	}

	//获得play
	pResult = (*m_pPlayerObject)->GetInterface(m_pPlayerObject, SL_IID_PLAY, &m_pPlayerPlay);
	if( pResult != SL_RESULT_SUCCESS )
	{
		LOGE("createAudioPlayer err=GetPlayer");
		return false;
	}

	//获得seek
	pResult = (*m_pPlayerObject)->GetInterface(m_pPlayerObject, SL_IID_SEEK, &m_pPlayerSeek);
	if( pResult != SL_RESULT_SUCCESS )
	{
		LOGE("createAudioPlayer err=GetSeek");
		return false;
	}

	//获得mutesolo
	pResult = (*m_pPlayerObject)->GetInterface(m_pPlayerObject, SL_IID_MUTESOLO, &m_pPlayerMuteSolo);
	if( pResult != SL_RESULT_SUCCESS )
	{
		LOGE("createAudioPlayer err=GetMuteSolo");
		return false;
	}

	//获得设置音量的
	pResult = (*m_pPlayerObject)->GetInterface(m_pPlayerObject, SL_IID_VOLUME, &m_pPlayerVolume);
	if( pResult != SL_RESULT_SUCCESS )
	{
		LOGE("createAudioPlayer err=GetVolume");
		return false;
	}

	return true;
}
//------------------------------------------------------------------------------
bool JCAudioMp3Player::setPlayingAudioPlayer( short p_nState )
{
	if( m_bInit == false ) createAudioPlayer( m_sUrlName );
	if (m_pPlayerPlay != NULL )
	{
		if( p_nState == SL_PLAYSTATE_PLAYING )
		{
			(*m_pPlayerPlay)->SetPlayState( m_pPlayerPlay, SL_PLAYSTATE_STOPPED );
			(*m_pPlayerPlay)->SetPlayState( m_pPlayerPlay, SL_PLAYSTATE_PLAYING );
		}
		else
		{
			(*m_pPlayerPlay)->SetPlayState( m_pPlayerPlay, p_nState );
		}
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
bool JCAudioMp3Player::setLoopingAudioPlayer( bool p_bLoop )
{
	if( m_bInit == false ) createAudioPlayer( m_sUrlName );
	if ( m_pPlayerSeek != NULL )
	{
		(*m_pPlayerSeek)->SetLoop(m_pPlayerSeek, (SLboolean) p_bLoop, 0,SL_TIME_UNKNOWN);
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
bool JCAudioMp3Player::setChannelMuteAudioPlayer( int p_nChannel,bool p_nMute )
{
	if( m_bInit == false ) createAudioPlayer( m_sUrlName );
	if ( m_pPlayerMuteSolo != NULL )
	{
		(*m_pPlayerMuteSolo)->SetChannelMute( m_pPlayerMuteSolo, p_nChannel, p_nMute );
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
bool JCAudioMp3Player::setChannelSoloAudioPlayer( int p_nChannel,bool p_nSolo )
{
	if( m_bInit == false ) createAudioPlayer( m_sUrlName );
	if ( m_pPlayerMuteSolo != NULL )
	{
		(*m_pPlayerMuteSolo)->SetChannelSolo( m_pPlayerMuteSolo, p_nChannel, p_nSolo );
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
bool JCAudioMp3Player::setVolumeAudioPlayer( int p_nVolume )
{
	if( m_bInit == false ) createAudioPlayer( m_sUrlName );
	m_nCurrentVolume = p_nVolume;
	if ( m_pPlayerVolume != NULL )
	{
		(*m_pPlayerVolume)->SetVolumeLevel(m_pPlayerVolume, p_nVolume );
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
bool JCAudioMp3Player::setMute( bool p_bMute )
{
	if( m_bInit == false ) createAudioPlayer( m_sUrlName );
	if ( m_pPlayerVolume != NULL )
	{
		(*m_pPlayerVolume)->SetMute(m_pPlayerVolume, p_bMute );
		if( p_bMute == true )
		{
			(*m_pPlayerVolume)->SetVolumeLevel(m_pPlayerVolume, -10000 );
		}
		else
		{
			(*m_pPlayerVolume)->SetVolumeLevel(m_pPlayerVolume, m_nCurrentVolume );
		}
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
}

//-----------------------------END FILE--------------------------------
