/**
@file			JCConch.cpp
@brief
@author			guo
@version		1.0
@date			2016_5_13
*/

#include "JCConch.h"
#include <algorithm>
#include <util/Log.h>
#include <util/JCCommonMethod.h>
#include "JCConch.h"
#include "JCScrpitRuntimeWASM.h"
int g_nInnerWidth = 1024;
int g_nInnerHeight = 768;
bool g_bGLCanvasSizeChanged = false;
namespace laya
{
    JCConch* JCConch::s_pConch = NULL;
    int64_t JCConch::s_nUpdateTime = 0;
    std::shared_ptr<JCConchRender> JCConch::s_pConchRender;
    JCConch::JCConch(int nDownloadThreadNum)
    {
        s_pConchRender.reset(new JCConchRender(NULL));
        s_pConchRender->setSize(g_nInnerWidth, g_nInnerHeight);
        m_pScrpitRuntime = new JCScriptRuntime();
        m_pScrpitRuntime->init(s_pConchRender->m_pFreeTypeRender);
        s_pConch = this;
    }
    JCConch::~JCConch()
    {
        if (m_pScrpitRuntime)
        {
            delete m_pScrpitRuntime;
            m_pScrpitRuntime = NULL;
        }
        s_pConchRender.reset();
        s_pConch = NULL;
    }
    void JCConch::onAppStart()
    {
    }
    void JCConch::reload()
    {
    }
    void JCConch::DelAppRes()
    {
    }
    void JCConch::onClearMemory()
    {
    }
    void JCConch::onAppDestory()
    {
    }
    void JCConch::postCmdToMainThread(int p_nCmd, int p_nParam1, int p_nParam2)
    {
        m_funcPostMsgToMainThread(p_nCmd, p_nParam1, p_nParam2);
    }
    int JCConch::urlHistoryLength()
    {
        return 0;
    }
    void JCConch::urlBack()
    {
        urlGo(-1);
    }
    void JCConch::urlGo(int s)
    {
    }
    void JCConch::urlForward()
    {
        urlGo(1);
    }
    void JCConch::urlHistoryPush(const char* pUrl)
    {

    }
    void JCConch::onRunCmdInMainThread(int p_nCmd, int p_nParam1, int p_nParam2) 
    {
        switch (p_nCmd) 
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
        }
    }
};
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
