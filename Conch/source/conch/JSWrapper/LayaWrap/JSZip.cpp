/**
@file			JSZip.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_18
*/

#include "JSZip.h"
#include "util/Log.h"
#include <zip.h>

namespace laya 
{
	ADDJSCLSINFO(JSZip, JSObjNode);
	JSZip::JSZip() 
    {
		m_pZip = NULL;
        m_pZipSource = NULL;
	}
	JSZip::~JSZip() 
    {
        close();
	}
	bool JSZip::openZip(const char* p_pszFile) 
    {
		int error =0;
		zip* pZip = zip_open(p_pszFile, 0, &error);
		if (!pZip) 
        {
            LOGE("JSZip can't open zip: %d", error);
			return false;
		}
		m_pZip = pZip;
		return true;
	}
	bool JSZip::setSrc(const char* p_pSrc) 
    {
        close();
		return openZip(p_pSrc);
	}
	JsValue JSZip::readFileExp(int idx)
    {
		return readFile(idx);
	}
	JsValue JSZip::readFile(int idx) 
    {
		if (!m_pZip)
			return JSP_TO_JS_NULL;
		zip* pZip = (zip*)m_pZip;
		struct zip_stat sb;
		zip_stat_init(&sb);
		if (zip_stat_index(pZip, idx, 0, &sb) < 0)
        {
		}
        else 
        {
			zip_file* pzf = zip_fopen_index(pZip, idx, 0);
			if (pzf == NULL) 
            {
				int a = 0;
			}
			else 
            {
				//laya::JSArrayBuffer* pAB = laya::JSArrayBuffer::create(sb.size);
				char* pBuff = new char[(unsigned int)sb.size];
				zip_fread(pzf, pBuff, sb.size);
				zip_fclose(pzf);
				JsValue ab = createJSAB(pBuff,(int)sb.size);
				delete [] pBuff;
				//return pAB;
				return ab;
			}
		}
		return JSP_TO_JS_NULL;
	}
    JsValue JSZip::readFileByName(const char* pName)
    {
        if (!pName)
            return JSP_TO_JS_NULL;

        if (!m_pZip)
            return JSP_TO_JS_NULL;

        zip* pZip = (zip*)m_pZip;
        zip_int64_t idx = zip_name_locate(pZip, pName, 0);

        if (idx == -1)
            return JSP_TO_JS_NULL;

        return readFile(idx);
    }
    JsValue JSZip::readFileAsText(int idx)
    {
        if (!m_pZip)
            return JSP_TO_JS_NULL;
        zip* pZip = (zip*)m_pZip;
        struct zip_stat sb;
        zip_stat_init(&sb);
        if (zip_stat_index(pZip, idx, 0, &sb) < 0)
        {
        }
        else
        {
            zip_file* pzf = zip_fopen_index(pZip, idx, 0);
            if (pzf == NULL)
            {
                int a = 0;
            }
            else
            {
                std::unique_ptr<char[]> pBuff(new char[(unsigned int)sb.size + 1]);
                zip_fread(pzf, pBuff.get(), sb.size);
                zip_fclose(pzf);
                pBuff[(unsigned int)sb.size] = 0;
#ifdef _DEBUG
                if (IsTextUTF8((char*)pBuff.get(), (unsigned int)sb.size) == false)
                {
                    const char* pName = zip_get_name(pZip, idx, 0);
                    LOGE("error JSZip readFileAsText %s is not utf8 format", pName);
                }
#endif
                return (JSP_TO_JS_STR(pBuff.get()));
            }
        }
        return JSP_TO_JS_NULL;
    }
    JsValue JSZip::readFileAsTextByName(const char* pName)
    {
        if (!pName)
            return JSP_TO_JS_NULL;

        if (!m_pZip)
            return JSP_TO_JS_NULL;

        zip* pZip = (zip*)m_pZip;
        zip_int64_t idx = zip_name_locate(pZip, pName, 0);

        if (idx == -1)
            return JSP_TO_JS_NULL;

        return readFileAsText(idx);
    }
	void JSZip::enumFiles( JSValueAsParam pObj)
    {
		if (!m_pZip)
			return;
		zip* pZip = (zip*)m_pZip;
		int entNum = zip_get_num_files(pZip);
		for (int i = 0; i < entNum; i++)
        {
			struct zip_stat sb;
			zip_stat_init(&sb);
			if (zip_stat_index(pZip, i, 0, &sb) < 0)
            {
			}
			else 
            {
				bool bDir = sb.name[strlen(sb.name) - 1] == '/';
#ifdef JS_V8
				if (!pObj->IsFunction())return;
				v8::Local<v8::Function> jsfun = v8::Local<v8::Function>::Cast(pObj);
				callJsFunc(jsfun, i, sb.name, bDir, sb.size);
#elif JS_JSVM
				JSVM_Status status;
                bool result;
                JSVM_API_CALL(status, ENV, OH_JSVM_IsFunction(ENV, pObj, &result));
                if (!result) return;
				callJsFunc(pObj, i, sb.name, bDir, sb.size);
#elif JS_JSC
                JSContextRef ctx = laya::__TlsData::GetInstance()->GetCurContext();
                if (!JSValueIsObject(ctx, pObj))return;
                if (!JSObjectIsFunction(ctx, JSValueToObject(ctx, pObj, nullptr)))return;
                callJsFunc(pObj, i, sb.name, bDir, sb.size);
#endif
			}
		}
		//zip_close(pZip);
	}
	void JSZip::close()
    {
        if (m_pZip)
        {
            zip_close((zip*)m_pZip);
        }
        m_pZip = NULL;
        m_pZipSource = NULL;
	}
    bool JSZip::setBuffer(JSValueAsParam pArrayBuffer)
    {
        char* pArrayBufferPtr = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = extractJSAB(pArrayBuffer, pArrayBufferPtr, nABLen);
        if (!bIsArrayBuffer)
        {
            LOGE("JSZip::setBuffer param is not an ArrayBuffer!");
            return false;
        }
        if (nABLen <= 0)
        {
            return false;
        }

        close();
        char* pBuff = new char[nABLen];
        memcpy(pBuff, pArrayBufferPtr, nABLen);
        zip_error_t error;
        zip_error_init(&error);
        m_pZipSource = zip_source_buffer_create(pBuff, nABLen, 1, &error);

        if (m_pZipSource == NULL)
        {
            LOGE("JSZip can't create source: %s", zip_error_strerror(&error));
            zip_error_fini(&error);
            delete[] pBuff;
            return false;
        }

        m_pZip = zip_open_from_source((zip_source*)m_pZipSource, 0, &error);
        if (!m_pZip)
        {
            LOGE("JSZip can't open zip from source: %s", zip_error_strerror(&error));
            zip_source_free((zip_source*)m_pZipSource);
            zip_error_fini(&error);
            return false;
        }
        zip_error_fini(&error);
        return true;
    }
	void JSZip::exportJS() 
    {
        JSP_CLASS("ZipFile", JSZip);
		JSP_ADD_METHOD("setSrc", JSZip::setSrc);
		JSP_ADD_METHOD("forEach", JSZip::enumFiles);
		JSP_ADD_METHOD("readFile1", JSZip::readFileExp);//这个现在有问题,在导出函数中调用这个无法多次调用。例如enumFiles中调用,只有一次有效。
		JSP_ADD_METHOD("readFile", JSZip::readFile);
        JSP_ADD_METHOD("readAsArrayBufferByID", JSZip::readFile);
        JSP_ADD_METHOD("readAsArrayBufferByName", JSZip::readFileByName);
        JSP_ADD_METHOD("readAsTextByID", JSZip::readFileAsText);
        JSP_ADD_METHOD("readAsTextByName", JSZip::readFileAsTextByName);
		JSP_ADD_METHOD("close", JSZip::close);
        JSP_ADD_METHOD("setBuffer", JSZip::setBuffer);
		JSP_INSTALL_CLASS("ZipFile", JSZip);
	}
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
