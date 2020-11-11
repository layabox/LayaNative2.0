/**
@file			JCAudioMp3Player.cpp
@brief
@author			James
@version		1.0
@date			2014_11_26
*/

#include "JCAudioMp3Player.h"
#include "util/JCCommonMethod.h"

//------------------------------------------------------------------------------
namespace laya
{
    //------------------------------------------------------------------------------
    JCAudioMp3Player::JCAudioMp3Player()
    {
	    m_pGraphBuilder = NULL;
	    m_pMediaControl = NULL;
	    m_pMediaEvent = NULL;
	    m_pMediaPos = NULL;
	    m_pBasicAudio = NULL;
	    m_pMediaSeeking = NULL;
	    m_nPlayTimes = -1;
	    m_sUrl = "";
	    m_nPlayCount = 0;
	    m_fPos = 0;
	    m_nNotifyThreadID = 0;
	    m_fCurrentVolume = 1;
    }
    //------------------------------------------------------------------------------
    JCAudioMp3Player::~JCAudioMp3Player()
    {
	    release();
    }
    //------------------------------------------------------------------------------
    DWORD WINAPI handleMp3ListenerThread( void* p_pParam  )
    {
	    JCAudioMp3Player* pThis =(JCAudioMp3Player*)p_pParam;
	    if( pThis->m_nPlayTimes == 1 ) return -1;
	    MSG msg;
	    while( true )
	    {
		    PeekMessage( &msg, NULL, 0, 0, PM_REMOVE );
		    if( msg.message == WM_QUIT ) 
		    {
			    return 0;
		    }
		    long nEventCode, param1, param2;
            if( SUCCEEDED( pThis->m_pMediaEvent->GetEvent( &nEventCode, &param1, &param2, 1 ) ) )
            {
			    if( pThis->m_nPlayTimes == -1 )
			    {
				    if( nEventCode == EC_COMPLETE )
				    {
					    pThis->m_pMediaPos-> put_CurrentPosition(0);
				    }
			    }
			    else if( pThis->m_nPlayTimes > 1 )
			    {
				    if(nEventCode == EC_COMPLETE)
				    {
					    pThis->m_nPlayCount++;
					    if( pThis->m_nPlayCount >= pThis->m_nPlayTimes )
					    {
						    return 0;
					    }
					    pThis->m_pMediaPos-> put_CurrentPosition(0);
				    } 	
			    }
            }
		    pThis->m_pMediaEvent->FreeEventParams( nEventCode, param1, param2 );
		    Sleep( 500 );
	    }
	    return 0;
    }
    //------------------------------------------------------------------------------
    bool JCAudioMp3Player::Mp3Init()
    {
	    CoInitialize(0);
	    if( FAILED( CoCreateInstance( CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder,  (void**)&m_pGraphBuilder) ) )
	    {
		    return FALSE;
	    }
	    m_pGraphBuilder->QueryInterface(IID_IMediaControl, (void**)&m_pMediaControl);
	    m_pGraphBuilder->QueryInterface(IID_IMediaEvent, (void**)&m_pMediaEvent);
	    m_pGraphBuilder->QueryInterface(IID_IMediaPosition, (void**)&m_pMediaPos);
	    m_pGraphBuilder->QueryInterface(IID_IBasicAudio,(void **)&m_pBasicAudio);
	    m_pGraphBuilder->QueryInterface(IID_IMediaSeeking,(void **)&m_pMediaSeeking);
	    return true;
	    if( m_nNotifyThreadID == 0 )
	    {
		    //创建一个线程接收notify消息
		    m_hNotifyThread = CreateThread( NULL, 0, handleMp3ListenerThread, this, 0, &m_nNotifyThreadID ); 
	    }
	    return true;
    }
    //------------------------------------------------------------------------------
    void JCAudioMp3Player::play( const char* p_sUrl,int p_nTimes, float nCurrentTime,JCAudioInterface* p_pJSAudio )
    {
	    m_sUrl = p_sUrl;
	    Mp3Init();
	    m_nPlayCount = 0;
	    m_nPlayTimes = p_nTimes;
	    WCHAR sWfilename[MAX_PATH] = {0};
	    mbstowcs( sWfilename, p_sUrl, MAX_PATH );
	    m_pMediaControl->RenderFile( sWfilename );
	    m_pMediaControl->Run();
    }

