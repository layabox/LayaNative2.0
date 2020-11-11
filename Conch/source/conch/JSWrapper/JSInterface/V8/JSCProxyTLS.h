//
//  JSCProxyTLS.h
//  jsc_test
//
//  Created by 蒋 宇彤 on 13-11-25.
//  Copyright (c) 2013年 蒋 宇彤. All rights reserved.
//

#ifndef jsc_test_JSCProxyTLS_h
#define jsc_test_JSCProxyTLS_h

#include <v8.h>
#include <string>
#include <util/JCCommonMethod.h>
#include <util/Log.h>
#include <sstream>

namespace laya{
    extern void JSAlert(const char* p_sBuffer);
    class __JsThrow    {
    public:
        static void Throw( const char *p_pszInfo ) {
			v8::Isolate::GetCurrent()->ThrowException(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), (0==p_pszInfo)?"unknown error":p_pszInfo));
        }
    };
    extern bool gbAlertException;
    class __JSRun  {
    public:
		// Extracts a C string from a V8 Utf8Value.
		static const char* ToCString(const v8::String::Utf8Value& value) {
		  return *value ? *value : "<string conversion failed>";
		}

        static void ReportException(v8::Isolate* isolate, v8::TryCatch* try_catch);

        static bool Run( const char *p_pszScript ) {
			v8::Isolate* isolate = v8::Isolate::GetCurrent();
			v8::HandleScope handle_scope(isolate);
			v8::TryCatch try_catch(isolate);

			v8::Handle<v8::String> source = v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), p_pszScript);

			v8::Handle<v8::Script> script = v8::Script::Compile(source);
			if( script.IsEmpty() ){
				//打印编译错误信息
				printf("---Compile script error---\n");
				ReportException(isolate, &try_catch);
				return false;
			}

			v8::Handle<v8::Value> res = script->Run();

			if( !res.IsEmpty() && !res->IsUndefined() )
			{
				v8::String::Utf8Value ascii(res);
				printf("run result: [%s]\n", *ascii);
			}

			if( try_catch.HasCaught()){
				printf("---run script error---\n");
				ReportException(isolate, &try_catch);
			}
            
            return true;
        }
    };
}

#endif
