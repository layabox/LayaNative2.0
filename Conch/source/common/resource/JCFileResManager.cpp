/**
@file			JCFileResManager.cpp
@brief
@author			James
@version		1.0
@date			2016_5_11
*/

#include "JCFileResManager.h"
#include "../downloadCache/JCServerFileCache.h"
#include "../downloadCache/JCFileTable.h"
#include "../misc/JCWorkerThread.h"
#include "../util/JCCommonMethod.h"
#include "../util/Log.h"
#include "../util/JCLayaUrl.h"
#include "../fileSystem/JCFileSystem.h"
#include "../downloadMgr/JCCurlWrap.h"
//#include "../misc/conchDebugThread.h"
#include <algorithm>
#include "../downloadMgr/JCDownloadMgr.h"
#include "../util/JCCrypto.h"
#include "../util/JCIThreadCmdMgr.h"
#include "../downloadMgr/JCHttpHeader.h"

namespace laya
{
    HandleFileData gHandleDataFunc = nullptr;

    char* _conch_mallocData(int len) {
        return new char[len];
    }

    void setFileDataHandler(HandleFileData h) {
        gHandleDataFunc = h;
    }
    char* tttt(const char* data, int& len) {
        //test setFileDataHandler
        char* pd = _conch_mallocData(len);
        memcpy(pd, data, len);
        return pd;
    }

    std::string JCFileRes::s_strUploadChkErrUrl = "";
    bool JCFileRes::s_bHasIgnoreChksum = false;
    std::vector<std::string>	JCFileRes::s_vIgnoreChksumError;
    std::mutex	JCFileRes::s_ignorechklock;
    std::string JCFileRes::s_strExtVersion;
    JCFileRes::JCFileRes(JCDownloadMgr* pNetLoader, JCFileResManager* pMgr)
    {
        m_nLastAction = INIT;
        m_nLength = 0;
        m_nLocalFileID = 0;
        m_bDownloading = false;
        m_CallbackRef.reset(new int(1));
        m_bIgnoreError = false;
        m_bSendToJS_complete = false;
        m_pNetLoader = pNetLoader;
        m_pMgr = pMgr;
    }

    JCFileRes::~JCFileRes() {
        m_pBuffer.reset((char*)0);
        m_CallbackRef.reset();
    }

    void JCFileRes::normalizeUrl() {
        bool bToLower = m_pMgr ? m_pMgr->m_bUrlToLowerCase : false;
        if (m_Url.m_nProto == JCUrl::file) {
            std::string ret = m_Url.m_strPath + "/" + m_Url.m_vPath[m_Url.m_vPath.size() - 1];
            if (bToLower)
                UTF8ToLowercase((char*)ret.c_str());
            m_strURL = ret;
            return ;
        }
        std::string file = m_Url.m_vPath.size() > 0 ? m_Url.m_vPath[m_Url.m_vPath.size() - 1] : "";
        std::string attFile = file.length() > 0 ? ("/" + file) : "/";
        std::string& query = m_Url.m_Query;
        std::string attQuery = query.length() > 0 ? (query) : "";
        std::string ret = m_Url.m_strPath + attFile;
        if (bToLower)
            UTF8ToLowercase((char*)ret.c_str());
        ret += attQuery;
        m_strURL = ret;
    }

