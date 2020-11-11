/**
@file			JSArrayBufferRef.cpp
@brief			
@author			James
@version		1.0
@date			2017_11_29
*/

#include "JSArrayBufferRef.h"
#include "../JCWebGLPlus.h"
#include "V8Util.h"

namespace laya
{
    v8::Persistent<v8::ObjectTemplate> JSArrayBufferRef::persistentObjectTemplate;
    JSArrayBufferRef::JSArrayBufferRef()
    {
        if (JSObjNode::s_pListJSObj)
        {
            JSObjNode::s_pListJSObj->push_back(this);
        }

        m_nID = 0;
        m_bSyncToRender = false;
        v8::Isolate::GetCurrent()->AdjustAmountOfExternalAllocatedMemory(128);
    }
    JSArrayBufferRef::~JSArrayBufferRef()
    {
        if (JSObjNode::s_pListJSObj)
        {
            JSObjNode::s_pListJSObj->delNode(this);
        }
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
    static void _GetID(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
    {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Object> pthis = info.This();

        JSArrayBufferRef* self = (JSArrayBufferRef*)pthis->GetAlignedPointerFromInternalField(0);
        int v = self->getID();
        info.GetReturnValue().Set(v8::Int32::New(isolate, v));
    }

    static void IsSyncToRender(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Object> pthis = args.This();
        JSArrayBufferRef *pObj = (JSArrayBufferRef*)(pthis->GetAlignedPointerFromInternalField(0));
        bool value = pObj->getIsSyncToRender();
        args.GetReturnValue().Set(v8::Boolean::New(isolate, value));
    }
    void JSArrayBufferRef::WeakCallback(const v8::WeakCallbackInfo<JSArrayBufferRef>& data)
    {
        JSArrayBufferRef* wrap = data.GetParameter();
        wrap->persistentObject.Reset();
        delete wrap;
    }
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();

        if (args.IsConstructCall())
        {
            JSArrayBufferRef* obj = new JSArrayBufferRef();
            args.This()->SetAlignedPointerInInternalField(0, obj);
            obj->persistentObject.Reset(isolate, args.This());
            obj->persistentObject.SetWeak(obj, JSArrayBufferRef::WeakCallback, v8::WeakCallbackType::kParameter);

            args.GetReturnValue().Set(args.This());
        }
    }

    void JSArrayBufferRef::exportJS(v8::Local<v8::Object> object)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::HandleScope sc(isolate);
        v8::Local<v8::FunctionTemplate> functionTemplate = v8::FunctionTemplate::New(isolate, New);
        functionTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "ArrayBufferRef", v8::NewStringType::kNormal).ToLocalChecked());
        v8::Local<v8::ObjectTemplate> instanceTemplate = functionTemplate->InstanceTemplate();
        persistentObjectTemplate.Reset(isolate, instanceTemplate);
        instanceTemplate->SetInternalFieldCount(1);
        instanceTemplate->SetAccessor(v8::String::NewFromUtf8(isolate, "id"), _GetID, 0, v8::Local<v8::Value>(), v8::DEFAULT, v8::ReadOnly);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "isSyncToRender", IsSyncToRender);
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        object->Set(v8::String::NewFromUtf8(isolate, "ArrayBufferRef"), functionTemplate->GetFunction(context).ToLocalChecked());
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
