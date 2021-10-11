
#ifdef JS_V8_DEBUGGER
#include "debug-agent.h"
#include <util/Log.h>
#include "V8Socket.h"
#include <fileSystem/JCFileSystem.h>
#include <util/JCJson.h>
#include <util/JCCommonMethod.h>
#include "../LayaWrap/JSConchConfig.h"
#include "../../JCScriptRuntime.h"
#include <string>
#include <chrono>
#include <thread>
#include "V8WSSv.h"

using namespace laya;

bool g_bSendLogToDbg = true;

void mygLayaLog(int level, const char* file, int line, const char* fmt, ...) {
    if (!JCScriptRuntime::s_JSRT)
        return;
    DebuggerAgent* pDbgAgent = JCScriptRuntime::s_JSRT->m_pDbgAgent;
	if (!g_bSendLogToDbg || !pDbgAgent) {
		va_list args;
		va_start(args, fmt);
		vprintf(fmt, args);
		va_end(args);
		return;
	}
	char buf[1024];
	char* pBuf = NULL;
	va_list args;
	va_start(args, fmt);
	int len = vsnprintf(buf, 1024,fmt, args);
    if (len < 0) {
        printf("log error! \n");
        return;
    }
    if (len > 1024) {
        pBuf = new char[len + 1];
        len = vsnprintf(pBuf, len + 1, fmt, args);
        if (len < 0)
            return;
    }
	va_end(args);
	const char* pTypes[] = { "warning","error", "debug", "log","runtime" };
	int sz = sizeof(pTypes) / sizeof(const char*);
    pDbgAgent->sendToDbgConsole(pBuf ? pBuf : buf, file, line, 0, level<sz ? pTypes[level] : "unknown");
	if (pBuf) {
		delete[] pBuf;
	}
}

void mygLayaLogSimp(int level, const char* file, int line, const char* msg) {
    if (!JCScriptRuntime::s_JSRT)
        return;
    DebuggerAgent* pDbgAgent = JCScriptRuntime::s_JSRT->m_pDbgAgent;
    if (!g_bSendLogToDbg || !pDbgAgent) {
		printf("%s", msg);
		return;
	}
	const char* pTypes[] = { "warning","error", "debug", "log","runtime" };
	int sz = sizeof(pTypes) / sizeof(const char*);
	pDbgAgent->sendToDbgConsole((char*)msg, file, line, 0, level<sz ? pTypes[level] : "unknown");
}


namespace laya {

    int   DebuggerAgent::sMsgID=0;
    std::string encodeStrForJSON(const char* pStr) {
        std::string ret = "";
        ret.reserve(2048);
        const char* st = pStr;
        unsigned char stv;
        int len = 0;
        while (stv = *st++) {
            switch (stv) {
            case '\\':
                if (len > 0) {
                    ret.append(st - len - 1, len); len = 0;
                }
                ret += "\\\\"; break;
            case '\"':
                if (len > 0) {
                    ret.append(st - len - 1, len); len = 0;
                }
                ret += "\\\""; break;
            case '\t':
                if (len > 0) {
                    ret.append(st - len - 1, len); len = 0;
                }
                ret += "\\t"; break;
            case '\r':
                if (len > 0) {
                    ret.append(st - len - 1, len); len = 0;
                }
                ret += "\\r"; break;
            case '\n':
                if (len > 0) {
                    ret.append(st - len - 1, len); len = 0;
                }
                ret += "\\n"; break;
            default:len++; break;
            }
        }
        if (len > 0) {
            ret.append(st - len - 1, len); len = 0;
        }
        return ret;
    }


    class InspectorFrontend final : public v8_inspector::V8Inspector::Channel {
    public:
        explicit InspectorFrontend(Local<Context> context) {
            isolate_ = context->GetIsolate();
            context_.Reset(isolate_, context);
        }
        virtual ~InspectorFrontend() = default;

    private:
        void sendResponse(int callId, std::unique_ptr<v8_inspector::StringBuffer> message) override {
            Send(message->string());
        }
        void sendNotification(std::unique_ptr<v8_inspector::StringBuffer> message) override {
            Send(message->string());
        }
        void flushProtocolNotifications() override {}

