
/*
	写到文件中的日志。
*/

#ifndef _LAYA_FILE_LOG_H__
#define _LAYA_FILE_LOG_H__
#include <stdio.h>
#include <stdarg.h>

namespace laya{
	class JCFileLog{
	public:
        JCFileLog();
		~JCFileLog();
		//打开日志文件，会清掉之前的。
		bool init(const char* p_pszFile );
		//是否自动加上回车
		void setAutoRet(bool b){ m_bAutoRet=b;};
		bool getAutoRet(){return m_bAutoRet;};

		//注意这个函数可能在多个线程调用。
		void Log(const char* pFormat,...);
		//同上。只是不用...,pArgs实际是很多参数
		//void Log1(const char* pFormat,const char* pArgs );
		void Log1(const char* pFormat, va_list pArgs );
	protected:
		FILE* m_pFile;
		bool m_bAutoRet;
	};
}

#endif
