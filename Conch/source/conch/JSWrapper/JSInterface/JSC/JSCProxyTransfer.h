
#ifndef _JSCProxyTrnasfer_h
#define _JSCProxyTrnasfer_h

#include <JavaScriptCore/JSContextRef.h>
#include <JavaScriptCore/JSStringRef.h>
#include <JavaScriptCore/JSValueRef.h>
#include <string>
#include <vector>
#include "JSCProxyTLS.h"
#include "JSCProxyType.h"


namespace laya
{
    void resetJsStrBuf();
    
    template <typename T>class JSCClass;
    
    template <typename T> class __TransferToCpp
    {
        public:
            static T ToCpp( JSValueRef p_vl ){
                assert(true);
                return T();
            }
            static bool is( JSValueRef p_vl ){
                return __CheckClassType::IsTypeOf<T>(p_vl);
            }
    };
    
    template <typename T> class __TransferToCpp<T*>
    {
        public:
        static T* ToCpp( JSValueRef p_vl ){
            JSContextRef pCtx = __TlsData::GetInstance()->GetCurContext();
            if( 0 == p_vl || JSValueIsUndefined(pCtx, p_vl) || JSValueIsNull(pCtx, p_vl) )
                return nullptr;
            else
                return (T*)JSObjectGetPrivate((JSObjectRef)p_vl);
            }
        static bool is( JSValueRef p_vl ){
            return __CheckClassType::IsTypeOf<T>(p_vl);
        }
    };

    
    template <> class __TransferToCpp<int32_t>
    {
        public:
        static int32_t ToCpp( JSValueRef p_vl ){
            JSContextRef pCtx = __TlsData::GetInstance()->GetCurContext();
            if(0!=p_vl)
                return (int32_t)JSValueToNumber(pCtx,p_vl,0);
            else return 0;
        }
        static bool is( JSValueRef p_vl ){
            JSContextRef pCtx = __TlsData::GetInstance()->GetCurContext();
            return JSValueIsNumber(pCtx, p_vl);
        }
    };
    template <>
    class __TransferToCpp<uint32_t>
    {
    public:
        static uint32_t ToCpp( JSValueRef p_vl )
        {
            JSContextRef pCtx = __TlsData::GetInstance()->GetCurContext();
            if(0!=p_vl)
                return (uint32_t)JSValueToNumber(pCtx,p_vl,0);
            else return 0;
        }
        static bool is( JSValueRef p_vl ){
            JSContextRef pCtx = __TlsData::GetInstance()->GetCurContext();
            return JSValueIsNumber(pCtx, p_vl);
        }
    };
    template <> class __TransferToCpp<int16_t>
    {
    public:
        static int16_t ToCpp( JSValueRef p_vl ){
            JSContextRef pCtx = __TlsData::GetInstance()->GetCurContext();
            if(0!=p_vl)
                return (int16_t)JSValueToNumber(pCtx,p_vl,0);
            else return 0;
        }
        static bool is( JSValueRef p_vl ){
            JSContextRef pCtx = __TlsData::GetInstance()->GetCurContext();
            return JSValueIsNumber(pCtx, p_vl);
        }
    };
    
    template <> class __TransferToCpp<intptr_t>
    {
    public:
        static intptr_t ToCpp( JSValueRef p_vl ){
            JSContextRef pCtx = __TlsData::GetInstance()->GetCurContext();
            if(0!=p_vl)
                return (intptr_t)JSValueToNumber(pCtx,p_vl,0);
            else return 0;
        }
        static bool is( JSValueRef p_vl ){
            JSContextRef pCtx = __TlsData::GetInstance()->GetCurContext();
            return JSValueIsNumber(pCtx, p_vl);
        }
};
    
    template <> class __TransferToCpp<uint16_t>
    {
        public:
        static uint16_t ToCpp( JSValueRef p_vl ){
            JSContextRef pCtx=__TlsData::GetInstance()->GetCurContext();
            if(0!=p_vl)
                return (uint16_t)JSValueToNumber(pCtx,p_vl,0);
            else
                return 0;
        }
        static bool is( JSValueRef p_vl ){
            JSContextRef pCtx = __TlsData::GetInstance()->GetCurContext();
            return JSValueIsNumber(pCtx, p_vl);
        }
    };
    
