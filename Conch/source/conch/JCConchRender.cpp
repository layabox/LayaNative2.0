/**
@file			JCConchRender.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#include "JCConchRender.h"
#include <util/Log.h>
#include <util/JCCommonMethod.h>
#include "JCSystemConfig.h"
#include <downloadCache/JCFileSource.h>
#include <downloadCache/JCServerFileCache.h>
#include "JCScriptRuntime.h"
#include "JSWrapper/LayaWrap/JSConchConfig.h"
#include "JCConch.h"
#include "JSWrapper/LayaWrap/JSLayaGL.h"
#include <LayaGL/JCLayaGLDispatch.h>

extern int g_nInnerHeight;
extern int g_nInnerWidth;
int s_nGLCaps = laya::GLC_NONE;
namespace laya
{
   
	JCConchRender::JCConchRender(void* pFileResManager, JCArrayBufferManager* pArrayBufferManager, JCRegister* pRegister, JCWebGLPlus* pWebGLPlus)
	{
        m_pRenderThread = NULL;
        m_fMouseMoveTime = 0.0f;
        m_pAssetsRes = NULL;
        m_bExit = false;
        m_nFrameCount = 0;
		m_fShowPerfScale = 0;
        m_bStopRender = false;
        m_pImageManager = new JCImageManager();
        m_pArrayBufferManager = pArrayBufferManager;
        if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_DOUBLE)
        {
            m_pRegister = new JCRegister();
        }
        else
        {   
            m_pRegister = pRegister;
        }
        m_pIDGenerator = new JCIDGenerator();
        m_pProgramLocationTable = new JCIDGenerator();
        m_pIDGenerator->reset();
        m_pProgramLocationTable->reset();
        m_pLayaGL = new JCLayaGL(g_nInnerWidth, g_nInnerHeight,m_pArrayBufferManager,m_pImageManager, m_pIDGenerator, m_pProgramLocationTable,m_pRegister,pWebGLPlus);
        JCLayaGLDispatch::ms_pLayaGL = m_pLayaGL;
        m_pFileResManager = (JCFileResManager*)pFileResManager;
        m_nFPS = 0;
        m_nDelayTime = 0;
        m_nCountDelayTime = 0;
#ifndef __APPLE__
        m_bClearAllData = false;
#endif
        if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_DOUBLE)
        {
            m_pCurrentRenderCmds = new JCCommandEncoderBuffer(102400, 1280);
        }
        else
        {
            m_pCurrentRenderCmds = NULL;
        }
	}
	JCConchRender::~JCConchRender()
	{
        //这个是在JCConch中传过来的，但是为了确保最后一帧，在这释放了，因为JCConchRender全局只有一个
        if (m_pFileResManager)
        {
            //这个fileCache也是同理，需要在这删除，
            if (m_pFileResManager->m_pFileCache)
            {
                delete m_pFileResManager->m_pFileCache;
                m_pFileResManager->m_pFileCache = NULL;
            }
            delete m_pFileResManager;
            m_pFileResManager = NULL;
        }
        if (m_pImageManager)
        {
            delete m_pImageManager;
            m_pImageManager = NULL;
        }
        if (m_pLayaGL)
        {
            delete m_pLayaGL;
            m_pLayaGL = NULL;
        }
        m_pArrayBufferManager = NULL;
        if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_DOUBLE)
        {
            if (m_pRegister)
            {
                delete m_pRegister;
                m_pRegister = NULL;
            }
        }
        if (m_pIDGenerator)
        {
            delete m_pIDGenerator;
            m_pIDGenerator = NULL;
        }
        if (m_pProgramLocationTable)
        {
            delete m_pProgramLocationTable;
            m_pProgramLocationTable = NULL;
        }
        if (m_pCurrentRenderCmds)
        {
            delete m_pCurrentRenderCmds;
            m_pCurrentRenderCmds = NULL;
        }
	}
    void JCConchRender::setAssetRes(JCFileSource* pAssetRes)
    {
        m_pAssetsRes = pAssetRes;
    }
    void JCConchRender::initOpenGLES() 
    {
        const char* pstrVersion = (const char*)glGetString(GL_VERSION);
        LOGI("OpenGL ES version [%s]", pstrVersion);
        s_nGLCaps |= GLC_TEXTURE_TPG;
        char* pStrExt = (char *)glGetString(GL_EXTENSIONS);
        if (pStrExt != nullptr) 
        {
            if (strstr(pStrExt, "GL_IMG_texture_compression_pvrtc")) 
            {
                s_nGLCaps |= GLC_TEXTURE_COMPRESSION_PVR;
            }
            if (strstr(pstrVersion, "OpenGL ES 3.")) 
            {
                s_nGLCaps |= GLC_NOPT;
#ifdef ANDROID
                s_nGLCaps |= GLC_TEXTURE_COMPRESSION_ETC1;
                s_nGLCaps |= GLC_TEXTURE_COMPRESSION_ETC2;
                s_nGLCaps |= GLC_INSTANCEING;
#endif
            }

#ifndef ANDROID
            s_nGLCaps |= GLC_INSTANCEING;
#endif
#ifdef GL_ETC1_RGB8_OES
            if (strstr(pStrExt, "GL_OES_compressed_ETC1_RGB8_texture")) 
            {
                s_nGLCaps |= GLC_TEXTURE_COMPRESSION_ETC1;
            }
#endif
        }
    }
	void JCConchRender::onGLReady()
	{
        initOpenGLES();
	}
    void JCConchRender::clearAllData()
    {
#ifdef __APPLE__
        _clearAllData();
#else
        m_bClearAllData = true;
#endif
    }
	void JCConchRender::_clearAllData()
	{
        LOGI(">>>JCConchRender::clearAllData =%d", std::this_thread::get_id());
        m_kPerfRender.invalidGLRes();
        m_pLayaGL->deleteAllGLRes();
        //图片全部清空
        if (m_pImageManager)
        {
            m_pImageManager->resetRenderThread();
        }
        m_pArrayBufferManager->clearAll();
        m_pIDGenerator->reset();
        m_pProgramLocationTable->reset();
        m_bStopRender = false;
#ifndef __APPLE__
        m_bClearAllData = false;
#endif
	}
    void JCConchRender::syncArrayBuffer(JCArrayBufferManager* pJSManager, JCArrayBufferManager::ArrayBufferContent* pSyncBufferList,int nSyncCount)
    {
        if (nSyncCount <= 0)return;
        int nSize = pJSManager->m_vBuffers.size();
        if ((size_t)nSize > m_pArrayBufferManager->m_vBuffers.size())
        {
            m_pArrayBufferManager->m_vBuffers.resize(nSize);
        }
        int* pBuffer = (int*)pSyncBufferList->m_pBuffer;
        for (int i = 0; i < nSyncCount;i++)
        {
            int nIndex = pBuffer[i];
            JCArrayBufferManager::ArrayBufferContent* pBuffer1 = pJSManager->m_vBuffers[nIndex];
            if (pBuffer1)
            {
                if (m_pArrayBufferManager->m_vBuffers[nIndex] == NULL)
                {
                    m_pArrayBufferManager->m_vBuffers[nIndex] = new JCArrayBufferManager::ArrayBufferContent(pBuffer1->m_nType,true);
                }
                m_pArrayBufferManager->m_vBuffers[nIndex]->syncContent(pBuffer1);
            }
            else
            {
                JCArrayBufferManager::ArrayBufferContent* pBuffer2 = m_pArrayBufferManager->m_vBuffers[nIndex];
                if (pBuffer2)
                {
                    delete pBuffer2;
                    m_pArrayBufferManager->m_vBuffers[nIndex] = NULL;
                }
            }
        }
        /*
        for (int i = 0; i < nSize;i++)
        {
            int nIndex = i;
            JCArrayBufferManager::ArrayBufferContent* pBuffer1 = pJSManager->m_vBuffers[nIndex];
            if (pBuffer1)
            {
                if (m_pArrayBufferManager->m_vBuffers[nIndex] == NULL)
                {
                    m_pArrayBufferManager->m_vBuffers[nIndex] = new JCArrayBufferManager::ArrayBufferContent(pBuffer1->m_nType,true);
                }
                m_pArrayBufferManager->m_vBuffers[nIndex]->syncContent(pBuffer1);
            }
            else
            {
                JCArrayBufferManager::ArrayBufferContent* pBuffer2 = m_pArrayBufferManager->m_vBuffers[nIndex];
                if (pBuffer2)
                {
                    delete pBuffer2;
                    m_pArrayBufferManager->m_vBuffers[nIndex] = NULL;
                }
            }
        }
        */
    }
    void JCConchRender::syncDeleteArrayBuffer(JCArrayBufferManager* pJSManager)
    {
        if (pJSManager->m_vPrepareDelIDs.size() <= 0)return;
        std::vector<int>& vDeleteLists = pJSManager->m_vPrepareDelIDs;
        int nSize = vDeleteLists.size();
        for (int i = 0; i < nSize; i++)
        {
            int nIndex = vDeleteLists[i];
            pJSManager->removeArrayBuffer(nIndex);
            JCArrayBufferManager::ArrayBufferContent* pBuffer2 = m_pArrayBufferManager->m_vBuffers[nIndex];
            if (pBuffer2)
            {
                delete pBuffer2;
                m_pArrayBufferManager->m_vBuffers[nIndex] = NULL;
            }
        }
        vDeleteLists.clear();
    }
    void JCConchRender::setRenderData(JCArrayBufferManager* pJSArrayBufferManger, JCArrayBufferManager::ArrayBufferContent* pSyncBufferList, int nSyncCount, JCCommandEncoderBuffer*& pRenderCmd,double& fDelay, double& fFPS)
    {
        if (pJSArrayBufferManger == NULL)
        {
            m_kRenderSem.waitUntilNoData();
            if (pRenderCmd)
            {
                std::swap(pRenderCmd, m_pCurrentRenderCmds);
            }
            fDelay = m_nDelayTime;
            fFPS = m_nFPS;
            m_kRenderSem.setDataNum(1);
        }
        else
        {
            //开启线程锁
            m_kRenderSem.waitUntilNoData();
            //double nTime = tmGetCurms();
            syncArrayBuffer(pJSArrayBufferManger, pSyncBufferList, nSyncCount);
            syncDeleteArrayBuffer(pJSArrayBufferManger);
            //double nSpace = tmGetCurms() - nTime;
            //LOGI("syncArrayBuffer time space=%f",nSpace);

            if (pRenderCmd)
            {
                std::swap(pRenderCmd, m_pCurrentRenderCmds);
            }
            fDelay = m_nDelayTime;
            fFPS = m_nFPS;
            m_kRenderSem.setDataNum(1);
        }
    }
    void JCConchRender::setInterruptFunc(std::function<void(void)> pFunc)
    {
        if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
        {
            pFunc();
        }
        else
        {
            m_kRenderSem.waitUntilNoData();
            m_pInterruptFunc = pFunc;
            m_kRenderSem.setDataNum(2);
            m_kRenderSem.waitUntilNoData();
        }
    }
    void onJSUpdate(void* pThis)
    {
        if (!JCConch::s_pConchRender->m_bStopRender)
        {
            JCScriptRuntime* pScriptRuntime = (JCScriptRuntime*)pThis;
            pScriptRuntime->onUpdate();
        }
    }
