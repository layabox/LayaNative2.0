/**
@file			JCFileResManager.h
@brief			
@author			James
@version		1.0
@date			2016_5_11
*/

#ifndef __JCFileResManager_H__
#define __JCFileResManager_H__


#include "JCResource.h"
#include <map>
#include <string>
#include <memory>
#include "../buffer/JCBuffer.h"
#include <mutex>
#include "../util/JCLayaUrl.h"

#define MAXDOWNLOADTRY 3

namespace laya{
    /**
     * @param [in] data {const char*} 原始文件内容。
     * @param [in out] len {int&} 原始文件内容的长度。
     * @return char* 如果改变了数据，就返回新的指针。
    */
    typedef char* (*HandleFileData)(const char* data, int& len);

    /**@brief 设置一个函数，可以用来修改下载的文件内容。一般用来解密。
    */
    void setFileDataHandler(HandleFileData h);

    /**
     * @brief 其实就是一个new，怕万一跨so的话有分配的问题。
    */
    char* _conch_mallocData(int len);

	class JCFileResManager;
	class JCServerFileCache;
    class JCDownloadMgr;
    class IConchThreadCmdMgr;
    //文件资源
	//例如避免下载的时候重复下载之类的
	//由于downloadManager已经缓存了，因此本身不缓存。
	class JCFileRes:public JCResStateDispatcher{
		friend class JCFileResManager;
    public:
        //最后的行为。测试用
        enum Action {
            INIT,
            REQUEST,
            LOADFROMCACHE,
            DOWNLOADING,
            DOWNLOADED,
            UPDATECACHE,
        };

        JCFileRes(JCDownloadMgr* pNetLoader, JCFileResManager* pMgr);	//只能由JCFileResManager创建
        ~JCFileRes();
        virtual bool	restoreRes();
        //直接读取缓存的。如果不在缓存中，则返回false
        bool	loadFromCache(JCBuffer& buff, bool bDoCheckSum);
        void	setIgnoreError(bool b) { m_bIgnoreError = b; };

        //设置不需要严格校验的文件。对于这些文件，如果校验失败了，则直接算作成功，并且保存下来
        static void addChkIgnoreChksumExt(const char* p_pszExt);
        static void clearChkIgnoreChksumExt();
        static bool checkIgnoreChksum(char* p_pszExt);

        /** @brief 加载资源。注意只允许调试或者manager调用。否则可能会同一个资源加载多次。
         *  @param[in] 
         *  @return 
        */
        void load(const char* p_pszURL, JCSharedBuffer* pSyncResult);
	protected:
		void onResDownloadOK_JSThread(std::weak_ptr<int> p_cbref);
		void onResDownloadErr_JSThread(std::weak_ptr<int> p_cbref,int p_nError, int p_nHttpResponse);

        void notifyErrorHandler(int p_nError, int p_nHttpResponse);

        /** @brief 下载完成的回调。
         *  @param[in] p_Buff 下载结果。
         *  @param[in] pLocalAddr 下载线程获得的本地地址。
         *  @param[in] pSvAddr 下载线程获得的服务器地址。
         *  @param[in] p_nChkSum 本次下载请求希望的校验值
         *  @param[in] p_nDownloadNum 这是本请求的第几次下载。因为校验错误的话会尝试多次下载。
         *  @param[in] p_cbref 保护用。
         *  @return void
        */
		void onDownloaded( JCBuffer& p_Buff , 
                const std::string& pLocalAddr, const std::string& pSvAddr, 
                int pnCurlRet, int pnHttpRet,
                const std::string& pstrHeader,
                unsigned int p_nChkSum, int p_nDownloadNum, std::weak_ptr<int> p_cbref );
        void onDownloadedOld(JCBuffer& p_Buff,
            const std::string& pLocalAddr, const std::string& pSvAddr,
            int pnCurlRet, int pnHttpRet,
            const std::string& pstrHeader,
            unsigned int p_nChkSum, int p_nDownloadNum, std::weak_ptr<int> p_cbref);

		void onDownloadError(int p_nError, int p_nHttpResponse,std::weak_ptr<int> p_cbref );
		int onProgress(unsigned int, unsigned int,float,std::weak_ptr<int> p_cbref );

		//带校验的下载。p_nChkSum可以为0，表示不检查
		void verifyDownload(const char* p_pszURL, unsigned int p_nChkSum );
		bool checkIsEncrypted(char *buf,int len);
        
        void normalizeUrl();
	public:
        Action                          m_nLastAction;  //测试用
		std::string						m_strURL;
        std::string                     m_strSvIP;      //调试用
		unsigned int					m_nLocalFileID;
        unsigned int                    m_nExtVersionID = 0;      //外部版本号。是一个hash后的值。
        bool                            m_bExtVersion = false;
		std::shared_ptr<char>		    m_pBuffer;	            //注意必须指定deleter std::default_delete<char[]>
		int								m_nLength;
		std::shared_ptr<int>			m_CallbackRef;
		static std::string	            s_strUploadChkErrUrl;	//上传校验错误的url，不需要则为空
        static std::string              s_strExtVersion;        //外部版本控制字符串。例如 ?v=1, 则为 v= 加上=是为了避免 v=2和vxx=2分不开
        int                             m_nLastHttpResponse = 0;
        int                             m_nConnTimeout = 0;
        int                             m_nOptTimeout = 0;
	protected:
        JCDownloadMgr*                  m_pNetLoader;
        JCFileResManager*	            m_pMgr;
		bool                            m_bDownloading;
		bool	                        m_bIgnoreError;			//是否忽略下载错误
		static std::vector<std::string>	s_vIgnoreChksumError;
		static bool						s_bHasIgnoreChksum;
		static std::mutex				s_ignorechklock;
        std::mutex                      m_CallbackLock;
		bool							m_bSendToJS_complete;	//完成事件已经post给js队列等待处理了。避免同一个对象多次post。必须都在js线程处理这个变量
        JCUrl                           m_Url;
	};

	class JCFileResManager{
	public:
		typedef std::map<std::string, JCFileRes*>	FileResMap;

		JCFileResManager(JCDownloadMgr* pDownloadMgr);

		~JCFileResManager();

		void setFileCache( JCServerFileCache* p_pFileCache){
			m_pFileCache = p_pFileCache;
		}
        JCFileRes* getRes(const std::string& url,int p_nConnTimeout=0,int p_nOptTimeout=0);
        bool delRes(const char* p_pszURL);
        void clear();
	public:
		JCServerFileCache*		    m_pFileCache;
		bool			            m_bUrlToLowerCase;
		std::mutex		        m_maplock;
        std::vector<std::string>    m_vExtNeedSave;     //这种扩展名的需要保存到临时缓存中。
        IConchThreadCmdMgr*         m_pCmdPoster;
    protected:
        JCDownloadMgr*              m_pDownloadMgr;
        FileResMap			        m_ResMap;
	};

}
//------------------------------------------------------------------------------


#endif //__JCFileResManager_H__
