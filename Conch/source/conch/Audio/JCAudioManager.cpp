/**
@file			JCAudioManager.cpp
@brief
@author			James
@version		1.0
@date			2014_11_26
*/

//包含头文件
#include "JCAudioManager.h"
#include "resource/Audio/JCWaveParser.h"
#include "util/Log.h"
#include "../JCConch.h"

namespace laya
{
    //------------------------------------------------------------------------------
    JCAudioManager* JCAudioManager::m_sAudioManager = NULL;
    std::mutex      JCAudioManager::m_mutex;
    //------------------------------------------------------------------------------
    JCAudioManager::JCAudioManager(JCFileResManager* pFileResManger)
    {
        m_bStopMp3 = false;
	    m_bMuteMp3 = false;
	    m_nVolumeMp3 = 1.0f;
	    createMp3player();
	    m_pWavPlayer = new JCAudioWavPlayer(pFileResManger);
    }
    //------------------------------------------------------------------------------
    JCAudioManager::~JCAudioManager( void )
    {
	    ClearAllAudioBufferPlay();
	    if( m_pMp3Player != NULL )
	    {
		    delete m_pMp3Player;
		    m_pMp3Player = NULL;
	    }
	    if( m_pWavPlayer != NULL )
	    {
		    m_pWavPlayer->ClearAllWaveInfo();
		    delete m_pWavPlayer;
		    m_pWavPlayer = NULL;
	    }
    }
    //------------------------------------------------------------------------------
    void JCAudioManager::update()
    {
	    m_pWavPlayer->checkWavePlayEnd();
    }
    //------------------------------------------------------------------------------
    JCAudioManager* JCAudioManager::GetInstance( void )
    {
	    if( m_sAudioManager == NULL )
	    {
            std::lock_guard<std::mutex> lk(m_mutex);
		    m_sAudioManager = new JCAudioManager(JCConch::s_pConch->m_pFileResMgr);
	    }
	    return m_sAudioManager;
    }
    //------------------------------------------------------------------------------
    void JCAudioManager::ClearAllWork(){
	    if( m_sAudioManager != NULL )
	    {
		    m_sAudioManager->m_bMuteMp3 = false;
		    JCAudioWavPlayer* pWavPlayer = m_sAudioManager->m_pWavPlayer;
		    if( pWavPlayer != NULL )
		    {
                int nALCount = pWavPlayer->m_pOpenALSource.size();
                for (int i = 0; i < nALCount; i++)
			    {
				    if( pWavPlayer->m_pOpenALSource[i]->m_bPlaying == true )
				    {
					    alSourceStop( pWavPlayer->m_pOpenALSource[i]->m_nOpenALSouceID );
					    pWavPlayer->m_pOpenALSource[i]->m_pAudio = NULL;
					    pWavPlayer->m_pOpenALSource[i]->m_bPlaying = false;
				    }
			    }
			    pWavPlayer->ClearAllWaveInfo();
		    }
		    m_sAudioManager->ClearAllAudioBufferPlay();
	    }
    }

