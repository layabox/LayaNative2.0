/**
@file			JSKeyframeNode.h
@brief			
@author			James
@version		1.0
@date			2018_7_12
*/

#ifndef __JSKeyframeNode_H__
#define __JSKeyframeNode_H__

#include <stdio.h>
#include <string>
#include <map>
#include "../JSObjBase.h"
#include <vector>
#include "JSFloatKeyframe.h"
#include "JSFloatArrayKeyframe.h"
#include <v8.h>
#include "../Animation/JCKeyframeNode.h"

namespace laya
{
    class JSKeyframeNode : public JCListNode, public JCKeyframeNode
    {
    public:

        static void exportJS(v8::Local<v8::Object> object);

        JSKeyframeNode();

        ~JSKeyframeNode();

    public:

        static void WeakCallback(const v8::WeakCallbackInfo<JSKeyframeNode>& data);

        v8::Persistent<v8::Object> persistentObject;

        static v8::Persistent<v8::ObjectTemplate> persistentObjectTemplate;

    };
}
//------------------------------------------------------------------------------


#endif //__JSKeyframeNode_H__

//-----------------------------END FILE--------------------------------