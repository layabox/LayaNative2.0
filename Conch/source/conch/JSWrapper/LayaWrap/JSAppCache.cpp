/**
@file			JSAppCache.cpp
@brief			
@author			James
@version		1.0
@date			2017_11_28
*/

#include "JSAppCache.h"
#include "downloadCache/JCServerFileCache.h"
#include "fileSystem/JCFileSystem.h"
#include "resource/JCFileResManager.h"
#include "util/Log.h"
#include "JSFile.h"
#include "../../JCScriptRuntime.h"
#include "util/JCSimpleCRC.h"
#include "downloadCache/JCFileSource.h"

extern std::string gRedistPath;
#ifdef _DEBUG
    #define VERIFY(a,msg) {if(!(a)){LOGE(msg); throw -1;};}
#else
    #define VERIFY(a,msg)
#endif
namespace laya
{
    char* GlobalTransUrl(void* pData, const char* pUrl) 
    {
        JsAppCache* pCache = (JsAppCache*)pData;
        if (pCache) 
        {
            return pCache->getTransedUrl(pUrl);
        }
        return nullptr;
    }
	ADDJSCLSINFO(JsAppCache, JSObjNode);
	JsAppCache::JsAppCache()
    {
        //大概估算内部变量
        AdjustAmountOfExternalAllocatedMemory( 1024 );
		m_bEnableCache=true;
        m_pSvFileCache = new JCServerFileCache();
        m_pSvFileCache->m_pFuncTransUrl = GlobalTransUrl;
        m_pSvFileCache->m_pFuncTransUrlData = this;
        m_pSvFileCache->setCachePath((gRedistPath + "/appCache").c_str());
        if (JCScriptRuntime::s_JSRT->m_pFileResMgr)
        {
            JCFileResManager* pFileResManager = JCScriptRuntime::s_JSRT->m_pFileResMgr;
            //TODO 这段代码有点恶心，日后再整理
            //为了刷新的时候，再次设置fileCache，所以要把原来的删除掉
            if (pFileResManager->m_pFileCache)
            {
                delete pFileResManager->m_pFileCache;
                pFileResManager->m_pFileCache = NULL;
            }
            pFileResManager->setFileCache(m_pSvFileCache);
        }
	}
	JsAppCache::JsAppCache(const char* p_pszURL)
    {
        //大概估算内部变量
        AdjustAmountOfExternalAllocatedMemory( 1024 );
		m_bEnableCache=true;
		m_strURL = p_pszURL;
        m_pSvFileCache = new JCServerFileCache();
        m_pSvFileCache->m_pFuncTransUrl = GlobalTransUrl;
        m_pSvFileCache->m_pFuncTransUrlData = this;
        m_pSvFileCache->setCachePath((gRedistPath + "/appCache").c_str());
        if (JCScriptRuntime::s_JSRT->m_pFileResMgr)
        {
            JCFileResManager* pFileResManager = JCScriptRuntime::s_JSRT->m_pFileResMgr;
            //TODO 这段代码有点恶心，日后再整理
            //为了刷新的时候，再次设置fileCache，所以要把原来的删除掉
            if (pFileResManager->m_pFileCache)
            {
                delete pFileResManager->m_pFileCache;
                pFileResManager->m_pFileCache = NULL;
            }
            pFileResManager->setFileCache(m_pSvFileCache);
        }
        m_pSvFileCache->switchToApp(p_pszURL);
		AdjustAmountOfExternalAllocatedMemory( 12+13+128);
		JCMemorySurvey::GetInstance()->newClass( "AppCache",12+13+128,this );
	}
	JsAppCache::~JsAppCache()
    {
		JCMemorySurvey::GetInstance()->releaseClass( "AppCache",this );
        //TODO 这段代码有点恶心，日后再整理
        //删除这个svFileCache不能在这删除，因为渲染线程还在运行，所以放到渲染线程删除FileResManager的时候删除了
        //但是这个JCAppCache有多个，就会有内存泄露
        /*
        if (m_pSvFileCache != NULL)
        {
            delete m_pSvFileCache;
        }
        */
        m_pSvFileCache = NULL;
	}
	unsigned int JsAppCache::getCacheSize()
    {
		return 0;
	}
	std::string JsAppCache::getCachePath()
    {
		return m_pSvFileCache->getAppPath();
	}
	bool JsAppCache::getEnableCache()
    {
		return m_bEnableCache;
	}
	void JsAppCache::setEnableCache(bool b )
    {
		m_bEnableCache = b;
	}
	void onCheckOKRunInJs(JsAppCache* pobj)
    {
		pobj->m_funcCheckOK.Call();
	}
	void onCheckErrorRunInJs(JsAppCache* pobj, const char* pError)
    {
		pobj->m_funcCheckError.Call();
	}
	void onCheckOK(JsAppCache* pobj)
    {
		std::function<void()> pFunction = std::bind(onCheckOKRunInJs,pobj);
		//JSConch::GetInstance()->postFuncToJSThread(pFunction);
	}
	void onCheckError(JsAppCache* pobj)
    {
		std::function<void()> pFunction = std::bind(onCheckErrorRunInJs,pobj,"");
		//JSConch::GetInstance()->postFuncToJSThread(pFunction);
	}
	void JsAppCache::update( const char* p_pszDccURL, unsigned int p_ulCheckSum, JSValueAsParam p_pFuncCheckOK, JSValueAsParam p_pFuncCheckError )
    {
		/*
		JCDownloadManager* pDown =  JCDownloadManager::GetInstance();
		if( pDown )
        {
			m_funcCheckOK = *p_pFuncCheckOK;
			m_funcCheckError = *p_pFuncCheckError;
			if( !m_bEnableCache )
            {
				onCheckOK(this);
			}
            else
            {
				pDown->m_onWebCheckOK = std::bind( onCheckOK, this );
				pDown->m_onWebCheckError = std::bind( onCheckError, this );
				pDown->checkWeb(p_pszDccURL, m_strURL.c_str(), p_ulCheckSum );
			}
		}
		*/
	}
    bool JsAppCache::updateFileForJs(int p_nFileID, unsigned int p_nCheckSum,JSValueAsParam p_pBuffer, bool p_bExtVersion)
    {
        char* pABPtr = NULL;
        int nABLen = 0;
        bool isab = extractJSAB(p_pBuffer, pABPtr, nABLen);
        if (pABPtr && nABLen > 0)
        {
            return updateFile(p_nFileID, p_nCheckSum, pABPtr, nABLen, p_bExtVersion);
        }
        else
        {
            return false;
        }
    }
	bool JsAppCache::updateFile(int p_nFileID, unsigned int p_ulChkSum,const char* p_pBuffer, int len, bool p_bExtVersion) 
    {
		if (p_pBuffer == NULL || len <= 0)
			return false;
		if (p_ulChkSum == 0) 
        {
            p_ulChkSum = JCCachedFileSys::getChkSum((char*)p_pBuffer, len);
		}
        /*
		unsigned int localid=0;
		if (strstr(p_pszUrl, "http://")) 
        {
			localid = m_pSvFileCache->hashURLFull(p_pszUrl);
		}
		else 
        {
			localid = m_pSvFileCache->hashURLR(p_pszUrl);
		}
        */
        if (p_bExtVersion) 
        {
            m_pSvFileCache->updateAFile(p_nFileID, (char*)p_pBuffer, len, p_ulChkSum,
                true, 0, false);
            return true;
        }
        else 
        {
            unsigned int chk = 0;
            if (m_pSvFileCache->getFileInfo(p_nFileID, chk)) 
            {
                if (p_ulChkSum != chk) 
                {
                    LOGE("updateFileErr:S:%x R:%x", chk, p_ulChkSum);
                    return false;
                }
                m_pSvFileCache->updateAFile(p_nFileID, (char*)p_pBuffer, len, p_ulChkSum,false, 0, false);
                return true;
            }
            LOGE("updateFile error, not in table:%x", p_nFileID);
        }
		return false;
	}
	std::string	JsAppCache::url2Local(const char* p_pszURL )
    {
		return "";
	}
	void JsAppCache::setResourceID(const char* p_pszResource, const char* p_pszVal)
    {
		return m_pSvFileCache->setResourceID(p_pszResource, p_pszVal);
	}
	std::string	JsAppCache::getResourceID(const char* p_pszResource )
    {
		return m_pSvFileCache->getResourceID(p_pszResource);
	}
	void JsAppCache::saveFileTable(const char* p_pszFileContent)
    {
        m_pSvFileCache->saveFileTable(p_pszFileContent);
	}
    void JsAppCache::setFileTable(const char* p_pszFileContent) 
    {
        m_pSvFileCache->setFileTables(p_pszFileContent);
    }
    void JsAppCache::setUrlTransTable(const char* p_pszFileContent,int split) 
    {
        m_pSvFileCache->setUrlTransTable(p_pszFileContent,(char)split);
    }
    void JsAppCache::setTransUrlToCachedUrl(JSValueAsParam pObj) 
    {
        m_funcTransUrl.set(transUrlFun, this, pObj);
    }
    char* JsAppCache::getTransedUrl(const char* pUrl) 
    {
        if (!m_funcTransUrl.Empty()) 
        {
			char* ret = NULL;
            m_funcTransUrl.CallWithReturn(pUrl, ret);
            return ret;
        }
        return nullptr;
    }
	std::string JsAppCache::loadCachedURL( const char* p_pszUrl )
    {
        JCFileResManager* pfsMgr = JCScriptRuntime::s_JSRT->m_pFileResMgr;
		laya::JCFileRes* res = pfsMgr->getRes(p_pszUrl);
		JCBuffer buff;
		std::string strOut="";
		if( res->loadFromCache(buff,false) && buff.m_pPtr)
        {
			if (buff.m_nLen >= 3 && ((*(int*)(buff.m_pPtr)) & 0x00ffffff) == 0xbfbbef)
            {
				strOut.append(buff.m_pPtr+3, buff.m_nLen-3);
			}
            else
            {
                strOut.append(buff.m_pPtr, buff.m_nLen);
            }
		}
        else
        {
			LOGE("JsAppCache::loadCachedURL Error, no cache data.");
		}
		//LOGE("loadCachedURL:%s,%s", p_pszUrl, strOut.c_str());
		return strOut;
	}
	bool JsAppCache::isFileTableValid()
    {
		std::string file =  m_pSvFileCache->getAppPath()+"/"+"filetable.txt";
		JCBuffer buf;
		if(!readFileSync(file.c_str(),buf))
			return false;
		if(buf.m_nLen<=0)
			return false;
		return true;
	}
    extern unsigned char* AllocSharedBuffer(int sz, void* pUserData);
	bool JsAppCache::isUrlNeedDownload(const char* p_pszFile) {

		JCCachedFileSys::fileShell fs;
		time_t tm;
        unsigned int chksumInFS = 0;
		unsigned int id = m_pSvFileCache->getFileID(p_pszFile);
		std::string pathstr;
		std::string file = m_pSvFileCache->m_FileSys.fileToPath(id, pathstr, false);
        
        // 读取本地缓存中保存的校验值
        if (!m_pSvFileCache->loadShell(file.c_str(), fs, tm)) {
            JCFileSource* pAssets = m_pSvFileCache->getAssets();
            std::string obbPath = m_pSvFileCache->m_FileSys.fileToStr(id, true);
            if (pAssets && pAssets->isFileExist(obbPath.c_str())) {
                JCSharedBuffer p_BufRet;
                int sz;
                if (pAssets->loadFileContent(obbPath.c_str(), AllocSharedBuffer, (void*)&p_BufRet, sz)) {
                    //生成校验码
                    JCSimpleCRC crc;
                    crc.process_bytes(p_BufRet.m_pBuffer.get(), sz);
                    chksumInFS = crc.checksum();
                }
                else {
                    return true;
                }
            }
            else {
                // 读取缓存失败，要重新下载
                return true;
            }
        }
        else {
            chksumInFS = fs.chkSum;
        }
		// 获得dcc表中保存的校验值
		unsigned int chksumInDcc;
		if (m_pSvFileCache->getFileInfo(id, chksumInDcc)) {
			// 两个校验值不相等就需要下载
			return chksumInDcc != chksumInFS;
		}
		else {
			// dcc中没有，需要下载
			return true;
		}
	}

