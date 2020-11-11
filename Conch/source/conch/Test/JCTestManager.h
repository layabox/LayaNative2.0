#ifdef _TEST_
#ifndef __TESTMANAGER_H_0115__
#define __TESTMANAGER_H_0115__

#define _RUN_TEST_
#ifdef _RUN_TEST_
#define ADDTESTCASE(func,desc)	JCTest gCTest##func(func,desc);
#else
#define ADDTESTCASE()
#endif

#include <string>
#include <vector>

/*
	执行测试并且打印结果
*/
class JCTestMgr{
public:
	struct JCTestCase{
		typedef void (*TESTFUNC)();
		TESTFUNC	testFunc;
		std::string		strDesc;
	};

	struct RunContext{
		const char* pszFile;
		int			nLine;
		std::string strDesc;
		bool		bOK;
        RunContext(){
			pszFile = NULL;
			bOK=false;
			nLine = 0;
		}
	};

	static JCTestMgr* getInstance(){
		static JCTestMgr gTestMgr;
		return  &gTestMgr;
	}

	JCTestMgr(){
		m_nTotalNum = 0;
		m_nOKNum = 0;
	}

	void addTestCase(JCTestCase::TESTFUNC func, const char* desc ){
		JCTestCase tc = {func, desc};
		m_AllTestCases.push_back(tc);
	}

	void run(const char* pRunTestCase);

	void output(const char* szFormat, ...);
	
	void printResult( RunContext& p_RunCtx );

	template<class T>
	std::string toString(T t);

	template<class T>
	void verifyEq(T t1, T t2, const char* pDesc, const char* pFile, int pnLine ){
		m_nTotalNum++;
		m_runCtx.pszFile = pFile;
		m_runCtx.nLine = pnLine;
		m_runCtx.strDesc = pDesc;
		if( t1!=t2 ){
			std::string desc = "Error!";
			desc += pDesc;
			desc = desc+ "。期望值是:"+toString(t2)+",实际是:"+toString(t1)+"\n";
			m_runCtx.strDesc=desc;
			m_runCtx.bOK = false;
		}else{
			m_nOKNum++;
			m_runCtx.bOK = true;
		}
		printResult(m_runCtx);
	}

	template<class T>
	void verifyEqRange(T t1, T t2, T range, const char* pDesc, const char* pFile, int pnLine ){
		m_nTotalNum++;
		m_runCtx.pszFile = pFile;
		m_runCtx.nLine = pnLine;
		m_runCtx.strDesc = pDesc;
		if( (t1-t2)>range ||(t2-t1)>range ){
			std::string desc = "Error!";
			desc += pDesc;
			desc = desc+ "。期望值是范围:"+toString(t2)+"+"+toString(range)+",实际是:"+toString(t1)+"\n";
			m_runCtx.strDesc=desc;
			m_runCtx.bOK = false;
		}else{
			m_nOKNum++;
			m_runCtx.bOK = true;
		}
		printResult(m_runCtx);
	}

	template<class T>
	void verifyNEq(T t1, T t2, const char* pDesc, const char* pFile, int pnLine ){
		m_nTotalNum++;
		m_runCtx.pszFile = pFile;
		m_runCtx.nLine = pnLine;
		m_runCtx.strDesc = pDesc;
		if( t1==t2 ){
			std::string desc = "Error!";
			desc += pDesc;
			desc = desc+ "。期望值是不要等于:"+toString(t2)+",实际是:"+toString(t1)+"\n";
			m_runCtx.strDesc=desc;
			m_runCtx.bOK = false;
		}else{
			m_nOKNum++;
			m_runCtx.bOK = true;
		}
		printResult(m_runCtx);
	}

protected:
	RunContext		m_runCtx;
	int				m_nTotalNum;
	int				m_nOKNum;
	std::vector<JCTestCase>	m_AllTestCases;
};


class JCTest{
public:
	JCTest( JCTestMgr::JCTestCase::TESTFUNC func, const char* desc){
		JCTestMgr::getInstance()->addTestCase(func,desc);
	}
};

#define VERIFYEQ(v,s,desc)		JCTestMgr::getInstance()->verifyEq(v,s,desc,__FILE__,__LINE__);
#define VERIFYNEQ(v,ns,desc)	JCTestMgr::getInstance()->verifyNEq(v,ns,desc,__FILE__,__LINE__);
#define VERIFYEQR(v,s,r,desc)	JCTestMgr::getInstance()->verifyEqRange(v,s,r,desc,__FILE__,__LINE__);

void Sleep(int ms);

void WaitUtil(bool* pCond, int timeout);

#endif
#endif
