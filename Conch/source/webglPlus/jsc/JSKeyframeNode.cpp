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
#include "JSCUtil.h"

namespace laya
{
    std::string JCKeyframeNode::s_sTempString = "";//TODO
    //ADDJSCLSINFO(JSKeyframeNode, JSObjNode);
    //------------------------------------------------------------------------------
    JSKeyframeNode::JSKeyframeNode()
    {
        if (JSObjNode::s_pListJSObj)
        {
            JSObjNode::s_pListJSObj->push_back(this);
        }
        /*m_nIndexInList = 0;
        m_nType = 0;
        m_nDataType = 0;
        m_pDataFloat = 0;
        AdjustAmountOfExternalAllocatedMemory(4096);
        JCMemorySurvey::GetInstance()->newClass("JSKeyframeNode", 4096, this);*/
    }
    //------------------------------------------------------------------------------
    JSKeyframeNode::~JSKeyframeNode()
    {
        if (JSObjNode::s_pListJSObj)
        {
            JSObjNode::s_pListJSObj->delNode(this);
        }
        /* m_vOwnerPath.clear();
        m_vPropertys.clear();
        m_vKeyFrames.clear();
        JCMemorySurvey::GetInstance()->releaseClass("JSKeyframeNode", this); */
    }
    static JSValueRef SetFloat32ArrayData(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(thisObject);
        if (argumentCount < 1)
        {
            return JSValueMakeUndefined(ctx);
        }
        char* pArrayBuffer = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = JSCUtil::extractJSAB(ctx, arguments[0], pArrayBuffer, nABLen);
        if (bIsArrayBuffer)
        {
            self->m_pDataFloatArray.data = pArrayBuffer;
            self->m_pDataFloatArray.byteSize = nABLen;
        }
        return JSValueMakeUndefined(ctx);
    }
    static JSValueRef SetOwnerPathCount(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        if (argumentCount < 1)
        {
            return JSValueMakeUndefined(ctx);
        }
        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(thisObject);
        assert(JSValueIsNumber(ctx, arguments[0]));
        int index = JSValueToNumber(ctx, arguments[0], 0);
        self->_setOwnerPathCount(index);
        return JSValueMakeUndefined(ctx);
    }
    static JSValueRef SetOwnerPathByIndex(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        if (argumentCount < 2)
        {
            return JSValueMakeUndefined(ctx);
        }
        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(thisObject);
        assert(JSValueIsNumber(ctx, arguments[0]));
        int index = JSValueToNumber(ctx, arguments[0], 0);
        
        self->_setOwnerPathByIndex(index, JSCUtil::toCString(ctx, arguments[1]));
        return JSValueMakeUndefined(ctx);
    }
    static JSValueRef JoinOwnerPath(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        if (argumentCount < 1)
        {
            return JSValueMakeUndefined(ctx);
        }
        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(thisObject);
        
        const char* v = self->_joinOwnerPath(JSCUtil::toCString(ctx, arguments[0]));
        
        JSStringRef pStr = JSStringCreateWithUTF8CString(v);
        JSValueRef pRet = JSValueMakeString(ctx, pStr);
        JSStringRelease(pStr);
        
        return pRet;
    }
    static JSValueRef SetPropertyCount(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        if (argumentCount < 1)
        {
            return JSValueMakeUndefined(ctx);
        }
        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(thisObject);
        assert(JSValueIsNumber(ctx, arguments[0]));
        int index = JSValueToNumber(ctx, arguments[0], 0);
        self->_setPropertyCount(index);
        return JSValueMakeUndefined(ctx);
    }
    static JSValueRef SetPropertyByIndex(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        if (argumentCount < 2)
        {
            return JSValueMakeUndefined(ctx);
        }
        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(thisObject);
        assert(JSValueIsNumber(ctx, arguments[0]));
        int index = JSValueToNumber(ctx, arguments[0], 0);
        
        self->_setPropertyByIndex(index, JSCUtil::toCString(ctx, arguments[1]));
        return JSValueMakeUndefined(ctx);
    }
    static JSValueRef JoinProperty(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        if (argumentCount < 1)
        {
            return JSValueMakeUndefined(ctx);
        }
        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(thisObject);
        const char* v = self->_joinProperty(JSCUtil::toCString(ctx, arguments[0]));
        
        JSStringRef pStr = JSStringCreateWithUTF8CString(v);
        JSValueRef pRet = JSValueMakeString(ctx, pStr);
        JSStringRelease(pStr);
        
        return pRet;
    }
    static JSValueRef SetKeyframeByIndex0(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        if (argumentCount < 2)
        {
            return JSValueMakeUndefined(ctx);
        }
        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(thisObject);
        assert(JSValueIsNumber(ctx, arguments[0]));
        int index = JSValueToNumber(ctx, arguments[0], 0);
        JSValueRef keyframe = arguments[1];
        if (JSValueIsObject(ctx, keyframe))
        {
            JSFloatKeyframe* pNode = (JSFloatKeyframe*)JSObjectGetPrivate(JSValueToObject(ctx, keyframe, NULL));
            if (pNode)
            {
                self->m_vKeyFrames[index] = pNode;
            }
        }
    
        return JSValueMakeUndefined(ctx);
    }
    static JSValueRef SetKeyframeByIndex1(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        if (argumentCount < 2)
        {
            return JSValueMakeUndefined(ctx);
        }
        
        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(thisObject);
        assert(JSValueIsNumber(ctx, arguments[0]));
        int index = JSValueToNumber(ctx, arguments[0], 0);
        JSValueRef keyframe = arguments[1];
        if (JSValueIsObject(ctx, keyframe))
        {
            JSFloatArrayKeyframe* pNode = (JSFloatArrayKeyframe*)JSObjectGetPrivate(JSValueToObject(ctx, keyframe, NULL));
            if (pNode)
            {
                self->m_vKeyFrames[index] = pNode;
            }
        }
        return JSValueMakeUndefined(ctx);
    }
    static JSValueRef SetKeyframeCount(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        if (argumentCount < 1)
        {
            return JSValueMakeUndefined(ctx);
        }
        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(thisObject);
        assert(JSValueIsNumber(ctx, arguments[0]));
        int value = JSValueToNumber(ctx, arguments[0], 0);
        self->_setKeyframeCount(value);
        return JSValueMakeUndefined(ctx);
    }
    static JSValueRef GetOwnerPathByIndex(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        if (argumentCount < 1)
        {
            return JSValueMakeUndefined(ctx);
        }
        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(thisObject);
        assert(JSValueIsNumber(ctx, arguments[0]));
        int index = JSValueToNumber(ctx, arguments[0], 0);
        const char* v = self->getOwnerPathByIndex(index);

        JSStringRef pStr = JSStringCreateWithUTF8CString(v);
        JSValueRef pRet = JSValueMakeString(ctx, pStr);
        JSStringRelease(pStr);
        
        return pRet;
    }
    static JSValueRef GetPropertyByIndex(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        if (argumentCount < 1)
        {
            return JSValueMakeUndefined(ctx);
        }

        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(thisObject);
        assert(JSValueIsNumber(ctx, arguments[0]));
        int index = JSValueToNumber(ctx, arguments[0], 0);
        const char* v = self->getPropertyByIndex(index);
        
        JSStringRef pStr = JSStringCreateWithUTF8CString(v);
        JSValueRef pRet = JSValueMakeString(ctx, pStr);
        JSStringRelease(pStr);
        
        return pRet;
    }
    static JSValueRef GetKeyframeByIndex(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        return JSValueMakeNull(ctx);
    }
    static JSValueRef GetFloatData(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        JSKeyframeNode *pObj = (JSKeyframeNode*)JSObjectGetPrivate(thisObject);
        float v = pObj->getFloatData();
        return JSValueMakeNumber(ctx, v);
    }
    static JSValueRef GetDataType(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        JSKeyframeNode *pObj = (JSKeyframeNode*)JSObjectGetPrivate(thisObject);
        int v = pObj->getDataType();
        return JSValueMakeNumber(ctx, v);
    }

