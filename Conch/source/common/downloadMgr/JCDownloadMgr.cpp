/**
@file			JCDownloadMgr.cpp
@brief			
@author			hugao
@version		1.0
@date			2016_5_11
*/

#include "../downloadMgr/JCDownloadMgr.h"
#include "../misc/JCThreadPool.h"
#include <ctime>
#ifndef THIN_COMMON_WITHOUT_DOWNLOAD
#include "../downloadMgr/JCCurlWrap.h"
#endif
#include "../util/Log.h"
#include <algorithm>
#include "../util/JCCommonMethod.h"
#include <thread>
#include "../util/JCLayaUrl.h"
#include <atomic>
#include <stdlib.h>
#define TIMEOUTTRYNUM 40
#define CONNECTERRTRYNUM 80
#define P_NOPOSTDATA nullptr
#define P_NOTIMEOUT 0
#define P_NOCONNTIMEOUT 0
#define P_PRIORITY_NORMAL 0
#define P_PRIORITY_LOW 1
#define P_NOLOCALFILE nullptr

#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>

namespace laya{
	typedef std::recursive_mutex _mutex_t;
	static std::uint32_t _l_ServerStop = 0;
	int JCDownloadMgr::s_nNoResponseTimeout = 15000;
    int JCDownloadMgr::s_nConnTimeout = 0;
    int JCDownloadMgr::s_nOptTimeout = 0;
    bool  JCDownloadMgr::s_bEncodeURI=true;
	bool JCDownloadMgr::m_bCancelTask = false;
    JCDownloadMgr* gDownloadMgr = NULL;
    int _OnProgress(unsigned int total, unsigned int now, float p_fSpeed, void* p_userData);
    std::string JCDownloadMgr::s_curlProxyString = "";
#ifndef THIN_COMMON_WITHOUT_DOWNLOAD
    class _QueryBase {
    public:
        virtual ~_QueryBase() {
            int a = 10;
        };
        //false则重试。自己内部做sleep
        virtual bool run(Curl* pCurl) { return true; };
    };

    class _QueryDownload:public _QueryBase {
    protected:
        const char*		mpPostData = nullptr;
        int				mnPostDataLen = 0;
    public:
        short           mnOptTimeout=0;
        short           mnConnTimeout = 0;
        unsigned int	mnTryNum = 0;
        bool            mbOnlyHeader = false; //只下载header
        bool            mbChkRemote = false;
        std::string                 mUrl;
        std::vector<std::string>	mHeaders;
        JCDownloadMgr::onProgressFunc	mOnProg;
        JCDownloadMgr::onEndFunc    mOnEnd;	//注意：传过来的指针不可以保存。
        std::string		            mstrLocalFile;		//直接写文件

        _QueryDownload(const char* pszUrl) {
            if(pszUrl)
                mUrl = pszUrl;
        }

        void setPostData(const char* pData, int nLen) {
            if (mpPostData)delete[] mpPostData;
            mpPostData = new char[nLen];
            memcpy((void*)mpPostData, pData, nLen);
            mnPostDataLen = nLen;
        }
        virtual bool run(Curl* pCurl) {
            if (JCDownloadMgr::m_bCancelTask) {
                return true;
            }
            pCurl->setProgressCB(_OnProgress, this);

            unsigned char *pContentBuff = nullptr;
            size_t iContentBuffSize = 0;
            bool bBigFile = mstrLocalFile.length() > 0;
#ifdef OHOS
            LOGI("Download [%{public}c%{public}c]:%{public}s", mbOnlyHeader ? 'H' : ' ', bBigFile ? 'B' : ' ', mUrl.c_str());
#else
            LOGI("Download [%c%c]:%s", mbOnlyHeader ? 'H' : ' ', bBigFile ? 'B' : ' ', mUrl.c_str());
#endif
            // 下载文件
            JCUrl url(mUrl.c_str());
            std::string encodeUrl = mUrl;// url.encode();
            bool bHasQuery = url.m_Query.length() > 0;
            char *pszUrlBuff = gDownloadMgr->getFinalUrl(encodeUrl.c_str());
            __Buffer* _pContentBuffer = NULL;

            pCurl->query(pszUrlBuff, &_pContentBuffer,
                mpPostData, mnPostDataLen,
                mbOnlyHeader,
                mnOptTimeout, mnConnTimeout,
                mHeaders,
                bBigFile ? mstrLocalFile.c_str() : nullptr,
                bBigFile);
            if (_pContentBuffer) {
                iContentBuffSize = _pContentBuffer->GetDataSize();
                pContentBuff = (unsigned char*)_pContentBuffer->SwapBuff(0, 0);
            }
            if(mpPostData)
                delete mpPostData;
            mpPostData = NULL;

            if (mOnEnd) {
                if (pCurl->m_nCurlRet != CURLE_OK) {
                    //curl 执行失败
                    static std::string nullstr;
                    JCBuffer jb;
                    mOnEnd(jb, pCurl->m_strLocalAddr, pCurl->m_strSvAddr, pCurl->m_nCurlRet, pCurl->m_nResponseCode, nullstr);
                }
                else {
                    //bool bHttpOK = pCurl->m_nResponseCode >= 200 && pCurl->m_nResponseCode < 300;
                    #ifdef OHOS
				    LOGI("Download end:%{public}d", pCurl->m_nResponseCode);
        			#else
                    LOGI("Download end:%d", pCurl->m_nResponseCode);
        			#endif
                    if (bBigFile || iContentBuffSize <= 0) {//大文件没有buffer
                        JCBuffer jb;
                        mOnEnd(jb, pCurl->m_strLocalAddr, pCurl->m_strSvAddr,
                            CURLE_OK, pCurl->m_nResponseCode,
                            pCurl->m_strResponseHead);
                    }
                    else {
                        if(pContentBuff && iContentBuffSize>0){
                            int sz = iContentBuffSize;
                            unsigned char* posthandleData = pContentBuff;
                            gDownloadMgr->postDownload(pszUrlBuff, posthandleData, sz);
                            iContentBuffSize = sz;
                            if (posthandleData != pContentBuff) {
                                delete[] pContentBuff;
                                pContentBuff = posthandleData;
                            }
                        }

                        JCBuffer buf((void*)pContentBuff, iContentBuffSize, false, true);
                        mOnEnd(buf, pCurl->m_strLocalAddr, pCurl->m_strSvAddr,
                            CURLE_OK, pCurl->m_nResponseCode,
                            pCurl->m_strResponseHead);
                    }
                }
            }
            delete[] pszUrlBuff;
            return true;
        }
    };

