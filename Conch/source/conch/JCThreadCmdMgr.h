/**
@file			JCThreadCmdMgr.h
@brief			
@author			James
@version		1.0
@date			2017_11_28
*/

#ifndef __JCThreadCmdMgr_H__
#define __JCThreadCmdMgr_H__


#include <functional>
#include <vector>
#include <mutex>
#include <util/JCIThreadCmdMgr.h>

using namespace std;
namespace laya 
{
    class JCWorkerThread;
    class JCThreadCmdMgr:public IConchThreadCmdMgr 
    {
    public:
        enum ThreadID
        {
            JS,
            DOWNLOADER,
            DECODER,
            NUM
        };
    public:
        JCThreadCmdMgr();
        ~JCThreadCmdMgr();
        void postTo(int tid, const function<void(void)>& f);
        virtual void postToJS(const function<void(void)>& f);
        virtual void postToDownload(const function<void(void)>& f);
        virtual void postToDecoder(const function<void(void)>& f);
        void regThread(int id, JCWorkerThread* pTh);
        void stop();
        void start();
    public:
        JCWorkerThread*     m_Threads[NUM];
        bool                m_bStop;
        recursive_mutex     m_Lock;
    };
}
//------------------------------------------------------------------------------


#endif //__JCThreadCmdMgr_H__

//-----------------------------END FILE--------------------------------