    /*template <> class __TransferToCpp<long>
    {
        public:
        static long ToCpp( JSValueRef p_vl ){
            JSContextRef pCtx=__TlsData::GetInstance()->GetCurContext();
            if(0!=p_vl)
                return (long)JSValueToNumber(pCtx,p_vl,0);
            else
                return 0;
        }
        static bool is( JSValueRef p_vl ){
            JSContextRef pCtx = __TlsData::GetInstance()->GetCurContext();
            return JSValueIsNumber(pCtx, p_vl);
        }
    };
    
    template <> class __TransferToCpp<unsigned long>
    {
        public:
        static unsigned long ToCpp( JSValueRef p_vl ){
            JSContextRef pCtx=__TlsData::GetInstance()->GetCurContext();
            if(0!=p_vl)
                return (unsigned long)JSValueToNumber(pCtx,p_vl,0);
            else
                return 0;
        }
        static bool is( JSValueRef p_vl ){
            JSContextRef pCtx = __TlsData::GetInstance()->GetCurContext();
            return JSValueIsNumber(pCtx, p_vl);
        }
    };*/
    
    template <> class __TransferToCpp<bool>
    {
        public:
        static bool ToCpp( JSValueRef p_vl ){
            JSContextRef pCtx=__TlsData::GetInstance()->GetCurContext();
            if(0!=p_vl)
                return JSValueToBoolean(pCtx,p_vl);
            else
                return false;
        }
        static bool is( JSValueRef p_vl ){
            JSContextRef pCtx = __TlsData::GetInstance()->GetCurContext();
            return JSValueIsBoolean(pCtx, p_vl);
        }
    };
    
    template <> class __TransferToCpp<float>
    {
        public:
        static float ToCpp( JSValueRef p_vl ){
            JSContextRef pCtx=__TlsData::GetInstance()->GetCurContext();
            if(0!=p_vl)
                return (float)JSValueToNumber(pCtx,p_vl,0);
            else
                return 0;
        }
        static bool is( JSValueRef p_vl ){
            JSContextRef pCtx = __TlsData::GetInstance()->GetCurContext();
            return JSValueIsNumber(pCtx, p_vl);
        }
    };
    
    template <> class __TransferToCpp<double>
    {
        public:
        static double ToCpp( JSValueRef p_vl ){
            JSContextRef pCtx=__TlsData::GetInstance()->GetCurContext();
            if(0!=p_vl)
                return JSValueToNumber(pCtx,p_vl,0);
            else
                return 0;
        }
        static bool is( JSValueRef p_vl ){
            JSContextRef pCtx = __TlsData::GetInstance()->GetCurContext();
            return JSValueIsNumber(pCtx, p_vl);
        }
    };
    
    template <> class __TransferToCpp<int64_t>
    {
        public:
        static int64_t ToCpp( JSValueRef p_vl ){
            JSContextRef pCtx=__TlsData::GetInstance()->GetCurContext();
            if(0!=p_vl)
            {
                double value = JSValueToNumber(pCtx,p_vl,0);
                return *reinterpret_cast<int64_t*>(&value);
            }
            else
                return 0;
        }
        static bool is( JSValueRef p_vl ){
            JSContextRef pCtx = __TlsData::GetInstance()->GetCurContext();
            return JSValueIsNumber(pCtx, p_vl);
        }
    };
    
    template <> class __TransferToCpp<uint64_t>
    {
        public:
        static uint64_t ToCpp( JSValueRef p_vl ){
            JSContextRef pCtx=__TlsData::GetInstance()->GetCurContext();
            if(0!=p_vl)
            {
                double value = JSValueToNumber(pCtx,p_vl,0);
                return *reinterpret_cast<uint64_t*>(&value);
            }
            else return 0;
        }
        static bool is( JSValueRef p_vl ){
            JSContextRef pCtx = __TlsData::GetInstance()->GetCurContext();
            return JSValueIsNumber(pCtx, p_vl);
        }
    };
    
    
    template <> class __TransferToCpp<JSValueRef>
    {
    public:
        static JSValueRef ToCpp( JSValueRef p_vl ){
            return p_vl;
        }
        static bool is( JSValueRef p_vl ){
            return true;
        }
    };
    
    char* JsCharToC(JSValueRef p_vl);
    char* JsCharToC(JSStringRef p_vl);
#define __DeclareStringTransferToCpp(Tp)    \
    template <> class __TransferToCpp<Tp>   \
    {   \
    public: \
        static char *ToCpp( JSValueRef p_vl )   \
        {   \
            return JsCharToC(p_vl); \
        }   \
        static char *ToCpp( JSStringRef p_vl )   \
        {   \
            return JsCharToC(p_vl); \
        }   \
        static std::string ToCppStd( JSValueRef p_vl )  \
        {   \
            return JsCharToC(p_vl); \
        }   \
        static std::string ToCppStd( JSStringRef p_vl )  \
        {   \
            return JsCharToC(p_vl); \
        }  \
        static bool is( JSValueRef p_vl ){ \
            JSContextRef pCtx = __TlsData::GetInstance()->GetCurContext(); \
            return JSValueIsString(pCtx, p_vl); \
        } \
    };
    
