/**
@file			JCCurlWrap.cpp
@brief			
@author			hugao
@version		1.0
@date			2016_5_11
*/

#include "../downloadMgr/JCCurlWrap.h"
#include "../misc/JCGetClockExact.h"
#include "../util/Log.h"
#include "../util/JCCommonMethod.h"
#include "../downloadMgr/JCDownloadMgr.h"
#include "../util/JCLayaUrl.h"
#include <cmath>
#ifdef WIN32
#ifdef _DEBUG
#pragma comment(lib,"libcurld.lib")
#else
#pragma comment(lib,"libcurl.lib")
#endif

#pragma comment(lib,"WS2_32.lib")
#pragma comment(lib,"wldap32.lib")
#endif

/*
	Curl 多线程应用：
	1. curl_global_init 函数不是线程安全的，所以必须首先在主线程中调用，避免多线程 curl_easy_init
	   引起调用 curl_global_init 产生重入
    2. 必须 curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L) 禁用掉 alarm 的使用，再多线程中 alarm + siglongjmp
	   会产生莫名奇妙的崩溃
    3. curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1); 避免连接重用导致可能的 CLOSE_WAIT 连接过多造成效率问题。
 */

#define DEF_OPTTIMEOUT 1800
#define P_NOPOSTDATA nullptr
#define P_NOTIMEOUT 0
#define P_NOCONNTIMEOUT 0
#define P_PRIORITY_NORMAL 0
#define P_PRIORITY_LOW 1
#define P_NOLOCALFILE nullptr

namespace laya{
	bool	Curl::s_bUseCurlCookie = true;
    std::vector<std::string> Curl::NoHeader;

    static size_t save_header(void *ptr, size_t size, size_t nmemb, void *data) {
        std::string* pRecv = (std::string*)data;
        int sz = size*nmemb;
        if (pRecv) {
            (*pRecv).append((const char*)ptr, sz);
        }
        return (size_t)sz;
    }

	Curl::Curl(){
		m_pCurlHandle = 0;
		m_nOptTimeout = 0;
		m_tmTaskBegin = 0;
		m_tmLastProg = 0;
		m_tmLastNotify = 0;
		m_bStopAndRetry = false;
		m_tmLastHasData = 0;
		m_nResponseCode = 0;
	}

	Curl::~Curl()
	{
		Release();
	}

    void Curl::global_init() {

    }

	bool Curl::Init( ){
		if( 0 == m_pCurlHandle )
		{
			m_pCurlHandle = curl_easy_init();
			if( 0 == m_pCurlHandle )
				return false;
		}
        if (JCDownloadMgr::s_curlProxyString.length() > 0) {
            CURLcode ret = curl_easy_setopt(m_pCurlHandle, CURLOPT_PROXY, JCDownloadMgr::s_curlProxyString.c_str());
            if (ret!= CURLE_OK) {
                LOGE("setcurlproxy error");
            }
        }
		return true;
	}

	void Curl::Release()
	{
		if( 0 != m_pCurlHandle )
		{
			curl_easy_cleanup( m_pCurlHandle );
			m_pCurlHandle = 0;
		}
	}

