/**
@file			JSFloatArrayKeyframe.h
@brief			
@author			James
@version		1.0
@date			2018_7_12
*/

#ifndef __JSFloatArrayKeyframe_H__
#define __JSFloatArrayKeyframe_H__

#include <stdio.h>
#include <string>
#include <map>
#include "../JSObjBase.h"
#include "../Animation/JCFloatArrayKeyframe.h"
#include <v8.h>

namespace laya
{
    class JSFloatArrayKeyframe : public JCFloatArrayKeyframe, public JCListNode
    {
    public:

	    static void exportJS(v8::Local<v8::Object> object);

        JSFloatArrayKeyframe();

	    ~JSFloatArrayKeyframe();

    public:

        v8::Persistent<v8::Object> persistentObject;

        static v8::Persistent<v8::ObjectTemplate> persistentObjectTemplate;

    };
}
//------------------------------------------------------------------------------


#endif //__JSFloatArrayKeyframe_H__

//-----------------------------END FILE--------------------------------