	//返回的是一个array，元素类型为 {path:string,url:string}
	JsValue JsAppCache::getAppList() 
    {
		std::vector <std::string> paths;
		std::string path = m_pSvFileCache->getAppPath();
		//要去掉 sessionFiles
		fs::path full_path(path.c_str());
		full_path.remove_filename();
		if (fs::exists(full_path))
        {
			fs::directory_iterator item_begin(full_path);
			fs::directory_iterator item_end;
			for (; item_begin != item_end; item_begin++)
            {
				if (fs::is_directory(*item_begin))
                {
					auto urlid = (*item_begin).path() / "sourceid" / "appurl";
					fs::path apppath = (*item_begin).path().filename();
					paths.push_back(apppath.generic_string());	//先是实际路径，然后是对应的url，如果没有，对应的url就是""
					std::string url = "";
					if (fs::exists(urlid)) 
                    {
						JCBuffer buf;
						if (readFileSync(urlid.generic_string().c_str(), buf, JCBuffer::utf8)) {
							url = buf.m_pPtr;
							paths.push_back(url);
						}
					}
					if(url.length()<=0)
                    {
						paths.push_back("");
					}
				}
                else
                {
					//文件怎么处理
				}
			}
		}
#ifdef JS_V8
        //现在提供的转换不太方便转vector<string>，所以先自己写一个
        int size = (int)paths.size();
		v8::Isolate* piso = m_isolate;
		v8::Local< v8::Context> context = piso->GetCurrentContext();
		if (0 == size){
			v8::Local<v8::Array> array = v8::Array::New(piso, 0);
			return array;
		}
		else {
			v8::Local<Array> array = Array::New(piso, size);
			v8::HandleScope sc(m_isolate);
			for (int i = 0; i < size/2; i++) {
				v8::Local<Object> retobj = v8::Object::New(m_isolate);
				std::string& path = paths[i * 2];
				std::string& url = paths[i * 2 + 1];
				retobj->Set(context, Js_Str(m_isolate, "path"), Js_Str(m_isolate,(const char*)path.c_str()));
				if (url.length() > 0)
					retobj->Set(context, Js_Str(m_isolate, "url"), Js_Str(m_isolate, (const char*)url.c_str()));
				else
					retobj->Set(context, Js_Str(m_isolate, "url"), v8::Null(m_isolate));
				//array->Set(i, __TransferToJs<const char*>::ToJs(paths[i].c_str()));
				array->Set(context, i, retobj);
			}
			return array;
		}
#elif JS_JSC
        int size = (int)paths.size();
        JSContextRef ctx = __TlsData::GetInstance()->GetCurContext();
        if (0 == size){
            JSObjectRef array = JSObjectMakeArray(ctx, 0, nullptr, nullptr);
            return array;
        }
        else {
            size = size/2;
            std::vector<JSValueRef> arguments;
            arguments.reserve(size);
            for (int i = 0; i < size; i++){
                JSObjectRef retobj = JSObjectMake(ctx, nullptr, nullptr);
                std::string& path = paths[i * 2];
                std::string& url = paths[i * 2 + 1];
                JSStringRef jsstrKeyPath = JSStringCreateWithUTF8CString("path");
                JSStringRef jsstrKeyUrl = JSStringCreateWithUTF8CString("url");
                JSStringRef jsstrValuePath = JSStringCreateWithUTF8CString((const char*)path.c_str());
                JSStringRef jsstrValueUrl = JSStringCreateWithUTF8CString((const char*)url.c_str());
                
                JSObjectSetProperty(ctx, retobj, jsstrKeyPath, JSValueMakeString(ctx,jsstrValuePath), kJSPropertyAttributeNone, nullptr);
                if (url.length() > 0)
                    JSObjectSetProperty(ctx, retobj, jsstrKeyUrl, JSValueMakeString(ctx,jsstrValueUrl), kJSPropertyAttributeNone, nullptr);
                else
                    JSObjectSetProperty(ctx, retobj, jsstrKeyUrl, JSValueMakeNull(ctx), kJSPropertyAttributeNone, nullptr);
                arguments[i] = retobj;
            }
            
            JSObjectRef array= JSObjectMakeArray(ctx, size,&arguments[0], nullptr);
            return array;
        }
#endif
	}
	int32_t JsAppCache::getAppSize(const char* appurl)
    {
		return 0;
	}
    void JsAppCache::delCurAppCache() 
    {
        m_pSvFileCache->clearAllCachedFile();
    }

