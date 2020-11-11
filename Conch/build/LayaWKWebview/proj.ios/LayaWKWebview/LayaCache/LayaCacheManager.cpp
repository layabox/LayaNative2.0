

#include "LayaCacheManager.h"
#include "resource/JCFileResManager.h"
#include "downloadCache/JCServerFileCache.h"
#include "downloadMgr/JCDownloadMgr.h"
#include "misc/JCWorkerThread.h"
#include <cmath>
std::string gAssetRootPath = "";
std::string gRedistPath = "";
namespace laya {

    LayaCacheManager* LayaCacheManager::m_pInstance = nullptr;
    
    LayaCacheManager::LayaCacheManager()
    {
        m_pSvFileCache = nullptr;
        pthread_key_create(&JCWorkerThread::s_tls_curThread, NULL);
        pthread_key_create(&s_tls_curDataThread, NULL);
        JCDownloadMgr::getInstance()->init(3);//多加一个线程，可能要给优先级较低的任务用。
        m_pFileResMgr = new JCFileResManager(JCDownloadMgr::getInstance());
      
        m_CallbackRef.reset(new int(1));
    }
    
    LayaCacheManager::~LayaCacheManager()
    {
    }
    
 
    LayaCacheManager* LayaCacheManager::getInstance(){
        if( m_pInstance == NULL){
            m_pInstance = new LayaCacheManager();
        }
        return m_pInstance;
    }
    
    void LayaCacheManager::delInstance(){
        if(m_pInstance){
            delete m_pInstance;
            m_pInstance = NULL;
        }
    }

    std::string LayaCacheManager::preUpdateDcc(const std::string& redistPath,const std::string& domain)
    {
        if (m_pSvFileCache != nullptr){
            delete m_pSvFileCache;
        }
        m_pSvFileCache = new laya::JCServerFileCache();
        m_pFileResMgr->setFileCache(m_pSvFileCache);
        m_pSvFileCache->setCachePath((redistPath + "appCache").c_str());
        m_pSvFileCache->switchToApp(domain.c_str());
        
        m_pSvFileCache->setResourceID("appurl", domain.c_str());
        return m_pSvFileCache->getResourceID("netassetsid");
    }
    
    void LayaCacheManager::updateDccClearAssetsid(const std::string& redistPath,const std::string& domain)
    {
        m_pSvFileCache->saveFileTable("");
        m_pSvFileCache->setResourceID("netassetsid", "");
        if (m_pSvFileCache != nullptr){
            delete m_pSvFileCache;
        }
        m_pSvFileCache = new laya::JCServerFileCache();
        m_pFileResMgr->setFileCache(m_pSvFileCache);
        m_pSvFileCache->setCachePath((redistPath + "appCache").c_str());
        m_pSvFileCache->switchToApp(domain.c_str());
    }
    
    void LayaCacheManager::doUpdateDcc(const std::string& redistPath,const std::string& domain,const std::string& txtdcc,const std::string&  assetsid)
    {
        m_pSvFileCache->saveFileTable(txtdcc.c_str());
        if (m_pSvFileCache != nullptr){
            delete m_pSvFileCache;
        }
        m_pSvFileCache = new laya::JCServerFileCache();
        m_pFileResMgr->setFileCache(m_pSvFileCache);
        m_pSvFileCache->setCachePath((redistPath + "appCache").c_str());
        m_pSvFileCache->switchToApp(domain.c_str());
    
        m_pSvFileCache->setResourceID("netassetsid", assetsid.c_str());
    }
    void LayaCacheManager::handleRequest(const char* strUrl, std::function<void(void* pData, int length)> onDownload, std::function<void(int errCode)> onError)
    {
        laya::JCFileRes* pRes = m_pFileResMgr->getRes(strUrl);
        std::weak_ptr<int> cbref(m_CallbackRef);
        pRes->setOnReadyCB(std::bind(&LayaCacheManager::onDownloadEnd,this, std::placeholders::_1, cbref, onDownload));
        pRes->setOnErrorCB(std::bind(&LayaCacheManager::onDownloadErr, this, std::placeholders::_1, std::placeholders::_2, cbref, onError));
        
    }
    bool LayaCacheManager::onDownloadErr(void* p_pRes, int p_nErrCode,std::weak_ptr<int> callbackref, std::function<void(int errCode)> onError)
    {
        if (!callbackref.lock())
            return false;
        onError(p_nErrCode);
        return true;
    }
    bool LayaCacheManager::onDownloadEnd(void* p_pRes, std::weak_ptr<int> callbackref, std::function<void(void* pData, int length)> onDownload)
    {
        if( !callbackref.lock() )
            return false;
        
        laya::JCResStateDispatcher* pRes = (laya::JCResStateDispatcher*)p_pRes;
        laya::JCFileRes* pFileRes = (laya::JCFileRes*)pRes;
        
        if( pFileRes->m_pBuffer.get()==NULL || pFileRes->m_nLength==0 ){
            return false;
        }
        
        onDownload(pFileRes->m_pBuffer.get(), pFileRes->m_nLength);
        return true;
    }
}