        void Send(const v8_inspector::StringView& str) {
            v8::Isolate::AllowJavascriptExecutionScope allow_script(isolate_);
            int length = static_cast<int>(str.length());
            bool utf8 = str.is8Bit();
            if (!utf8) {
                int unicodeLen = length;
                char* pUTF8Buff = new char[unicodeLen * 4 + 4];
                int uniNum = 0, buffUsed = 0;
                char* ret = UnicodeStrToUTF8Str((short*)str.characters16(), pUTF8Buff, unicodeLen * 4, uniNum, buffUsed);
                /*
                //最后是0，要去掉
                buffUsed--;
                ret[buffUsed++] = '\r'; ret[buffUsed++] = '\n';
                ret[buffUsed++] = '\r'; ret[buffUsed++] = '\n';
                ret[buffUsed++] = 0;
                */
                if (pAgent) {
                    pAgent->sendMsgToFrontend(ret, buffUsed - 1);   //不能发送0
                }
                delete[] pUTF8Buff;
            }
            else {
                *(int*)0 = 0;//没做。要\r\n\r\n 结束
                if (pAgent) {
                    pAgent->sendMsgToFrontend((char*)str.characters8(), str.length());
                }
                //LOGE("re %s", str.characters8());
            }
        }

        Isolate* isolate_;
        Global<Context> context_;
    public:
        //V8Socket*   pConn = nullptr;
        DebuggerAgent* pAgent = nullptr;
    };


	DebuggerAgent::DebuggerAgent(const char* name, int port) :name_(name),
		port_(port),
		terminate_(false)/*,
		terminate_now_(0)*/{
		pJSThread_ = NULL;
		isolate_ = NULL;
	}

	DebuggerAgent::~DebuggerAgent() {
	}
    class MyV8InspectorClient :public v8_inspector::V8InspectorClient {
    public:
        MyV8InspectorClient(JSThreadInterface* pJS) {
            pJSThread = pJS;
        }
        virtual ~MyV8InspectorClient() {
            int a = 10;
        }
        void runMessageLoopOnPause(int context_group_id) override {
            terminated_ = false;
            //下面必须要等待并处理前端消息，因为这个函数完了就会立即执行resume
            while (!terminated_ && waitForFrontendEvent()) {
                //while (platform_->FlushForegroundTasks(env_->isolate())) {}
            }
            terminated_ = false;
        }

        bool waitForFrontendEvent() {
            if (pJSThread->hasDbgFuncs()) {
                pJSThread->runDbgFuncs();
            }
            else {
                std::this_thread::sleep_for(std::chrono::microseconds(100));
            }
            return true;
        }

        void quitMessageLoopOnPause() override {
            terminated_ = true;
        }
        /*void consoleAPIMessage(int contextGroupId,
            v8::Isolate::MessageErrorLevel level,
            const v8_inspector::StringView& message,
            const v8_inspector::StringView& url, unsigned lineNumber,
            unsigned columnNumber, v8_inspector::V8StackTrace*) override {
            if (gLayaLog)
                return;
            static char consolebuf[1024];
            if (!message.is8Bit()) {
                bool needDel = false;
                char* pUTF8Buff = nullptr;
                int unicodeLen = message.length();
                int utf8len = unicodeLen * 4 + 4;
                if (utf8len < 1024) {
                    pUTF8Buff = consolebuf;
                }
                else {
                    pUTF8Buff = new char[utf8len];
                }
                int uniNum = 0, buffUsed = 0;
                char* ret = UnicodeStrToUTF8Str((short*)message.characters16(), pUTF8Buff, unicodeLen * 4, uniNum, buffUsed);
                LOGI("%s", ret);
            }
            else {
                LOGI("%s",message.characters8());
            }
        }*/
        double currentTimeMS() override { return tmGetCurms(); }

        //调试器请求定时执行一个任务。例如 Allocation instrumentation timeline
        //这个是js线程执行的。callback也在js线程执行就行
        //interval_s 单位是秒，现在是0.05秒，这个精度用update应该能满足。
        void startRepeatingTimer(double interval_s, TimerCallback callback, void* data) override {
            LOGE("没做");
        }
        //上面请求的定时任务停止了
        void cancelTimer(void* data) override {
            LOGE("没做");
        }


