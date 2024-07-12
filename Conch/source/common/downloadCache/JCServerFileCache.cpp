/**
@file			JCServerFileCache.cpp
@brief			
@author			guo
@version		1.0
@date			2016_5_11
*/

#include "JCServerFileCache.h"
#include "../downloadCache/JCFileTable.h"
#include "../util/JCCrypto.h"
#include "../util/JCSimpleCRC.h"
#include <zlib.h>
#include <sys/stat.h>
#include "../resource/JCFileResManager.h"
#include "../fileSystem/JCFileSystem.h"
#include "../util/JCCommonMethod.h"
#include "../util/Log.h"
#include "../util/JCZipFile.h"
#include "../util/JCCrypto.h"
#include <time.h>
#include <chrono>
#include "../downloadMgr/JCHttpHeader.h"

#ifdef ANDROID
    #include "../downloadCache/JCAndroidFileSource.h"
#elif __APPLE__
    #include "../downloadCache/JCIosFileSource.h"
#elif OHOS
    #include "../downloadCache/JCOHOSFileSource.h"
    #include "napi/plugin_manager.h"
#endif
#define PATH_SOURCEID "sourceid"
#define ERROR_FILE_C_R_W (-6)

extern std::string gRedistPath;
//这个对象不依赖于process，所以放到外面。

#ifdef ANDROID
    AAssetManager* g_pAssetManager=nullptr;
    std::string gAssetRootPath="";
	std::string gAPKExpansionMainPath="";
	std::string gAPKExpansionPatchPath="";
#elif __APPLE__
extern std::string gAssetRootPath;
#elif OHOS
NativeResourceManager* g_pAssetManager = nullptr;
std::string gAssetRootPath="";
#else
std::string gAssetRootPath="d:/temp/myassets";
#endif

namespace laya
{

#define FALSE_RET(v,msg,...) {if(!(v)){ /*LOGE(msg,__VA_ARGS__);*/ return false;}}
#define FALSE_RET0(v,msg) {if(!(v)){ LOGE(msg); return false;}}  
    JCServerFileCache::SessionFileCacheType JCServerFileCache::s_bSessionCacheType =
        JCServerFileCache::CT_ValidInCurProcess;

	JCCachedFileSys::JCCachedFileSys() {
		memset(m_HasDir, 0, sizeof(m_HasDir));
	}

	std::string JCCachedFileSys::fileToStr( JCCachedFileSys::typeFile file, bool bHex ){
		char ret[32];
		if( bHex )
			sprintf(ret, "%08x", file );
		else
			sprintf(ret, "%u", file );
		return std::string(ret);
	}

	JCCachedFileSys::typeChkSum JCCachedFileSys::hashRaw(const char* p_pszStr){
		if(!p_pszStr)
			return 0;
        return  ::crc32(0, (const Bytef*)p_pszStr, strlen(p_pszStr));
	}
	
	void JCCachedFileSys::setCachePath(const char* p_pszPath){
		m_strCacheFilesPath = p_pszPath;
		memset(m_HasDir, 0, sizeof(m_HasDir));
		/* 有两种方案，一种是在这里创建所有的目录，这样下载某个文件的时候就不用再检查和创建了。
		这种可能会启动的时候慢一些，文件少的时候查找慢一些。
		std::string preDir = m_strCacheFilesPath+"00/";
		const char* pPreDir = preDir.c_str();
		int stPos = m_strCacheFilesPath.length();
		static unsigned char hexchar[]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
		for(int i=0; i<256; i++){
			unsigned char v=i;
			preDir[stPos]=hexchar[((v&0xf0)>>4)];
			preDir[stPos+1]=hexchar[(v&0xf)];
			if( !fs::exists(pPreDir)){
				//如果目录不存在
				fs::create_directories(pPreDir);
			}
		}
		*/
	}