    void JCFileRes::load(const char* p_pszURL, JCSharedBuffer* pSyncResult) {
        if (!p_pszURL)
            return;
        std::weak_ptr<int> wptr(m_CallbackRef);
        m_Url.parse(p_pszURL);
        normalizeUrl();
        //bool bToLower = m_pMgr ? m_pMgr->m_bUrlToLowerCase : false;
        //m_strURL = normalizePath(p_pszURL, bToLower, nProto);
        //判断是否是file://
        if (m_Url.m_nProto == JCUrl::file) {
            std::weak_ptr<int> wptr(m_CallbackRef);
            const char* pPath = m_strURL.c_str() + strlen("file://");
            if (pPath[2] == ':') {
                pPath += 1;
            }
            JCBuffer buf;
            bool bret = readFileSync(pPath, buf);
            if (bret && buf.m_pPtr) {
                //如果本地打开成功，则可以立刻设置回调，因为是在一个线程
                //这么写可以做到立即回调，相当于同步函数。这样即使没有缓存也可以应付 JsAppCache::loadCachedURL 的要求。
                m_pBuffer.reset(new char[buf.m_nLen]);
                memcpy(m_pBuffer.get(), buf.m_pPtr, buf.m_nLen);
                m_nLength = buf.m_nLen;
                //hugao add
                checkIsEncrypted(m_pBuffer.get(), m_nLength);
                if (gHandleDataFunc) {
                    int nNewlen = m_nLength;
                    char* pNewData = gHandleDataFunc(m_pBuffer.get(), nNewlen);
                    if (pNewData ) {
                        m_nLength = nNewlen;
                        m_pBuffer.reset(pNewData);
                    }
                }
                setState(ready);
                if (pSyncResult) {
                    pSyncResult->m_pBuffer = m_pBuffer;
                    pSyncResult->m_nLen = m_nLength;
                }
                //立即失效。如果再有相同请求，需要重新加载
                m_pBuffer.reset((char*)0);	//TODO 测试：这个不一定会导致释放
                m_nLength = 0;
                setState(freed);
            }
            else {
                onDownloadError(1, 0, wptr);
            }
            return;
        }
        unsigned int chksum = 0;
        //再尝试从缓存中取
        JCSharedBuffer buff;
        bool needDownload = true;
        JCServerFileCache* pSvFileCache = m_pMgr->m_pFileCache;
        if (pSvFileCache) {
            std::string strQuery = m_Url.m_Query;
            std::string& extV = s_strExtVersion;
            if (strQuery.length() > 1) {
                if ( extV.length()>0 &&
                        strQuery.length() > extV.length() && 
                        memcmp(strQuery.c_str() + 1, extV.c_str(), extV.length()) == 0 && 
                        strQuery.find('&') == std::string::npos) {
                    m_bExtVersion = true;
                    //符合版本号的规则。这时候，只计算文件本身，？后面的都不算
                    std::string fullfile = m_Url.m_strPath + "/" + m_Url.m_vPath[m_Url.m_vPath.size() - 1];
                    m_nLocalFileID = pSvFileCache->getFileID(fullfile.c_str());

                    const char* pExtV = strQuery.c_str() + 1 + extV.length(); 
                    //不管是不是0都要计算真正的hash值，因为可能资源中没有，需要下载，需要缓存
                    m_nExtVersionID = JCCachedFileSys::hashRaw(pExtV);
                    if (*pExtV == '0'&& *(pExtV+1)==0) {
                        //版本为0的特殊处理
                        //前提是肯定有dcc？
                        pSvFileCache->getFileInfo(m_nLocalFileID, chksum);
                        needDownload = !pSvFileCache->load(m_nLocalFileID, chksum, buff,false,true);
                    }
                    else {
                        JCCachedFileSys::typeChkSum tcs = (JCCachedFileSys::typeChkSum)m_nExtVersionID;
                        needDownload = !pSvFileCache->load(m_nLocalFileID, tcs, buff,true,true);
                    }
                }
                else {
                    //不是外部版本控制，
                    //20170821 ?? 这个要走缓存么？ 已经忘了为什么这么写了。
                    // m_strURL是完整的带？的，如果缓存中通过某种方法加了这种地址，应该也允许缓存
                    m_nLocalFileID = pSvFileCache->getFileID(m_strURL.c_str());
                    JCCachedFileSys::typeChkSum tcs = 0;
                    needDownload = !pSvFileCache->load(m_nLocalFileID, tcs, buff, false,true);
                }
            }
            else {
                //没有？,可以用dcc
                m_nLocalFileID = pSvFileCache->getFileID(m_strURL.c_str());// hashURLFull(m_strURL.c_str());
                pSvFileCache->getFileInfo(m_nLocalFileID, chksum);
                needDownload = !pSvFileCache->load(m_nLocalFileID, chksum, buff,false,true);
            }
        }

        if (needDownload) {
            if (m_bDownloading)
                return;

            //注意不能是p_pszURL。因为不可靠。如果是p_pszURL就是m_strURL获得的，则在 m_strURL = normalizePath
            //的时候，会导致p_pszURL被释放。
            m_bDownloading = true;
            if (chksum != 0) {
                char chkbuf[64] = { 0 };
                sprintf(chkbuf, "%x", chksum);
                std::string cdnfriendUrl = m_strURL + "?" + chkbuf;
                verifyDownload(cdnfriendUrl.c_str(), chksum);
            }
            else {
                verifyDownload(m_strURL.c_str(), chksum);
            }
            goto end;
        }
    //loadok:
        {
            m_nLastAction = LOADFROMCACHE;
            //如果本地打开成功，则可以立刻设置回调，因为是在一个线程
            m_pBuffer = buff.m_pBuffer;
            m_nLength = buff.m_nLen;
            if (pSyncResult) {
                pSyncResult->m_pBuffer = buff.m_pBuffer;
                pSyncResult->m_nLen = m_nLength;
            }
            //转到js线程执行setState。不要同步了，保持与浏览器一致。
            if (m_pMgr->m_pCmdPoster) {
                if (!m_bSendToJS_complete) {
                    m_bSendToJS_complete = true;	//这里肯定是js线程，可以处理这个标志
                    std::function<void()> cb = std::bind(&JCFileRes::onResDownloadOK_JSThread, this, wptr);
                    m_pMgr->m_pCmdPoster->postToJS(cb);
                }
            }
            else {
                onResDownloadOK_JSThread(wptr);
            }
        }
    end:
        int a = 0;
    }

