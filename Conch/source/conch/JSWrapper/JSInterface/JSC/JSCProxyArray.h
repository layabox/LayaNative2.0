//
//  JSCPropertyHandle.h
//  conch
//
//  Created by joychina on 15/6/27.
//  Copyright (c) 2015年 LayaBox. All rights reserved.
//

#ifndef conch_JSCPropertyHandle_h
#define conch_JSCPropertyHandle_h

#include <JavaScriptCore/JavaScriptCore.h>
#include <JavaScriptCore/JavaScript.h>
#include <JavaScriptCore/JSObjectRef.h>
#include "JSCProxyTLS.h"
namespace laya
{
    class JSCProxyArray
    {
    public:
        JSValueRef pJSArray;
        JSContextRef pContext;
        JSValueRef pTmp;
        
    public:
        void initArrayByScript(const char* szScript)
        {
            __JSRun::Run(szScript,&pJSArray);
            pContext = __TlsData::GetInstance()->GetCurContext();
        }
        
        int getLength()
        {
            JSStringRef pname = JSStringCreateWithUTF8CString("length");
            JSValueRef exception = 0;
            JSValueRef val = JSObjectGetProperty(pContext, (JSObjectRef)pJSArray, pname, &exception);
            
            if( 0 != exception )
            {
                __JSRun::OutputException( exception );
            }
            
            double _len =JSValueToNumber(pContext, val, &exception);
            if( 0 != exception )
            {
                __JSRun::OutputException( exception );
            }
            JSStringRelease(pname);
            return (int)_len;
        }
        
        JSValueRef &operator [](int nIndex)
        {
            JSValueRef exception = 0;
            pTmp = JSObjectGetPropertyAtIndex(pContext, (JSObjectRef)pJSArray, nIndex, &exception);
            if( 0 != exception )
            {
                __JSRun::OutputException( exception );
            }
            return pTmp;
        }
        
        void setPropertyValue(unsigned int index,JSValueRef value){
            JSValueRef exception = 0;
            JSObjectSetPropertyAtIndex(pContext, (JSObjectRef)pJSArray, index, value, &exception);
            if( 0 != exception )
            {
                __JSRun::OutputException( exception );
            }
        }
    };
}
#endif
