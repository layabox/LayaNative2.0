#ifdef JS_V8

#include "JSArrayBuffer.h"
#include <util/JCMemorySurvey.h>
#include <util/Log.h>
#include "../JSInterface.h"
#include "JSCProxyTrnasfer.h"

namespace laya {
	//TODO 这里没有考虑多线程的问题，如果js要支持多线程的话，需要修改。
	struct JsStrBuff {
		char* _buff;
		int _len;
		static std::vector<JsStrBuff> jsstrbuffs;
		static int curIdx;
		JsStrBuff() {
			_buff = NULL;
			_len = 0;
		}

		static JsStrBuff& getAData() {
			if (JsStrBuff::curIdx >= (int)JsStrBuff::jsstrbuffs.size()) {
				JsStrBuff::jsstrbuffs.push_back(JsStrBuff());
				JsStrBuff::curIdx++;
				return JsStrBuff::jsstrbuffs.back();
			}
			else {
				return JsStrBuff::jsstrbuffs[JsStrBuff::curIdx++];
			}
		}
	};
	std::vector<JsStrBuff> JsStrBuff::jsstrbuffs;
	int JsStrBuff::curIdx = 0;

	//
	void resetJsStrBuf() {
		JsStrBuff::curIdx = 0;
	}
	char* JsCharToC(Local<Value> p_vl) {
		int len = 0;
		auto isolate = v8::Isolate::GetCurrent();
		Local<String> str = p_vl->ToString(isolate->GetCurrentContext()).ToLocalChecked();
		len = str->Utf8Length(isolate);
		if (len <= 0)
			return "";
		JsStrBuff& curdata = JsStrBuff::getAData();
		char*& tocharBuf = curdata._buff;
		int& tocharBufLen = curdata._len;

		//tocharBuf= new char[len + 1];
		if (len > tocharBufLen) {
			tocharBufLen = len;
			if (tocharBuf != NULL)
				delete[] tocharBuf;
			tocharBuf = new char[len+1];
		}
		else {
			//如果占用空间太大，也要重新分配
			if (tocharBufLen > 1024 ) {
				tocharBufLen = len;
				if (tocharBuf != NULL)
					delete[] tocharBuf;
				tocharBuf = new char[len+1];
			}
		}
		str->WriteUtf8(isolate, tocharBuf);
		return tocharBuf;
	}
}

namespace laya{
    ArrayBufferAllocator::ArrayBufferAllocator() {
    }

    ArrayBufferAllocator::~ArrayBufferAllocator() {
    }

	void* ArrayBufferAllocator::Allocate(size_t length) { 
		char* pRet = new char[length]; 
		memset(pRet,0,length); 
		return pRet; 
	}
	void* ArrayBufferAllocator::AllocateUninitialized(size_t length){
		char* pRet = new char[length]; 
		return pRet;
	};
	void ArrayBufferAllocator::Free(void* data, size_t length) 
    {
        if (data != NULL || length > 0)
        {
            delete[]((char*)data);
        }
        else
        {
            LOGI("ArrayBufferAllocator::Free data=%d length=%d",(intptr_t)data, length);
        }
	}

	ArrayBufferAllocator* ArrayBufferAllocator::getInstance(){
		return new ArrayBufferAllocator();
	}

	v8::Local<v8::ArrayBuffer> createJSAB(char* pData, int len) {
		v8::Local<v8::ArrayBuffer> ab = v8::ArrayBuffer::New(v8::Isolate::GetCurrent(), len);
        v8::ArrayBuffer::Contents contents = ab->GetContents();// ab->Externalize();
		char* pPtr = (char*)contents.Data();
		memcpy(pPtr, pData, len);
		//Externalize 以后会减去内存占用，导致不能正确GC，所以再给加回来。不知道管理ArrayBuffer的正确方法是什么。
		//v8::Isolate::GetCurrent()->AdjustAmountOfExternalAllocatedMemory(len);
		return ab;
	}

    v8::Local<v8::ArrayBuffer> createJSABAligned(char* pData, int len) {
        int asz = (len + 3) & 0xfffffffc;
        v8::Local<v8::ArrayBuffer> ab = v8::ArrayBuffer::New(v8::Isolate::GetCurrent(), asz);
        v8::ArrayBuffer::Contents contents = ab->GetContents();// ab->Externalize();
        char* pPtr = (char*)contents.Data();
        memcpy(pPtr, pData, len);
        //ArrayBuffer 自己已经初始化为0了
        //Externalize 以后会减去内存占用，导致不能正确GC，所以再给加回来。不知道管理ArrayBuffer的正确方法是什么。
        //v8::Isolate::GetCurrent()->AdjustAmountOfExternalAllocatedMemory(asz);
        return ab;
    }