    bool JCFileRes::loadFromCache(JCBuffer& buff, bool bDoCheckSum) {
        JCUrl url;
        url.parse(m_strURL.c_str());
        if (url.m_nProto == JCUrl::file) {
            char* pFile = (char*)m_strURL.c_str();
            if (strstr(m_strURL.c_str(), "file:///") == pFile) {
                if (pFile[9] == ':') {
                    pFile += 8;
                }
                else {
                    pFile += 7;
                }
            }
            return readFileSync(pFile, buff);
        }

        if (m_bDownloading)
            return false;
        if (!m_pMgr || !m_pMgr->m_pFileCache)
            return false;
        if (bDoCheckSum && m_bExtVersion) {
            JCSharedBuffer _buff;
            JCCachedFileSys::typeChkSum tcs = (JCCachedFileSys::typeChkSum)m_nExtVersionID;
            if (m_pMgr->m_pFileCache->load(m_nLocalFileID, tcs, _buff, true, true)) {
                if (_buff.m_pBuffer.get()) {
                    buff.create(_buff.m_nLen);
                    memcpy(buff.m_pPtr, _buff.m_pBuffer.get(), _buff.m_nLen);
                    return true;
                }
            }
            return false;
        }
        return m_pMgr->m_pFileCache->load(m_nLocalFileID, buff, bDoCheckSum);
    }

    bool JCFileRes::checkIsEncrypted(char *buf, int len){
#ifndef THIN_COMMON_WITHOUT_DOWNLOAD
        bool temp = JCEncrypt::decrypt(buf, len);
        if (temp)m_nLength = len - JCEncrypt::s_nPreLen;
        return temp;
#else
        return false;
#endif
    }

    void JCFileRes::verifyDownload(const char* p_pszURL, unsigned int p_nChkSum) {
        m_nLastAction = DOWNLOADING;
        JCDownloadMgr* pNetLoader = m_pNetLoader;
        std::weak_ptr<int> wptr(m_CallbackRef);
        pNetLoader->download(p_pszURL, 0,
            std::bind(&JCFileRes::onProgress, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, wptr),
            std::bind(&JCFileRes::onDownloaded, this, 
                std::placeholders::_1, 
                std::placeholders::_2, 
                std::placeholders::_3, 
                std::placeholders::_4,
                std::placeholders::_5,
                std::placeholders::_6,
                p_nChkSum, 1, wptr), m_nOptTimeout, m_nConnTimeout);
    }

    void JCFileRes::onResDownloadOK_JSThread(std::weak_ptr<int> p_cbref) {
        if (!p_cbref.lock())
            return;
        if (m_nLength == 0)	//如果已经为0了，则表示已经处理的，状态改变已经通知给需要的人了，直接返回。
            return;
        //hugao add
        checkIsEncrypted(m_pBuffer.get(), m_nLength);
        if (gHandleDataFunc) {
            int nNewlen = m_nLength;
            char* pNewData = gHandleDataFunc(m_pBuffer.get(), nNewlen);
            if (pNewData) {
                m_nLength = nNewlen;
                m_pBuffer.reset(pNewData);
            }
        }
        setState(ready);
        //立即失效。如果再有相同请求，需要重新加载
        m_pBuffer.reset((char*)0);	//TODO 测试：这个不一定会导致释放
        m_nLength = 0;
        setState(freed);
        m_bSendToJS_complete = false;	//处理完了，可以继续post了。
    }

