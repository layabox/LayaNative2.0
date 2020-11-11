#include "WinCtrl.h"

namespace laya {
	std::map<HWND, WinControl*> WinCtrlEvtManager::ms_ctrlMap;


	bool WinCtrlEvtManager::Add(HWND hwnd, WinControl* ctrl)
	{
		if (!ctrl)
			return false;

		if (!hwnd)
			return false;

		auto it = ms_ctrlMap.find(hwnd);
		if (it != ms_ctrlMap.end())
		{
			it->second = ctrl;
		}
		else
		{
			ms_ctrlMap.insert(std::make_pair(hwnd, ctrl));
		}

		return true;
	}

	void WinCtrlEvtManager::Remove(HWND hwnd)
	{
		auto it = ms_ctrlMap.find(hwnd);
		if (it != ms_ctrlMap.end())
		{
			ms_ctrlMap.erase(it);
		}
	}

	WinControl* WinCtrlEvtManager::Get(HWND hwnd)
	{
		auto it = ms_ctrlMap.find(hwnd);
		if (it != ms_ctrlMap.end())
		{
			return it->second;
		}

		return nullptr;
	}

	void WinCtrlEvtManager::Clear()
	{
		auto it = ms_ctrlMap.begin();
		while (it != ms_ctrlMap.end())
		{
			it = ms_ctrlMap.erase(it);
		}
	}
};