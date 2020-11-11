/**
@file			JSFloatKeyframe.h
@brief			
@author			James
@version		1.0
@date			2018_7_12
*/

#ifndef __JSFloatKeyframe_H__
#define __JSFloatKeyframe_H__

#include <stdio.h>
#include <string>
#include <map>
#include "../Animation/JCKeyframeNode.h"
#include "../JSObjBase.h"
#include <v8.h>

namespace laya
{


    class JSFloatKeyframe : public JCListNode, public JCFloatKeyframe
    {
    public:
	    static void exportJS(v8::Local<v8::Object> object);

        JSFloatKeyframe();

	    ~JSFloatKeyframe();

        static void WeakCallback(const v8::WeakCallbackInfo<JSFloatKeyframe>& data);

    public:

        v8::Persistent<v8::Object> persistentObject;

        static v8::Persistent<v8::ObjectTemplate> persistentObjectTemplate;

    };
}
//------------------------------------------------------------------------------


#endif //__JSFloatKeyframe_H__

//-----------------------------END FILE--------------------------------