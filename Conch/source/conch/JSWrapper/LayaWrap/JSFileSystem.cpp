/**
@file			JSFileSystem.cpp
@brief			
@author			James
@version		1.0
@date			2017_11_28
*/

#include "JSFileSystem.h"
#include "../JSInterface/JSInterface.h"
#include <util/Log.h>
#include <fileSystem/JCFileSystem.h>
#include <ctime>

namespace laya
{
	bool JSFileSystem::exists(const char* p_pszPath )
    {
        bool bret = false;
        try {
            bret = fs::exists(p_pszPath);
        }
        catch (...) {
            return false;
        }
        return bret;
	}
	bool JSFileSystem::mkdir( const char* p_pszPath )
    {
		bool bret = false;
		try 
        {
			bret = fs::create_directories(p_pszPath);
		}
		catch (...) 
        {
			return false;
		}
		return bret;
	}
	bool ChkPermission( const char* p_pszFile, const char* p_pszDesc ) 
    {
        return true;
	}
	bool JSFileSystem::rm(const char* p_pszFile) 
    {
		if(!ChkPermission(p_pszFile,"rm is forbidden!"))
        {
			return false;
		}
		try 
        {
			return fs::remove(p_pszFile);
		}
		catch (...)
        {
			return false;
		}
	}
	bool JSFileSystem::rmDir(const char* p_pszPath, JSValueAsParam onprogress, JSValueAsParam oncomplete, JSValueAsParam onerror) 
    {
		return true;
	}
	bool JSFileSystem::rmDirSync(const char* p_pszPath) 
    {
		return true;
	}
	JsValue JSFileSystem::readdirSync(const char* pPath) 
    {
		std::vector < std::string> paths;
		std::string path = pPath;
		if (!fs::exists(path))
			return JSP_TO_JS_NULL;
		fs::directory_iterator item_begin(path);
		fs::directory_iterator item_end;
		for (; item_begin != item_end; item_begin++) {
			auto pp = (*item_begin).path().filename();
			paths.push_back(pp.generic_string());
		}
		return __TransferToJs<std::vector<std::string> >::ToJs(paths);
	}
	JsValue JSFileSystem::lstatSync(const char* pPath) 
    {
		std::vector < std::string> paths;
		std::string path = pPath;
		if (!fs::exists(path))
			return JSP_TO_JS_NULL;
		try {
			auto st = fs::status(path);
			std::time_t wtime;
#ifdef WIN32
            wtime = std::chrono::system_clock::to_time_t(fs::last_write_time(path));
#else
            wtime = fs::last_write_time(path);
#endif
			bool isDir = fs::is_directory(st);
			bool isFile = fs::is_regular_file(st);
			int sz = 0;
			if (!isDir)sz = (int)fs::file_size(path);
#ifdef JS_V8
			//st.type;
			v8::Isolate* pIso = v8::Isolate::GetCurrent();
			v8::Local<v8::Context> context = pIso->GetCurrentContext();
			//v8::HandleScope scope(pIso); 不用了，还得想办法escape
			v8::Local<v8::Object> retobj = v8::Object::New(pIso);
			retobj->Set(context, Js_Str(pIso, "isDirectory"), v8::Boolean::New(pIso, isDir));
			retobj->Set(context, Js_Str(pIso, "isFile"), v8::Boolean::New(pIso, isFile));
			retobj->Set(context, Js_Str(pIso, "size"), v8::Number::New(pIso, sz));
			retobj->Set(context, Js_Str(pIso, "mtime"), v8::Date::New(context, (double)(wtime*1000)).ToLocalChecked());
			return retobj;
#elif JS_JSC
            JSContextRef ctx = laya::__TlsData::GetInstance()->GetCurContext();
			JSObjectRef retobj = JSObjectMake(ctx, nullptr, nullptr);
			JSObjectSetProperty(ctx, retobj, JSStringCreateWithUTF8CString("isDirectory"), JSValueMakeBoolean(ctx, isDir), kJSPropertyAttributeNone, nullptr);
			JSObjectSetProperty(ctx, retobj, JSStringCreateWithUTF8CString("isFile"), JSValueMakeBoolean(ctx,isFile), kJSPropertyAttributeNone, nullptr);
			JSObjectSetProperty(ctx, retobj, JSStringCreateWithUTF8CString("size"), JSValueMakeNumber(ctx,sz), kJSPropertyAttributeNone, nullptr);
			JSObjectSetProperty(ctx, retobj, JSStringCreateWithUTF8CString("mtime"), laya::__TransferToJs<uint64_t>::ToJsDate(wtime*1000), kJSPropertyAttributeNone, nullptr);
			return retobj;
#endif
		}
		catch (...) 
        {
			JSP_THROW("lstatSync error!");
		}
		return JSP_TO_JS_NULL;
	}
    bool JSFileSystem::JSWriteFileSync(const char* p_sUrl, JSValueAsParam args)
    {
        if (!p_sUrl) return false;
        char* pABPtr = NULL;
        int nABLen = 0;
        bool bisab = extractJSAB(args, pABPtr, nABLen);
        bool bret = false;
        if (bisab) 
        {
            if (pABPtr && nABLen > 0)
            {
                bret = writeFileSync1(p_sUrl, pABPtr, nABLen, 0);
            }
        }
        else 
        {
            if (__TransferToCpp<char *>::is(args))
            {
                char* pData = JS_TO_CPP(char*, args);
                if ( pData )
                {
                    int len = strlen(pData);
                    JCBuffer buf((char*)pData, len, false, false);
                    bret = writeFileSync( p_sUrl, buf, JCBuffer::utf8);
                }
            }
        }
        return bret;
    }
	JsValue JSFileSystem::readBinFileSync(const char* p_pszFile) 
    {
		if(!ChkPermission(p_pszFile,"readBinFileSync is forbidden!"))
        {
			return JSP_TO_JS_NULL;
		}
        JCBuffer buf;
		if (readFileSync(p_pszFile, buf, JCBuffer::raw))
        {
			return laya::createJSAB(buf.m_pPtr, buf.m_nLen);
		}
		else 
        {
			return JSP_TO_JS_NULL;
		}
    }
	void JSFileSystem::exportJS()
    {
	}
};
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
