/**
@file			JCMp3Interface.h
@brief			
@author			James
@version		1.0
@date			2014_11_26
*/

#ifndef __JCMp3Interface_H__
#define __JCMp3Interface_H__
#include "JCAudioInterface.h"
#include <stdio.h>

/** 
 * @brief JCMp3Interface
 * 纯接口类
*/
namespace laya
{
class JCMp3Interface
{
public:

	virtual void play( const char* p_sUrl,int p_nTimes,float nCurrentTime,JCAudioInterface* p_pJSAudio )=0;

	virtual void delAudio( JCAudioInterface* p_pJSAudio )=0;

	virtual void pause()=0;

	virtual void stop()=0;

	virtual void resume()=0;

	virtual void setVolume( float p_nVolume )=0;

	virtual void setMute(bool p_bMute)=0;

	virtual void onPlayEnd()=0;
	
	virtual void setCurrentTime(double nCurrentTime) = 0;
    
    virtual double getCurrentTime() = 0;
    
    virtual double getDuration() = 0;
};
}

#endif //__JCMp3Interface_H__

//-----------------------------END FILE--------------------------------