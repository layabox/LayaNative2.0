#ifndef __JCAudioWavPlayer_OHOS_H__
#define __JCAudioWavPlayer_OHOS_H__

#include <stdio.h>
#include <thread>
#include "../../util/JCCommonMethod.h"
#include "JCWaveInfo.h"
#include "JCAudioInterface.h"
#include <unordered_map>
#include <vector>
#include <ohaudio/native_audiostreambuilder.h>
#include <ohaudio/native_audiorenderer.h>

namespace laya{
    class JCFileResManager;
    class JCAudioWavPlayer{
        public:
            typedef std::unordered_map<std::string, JCWaveInfo*> MapWaveInfo;
            typedef MapWaveInfo::iterator MapWaveInfoIter;
        public:
            JCAudioWavPlayer(JCFileResManager* pFileResManager);
            ~JCAudioWavPlayer();
            void Release();
        public:
            OHAudioRenderInfo* playAudio(JCAudioInterface* p_pAudio, const std::string& p_sSrc, bool bIsOgg);
            void delAudio(JCAudioInterface* p_pAudio);
            OHAudioRenderInfo* playAudioFromBuffer(JCAudioInterface* p_pAudio, const char* p_pBuffer, unsigned int p_nBufferSize, int p_nRate, int p_nBitsPerSample, int nChannels);
            void setVolume(OHAudioRenderInfo* pAudioRenderInfo, float p_nVolume);
            void stop(OHAudioRenderInfo* pAudioRenderInfo);
            void setAllVolume(float p_nVolume);
            void stopAll();
            void checkWavePlayEnd();
            void pause();
            void resume();
            static int32_t AudioRendererOnWriteData(OH_AudioRenderer *renderer, void *userData, void *buffer, int32_t bufferLen);
            static int32_t AudioRendererOnInterrupt(OH_AudioRenderer *renderer, void *userData,
                                                    OH_AudioInterrupt_ForceType type, OH_AudioInterrupt_Hint hint);
        public:
            /**
             * @brief 添加资源
             * @return
            */
            JCWaveInfo *AddWaveInfo(const std::string &p_sUrl, unsigned char *p_pBuffer, int p_nSize, const char *p_sFilePath, void *p_pExternalMark, bool p_bIsOgg);
            /**
             * @brief 查找资源根据ulid
             * @param[in] ulid
             * @return
            */
           JCWaveInfo* FindWaveInfo(const std::string& p_sUrl);
            /**
             * @brief 清空所有
             */
            bool ClearAllWaveInfo(void);
            void autoGarbageCollection();
        public:
            static int s_nGarbageCollectionTime;
            MapWaveInfo m_vWaveInfos; //wave的信息
        public:
            JCFileResManager* m_pFileResManager;
            OH_AudioRenderer *audioRenderer;
            std::vector<OHAudioRenderInfo*> m_pAudioRenderSource; //播放声音用的
    };
}
#endif //__JCAudioWavPlayer_H__