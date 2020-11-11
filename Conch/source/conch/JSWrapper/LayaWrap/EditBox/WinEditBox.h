#pragma once
#include <Windows.h>
#include <CommCtrl.h>
#include "../../source/conch/WindowsEnv/WinCtrl.h"
#include <string>

namespace laya {

	class WinEditBox final : public WinControl{
	public:
		class Style {
		public:
			Style(WinEditBox* owner) : m_owner(owner)
			{
				bgColor = 0xffffff;
				fontColor = 0x0;
				left = -1000;
				top = -1000;
				width = 0;
				height = 0;
				fontSize = 10;

				isDirty = false;
			}

			void SetLeft(int val);
			void SetTop(int val);
			void SetWidth(int val);
			void SetHeight(int val);
			void SetFontSize(int val);
			void SetBgColor(COLORREF val);
			void SetFontColor(COLORREF val);


		private:
			void UpdatePaintOrDirty();
			void UpdateSizeOrDirty();

		private:
			friend class WinEditBox;

			COLORREF bgColor;
			COLORREF fontColor;
			int left;
			int top;
			int width;
			int height;
			int fontSize;

			bool isDirty;

			WinEditBox* m_owner;
		};

	public:
		WinEditBox();
		virtual void Init() override;
		virtual ControlType GetType() override { return CT_Edit; }

		WNDPROC GetDefaultWndProc() const { return m_defaultWndProc; }

		Style& GetStyle() { return *m_style; }

		bool IsFocus() const { return m_isFocus; }
		void SetFocus(bool isFocus);

		void SetText(const char* text);
		const char* GetText();

		void SetMutiLine(bool val);

		void ForceUpdateWindow();


		// event
		virtual void OnPaint() override;
		virtual void OnCtrlColor(HDC hdc) override;
		virtual void OnSetFocus() override;

		bool OnNCCalcSize(WPARAM wParam, LPARAM lParam);
		bool OnNCPaint(WPARAM wParam, LPARAM lParam);

		// ref count
		void Retain();
		bool Release();

	private:
		void UpdateSize();
		void UpdateFont();
		void RenderClient();
		void GetTextFromWindow();

		HWND GetCurHWND() { return m_isMultiLine ?  m_hMultiEditWnd : m_hSingleEditWnd;
		}

	private:
		WinEditBox(const WinEditBox& other) = delete;
		void operator=(const WinEditBox& other) = delete;
		~WinEditBox();

	private:		
		bool m_isInitialized;

		bool m_isFocus;
		std::string m_text;

		bool m_isMultiLine;

		Style* m_style;
		HWND m_hSingleEditWnd;
		HWND m_hMultiEditWnd;
		HFONT m_font;

		WNDPROC m_defaultWndProc;

		RECT m_ncRect;

		int m_refCount;
	};
};