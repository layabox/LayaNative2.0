/**
@file			gles.h
@brief			
@author			guo
@version		1.0
@date			2016_5_27
*/

#ifndef __gles_H__
#define __gles_H__

#include "esUtil.h"
#include <misc/JCWorkerThread.h>

namespace laya{
    class winWindows;
    class JCWorkerThread;
    class JCIGLRender;
    class gles{
    public:
        gles(JCWorkerThread* pMyThread);//所在线程可以为空

        bool initFromWindow( winWindows& p_Windows /*其他参数*/);

        bool setRenderer( JCIGLRender* p_pRender );

        bool renderLoop();//必须与initFromWindow在同一个线程

        //当窗口大小改变的时候，自动调到这里
        void onWndResize( int p_nWidth, int p_nHeight );

    public:
        ESContext       m_esContext;
        JCIGLRender*    m_pRender;
        bool            m_bInited;
        JCWorkerThread* m_pMyThread;
    };

    //当前线程的GL
    class GLEnv{
    public:
        GLEnv();
        ~GLEnv();
        void init(JCIGLRender* pRender, int p_nWidth, int p_nHeight);
        //渲染一帧
        void drawAFrame();
    protected:
        void onGLStart();
    public:
        winWindows*     m_pWindows;
        gles*           m_pGLES;
        JCIGLRender*    m_pRenderer;
    };

    //独立线程的GL
    class GLEnvThread{
    public:
        GLEnvThread();
        ~GLEnvThread();
        void init(JCIGLRender* pRender,int p_nWidth, int p_nHeight);
        void stop();
        //重新创建GLES环境
        void onResume();
        //删除GLES环境
        void onPause();
        void onGLStart( JCEventEmitter::evtPtr evt);
        void onGLExit(JCEventEmitter::evtPtr evt);
        void onWindowClose();
    public:
        winWindows*     m_pWindows;
        gles*           m_pGLES;
        JCWorkerThread  m_RenderThread;
        JCIGLRender*    m_pRenderer;
    };
};

#endif //__gles_H__

