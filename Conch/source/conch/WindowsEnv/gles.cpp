
#include "gles.h"
#include "winWindows.h"
#include <misc/JCWorkerThread.h>
#include <JCIGLRender.h>
#include "../JCScriptRuntime.h"
#include "../JCSystemConfig.h"
#include "../JCConch.h"

extern int g_nInnerWidth;
extern int g_nInnerHeight;
extern bool g_bGLCanvasSizeChanged;
#pragma warning(disable:4800)
namespace laya{

    gles::gles(JCWorkerThread* pMyThread){
        m_pRender = nullptr;
        m_pMyThread = pMyThread;
    }

    void gles_onEvent_WndResize( gles* p_gles, JCEventEmitter::evtPtr evt ){
        p_gles->onWndResize(evt->m_nP1, evt->m_nP2);
    }

    bool gles::initFromWindow( winWindows& p_Windows /*其他参数*/){
        m_esContext.hWnd = p_Windows.m_hWnd;
        m_bInited = (bool)esCreateContext(&m_esContext, ES_WINDOW_RGB|ES_WINDOW_DEPTH);
        p_Windows.on(winWindows::Evt_WndResize,std::bind(gles_onEvent_WndResize,this, std::placeholders::_1),m_pMyThread);
        return true;
    }

    bool gles::setRenderer( JCIGLRender* p_pRender ){
        if(!m_bInited)//必须先初始化 initFromWindow才行
            throw -1;
        m_pRender = p_pRender;
        m_pRender->onGLReady();
        return true;
    }

    bool gles::renderLoop(){
        m_pRender->renderFrame(0,false);
        eglSwapBuffers ( m_esContext.eglDisplay, m_esContext.eglSurface );    
        return true;
    }

    void gles::onWndResize(int p_nWidth, int p_nHeight){
        g_nInnerWidth=p_nWidth;
        g_nInnerHeight=p_nHeight;
        g_bGLCanvasSizeChanged=true;
    }

    /////////////////////////////////////////////////////////////////////
    GLEnv::GLEnv(){
        m_pWindows = new winWindows();;
        m_pGLES = nullptr;
        m_pRenderer = nullptr;
    }

    GLEnv::~GLEnv(){
        if(m_pWindows){delete m_pWindows;}
        if(m_pGLES){delete m_pGLES;}
    }

    void GLEnv::init(JCIGLRender* pRender, int p_nWidth, int p_nHeight){
        m_pRenderer = pRender;
        m_pWindows->create(0,0,p_nWidth, p_nHeight);
        m_pGLES = new gles(nullptr);
        onGLStart();
        //m_RenderThread.setLoopFunc(std::bind(&gles::renderLoop,m_pGLES));
    }

    void GLEnv::drawAFrame(){
        m_pGLES->renderLoop();
    }

    void GLEnv::onGLStart(){
        m_pGLES->initFromWindow(*m_pWindows);
        m_pGLES->setRenderer(m_pRenderer);
    }

    /////////////////////////////////////////////////////////////////////
    //独立线程的GL
    GLEnvThread::GLEnvThread(){
        m_pWindows = new winWindows();;
        m_pGLES = nullptr;
        m_pRenderer = nullptr;
    }

    GLEnvThread::~GLEnvThread(){
        if(m_pWindows){delete m_pWindows;}
        if(m_pGLES){delete m_pGLES;}
    }

    void GLEnvThread::init(JCIGLRender* pRender,int p_nWidth, int p_nHeight){
        m_pRenderer = pRender;
        m_pWindows->create(0,0,p_nWidth,p_nHeight);
        m_pWindows->on(winWindows::Evt_WndClose, std::bind(&GLEnvThread::onWindowClose,this));//关闭窗口就关闭渲染线程
        m_pGLES = new gles(&m_RenderThread);
        m_RenderThread.on(JCWorkerThread::Event_threadStart, std::bind(&GLEnvThread::onGLStart,this, std::placeholders::_1));//在线程开始的时候初始化GL环境。
        m_RenderThread.on(JCWorkerThread::Event_threadStop, std::bind(&GLEnvThread::onGLExit,this, std::placeholders::_1));
        m_RenderThread.setLoopFunc(std::bind(&gles::renderLoop,m_pGLES));
        m_RenderThread.setThreadName("GLRender");
        m_RenderThread.start();
    }

    void GLEnvThread::stop(){
        m_RenderThread.stop();
    }

    void GLEnvThread::onResume(){
        if(!m_pGLES){
            m_pGLES = new gles(&m_RenderThread);
            m_RenderThread.on(JCWorkerThread::Event_threadStart, std::bind(&GLEnvThread::onGLStart,this, std::placeholders::_1));//在线程开始的时候初始化GL环境。
            m_RenderThread.on(JCWorkerThread::Event_threadStop, std::bind(&GLEnvThread::onGLExit,this, std::placeholders::_1));
            m_RenderThread.setLoopFunc(std::bind(&gles::renderLoop,m_pGLES));
            m_RenderThread.setThreadName("GLRender");
            m_RenderThread.start();
        }
    }

    void GLEnvThread::onPause(){
        m_RenderThread.stop();
        if(m_pGLES){
            delete m_pGLES;
            m_pGLES = nullptr;
        }
    }

    void GLEnvThread::onGLStart(JCEventEmitter::evtPtr evt){
        m_pGLES->initFromWindow(*m_pWindows);
        m_pGLES->setRenderer(m_pRenderer);
        if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
        {
            JCScriptRuntime::s_JSRT->start(JCConch::s_pConch->m_strStartJS.c_str());
        }
    }

    void GLEnvThread::onGLExit(JCEventEmitter::evtPtr evt){
        if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
        {
            JCScriptRuntime::s_JSRT->stop();
        }
    }

    void GLEnvThread::onWindowClose(){
        m_RenderThread.stop();
    }
};