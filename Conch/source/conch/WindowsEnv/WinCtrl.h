#pragma once
#include <map>
#include <Windows.h>

namespace laya {
	enum ControlType {
		CT_Edit,
	};

	class WinControl {
	public:
		static bool IsEidt(WinControl* winCtrl)
		{
			return winCtrl && winCtrl->GetType() == CT_Edit;
		}

	public:
		virtual void Init() = 0;
		virtual ControlType GetType() = 0;
		
		// call back
		virtual void OnCtrlColor(HDC hdc) {}
		virtual void OnSetFocus() {}
		virtual void OnPaint() {};

	};


	class WinCtrlEvtManager {
	public:
		static bool Add(HWND hwnd, WinControl* ctrl);
		static void Remove(HWND hwnd);
		static WinControl* Get(HWND hwnd);
		static void Clear();

	private:
		WinCtrlEvtManager() {}

	private:
		static std::map < HWND, WinControl*> ms_ctrlMap;
	};
};