    __DeclareStringTransferToCpp(char *);
    __DeclareStringTransferToCpp(const char *);
    __DeclareStringTransferToCpp(unsigned char *);
    __DeclareStringTransferToCpp(const unsigned char *);
    
    template <typename T>
    class __TransferToJs
    {
    public:
        static JSValueRef ToJs(T p_vl)
        {
            assert(true);
            JSContextRef pCtx=__TlsData::GetInstance()->GetCurContext();
            return JSValueMakeUndefined(pCtx);
        }
    };

    template <typename T>
    class __TransferToJs<T*>
    {
    public:
        static JSValueRef ToJs(T* p_vl)
        {
            return JSCClass<T>::getInstance()->transferObjPtrToJS(p_vl);
        }
    };
    
    template <> class __TransferToJs<void>
    {public:static JSValueRef ToJs( int p_vl )
        {
            JSContextRef pCtx=__TlsData::GetInstance()->GetCurContext();
            if(0==p_vl)
                return JSValueMakeUndefined(pCtx);
            else
                return JSValueMakeNull(pCtx);
        }};
    
    template <> class __TransferToJs<JSValueRef>
    {public:static JSValueRef ToJs( JSValueRef p_vl ){return p_vl;}};
    
    
    template <> class __TransferToJs<bool>
    {public:static JSValueRef ToJs( bool p_vl ){return JSValueMakeBoolean(__TlsData::GetInstance()->GetCurContext(),p_vl);}};
    
    template <> class __TransferToJs<int32_t>
    {public:static JSValueRef ToJs(int32_t p_vl ){return JSValueMakeNumber(__TlsData::GetInstance()->GetCurContext(),(double)p_vl);}};
    
    template <> class __TransferToJs<uint32_t>
    {public:static JSValueRef ToJs(uint32_t p_vl ){return JSValueMakeNumber(__TlsData::GetInstance()->GetCurContext(),(double)p_vl);}};
    
    template <> class __TransferToJs<float>
    {public:static JSValueRef ToJs( float p_vl ){return JSValueMakeNumber(__TlsData::GetInstance()->GetCurContext(),(double)p_vl);}};
    
    template <> class __TransferToJs<double>
    {public:static JSValueRef ToJs( double p_vl ){return JSValueMakeNumber(__TlsData::GetInstance()->GetCurContext(),p_vl);}};
    
    template <> class __TransferToJs<int64_t>
    {
    public:
        static JSValueRef ToJs(int64_t p_vl )
        {
            double value = *reinterpret_cast<int64_t*>(&p_vl);
            return JSValueMakeNumber(__TlsData::GetInstance()->GetCurContext(), value);
            
        }
        
        static JSValueRef ToJsDate(int64_t p_vl )
        {
            JSValueRef n = ToJs(p_vl);
            return JSObjectMakeDate(__TlsData::GetInstance()->GetCurContext(), 1, &n, 0);
        }
    };
    template <> class __TransferToJs<intptr_t>
    {
    public:
        static JSValueRef ToJs(intptr_t p_vl )
        {
            double value = *reinterpret_cast<int64_t*>(&p_vl);
            return JSValueMakeNumber(__TlsData::GetInstance()->GetCurContext(), value);
        }
    };
	/*template <> class __TransferToJs<long>
    {
    public:
        static JSValueRef ToJs( long p_vl ){return JSValueMakeNumber(__TlsData::GetInstance()->GetCurContext(),(double)p_vl);}
        
        static JSValueRef ToJsDate( long p_vl )
        {
            JSValueRef n = ToJs(p_vl);
            return JSObjectMakeDate(__TlsData::GetInstance()->GetCurContext(), 1, &n, 0);
        }
    };*/
    
    template <> class __TransferToJs<uint64_t>
    {
    public:
        static JSValueRef ToJs(uint64_t p_vl )
        {
            double value = *reinterpret_cast<uint64_t*>(&p_vl);
            return JSValueMakeNumber(__TlsData::GetInstance()->GetCurContext(), value);
        }
    
        static JSValueRef ToJsDate(uint64_t p_vl )
        {
            JSValueRef n = ToJs(p_vl);
            return JSObjectMakeDate(__TlsData::GetInstance()->GetCurContext(), 1, &n, 0);
        }
    };
    
