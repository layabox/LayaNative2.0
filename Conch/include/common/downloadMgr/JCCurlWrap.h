/**
@file			JCCurlWrap.h
@brief			
@author			hugao
@version		1.0
@date			2016_5_11
*/

#ifndef __JCCurlWrap_H__
#define __JCCurlWrap_H__

// 这个宏指示是在使用 CURL 的静态库，必须要有
#define CURL_STATICLIB

#ifdef __cplusplus
extern "C" {
#endif
#include <curl/curl.h>
#ifdef __cplusplus
}
#endif

#include <memory.h>
#include <list>
#include <vector>
#include <string>
#include <functional>

namespace laya{
	class __Buffer{
	public:
		__Buffer(){
			m_pBuffer = 0;
			m_iDataSize = m_iBufferSize = 0;
		}

		~__Buffer(){
			if( 0 != m_pBuffer ){
				delete[] m_pBuffer;
				m_pBuffer = 0;
			}
		}

		char *SwapBuff( char *p_pBuff, size_t p_iBuffSize, size_t p_iDataSize=0 ){
			char *pRet = m_pBuffer;
			m_pBuffer = p_pBuff;
			m_iBufferSize = p_iBuffSize;
			m_iDataSize = p_iDataSize;
			return pRet;
		}

		size_t GetDataSize(){
			return m_iDataSize;
		}

		char *GetData(){
			if( 0 == m_iDataSize )
				return 0;
			else
				return m_pBuffer;
		}

		void ClearData(){
			m_iDataSize = 0;
		}

		size_t GetFreeSize(){
			return (m_iBufferSize-m_iDataSize);
		}

		void AddData( const void *p_pData, size_t p_iSize ){
			for(;GetFreeSize()<p_iSize;){
				_Expand();
			}

			memcpy( m_pBuffer+m_iDataSize, p_pData, p_iSize );
			m_iDataSize += p_iSize;
		}

		void Finish(){
			AddData( "", 1 );
		}
    private:

        void _Expand(){
            if (0 == m_iBufferSize)
                m_iBufferSize = 16 * 1024;
            else
                m_iBufferSize = m_iBufferSize << 1;

            char *pBuff = new char[m_iBufferSize];

            if (0 != m_pBuffer){
                if (m_iDataSize > 0){
                    memcpy(pBuff, m_pBuffer, m_iDataSize);
                }
                delete[] m_pBuffer;
            }

            m_pBuffer = pBuff;
        }

    private:
        char *m_pBuffer;
        size_t m_iDataSize;
        size_t m_iBufferSize;
	};

    class Curl{
    protected:
        typedef int (*WRITECALLBACK)(void *p_pDataBuffer, size_t p_size, size_t p_nmemb, void *p_pUserData);
        typedef int (*PROGRESSCALLBACK)(void *clientp, curl_off_t  dltotal, curl_off_t dlnow, 
            curl_off_t ultotal, curl_off_t ulnow);
        typedef size_t(*HEADERCALLBACK)(void *ptr, size_t size, size_t nmemb, void *data);
    public:
        //进度的回调。返回1则中止。参数是total，now, speed, userdata
        typedef int(*FuncOnProg)(unsigned int, unsigned int, float, void*);

        Curl();
		~Curl();

        static void global_init();
		bool Init( );
		void Release();
        void setProgressCB(FuncOnProg cb, void* userData) {
            m_pExtOnProg = cb;
            m_pExtOnProgData = userData;
        }

        Curl& begin();
        Curl& end();
        Curl& set_GET();
        Curl& set_POST();
        Curl& set_Url(const char* pUrl);
        Curl& set_CookieList(const char* pC);
        Curl& set_CookieFile(const char* pFile);
        //是否自动重定向
        Curl& set_FollowLocation(long v);
        //设置超时。单位秒
        Curl& set_Timeout(long v);
        // 连接超时时间 秒
        Curl& set_ConnectTimeout(long v);
        Curl& set_OnData(WRITECALLBACK func, void* pUserData);
        Curl& set_EnableProg(bool b);
        Curl& set_UserAgent(const char* pAgent);//这个也可以通过header来设置?
        Curl& set_PostData(const char* pPostData, int nPostLen);
        //只要header?
        Curl& set_OnlyHead(bool b);