    int _OnProgress(unsigned int total, unsigned int now, float p_fSpeed, void* p_userData) {
        if (!gDownloadMgr)
            return 1;
        if (gDownloadMgr->m_bCancelTask)
            return 1;
        _QueryDownload* pQ = (_QueryDownload*)p_userData;
        if (pQ && pQ->mOnProg) {
            return pQ->mOnProg(total, now, p_fSpeed);
        }
        return 0;
    }

    class _QueryFunction :public _QueryBase {
    public:
        std::function<int(void*, unsigned char**, size_t*)> mTask;
        virtual bool run(Curl* pCurl) {
            unsigned char *pContentBuff = nullptr;
            size_t iContentBuffSize = 0;
            mTask(pCurl, &pContentBuff, &iContentBuffSize);
            //TODO 要回调么
            return true;
        }
    };
    //设置cookie file
    class _QuerySetCookieFile :public _QueryBase {
    public:
        //设置cookie文件。注意不一定什么时候执行，最好是在所有下载开始之前        
        std::string m_strSetCookieFile;
        _QuerySetCookieFile(const char* pszCF) {
            m_strSetCookieFile = pszCF;
        }
        virtual bool run(Curl* pCurl) {
            pCurl->setCookieFile(m_strSetCookieFile.c_str());	//请求设置cookie文件
            return true;
        }
    };
    //等待清空队列。
    class _QueryWaitEmpty :public _QueryBase {
    protected:
#ifdef WIN32
        std::atomic_uint32_t* mpNum = nullptr;
    public:
        _QueryWaitEmpty(std::atomic_uint32_t* pNum) {
            mpNum = pNum;
        }
#else
        std::atomic_uint* mpNum = nullptr;
    public:
        _QueryWaitEmpty(std::atomic_uint* pNum) {
            mpNum = pNum;
        }
#endif
        virtual bool run(Curl* pCurl) {
            if(mpNum)
                (*mpNum)--;	//请求停止
            return true;
        }
    };
#endif
    //设置代理
    class _QuerySetProxy :public _QueryBase {
    protected:
        std::string strProxy ;
    public:
        _QuerySetProxy(const char* pProxy) {
            if(pProxy)
                strProxy = pProxy;
        }
        virtual bool run(Curl* pCurl) {
            if (strProxy.length() > 0) {
                pCurl->setProxyString(strProxy.c_str());
            }
            return true;
        }
    };

	void JCDownloadMgr::defCompleteFunc(JCBuffer&,const std::string&, const std::string&,
        int,int,const std::string&){};
	int JCDownloadMgr::defProgressFunc(unsigned int, unsigned int,float){return 0;}

    JCDownloadMgr* JCDownloadMgr::getInstance(){
		if( gDownloadMgr == NULL){
			gDownloadMgr = new JCDownloadMgr();
			//gDownloadMgr->init(2);	//线程数目
		}
		return gDownloadMgr;
	}

	void JCDownloadMgr::delInstance(){
		if(gDownloadMgr){
			delete gDownloadMgr;
			gDownloadMgr = NULL;
		}
	}

