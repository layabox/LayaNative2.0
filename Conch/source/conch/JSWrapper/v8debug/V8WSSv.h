#ifdef JS_V8_DEBUGGER
#ifndef _V8_WEBSOCKET_SERVER_H__
#define _V8_WEBSOCKET_SERVER_H__

#include <libwebsockets.h>
#include "debug-agent.h"
#include <deque>

#define MAX_V8DBG_PAYLOAD 10240

namespace laya {

    //每个session的附加数据
    class per_session_data__v8dbg {
    public:
        size_t rx, tx;
        unsigned char* pRecvBuff;   //如果长度太大，就用这个，这个需要释放
        unsigned char RecvBuf[LWS_PRE + MAX_V8DBG_PAYLOAD];
        unsigned int nRecvLen;   //接收的长度
        unsigned int index;
        unsigned char* pSendBuff;
        int final;
        int continuation;
        int binary;
        DebuggerAgent* pDbgAgent;   //如果有多个session的话，这里可以保存session
        std::recursive_mutex     pTaskLock;
        std::deque<std::string> pSendTask;
        //std::vector<std::string>    pSendTask;
    };


    void startWSSV(int port, DebuggerAgent* pDbgAgent);
    void stopWSSV();
}

#endif

#endif