	bool JsAppCache::delAppCache(const char* appurl) 
	{
        m_pSvFileCache->clearAllCachedFile();
        return false;
	}
	void JsAppCache::delAllCache() 
    {

	}
    int JsAppCache::hashString(const char* str) 
    {
        return JCCachedFileSys::hashRaw(str);
    }
	void JsAppCache::exportJS()
    {
        JSP_CLASS("AppCache", JsAppCache);
        JSP_ADD_PROPERTY_RO(cacheSize, JsAppCache, getCacheSize);
        JSP_ADD_METHOD("update", JsAppCache::update)
        JSP_ADD_METHOD("url2Local", JsAppCache::url2Local);
        JSP_ADD_PROPERTY(enableCache, JsAppCache, getEnableCache, setEnableCache);
        JSP_ADD_METHOD("getResourceID", JsAppCache::getResourceID);
        JSP_ADD_METHOD("setResourceID", JsAppCache::setResourceID);
        JSP_ADD_METHOD("saveFileTable", JsAppCache::saveFileTable);
        JSP_ADD_METHOD("setFileTable", JsAppCache::setFileTable);
        JSP_ADD_METHOD("setUrlTransTable", JsAppCache::setUrlTransTable);
        JSP_ADD_METHOD("transUrlToCachedUrl", JsAppCache::setTransUrlToCachedUrl);
        JSP_ADD_METHOD("loadCachedURL", JsAppCache::loadCachedURL);
        JSP_ADD_METHOD("getCachePath", JsAppCache::getCachePath);
        JSP_ADD_METHOD("isFileTableValid", JsAppCache::isFileTableValid);	//为了打补丁用的
        JSP_ADD_METHOD("getAppList", JsAppCache::getAppList);
        JSP_ADD_METHOD("getAppSize", JsAppCache::getAppSize);
        JSP_ADD_METHOD("delAppCache", JsAppCache::delAppCache);
        JSP_ADD_METHOD("delCurAppCache", JsAppCache::delCurAppCache);
        JSP_ADD_METHOD("delAllCache", JsAppCache::delAllCache);
        JSP_ADD_METHOD("updateFile", JsAppCache::updateFileForJs);
        JSP_ADD_METHOD("hashstr", JsAppCache::hashString);
        JSP_ADD_METHOD("isUrlNeedDownload", JsAppCache::isUrlNeedDownload);
        JSP_REG_CONSTRUCTOR(JsAppCache, const char *);
        JSP_INSTALL_CLASS("AppCache", JsAppCache);
	}
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------