/**
@file			JCThreadCmdMgr.cpp
@brief			
@author			James
@version		1.0
@date			2017_11_28
*/

#include "JCThreadCmdMgr.h"
#include <misc/JCWorkerThread.h>


namespace laya 
{
    JCThreadCmdMgr::JCThreadCmdMgr() 
    {
        for (int i = 0; i < NUM; i++)
        {
            m_Threads[i] = nullptr;
        }
        m_bStop = false;
    }
    JCThreadCmdMgr::~JCThreadCmdMgr() 
    {
        stop();
    }
    void JCThreadCmdMgr::postTo(int tid, const function<void(void)>& f) 
    {
        m_Lock.lock();  //保护下面的post，一旦m_bStop设置了，下面的pTarget就随时可能无效。所以对pTarget的使用都要保护起来。
        if (!m_bStop) 
        {
            JCWorkerThread* pTarget = m_Threads[tid];
            if (pTarget) 
            {
                pTarget->post(f);
            }
        }
        m_Lock.unlock();
    }
    void JCThreadCmdMgr::postToJS(const function<void(void)>& f) 
    {
        postTo(JS, f);
    }
    void JCThreadCmdMgr::postToDownload(const function<void(void)>& f) 
    {
        postTo(DOWNLOADER, f);
    }
    void JCThreadCmdMgr::postToDecoder(const function<void(void)>& f) 
    {
        postTo(DECODER, f);
    }
    void JCThreadCmdMgr::regThread(int id, JCWorkerThread* pTh) 
    {
        m_Lock.lock();
        m_Threads[id] = pTh;
        m_Lock.unlock();
    }
    void JCThreadCmdMgr::start()
    {
        m_Lock.lock();
        m_bStop = false;
        for (int i = 0; i < NUM; i++) 
        {
            m_Threads[i] = nullptr;
        }
        m_Lock.unlock();
    }
    void JCThreadCmdMgr::stop() 
    {
        m_Lock.lock();
        m_bStop = true;
        for (int i = 0; i < NUM; i++) 
        {
            m_Threads[i] = nullptr;
        }
        m_Lock.unlock();
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
