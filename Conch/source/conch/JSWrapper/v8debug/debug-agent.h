
#ifndef V8_DEBUG_AGENT_H_
#define V8_DEBUG_AGENT_H_
#ifdef JS_V8_DEBUGGER
#include <v8.h>
#include <v8-inspector.h>
//#include <misc/boostSemaphore.h>
#include <thread>
#include <mutex>
#include "buffer/JCBuffer.h"

#define ASSERT(condition)      ((void) 0)

namespace laya {
	class JSMThread;
    class JSThreadInterface;
    class strIter {
    public:
        //返回空就表示没有了。
        virtual char* get(int& len) = 0;
    };
    class StrBuff :public JCCommandEncoderBuffer {
    public:
        StrBuff(int sz, int adsz) :JCCommandEncoderBuffer(sz, adsz) {}
        StrBuff& operator <<(int v) {
            char buf[64];
            sprintf(buf, "%d", v);
            addStr(buf);
            return *this;
        }
        StrBuff& operator <<(double v) {
            char buf[64];
            sprintf(buf, "%f", v);
            addStr(buf);
            return *this;
        }
        StrBuff& operator <<(unsigned int v) {
            char buf[64];
            sprintf(buf, "%d", v);
            addStr(buf);
            return *this;
        }
        StrBuff& operator <<(int64_t v) {
            char buf[64];
            sprintf(buf, "%lld", v);
            addStr(buf);
            return *this;
        }
        StrBuff& operator <<(const char* v) {
            addStr(v);
            return *this;
        }
        StrBuff& operator <<(strIter& pIter) {
            int len;
            while (true) {
                char* pStr = pIter.get(len);
                if (!pStr || len <= 0)break;
                append(pStr, len);
            }
            return *this;
        }
        void addStr(const char* str) {
            append(str, strlen(str));
        }
    };

    //替换字符串中的',",这样可以不必产生新的内存需求。
    //这个是为JSON准备的
    std::string encodeStrForJSON(const char* pStr);
    class InspectorFrontend;
    class per_session_data__v8dbg;
	class DebuggerAgent {
	public:
		DebuggerAgent(const char* name, int port);//如果只是测试Socket和消息收发部分，直接New DebuggerAgent(const char* name, int port)
		//如果是完整调试则要从ChormeDebugAPI开始调用
		~DebuggerAgent();

		void Shutdown();
		v8::Isolate* isolate() { return isolate_; }
		/**
		* 启动js线程了，创建一个新的jsid，以后js相关的消息，都使用这个jsid。
		* 提供一个函数，希望js线程在循环中调用他。
		*/
		void onJSStart(JSThreadInterface* pJSThread,bool bDebugWait);
		/**
		* js线程结束了，当前的jsid就失效了，以后接收到的此id的消息都忽略。
		*/
		void onJSExit();
		//给调试器发送log
		void sendToDbgConsole(char* pMsg, const char* src, int line, int colum, const char* type);

        void onAcceptNewFrontend(per_session_data__v8dbg* pData);
        void onFrontEndClose();
        void onDbgMsg(char* pMsg, int len);
        void sendMsgToFrontend(char* pMsg, int len);
        void onMsgToV8End(int id);    //js线程执行完毕的回调。是在js线程
	private:

		v8::Isolate* isolate_;
		std::string name_;
		int port_; 
		bool terminate_;  
		std::mutex session_access_;  // Mutex guarging access to session_.
		//semaphore terminate_now_;  // Semaphore to signal termination.
		JSThreadInterface*	pJSThread_;
        per_session_data__v8dbg*    pWsSessionData=nullptr;
        bool        bHasFrontend = false;//等到有人连进来才跑js
        bool        bFirst = true;
        int         nFrontEndMsgID = 0; //自己规定的消息id，并不是解析的消息的json，这样容易一些。现在主要是判断是否处理的Debugger.enable
        int         nEnableDebuggerMsgID = -1;  //等待这个消息被处理，所以需要记录是哪个
        static int  sMsgID;
	public:
        std::unique_ptr<v8_inspector::V8Inspector> _new_inspector;
        std::unique_ptr<v8_inspector::V8InspectorSession> _dbg_session_; //new debugger
        v8_inspector::V8InspectorClient*    m_pInspectorClient=nullptr;
        InspectorFrontend*                  m_pInspectorChannel = nullptr;
	};
}
#endif
#endif  // V8_DEBUG_AGENT_H_
