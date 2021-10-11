/**
@file			JCConch.cpp
@brief			
@author			James
@version		1.0
@date			2017_11_28
*/

#include "JCConch.h"
#include <algorithm>
#include <util/Log.h>
#include <util/JCCommonMethod.h>
#include <fileSystem/JCFileSystem.h>
#include <downloadCache/JCFileSource.h>
#include <resource/JCFileResManager.h>
#include "JCConch.h"
#include "JSWrapper/JSInterface/JSInterface.h"
#include "JSWrapper/LayaWrap/JSFileReader.h"
#include "JSWrapper/LayaWrap/JSGlobalExportCFun.h"
#include "JCScriptRuntime.h"
#include <downloadMgr/JCDownloadMgr.h>
#include "JCThreadCmdMgr.h"
#include "JCSystemConfig.h"
#include <LayaGL/JCLayaGL.h>
#include <webglplus/JCWebGLPlus.h>
#ifdef JS_V8
    #include "JSWrapper/v8debug/debug-agent.h"
#endif
#ifdef ANDROID
    #include "WebSocket/WebSocket.h"
    #include "CToJavaBridge.h"
    #include <dlfcn.h>
    #include <pthread.h>
    #include <sys/types.h>
    #include <unistd.h>
#elif __APPLE__
    #include "CToObjectC.h"
    #include "pthread.h"
#elif _WIN32
    #include <windows.h>
#endif

std::string gRedistPath = "";
std::string gResourcePath = "";
#ifdef __APPLE__
    std::string gAssetRootPath = "";
#endif
int g_nInnerWidth = 1024;
int g_nInnerHeight = 768;
bool g_bGLCanvasSizeChanged = false;


