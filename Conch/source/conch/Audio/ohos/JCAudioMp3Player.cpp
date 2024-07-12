#include <vector>
#include "JCAudioMp3Player.h"
#include "util/Log.h"
#include "aki/jsbind.h"
#include "helper/NapiHelper.h"

namespace laya{
    JCAudioMp3Player::JCAudioMp3Player(){
        m_nCurrentVolume = 1.0;
        m_pJSAudio = NULL;
    }
    JCAudioMp3Player::~JCAudioMp3Player(void){
        m_pJSAudio = NULL;
    }
    void JCAudioMp3Player::play(const char* p_sUrl, int p_nTimes, float nCurrentTime, JCAudioInterface* p_pJSAudio){
        m_pJSAudio = p_pJSAudio;
        NapiHelper::GetInstance()->playBackgroundMusic(p_sUrl, p_nTimes, (int)(nCurrentTime*1000));
    }
    void JCAudioMp3Player::delAudio(JCAudioInterface* p_pJSAudio){
        if(m_pJSAudio ==  p_pJSAudio){
            m_pJSAudio = NULL;
        }
    }
    void JCAudioMp3Player::pause(){
        NapiHelper::GetInstance()->pauseBackgroundMusic();
    }
    void JCAudioMp3Player::stop(){
        NapiHelper::GetInstance()->stopBackgroundMusic();
        m_pJSAudio = NULL;
    }
    void JCAudioMp3Player::resume(){
        NapiHelper::GetInstance()->resumeBackgroundMusic();
    }
    void JCAudioMp3Player::setVolume(float p_nVolume){
        m_nCurrentVolume = p_nVolume;
        NapiHelper::GetInstance()->setBackgroundMusicVolume(p_nVolume);
    }
    void JCAudioMp3Player::setMute(bool p_bMute){
        if(p_bMute == true){
            float nTemp = m_nCurrentVolume;
            setVolume(0.0f);
            m_nCurrentVolume = nTemp;
        }else{
            setVolume(m_nCurrentVolume);
        }
    }
    void JCAudioMp3Player::onPlayEnd(){
        if(m_pJSAudio){
            m_pJSAudio->onPlayEnd();
        }
    }
    void JCAudioMp3Player::setCurrentTime(double nCurrentTime){
        NapiHelper::GetInstance()->setCurrentTime(nCurrentTime);
    }
    double JCAudioMp3Player::getCurrentTime(){
        return NapiHelper::GetInstance()->getCurrentTime();
    }
    double JCAudioMp3Player::getDuration(){
        return NapiHelper::GetInstance()->getDuration();
    }
}