/**
@file			JCZipFile.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#include "JCZipFile.h"
#include "Log.h"

namespace laya
{
	JCZipFile::JCZipFile(){
		m_pZip = NULL;
	}
	JCZipFile::~JCZipFile(){
		if(m_pZip){
			zip_close(m_pZip);
		}
	}
	bool JCZipFile::open(const char* p_pZipFile ){
		int err = 0;
		m_pZip = zip_open( p_pZipFile, 0, &err);
		if( err ){
			LOGI("open zip %s failed erro code %d",p_pZipFile,err);
			m_pZip = NULL;
		}
		return m_pZip!=NULL;
	}
	void JCZipFile::InitDir(const char* p_pszDir ){
        m_sInitDir = p_pszDir;
	}
	bool JCZipFile::getSize(const char* p_pszFile, int& p_nSize, int& p_nCompSize ){
		if( !m_pZip )
			return false;
		struct zip_stat st;
		zip_stat_init(&st);
		std::string file = p_pszFile;
		if(m_sInitDir.size() > 0 )
		{
			file = m_sInitDir + "/" + p_pszFile;
		}
		zip_stat(m_pZip, file.c_str(), 0, &st);
		p_nSize = (int)(st.size);
		p_nCompSize = (int)(st.comp_size);
		return true;
	}
	bool JCZipFile::read(const char* p_pszFile, const char* p_pBuff, int p_nBufSize ){
		if( !m_pZip )
			return false;
		return true;
	}
	bool JCZipFile::isFileExist( const char* p_pszFile){
		int sz=0;
		int compsz=0;
		getSize(p_pszFile,sz,compsz);
		return sz>0;
	}
	unsigned int JCZipFile::getFileSize( const char* p_pszFile ){
		int sz=0;
		int compsz=0;
		getSize(p_pszFile,sz,compsz);
		return sz;
	}
	bool JCZipFile::loadFileContent( const char* p_pszFile, char*& p_pBuff, int& p_iBuffSize ){
        p_iBuffSize = 0;
        int sz = 0;
        int compsz = 0;
        getSize(p_pszFile, sz, compsz);
        if (sz <= 0)
            return false;
        p_iBuffSize = sz;
        std::string file = p_pszFile;
        if (m_sInitDir.size() > 0) {
            file = m_sInitDir + "/" + p_pszFile;
        }
        zip_file *f = zip_fopen(m_pZip, file.c_str(), 0);
        p_pBuff = new char[sz+1];
        zip_fread(f, p_pBuff, sz);
        zip_fclose(f);
        p_pBuff[sz] = 0;
        return true;
	}
	bool JCZipFile::loadFileContent(const char* p_pszFile, ALLOCMEM alloc, void* pUserData, int& p_iBuffSize) {
		p_iBuffSize = 0;
		int sz = 0;
		int compsz = 0;
		getSize(p_pszFile, sz, compsz);
		if (sz <= 0)
			return false;
		p_iBuffSize = sz;
		std::string file = p_pszFile;
		if (m_sInitDir.size() > 0){
			file = m_sInitDir + "/" + p_pszFile;
		}
		zip_file *f = zip_fopen(m_pZip, file.c_str(), 0);
		unsigned char* pBuf = alloc(sz, pUserData);
		zip_fread(f, pBuf, sz);
		zip_fclose(f);
		return true;
	}
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