#ifdef __APPLE__
    int JCConchRender::renderFrame( long nCurrentFrame, bool bStopRender)
    {
        if(g_kSystemConfig.m_nThreadMODE == THREAD_MODE_DOUBLE)
        {
            if (m_bExit)return 0;
            PERF_INITVAR(nTime123);
            /*
            if (!m_bStopRender)
            {
                if (JCScriptRuntime::s_JSRT)
                {
                    JCScriptRuntime::s_JSRT->m_ScriptThread.post(std::bind(&JCScriptRuntime::onUpdate,JCScriptRuntime::s_JSRT));
                }
            }
            */
            if (!m_bStopRender)
            {
                //线程等待
                while (true)
                {
                    m_kRenderSem.waitUntilHasData();
                    if (m_kRenderSem.getDataNum() == 2)
                    {
                        m_pInterruptFunc();
                        m_kRenderSem.setDataNum(0);
                    }
                    else
                    {
                        break;
                    }
                }
            }
            //计算FPS
            //------------------------------------------------------------------------------
            m_nFrameCount++;
            PERF_UPDATE_DTIME(JCPerfHUD::PHUD_FRAME_DELAY, JCPerfHUD::m_tmCurRender, JCPerfHUD::m_tmDelayTime);
            m_nCountDelayTime += JCPerfHUD::m_tmDelayTime;
            if (m_nFrameCount % 10 == 0)
            {
                m_nFPS = (10000 / m_nCountDelayTime);
                m_nDelayTime = m_nCountDelayTime / 10;
                //LOGI(">>>FPS=%.2f,DELAY=%.2f", m_nFPS, m_nDelayTime);
                m_nCountDelayTime = 0;
            }

            //开始渲染
            //------------------------------------------------------------------------------
            PERF_INITVAR(nTime);
            //开始解析
            JCLayaGLDispatch::dispatchAllCmds(m_pCurrentRenderCmds);
            m_pCurrentRenderCmds->clearData();

            //imageManager的update 不需要每帧都调用
            if (m_nFrameCount % 60 == 0)
            {
                m_pImageManager->update(m_nFrameCount);
            }
            //性能测试
            PERF_UPDATE_DATA(JCPerfHUD::PHUD_RENDER_DELAY, (float)(tmGetCurms() - nTime));
            //------------------------------------------------------------------------------

            m_kRenderSem.setDataNum(0);
            if (m_fShowPerfScale != 0)
            {
                m_kPerfRender.drawData();
            }
            FRAME_TYPE nFrameType = (g_kSystemConfig.m_nFrameType == FT_MOUSE) ? (nTime123 - m_fMouseMoveTime < g_kSystemConfig.m_nFrameThreshold ? FT_FAST : FT_SLOW) : g_kSystemConfig.m_nFrameType;
            if (nFrameType == FT_SLOW)
            {
                int64_t nSleepTime = g_kSystemConfig.m_nSleepTime - (tmGetCurms() - nTime123);
                if (nSleepTime > 0)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(nSleepTime));
                }
            }
            return 0;
        }
        else
        {
            if (m_bExit)return 0;
            PERF_INITVAR(nTime123);
            if (JCScriptRuntime::s_JSRT && !m_bStopRender)
            {
                JSThreadInterface* pScriptThread = JCScriptRuntime::s_JSRT->m_pScriptThread;
                pScriptThread->run(NULL, NULL);
            }
            //计算FPS
            //------------------------------------------------------------------------------
            m_nFrameCount++;
            PERF_UPDATE_DTIME(JCPerfHUD::PHUD_FRAME_DELAY, JCPerfHUD::m_tmCurRender, JCPerfHUD::m_tmDelayTime);
            m_nCountDelayTime += JCPerfHUD::m_tmDelayTime;
            if (m_nFrameCount % 10 == 0)
            {
                m_nFPS = (10000 / m_nCountDelayTime);
                m_nDelayTime = m_nCountDelayTime / 10;
                //LOGI(">>>FPS=%.2f,DELAY=%.2f", m_nFPS, m_nDelayTime);
                m_nCountDelayTime = 0;
            }

            //开始渲染
            //------------------------------------------------------------------------------
            PERF_INITVAR(nTime);
            //imageManager的update 不需要每帧都调用
            if (m_nFrameCount % 60 == 0)
            {
                m_pImageManager->update(m_nFrameCount);
            }
            //性能测试
            PERF_UPDATE_DATA(JCPerfHUD::PHUD_RENDER_DELAY, (float)(tmGetCurms() - nTime));
            //------------------------------------------------------------------------------

            if (m_fShowPerfScale != 0)
            {
                m_kPerfRender.drawData();
            }
            FRAME_TYPE nFrameType = (g_kSystemConfig.m_nFrameType == FT_MOUSE) ? (nTime123 - m_fMouseMoveTime < g_kSystemConfig.m_nFrameThreshold ? FT_FAST : FT_SLOW) : g_kSystemConfig.m_nFrameType;
            if (nFrameType == FT_SLOW)
            {
                int64_t nSleepTime = g_kSystemConfig.m_nSleepTime - (tmGetCurms() - nTime123);
                if (nSleepTime > 0)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(nSleepTime));
                }
            }
            return 0;
        }
    }
