/**
@file			winWindows.cpp
@brief			
@author			James
@version		1.0
@date			2017_11_28
*/

#include "winWindows.h"
#include "AppMessage.h"
#include "../JCScriptRuntime.h"
#include "WinCtrl.h"

#include <CommCtrl.h>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"libEGL.lib")
#pragma comment(lib,"libGLESv2.lib")

HWND g_hWnd;
extern int g_nInnerWidth;
extern int g_nInnerHeight;

namespace laya
{
    /*
    这个函数中不应该直接处理，应该转换一下通过事件发出去
    */
    LRESULT WINAPI winWindows::WindowProc ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
    {
        LONG udata = GetWindowLongPtr(hWnd,GWL_USERDATA);
        winWindows* pWnd = (winWindows*)udata;
        LRESULT  lRet = 1; 

        if( uMsg>=WM_CMD_BASE && uMsg<=WM_CMD_MAX )
        {
            JCEventEmitter::evtPtr e(new  JCEventBase );
            e->m_nID=winWindows::Evt_RunCmd;
            e->m_nP1 = uMsg;
            e->m_nP2 = wParam;
            e->setDataAsInt(lParam);
            pWnd->emit(e);
            return 1;
        }
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        switch (uMsg)
        {
        case WM_CREATE:
            break;
        case WM_SIZE:
        {
            int width = LOWORD( lParam );
            int height = HIWORD( lParam );
            JCEventEmitter::evtPtr e(new  JCEventBase );
            e->m_nID=winWindows::Evt_WndResize;
            e->m_nP1=width;
            e->m_nP2=height;
            pWnd->emit(e);

            break;
        }
        case WM_PAINT:
            break;
        case WM_DESTROY:
        {
            JCEventEmitter::evtPtr e(new  JCEventBase );
            e->m_nID=winWindows::Evt_WndClose;
            pWnd->emit(e);
            PostQuitMessage(0);             
            break;
        }
        case WM_KEYDOWN:
        {
            inputEvent e;
            e.nTouchType = e.nType = E_ONKEYDOWN;
            strncpy(e.type, "keydown", 256);
            e.keyCode = wParam;
            if (JCScriptRuntime::s_JSRT)
            {
                JCScriptRuntime::s_JSRT->dispatchInputEvent(e);
            }
            break;
        }
        case WM_CHAR:
            break;
        case WM_KEYUP:
        {
            inputEvent e;
            e.nTouchType = e.nType = E_ONKEYUP;
            strncpy(e.type, "keyup", 256);
            e.keyCode = wParam;
            if (JCScriptRuntime::s_JSRT)
            {
                JCScriptRuntime::s_JSRT->dispatchInputEvent(e);
            }
            break;
        }
        case WM_LBUTTONDOWN:
        {
            inputEvent e;
            e.nTouchType = e.nType = E_ONMOUSEDOWN;
            e.posX = x;
            e.posY = y;
            strncpy(e.type, "mousedown", 256);
            if (JCScriptRuntime::s_JSRT)
                JCScriptRuntime::s_JSRT->dispatchInputEvent(e);
            break;
        }
        case WM_RBUTTONDOWN:
        {
            inputEvent e;
            e.nTouchType = e.nType = E_ONRIGHTMOUSEDOWN;
            e.posX = x;
            e.posY = y;
            strncpy(e.type, "rightmousedown", 256);
            if (JCScriptRuntime::s_JSRT)
                JCScriptRuntime::s_JSRT->dispatchInputEvent(e);
            break;
        }
        case WM_LBUTTONUP:
        {
            inputEvent e;
            e.nTouchType = e.nType = E_ONMOUSEUP;
            e.posX = x;
            e.posY = y;
            strncpy(e.type, "mouseup", 256);
            if (JCScriptRuntime::s_JSRT)
            {
                JCScriptRuntime::s_JSRT->dispatchInputEvent(e);
            }
            break;
        }
        case WM_RBUTTONUP:
        {
            inputEvent e;
            e.nTouchType = e.nType = E_ONRIGHTMOUSEUP;
            e.posX = x;
            e.posY = y;
            strncpy(e.type, "rightmouseup", 256);
            if (JCScriptRuntime::s_JSRT)
            {
                JCScriptRuntime::s_JSRT->dispatchInputEvent(e);
            }
            break;
        }
        case WM_MOUSEMOVE:
        {
            inputEvent e;
            e.nTouchType = e.nType = E_ONMOUSEMOVE;
            strncpy(e.type, "mousemove", 256);
            e.posX = x;
            e.posY = y;
            if (JCScriptRuntime::s_JSRT)
            {
                JCScriptRuntime::s_JSRT->dispatchInputEvent(e);
            }
            break;
        }
            
        case WM_MOUSEWHEEL:
        {
            inputEvent e;
            e.nTouchType = e.nType = E_ONMOUSEWHEEL;
            strncpy(e.type, "mousewheel", 256);
            e.posX = x;
            e.posY = y;
            short nWheel = (short)HIWORD(wParam);
            e.nWheel = nWheel;
            if (JCScriptRuntime::s_JSRT)
            {
                JCScriptRuntime::s_JSRT->dispatchInputEvent(e);
            }
            break;
        }

		case WMU_CREATE_CTRL:
		{
			WinControl* winCtrl = (WinControl*)lParam;
			winCtrl->Init();
			break;
		}
		case WM_CTLCOLOREDIT:
		{
			HWND hwnd = (HWND)lParam;
			WinControl* ctrl = WinCtrlEvtManager::Get(hwnd);
			if (WinControl::IsEidt(ctrl))
			{
				HDC hdc = (HDC)wParam;
				ctrl->OnCtrlColor(hdc);
			}
			break;
		}

        case WM_INPUTLANGCHANGE://切换输入法的时候会触发
            break;
        case WM_IME_SETCONTEXT:
            break;
        case WM_IME_STARTCOMPOSITION://开始使用ime，要进行初始化
            break;
        case WM_IME_COMPOSITION://ime的消息监测，出现输入法的时候，英文输入的内容在这可以获得,中文的候选框，在NOTYFY中获得
            break;
        case WM_IME_ENDCOMPOSITION://结束ime。一个输入已经完成
            break;
        case WM_IME_NOTIFY://ime的消息处理，中文的候选框在这获得
            break;

        default: 
            lRet = DefWindowProc (hWnd, uMsg, wParam, lParam);
            break; 
        } 
        return lRet; 
    }
    winWindows::winWindows()
    {
        m_hWnd = 0;
        m_AllListener.resize(winWindows::Evt_Num);
    }
    bool winWindows::create(int x, int y, int w, int h )
    {
        WNDCLASS wndclass = {0}; 
        RECT     windowRect;
        HINSTANCE hInstance = GetModuleHandle(NULL);
        wndclass.style         = CS_OWNDC;
        wndclass.lpfnWndProc   = (WNDPROC)winWindows::WindowProc; 
        wndclass.hInstance     = hInstance; 
		wndclass.hbrBackground = NULL;// (HBRUSH)GetStockObject(BLACK_BRUSH);
        wndclass.lpszClassName = TEXT("opengles3.0");

        //如果要多次创建窗口，则需要先执行UnReg
        UnregisterClass(wndclass.lpszClassName, hInstance);
        if (!RegisterClass (&wndclass) ) return FALSE; 
        DWORD wStyle =  WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN ;//WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION;
        //DWORD wStyle = WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_POPUP;
        g_nInnerWidth=w;
        g_nInnerHeight=h;
        windowRect.left = 0;
        windowRect.top = 0;
        windowRect.right = w;
        windowRect.bottom = h;
        AdjustWindowRect ( &windowRect, wStyle, FALSE );
        m_hWnd = CreateWindow(
            TEXT("opengles3.0"),TEXT("LAYABOX-2.0"),wStyle,0,0,
            windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top,
            NULL,NULL,hInstance,NULL);

        g_hWnd = m_hWnd;
        SetWindowLongPtr ( m_hWnd, GWL_USERDATA, (LONG) (LONG_PTR) this );



        if ( m_hWnd == NULL )
            return false;
        ShowWindow ( m_hWnd, TRUE );
        return true;
    }
    void winWindows::show(bool b)
    {
        ShowWindow ( m_hWnd, b?TRUE:FALSE );
    }
    void winWindows::close() 
    {
        PostMessage(m_hWnd, WM_QUIT, 0, 0);
    }
    void winWindows::handleMessage( UPDATEFUNC pUpdate)
    {
       MSG msg = { 0 };
       while (true)
       {
          int gotMsg = (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0);
          if ( gotMsg )
          {
             if (msg.message==WM_QUIT)
             {
                 break;//结束 
             }
             else
             {
                 TranslateMessage(&msg); 
                 DispatchMessage(&msg); 
             }
             if(pUpdate)pUpdate();
          }
       }
    }
};
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------