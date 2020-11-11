/**
@file			JCIosFileSource.h
@brief			
@author			James
@version		1.0
@date			2016_5_11
*/

#ifndef __JCIosFileSource_H__
#define __JCIosFileSource_H__

#include <stdio.h>
#include "JCFileSource.h"
#include "../util/JCCommonMethod.h"
#include "../util/Log.h"
#include <string.h>
#pragma warning (disable: 4996)

namespace laya
{
	class JCIosFileSource : public JCFileSource
	{
	public:
        JCIosFileSource()
		{
		}
		~JCIosFileSource()
		{
		}
		bool Init( const char* sRootPath )
		{
			if( sRootPath )
			{
                m_sRootPath = sRootPath;
				return true;
			}
			else
			{
				return false;
			}
		}
	public:
        virtual bool isFileExistInZipAPKExpansion(const char* sFileName) { return false; }
		virtual bool isFileExist( const char* sFileName )
		{
            std::string sFullName = m_sRootPath + "/"+sFileName;
            FILE* fp = fopen(sFullName.c_str(),"rb");
            if( !fp )
            {
                return false;
            }
            fclose(fp);
            return true;
		}
        virtual unsigned int getFileSize(const char* sFileName)
        {
            std::string sFullName = m_sRootPath + "/"+ sFileName;
            FILE* fp = fopen(sFullName.c_str(), "rb");
            if (!fp) return 0;
            fseek(fp, 0, SEEK_END);
            int nBuffSize = (int)ftell(fp);
            fclose(fp);
            return nBuffSize;
        }
		virtual bool loadFileContent(const char* sFileName, ALLOCMEM pAllocFunc, void* pUserData, int& nBuffSize)
        {
			bool bRet = false;
            std::string sFullName = m_sRootPath +"/"+ sFileName;
            FILE* fp = fopen(sFullName.c_str(), "rb");
            if (!fp) return false;
            fseek(fp, 0, SEEK_END);
            nBuffSize = (int)ftell(fp);
            fseek(fp, 0, SEEK_SET);
            unsigned char* pBuff = pAllocFunc(nBuffSize, pUserData);
            if (!pBuff)
            {
                bRet = false;
            }
            else
            {
                int nReadSize = fread((char*)pBuff, 1, nBuffSize, fp);
                if (nReadSize == nBuffSize)
                {
                    LOGI("found the file in the package:%s\n", sFileName);
                    bRet = true;
                }
            }
            fclose(fp);
            return bRet;
		}
        virtual bool loadFileContent(const char* sFileName, char*& pBuffer, int& nBuffSize)
        {
            bool bRet = false;
            std::string sFullName = m_sRootPath + "/"+sFileName;
            FILE* fp = fopen(sFullName.c_str(), "rb");
            if (!fp) return false;
            fseek(fp, 0, SEEK_END);
            nBuffSize = (unsigned int)ftell(fp);
            fseek(fp, 0, SEEK_SET);
            pBuffer = new char[nBuffSize+1];
            int nReadSize = fread((char*)pBuffer, 1, nBuffSize, fp);
            if (nReadSize == nBuffSize)
            {
                LOGI("found the file in the package:%s\n", sFileName);
                bRet = true;
            }
            pBuffer[nBuffSize] = 0;
            fclose(fp);
            return bRet;
        }
    private:
        std::string             m_sRootPath;
	};
}
//------------------------------------------------------------------------------


#endif //__JCIosFileSource_H__

//-----------------------------END FILE--------------------------------

