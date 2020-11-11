
#include "JCFlog.h"
#include <stdarg.h>
#include "../util/JCCommonMethod.h"
#pragma warning(disable:4996)

namespace laya{
    JCFileLog::JCFileLog(){
		m_pFile = NULL;
		m_bAutoRet = true;
	}

    JCFileLog::~JCFileLog(){
		if(m_pFile){
			fclose(m_pFile);
		}
	}

	bool JCFileLog::init(const char* p_pszFile ){
		m_pFile = fopen(p_pszFile,"w");
		return m_pFile!=NULL;
	}
	
	void JCFileLog::Log(const char* pFormat,...){
		if(m_pFile==NULL)
			return;
		double curtm = tmGetCurms();
		int* ptm = (int*)&curtm;
		fprintf( m_pFile, "[%d,%d ]",ptm[1],ptm[0]);
		va_list args;
		va_start(args,pFormat);
		vfprintf(m_pFile,pFormat,args);// (char *)(&pFormat+1));
		va_end(args);
		if(m_bAutoRet){
			fprintf(m_pFile,"\n");
		}

		fflush(m_pFile);
	}

	void JCFileLog::Log1(const char* pFormat,va_list pArgs ){
		if(m_pFile==NULL)
			return;
		double curtm = tmGetCurms();
		int* ptm = (int*)&curtm;
		fprintf( m_pFile, "[%d,%d ]",ptm[1],ptm[0]);
		vfprintf(m_pFile,pFormat,pArgs);
		if(m_bAutoRet){
			fprintf(m_pFile,"\n");
		}

		fflush(m_pFile);
	}
}