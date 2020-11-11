/**
@file			JSInput.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_18
*/

#include "JSInput.h"
#include <util/Log.h>
#include "../../JSWrapper/LayaWrap/JSInput.h"
#include <util/JCIThreadCmdMgr.h>
#include "../../JCConch.h"

namespace laya
{
    ADDJSCLSINFO(JSInput, JSObjNode);
    JSInput* JSInput::m_pInstance = NULL;
    JSInput* JSInput::getInstance()
    {
        if (m_pInstance == NULL)
        {
            m_pInstance = new JSInput();
        }
        return m_pInstance;
    }
    void JSInput::Reset() 
    {

    }
    JSInput::JSInput() 
    {
        m_pCmdPoster = JCScriptRuntime::s_JSRT->m_pPoster;
        m_bTouchMode = false;
        m_vInputEvents.reserve( 64 );
        m_vInputEventsJS.reserve( 64 );
        m_nTouchFrame = 0;
    }
    JSInput::~JSInput()
    {
    }
    void JSInput::onDeviceOrientationCallJSFunction(DeviceOrientationEvent e)
    {
        JCScriptRuntime::s_JSRT->m_pJSDeviceMotionEvtFunction.Call(e.type,e.ra,e.rb,e.rg);
    }
    void JSInput::onDeviceMotionCallJSFunction(DeviceMotionEvent e)
    {
        float* t = (float *)JCScriptRuntime::s_JSRT->m_pOtherBufferSharedWidthJS;
        t[0] = e.ax;
        t[1] = e.ay;
        t[2] = e.az;
        t[3] = e.agx;
        t[4] = e.agy;
        t[5] = e.agz;
        t[6] = e.ra;
        t[7] = e.rb;
        t[8] = e.rg;
        t[9] = e.interval;
        JCScriptRuntime::s_JSRT->m_pJSDeviceMotionEvtFunction.Call(e.type);
    }
    void JSInput::setTouchMode(bool bMode)
    {
        m_bTouchMode = bMode;
    }
    bool JSInput::getTouchMode()
    {
        return m_bTouchMode;
    }
    void JSInput::swapCurrentTouchEvent()
    {
        m_Lock.lock();
        std::swap(m_vInputEvents, m_vInputEventsJS);
        m_vInputEvents.clear();
        m_Lock.unlock();
    }
    void JSInput::onInputCallJSFunction(inputEvent e)
    {
        //touch
        if (e.nType <= E_ONACTION_POINTER_UP)
        {
            JCScriptRuntime::s_JSRT->m_pJSTouchEvtFunction.Call(e.nTouchType, e.id, e.type, e.posX, e.posY);
        }
        //鼠标
        else if (e.nType >= E_ONMOUSEDOWN && e.nType <= E_ONRIGHTMOUSEUP)
        {
            JCScriptRuntime::s_JSRT->m_pJSMouseEvtFunction.Call(e.nTouchType, e.type, e.posX, e.posY, e.nWheel);
        }
        //键盘
        else if (e.nType >= E_ONKEYDOWN && e.nType <= E_ONKEYUP)
        {
            int bAlt = e.bAlt ? 1 : 0;
            int bShift = e.bShift ? 2 : 0;
            int bCtrl = e.bCtrl ? 4 : 0;
            JCScriptRuntime::s_JSRT->m_pJSKeyEvtFunction.Call(e.type, e.keyCode, e.keyChar, bAlt&bShift&bCtrl);
        }
        else if (e.nType == E_JOYSTICK) 
        {
            //JCScriptRuntime::s_JSRT->m_JoystickEvtHandler.Call(e.type, e.fTHUMBL_xOffset, e.fTHUMBL_yOffset, e.fTHUMBR_xOffset, e.fTHUMBR_yOffset, e.fLT_Offset, e.fRT_Offset);
        }
    }
    bool JSInput::activeCall(inputEvent e)
    {
        if (e.nType > E_TYPE_COUNT)
        {
            return false;
        }
        if (e.nType <= E_ONACTION_POINTER_UP && m_bTouchMode == true)
        {
            TouchEventInfo info = { e.nTouchType,e.id,e.posX,e.posY};
            m_Lock.lock();
            m_vInputEvents.push_back(info);
            m_Lock.unlock();
        }
        else
        {
#ifdef WIN32
            if (e.nType == E_ONMOUSEDOWN || e.nType == E_ONMOUSEMOVE)
            {
#else
            if (e.nType == E_ONTOUCHSTART || e.nType == E_ONTOUCHMOVE)
            {
#endif
                double fTime = tmGetCurms();
#ifdef __APPLE__
                if (JCConch::s_pConchRender)
                    JCConch::s_pConchRender->onTouchStart(fTime);
#elif ANDROID  
				//如何转到渲染线程？有线程问题但应该影响不大
				if (JCConch::s_pConchRender)
                    JCConch::s_pConchRender->onTouchStart(fTime);
#elif WIN32
                if (JCConch::s_pConchRender)
                {
                    std::function<void(void)> pFunction = std::bind(&JCConchRender::onTouchStart, JCConch::s_pConchRender, fTime);
                    JCConch::s_pConchRender->m_pRenderThread->post(pFunction);
                }
#endif
            }
            std::function<void(void)> pFunction = std::bind(&JSInput::onInputCallJSFunction, this, e);
            if (JCScriptRuntime::s_JSRT->m_pPoster)
            {
                JCScriptRuntime::s_JSRT->m_pPoster->postToJS(pFunction);
            }
        }
        return true;
    }
    bool JSInput::activeCall(DeviceMotionEvent e)
    {
        if (e.nType > E_TYPE_COUNT)
        {
            return false;
        }
        std::function<void(void)> pFunction = std::bind(&JSInput::onDeviceMotionCallJSFunction, this, e);
        if (JCScriptRuntime::s_JSRT->m_pPoster)
            JCScriptRuntime::s_JSRT->m_pPoster->postToJS(pFunction);
        return true;
    }
    bool JSInput::activeCall(DeviceOrientationEvent e)
    {
        std::function<void(void)> pFunction = std::bind(&JSInput::onDeviceOrientationCallJSFunction, this, e);
        if (JCScriptRuntime::s_JSRT->m_pPoster)
            JCScriptRuntime::s_JSRT->m_pPoster->postToJS(pFunction);
        return true;
    }
    void JSInput::captureScreenCallBack(char *p_pBuffer, int p_nLen, int p_nW, int p_nH)
    {
        std::function<void(void)> pFunction = std::bind(&JSInput::onCaptureScreenCallJSFunction, this, p_pBuffer,p_nLen,p_nW,p_nH);
        if (JCScriptRuntime::s_JSRT->m_pPoster)
            JCScriptRuntime::s_JSRT->m_pPoster->postToJS(pFunction);
    }
    void JSInput::onCaptureScreenCallJSFunction(char *p_pBuffer, int p_nlen, int p_nW, int p_nH)
    {
#ifdef JS_V8
        v8::HandleScope scope(v8::Isolate::GetCurrent());
#endif
        JsValue ab = createJSAB(p_pBuffer, p_nlen);
		delete[] p_pBuffer;
        JCScriptRuntime::s_JSRT->m_pJSOnceOtherEvtFuction.Call(ab, p_nW,p_nH);
    }
    void JSInput::exportJS()
    {
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
