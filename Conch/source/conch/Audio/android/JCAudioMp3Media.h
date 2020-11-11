/**
@file			JCAudioMp3Play.h
@brief			
@author			dt
@version		1.0
@date			2014_12_24
*/

#ifndef __JCAudioMp3Media_H__
#define __JCAudioMp3Media_H__

//包含头文件
#include <stdio.h>
#include <string>
#include <string.h>
#include "resource/Audio/JCMp3Interface.h"

namespace laya
{
/** 
 * @brief 
*/
class JCAudioMp3Media : public JCMp3Interface
{
public:

	//构造函数
	JCAudioMp3Media();

	//析构函数
	~JCAudioMp3Media( void );

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
