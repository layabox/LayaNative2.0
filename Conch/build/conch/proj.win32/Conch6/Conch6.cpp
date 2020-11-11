/**
@file			Conch6.cpp
@brief			
@author			James
@version		1.0
@date			2017_11_28
*/

#include "stdafx.h"
#include <windows.h>
#include <string>
#include "../../../../Conch/source/conch/WindowsEnv/JCConchApp.h"
#include "../../../../Conch/source/conch/JCSystemConfig.h"
#include "../../../../Conch/source/conch/WindowsEnv/winWindows.h"
#include <downloadCache/JCIosFileSource.h>
#ifdef _TEST_
    #include "../../../../Conch/source/conch/Test/JCTestManager.h"
#endif
#include <filesystem>

namespace  fs = std::experimental::filesystem::v1;
using namespace laya;
extern std::string gRedistPath;
extern int g_nInnerWidth;
extern int g_nInnerHeight;
/*
用法：
    conch6.exe [options] url
参数：
    -test
        只执行c的测试用例。这时候不再使用url
    -perf
        执行一个url进行统计
    -perf_count
        update的次数
    -opath    path
        指定效率测试的输出目录
    -debug=""
*/
static std::string	GetTypeString(LPCTSTR section_name, LPCTSTR key_name, LPCTSTR ini_file_path)
{
    const int kMaxString = 256;
    TCHAR buffa[kMaxString];
    GetPrivateProfileString(section_name, key_name, "not found", buffa, kMaxString, ini_file_path);
    return buffa;
}
int main(int argc, _TCHAR* argv[])
{
    bool bRunTest = false;
    char* pRunTestCase = NULL;
    //解析参数
    for (int i = 1; i < argc; i++) 
    {
        if (argv[i][0] != '-') 
        {
            if (g_kSystemConfig.m_strStartURL.length() <= 0) 
            {
                g_kSystemConfig.m_strStartURL = argv[i];
            }
            else
            {
                printf(" 参数错误！只能有一个启动url，否则就是参数，需要以‘-’开头!\n ");
                return 1;
            }
            if (memcmp(argv[i], "http", 4) != 0) 
            {
                g_kSystemConfig.m_strStartURL = "http://127.0.0.1:9988/";
                g_kSystemConfig.m_strStartURL += argv[i];
            }
        }
        else 
        {
            char* cargv = (char*)argv[i] + 1;
            if (memcmp(cargv, "test", 4) == 0) 
            {
                bRunTest = true;
                if (strlen(cargv) > 4) {
                    pRunTestCase = cargv + 4;
                }
            }
            else if (memcmp(cargv, "perf", 4) == 0) 
            {
                g_kSystemConfig.m_bPerfStat = true;
            }
            else if (memcmp(cargv, "opath", 5) == 0) 
            {
                g_kSystemConfig.m_strPerfOut = cargv + 5;
            }
            else if (memcmp(cargv, "perf_count", 10) == 0) 
            {
                g_kSystemConfig.m_nPerf_UpdateNum = atoi(cargv + 10);
            }
            else if (memcmp(cargv, "debug_port", 10) == 0)
            {
                //Conch::s_nDebugPort = atoi(cargv + 10);
            }
            else 
            {
                printf("Unknown param:%s\n", cargv);
            }
        }
    }
    TCHAR   szPath[MAX_PATH];
    ::GetModuleFileName(NULL, szPath, MAX_PATH);
    gRedistPath.append(szPath, strlen(szPath) - 10);
    printf("start .exePath=%s\n", gRedistPath.c_str());
    //根据配置文件设置宽高
    fs::path configpath(szPath);
    configpath.remove_filename();
    configpath /= "config.ini";
    if (!fs::exists(configpath)) 
    {
        printf("No config.ini file!\n");
    }
    std::string strW = GetTypeString("default", "scrWidth", configpath.generic_string().c_str());
    std::string strH = GetTypeString("default", "scrHeight", configpath.generic_string().c_str()); 
    if (strW.length() > 0) 
    {
        g_nInnerWidth = atoi(strW.c_str());
    }
    if (strH.length() > 0) 
    {
        g_nInnerHeight = atoi(strH.c_str());
    }
    if (g_nInnerHeight == 0 || g_nInnerHeight == 0) 
    {
        printf(" The value of default.scrHeight or default.scrWidth in config.ini is wrong.\n");
        throw - 1;
    }
    int nJSDebugMode = 0;
    int nJSDebugPort = 0;
    std::string cfgDebug = GetTypeString("default", "JSDebugPort", configpath.generic_string().c_str());
    if (cfgDebug.length() > 0) 
    {
        nJSDebugPort = atoi(cfgDebug.c_str());
    }
    std::string cfgBreakOnFirst = GetTypeString("default", "JSDebugMode", configpath.generic_string().c_str());
    if (cfgBreakOnFirst.length() > 0)
    {
        nJSDebugMode = atoi(cfgBreakOnFirst.c_str());
    }
    std::string cfgThreadMode = GetTypeString("default", "ThreadMode", configpath.generic_string().c_str());
    if (cfgThreadMode.length() > 0)
    {
        THREAD_MODE nMode = (THREAD_MODE)(atoi(cfgThreadMode.c_str()));
        if (nMode == THREAD_MODE_SINGLE)
        {
            g_kSystemConfig.m_nThreadMODE = nMode;
            LOGI(">>>>>>Thread Mode = single");
        }
        else if (nMode == THREAD_MODE_DOUBLE)
        {
            g_kSystemConfig.m_nThreadMODE = nMode;
            LOGI(">>>>>>Thread Mode = double");
        }
        else
        {
            LOGI(">>>>>>Thread Mode = %d", g_kSystemConfig.m_nThreadMODE);
        }
    }
    JCIosFileSource* pAssets = new JCIosFileSource();
    pAssets->Init(gRedistPath.c_str());
    JCConch::s_pAssetsFiles = pAssets;
    if (bRunTest) 
    {
#ifdef _TEST_
        JCTestMgr* pTest = JCTestMgr::getInstance();
        pTest->run(pRunTestCase);
#endif
        //JSMemorySurvey::DelInstance();
        //svFileCache::delInstance();
        //delete pAssets;
        return 0;
    }
    //if (g_kSystemConfig.m_bPerfStat) {
    //    gRunStat.strTestID = g_kSystemConfig.m_strStartURL;
    //}
    JCConchApp app(nJSDebugMode,nJSDebugPort);
    app.startApp();
    winWindows::handleMessage(NULL);
    //app.handleMessage();
    app.exitApp();
    //delete pAssets;
    return 0;
}

//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