    void JCFileRes::notifyErrorHandler(int p_nError, int p_nHttpResponse) {
        if (!m_bIgnoreError) {
        }
    }

    void JCFileRes::onResDownloadErr_JSThread(std::weak_ptr<int> p_cbref, int p_nError, int p_nHttpResponse) {
        if (!p_cbref.lock())
            return;
        //mnErrNo = downloadError;
        m_bDownloading = false;
        m_nErrNo = p_nError;
        m_nLastHttpResponse = p_nHttpResponse;
        setState(error);
        //立即失效。如果再有相同请求，需要重新加载
        m_pBuffer.reset((char*)0);	//TODO 测试：这个不一定会导致释放
        m_nLength = 0;
        //setState(freed);	不应该再设置成freed，应该保留error，否则再次设置 setOnErrorCB的话
    }

    inline bool _MaybeAnImg(const char* pBuff, int nLen) {
        if (pBuff && nLen > 4) {
            static int jpegID = 0x00ffd8ff;	//最高位00那个位置不一定是什么
            static int gifID = 0x38464947;
            static int pngID = 0x474e5089;

            int idval = *(int*)pBuff;
            return (idval == pngID || idval == gifID || (idval & 0xffffff) == jpegID);
        }
        return false;
    }

    inline bool _StrInVec(const char* pStr, std::vector<std::string>& vec) {
        for (int ei = 0, sz = vec.size(); ei < sz; ei++) {
            if (vec[ei] == pStr) {
                return true;
            }
        }
        return false;
    }

