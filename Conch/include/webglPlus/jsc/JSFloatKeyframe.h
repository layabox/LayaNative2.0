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
#include <JavaScriptCore/JavaScriptCore.h>
#include <JavaScriptCore/JavaScriptCore.h>

namespace laya
{
    class JSFloatKeyframe : public JCListNode, public JCFloatKeyframe
    {
    public:
	    static void exportJS(JSContextRef ctx, JSObjectRef object);

        JSFloatKeyframe();

	    ~JSFloatKeyframe();

    };
}
//------------------------------------------------------------------------------


#endif //__JSFloatKeyframe_H__

//-----------------------------END FILE--------------------------------
