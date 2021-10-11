/**
@file			JCScriptRuntime.h
@brief			
@author			James
@version		1.0
@date			2016_5_13
*/
 
#ifndef __JCScriptRuntime_H__
#define __JCScriptRuntime_H__

#include <vector>
#include <util/JCCommonMethod.h>
#include "JSWrapper/JSInterface/JSInterface.h"
#include <fontMgr/JCFreeTypeRender.h>
#include "util/JCLayaUrl.h"
#include <mutex>
#include <manager/JCArrayBufferManager.h>
#include <Manager/JCOrderResManager.h>
#include <RenderEx/JCRegister.h>
#ifdef ANDROID
    #include "JSWrapper/LayaWrap/JSAndroidEditBox.h"    
#elif __APPLE__
    #include "JSWrapper/LayaWrap/JSIOSEditBox.h"
#endif
#include <util/JCIThreadCmdMgr.h>

namespace laya
{
    enum NetworkType 
    {
        NET_NO = 0,
        NET_WIFI = 1,
        NET_2G = 2,
        NET_3G = 3,
        NET_4G = 4,
        NET_YES = 5,
    };
    struct inputEvent
    {
        char type[256];
        int nType;
        int key;
        int posX;
        int posY;
        int keyChar;
        int keyCode;
        int nWheel;
        float fTHUMBL_xOffset;
        float fTHUMBL_yOffset;
        float fTHUMBR_xOffset;
        float fTHUMBR_yOffset;
        float fLT_Offset;
        float fRT_Offset;
        bool bCtrl;
        bool bAlt;
        bool bShift;
        int id;
        int nTouchType;   //是用来传给js的
        inputEvent()
        {
            memset(type, 0, 256);
            nType = 0;
            key = 0;
            posX = 0;
            posY = 0;
            keyChar = 0;
            keyCode = 0;
            nWheel = 0;
            fTHUMBL_xOffset = fTHUMBL_yOffset = fTHUMBR_xOffset = fTHUMBR_yOffset = fLT_Offset = fRT_Offset = .0f;
            bCtrl = bAlt = bShift = false;
            id = 0;
            nTouchType = 0;
        }
    };
    struct DeviceOrientationEvent
    {
        char type[256];
        int nType;
        float ra;
        float rb;
        float rg;
    };
    struct DeviceMotionEvent:public DeviceOrientationEvent
    {
        float ax;
        float ay;
        float az;
        float agx;
        float agy;
        float agz;
        float interval;
    };
    enum EINPUTTYPE
    {
        E_ONTOUCHSTART = 0X01,
        E_ONTOUCHMOVE,
        E_ONTOUCHEND,
        E_ONACTION_POINTER_DOWN,
        E_ONACTION_POINTER_UP,
        E_ONMOUSEDOWN,
        E_ONMOUSEMOVE,
        E_ONMOUSEWHEEL,
        E_ONMOUSEUP,
        E_ONRIGHTMOUSEDOWN,
        E_ONRIGHTMOUSEUP,
        E_ONKEYDOWN,
        E_ONKEYUP,
        E_JOYSTICK,
        E_DEVICEMOTION,
        E_DEVICEORIENTATION,
        E_TYPE_COUNT,
    };
#ifdef JS_V8
    class DebuggerAgent;
#endif
    class JSLayaGL;
    class JCFileResManager;
    class JCFileSource;
    class JSCallbackFuncObj;
    class JCScriptRuntime : public IConchThreadCmdMgr
    {
    public:
        
        JCScriptRuntime();

        ~JCScriptRuntime();

        /** @brief
                初始化js环境。
         *  @param[in] pFileMgr
         *  @param[in] pAssetRes
         *  @param[in] pThreadCmdSender 跨线程发送消息的统一管理。避免各个线程直接发送消息导，以简化退出流程。
         *  @return void
        */
        void init(JCFileResManager* pFileMgr, JCFileSource* pAssetRes, IConchThreadCmdMgr* pThreadCmdSender);

        void start(const char* pStartJS);

        void stop();

        void reload();

        void onThreadInit(JCEventEmitter::evtPtr evt);

        bool onUpdate();
        
        void onUpdateTimer();

        //每次需要渲染的时候触发js
        void onUpdateDraw(double vsyncTime);

        void flushSharedCmdBuffer();

        void dispatchLayaGLBuffer(bool bDispatchGC);

        void runLayaGL();

        //输入事件触发js
        void onUpdateInput();

        /*
            可以调用渲染流程了。
            参数是当前的垂直回扫的时间。
            本来打算在垂直回扫的时候post一个函数到js线程执行。但是这样有个问题就是如果js卡了，会积累大量的函数。
            所以采用这个简单的方法。
        */
        void onVSyncEvent(double vsyncTime);

        void onThreadExit(JCEventEmitter::evtPtr evt);

        void dispatchInputEvent(inputEvent e);

        void dispatchInputEvent(DeviceOrientationEvent e);

        void dispatchInputEvent(DeviceMotionEvent e);

