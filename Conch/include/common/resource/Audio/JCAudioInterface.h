/**
@file			JCAudioInterface.h
@brief			
@author			James
@version		1.0
@date			2014_11_26
*/

#ifndef __JCAudioInterface_H__
#define __JCAudioInterface_H__

#ifdef OHOS
#include <ohaudio/native_audiostream_base.h>
#endif
#include <stdio.h>

/** 
 * @brief JCAudioInterface
 * 纯接口类
*/
namespace laya
{
#ifdef OHOS
    class OHAudioRenderInfo;
#endif
class JCAudioInterface
{
public:

	virtual void onPlayEnd() = 0;

};

#ifdef OHOS
class OHAudioRenderInfo {
public:
    OH_AudioRenderer *_audioRender;
    OH_AudioStreamBuilder *_builder;
    JCAudioInterface *m_pAudio; // Audio 为了回调使用
    bool m_bPlaying;            // 是否为播放状态
    const char *pcmBuffer;
    int32_t m_pBufferSize;
    int32_t writeOffset = 0;

public:
    void operator=(const OHAudioRenderInfo &other) {
        m_bPlaying = other.m_bPlaying;
        _audioRender = other._audioRender;
        m_pAudio = other.m_pAudio;
    }
};
#endif
}

#endif //__JCAudioInterface_H__

//-----------------------------END FILE--------------------------------