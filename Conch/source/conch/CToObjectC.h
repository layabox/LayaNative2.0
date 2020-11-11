/**
@file			CToObjectC.h
@brief			
@author			wyw
@version		1.0
@date			2014_8_26
*/

#ifndef _CToObjectC_H_
#define _CToObjectC_H_

#include <stdio.h>
#include <string>
#include <vector>
#include <JavaScriptCore/JSBase.h>
#include "resource/Audio/JCAudioInterface.h"
#include "resource/Audio/JCWaveInfo.h"
#include <functional>
#include "../render/Image/JCVideo.h"

void CToObjectCPostMainThread(int cmd,int param1,int param2);

char* CToObjectCGetFontBuffer(long& dataSize);

void CToObjectCPostEditBox(int cmd,int param1,int param2=0,bool bparam2=true,const char* sparam3=NULL);

void ObjectCOperateEditBox(int cmd,int param1,int param2=0,bool bparam2=true,std::string sparam3="");


void CToObjectCAlert(const char* message);

//以下是EditBox相关的
//------------------------------------------------------------------------------
enum IOSEditBoxOperator
{
    IOS_EDITBOX_SETPOSX = 1,
    IOS_EDITBOX_SETPOSY,
    IOS_EDITBOX_SETWIDTH,
    IOS_EDITBOX_SETHEIGHT,
    IOS_EDITBOX_SETVALUE,
    IOS_EDITBOX_SETSTYLE,
    IOS_EDITBOX_SETVISIBLE,
    IOS_EDITBOX_SETFOCUS,
    IOS_EDITBOX_SETBLUR,
    IOS_EDITBOX_SETCOLOR,
    IOS_EDITBOX_SETFONTSIZE,
    IOS_EDITBOX_SETFONTPOS,
    IOS_EDITBOX_SETFONTSIZE2,
    IOS_EDITBOX_SETCURSORPOSITION,
    IOS_EDITBOX_SETMAXLENGTH,
    IOS_EDITBOX_SETPASSWORD,
    IOS_EDITBOX_SETREGULAR,
    IOS_EDITBOX_SETNUMBERONLY,
    IOS_EDITBOX_SETMULTIABLE,
    IOS_EDITBOX_SETFORBIDEDIT,
};

static const int IOS_EDITBOX_POSX = 1;

void IOS_SetEditBoxStyle(const char* p_sType);

void CToObjectCSetEditBoxX( int p_nX );

void CToObjectCSetEditBoxY( int p_nY );

void CToObjectCSetEditBoxWidth( int p_nWidth );

void CToObjectCSetEditBoxHeight( int p_nHeight );

void CToObjectCSetEditBoxValue( const char* p_sValue );

void CToObjectCSetEditBoxStyle( const char* p_sType );

void CToObjectCSetEditBoxVisible( bool p_bVisible );

void CToObjectCSetEditBoxFocus();

void CToObjectCSetEditBoxBlur();

void CToObjectCSetEditBoxColor( int p_nColor );

void CToObjectCSetEditBoxFontSize( int p_nFontSize );

void CToObjectCSetEditBoxFontPos( int p_nX,int p_nY );

void CToObjectCSetEditBoxFontSize( int p_nWidth,int p_nHeight );

void CToObjectCSetEditBoxCursorPosition( int p_nPos );

void CToObjectCSetEditBoxMaxLength( int p_nMaxLength );

void CToObjectCSetEditBoxPassword( bool p_bPassword );

void CToObjectCSetEditBoxRegular( const char* p_sRegular );

void CToObjectCSetEditBoxNumberOnly( bool p_bNumberOnly );

void CToObjectCSetEditBoxForbidEdit( bool p_bForbidEdit );

const char* CToObjectCGetEditBoxValue();

void CToObjectCSetEditBoxMultiAble(bool p_bMultiAble);

void CToObjectCSetCurrentTime(double nCurrentTime);
double CToObjectCGetCurrentTime();
double CToObjectCGetDuration();

//------------------------------------------------------------------------------
//以下是声音相关的
//------------------------------------------------------------------------------
void CToObjectCPlayMp3Audio( const char* p_sUrl,int p_nTimes,float nCurrentTime );
void CToObjectCSetMp3Volume( float p_nVolume );
void CToObjectCStopMp3();
void CToObjectCResumeMp3();
void CToObjectCPauseMp3();

