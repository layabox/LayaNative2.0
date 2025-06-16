#include <ace/xcomponent/native_xcomponent_key_event.h>
#include <stdint.h>
#include <unistd.h>

#include "plugin_render.h"
#include "../../napi/plugin_manager.h"
#include "../modules/TouchesNapi.h"
#include "../../napi/NAPIFun.cpp"
#include "../modules/KeyNapi.h"
#include "native_window/external_window.h"
#include "native_buffer/native_buffer.h"

#include <assert.h>
#include "util/Log.h"

using namespace laya;

#ifdef __cplusplus
extern "C" {
#endif

PluginRender* PluginRender::instance_ = nullptr;
OH_NativeXComponent_Callback PluginRender::callback_;
uint64_t PluginRender::animationInterval_ = 16;
uint64_t PluginRender::lastTime = 0;


void OnSurfaceCreatedCB(OH_NativeXComponent* component, void* window)
{
    LOGI("OnSurfaceCreatedCB");
    PluginRender::GetInstance()->sendMsgToWorker(MessageType::WM_XCOMPONENT_SURFACE_CREATED, component, window);
}

void OnSurfaceChangedCB(OH_NativeXComponent* component, void* window)
{
    LOGI("OnSurfaceChangedCB");
    PluginRender::GetInstance()->sendMsgToWorker(MessageType::WM_XCOMPONENT_SURFACE_CHANGED, component, window);
}

void OnSurfaceDestroyedCB(OH_NativeXComponent* component, void* window)
{
    LOGI("OnSurfaceDestroyedCB");
    PluginRender::GetInstance()->sendMsgToWorker(MessageType::WM_XCOMPONENT_SURFACE_DESTROY, component, window);
}

void OnSurfaceHideCB(OH_NativeXComponent* component, void* window) {
    LOGI("OnSurfaceHideCB");
    
    int32_t ret;
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    ret = OH_NativeXComponent_GetXComponentId(component, idStr, &idSize);
    if (ret != OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        return;
    }
    
    PluginRender::GetInstance()->sendMsgToWorker(MessageType::WM_XCOMPONENT_SURFACE_HIDE,component, window);
}

void OnSurfaceShowCB(OH_NativeXComponent* component, void* window) {
    LOGI("OnSurfaceShowCB");
    
    int32_t ret;
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    ret = OH_NativeXComponent_GetXComponentId(component, idStr, &idSize);
    if (ret != OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        return;
    }
    
    PluginRender::GetInstance()->sendMsgToWorker(MessageType::WM_XCOMPONENT_SURFACE_SHOW,component, window);
}

void DispatchTouchEventCB(OH_NativeXComponent* component, void* window)
{
    OH_NativeXComponent_TouchEvent* touchEvent = new(std::nothrow) OH_NativeXComponent_TouchEvent();
    if (!touchEvent) {
        LOGI("DispatchTouchEventCB::touchEvent alloc failed");
        return;
    }
    int32_t ret = OH_NativeXComponent_GetTouchEvent(component, window, touchEvent);
    if (ret == OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        PluginRender::GetInstance()->sendMsgToWorker(MessageType::WM_XCOMPONENT_TOUCH_EVENT, component, window, touchEvent);
    } else {
        delete touchEvent;
    }
}

void dispatchKeyEventCB(OH_NativeXComponent* component, void* window) {
    OH_NativeXComponent_KeyEvent* keyEvent;
    if (OH_NativeXComponent_GetKeyEvent(component, &keyEvent) >= 0) {
        PluginRender::GetInstance()->sendMsgToWorker(MessageType::WM_XCOMPONENT_KEY_EVENT, component, window);
    } else {
        // OpenHarmonyPlatform::getKeyEventError
    }
}

void dispatchMouseEventCB(OH_NativeXComponent* component, void* window) {
    OH_NativeXComponent_MouseEvent* mouseEvent = new(std::nothrow) OH_NativeXComponent_MouseEvent();
    int32_t ret = OH_NativeXComponent_GetMouseEvent(component, window, mouseEvent);
    if (ret == OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        PluginRender::GetInstance()->sendMsgToWorker(MessageType::WM_XCOMPONENT_MOUSE_EVENT, component, window, mouseEvent);
    }
}

void dispatchHoverEventCB(OH_NativeXComponent* component, bool isHover) {
    // OpenHarmonyPlatform::DispatchHoverEventCB
}

PluginRender::PluginRender() : component_(nullptr)
{
    auto renderCallback = PluginRender::GetNXComponentCallback();
    renderCallback->OnSurfaceCreated = OnSurfaceCreatedCB;
    renderCallback->OnSurfaceChanged = OnSurfaceChangedCB;
    renderCallback->OnSurfaceDestroyed = OnSurfaceDestroyedCB;
    renderCallback->DispatchTouchEvent = DispatchTouchEventCB;
}

PluginRender* PluginRender::GetInstance()
{
    if (instance_ == nullptr) {
        instance_ = new PluginRender();
    }
    return instance_;
}

OH_NativeXComponent_Callback* PluginRender::GetNXComponentCallback()
{
    return &PluginRender::callback_;
}

// static
void PluginRender::onMessageCallback(const uv_async_t* /* req */) {
    void* window = nullptr;
    WorkerMessageData msgData;
    PluginRender* render = PluginRender::GetInstance();

    while (true) {
        //loop until all msg dispatch
        if (!render->dequeue(reinterpret_cast<WorkerMessageData*>(&msgData))) {
            // Queue has no data
            break;
        }

        if ((msgData.type >= MessageType::WM_XCOMPONENT_SURFACE_CREATED) && (msgData.type <= MessageType::WM_XCOMPONENT_SURFACE_DESTROY)) {
            OH_NativeXComponent* nativexcomponet = reinterpret_cast<OH_NativeXComponent*>(msgData.data);
            // CC_ASSERT(nativexcomponet != nullptr);

            if (msgData.type == MessageType::WM_XCOMPONENT_SURFACE_CREATED) {
                render->OnSurfaceCreated(nativexcomponet, msgData.window);
            } else if (msgData.type == MessageType::WM_XCOMPONENT_TOUCH_EVENT) {
                OH_NativeXComponent_TouchEvent* touchEvent = reinterpret_cast<OH_NativeXComponent_TouchEvent*>(msgData.eventData);
                render->DispatchTouchEvent(nativexcomponet, msgData.window, touchEvent);
            } else if (msgData.type == MessageType::WM_XCOMPONENT_KEY_EVENT) {
                render->DispatchKeyEvent(nativexcomponet, msgData.window);
            } else if (msgData.type == MessageType::WM_XCOMPONENT_MOUSE_EVENT) {
                OH_NativeXComponent_MouseEvent* mouseEvent = reinterpret_cast<OH_NativeXComponent_MouseEvent*>(msgData.eventData);
                render->DispatchMouseEvent(nativexcomponet, msgData.window, mouseEvent);
            } else if (msgData.type == MessageType::WM_XCOMPONENT_SURFACE_CHANGED) {
                render->OnSurfaceChanged(nativexcomponet, msgData.window);
            } else if (msgData.type == MessageType::WM_XCOMPONENT_SURFACE_HIDE) {
                render->OnSurfaceHide();
            } else if (msgData.type == MessageType::WM_XCOMPONENT_SURFACE_SHOW) {
                render->OnSurfaceShow(msgData.window);
            } else if (msgData.type == MessageType::WM_XCOMPONENT_SURFACE_DESTROY) {
                render->OnSurfaceDestroyed(nativexcomponet, msgData.window);
            } else {
                // CC_ASSERT(false);
            }
            continue;
        }

        if (msgData.type == MessageType::WM_APP_SHOW) {
            render->OnShowNative();
        } else if (msgData.type == MessageType::WM_APP_HIDE) {
            render->OnHideNative();
        } else if (msgData.type == MessageType::WM_APP_DESTROY) {
            render->OnDestroyNative();
        }
        if(msgData.type == MessageType::WM_VSYNC) {
            // render->runTask();
        }
    }
}

static uint64_t getCurrentMillSecond() {
    struct timeval stCurrentTime;

    gettimeofday(&stCurrentTime,NULL);
    return stCurrentTime.tv_sec * 1000 + stCurrentTime.tv_usec / 1000; //millseconds
}

// static
void PluginRender::timerCb(uv_timer_t* handle)
{
    // LOGI("PluginRender::timerCb, animationInterval_ is %{public}lu", animationInterval_);
    if (PluginRender::GetInstance()->eglCore_ != nullptr) {
        PluginRender::GetInstance()->eglCore_->Update();
        NAPIFun::ConchNAPI_onDrawFrame();
    }
}

void PluginRender::SetNativeXComponent(OH_NativeXComponent* component)
{
    component_ = component;
    OH_NativeXComponent_RegisterCallback(component_, &PluginRender::callback_);
    OH_NativeXComponent_RegisterSurfaceHideCallback(component_, OnSurfaceHideCB);
    OH_NativeXComponent_RegisterSurfaceShowCallback(component_, OnSurfaceShowCB);
    
    // register keyEvent
    OH_NativeXComponent_RegisterKeyEventCallback(component_, dispatchKeyEventCB);
    // register mouseEvent
    _mouseCallback.DispatchMouseEvent = dispatchMouseEventCB;
    _mouseCallback.DispatchHoverEvent = dispatchHoverEventCB;
    OH_NativeXComponent_RegisterMouseEventCallback(component_, &_mouseCallback);
}

void PluginRender::dispatchMouseWheelCB(std::string eventType, float localX, float localY, float offsetY) {
    if (PluginRender::GetInstance()->isMouseLeftActive) {
        return;
    }
    if (eventType == "actionUpdate") {
        float moveScrollY = offsetY - scrollDistance;
        scrollDistance = offsetY;
        inputEvent e;
        e.nTouchType = e.nType = E_ONMOUSEWHEEL;
        strncpy(e.type, "mousewheel", 256);
        e.posX = localX;
        e.posY = localY;
        e.nWheel = moveScrollY;
        if (!e.nWheel) return;
        JCScriptRuntime::s_JSRT->dispatchInputEvent(e);
    } else {
        scrollDistance = 0;
    }
}

void PluginRender::workerInit(napi_env env, uv_loop_t* loop) {
    LOGI("PluginRender::workerInit");
    workerLoop_ = loop;
    if (workerLoop_) {
        uv_async_init(workerLoop_, &messageSignal_, reinterpret_cast<uv_async_cb>(PluginRender::onMessageCallback));
        if (!messageQueue_.empty()) {
            triggerMessageSignal(); // trigger the signal to handle the pending message
        }
    }
}


void PluginRender::sendMsgToWorker(const MessageType& type, OH_NativeXComponent* component, void* window) {
    WorkerMessageData data{type, static_cast<void*>(component), window};
    enqueue(data);
}

void PluginRender::sendMsgToWorker(const MessageType& type, OH_NativeXComponent* component, void* window, void* eventData) {
    WorkerMessageData data{type, static_cast<void*>(component), window, eventData};
    enqueue(data);
}

void PluginRender::enqueue(const WorkerMessageData& msg) {
    messageQueue_.enqueue(msg);
    triggerMessageSignal();
}

bool PluginRender::dequeue(WorkerMessageData* msg) {
    return messageQueue_.dequeue(msg);
}

void PluginRender::triggerMessageSignal() {
    if(workerLoop_ != nullptr) {
        // It is possible that when the message is sent, the worker thread has not yet started.
        uv_async_send(&messageSignal_);
    }
}

void PluginRender::run() {
    LOGI("PluginRender::run");
    if (workerLoop_) {
        uv_timer_init(workerLoop_, &timerHandle_);
        timerInited_ = true;
    }
}

void PluginRender::changeFPS(uint64_t animationInterval) {
    LOGI("PluginRender::changeFPS, animationInterval from %{public}lu to %{public}lu", animationInterval_, animationInterval);
    animationInterval_ = animationInterval;
}

void PluginRender::OnSurfaceCreated(OH_NativeXComponent* component, void* window)
{
    LOGI("PluginRender::OnSurfaceCreated");
    eglCore_ = new EGLCore();
    int32_t ret = OH_NativeXComponent_GetXComponentSize(component, window, &width_, &height_);
    if (ret == OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        int32_t code = SET_USAGE;
        OHNativeWindow *oHNativeWindow = static_cast<OHNativeWindow *>(window);
        int32_t ret = OH_NativeWindow_NativeWindowHandleOpt(oHNativeWindow, code, NATIVEBUFFER_USAGE_MEM_DMA);
        eglCore_->GLContextInit(window, width_, height_);
        NAPIFun::ConchNAPI_OnGLReady(width_,height_);
    }
}

void PluginRender::OnSurfaceChanged(OH_NativeXComponent* component, void* window)
{
    LOGI("PluginRender::OnSurfaceChanged");
    int32_t ret = OH_NativeXComponent_GetXComponentSize(component, window, &width_, &height_);
    if (ret == OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        NAPIFun::ConchNAPI_OnGLReady(width_,height_);
    }
}

void PluginRender::OnSurfaceDestroyed(OH_NativeXComponent* component, void* window)
{
    delete eglCore_;
    eglCore_ = nullptr;
}

void PluginRender::OnSurfaceHide()
{
    eglCore_->DestroySurface();
}

void PluginRender::OnSurfaceShow(void* window)
{
    eglCore_->CreateSurface(window);
}

void PluginRender::DispatchTouchEvent(OH_NativeXComponent* component, void* window, OH_NativeXComponent_TouchEvent* touchEvent)
{
    intptr_t ids[touchEvent->numPoints];
    float xs[touchEvent->numPoints];
    float ys[touchEvent->numPoints];
    for (int i = 0; i < touchEvent->numPoints; i++) {
        ids[i] = touchEvent->touchPoints[i].id;
        xs[i] = touchEvent->touchPoints[i].x;
        ys[i] = touchEvent->touchPoints[i].y;
        LOGI("Touch Info : x = %{public}f, y = %{public}f", xs[i], ys[i]);
    }
    switch (touchEvent -> type) {
        case OH_NATIVEXCOMPONENT_DOWN:
            nativeHandleTouchDown(touchEvent->id, touchEvent->x,touchEvent->y);
            LOGI("Touch Info : OH_NATIVEXCOMPONENT_DOWN");
            break;
        case OH_NATIVEXCOMPONENT_UP:
            nativeHandleTouchUp(touchEvent->id, touchEvent->x,touchEvent->y);
            LOGI("Touch Info : OH_NATIVEXCOMPONENT_UP");
            break;
        case OH_NATIVEXCOMPONENT_MOVE:
            nativeHandleTouchMove(touchEvent->id, touchEvent->x,touchEvent->y);
            LOGI("Touch Info : OH_NATIVEXCOMPONENT_MOVE");
            break;
        case OH_NATIVEXCOMPONENT_CANCEL:
            nativeHandleTouchCancel(touchEvent->id, touchEvent->x,touchEvent->y);
            LOGI("Touch Info : OH_NATIVEXCOMPONENT_CANCEL");
            break;
        case OH_NATIVEXCOMPONENT_UNKNOWN:
            LOGI("Touch Info : OH_NATIVEXCOMPONENT_UNKNOWN");
            break;
        default:
            LOGI("Touch Info : default");
            break;
    }
    delete touchEvent;
}

void PluginRender::DispatchKeyEvent(OH_NativeXComponent* component, void* window)
{
    OH_NativeXComponent_KeyEvent* keyEvent;
    if (OH_NativeXComponent_GetKeyEvent(component, &keyEvent) >= 0) {
        static const int keyCodeUnknownInOH = -1;
        static const int keyActionUnknownInOH = -1;
        OH_NativeXComponent_KeyAction action;
        OH_NativeXComponent_GetKeyEventAction(keyEvent, &action);
        OH_NativeXComponent_KeyCode code;
        OH_NativeXComponent_GetKeyEventCode(keyEvent, &code);
        if (code == keyCodeUnknownInOH || action == keyActionUnknownInOH) {
            // "unknown code and action dont't callback"
            return;
        }
        nativeHandleKey(action, code);
    } else {
        // OpenHarmonyPlatform::getKeyEventError
    }
}

void PluginRender::DispatchMouseEvent(OH_NativeXComponent* component, void* window, OH_NativeXComponent_MouseEvent* mouseEvent)
{
    inputEvent e;
    e.posX = mouseEvent->x;
    e.posY = mouseEvent->y;
    
    if (mouseEvent->action == OH_NativeXComponent_MouseEventAction::OH_NATIVEXCOMPONENT_MOUSE_PRESS) {
        if (mouseEvent->button == OH_NativeXComponent_MouseEventButton::OH_NATIVEXCOMPONENT_LEFT_BUTTON) {
            PluginRender::GetInstance()->isMouseLeftActive = true;
            e.nTouchType = e.nType = E_ONMOUSEDOWN;
            strncpy(e.type, "mousedown", 256);
        }
        else if(mouseEvent->button == OH_NativeXComponent_MouseEventButton::OH_NATIVEXCOMPONENT_RIGHT_BUTTON) {
            e.nTouchType = e.nType = E_ONRIGHTMOUSEDOWN;
            strncpy(e.type, "rightmousedown", 256);
        }
    }
    else if (mouseEvent->action == OH_NativeXComponent_MouseEventAction::OH_NATIVEXCOMPONENT_MOUSE_RELEASE) {
        if (mouseEvent->button == OH_NativeXComponent_MouseEventButton::OH_NATIVEXCOMPONENT_LEFT_BUTTON) {
            PluginRender::GetInstance()->isMouseLeftActive = false;
            e.nTouchType = e.nType = E_ONMOUSEUP;
            strncpy(e.type, "mouseup", 256);
        }
        else if(mouseEvent->button == OH_NativeXComponent_MouseEventButton::OH_NATIVEXCOMPONENT_RIGHT_BUTTON) {
            e.nTouchType = e.nType = E_ONRIGHTMOUSEUP;
            strncpy(e.type, "rightmouseup", 256);
        }
    }
    else if (mouseEvent->action == OH_NativeXComponent_MouseEventAction::OH_NATIVEXCOMPONENT_MOUSE_MOVE) {
        e.nTouchType = e.nType = E_ONMOUSEMOVE;
        strncpy(e.type, "mousemove", 256);
    }
    else {
        return;
    }
    e.nWheel = 0;
    
    JCScriptRuntime::s_JSRT->dispatchInputEvent(e);
    delete mouseEvent;
}

void PluginRender::OnCreateNative(napi_env env, uv_loop_t* loop) {
    LOGI("PluginRender::OnCreateNative");
}

void PluginRender::OnShowNative() {
    LOGI("PluginRender::OnShowNative");
    if (timerInited_) {
        uv_timer_start(&timerHandle_, &PluginRender::timerCb, 0, 1);
    }
    JCAudioManager* am = JCAudioManager::GetInstance();
    if(am->getMp3Mute() == false && am ->getMp3Stopped() == false)
    {
        NapiHelper::GetInstance()->__resumeBackgroundMusic();
    }
    laya::JCAudioManager::GetInstance()->m_pWavPlayer->resume();
}

void PluginRender::OnHideNative() {
    LOGI("PluginRender::OnHideNative");
    JCAudioManager* am =JCAudioManager::GetInstance();
    if(am->getMp3Mute() == false && am->getMp3Stopped() == false)
    {
        NapiHelper::GetInstance()->__pauseBackgroundMusic();
    }
    laya::JCAudioManager::GetInstance()->m_pWavPlayer->pause();

    if (timerInited_) {
        uv_timer_stop(&timerHandle_);
    }
}

void PluginRender::OnDestroyNative() {
    LOGI("PluginRender::OnDestoryNative");
    if (timerInited_) {
        uv_timer_stop(&timerHandle_);
    }
    NAPIFun::ConchNAPI_ReleaseDLib();
}

napi_value PluginRender::Export(napi_env env, napi_value exports)
{
    LOGI("PluginRender::Export");
    // Register JS API
    napi_property_descriptor desc[] = {
        DECLARE_NAPI_FUNCTION("changeShape", PluginRender::NapiChangeShape),
        DECLARE_NAPI_FUNCTION("drawTriangle", PluginRender::NapiDrawTriangle),
        DECLARE_NAPI_FUNCTION("changeColor", PluginRender::NapiChangeColor),
    };
    NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
    return exports;
}

napi_value PluginRender::NapiChangeShape(napi_env env, napi_callback_info info)
{
    LOGI("PluginRender::NapiChangeShape");
    PluginRender* instance = PluginRender::GetInstance();
    if (instance) {
        instance->eglCore_->Update();
    }
    return nullptr;
}

napi_value PluginRender::NapiDrawTriangle(napi_env env, napi_callback_info info)
{
    LOGI("NapiDrawTriangle");
    return nullptr;
}

napi_value PluginRender::NapiChangeColor(napi_env env, napi_callback_info info)
{
    LOGI("NapiChangeColor");
    return nullptr;
}

#ifdef __cplusplus
}
#endif