    void JCAudioManager::DelInstance( void )
    {
	    if( m_sAudioManager != NULL )
	    {
            std::lock_guard<std::mutex> lk(m_mutex);
		    m_sAudioManager->m_pWavPlayer->ClearAllWaveInfo();
		    m_sAudioManager->ClearAllAudioBufferPlay();

		    delete m_sAudioManager;
		    m_sAudioManager = NULL;
	    }
    }
    //------------------------------------------------------------------------------
    void JCAudioManager::createMp3player()
    {
    #ifdef WIN32
	    m_pMp3Player = new JCAudioMp3Player();
    #elif ANDROID
	    m_pMp3Player = new JCAudioMp3Media();
    #elif __APPLE__
        m_pMp3Player = new JCAudioMp3Player();
    #endif
    }
    //------------------------------------------------------------------------------
    void JCAudioManager::setMp3Mute( bool p_bMute )
    {
	    m_bMuteMp3 = p_bMute;
	    if( m_pMp3Player != NULL )
	    {
		    m_pMp3Player->setMute( p_bMute );
	    }
    }
    //------------------------------------------------------------------------------
    bool JCAudioManager::getMp3Mute( void )
    {
	    return m_bMuteMp3;
    }
    //------------------------------------------------------------------------------
    bool JCAudioManager::getMp3Stopped()
    {
        return m_bStopMp3;
    }
    //------------------------------------------------------------------------------
    void JCAudioManager::setMp3Volume( float p_nVolume )
    {
	    m_nVolumeMp3 = p_nVolume;
	    if( m_pMp3Player != NULL )
	    {
		    m_pMp3Player->setVolume( p_nVolume );
	    }
    }
    //------------------------------------------------------------------------------
    float JCAudioManager::getMp3Volume( void )
    {
	    return m_nVolumeMp3;
    }
    //------------------------------------------------------------------------------
    void JCAudioManager::playMp3( const char* p_sUrl,int p_nTimes, int nCurrentTime,JCAudioInterface* p_pJSAudio )
    {
	    m_bStopMp3 = false;
	    //每次播放mp3都是重新new
	    if( m_pMp3Player == NULL )
	    {
		    createMp3player();
	    }
	    else
	    {
		    delete m_pMp3Player;
		    createMp3player();
	    }
        if( m_pMp3Player == NULL )return;
	    m_pMp3Player->play( p_sUrl,p_nTimes, (float)nCurrentTime,p_pJSAudio );
	    if( m_bMuteMp3 )
	    {
		    m_pMp3Player->setMute(true);
	    }
	    else
	    {
		    m_pMp3Player->setVolume( (float)m_nVolumeMp3 );
	    }
    }
    void JCAudioManager::delMp3Obj(JCAudioInterface* p_pJSAudio)
    {
	    if(m_pMp3Player)
		    m_pMp3Player->delAudio(p_pJSAudio);
    }
    //------------------------------------------------------------------------------
    void JCAudioManager::pauseMp3()
    {
        LOGI("JCAudioManager::pauseMp3");
	    if( m_pMp3Player != NULL )
	    {
		    m_pMp3Player->pause();
	    }
    }
    //------------------------------------------------------------------------------
    void JCAudioManager::stopMp3()
    {
        LOGI("JCAudioManager::stopMp3");
	    m_bStopMp3 = true;
	    if( m_pMp3Player != NULL )
	    {
		    m_pMp3Player->stop();
	    }
    }
    //------------------------------------------------------------------------------
    void JCAudioManager::resumeMp3()
    {
        m_bStopMp3 = false;
	    if( m_pMp3Player != NULL )
	    {
		    m_pMp3Player->resume();
	    }
    }
    //------------------------------------------------------------------------------
    OpenALSourceInfo* JCAudioManager::playWav(JCAudioInterface* p_pAudio, const std::string& p_sUrl, bool bIsOgg)
    {
        return m_pWavPlayer->playAudio(p_pAudio, p_sUrl, bIsOgg);
    }
    void JCAudioManager::stopWav(OpenALSourceInfo* pOpenALInfo)
    {
        m_pWavPlayer->stop(pOpenALInfo);
    }
    void JCAudioManager::stopAllWav()
    {
        m_pWavPlayer->stopAll();
    }
    void JCAudioManager::setWavVolume(OpenALSourceInfo* pOpenALInfo, float nVolume)
    {
        m_pWavPlayer->setVolume(pOpenALInfo, nVolume);
    }
    void JCAudioManager::setAllWavVolume(float nVolume)
    {
        m_pWavPlayer->setAllVolume(nVolume);
    }
    //------------------------------------------------------------------------------
    bool JCAudioManager::ClearAllAudioBufferPlay( void )
    {
	    return true;
    }
    //------------------------------------------------------------------------------
    void JCAudioManager::delWav(JCAudioInterface* p_pAudio)
    {
	    m_pWavPlayer->delAudio(p_pAudio);
    }
    //------------------------------------------------------------------------------
    JCWaveInfo* JCAudioManager::AddWaveInfo( const std::string& p_sUrl,unsigned char* p_pBuffer,int p_nSize,const char* p_sFilePath,void* p_pExternalMark,bool p_bIsOgg )
    {
	    return m_pWavPlayer->AddWaveInfo( p_sUrl,p_pBuffer,p_nSize,p_sFilePath,p_pExternalMark,p_bIsOgg );
    }
    //------------------------------------------------------------------------------
    JCWaveInfo* JCAudioManager::AddWaveInfo( const std::string& p_sUrl,JCBuffer& p_pBuffer,int p_nSize,void* p_pExternalMark,bool p_bIsOgg )
    {
	    return m_pWavPlayer->AddWaveInfo( p_sUrl,(unsigned char*)p_pBuffer.m_pPtr,p_nSize,NULL,p_pExternalMark,p_bIsOgg );
    }
    //------------------------------------------------------------------------------
    JCWaveInfo* JCAudioManager::FindWaveInfo( const std::string& p_sUrl )
    {
	    return m_pWavPlayer->FindWaveInfo( p_sUrl );
    }
    //------------------------------------------------------------------------------
	void JCAudioManager::setCurrentTime(double nCurrentTime)
	{
		if( m_pMp3Player != NULL )
		{
			m_pMp3Player->setCurrentTime(nCurrentTime);
		}
	}
	//------------------------------------------------------------------------------
	double JCAudioManager::getCurrentTime()
	{
		if( m_pMp3Player != NULL )
		{
			return m_pMp3Player->getCurrentTime();
		}
		return 0.0f;
	}
	//------------------------------------------------------------------------------
	double JCAudioManager::getDuration()
	{
		if( m_pMp3Player != NULL )
		{
			return m_pMp3Player->getDuration();
		}
		return 0.0f;
	}

}

//-----------------------------END FILE--------------------------------
