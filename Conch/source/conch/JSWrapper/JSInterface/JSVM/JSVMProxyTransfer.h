#ifndef _JSCProxyTrnasfer_h
#define _JSCProxyTrnasfer_h

#include <ark_runtime/jsvm.h>
#include <ark_runtime/jsvm_types.h>
#include <cassert>
#include <cstdint>
#include <string>
#include <unistd.h>
#include <vector>
#include "JSVMProxyTLS.h"
#include "JSVMProxyType.h"
#include "JSVMEnv.h"
#include "IsolateData.h"

#define ENV Javascript::getEnv()

namespace laya{

void resetJsStrBuf();

template<class T> JSVM_Value createJsObjAttachCObj(T* cobj);

template <typename T> class __TransferToCpp {
public:
    static T ToCpp(JSVM_Value p_vl) {
        assert(false && "should not reach here");
        return T();
    }
    //问题：如果是对象的话，当请求 T或者T* 认为是相同的
    static bool is(JSVM_Value p_vl) {
        assert(false && "should not reach here");
        return false;
    }
};

//不知道怎么区分对象和对象指针，先硬写一个
template <typename T> class __TransferToCpp<T*> {
public:
    static T *ToCpp(JSVM_Value p_vl) {
        JSVM_Status status;
        bool is_null_or_undefined;
        JSVM_API_CALL(status, ENV, OH_JSVM_IsNullOrUndefined(ENV, p_vl, &is_null_or_undefined));
        if (0 == p_vl || is_null_or_undefined)
            return (T *)0;
        else {
            void *result;
            JSVM_API_CALL(status, ENV, OH_JSVM_Unwrap(ENV, p_vl, &result));
            return static_cast<T *>(result);
        }
    }
    //问题：如果是对象的话，当请求 T或者T* 认为是相同的
    static bool is(JSVM_Value p_vl) {
		JSVM_Status status;
        bool is_null_or_undefined;
        JSVM_API_CALL(status, ENV, OH_JSVM_IsNullOrUndefined(ENV, p_vl, &is_null_or_undefined));
        if (0 == p_vl || is_null_or_undefined)
            return false;
        else {
            void *result;
            JSVM_API_CALL(status, ENV, OH_JSVM_Unwrap(ENV, p_vl, &result));
            if (static_cast<T *>(result) == &T::JSCLSINFO) {
                return true;
            }
            return false;
        }
    }
};

template <> class __TransferToCpp<int32_t> {
public:
    static int32_t ToCpp(JSVM_Value p_vl) {
        JSVM_Status status;
        int32_t result;
        JSVM_API_CALL(status, ENV, OH_JSVM_GetValueInt32(ENV, p_vl, &result));
        return result;
    }

    static bool is(JSVM_Value p_vl) {
        JSVM_Status status;
        bool is_number;
        JSVM_API_CALL(status, ENV, OH_JSVM_IsNumber(ENV, p_vl, &is_number));
        return is_number;
    }
};


inline int64_t getInt64Noloss(JSVM_Value value)
{
    int64_t result;
    JSVM_Status status;
    bool lossless;
    status = OH_JSVM_GetValueBigintInt64(ENV, value, &result, &lossless);
    //DEBUG_CHECK(lossless == true);
    //DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return result;
}
inline JSVM_Value makeInt64Noloss(int64_t value)
{
    JSVM_Value result;
    JSVM_Status status;
    status = OH_JSVM_CreateBigintInt64(ENV, value, &result);
    return result;
}
inline uint64_t getUint64Noloss(JSVM_Value value)
{
    uint64_t result;
    JSVM_Status status;
    bool lossless;
    status = OH_JSVM_GetValueBigintUint64(ENV, value, &result, &lossless);
    //DEBUG_CHECK(lossless == true);
    //DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return result;
}
inline JSVM_Value makeUint64Noloss(uint64_t value)
{
    JSVM_Value result;
    JSVM_Status status;
    status = OH_JSVM_CreateBigintUint64(ENV, value, &result);
    return result;
}
inline bool isBigInt(JSVM_Value value)
{
    JSVM_ValueType valueType;
    JSVM_Status status = OH_JSVM_Typeof(ENV, value, &valueType);
    //DEBUG_CHECK(status == jsvm_status::jsvm_ok);
    return valueType == JSVM_ValueType::JSVM_BIGINT;
}
// 用bigint 保证精度不丢失,可以用于bullet对象指针
template <> class __TransferToCpp<int64_t> {
public:
    static int64_t ToCpp(JSVM_Value p_vl) {
        //JSVM_Status status;
        //int64_t result;
        //JSVM_API_CALL(status, ENV, OH_JSVM_GetValueInt64(ENV, p_vl, &result));
        //return result;

        JSVM_ValueType valueType;
        JSVM_Status status = OH_JSVM_Typeof(ENV, p_vl, &valueType);
        //DEBUG_CHECK(status == JSVM_OK);
        if (valueType == JSVM_ValueType::JSVM_NULL || valueType == JSVM_ValueType::JSVM_UNDEFINED)
        {
            return 0;
        }
        return getInt64Noloss(p_vl);
    }

    static bool is(JSVM_Value p_vl) {
        //JSVM_Status status;
        //bool is_number;
        //JSVM_API_CALL(status, ENV, OH_JSVM_IsNumber(ENV, p_vl, &is_number));
        //return is_number;
        return isBigInt(p_vl);
    }
};

template <> class __TransferToCpp<uint32_t> {
public:
    static uint32_t ToCpp(JSVM_Value p_vl) { 
        JSVM_Status status;
        uint32_t result;
        JSVM_API_CALL(status, ENV, OH_JSVM_GetValueUint32(ENV, p_vl, &result));
        return result;
    }

    static bool is(JSVM_Value p_vl) { 
        JSVM_Status status;
        bool is_number;
        JSVM_API_CALL(status, ENV, OH_JSVM_IsNumber(ENV, p_vl, &is_number));
        return is_number;
    }
};

template <> class __TransferToCpp<uint64_t> {
public:
    static uint64_t ToCpp(JSVM_Value p_vl) { 
        //JSVM_Status status;
        //int64_t result;
        //JSVM_API_CALL(status, ENV, OH_JSVM_GetValueInt64(ENV, p_vl, &result));
        //return *reinterpret_cast<uint64_t *>(&result);
        return getUint64Noloss(p_vl);
    }

    static bool is(JSVM_Value p_vl) {
        //JSVM_Status status;
        //bool is_number;
        //JSVM_API_CALL(status, ENV, OH_JSVM_IsNumber(ENV, p_vl, &is_number));
        //return is_number;
        return isBigInt(p_vl);
    }
};

//   template <> class __TransferToCpp<int>{public:
//	static int ToCpp( Local<Value> p_vl ){return p_vl->Int32Value();}
//	static bool is(Local<Value> p_vl) { return p_vl->IsInt32(); }
//};

//template <> class __TransferToCpp<short> {public:
//	static short ToCpp(Local<Value> p_vl) { return p_vl->Int32Value(); }
//	static bool is(Local<Value> p_vl) { return p_vl->IsInt32(); }
//};

//   template <> class __TransferToCpp<unsigned int> {public:
//	static unsigned int ToCpp( Local<Value> p_vl ){return p_vl->Uint32Value();}
//	static bool is(Local<Value> p_vl) { return p_vl->IsUint32(); }
//};

//   template <> class __TransferToCpp<long>{public:
//	static long ToCpp( Local<Value> p_vl ){return p_vl->Int32Value();}
//	static bool is(Local<Value> p_vl) { return p_vl->IsInt32(); }
//};

//   template <> class __TransferToCpp<unsigned long>{public:
//	static unsigned long ToCpp( Local<Value> p_vl ){return p_vl->Uint32Value();}
//	static bool is(Local<Value> p_vl) { return p_vl->IsUint32(); }
//};
    
template <> class __TransferToCpp<bool> {
public:
    static bool ToCpp(JSVM_Value p_vl) {
        JSVM_Status status;
        bool result;
        JSVM_API_CALL(status, ENV, OH_JSVM_GetValueBool(ENV, p_vl, &result));
        return result;
    }

    static bool is(JSVM_Value p_vl) {
        JSVM_Status status;
        bool is_number;
        JSVM_API_CALL(status, ENV, OH_JSVM_IsBoolean(ENV, p_vl, &is_number));
        return is_number;
    }
};
    
template <> class __TransferToCpp<float> {
public:
    static float ToCpp(JSVM_Value p_vl) {
        JSVM_Status status;
        double result;
        JSVM_API_CALL(status, ENV, OH_JSVM_GetValueDouble(ENV, p_vl, &result));
        return static_cast<float>(result);
    }

    static bool is(JSVM_Value p_vl) {
        JSVM_Status status;
        bool is_number;
        JSVM_API_CALL(status, ENV, OH_JSVM_IsNumber(ENV, p_vl, &is_number));
        return is_number;
    }
};
    
template <> class __TransferToCpp<double> {
public:
    static double ToCpp(JSVM_Value p_vl) {
        JSVM_Status status;
        double result;
        JSVM_API_CALL(status, ENV, OH_JSVM_GetValueDouble(ENV, p_vl, &result));
        return result;
    }

    static bool is(JSVM_Value p_vl) { 
        JSVM_Status status;
        bool is_number;
        JSVM_API_CALL(status, ENV, OH_JSVM_IsNumber(ENV, p_vl, &is_number));
        return is_number;
    }
};
    
//   template <> class __TransferToCpp<long long>{public:
//	static long long ToCpp( Local<Value> p_vl ){return p_vl->IntegerValue();}
//	static bool is(Local<Value> p_vl) { return p_vl->IsNumber(); }
//};

//   template <> class __TransferToCpp<unsigned long long>{public:
//	static unsigned long long ToCpp( Local<Value> p_vl ){return (unsigned long long)p_vl->IntegerValue();}
//	static bool is(Local<Value> p_vl) { return p_vl->IsNumber(); }
//};

template <> class __TransferToCpp<JSVM_Value> {
public:
    static JSVM_Value ToCpp(JSVM_Value p_vl) { return p_vl; }
    static bool is(JSVM_Value p_vl) { return true; }
};

//注意：这个函数返回的字符串使用了一个通用buffer，所以注意复制。
//这个函数必须配合 resetJsStrBuf 函数使用。
//例如 
// char* p1 = JsCharToC(args[0]);
// char* p2 = JsCharToC(args[1]);
// use(p1,p2);
// resetJsStrBuf();
char *JsCharToC(JSVM_Value p_vl);
class transToCharPtr {
public:
    static char *ToCpp1(JSVM_Value p_vl) {
        JSVM_Status status;
        size_t bufsize;
        size_t result;
        JSVM_API_CALL(status, ENV, OH_JSVM_GetValueStringUtf8(ENV, p_vl, nullptr, bufsize, &result));
        bufsize = result + 1;
        char *buf = new char[bufsize];
        JSVM_API_CALL(status, ENV, OH_JSVM_GetValueStringUtf8(ENV, p_vl, buf, bufsize, &result));
        return buf;
    }
    static std::string ToCppStd(JSVM_Value p_vl) {
        JSVM_Status status;
        size_t bufsize;
        size_t result;
        JSVM_API_CALL(status, ENV, OH_JSVM_GetValueStringUtf8(ENV, p_vl, nullptr, bufsize, &result));
        bufsize = result + 1;
        char *buf = new char[bufsize];
        JSVM_API_CALL(status, ENV, OH_JSVM_GetValueStringUtf8(ENV, p_vl, buf, bufsize, &result));
        return std::string(buf);
    }
};

template <> class __TransferToCpp<char *> {
public:
    //注意：这个函数返回的字符串使用了一个通用buffer，所以注意复制
    static char* ToCpp(JSVM_Value p_vl) { return JsCharToC(p_vl); }
    static bool is(JSVM_Value p_vl) { 
        JSVM_Status status;
        bool result;
        JSVM_API_CALL(status, ENV, OH_JSVM_IsString(ENV, p_vl, &result));
        return result;
    }
    static std::string ToCppStd(JSVM_Value p_vl) { return transToCharPtr::ToCppStd(p_vl); }
};
template <> class __TransferToCpp<const char *> {
public:
    //注意：这个函数返回的字符串使用了一个通用buffer，所以注意复制
    static const char* ToCpp(JSVM_Value p_vl) { return (const char *)JsCharToC(p_vl); }
    static bool is(JSVM_Value p_vl) { 
        JSVM_Status status;
        bool result;
        JSVM_API_CALL(status, ENV, OH_JSVM_IsString(ENV, p_vl, &result));
        return result;
    }
    static std::string ToCppStd(JSVM_Value p_vl) { return transToCharPtr::ToCppStd(p_vl); }
};
template <> class __TransferToCpp<unsigned char *> {
public:
    //注意：这个函数返回的字符串使用了一个通用buffer，所以注意复制
    static unsigned char* ToCpp(JSVM_Value p_vl) { return (unsigned char *)JsCharToC(p_vl); } 
    static bool is(JSVM_Value p_vl) { 
        JSVM_Status status;
        bool result;
        JSVM_API_CALL(status, ENV, OH_JSVM_IsString(ENV, p_vl, &result));
        return result;
    }
    static std::string ToCppStd(JSVM_Value p_vl) { return transToCharPtr::ToCppStd(p_vl); }
};
template <> class __TransferToCpp<const unsigned char *> {
public:
    //注意：这个函数返回的字符串使用了一个通用buffer，所以注意复制
    static const unsigned char* ToCpp(JSVM_Value p_vl) { return (const unsigned char*)JsCharToC(p_vl); }
    static bool is(JSVM_Value p_vl) {
        JSVM_Status status;
        bool result;
        JSVM_API_CALL(status, ENV, OH_JSVM_IsString(ENV, p_vl, &result));
        return result;
    }
    static std::string ToCppStd(JSVM_Value p_vl) { return transToCharPtr::ToCppStd(p_vl); }
};

//template<> class __TransferToCpp<laya::JSArrayBuffer*>{public:
//	static bool is(Local<Value> p_vl) { return p_vl->IsArrayBuffer() || p_vl->IsArrayBufferView(); }
//	static laya::JSArrayBuffer* ToCpp(Local<Value> p_vl) { return laya::JSArrayBuffer::fromeJSObj(p_vl); };
//};
	
template <typename T> class __TransferToJs {
public:
    static JSVM_Value ToJs(T p_vl) { 
        assert(false && "type not handled");
        JSVM_Status status;
        JSVM_Value result;
        JSVM_API_CALL(status, ENV, OH_JSVM_GetUndefined(ENV, &result));
        return result;
    }
};
	
template <typename T> class __TransferToJs<T*> {
public:
    static JSVM_Value ToJs(T* p_vl) { return laya::createJsObjAttachCObj(p_vl); }
};
    
//为什么不直接把没有参数作为void？
template <> class __TransferToJs<void> {
public:
    static JSVM_Value ToJs(int p_vl) {
        if(0 == p_vl) {
            JSVM_Status status;
            JSVM_Value result;
            JSVM_API_CALL(status, ENV, OH_JSVM_GetUndefined(ENV, &result));
            return result;
        } else {
            JSVM_Status status;
            JSVM_Value result;
            JSVM_API_CALL(status, ENV, OH_JSVM_GetNull(ENV, &result));
            return result;
        }
    }
};

template <> class __TransferToJs<JSVM_Value> {
public:
    static JSVM_Value ToJs(JSVM_Value p_vl) { return p_vl; }
};

template <> class __TransferToJs<bool> {
public:
    static JSVM_Value ToJs(bool p_vl) {
        JSVM_Status status;
        JSVM_Value result;
        JSVM_API_CALL(status, ENV, OH_JSVM_GetBoolean(ENV, p_vl, &result));
        return result;
    }
};
    
/*template <> 
class __TransferToJs<int32_t>
{
public:
    static v8::Local<Value> ToJs(int32_t p_vl )
    {
        return Int32::New(Isolate::GetCurrent(), p_vl);
    }
};*/
    
template <> class __TransferToJs<uint32_t> {
public:
    static JSVM_Value ToJs(uint32_t p_vl) {
        JSVM_Status status;
        JSVM_Value result;
        JSVM_API_CALL(status, ENV, OH_JSVM_CreateUint32(ENV, p_vl, &result));
        return result;
    }
};
    
/*template <> class __TransferToJs<long unsigned int>
{
public:static Handle<Value> ToJs(long unsigned int p_vl) { return Number::New(Isolate::GetCurrent(), p_vl); }
};*/
	
template <> class __TransferToJs<float> {
public:
    static JSVM_Value ToJs(float p_vl) {
        JSVM_Status status;
        JSVM_Value result;
        JSVM_API_CALL(status, ENV, OH_JSVM_CreateDouble(ENV, p_vl, &result));
        return result;
    }
};
    
template <> class __TransferToJs<double> {
public:
    static JSVM_Value ToJs(double p_vl) {
        JSVM_Status status;
        JSVM_Value result;
        JSVM_API_CALL(status, ENV, OH_JSVM_CreateDouble(ENV, p_vl, &result));
        return result;
    }
};
    
template <> class __TransferToJs<int64_t> {
public:
    static JSVM_Value ToJs(int64_t p_vl) {
        //JSVM_Status status;
        //JSVM_Value result;
        //JSVM_API_CALL(status, ENV, OH_JSVM_CreateInt64(ENV, p_vl, &result));
        //return result;
        return makeInt64Noloss(p_vl);
    }

    static JSVM_Value ToJsDate(int64_t p_vl) {
        JSVM_Status status;
        JSVM_Value result;
        JSVM_API_CALL(status, ENV, OH_JSVM_CreateDate(ENV, (double)p_vl, &result));
        return result;
    }
};

template <> class __TransferToJs<int32_t> {
public:
    static JSVM_Value ToJs(int32_t p_vl) {
        JSVM_Status status;
        JSVM_Value result;
        JSVM_API_CALL(status, ENV, OH_JSVM_CreateInt32(ENV, p_vl, &result));
        return result;
    }

    static JSVM_Value ToJsDate(int32_t p_vl) {
        JSVM_Status status;
        JSVM_Value result;
        JSVM_API_CALL(status, ENV, OH_JSVM_CreateDate(ENV, (double)p_vl, &result));
        return result;
    }
};
    
template <> class __TransferToJs<uint64_t> {
public:
    static JSVM_Value ToJs(uint64_t p_vl) {
        //JSVM_Status status;
        //JSVM_Value result;
        //JSVM_API_CALL(status, ENV, OH_JSVM_CreateInt64(ENV, p_vl, &result));
        //return result;
        return makeUint64Noloss(p_vl);
    }

    static JSVM_Value ToJsDate(uint64_t p_vl) {
        JSVM_Status status;
        JSVM_Value result;
        JSVM_API_CALL(status, ENV, OH_JSVM_CreateDate(ENV, (double)p_vl, &result));
        return result;
    }
};
    
template <> class __TransferToJs<char *> {
public:
    static JSVM_Value ToJs(char *p_vl) {
        if(p_vl == 0) 
            p_vl = (char *)"";
        JSVM_Status status;
        JSVM_Value result;
        JSVM_API_CALL(status, ENV, OH_JSVM_CreateStringUtf8(ENV, p_vl, strlen(p_vl), &result));
        
        return result;
    }
};
    
template <> class __TransferToJs<const char *> {
public:
    static JSVM_Value ToJs(const char *p_vl) {
        if(p_vl == 0)
            p_vl = (const char *)"";
        JSVM_Status status;
        JSVM_Value result;
        JSVM_API_CALL(status, ENV, OH_JSVM_CreateStringUtf8(ENV, p_vl, strlen(p_vl), &result));
        return result;
    }
};

template <> class __TransferToJs<unsigned char *> {
public:
    static JSVM_Value ToJs(const unsigned char *p_vl) {
        if(p_vl == 0) 
            p_vl = (const unsigned char *)"";
        JSVM_Status status;
        JSVM_Value result;
        JSVM_API_CALL(status, ENV, OH_JSVM_CreateStringUtf8(ENV, (const char *)p_vl, strlen((const char *)p_vl), &result));
        return result;
    }
};

template <> class __TransferToJs<const unsigned char *> {
public:
    static JSVM_Value ToJs(const unsigned char *p_vl) {
        if(p_vl == 0) 
            p_vl = (const unsigned char *)""; 
        JSVM_Status status;
        JSVM_Value result;
        JSVM_API_CALL(status, ENV, OH_JSVM_CreateStringUtf8(ENV, (const char *)p_vl, strlen((const char *)p_vl), &result));
        return result;
    }
};

template <> class __TransferToJs<std::string> {
public:
    static JSVM_Value ToJs(std::string p_vl ) {
        JSVM_Status status;
        JSVM_Value result;
        JSVM_API_CALL(status, ENV, OH_JSVM_CreateStringUtf8(ENV, p_vl.c_str(), p_vl.length(), &result));
        return result;
    }
};

template <> class __TransferToJs<std::string &> {
public:
    static JSVM_Value ToJs(std::string &p_vl) { 
        JSVM_Status status;
        JSVM_Value result;
        JSVM_API_CALL(status, ENV, OH_JSVM_CreateStringUtf8(ENV, p_vl.c_str(), p_vl.length(), &result));
        return result;
    }
};
//template <> class __TransferToJs<laya::JSArrayBuffer*>
//{public:static Handle<Value> ToJs( laya::JSArrayBuffer* p_vl ){return p_vl->toLocal();}};
template <typename T> class __JsArray {
public:
    static JSVM_Value ToJsArray(const std::vector<T *> &p_v1) {
        JSVM_Status status;
        JSVM_Value result;
        JSVM_API_CALL(status, ENV, OH_JSVM_CreateArrayWithLength(ENV, p_v1.size(), &result));
        for (int i = 0; i < p_v1.size(); i++) {
            JSVM_API_CALL(status, ENV, OH_JSVM_SetElement(ENV, result, i, __TransferToJs<T*>::ToJs(p_v1.at(i))));
        }
        return result;
    }
    static JSVM_Value ToJsArray(const std::vector<T> &p_v1) {
        JSVM_Status status;
        JSVM_Value result;
        JSVM_API_CALL(status, ENV, OH_JSVM_CreateArrayWithLength(ENV, p_v1.size(), &result));
        for (int i = 0; i < p_v1.size(); i++) {
            JSVM_API_CALL(status, ENV, OH_JSVM_SetElement(ENV, result, i, __TransferToJs<T>::ToJs(p_v1.at(i))));
        }
        return result;
    }
};
class __JsByteArray {
public:
    static JSVM_Value ToJsByteArray(const unsigned char *p_vl, int p_iSize) {
        if( 0 == p_vl || p_iSize <= 0 ) {
            JSVM_Status status;
            JSVM_Value result;
            JSVM_API_CALL(status, ENV, OH_JSVM_GetNull(ENV, &result));
            return result;
        } else {
            JSVM_Status status;
            JSVM_Value result;
            JSVM_API_CALL(status, ENV, OH_JSVM_CreateArrayWithLength(ENV, p_iSize, &result));
            for (int i = 0; i < p_iSize; i++) {
                JSVM_Value j;
                JSVM_API_CALL(status, ENV, OH_JSVM_CreateInt32(ENV, (int32_t)p_vl[i], &j));
                JSVM_API_CALL(status, ENV, OH_JSVM_SetElement(ENV, result, i, j));
            }
            return result;
        }
    }
};

template <typename T> class __TransferToJs<std::vector<T *>> {
public:
    static JSVM_Value ToJs(const std::vector<T*>& p_vl) { return __JsArray<T>::ToJsArray(p_vl); }
};

template <typename T> class __TransferToJs<std::vector<T>> {
public:
    static JSVM_Value ToJs(const std::vector<T> &p_vl) { return __JsArray<T>::ToJsArray(p_vl); }
};

}

#endif