    void JCFileRes::onDownloaded(JCBuffer& p_Buff,
        const string& pLocalAddr, const string& pSvAddr,
        int pnCurlRet, int pnHttpRet,
        const string& pstrHeader,
        unsigned int p_nChkSum,
        int p_nDownloadNum, std::weak_ptr<int> p_cbref) {
        //用这种方式并不可靠，当多线程的时候，可能在这时候还有效，执行完这句就被删除了。
        if (!p_cbref.lock())
            return;
        m_nLastAction = DOWNLOADED;
        m_strSvIP = pSvAddr;
        LOGI("Downloaded %s@%s s=%x l=%d", m_strURL.c_str(), pSvAddr.c_str(), p_nChkSum, p_Buff.m_nLen);
        bool chkErr = false;
        JCServerFileCache* pSvFileCache = nullptr;
        if (pnCurlRet == CURLE_OK && pnHttpRet >= 200 && pnHttpRet < 300) {
            //如果什么都没有返回，则不用继续处理了
            if (p_Buff.m_pPtr == NULL || p_Buff.m_nLen == 0)
                goto end;
            pSvFileCache = m_pMgr->m_pFileCache;
            if (pSvFileCache) {
                if (p_nChkSum > 0) {
                    unsigned int downchk = JCCachedFileSys::getChkSum(p_Buff.m_pPtr, p_Buff.m_nLen);
                    if (downchk != p_nChkSum) {
                        //校验没有通过。
                        if (s_strUploadChkErrUrl.length() > 0) {
                            //上传一次。注意这里必须是在下载线程，以免造成卡顿
                            JCDownloadMgr* pNetLoader = m_pNetLoader;
                            if (pNetLoader) {
                                char postbuf[1024];
                                sprintf(postbuf, "%s,%s,%08x,%08x,%d,%s",
                                    pLocalAddr.c_str(),
                                    pSvAddr.c_str(),
                                    downchk,	//实际
                                    p_nChkSum,	//应该
                                    p_Buff.m_nLen,//长度
                                    m_strURL.c_str());//url
                                pNetLoader->postData(s_strUploadChkErrUrl.c_str(),
                                    postbuf, strlen(postbuf), JCDownloadMgr::defCompleteFunc
                                    );
                            }
                        }
                        //这时候，只能算作临时文件
                        chkErr = true;
                        goto updatecache;
                    }
                }
                goto updatecache;    //
            }
            //如果没有appcache也要通知onok
            //转到js线程执行setState。
            m_pBuffer = std::shared_ptr<char>(new char[p_Buff.m_nLen], std::default_delete<char[]>());
            memcpy(m_pBuffer.get(), p_Buff.m_pPtr, p_Buff.m_nLen);
            m_nLength = p_Buff.m_nLen;
            std::weak_ptr<int> wptr(m_CallbackRef);
            if (m_pMgr->m_pCmdPoster) {
                std::function<void()> cb = std::bind(&JCFileRes::onResDownloadOK_JSThread, this, wptr);
                m_pMgr->m_pCmdPoster->postToJS(cb);
            }
            else {
                onResDownloadOK_JSThread(wptr);
            }
            goto end;
        }
        else {
            //error
            std::weak_ptr<int> wptr(m_CallbackRef);
            onDownloadError(pnCurlRet, pnHttpRet, wptr);
            goto end;
        }

    updatecache:{
            std::string ext = getLowercaseExtOfUrl(m_strURL.c_str());
            m_pBuffer = std::shared_ptr<char>(new char[p_Buff.m_nLen], std::default_delete<char[]>());
            memcpy(m_pBuffer.get(), p_Buff.m_pPtr, p_Buff.m_nLen);
            m_nLength = p_Buff.m_nLen;
            std::weak_ptr<int> wptr(m_CallbackRef);
            //需要更新缓存。
            bool bNeedSave = false;
            if (m_bExtVersion || (p_nChkSum != 0 && !chkErr)) {
                //长期保存
                std::string strLocalFile = pSvFileCache->updateAFile(m_nLocalFileID,
                    m_pBuffer.get(), p_Buff.m_nLen,
                    m_bExtVersion ? m_nExtVersionID : p_nChkSum,
                    m_bExtVersion,
                    0,0);
            }
            else if (_MaybeAnImg(p_Buff.m_pPtr, p_Buff.m_nLen) || 
                _StrInVec(ext.c_str(), m_pMgr->m_vExtNeedSave) ||
                chkErr ) {
                //临时保存。进程内不允许删除
                JCHttpHeader hh(pstrHeader.c_str());
                //hh.getCacheContrl()
                //TODO 计算过期时间
                std::string strLocalFile = pSvFileCache->updateAFile(m_nLocalFileID,
                    m_pBuffer.get(), p_Buff.m_nLen,
                    0,
                    0,
                    0, 1);
            }
            else {
                //临时保存，进程内可以删除
                //TODO  需要实现临时缓存的管理
                /*
                std::string strLocalFile = pSvFileCache->updateAFile(m_nLocalFileID,
                    m_pBuffer.get(), p_Buff.m_nLen,
                    m_bExtVersion ? m_nExtVersionID : p_nChkSum,
                    m_bExtVersion,
                    0, 0);
                */
            }
            m_nLastAction = UPDATECACHE;

            //转到js线程执行setState。
            if (m_pMgr->m_pCmdPoster) {
                std::function<void()> cb = std::bind(&JCFileRes::onResDownloadOK_JSThread, this, wptr);
                m_pMgr->m_pCmdPoster->postToJS(cb);
            }
            else {
                onResDownloadOK_JSThread(wptr);
            }
        }
    //nocache:
        //int b = 0;
    end:
        //int c = 0;

        m_bDownloading = false;
    }

