#ifdef ANDROID
#include "../../../CToJavaBridge.h"
#include "JSVideo.h"

namespace laya
{
	static const char* s_className = "layaair/game/browser/LayaVideoPlayer";

	std::set<std::string> JSVideo::ms_supportFormat = {
		"video/mp4",
		"video/ogg",
	};

	struct AndroidVideoHandler : public IVideoHandler
	{
		AndroidVideoHandler()
		{
			obj = nullptr;
		}

		virtual ~AndroidVideoHandler()
		{
		}

		virtual bool isFrameUpdated()
		{
			bool ret = false;
			if (obj == nullptr)
				return ret ;

			CToJavaBridge::GetInstance()->callObjRetMethod(obj, s_className, "isFrameAvailable", &ret);
			return ret;
		}
		virtual void updateBitmapData(BitmapData* bitmapData)
		{
			if (obj == nullptr)
			{
				return;
			}

			int64_t ptr = reinterpret_cast<int64_t>(bitmapData);
//			LOGI("[Debug][Video] ptr is %ld", ptr);
			CToJavaBridge::GetInstance()->callObjVoidMethod(obj, s_className, "updateBitmap", ptr);
		}

		jobject obj;
	};

	static jobject& GetObj(IVideoHandler* videoHandler)
	{
		AndroidVideoHandler* pHandler = static_cast<AndroidVideoHandler*>(videoHandler);
		return pHandler->obj;
	}

	static void EmptyObj(IVideoHandler* videoHandler)
	{
		AndroidVideoHandler* pHandler = static_cast<AndroidVideoHandler*>(videoHandler);
		pHandler->obj = nullptr;
	}

	JSVideo::JSVideo()
	{
		Init();

//		LOGI("[Debug][Video] call JSVideo");

		AdjustAmountOfExternalAllocatedMemory(sizeof(JSVideo));
		JCMemorySurvey::GetInstance()->newClass("JSVideo", sizeof(JSVideo), this);

		m_pVideoHandler = new AndroidVideoHandler();
		
		CToJavaBridge::GetInstance()->newObject(&(GetObj(m_pVideoHandler)), s_className, reinterpret_cast<intptr_t>(this));

		m_pJCVideo->setVideoHandler(m_pVideoHandler);

//		LOGI("[Debug][Video] create obj id is %d", GetObj(m_pVideoHandler));
	}

	JSVideo::~JSVideo()
	{
		Dispose();
		JCMemorySurvey::GetInstance()->releaseClass("JSVideo", this);
	}

	void JSVideo::_releaseHandler()
	{
		CToJavaBridge::GetInstance()->disposeObject(GetObj(m_pVideoHandler), s_className, "Dispose");
		EmptyObj(m_pVideoHandler);
	}


	void JSVideo::LoadInternal(const std::string& path)
	{
//		LOGI("%s", path.c_str());
//		LOGI("[Debug][Video]call Load:  obj id is %d", GetObj(m_pVideoHandler));
		CToJavaBridge::GetInstance()->callObjVoidMethod(GetObj(m_pVideoHandler), s_className, "Load", path.c_str());
	}

	void JSVideo::Play()
	{
		if (m_isDownloadWaitLoad)
		{
			m_isDownloadWaitPlay = true;
			return;
		}
		m_isDownloadWaitPlay = false;
		CToJavaBridge::GetInstance()->callObjVoidMethod(GetObj(m_pVideoHandler), s_className, "Play");
	}

	void JSVideo::Pause()
	{
		m_isDownloadWaitPlay = false;
		CToJavaBridge::GetInstance()->callObjVoidMethod(GetObj(m_pVideoHandler), s_className, "Pause");
	}

	void JSVideo::Stop()
	{
	}

	bool JSVideo::GetPaused()
	{
		bool ret = false;
		CToJavaBridge::GetInstance()->callObjRetMethod(GetObj(m_pVideoHandler), s_className, "IsPaused", &ret);
		return ret;
	}

