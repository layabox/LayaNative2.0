/**
@file			JCServerFileCache.h
@brief			
@author			guo
@version		1.0
@date			2016_5_11 
*/

#ifndef __JCServerFileCache_H__
#define __JCServerFileCache_H__

#include <functional>
#include <thread>
#include <mutex>
#include "../buffer/JCBuffer.h"
#include <string>
#include <map>
#define CURCACHEFILEVER 2
namespace laya
{
	class JCFileTable;
	class JCFileSource;
	class JCResStateDispatcher;

    /** 
     * @brief 用特殊目录和格式管理的文件缓存。
     * 设置一个目录path，然后会把所有的文件都保存在这个目录下。
     * path随时可以换。
     * 文件以id来查找。字符串也可以？
     * 文件都带一个壳，保存了校验码。
     * 壳可以与实际文件可以分离。
     * 可以更新每个文件。
    */
	class JCCachedFileSys {
	public:
		typedef unsigned int typeFile;
		typedef unsigned int typeChkSum;
		//每个文件的头
		struct fileShell{
			enum {
				magic_filid = 0x7788eeff,
			};
			fileShell(){
				magicNum=magic_filid;
				version = CURCACHEFILEVER;
				headSize = sizeof(fileShell);
				hashtype = ready = link = linkPackID = padd1= chkSum =0;
				linkSameFile = 1;
				reserved[0]=reserved[1]=reserved[2]=reserved[3]=0;
				linkFile=0;
				extfile = 0;
			}
			int magicNum;
			int version:8;
			unsigned int headSize:8;
			unsigned int hashtype:3;		//0 simplecrc 1 crc 2:BKDR ; 3:MD5
			unsigned int ready:1;			//1表示已经完成了，内容也是正确的。0表示还没有内容
			unsigned int link:1;			//1表示是一个链接。
			unsigned int linkPackID:3;		//使用哪个外部包
			unsigned int linkSameFile:1;	//1表示链接的文件时同名的。否则，就要使用下面的linkFile;
			unsigned int extfile : 1;		//实际文件内容在一个外部文件中，即头和文件体是分开的。主要是为了满足有的库需要直接访问文件。
            unsigned int extVersionMgr : 1; //使用外部版本管理，版本號只是一個int，佔用chkSum
            unsigned int tmpFile : 1;       //临时文件。为1的时候，expiredTime有效
            unsigned int withprocess : 1;   //临时文件，但是进程中不允许删除，一般是资源文件。
			int padd1:3;
			typeChkSum chkSum;
			int reserved[4];
			int linkFile;
            int svETag;                     //ETag的hash
            time_t expiredTime;             //过期时间
		};

		JCCachedFileSys();

		static typeChkSum hashRaw( const char* p_pszStr );

		void setCachePath(const char* p_pszPath);

        /** @brief 从缓存中加载一个文件。
         *  @param p_nFileID
         *  @param p_nChkSum 这个=0 表示不用进行校验
         *  @return
         *    如果从cache或者包中都找不到文件，则返回false，否则返回true，并设置文件内容和校验码
         *  可能会导致创建链接文件。对于不存在的不会创建壳。
        */
		bool load(typeFile p_nFileID, typeChkSum& p_nChkSum, JCSharedBuffer& p_BufRet );

        /** @brief 更新cache中的一个资源。不做校验码的检查。检查需要在外面做
         *  @param[in] p_nFileID
         *  @param[in] p_pBuff
         *  @param[in] p_nLen
         *  @param[in] p_nChkSum 这个值必须外部提供。此函数只是直接与头的记录进行比较。
         *  @param[in] p_bExtVersion 外部提供的版本号，如果为true，则 p_nChkSum就是版本号
         *  @param[in] p_tmExpiredTm 过期时间。只对临时文件有效
         *  @param[in] p_bWithProcess 进程内不允许删除。
         *  @return 如果保存成功。返回本地路径。否则返回"";
        */
		std::string updateAFile(typeFile p_nFileID, char* p_pBuff, int p_nLen, typeChkSum p_nChkSum,
            bool p_bExtVersion, time_t p_tmExpiredTm, bool p_bWithProcess);

        /** @brief 删除cache中的文件资源
         *  @param p_nFileID 
        */
		void delFromCache( typeFile p_nFileID );

		static typeChkSum  getChkSum( char* p_pBuff, int p_nLen );

        /** @brief 从缓存目录中读取资源。
         *    基本上相当于简单包装了的 readFileSync 
         *  @param  p_pszFile
         *  @param p_BufRet[out] 是文件内容，不含壳信息。
         *  @return  如果缓存中没有，就返回false。
        */
		bool load(const char* p_pszFile, JCSharedBuffer& p_BufRet, fileShell& p_nFs ,time_t& p_tmLastModify);