	bool JCCachedFileSys::load(const char* p_pszFile, JCSharedBuffer& p_BufRet, fileShell& p_nFs, 
            time_t& p_tmLastModify){
        std::lock_guard<std::recursive_mutex> lock(m_lockFileRW);
        struct stat buf;
        int fd=0;
		FILE* pf = fopen(p_pszFile,"rb+");
		if(pf){//如果存在文件
            fd = fileno(pf);
            fstat(fd, &buf);
            p_tmLastModify = buf.st_mtime; //最后修改时间
			//fseek(pf,0,SEEK_END);
            unsigned int filesz = buf.st_size;// ftell(pf);	//注意：这个函数对于超过2G的文件无法返回正确的结果
			//fseek(pf,0,SEEK_SET);
			int l = fread(&p_nFs,1,sizeof(fileShell),pf);
			if(l<sizeof(fileShell)){
				fclose(pf);
				LOGE("Error! JCServerFileCache::loadInCache read error! len=%d, should %d\n",l, sizeof(fileShell));
				//throw -1;
				return false;
			}
			if( !p_nFs.ready){
			  fclose(pf);
			  return false;
			}
			  
			int contentSz = filesz-sizeof(fileShell);
			if(contentSz>0){
				p_BufRet.create(contentSz);
				int nReadSz = fread(p_BufRet.m_pBuffer.get(), 1, contentSz, pf);
				if( nReadSz!=contentSz ){
					//LOGE("Error! JCServerFileCache::_loadFromCache fread error, want=%d, read=%d\n", contentSz, nReadSz);
					fclose(pf);
					return false;
				}
			}
			fclose(pf);
			return true;
		}else
			return false;
	}

	bool JCCachedFileSys::loadShell(const char* p_pszFile, fileShell& p_nFs, time_t& p_tmLastModify){
        std::lock_guard<std::recursive_mutex> lock(m_lockFileRW);
        struct stat buf;
        int fd=0;
		FILE* pf = fopen(p_pszFile,"rb+");
		if(pf){//如果存在文件
            fd = fileno(pf);
            fstat(fd, &buf);
            p_tmLastModify = buf.st_mtime; //最后修改时间
			//fseek(pf,0,SEEK_END);
            unsigned int filesz = buf.st_size;// ftell(pf);	//注意：这个函数对于超过2G的文件无法返回正确的结果
			//fseek(pf,0,SEEK_SET);
			int l = fread(&p_nFs,1,sizeof(fileShell),pf);
			if(l<sizeof(fileShell)){
				fclose(pf);
				LOGE("Error! JCServerFileCache::loadInCache read error! len=%d, should %d\n",l, sizeof(fileShell));
				//throw -1;
				return false;
			}
			if( !p_nFs.ready){
			  fclose(pf);
			  return false;
			}
			  
			int contentSz = filesz-sizeof(fileShell);
			if(contentSz>0){
			}
			fclose(pf);
			return true;
		}else
			return false;
	}

	bool JCCachedFileSys::load(typeFile p_nFileID, typeChkSum& p_nChkSum, JCSharedBuffer& p_bufRet){
		std::string path;
		std::string pathfile =  fileToPath(p_nFileID,path,false);
		fileShell fs;
        time_t tmModify = 0;
		bool bret = load(pathfile.c_str(), p_bufRet, fs, tmModify);
		p_nChkSum = fs.chkSum;
		return bret;
	}

	JCCachedFileSys::typeChkSum JCCachedFileSys::getChkSum( char* p_pBuff, int p_nLen ){
		JCSimpleCRC crc;
		crc.process_bytes(p_pBuff, p_nLen);
		typeChkSum chksum = crc.checksum();
		return chksum;
	}

    std::string JCCachedFileSys::updateAFile(typeFile p_nFileID, char* p_pBuff, int p_nLen, typeChkSum p_nChkSum,
        bool p_bExtVersion, time_t p_tmExpiredTm, bool p_bWithProcess){
        std::lock_guard<std::recursive_mutex> lock(m_lockFileRW);
		//写文件
		std::string path;
		try {
			std::string pathfile = fileToPath(p_nFileID,path,true);
			FILE* pf = fopen(pathfile.c_str(),"wb");
			if(pf){
				fileShell fs;
                if (p_bExtVersion)
                    fs.extVersionMgr = 1;
				fs.chkSum = p_nChkSum;
                fs.expiredTime = p_tmExpiredTm;
                fs.withprocess = p_bWithProcess;
                //p_tmExpiredTm==0也是持久的。==0并且不持久的话，不会到这里
                if (p_bExtVersion || p_nChkSum != 0 || p_tmExpiredTm==0) {
                    fs.tmpFile = 0;
#ifdef _DEBUG
                    if (p_tmExpiredTm != 0) {
                        LOGE("持久缓存类型的文件，不能设置失效期");
                        *(int*)0 = 10;
                    }
#endif
                }
                else {
                    fs.tmpFile = 1;
#ifdef _DEBUG
                    if (p_tmExpiredTm == 0) {
                        LOGE("临时缓存类型的文件，必须设置失效期");
                        *(int*)0 = 10;
                    }
#endif
                }
				int l = fwrite(&fs,1,sizeof(fs),pf);
				if(l<sizeof(fs)){
					fclose(pf);
					throw ERROR_FILE_C_R_W;
				}
				l = fwrite( p_pBuff, 1,p_nLen, pf);
				if(l<p_nLen){
					fclose(pf);
					throw ERROR_FILE_C_R_W;
				}
				fs.ready=true;
				fflush(pf);
				fseek(pf,0,SEEK_SET);
				l = fwrite(&fs,1,sizeof(fs),pf);
				if(l<sizeof(fs)){
					fclose(pf);
					throw ERROR_FILE_C_R_W;
				}
				fflush(pf);
				fclose(pf);
                return pathfile;
			}else{
				//打开文件失败，应该是目录不对，或者没有权限。
				LOGE("Error! JCServerFileCache::onFileDownloaded fopen error! file=%08x\n", p_nFileID);
				throw ERROR_FILE_C_R_W;
			}
		}catch(...){
			if( global_onCreateFileError){
				global_onCreateFileError();
			}
		}
        static string errret = "";
        return errret;
	}

