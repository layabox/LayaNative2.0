/**
@file			JCDownloadMgr.h
@brief			
@author			hugao
@version		1.0
@date			2016_5_11
*/

#ifndef __JCDownloadMgr_H__
#define __JCDownloadMgr_H__

#include "../buffer/JCBuffer.h"
#include <functional>
#include <thread>
#include <mutex>
#include <map>
#include <atomic>
#include "../misc/JCLayaThreadPool.h"

namespace laya{
#ifndef THIN_COMMON_WITHOUT_DOWNLOAD
	class Curl;
#endif
	struct _QueryBase;
	class __Buffer;
    /** 
     * @brief	只负责从网络下载资源。post数据。不管缓存。更新。
     *          同时做转换层的工作：替换扩展名解密
    */
	class JCDownloadMgr{    
    public:
        struct maskinfo {
            unsigned int key;
            int len;
        };
        //回调
        typedef std::function<int(unsigned int, unsigned int, float)> onProgressFunc;	//返回1表示终止下载
        //数据，localip，svip，curlret，httpret，header
        typedef std::function<void(JCBuffer& buff, const std::string& localip, 
            const std::string& svip, int curlret, int httpret,
            const std::string& httpresheader)> onEndFunc;
	
    public:
		static void defCompleteFunc(JCBuffer&, const std::string&, const std::string&,
            int, int, const std::string&);
		static int defProgressFunc(unsigned int, unsigned int,float);

		~JCDownloadMgr();

		static JCDownloadMgr* getInstance();
		static void delInstance();

        //初始化。p_nWorkThreadNum 表示开启几个下载线程。
		void init( int p_nWorkThreadNum );

		//设置操作超时
		void setOpt_optTimeout( int p_nTimeout);	
		//设置超时后的重试次数。第一个是操作超时后的重试次数，第二个是连接超时后的重试次数。
		void setOpt_tryNumOnTimeout(int p_nOptTry, int p_nConnTry){ m_nTryNumConn = p_nConnTry; m_nTryNumOpt=p_nOptTry;};
		void getOpt_tryNumOnTimeout(int& p_nOptTry, int& p_nConnTry){ p_nConnTry=m_nTryNumConn ; p_nOptTry= m_nTryNumOpt;};
		void setOpt_progCB(onProgressFunc p_ProgCb);
		void setOpt_completeCB(onEndFunc p_CompleteCb);

        /*
         * @brief 完整参数的下载请求
         * @prame p_nOptTimeout 下载超时。单位是秒， 要求>2, 如果为0，则使用缺省的。
         */
        void download(const char *p_pszUrl,int p_nPriority,
            onProgressFunc p_ProgCb, onEndFunc p_CompleteCb,
            const char* p_pPostData, int p_nPostLen,  //可以为0
            bool p_bOnlyHeader,                       //一般是false
            int p_nTimeout, int p_nConnTimeout,       //为0则缺省
            std::vector<std::string> p_vHeaders,      //size()==0则忽略
            const char* p_pLocalFile,                 //一边下载，一边保存，一般用在大文件下载，0则忽略
            bool p_bChkRemoteChange=false             //检查远端文件是否改变了，大文件用
            );

		void download(const char* p_pszURL);
		virtual void download(const char* p_pszURL,int p_nPriority, onProgressFunc p_ProgCb, 
            onEndFunc p_CompleteCb ,int p_nOptTimeout);
        virtual void download(const char* p_pszURL, int p_nPriority, onProgressFunc p_ProgCb,
            onEndFunc p_CompleteCb, int p_nOptTimeout,int p_nConnTimeout);
        virtual void download(const char* p_pszURL,int p_nPriority, onProgressFunc p_ProgCb,
            onEndFunc p_CompleteCb , int p_nOptTimeout,
			std::vector<std::string>& p_headers);

		/*
         * @brief 
		 *	    下载大文件，如果失败，回自动断点续传。
		 *	    中间过程会直接写文件，而普通的下载是下载到内存中。
		 * @pram p_pszLocal 本地文件位置，包括文件名，要求目录必须存在，且有写的权限。
		 */
		void downloadBigFile(const char* p_pszURL, const char* p_pszLocal, onProgressFunc p_ProgCb, onEndFunc p_CompleteCb, int p_nTryNum, int p_nOptTimeout);
		void getHeader(const char* p_pszUrl, onEndFunc p_CompleteCb, int p_nTryNum, int p_nOptTimeout);

		void postData(const char* p_pszURL,const char* p_Buffer, int p_nLength, onEndFunc p_completeCb);
		void postData(const char* p_pszURL,const char* p_Buffer, int p_nLength, onEndFunc p_completeCb,
			std::vector<std::string>& p_headers);

		//删除所有挂起的任务
		void clearAllAsyncTask();

		//结束当前的任务
		void stopCurTask();

