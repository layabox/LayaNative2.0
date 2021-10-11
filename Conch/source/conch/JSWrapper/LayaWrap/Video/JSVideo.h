#ifndef __JsVideo_h__
#define __JsVideo_h__

#include "../../JSInterface/JSInterface.h"
#include "../../JSInterface/JSObjBase.h"

#include <map>
#include <set>
#include <string>

#include "VideoCache.h"
#include <Image/JCVideo.h>

namespace laya
{

//#define SHOW_NOT_IMPLEMENT_LOG

#ifdef SHOW_NOT_IMPLEMENT_LOG
	#define NOT_IMPLEMENT() do { LOGW("%s is not implemented", __FUNCTION__); } while(false);
	#define NOT_IMPLEMENT_RET(value) do { LOGW("%s is not implemented", __FUNCTION__); }  while(false); return value;
#else
	#define NOT_IMPLEMENT() 
	#define NOT_IMPLEMENT_RET(value) return value;
#endif

	class JSVideo : public JsObjBase, public JSObjNode
	{
	public:
		static JsObjClassInfo JSCLSINFO;
		static void exportJS();

		JSVideo();
		~JSVideo();

		void AddEvent(const char* evtName);
		void RemoveEvent(const char* evtName);
		void _setDispatchEventFunc(JSValueAsParam emitFunc);

		void Load();
		void Play();
		void Pause();
		void Stop();
		bool CanPlayType(const char* type);

		void SetAutoplay(bool val);
		bool GetAutoplay();

		void SetX(double val);
		double GetX();

		void SetY(double val);
		double GetY();

		bool GetPaused();
		
		bool GetLoop();
		void SetLoop(bool value);

		const char* GetSrc();
		void SetSrc(const char* url);

		double GetVideoWidth();
		double GetVideoHeight();

		double GetWidth();
		void SetWidth(double val);

		double GetHeight();
		void SetHeight(double val);

		double GetCurrentTime();
		void SetCurrentTime(double val);

		double GetDuration();

		double GetVolume();
		void SetVolume(double val);

		void CallHandle(const char* evtName);

		int32_t GetReadyState();

		int getImageID();

		void _releaseHandler();

	private:
		void Init();
		void Dispose();
		void LoadInternal(const std::string& path);
		void LoadUrlOnDownloadEnd();
        void DispatchEvtToJS(const std::string path);

	private:
		static int ms_handleIdx;
		static std::set<std::string> ms_supportFormat;

		VideoCache m_videoCache;
		std::map<std::string, int> m_evtMap;
		JsObjHandle m_dispatchHandle;
		std::string m_src;

		bool m_isDownloadWaitLoad;
		bool m_isDownloadWaitPlay;

		IVideoHandler* m_pVideoHandler; /// !! released it by JCVideo

		JCVideo* m_pJCVideo; 
		int m_nID;
	};
};
#endif
