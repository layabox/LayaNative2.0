/**
@file			JNIFun.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_19
*/

#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef ANDROID
    #include <sys/syscall.h>
    #define gettidv1() syscall(__NR_gettid)
    #define gettidv2() syscall(SYS_gettid)
#endif
#include <downloadCache/JCAndroidFileSource.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/bitmap.h>
#include <downloadMgr/JCDownloadMgr.h>
#include <util/Log.h>
#include "JCConch.h"
//#include "JSWrapper/LayaWrap/JSMarket.h"
#include "CToJavaBridge.h"
#include "Audio/JCAudioManager.h"
#include "JSWrapper/LayaWrap/JSInput.h"
#include "util/JCZipFile.h"
#include "JCSystemConfig.h"
#include "JSWrapper/LayaWrap/JSGlobalExportCFun.h"
#include "JCConchRender.h"
#include "JCScriptRuntime.h"
#include "JSWrapper/LayaWrap/Video/JSVideo.h"
#include <imageLib/JCImageRW.h>

using namespace laya;
extern int g_nInnerWidth;
extern int g_nInnerHeight;
extern bool g_bGLCanvasSizeChanged;
extern std::string gRedistPath;
//------------------------------------------------------------------------------
bool g_bEngineInited = false;
std::mutex g_kReadyLock;
laya::JCConch* g_pConch = NULL;
extern AAssetManager* g_pAssetManager;
extern std::string gAssetRootPath;
extern std::string gAPKExpansionMainPath;
extern std::string gAPKExpansionPatchPath;
bool g_bInBKGround=false;
int64_t g_nInitTime = 0;
//------------------------------------------------------------------------------
extern "C"
{
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_configSetParamExt(JNIEnv * env, jobject obj,jstring p_strParamExt);//extparam
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_configSetURL(JNIEnv * env, jobject obj,jstring p_strUrl);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_configSetIsPlug(JNIEnv * env, jobject obj, jboolean p_bIsPlug);
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_InitDLib(JNIEnv * env, jobject obj,jobject assetManager,jint nDownloadThreadNum,jstring p_strAssetRootPath,jstring p_strCachePath, jstring p_strAPKExpansionMainPath, jstring p_strAPKExpansionPatchPath,int threadMode,int debugMode,int debugPort);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_SetLocalStoragePath(JNIEnv * env, jobject obj,jstring p_strLocalStorage );
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleTouch(JNIEnv * env, jobject obj,jint type,jint id,jint x,jint y );
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleKeyEvent(JNIEnv * env, jobject obj,jint keyCode,jint actionType);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleJoystickEvent(JNIEnv * env, jobject obj,float THUMBL_xOffset,float THUMBL_yOffset,float THUMBR_xOffset,float THUMBR_yOffset,float LT_Offset,float RT_Offset);
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleDeviceMotionEvent(JNIEnv * env, jobject obj, float ax, float ay, float az, float agx, float agy, float agz, float ra, float rb, float rg, float interval);
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleDeviceOrientationEvent(JNIEnv * env, jobject obj, float ra, float rb, float rg);
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_ReleaseDLib(JNIEnv * env, jobject obj );
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_OnAppDestroy(JNIEnv * env, jobject obj );
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_OnAppPause(JNIEnv * env, jobject obj );
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_OnGLReady(JNIEnv * env, jobject obj, int width,int height );
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_OnAppResume(JNIEnv * env, jobject obj );
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onDrawFrame(JNIEnv * env, jobject obj );
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onVSyncCallback(JNIEnv * env, jobject obj, jlong vsynctm);
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_exportStaticMethodToC(JNIEnv * env, jobject obj, jstring packcls);
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_closeExternalWebView(JNIEnv * env, jobject obj );
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_alertCallback(JNIEnv * env, jobject obj );
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onSensorChanged(JNIEnv * env, jobject obj,float arc );
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_reloadJS(JNIEnv * env, jobject obj );
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onRunCmd(JNIEnv* env, jobject obj, jint cmd, jint nParam1, jint nParam2 );
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_RunJS(JNIEnv* env, jobject obj, jstring jsstr );
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_postMsgToRuntime(JNIEnv* env, jobject obj, jstring msg, jstring params);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onBuyPropsCallback( JNIEnv * env, jobject obj,jstring p_sDesc );
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onInviteCallback( JNIEnv * env, jobject obj,jstring p_sJsonParam );
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_exitToPlatform( JNIEnv * env, jobject obj );
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_audioMusicPlayEnd( JNIEnv * env, jobject obj );
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_networkChanged(JNIEnv* env, jobject obj, jint nNetworkType );
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_loginCallback(JNIEnv* env, jobject obj, jstring p_sJsonParam);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_authorizeCallback(JNIEnv* env, jobject obj, jstring p_sJsonParam);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_refreshTokenCallback(JNIEnv* env, jobject obj, jstring p_sJsonParam);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_rechargeEvent(JNIEnv* env, jobject obj, jstring p_sJsonParam );
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_inputChange(JNIEnv* env, jobject obj, jint keycode );
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onShareAndFeed( JNIEnv * env, jobject obj,jstring p_sJsonParam );
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onGetGameFriends( JNIEnv * env, jobject obj,jstring p_sJsonParam);
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onSendToDesktop( JNIEnv * env, jobject obj,jstring p_sJsonParam);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onLogout( JNIEnv * env, jobject obj,jstring p_sJsonParam);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onMarketInit( JNIEnv * env, jobject obj,jstring p_sJsonParam );
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onTopicCircle( JNIEnv * env, jobject obj,jstring p_sJsonParam );
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onSwitchUserCallback( JNIEnv * env, jobject obj,jstring p_sJsonParam );
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onEnterPlatformCallback( JNIEnv * env, jobject obj,jstring p_sJsonParam );
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onEnterBBSCallback( JNIEnv * env, jobject obj,jstring p_sJsonParam );	
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onEnterFeedbackCallback( JNIEnv * env, jobject obj,jstring p_sJsonParam );	
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onEnterAccountMgrCallback( JNIEnv * env, jobject obj,jstring p_sJsonParam );	
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onSetRechargeInfoCallback( JNIEnv * env, jobject obj,jstring p_sJsonParam );	
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onSendMessageToPlatformCallback( JNIEnv * env, jobject obj,jstring p_sJsonParam );
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onGetUserInfoCallback(JNIEnv * env, jobject obj, jstring p_sJsonParam);
	JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onGetAvailableLoginTypeCallback(JNIEnv * env, jobject obj, jstring p_sJsonParam);
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_setLocalizable(JNIEnv * env, jobject obj, jboolean p_bIsLocalPackage);
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_callConchJSFunction(JNIEnv* env, jobject obj, jstring sFunctionName,jstring sJsonParam,jstring sCallbackFunction);
    JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_captureScreenCallBack(JNIEnv* env, jobject obj, jint w, jint h, jbyteArray byteArray);
	JNIEXPORT jboolean JNICALL Java_layaair_game_browser_ConchJNI_onBackPressed(JNIEnv* env, jobject obj);

    JNIEXPORT void JNICALL Java_layaair_game_browser_LayaVideoPlayer_emit(JNIEnv* env, jobject obj, jlong ptr, jstring str);
	JNIEXPORT void JNICALL Java_layaair_game_browser_LayaVideoPlayer_transferBitmap(JNIEnv* env, jobject obj, jobject bitmap, jlong dataPtr);
};
void postCmdToMainThread(int p_nCmd, int p_nParam1, int p_nParam2)
{
	CToJavaBridge::JavaRet ret;
    char buffer[60];
    sprintf(buffer, "[%d,%d,%d]",p_nCmd,p_nParam1,p_nParam2);
    std::string params(buffer);
    CToJavaBridge::GetInstance()->callMethod(-1, true, CToJavaBridge::JavaClass.c_str(), "postCmdToMain", params.c_str(), ret);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_configSetURL(JNIEnv * env, jobject obj,jstring p_strUrl)
{
    char* pstrUrl =(char*) env->GetStringUTFChars( p_strUrl, NULL ); 
	g_kSystemConfig.m_strStartURL = pstrUrl;
	LOGI("JNI seturl：%s", pstrUrl);
	env->ReleaseStringUTFChars(p_strUrl, pstrUrl);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_configSetIsPlug(JNIEnv * env, jobject obj, jboolean p_bIsPlug)
{
    JCSystemConfig::s_bIsPlug = p_bIsPlug;
	LOGI("JNI setIsPlug：%d", p_bIsPlug);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_configSetParamExt(JNIEnv * env, jobject obj,jstring p_strParamExt)
{
    char* pstrParamExt =(char*) env->GetStringUTFChars( p_strParamExt, NULL ); 
	g_kSystemConfig.m_jsonparamExt = pstrParamExt;
	LOGI("JNI setParamExt：%s", pstrParamExt);
	env->ReleaseStringUTFChars(p_strParamExt, pstrParamExt);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_InitDLib(JNIEnv * env, jobject obj,jobject assetManager,int nThreadNum,jstring p_strAssetRootPath, jstring p_strCachePath , jstring p_strAPKExpansionMainPath, jstring p_strAPKExpansionPatchPath,int threadMode,int debugMode,int debugPort)
{
	LOGI("JNI InitDLib");
	if(g_pConch)
    {
        LOGI("JNI has an old conch object! delete it");
		//如果上次不正常退出，如果时间太短，可能有的线程还在创建过程中。所以等待一会儿。
		//例如g_pConch突然为null，可能有人还在用。
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        //必须走完整流程，例如有的线程还在创建，完了后就正常跑，然后这里g_pConch又被删了
        Java_layaair_game_browser_ConchJNI_ReleaseDLib(env,obj);
        LOGI("JNI del old end");        
	}
    //这个不要放到开始，以影响面上面的异常处理
    g_nInitTime=tmGetCurms();
    char* pAssetRootPath =(char*) env->GetStringUTFChars( p_strAssetRootPath, NULL ); 
	char* pCachePath = (char*)env->GetStringUTFChars( p_strCachePath, NULL);
	char* pAPKExpansionMain =(char*) env->GetStringUTFChars( p_strAPKExpansionMainPath, NULL ); 
	char* pAPKExpansionPatch = (char*)env->GetStringUTFChars( p_strAPKExpansionPatchPath, NULL);
	LOGI( "JNI InitDownLoadManager CachePath=%s, assetroot=%s, APKExpansionMain=%s, APKExpansionPatch=%s ", pCachePath, pAssetRootPath, pAPKExpansionMain, pAPKExpansionPatch);
	gRedistPath = pCachePath;
	gRedistPath +="/";
	gAssetRootPath = pAssetRootPath;
	gAPKExpansionMainPath= pAPKExpansionMain;
	gAPKExpansionPatchPath = pAPKExpansionPatch;
	if( assetManager==0 || !(g_pAssetManager = AAssetManager_fromJava(env,assetManager)))
    {
		LOGI("JNI Warning! AssetManager =NULL!! 下面要采用jar流程了。"); 
		JCZipFile* pZip = new laya::JCZipFile();
		if( strstr(pAssetRootPath,".jar" ) ||strstr(pAssetRootPath,".JAR" )||strstr(pAssetRootPath,".zip" )||strstr(pAssetRootPath,".apk")||strstr(pAssetRootPath,".APK") ){
			if(pZip->open(pAssetRootPath))
            {
                LOGI("JNI 打开jar成功。");
				pZip->InitDir("assets");
			}
			JCConch::s_pAssetsFiles = pZip;
		}
        else
        {
            LOGI("JNI 没有设置assetMgr，也没有传入jar包。");
		} 
	}
    else
    {
		laya::JCAndroidFileSource* pAssets = new laya::JCAndroidFileSource();
		pAssets->Init(g_pAssetManager, "","", "", "");
		JCConch::s_pAssetsFiles = pAssets;
	}
	env->ReleaseStringUTFChars(p_strAssetRootPath, pAssetRootPath);
	env->ReleaseStringUTFChars(p_strCachePath, pCachePath);
	env->ReleaseStringUTFChars(p_strAPKExpansionMainPath, pAPKExpansionMain);
	env->ReleaseStringUTFChars(p_strAPKExpansionPatchPath, pAPKExpansionPatch);
	
	THREAD_MODE nMode = (THREAD_MODE)(threadMode);
    if (nMode == THREAD_MODE_SINGLE)
    {
        g_kSystemConfig.m_nThreadMODE = nMode;
        LOGI(">>>>>>Thread Mode = single");
    }
    else if (nMode == THREAD_MODE_DOUBLE)
    {
        g_kSystemConfig.m_nThreadMODE = nMode;
        LOGI(">>>>>>Thread Mode = double");
    }
    else
    {
        LOGI(">>>>>>Thread Mode = %d", g_kSystemConfig.m_nThreadMODE);
    }
		
	g_pConch = new laya::JCConch(nThreadNum,(laya::JS_DEBUG_MODE)debugMode,debugPort);
	g_pConch->m_funcPostMsgToMainThread = std::bind(postCmdToMainThread, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_SetLocalStoragePath(JNIEnv * env, jobject obj,jstring p_strLocalStorage )
{
    char* pLocalStoragePath =(char*) env->GetStringUTFChars( p_strLocalStorage, NULL ); 
	LOGI( "JNI localStoragePath=%s", pLocalStoragePath);
	env->ReleaseStringUTFChars(p_strLocalStorage, pLocalStoragePath);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleTouch( JNIEnv * env, jobject obj,jint type,jint id,jint x,jint y )
{
	enum java_motion_Action
    {
		ACTION_DOWN=0,
		ACTION_UP=1,
		ACTION_MOVE=2,
		ACTION_POINTER_DOWN=5,
		ACTION_POINTER_UP=6,
	};
	switch(type)
    {
	case java_motion_Action::ACTION_DOWN:
        {
		    inputEvent e;
		    e.nType =E_ONTOUCHSTART;
		    e.nTouchType = type;
		    e.posX = x;
		    e.posY = y;
		    e.id = id;
		    strncpy(e.type, "touchstart", 256 );
            JCScriptRuntime::s_JSRT->dispatchInputEvent(e);
            break;
	    }
	case java_motion_Action::ACTION_UP:
        {
		    inputEvent e;
		    e.nType = E_ONTOUCHEND;
		    e.nTouchType = type;
		    e.posX = x;
		    e.posY = y;
		    e.id = id;
		    strncpy(e.type, "touchend", 256 );
            JCScriptRuntime::s_JSRT->dispatchInputEvent(e);
            break;
	    }                             
    case java_motion_Action::ACTION_MOVE: 
    {
        inputEvent e;
        e.nType = E_ONTOUCHMOVE;
        e.nTouchType = type;
        e.posX = x;
        e.posY = y;
        e.id = id;
        strncpy(e.type, "touchmove", 256);
        JCScriptRuntime::s_JSRT->dispatchInputEvent(e);
        break;
    }
	case java_motion_Action::ACTION_POINTER_DOWN:
        {
		    inputEvent e;
		    e.nType = E_ONACTION_POINTER_DOWN;
		    e.nTouchType = type;
		    e.posX = x;
		    e.posY = y;
		    e.id = id;
		    strncpy(e.type, "touchstart", 256 );
            JCScriptRuntime::s_JSRT->dispatchInputEvent(e);
            break;
	    }
	case java_motion_Action::ACTION_POINTER_UP:
        {
		    inputEvent e;
		    e.nType = E_ONACTION_POINTER_UP;
		    e.nTouchType = type;
		    e.posX = x;
		    e.posY = y;
		    e.id = id;
		    strncpy(e.type, "touchend", 256 );
            JCScriptRuntime::s_JSRT->dispatchInputEvent(e);
            break;
	    }
	}
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleKeyEvent(JNIEnv * env, jobject obj,jint keyCode,jint actionType)
{
	enum android_key_Code
	{
		KEYCODE_BACK = 4,
		KEYCODE_DPAD_UP = 19,
		KEYCODE_DPAD_DOWN = 20,
		KEYCODE_DPAD_LEFT = 21,
		KEYCODE_DPAD_RIGHT = 22,
		KEYCODE_DPAD_CENTER = 23,
		KEYCODE_VOLUME_UP = 24,
		KEYCODE_VOLUME_DOWN = 25,
		KEYCODE_ENTER = 66,
		KEYCODE_MENU = 82,
		KEYCODE_BUTTON_A = 96,
		KEYCODE_BUTTON_B = 97,
		KEYCODE_BUTTON_X = 99,
		KEYCODE_BUTTON_Y = 100,
		KEYCODE_BUTTON_L1 = 102,
		KEYCODE_BUTTON_R1 = 103,
		KEYCODE_BUTTON_L2 = 104,
		KEYCODE_BUTTON_R2 = 105,
		KEYCODE_BUTTON_THUMBL = 106,
		KEYCODE_BUTTON_THUMBR = 107,
		KEYCODE_BUTTON_START = 108,
		KEYCODE_BUTTON_SELECT = 109,
	};
	enum android_key_Action
	{
		ACTION_DOWN = 0,
		ACTION_UP = 1,
	};
	if(actionType == android_key_Action::ACTION_DOWN)
	{
		inputEvent e;
		e.nType = E_ONKEYDOWN;
		e.keyCode = keyCode;
		strncpy(e.type, "keydown", 256 );
        JCScriptRuntime::s_JSRT->dispatchInputEvent(e);
	}
	else if(actionType == android_key_Action::ACTION_UP)
	{
		inputEvent e;
		e.nType =E_ONKEYUP;
		e.keyCode = keyCode;
		strncpy(e.type, "keyup", 256 );
        JCScriptRuntime::s_JSRT->dispatchInputEvent(e);
	}
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleJoystickEvent( JNIEnv * env, jobject obj,float THUMBL_xOffset,float THUMBL_yOffset,float THUMBR_xOffset,float THUMBR_yOffset,float LT_Offset,float RT_Offset )
{
	inputEvent e;
	e.nType = E_JOYSTICK;
	e.fTHUMBL_xOffset = THUMBL_xOffset;
	e.fTHUMBL_yOffset = THUMBL_yOffset;
	e.fTHUMBR_xOffset = THUMBR_xOffset;
	e.fTHUMBR_yOffset = THUMBR_yOffset;
	e.fLT_Offset = LT_Offset;
	e.fRT_Offset = RT_Offset;
	strncpy(e.type, "onjoystick", 256 );
    JCScriptRuntime::s_JSRT->dispatchInputEvent(e);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleDeviceMotionEvent(JNIEnv * env, jobject obj, float ax, float ay, float az, float agx, float agy, float agz, float ra, float rb, float rg, float interval)
{
    DeviceMotionEvent e;
    e.nType = E_DEVICEMOTION;
    e.ax = ax;
    e.ay = ay;
    e.az = az;
    e.agx = agx;
    e.agy = agy;
    e.agz = agz;
    e.ra = ra;
    e.rb = rb;
    e.rg = rg;
    e.interval = interval;
    strncpy(e.type, "devicemotion", 256);
    JCScriptRuntime::s_JSRT->dispatchInputEvent(e);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_handleDeviceOrientationEvent(JNIEnv * env, jobject obj, float ra, float rb, float rg)
{
    DeviceOrientationEvent e;
    e.nType = E_DEVICEORIENTATION;
    e.ra = ra;
    e.rb = rb;
    e.rg = rg;
    strncpy(e.type, "deviceorientation", 256);
    JCScriptRuntime::s_JSRT->dispatchInputEvent(e);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_ReleaseDLib(JNIEnv * env, jobject obj )
{
	LOGI("JNI del engine");   
	JCAudioManager::GetInstance()->stopMp3();
	if(g_pConch)
	{
        if(tmGetCurms()- g_nInitTime<2000)
        {
            LOGI("JNI exit need a little wait");
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
		if (g_bInBKGround)
        {
            LOGI("JNI App in background!");
            JCConch::s_pConchRender->willExit();
        }
		g_pConch->onAppDestory();
		if (g_kSystemConfig.m_nThreadMODE==THREAD_MODE_SINGLE)
        {
            JCScriptRuntime::s_JSRT->stop();
        }
		delete g_pConch;
		g_pConch=NULL;
	}
	g_bEngineInited = false;
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_OnAppDestroy(JNIEnv * env, jobject obj )
{
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_OnAppPause(JNIEnv * env, jobject obj )
{
    LOGI("JNI OnAppPause");
    g_bInBKGround = true;
	if( laya::JCAudioManager::GetInstance()->getMp3Mute() == false && laya::JCAudioManager::GetInstance()->getMp3Stopped() == false)
	{
		JCAudioManager::GetInstance()->pauseMp3();
	}
	laya::JCAudioManager::GetInstance()->m_pWavPlayer->pause();
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_OnAppResume(JNIEnv * env, jobject obj )
{
    LOGI("JNI OnAppResume");
    if (!g_pConch)
        return;
    g_bInBKGround=false;
	//继续声音
	if( laya::JCAudioManager::GetInstance()->getMp3Mute() == false && laya::JCAudioManager::GetInstance()->getMp3Stopped() == false)
	{
		laya::JCAudioManager::GetInstance()->resumeMp3();
	}
	laya::JCAudioManager::GetInstance()->m_pWavPlayer->resume();
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_OnGLReady(JNIEnv * env, jobject obj, int width,int height )
{
	LOGI("JNI onGLReady tid=%ld", gettidv1());
	auto pRender = JCConch::s_pConchRender;
	if( g_nInnerWidth!=width || g_nInnerHeight != height )
    {
        LOGI("JNI surface innersize changed : g_nInnerWidth=%d,g_nInnerHeight=%d",width,height);
		g_nInnerWidth = width;
		g_nInnerHeight = height;
		g_bGLCanvasSizeChanged=true;
	}
    LOGI("JNI init dev w=%d,h=%d",width,height);
    //pRender->onGLDeviceLosted();
    pRender->onGLReady();
    g_kReadyLock.lock();
	if( !g_bEngineInited )
    {
		//启动js线程
		g_pConch->onAppStart();
		g_bEngineInited = true;
		if (g_kSystemConfig.m_nThreadMODE==THREAD_MODE_SINGLE)
		{
			JCScriptRuntime::s_JSRT->start(JCConch::s_pConch->m_strStartJS.c_str());
		}
	}	
    g_kReadyLock.unlock();
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onDrawFrame(JNIEnv * env, jobject obj )
{
    auto pRender = JCConch::s_pConchRender;
	if (pRender)
	{
        pRender->renderFrame(0,false);
	}
}

JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onVSyncCallback(JNIEnv * env, jobject obj, jlong VSynctm) {
    double vsynctm = VSynctm / 1e6;
    //auto ctm = laya::tmGetCurms();
    //LOGE("---TM:%f,d=%f,cur=%f,d=%f", (float)vsynctm, (float)(vsynctm - lastVSYNC1) , ctm,(ctm-vsynctm));
    if (JCScriptRuntime::s_JSRT) {
        JCScriptRuntime::s_JSRT->onVSyncEvent(vsynctm);
    }
}

JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_closeExternalWebView(JNIEnv * env, jobject obj )
{
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_alertCallback(JNIEnv * env, jobject obj )
{
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_exportStaticMethodToC(JNIEnv * env, jobject obj, jstring packcls)
{
    const char* rawPackCls = env->GetStringUTFChars(packcls, NULL);
	CToJavaBridge::GetInstance()->addStaticMethod(env,rawPackCls);
    env->ReleaseStringUTFChars(packcls, rawPackCls);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onSensorChanged(JNIEnv * env, jobject obj,float arc )
{
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_reloadJS(JNIEnv * env, jobject obj )
{
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_onRunCmd(JNIEnv* env, jobject obj, jint cmd, jint nParam1, jint nParam2 )
{
	if(g_pConch )
    {
        g_pConch->onRunCmdInMainThread( cmd, nParam1, nParam2 );
	}
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_RunJS(JNIEnv* env, jobject obj, jstring jsstr )
{
    if (g_pConch && JCScriptRuntime::s_JSRT && jsstr)
    {
        const char* rawString = env->GetStringUTFChars(jsstr, NULL);
        JCScriptRuntime::s_JSRT->callJSString(rawString);
        env->ReleaseStringUTFChars(jsstr, rawString);
    }
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_postMsgToRuntime(JNIEnv* env, jobject obj, jstring msg, jstring params)
{
    const char* rawMsg = env->GetStringUTFChars(msg, NULL);
    const char* rawParams = env->GetStringUTFChars(params, NULL);
    env->ReleaseStringUTFChars(msg, rawMsg);
    env->ReleaseStringUTFChars(params, rawParams);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_exitToPlatform( JNIEnv * env, jobject obj )
{
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_audioMusicPlayEnd( JNIEnv * env, jobject obj )
{
	laya::JCMp3Interface* pMp3Player = laya::JCAudioManager::GetInstance()->m_pMp3Player;
	if( pMp3Player )
	{
		pMp3Player->onPlayEnd();
	}
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_networkChanged(JNIEnv* env, jobject obj, jint nNetworkType )
{
    JCScriptRuntime::s_JSRT->onNetworkChanged(nNetworkType);
}

JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_inputChange(JNIEnv* env, jobject obj, jint keycode)
{
    if (JCScriptRuntime::s_JSRT->m_pCurEditBox)
    {
        JCScriptRuntime::s_JSRT->m_pCurEditBox->onInput();
    }
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_setLocalizable(JNIEnv * env, jobject obj, jboolean p_bIsLocalPackage)
{
    JCSystemConfig::s_bLocalizable = p_bIsLocalPackage;
    LOGI("setLocalizable：%d", p_bIsLocalPackage);
}
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_callConchJSFunction(JNIEnv* env, jobject obj, jstring p_sFunctionName,jstring p_sJsonParam,jstring p_sCallbackFunction)
{
    const char* sFunctionName = env->GetStringUTFChars(p_sFunctionName, NULL);
    const char* sJsonParam = env->GetStringUTFChars(p_sJsonParam, NULL);
    const char* sCallbackFunction = env->GetStringUTFChars(p_sCallbackFunction, NULL);
    LOGI(">>>>>>>>Java_layaair_game_browser_ConchJNI_callConchJSFunction functionName=%s, jsonParam=%s, callbackFuncton=%s",sFunctionName, sJsonParam,sCallbackFunction );
    JCScriptRuntime::s_JSRT->callJC( sFunctionName,sJsonParam,sCallbackFunction );
	env->ReleaseStringUTFChars(p_sFunctionName, sFunctionName);
    env->ReleaseStringUTFChars(p_sJsonParam, sJsonParam);
    env->ReleaseStringUTFChars(p_sCallbackFunction, sCallbackFunction);
}
//------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_layaair_game_browser_ConchJNI_captureScreenCallBack(JNIEnv* env, jobject obj, jint w, jint h, jbyteArray byteArray) {
    jint len = env->GetArrayLength(byteArray);
   
    jbyte* ba = env->GetByteArrayElements(byteArray, JNI_FALSE);
	char* result = new char[len];
    memcpy(result, ba, len);
    JSInput::getInstance()->captureScreenCallBack(result,len, w, h);
    env->ReleaseByteArrayElements(byteArray, ba, 0);

}
//------------------------------------------------------------------------------
JNIEXPORT jboolean JNICALL Java_layaair_game_browser_ConchJNI_onBackPressed(JNIEnv* env, jobject obj)
{
	 LOGI(">>>>>>>>Java_layaair_game_browser_ConchJNI_onBackPressed");
	 bool ret = JCScriptRuntime::s_JSRT->onBackPressed();
	 return jboolean(ret);
}

JNIEXPORT void JNICALL Java_layaair_game_browser_LayaVideoPlayer_emit(JNIEnv* env, jobject obj, jlong ptr, jstring str)
{
	JSVideo* pVideo = reinterpret_cast<JSVideo*>(ptr);

	if(!pVideo)
		return;

	const char* evtName = env->GetStringUTFChars(str, NULL);
	
//	LOGI("[DEBUG][Video]Call emit function %s", evtName);

	pVideo->CallHandle(evtName);
	env->ReleaseStringUTFChars(str, evtName);
}

JNIEXPORT void JNICALL Java_layaair_game_browser_LayaVideoPlayer_transferBitmap(JNIEnv* env, jobject obj, jobject bitmap, jlong dataPtr)
{
    AndroidBitmapInfo bmpInfo={0};
	if(AndroidBitmap_getInfo(env, bitmap, &bmpInfo) < 0)
	{
//		LOGE("[Debug][Video]bitmap: Error getInfo");
		return;
	}

	char** dataFromBmp=NULL;
    if(AndroidBitmap_lockPixels(env,bitmap,(void**)&dataFromBmp))
    {
//		LOGE("[Debug][Video]bitmap: Error lockPixels ");
		return;
	}

//	LOGI("[Debug][Video]Call native transferBitmap %ld", dataPtr);

	BitmapData* pBitmapData = reinterpret_cast<BitmapData*>(dataPtr);
	if(!pBitmapData)
	{
//		LOGE("[Debug][Video]Error pBitmapData");
		AndroidBitmap_unlockPixels(env,bitmap);
		return;
	}

	pBitmapData->reconfigure(bmpInfo.width, bmpInfo.height, 32, ImgType_unknow);
	memcpy(pBitmapData->m_pImageData, dataFromBmp, sizeof(int32_t) * pBitmapData->m_nWidth * pBitmapData->m_nHeight);

	AndroidBitmap_unlockPixels(env,bitmap);
}

//-----------------------------END FILE--------------------------------