        bool        terminated_ = false;
        JSThreadInterface*   pJSThread=nullptr;
    };


    void DebuggerAgent::onAcceptNewFrontend(per_session_data__v8dbg* pData) {
        printf("==============new v8 debugger===================\n");
        DebuggerAgent::sMsgID = 0;
        /*
        connect是创建一个新的V8InspectorSessionImpl， 然后创建一堆新的V8RuntimeAgentImpl，
        V8DebuggerAgentImpl，V8ProfilerAgentImpl，V8HeapProfilerAgentImpl，V8ConsoleAgentImpl，V8SchemaAgentImpl
        然后把这些AgentImpl都连接到这个SessionImpl, 这样不同的调试命令就能发给不同的Agent
        */
        _dbg_session_ = _new_inspector->connect(1, m_pInspectorChannel, v8_inspector::StringView());

        //注意 由于线程问题，如果有新的前端连进来（或者前端刷新），而js又在用这个对象，可能会导致非法。不过这种情况很少，加锁的话又不好看，先忽略。
        pWsSessionData = pData;
        gLayaLog = mygLayaLog;
        gLayaLogNoParam = mygLayaLogSimp;
        m_pInspectorChannel->pAgent = this;
        /*
        if (bFirst) {
            semWaitDebugger.signal();
            bFirst = false;
        }
        */
    }

    void DebuggerAgent::onFrontEndClose() {//TODO 还没有调用
        gLayaLog = nullptr;
        gLayaLogNoParam = nullptr;
    }

    void dispatchProtocolMsg_inJSThread(DebuggerAgent* pAgent, v8_inspector::StringView msg, int msgid) {
        pAgent->_dbg_session_->dispatchProtocolMessage(msg);
        pAgent->onMsgToV8End(msgid);
        delete[] msg.characters8();
    }

    void DebuggerAgent::onMsgToV8End(int id) {
        if (bFirst && nEnableDebuggerMsgID > 0 && nEnableDebuggerMsgID == id) {
            //semWaitDebugger.signal();
            bHasFrontend = true;
            bFirst = false;
        }
    }

    void DebuggerAgent::onDbgMsg(char* pMsg, int len) {
        //printf(">>>%s\n", pMsg);
        nFrontEndMsgID = sMsgID++;
        if (bFirst && nEnableDebuggerMsgID<0) {
            if (strstr(pMsg, "Debugger.enable") != NULL) {
                nEnableDebuggerMsgID = nFrontEndMsgID;
            }
            //bFirst = false;
        }
        bool toV8 = true;
        const char* msg = (const char*)pMsg;

        int msglen = 0;
        //writeFileSync1("d:/temp/v8in.txt", message, strlen(message), buffer::raw);
        //先检查一下command，分配给不同的对象处理。
        msglen = strlen(msg);
        char* ptmpMsg = new char[msglen + 1];
        ptmpMsg[msglen] = 0;
        memcpy(ptmpMsg, msg, msglen);
        JCJson json;
        if (json.paserJson((char*)ptmpMsg)) {
            JsonObject* pRoot = (JsonObject*)json.getRoot();
            JsonValue* pMethod = (JsonValue*)pRoot->getNode("method");
            JsonValue* pID = (JsonValue*)pRoot->getNode("id");
            char* strID = pID->m_sValue;
            char* pstrMethod = pMethod->m_sValue;
            switch (pstrMethod[0]) {
            case 'C':
                if (strstr(pstrMethod, "CSS") == pstrMethod) {
                    //to CSS
                    toV8 = false;
                }
                break;
            case 'D':
                if (strstr(pstrMethod, "DOM") == pstrMethod) {
                    //to DOM
                    toV8 = false;
                }
                break;
            case 'I':
                if (strstr(pstrMethod, "Inspector") == pstrMethod) {
                    //to Inspector
                    toV8 = false;
                }
                break;
            case 'L':
                if (strstr(pstrMethod, "Log") == pstrMethod) {
                    //to Log
                    toV8 = false;
                }
                break;
            case 'N':
                if (strstr(pstrMethod, "Network") == pstrMethod) {
                    //to Network
                    toV8 = false;
                }
                else {
                }
                break;
            case 'O':
                if (strstr(pstrMethod, "Overlay") == pstrMethod) {
                    //to Overlay
                    toV8 = false;
                }

                break;
            case 'P':
                if (strstr(pstrMethod, "Page") == pstrMethod) {
                    //to Page
                    toV8 = false;
                }
                break;
            case 'S':
                if (strstr(pstrMethod, "ServiceWorker") == pstrMethod) {
                    //to ServiceWorker
                    toV8 = false;
                }
                else if (strstr(pstrMethod, "Security") == pstrMethod) {
                    //to Security
                    toV8 = false;
                }

                break;
            case 'T':
                if (strstr(pstrMethod, "Target") == pstrMethod) {
                    //to Target
                    toV8 = false;
                }
                break;
            default:
                break;
            }
            if (toV8) {
                // Convert UTF-8 to UTF-16.
                uint16_t* pUTF16 = new uint16_t[msglen * 4];
                int uniLen = UTF8StrToUnicodeStr((unsigned char*)msg, pUTF16, msglen);

                v8_inspector::StringView message_view(pUTF16, uniLen);

                //v8::Locker(agent_->isolate_);
                if (pJSThread_) {
                    pJSThread_->pushDbgFunc(std::bind(dispatchProtocolMsg_inJSThread, this, message_view, nFrontEndMsgID));
                }
                //delete[] pUTF16;  发送到js执行的话，就不要在这里删除了
            }
            else {
                StrBuff strbuf(1024, 512);
                strbuf.setAlign(false);
                strbuf << R"({"error":{"code":-32601,"message":"')" << pstrMethod << R"(' wasn't found"},"id":)" << strID << "}";// \r\n\r\n";
                sendMsgToFrontend(strbuf.getBuffer(), strbuf.getDataSize());
            }
        }
        else {
            LOGE("parse error!");
        }
        delete[] ptmpMsg;
    }

