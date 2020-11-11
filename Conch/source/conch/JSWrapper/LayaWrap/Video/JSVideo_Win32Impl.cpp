#include "JSVideo.h"

#if (!defined ANDROID) && (!defined __APPLE__)

namespace laya
{
	std::set<std::string> JSVideo::ms_supportFormat = {
		//"video/mp4",
		//"video/ogg",
		//"video/webm"
	};

	class Win32VideoHandler final : public IVideoHandler
	{
	public:
		Win32VideoHandler()
		{
		}

		virtual ~Win32VideoHandler()
		{
		}

		virtual bool isFrameUpdated()
		{
			NOT_IMPLEMENT_RET(false);
		}

		virtual void updateBitmapData(BitmapData* bitmapData)
		{
			NOT_IMPLEMENT();
		}
	};

	JSVideo::JSVideo()
	{
		Init();

		AdjustAmountOfExternalAllocatedMemory(sizeof(JSVideo));
		JCMemorySurvey::GetInstance()->newClass("JSVideo", sizeof(JSVideo), this);

		m_pVideoHandler = new Win32VideoHandler;
		m_pJCVideo->setVideoHandler(m_pVideoHandler);
	}

	JSVideo::~JSVideo()
	{
		Dispose();

		JCMemorySurvey::GetInstance()->releaseClass("JSVideo", this);
	}


	void JSVideo::_releaseHandler()
	{
	}

	void JSVideo::LoadInternal(const std::string& path)
	{
		LOGI("%s", path.c_str());
		CallHandle("loadedmetadata");
	}

	void JSVideo::Play()
	{
		NOT_IMPLEMENT();
	}

	void JSVideo::Pause()
	{
		NOT_IMPLEMENT();
	}


	void JSVideo::Stop()
	{
		NOT_IMPLEMENT();
	}


	bool JSVideo::GetPaused()
	{
		NOT_IMPLEMENT_RET(true);
	}

	bool JSVideo::GetLoop()
	{
		NOT_IMPLEMENT_RET(false);
	}

	void JSVideo::SetLoop(bool value)
	{
		NOT_IMPLEMENT();
	}

	void JSVideo::SetAutoplay(bool value)
	{
		NOT_IMPLEMENT();
	}

	bool JSVideo::GetAutoplay()
	{
		NOT_IMPLEMENT_RET(false);
	}

	double JSVideo::GetVideoWidth()
	{
		NOT_IMPLEMENT_RET(0);
	}

	double JSVideo::GetVideoHeight()
	{
		NOT_IMPLEMENT_RET(0);
	}

	double JSVideo::GetWidth()
	{
		NOT_IMPLEMENT_RET(0);
	}

	void JSVideo::SetWidth(double val)
	{
		NOT_IMPLEMENT();
	}

	double JSVideo::GetHeight()
	{
		NOT_IMPLEMENT_RET(0);
	}

	void JSVideo::SetHeight(double val)
	{
	}

	double JSVideo::GetCurrentTime()
	{
		NOT_IMPLEMENT_RET(0);
	}

	void JSVideo::SetCurrentTime(double val)
	{
		NOT_IMPLEMENT();
	}

	double JSVideo::GetDuration()
	{
		NOT_IMPLEMENT_RET(0);
	}

	double JSVideo::GetVolume()
	{
		NOT_IMPLEMENT_RET(0);
	}

	void JSVideo::SetVolume(double val)
	{
		NOT_IMPLEMENT();
	}

	void JSVideo::SetX(double val)
	{
		NOT_IMPLEMENT();
	}

	double JSVideo::GetX()
	{
		NOT_IMPLEMENT_RET(0);
	}

	void JSVideo::SetY(double val)
	{
		NOT_IMPLEMENT();
	}

	double JSVideo::GetY()
	{
		NOT_IMPLEMENT_RET(0);
	}

	int32_t JSVideo::GetReadyState()
	{
		NOT_IMPLEMENT_RET(1);
	}

};

#endif