namespace laya
{
    extern JCWorkerThread* g_DecThread;
    JCConch* JCConch::s_pConch = NULL;
    int64_t JCConch::s_nUpdateTime = 0;
    JCFileSource* JCConch::s_pAssetsFiles = NULL;
    std::shared_ptr<JCConchRender> JCConch::s_pConchRender;
    void _vibrate()
    {
#ifdef ANDROID
        CToJavaBridge::JavaRet kRet;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "vibrate", kRet);
#endif
    }
    JCConch::JCConch(int nDownloadThreadNum, JS_DEBUG_MODE nJSDebugMode, int nJSDebugPort)
    {
        s_pConch = this;
#ifdef __APPLE__
        pthread_key_create(&JCWorkerThread::s_tls_curThread, NULL);
        pthread_key_create(&s_tls_curDataThread, NULL);
        
#ifdef JS_JSC
        pthread_key_create(&__TlsData::s_tls_curThread, NULL);
        pthread_key_create(&JSClassMgr::s_tls_curThread, NULL);
#endif
#elif _WIN32
        HMODULE libHandle = LoadLibrary("libGLESv2.dll");
#elif ANDROID
        //void *libhandle = dlopen("libGLESv2.so", RTLD_LAZY);
#endif
        m_nUrlHistoryPos = -1;
        m_sCachePath = gRedistPath + "/appCache";
        g_DecThread = new JCWorkerThread(true);
        g_DecThread->setThreadName("image decode");
        JCDownloadMgr* pdmgr = JCDownloadMgr::getInstance();
        LOGI("download thread num = %d", nDownloadThreadNum);
        pdmgr->init(nDownloadThreadNum);
        m_pFileResMgr = new JCFileResManager(pdmgr);
        JCWebGLPlus::getInstance()->init(g_kSystemConfig.m_nThreadMODE);
        m_pScrpitRuntime = new JCScriptRuntime();
		s_pConchRender.reset(new JCConchRender(m_pFileResMgr,JCWebGLPlus::getInstance()->m_pRArrayBufferManager,m_pScrpitRuntime->m_pRegister,JCWebGLPlus::getInstance()));

        //------------------------------------------------------------------------------
        //绑定函数
        JCWebGLPlus* pWebGLPlus = JCWebGLPlus::getInstance();
        JCLayaGL* pLayaGL = s_pConchRender->m_pLayaGL;
        pWebGLPlus->uniform1f = std::bind(&JCLayaGL::uniform1f,pLayaGL, std::placeholders::_1, std::placeholders::_2);
        pWebGLPlus->uniform1fv = std::bind(&JCLayaGL::uniform1fv, pLayaGL, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        pWebGLPlus->uniform1i = std::bind(&JCLayaGL::uniform1i, pLayaGL, std::placeholders::_1, std::placeholders::_2);
        pWebGLPlus->uniform1iv = std::bind(&JCLayaGL::uniform1iv, pLayaGL, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        pWebGLPlus->uniform2f = std::bind(&JCLayaGL::uniform2f, pLayaGL, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        pWebGLPlus->uniform2fv = std::bind(&JCLayaGL::uniform2fv, pLayaGL, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        pWebGLPlus->uniform2i = std::bind(&JCLayaGL::uniform2i, pLayaGL, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        pWebGLPlus->uniform2iv = std::bind(&JCLayaGL::uniform2iv, pLayaGL, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        pWebGLPlus->uniform3f = std::bind(&JCLayaGL::uniform3f, pLayaGL, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,std::placeholders::_4);
        pWebGLPlus->uniform3fv = std::bind(&JCLayaGL::uniform3fv, pLayaGL, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        pWebGLPlus->uniform3i = std::bind(&JCLayaGL::uniform3i, pLayaGL,std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,std::placeholders::_4);
        pWebGLPlus->uniform3iv = std::bind(&JCLayaGL::uniform3iv, pLayaGL, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        pWebGLPlus->uniform4f = std::bind(&JCLayaGL::uniform4f, pLayaGL, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
        pWebGLPlus->uniform4fv = std::bind(&JCLayaGL::uniform4fv, pLayaGL, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        pWebGLPlus->uniform4i = std::bind(&JCLayaGL::uniform4i, pLayaGL,std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
        pWebGLPlus->uniform4iv = std::bind(&JCLayaGL::uniform4iv, pLayaGL, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        pWebGLPlus->uniformMatrix2fv = std::bind(&JCLayaGL::uniformMatrix2fv, pLayaGL,std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        pWebGLPlus->uniformMatrix3fv = std::bind(&JCLayaGL::uniformMatrix3fv, pLayaGL,std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        pWebGLPlus->uniformMatrix4fv = std::bind(&JCLayaGL::uniformMatrix4fv, pLayaGL,std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        pWebGLPlus->activeTexture = std::bind(&JCLayaGL::activeTexture, pLayaGL,std::placeholders::_1);
        pWebGLPlus->bindTexture = std::bind(&JCLayaGL::bindTexture, pLayaGL,std::placeholders::_1, std::placeholders::_2);
        //------------------------------------------------------------------------------

        m_pAssetsRes = JCConch::s_pAssetsFiles;
        s_pConchRender->setAssetRes(m_pAssetsRes);
        m_strStartJS = "scripts/apploader.js";
        if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_DOUBLE)
        {
            m_pScrpitRuntime->init(m_pFileResMgr, m_pAssetsRes, &m_ThreadCmdMgr);
            m_pFileResMgr->m_pCmdPoster = &m_ThreadCmdMgr;
        }
        else
        {
            m_pScrpitRuntime->init(m_pFileResMgr, m_pAssetsRes, m_pScrpitRuntime);
            m_pFileResMgr->m_pCmdPoster = m_pScrpitRuntime;
        }
        m_nJSDebugMode = nJSDebugMode;
        m_nJSDebugPort = nJSDebugPort;
#ifdef JS_V8_DEBUGGER
        m_pDbgAgent = NULL;
        if (m_nJSDebugMode != JS_DEBUG_MODE_OFF)
        {
            LOGI("open js debug port at %d", m_nJSDebugPort);
            m_pDbgAgent = new DebuggerAgent("layabox", m_nJSDebugPort);
            m_pScrpitRuntime->m_pDbgAgent = m_pDbgAgent;
        }
        else 
        {
            m_pDbgAgent = NULL;
			m_pScrpitRuntime->m_pDbgAgent = NULL;
        }
#endif
	}

	JCConch::~JCConch() 
    {
		s_pConchRender.reset();
		s_pConch = NULL;
        if (m_pScrpitRuntime) 
        {
            delete m_pScrpitRuntime;
            m_pScrpitRuntime = NULL;
        }
#ifdef JS_V8_DEBUGGER
        if (m_pDbgAgent) 
        {
            m_pDbgAgent->Shutdown();
            delete m_pDbgAgent;
            m_pDbgAgent = NULL;
        }
#endif
	}
	void JCConch::onAppStart() 
    {
        m_strLocalStoragePath = gRedistPath + "/localstorage/";
        try
        {
            fs::create_directories(m_strLocalStoragePath);
        }
        catch (...)
        {
            if (global_onCreateFileError)
            {
                global_onCreateFileError();
            }
            return;
        }
#ifdef __APPLE__
        if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_DOUBLE)
        {
            m_ThreadCmdMgr.regThread(JCThreadCmdMgr::JS, m_pScrpitRuntime->m_pScriptThread->getWorker());
        }
        //因为iOS界面和渲染是一个线程，所以即使单线程也可以在这启动
        m_pScrpitRuntime->start(m_strStartJS.c_str());
#else
        if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_DOUBLE)
        {
            m_ThreadCmdMgr.regThread(JCThreadCmdMgr::JS, m_pScrpitRuntime->m_pScriptThread->getWorker());
            m_pScrpitRuntime->start(m_strStartJS.c_str());
        }
#endif
	}
    void JCConch::reload() 
    {
        LOGI("JCConch::reload start...");
        //先通知消息管理器，关闭各个线程之间的post
        m_ThreadCmdMgr.stop();
#ifdef __APPLE__
        m_pScrpitRuntime->reload();
        m_ThreadCmdMgr.start();
        if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_DOUBLE)
        {
            m_ThreadCmdMgr.regThread(JCThreadCmdMgr::JS, m_pScrpitRuntime->m_pScriptThread->getWorker());
        }
#else
        if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_DOUBLE)
        {
            m_pScrpitRuntime->reload();
            m_ThreadCmdMgr.start();
            m_ThreadCmdMgr.regThread(JCThreadCmdMgr::JS, m_pScrpitRuntime->m_pScriptThread->getWorker());
        }
        else
        {
            m_pScrpitRuntime->m_bReload = true;
        }
#endif
        LOGI("JCConch::reload end.");
    }
	void JCConch::delAppRes() 
    {
	}
	void JCConch::onClearMemory() 
    {
	}
	void JCConch::onAppDestory() 
    {
        //先通知消息管理器，关闭各个线程之间的post
        m_ThreadCmdMgr.stop();
        //渲染要继续
        JCConch::s_pConchRender->willExit();
        //关闭解码线程
        delete g_DecThread;
        //如果是单线程  在渲染线程析构了
        if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_DOUBLE)
        {
            if (m_pScrpitRuntime)
            {
                delete m_pScrpitRuntime;
                m_pScrpitRuntime = NULL;
            }
        }
        //关闭下载线程
        //删除所有的下载任务
        JCDownloadMgr* pNetLoader = JCDownloadMgr::getInstance();
        pNetLoader->stopCurTask();
        pNetLoader->clearAllAsyncTask();
        JCDownloadMgr::delInstance();
    }
	void JCConch::postCmdToMainThread(int nCmd, int nParam1, int nParam2) 
    {
#ifdef __APPLE__
		CToObjectCPostMainThread(nCmd, nParam1, nParam2);
#else
		m_funcPostMsgToMainThread(nCmd, nParam1, nParam2);
#endif
	}
    int JCConch::urlHistoryLength() 
    {
        return m_vUrlHistory.size();
    }
    void JCConch::urlBack() 
    {
        urlGo(-1);
    }
    void JCConch::urlGo(int s) 
    {
#ifdef __APPLE__
        CToObjectCRunStopJSLoop();
#endif
        int sz = m_vUrlHistory.size();
        m_nUrlHistoryPos += s;
        if (m_nUrlHistoryPos >= sz) m_nUrlHistoryPos = sz - 1;
        if (m_nUrlHistoryPos < 0) m_nUrlHistoryPos = 0;
        if ((size_t)m_nUrlHistoryPos < m_vUrlHistory.size())
        {
            g_kSystemConfig.m_strStartURL = m_vUrlHistory[m_nUrlHistoryPos];
        }
        postCmdToMainThread(CMD_ReloadProcess, 0, 0);
    }
    void JCConch::urlForward() 
    {
        urlGo(1);
    }
    void JCConch::urlHistoryPush(const char* sUrl) 
    {
        m_nUrlHistoryPos++; //位置
        int nsz = m_nUrlHistoryPos+1; //希望的大小
        m_vUrlHistory.resize(nsz);
        m_vUrlHistory[m_nUrlHistoryPos] = sUrl;
        int sz = m_vUrlHistory.size();
        if (sz>1) 
        {
            if (m_vUrlHistory[sz - 1] == m_vUrlHistory[sz - 2]) 
            {
                m_vUrlHistory.resize(sz - 1);
                m_nUrlHistoryPos--;
            }
        }
    }
	void JCConch::onRunCmdInMainThread(int nCmd, int nParam1, int nParam2) 
    {
		switch (nCmd) 
        {
		case JCConch::CMD_ActiveProcess:
			break;
		case JCConch::CMD_DeactiveProcess:
			break;
		case JCConch::CMD_CloseProcess:
			break;
		case JCConch::CMD_ReloadProcess:
            reload();
			break;
		case JCConch::CMD_UrlBack:
            urlBack();
			break;
		case JCConch::CMD_UrlForward:
			break;
		case JCConch::CMD_onOrientationChanged:
			break;
        case CMD_ClearRender:
            if(JCConch::s_pConchRender)
            {
                JCConch::s_pConchRender->clearAllData();
            }
            break;
        case CMD_MgrStartThread:
            m_ThreadCmdMgr.start();
            break;
		}
	}
};
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