    template <> class __TransferToJs<char *>
    {public:static JSValueRef ToJs( char *p_vl )
        {
        JSStringRef pStr = JSStringCreateWithUTF8CString(p_vl);
        JSValueRef pRet = JSValueMakeString(__TlsData::GetInstance()->GetCurContext(), pStr);
        JSStringRelease(pStr);
        return pRet;
        }};
    
    template <> class __TransferToJs<const char *>
    {public:static JSValueRef ToJs( const char *p_vl )
        {
            JSStringRef pStr = JSStringCreateWithUTF8CString(p_vl);
            JSValueRef pRet = JSValueMakeString(__TlsData::GetInstance()->GetCurContext(), pStr);
            JSStringRelease(pStr);
            return pRet;
        }};
    
    template <> class __TransferToJs<unsigned char *>
    {public:static JSValueRef ToJs( unsigned char *p_vl )
        {
            JSStringRef pStr = JSStringCreateWithUTF8CString((char *)p_vl);
            JSValueRef pRet = JSValueMakeString(__TlsData::GetInstance()->GetCurContext(), pStr);
            JSStringRelease(pStr);
            return pRet;
        }};
    
    template <> class __TransferToJs<const unsigned char *>
    {public:static JSValueRef ToJs( const unsigned char *p_vl )
        {
            JSStringRef pStr = JSStringCreateWithUTF8CString((char *)p_vl);
            JSValueRef pRet = JSValueMakeString(__TlsData::GetInstance()->GetCurContext(), pStr);
            JSStringRelease(pStr);
            return pRet;
        }};

	template <> class __TransferToJs<std::string>
	{public:static JSValueRef ToJs( std::string p_vl )
	{
		JSStringRef pStr = JSStringCreateWithUTF8CString(p_vl.c_str());
		JSValueRef pRet = JSValueMakeString(__TlsData::GetInstance()->GetCurContext(), pStr);
		JSStringRelease(pStr);
		return pRet;
	}};
    
    template <typename T> class __JsArray{
    public:
        static JSObjectRef ToJsArray(const std::vector<T*>& p_vl)
        {
            int p_iSize = p_vl.size();
            
            JSValueRef pValArray[p_iSize];
            for(int i=0;i<p_iSize;++i)
            {
                pValArray[i] = __TransferToJs<T*>::ToJs(p_vl[i]);
            }
            
            JSObjectRef pRet = JSObjectMakeArray( __TlsData::GetInstance()->GetCurContext(), p_iSize, pValArray, 0 );
            return pRet;
        }
        
        static JSObjectRef ToJsArray(const std::vector<T>& p_vl)
        {
            
            int p_iSize = p_vl.size();
            
            JSValueRef pValArray[p_iSize];
            for(int i=0;i<p_iSize;++i)
            {
                pValArray[i] = __TransferToJs<T>::ToJs(p_vl[i]);
            }
            
            JSObjectRef pRet = JSObjectMakeArray( __TlsData::GetInstance()->GetCurContext(), p_iSize, pValArray, 0 );
            return pRet;
        }
    };
    
    class __JsByteArray
    {
    public:
        static JSObjectRef ToJsByteArray( const unsigned char *p_vl, int p_iSize )
        {
            if( 0 == p_vl || p_iSize <= 0 )
            {
                return NULL;
            }
            else
            {
                JSValueRef pValArray[p_iSize];
                for(int i=0;i<p_iSize;++i)
                {
                    pValArray[i] = __TransferToJs<unsigned int>::ToJs(p_vl[i]);
                }
                
                JSObjectRef pRet = JSObjectMakeArray( __TlsData::GetInstance()->GetCurContext(), p_iSize, pValArray, 0 );
                return pRet;
            }
        }
    };


    
    template <typename T> class __TransferToJs<std::vector<T*> >
    {
    public:static JSObjectRef ToJs( const std::vector<T*>& p_vl )
        {
            return __JsArray<T>::ToJsArray(p_vl);
        }
    };
    
    template <typename T> class __TransferToJs<std::vector<T> >
    {
    public:static JSObjectRef ToJs(const std::vector<T>& p_vl)
        {
            return __JsArray<T>::ToJsArray(p_vl);
        }
    };
    
}

#endif
