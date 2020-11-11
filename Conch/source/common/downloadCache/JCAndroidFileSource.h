/**
@file			JCAndroidFileSource.h
@brief			
@author			James
@version		1.0
@date			2016_5_11
*/

#ifndef __JCAndroidFileSource_H__
#define __JCAndroidFileSource_H__

//包含头文件
#include "JCFileSource.h"
#include <android/asset_manager.h>
#include <android/log.h>
#include "../util/Log.h"
#include "../util/JCZipFile.h"

namespace laya{

	class JCAndroidFileSource : public JCFileSource
	{
	public:
		JCAndroidFileSource()
		{
			m_pszRoot = 0;
			m_pMgr = 0;
			m_pZipAPKExpansionMain = 0;
			m_pZipAPKExpansionPatch = 0;
		}
		~JCAndroidFileSource()
		{
			if( 0 != m_pszRoot )
			{
				delete[] m_pszRoot;
				m_pszRoot = 0;
			}
			if( 0 != m_pZipAPKExpansionMain )
			{
				delete m_pZipAPKExpansionMain;
				m_pZipAPKExpansionMain = 0;
			}
			if( 0 != m_pZipAPKExpansionPatch )
			{
				delete m_pZipAPKExpansionPatch;
				m_pZipAPKExpansionPatch = 0;
			}
		}
		bool Init( AAssetManager *p_pMgr, const char *p_pszRoot  ,const std::string& strAPKExpansionMain, const std::string& strAPKExpansionPatch, const std::string& strWebBase)
		{
			m_pMgr = p_pMgr;
			if( 0 != m_pszRoot )
			{
				delete[] m_pszRoot;
				m_pszRoot = 0;
			}
			int len;
			if( 0 != p_pszRoot && 0 != (len=strlen(p_pszRoot)) )
			{
				m_pszRoot = new char[len+1];
				memcpy( m_pszRoot, p_pszRoot, len+1 );

				if( '\\' == m_pszRoot[len-1] || '/' == m_pszRoot[len-1] )
				{
					m_pszRoot[len-1] = 0;
				}
			}
			
			if (strAPKExpansionMain != "")
			{
				m_pZipAPKExpansionMain = new JCZipFile();
				if( m_pZipAPKExpansionMain->open(strAPKExpansionMain.c_str()))
				{
					LOGE("打开zip成功：%s", strAPKExpansionMain.c_str());
					std::string root = "cache/" + strWebBase;
					m_pZipAPKExpansionMain->InitDir(root.c_str());
				}
				else
				{
					LOGE("打开zip失败：%s", strAPKExpansionMain.c_str());
					delete m_pZipAPKExpansionMain;
					m_pZipAPKExpansionMain = nullptr;
				}
			}
			
			if (strAPKExpansionPatch != "")
			{
				m_pZipAPKExpansionPatch = new JCZipFile();
				if( m_pZipAPKExpansionPatch->open(strAPKExpansionPatch.c_str()))
				{
					LOGE("打开zip成功：%s", strAPKExpansionPatch.c_str());
					std::string root = "cache/" + strWebBase;
					m_pZipAPKExpansionPatch->InitDir(root.c_str());
				}
				else
				{
					LOGE("打开zip失败：%s", strAPKExpansionPatch.c_str());
					delete m_pZipAPKExpansionPatch;
					m_pZipAPKExpansionPatch = nullptr;
				}
			}
			return ((0!=m_pMgr)&&(0!=m_pszRoot));
		}
	public:

        bool isFileExistInZipAPKExpansion(const char* p_pszFile)
        {
            if (m_pZipAPKExpansionMain && m_pZipAPKExpansionMain->isFileExist(p_pszFile))
            {
                return true;
            }

            if (m_pZipAPKExpansionPatch && m_pZipAPKExpansionPatch->isFileExist(p_pszFile))
            {
                return true;
            }

            return false;
        }

