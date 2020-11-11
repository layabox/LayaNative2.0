/**
@file			JSKeyframeNode.cpp
@brief			
@author			James
@version		1.0
@date			2018_7_12
*/

#include "JSKeyframeNode.h"
#define NDEBUG
#include <assert.h>
#include "V8Util.h"

namespace laya
{
    v8::Persistent<v8::ObjectTemplate> JSKeyframeNode::persistentObjectTemplate;
    std::string JCKeyframeNode::s_sTempString = "";
    //------------------------------------------------------------------------------
    JSKeyframeNode::JSKeyframeNode()
    {
        if (JSObjNode::s_pListJSObj)
        {
            JSObjNode::s_pListJSObj->push_back(this);
        }
        v8::Isolate::GetCurrent()->AdjustAmountOfExternalAllocatedMemory(4096);
        //JCMemorySurvey::GetInstance()->newClass("JSKeyframeNode", 4096, this);
    }
    //------------------------------------------------------------------------------
    JSKeyframeNode::~JSKeyframeNode()
    {
        if (JSObjNode::s_pListJSObj)
        {
            JSObjNode::s_pListJSObj->delNode(this);
        }
        //JCMemorySurvey::GetInstance()->releaseClass("JSKeyframeNode", this); 
    }

    static void SetFloat32ArrayData(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Local<v8::Object> pthis = args.This();
        JSKeyframeNode* self = (JSKeyframeNode*)(pthis->GetAlignedPointerFromInternalField(0));
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
            self->m_pDataFloatArray.data = pArrayBuffer;
            self->m_pDataFloatArray.byteSize = nABLen;
        }
    }
   
    static void SetOwnerPathCount(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Isolate* isolate = args.GetIsolate();
        v8::Local< v8::Context> context = isolate->GetCurrentContext();

        int len = args.Length();
        if (len < 1)
        {
            return;
        }

        v8::Local<v8::Object> pthis = args.This();
        JSKeyframeNode* self = (JSKeyframeNode*)(pthis->GetAlignedPointerFromInternalField(0));

        assert(args[0]->IsInt32());
        int index = args[0]->Int32Value();
        self->_setOwnerPathCount(index);
    }
    static void SetOwnerPathByIndex(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Isolate* isolate = args.GetIsolate();
        v8::Local< v8::Context> context = isolate->GetCurrentContext();

        int len = args.Length();
        if (len < 2)
        {
            return;
        }

        v8::Local<v8::Object> pthis = args.This();
        JSKeyframeNode* self = (JSKeyframeNode*)(pthis->GetAlignedPointerFromInternalField(0));

        assert(args[0]->IsInt32());
        int index = args[0]->Int32Value();
        assert(args[1]->IsString());
        v8::String::Utf8Value utf8str(args[1]->ToString());

        self->_setOwnerPathByIndex(index, *utf8str);
    }
    static void JoinOwnerPath(const v8::FunctionCallbackInfo<v8::Value>& args)
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
        JSKeyframeNode* self = (JSKeyframeNode*)(pthis->GetAlignedPointerFromInternalField(0));

        assert(args[0]->IsString());
        v8::String::Utf8Value utf8str(args[0]->ToString());

        const char* value = self->_joinOwnerPath(*utf8str);
        args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, value));
    }
    static void SetPropertyCount(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Isolate* isolate = args.GetIsolate();
        v8::Local< v8::Context> context = isolate->GetCurrentContext();

        int len = args.Length();
        if (len < 1)
        {
            return;
        }

        v8::Local<v8::Object> pthis = args.This();
        JSKeyframeNode* self = (JSKeyframeNode*)(pthis->GetAlignedPointerFromInternalField(0));

        assert(args[0]->IsInt32());
        int index = args[0]->Int32Value();
        self->_setPropertyCount(index);
    }
    static void SetPropertyByIndex(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Isolate* isolate = args.GetIsolate();
        v8::Local< v8::Context> context = isolate->GetCurrentContext();

        int len = args.Length();
        if (len < 2)
        {
            return;
        }

        v8::Local<v8::Object> pthis = args.This();
        JSKeyframeNode* self = (JSKeyframeNode*)(pthis->GetAlignedPointerFromInternalField(0));

        assert(args[0]->IsInt32());
        int index = args[0]->Int32Value();
        assert(args[1]->IsString());
        v8::String::Utf8Value utf8str(args[1]->ToString());

        self->_setPropertyByIndex(index, *utf8str);
    }
    static void JoinProperty(const v8::FunctionCallbackInfo<v8::Value>& args)
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
        JSKeyframeNode* self = (JSKeyframeNode*)(pthis->GetAlignedPointerFromInternalField(0));

        assert(args[0]->IsString());
        v8::String::Utf8Value utf8str(args[0]->ToString());

        const char* value = self->_joinProperty(*utf8str);
        args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, value));
    }
    static void SetKeyframeByIndex0(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Isolate* isolate = args.GetIsolate();
        v8::Local< v8::Context> context = isolate->GetCurrentContext();

        int len = args.Length();
        if (len < 2)
        {
            return;
        }

        v8::Local<v8::Object> pthis = args.This();
        JSKeyframeNode* self = (JSKeyframeNode*)(pthis->GetAlignedPointerFromInternalField(0));

        assert(args[0]->IsInt32());
        int index = args[0]->Int32Value();

        v8::Local<v8::Value> keyframe = args[1];

        if (!keyframe.IsEmpty() && keyframe->IsObject())
        {
            JSFloatKeyframe* pNode = (JSFloatKeyframe*)keyframe.As<v8::Object>()->GetAlignedPointerFromInternalField(0);
            if (pNode)
            {
                self->m_vKeyFrames[index] = pNode;
            }
        }
    }
    static void SetKeyframeByIndex1(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Isolate* isolate = args.GetIsolate();
        v8::Local< v8::Context> context = isolate->GetCurrentContext();

        int len = args.Length();
        if (len < 2)
        {
            return;
        }

        v8::Local<v8::Object> pthis = args.This();
        JSKeyframeNode* self = (JSKeyframeNode*)(pthis->GetAlignedPointerFromInternalField(0));

        assert(args[0]->IsInt32());
        int index = args[0]->Int32Value();

        v8::Local<v8::Value> keyframe = args[1];

        if (!keyframe.IsEmpty() && keyframe->IsObject())
        {
            JSFloatArrayKeyframe* pNode = (JSFloatArrayKeyframe*)keyframe.As<v8::Object>()->GetAlignedPointerFromInternalField(0);
            if (pNode)
            {
                self->m_vKeyFrames[index] = pNode;
            }
        }
    }
    static void SetKeyframeCount(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Object> pthis = args.This();
        int len = args.Length();
        if (len < 1)
        {
            return;
        }

        JSKeyframeNode* self = (JSKeyframeNode*)(pthis->GetAlignedPointerFromInternalField(0));

        assert(args[0]->IsInt32());
        int value = args[0]->Int32Value();

        self->_setKeyframeCount(value);
    }
    static void GetOwnerPathByIndex(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Object> pthis = args.This();
        int len = args.Length();
        if (len < 1)
        {
            args.GetReturnValue().Set(v8::Null(isolate));
            return;
        }

        JSKeyframeNode* self = (JSKeyframeNode*)(pthis->GetAlignedPointerFromInternalField(0));

        assert(args[0]->IsInt32());
        int index = args[0]->Int32Value();

        const char* value = self->getOwnerPathByIndex(index);

        args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, value));
    }
    static void GetPropertyByIndex(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Object> pthis = args.This();
        int len = args.Length();
        if (len < 1)
        {
            args.GetReturnValue().Set(v8::Null(isolate));
            return;
        }

        JSKeyframeNode* self = (JSKeyframeNode*)(pthis->GetAlignedPointerFromInternalField(0));

        assert(args[0]->IsInt32());
        int index = args[0]->Int32Value();

        const char* value = self->getPropertyByIndex(index);

        args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, value));
    }
    static void GetKeyframeByIndex(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Isolate* isolate = args.GetIsolate();
        args.GetReturnValue().Set(v8::Null(isolate));
    }
    static void GetFloatData(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Object> pthis = args.This();
        JSKeyframeNode *pObj = (JSKeyframeNode*)(pthis->GetAlignedPointerFromInternalField(0));
        float value = pObj->getFloatData();
        args.GetReturnValue().Set(v8::Number::New(isolate, value));
    }

    static void GetDataType(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Local<v8::Object> pthis = args.This();
        JSKeyframeNode *pObj = (JSKeyframeNode*)(pthis->GetAlignedPointerFromInternalField(0));
        int value = pObj->getDataType();
        args.GetReturnValue().Set(v8::Int32::New(v8::Isolate::GetCurrent(), value));
    }

    void JSKeyframeNode::WeakCallback(const v8::WeakCallbackInfo<JSKeyframeNode>& data) {
        JSKeyframeNode* wrap = data.GetParameter();
        wrap->persistentObject.Reset();
        delete wrap;
    }

    static void New(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();

        if (args.IsConstructCall())
        {
            JSKeyframeNode* obj = new JSKeyframeNode();

            args.This()->SetAlignedPointerInInternalField(0, obj);
            obj->persistentObject.Reset(isolate, args.This());
            obj->persistentObject.SetWeak(obj, JSKeyframeNode::WeakCallback, v8::WeakCallbackType::kParameter);

            args.GetReturnValue().Set(args.This());
        }
    }

    static void GetKeyFramesCount(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Object> pthis = args.This();
        JSKeyframeNode* self = (JSKeyframeNode*)(pthis->GetAlignedPointerFromInternalField(0));
        int value = self->getKeyFramesCount();
        args.GetReturnValue().Set(v8::Int32::New(isolate, value));
    }
    static void GetOwnerPathCount(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Object> pthis = args.This();
        JSKeyframeNode* self = (JSKeyframeNode*)(pthis->GetAlignedPointerFromInternalField(0));
        int value = self->getOwnerPathCount();
        args.GetReturnValue().Set(v8::Int32::New(isolate, value));
    }
    static void GetPropertyCount(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Object> pthis = args.This();
        laya::JSKeyframeNode* self = (JSKeyframeNode*)(pthis->GetAlignedPointerFromInternalField(0));
        int value = self->getPropertyCount();
        args.GetReturnValue().Set(v8::Int32::New(isolate, value));
    }
    static void _GetIndexInList(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
    {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Object> pthis = info.This();

        JSKeyframeNode* self = (JSKeyframeNode*)pthis->GetAlignedPointerFromInternalField(0);
        int v = self->getIndexInList();
        info.GetReturnValue().Set(v8::Int32::New(isolate, v));
    }
    static void _SetIndexInList(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
    {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Object> pthis = info.This();
        JSKeyframeNode* self = (JSKeyframeNode*)pthis->GetAlignedPointerFromInternalField(0);

        assert(value->IsInt32());
        int v = value->Int32Value();
        self->setIndexInList(v);
    }
    static void _GetType(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
    {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Object> pthis = info.This();

        JSKeyframeNode* self = (JSKeyframeNode*)pthis->GetAlignedPointerFromInternalField(0);
        int v = self->getType();
        info.GetReturnValue().Set(v8::Int32::New(isolate, v));
    }
    static void _SetType(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
    {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Object> pthis = info.This();
        JSKeyframeNode* self = (JSKeyframeNode*)pthis->GetAlignedPointerFromInternalField(0);

        assert(value->IsInt32());
        int v = value->Int32Value();
        self->setType(v);
    }
    static void _GetOwnerPathCount(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
    {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Object> pthis = info.This();

        JSKeyframeNode* self = (JSKeyframeNode*)pthis->GetAlignedPointerFromInternalField(0);
        int v = self->getOwnerPathCount();
        info.GetReturnValue().Set(v8::Int32::New(isolate, v));
    }
    static void _SetOwnerPathCount(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
    {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Object> pthis = info.This();
        JSKeyframeNode* self = (JSKeyframeNode*)pthis->GetAlignedPointerFromInternalField(0);

        assert(value->IsInt32());
        int v = value->Int32Value();
        self->_setOwnerPathCount(v);
    }
    static void _GetKeyFramesCount(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
    {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Object> pthis = info.This();

        JSKeyframeNode* self = (JSKeyframeNode*)pthis->GetAlignedPointerFromInternalField(0);
        int v = self->getKeyFramesCount();
        info.GetReturnValue().Set(v8::Int32::New(isolate, v));
    }
    static void _SetKeyframeCount(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
    {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Object> pthis = info.This();
        JSKeyframeNode* self = (JSKeyframeNode*)pthis->GetAlignedPointerFromInternalField(0);

        assert(value->IsInt32());
        int v = value->Int32Value();
        self->_setKeyframeCount(v);
    }
    static void _GetPropertyCount(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
    {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Object> pthis = info.This();

        JSKeyframeNode* self = (JSKeyframeNode*)pthis->GetAlignedPointerFromInternalField(0);
        int v = self->getPropertyCount();
        info.GetReturnValue().Set(v8::Int32::New(isolate, v));
    }
    static void _SetPropertyCount(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
    {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Object> pthis = info.This();
        JSKeyframeNode* self = (JSKeyframeNode*)pthis->GetAlignedPointerFromInternalField(0);

        assert(value->IsInt32());
        int v = value->Int32Value();
        self->_setPropertyCount(v);
    }
    static void _GetPropertyOwner(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
    {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Object> pthis = info.This();

        JSKeyframeNode* self = (JSKeyframeNode*)pthis->GetAlignedPointerFromInternalField(0);
        const char* v = self->getPropertyOwner();
        info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, v));
    }
    static void _SetPropertyOwner(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
    {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Object> pthis = info.This();
        JSKeyframeNode* self = (JSKeyframeNode*)pthis->GetAlignedPointerFromInternalField(0);

        assert(value->IsString());
        v8::String::Utf8Value utf8str(value->ToString());
        self->setPropertyOwner(*utf8str);
    }
    static void _GetFullPath(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
    {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Object> pthis = info.This();

        JSKeyframeNode* self = (JSKeyframeNode*)pthis->GetAlignedPointerFromInternalField(0);
        const char* v = self->getFullPath();
        info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, v));
    }
    static void _SetFullPath(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
    {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Object> pthis = info.This();
        JSKeyframeNode* self = (JSKeyframeNode*)pthis->GetAlignedPointerFromInternalField(0);

        assert(value->IsString());
        v8::String::Utf8Value utf8str(value->ToString());
        self->setFullPath(*utf8str);
    }
    void JSKeyframeNode::exportJS(v8::Local<v8::Object> object)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::HandleScope sc(isolate);
        v8::Local<v8::FunctionTemplate> functionTemplate = v8::FunctionTemplate::New(isolate, New);
        functionTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "_conchKeyframeNode", v8::NewStringType::kNormal).ToLocalChecked());
        functionTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        persistentObjectTemplate.Reset(isolate, functionTemplate->InstanceTemplate());
        
        functionTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "_indexInList"), _GetIndexInList, _SetIndexInList);
        functionTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "type"), _GetType, _SetType);
        functionTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "fullPath"), _GetFullPath, _SetFullPath);
        functionTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "propertyOwner"), _GetPropertyOwner, _SetPropertyOwner);
        functionTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "ownerPathCount"), _GetOwnerPathCount, _SetOwnerPathCount);
        functionTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "keyFramesCount"), _GetKeyFramesCount, _SetKeyframeCount);
        functionTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "propertyCount"), _GetPropertyCount, _SetPropertyCount);
      
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "getOwnerPathCount", GetOwnerPathCount);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "getPropertyCount", GetPropertyCount);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "getKeyFramesCount", GetKeyFramesCount);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "_setOwnerPathCount", SetOwnerPathCount);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "_setOwnerPathByIndex", SetOwnerPathByIndex);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "_joinOwnerPath", JoinOwnerPath);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "_setPropertyCount", SetPropertyCount);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "_setPropertyByIndex", SetPropertyByIndex);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "_joinProperty", JoinProperty);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "_setKeyframeCount", SetKeyframeCount);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "_setKeyframeByIndex0", SetKeyframeByIndex0);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "_setKeyframeByIndex1", SetKeyframeByIndex1);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "getOwnerPathByIndex", GetOwnerPathByIndex);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "getPropertyByIndex", GetPropertyByIndex);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "getKeyframeByIndex", GetKeyframeByIndex);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "getDataType", GetDataType);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "getFloatData", GetFloatData);
        V8Util::SET_PROTOTYPE_METHOD(functionTemplate, "setFloat32ArrayData", SetFloat32ArrayData);

        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        object->Set(v8::String::NewFromUtf8(isolate, "_conchKeyframeNode"), functionTemplate->GetFunction(context).ToLocalChecked());
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
