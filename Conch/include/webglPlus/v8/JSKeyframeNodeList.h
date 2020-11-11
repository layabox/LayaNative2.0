/**
@file			JSKeyframeNodeList.h
@brief			
@author			James
@version		1.0
@date			2018_7_12
*/

#ifndef __JSKeyframeNodeList_H__
#define __JSKeyframeNodeList_H__

#include <stdio.h>
#include <string>
#include <map>
#include "../JSObjBase.h"
#include "JSKeyframeNode.h"
#include <vector>
#include <v8.h>
#include "../Animation/JCKeyframeNodeList.h"

namespace laya
{
    class JSKeyframeNodeList : public JCListNode, public JCKeyframeNodeList
    {
    public:

	    static void exportJS(v8::Local<v8::Object> object);

        JSKeyframeNodeList();

	    ~JSKeyframeNodeList();

    public:

        v8::Persistent<v8::Object> persistentObject;

        static v8::Persistent<v8::ObjectTemplate> persistentObjectTemplate;

    };
}
//------------------------------------------------------------------------------


#endif //__JSKeyframeNodeList_H__

//-----------------------------END FILE--------------------------------