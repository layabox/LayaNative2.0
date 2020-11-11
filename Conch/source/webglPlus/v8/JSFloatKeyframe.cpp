/**
@file			JSFloatKeyframe.cpp
@brief			
@author			James
@version		1.0
@date			2018_7_12
*/

#include "JSFloatKeyframe.h"
#define NDEBUG
#include <assert.h>
#include "V8Util.h"

namespace laya
{
    v8::Persistent<v8::ObjectTemplate> JSFloatKeyframe::persistentObjectTemplate;
    //------------------------------------------------------------------------------
    JSFloatKeyframe::JSFloatKeyframe()
    {
        if (JSObjNode::s_pListJSObj)
        {
            JSObjNode::s_pListJSObj->push_back(this);
        }
        v8::Isolate::GetCurrent()->AdjustAmountOfExternalAllocatedMemory(16);
        //JCMemorySurvey::GetInstance()->newClass("conchFloatKeyframe", 16, this);
    }
    JSFloatKeyframe::~JSFloatKeyframe()
    {
        if (JSObjNode::s_pListJSObj)
        {
            JSObjNode::s_pListJSObj->delNode(this);
        }
        //JCMemorySurvey::GetInstance()->releaseClass("conchFloatKeyframe", this);
    }
    void JSFloatKeyframe::WeakCallback(const v8::WeakCallbackInfo<JSFloatKeyframe>& data) {
        JSFloatKeyframe* wrap = data.GetParameter();
        wrap->persistentObject.Reset();
        delete wrap;
    }

    static void New(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();

        if (args.IsConstructCall())
        {
            JSFloatKeyframe* obj = new JSFloatKeyframe();

            args.This()->SetAlignedPointerInInternalField(0, obj);
            obj->persistentObject.Reset(isolate, args.This());
            obj->persistentObject.SetWeak(obj, JSFloatKeyframe::WeakCallback, v8::WeakCallbackType::kParameter);

            args.GetReturnValue().Set(args.This());
        }
    }

    static void Clone(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Isolate* isolate = args.GetIsolate();
        v8::Local< v8::Context> context = isolate->GetCurrentContext();

        v8::Local<v8::Object> pthis = args.This();
        JSFloatKeyframe *self = (JSFloatKeyframe*)(pthis->GetAlignedPointerFromInternalField(0));

        JSFloatKeyframe* pKeyframe = new JSFloatKeyframe();
        self->_cloneTo(pKeyframe);


        v8::Local<v8::ObjectTemplate> objectTemplate = v8::Local<v8::ObjectTemplate>::New(isolate, JSFloatKeyframe::persistentObjectTemplate);
        v8::Local<v8::Object> pNewIns = objectTemplate->NewInstance();
        pNewIns->SetAlignedPointerInInternalField(0, pKeyframe);
        pKeyframe->persistentObject.Reset(isolate, pNewIns);
        pKeyframe->persistentObject.SetWeak(pKeyframe, JSFloatKeyframe::WeakCallback, v8::WeakCallbackType::kParameter);
        args.GetReturnValue().Set(pNewIns);
    }
    static void CloneTo(const v8::FunctionCallbackInfo<v8::Value>& args)
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

        JSFloatKeyframe* self = (JSFloatKeyframe*)(pthis->GetAlignedPointerFromInternalField(0));

        v8::Local<v8::Value> destObj = args[0];

        if (!destObj.IsEmpty() && destObj->IsObject())
        {
            JSFloatKeyframe* pKeyFrame = (JSFloatKeyframe*)destObj.As<v8::Object>()->GetAlignedPointerFromInternalField(0);
            if (pKeyFrame)
            {
                self->_cloneTo(pKeyFrame);
            }
        }
    }
    static void _GetTime(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
    {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Object> pthis = info.This();
       
        JSFloatKeyframe* self = (JSFloatKeyframe*)pthis->GetAlignedPointerFromInternalField(0);
        float v = self->getTime();
        info.GetReturnValue().Set(v8::Number::New(isolate, v));
    }
    static void _SetTime(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info) 
    {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Object> pthis = info.This();
        JSFloatKeyframe* self = (JSFloatKeyframe*)pthis->GetAlignedPointerFromInternalField(0);
        
        assert(value->IsNumber());
        float v = value->NumberValue();
        self->setTime(v);
    }
    static void _GetInTangent(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
    {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Object> pthis = info.This();

        JSFloatKeyframe* self = (JSFloatKeyframe*)pthis->GetAlignedPointerFromInternalField(0);
        float v = self->getInTangent();
        info.GetReturnValue().Set(v8::Number::New(isolate, v));
    }

    static void _SetInTangent(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info) 
    {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Object> pthis = info.This();
        JSFloatKeyframe* self = (JSFloatKeyframe*)pthis->GetAlignedPointerFromInternalField(0);

        assert(value->IsNumber());
        float v = value->NumberValue();
        self->setInTangent(v);
    }
    static void _GetOutTangent(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
    {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Object> pthis = info.This();

        JSFloatKeyframe* self = (JSFloatKeyframe*)pthis->GetAlignedPointerFromInternalField(0);
        float v = self->getOutTangent();
        info.GetReturnValue().Set(v8::Number::New(isolate, v));
    }
    static void _SetOutTangent(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
    {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Object> pthis = info.This();
        JSFloatKeyframe* self = (JSFloatKeyframe*)pthis->GetAlignedPointerFromInternalField(0);

        assert(value->IsNumber());
        float v = value->NumberValue();
        self->setOutTangent(v);
    }
    static void _GetValue(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
    {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Object> pthis = info.This();

        JSFloatKeyframe* self = (JSFloatKeyframe*)pthis->GetAlignedPointerFromInternalField(0);
        float v = self->getValue();
        info.GetReturnValue().Set(v8::Number::New(isolate, v));
    }
    static void _SetValue(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
    {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Object> pthis = info.This();
        JSFloatKeyframe* self = (JSFloatKeyframe*)pthis->GetAlignedPointerFromInternalField(0);

        assert(value->IsNumber());
        float v = value->NumberValue();
        self->setValue(v);
    }
    void JSFloatKeyframe::exportJS(v8::Local<v8::Object> object)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::HandleScope sc(isolate);
        v8::Local<v8::FunctionTemplate> functionTemplate = v8::FunctionTemplate::New(isolate, New);
        functionTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "conchFloatKeyframe", v8::NewStringType::kNormal).ToLocalChecked());
        functionTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        persistentObjectTemplate.Reset(isolate, functionTemplate->InstanceTemplate());

        functionTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "time"), _GetTime, _SetTime);
        functionTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "inTangent"), _GetInTangent, _SetInTangent);
        functionTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "outTangent"), _GetOutTangent, _SetOutTangent);
        functionTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "value"), _GetValue, _SetValue);

        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "clone", Clone);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "cloneTo", CloneTo);

        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        object->Set(v8::String::NewFromUtf8(isolate, "conchFloatKeyframe"), functionTemplate->GetFunction(context).ToLocalChecked());
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------