#else
	int JCConchRender::renderFrame( long nCurrentFrame, bool bStopRender)
	{
        if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_DOUBLE)
        {
            PERF_INITVAR(nTime123);
			if (m_bClearAllData)
			{
				_clearAllData();
				m_bClearAllData = false;
			}
            if (JCScriptRuntime::s_JSRT)
            {
                JCScriptRuntime::s_JSRT->m_pScriptThread->post(std::bind(&JCScriptRuntime::onUpdate, JCScriptRuntime::s_JSRT));
            }
            if (m_bExit)return 0;
            //线程等待
            while (true)
            {
                m_kRenderSem.waitUntilHasData();
                if (m_kRenderSem.getDataNum() == 2)
                {
                    m_pInterruptFunc();
                    m_kRenderSem.setDataNum(0);
                }
                else
                {
                    break;
                }
            }

            //计算FPS相关的
            //------------------------------------------------------------------------------
            m_nFrameCount++;
            PERF_UPDATE_DTIME(JCPerfHUD::PHUD_FRAME_DELAY, JCPerfHUD::m_tmCurRender, JCPerfHUD::m_tmDelayTime);
            m_nCountDelayTime += JCPerfHUD::m_tmDelayTime;
            if (m_nFrameCount % 10 == 0)
            {
                m_nFPS = (10000 / m_nCountDelayTime);
                m_nDelayTime = m_nCountDelayTime / 10;
                //LOGI(">>>FPS=%.2f,DELAY=%.2f", m_nFPS, m_nDelayTime);
                m_nCountDelayTime = 0;
            }

            //开始渲染
            //------------------------------------------------------------------------------
            PERF_INITVAR(nTime);


            //开始解析
            JCLayaGLDispatch::dispatchAllCmds(m_pCurrentRenderCmds);
            m_pCurrentRenderCmds->clearData();

            //imageManager的update 不需要每帧都调用
            m_pImageManager->update(m_nFrameCount);
            //性能测试
            PERF_UPDATE_DATA(JCPerfHUD::PHUD_RENDER_DELAY, (float)(tmGetCurms() - nTime));
            //------------------------------------------------------------------------------

            m_kRenderSem.setDataNum(0);
            if (m_fShowPerfScale != 0)
            {
                m_kPerfRender.drawData();
            }
            FRAME_TYPE nFrameType = (g_kSystemConfig.m_nFrameType == FT_MOUSE) ? (nTime123 - m_fMouseMoveTime < g_kSystemConfig.m_nFrameThreshold ? FT_FAST : FT_SLOW) : g_kSystemConfig.m_nFrameType;
            if (nFrameType == FT_SLOW)
            {
                int64_t nSleepTime = g_kSystemConfig.m_nSleepTime - (int)((tmGetCurms() - nTime123));
                if (nSleepTime > 0)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(nSleepTime));
                }
            }
            return 0;
        }
        else
        {
            PERF_INITVAR(nTime123);
			if (m_bClearAllData)
			{
				_clearAllData();
				m_bClearAllData = false;
			}
            if (JCScriptRuntime::s_JSRT->m_bReload == true)
            {
                JCScriptRuntime::s_JSRT->reload();
            }
            if (JCScriptRuntime::s_JSRT && !m_bStopRender)
            {
                JSThreadInterface* pScriptThread = JCScriptRuntime::s_JSRT->m_pScriptThread;
                pScriptThread->run(onJSUpdate, JCScriptRuntime::s_JSRT);
                //pScriptThread->post(std::bind(&JCScriptRuntime::onUpdate, JCScriptRuntime::s_JSRT));
                //pScriptThread->run(NULL, NULL);
            }
            if (m_bExit)return 0;
            //计算FPS相关的
            //------------------------------------------------------------------------------
            m_nFrameCount++;
            PERF_UPDATE_DTIME(JCPerfHUD::PHUD_FRAME_DELAY, JCPerfHUD::m_tmCurRender, JCPerfHUD::m_tmDelayTime);
            m_nCountDelayTime += JCPerfHUD::m_tmDelayTime;
            if (m_nFrameCount % 10 == 0)
            {
                m_nFPS = (10000 / m_nCountDelayTime);
                m_nDelayTime = m_nCountDelayTime / 10;
                //LOGI(">>>FPS=%.2f,DELAY=%.2f", m_nFPS, m_nDelayTime);
                m_nCountDelayTime = 0;
            }
            //开始渲染
            //------------------------------------------------------------------------------
            PERF_INITVAR(nTime);

            //imageManager的update 不需要每帧都调用
            m_pImageManager->update(m_nFrameCount);
            //性能测试
            PERF_UPDATE_DATA(JCPerfHUD::PHUD_RENDER_DELAY, (float)(tmGetCurms() - nTime));
            //------------------------------------------------------------------------------
            if (m_fShowPerfScale != 0)
            {
                m_kPerfRender.drawData();
            }
            FRAME_TYPE nFrameType = (g_kSystemConfig.m_nFrameType == FT_MOUSE) ? (nTime123 - m_fMouseMoveTime < g_kSystemConfig.m_nFrameThreshold ? FT_FAST : FT_SLOW) : g_kSystemConfig.m_nFrameType;
            if (nFrameType == FT_SLOW)
            {
                int64_t nSleepTime = g_kSystemConfig.m_nSleepTime - (int)((tmGetCurms() - nTime123));
                if (nSleepTime > 0)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(nSleepTime));
                }
            }
            return 0;
        }
	}
#endif

	void JCConchRender::willExit()
	{
        LOGI("render will exit");
        m_bExit = true;
		m_kRenderSem.notifyAllWait();
	}
    void JCConchRender::onTouchStart(double fTime)
    {
        m_fMouseMoveTime = fTime;
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
