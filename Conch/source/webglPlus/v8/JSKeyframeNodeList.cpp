/**
@file			JSKeyframeNodeList.cpp
@brief			
@author			James
@version		1.0
@date			2018_7_12
*/

#include "JSKeyframeNodeList.h"
#include <v8.h>
#define NDEBUG
#include <assert.h>
#include "V8Util.h"

namespace laya
{
    v8::Persistent<v8::ObjectTemplate> JSKeyframeNodeList::persistentObjectTemplate;
    //------------------------------------------------------------------------------
    JSKeyframeNodeList::JSKeyframeNodeList()
    {
        if (JSObjNode::s_pListJSObj)
        {
            JSObjNode::s_pListJSObj->push_back(this);
        }
        v8::Isolate::GetCurrent()->AdjustAmountOfExternalAllocatedMemory(8192);
        //JCMemorySurvey::GetInstance()->newClass("JSKeyframeNodeList", 8192, this);
    }
    //------------------------------------------------------------------------------
    JSKeyframeNodeList::~JSKeyframeNodeList()
    {
        if (JSObjNode::s_pListJSObj)
        {
            JSObjNode::s_pListJSObj->delNode(this);
        }
        //JCMemorySurvey::GetInstance()->releaseClass("JSKeyframeNodeList", this);
    }

    static void SetCount(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Local<v8::Object> pthis = args.This();
        laya::JSKeyframeNodeList *pObj = (laya::JSKeyframeNodeList*)(pthis->GetAlignedPointerFromInternalField(0));
        int len = args.Length();
        if (len < 1)
        {
            return;
        }
        assert(args[0]->IsInt32());
        int value = args[0]->Int32Value();
        pObj->setCount(value);
    }

    static void GetCount(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Local<v8::Object> pthis = args.This();
        laya::JSKeyframeNodeList *pObj = (laya::JSKeyframeNodeList*)(pthis->GetAlignedPointerFromInternalField(0));
        int value = pObj->getCount();
        args.GetReturnValue().Set(v8::Int32::New(v8::Isolate::GetCurrent(), value));
    }

    static void GetNodeByIndex(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Isolate* isolate = args.GetIsolate();
        v8::Local< v8::Context> context = isolate->GetCurrentContext();

        int len = args.Length();
        if (len < 1)
        {
            args.GetReturnValue().Set(v8::Null(isolate));
            return;
        }

        v8::Local<v8::Object> pthis = args.This();
        laya::JSKeyframeNodeList* self = (laya::JSKeyframeNodeList*)(pthis->GetAlignedPointerFromInternalField(0));

        assert(args[0]->IsInt32());
        int index = args[0]->Int32Value();

        JSKeyframeNode* pNode = (JSKeyframeNode*)self->m_vNodes[index];
        v8::Local<v8::ObjectTemplate> objectTemplate = v8::Local<v8::ObjectTemplate>::New(isolate, JSKeyframeNode::persistentObjectTemplate);
        v8::Local<v8::Object> pNewIns = objectTemplate->NewInstance();
        pNewIns->SetAlignedPointerInInternalField(0, pNode);
        pNode->persistentObject.Reset(isolate, pNewIns);
        pNode->persistentObject.SetWeak(pNode, JSKeyframeNode::WeakCallback, v8::WeakCallbackType::kInternalFields);
    }

    static void SetNodeByIndex(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Isolate* isolate = args.GetIsolate();
        v8::Local< v8::Context> context = isolate->GetCurrentContext();

        int len = args.Length();
        if (len < 2)
        {
            args.GetReturnValue().Set(v8::Null(isolate));
            return;
        }

        v8::Local<v8::Object> pthis = args.This();
        JSKeyframeNodeList* self = (JSKeyframeNodeList*)(pthis->GetAlignedPointerFromInternalField(0));

        assert(args[0]->IsInt32());
        int index = args[0]->Int32Value();

        v8::Local<v8::Value> keyframeNode = args[1];

        if (!keyframeNode.IsEmpty() && keyframeNode->IsObject())
        {
            JSKeyframeNode* pNode = (JSKeyframeNode*)keyframeNode.As<v8::Object>()->GetAlignedPointerFromInternalField(0);
            if (pNode)
            {
                self->m_vNodes[index] = pNode;
            }
        }
    }
    static void WeakCallback(const v8::WeakCallbackInfo<JSKeyframeNodeList>& data) {
        JSKeyframeNodeList* wrap = data.GetParameter();
        wrap->persistentObject.Reset();
        delete wrap;
    }

    static void New(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();

        if (args.IsConstructCall())
        {
            JSKeyframeNodeList* obj = new JSKeyframeNodeList();

            args.This()->SetAlignedPointerInInternalField(0, obj);
            obj->persistentObject.Reset(isolate, args.This());
            obj->persistentObject.SetWeak(obj, WeakCallback, v8::WeakCallbackType::kParameter);

            args.GetReturnValue().Set(args.This());
        }
    }

    void JSKeyframeNodeList::exportJS(v8::Local<v8::Object> object)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::HandleScope sc(isolate);
        v8::Local<v8::FunctionTemplate> functionTemplate = v8::FunctionTemplate::New(isolate, New);
        functionTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "_conchKeyframeNodeList", v8::NewStringType::kNormal).ToLocalChecked());
        functionTemplate->InstanceTemplate()->SetInternalFieldCount(1);

        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "setNodeByIndex", SetNodeByIndex);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "getNodeByIndex", GetNodeByIndex);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "getCount", GetCount);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "setCount", SetCount);

        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        object->Set(v8::String::NewFromUtf8(isolate, "_conchKeyframeNodeList"), functionTemplate->GetFunction(context).ToLocalChecked());
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
