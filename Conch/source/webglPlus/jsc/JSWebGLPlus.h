/**
@file			JSWebGLPlus.h
@brief			
@author			James
@version		1.0
@date			2019_8_24
*/

#ifndef __JSWebGLPlus_H__
#define __JSWebGLPlus_H__
#include "../JSObjBase.h"
#include <JavaScriptCore/JavaScriptCore.h>
/** 
 * @brief 
*/
namespace laya 
{
    class JSWebGLPlus : public JCListNode
    {
    public:
        
        void exportJS(JSContextRef ctx, JSObjectRef object);

        static JSWebGLPlus* getInstance();
        
        JSWebGLPlus();

        ~JSWebGLPlus();

    public:

        bool updateArrayBufferRef(int nID, bool bSyncToRender, JSValueRef pArrayBuffer);

    public:
        
        static JSWebGLPlus*                 s_pWebGLPlus;
        
    };
}
//------------------------------------------------------------------------------


#endif //__JSWebGLPlus_H__

//-----------------------------END FILE--------------------------------