    bool extractJSAB(JsValue jsval, char*& data, int& len) {
		v8::Local<v8::ArrayBuffer> ab;
		if (jsval->IsArrayBufferView()) {
			v8::Local<v8::ArrayBufferView> abv = v8::Local<v8::ArrayBufferView>::Cast(jsval);
            len = abv->ByteLength();
			ab = abv->Buffer();
            v8::ArrayBuffer::Contents contents = ab->GetContents();
            //len = contents.ByteLength();  这种情况下，用view的长度，因为可能多个view公用一个大buffer
            data = (char*)contents.Data()+abv->ByteOffset();
        }
		else if (jsval->IsArrayBuffer()) {
			ab = v8::Local<v8::ArrayBuffer>::Cast(jsval);
            v8::ArrayBuffer::Contents contents = ab->GetContents();
            len = contents.ByteLength();
            data = (char*)contents.Data();
        }
		else {
			data = NULL;
			len = 0;
			return false;
		}

        /*
		if (ab->IsExternal()) {
			v8::ArrayBuffer::Contents contents = ab->GetContents();
			len = contents.ByteLength();
			data = (char*)contents.Data();
		}
		else {
			v8::ArrayBuffer::Contents contents = ab->Externalize();
			len = contents.ByteLength();
			v8::Isolate::GetCurrent()->AdjustAmountOfExternalAllocatedMemory(len);
			data = (char*)contents.Data();
		}
        */
		return true;
	}

    void __JSRun::ReportException(v8::Isolate* isolate, v8::TryCatch* try_catch) {
        v8::HandleScope handle_scope(isolate);
        v8::String::Utf8Value exception(isolate, try_catch->Exception());
        const char* exception_string = ToCString(exception);
        v8::Local<v8::Message> message = try_catch->Message();
        static char errInfo[2048];
        int curpos = 0;
        if (message.IsEmpty()) {
            // V8 didn't provide any extra information about this error; just
            // print the exception.
            int off = snprintf(errInfo, sizeof(errInfo), "%s\n", exception_string);

            //通知全局错误处理脚本
            std::string kBuf = "if(conch.onerror){conch.onerror('";
            kBuf += UrlEncode(exception_string);
            kBuf += "','undefined','undefined','undefined','";
            kBuf += UrlEncode(exception_string);
            kBuf += "');};";
            __JSRun::Run(kBuf.c_str());
        }
        else {
            auto ctx = isolate->GetCurrentContext();
            v8::String::Utf8Value fnstr (isolate, message->GetScriptResourceName());
            const char* filename_string = ToCString(fnstr);
            v8::MaybeLocal<String> source_line_maybe = message->GetSourceLine(ctx);
            v8::String::Utf8Value srclinestr(isolate, source_line_maybe.ToLocalChecked());
            const char* sourceline_string = ToCString(srclinestr);
            int linenum = message->GetLineNumber(ctx).FromJust();
            int start = message->GetStartColumn(ctx).FromMaybe(0);
            int end = message->GetEndColumn(ctx).FromMaybe(0);
            v8::ScriptOrigin origin = message->GetScriptOrigin();
            int lineoff = origin.ResourceLineOffset()->Value();
            int startcol = origin.ResourceColumnOffset()->Value();
            if (start > startcol) {
                start -= startcol;
                end -= startcol;
            }

            //错误行可能非常长，只取一部分
            char errLineSrc[128];
            if (strlen(sourceline_string) > 128) {
                int startoff = start > 50 ? (start - 50) : 0;
                start -= startoff;
                end -= startoff;
                if (end >= 128)end = 127;

                memcpy(errLineSrc, sourceline_string+startoff, 128);
                sourceline_string = errLineSrc;
            }
            curpos += snprintf(errInfo, sizeof(errInfo), "%s:%i:\n%s\n%s\n", 
                filename_string, 
                linenum, 
                exception_string, 
                sourceline_string);
            //打印具体哪一行，哪一列
            if (curpos < sizeof(errInfo)) {
                int st = curpos;
                int srclen = snprintf(errInfo + curpos, sizeof(errInfo) - curpos, "%s\n", sourceline_string);
                curpos += srclen;
                if (curpos < sizeof(errInfo)) {
                    for (int si = 0; si < srclen; si++) {
                        char& c = errInfo[st+si];
                        if (c != ' ' && c != '\t' && c != '\r')c = ' ';
                        if (si >= start && si <= end)c = '^';
                    }
                }
            }
            curpos += snprintf(errInfo + curpos,sizeof(errInfo)-curpos, "\n");
			v8::Local<v8::Value> stack_trace_string;
			if (try_catch->StackTrace(ctx).ToLocal(&stack_trace_string) && stack_trace_string->IsString() && v8::Local<v8::String>::Cast(stack_trace_string)->Length() > 0) 
			{
				v8::String::Utf8Value stack_trace(isolate, stack_trace_string);
                const char* stack_trace_string = ToCString(stack_trace);
                if (curpos < sizeof(errInfo)) {
                    curpos += snprintf(errInfo + curpos,sizeof(errInfo)-curpos, "%s", stack_trace_string);
                }
            }

            //通知全局错误处理脚本
            std::string kBuf = "if(conch.onerror){conch.onerror('";
            kBuf += UrlEncode(exception_string);
            kBuf += "','";
            kBuf += UrlEncode(filename_string);
            kBuf += "','";
            //kBuf += std::to_string(linenum);
            std::ostringstream os;
            os << linenum;
            kBuf += os.str();
            kBuf += "','";
            kBuf += "undefined";
            kBuf += "','";
            kBuf += UrlEncode(errInfo);
            kBuf += "');};";
            __JSRun::Run(kBuf.c_str());

        }

		if (gbAlertException)
		{
			JSAlert(errInfo);
		}
		LOGE("==JSERROR:\n%s", errInfo);
    }
}
#endif