    static JSValueRef GetKeyFramesCount(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(thisObject);
        int v = self->getKeyFramesCount();
        return JSValueMakeNumber(ctx, v);
    }
    static JSValueRef GetOwnerPathCount(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(thisObject);
        int v = self->getOwnerPathCount();
        return JSValueMakeNumber(ctx, v);
    }
    static JSValueRef GetPropertyCount(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
    {
        laya::JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(thisObject);
        int v = self->getPropertyCount();
        return JSValueMakeNumber(ctx, v);
    }
    static JSValueRef _GetIndexInList(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
    {
        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(object);
        int v = self->getIndexInList();
        return JSValueMakeNumber(ctx, v);
    }
    static bool _SetIndexInList(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception)
    {
        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(object);
        assert(JSValueIsNumber(ctx, value));
        int v = JSValueToNumber(ctx, value, 0);
        self->setIndexInList(v);
        return true;
    }
    static JSValueRef _GetType(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
    {
        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(object);
        int v = self->getType();
        return JSValueMakeNumber(ctx, v);
    }
    static bool _SetType(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception)
    {
        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(object);
        assert(JSValueIsNumber(ctx, value));
        int v = JSValueToNumber(ctx, value, 0);
        self->setType(v);
        return true;
    }
    static JSValueRef _GetOwnerPathCount(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
    {
        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(object);
        int v = self->getOwnerPathCount();
        return JSValueMakeNumber(ctx, v);
    }
    static bool _SetOwnerPathCount(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception)
    {
        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(object);
        assert(JSValueIsNumber(ctx, value));
        int v = JSValueToNumber(ctx, value, 0);
        self->_setOwnerPathCount(v);
        return true;
    }
    static JSValueRef _GetKeyFramesCount(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
    {
        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(object);
        int v = self->getKeyFramesCount();
        return JSValueMakeNumber(ctx, v);
    }
    static bool _SetKeyframeCount(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception)
    {
        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(object);
        assert(JSValueIsNumber(ctx, value));
        int v = JSValueToNumber(ctx, value, 0);
        self->_setKeyframeCount(v);
        return true;
    }
    static JSValueRef _GetPropertyCount(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
    {
        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(object);
        int v = self->getPropertyCount();
        return JSValueMakeNumber(ctx, v);
    }
    static bool _SetPropertyCount(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception)
    {
        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(object);
        assert(JSValueIsNumber(ctx, value));
        int v = JSValueToNumber(ctx, value, 0);
        self->_setPropertyCount(v);
        return true;
    }
    static JSValueRef _GetPropertyOwner(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
    {
        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(object);
        const char* v = self->getPropertyOwner();
        
        JSStringRef pStr = JSStringCreateWithUTF8CString(v);
        JSValueRef pRet = JSValueMakeString(ctx, pStr);
        JSStringRelease(pStr);
        
        return pRet;
    }
    static bool _SetPropertyOwner(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception)
    {
        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(object);
        self->setPropertyOwner(JSCUtil::toCString(ctx, value));
        return true;
    }
    static JSValueRef _GetFullPath(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
    {
        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(object);
        const char* v = self->getFullPath();
        
        JSStringRef pStr = JSStringCreateWithUTF8CString(v);
        JSValueRef pRet = JSValueMakeString(ctx, pStr);
        JSStringRelease(pStr);
        
        return pRet;
    }
    static bool _SetFullPath(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception)
    {
        JSKeyframeNode* self = (JSKeyframeNode*)JSObjectGetPrivate(object);
    
        self->setFullPath(JSCUtil::toCString(ctx, value));
        return true;
    }
    void JSKeyframeNode::exportJS(JSContextRef ctx, JSObjectRef object)
    {
        JSCBinder<JSKeyframeNode>* binder = JSCBinder<JSKeyframeNode>::GetInstance();
        binder->begin(ctx);
        binder->addProperty("_indexInList", _GetIndexInList, _SetIndexInList);
        binder->addProperty("type", _GetType, _SetType);
        binder->addProperty("fullPath", _GetFullPath, _SetFullPath);
        binder->addProperty("propertyOwner", _GetPropertyOwner, _SetPropertyOwner);
        binder->addProperty("ownerPathCount", _GetOwnerPathCount, _SetOwnerPathCount);
        binder->addProperty("keyFramesCount", _GetKeyFramesCount, _SetKeyframeCount);
        binder->addProperty("propertyCount", _GetPropertyCount, _SetPropertyCount);
        binder->addMethod("getOwnerPathCount", GetOwnerPathCount);
        binder->addMethod("getPropertyCount", GetPropertyCount);
        binder->addMethod("getKeyFramesCount", GetKeyFramesCount);
        binder->addMethod("_setOwnerPathCount", SetOwnerPathCount);
        binder->addMethod("_setOwnerPathByIndex", SetOwnerPathByIndex);
        binder->addMethod("_joinOwnerPath", JoinOwnerPath);
        binder->addMethod("_setPropertyCount", SetPropertyCount);
        binder->addMethod("_setPropertyByIndex", SetPropertyByIndex);
        binder->addMethod("_joinProperty", JoinProperty);
        binder->addMethod("_setKeyframeCount", SetKeyframeCount);
        binder->addMethod("_setKeyframeByIndex0", SetKeyframeByIndex0);
        binder->addMethod("_setKeyframeByIndex1", SetKeyframeByIndex1);
        binder->addMethod("getOwnerPathByIndex", GetOwnerPathByIndex);
        binder->addMethod("getPropertyByIndex", GetPropertyByIndex);
        binder->addMethod("getKeyframeByIndex", GetKeyframeByIndex);
        binder->addMethod("getDataType", GetDataType);
        binder->addMethod("getFloatData", GetFloatData);
        binder->addMethod("setFloat32ArrayData", SetFloat32ArrayData);
        binder->end(object, "_conchKeyframeNode");
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