		/*
		  加载缓存的文件的信息。不实际加载文件。通常用来判断是否需要更新。
		*/
		bool loadShell(const char* p_pszFile, fileShell& p_nFs, time_t& p_tmLastModify);

		std::string fileToPath(typeFile p_nFile, std::string& p_strPath, bool p_bCreateDir);

		bool createLink( const char* p_pszFile, typeChkSum p_nChkSum);

		//转成字符串。
		std::string fileToStr(typeFile, bool bHex );

		//创建一个壳。还没有实际内容
		bool createShell(typeFile p_nFileID, typeChkSum p_nChkSum);

		//返回整个cache的大小。
		long size();

		//这个主要是为了效率，减少目录是否存在的判断。
		bool hasDir(unsigned char dir) {
			int pos = dir / 8;
			int off = dir % 8;
			return (m_HasDir[pos] >> off)&1;
		}
		void setHasDir(unsigned char dir) {
			int pos = dir / 8;
			int off = dir % 8;
			m_HasDir[pos] |= (1 << off);
		}
	protected:
		typedef std::recursive_mutex _mutex_t;
		_mutex_t		m_lockFileRW;
		std::string		m_strCacheFilesPath;
		unsigned char	m_HasDir[32];	//最多有256個目錄。每個bit表示一個。
	};

	class JCServerFileCache{
	public:
		typedef std::function<void(int,int)> funcOnUpdateProgress;	//更新过程
		typedef std::function<void(void)> funcOnUpdateOK;		//所有资源更新完成后的回调

        /*
            绝对地址的URL转换函数，为了能多个url对应一个cache入口
            返回0表示放弃转换。
            不允许直接修改原始url内存。
        */
        typedef char* (*FUN_TRANSLATEURL)(void* pUserData, const char*);
	public:
		JCServerFileCache();
		~JCServerFileCache();

        void saveFileTable(const char* p_pszFileContent);
        
        /** @brief 设置所有app的缓存根目录
         *  @param p_pszCachePath [in] 文件缓存的目录。是绝对路径。
         */
		void setCachePath(const char* p_pszCachePath );
		
        /** @brief  得到当前应用的缓存目录。最后没有/
         */
		std::string getAppPath();
        std::string getDccFile() { return getAppPath() + "/" + "filetable.txt";  };

        /** @brief 切换到某个url的缓存。
         *    大约相当于把url映射到一个本地path上
         *    每个process只允许调用一次。
         *  @param p_pszWebBase [in] 是index.html所在网络目录，例如 http://jx.laya8.com/game1/
         */
		void switchToApp(const char* p_pszWebBase);

        /** @brief  设置资源对象。提供访问文件的接口。
        */
		void setAssets(JCFileSource* pAssets );

		JCFileSource* getAssets(){ return m_pAssets;}

		/** @brief 计算相对路径的hash
		 *    注意url必须是一个相对路径，前面可以有 / 或者没有
         */
		JCCachedFileSys::typeChkSum  hashURLR(const char* p_pszURL );

        /** @brief 从cache中加载一个文件。
         *    如果cache中没有则尝试从资源包中加载。
         *    如果在资源包中存在，会创建一个链接文件。
         *    如果是需要管理的文件，并且需要下载，则先创建壳，然后再下载。
         *  @param p_nFileID [in] 
         *  @param p_nChkSum [in] 为0表示不用进行校验。
         *  @param p_BufRet [out] 返回的内容
         *  @param p_bUseVersion [in] 检查版本号
         *  @return true则存在，并且p_BufRet会有文件内容。否则返回 false
        */
		bool load(JCCachedFileSys::typeFile p_nFileID, JCCachedFileSys::typeChkSum& p_nChkSum, JCSharedBuffer& p_BufRet ,bool p_bUseVersion, bool doCheckSum);

        /** @brief  load一个文件。如果有dcc，就会把加载的内容进行dcc校验，否则就直接从缓存中加载。如果缓存中没有，就返回false。
         *      这个函数实际上最终会调用上面的load函数
        *  @param[in] nFileID 文件的本地id。
        *  @param[out] buff 返回的buffer
        *  @param[in] doCheckSum 是否进行版本号或者校验码的验证
        *  @return 加载成功就返回true，否则返回false
        */
        bool load(unsigned int nFileID, JCBuffer& buff, bool doCheckSum=true);

		/*
		加载缓存的文件的信息。不实际加载文件。通常用来判断是否需要更新。
		*/
		bool loadShell(const char* p_pszFile, JCCachedFileSys::fileShell& p_nFs, time_t& p_tmLastModify);

