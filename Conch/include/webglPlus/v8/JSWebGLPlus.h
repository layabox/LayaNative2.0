/**
@file			JSWebGLPlus.h
@brief			
@author			James
@version		1.0
@date			2019_8_24
*/

#ifndef __JSWebGLPlus_H__
#define __JSWebGLPlus_H__
#include <v8.h>
#include "../JSObjBase.h"
#include "../JCWebGLPlus.h"
/** 
 * @brief 
*/
namespace laya 
{
    class JSWebGLPlus : public JCListNode
    {
    public:

        void exportJS(v8::Local<v8::Object> object);

        static JSWebGLPlus* getInstance();

        JSWebGLPlus();

        ~JSWebGLPlus();

        bool updateArrayBufferRef(int nID, bool bSyncToRender, v8::Local<v8::Value> pArrayBuffer);

    public:
        static JSWebGLPlus*                 s_pWebGLPlus;
        v8::Persistent<v8::Object>          persistentObject;
    };
}
//------------------------------------------------------------------------------


#endif //__JSWebGLPlus_H__

//-----------------------------END FILE--------------------------------