#include "WinEditBox.h"
#include "../../source/conch/WindowsEnv/winWindows.h"
#include "util/Log.h"

extern HWND g_hWnd;

static HMENU IDL_EditBox = (HMENU) 100;

static void CheckError()
{
	DWORD error = GetLastError();
	if (error)
	{
		LOGE("[Error][Edit] Error no: %d", error);
	}
}

static LRESULT CALLBACK EditWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	laya::WinEditBox* editBox = (laya::WinEditBox*) GetWindowLong(hWnd, GWL_USERDATA);
	if (!editBox)
		return true;

	switch (message)
	{
	//case WM_SETFOCUS:
	//{
	//	laya::WinControl* winCtrl = laya::WinCtrlEvtManager::Get(hWnd);
	//	if (winCtrl)
	//	{
	//		winCtrl->OnSetFocus();
	//	}
	//	break;
	//}
	//case WM_KILLFOCUS:
	//{
	//	DestroyCaret();
	//	break;
	//}
	case WM_KEYDOWN:
	case WM_KEYUP:
	{
		SendMessage(g_hWnd, message, wParam, lParam);
		break;
	}
	case WM_NCCALCSIZE:
	{

		bool ret = editBox->OnNCCalcSize(wParam, lParam);

		if (!ret)
		{
			return CallWindowProc(editBox->GetDefaultWndProc(), hWnd, message, wParam, lParam);
		}

		break;
	}

	case WM_NCPAINT:
	{
		bool ret = editBox->OnNCPaint(wParam, lParam);
		if (!ret)
		{
			return CallWindowProc(editBox->GetDefaultWndProc(), hWnd, message, wParam, lParam);
		}
		break;
	}

	case WM_PAINT:
	{
		editBox->OnPaint();
		break;
	}
	default:
		return CallWindowProc(editBox->GetDefaultWndProc(), hWnd, message, wParam, lParam);
	}
	return true;
}

namespace laya {

	/// style
	void WinEditBox::Style::SetLeft(int val)
	{
		left = val;
		UpdateSizeOrDirty();
	}

	void WinEditBox::Style::SetTop(int val)
	{
		top = val;
		UpdateSizeOrDirty();
	}

	void WinEditBox::Style::SetWidth(int val)
	{
		width = val;
		UpdateSizeOrDirty();
	}

	void WinEditBox::Style::SetHeight(int val)
	{
		height = val;
		UpdateSizeOrDirty();
	}

	void WinEditBox::Style::SetFontSize(int val)
	{
		fontSize = val + 2;
		if (m_owner->IsFocus())
		{
			m_owner->UpdateFont();
			m_owner->ForceUpdateWindow();
		}
		else
		{
			isDirty = true;
		}
	}

	void WinEditBox::Style::SetBgColor(COLORREF val)
	{
		bgColor = val;
		UpdatePaintOrDirty();
	}

	void WinEditBox::Style::SetFontColor(COLORREF val)
	{
		fontColor = val;
		UpdatePaintOrDirty();
	}

	void WinEditBox::Style::UpdatePaintOrDirty()
	{
		if (m_owner->IsFocus())
		{
			m_owner->ForceUpdateWindow();
		}
		else
		{
			isDirty = true;
		}
	}

	void WinEditBox::Style::UpdateSizeOrDirty()
	{
		if (m_owner->IsFocus())
		{
			m_owner->UpdateSize();
			m_owner->ForceUpdateWindow();
		}
		else
		{
			isDirty = true;
		}
	}

	/// WinEditBox

	WinEditBox::WinEditBox() :
		m_hSingleEditWnd(0),
		m_hMultiEditWnd(0),
		m_isFocus(false),
		m_isMultiLine(false),
		m_isInitialized(false),
		m_refCount(0),
		m_defaultWndProc(NULL)
	{
		m_style = new Style(this);
		m_font = CreateFont(m_style->fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));

