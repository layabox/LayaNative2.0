/**
@file			JCAudioMp3Play.h
@brief			
@author			wyw
@version		1.0
@date			2012_11_14
*/

#ifndef __JCAudioMp3Player_H__
#define __JCAudioMp3Player_H__

//包含头文件
#include <stdio.h>
#include <string>
#include <string.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <sys/types.h>

namespace laya
{
/** 
 * @brief 
*/
class JCAudioMp3Player
{
public:

	//构造函数
	JCAudioMp3Player( const char* p_sFilePath );

	//析构函数
	~JCAudioMp3Player( void );

	//释放资源
	void shutdown( void );

	//创建player
	bool createAudioPlayer( std::string p_sUrl );

	//设置播放
	bool setPlayingAudioPlayer( short p_nState );

	//设置是否循环
	bool setLoopingAudioPlayer( bool p_bLoop );

	//设置左右声道的静音
	bool setChannelMuteAudioPlayer( int p_nChannel,bool p_nMute );

	//设置左右声道是否独唱
	bool setChannelSoloAudioPlayer( int p_nChannel,bool p_nSolo );

	//设置音量
	bool setVolumeAudioPlayer( int p_nVolume );

	//设置静音
	bool setMute( bool p_bMute );

protected:

	bool			m_bInit;					//是否初始化

	std::string		m_sUrlName;					//url

	SLObjectItf 	m_pPlayerObject;			//播放声音的object

	SLPlayItf 		m_pPlayerPlay;				//播放用的

	SLSeekItf 		m_pPlayerSeek;				//设置是否循环用的

	SLMuteSoloItf 	m_pPlayerMuteSolo;			//设置静音 独唱等用的

	SLVolumeItf 	m_pPlayerVolume;			//设置音量用的

	int				m_nCurrentVolume;			//当前音量，为了解决三星设置静音不好用的问题

};

}

#endif //__JCAudioMp3Play_H__

//-----------------------------END FILE--------------------------------