        //设置代理
        void setProxyString(const char* pProxy);
        /*
         * @brief 替换某个路径。
         *  这个是为了解决有些cdn实在不好用而做的。如果路径中存在p_pstrPath（必须完全匹配）, 则用p_pszReplace替换。
         *  这样可以导致一个动态目录，从而规避cdn问题。
         *  注意替换后url的长度不要增加太多。与下面的tail结合不许比原始大小大512
         */
		void setFinalReplacePath(const char* p_pstrPath, const char* p_pszReplace);

        /*
         * @brief 设置下载的时候附加的东西。
         *  注意不要让url的长度增加太多。与上面的替换结合不许比原始大小大512
         * @param type 
         *  0表示不附加
         *	1随机数，形式为rnd=xxxx
         *	2外面设置的字符串
         * @param p_strTail 
         */
		void setDownloadTail(int type, const char* p_strTail);

		//根据替换和添加规则，得到一个新的url，内部会重新分配空间，所以需要外部delete
		char* getFinalUrl(const char* p_pszUrl);

		void resetFinalReplacePath(){ 
            m_strStubPath=""; m_strStubReplace="";
        }
		void resetDownloadTail(){ m_nDownloadTailType=0; m_strDownloadTail="";};

		//下载的时候修改文件的扩展名，以防止中间路途被修改。
		//由于没有进行多线程保护。这个必须要保证在程序刚开始的时候设置。
		void setDownloadReplaceExt(const char* p_pszOrigin, const char* p_pszNew );
		void resetDownloadReplaceExt();

		//凡是扩展名为p_pszExt的文件,用key来进行异或操作，操作长度为p_nLen
		void setDownloadUnmask(const char* p_pszExt, unsigned char p_nKey, int p_nLen );
		void resetDownloadUnmask();
		//如果不存在，则key为0
		maskinfo getMaskInfo(const char* p_pszExt );
		//如果buffer长度太小，则mask整个buffer
		void maskBuffer( maskinfo& p_mask, char* p_pBuff, int p_nLen );

		int getThreadNum(){ return m_nThreadNum; }

		void setCookieFile( const char* p_pszCookieFile );
		//优先级最低的给一个独立的线程
		void postToDownloadThread(std::function<int(void*, unsigned char **,size_t*)> task, int p_nPriority);
	
	protected:
        //这是一个单件，不允许直接构造
        JCDownloadMgr();
		int	__DownloadBigFile(void *p_pCurl, const char *p_pszUrl, const char* p_pszLocal, bool p_bHaveQuery, int p_nOptTimeout, int p_nTryNum);
		int	__DownloadHeader(void *p_pCurl, const char *p_pszUrl, unsigned char **p_ppBuff, size_t *p_piSize, bool p_bHaveQuery, int p_nOptTimeout, int p_nTryNum);

    public://不对外的
        void __WorkThread();
        //下载完数据的处理。
        bool	postDownload(const char* p_pszUrl, unsigned char*& p_pBuff, int& p_nLen);

        //返回错误代码。实际使用的是 Curl::ErrorCode
        //p_bHaveQuery 是表示是否已经有Query了，因为需要决定添加?rnd=xx还是&rnd=xx
        int	__Download(void *p_pCurl, const char *p_pszUrl, unsigned char **p_ppBuff, size_t *p_piSize, bool p_bHaveQuery);

        static bool	m_bCancelTask;
	public:
		char**	            m_pCurDownloadingUrl;	//正在下载的文件。调试用。如果某一个为0则表示正在空闲
		static int	        s_nNoResponseTimeout;	//如果下载中途没有数据了，则超过这么长时间之后就重来。
        static std::string  s_curlProxyString;
        static int          s_nConnTimeout;//超时的全局设置
        static int          s_nOptTimeout;  
        static bool         s_bEncodeURI;
    protected:
        short		m_nTryNumOpt;
        short		m_nTryNumConn;
        short		m_nThreadNum;
        JCThreadPool<_QueryBase *>	m_ThreadPool;
        std::recursive_mutex m_CancelMutex;
#ifdef WIN32
        std::atomic_uint32_t m_nStopNum;				//完成个数
#else
        std::atomic_uint m_nStopNum;				//完成个数
#endif
        std::string		m_strStubPath;
        std::string		m_strStubReplace;
        int				m_nDownloadTailType;
        std::string		m_strDownloadTail;
        std::vector<std::string> m_vExtReplace;	//所有需要替换的扩展名。两个一对，第一个是原始的，第二个是新的
        int				m_nTimeout;
        onProgressFunc	m_funcProgress;
        onEndFunc	    m_funcComplete;
        std::string		m_strCookieFile;
        typedef std::map<std::string, maskinfo> maskMap;
        maskMap			m_maskInfo;
	};

    std::string encodeURI(const char* url);
}
//------------------------------------------------------------------------------


#endif //__JCDownloadMgr_H__
