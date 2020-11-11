/**
@file			JCAudioMp3Player.h
@brief			
@author			wyw
@version		1.0
@date			2015_6_2
*/

#ifndef __JCAudioMp3Player_H__
#define __JCAudioMp3Player_H__

//包含头文件
#include <stdio.h>
#include <string>
#include "CToObjectC.h"
#include "resource/Audio/JCMp3Interface.h"

namespace laya
{
/** 
 * @brief 
*/
class JCAudioMp3Player : public JCMp3Interface
{
public:

	//构造函数
	JCAudioMp3Player();

	//析构函数
	~JCAudioMp3Player( void );

	void play( const char* p_sUrl,int p_nTimes,float nCurrentTime,JCAudioInterface* p_pJSAudio );

	void delAudio( JCAudioInterface* p_pJSAudio );
	
	void pause();
	
	void stop();

	void resume();
	
	void setVolume( float p_nVolume );

	void setMute( bool p_bMute );

	void onPlayEnd();

	void setCurrentTime(double nCurrentTime);
    
    double getCurrentTime();
    
    double getDuration();
	
private:

	JCAudioInterface* m_pJSAudio;

	float m_nCurrentVolume;
};

}

#endif //__JCAudioMp3Play_H__

//-----------------------------END FILE--------------------------------