		virtual bool isFileExist( const char* p_pszFile )
		{
			char szName[512];
			char* pathfile = (char*)p_pszFile;
			if( m_pszRoot ){
				sprintf(szName,"%s/%s", m_pszRoot, p_pszFile);
				pathfile = szName;
			}
			
			AAsset *pFile = AAssetManager_open( m_pMgr, pathfile, AASSET_MODE_BUFFER );
			if( 0 != pFile )
			{
				AAsset_close( pFile );
				return true;
			}
			
			if (m_pZipAPKExpansionMain && m_pZipAPKExpansionMain->isFileExist(p_pszFile))
			{
				return true;
			}
			
			if (m_pZipAPKExpansionPatch && m_pZipAPKExpansionPatch->isFileExist(p_pszFile))
			{
				return true;
			}
			
			return false;
		}
		virtual unsigned int getFileSize( const char* p_pszFile )
		{
			off_t iSize=0;
			char szName[512];
			char* pathfile = (char*)p_pszFile;
			if( m_pszRoot ){
				sprintf(szName,"%s/%s", m_pszRoot, p_pszFile);
				pathfile = szName;
			}
			AAsset *pFile = AAssetManager_open( m_pMgr, pathfile, AASSET_MODE_BUFFER );
			if( 0 != pFile )
			{
				off_t iSize = AAsset_getLength( pFile );
				AAsset_close( pFile );
				if (iSize >= 0)
					return iSize;
			}
			
			if (m_pZipAPKExpansionMain)
			{
				int sz=0;
				int compsz=0;
				m_pZipAPKExpansionMain->getSize(p_pszFile,sz,compsz);
				if (sz > 0)
				{
					return sz;
				}
			}
			
			if (m_pZipAPKExpansionPatch)
			{
				int sz=0;
				int compsz=0;
				m_pZipAPKExpansionPatch->getSize(p_pszFile,sz,compsz);
				if (sz > 0)
				{
					return sz;
				}
			}
			return 0;
		}
		virtual bool loadFileContent( const char* p_pszFile, char*& p_pBuff, int& p_iBuffSize )
        {
            p_iBuffSize = 0;
            bool bRet = false;
            char szName[512];
            char* pathfile = (char*)p_pszFile;
            if (m_pszRoot) {
                sprintf(szName, "%s/%s", m_pszRoot, p_pszFile);
                pathfile = szName;
            }
			
            AAsset *pFile = AAssetManager_open(m_pMgr, pathfile, AASSET_MODE_BUFFER);
            if (0 != pFile)
            {
                p_iBuffSize = AAsset_getLength(pFile);
                p_pBuff = new char[p_iBuffSize+1];
                if (p_iBuffSize == AAsset_read(pFile, p_pBuff, p_iBuffSize))
                {
                    bRet = true;
                }
                p_pBuff[p_iBuffSize] = 0;
                AAsset_close(pFile);
                LOGI("found the file in the package:%s\n", pathfile);
				if (bRet)
				{
					return true;
				}
            }
			
			if (m_pZipAPKExpansionMain && m_pZipAPKExpansionMain->loadFileContent( p_pszFile, p_pBuff, p_iBuffSize ))
			{
				LOGI("found the file in the package expansion main:%s\n", p_pszFile);
				return true;
			}
			
			if (m_pZipAPKExpansionPatch && m_pZipAPKExpansionPatch->loadFileContent( p_pszFile, p_pBuff, p_iBuffSize ))
			{
				LOGI("found the file in the package expansion patch:%s\n", p_pszFile);
				return true;
			}
			return false;

		}
		virtual bool loadFileContent(const char* p_pszFile, ALLOCMEM alloc, void* pUserData, int& p_iBuffSize) 
        {
			p_iBuffSize = 0;
			bool bRet = false;
			char szName[512];
			char* pathfile = (char*)p_pszFile;
			if (m_pszRoot) {
				sprintf(szName, "%s/%s", m_pszRoot, p_pszFile);
				pathfile = szName;
			}
			
			AAsset *pFile = AAssetManager_open(m_pMgr, pathfile, AASSET_MODE_BUFFER);
			if (0 != pFile)	
            {
				p_iBuffSize = AAsset_getLength(pFile);
				unsigned char* pBuff = alloc(p_iBuffSize, pUserData);
				if (p_iBuffSize == AAsset_read(pFile, pBuff, p_iBuffSize))
					bRet = true;
				AAsset_close(pFile);
                LOGI("found the file in the package:%s\n", pathfile);
				if (bRet)
				{
					return true;
				}
			}
			
			if (m_pZipAPKExpansionMain && m_pZipAPKExpansionMain->loadFileContent( p_pszFile, alloc, pUserData, p_iBuffSize ))
			{
				LOGI("found the file in the package expansion main:%s\n", p_pszFile);
				return true;
			}
			
			if (m_pZipAPKExpansionPatch && m_pZipAPKExpansionPatch->loadFileContent( p_pszFile, alloc, pUserData, p_iBuffSize))
			{
				LOGI("found the file in the package expansion patch:%s\n", p_pszFile);
				return true;
			}
			return false;
		}
    private:
        AAssetManager*  m_pMgr;
        char*           m_pszRoot;
		JCZipFile* 		m_pZipAPKExpansionMain;
		JCZipFile* 		m_pZipAPKExpansionPatch;
	};
}

//------------------------------------------------------------------------------


#endif //__JCAndroidFileSource_H__

//-----------------------------END FILE--------------------------------
