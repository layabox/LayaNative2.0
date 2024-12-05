#ifdef JS_JSVM

#include "JSVMArrayBuffer.h"
#include <util/JCMemorySurvey.h>
#include <util/Log.h>
#include "../JSInterface.h"
#include "JSVMProxyTransfer.h"

#define ENV Javascript::getEnv()

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

    static JsStrBuff &getAData() {
        if (JsStrBuff::curIdx >= (int)JsStrBuff::jsstrbuffs.size()) {
            JsStrBuff::jsstrbuffs.push_back(JsStrBuff());
            JsStrBuff::curIdx++;
            return JsStrBuff::jsstrbuffs.back();
        } else {
            return JsStrBuff::jsstrbuffs[JsStrBuff::curIdx++];
        }
    }
};
std::vector<JsStrBuff> JsStrBuff::jsstrbuffs;
int JsStrBuff::curIdx = 0;

//
void resetJsStrBuf() { JsStrBuff::curIdx = 0; }
char* JsCharToC(JSVM_Value p_vl) {
    AutoHandleScope hs;
    JSVM_Status status;
    JSVM_Value  result;
    JSVM_API_CALL(status, ENV, OH_JSVM_CoerceToString(ENV, p_vl, &result));
    
    size_t bufsize;
    size_t len;
    JSVM_API_CALL(status, ENV, OH_JSVM_GetValueStringUtf8(ENV, result, nullptr, bufsize, &len));
    if (len <= 0)
        return "";
    
    JsStrBuff &curdata = JsStrBuff::getAData();
    char *&tocharBuf = curdata._buff;
    int &tocharBufLen = curdata._len;
    
    if (len > tocharBufLen) {
        tocharBufLen = len;
        if (tocharBuf != NULL)
            delete[] tocharBuf;
        tocharBuf = new char[len + 1];
    } else {
        
        if (tocharBufLen > 1024) {
            tocharBufLen = len;
            if (tocharBuf != NULL)
                delete[] tocharBuf;
            tocharBuf = new char[len + 1];
        }
    }
    JSVM_API_CALL(status,ENV, OH_JSVM_GetValueStringUtf8(ENV, result, tocharBuf, len + 1, &bufsize));
    return tocharBuf;
}
}

namespace laya {

JSVM_Value createJSAB(char *pData, int len) {
    JSVM_Value  result;
    JSVM_Status status;
    void*       retData;
    JSVM_API_CALL(status, ENV, OH_JSVM_CreateArraybuffer(ENV, len, &retData, &result));
    if (status == JSVM_OK) {
        if (pData) {
            memcpy(retData, pData, len);
        }
    }
    return result;
}

JSVM_Value createJSABAligned(char* pData, int len) {
    int asz = (len + 3) & 0xfffffffc;
    JSVM_Value  result;
    JSVM_Status status;
    void*       retData;
    JSVM_API_CALL(status, ENV, OH_JSVM_CreateArraybuffer(ENV, asz, &retData, &result));
    if (status == JSVM_OK) {
        if (pData) {
            memcpy(retData, pData, len);
        }
    }
    return result;
}

bool extractJSAB(JSVM_Value ab, char*& data, int& len) {
    JSVM_Status status;
    auto env = ENV;
    bool isArrayBuffer;
    JSVM_API_CALL(status, env, OH_JSVM_IsArraybuffer(env, ab, &isArrayBuffer));
    if (isArrayBuffer) {
        size_t length = 0;
        JSVM_API_CALL(status, env, OH_JSVM_GetArraybufferInfo(env, ab, reinterpret_cast<void**>(&data), &length));
        len = length;
        return true;
    }
    bool isTypedArray;
    JSVM_API_CALL(status, env, OH_JSVM_IsTypedarray(env, ab, &isTypedArray));
    if (isTypedArray) {
        JSVM_TypedarrayType type;
        JSVM_Value           inputBuffer;
        size_t               byteOffset;
        size_t               arrayLength;
        void*                bufData = nullptr;
        JSVM_API_CALL(status, env, OH_JSVM_GetTypedarrayInfo(env, ab, &type, &arrayLength, &bufData, &inputBuffer, &byteOffset));
        data = (char*)(bufData);
        
        size_t bytesOfElement = 1;
        switch (type) {
        case JSVM_INT16_ARRAY:
        case JSVM_UINT16_ARRAY:
            bytesOfElement = 2;
            break;
        case JSVM_INT32_ARRAY:
        case JSVM_UINT32_ARRAY:
        case JSVM_FLOAT32_ARRAY:
            bytesOfElement = 4;
            break;
        case JSVM_FLOAT64_ARRAY:
        case JSVM_BIGINT64_ARRAY:
        case JSVM_BIGUINT64_ARRAY:
            bytesOfElement = 8;
            break;
        default:
            break;
        }
        len = arrayLength * bytesOfElement;
        return true;
    }
    return false;
}

    void __JSRun::ReportException(const char* stackStr, const char* messageStr) {
        
        if (stackStr == nullptr) {
            std::string str = "undefined stackStr";
            stackStr = str.c_str();
        }
        LOGE("JSVM error stack: %{public}s", stackStr);
        if (messageStr == nullptr) {
            std::string str = "undefined messageStr";
            messageStr = str.c_str();
        }
        LOGE("JSVM error message: %{public}s", messageStr);
        std::string kBuf = "if(conch.onerror){conch.onerror('";
        kBuf += UrlEncode("undefined");
        kBuf += "','";
        kBuf += UrlEncode(stackStr);
        kBuf += "','undefined','";
        kBuf += "undefined";
        kBuf += "','";
        kBuf += UrlEncode(messageStr);
        kBuf += "');};";
        laya::__JSRun::Run(kBuf.c_str(), nullptr);
		if (gbAlertException)
		{
            std::string errInfo = stackStr;
            errInfo += messageStr;
			JSAlert(errInfo.c_str());
		}
    }
}
#endif