    void JCAudioMp3Player::delAudio( JCAudioInterface* p_pJSAudio ){
    }
    //------------------------------------------------------------------------------
    void JCAudioMp3Player::pause()
    {
	    if( m_pMediaControl != NULL )
	    {
		    m_pMediaPos->get_CurrentPosition( &m_fPos );
		    m_pMediaControl->Pause();
	    }
    }
    //------------------------------------------------------------------------------
    void JCAudioMp3Player::resume()
    {
	    if( m_pMediaControl != NULL )
	    {
		    m_pMediaPos->get_CurrentPosition( &m_fPos );
		    m_pMediaControl->Run();
	    }
    }
    //------------------------------------------------------------------------------
    void JCAudioMp3Player::stop()
    {
	    if( m_pMediaControl != NULL )
	    {
		    m_pMediaControl->Stop();
	    }
    }
    //------------------------------------------------------------------------------
    void JCAudioMp3Player::setVolume( float p_nVolume )
    {
        p_nVolume=p_nVolume*10000.0f-10000.0f;
	    m_fCurrentVolume =  p_nVolume;
	    if( m_pBasicAudio != NULL )
	    {
		    m_pBasicAudio->put_Volume( (long)p_nVolume );
	    }
    }
    //------------------------------------------------------------------------------
    void JCAudioMp3Player::setMute(bool p_bMute)
    {
	    if( m_pBasicAudio != NULL )
	    {
		    m_pBasicAudio->put_Volume( (long)(p_bMute?-10000:m_fCurrentVolume) );
	    }
    }
    //------------------------------------------------------------------------------
    void JCAudioMp3Player::releaseThread()
    {
	    if( m_nNotifyThreadID != 0 )
	    {
		    PostThreadMessage( m_nNotifyThreadID, WM_QUIT, 0, 0 );
		    WaitForSingleObject( m_hNotifyThread, INFINITE );
		    CloseHandle( m_hNotifyThread );
	    }
    }
    //------------------------------------------------------------------------------
    void JCAudioMp3Player::release()
    {
	    releaseThread();
	    m_fPos = 0;
	    m_nPlayCount = 0;
	    if( m_pMediaControl )
	    {
		    m_pMediaControl->Stop();
	    }
	    if( m_pMediaEvent )
	    {
		    m_pMediaEvent->Release();
		    m_pMediaEvent = NULL;
	    }
	    if( m_pMediaControl )
	    {
		    m_pMediaControl->Release();
		    m_pMediaControl = NULL;
	    }
	    if( m_pMediaPos )
	    {
		    m_pMediaPos->Release();
		    m_pMediaPos = NULL;
	    }
	    if( m_pBasicAudio )
	    {
		    m_pBasicAudio->Release();
		    m_pBasicAudio = NULL;
	    }
	    if( m_pGraphBuilder )
	    {
		    m_pGraphBuilder->Release();
		    m_pGraphBuilder = NULL;
	    }
	    CoUninitialize();
    }
    //------------------------------------------------------------------------------
    void JCAudioMp3Player::onPlayEnd()
    {
    }
	void JCAudioMp3Player::setCurrentTime(double nCurrentTime)
    {

    }

    double JCAudioMp3Player::getCurrentTime()
    {
        return 0.0f;
    }

    double JCAudioMp3Player::getDuration()
    {
        return 0.0f;
    }
    //------------------------------------------------------------------------------
}

//-----------------------------END FILE--------------------------------
