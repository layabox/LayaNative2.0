/**
@file			JCConchRender.h
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#ifndef __JCConchRender_H__
#define __JCConchRender_H__

#include <JCIGLRender.h>
#include <fontMgr/JCFreeTypeRender.h>
#include <resource/JCFileResManager.h>
#include <misc/JCWorkSemaphore.h>
#include <Performance/JCPerfHUD.h>
#include <Manager/JCOrderResManager.h>
#include <LayaGL/JCLayaGL.h>
#include "RenderEx/JCRegister.h"
#include <misc/JCWorkerThread.h>
#ifdef __APPLE__
#include "IOSFreetype/JCIOSFreeType.h"
#endif
#include <atomic>

namespace laya
{
    enum GL_CAPS
    {
        GLC_NONE = 0,
        GLC_TEXTURE_COMPRESSION_PVR = 1 << 1,
        GLC_TEXTURE_COMPRESSION_ETC1 = 1 << 2,
        GLC_TEXTURE_COMPRESSION_ETC2 = 1 << 3,
        GLC_NOPT = 1 << 4,
        GLC_TEXTURE_TPG = 1 << 5,
        GLC_INSTANCEING = 1 << 6,
    };

    class JCFileSource;
    class JCConchRender : public JCIGLRender
    {
    public:
        JCConchRender(void* pFileResManager,JCArrayBufferManager* pArrayBufferManager, JCRegister* pRegister,JCWebGLPlus* pWebGLPlus);
        ~JCConchRender();
        void initOpenGLES();
        void onGLReady();
        void setRenderData(JCArrayBufferManager* pJSArrayBufferManger, JCArrayBufferManager::ArrayBufferContent* pSyncBufferList,int nSyncCount, JCCommandEncoderBuffer*& pRenderCmd,double& fDelay,double& fFPS);
        void setInterruptFunc(std::function<void(void)> pFunc);
        void syncArrayBuffer(JCArrayBufferManager* pManager, JCArrayBufferManager::ArrayBufferContent* pSyncBufferList,int nSyncCount);
        void syncDeleteArrayBuffer(JCArrayBufferManager* pJSManager);
        int renderFrame(long nCurrentFrame, bool bStopRender);
        void clearAllData();
        void willExit();
        void setAssetRes(JCFileSource* pAssetRes);
        void onTouchStart(double fTime);
    private:
        void _clearAllData();
    public:
        JCWorkerThread*                         m_pRenderThread;
        float				                    m_fShowPerfScale;               ///<是否显示性能测试
        long				                    m_nFrameCount;                  ///<frameCount
        bool                                    m_bStopRender;                  ///<这个变量仅仅为了IOS使用的，因为IOS主线程（UI线程）和渲染线程是一个，所以增加了这个标记
        bool                                    m_bExit;                        ///<是否退出
        JCImageManager*                         m_pImageManager;                ///<Image管理器
        JCIDGenerator*                          m_pIDGenerator;
        JCIDGenerator*                          m_pProgramLocationTable;
        JCLayaGL*                               m_pLayaGL;                      ///<layaGL
        JCFileResManager*                       m_pFileResManager;              ///<FileResManager 外部设置的
        JCWorkSemaphore		                    m_kRenderSem;                   ///<与js线程同步的对象。
        JCPerfDataRender                        m_kPerfRender;                  ///<性能测试
        JCArrayBufferManager*                   m_pArrayBufferManager;          ///<ArrayBufferManager
        JCRegister*                             m_pRegister;
        JCCommandEncoderBuffer*                 m_pCurrentRenderCmds;           ///<渲染指令流
    private:
        JCFileSource*                           m_pAssetsRes;                   ///<读取文件用的fileSource
        double                                  m_nFPS;
        double                                  m_nCountDelayTime;
        double                                  m_nDelayTime;
        std::function<void(void)>               m_pInterruptFunc;
        float                                   m_fMouseMoveTime;
#ifndef __APPLE__
        std::atomic<bool>                       m_bClearAllData{ false };
#endif
    };
}
//------------------------------------------------------------------------------
#endif //__JCConchRender_H__

//-----------------------------END FILE--------------------------------