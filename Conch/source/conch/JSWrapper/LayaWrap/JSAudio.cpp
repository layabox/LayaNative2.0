/**
@file			JSAudio.cpp
@brief			
@author			James
@version		1.0
@date			2014_11_26
*/

//包含头文件
#include "JSAudio.h"
#include "../JSInterface/JSInterface.h"
#include <util/Log.h>
#include <util/JCCommonMethod.h>
#include <downloadMgr/JCDownloadMgr.h>
#include <util/JCMemorySurvey.h>
#include <downloadCache/JCServerFileCache.h>
#include <fileSystem/JCFileSystem.h>
#include <resource/JCFileResManager.h>
#include <util/JCLayaUrl.h>
#include <util/JCCommonMethod.h>
#include "JSFile.h"
#include "../../JCScriptRuntime.h"
#include <util/JCIThreadCmdMgr.h>
#include <functional>
#include "../../Audio/JCAudioManager.h"

namespace laya
{

	ADDJSCLSINFO(JSAudio, JSObjNode);
	//为了确保mp3只被写入文件一次
    std::map<std::string,std::string>	ms_vSaveMp3File;
    //------------------------------------------------------------------------------
    JSAudio::JSAudio()
    {
        m_nCurrentTime = 0;
	    m_bNeedHandlePlay = false;
	    m_nType = -1;
	    m_bAutoPlay = false;
	    m_bLoop = false;
	    m_bMuted = false;
	    m_nVolume = 1;
	    m_sSrc = "";
	    m_sLocalFileName = "";
	    m_bIsOgg = false;
	    m_bDownloaded = false;
        m_pOpenALInfo = NULL;
	    AdjustAmountOfExternalAllocatedMemory( 534 );
	    JCMemorySurvey::GetInstance()->newClass( "audio",534,this );
	    m_CallbackRef.reset(new int(1));
    }
    //------------------------------------------------------------------------------
    JSAudio::~JSAudio()
    {
        JCMemorySurvey::GetInstance()->releaseClass( "audio",this );
	    JCAudioManager::GetInstance()->delWav(this);
	    JCAudioManager::GetInstance()->delMp3Obj(this);
    }
    //------------------------------------------------------------------------------
    void JSAudio::addEventListener( const char* p_sName, JSValueAsParam p_pFunction )
    {
	    if( strcmp( p_sName,"ended" ) == 0 )
	    {
		    m_pJSFunctionAudioEnd.set(0,this,p_pFunction);
	    }
	    else if(strcmp(p_sName,"canplaythrough")==0)
	    {
		    m_pJSFunctionCanPlay.set(1,this,p_pFunction);
	    }
	    else if (strcmp(p_sName, "error") == 0)
	    {
		    m_pJSFunctionError.set(2, this, p_pFunction);
	    }
        else
        {
            LOGW("JSAudio::addEventListener(%s)尚未支持",p_sName );
        }
    }
    //------------------------------------------------------------------------------
    void JSAudio::setAutoPlay( bool p_bAutoPlay )
    {
	    m_bAutoPlay = p_bAutoPlay;
    }
    //------------------------------------------------------------------------------
    bool JSAudio::getAutoPlay()
    {
	    return m_bAutoPlay;
    }
    //------------------------------------------------------------------------------
    void JSAudio::setLoop( bool p_bLoop )
    {
	    m_bLoop = p_bLoop;
    }
    //------------------------------------------------------------------------------
    bool JSAudio::getLoop()
    {
	    return m_bLoop;
    }
    //------------------------------------------------------------------------------
    void JSAudio::setMuted( bool p_bMuted )
    {
	    m_bMuted = p_bMuted;
	    if( m_nType == 0 )
	    {
		    JCAudioManager::GetInstance()->setMp3Mute( m_bMuted );
	    }
	    else if( m_nType == 1 )
	    {
            if (m_pOpenALInfo && m_pOpenALInfo->m_pAudio == this)
            {
                JCAudioManager::GetInstance()->setWavVolume(m_pOpenALInfo, m_bMuted ? 0 : m_nVolume);
            }
	    }
    }
    //------------------------------------------------------------------------------
    bool JSAudio::getMuted()
    {
	    return m_bMuted;
    }
    //------------------------------------------------------------------------------
    void JSAudio::setSrc( const char* p_sSrc )
    {
	    //如果和原来播放的一样，直接播放就行了
        std::string sSrc = p_sSrc;
	    if( m_sSrc == sSrc )
	    {
            std::weak_ptr<int> cbref(m_CallbackRef);
            auto pFunction = std::bind(&JSAudio::onCanplayCallJSFunction,this, cbref);
            JCScriptRuntime::s_JSRT->m_pPoster->postToJS( pFunction );
		    if( m_bAutoPlay == true )
		    {
			    //直接播放
			    play();
		    }
		    return;
	    }
	    //这段是第一次播放此音乐，先下载走正规流程
	    m_sSrc = sSrc;
	    //为了android
	    m_sSrc.at(0) = m_sSrc.at(0);


		//去掉?后面的，因为可能增加版本号
		std::string sTemp = m_sSrc;
		int p3 = sTemp.rfind('?');
		//去下载文件
		char* sT = (char*)(sTemp.c_str());
		if (p3 != -1)
		{
			sT[p3] = 0;
		}
		char* sExtName = (char*)(getExtName(LayaStrlwr(sT)));
		if (strcmp(sExtName, "mp3") == 0)
		{
			m_nType = 0;
		}
		else if (strcmp(sExtName, "wav") == 0)
		{
			m_nType = 1;
			m_bIsOgg = false;
		}
		else if (strcmp(sExtName, "ogg") == 0)
		{
			m_nType = 1;
			m_bIsOgg = true;
		}
		else
		{
			m_nType = -1;
			LOGW("JSAudio::setSrc extname != mp3 && extname != wav && exname != ogg");
			return;
		}
	    //这段代码是查找，是否在缓存中已经有此音乐了，直接找到播放
	    JCWaveInfo* pWavInfo = JCAudioManager::GetInstance()->FindWaveInfo( p_sSrc );
	    if( pWavInfo != NULL )
	    {
		    m_nType = 1;
		    m_bDownloaded = true;
            std::weak_ptr<int> cbref(m_CallbackRef);
            std::function<void(void)> pFunction = std::bind(&JSAudio::onCanplayCallJSFunction,this, cbref);
            JCScriptRuntime::s_JSRT->m_pPoster->postToJS( pFunction );
		    if( m_bAutoPlay == true )
		    {
			    play();
		    }
		    return;
	    }
	    
	    if(m_nType==0)
	    {
		    //必须确保 如果是mp3必须每次都new JSAudio这个类，否则会出问题
		    //临时用的这种办法，现在不好，因为有二次写文件的事情。。
		    //TODO以后得修改
		    std::map<std::string,std::string>::iterator iter = ms_vSaveMp3File.find( m_sSrc );
		    if( iter == ms_vSaveMp3File.end() )
		    {
			    laya::JCFileRes* res = JCScriptRuntime::s_JSRT->m_pFileResMgr->getRes(m_sSrc);
			    std::weak_ptr<int> cbref(m_CallbackRef);
			    res->setOnReadyCB( std::bind(&JSAudio::onDownloaded,this, std::placeholders::_1,cbref));
			    res->setOnErrorCB(std::bind(&JSAudio::onDownloadErr, this, std::placeholders::_1, std::placeholders::_2, cbref));
		    }
		    else
		    {
			    m_sLocalFileName = iter->second;
			    if(fs::exists(m_sLocalFileName.c_str()))
                {
				    m_bDownloaded = true;
                    std::weak_ptr<int> cbref(m_CallbackRef);
                    auto pFunction = std::bind(&JSAudio::onCanplayCallJSFunction,this, cbref);
                    JCScriptRuntime::s_JSRT->m_pPoster->postToJS( pFunction );
				    if( m_nType == 0 )
				    {
					    if( m_bAutoPlay || m_bNeedHandlePlay == true )
					    {
						    m_bNeedHandlePlay = false;
						    play();
					    }
				    }
			    }
                else
                {
				    LOGW("JSAudio::setSrc music 当前文件不存在,%s",m_sLocalFileName.c_str());
				    ms_vSaveMp3File.erase(iter);
				    laya::JCFileRes* res = JCScriptRuntime::s_JSRT->m_pFileResMgr->getRes(m_sSrc);

				    std::weak_ptr<int> cbref(m_CallbackRef);
				    res->setOnReadyCB( std::bind(&JSAudio::onDownloaded,this, std::placeholders::_1,cbref));
				    res->setOnErrorCB(std::bind(&JSAudio::onDownloadErr, this, std::placeholders::_1, std::placeholders::_2, cbref));
			    }
		    }
	    }
	    else
	    {
		    laya::JCFileRes* res = JCScriptRuntime::s_JSRT->m_pFileResMgr->getRes(m_sSrc);
		    std::weak_ptr<int> cbref(m_CallbackRef);
		    res->setOnReadyCB( std::bind(&JSAudio::onDownloaded,this, std::placeholders::_1,cbref));
		    res->setOnErrorCB( std::bind(&JSAudio::onDownloadErr,this,std::placeholders::_1,std::placeholders::_2,cbref));
	    }
    }
    //------------------------------------------------------------------------------
    bool JSAudio::onDownloadErr(void* p_pRes, int p_nErrCode,std::weak_ptr<int> callbackref)
    {
	    if (!callbackref.lock())return false;
	    auto pFunction = std::bind(&JSAudio::onErrorCallJSFunction, this, p_nErrCode,callbackref);
	    JCScriptRuntime::s_JSRT->m_pPoster->postToJS(pFunction);
	    return true;
    }
    bool JSAudio::onDownloaded(void* p_pRes, std::weak_ptr<int> callbackref)
    {
	    if( !callbackref.lock() )return false;
	    laya::JCResStateDispatcher* pRes = (laya::JCResStateDispatcher*)p_pRes;
	    laya::JCFileRes* pFileRes = (laya::JCFileRes*)pRes;
	    if( pFileRes->m_pBuffer.get()==NULL || pFileRes->m_nLength==0 )
        {
		    return false;
	    }
	    JCBuffer p_buf;
	    p_buf.m_pPtr=pFileRes->m_pBuffer.get();
	    p_buf.m_nLen = pFileRes->m_nLength;
	    m_bDownloaded = true;
       // std::weak_ptr<int> cbref(m_CallbackRef);
        //std::function<void(void)> pFunction = std::bind(&JSAudio::onCanplayCallJSFunction,this, callbackref);
        auto pFunction = std::bind(&JSAudio::onCanplayCallJSFunction, this, callbackref);
	    JCScriptRuntime::s_JSRT->m_pPoster->postToJS( pFunction );
	    if( m_nType == 0 )
	    {
		    //必须确保 如果是mp3必须每次都new JSAudio这个类，否则会出问题
		    //临时用的这种办法，现在不好，因为有二次写文件的事情。。
		    //TODO以后得修改
		    std::map<std::string,std::string>::iterator iter = ms_vSaveMp3File.find( m_sSrc );
		    if( iter != ms_vSaveMp3File.end() )
		    {
			    m_sLocalFileName = iter->second;
		    }
		    else
		    {
			    int p1 = m_sSrc.rfind('/');
			    int p2 = m_sSrc.rfind('\\');
			    int pos = std::max<int>(p1,p2);
			    std::string audiofile = m_sSrc.substr(pos+1,m_sSrc.length());
			    //去掉?后面的，因为可能增加版本号
			    int p3 = audiofile.rfind('?');
			    char* sT = (char*)(audiofile.c_str());
			    if( p3 != -1 )
			    {
				    sT[p3]=0;
			    }
                unsigned int hash = JCCachedFileSys::hashRaw(m_sSrc.c_str());
                char tmpBuf[32];
                sprintf(tmpBuf, "%x_", hash);
			    m_sLocalFileName =JCScriptRuntime::s_JSRT->m_pFileResMgr->m_pFileCache->getAppPath()+"/"+ tmpBuf +audiofile;
			    writeFileSync(m_sLocalFileName.c_str(),p_buf);
			    ms_vSaveMp3File[m_sSrc] = m_sLocalFileName;
		    }
	    }
	    if( m_nType == 0 )
	    {
		    if( m_bAutoPlay || m_bNeedHandlePlay == true )
		    {
			    m_bNeedHandlePlay = false;
			    play();
		    }
	    }
	    else if( m_nType == 1 )
	    {
		    JCAudioManager::GetInstance()->AddWaveInfo( m_sSrc,p_buf,(int)(p_buf.m_nLen),this,m_bIsOgg );
		    if( m_bAutoPlay || m_bNeedHandlePlay == true )
		    {
			    m_bNeedHandlePlay = false;
			    play();
		    }
	    }
	    return true;
    }
    //------------------------------------------------------------------------------
    const char* JSAudio::getSrc()
    {
	    return m_sSrc.c_str();
    }
    //------------------------------------------------------------------------------
    void JSAudio::setVolume( float p_nVolume )
    {
        if (p_nVolume > 1)p_nVolume = 1;
        if (p_nVolume < 0)p_nVolume = 0;
	    m_nVolume = p_nVolume;
	    if( m_nType == 0 )
	    {
		    JCAudioManager::GetInstance()->setMp3Volume( m_nVolume );
	    }
	    else if( m_nType == 1 )
	    {
            if (m_pOpenALInfo && m_pOpenALInfo->m_pAudio == this)
            {
                JCAudioManager::GetInstance()->setWavVolume(m_pOpenALInfo,m_nVolume);
            }
	    }
    }
    //------------------------------------------------------------------------------
    float JSAudio::getVolume()
    {
	    return m_nVolume;
    }
    //------------------------------------------------------------------------------
    void JSAudio::play()
    {
        //if (m_bMuted)return;
	    if( m_bDownloaded == false )
	    {
		    m_bNeedHandlePlay = true;
		    return;
	    }
	    if( m_nType == 0 )
	    {
		    JCAudioManager::GetInstance()->stopMp3();
		    if( m_sLocalFileName.length() > 0 )
		    {
			    JCAudioManager::GetInstance()->playMp3( m_sLocalFileName.c_str(),m_bLoop?-1:0,(int)m_nCurrentTime,this );
		    }
		    else
		    {
			    JCAudioManager::GetInstance()->playMp3( m_sSrc.c_str(),m_bLoop?-1:0,(int)m_nCurrentTime,this );
		    }
	    }
	    else if( m_nType == 1 )
	    {
		    m_pOpenALInfo = JCAudioManager::GetInstance()->playWav( this,m_sSrc, m_bIsOgg);
            if (m_pOpenALInfo)
            {
                JCAudioManager::GetInstance()->setWavVolume(m_pOpenALInfo, m_nVolume);
            }
	    }
    }
    //------------------------------------------------------------------------------
    void JSAudio::pause()
    {
	    if( m_nType == 0 )
	    {
			m_nCurrentTime = JCAudioManager::GetInstance()->getCurrentTime();
		    JCAudioManager::GetInstance()->pauseMp3();
	    }
        else
        {
            if (m_pOpenALInfo && m_pOpenALInfo->m_pAudio == this)
            {
                JCAudioManager::GetInstance()->stopWav(m_pOpenALInfo);
                m_pOpenALInfo = NULL;
            }
        }
    }
    //------------------------------------------------------------------------------
    void JSAudio::stop()
    {
	    if( m_nType == 0 )
	    {
		    JCAudioManager::GetInstance()->stopMp3();
	    }
        else
        {
            if (m_pOpenALInfo && m_pOpenALInfo->m_pAudio == this)
            {
                JCAudioManager::GetInstance()->stopWav(m_pOpenALInfo);
                m_pOpenALInfo = NULL;
            }
        }
    }
    void JSAudio::setCurrentTime(double nCurrentTime)
	{
		m_nCurrentTime = nCurrentTime;
		if( m_nType == 0 )
		{
			//JCAudioManager::GetInstance()->setCurrentTime(nCurrentTime);
		}
		else
		{
		}
	}
	double JSAudio::getCurrentTime()
	{
		if( m_nType == 0 )
		{
			return JCAudioManager::GetInstance()->getCurrentTime();
		}
		else
		{
			return 0.0f;
		}
	}
	double JSAudio::getDuration()
	{
		if( m_nType == 0 )
		{
			return JCAudioManager::GetInstance()->getDuration();
		}
		else
		{
			return 0.0f;
		}
	}
    //------------------------------------------------------------------------------
    void JSAudio::onPlayEnd()
    {
	    std::weak_ptr<int> cbref(m_CallbackRef);
	    auto pFunction = std::bind(&JSAudio::onPlayEndCallJSFunction,this, cbref);
	    JCScriptRuntime::s_JSRT->m_pPoster->postToJS( pFunction );
    }
    //------------------------------------------------------------------------------
    void JSAudio::onPlayEndCallJSFunction( std::weak_ptr<int> callbackref)
    {
	    if( !callbackref.lock())return;
	    m_pJSFunctionAudioEnd.Call();
    }
    //------------------------------------------------------------------------------
    void JSAudio::onCanplayCallJSFunction( std::weak_ptr<int> callbackref)
    {
        if( !callbackref.lock())
            return;
        m_pJSFunctionCanPlay.Call();
    }
    void JSAudio::onErrorCallJSFunction(int p_nErrorCode,std::weak_ptr<int> callbackref)
    {
	    if (!callbackref.lock())return;
	    m_pJSFunctionError.Call(p_nErrorCode);
    }
    void JSAudio::exportJS() 
    {
        JSP_CLASS("ConchAudio",JSAudio);
        JSP_ADD_PROPERTY(autoplay, JSAudio, getAutoPlay, setAutoPlay);
        JSP_ADD_PROPERTY(loop, JSAudio, getLoop, setLoop);
        JSP_ADD_PROPERTY(muted, JSAudio, getMuted, setMuted);
        JSP_ADD_PROPERTY(src, JSAudio, getSrc, setSrc);
        JSP_ADD_PROPERTY(volume, JSAudio, getVolume, setVolume);
        JSP_ADD_PROPERTY(currentTime, JSAudio, getCurrentTime, setCurrentTime);
	    JSP_ADD_METHOD("setLoop", JSAudio::setLoop);
	    JSP_ADD_METHOD("play", JSAudio::play);
	    JSP_ADD_METHOD("pause", JSAudio::pause);
	    JSP_ADD_METHOD("stop", JSAudio::stop);
	    JSP_ADD_METHOD("addEventListener", JSAudio::addEventListener);
		JSP_ADD_PROPERTY_RO(duration, JSAudio, getDuration)
	    JSP_INSTALL_CLASS("ConchAudio", JSAudio);
    }
}
//------------------------------------------------------------------------------

//-----------------------------END FILE--------------------------------
