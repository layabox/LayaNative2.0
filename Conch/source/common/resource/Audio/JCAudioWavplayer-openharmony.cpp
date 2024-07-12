#include "JCAudioWavPlayer-openharmony.h"
#include "../../util/JCCommonMethod.h"
#include "../../util/Log.h"
#include "JCWaveParser.h"
#include "JCOggParser.h"
#include "../JCFileResManager.h"

namespace laya{
    int JCAudioWavPlayer::s_nGarbageCollectionTime = 30000;//30秒
    JCAudioWavPlayer::JCAudioWavPlayer(JCFileResManager* pFileResManager){
        m_pFileResManager = pFileResManager;
        m_pAudioRenderSource.reserve(128);
    }
    JCAudioWavPlayer::~JCAudioWavPlayer(){
        Release();
    }
    void JCAudioWavPlayer::checkWavePlayEnd(){

    }
    OHAudioRenderInfo* JCAudioWavPlayer::playAudio(JCAudioInterface* p_pAudio, const std::string& p_sSrc, bool bIsOgg){
        JCWaveInfo* pInfo = NULL;
        MapWaveInfoIter iter = m_vWaveInfos.find(p_sSrc);
        if(iter != m_vWaveInfos.end()){
            pInfo = iter->second;
        }else{
            JCFileRes* pRes = m_pFileResManager->getRes(p_sSrc.c_str());
            JCBuffer kBuffer;
            if(pRes && pRes->loadFromCache(kBuffer,false)){
                pInfo = AddWaveInfo(p_sSrc,(unsigned char*)kBuffer.m_pPtr,kBuffer.m_nLen,NULL,p_pAudio,bIsOgg);
            }else{
                LOGE("JCAudioWavPlayer::playAudio load res error");
            }
        }
        if(pInfo!=NULL){
            pInfo -> m_nTouchTime = tmGetCurms();
            WAVE_FORMAT* pFormat = &(pInfo->m_kFmtBlock.wavFormat);
            return playAudioFromBuffer(p_pAudio,(char*)(pInfo->m_pData),pInfo->m_nRealDataSize,pFormat->dwSamplesPerSec,pFormat->wBitsPerSample,pFormat->wChannels);
        }
        return NULL;
    }
    void JCAudioWavPlayer::delAudio(JCAudioInterface* p_pAudio){
        int m_ohAudioCount = m_pAudioRenderSource.size();
        for(int i = 0; i <m_ohAudioCount; i++){
            if(m_pAudioRenderSource[i]->m_pAudio==p_pAudio){
                m_pAudioRenderSource[i]->m_pAudio = NULL;
            }
        }
    }
    //----------------------------------------------------------------------------------
    OHAudioRenderInfo* JCAudioWavPlayer::playAudioFromBuffer(JCAudioInterface* p_pAudio,const char* p_pBuffer, unsigned int p_nBufferSize,
                    int p_nRate, int nBitsPerSample, int nChannels){
        OHAudioRenderInfo* audioRenderInfo = new OHAudioRenderInfo;
        OH_AudioStream_Result ret;
        OH_AudioStream_Type type = AUDIOSTREAM_TYPE_RENDERER;
        OH_AudioStreamBuilder* _builder;
        ret = OH_AudioStreamBuilder_Create(&_builder, type);
        if(ret != AUDIOSTREAM_SUCCESS){
            return nullptr;
        }
        OH_AudioStreamBuilder_SetSamplingRate(_builder, p_nRate);
        OH_AudioStreamBuilder_SetChannelCount(_builder, nChannels);

        OH_AudioStream_SampleFormat nFormat;
        switch (nBitsPerSample)
        {
        case 8:
            nFormat = AUDIOSTREAM_SAMPLE_U8;
            break;
        case 16:
            nFormat = AUDIOSTREAM_SAMPLE_S16LE;
            break;
        default:
            LOGE("JCAudioWavPlayer::playAudioFromBuffer sound bitsPerSample error, only support 8 or 16");
            nFormat = AUDIOSTREAM_SAMPLE_S16LE;
            break;
        }
        OH_AudioStreamBuilder_SetSampleFormat(_builder, nFormat);
        //设置低时延模式
        OH_AudioStreamBuilder_SetLatencyMode(_builder, AUDIOSTREAM_LATENCY_MODE_FAST);

        OH_AudioStreamBuilder_SetRendererInfo(_builder, AUDIOSTREAM_USAGE_GAME);
        OH_AudioRenderer_Callbacks callbacks;
        callbacks.OH_AudioRenderer_OnWriteData = AudioRendererOnWriteData;
        ret = OH_AudioStreamBuilder_SetRendererCallback(_builder, callbacks, (void *)audioRenderInfo);
        if(ret!=AUDIOSTREAM_SUCCESS){
            return nullptr;
        }
        ret = OH_AudioStreamBuilder_GenerateRenderer(_builder, &audioRenderer);
        if(ret!=AUDIOSTREAM_SUCCESS){
            return nullptr;
        } 

        audioRenderInfo->_audioRender=audioRenderer;
        audioRenderInfo->_builder = _builder;
        audioRenderInfo->m_pAudio = p_pAudio;
        audioRenderInfo->m_bPlaying = true;
        audioRenderInfo->pcmBuffer = p_pBuffer;
        audioRenderInfo->m_pBufferSize = p_nBufferSize;
        m_pAudioRenderSource.push_back(audioRenderInfo);

        ret = OH_AudioRenderer_Start(audioRenderer);
        return audioRenderInfo;
    }
    int32_t JCAudioWavPlayer::AudioRendererOnWriteData(OH_AudioRenderer *renderer, void *userData, void *buffer, int32_t bufferLen){
        OHAudioRenderInfo *audioRenderInfo = (OHAudioRenderInfo *)userData;
        const char *dataBuffer = audioRenderInfo->pcmBuffer;
        if(dataBuffer == nullptr){
            return 0;
        }
        int32_t writeBytes = audioRenderInfo->m_pBufferSize - audioRenderInfo->writeOffset >bufferLen?bufferLen: audioRenderInfo->m_pBufferSize - audioRenderInfo->writeOffset;
        if(writeBytes<=0){
            if(audioRenderInfo->m_bPlaying ==false){
                return 0;
            }
            if(audioRenderInfo->m_pAudio !=NULL){
                audioRenderInfo->m_pAudio->onPlayEnd();
                audioRenderInfo->m_pAudio=NULL;
            }
            if(audioRenderInfo->_audioRender!=nullptr){
                OH_AudioRenderer_Stop(audioRenderInfo->_audioRender);
                OH_AudioRenderer_Release(audioRenderInfo->_audioRender);
            }
            if(audioRenderInfo->_builder != nullptr){
                OH_AudioStreamBuilder_Destroy(audioRenderInfo->_builder);
            }
            audioRenderInfo->m_pAudio = NULL;
            audioRenderInfo->m_bPlaying = false;
            return 0;
        }
        memcpy(buffer,dataBuffer+audioRenderInfo->writeOffset,writeBytes);
        audioRenderInfo->writeOffset += writeBytes;
        return 0;
    }
    void JCAudioWavPlayer::stopAll(){
        int m_ohAudioCount = m_pAudioRenderSource.size();
        for (int i = 0; i < m_ohAudioCount; i++)
        {
            if(m_pAudioRenderSource[i]->m_bPlaying == true){
                OHAudioRenderInfo *pAudioRenderInfo = m_pAudioRenderSource[i];
                if(pAudioRenderInfo->_audioRender != nullptr){
                    OH_AudioRenderer_Stop(pAudioRenderInfo->_audioRender);
                    OH_AudioRenderer_Release(pAudioRenderInfo->_audioRender);
                }

                if(pAudioRenderInfo->_builder != nullptr){
                    OH_AudioStreamBuilder_Destroy(pAudioRenderInfo->_builder);
                }
                pAudioRenderInfo->m_bPlaying = false;
            }
        }
    }
    void JCAudioWavPlayer::pause(){
        int32_t m_ohAudioCount = m_pAudioRenderSource.size();
        for(int i=0; i< m_ohAudioCount; i++){
            if(m_pAudioRenderSource[i]->_audioRender != nullptr){
                OH_AudioRenderer_Pause(m_pAudioRenderSource[i]->_audioRender);
            }
        }
    }
    void JCAudioWavPlayer::resume(){
        int m_ohAudioCount = m_pAudioRenderSource.size();
        for(int i=0; i< m_ohAudioCount; i++){
            if(m_pAudioRenderSource[i]->_audioRender != nullptr){
                OH_AudioRenderer_Start(m_pAudioRenderSource[i]->_audioRender);
            }
        }
    }
    void JCAudioWavPlayer::setAllVolume(float p_nVolume){
        int m_ohAudioCount = m_pAudioRenderSource.size();
        for(int i=0; i< m_ohAudioCount; i++){
            if(m_pAudioRenderSource[i]->_audioRender != nullptr){
                //todo OHAudio设置音量接口暂不支持 后续补充
            }
        }
    }
    
