/**
@file			JSCUtil.cpp
@brief			
@author			James
@version		1.0
@date			2017_11_29
*/

#include "JSCUtil.h"
#include <JavaScriptCore/JavaScriptCore.h>
#include <vector>
namespace laya
{
    JSContextRef JSCUtil::s_ctx = NULL;
    char* JSCUtil::toCString(JSContextRef ctx,JSStringRef value)
    {
        int len = JSStringGetMaximumUTF8CStringSize(value);
        static std::vector<char> utf8str;
        utf8str.resize(len);
        JSStringGetUTF8CString(value, &utf8str[0], len);
        return &utf8str[0];
    }
    char* JSCUtil::toCString(JSContextRef ctx,JSValueRef value)
    {
        assert(JSValueIsString(ctx, value));
        JSStringRef str = JSValueToStringCopy(ctx, value, nullptr);
        char* ret = toCString(ctx, str);
        JSStringRelease(str);
        return ret;
    }
    bool JSCUtil::extractJSAB(JSContextRef ctx,JSValueRef  ab, char*& data, int& len)
    {
        JSObjectRef arrayObj = JSValueToObject(ctx, ab, NULL);
        JSTypedArrayType arrayType = JSValueGetTypedArrayType(ctx, ab, NULL);
        switch (arrayType)
        {
        case kJSTypedArrayTypeNone:
        {
            data = NULL;
            len = 0;
            return false;
        }
        break;
        case kJSTypedArrayTypeArrayBuffer:
        {
            data = (char*)JSObjectGetArrayBufferBytesPtr(ctx, arrayObj, NULL);
            len = (int)JSObjectGetArrayBufferByteLength(ctx, arrayObj, NULL);
            return true;
        }
        break;
        default:
        {
            data = (char*)JSObjectGetTypedArrayBytesPtr(ctx, arrayObj, NULL) + JSObjectGetTypedArrayByteOffset(ctx, arrayObj, NULL);
            len = (int)JSObjectGetTypedArrayByteLength(ctx, arrayObj, NULL);
            return true;
        }
        break;
        }
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
