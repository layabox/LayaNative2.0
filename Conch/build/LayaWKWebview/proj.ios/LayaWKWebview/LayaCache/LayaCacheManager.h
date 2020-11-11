
#ifndef LayaCacheManager_hpp
#define LayaCacheManager_hpp

#include <string>
#include <stdio.h>
#include <functional>

namespace laya {
    
    class JCFileResManager;
    class JCServerFileCache;
    class JCFileRes;
    class LayaCacheManager
    {
    public:
        LayaCacheManager();
        virtual ~LayaCacheManager();
        static LayaCacheManager* getInstance();
        static void delInstance();
        void handleRequest(const char* strUrl, std::function<void(void* pData, int length)> onDownload, std::function<void(int errCode)> onError);
        bool onDownloadErr(void* p_pRes, int p_nErrCode,std::weak_ptr<int> callbackref, std::function<void(int errCode)> onError);
        bool onDownloadEnd(void* p_pRes, std::weak_ptr<int> callbackref, std::function<void(void* pData, int length)> onDownload);
        std::string preUpdateDcc(const std::string& redistPath,const std::string& domain);
        void updateDccClearAssetsid(const std::string& redistPath,const std::string& domain);
        void doUpdateDcc(const std::string& redistPath,const std::string& domain,const std::string& txtdcc,const std::string&  assetsid);
    protected:
        JCFileResManager* m_pFileResMgr;
        JCServerFileCache* m_pSvFileCache;
        static LayaCacheManager* m_pInstance;
        std::shared_ptr<int>    m_CallbackRef;
    };
}
#endif /* LayaCacheManager_hpp */
