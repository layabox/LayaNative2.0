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
#include "../Animation/JCKeyframeNodeList.h"
#include <JavaScriptCore/JavaScriptCore.h>

namespace laya
{
    class JSKeyframeNodeList : public JCListNode, public JCKeyframeNodeList
    {
    public:

	    static void exportJS(JSContextRef ctx, JSObjectRef object);

        JSKeyframeNodeList();

	    ~JSKeyframeNodeList();

    };
}
//------------------------------------------------------------------------------


#endif //__JSKeyframeNodeList_H__

//-----------------------------END FILE--------------------------------
