/**
@file			JSFloatKeyframe.cpp
@brief			
@author			James
@version		1.0
@date			2018_7_12
*/

#include "JSFloatKeyframe.h"
#include "JSCUtil.h"

namespace laya
{
    //ADDJSCLSINFO(JSFloatKeyframe, JSObjNode);
    //------------------------------------------------------------------------------
    JSFloatKeyframe::JSFloatKeyframe()
    {
        if (JSObjNode::s_pListJSObj)
        {
            JSObjNode::s_pListJSObj->push_back(this);
        }
        /*m_nTime = 0;
        m_nInTangent = 0;
        m_nOutTangent = 0;
        m_nValue = 0;
        AdjustAmountOfExternalAllocatedMemory(16);
        JCMemorySurvey::GetInstance()->newClass("conchFloatKeyframe", 16, this);*/
    }
    JSFloatKeyframe::~JSFloatKeyframe()
    {
        if (JSObjNode::s_pListJSObj)
        {
            JSObjNode::s_pListJSObj->delNode(this);
        }
        //JCMemorySurvey::GetInstance()->releaseClass("conchFloatKeyframe", this);
    }
    static JSValueRef Clone(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        JSFloatKeyframe* self = (JSFloatKeyframe*)JSObjectGetPrivate(thisObject);
        JSFloatKeyframe* pKeyframe = new JSFloatKeyframe();
        self->_cloneTo(pKeyframe);
        return JSCBinder<JSFloatKeyframe>::GetInstance()->transferObjPtrToJS(ctx, pKeyframe);
    }
    static JSValueRef CloneTo(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        if (argumentCount < 1)
        {
            return JSValueMakeUndefined(ctx);
        }
        JSFloatKeyframe* self = (JSFloatKeyframe*)JSObjectGetPrivate(thisObject);
        JSValueRef destObj = arguments[0];
        if (JSValueIsObject(ctx, destObj))
        {
            JSFloatKeyframe* pKeyFrame = (JSFloatKeyframe*)JSObjectGetPrivate(JSValueToObject(ctx, destObj, NULL));
            if (pKeyFrame)
            {
                self->_cloneTo(pKeyFrame);
            }
        }
        return JSValueMakeUndefined(ctx);
    }
    static JSValueRef _GetTime(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
    {
        JSFloatKeyframe* self = (JSFloatKeyframe*)JSObjectGetPrivate(object);
        float v = self->getTime();
        return JSValueMakeNumber(ctx, v);
    }
    static bool _SetTime(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception)
    {
        JSFloatKeyframe* self = (JSFloatKeyframe*)JSObjectGetPrivate(object);
        assert(JSValueIsNumber(ctx, value));
        float v = JSValueToNumber(ctx, value, 0);
        self->setTime(v);
        return true;
    }
    static JSValueRef _GetInTangent(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
    {
        JSFloatKeyframe* self = (JSFloatKeyframe*)JSObjectGetPrivate(object);
        float v = self->getInTangent();
        return JSValueMakeNumber(ctx, v);
    }
    static bool _SetInTangent(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception)
    {
        JSFloatKeyframe* self = (JSFloatKeyframe*)JSObjectGetPrivate(object);
        assert(JSValueIsNumber(ctx, value));
        float v = JSValueToNumber(ctx, value, 0);
        self->setInTangent(v);
        return true;
    }
    static JSValueRef _GetOutTangent(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
    {
        JSFloatKeyframe* self = (JSFloatKeyframe*)JSObjectGetPrivate(object);
        float v = self->getOutTangent();
        return JSValueMakeNumber(ctx, v);
    }
    static bool _SetOutTangent(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception)
    {
        JSFloatKeyframe* self = (JSFloatKeyframe*)JSObjectGetPrivate(object);
        assert(JSValueIsNumber(ctx, value));
        float v = JSValueToNumber(ctx, value, 0);
        self->setOutTangent(v);
        return true;
    }
    static JSValueRef _GetValue(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
    {
        JSFloatKeyframe* self = (JSFloatKeyframe*)JSObjectGetPrivate(object);
        float v = self->getValue();
        return JSValueMakeNumber(ctx, v);
    }
    static bool _SetValue(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception)
    {
        JSFloatKeyframe* self = (JSFloatKeyframe*)JSObjectGetPrivate(object);
        assert(JSValueIsNumber(ctx, value));
        float v = JSValueToNumber(ctx, value, 0);
        self->setValue(v);
        return true;
    }

    void JSFloatKeyframe::exportJS(JSContextRef ctx, JSObjectRef object)
    {
        JSCBinder<JSFloatKeyframe>* binder = JSCBinder<JSFloatKeyframe>::GetInstance();
        binder->begin(ctx);
        binder->addProperty("time", _GetTime, _SetTime);
        binder->addProperty("inTangent", _GetInTangent, _SetInTangent);
        binder->addProperty("outTangent", _GetOutTangent, _SetOutTangent);
        binder->addProperty("value", _GetValue, _SetValue);
        binder->addMethod("clone", Clone);
        binder->addMethod("cloneTo", CloneTo);
        binder->end(object, "conchFloatKeyframe");
    }
    
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
