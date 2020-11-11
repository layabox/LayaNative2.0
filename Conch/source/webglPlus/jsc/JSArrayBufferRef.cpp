/**
@file			JSArrayBufferRef.cpp
@brief			
@author			James
@version		1.0
@date			2017_11_29
*/

#include "JSArrayBufferRef.h"
#include "../JCWebGLPlus.h"
#include "JSCUtil.h"

namespace laya
{
    //ADDJSCLSINFO(JSArrayBufferRef, JSObjNode);
    JSArrayBufferRef::JSArrayBufferRef()
    {
        if (JSObjNode::s_pListJSObj)
        {
            JSObjNode::s_pListJSObj->push_back(this);
        }

        m_nID = 0;
        m_bSyncToRender = false;
    }
    JSArrayBufferRef::~JSArrayBufferRef()
    {
        callManagerRemoveArrayBuffer();
    }
    void JSArrayBufferRef::callManagerRemoveArrayBuffer()
    {
        if (JCWebGLPlus::getInstance()->m_nThreadMODE == THREAD_MODE_DOUBLE)
        {
            if (m_bSyncToRender)
            {
                JCWebGLPlus::getInstance()->m_pJSABManagerSyncToRender->prepareRemoveArrayBuffer(m_nID);
            }
            else
            {
                JCWebGLPlus::getInstance()->m_pJSArrayBufferManager->removeArrayBuffer(m_nID);
            }
        }
        else
        {
            JCWebGLPlus::getInstance()->m_pJSArrayBufferManager->removeArrayBuffer(m_nID);
        }
    }
    int JSArrayBufferRef::getID()
    {
        return m_nID;
    }
    bool JSArrayBufferRef::getIsSyncToRender()
    {
        return m_bSyncToRender;
    }
    static JSValueRef _GetID(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
    {
        JSArrayBufferRef* self = (JSArrayBufferRef*)JSObjectGetPrivate(object);
        int v = self->getID();
        return JSValueMakeNumber(ctx, v);
    }
    static JSValueRef IsSyncToRender(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        JSArrayBufferRef *pObj = (JSArrayBufferRef*)JSObjectGetPrivate(thisObject);
        bool value = pObj->getIsSyncToRender();
        return JSValueMakeBoolean(ctx, value);
    }

    void JSArrayBufferRef::exportJS(JSContextRef ctx, JSObjectRef object)
    {
        JSCBinder<JSArrayBufferRef>* binder = JSCBinder<JSArrayBufferRef>::GetInstance();
        binder->begin(ctx);
        binder->addProperty("id", _GetID);
        binder->addMethod("isSyncToRender", IsSyncToRender);
        binder->end(object, "ArrayBufferRef");
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