    JCDownloadMgr::JCDownloadMgr(){
		//_l_PoolQuery = new JCThreadPoolSingle<_Query_t *>();
		m_nTryNumOpt = TIMEOUTTRYNUM;
		m_nTryNumConn = CONNECTERRTRYNUM;
		m_nStopNum = 0;
		m_nDownloadTailType = 0;    //缺省为不加随机数，防止瞬间回流造成服务器的负载太大。
		m_nThreadNum = 0;
		m_pCurDownloadingUrl=NULL;
		m_nTimeout = 0;
	}

    JCDownloadMgr::~JCDownloadMgr(){
		_l_ServerStop = 1;
		clearAllAsyncTask();
		//因为可能已经关掉了，没必要再跑一次
		if(!m_bCancelTask)
			stopCurTask();
		//if(m_pCurDownloadingUrl)
		//	delete [] m_pCurDownloadingUrl;
		/*
		_l_PoolQuery->Stop();

		if( _l_PoolQuery ){
			delete _l_PoolQuery ;
			_l_PoolQuery = NULL;
		}
		*/
	}

	void JCDownloadMgr::setOpt_optTimeout( int p_nTimeout){
		m_nTimeout = p_nTimeout;
	}
	void JCDownloadMgr::setOpt_progCB(onProgressFunc p_ProgCb){
		m_funcProgress = p_ProgCb;
	}
	void JCDownloadMgr::setOpt_completeCB(onEndFunc p_CompleteCb){
		m_funcComplete = p_CompleteCb;
	}

	void JCDownloadMgr::setCookieFile( const char* p_pszCookieFile ){
#ifndef THIN_COMMON_WITHOUT_DOWNLOAD
		m_strCookieFile = p_pszCookieFile;
		int num = m_ThreadPool.getThreadNum();
		for( int i=0; i<num; i++){
			_QuerySetCookieFile *pQuery = new _QuerySetCookieFile(p_pszCookieFile);
			m_ThreadPool.sendToThread(pQuery,i);
		}
#endif
	}

    void JCDownloadMgr::download(const char *p_pszURL, int p_nPriority,
        onProgressFunc p_ProgCb, onEndFunc p_CompleteCb,
        const char* p_pPostData, int p_nPostLen,  //可以为0
        bool p_bOnlyHeader,                       //一般是false
        int p_nTimeout, int p_nConnTimeout,       //为0则缺省
        std::vector<std::string> p_vHeaders,      //size()==0则忽略
        const char* p_pLocalFile,                 //一边下载，一边保存，一般用在大文件下载，0则忽略
        bool p_bChkRemoteChange             //检查远端文件是否改变了，大文件用
        ) {
#ifndef THIN_COMMON_WITHOUT_DOWNLOAD
        m_bCancelTask = false;
        if (0 != p_pszURL) {
            if (strlen(p_pszURL) <= 0) {
                LOGE("Error! downloadMgr::download url len=0");
                return;
            }
            static int nTh = 0;
            int nThNum = m_ThreadPool.getThreadNum();// _l_PoolQuery->GetSize();
            if (nThNum <= 0)
                return;
            _QueryDownload* pQuery = new _QueryDownload(p_pszURL);
            pQuery->mOnEnd = p_CompleteCb;
            pQuery->mOnProg = p_ProgCb;
            pQuery->mnOptTimeout = p_nTimeout>0?p_nTimeout:s_nOptTimeout;
            pQuery->mnConnTimeout = p_nConnTimeout > 0 ? p_nConnTimeout : s_nConnTimeout;
            pQuery->mHeaders = p_vHeaders;
            pQuery->mbOnlyHeader = p_bOnlyHeader;
            if(p_pPostData)
                pQuery->setPostData(p_pPostData, p_nPostLen);
            if (p_pLocalFile) {
                pQuery->mstrLocalFile = p_pLocalFile;
            }
            if (p_nPriority == 1 || nThNum == 1)
                m_ThreadPool.sendToThread(pQuery, 0);
            else {
                nTh %= (nThNum - 1);
                m_ThreadPool.sendToThread(pQuery, nTh + 1);
            }
            nTh++;
        }
#endif
    }

	void JCDownloadMgr::download(const char* p_pszURL){
#ifndef THIN_COMMON_WITHOUT_DOWNLOAD
        download(p_pszURL,P_PRIORITY_NORMAL, 
            defProgressFunc,defCompleteFunc,
            P_NOPOSTDATA,0,
            false,
            P_NOTIMEOUT,P_NOCONNTIMEOUT,
            Curl::NoHeader,
            P_NOLOCALFILE, false);
#endif
	}