//------------------------------------------------------------------------------
//以下是视频相关的
//------------------------------------------------------------------------------
void CToObjectNewVideoPlayer(void** obj, std::function<void(const char*)> emitFunc);
void CToObjectDisposeVideoPlayer(void* obj);
void CToObjectReleaseVideoPlayer(void* obj);
void CToObjectVideoPlayerLoad(void* obj, const char* path);
void CToObjectVideoPlayerPlay(void* obj);
void CToObjectVideoPlayerPause(void* obj);
void CToObjectVideoPlayerGetPaused(void* obj, bool* ret);
void CToObjectVideoPlayerGetVideoWidth(void* obj, double* ret);
void CToObjectVideoPlayerGetVideoHeight(void* obj, double* ret);
void CToObjectVideoPlayerSetX(void* obj, double val);
void CToObjectVideoPlayerSetY(void* obj, double val);
void CToObjectVideoPlayerSetWidth(void* obj, double val);
void CToObjectVideoPlayerGetWidth(void* obj, double* val);
void CToObjectVideoPlayerSetHeight(void* obj, double val);
void CToObjectVideoPlayerGetHeight(void* obj, double* val);
void CToObjectVideoPlayerGetCurrentTime(void* obj, double* val);
void CToObjectVideoPlayerSetCurrentTime(void* obj, double val);
void CToObjectVideoPlayerGetDuration(void* obj, double* val);
void CToObjectVideoPlayerSetVolume(void* obj, double val);
void CToObjectVideoPlayerGetVolume(void* obj, double* val);
void CToObjectVideoPlayerSetLoop(void* obj, bool val);
void CToObjectVideoPlayerGetLoop(void* obj, bool* val);
void CToObjectVideoPlayerGetReadyState(void* obj, int* val);
void CToObjectVideoPlayerGetMuted(void* obj, bool* val);
void CToObjectVideoPlayerSetMuted(void* obj, bool val);
void CToObjectVideoPlayerGetAutoplay(void* obj, bool* val);
void CToObjectVideoPlayerSetAutoplay(void* obj, bool val);
void CToObjectVideoPlayerGetBitmap(void* obj, laya::BitmapData* bitmap);
void CToObjectVideoPlayerIsFrameUpdate(void* obj, bool* val);


//------------------------------------------------------------------------------
//JSC比较恶心，必须在ObjectC层开启和关闭js线程的循环
void CToObjectCRunJSLoop();
void CToObjectCRunStopJSLoop();
void CToObjectCPostFunc(std::function<void(void)> func);
//------------------------------------------------------------------------------
//本地推送
void CToObjectCSetRepeatNotify( int p_nID,long p_nStartTime,int p_nRepeatType,const char* p_sTickerText,const char* p_sTitleText,const char* p_sDesc );
void CToObjectCSetOnceNotify( int p_nID,long p_nStartTime,const char* p_sTickerText,const char* p_sTitleText,const char* p_sDesc );
void CToObjectCDeleteOnceNotify( int p_nID );
void CToObjectCDeleteAllNotify();

//------------------------------------------------------------------------------
//以下是一些杂项
//------------------------------------------------------------------------------
//获得总的内存数
long CToObjectCGetTotalMem();
long CToObjectCGetUsedMem();
long CToObjectCGetAvalidMem();
float CToObjectCGetScreenInch();
void CToObjectCSetScreenOrientation( int p_nType );
long CToObjectCGetScreenOrientation();
int CToObjectCGetNetworkType();
std::string CToObjectCGetGUID();
std::string CToObjectCGetDeviceModel();
std::string CToObjectCGetDeviceInfo();
float CToObjectCGetDeviceSystemVersion();
std::string CToObjectCGetAppVersion();
std::string CToObjectCGetAppLocalVersion();
void CToObjectCSetExternalLink( const char* p_sUrl,int x,int y,int w,int h,bool bShowCloseButton );
void CToObjectCCloseExternalLink();
void CToObjectCSetScreenWakeLock( bool p_bWakeLock );
void CToObjectCShowToast( const char* p_sInfo );
void CToObjectCOpenAppStoreUrl( const char* p_sAppID );
std::string CToObjectCCallMethod(int objid,bool isSync, const char*clsName, const char* methodName, const char* paramStr);//
void CToObjectCCallWebviewJS(const char* functionName, const char* jsonParam, const char* callback);
void CToObjectCShowWebView();
void CToObjectCHideWebView();
void CToObjectCSetSensorAble(bool p_bAble);
void CToObjectCRunJS(const std::string& script);
void CToObjectCCaptureScreen();
float CToObjectCGetDevicePixelRatio();
void CToObjectCOnBlur();
void CToObjectCOnFocus();
#endif //_CToObjectC_H_

//-----------------------------END FILE--------------------------------
