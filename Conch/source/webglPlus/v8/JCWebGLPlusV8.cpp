/**
@file			JCWebGLPlus.cpp
@brief			
@author			James
@version		1.0
@date			2019_8_24
*/

#include "../JCWebGLPlus.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../Log.h"
#include "JSWebGLPlus.h"
#include "JSArrayBufferRef.h"
#include "JSKeyframeNode.h"
#include "JSFloatKeyframe.h"
#include "JSKeyframeNodeList.h"

namespace laya
{
    void JCWebGLPlus::exportJS(void* ctx, void* object)
    {
        JSWebGLPlus::getInstance()->exportJS(*(v8::Local<v8::Object>*)object);
        JSArrayBufferRef::exportJS(*(v8::Local<v8::Object>*)object);
        JSFloatKeyframe::exportJS(*(v8::Local<v8::Object>*)object);
        JSFloatArrayKeyframe::exportJS(*(v8::Local<v8::Object>*)object);
        JSKeyframeNode::exportJS(*(v8::Local<v8::Object>*)object);
        JSKeyframeNodeList::exportJS(*(v8::Local<v8::Object>*)object);
    }
    void JCWebGLPlus::clean()
    {
    }
	void JCWebGLPlus::clearAll()
	{
		if (JSWebGLPlus::s_pWebGLPlus != NULL)
		{
			JSWebGLPlus::s_pWebGLPlus->persistentObject.Reset();
		}
        JSArrayBufferRef::persistentObjectTemplate.Reset();
        JSFloatKeyframe::persistentObjectTemplate.Reset();
        JSFloatArrayKeyframe::persistentObjectTemplate.Reset();
        JSKeyframeNode::persistentObjectTemplate.Reset();
        JSKeyframeNodeList::persistentObjectTemplate.Reset();

        m_pJSArrayBufferManager->clearAll();
        m_pJSABManagerSyncToRender->clearAll();
        m_pRArrayBufferManager->clearAll();
        clean();
    }
}

//-----------------------------END FILE--------------------------------
