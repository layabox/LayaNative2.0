/**
@file			JCScrpitRuntimeWASM.cpp
@brief			
@author			James
@version		1.0
@date			2017_9_25
*/

#include "JCScrpitRuntimeWASM.h"
#include <algorithm>
#include <util/Log.h>
#include "JCSystemConfig.h"
#include "JCConch.h"
//#include <Performance/JCPerfHUD.h>
#include "JCCmdDispatchManager.h"
#include <downloadMgr/JCDownloadMgr.h>
#include <inttypes.h>
#include <RenderCmd/CmdBase.h>
#include <RenderCmd/CmdFunction.h>

namespace laya 
{
    #define RENDERCMD_RESERVE_NUM 2048
    JCScriptRuntime* JCScriptRuntime::s_JSRT = NULL;
    JCScriptRuntime::JCScriptRuntime() 
    {
        s_JSRT = this;
        JCICmdDispatch::resetDispathGlobalID();
        m_pJSCmdBuffer = new JCMemClass(MEM_RENDER_COMMAND_SIZE, MEM_RENDER_COMMAND_SIZE);
        m_pJSCmdBuffer->setAlign(true);
        m_pTextManager = new JCTextManager(JCConch::s_pConchRender->m_pAtlasManager);
        m_pFontManager = new JCFontManager();
        m_pMeasureTextManager = new JCMeasureTextManager();
        m_pCmdDispathManager = new JCCmdDispathManager();
        m_nCountGroup = 0;
        m_nCountVertex = 0;
		m_nCountNode = 0;
        m_pRootHtml5Context = NULL;
        m_pRootNode = NULL;
        m_pFreeTypeRender = NULL;
        onThInit();
    }
    JCScriptRuntime::~JCScriptRuntime() 
    {
        onThExit();
        s_JSRT = NULL;
        m_pFreeTypeRender = NULL;
        if (m_pJSCmdBuffer)
        {
            delete m_pJSCmdBuffer;
            m_pJSCmdBuffer = NULL;
        }
        if (m_pFontManager)
        {
            delete m_pFontManager;
            m_pFontManager = NULL;
        }
        if( m_pMeasureTextManager )
        {
            delete m_pMeasureTextManager;
            m_pMeasureTextManager = nullptr;
        }
        if (m_pTextManager)
        {
            delete m_pTextManager;
            m_pTextManager = NULL;
        }
        if (m_pCmdDispathManager)
        {
            delete m_pCmdDispathManager;
            m_pCmdDispathManager = NULL;
        }
        m_pRootNode = NULL;
        JCNode2DRenderer::clearAll();
    }
    void JCScriptRuntime::init(JCFreeTypeFontRender* pFreeTypeRender ) 
    {
        m_pFreeTypeRender = pFreeTypeRender;
        m_pMeasureTextManager->setFreeTypeFontRender(m_pFreeTypeRender);
        m_pTextManager->setFreeTypeRender(m_pFreeTypeRender);
    }
    void JCScriptRuntime::onThInit() 
    {
        m_pRootHtml5Context = NULL;
        //JCPerfHUD::resetFrame();
        m_vRenderCmds.reserve(RENDERCMD_RESERVE_NUM);
        JCConch::s_pConchRender->m_pImageManager->resetJSThread();
        JCICmdDispatch::resetDispathGlobalID();
        m_pJSCmdBuffer->clearData();
        startScriptOnRenderThread();
        JCNode2DRenderer::initAll();
    }
    bool JCScriptRuntime::onUpdate() 
    {
        m_nUpdateCount++;
        onUpdateDraw(0);
        //释放已经丢弃的cmd指令
        //CmdBase::releaseDiscardedCmds();
        return true;
    }
    void JCScriptRuntime::onUpdateDraw(double vsyncTime) 
    {
        dispatchScriptCmd();
        if (JCConch::s_pConchRender != NULL)
        {
            if (m_pRootHtml5Context)
            {
                if (m_pRootNode)
                {
                    m_pRootNode->render(m_pRootHtml5Context, 0, 0);
                }
            }
            //setRenderData
            int nRenderCmdSize = m_vRenderCmds.size();
            JCConch::s_pConchRender->setRenderData(m_vRenderCmds, m_nCountGroup, m_nCountVertex, m_nCountNode);
            JCConch::s_pConchRender->renderFrame(0,false);
            if (nRenderCmdSize > RENDERCMD_RESERVE_NUM)
            {
                m_vRenderCmds.reserve(nRenderCmdSize);
            }
        }
    }
    void JCScriptRuntime::onThExit()
    {
        m_pRootHtml5Context = NULL;
        //清空文字相关的
        m_pMeasureTextManager->clearAll();
        m_pFontManager->clearAllData();
        //清空会非法，这个地方先重用这
        //m_pTextManager->clearAll();
        m_pCmdDispathManager->clearAll();
    }
    void JCScriptRuntime::flushSharedCmdBuffer() 
    {
        char* pBuff = m_pCmdBufferSharedWithJS;
        if (!pBuff)return;
        int len = *(int*)pBuff;
        m_pJSCmdBuffer->append(pBuff + 4, len-4);
        ((int*)pBuff)[0] = 4;
    }
    void JCScriptRuntime::setJSCmdBuffer(char* pBuffer)
    {
        int nLen = *(int*)pBuffer;
        m_pJSCmdBuffer->setExternalBuffer(pBuffer + 4, nLen - 4);
    }
    void JCScriptRuntime::dispatchScriptCmd()
    {
        m_pCmdDispathManager->dispatchScriptCmd(*m_pJSCmdBuffer, m_nUpdateCount);
        m_pJSCmdBuffer->clearData();
    }
    void JCScriptRuntime::setRootNode(JCNode2D* pNode)
    {
        m_pRootNode = pNode;
    }
    void JCScriptRuntime::startScriptOnRenderThread()
    {
        CmdFunction* pObject = CmdFunction::create(-1);
        pObject->m_pFunction = std::bind([]() {
            JCConch::s_pConchRender->clearAllData();
        });
        m_vRenderCmds.push_back(pObject);
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------