	void JCDownloadMgr::download(const char* p_pszURL, int p_nPriority, onProgressFunc p_ProgCb, 
            onEndFunc p_CompleteCb , int p_nOptTimeout ){
#ifndef THIN_COMMON_WITHOUT_DOWNLOAD
        download(p_pszURL, p_nPriority,
            p_ProgCb, p_CompleteCb,
            P_NOPOSTDATA, 0,
            false,
            p_nOptTimeout, P_NOCONNTIMEOUT,
            Curl::NoHeader,
            P_NOLOCALFILE, false);
#endif
	}

    void JCDownloadMgr::download(const char* p_pszURL, int p_nPriority, onProgressFunc p_ProgCb,
        onEndFunc p_CompleteCb, int p_nOptTimeout, int p_nConnTimeout) {
#ifndef THIN_COMMON_WITHOUT_DOWNLOAD
        download(p_pszURL, p_nPriority,
            p_ProgCb, p_CompleteCb,
            P_NOPOSTDATA, 0,
            false,
            p_nOptTimeout, p_nConnTimeout,
            Curl::NoHeader,
            P_NOLOCALFILE, false);
#endif
    }
	void JCDownloadMgr::download(const char* p_pszURL, int p_nPriority, onProgressFunc p_ProgCb, 
            onEndFunc p_CompleteCb , int p_nOptTimeout,
		    std::vector<std::string>& p_headers){
        download(p_pszURL, p_nPriority,
            p_ProgCb, p_CompleteCb,
            P_NOPOSTDATA, 0,
            false,
            p_nOptTimeout, P_NOCONNTIMEOUT,
            p_headers,
            P_NOLOCALFILE, false);
	}

	void JCDownloadMgr::downloadBigFile(const char* p_pszURL, const char* p_pszLocal,
		    onProgressFunc p_ProgCb, onEndFunc p_CompleteCb, int p_nTryNum, int p_nOptTimeout) {
#ifndef THIN_COMMON_WITHOUT_DOWNLOAD
        download(p_pszURL, P_PRIORITY_NORMAL,
            p_ProgCb, p_CompleteCb,
            P_NOPOSTDATA, 0,
            false,
            p_nOptTimeout, P_NOCONNTIMEOUT,
            Curl::NoHeader,
            p_pszLocal, true);
#endif
	}

	void JCDownloadMgr::postToDownloadThread(std::function<int(void*, unsigned char **,size_t*)> task, int p_nPriority) {
#ifndef THIN_COMMON_WITHOUT_DOWNLOAD
        _QueryFunction *pQuery = new _QueryFunction();
        pQuery->mTask = task;
		static int nTh = 0;
		int nThNum= m_ThreadPool.getThreadNum();// _l_PoolQuery->GetSize();
		if (nThNum <= 0)
			return;
		if( p_nPriority==1 || nThNum==1)
			m_ThreadPool.sendToThread(pQuery, 0);
		else {
			nTh %= (nThNum - 1);
			m_ThreadPool.sendToThread(pQuery, nTh+1);
		}
		nTh++;
#endif
	}

	void JCDownloadMgr::getHeader(const char* p_pszUrl, onEndFunc p_CompleteCb, int p_nTryNum, int p_nOptTimeout) {
#ifndef THIN_COMMON_WITHOUT_DOWNLOAD
        download(p_pszUrl, P_PRIORITY_NORMAL,
            defProgressFunc, p_CompleteCb,
            P_NOPOSTDATA, 0,
            true,
            p_nOptTimeout, P_NOCONNTIMEOUT,
            Curl::NoHeader,
            P_NOLOCALFILE, false);
#endif
	}

	void JCDownloadMgr::postData(const char* p_pszURL,const char* p_Buffer, int p_nLength, 
        onEndFunc p_completeCb){
		std::vector<std::string> headers;
		postData(p_pszURL,p_Buffer,p_nLength,p_completeCb,headers);
	}
	void JCDownloadMgr::postData(const char* p_pszURL,const char* p_Buffer, int p_nLength, onEndFunc p_completeCb,
		std::vector<std::string>& p_headers){
#ifndef THIN_COMMON_WITHOUT_DOWNLOAD
		m_bCancelTask = false;
        _QueryDownload* pQuery = new _QueryDownload(p_pszURL);
		pQuery->mOnEnd = p_completeCb;
        pQuery->setPostData(p_Buffer, p_nLength);
		pQuery->mHeaders = p_headers;
		m_ThreadPool.sendToThread(pQuery,0);
#endif
	}

	void JCDownloadMgr::clearAllAsyncTask(){
		int num = m_ThreadPool.getThreadNum();
		for( int i=0; i<num; i++){
			//_l_PoolQuery->ClearDataOfThread(i);
			m_ThreadPool.ClearDataOfThread(i);
		}
	}