	void JCCachedFileSys::delFromCache( typeFile p_nFileID ){
		std::string path;
		std::string pathfile = fileToPath(p_nFileID,path,false);
		remove(pathfile.c_str());
	}

	bool JCCachedFileSys::createShell(JCCachedFileSys::typeFile p_nFileID, JCCachedFileSys::typeChkSum p_nChkSum){
        std::lock_guard<std::recursive_mutex> lock(m_lockFileRW);
		std::string path;
		try{
			std::string pathfile = fileToPath(p_nFileID, path,true);

			FILE* pf = fopen(pathfile.c_str(),"wb");
			if(pf){
				fileShell fs;
				fs.chkSum = p_nChkSum;
				int l = fwrite(&fs,1,sizeof(fs),pf);
				if(l<sizeof(fs)){
					fclose(pf);
					throw ERROR_FILE_C_R_W;
				}
				fclose(pf);
				return true;
			}else{
				//打开文件失败，应该是目录不对，或者没有权限。
				throw ERROR_FILE_C_R_W;
				return false;
			}
		}catch(...){
			if( global_onCreateFileError){
				global_onCreateFileError();
			}
			return false;
		}
		return true;
	}

	bool JCCachedFileSys::createLink( const char* p_pszFile, typeChkSum p_nChkSum){
        std::lock_guard<std::recursive_mutex> lock(m_lockFileRW);

		try{
			FILE* linkf = fopen(p_pszFile, "wb");
			if(!linkf){
				LOGE("Error: can't create link file %s\n", p_pszFile);
				throw ERROR_FILE_C_R_W;
				return false;
			}
			fileShell fs;
			fs.link = 1;
			fs.chkSum=p_nChkSum;
			fs.ready = 1;
			int l = fwrite(&fs,1,sizeof(fs),linkf);
			if(l<sizeof(fs)){
				fclose(linkf);
				throw ERROR_FILE_C_R_W;
			}
			fclose(linkf);
		}catch(int e){
			if( e==ERROR_FILE_C_R_W && global_onCreateFileError){
				global_onCreateFileError();
			}
			return false;
		}
		return true;
	}

