//
//  JSVideo_IOSImpl.cpp
//  conchRuntime
//
//  Created by LayaBox on 2018/11/2.
//  Copyright © 2018 LayaBox. All rights reserved.
//

#ifdef __APPLE__

#include "JSVideo.h"
#include "../../../CToObjectC.h"

namespace laya {
    std::set<std::string> JSVideo::ms_supportFormat = {
        "video/mp4",
    };
    
    
    struct IOSVideoHandler : public IVideoHandler
    {
        IOSVideoHandler()
        {
            obj = nullptr;
        }
        
        virtual ~IOSVideoHandler()
        {
            
        }
        
        virtual bool isFrameUpdated()
        {
            if(!obj)
                return false;
            bool ret = false;
            CToObjectVideoPlayerIsFrameUpdate(obj, &ret);
            return ret;
        }
        
        virtual void updateBitmapData(BitmapData* bitmapData)
        {
            if(obj == nullptr)
                return;
            
            CToObjectVideoPlayerGetBitmap(obj, bitmapData);
        }
        
        void* obj;
    };
    
    static inline void* GetObj(IVideoHandler* videoHandler)
    {
        IOSVideoHandler* pHandler = static_cast<IOSVideoHandler*>(videoHandler);
        return pHandler->obj;
    }
    
    static inline void** GetObjPtr(IVideoHandler* videoHandler)
    {
        IOSVideoHandler* pHandler = static_cast<IOSVideoHandler*>(videoHandler);
        return &pHandler->obj;
    }
    
    static inline void EmptyObj(IVideoHandler* videoHandler)
    {
        IOSVideoHandler* pHandler = static_cast<IOSVideoHandler*>(videoHandler);
        pHandler->obj = nullptr;
    }
    
    JSVideo::JSVideo()
    {
        Init();
        
        AdjustAmountOfExternalAllocatedMemory(sizeof(JSVideo));
        JCMemorySurvey::GetInstance()->newClass("JSVideo", sizeof(JSVideo), this);
        
        m_pVideoHandler = new IOSVideoHandler(); // released it by JCVideo in render thread
        
        CToObjectNewVideoPlayer(GetObjPtr(m_pVideoHandler), std::bind(&JSVideo::CallHandle, this, std::placeholders::_1));
        
        m_pJCVideo->setVideoHandler(m_pVideoHandler);
    }
    
    JSVideo::~JSVideo()
    {
		Dispose();
        JCMemorySurvey::GetInstance()->releaseClass("JSVideo", this);
    }
    
	void JSVideo::_releaseHandler()
	{
		CToObjectDisposeVideoPlayer(GetObj(m_pVideoHandler));
		EmptyObj(m_pVideoHandler);
	}

    void JSVideo::LoadInternal(const std::string& path)
    {
        CToObjectVideoPlayerLoad(GetObj(m_pVideoHandler), path.c_str());
    }
    
    void JSVideo::Play()
    {
        if(m_isDownloadWaitLoad)
        {
            m_isDownloadWaitPlay = true;
            return;
        }
        m_isDownloadWaitPlay = false;
        CToObjectVideoPlayerPlay(GetObj(m_pVideoHandler));
    }
    
    void JSVideo::Pause()
    {
        m_isDownloadWaitPlay = false;
        CToObjectVideoPlayerPause(GetObj(m_pVideoHandler));
        
    }
    
    void JSVideo::Stop()
    {
        NOT_IMPLEMENT();
    }
    
    bool JSVideo::GetPaused()
    {
        bool ret = false;
        CToObjectVideoPlayerGetPaused(GetObj(m_pVideoHandler), &ret);
        return ret;
    }
    
    bool JSVideo::GetLoop()
    {
        bool ret = false;
        CToObjectVideoPlayerGetLoop(GetObj(m_pVideoHandler), &ret);
        return ret;
    }
    
    void JSVideo::SetLoop(bool value)
    {
        CToObjectVideoPlayerSetLoop(GetObj(m_pVideoHandler), value);
    }
    
    void JSVideo::SetX(double val)
    {
        CToObjectVideoPlayerSetX(GetObj(m_pVideoHandler), val);
    }
    
    double JSVideo::GetX()
    {
        NOT_IMPLEMENT_RET(0);
    }
    
    void JSVideo::SetY(double val)
    {
        CToObjectVideoPlayerSetY(GetObj(m_pVideoHandler), val);
    }
    
    double JSVideo::GetY()
    {
        NOT_IMPLEMENT_RET(0);
    }
    
    double JSVideo::GetVideoWidth()
    {
        double ret = 0;
        CToObjectVideoPlayerGetVideoWidth(GetObj(m_pVideoHandler), &ret);
        return ret;
    }
    
    double JSVideo::GetVideoHeight()
    {
        double ret = 0;
        CToObjectVideoPlayerGetVideoHeight(GetObj(m_pVideoHandler), &ret);
        return ret;    
	}
    
    double JSVideo::GetWidth()
    {
        double ret = 0;
        CToObjectVideoPlayerGetWidth(GetObj(m_pVideoHandler), &ret);
        return ret;
    }
    
    void JSVideo::SetWidth(double val)
    {
        CToObjectVideoPlayerSetWidth(GetObj(m_pVideoHandler), val);
    }
    
    double JSVideo::GetHeight()
    {
        double ret = 0;
        CToObjectVideoPlayerGetHeight(GetObj(m_pVideoHandler), &ret);
        return ret;
    }
    
    void JSVideo::SetHeight(double val)
    {
        CToObjectVideoPlayerSetHeight(GetObj(m_pVideoHandler), val);
    }
    
    double JSVideo::GetCurrentTime()
    {
        double ret = 0;
        CToObjectVideoPlayerGetCurrentTime(GetObj(m_pVideoHandler), &ret);
        return ret;
    }
    
    void JSVideo::SetCurrentTime(double val)
    {
        CToObjectVideoPlayerSetCurrentTime(GetObj(m_pVideoHandler), val);
    }
    
    double JSVideo::GetDuration()
    {
        double ret = 0;
        CToObjectVideoPlayerGetDuration(GetObj(m_pVideoHandler), &ret);
        return ret;
    }
    
    double JSVideo::GetVolume()
    {
        double ret = 0;
        CToObjectVideoPlayerGetVolume(GetObj(m_pVideoHandler), &ret);
        return ret;
    }
    
    void JSVideo::SetVolume(double val)
    {
        CToObjectVideoPlayerSetVolume(GetObj(m_pVideoHandler), val);
    }
    
    int32_t JSVideo::GetReadyState()
    {
        int32_t ret = 0;
        CToObjectVideoPlayerGetReadyState(GetObj(m_pVideoHandler), &ret);
        return ret;
    }

    void JSVideo::SetAutoplay(bool val)
    {
        CToObjectVideoPlayerSetAutoplay(GetObj(m_pVideoHandler), val);
    }
    
    bool JSVideo::GetAutoplay()
    {
        bool ret = false;
        CToObjectVideoPlayerGetAutoplay(GetObj(m_pVideoHandler), &ret);
        return ret;
    }
};

#endif
