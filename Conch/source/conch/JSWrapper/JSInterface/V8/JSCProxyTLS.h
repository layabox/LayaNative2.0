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
#include <string>
#include <cassert>

namespace laya{
    extern void JSAlert(const char* p_sBuffer);
    class __JsThrow    {
    public:
        static void Throw( const char *p_pszInfo ) {
			v8::Isolate::GetCurrent()->ThrowException(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), (0==p_pszInfo)?"unknown error":p_pszInfo).ToLocalChecked());
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

        static bool Run( const char *p_pszScript )
		{
			v8::Isolate* isolate = v8::Isolate::GetCurrent();
			v8::HandleScope handle_scope(isolate);
			v8::TryCatch try_catch(isolate);
			std::string strScript(p_pszScript);

			//v8::Local<v8::String> file_name = v8::String::NewFromUtf8(isolate, "unnamed").ToLocalChecked();
			//v8::ScriptOrigin origin(file_name);

			v8::MaybeLocal<v8::String> source = v8::String::NewFromUtf8(isolate, strScript.c_str());
			if (source.IsEmpty())
			{
				LOGI("source.IsEmpty()");
				return false;
			}
			v8::Local<v8::Script> script;
			if (!v8::Script::Compile(isolate->GetCurrentContext(), source.ToLocalChecked()/*, &origin*/).ToLocal(&script))
			{
				//打印编译错误信息
				LOGI("---Compile script error---");
				ReportException(isolate, &try_catch);
				return false;
			}
			v8::Local<v8::Value> result;
			if (!script->Run(isolate->GetCurrentContext()).ToLocal(&result))
			{
				assert(try_catch.HasCaught());
				printf("---run script error---\n");
				// Print errors that happened during execution.
				ReportException(isolate, &try_catch);
				return false;
			}
			else
			{
				assert(!try_catch.HasCaught());
				return true;
			}
        }
    };
}

#endif
