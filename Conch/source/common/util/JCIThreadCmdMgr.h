

/**
@file			JCIThreadMgr.h
@brief			
@author			guo
@version		1.0
@date			2016_5_19
*/

#ifndef __JCIThreadMgr_H__
#define __JCIThreadMgr_H__

#include <functional>

namespace laya {
    class IConchThreadCmdMgr {
    public:
        virtual void postToJS(const std::function<void(void)>& f) = 0;
        virtual void postToDownload(const std::function<void(void)>& f)=0;
        virtual void postToDecoder(const std::function<void(void)>& f)=0;
    };
}

#endif //__JCIThreadMgr_H__
