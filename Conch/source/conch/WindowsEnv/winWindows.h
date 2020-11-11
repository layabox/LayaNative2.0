/**
@file			winWindows.h
@brief			
@author			James
@version		1.0
@date			2017_11_28
*/

#ifndef __winWindows_H__
#define __winWindows_H__


#include <Windows.h>
#include <event/JCEmitter.h>

#define WMU_CREATE_CTRL (WM_USER + 1)
#define WMU_DESTORY_CTRL (WM_USER + 2)


namespace laya
{
	class winWindows:public JCEventEmitter
    {
	public:

		//消息循环的每帧都会调用的函数
		typedef void (*UPDATEFUNC)();

		static void handleMessage( UPDATEFUNC pUpdate);

		enum winWindowsEvent
        {
			Evt_WndResize=0,
			Evt_WndClose=1,
			Evt_RunCmd=2,
			Evt_KeyUp=3,
			Evt_Num,
		};

		winWindows();

		bool create(int x, int y, int w, int h );

		void resize(int w, int h);

		void show( bool b);

		void close();

		static LRESULT WINAPI WindowProc ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	public:

		HWND	m_hWnd;

	};
}
//------------------------------------------------------------------------------


#endif //__winWindows_H__

//-----------------------------END FILE--------------------------------