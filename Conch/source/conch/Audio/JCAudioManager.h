/**
@file			JCAudioManager.h
@brief			
@author			James
@version		1.0
@date			2014_11_26
*/

#ifndef __JCAudioManager_H__
#define __JCAudioManager_H__

//包含头文件
#ifdef ANDROID
	#include "android/JCAudioMp3Media.h"
#elif WIN32
	#include "windows/JCAudioMp3Player.h"	
#elif __APPLE__
    #include "ios/JCAudioMp3Player.h"
#elif OHOS
    #include "ohos/JCAudioMp3Player.h"
#endif

#include "resource/Audio/JCAudioWavPlayer.h"
#include <map>
#include <vector>
#include "resource/Audio/JCWaveInfo.h"
#include "buffer/JCBuffer.h"
#include "resource/Audio/JCMp3Interface.h"
#include <mutex>
namespace laya
{
    /** 
     * @brief 
    */
    class JCAudioManager
    {
    public:

	    /** @brief构造函数
	    */
	    JCAudioManager( JCFileResManager* pFileResManger );

	    /** @brief析构函数
	    */
	    ~JCAudioManager( void );

	    /*
	     * getInstace()
	    */
	    static JCAudioManager* GetInstance( void );
	    static void ClearAllWork();
	    static void DelInstance( void );

    public:

	    //以下是mp3的接口

	    void createMp3player();
	    //设置全局的静音
	    void setMp3Mute( bool p_bMute );

	    //获得全局的静音
	    bool getMp3Mute( void );

        //获得MP3是不是停止播放了
        bool getMp3Stopped();

	    //全局音量
	    void setMp3Volume( float p_nVolume );

	    //获得音量
	    float getMp3Volume( void );

	    void playMp3( const char* p_sUrl,int p_nTimes,int nCurrentTime,JCAudioInterface* p_pJSAudio );
	    void delMp3Obj(JCAudioInterface* p_pJSAudio);	//如果js对象删除了，就要调用这个

	    void pauseMp3();

	    void stopMp3();
	
	    void resumeMp3();
		
		void setCurrentTime(double nCurrentTime);
    
		double getCurrentTime();
    
		double getDuration();

    public:

	    /** @brief 清空所有
	    */
	    bool ClearAllAudioBufferPlay( void );

        /** @brief 播放声音
         *  @param[in] 声音interface
        */
        OpenALSourceInfo* playWav(JCAudioInterface* p_pAudio, const std::string& p_sUrl,bool bIsOgg);

	    void setWavVolume(OpenALSourceInfo* pOpenALInfo,float nVolume );

        void stopWav(OpenALSourceInfo* pOpenALInfo );

        /** @brief 删除wav
         *  @param[in] 声音的interface
        */
        void delWav(JCAudioInterface* p_pAudio);


        void stopAllWav();


        void setAllWavVolume( float nVolume );

	    /** @brief 添加资源
	     *  @return
	    */
	    JCWaveInfo* AddWaveInfo( const std::string& p_sUrl,unsigned char* p_pBuffer,int p_nSize,const char* p_sFilePath,void* p_pExternalMark,bool p_bIsOgg );

        JCWaveInfo* AddWaveInfo( const std::string& p_sUrl,JCBuffer& p_pBuffer,int p_nSize,void* p_pExternalMark,bool p_bIsOgg );

	    JCWaveInfo* FindWaveInfo( const std::string& p_sUrl );

	    void update();

    public:

	    JCMp3Interface*					m_pMp3Player;

	    JCAudioWavPlayer*				m_pWavPlayer;				//播放wav

    protected:

	    bool							m_bMuteMp3;					//MP3是否静音

	    float							m_nVolumeMp3;				//MP3的音量

        bool                            m_bStopMp3;                 //是否停止了mp3的播放

    protected:

	    static	JCAudioManager*			m_sAudioManager;			//静态的this指针
        static  std::mutex              m_mutex;
    };

}

#endif //__JCAudioManager_H__

//-----------------------------END FILE--------------------------------