        void ApplyHeaders();
		//设置header，每次用完后会自动清理
		void set_Header( const std::vector<std::string>& p_headers ){m_headers = p_headers;};
        //检查当前任务的返回值。触发对应的回调。
		bool checkResult(const char *p_pszUrl);
        //通用的下载请求
        void query(const char *p_pszUrl, __Buffer **p_ppResBuff,
            const char* p_pPostData, int p_nPostLen,  //可以为0
            bool p_bOnlyHeader,                       //一般是false
            int p_nTimeout, int p_nConnTimeout,       //为0则缺省
            const std::vector<std::string>& p_vHeaders,      //size()==0则忽略
            const char* p_pLocalFile,                 //一边下载，一边保存，一般用在大文件下载，0则忽略
            bool p_bChkRemoteChange                   //检查远端文件是否改变了，大文件用
            );
        //如果有 p_pPostData,则在 CURLOPT_POSTFIELDS 发送数据
        void query( const char *p_pszUrl, __Buffer **p_ppResBuff, const char* p_pPostData, int p_nPostLen);

		void downloadBigFile(const char *p_pszUrl, const char* p_pszLocal, int p_nOptTimeout);
		void downloadHeader(const char *p_pszUrl, __Buffer **p_ppResBuff, int p_nOptTimeout);
		//发送一个只有一个 data 字段的数据
		void PostMultipart( const char *p_pszUrl, __Buffer **p_ppResBuff, const char* p_pUserName, const char* p_pPostData, int p_nPostDataLen );
		//上传文件。
		void upload(const char *p_pszUrl, __Buffer **p_ppResBuff, const char* p_pUserName, const char* p_pszFile );

		//发送一个带一个二进制buffer的formdata。
		//二进制数据对应的formdata的名字是 p_pDataName
		//formdata 是其他字段，每两个为一对。
		void _PostMultipart( const char *p_pszUrl, __Buffer **p_ppResBuff, std::vector<std::string>& formdata, const char* p_pDataName,const char* p_pPostData, int p_nPostDataLen);

		//设置curl的cookie文件
		void setCookieFile( const char* p_pszCookieFile );
        void setProxyString(const char* p_pszProxy);
        static int _ProgressCallback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, 
            curl_off_t ultotal, curl_off_t ulnow);
    protected:
        bool _Prepare();
        static int _WriteCallback(void *p_pDataBuffer, size_t p_size, size_t p_nmemb, void *p_pUserData);

    public:
        static std::vector<std::string> NoHeader;
        static bool		s_bUseCurlCookie;
        long        m_nLocalFileLen = 0;    //本地文件的长度。用于断点续传。
        //下面是返回结果
        std::string m_strSvAddr;	//服务器地址
        std::string m_strLocalAddr;
        int         m_nCurlRet = 0;
        long			m_nResponseCode = 0;
        std::string     m_strResponseHead;  //响应的header
    protected:
        CURL *      m_pCurlHandle;
        int		    m_nOptTimeout;
        __Buffer    m_Buffer;
        double     m_tmTaskBegin;
        curl_off_t  m_dDownedSize;
        curl_off_t  m_dLastDownedSize;
        double     m_tmLastProg;
        double     m_tmLastNotify;
        bool	    m_bStopAndRetry;	//停止当前任务，重新尝试
        std::vector<std::string> m_headers;	//每一个都是 key:value
                                            //传了一部分后没有反应的检测
        double     m_tmLastHasData;	//上次接收到数据的时间
        std::string	m_strCookieFile;
        struct curl_slist *m_pslist = nullptr;
        FuncOnProg  m_pExtOnProg = nullptr;
        void*       m_pExtOnProgData = nullptr;
	};
}
//------------------------------------------------------------------------------


#endif //__JCCurlWrap_H__

//-----------------------------END FILE--------------------------------