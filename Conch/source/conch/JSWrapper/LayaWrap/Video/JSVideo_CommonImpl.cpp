#include "JSVideo.h"
#include "../../../JCScriptRuntime.h"
#include "../../../JCConch.h"
#include "../../../JCSystemConfig.h"
#include <LayaGL/JCLayaGLDispatch.h>


namespace laya
{
	int JSVideo::ms_handleIdx = 0;

	void JSVideo::Init()
	{
		m_pJCVideo = new JCVideo();
		m_nID = JCConch::s_pConchRender->m_pImageManager->getImageID();

//		LOGI("[Debug][Video] ID is %d", m_nID);

		m_pJCVideo->setManager(JCConch::s_pConchRender->m_pFileResManager, JCConch::s_pConchRender->m_pImageManager);

		m_isDownloadWaitLoad = false;
		m_isDownloadWaitPlay = false;
		m_src = "";

		if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_DOUBLE)
		{
			JCScriptRuntime::s_JSRT->flushSharedCmdBuffer();
            JCCommandEncoderBuffer* pCmd = JCScriptRuntime::s_JSRT->m_pRenderCmd;
			pCmd->append(LAYA_CREATE_IMAGE_ON_RENDER_THREAD);
			pCmd->append(m_nID);
			pCmd->append((intptr_t)(m_pJCVideo));
		}
		else
		{
			JCConch::s_pConchRender->m_pImageManager->setImage(m_nID, m_pJCVideo);
		}
	}

	void JSVideo::Dispose()
	{
        JCCommandEncoderBuffer* pCmd = JCScriptRuntime::s_JSRT->m_pGCCmd;
		pCmd->append(LAYA_DELETE_IMAGE_ON_RENDER_THREAD);
		pCmd->append(m_nID);
	}

	void JSVideo::Load()
	{
		if (m_videoCache.IsDownloaded(m_src.c_str()))
		{
			m_isDownloadWaitPlay = false;
			const std::string localPath = m_videoCache.GetCacheFilePath(m_src.c_str());
			LoadInternal(localPath);
			m_videoCache.CancelDownload();
		}
		else
		{
			m_isDownloadWaitPlay = true;
			m_videoCache.DownloadFile(m_src.c_str());
			m_videoCache.OnLastFileDownloaded(std::bind(&JSVideo::LoadUrlOnDownloadEnd, this));
		}
	}

	bool JSVideo::CanPlayType(const char* type)
	{
		auto it = ms_supportFormat.find(type);
		return it != ms_supportFormat.end();
	}

	const char* JSVideo::GetSrc()
	{
		return m_src.c_str();
	}

	void JSVideo::SetSrc(const char* url)
	{
		m_src = url;
		Load();
	}

	int JSVideo::getImageID()
	{
		return m_nID;
	}

	void JSVideo::AddEvent(const char* evtName)
	{
		auto it = m_evtMap.find(evtName);
		if (it == m_evtMap.end())
			m_evtMap.insert(std::make_pair(evtName, 1));
		else
			it->second++;
	}

	void JSVideo::RemoveEvent(const char* evtName)
	{
		auto it = m_evtMap.find(evtName);
		if (it != m_evtMap.end())
		{
			it->second--;
			if (it->second == 0)
				m_evtMap.erase(it);
		}
	}

	void JSVideo::_setDispatchEventFunc(JSValueAsParam emitFunc)
	{
		m_dispatchHandle.set(0, this, emitFunc);
	}

	void JSVideo::CallHandle(const char* evtName)
	{
		std::string evt(evtName);
//		LOGI("[Debug][Video]Call handle %s", evtName);
		auto it = m_evtMap.find(evtName);
		if (it != m_evtMap.end())
		{
            if(JCScriptRuntime::s_JSRT)
            {
                JCScriptRuntime::s_JSRT->m_pScriptThread->post(std::bind(&JSVideo::DispatchEvtToJS, this, evt));
            }
		}
	}

    void JSVideo::DispatchEvtToJS(const std::string evtName)
    {
        m_dispatchHandle.Call(evtName.c_str());
    }
    
	void JSVideo::LoadUrlOnDownloadEnd()
	{
		Load();

		if (!m_isDownloadWaitLoad && m_isDownloadWaitPlay)
		{
			Play();
		}
	}

	ADDJSCLSINFO(JSVideo, JSObjNode);

	void JSVideo::exportJS()
	{
		JSP_CLASS("ConchVideo", JSVideo);
		JSP_REG_CONSTRUCTOR(JSVideo);
		JSP_ADD_METHOD("load", JSVideo::Load);
		JSP_ADD_METHOD("play", JSVideo::Play);
		JSP_ADD_METHOD("pause", JSVideo::Pause);
		JSP_ADD_METHOD("stop", JSVideo::Stop);
		JSP_ADD_METHOD("addEvent", JSVideo::AddEvent);
		JSP_ADD_METHOD("removeEvent", JSVideo::RemoveEvent);
		JSP_ADD_METHOD("_setDispatchEventFunc", JSVideo::_setDispatchEventFunc);
		JSP_ADD_METHOD("_releaseHandler", JSVideo::_releaseHandler);
		JSP_ADD_METHOD("canPlayType", JSVideo::CanPlayType);
		JSP_ADD_PROPERTY_RO(paused, JSVideo, GetPaused);
		JSP_ADD_PROPERTY(loop, JSVideo, GetLoop, SetLoop);
		JSP_ADD_PROPERTY(x, JSVideo, GetX, SetX);
		JSP_ADD_PROPERTY(y, JSVideo, GetY, SetY);
		JSP_ADD_PROPERTY_RO(videoWidth, JSVideo, GetVideoWidth);
		JSP_ADD_PROPERTY_RO(videoHeight, JSVideo, GetVideoHeight);
		JSP_ADD_PROPERTY_RO(readyState, JSVideo, GetReadyState);
		JSP_ADD_PROPERTY_RO(conchImgId, JSVideo, getImageID);
		JSP_ADD_PROPERTY(autoplay, JSVideo, GetAutoplay, SetAutoplay);
		JSP_ADD_PROPERTY(width, JSVideo, GetWidth, SetWidth);
		JSP_ADD_PROPERTY(height, JSVideo, GetHeight, SetHeight);
		JSP_ADD_PROPERTY(currentTime, JSVideo, GetCurrentTime, SetCurrentTime);
		JSP_ADD_PROPERTY_RO(duration, JSVideo, GetDuration);
		JSP_ADD_PROPERTY(src, JSVideo, GetSrc, SetSrc);
		JSP_ADD_PROPERTY(currentSrc, JSVideo, GetSrc, SetSrc);
		JSP_ADD_PROPERTY(volume, JSVideo, GetVolume, SetVolume);
		JSP_INSTALL_CLASS("ConchVideo", JSVideo);
	}

}
