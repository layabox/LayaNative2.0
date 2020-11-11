//
//  JSCProxyTrnasfer.h
//  jsc_test
//
//  Created by 蒋 宇彤 on 13-11-25.
//  Copyright (c) 2013年 蒋 宇彤. All rights reserved.
//

#ifndef jsc_test_JSCProxyTrnasfer_h
#define jsc_test_JSCProxyTrnasfer_h

#include <v8.h>
#include <string>
#include <vector>
#include "JSArrayBuffer.h"
//#include "JsBinder.h"

using namespace v8;
namespace laya{
	//为了能快速返回js的字符串，现在转字符串的函数使用了通用buffer，
	//为了能同时返回多个buffer指针，就用了一个vector，为了避免vector一直增加，内存泄露
	//就用了这个函数。具体看代码。
	void resetJsStrBuf();

    template<class T>
    Local<Object> createJsObjAttachCObj(T* cobj, bool weak);

    template <typename T> class __TransferToCpp{public:
		static T ToCpp( Local<Value> p_vl ){
            // 这是很危险的
			if( !p_vl.IsEmpty() && p_vl->IsObject() ){
				return static_cast<T>((p_vl.As<Object>())->GetAlignedPointerFromInternalField(0));
			}else{
				return (T)0;
			}
        }
		//问题：如果是对象的话，当请求 T或者T* 认为是相同的
		static bool is(Local<Value> p_vl) {
			if (!p_vl->IsObject())
				return false;
			Local<Object> obj = p_vl.As<Object>();
			if (obj->InternalFieldCount() < 2)
				return false;
			void* pdt = obj->GetAlignedPointerFromInternalField(1);
			if( pdt == &T::JSCLSINFO) {
				return true;
			}
			return false;
		}
	};

	//不知道怎么区分对象和对象指针，先硬写一个
	template <typename T> class __TransferToCpp<T*> {
	public:
		static T* ToCpp(Local<Value> p_vl) {
			// 这是很危险的
			if (!p_vl.IsEmpty() && p_vl->IsObject()) {
                Local<Object> obj = p_vl.As<Object>();
				return static_cast<T*>(obj->GetAlignedPointerFromInternalField(0));
			}
			else {
				return (T*)0;
			}
		}
		//问题：如果是对象的话，当请求 T或者T* 认为是相同的
		static bool is(Local<Value> p_vl) {
			if (!p_vl->IsObject())
				return false;
			Local<Object> obj = p_vl.As<Object>();
			void* pdt = obj->GetAlignedPointerFromInternalField(1);
			if (pdt == &T::JSCLSINFO) {
				return true;
			}
			return false;
		}
	};
    
	template <> class __TransferToCpp<int32_t> {
	public:
		static int32_t ToCpp(Local<Value> p_vl) { return p_vl->Int32Value(); }
		static bool is(Local<Value> p_vl) { return p_vl->IsInt32(); }
	};

	template <> class __TransferToCpp<int64_t> {
	public:
		static int64_t ToCpp(Local<Value> p_vl) { return p_vl->IntegerValue(); }
		static bool is(Local<Value> p_vl) { return p_vl->IsNumber(); }
	};

	template <> class __TransferToCpp<uint32_t> {
	public:
		static uint32_t ToCpp(Local<Value> p_vl) { return p_vl->Uint32Value(); }
		static bool is(Local<Value> p_vl) { return p_vl->IsUint32(); }
	};

