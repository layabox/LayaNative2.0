/**
@file			JCConchApp.h
@brief			
@author			James
@version		1.0
@date			2017_11_28
*/

#ifndef __JCConchApp_H__
#define __JCConchApp_H__


#include <Windows.h>
#include <thread>
#include <misc/JCWorkerThread.h>
#include "gles.h"
#include "../JCConch.h"
#include <map>
#include <string>

extern HWND g_hWnd;
namespace laya
{
	class GLEnvThread;
	class winWindows;
	class FreeTypeFontRender;
	class JCConchApp
    {
	public:
		JCConchApp(int nJSDebugMode,int nJSDebugPort);

		~JCConchApp();

		void startApp();
		//
		void exitApp();

		//windows 已经创建好了，下面可以开始渲染，js等的初始化了
		void onGLReady(HWND hwnd);

		void onPause();

		void onResume();

		void onDestroy();

		void onKeyEvent();

		void onMouseEvent();

		void onUpdate(float dt);

		void onIdle();

		void postMsgToWindow(unsigned int nMsg, unsigned int param1, unsigned long param2);

		//与上面的函数的区别是，nCmd会转换成一个WM消息。所以可以是0到2048的任意值
		void postCmdToWindow(unsigned int nCmd, unsigned int param1, unsigned long param2);

		char* getFontfilePath(char* pFontfilePath);

	protected:

		void            onRenderThreadStart(JCEventEmitter::evtPtr evt);
		void            onRenderThreadStop(JCEventEmitter::evtPtr evt);

	protected:

		bool            m_bUseUpdate;		//是否需要调用本类的onUpdate
		winWindows*	    m_pWindows;
		GLEnvThread*    m_pGLEnv;
		JCConch*        m_pConch;
		bool            m_bExiting;

	};
}
//------------------------------------------------------------------------------


#endif //__JCConchApp_H__

//-----------------------------END FILE--------------------------------