	void JCDownloadMgr::stopCurTask(){
#ifndef THIN_COMMON_WITHOUT_DOWNLOAD
		m_bCancelTask = true;
		//等待所有的线程完成
		int num = m_ThreadPool.getThreadNum();
		for( int i=0; i<num; i++){
            _QueryWaitEmpty *pQuery = new _QueryWaitEmpty(&m_nStopNum);
			if (m_ThreadPool.sendToThread(pQuery, i)) {
				m_nStopNum++;
			}
		}
		int left = m_nStopNum;
		//TODO 临时加一个超时，否则可能有线程问题导致退出卡死。
		auto st = tmGetCurms();
		while(left>0 && tmGetCurms()-st<3000){
			left = m_nStopNum;
		}
        #ifdef OHOS
		LOGI("stopCurTask end stopnum=%{public}d",(int)m_nStopNum);
        #else
		LOGI("stopCurTask end stopnum=%d",(int)m_nStopNum);
        #endif		
#endif
	}
	
    void JCDownloadMgr::setProxyString(const char* pProxy) {
        int nThNum = m_ThreadPool.getThreadNum();// _l_PoolQuery->GetSize();
        if (nThNum <= 0)
            return;
        for (int i = 0; i < nThNum; i++) {
            _QuerySetProxy *pQuery = new _QuerySetProxy(pProxy);
            m_ThreadPool.sendToThread(pQuery, i);
        }
    }

	void JCDownloadMgr::init( int p_nWorkThreadNum ){
#ifndef THIN_COMMON_WITHOUT_DOWNLOAD
        Curl::global_init();
#endif
		//等10ms是为了防止上一个线程还没有起来（假如存在的话）。
		//否则：这里设置了_l_ServerStop=true会被线程起来后_l_ServerStop=false冲掉。
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
		_l_ServerStop = true;//这个是为了能让之前的线程正确退出。
		m_nThreadNum = p_nWorkThreadNum;
		//m_pCurDownloadingUrl = new char* [p_nWorkThreadNum];
		//memset(m_pCurDownloadingUrl,0,p_nWorkThreadNum*sizeof(char*));
		m_ThreadPool.setThreadName("download thread");
		m_ThreadPool.init(p_nWorkThreadNum, std::bind(&JCDownloadMgr::__WorkThread, this));
		/*
		_l_PoolQuery->Stop(); //TODO 如果加上stop会导致死锁。
		for(int i=0;i<p_nWorkThreadNum;++i)
		{
			_l_PoolQuery->CreateThread( std::bind(&downloadMgr::__WorkThread, this) );
		}
		*/
	}

	inline void _addsplitchar(std::string& str){
		if( str.at(0)=='/'){}
		else if(str.at(0)=='\\') str.at(0)='/';
		else
			str=std::string("/")+str;
		if( str.at(str.length()-1)=='/'){}
		else if(str.at(str.length()-1)=='\\') str.at(str.length()-1)='/';
		else
			str+="/";
	}

	void JCDownloadMgr::setFinalReplacePath(const char* p_pszPath, const char* p_pszReplace){
		m_strStubPath = p_pszPath?p_pszPath:"";
		m_strStubReplace = p_pszReplace?p_pszReplace:"";
		if(m_strStubPath.length()>0){
			_addsplitchar(m_strStubPath);
			_addsplitchar(m_strStubReplace);
		}
        #ifdef OHOS
		LOGI("setFinalReplacePath:%{public}s,%{public}s", m_strStubPath.c_str(), m_strStubReplace.c_str());
        #else
		LOGI("setFinalReplacePath:%s,%s", m_strStubPath.c_str(), m_strStubReplace.c_str());
        #endif		
	}

	void JCDownloadMgr::setDownloadTail(int type, const char* p_strTail){
		if(type<0||type>2)
			type=1;
		m_nDownloadTailType = type;
		if( m_nDownloadTailType==2){
			if(p_strTail)
				m_strDownloadTail = p_strTail;
			else
				m_nDownloadTailType=1;
		}
	}