    public:

        void jsGC();
        void jsGCCallJSFunction();

        void callJSString( std::string sBuffer );
        void callJSStringFunction( std::string sBuffer );

        void jsReloadUrl();
        void jsReloadUrlJSFunction();

        void jsUrlback();
        void jsUrlbackJSFunction();

        void restoreAudio();
        void jsRestoreAudioFunction();

        void onNetworkChanged(int nType);
        void onNetworkChangedCallJSFunction(int nType);

        void callJC( std::string sFunctionName,std::string sJsonParam,std::string sCallbackFunction );
        void callJSFuncton(std::string sFunctionName, std::string sJsonParam, std::string sCallbackFunction);
        
		bool onBackPressed();

    public:

        void postToJS(const std::function<void(void)>& func);

        void postToDownload(const std::function<void(void)>& funcf);

        void postToDecoder(const std::function<void(void)>& func);\

    private:

        void startScriptOnRenderThread();

    public:
        static JCScriptRuntime*             s_JSRT;
        JCCommandEncoderBuffer*				m_pRenderCmd;                   //渲染指令流
        JCCommandEncoderBuffer*             m_pGCCmd;                       //垃圾回收的指令流
        bool                                m_bReload;
        JSThreadInterface*                  m_pScriptThread;
        bool                                m_bHasJSThread;	                //js线程是否在工作
        JsObjHandle                         m_pJSOnFrameFunction;
        JsObjHandle                         m_pJSOnDrawFunction;            //垂直回扫同步
        JsObjHandle                         m_pJSOnResizeFunction;
        JsObjHandle                         m_pJSOnBlurFunction;
        JsObjHandle                         m_pJSOnFocusFunction;
        JsObjHandle                         m_pJSMouseEvtFunction;          //鼠标事件回调
        JsObjHandle                         m_pJSKeyEvtFunction;
        JsObjHandle                         m_pJSTouchEvtFunction;
        JsObjHandle                         m_pJSDeviceMotionEvtFunction;   //重力感应
        JsObjHandle                         m_pJSOnceOtherEvtFuction;       //注册一次的事件 如截屏
        JsObjHandle					        m_pJSNetworkEvtFunction;        //网络事件的监听
        JsObjHandle					        m_pJSOnBackPressedFunction;
        bool 						        m_bJSOnBackPressedFunctionSet;
        JsObjHandle                         m_bJSBulletGetWorldTransformHandle;
        JsObjHandle                         m_bJSBulletSetWorldTransformHandle;
        std::mutex		        	        m_OnBackPressedMutex;
        std::string                         m_strStartJS;
        JCFileResManager*                   m_pFileResMgr;                  //外部设置的。本地不允许删除
        JCFileSource*                       m_pAssetsRes;	                //外部设置的。本地不允许删除
        IConchThreadCmdMgr*                 m_pPoster;
        bool                                m_bIsExit;
        char*                               m_pOtherBufferSharedWidthJS;
        int							        m_nUpdateCount;                 //update次数
        JCUrl*                              m_pUrl;
        /*
         * 确保线程已经开始了。有时候想结束线程，但是实际线程还没起来（结束请求太快）
         * 确保线程已经开始了。有时候想结束线程，但是实际线程还没起来（结束请求太快）
         * 需要靠这个状态来判断是否已经起来了。防止stop之后，线程才起来，导致混乱。
        */
        int                                 m_nThreadState;                 
        JCOrderResManager<JSCallbackFuncObj>*   m_pCallbackFuncManager;     ///<回调函数的管理器
        JCRegister*                         m_pRegister;                    ///<寄存器
        JCArrayBufferManager*               m_pArrayBufferManager;          ///<ArrayBufferManager
        JCArrayBufferManager*               m_pABManagerSyncToRender;       ///<ArrayBufferManager，这个是需要同步给渲染线程的
        bool                                m_bHasPostVsync;                ///避免发送太多的同步事件
#ifdef ANDROID
        JSAndroidEditBox*                   m_pCurEditBox;
#elif __APPLE__
        JSIOSEditBox *                      m_pCurEditBox;
#endif
#ifdef JS_V8
        DebuggerAgent*				        m_pDbgAgent;
#endif
    private:
        bool                                m_bRunDraw;
        /*
            希望每次垂直回扫的时候都能执行js。但实际做不到。
            记录 m_dbLastUsedVsync 可以知道实际两次调用之间到底跨越了多少，即丢了多少帧
            记录 m_dbCurVsync 可以用来判断距离最近的垂直回扫的时间，从而可以判断是否需要主动减少当前帧的工作量（例如不重要的事情可以分帧做）
            上次实际调用draw的时候对应的时间。在卡的情况下
        */
        double                              m_dbLastUsedVsync;
        double                              m_dbCurVsync;
        double                              m_nFPS;
        double                              m_nDelayTime;
    };
}

//------------------------------------------------------------------------------


#endif //__JCScriptRuntime_H__

//-----------------------------END FILE--------------------------------