	long JCCachedFileSys::size() {
		return 0;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////

	JCServerFileCache::JCServerFileCache(){
		m_pAssets = NULL;
		/*
#ifdef ANDROID
		m_pAssets = new JCAndroidFileSource();
#elif __APPLE__
		m_pAssets = new JCIosFileSource();
#elif OHOS
		m_pAssets = new JCOHOSFileSource();
#endif
		*/
		m_pFileTable=NULL;
		m_strAppPath="app";
        m_tmCacheMgrCreateTime = 0;
	}

	JCServerFileCache::~JCServerFileCache(){
		if( m_pFileTable ){
			delete m_pFileTable;
			m_pFileTable=NULL;
		}
		if( m_pAssets ){
			delete m_pAssets ;
			m_pAssets = NULL;
		}
	}
    
    void JCServerFileCache::saveFileTable(const char* p_pszFileContent)
    {
        if(!p_pszFileContent)
            p_pszFileContent="";//写空的
        std::string file =  this->getAppPath()+"/"+"filetable.txt";
        JCBuffer buf((char*)p_pszFileContent, strlen(p_pszFileContent),false,false);
        writeFileSync( file.c_str(), buf, JCBuffer::utf8);
    }
    
	void JCServerFileCache::setCachePath( const char* p_pszCachePath ){
		static char buf[512];
		int len = strlen(p_pszCachePath);
		strcpy(buf,p_pszCachePath);
		if( p_pszCachePath[len-1]=='\\' )buf[len-1]='/';
		else if(buf[len-1]!='/')strcat(buf,"/");
		m_strCachePath = buf;
		LOGI("setCachePath:%s", buf);
		try{
			if( !fs::exists(m_strCachePath.c_str())){
				//如果目录不存在
				fs::create_directories(m_strCachePath.c_str());
			}
		}catch(...){
			if( global_onCreateFileError){
				global_onCreateFileError();
			}
		}			
	}

	std::string&   replace_all(std::string& str,const std::string&   old_value, const std::string&   new_value){   
		while(true)   {   
			std::string::size_type pos(0);   
			if( (pos=str.find(old_value))!=std::string::npos   )   
				str.replace(pos,old_value.length(),new_value);   
			else   break;   
		}   
		return   str;   
	}

	void JCServerFileCache::switchToApp(const char* p_pszWebBase){
		static char buf[512];
		int len = strlen(p_pszWebBase);
		strcpy(buf,p_pszWebBase);
		if( buf[len-1]=='\\') buf[len-1]='/';
		else if(buf[len-1]!='/')strcat(buf,"/");
		m_strWebBase = buf;
		JCCachedFileSys::typeChkSum appurlchk = m_FileSys.hashRaw(p_pszWebBase);
		sprintf(buf,"%08x", appurlchk);
		m_strAppPath = buf;

		//文件对象目录
		m_strCacheFilesPath = m_strCachePath+m_strAppPath+ "/files/";
		try {
			if (!fs::exists(m_strCacheFilesPath.c_str())) {
				//如果目录不存在
				fs::create_directories(m_strCacheFilesPath.c_str());
			}
		}
		catch (...) {
			if(global_onCreateFileError)
				global_onCreateFileError();
		}
		m_FileSys.setCachePath( m_strCacheFilesPath.c_str());

		//资源id目录
		std::string sourceidPath = m_strCachePath+m_strAppPath+ "/"+PATH_SOURCEID+"/";
		try {
			if (!fs::exists(sourceidPath.c_str())) {
				fs::create_directories(sourceidPath.c_str());
			}
		}
		catch (...) {
			if(global_onCreateFileError)
				global_onCreateFileError();
		}
		std::string tmpWebBase = m_strWebBase;
		if(tmpWebBase[tmpWebBase.length()-1]=='/' || tmpWebBase[tmpWebBase.length()-1]=='\\'){
			tmpWebBase = tmpWebBase.substr(0,tmpWebBase.length()-1);
		}
		replace_all(tmpWebBase,"http://","");
		replace_all(tmpWebBase,":",".");
		replace_all(tmpWebBase,"/","_");
		replace_all(tmpWebBase,"\\","_");
		std::string assetsPath = gAssetRootPath+"/"+tmpWebBase;
		JCFileSource* pFileReader = NULL;
#ifdef ANDROID
		if(g_pAssetManager)
        {
			JCAndroidFileSource* pAssets = new JCAndroidFileSource();
			pAssets->Init((AAssetManager*)g_pAssetManager, assetsPath.c_str(), gAPKExpansionMainPath, gAPKExpansionPatchPath, tmpWebBase);
			pFileReader = pAssets;
		}
        else
        {
            JCZipFile* pfr = new JCZipFile();
			if( pfr->open(gAssetRootPath.c_str())){
				pfr->InitDir(assetsPath.c_str());
				pFileReader = pfr;
			}else{
				LOGE("打开jar失败：%s", gAssetRootPath.c_str());
				delete pfr;
			}
		}
#elif __APPLE__
		JCIosFileSource* pAssets = new JCIosFileSource();
		pAssets->Init(assetsPath.c_str());
		pFileReader = pAssets;
#elif OHOS
		JCOHOSFileSource* pAssets = new JCOHOSFileSource();
		pAssets->Init((NativeResourceManager*)g_pAssetManager, assetsPath.c_str());
		pFileReader = pAssets;
#endif

		setAssets( pFileReader);

		if(m_pFileTable){
			delete m_pFileTable;
			m_pFileTable=NULL;
		}

		std::string filetable = getAppPath()+"/"+"filetable.txt";

        JCBuffer ftbuf;
		readFileSync(filetable.c_str(), ftbuf, JCBuffer::utf8);
		LOGI("read index:%s", filetable.c_str());
		if(ftbuf.m_pPtr)
			setFileTables(ftbuf.m_pPtr);
		else
			LOGW("read DCC file table failed");

        time(&m_tmCacheMgrCreateTime);
	}

    void JCServerFileCache::clearAllCachedFile() {
        //保护一下，防止设置错误导致的删除了其他目录
        if (m_strCachePath.length() < 4 || m_strAppPath.length() <= 0) {
            LOGE("clearAllCachedFile error！");
            return;
        }
        std::string sourceidPath = m_strCachePath + m_strAppPath + "/files/";
        try {
            fs::remove_all(sourceidPath.c_str());
            fs::create_directories(sourceidPath.c_str());
        }
        catch (...) {
        }
    }

	std::string JCServerFileCache::getAppPath(){
		return m_strCachePath+m_strAppPath;
	}

	JCCachedFileSys::typeChkSum  JCServerFileCache::hashURLR(const char* p_pszURL ){
		if(!p_pszURL)
			return 0;
		static char url[512];
		//前面加上'/'主要是为了与以前的版本兼容。
		if( p_pszURL[0]!='/'){
			url[0]='/'; url[1]=0;
			strcat(url,p_pszURL);
			return m_FileSys.hashRaw(url);
		}
		return m_FileSys.hashRaw(p_pszURL);
	}

	void JCServerFileCache::setAssets( JCFileSource* pAssets ){
		if( m_pAssets ){
			delete m_pAssets ;
			m_pAssets = NULL;
		}
		m_pAssets = pAssets;
		/*
			说明：
				如果 assetsid和本地记录的assetsid 不一致，则表示重新安装了（较少可能是清理资源了），则从资源包中拷贝filetable，并记录资源id
				如果之前已经从网络更新过，则相当于抛弃掉网络下载的filetable
				如果没有资源包信息，则不从资源包更新assetsid和filetable。一切从网络下载。
				网络获得的assetsid在本地记录为netassetsid
		*/
		//打开资源中的版本
        char* assetsid = NULL;
        int assetsidLen = 0;
		if( m_pAssets->isFileExist("assetsid.txt"))
        {
			if( m_pAssets->loadFileContent("assetsid.txt", assetsid, assetsidLen) == false)
            {
                LOGE("something was wrong that reading file assetsid.txt from the resource package.\n");
			}
		}
		//查看版本，看是否需要更新
		std::string cachedAssetsID = getResourceID("assetsid");

		//拷贝资源包中的fileTable
		const char* ftfile="filetable.txt";
		std::string filetable = getAppPath()+"/"+ftfile;

		bool hasAssetsFt = m_pAssets->isFileExist(ftfile);
		bool hasAssetsID = assetsidLen>0;

		bool hasAssets = hasAssetsFt || hasAssetsID ;

		if(hasAssets && (cachedAssetsID.length()==0 || assetsidLen != cachedAssetsID.length()|| strcmp(assetsid, cachedAssetsID.c_str())!=0) ){
			//清理文件缓存
			clearAllCachedFile();
			//先获取资源中的filteTable
			char* pFileTableBuf=NULL;
			int nFileTableLen = 0;
			if(hasAssetsFt)
            {
				if( m_pAssets->loadFileContent(ftfile, pFileTableBuf, nFileTableLen ) == false )
                {
					LOGE("read the file which names filetable.txt error!");
				}	
			}
            else
            {
				LOGI("the file which names filetable.txt is not exist");
			}
			if( pFileTableBuf )
            {
				//如果没有指定assetsid,则需要根据filetables来计算
				if( assetsidLen==0 ){
					JCCachedFileSys::typeChkSum chksum = m_FileSys.hashRaw(pFileTableBuf);
					sprintf(assetsid,"%x",chksum);
					assetsidLen = strlen(assetsid);
					if( assetsidLen==cachedAssetsID.length() && strcmp(assetsid,cachedAssetsID.c_str())==0)
                    {
                        delete[] assetsid;
                        delete[] pFileTableBuf;
						return;
					}
				} 
				try
                {
					writeFileSync1(filetable.c_str(), pFileTableBuf, nFileTableLen);
				}
				catch (...) 
                {
					if(global_onCreateFileError)
						global_onCreateFileError();
				}
                delete[] pFileTableBuf;
				//记录资源id
				setResourceID("assetsid",assetsid);
			}
            else{
				char emptybuf[]={'\0'};
                JCBuffer ftbuf(emptybuf,sizeof(emptybuf),false,false);
				try {
					writeFileSync(filetable.c_str(), ftbuf);
				}
				catch (...) {
					if(global_onCreateFileError)
						global_onCreateFileError();
				}
			}
		}
        delete[] assetsid;
	}

	int JCServerFileCache::setFileTables(const char* p_pszFiles ){
		if( m_pFileTable ){
			delete m_pFileTable;
			m_pFileTable = NULL;
		}
		m_pFileTable = new JCFileTable();
		//buffer buf;
		//readFileSync(p_pszFile, buf, buffer::utf8);
		return m_pFileTable->initByString(p_pszFiles);
	}

    /**
        这个能处理绝对路径和相对路径，以后要代替 hashURLFull和 hashURLR
    */
    unsigned int JCServerFileCache::getFileID(const char* Url) {
        if (Url==nullptr)
            return 0;
        char* nUrl = (char*)Url;
        bool bChanged = false;
        if (m_pFuncTransUrl && m_pFuncTransUrlData) {
            //先做外部提供的url转换。url转换通常是为了让不同的url指向同一个缓存，例如不同的cdn指向同一个缓存。
            nUrl = (*m_pFuncTransUrl)(m_pFuncTransUrlData,Url);
            if (!nUrl)
                nUrl = (char*)Url;
            else if(nUrl!=Url)//这里有个规则，就是转换函数不允许直接操作字符串内存。
                bChanged = true;
        }

        int len = strlen(nUrl);
        char* pRelUrl =(char*)nUrl;
        bool isAbs = (len > 7 && memcmp(nUrl, "http://", 7)==0) || (len > 8 && memcmp(nUrl, "https://", 8)==0);
        if (isAbs) {
            if (memcmp(nUrl, m_strWebBase.c_str(), m_strWebBase.length()) == 0) {
                pRelUrl = (char*)(nUrl + m_strWebBase.length() - 1);
            }
            else {
                LOGW("Error: %s[%s] does not belong to the same app domain: %s ", Url,(bChanged?nUrl:""), m_strWebBase.c_str());
                return m_FileSys.hashRaw(Url);
            }
        }
        //如果是相对路径需要保证以/开头
        static char _url[512];//注意：这个是非线程安全的
        //前面加上'/'主要是为了与以前的版本兼容。
        if (pRelUrl[0] != '/') {
            _url[0] = '/'; _url[1] = 0;
            strcat(_url, pRelUrl);
            pRelUrl = _url;
        }

        unsigned int hv = m_FileSys.hashRaw(pRelUrl);
        //然后再做相对路径的转换。
        auto it = m_UrlTrans.find(hv);
        if (it != m_UrlTrans.end()) {
            hv = m_FileSys.hashRaw(it->second.c_str());
        }
        return hv;
    }

    void JCServerFileCache::setUrlTransTable(const char* p_pszTable,char split) {
        m_UrlTrans.clear();
        const char* pCur = p_pszTable;
        const char* pCurData = pCur;
        static char svUrl[2048];
        static char cacheUrl[2048];
        while (*pCur != 0) {
            while (*pCur == split || *pCur == '\r' || *pCur == '\n') {
                pCur++;
            }
            if (*pCur == 0)
                break;
            const char* pData = pCur; //起点
            while (*pCur != split && *pCur != 0) {
                pCur++;
            }
            int len = (long)pCur - (long)pData ;
            memcpy(svUrl, pData, len);
            svUrl[len] = 0;
            if (*pCur == 0)
                break;

            while (*pCur == split || *pCur == '\r' || *pCur == '\n') {
                pCur++;
            }
            if (*pCur == 0)
                break;
            pData = pCur;
            while (*pCur != '\r'&&*pCur != '\n'&&*pCur != 0) {
                pCur++;
            }
            len = (long)pCur - (long)pData;
            memcpy(cacheUrl, pData, len);
            cacheUrl[len] = 0;
            int hv = m_FileSys.hashRaw(svUrl);
            if (m_UrlTrans.find(hv) != m_UrlTrans.end()) {
                LOGE("错误：设置url转换表有问题，路径重复，或者hash计算冲突！这个会导致更新cache或者从cache中取文件的时候出错！");
            }
            m_UrlTrans[hv] = cacheUrl;
        }
    }

    int JCServerFileCache::reloadDccFile() {
        std::string filetable = getAppPath() + "/" + "filetable.txt";
        JCBuffer ftbuf;
        readFileSync(filetable.c_str(), ftbuf, JCBuffer::utf8);
        LOGI("read index:%s", filetable.c_str());
        if (ftbuf.m_pPtr)
            return setFileTables(ftbuf.m_pPtr);
        else
            LOGE("read index failed");
        return 0;
    }

	bool JCServerFileCache::getFileInfo(unsigned int p_nFileID, unsigned int& p_nChkSum ){
		if( !m_pFileTable )
			return false;
		return m_pFileTable->find(p_nFileID, p_nChkSum );
	}

    bool _after_cache_loaded(const char* pfilename, char* ptr, int len, char*& newptr, int& newlen) {
        //如果加密了要先解密
        int rlen = len;
        bool temp = JCEncrypt::decrypt(ptr, rlen);
        if (temp) {
            rlen = rlen - JCEncrypt::s_nPreLen;
        }
        newlen = rlen;
        newptr = ptr;
        return temp;
    }

    bool JCServerFileCache::load(JCCachedFileSys::typeFile p_nFileID, JCCachedFileSys::typeChkSum& p_nChkSum,
        JCSharedBuffer& p_bufRet, bool p_bUseVersion, bool doCheckSum) {
        bool bret = __load(p_nFileID, p_nChkSum, p_bufRet, p_bUseVersion, doCheckSum);
        char* ptr = p_bufRet.m_pBuffer.get();
        if (bret && p_bufRet.m_nLen && ptr) {
            char* newptr = nullptr;
            int newlen = 0;
            bool bchanged = _after_cache_loaded(nullptr, ptr, p_bufRet.m_nLen, newptr, newlen);
            if (bchanged) {
            }
            if (newptr ){
                if (newptr != ptr) {
                    p_bufRet.create(newlen);
                    memcpy(p_bufRet.m_pBuffer.get(), newptr, newlen);
                    delete[] newptr;
                }
                else {
                    if (newlen < p_bufRet.m_nLen) {
                        p_bufRet.m_nLen = newlen;//这个需要重新分配一下么
                    }
                }
            }
        }
        return bret;
    }

	bool JCServerFileCache::__load(JCCachedFileSys::typeFile p_nFileID, JCCachedFileSys::typeChkSum& p_nChkSum, 
        JCSharedBuffer& p_bufRet, bool p_bUseVersion, bool doCheckSum){
		try {
			std::string path;
			std::string pathfile = m_FileSys.fileToPath(p_nFileID, path, true);
			//struct stat statbuf;
			//if(stat(pathfile, &statbuf)<0){
			//	if( errno == ENOENT){//文件不存在
			//	}
			//}
			JCCachedFileSys::typeChkSum assetChk = 0;
			JCCachedFileSys::fileShell fs;
            time_t tmModify = 0;
			if (m_FileSys.load(pathfile.c_str(), p_bufRet, fs,tmModify)) {
				//如果已经读取到了
				if (fs.link) {
                    //资源包中的文件一定是要管理的。
					FALSE_RET(_loadFromAssets(p_nFileID, p_bufRet, assetChk, p_nChkSum != 0), "Error! _loadFromAssets failed :file=%08x\n", p_nFileID);
					FALSE_RET0((!p_nChkSum || assetChk == p_nChkSum), "Waring! _loadFromAssets returns incorrect contents! what happened!\n");
				}
				else {
                    if (!fs.ready)	//如果还没下载下来
                        return false;
                    //如果是其他版本的不能使用
                    if (fs.version != CURCACHEFILEVER)
                        return false;
                    //不检查版本号的话，就成功了
                    if (!doCheckSum)
                        return true;

                    if (p_bUseVersion) {
                        return (fs.extVersionMgr && fs.chkSum == p_nChkSum);
                    }
                    else if (p_nChkSum) {
                        return (fs.chkSum == p_nChkSum);
                    }
                    //不走dcc和版本号的话。
                    switch (s_bSessionCacheType) {
                    case CT_AllwaysReload:
                        return false;
                        break;
                    case CT_UseHeaderInfo:
                        break;
                    case CT_ValidInCurProcess:
                    default:
                        //如果没有校验值，就是临时文件，通过时间来判断是不是新的（比管理器启动时间大就是新的）。
                        return tmModify >= m_tmCacheMgrCreateTime;
                    }
				}
				return true;
			}
			else {
				FALSE_RET(_loadFromAssets(p_nFileID, p_bufRet, assetChk), "Error! _loadFromAssets failed :file=%08x\n", p_nFileID);
				FALSE_RET0((!p_nChkSum || assetChk == p_nChkSum), "Waring! _loadFromAssets returns incorrect contents! what happened!\n");
				return m_FileSys.createLink(pathfile.c_str(), assetChk);
			}
		}catch (...) {
			if (global_onCreateFileError) {
				global_onCreateFileError();
			}
		}
		return false;
	}


    bool JCServerFileCache::load(unsigned int nFileID, JCBuffer& buff, bool doCheckSum) {
        unsigned int chksum = 0;
        JCSharedBuffer sbuf;
        //如果是dcc的，就能获得一个chksum
        bool inDcc = getFileInfo(nFileID, chksum);
        bool bloaded = load(nFileID, chksum, sbuf,false, doCheckSum);
        if (bloaded && sbuf.m_pBuffer.get()) {
            buff.create(sbuf.m_nLen);
            memcpy(buff.m_pPtr, sbuf.m_pBuffer.get(), sbuf.m_nLen);
            return true;
        }
        return false;
    }

	bool JCServerFileCache::loadShell(const char* p_pszFile, JCCachedFileSys::fileShell& p_nFs, time_t& p_tmLastModify) {
		return m_FileSys.loadShell(p_pszFile, p_nFs, p_tmLastModify);
	}

	std::string JCCachedFileSys::fileToPath(typeFile p_nFile, std::string& p_strPath, bool p_bCreateDir) {
		std::string file = fileToStr(p_nFile, true);
		std::string fileGroup = file.substr(0, 2);
		p_strPath = m_strCacheFilesPath + fileGroup + "/";
		file = p_strPath + file.substr(2, file.length());
		//因为这里比较容易得到子目录，所以检测和创建目录放到这个函数里面。
		if (p_bCreateDir) {
			const char* pFG = fileGroup.c_str();
			int dirID = hexStringToInt(pFG);
			if (!hasDir(dirID)) {
				if (!fs::exists(p_strPath.c_str())) {
					//如果目录不存在
					fs::create_directories(p_strPath.c_str());
					setHasDir(dirID);
				}
				else {
					setHasDir(dirID);
				}
			}
		}
		return file;
	}

	unsigned char* AllocSharedBuffer(int sz, void* pUserData) {
		JCSharedBuffer* pSB = (JCSharedBuffer*)pUserData;
		if (!pSB)
			return NULL;
		pSB->create(sz);
		return (unsigned char*)pSB->m_pBuffer.get();
	}

	bool JCServerFileCache::_loadFromAssets(JCCachedFileSys::typeFile p_nFileID, JCSharedBuffer& p_BufRet, JCCachedFileSys::typeChkSum& p_nChkSum, bool p_bGetChkSum ){
		if (!m_pAssets)
			return false;
		int sz = 0;
		//2M,4ms
		if (m_pAssets->loadFileContent(m_FileSys.fileToStr(p_nFileID,true).c_str(),AllocSharedBuffer, (void*)&p_BufRet, sz)) {
			//生成校验码
			if (p_bGetChkSum) {
				JCSimpleCRC crc;
				crc.process_bytes(p_BufRet.m_pBuffer.get(), sz);
				p_nChkSum = crc.checksum();
			}
			return true;
		}
		return false;
	}

	void JCServerFileCache::updateAll( JCFileTable* p_pTable, funcOnUpdateOK p_OnUpdateOK ){
		JCFileTable* pUsedTable = p_pTable?p_pTable:m_pFileTable;
	}

	void JCServerFileCache::setResourceID(const char* p_pszResource, const char* p_pszVal){
		if(!p_pszVal)
			return;

		std::string resourceidfile = getAppPath()+"/"+PATH_SOURCEID+"/"+p_pszResource;
		JCBuffer buf((char*)p_pszVal,strlen(p_pszVal),false,false);
		writeFileSync(resourceidfile.c_str(),buf,JCBuffer::utf8);
	}

	std::string JCServerFileCache::getResourceID(const char* p_pszResource ){
		std::string resourceidfile = getAppPath()+"/"+PATH_SOURCEID+"/"+p_pszResource;
        JCBuffer buf;
		readFileSync(resourceidfile.c_str(),buf, JCBuffer::utf8);
		if( buf.m_pPtr )
			return buf.m_pPtr;
		return "";
	}
}
//------------------------------------------------------------------------------

