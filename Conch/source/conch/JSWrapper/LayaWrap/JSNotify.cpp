/**
@file			JSNotify.cpp
@brief			
@author			James
@version		1.0
@date			2015_9_26
*/

//包含头文件
#include "JSNotify.h"
#include "util/Log.h"
#include "util/JCMemorySurvey.h"
#ifdef ANDROID
	#include <jni.h>
	#include "../../CToJavaBridge.h"
#elif __APPLE__
    #include "../../CToObjectC.h"
#endif


namespace laya 
{
	ADDJSCLSINFO(JSNotify, JSObjNode);
    JSNotify* JSNotify::ms_pNotify = NULL;
    //------------------------------------------------------------------------------
    JSNotify* JSNotify::getInstance()
    {
        if( ms_pNotify == NULL )
        {
            ms_pNotify = new JSNotify();
        }
        return ms_pNotify;
    }
    //------------------------------------------------------------------------------
    JSNotify::JSNotify()
    {
    
    }
    //------------------------------------------------------------------------------
    JSNotify::~JSNotify()
    {
        ms_pNotify = NULL;
    }
    //------------------------------------------------------------------------------
    void JSNotify::setRepeatNotify( int p_nID,int p_nStartTime,int p_nRepeatType,const char* p_sTickerText,const char* p_sTitleText,const char* p_sDesc )
    {
        LOGI("JSNotify::setRepeatNotify id=%d,startTime=%ld,type=%d,tickerText=%s,titleText=%s,desc=%s",p_nID,p_nStartTime,p_nRepeatType,p_sTickerText,p_sTitleText,p_sDesc );
        #ifdef ANDROID
            std::vector<intptr_t> params;
            params.push_back(p_nID);
            params.push_back((long)p_nStartTime);
            params.push_back(p_nRepeatType);
            params.push_back((long)p_sTickerText);
            params.push_back((long)p_sTitleText);
            params.push_back((long)p_sDesc);
            CToJavaBridge::JavaRet kRet;
            CToJavaBridge::GetInstance()->callMethod("laya.game.Notifycation.LayaNotifyManager", "setRepeatingNotify", p_nID, p_nStartTime,p_nRepeatType,p_sTickerText, p_sTitleText, p_sDesc, kRet);
        #elif __APPLE__
            CToObjectCSetRepeatNotify( p_nID,p_nStartTime,p_nRepeatType,p_sTickerText,p_sTickerText,p_sDesc );
        #elif WIN32
            
        #endif
    }
    //------------------------------------------------------------------------------
    void JSNotify::setOnceNotify( int p_nID,int p_nStartTime,const char* p_sTickerText,const char* p_sTitleText,const char* p_sDesc )
    {
        LOGI("JSNotify::setOnceNotify id=%d,startTime=%ld,tickerText=%s,titleText=%s,desc=%s",p_nID,p_nStartTime,p_sTickerText,p_sTitleText,p_sDesc );
        #ifdef ANDROID
            std::vector<intptr_t> params;
            params.push_back(p_nID);
            params.push_back((long)p_nStartTime);
            params.push_back((long)p_sTickerText);
            params.push_back((long)p_sTitleText);
            params.push_back((long)p_sDesc);
            CToJavaBridge::JavaRet kRet;
            CToJavaBridge::GetInstance()->callMethod("layaair.game.Notifycation.LayaNotifyManager", "setOnceNotify", p_nID,p_nStartTime,p_sTickerText,p_sTitleText,p_sDesc,kRet);
        #elif __APPLE__
            CToObjectCSetOnceNotify( p_nID,p_nStartTime,p_sTickerText,p_sTitleText,p_sDesc );
        #elif WIN32
            
        #endif
    }
    //------------------------------------------------------------------------------
    void JSNotify::deleteOnceNotify( int p_nID )
    {
        LOGI("JSNotify::deleteOnceNotify id=%d",p_nID );
        #ifdef ANDROID
            CToJavaBridge::JavaRet kRet;
            CToJavaBridge::GetInstance()->callMethod("layaair.game.Notifycation.LayaNotifyManager", "removeNotify", p_nID, kRet);
        #elif __APPLE__
            CToObjectCDeleteOnceNotify( p_nID );
        #elif WIN32
            
        #endif
    }
    //------------------------------------------------------------------------------
    void JSNotify::deleteAllNotify()
    {
        LOGI("JSNotify::deleteAllNotify" );
        #ifdef ANDROID
            CToJavaBridge::JavaRet kRet;
            CToJavaBridge::GetInstance()->callMethod("layaair.game.Notifycation.LayaNotifyManager", "removeAllNotify", kRet);
        #elif __APPLE__
            CToObjectCDeleteAllNotify();
        #elif WIN32
            
        #endif
    }
    //------------------------------------------------------------------------------
    void JSNotify::exportJS() 
    {
        JSP_GLOBAL_CLASS("conchNotify", JSNotify, JSNotify::getInstance());
		JSP_GLOBAL_ADD_METHOD("setRepeatNotify", JSNotify::setRepeatNotify);
		JSP_GLOBAL_ADD_METHOD("setOnceNotify", JSNotify::setOnceNotify);
		JSP_GLOBAL_ADD_METHOD("deleteOnceNotify", JSNotify::deleteOnceNotify);
		JSP_GLOBAL_ADD_METHOD("deleteAllNotify", JSNotify::deleteAllNotify);
	    JSP_INSTALL_GLOBAL_CLASS("conchNotify", JSNotify, JSNotify::getInstance());
    }
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//-----------------------------END FILE--------------------------------