    void DebuggerAgent::sendMsgToFrontend(char* pMsg, int len) {
        if (!pWsSessionData)
            return;
        std::string msg = "";
        msg.append(pMsg, len);
        pWsSessionData->pTaskLock.lock();
        pWsSessionData->pSendTask.push_back(msg);
        pWsSessionData->pTaskLock.unlock();
        //lws_callback_on_writable(pWsSessionData-> wsi);//一旦有机会，触发写回调
    }


    v8_inspector::StringView& Utf8ToStringView(const std::string& message, v8_inspector::StringView& view) {
        auto isolate_ = (v8::Isolate::GetCurrent());
        Local<String> str = String::NewFromUtf8(isolate_, message.c_str(), NewStringType::kNormal).ToLocalChecked();
        int len = str->Length();
        std::unique_ptr<uint16_t[]> buff(new uint16_t[len]);
        str->Write(isolate_, buff.get(), 0, len);
        view = std::move(v8_inspector::StringView(buff.get(), len));
        return view;
    }

	void DebuggerAgent::onJSStart(JSThreadInterface* pJSThread,bool bDebugWait) {
		pJSThread_ = pJSThread;
        isolate_ = (v8::Isolate::GetCurrent());
		v8::HandleScope handle_scope(isolate_);
        Local<String> nameStr = String::NewFromUtf8(isolate_, "layabox", NewStringType::kNormal).ToLocalChecked();
        int nameLen = nameStr->Length();
        std::unique_ptr<uint16_t[]> nameBuffer(new uint16_t[nameLen]);
        nameStr->Write(isolate_, nameBuffer.get(), 0, nameLen);

        m_pInspectorClient = new MyV8InspectorClient(pJSThread);
        _new_inspector = v8_inspector::V8Inspector::create(isolate_, m_pInspectorClient);
        Local<Context> context = isolate_->GetCurrentContext();
        m_pInspectorChannel = new InspectorFrontend(context);

        v8_inspector::StringView ctx_name(nameBuffer.get(), nameLen);
        _new_inspector->contextCreated(v8_inspector::V8ContextInfo(context, 1, ctx_name));
        /*
            connect是创建一个新的V8InspectorSessionImpl， 然后创建一堆新的V8RuntimeAgentImpl，
            V8DebuggerAgentImpl，V8ProfilerAgentImpl，V8HeapProfilerAgentImpl，V8ConsoleAgentImpl，V8SchemaAgentImpl
            然后把这些AgentImpl都连接到这个SessionImpl, 这样不同的调试命令就能发给不同的Agent
            一旦有新的session,v8就会把当前已经编译的脚本的发给对应的session（只有文件名），而不是前端主动要。
        */
        //_dbg_session_ = _new_inspector->connect(1, m_pInspectorChannel, v8_inspector::StringView());

        //启动websocket server用来监听调试信息
        startWSSV(port_,this);
        
        //如果要一上来就暂停，就要特殊处理
        if (bDebugWait) {
            while (!bHasFrontend) {
                pJSThread->runDbgFuncs();
                std::this_thread::sleep_for(std::chrono::microseconds(100));
            }
            /*
            如果用wait的话，只能自己发送消息打开debugger，但是这样并不标准，所以改成循环，一旦前端发来Debugger.enable就可以继续了
            semWaitDebugger.wait(); //先等待调试器连接上，这样就可以避免保存调试信息，堆栈之类的可以直接发给调试器。
            //直接设置一些必须的调试信息。schedulePauseOnNextStatement 需要这些设置，而前端发来的设置又太晚了
            static char* pMsg1 = R"({"id":5,"method":"Runtime.enable"})";
            static char* pMsg2 = R"({"id":6,"method":"Debugger.enable"})";
            uint16_t UTF16[1024];

            int uniLen = UTF8StrToUnicodeStr((unsigned char*)pMsg1, UTF16, strlen(pMsg1));
            v8_inspector::StringView message_view1(UTF16, uniLen);
            //_dbg_session_->dispatchProtocolMessage(message_view1);

            uniLen = UTF8StrToUnicodeStr((unsigned char*)pMsg2, UTF16, strlen(pMsg2));
            v8_inspector::StringView message_view2(UTF16, uniLen);
            //_dbg_session_->dispatchProtocolMessage(message_view2);

            //现在可以设置停在第一句了。
            uniLen = UTF8StrToUnicodeStr((unsigned char*)"{}", UTF16, strlen(pMsg2));
            v8_inspector::StringView xx(UTF16, uniLen);
            //_dbg_session_->schedulePauseOnNextStatement(xx, xx);
            */
        }
	}

