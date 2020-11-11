/**
@file			JCZipFile.h
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/
#ifndef __JCZipFile_H__
#define __JCZipFile_H__

#include "../downloadCache/JCFileSource.h"
#include <zip.h>
#include <stdio.h>
#include <string>

namespace laya
{
	class JCZipFile: public JCFileSource
    {
	public:
		
        JCZipFile();

		~JCZipFile();

		bool open(const char* p_pZipFile );

		void InitDir(const char* p_pszDir);

		bool getSize(const char* p_pszFile, int& p_nSize, int& p_nCompSize );

		bool read(const char* p_pszFile, const char* p_pBuff, int p_nBufSize );

    public:
        virtual bool isFileExistInZipAPKExpansion(const char* sFileName) { return false; };

		virtual bool isFileExist( const char* p_pszFile);

		virtual unsigned int getFileSize( const char* p_pszFile );

        virtual bool loadFileContent(const char* sFileName, ALLOCMEM pAllocFunc, void* pUserData, int& nBuffSize);
        
        virtual bool loadFileContent(const char* sFileName, char*& pBuffer, int& nBuffSize);

	protected:

		zip*		m_pZip;

		std::string m_sInitDir;

	};
};
//------------------------------------------------------------------------------


#endif //__JCZipFile_H__

//-----------------------------END FILE--------------------------------

