/**
@file			JSFloatArrayKeyframe.cpp
@brief			
@author			James
@version		1.0
@date			2018_7_12
*/

#include "JSFloatArrayKeyframe.h"
#include <v8.h>
#define NDEBUG
#include <assert.h>
#include "V8Util.h"

namespace laya
{
    v8::Persistent<v8::ObjectTemplate> JSFloatArrayKeyframe::persistentObjectTemplate;
    
    //------------------------------------------------------------------------------
    JSFloatArrayKeyframe::JSFloatArrayKeyframe()
    {
        if (JSObjNode::s_pListJSObj)
        {
            JSObjNode::s_pListJSObj->push_back(this);
        }
        v8::Isolate::GetCurrent()->AdjustAmountOfExternalAllocatedMemory(1024);
        //JCMemorySurvey::GetInstance()->newClass("conchFloatArrayKeyframe", 1024, this);
    }
    //------------------------------------------------------------------------------
    JSFloatArrayKeyframe::~JSFloatArrayKeyframe()
    {
        if (JSObjNode::s_pListJSObj)
        {
            JSObjNode::s_pListJSObj->delNode(this);
        }

        //JCMemorySurvey::GetInstance()->releaseClass("conchFloatArrayKeyframe", this);
    }
    static void SetInTangent(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Local<v8::Object> pthis = args.This();
        JSFloatArrayKeyframe *pObj = (JSFloatArrayKeyframe*)(pthis->GetAlignedPointerFromInternalField(0));
        int len = args.Length();
        if (len < 1)
        {
            return;
        }

        char* pArrayBuffer = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = V8Util::extractJSAB(args[0], pArrayBuffer, nABLen);
        if (bIsArrayBuffer)
        {
            pObj->m_nInTangent.data = pArrayBuffer;
            pObj->m_nInTangent.byteSize = nABLen;
        }
    }

    static void SetOutTangent(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Local<v8::Object> pthis = args.This();
        JSFloatArrayKeyframe *pObj = (JSFloatArrayKeyframe*)(pthis->GetAlignedPointerFromInternalField(0));
        int len = args.Length();
        if (len < 1)
        {
            return;
        }

        char* pArrayBuffer = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = V8Util::extractJSAB(args[0], pArrayBuffer, nABLen);
        if (bIsArrayBuffer)
        {
            pObj->m_nOutTangent.data = pArrayBuffer;
            pObj->m_nOutTangent.byteSize = nABLen;
        }
    }

    static void SetValue(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Local<v8::Object> pthis = args.This();
        JSFloatArrayKeyframe *pObj = (JSFloatArrayKeyframe*)(pthis->GetAlignedPointerFromInternalField(0));
        int len = args.Length();
        if (len < 1)
        {
            return;
        }

        char* pArrayBuffer = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = V8Util::extractJSAB(args[0], pArrayBuffer, nABLen);
        if (bIsArrayBuffer)
        {
            pObj->m_nValue.data = pArrayBuffer;
            pObj->m_nValue.byteSize = nABLen;
        }
    }
    static void SetData(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Local<v8::Object> pthis = args.This();
        JSFloatArrayKeyframe *pObj = (JSFloatArrayKeyframe*)(pthis->GetAlignedPointerFromInternalField(0));
        int len = args.Length();
        if (len < 1)
        {
            return;
        }

        char* pArrayBuffer = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = V8Util::extractJSAB(args[0], pArrayBuffer, nABLen);
        if (bIsArrayBuffer)
        {
            pObj->m_pData.data = pArrayBuffer;
            pObj->m_pData.byteSize = nABLen;
        }
    }

    static void SetTime(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Local<v8::Object> pthis = args.This();
        JSFloatArrayKeyframe *pObj = (JSFloatArrayKeyframe*)(pthis->GetAlignedPointerFromInternalField(0));
        int len = args.Length();
        if (len < 1)
        {
            return;
        }
        assert(args[0]->IsNumber());
        float value = args[0]->NumberValue();
        pObj->setTime(value);
    }

    static void GetTime(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Object> pthis = args.This();
        JSFloatArrayKeyframe *pObj = (JSFloatArrayKeyframe*)(pthis->GetAlignedPointerFromInternalField(0));
        float value = pObj->getTime();
        args.GetReturnValue().Set(v8::Number::New(isolate, value));
    }
    static void WeakCallback(const v8::WeakCallbackInfo<JSFloatArrayKeyframe>& data) {
        JSFloatArrayKeyframe* wrap = data.GetParameter();
        wrap->persistentObject.Reset();
        delete wrap;
    }

    static void New(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();

        if (args.IsConstructCall())
        {
            JSFloatArrayKeyframe* obj = new JSFloatArrayKeyframe();

            args.This()->SetAlignedPointerInInternalField(0, obj);
            obj->persistentObject.Reset(isolate, args.This());
            obj->persistentObject.SetWeak(obj, WeakCallback, v8::WeakCallbackType::kParameter);

            args.GetReturnValue().Set(args.This());
        }
    }

    

    void JSFloatArrayKeyframe::exportJS(v8::Local<v8::Object> object)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::HandleScope sc(isolate);
        v8::Local<v8::FunctionTemplate> functionTemplate = v8::FunctionTemplate::New(isolate, New);
        functionTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "_conchFloatArrayKeyframe", v8::NewStringType::kNormal).ToLocalChecked());
        functionTemplate->InstanceTemplate()->SetInternalFieldCount(1); 
        persistentObjectTemplate.Reset(isolate, functionTemplate->InstanceTemplate());

        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "setTime", SetTime);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "getTime", GetTime);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "setInTangent", SetInTangent);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "setOutTangent", SetOutTangent);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "setValue", SetValue);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "setData", SetData);

        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        object->Set(v8::String::NewFromUtf8(isolate, "_conchFloatArrayKeyframe"), functionTemplate->GetFunction(context).ToLocalChecked());
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
