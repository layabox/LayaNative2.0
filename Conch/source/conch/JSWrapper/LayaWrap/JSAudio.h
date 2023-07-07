/**
@file			JSAudio.h
@brief			
@author			James
@version		1.0
@date			2014_11_26
*/

#ifndef __JSAudio_H__
#define __JSAudio_H__

//包含头文件
#include <stdio.h>
#include "../JSInterface/JSInterface.h"
#include "resource/Audio/JCAudioInterface.h"
#include "buffer/JCBuffer.h"


/** 
 * @brief 
*/
namespace laya 
{
    class OpenALSourceInfo;
    class JSAudio : public JsObjBase, public JSObjNode,JCAudioInterface
    {
    public:
	    static JsObjClassInfo JSCLSINFO;
	    static void exportJS();

	    JSAudio();

	    ~JSAudio();

    public:

	    void setAutoPlay( bool p_bAutoPlay );

	    bool getAutoPlay();

	    void setLoop( bool p_bLoop );

	    bool getLoop();

	    void setMuted( bool p_bMuted );

	    bool getMuted();

	    void setSrc( const char* p_sSrc );

	    const char* getSrc();

	    void setVolume( float p_nVolume );

	    float getVolume();

        void setCurrentTime(double nCurrentTime);

		double getCurrentTime();
    
		double getDuration();

    public:

	    void addEventListener( const char* p_sName, JSValueAsParam p_pFunction );

	    void play();

	    void pause();

	    void stop();

	    //下载线程的回调
	    void onDownloaded( void* p_pRes, std::weak_ptr<int> callbackref );

	    void onDownloadErr(void* p_pRes, int p_nErrCode, std::weak_ptr<int> callbackref);

    public:

	    void onPlayEnd();

        void onCanplayCallJSFunction(std::weak_ptr<int> callbackref);

	    void onPlayEndCallJSFunction(std::weak_ptr<int> callbackref);

	    void onErrorCallJSFunction(int p_nErrCode,std::weak_ptr<int> callbackref);

    public:

        bool			        m_bDownloaded;			//是否下载完成	

        int				        m_nType;				//类型  -1为无效值， 0为mp3  1为wav	

        bool			        m_bIsOgg;				//是否为ogg

        bool			        m_bAutoPlay;			//是否自动播放

        bool			        m_bLoop;				//是否循环

        bool			        m_bMuted;				//是否静音

        float                   m_nCurrentTime;         //播放到的时间

        std::string		        m_sSrc;					//src

        float                   m_nVolume;				//音量

        std::string		        m_sLocalFileName;

	    std::shared_ptr<int>	m_CallbackRef;

    public:

        JsObjHandle		    m_pJSFunctionAudioEnd;	//JS的回调
        JsObjHandle         m_pJSFunctionCanPlay;   //JS的回调
        JsObjHandle         m_pJSFunctionError;     //JS的回调
        OpenALSourceInfo*   m_pOpenALInfo;          //openAL的指针

    private:

        bool			        m_bNeedHandlePlay;

    };
}

#endif //__JSAudio_H__

//-----------------------------END FILE--------------------------------