	void JCDownloadMgr::__WorkThread(){
#ifndef THIN_COMMON_WITHOUT_DOWNLOAD
		_l_ServerStop = false;
		_QueryBase *pQuery;
		#ifdef WIN32
		srand(timeGetTime());
		#else
		//TODO 随机数种子	在线程中访问全局随机数可能有问题
		#endif

		Curl _lCurl;
		if( !_lCurl.Init() ){
			printf("Curl init failed, thread exit\n");
			return;
		}

		for(;0==_l_ServerStop;){
			pQuery = 0;
			//现在的waitdata返回false不再表示要退出。
			//if( !m_ThreadPool.waitData(&pQuery ) )	
			//	break;
			//if( !_l_PoolQuery->WaitData( &pQuery ) )
			//	break;
			void* pThread = m_ThreadPool.getCurThread();
			int nID = m_ThreadPool.getCurThreadNo(pThread);
			if( !m_ThreadPool.waitData(pThread,&pQuery ) ){	
				continue;
			}

			if( 0 != pQuery ){
                while (! pQuery->run(&_lCurl)) {
                    LOGI("processQuery 再次尝试");
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
				delete pQuery;
			}
		}
		_lCurl.Release();
#endif
	}

	char* JCDownloadMgr::getFinalUrl(const char* p_pszUrl){
		size_t iSize = strlen(p_pszUrl);
		char *pszUrlBuff = new char[iSize+512];//这里大小会有问题么

		const char* pQuery = strchr(p_pszUrl,'?');
		bool bQuery = pQuery!=0;

		//扩展名
		std::string pathNoExt;  //没有扩展名，没有query的部分
		std::string ext;
		const char* pExtDot = NULL;
		const char* pExtEnd = NULL;
		int extreplaceSz = m_vExtReplace.size();
		if(extreplaceSz==0 && m_strStubPath.length()==0 ){
			//简单情况
			strcpy(pszUrlBuff,p_pszUrl);
		}else{
			//if(extreplaceSz)
			{
				int urllen = strlen(p_pszUrl);
				pExtEnd = pQuery?(pQuery-1):(p_pszUrl+urllen-1);
				char* pCurDt=(char*)pExtEnd;
				while(pCurDt>p_pszUrl){
					if(*pCurDt=='/'||*pCurDt==':'||*pCurDt=='\\')
						break;	//没有扩展名
					if(*pCurDt=='.'){
						pExtDot=pCurDt;
					}
					pCurDt--;
				}
				if( pExtDot==pExtEnd)
					pExtDot=NULL;
				if( pExtDot ){
					pathNoExt.append(p_pszUrl,(pExtDot-p_pszUrl));
					ext.append(pExtDot+1,(pExtEnd-pExtDot));
					for( int i=0; i<extreplaceSz/2; i++){
						if( m_vExtReplace[i*2]==ext){
							ext = m_vExtReplace[i*2+1];
							break;
						}
					}
                }
                else {
                    if (bQuery)
                        pathNoExt.append(p_pszUrl, (pQuery - p_pszUrl));
                    else
                        pathNoExt = p_pszUrl;
                }
			}

			const char* pPathNoExt = pathNoExt.c_str();
			if(m_strStubPath.length()>=3){
				const char* pPos = strstr(pPathNoExt,m_strStubPath.c_str());
				if(pPos){//&&(!bQuery||pPos<pQuery)){//不允许替换？后面的。
					char* pCur = pszUrlBuff;
					int l1 = pPos-pPathNoExt;
					memcpy(pCur,pPathNoExt,l1);	//拷贝替换路径之前的内容
					pCur+=l1;

					int l2 = m_strStubReplace.length();
					memcpy(pCur,m_strStubReplace.c_str(),l2);//加上替换路径
					pCur+=l2;

					strcpy(pCur,pPos+m_strStubPath.length());//剩下的其他内容
				}else{
					strcpy(pszUrlBuff,pPathNoExt);
				}
			}else
				strcpy(pszUrlBuff,pPathNoExt);

			//加上扩展名
			if(ext.length()){
				strcat(pszUrlBuff,".");
				strcat(pszUrlBuff,ext.c_str());
			}
			//加上其他的
			if(pQuery)
				strcat(pszUrlBuff,pQuery);
		}
    
		char strTail[512];
		switch(m_nDownloadTailType){
			case 1://随机数
				sprintf( strTail, "%crnd=%d", bQuery?'&':'?', rand());
				break;
			case 2://指定
				sprintf( strTail, "%c%s", bQuery?'&':'?', m_strDownloadTail.c_str());
				break;
			default://无
				strTail[0]=0;
				break;
		}
		strcat(pszUrlBuff,strTail);
		return pszUrlBuff;
	}

	int JCDownloadMgr::__Download( void *p_pCurl, const char *p_pszUrl, unsigned char **p_ppBuff, 
            size_t *p_piSize, bool p_bHaveQuery){
        /*
		Curl::ErrorCode ret=Curl::EC_Unknown;
		__Buffer *pResBuffer=NULL;
		Curl * pCurl = (Curl*)p_pCurl;
		*p_ppBuff = 0;
		*p_piSize = 0;

		char *pszUrlBuff = getFinalUrl(p_pszUrl);
	    LOGI("process:DownloadFile:%s",pszUrlBuff);
		int i=0;
		Curl::ErrorCode ecode=Curl::EC_Unknown;
		int trynum = m_nTryNumOpt;
		for(;i<trynum&&!m_bCancelTask;i++)	//这里会进行多次尝试
		{
			ecode = pCurl->query( pszUrlBuff, &pResBuffer, Curl::__Type_GET );
			if( ecode == Curl::EC_DownloadTimeout||ecode==Curl::EC_PartialFile){
				LOGW("download timeout. i=%d",i);
				int UNK=0;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}else if(ecode==Curl::EC_Success){
				*p_piSize = pResBuffer->GetDataSize();
				*p_ppBuff = (unsigned char *)pResBuffer->SwapBuff( 0, 0 );
				if( *p_piSize>0 && (void*)*p_ppBuff ){
					int bufsz = (int)*p_piSize;
					unsigned char* pBuff = *p_ppBuff;
					postDownload(pszUrlBuff, pBuff, bufsz);
					*p_piSize = bufsz;
					if( *p_ppBuff != pBuff){
						delete [] *p_ppBuff;
						*p_ppBuff = pBuff;
					}
				}
				break;
			}else if(ecode==Curl::EC_ConnectError){
				if( i==0){trynum=m_nTryNumConn; }
				int UNK=0;
				LOGW("connect timeout ! i=%d,trynum=%d,m_bCanceltask=%d", i, trynum, m_bCancelTask);
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}else if(ecode==Curl::EC_404){
				LOGE("download err:404");
				*p_ppBuff=0;
				*p_piSize=0;
				break;
			}else if(ecode==Curl::EC_Cancel){
				*p_ppBuff=0;
				*p_piSize=0;
				break;
			}
			else{
				//其他错误
				LOGE("download unk error!");
				break;
			}
		}
		delete [] pszUrlBuff;
		return ecode;
        */
        return 0;
	}

	int	JCDownloadMgr::__DownloadHeader(void *p_pCurl, const char *p_pszUrl, unsigned char **p_ppBuff, size_t *p_piSize,bool p_bHaveQuery, int p_nOptTimeout, int p_nTryNum) {
        /*  TODO 打开
		Curl * pCurl = (Curl*)p_pCurl;
		*p_ppBuff = 0;
		*p_piSize = 0;
		__Buffer *pResBuffer = NULL;
		char *pszUrlBuff = getFinalUrl(p_pszUrl);
		LOGI("流程：DownloadHeader:%s", pszUrlBuff);
		//JCErrorHandleInterface* pEH = getErrorHandler();
		int i = 0;
		Curl::ErrorCode ecode = Curl::EC_Unknown;
		int trynum = p_nTryNum;
		for (; i<trynum&&!m_bCancelTask; i++) {	//这里会进行多次尝试
			ecode = pCurl->downloadHeader(pszUrlBuff, &pResBuffer, p_nOptTimeout);
			if (ecode == Curl::EC_DownloadTimeout || ecode == Curl::EC_PartialFile) {
				LOGW("download timeout. i=%d", i);
				int UNK = 0;
				//if (pEH) pEH->networkSlowly((float)UNK,pCurl->m_nResponseCode);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
			else if (ecode == Curl::EC_Success) {
				*p_piSize = pResBuffer->GetDataSize();
				*p_ppBuff = (unsigned char *)pResBuffer->SwapBuff(0, 0);
				break;
			}
			else if (ecode == Curl::EC_ConnectError) {
				if (i == 0) { trynum = m_nTryNumConn; }
				int UNK = 0;
				LOGW("connect timeout ! i=%d,trynum=%d,m_bCanceltask=%d", i, trynum, m_bCancelTask);
				//if (pEH) pEH->networkSlowly((float)UNK,pCurl->m_nResponseCode);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
			else if (ecode == Curl::EC_404) {
				LOGE("download err:404");
				break;
			}
			else if (ecode == Curl::EC_Cancel) {
				break;
			}
			else {
				//其他错误
				LOGE("download unk error!");
				break;
			}
		}
		delete[] pszUrlBuff;
		return ecode;
        */
        return 0;
	}

	int JCDownloadMgr::__DownloadBigFile(void *p_pCurl, const char *p_pszUrl, 
        const char* p_pszLocal, bool p_bHaveQuery, int p_nOptTimeout, int p_nTryNum){
        /*
		Curl * pCurl = (Curl*)p_pCurl;
		char *pszUrlBuff = getFinalUrl(p_pszUrl);

	    LOGI("流程：DownloadFile:%s",pszUrlBuff);
		int i=0;
		Curl::ErrorCode ecode=Curl::EC_Unknown;
		int trynum = p_nTryNum;
		for(;i<trynum&&!m_bCancelTask;i++){	//这里会进行多次尝试
			ecode = pCurl->downloadBigFile(pszUrlBuff, p_pszLocal, p_nOptTimeout );
			if( ecode == Curl::EC_DownloadTimeout||ecode==Curl::EC_PartialFile){
				LOGW("download timeout. i=%d",i);
				int UNK=0;
				//JCErrorHandleInterface* pEH = getErrorHandler();
				//if (pEH) pEH->networkSlowly((float)UNK,pCurl->m_nResponseCode); 
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}else if(ecode==Curl::EC_Success){
				break;
			}else if(ecode==Curl::EC_ConnectError){
				if( i==0){trynum=m_nTryNumConn; }
				int UNK=0;
				LOGW("connect timeout ! i=%d,trynum=%d,m_bCanceltask=%d", i, trynum, m_bCancelTask);
				//JCErrorHandleInterface* pEH = getErrorHandler();
				//if (pEH) 
				//	pEH->networkSlowly((float)UNK, pCurl->m_nResponseCode);
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}else if(ecode==Curl::EC_404){
				LOGE("download err:404");
				break;
			}else if(ecode==Curl::EC_Cancel){
				break;
			}
			else{
				//其他错误
				LOGE("download unk error!");
				break;
			}
		}
		delete [] pszUrlBuff;
		return ecode;
        */
        return 0;
	}

	bool	JCDownloadMgr::postDownload(const char* p_pszUrl, unsigned char*& p_pBuff, int& p_nLen ){
		if( !p_pszUrl)
			return true;
		std::string ext = getLowercaseExtOfUrl(p_pszUrl);

		maskinfo mi = getMaskInfo(ext.c_str());
		if(mi.key)
			maskBuffer(mi, (char*)p_pBuff, p_nLen );
		return true;
	}

	void JCDownloadMgr::setDownloadReplaceExt(const char* p_pszOrigin, const char* p_pszNew ){
		if(p_pszOrigin==NULL || p_pszNew==NULL)
			return;
		for(int i=0,sz=m_vExtReplace.size(); i<sz/2; i++){
			if(m_vExtReplace[i*2]==p_pszOrigin){
				return;
			}
		}
		m_vExtReplace.push_back(p_pszOrigin);
		m_vExtReplace.push_back(p_pszNew);
	}

	void JCDownloadMgr::resetDownloadReplaceExt(){
		m_vExtReplace.clear();
	}

	void JCDownloadMgr::setDownloadUnmask(const char* p_pszExt, unsigned char p_nKey, int p_nLen ){
		if(!p_pszExt )
			return;
		std::string sExt = p_pszExt;
		std::transform(sExt.begin(), sExt.end(),sExt.begin(),::tolower);
		maskMap::iterator it = m_maskInfo.find(sExt);
		if(it==m_maskInfo.end()){
			if(p_nKey!=0){
				maskinfo mi={(unsigned int)p_nKey, p_nLen};
				m_maskInfo[sExt]=mi;
			}
		}else{
			if(p_nKey==0){
				m_maskInfo.erase(it);
			}else{
				maskinfo& info = (*it).second;
				info.key = p_nKey;
				info.len = p_nLen;
			}
		}
	}

	void JCDownloadMgr::resetDownloadUnmask(){
		m_maskInfo.clear();
	}

    JCDownloadMgr::maskinfo JCDownloadMgr::getMaskInfo(const char* p_pszExt ){
		maskinfo mi={0,0};
		if(!p_pszExt )
			return mi;
		std::string sExt = p_pszExt;
		std::transform(sExt.begin(), sExt.end(),sExt.begin(),::tolower);
		maskMap::iterator it = m_maskInfo.find(sExt);
		if(it==m_maskInfo.end()){
			return mi;
		}else{
			return (*it).second;
		}
	}

	void JCDownloadMgr::maskBuffer(JCDownloadMgr::maskinfo& p_mask, char* p_pBuff, int p_nLen ){
		if(p_mask.key==0)
			return;
		int l = p_mask.len>p_nLen?p_nLen:p_mask.len;
		unsigned char key = p_mask.key&0xff;
		for( int i=0; i<l; i++){
			p_pBuff[i]^=key;
		}
	}


    std::string encodeURI(const char* value) {
        if (!value)
            return "";
        std::ostringstream escaped;
        escaped.fill('0');
        escaped << std::hex;
        unsigned char* pc = (unsigned char*)value;
        unsigned char c = *pc;
        bool afterQuery = false;    //query中的'也要处理.  query中的 |^ 不处理
        while (c=*pc) {
            c = *pc;
            //标准是没有%的，但是chrome并不处理%，所以这里也不处理
            if (isalnum(c) || c ==';' ||c=='/' || c =='?' || c == ':' || c == '@' || c == '&' || c == '=' || c == '+' ||
                c == '$' || c == ',' || c == '-' || c == '_' || c == '.' || c == '!' || c == '~' || c == '*' || 
                (!afterQuery && c == '\'' )|| c == '(' || c == ')' || c == '[' || c == ']' ||c=='%' ||
                (afterQuery && c=='|') || 
                (afterQuery && c=='^') ) {
                if (!afterQuery&&c == '?')
                    afterQuery = true;
                escaped << c;
                pc++;
            }
            else {
                escaped << std::uppercase;
                escaped << '%' << std::setw(2) << int((unsigned char)c);
                escaped << std::nouppercase;
                pc++;
            }
        }
        return escaped.str();
    }
}