    //问题：这个可能是在另外的线程调用的
    void JCFileRes::onDownloadedOld(JCBuffer& p_Buff, 
            const string& pLocalAddr, const string& pSvAddr, 
            int pnCurlRet, int pnHttpRet,
            const string& pstrHeader,
            unsigned int p_nChkSum, 
            int p_nDownloadNum, std::weak_ptr<int> p_cbref) {
        //用这种方式并不可靠，当多线程的时候，可能在这时候还有效，执行完这句就被删除了。
        if (!p_cbref.lock())
            return;
        m_nLastAction = DOWNLOADED;
        m_strSvIP = pSvAddr;
        LOGI("Downloaded %s@%s s=%x l=%d", m_strURL.c_str(), pSvAddr.c_str(), p_nChkSum, p_Buff.m_nLen);
        bool downError = (p_Buff.m_pPtr == NULL || p_Buff.m_nLen == 0);
        std::string ext = getLowercaseExtOfUrl(m_strURL.c_str());
        JCServerFileCache* pSvFileCache = m_pMgr->m_pFileCache;
        if (!downError) {
            //先写到fileCache中。这时候还不是js线程，这里写是为了不影响js线程的效率。
            //问题是可能导致load和写cache不在一个线程。这个可能问题不大
            //如果非要一个线程，则这里不要调用，通过 setOnReadyCB( std::bind(&svFileCache::onFileDownloaded, pSvFileCache, std::placeholders::_1, chksum)) 的方法在js线程中执行
            bool chkok = p_nChkSum == 0;
            if (p_nChkSum) {
                unsigned int downchk = 0;
                //如果是文本文件，把\r\n统一换成\n再进行校验，以绕开各个环节的转换
                downchk = JCCachedFileSys::getChkSum(p_Buff.m_pPtr, p_Buff.m_nLen);
                chkok = (downchk == p_nChkSum);
                if (!chkok && checkIgnoreChksum((char*)ext.c_str())) {
                    LOGW("[%s@%s]Check error,but you can ignore,real:%08x,should be:%08x,loc=%s\n", m_strURL.c_str(), pSvAddr.c_str(),
                        downchk, p_nChkSum, pLocalAddr.c_str());
                    //errorflog("[%s@%s]校验错误，但忽略，实%08x,应 %08x,loc=%s\n", m_strURL.c_str(), p_pCurl->m_strSvAddr.c_str(), downchk, p_nChkSum,	p_pCurl->m_strLocalAddr.c_str());
                    chkok = true;	//算作正确
                    //上传一次
                    if (s_strUploadChkErrUrl.length() > 0) {
                        //上传一次。注意这里必须是在下载线程，以免造成卡顿
                        JCDownloadMgr* pNetLoader = m_pNetLoader;
                        if (pNetLoader) {
                            char postbuf[1024];
                            sprintf(postbuf, "%s,%s,%08x,%08x,%d,%s",
                                pLocalAddr.c_str(),
                                pSvAddr.c_str(),
                                downchk,	//实际
                                p_nChkSum,	//应该
                                p_Buff.m_nLen,//长度
                                m_strURL.c_str());//url
                            pNetLoader->postData(s_strUploadChkErrUrl.c_str(), 
                                postbuf, strlen(postbuf), JCDownloadMgr::defCompleteFunc
                                );
                        }
                    }
                }
                if (!chkok) {
                    LOGW("[%s@%s]check error,real:%08x,should be:%08x,loc=%s\n", m_strURL.c_str(), pSvAddr.c_str(), downchk, 
                        p_nChkSum, pLocalAddr.c_str());
                    //errorflog("[%s@%s]校验错误，实%08x,应 %08x,loc=%s\n", m_strURL.c_str(), p_pCurl->m_strSvAddr.c_str(), downchk, p_nChkSum,	p_pCurl->m_strLocalAddr.c_str());
                    if (p_nDownloadNum >= MAXDOWNLOADTRY && s_strUploadChkErrUrl.length() > 0) {
                        //上传一次。注意这里必须是在下载线程，以免造成卡顿
                        JCDownloadMgr* pNetLoader = m_pNetLoader;
                        if (pNetLoader) {
                            char postbuf[1024];
                            sprintf(postbuf, "%s,%s,%08x,%08x,%d,%s",
                                pLocalAddr.c_str(),
                                pSvAddr.c_str(),
                                downchk,	//实际
                                p_nChkSum,	//应该
                                p_Buff.m_nLen,//长度
                                m_strURL.c_str());//url
                            pNetLoader->postData(s_strUploadChkErrUrl.c_str(), postbuf, strlen(postbuf), 
                                JCDownloadMgr::defCompleteFunc
                                );
                        }
                    }
                }
            }
            if (!chkok && p_nDownloadNum < MAXDOWNLOADTRY) {
                //继续下载
                LOGI("再次尝试下载");
                JCDownloadMgr* pNetLoader = m_pNetLoader;
                std::weak_ptr<int> wptr(m_CallbackRef);
                pNetLoader->download(m_strURL.c_str(), 0,
                    std::bind(&JCFileRes::onProgress, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, wptr),
                    std::bind(&JCFileRes::onDownloaded, this, 
                        std::placeholders::_1, 
                        std::placeholders::_2, 
                        std::placeholders::_3, 
                        std::placeholders::_4,
                        std::placeholders::_5,
                        std::placeholders::_6,
                        p_nChkSum, ++p_nDownloadNum, wptr),
                    0);
                return;
            }
            downError = !chkok;
            if (!chkok && s_strUploadChkErrUrl.length() <= 0) {//如果有上传地址，就不再需要提示了
                //JCErrorHandleInterface* pEH = getErrorHandler();
                //if(pEH){
                //	pEH->download_chkSumVerifyError(m_strURL.c_str());
                //}
            }
        }

        //下载的数据先copy一份，使用完了之后，立即删除
        //即使校验错误，也算作正确的。
        if (!downError || p_nDownloadNum > 2) {
            //注意：这里并没有复制buffer，外部使用者需要自己复制。
            m_pBuffer = std::shared_ptr<char>(new char[p_Buff.m_nLen],std::default_delete<char[]>());
            memcpy(m_pBuffer.get(), p_Buff.m_pPtr, p_Buff.m_nLen);
            m_nLength = p_Buff.m_nLen;
            std::weak_ptr<int> wptr(m_CallbackRef);

            //如果是缓存文件。需要更新缓存。
            bool bNeedSave = false;
            if (p_nChkSum) {
                bNeedSave = p_nDownloadNum <= 1;
            }
            else {
                //如果是图片文件要恢复
                if (p_Buff.m_pPtr && p_Buff.m_nLen > 4) {
                    static int jpegID = 0x00ffd8ff;	//最高位00那个位置不一定是什么
                    static int gifID = 0x38464947;
                    static int pngID = 0x474e5089;

                    int idval = *(int*)p_Buff.m_pPtr;
                    if (idval == pngID || idval == gifID || (idval & 0xffffff) == jpegID)
                        bNeedSave = true;
                }
                //如果是外部版本管理，需要保存
                if (!bNeedSave) {

                }
                if (!bNeedSave) {
                    //如果是需要同步恢复的资源文件，就要缓存
                    for (int ei = 0, sz = m_pMgr->m_vExtNeedSave.size(); ei < sz; ei++) {
                        if (m_pMgr->m_vExtNeedSave[ei] == ext) {
                            bNeedSave = true;
                            break;
                        }
                    }
                }
            }
            if (bNeedSave){
                std::string strLocalFile = pSvFileCache->updateAFile(
                    m_nLocalFileID, m_pBuffer.get(), p_Buff.m_nLen,
                    p_nChkSum, 0, 0, 0);
                if (strLocalFile.length() > 0) {
                    m_nLastAction = UPDATECACHE;
                }
            }

            //转到js线程执行setState。
            if (m_pMgr->m_pCmdPoster) {
                std::function<void()> cb = std::bind(&JCFileRes::onResDownloadOK_JSThread, this, wptr);
                m_pMgr->m_pCmdPoster->postToJS(cb);
            }
            else {
                onResDownloadOK_JSThread(wptr);
            }
        }
        else {
            m_pBuffer.reset((char*)0);
            m_nLength = 0;
            //notifyErrorHandler(Curl::EC_PartialFile, 0);	//校验错误的responsecode只能0了
            std::weak_ptr<int> wptr(m_CallbackRef);
            if (m_pMgr->m_pCmdPoster) {
                //转到js线程执行setState。
                std::function<void()> cb = std::bind(&JCFileRes::onResDownloadErr_JSThread, 
                    this, wptr, -1, 0);
                m_pMgr->m_pCmdPoster->postToJS (cb);
            }
            else {
                onResDownloadErr_JSThread(wptr, -1, 0);
            }
        }
        m_bDownloading = false;
    }

