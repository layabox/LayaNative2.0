#ifndef __JCOHOSFileSource_H__
#define __JCOHOSFileSource_H__
#include "JCFileSource.h"
#include <rawfile/raw_file_manager.h>
#include "../../napi/common/native_common.h"
#include "../util/Log.h"
#include "../util/JCZipFile.h"

namespace laya{
    class JCOHOSFileSource : public JCFileSource{
        private:
            NativeResourceManager* nativeResourceManager_;
            char *m_pszRoot;
        public:
            JCOHOSFileSource(){
                m_pszRoot=0;
                LOGI("JCOHOSFileSource::constructor ");
            }
            ~JCOHOSFileSource(){
                if(0!=m_pszRoot){
                    delete[] m_pszRoot;
                    m_pszRoot = 0;
                }
            }
            bool Init(NativeResourceManager* nativeResourceManager, const char *p_pszRoot){
                if(0!=m_pszRoot){
                    delete[] m_pszRoot;
                    m_pszRoot = 0;
                }
                int len;
                if(0!=p_pszRoot&&0!=(len = strlen(p_pszRoot))){
                    m_pszRoot = new char[len+1];
                    memcpy(m_pszRoot,p_pszRoot,len+1);
                    if('\\'==m_pszRoot[len-1]|| '/'==m_pszRoot[len-1]){
                        m_pszRoot[len-1]=0;
                    }
                }
                nativeResourceManager_ = nativeResourceManager;
                return ((0!=nativeResourceManager_)&&(0!=m_pszRoot));
            }
        public:
            virtual bool isFileExistInZipAPKExpansion(const char *sFileName){return false;}
            virtual bool isFileExist(const char *p_pszFile){
                char szName[512];
                char *pathfile = (char *)p_pszFile;
                if(m_pszRoot){
                    sprintf(szName,"%s/%s",m_pszRoot,p_pszFile);
                    pathfile = szName;
                }
                RawFile *fp = OH_ResourceManager_OpenRawFile(nativeResourceManager_,pathfile);
                if(fp){
                    LOGI("JCOHOSFileSource::isFileExist() - open %{public}s success", p_pszFile);
                    OH_ResourceManager_CloseRawFile(fp);
                    return true;
                }
                return false;
            }
            virtual unsigned int getFileSize(const char *p_pszFile)
            {
                char szName[512];
                char *pathfile = (char *)p_pszFile;
                if(m_pszRoot){
                    sprintf(szName, "%s%s", m_pszRoot, p_pszFile);
                    pathfile = szName;
                }
                RawFile *fp = OH_ResourceManager_OpenRawFile(nativeResourceManager_, pathfile);
                LOGI("JSOHOSFileSource::getFileSize ============================> doGetFileData %{public}s",p_pszFile);
                long size = OH_ResourceManager_GetRawFileSize(fp);
                OH_ResourceManager_CloseRawFile(fp);
                if(size!=-1){
                    return size;
                }
                return size;
            }
            virtual bool loadFileContent(const char *p_pszFile, char *&p_pBuff, int &p_iBuffSize){
                p_iBuffSize = 0;
                char szName[512];
                char *pathfile = (char *)p_pszFile;
                if(m_pszRoot){
                    sprintf(szName,"%s/%s",m_pszRoot,p_pszFile);
                    pathfile = szName;
                }
                RawFile *fp = OH_ResourceManager_OpenRawFile(nativeResourceManager_,pathfile);
                if(!fp){
                    LOGI("JCOHOSFileSource::loadFileContent1 fp is nullptr");
                    return false;
                }
                p_iBuffSize = OH_ResourceManager_GetRawFileSize(fp);
                p_pBuff = new char[p_iBuffSize+1];
                int readsize = OH_ResourceManager_ReadRawFile(fp,p_pBuff,p_iBuffSize);
                OH_ResourceManager_CloseRawFile(fp);
                if(readsize<p_iBuffSize){
                    return false;
                }
                p_pBuff[p_iBuffSize]=0;
                return true;
            }
            virtual bool loadFileContent(const char *p_pszFile,ALLOCMEM alloc,void *pUserData, int &p_iBuffSize){
                p_iBuffSize = 0;
                char szName[512];
                char *pathfile = (char *)p_pszFile;
                if(m_pszRoot){
                    sprintf(szName,"%s/%s",m_pszRoot,p_pszFile);
                    pathfile = szName;
                }
                RawFile *fp = OH_ResourceManager_OpenRawFile(nativeResourceManager_,pathfile);
                if(!fp){
                    LOGI("JCOHOSFileSource::loadFileContent2 fp is nullptr %s",p_pszFile);
                    return false;
                }
                p_iBuffSize = OH_ResourceManager_GetRawFileSize(fp);
                unsigned char *pBuff = alloc(p_iBuffSize,pUserData);

                int readsize = OH_ResourceManager_ReadRawFile(fp,pBuff,p_iBuffSize);
                OH_ResourceManager_CloseRawFile(fp);
                if(readsize<p_iBuffSize){
                    return false;
                }
                return true;
            }
    };
}
#endif //__JCOHOSFileSource_H__