/**
@file			JCAudioWavPlayer.h
@brief			
@author			James
@version		1.0
@date			2014_4_22
*/

#ifndef __JCAudioWavPlayer_H__
#define __JCAudioWavPlayer_H__

//包含头文件
#ifdef WIN32
    #include <xaudio2.h>
    #include <al.h>
    #include <alc.h>
#elif __APPLE__
    #include <OpenAL/al.h>
    #include <OpenAl/alc.h>
#elif ANDROID
	#include <AL/al.h>
    #include <AL/alc.h>
#endif
#include <stdio.h>
#include <thread>
#include "../../util/JCCommonMethod.h"
#include "JCWaveInfo.h"
#include "JCAudioInterface.h"
#include<unordered_map>
#include <vector>
namespace laya
{
class JCFileResManager;
#define OPENAL_SOURCE_NUM	10

struct OpenALSourceInfo
{
	ALuint				m_nOpenALSouceID;	//OpenAl的ID
	ALuint				m_nBufferID;		//bufferID
	ALfloat				m_vSourcePos[3];	//源声音的位置
	ALfloat				m_vSourceVel[3];	//源声音的速度
	bool				m_bPlaying;			//是否为播放状态
	JCAudioInterface*	m_pAudio;			//Audio 为了回调使用
    void operator=(const OpenALSourceInfo& other)
    {
        m_nOpenALSouceID = other.m_nOpenALSouceID;
        m_nBufferID = other.m_nBufferID;
        m_bPlaying = other.m_bPlaying;
        m_pAudio = other.m_pAudio;
        for (int i = 0; i < 3; i++)
        {
            m_vSourcePos[i] = other.m_vSourcePos[i];
            m_vSourceVel[i] = other.m_vSourceVel[i];
        }
    }
};

class JCAudioWavPlayer
{
public:

	typedef std::unordered_map<std::string,JCWaveInfo*>       MapWaveInfo;
	typedef MapWaveInfo::iterator								MapWaveInfoIter;

public:

	JCAudioWavPlayer(JCFileResManager* pFileResManager);

	~JCAudioWavPlayer();
	
	void Release();

public:

    OpenALSourceInfo* playAudio( JCAudioInterface* p_pAudio,const std::string& p_sSrc,bool bIsOgg );

	void delAudio(JCAudioInterface* p_pAudio);

    OpenALSourceInfo* playAudioFromBuffer( JCAudioInterface* p_pAudio,const char* p_pBuffer,unsigned int p_nBufferSize,int p_nRate, int p_nBitsPerSample, int nChannels);

	void setVolume(OpenALSourceInfo* pOpenALInfo,float p_nVolume );

	void stop(OpenALSourceInfo* pOpenALInfo);

    void setAllVolume(float p_nVolume);

    void stopAll();

	void checkWavePlayEnd();

    void pause();

    void resume();

public:

	/** @brief 添加资源
	 *  @return
	*/
    JCWaveInfo* AddWaveInfo( const std::string& p_sUrl,unsigned char* p_pBuffer,int p_nSize,const char* p_sFilePath,void* p_pExternalMark,bool p_bIsOgg );

	/** @brief 查找资源根据 ulid
	 *  @param[in] ulID
	 *  @return
	*/
	JCWaveInfo* FindWaveInfo( const std::string& p_sUrl );

	/** @brief 清空所有
	*/
	bool ClearAllWaveInfo( void );

    void autoGarbageCollection();

private:

	void releaseOpenAL( OpenALSourceInfo* pOpenALInfo );

    void createOpenALSource();

    OpenALSourceInfo* getOpenALSource();

public:

    static  int                     s_nGarbageCollectionTime;

	MapWaveInfo						m_vWaveInfos;				            //wave的信息

public:

    JCFileResManager*               m_pFileResManager;
	ALCdevice*						m_pDevice;								// 打开默认音频设备
    ALCcontext*						m_pContext;								// context
	std::vector<OpenALSourceInfo*>  m_pOpenALSource;	                    // 播放声音用的
    int                             m_nCurrentIndex;                        // 记录上次的位置
	bool							m_bStop;

};

}

#endif //__JCAudioWavPlayer_H__

//-----------------------------END FILE--------------------------------
