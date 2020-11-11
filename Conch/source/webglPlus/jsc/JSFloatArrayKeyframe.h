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
#include <JavaScriptCore/JavaScriptCore.h>

namespace laya
{
    class JSFloatArrayKeyframe : public JCFloatArrayKeyframe, public JCListNode
    {
    public:

	    static void exportJS(JSContextRef ctx, JSObjectRef object);

        JSFloatArrayKeyframe();

	    ~JSFloatArrayKeyframe();

    };
}
//------------------------------------------------------------------------------


#endif //__JSFloatArrayKeyframe_H__

//-----------------------------END FILE--------------------------------
