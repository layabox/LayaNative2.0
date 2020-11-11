/**
@file			JSFloatArrayKeyframe.cpp
@brief			
@author			James
@version		1.0
@date			2018_7_12
*/

#include "JSFloatArrayKeyframe.h"
#define NDEBUG
#include <assert.h>
#include "JSCUtil.h"

namespace laya
{
    //ADDJSCLSINFO(JSFloatArrayKeyframe, JSObjNode);
    //------------------------------------------------------------------------------
    JSFloatArrayKeyframe::JSFloatArrayKeyframe()
    {
        if (JSObjNode::s_pListJSObj)
        {
            JSObjNode::s_pListJSObj->push_back(this);
        }
        //AdjustAmountOfExternalAllocatedMemory(1024);
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
    static JSValueRef SetInTangent(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        JSFloatArrayKeyframe *pObj = (JSFloatArrayKeyframe*)JSObjectGetPrivate(thisObject);
        if (argumentCount < 1)
        {
            return JSValueMakeUndefined(ctx);
        }
        char* pArrayBuffer = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = JSCUtil::extractJSAB(ctx, arguments[0], pArrayBuffer, nABLen);
        if (bIsArrayBuffer)
        {
            pObj->m_nInTangent.data = pArrayBuffer;
            pObj->m_nInTangent.byteSize = nABLen;
        }
        return JSValueMakeUndefined(ctx);
    }
    static JSValueRef SetOutTangent(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        JSFloatArrayKeyframe *pObj = (JSFloatArrayKeyframe*)JSObjectGetPrivate(thisObject);
        if (argumentCount < 1)
        {
            return JSValueMakeUndefined(ctx);
        }
        char* pArrayBuffer = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = JSCUtil::extractJSAB(ctx, arguments[0], pArrayBuffer, nABLen);
        if (bIsArrayBuffer)
        {
            pObj->m_nOutTangent.data = pArrayBuffer;
            pObj->m_nOutTangent.byteSize = nABLen;
        }
        return JSValueMakeUndefined(ctx);
    }
    static JSValueRef SetValue(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        JSFloatArrayKeyframe *pObj = (JSFloatArrayKeyframe*)JSObjectGetPrivate(thisObject);
        if (argumentCount < 1)
        {
            return JSValueMakeUndefined(ctx);
        }
        char* pArrayBuffer = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = JSCUtil::extractJSAB(ctx, arguments[0], pArrayBuffer, nABLen);
        if (bIsArrayBuffer)
        {
            pObj->m_nValue.data = pArrayBuffer;
            pObj->m_nValue.byteSize = nABLen;
        }
        return JSValueMakeUndefined(ctx);
    }
    static JSValueRef SetData(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        JSFloatArrayKeyframe *pObj = (JSFloatArrayKeyframe*)JSObjectGetPrivate(thisObject);
        if (argumentCount < 1)
        {
            return JSValueMakeUndefined(ctx);
        }
        char* pArrayBuffer = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = JSCUtil::extractJSAB(ctx, arguments[0], pArrayBuffer, nABLen);
        if (bIsArrayBuffer)
        {
            pObj->m_pData.data = pArrayBuffer;
            pObj->m_pData.byteSize = nABLen;
        }
        return JSValueMakeUndefined(ctx);
    }
    static JSValueRef SetTime(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        JSFloatArrayKeyframe* pObj = (JSFloatArrayKeyframe*)JSObjectGetPrivate(thisObject);
        if (argumentCount < 1)
        {
            return JSValueMakeUndefined(ctx);
        }
        assert(JSValueIsNumber(ctx, arguments[0]));
        float value = JSValueToNumber(ctx, arguments[0], 0);
        pObj->setTime(value);
        return JSValueMakeUndefined(ctx);
    }
    static JSValueRef GetTime(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        JSFloatArrayKeyframe *pObj = (JSFloatArrayKeyframe*)JSObjectGetPrivate(thisObject);
        float value = pObj->getTime();
        return JSValueMakeNumber(ctx, value);
    }
    void JSFloatArrayKeyframe::exportJS(JSContextRef ctx, JSObjectRef object)
    {
        JSCBinder<JSFloatArrayKeyframe>* binder = JSCBinder<JSFloatArrayKeyframe>::GetInstance();
        binder->begin(ctx);
        binder->addMethod("setTime", SetTime);
        binder->addMethod("getTime", GetTime);
        binder->addMethod("setInTangent", SetInTangent);
        binder->addMethod("setOutTangent", SetOutTangent);
        binder->addMethod("setValue", SetValue);
        binder->addMethod("setData", SetData);
        binder->end(object, "_conchFloatArrayKeyframe");
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
