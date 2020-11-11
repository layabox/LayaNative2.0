//
//  JSCProxyType.h
//  jsc_test
//
//  Created by 蒋 宇彤 on 13-12-1.
//  Copyright (c) 2013年 蒋 宇彤. All rights reserved.
//

#ifndef jsc_test_JSCProxyType_h
#define jsc_test_JSCProxyType_h

#include <v8.h>
#include "JSCProxyTLS.h"

namespace laya{
    typedef enum    {
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

    
    class __CheckClassType
    {
    public:
        template <typename T>
        static bool IsTypeOf( v8::Local<v8::Value> p_Obj )
        {
			if( p_Obj.IsEmpty() || !p_Obj->IsObject() )
				return false;
			else {
				v8::Local<v8::Object> obj = p_Obj.As<v8::Object>();
				void* pdt = v8::Local<v8::External>::Cast(obj->GetInternalField(1))->Value();
				return pdt == &T::JSCLSINFO;
			}
        }
    };
    
}   // namespace __JSCProxy

#endif