	void DebuggerAgent::onJSExit() { 
		pJSThread_ = NULL;
		isolate_ = NULL;
        gLayaLog = nullptr;
        gLayaLogNoParam = nullptr;

        if (m_pInspectorClient)
            delete m_pInspectorClient;
        m_pInspectorClient = nullptr;
        if (m_pInspectorChannel)
            delete m_pInspectorChannel;
        m_pInspectorChannel = nullptr;
        _dbg_session_ = nullptr;
        _new_inspector = nullptr;
        stopWSSV();
	}

	void DebuggerAgent::Shutdown() {
		terminate_ = true;

		// Signal termination and make the server exit either its listen call or its
		// binding loop. This makes sure that no new sessions can be established.
		//terminate_now_.signal();
		// Close existing session if any.
	}

	void DebuggerAgent::sendToDbgConsole(char* pMsg, const char* src, int line, int colum, const char* type) {
        if (pMsg == NULL || src == NULL) {
            return;
        }
        int len1 = strlen(pMsg);
        std::string strMsg = encodeStrForJSON(pMsg);
        StrBuff strbuf(1024, 512);
        strbuf.setAlign(false);
        strbuf << R"({"method":"Runtime.consoleAPICalled","params":{"type":"log","args":[{"type":"string","value":")" << strMsg.c_str() << R"("}],"executionContextId":1,"timestamp":)" << tmGetCurms() << R"(,"stackTrace":{"callFrames":[{"functionName":"runtime","scriptId":"0","url":")" << encodeStrForJSON(src).c_str() << R"(","lineNumber":)" << line << R"(,"columnNumber":0}]}}})";
        //strbuf << "\r\n\r\n";
        sendMsgToFrontend(strbuf.getBuffer(), strbuf.getDataSize());
    }
}
#endif