        /**
        *  @brief 从缓存中加载指定版本号的某个文件。
        *  @param[in] p_nFileID 文件id
        *  @param[in] p_nVersion 版本号
        *  @param[out] p_BufRet 返回的文件内容
        *  @return 如果文件存在，并且版本符合，就返回true，并且p_BufRet有返回内容
        */
        //bool loadByVersion(unsigned int p_nFileID, int p_nVersion, JCSharedBuffer& p_BufRet);

		//删除cache中的文件资源
		void delFromCache( JCCachedFileSys::typeFile p_nFileID ){ m_FileSys.delFromCache(p_nFileID);}

        /** @brief 根据p_pTable来更新所有的内容。这样以后就不用再需要fileTable了
         *   如果用索引表的方法（不用分散文件），必须用这种方式
         *   系统资源也用这种方法更新。
         *  @param p_pTable 如果为空，则直接使用自己的 fileTable 来更新
        */
		void updateAll( JCFileTable* p_pTable, funcOnUpdateOK p_OnUpdateOK);

        /** @brief 获得某个文件的校验码。
         *  @return 如果没有管理这个文件，返回false
         */
		bool getFileInfo(unsigned int p_nFileID, unsigned int& p_nChkSum );

		void setResourceID(const char* p_pszResource, const char* p_pszVal);
		std::string getResourceID(const char* p_pszResource );

        /** @brief
         *  删除本url的所有的缓存文件。包括session文件。
         *  这个函数可能很危险，不要暴露出去。
         *  @return 
        */
        void clearAllCachedFile();

        /** @brief 文件下载完成。 更新文件的缓存。
         *  @param[in] p_nFileID 文件ID。
         *  @param[in] p_pBuff
         *  @param[in] p_nLen 
         *  @param[in] p_nChkSum 校验值。外部设置的，只是要求这个函数保存，并不要求检验。
         *  @return 如果成功的话，返回本地目录。否则返回"";
        */
        std::string updateAFile(JCCachedFileSys::typeFile p_nFileID, char* p_pBuff, int p_nLen, 
            JCCachedFileSys::typeChkSum p_nChkSum, bool p_bExtVersion, time_t p_tmExpiredTm,
            bool p_bWithProcess) {
			return m_FileSys.updateAFile(p_nFileID, p_pBuff, p_nLen, p_nChkSum, p_bExtVersion,
                p_tmExpiredTm, p_bWithProcess);
		}

        /** 
        *  @brief 重新加载app目录下的dcc文件。一般是在更新dcc之后调用。
        *  @return 创建的dcc中的文件的个数。
        */
        int reloadDccFile();

        /**
            获得一个url的fileid。
            如果有转换表需要先转换一下
        */
        unsigned int getFileID(const char* Url);
    //protected:
        //这个函数要小心。要避免线程问题。本来是私有的。为了方便改成public了
        /** @brief 根据字符串设置dcc信息。
                注意只有开始的时候才能调用这个，当正常使用的时候，不允许设置，否则应该加锁
                外部不允许调用这个函数。否则不知道怎么处理。
         *  @param[in] p_pszFiles 
         *  @return 返回文件的个数。
        */
		int setFileTables( const char* p_pszFiles );

        /**
            设置url转换表。必须是相对路径。必须以/开头
        */
        void setUrlTransTable(const char* p_pszTable, char split);

    protected:

		//从资源包中读取。返回内容和chksum，如果false则表示没有这个文件
		bool _loadFromAssets(JCCachedFileSys::typeFile p_nFileID, JCSharedBuffer& p_BufRet, JCCachedFileSys::typeChkSum& p_nChkSum, bool p_bGetChkSum=true );

        //为了统一的解密加的，上面的那个只是统一入口
        bool __load(JCCachedFileSys::typeFile p_nFileID, JCCachedFileSys::typeChkSum& p_nChkSum, JCSharedBuffer& p_BufRet, bool p_bUseVersion, bool doCheckSum);

	protected:
        time_t          m_tmCacheMgrCreateTime;
        JCFileTable*    m_pFileTable;
		std::string		m_strCachePath;
		std::string		m_strWebBase;
		std::string		m_strCacheFilesPath;//在m_strCachePath下面的files中
		std::string		m_strAppPath;	//这是一个相对路径
		JCFileSource*	m_pAssets;
        std::map<unsigned int, std::string> m_UrlTrans;
    public://config
		JCCachedFileSys	m_FileSys;
		enum SessionFileCacheType {
            CT_UseHeaderInfo=0,
            CT_ValidInCurProcess,
            CT_AllwaysReload
        };

        /*
         * 是否根据header中的参数来决定是否缓存。
         * 如果是false，则临时文件都是
         */
        static SessionFileCacheType s_bSessionCacheType ;

        FUN_TRANSLATEURL   m_pFuncTransUrl = nullptr;
        void*              m_pFuncTransUrlData = nullptr;
	};
}

#endif //__JCServerFileCache_H__