	bool JSVideo::GetLoop()
	{
		bool ret = false;
		CToJavaBridge::GetInstance()->callObjRetMethod(GetObj(m_pVideoHandler), s_className, "IsLoop", &ret);
		return ret;
	}

	void JSVideo::SetLoop(bool value)
	{
		CToJavaBridge::GetInstance()->callObjVoidMethod(GetObj(m_pVideoHandler), s_className, "SetLoop", value);
	}

	void JSVideo::SetAutoplay(bool value)
	{
		CToJavaBridge::GetInstance()->callObjVoidMethod(GetObj(m_pVideoHandler), s_className, "SetAutoplay", value);
	}

	bool JSVideo::GetAutoplay()
	{
		bool ret = false;
		CToJavaBridge::GetInstance()->callObjRetMethod(GetObj(m_pVideoHandler), s_className, "IsAutoplay", &ret);
		return ret;
	}

	void JSVideo::SetX(double val)
	{
		CToJavaBridge::GetInstance()->callObjVoidMethod(GetObj(m_pVideoHandler), s_className, "setX", (int)val);
	}

	double JSVideo::GetX()
	{
		return 0;
	}

	void JSVideo::SetY(double val)
	{
		CToJavaBridge::GetInstance()->callObjVoidMethod(GetObj(m_pVideoHandler), s_className, "setY", (int)val);
	}

	double JSVideo::GetY()
	{
		return 0;
	}

	double JSVideo::GetVideoWidth()
	{
		int ret = 0;
		CToJavaBridge::GetInstance()->callObjRetMethod(GetObj(m_pVideoHandler), s_className, "getVideoWidth", &ret);
		return (double)ret;
	}

	double JSVideo::GetVideoHeight()
	{
		int ret = 0;
		CToJavaBridge::GetInstance()->callObjRetMethod(GetObj(m_pVideoHandler), s_className, "getVideoHeight", &ret);
		return (double)ret;
	}

	double JSVideo::GetWidth()
	{
		return 0;
	}

	void JSVideo::SetWidth(double val)
	{
		CToJavaBridge::GetInstance()->callObjVoidMethod(GetObj(m_pVideoHandler), s_className, "setWidth", (int)val);
	}

	double JSVideo::GetHeight()
	{
		return 0;
	}

	void JSVideo::SetHeight(double val)
	{
		CToJavaBridge::GetInstance()->callObjVoidMethod(GetObj(m_pVideoHandler), s_className, "setHeight", (int)val);
	}

	double JSVideo::GetCurrentTime()
	{
		int ret = 0;
		CToJavaBridge::GetInstance()->callObjRetMethod(GetObj(m_pVideoHandler), s_className, "getCurrentTime", &ret);
		//LOGI("[Debug][Video] CurrentTime %d", ret);
		return (double)(ret * 0.001);
	}

	/// \param [in] val - the unit is seconds
	void JSVideo::SetCurrentTime(double val)
	{
		CToJavaBridge::GetInstance()->callObjVoidMethod(GetObj(m_pVideoHandler), s_className, "setCurrentTime", (int)(val * 1000));
	}

	double JSVideo::GetDuration()
	{
		int ret = 0;
		CToJavaBridge::GetInstance()->callObjRetMethod(GetObj(m_pVideoHandler), s_className, "getDuration", &ret);
		//LOGI("[Debug][Video] Duration %d", ret);
		return (double)(ret * 0.001);
	}

	double JSVideo::GetVolume()
	{
		int ret = 0;
		CToJavaBridge::GetInstance()->callObjRetMethod(GetObj(m_pVideoHandler), s_className, "getVolume", &ret);
		return (double)ret;
	}

	void JSVideo::SetVolume(double val)
	{
		CToJavaBridge::GetInstance()->callObjVoidMethod(GetObj(m_pVideoHandler), s_className, "setVolume", val);
	}

	int32_t JSVideo::GetReadyState()
	{
		int32_t ret = 0;
		CToJavaBridge::GetInstance()->callObjRetMethod(GetObj(m_pVideoHandler), s_className, "GetReadyState", &ret);

		return ret;
	}
};

#endif // ANDROID
