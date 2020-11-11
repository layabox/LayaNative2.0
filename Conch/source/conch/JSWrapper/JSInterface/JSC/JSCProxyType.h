
#ifndef __JSCProxyType_h
#define __JSCProxyType_h
#define NDEBUG
#include <assert.h>
#include "JSCProxyTLS.h"



#define __Js_class_typeid_property  "__cppclstypeid"

namespace laya
{
    typedef enum
    {
        __VT_void       = 0,
        __VT_string,
        __VT_bool,
        __VT_int,
        __VT_long,
        __VT_float,
        __VT_double,
        __VT_longlong,
		__VT_ArrayBuffer,
        __VT_object,
    } __ValueType;

    template <class _Key> struct __InferType
    {__ValueType iType;__InferType(){iType=__VT_object;}};
    
    template<> struct __InferType<void>
    {__ValueType iType;__InferType(){iType=__VT_void;}};
    
    template<> struct __InferType<bool>
    {__ValueType iType;__InferType(){iType=__VT_bool;}};
    
    template<> struct __InferType<char *>
    {__ValueType iType;__InferType(){iType=__VT_string;}};
    
    template<> struct __InferType<const char *>
    {__ValueType iType;__InferType(){iType=__VT_string;}};
    
    template<> struct __InferType<unsigned char *>
    {__ValueType iType;__InferType(){iType=__VT_string;}};
    
    template<> struct __InferType<const unsigned char *>
    {__ValueType iType;__InferType(){iType=__VT_string;}};
    
    template<> struct __InferType<int>
    {__ValueType iType;__InferType(){iType=__VT_int;}};
    
    template<> struct __InferType<unsigned int>
    {__ValueType iType;__InferType(){iType=__VT_int;}};
    
    template<> struct __InferType<long>
    {__ValueType iType;__InferType(){iType=__VT_long;}};
    
    template<> struct __InferType<unsigned long>
    {__ValueType iType;__InferType(){iType=__VT_long;}};
    
    template<> struct __InferType<float>
    {__ValueType iType;__InferType(){iType=__VT_float;}};
    
    template<> struct __InferType<double>
    {__ValueType iType;__InferType(){iType=__VT_double;}};
    
    template<> struct __InferType<long long>
    {__ValueType iType;__InferType(){iType=__VT_longlong;}};
    
    template<> struct __InferType<unsigned long long>
    {__ValueType iType;__InferType(){iType=__VT_longlong;}};

    template<typename T>
    class JSCClass;
    
    class __CheckClassType
    {
    public:
        template <typename T>
        static bool IsTypeOf( JSObjectRef p_pObj )
        {
            if( 0 == p_pObj )
                return false;
            
            JSStringRef pszName = JSStringCreateWithUTF8CString(__Js_class_typeid_property);
            JSValueRef pProperty = JSObjectGetProperty(__TlsData::GetInstance()->GetCurContext(), p_pObj, pszName, 0);
            JSStringRelease(pszName);
            
            if( 0 == pProperty )
            {
                return false;
            }
            else
            {
                JSContextRef pCtx = __TlsData::GetInstance()->GetCurContext();
                int nID = (int)JSValueToNumber(pCtx,pProperty,0);
                
                return (nID == (JSCClass<T>::getInstance()->getTypeID()));
            }
        }
        
        template <typename T>
        static bool IsTypeOf( JSValueRef p_pVal )
        {
            if( 0 == p_pVal )
                return false;
            
            JSContextRef pCtx = __TlsData::GetInstance()->GetCurContext();
            if( !JSValueIsObject(pCtx, p_pVal) )
            {
                return false;
            }
            
            return IsTypeOf<T>((JSObjectRef)p_pVal);
        }
    };

    
}   // namespace __JSCProxy

#endif