    bool JCFileRes::restoreRes() {
        load(m_strURL.c_str(),nullptr);
        return true;
    }

    void JCFileRes::onDownloadError(int p_nError, int p_nHttpResponse, std::weak_ptr<int> p_cbref) {
        //std::lock_guard<std::mutex> lock(m_pMgr->m_maplock);//要删除map，可能会与请求的时候冲突
        if (!p_cbref.lock())
            return;
        m_pBuffer.reset((char*)0);
        m_nLength = 0;

        if (!m_bIgnoreError) {
            LOGE("JCFileRes::onDownloadError file error[%d]:%s", p_nError, m_strURL.c_str());
            //errorflog("下载文件错误[%d]：%s",p_nError, m_strURL.c_str());
        }
        //m_pMgr->delRes(m_strURL.c_str());	到资源管理器中统一做
        //auto it = m_ResMap.find(m_strURL.c_str());
        notifyErrorHandler(p_nError, p_nHttpResponse);
        std::weak_ptr<int> wptr(m_CallbackRef);
        //转到js线程执行setState。
        if (m_pMgr->m_pCmdPoster) {
            std::function<void()> cb = std::bind(&JCFileRes::onResDownloadErr_JSThread, this, wptr, p_nError, p_nHttpResponse);
            m_pMgr->m_pCmdPoster->postToJS(cb);
        }
        else {
            onResDownloadErr_JSThread(wptr, p_nError, p_nHttpResponse);
        }
    }

