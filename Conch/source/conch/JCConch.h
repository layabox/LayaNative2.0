/**
@file			JCConch.h
@brief			
@author			James
@version		1.0
@date			2017_11_28
*/

#ifndef __JCConch_H__
#define __JCConch_H__

#include <vector>
#include <util/JCCommonMethod.h>
#include "JCConchRender.h"
#ifndef WEBASM
    #include "JSWrapper/JSInterface/JSInterface.h"
    #include "JCThreadCmdMgr.h"
#endif

namespace laya
{
    enum JS_DEBUG_MODE
    {
        JS_DEBUG_MODE_OFF = 0,      //关闭
        JS_DEBUG_MODE_NORMAL,       //正常
        JS_DEBUG_MODE_WAIT,         //等待
    };

#ifndef WEBASM
    class JCFileResManager;
	class JCFileSource;
	class DebuggerAgent;
#endif
    class JCScriptRuntime;
	class JCConch
	{
	public:

		enum CMDMSG 
        {
			CMD_ActiveProcess = 0x400 + 3333 + 100,	        //0x400=WM_USER
			CMD_DeactiveProcess,
			CMD_ReloadProcess,
			CMD_CloseProcess,
			CMD_UrlBack,
			CMD_UrlForward,
            CMD_ClearRender,
            CMD_MgrStartThread,
			CMD_onOrientationChanged = 0x400 + 3333 + 110,	//屏幕切换已经完成。
		};

	public:

		JCConch( int nDownloadThreadNum,JS_DEBUG_MODE nJSDebugMode,int nJSDebugPort );

		~JCConch();

		void delAppRes();

		void onRunCmdInMainThread(int nCmd, int nParam1, int nParam2);

		void onAppStart();

        void reload();

        int urlHistoryLength();

        void urlBack();

        void urlGo(int s);

        void urlForward();

        void urlHistoryPush(const char* sUrl);

		void exit();

		void onClearMemory();

		void onAppDestory();

		void postCmdToMainThread(int nCmd, int nParam1, int nParam2);

		const char* getLocalStoragePath() 
        {
            return m_strLocalStoragePath.c_str(); 
        }

    public:

        static JCConch*			                s_pConch;
        static int64_t			                s_nUpdateTime;
        static std::shared_ptr<JCConchRender>	s_pConchRender;
        std::function<void(int, int, int)>      m_funcPostMsgToMainThread;
        std::string				                m_strLocalStoragePath;
        JCScriptRuntime*                        m_pScrpitRuntime;
#ifndef WEBASM
        static JCFileSource*	                s_pAssetsFiles;
        JCFileSource*			                m_pAssetsRes;
        std::string				                m_strStartJS;
        std::string                             m_sCachePath;
        JCThreadCmdMgr                          m_ThreadCmdMgr;
        int				                        m_nJSDebugPort;
        JS_DEBUG_MODE                           m_nJSDebugMode;
    #ifdef JS_V8
        DebuggerAgent*			                m_pDbgAgent;
    #elif JS_JSVM
        DebuggerAgent*			                m_pDbgAgent;
    #endif
        JCFileResManager*	                    m_pFileResMgr;
    protected:
        bool					                m_bDestroying;
        std::vector<std::string>                m_vUrlHistory;
        int                                     m_nUrlHistoryPos;
#endif
	};
};
//------------------------------------------------------------------------------


#endif //__JCConch_H__

//-----------------------------END FILE--------------------------------