	bool Curl::_Prepare(){
		if( 0 == m_pCurlHandle ){
			m_pCurlHandle = curl_easy_init();
			if( 0 == m_pCurlHandle )
				return false;
		}

        //设置缺省值。
        curl_easy_setopt(m_pCurlHandle, CURLOPT_NOSIGNAL, 1L);
        curl_easy_setopt(m_pCurlHandle, CURLOPT_ACCEPT_ENCODING, "");
        curl_easy_setopt(m_pCurlHandle, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(m_pCurlHandle, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(m_pCurlHandle, CURLOPT_FOLLOWLOCATION, 1L);

        //header是必须保存的
        curl_easy_setopt(m_pCurlHandle, CURLOPT_HEADERFUNCTION, save_header);
        curl_easy_setopt(m_pCurlHandle, CURLOPT_WRITEHEADER, &m_strResponseHead);

        //缺省打开进度回调
        curl_easy_setopt(m_pCurlHandle, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(m_pCurlHandle, CURLOPT_XFERINFOFUNCTION, Curl::_ProgressCallback);
        curl_easy_setopt(m_pCurlHandle, CURLOPT_XFERINFODATA, this);

		return true;
	}

	int Curl::_WriteCallback( void *p_pDataBuffer, size_t p_size, size_t p_nmemb, void *p_pUserData ){
		Curl *pCurl= (Curl*)p_pUserData;
		if( 0 == pCurl )
			return 0;

		__Buffer *pBuffer = &pCurl->m_Buffer;

		size_t iSize = p_size*p_nmemb;
		pBuffer->AddData( p_pDataBuffer, iSize );
		return iSize;
	}

	int Curl::_ProgressCallback( void *clientp, curl_off_t dltotal, curl_off_t dlnow, 
        curl_off_t ultotal, curl_off_t ulnow ){
		//终止当前下载
		if (JCDownloadMgr::m_bCancelTask)
			return 1;

		Curl *pCurl = (Curl*)clientp;
		if( 0 == pCurl)
			return 0;//没有数据也没事。
		//std::uint32_t ulCur = GetClockExact::getInstance()->GetTimeMs();
		double curtm = laya::tmGetCurms();
		int iRet = 0;

		if( 0 != pCurl ){
            int dt = (int)(curtm - pCurl->m_tmTaskBegin);
            //float speed = ((nWriteSz-pCurl->m_dLastDownedSize)/1024.0f);
            float speed = dlnow / 1.024f / dt;

			//因为dlnow可能一直为0，所以采用buffer实际接收到的数据为准
			//LOGE("%f  t:%f,n:%f,curlen:%d",(dlnow+ pContext->nLocalLen)/(dltotal+1+ pContext->nLocalLen),(float)dltotal,(float)dlnow,pContext->nLocalLen);
			int nWriteSz = pCurl->m_Buffer.GetDataSize();
			nWriteSz = nWriteSz > dlnow?nWriteSz :(int) dlnow;
            if (nWriteSz <= 0)
                return 0;

			if( pCurl->m_dLastDownedSize==0 )
				pCurl->m_dLastDownedSize = nWriteSz;

			bool bNeedProg = false;
			if( fabsf((pCurl->m_dDownedSize-nWriteSz))>1){
				pCurl->m_tmLastHasData = curtm;
				pCurl->m_dDownedSize = nWriteSz;
				bNeedProg = true;
            }
            else {
                //LOGE("nodata:%d", (int)(curtm - pCurl->m_tmLastHasData));
            }
			//如果超过10秒还没有任何反应，则认为断线了，需要重来。
			if( laya::JCDownloadMgr::s_nNoResponseTimeout>0 && nWriteSz>0 &&
					curtm-pCurl->m_tmLastHasData>laya::JCDownloadMgr::s_nNoResponseTimeout ){
				LOGE("no received data over %d second,retry",laya::JCDownloadMgr::s_nNoResponseTimeout);
				pCurl->m_bStopAndRetry=true;
				return 1;	//取消
			}
			//与大小相关的超时。这个每隔2秒提醒一次
			int downloadtm = (int)((curtm-pCurl->m_tmTaskBegin)/1000);
			int maxtm = (int)(dltotal/(20*1024));//按照20k的速度下载所需要的时间。
			maxtm = maxtm<10?10:maxtm;
			//LOGE("dltotal=%d,downloadtm=%d,slowtm=%d",(int)dltotal, downloadtm, maxtm);
			if((dltotal>0||nWriteSz>0) && downloadtm >maxtm  && (curtm-pCurl->m_tmLastNotify)>2000 ){//按照每秒10k的速度计算的话，就是网络慢
				pCurl->m_tmLastNotify = curtm;
                //TODO 超时提醒
			}

			if( bNeedProg || curtm-pCurl->m_tmLastProg>2000){
				pCurl->m_dLastDownedSize= nWriteSz;
				//如果dlnow=0，可能有问题，例如设置续传的时候如果值不对，失败了，还会调用这个函数，但是，
				//total还是从0算，now 为0， 为了避免这种情况下的计算错误，直接忽略now为0的情况。
				if(dltotal>0 && dlnow>0 && pCurl->m_pExtOnProg){
					return pCurl->m_pExtOnProg((unsigned int)dltotal+ pCurl->m_nLocalFileLen, 
                        (unsigned int)dlnow+pCurl->m_nLocalFileLen, speed, pCurl->m_pExtOnProgData);
				}
			}
			/*
			//如果已经3秒了还是0%
			if( 0.0 == dlnow ){
				if( ulCur - pCurl->m_ulTaskBegin > 3*1000 )
				{
					iRet = 1;
				}
			}
			else if( dlnow > pCurl->m_dDownedSize )
			{
				pCurl->m_dDownedSize = dlnow;
				pCurl->m_ulTaskBegin = ulCur;
			}
			*/
		}
		return iRet;
	}

	bool Curl::checkResult(const char *p_pszUrl) {
		int opttimeout = m_nOptTimeout == 0 ? DEF_OPTTIMEOUT : m_nOptTimeout;
        switch (m_nCurlRet) {
        case CURLE_OPERATION_TIMEDOUT:	// 操作超时，是不是应该重新提交？
        {
            auto curtm = laya::tmGetCurms();
            int dt = (int)(curtm - m_tmTaskBegin);
            LOGW("download error: timeout, dt=%d,opttimeout=%d", dt / 1000, (opttimeout - 2));
        }
        break;
        case CURLE_PARTIAL_FILE: //可能是服务器关闭当前tcp连接了。
            LOGW("download error:PARTIAL_FILE");
            break;
        case CURLE_COULDNT_CONNECT: //无法连接到服务器，可能服务器没开。
            LOGW("download error: can't connect the server:%s", p_pszUrl);
            break;
        case CURLE_GOT_NOTHING:
            LOGW("download error: the server has nothing responce %s",p_pszUrl);
            break;
        case CURLE_ABORTED_BY_CALLBACK:
            if (m_bStopAndRetry) {
                //TODO 重新尝试
                m_bStopAndRetry = false;
            }
            break;
        case CURLE_OK:break;
        default:
            LOGW("curl_easy_perform failed, code=%d\nsrc=%s", m_nCurlRet, p_pszUrl ? p_pszUrl : "");
            break;
        }
        switch (m_nCurlRet){
		case CURLE_OPERATION_TIMEDOUT:	// 操作超时，是不是应该重新提交？
		case CURLE_PARTIAL_FILE: //可能是服务器关闭当前tcp连接了。
		case CURLE_GOT_NOTHING:
		case CURLE_ABORTED_BY_CALLBACK:
        case CURLE_OK:
        {
            bool httpok = false;
            CURLcode iCode = curl_easy_getinfo(m_pCurlHandle, CURLINFO_RESPONSE_CODE, &m_nResponseCode);
            if (CURLE_OK == iCode) {
                if (m_nResponseCode >= 200 && m_nResponseCode<300) {
                    //OK
                    httpok = true;
                }
            }
            char* pSvIP = NULL;
            char* pLocIP = NULL;
            curl_easy_getinfo(m_pCurlHandle, CURLINFO_PRIMARY_IP, &pSvIP);
            curl_easy_getinfo(m_pCurlHandle, CURLINFO_LOCAL_IP, &pLocIP);
            m_strSvAddr = pSvIP ? pSvIP : "";
            m_strLocalAddr = pLocIP ? pLocIP : "";

            if (Curl::s_bUseCurlCookie && m_nCurlRet== CURLE_OK) {
                curl_slist* plistdata = NULL;
                if (CURLE_OK == curl_easy_getinfo(m_pCurlHandle, CURLINFO_COOKIELIST, &plistdata)) {
                    if (plistdata) {
                        curl_easy_setopt(m_pCurlHandle, CURLOPT_COOKIELIST, "FLUSH");
                        curl_slist_free_all(plistdata);
                    }
                }
            }
            return httpok;
        }
            break;
		default:break;
		}

        return false;
	}

    Curl& Curl::begin() {
        //每次都要调用的
        if (s_bUseCurlCookie) {
            //这个必须每次都设置，因为curl内部有个data->change.cookielist
            curl_easy_setopt(m_pCurlHandle, CURLOPT_COOKIELIST, "SESS");
            curl_easy_setopt(m_pCurlHandle, CURLOPT_COOKIEFILE, m_strCookieFile.c_str());
        }
        m_nCurlRet = CURLE_FAILED_INIT;
        m_nLocalFileLen = 0;
        //这个必须在begin做，不能在end做。
        m_strResponseHead.clear();  
        m_Buffer.ClearData();
        m_tmTaskBegin = laya::tmGetCurms();
        m_tmLastHasData = m_tmTaskBegin;
        m_dDownedSize = 0.0;
        m_dLastDownedSize = 0.0;
        return *this;
    }

    Curl& Curl::end() {
        if (m_pslist)
            curl_slist_free_all(m_pslist);
        m_pslist = nullptr;
        return *this;
    }

    Curl& Curl::set_Url(const char* pUrl) {
        //JCUrl url(pUrl);
        //std::string encodeUrl = url.encode();
        if (JCDownloadMgr::s_bEncodeURI) {
            std::string uri = encodeURI(pUrl);
            curl_easy_setopt(m_pCurlHandle, CURLOPT_URL, uri.c_str());
        }
        else
            curl_easy_setopt(m_pCurlHandle, CURLOPT_URL, pUrl);
        return *this;
    }

    Curl& Curl::set_CookieList(const char* pC) {
        curl_easy_setopt(m_pCurlHandle, CURLOPT_COOKIELIST, pC);
        return *this;
    }

    Curl& Curl::set_CookieFile(const char* pFile) {
        curl_easy_setopt(m_pCurlHandle, CURLOPT_COOKIEFILE, pFile);
        return *this;
    }

    Curl& Curl::set_GET() {
        curl_easy_setopt(m_pCurlHandle, CURLOPT_HTTPGET, 1L);
        return *this;
    }

    Curl& Curl::set_POST() {
        return *this;
    }

    void Curl::ApplyHeaders() {
        if (m_pslist)
            curl_slist_free_all(m_pslist);
        m_pslist = nullptr;

        int sz = m_headers.size();
        if (sz <= 0) {
            curl_easy_setopt(m_pCurlHandle, CURLOPT_HTTPHEADER, nullptr);
            return ;
        }
        for (int i = 0; i<sz; i++) {
            m_pslist = curl_slist_append(m_pslist, m_headers[i].c_str());
        }
        //CURLcode ret = curl_easy_setopt(m_pCurlHandle, CURLOPT_HTTPHEADER, slist);
        //curl_slist_free_all(slist);
        m_headers.clear();
        curl_easy_setopt(m_pCurlHandle, CURLOPT_HTTPHEADER, m_pslist);
    }

    Curl& Curl::set_FollowLocation(long v) {
        curl_easy_setopt(m_pCurlHandle, CURLOPT_FOLLOWLOCATION, v);
        return *this;
    }

    Curl& Curl::set_Timeout(long v) {
        m_nOptTimeout = v;
        curl_easy_setopt(m_pCurlHandle, CURLOPT_TIMEOUT, v);
        return *this;
    }
    Curl& Curl::set_ConnectTimeout(long v) {
        curl_easy_setopt(m_pCurlHandle, CURLOPT_CONNECTTIMEOUT, v);
        return *this;
    }

    Curl& Curl::set_OnData(Curl::WRITECALLBACK func, void* pUserData) {
        curl_easy_setopt(m_pCurlHandle, CURLOPT_WRITEFUNCTION, func);
        curl_easy_setopt(m_pCurlHandle, CURLOPT_WRITEDATA, pUserData);
        return *this;
    }

    Curl& Curl::set_EnableProg(bool b) {
        curl_easy_setopt(m_pCurlHandle, CURLOPT_NOPROGRESS, b?0:1L);
        return *this;
    }

    Curl& Curl::set_PostData(const char* pPostData, int nPostLen) {
        curl_easy_setopt(m_pCurlHandle, CURLOPT_POSTFIELDS, pPostData);
        curl_easy_setopt(m_pCurlHandle, CURLOPT_POSTFIELDSIZE, nPostLen);
        curl_easy_setopt(m_pCurlHandle, CURLOPT_POST, 1L);
        return *this;
    }

    static int downLoadPackage(void *ptr, size_t size, size_t nmemb, void *userdata) {
        FILE *fp = (FILE*)userdata;
        size_t written = fwrite(ptr, size, nmemb, fp);
        return written;
    }
    static size_t NoWrite(void *ptr, size_t size, size_t nmemb, void *userdata) {
        return size*nmemb;
    }
    long GetLocalFileLenth(const char* fileName) {
        FILE* fp = fopen(fileName, "rb");
        if (fp != NULL) {
            fseek(fp, 0, SEEK_END);
            long len = ftell(fp);
            fclose(fp);
            return len;
        }
        return 0;
    }
    std::string getHeadInfo(std::string& head, const char* InfoKey) {
        std::string ret;
        int st = head.find(InfoKey);
        if (st <= 0)return ret;
        st += strlen(InfoKey);
        int ed = head.find("\r\n", st);
        if (ed > st) {
            ret.append(head.c_str() + st, ed - st);
        }
        return ret;
    }
    bool getRemoteFileInfo(CURL *m_pCurlHandle, const char* url, unsigned int& len, std::string& lastModified, std::string& etag) {
        bool retval = false;
        double dlen = 0.0;
        JCUrl _url(url);
        std::string encodeUrl = JCDownloadMgr::s_bEncodeURI ? encodeURI(url) : url;// _url.encode();

        len = 0;
        lastModified = "";
        etag = "";
        std::string headRet;

        curl_easy_setopt(m_pCurlHandle, CURLOPT_URL, encodeUrl.c_str());
        curl_easy_setopt(m_pCurlHandle, CURLOPT_HEADER, 1);    //只要求header头
        curl_easy_setopt(m_pCurlHandle, CURLOPT_NOBODY, 1);    //不需求body
        curl_easy_setopt(m_pCurlHandle, CURLOPT_WRITEHEADER, &headRet);
        curl_easy_setopt(m_pCurlHandle, CURLOPT_HEADERFUNCTION, save_header);
        curl_easy_setopt(m_pCurlHandle, CURLOPT_WRITEFUNCTION, NoWrite);
        curl_easy_setopt(m_pCurlHandle, CURLOPT_WRITEDATA, 0);
        curl_easy_setopt(m_pCurlHandle, CURLOPT_SSL_VERIFYPEER, 0L);//注意与Query的相互影响
        curl_easy_setopt(m_pCurlHandle, CURLOPT_SSL_VERIFYHOST, 0L);

        //不要破坏下载进度
        //curl_easy_setopt(m_pCurlHandle, CURLOPT_PROGRESSDATA, 0);
        //curl_easy_setopt(m_pCurlHandle, CURLOPT_NOPROGRESS, 0);
        //curl_easy_setopt(m_pCurlHandle, CURLOPT_PROGRESSFUNCTION, Curl::_ProgressCallback);

        CURLcode ret = curl_easy_perform(m_pCurlHandle);
        if (ret == CURLE_OK) {
            if (CURLE_OK == curl_easy_getinfo(m_pCurlHandle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &dlen)) {
                len = (unsigned int)dlen;
            }
            else {
                LOGW("curl_easy_getinfo failed!\n");
            }
            long filetime = 0;
            if (curl_easy_getinfo(m_pCurlHandle, CURLINFO_FILETIME, &filetime) == CURLE_OK) {

            }
            if (filetime <= 0) {
                lastModified = getHeadInfo(headRet, "Last-Modified:");
            }
            etag = getHeadInfo(headRet, "ETag:");
            //LOGE("%s%s", lastModified.c_str(), etag.c_str());
            retval = true;
        }
        else {
            retval = false;
        }
        //reset
        curl_easy_setopt(m_pCurlHandle, CURLOPT_HEADER, 0);
        curl_easy_setopt(m_pCurlHandle, CURLOPT_NOBODY, 0);
        curl_easy_setopt(m_pCurlHandle, CURLOPT_WRITEHEADER, 0);
        curl_easy_setopt(m_pCurlHandle, CURLOPT_HEADERFUNCTION, 0);
        return retval;
    }

    /*
     * 如果只是头的话，p_ppResBuff 也是头的内容。
    */
    void Curl::query(const char *p_pszUrl, __Buffer **p_ppResBuff,
            const char* p_pPostData, int p_nPostLen,  //可以为0
            bool p_bOnlyHeader,                       //一般是false
            int p_nTimeout, int p_nConnTimeout,       //为0则缺省
            const std::vector<std::string>& p_vHeaders,      //size()==0则忽略
            const char* p_pLocalFile,                 //一边下载，一边保存，一般用在大文件下载，0则忽略
            bool p_bChkRemoteChange                   //检查远端文件是否改变了，大文件用
            ){
        if(p_ppResBuff)
            *p_ppResBuff = 0;
        m_nResponseCode = 0;
        FILE* fp = nullptr;
        do {
            if (!_Prepare())
                break;
            begin();
            //如果要保存到本地的处理
            if (p_pLocalFile) {
                m_nLocalFileLen = GetLocalFileLenth(p_pLocalFile);
                if (p_bChkRemoteChange) {
                    //要检查远程是否改变了
                    unsigned int remoteLen=0;
                    std::string lastModified, etag;
                    //注意：这个会改变很多状态，所以需要在最前面执行。
                    bool br = getRemoteFileInfo(m_pCurlHandle, p_pszUrl, remoteLen, lastModified, etag);
                    //TODO 检查文件是否改变了，这个也可以在脚本中做
                    if (m_nLocalFileLen > 0 && (long)remoteLen == m_nLocalFileLen) {
                        m_nCurlRet = CURLE_OK;
                        m_nResponseCode = 200;
                        break;
                    }
                }
                fp = fopen(p_pLocalFile, "a+b");
                if (!fp) {
                    LOGW("Open file error:%s", p_pLocalFile);
                    m_nCurlRet = CURLE_GOT_NOTHING;
                    break;
                }
                fseek(fp, 0, SEEK_END);

                set_OnData(downLoadPackage,fp);
                //if(m_nLocalFileLen>0)
                    curl_easy_setopt(m_pCurlHandle, CURLOPT_RESUME_FROM, m_nLocalFileLen);
            }
            else {
                set_OnData(Curl::_WriteCallback, this);
                curl_easy_setopt(m_pCurlHandle, CURLOPT_RESUME_FROM, 0);
            }

            m_nOptTimeout = p_nTimeout;
            int opttimeout = p_nTimeout == 0 ? DEF_OPTTIMEOUT : p_nTimeout;
            set_Url(p_pszUrl);
            set_Header(p_vHeaders);
            ApplyHeaders();
            if (p_pPostData && p_nPostLen > 0) {
                set_PostData(p_pPostData, p_nPostLen);
            }
            else {
                set_GET();
            }
            set_OnlyHead(p_bOnlyHeader);
            set_Timeout(opttimeout);
            int connTimeout = p_nConnTimeout==0? 8: p_nConnTimeout;
            set_ConnectTimeout(connTimeout);

            m_nCurlRet = curl_easy_perform(m_pCurlHandle);
            if (!checkResult(p_pszUrl)) {
                m_Buffer.ClearData();
            }
            else if (p_bOnlyHeader) {
                m_Buffer.ClearData();
                m_Buffer.AddData(m_strResponseHead.c_str(), m_strResponseHead.length());
            }
            if(p_ppResBuff)
                *p_ppResBuff = &m_Buffer;
            //curl_easy_setopt(m_pCurlHandle, CURLOPT_WRITEHEADER, 0);
        } while (false);
        end();

        if (fp)
            fclose(fp);

        curl_easy_setopt(m_pCurlHandle, CURLOPT_HTTPHEADER, 0);
        curl_easy_setopt(m_pCurlHandle, CURLOPT_POSTFIELDS, 0);
        curl_easy_setopt(m_pCurlHandle, CURLOPT_POSTFIELDSIZE, 0);
        curl_easy_setopt(m_pCurlHandle, CURLOPT_POST, 0L);
        //不能调用这个。会丢失之前的有效设置。例如session
        //curl_easy_reset(m_pCurlHandle);
    }

	void Curl::query( const char *p_pszUrl, __Buffer **p_ppResBuff, const char* p_pPostData, int p_nPostLen){
        query(p_pszUrl, p_ppResBuff, p_pPostData, p_nPostLen,
            false, P_NOTIMEOUT, P_NOCONNTIMEOUT, NoHeader, P_NOLOCALFILE, false);
	}

    Curl& Curl::set_OnlyHead(bool b) {
        if (b) {
            curl_easy_setopt(m_pCurlHandle, CURLOPT_HEADER, 1);    //只要求header头
            curl_easy_setopt(m_pCurlHandle, CURLOPT_NOBODY, 1);    //不需求body
            set_EnableProg(false);
        }
        else {
            curl_easy_setopt(m_pCurlHandle, CURLOPT_NOBODY, 0); 
            set_EnableProg(true);
        }
        return *this;
    }

	void Curl::downloadHeader(const char *p_pszUrl, __Buffer **p_ppResBuff, int p_nOptTimeout) {
        query(p_pszUrl, p_ppResBuff, P_NOPOSTDATA, 0,
            true, p_nOptTimeout, P_NOCONNTIMEOUT, NoHeader, P_NOLOCALFILE, false);
	}

	void Curl::downloadBigFile(const char *p_pszUrl, const char* p_pszLocal, int p_nOptTimeout) {
        query(p_pszUrl, nullptr, P_NOPOSTDATA, 0,
            false, p_nOptTimeout, P_NOCONNTIMEOUT, NoHeader, p_pszLocal, true);
	}

	void Curl::PostMultipart( const char *p_pszUrl, __Buffer **p_ppResBuff, const char* p_pUserName, const char* p_pPostData, int p_nPostDataLen ){
		std::vector<std::string> formdata;
		formdata.push_back("username");
		formdata.push_back(p_pUserName);
		_PostMultipart(p_pszUrl,p_ppResBuff,formdata,"data",p_pPostData,p_nPostDataLen);
	}

	void Curl::_PostMultipart( const char *p_pszUrl, __Buffer **p_ppResBuff, std::vector<std::string>& formdata, const char* p_pDataName,const char* p_pPostData, int p_nPostDataLen){
		{
			int sz = formdata.size();
			if(sz%2!=0){
				LOGW("Curl::_PostMultipart transfer parameter is incorrect，formdata should be an even number，actually number is %d.",sz);
				return;
			}
		}
		*p_ppResBuff = 0;
		/*
		--------------------------d04f9c242ff033d8
		Content-Disposition: form-data; name="format"

		bin
		--------------------------d04f9c242ff033d8
		Content-Disposition: form-data; name="data"

		?????????????????????????????????????????????????????
		????????????????????
		--------------------------d04f9c242ff033d8--

		*/
        curl_httppost *pPost = 0;
        curl_httppost *lastptr = 0;
        do{
			if( !_Prepare() )
				break;
            begin();
            {
                int sz = formdata.size();
                if (sz>0) {
                    for (int i = 0; i<sz / 2; i++) {
                        curl_formadd(&pPost, &lastptr,
                            CURLFORM_PTRNAME, formdata[i * 2].c_str(),
                            CURLFORM_PTRCONTENTS, formdata[i * 2 + 1].c_str(),
                            CURLFORM_END);
                    }
                }
            }
            if (p_pPostData)
                curl_formadd(&pPost, &lastptr, CURLFORM_PTRNAME, p_pDataName, CURLFORM_PTRCONTENTS, p_pPostData, CURLFORM_CONTENTSLENGTH, p_nPostDataLen, CURLFORM_END);

			CURLcode iCode = CURLE_FAILED_INIT;
            ApplyHeaders();
			iCode = curl_easy_setopt(m_pCurlHandle, CURLOPT_HTTPPOST, pPost);
			int opttimeout = m_nOptTimeout==0?DEF_OPTTIMEOUT:m_nOptTimeout;
            set_Timeout(opttimeout);
            set_ConnectTimeout(2L);
            set_Url(p_pszUrl);
            set_OnData(Curl::_WriteCallback, this);
            set_EnableProg(true);

			m_Buffer.ClearData();
			m_tmTaskBegin = laya::tmGetCurms();// GetClockExact::getInstance()->GetTimeMs();
            m_tmLastHasData = m_tmTaskBegin;
			m_dDownedSize = 0.0;
			m_dLastDownedSize = 0.0;

			m_nCurlRet=curl_easy_perform(m_pCurlHandle);

            if (!checkResult(p_pszUrl)) {
				m_Buffer.ClearData();
			}
			*p_ppResBuff = &m_Buffer;
		} while(0);

		curl_formfree( pPost );
        end();
		//curl_easy_reset(m_pCurlHandle);
		curl_easy_setopt(m_pCurlHandle, CURLOPT_HTTPHEADER, 0);
		curl_easy_setopt(m_pCurlHandle, CURLOPT_HTTPPOST, 0);
	}

	//上传文件的回调函数
	size_t put_data(void *ptr,size_t size,size_t nmemb,void *userdata){
		return fread(ptr,size,nmemb,(FILE *)userdata);
	}

	void Curl::upload(const char *p_pszUrl, __Buffer **p_ppResBuff, const char* p_pUserName,const char* p_pszFile){
		*p_ppResBuff = 0;
		curl_httppost *pPost = 0;
		curl_httppost *lastptr  = 0;
		do{
			if( !_Prepare() )
				break;
			CURLcode iCode = CURLE_FAILED_INIT;
            begin();
            curl_formadd(&pPost, &lastptr, CURLFORM_PTRNAME, "username", CURLFORM_PTRCONTENTS, p_pUserName, CURLFORM_END);
            curl_formadd(&pPost, &lastptr, CURLFORM_COPYNAME, "upload", CURLFORM_FILE, p_pszFile, CURLFORM_END);
            ApplyHeaders();
			int opttimeout = m_nOptTimeout==0?DEF_OPTTIMEOUT:m_nOptTimeout;
            set_Timeout(opttimeout);
            set_ConnectTimeout(2L);
            set_Url(p_pszUrl);
			curl_easy_setopt(m_pCurlHandle, CURLOPT_HTTPPOST, pPost);
			//iCode=curl_easy_setopt(m_pCurlHandle,CURLOPT_READFUNCTION,put_data);	//读文件的回调
            set_EnableProg(false);

			//iCode=curl_easy_setopt(m_pCurlHandle, CURLOPT_UPLOAD, 1L);
			//iCode=curl_easy_setopt(m_pCurlHandle, CURLOPT_READDATA, pf);  
			//iCode=curl_easy_setopt(m_pCurlHandle, CURLOPT_INFILESIZE, fsz); 
			//iCode=curl_easy_setopt(m_pCurlHandle, CURLOPT_INFILESIZE_LARGE, fsz); 

			m_Buffer.ClearData();
			m_tmTaskBegin = laya::tmGetCurms();// GetClockExact::getInstance()->GetTimeMs();
            m_tmLastHasData = m_tmTaskBegin;
			m_dDownedSize = 0.0;
			m_dLastDownedSize = 0.0;

			m_nCurlRet=curl_easy_perform(m_pCurlHandle);
            if(!checkResult(p_pszUrl)){
				m_Buffer.ClearData();
			}
			*p_ppResBuff = &m_Buffer;
		} while(0);

        end();
		//curl_easy_reset(m_pCurlHandle);
		curl_easy_setopt(m_pCurlHandle, CURLOPT_HTTPHEADER, 0);
		curl_easy_setopt(m_pCurlHandle, CURLOPT_HTTPPOST, 0);
	}

	void Curl::setCookieFile( const char* p_pszCookieFile ){
		m_strCookieFile = p_pszCookieFile;
		if(!s_bUseCurlCookie)
			return;
		//curl_easy_setopt( m_pCurlHandle, CURLOPT_COOKIESESSION,true);  开了这个就不行了
		curl_easy_setopt( m_pCurlHandle, CURLOPT_COOKIEJAR, p_pszCookieFile);
		curl_easy_setopt( m_pCurlHandle, CURLOPT_COOKIELIST, "SESS");
		//curl_easy_setopt( m_pCurlHandle, CURLOPT_COOKIEFILE, p_pszCookieFile);
	}

    void Curl::setProxyString(const char* p_pszProxy) {
        if (p_pszProxy && strlen(p_pszProxy) > 0) {
            CURLcode ret = curl_easy_setopt(m_pCurlHandle, CURLOPT_PROXY, p_pszProxy);
            if (ret != CURLE_OK) {
                LOGW("setcurlproxy error");
            }
        }
    }
}
