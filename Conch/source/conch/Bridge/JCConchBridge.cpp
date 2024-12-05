/**
@file			JCConchBridge.cpp
@brief
@author			James
@version		1.0
@date			2016_5_27
*/

#include "JCConchBridge.h"
#include "../JCScriptRuntime.h"
#include "../JSWrapper/LayaWrap/JSCallbackFuncObj.h"

namespace laya
{
    void JCConchBridge::getPixelsRenderToJS(unsigned char * pPixels, int nSize, int w, int h, int callbackObjID, int funcID)
    {
        if (JCScriptRuntime::s_JSRT) {
            JCScriptRuntime::s_JSRT->m_pScriptThread->post([pPixels, nSize, w, h, callbackObjID, funcID]() {
                JSCallbackFuncObj* pCallbackObj = JCScriptRuntime::s_JSRT->m_pCallbackFuncManager->getRes(callbackObjID);
                if (pCallbackObj)
                {
#ifdef JS_V8
                    v8::HandleScope scope(v8::Isolate::GetCurrent());
#elif JS_JSVM
                    AutoHandleScope scope;
#endif
                    JsValue ab = createJSAB((char*)pPixels, nSize);
                    pCallbackObj->callJS(funcID, ab);
                    delete[] pPixels;
                }
            });
        }
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