    void JCAudioWavPlayer::stop(OHAudioRenderInfo * pAudioRenderInfo){
        if(pAudioRenderInfo->m_bPlaying==true){
            if(pAudioRenderInfo->_audioRender!=nullptr){
                OH_AudioRenderer_Stop(pAudioRenderInfo->_audioRender);
                OH_AudioRenderer_Release(pAudioRenderInfo->_audioRender);
            }
            if(pAudioRenderInfo->_builder!=nullptr){
                OH_AudioStreamBuilder_Destroy(pAudioRenderInfo->_builder);
            }
            pAudioRenderInfo->m_pAudio = NULL;
            pAudioRenderInfo->m_bPlaying = false;
        }
    }
    void JCAudioWavPlayer::setVolume(OHAudioRenderInfo * pAudioRenderInfo, float p_nVolume){
        //todo OHAudio设置音量接口暂不支持，后续补充
    }
    void JCAudioWavPlayer::Release(){
        for(int i=0; i< m_pAudioRenderSource.size(); i++){
           delete m_pAudioRenderSource[i];
        }
    }
    JCWaveInfo *JCAudioWavPlayer::AddWaveInfo(const std::string &p_sUrl, unsigned char *p_pBuffer, int p_nSize, const char *p_sFilePath, void *p_pExternalMark, bool p_bIsOgg){
        JCWaveInfo *pInfo = FindWaveInfo(p_sUrl);
        if(pInfo == NULL){
            if(p_bIsOgg == false){
                pInfo= JCWaveParser::GetInstance()->GetWaveInfoFromBuffer(p_pBuffer,p_nSize);
            }else{
                pInfo = JCOggParser::GetInstance()->GetWaveInfo(p_sFilePath,p_pBuffer,p_nSize);
            }
            if(pInfo!=NULL){
                pInfo->m_sUrl = p_sUrl;
                pInfo->m_nTouchTime = tmGetCurms();
                pInfo->m_pExternalMark = p_pExternalMark;
                m_vWaveInfos[p_sUrl] = pInfo;
            }else{
                LOGE("JCAudioWavPlayer::AddWaveInfo wave paser err");
            }
        }
        return pInfo;
    }
    void JCAudioWavPlayer::autoGarbageCollection(){
        if (m_vWaveInfos.size()<=0)
            return;
        double nCurrentTime = tmGetCurms();
        for(MapWaveInfoIter iter = m_vWaveInfos.begin(); iter!= m_vWaveInfos.end();){
            JCWaveInfo *pInfo = iter->second;
            if(pInfo&&(nCurrentTime- pInfo->m_nTouchTime > s_nGarbageCollectionTime)){
                delete pInfo;
                pInfo = NULL;
                iter = m_vWaveInfos.erase(iter);
            }else{
                iter++;
            }
        }
    }
    JCWaveInfo *JCAudioWavPlayer::FindWaveInfo(const std::string &p_sUrl){
        MapWaveInfoIter iter = m_vWaveInfos.find(p_sUrl);
        if(iter!=m_vWaveInfos.end()){
            return iter->second;
        }
        return NULL;
    }
    bool JCAudioWavPlayer::ClearAllWaveInfo(void){
        for(MapWaveInfoIter iter = m_vWaveInfos.begin();iter!=m_vWaveInfos.end();iter++){
            JCWaveInfo *pInfo = NULL;
            pInfo = iter->second;
            if(pInfo!=NULL){
                delete pInfo;
                pInfo = NULL;
            }
        }
        m_vWaveInfos.clear();
        return true;
    }
}