    int JCFileRes::onProgress(unsigned int now, unsigned int total, float speed, std::weak_ptr<int> p_cbref) {
        return 0;
    }

    JCFileResManager::JCFileResManager(JCDownloadMgr* pDownloadMgr) {
        m_pFileCache = nullptr;
        m_pCmdPoster = nullptr;
        m_bUrlToLowerCase = false;
        m_vExtNeedSave = {".png",".jpg",".wav",".ogg"};
        m_pDownloadMgr = pDownloadMgr;
    }

    JCFileResManager::~JCFileResManager() {
        clear();
    }

    void JCFileResManager::clear() {
        std::lock_guard<std::mutex> lock(m_maplock);
        FileResMap::iterator it = m_ResMap.begin();
        FileResMap::iterator ed = m_ResMap.end();
        while (it != ed) {
            delete (*it).second;
            it++;
        }
        m_ResMap.clear();
    }

    //如果已经下载完了，应该同步去打开文件，不要再走下载了（在load中做了）
    JCFileRes* JCFileResManager::getRes(const std::string& url, int p_nConnTimeout , int p_nOptTimeout) {
        std::lock_guard<std::mutex> lock(m_maplock);//下面要添加资源，需要锁。因为失败的话会删除
        JCFileRes* pRes = NULL;
        FileResMap::iterator it = m_ResMap.find(url);
        if (it == m_ResMap.end()) {
            pRes = new JCFileRes(m_pDownloadMgr,this);
            if (p_nConnTimeout>0) {
                pRes->m_nConnTimeout = p_nConnTimeout;
            }
            if (p_nOptTimeout > 0) {
                pRes->m_nOptTimeout = p_nOptTimeout;
            }
            m_ResMap[url] = pRes;
            //问题：load和下载回调不在一个线程会有问题么？
            // 如果load不修改表和文件内容，下载回调在最后再设置ready，应该没事
            pRes->load(url.c_str(),nullptr);
            //m_pThread = workerThread::getCurThread();
            return pRes;
        }
        pRes = (*it).second;
        if (p_nConnTimeout>0) {
            pRes->m_nConnTimeout = p_nConnTimeout;
        }
        if (p_nOptTimeout > 0) {
            pRes->m_nOptTimeout = p_nOptTimeout;
        }

        return pRes;
    }



    bool JCFileResManager::delRes(const char* p_pszURL) {
        std::lock_guard<std::mutex> lock(m_maplock);
        auto it = m_ResMap.find(p_pszURL);
        if (it != m_ResMap.end()) {
            m_ResMap.erase(it);
            return true;
        }
        return false;
    }

    void JCFileRes::addChkIgnoreChksumExt(const char* p_pszExt) {
        s_bHasIgnoreChksum = true;
        std::string ext = p_pszExt;
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        s_ignorechklock.lock();
        for (int i = 0, sz = s_vIgnoreChksumError.size(); i < sz; i++) {
            if (s_vIgnoreChksumError[i] == ext) {
                s_ignorechklock.unlock();
                return;
            }
        }
        s_vIgnoreChksumError.push_back(ext);
        s_ignorechklock.unlock();
    }

    void JCFileRes::clearChkIgnoreChksumExt() {
        s_bHasIgnoreChksum = false;
        s_ignorechklock.lock();
        s_vIgnoreChksumError.clear();
        s_ignorechklock.unlock();
    }

    bool JCFileRes::checkIgnoreChksum(char* p_pszExt) {
        if (!s_bHasIgnoreChksum)
            return false;
        std::string ext = p_pszExt;
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        s_ignorechklock.lock();
        for (int i = 0, sz = s_vIgnoreChksumError.size(); i < sz; i++) {
            if (s_vIgnoreChksumError[i] == ext) {
                s_ignorechklock.unlock();
                return true;
            }
        }
        s_ignorechklock.unlock();
        return false;
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
