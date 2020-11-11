#ifdef _TEST_
#include "JCTestManager.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <stdarg.h>
#include <thread>
#include <util/JCCommonMethod.h>
#include <stdarg.h>

template<>
std::string JCTestMgr::toString(char t){
	char str[8];
	sprintf(str,"%d",(int)t);
	return str;
}

template<>
std::string JCTestMgr::toString(short t){
	char str[8];
	sprintf(str,"%d",(int)t);
	return str;
}

template<>
std::string JCTestMgr::toString(int t){
	char str[16];
	sprintf(str,"%d",(int)t);
	return str;
}

template<>
std::string JCTestMgr::toString(unsigned int t) {
    char str[16];
    sprintf(str, "%d", (int)t);
    return str;
}

template<>
std::string JCTestMgr::toString(float t){
	char str[32];
	sprintf(str,"%f",t);
	return str;
}

template<>
std::string JCTestMgr::toString(bool t){
	char str[32];
	sprintf(str,"%s",t?"true":"false");
	return str;
}

template<>
std::string JCTestMgr::toString(std::string t){
	return t;
}

void JCTestMgr::output(const char* szFormat, ...){
	char szBuff[1024];
	va_list arg;
	va_start(arg, szFormat);
    #ifdef WIN32
	_vsnprintf(szBuff, sizeof(szBuff), szFormat, arg);
    #else
    vsnprintf(szBuff, sizeof(szBuff), szFormat, arg);
    #endif
	va_end(arg);
#ifdef WIN32
	OutputDebugString(szBuff);
#endif
	printf(szBuff);
	printf("\n");
}

void JCTestMgr::printResult( RunContext& p_RunCtx ){
	if( p_RunCtx.bOK){
		output("OK:%s\n", p_RunCtx.strDesc.c_str());//绿色
	}else{
		output("%s(%d): error: %s\n", p_RunCtx.pszFile?p_RunCtx.pszFile:"unkown file", p_RunCtx.nLine, p_RunCtx.strDesc.c_str());//红色
	}
}

void JCTestMgr::run(const char* pRunTestCase){
	for( int i=0,sz=m_AllTestCases.size(); i<sz; i++){
		JCTestCase& curTest= m_AllTestCases[i];
		if(pRunTestCase && strcmp(pRunTestCase, curTest.strDesc.c_str())!=0)
			continue;
		output("          %s\n",curTest.strDesc.c_str());
		output("=============================================\n");
		curTest.testFunc();
	}

	//打印最终结果
	output("测试结果:%d/%d\n", m_nOKNum, m_nTotalNum );
}

void Sleep(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void WaitUtil(bool* pCond, int timeout) {
    auto tm = laya::tmGetCurms();
    while (!*pCond) {
        if (timeout > 0 && (laya::tmGetCurms() - tm) > timeout)
            break;
        Sleep(100);
    }
}

#endif
