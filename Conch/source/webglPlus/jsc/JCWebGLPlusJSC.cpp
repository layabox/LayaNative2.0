/**
@file			JCWebGLPlus.cpp
@brief			
@author			James
@version		1.0
@date			2019_8_24
*/

#include "JCWebGLPlus.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Log.h"
#include "JSWebGLPlus.h"
#include "JSArrayBufferRef.h"
#include "JSKeyframeNode.h"
#include "JSFloatKeyframe.h"
#include "JSKeyframeNodeList.h"
#include "JSCUtil.h"

namespace laya
{
    void JCWebGLPlus::exportJS(void* ctx, void* object)
    {
        JSWebGLPlus::getInstance()->exportJS((JSContextRef)ctx, (JSObjectRef)object);
        JSArrayBufferRef::exportJS((JSContextRef)ctx, (JSObjectRef)object);
        JSFloatKeyframe::exportJS((JSContextRef)ctx, (JSObjectRef)object);
        JSFloatArrayKeyframe::exportJS((JSContextRef)ctx, (JSObjectRef)object);
        JSKeyframeNode::exportJS((JSContextRef)ctx, (JSObjectRef)object);
        JSKeyframeNodeList::exportJS((JSContextRef)ctx, (JSObjectRef)object);
    }
    void JCWebGLPlus::clean()
    {
        //JSWebGLPlus::getInstance()->releaseInstance();
        JSCBinder<JSWebGLPlus>::ReleaseInstance();
        JSCBinder<JSArrayBufferRef>::ReleaseInstance();
        JSCBinder<JSFloatKeyframe>::ReleaseInstance();
        JSCBinder<JSFloatArrayKeyframe>::ReleaseInstance();
        JSCBinder<JSKeyframeNode>::ReleaseInstance();
        JSCBinder<JSKeyframeNodeList>::ReleaseInstance();
    }
    void JCWebGLPlus::clearAll()
    {
        m_pJSArrayBufferManager->clearAll();
        m_pJSABManagerSyncToRender->clearAll();
        m_pRArrayBufferManager->clearAll();
        clean();
    }
}

//-----------------------------END FILE--------------------------------