		// Send Message for creating
		SendMessage(g_hWnd, WMU_CREATE_CTRL, NULL, (LPARAM)this);
	}

	WinEditBox::~WinEditBox()
	{
		WinCtrlEvtManager::Remove(m_hSingleEditWnd);
		WinCtrlEvtManager::Remove(m_hMultiEditWnd);

		SetWindowLong(m_hSingleEditWnd, GWL_USERDATA, (LONG)0);
		SetWindowLong(m_hMultiEditWnd, GWL_USERDATA, (LONG)0);

		delete m_style;
		m_style = nullptr;

		DestroyWindow(m_hSingleEditWnd);
		DestroyWindow(m_hMultiEditWnd);
	}

	void WinEditBox::Init()
	{
		if (!m_isInitialized)
		{
			m_isInitialized = true;
			m_hSingleEditWnd = CreateWindow(WC_EDIT, TEXT(""), WS_CHILD | ES_AUTOHSCROLL, m_style->left, m_style->top, m_style->width, m_style->height, g_hWnd, IDL_EditBox, (HINSTANCE)GetWindowLong(g_hWnd, GWL_HINSTANCE), NULL);

			m_hMultiEditWnd = CreateWindow(WC_EDIT, TEXT(""), WS_CHILD | ES_MULTILINE | ES_WANTRETURN, m_style->left, m_style->top, m_style->width, m_style->height, g_hWnd, IDL_EditBox, (HINSTANCE)GetWindowLong(g_hWnd, GWL_HINSTANCE), NULL);


			CheckError();
			if (m_isMultiLine)
			{
				m_defaultWndProc = (WNDPROC)SetWindowLong(m_hMultiEditWnd, GWL_WNDPROC, (LONG)EditWndProc);
			}
			else
			{
				m_defaultWndProc = (WNDPROC)SetWindowLong(m_hSingleEditWnd, GWL_WNDPROC, (LONG)EditWndProc);
			}

			SetWindowLong(m_hSingleEditWnd, GWL_USERDATA, (LONG)this);
			SetWindowLong(m_hMultiEditWnd, GWL_USERDATA, (LONG)this);

			WinCtrlEvtManager::Add(m_hSingleEditWnd, this);
			WinCtrlEvtManager::Add(m_hMultiEditWnd, this);
		}
	}


	void WinEditBox::UpdateSize()
	{
		::SetWindowPos(GetCurHWND(), NULL, m_style->left, m_style->top, m_style->width, m_style->height, NULL);
	}

	void WinEditBox::UpdateFont()
	{
		m_font = CreateFont(m_style->fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));

		SendMessage(GetCurHWND(), WM_SETFONT, (WPARAM)m_font, true);
	}

	void WinEditBox::SetFocus(bool isFocus)
	{
		if (m_isFocus == isFocus)
		{
			return;
		}
		m_isFocus = isFocus;
		if (!GetCurHWND())
			m_isFocus = false;


		if (m_style->isDirty)
		{
			UpdateSize();
			UpdateFont();
			m_style->isDirty = false;
		}

		if (m_isFocus)
		{
			SetText(m_text.c_str());
		}

		if (m_isFocus)
		{
			::SetFocus(GetCurHWND());
			ShowWindow(GetCurHWND(), true);
		}
		else
		{
			::SetFocus(NULL);
			ShowWindow(GetCurHWND(), SW_HIDE);
		}

		ForceUpdateWindow();
	}

	void WinEditBox::SetMutiLine(bool val)
	{
		if (m_isMultiLine == val)
			return;

		SetWindowLong(GetCurHWND(), GWL_WNDPROC, (LONG)m_defaultWndProc);
		m_isMultiLine = val;

		m_defaultWndProc = (WNDPROC) SetWindowLong(GetCurHWND(), GWL_WNDPROC, (LONG)EditWndProc);

		if (m_isFocus)
		{
			ShowWindow(m_hSingleEditWnd, !m_isMultiLine);
			ShowWindow(m_hMultiEditWnd, m_isMultiLine);
		}

		ForceUpdateWindow();
	}

	void WinEditBox::ForceUpdateWindow()
	{
		RECT r;
		GetClientRect(GetCurHWND(), &r);
		InvalidateRect(GetCurHWND(), &r, true);
		::UpdateWindow(GetCurHWND());
	}

	void WinEditBox::RenderClient()
	{
		PAINTSTRUCT paint;
		HDC hdc = BeginPaint(GetCurHWND(), &paint);
		SetBkColor(hdc, m_style->bgColor);
		SetTextColor(hdc, m_style->fontColor);
		SelectObject(hdc, m_font);

		FillRect(hdc, &m_ncRect, CreateSolidBrush(m_style->bgColor));
		DrawText(hdc, m_text.c_str(), -1, &m_ncRect, DT_LEFT | DT_TOP);

		EndPaint(GetCurHWND(), &paint);
	}

	void WinEditBox::GetTextFromWindow()
	{
		int length = GetWindowTextLength(GetCurHWND());
		m_text.resize(length + 1);
		GetWindowText(GetCurHWND(), &(*m_text.begin()), length + 1);
	}

	const char* WinEditBox::GetText()
	{
		if (m_isFocus)
		{
			GetTextFromWindow();
		}

		return m_text.c_str();
	}

	void WinEditBox::SetText(const char* text)
	{
		m_text = text;
		SetWindowText(GetCurHWND(), m_text.c_str());

		if (m_isFocus)
		{
			ForceUpdateWindow();
		}
	}

	void WinEditBox::OnPaint()
	{
		GetClientRect(GetCurHWND(), &m_ncRect);
		RenderClient();
	}

	void WinEditBox::OnCtrlColor(HDC hdc)
	{
		if (!m_isMultiLine)
		{
			SetWindowPos(GetCurHWND(), NULL, 0, 0, 0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);
		}

		SetBkColor(hdc, m_style->bgColor);
		SetTextColor(hdc, m_style->fontColor);
	}

	bool WinEditBox::OnNCCalcSize(WPARAM wParam, LPARAM lParam)
	{
		if (m_isMultiLine)
			return false;

		bool isValid = (bool)wParam;
		if (isValid)
		{
			RECT r;
			GetClientRect(GetCurHWND(), &r);

			LPNCCALCSIZE_PARAMS lpParams = (LPNCCALCSIZE_PARAMS)lParam;
			lpParams->rgrc[0].top += (r.bottom - r.top) / 2;
		}

		return true;
	}

	bool WinEditBox::OnNCPaint(WPARAM wParam, LPARAM lParam)
	{
		if (m_isMultiLine)
			return false;

		RenderClient();

		return true;
	}

	void WinEditBox::OnSetFocus()
	{
		//PAINTSTRUCT paint;
		//RECT r;
		//CopyRect(&r, &m_ncRect);
		////HideCaret(m_hWnd);

		//HDC hdc = BeginPaint(GetCurHWND(), &paint);
		//DrawText(hdc, m_text.c_str(), -1, &r, DT_LEFT | DT_TOP | DT_CALCRECT);
		//EndPaint(GetCurHWND(), &paint);

		//HideCaret(GetCurHWND());
		//CreateCaret(GetCurHWND(), NULL, 1, m_style->fontSize);
		//SetCaretPos(r.right, r.top);
		//ShowCaret(GetCurHWND());
	}

	void WinEditBox::Retain()
	{
		m_refCount++;
	}

	bool WinEditBox::Release()
	{
		m_refCount--;
		if (m_refCount <= 0)
		{
			delete this;
			return true;
		}

		return false;
	}
};