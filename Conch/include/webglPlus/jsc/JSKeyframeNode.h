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
#include "../Animation/JCKeyframeNode.h"
#include <JavaScriptCore/JavaScriptCore.h>

namespace laya
{
    class JSKeyframeNode : public JCListNode, public JCKeyframeNode
    {
    public:

        static void exportJS(JSContextRef ctx, JSObjectRef object);

        JSKeyframeNode();

        ~JSKeyframeNode();
        
    };
}
//------------------------------------------------------------------------------


#endif //__JSKeyframeNode_H__

//-----------------------------END FILE--------------------------------