	template <> class __TransferToCpp<uint64_t> {
	public:
		static uint64_t ToCpp(Local<Value> p_vl) { return (uint64_t)p_vl->IntegerValue(); }
		static bool is(Local<Value> p_vl) { return p_vl->IsNumber(); }
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
    
    template <> class __TransferToCpp<bool>{public:
		static bool ToCpp( Local<Value> p_vl ){return p_vl->BooleanValue();}
		static bool is(Local<Value> p_vl) { return p_vl->IsBoolean(); }
	};
    
    template <> class __TransferToCpp<float>{public:
		static float ToCpp( Local<Value> p_vl ){return (float)p_vl->NumberValue();}
		static bool is(Local<Value> p_vl) { return p_vl->IsNumber(); }
	};
    
    template <> class __TransferToCpp<double>{public:
		static double ToCpp( Local<Value> p_vl ){return p_vl->NumberValue();}
		static bool is(Local<Value> p_vl) { return p_vl->IsNumber(); }
	};
    
 //   template <> class __TransferToCpp<long long>{public:
	//	static long long ToCpp( Local<Value> p_vl ){return p_vl->IntegerValue();}
	//	static bool is(Local<Value> p_vl) { return p_vl->IsNumber(); }
	//};
    
 //   template <> class __TransferToCpp<unsigned long long>{public:
	//	static unsigned long long ToCpp( Local<Value> p_vl ){return (unsigned long long)p_vl->IntegerValue();}
	//	static bool is(Local<Value> p_vl) { return p_vl->IsNumber(); }
	//};

	template <> class __TransferToCpp<Local<Value>>{public:
		static Local<Value> ToCpp(Local<Value> p_vl) { return p_vl; }
		static bool is(Local<Value> p_vl) { return true; }
	};

	//注意：这个函数返回的字符串使用了一个通用buffer，所以注意复制。
	//这个函数必须配合 resetJsStrBuf 函数使用。
	//例如 
	// char* p1 = JsCharToC(args[0]);
	// char* p2 = JsCharToC(args[1]);
	// use(p1,p2);
	// resetJsStrBuf();
	char* JsCharToC(Local<Value> p_vl);
	class transToCharPtr {
	public:
		static char *ToCpp1(Local<Value> p_vl) {
			String::Utf8Value utf8str(p_vl->ToString());
			//return *utf8str;
			//下面有泄露
			char *pRet = 0;
			size_t len;
			if (0 != *utf8str && 0 != (len = strlen(*utf8str))){
				pRet = new char[len + 1];
				memcpy(pRet, *utf8str, len + 1);
			}
			return pRet;
		}
		static std::string ToCppStd(Local<Value> p_vl) {
			String::Utf8Value utf8str(p_vl->ToString());
			return std::string(*utf8str);
		}
	};

	template <> class __TransferToCpp<char *> {public:
		//注意：这个函数返回的字符串使用了一个通用buffer，所以注意复制
		static char* ToCpp(Local<Value> p_vl) { return JsCharToC(p_vl); }
		static bool is(Local<Value> p_vl) { return p_vl->IsString(); }
		static std::string ToCppStd(Local<Value> p_vl) { return transToCharPtr::ToCppStd(p_vl); }
	};
	template <> class __TransferToCpp<const char *> {public:
		//注意：这个函数返回的字符串使用了一个通用buffer，所以注意复制
		static const char* ToCpp(Local<Value> p_vl) { return (const char*)JsCharToC(p_vl); }
		static bool is(Local<Value> p_vl) { return p_vl->IsString(); }
		static std::string ToCppStd(Local<Value> p_vl) { return transToCharPtr::ToCppStd(p_vl); }
	};
	template <> class __TransferToCpp<unsigned char *> {public:
		//注意：这个函数返回的字符串使用了一个通用buffer，所以注意复制
		static unsigned char* ToCpp(Local<Value> p_vl) { return (unsigned char*)JsCharToC(p_vl); }
		static bool is(Local<Value> p_vl) { return p_vl->IsString(); }
		static std::string ToCppStd(Local<Value> p_vl) { return transToCharPtr::ToCppStd(p_vl); }
	};
	template <> class __TransferToCpp<const unsigned char *> {public:
		//注意：这个函数返回的字符串使用了一个通用buffer，所以注意复制
		static const unsigned char* ToCpp(Local<Value> p_vl) { return (const unsigned char*)JsCharToC(p_vl); }
		static bool is(Local<Value> p_vl) { return p_vl->IsString(); }
		static std::string ToCppStd(Local<Value> p_vl) { return transToCharPtr::ToCppStd(p_vl); }
	};

	//template<> class __TransferToCpp<laya::JSArrayBuffer*>{public:
	//	static bool is(Local<Value> p_vl) { return p_vl->IsArrayBuffer() || p_vl->IsArrayBufferView(); }
	//	static laya::JSArrayBuffer* ToCpp(Local<Value> p_vl) { return laya::JSArrayBuffer::fromeJSObj(p_vl); };
	//};

	template <typename T> class __TransferToJs{
	    public:static Handle<Value> ToJs(T* p_vl) { 
            return laya::createJsObjAttachCObj<T>(p_vl,true); 
        }
	};
    
	//为什么不直接把没有参数作为void？
	template <> class __TransferToJs<void>
	{public:static Handle<Value> ToJs( int p_vl )
	{
		if(0==p_vl)
			return Undefined(Isolate::GetCurrent());
		else
			return Null(Isolate::GetCurrent());
	}};

	template <> class __TransferToJs<v8::Local<v8::Value>>{
	public:static Handle<Value> ToJs(v8::Local<v8::Value> p_vl) { return p_vl; }
	};

	template <> class __TransferToJs<v8::Local<v8::Object> >
	{
	public:static Handle<Value> ToJs(v8::Local<v8::Object> p_vl) { return p_vl; }
	};

	template <> class __TransferToJs<bool>
    {public:static Handle<Value> ToJs( bool p_vl ){return Boolean::New(Isolate::GetCurrent(),p_vl);}};
    
    template <> class __TransferToJs<int>
    {public:static Handle<Value> ToJs( int p_vl ){return Int32::New(Isolate::GetCurrent(), p_vl);}};
    
    template <> class __TransferToJs<unsigned int>
    {public:static Handle<Value> ToJs( unsigned int p_vl ){return Uint32::New(Isolate::GetCurrent(), p_vl);}};
    
	template <> class __TransferToJs<long unsigned int>
    {
    public:static Handle<Value> ToJs(long unsigned int p_vl) { return Number::New(Isolate::GetCurrent(), p_vl); }
    };
	
    template <> class __TransferToJs<float>
    {public:static Handle<Value> ToJs( float p_vl ){return Number::New(Isolate::GetCurrent(), p_vl);}};
    
    template <> class __TransferToJs<double>
    {public:static Handle<Value> ToJs( double p_vl ){return Number::New(Isolate::GetCurrent(), p_vl);}};
    
    template <> class __TransferToJs<long long>
    {
	public:
		static Handle<Value> ToJs( long long p_vl )
		{
			return Number::New(Isolate::GetCurrent(), (double)p_vl);
		}

		static Handle<Value> ToJsDate( long long p_vl )
		{
			return Date::New(Isolate::GetCurrent(), (double)p_vl );
		}
	};

	template <> class __TransferToJs<long>
    {
	public:
		static Handle<Value> ToJs( long p_vl )
		{
			return Number::New(Isolate::GetCurrent(), (double)p_vl);
		}

		static Handle<Value> ToJsDate( long p_vl )
		{
			return Date::New(Isolate::GetCurrent(), (double)p_vl );
		}
	};
    
    template <> class __TransferToJs<unsigned long long>
    {
	public:
		static Handle<Value> ToJs( unsigned long long p_vl )
		{
			return Number::New(Isolate::GetCurrent(), (double)p_vl);
		}

		static Handle<Value> ToJsDate( unsigned long long p_vl )
		{
			return Date::New(Isolate::GetCurrent(), (double)p_vl );
		}
	};
    
    template <> class __TransferToJs<char *>
	{public:static Handle<Value> ToJs( char *p_vl ){if(p_vl==0) p_vl=(char*)""; return String::NewFromUtf8(Isolate::GetCurrent(),p_vl);}};
    
	template <> class __TransferToJs<const char *>
	{public:static Handle<Value> ToJs( const char *p_vl ){if(p_vl==0) p_vl=(const char *)""; return String::NewFromUtf8(Isolate::GetCurrent(),p_vl);}};

	template <> class __TransferToJs<unsigned char *>
	{public:static Handle<Value> ToJs( const unsigned char*p_vl ){if(p_vl==0) p_vl=(const unsigned char*)""; return String::NewFromUtf8(Isolate::GetCurrent(),(const char *)p_vl);}};

	template <> class __TransferToJs<const unsigned char *>
	{public:static Handle<Value> ToJs( const unsigned char *p_vl ){if(p_vl==0) p_vl=(const unsigned char *)""; return String::NewFromUtf8(Isolate::GetCurrent(),(const char *)p_vl);}};

    template <> class __TransferToJs<std::string>
	{public:static Handle<Value> ToJs( std::string p_vl ){return String::NewFromUtf8(Isolate::GetCurrent(), p_vl.c_str());}};

	template <> class __TransferToJs<std::string&> {
	public:static Handle<Value> ToJs(std::string& p_vl) { return String::NewFromUtf8(Isolate::GetCurrent(), p_vl.c_str()); }
	};
	//template <> class __TransferToJs<laya::JSArrayBuffer*>
	//{public:static Handle<Value> ToJs( laya::JSArrayBuffer* p_vl ){return p_vl->toLocal();}};
	template <typename T> class __JsArray{
	public:
		static Handle<Value> ToJsArray(const std::vector<T*>& p_v1)
		{
			int size=p_v1.size();
			if(0==size)
			{
				Handle<Array> __array = Array::New(Isolate::GetCurrent(), 0);
				return __array;
			}
			else
			{
				Handle<Array> __array = Array::New(Isolate::GetCurrent(), size);
				for(int i=0;i<size;i++)
				{
					//__array->Set(i, __JSCProxy_class<T>::GetInstance()->TransferObjPtrToJS(p_v1.at(i)));
					__array->Set(i, __TransferToJs<T>::ToJs(p_v1.at(i)));
				}
				return __array;
			}
		}
		static Handle<Value> ToJsArray(const std::vector<T>& p_v1)
		{
			int size = p_v1.size();
			if (0 == size)
			{
				Handle<Array> __array = Array::New(Isolate::GetCurrent(), 0);
				return __array;
			}
			else
			{
				Handle<Array> __array = Array::New(Isolate::GetCurrent(), size);
				for (int i = 0; i<size; i++)
				{
					//__array->Set(i, __JSCProxy_class<T>::GetInstance()->TransferObjPtrToJS(p_v1.at(i)));
					__array->Set(i, __TransferToJs<T>::ToJs(p_v1.at(i)));
				}
				return __array;
			}
		}
	};
	class __JsByteArray
	{
	public:
		static Handle<Value> ToJsByteArray( const unsigned char *p_vl, int p_iSize )
		{
			if( 0 == p_vl || p_iSize <= 0 )
			{
				return Null(Isolate::GetCurrent());
			}
			else
			{
				Handle<Array> __array = Array::New(Isolate::GetCurrent(), p_iSize);
				for(int i=0;i<p_iSize;++i)
				{
					__array->Set( i, Int32::New(Isolate::GetCurrent(), p_vl[i]) );
				}
				return __array;
			}
		}
	};

	template <typename T> class __TransferToJs<std::vector<T*> >
	{
		public:static Handle<Value> ToJs( const std::vector<T*>& p_vl )
		{
			return __JsArray<T>::ToJsArray(p_vl);
		}
	};

	template <typename T> class __TransferToJs<std::vector<T> >
	{
	public:static Handle<Value> ToJs(const std::vector<T>& p_vl)
	{
		return __JsArray<T>::ToJsArray(p_vl);
	}
	};

}

#endif
