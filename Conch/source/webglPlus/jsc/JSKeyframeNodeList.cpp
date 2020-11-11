/**
@file			JSKeyframeNodeList.cpp
@brief			
@author			James
@version		1.0
@date			2018_7_12
*/

#include "JSKeyframeNodeList.h"
#define NDEBUG
#include <assert.h>
#include "JSCUtil.h"

namespace laya
{
    //ADDJSCLSINFO(JSKeyframeNodeList, JSObjNode);
    //------------------------------------------------------------------------------
    JSKeyframeNodeList::JSKeyframeNodeList()
    {
        if (JSObjNode::s_pListJSObj)
        {
            JSObjNode::s_pListJSObj->push_back(this);
        }
        //AdjustAmountOfExternalAllocatedMemory(8192);
        //JCMemorySurvey::GetInstance()->newClass("JSKeyframeNodeList", 8192, this);
    }
    //------------------------------------------------------------------------------
    JSKeyframeNodeList::~JSKeyframeNodeList()
    {
        if (JSObjNode::s_pListJSObj)
        {
            JSObjNode::s_pListJSObj->delNode(this);
        }
        //m_vNodes.clear();
        //JCMemorySurvey::GetInstance()->releaseClass("JSKeyframeNodeList", this);
    }
    static JSValueRef SetCount(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        JSKeyframeNodeList *pObj = (JSKeyframeNodeList*)JSObjectGetPrivate(thisObject);
        if (argumentCount < 1)
        {
            return JSValueMakeUndefined(ctx);
        }
        assert(JSValueIsNumber(ctx, arguments[0]));
        int value = JSValueToNumber(ctx, arguments[0], 0);
        pObj->setCount(value);
        return JSValueMakeUndefined(ctx);
    }
    static JSValueRef GetCount(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        JSKeyframeNodeList *pObj = (JSKeyframeNodeList*)JSObjectGetPrivate(thisObject);
        int value = pObj->getCount();
        return JSValueMakeNumber(ctx, value);
    }
    static JSValueRef GetNodeByIndex(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        if (argumentCount < 1)
        {
            return JSValueMakeUndefined(ctx);
        }
        JSKeyframeNodeList* self = (JSKeyframeNodeList*)JSObjectGetPrivate(thisObject);
        assert(JSValueIsNumber(ctx, arguments[0]));
        int index = JSValueToNumber(ctx, arguments[0], 0);
        JSKeyframeNode* pNode = (JSKeyframeNode*)self->m_vNodes[index];
        return JSCBinder<JSKeyframeNode>::GetInstance()->transferObjPtrToJS(ctx, pNode);
    }
    static JSValueRef SetNodeByIndex(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        if (argumentCount < 2)
        {
            return JSValueMakeUndefined(ctx);
        }
        JSKeyframeNodeList* self = (JSKeyframeNodeList*)JSObjectGetPrivate(thisObject);
        assert(JSValueIsNumber(ctx, arguments[0]));
        int index = JSValueToNumber(ctx, arguments[0], 0);
        JSValueRef keyframeNode = arguments[1];
        if (JSValueIsObject(ctx, keyframeNode))
        {
            JSKeyframeNode* pNode = (JSKeyframeNode*)JSObjectGetPrivate(JSValueToObject(ctx, keyframeNode, NULL));
            if (pNode)
            {
                self->m_vNodes[index] = pNode;
            }
        }
        return JSValueMakeUndefined(ctx);
    }
    void JSKeyframeNodeList::exportJS(JSContextRef ctx, JSObjectRef object)
    {
        JSCBinder<JSKeyframeNodeList>* binder = JSCBinder<JSKeyframeNodeList>::GetInstance();
        binder->begin(ctx);
        binder->addMethod("setNodeByIndex", SetNodeByIndex);
        binder->addMethod("getNodeByIndex", GetNodeByIndex);
        binder->addMethod("getCount", GetCount);
        binder->addMethod("setCount", SetCount);
        binder->end(object, "_conchKeyframeNodeList");
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
