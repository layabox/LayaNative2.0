/**
@file			JSArrayBufferRef.h
@brief			
@author			James
@version		1.0
@date			2017_11_29
*/

#ifndef __JSArrayBufferRef_H__
#define __JSArrayBufferRef_H__

#include <stdio.h>
#include "../JSObjBase.h"
#include <v8.h>

/** 
 * @brief 
*/
namespace laya 
{
    class JSArrayBufferRef : public JCListNode
    {
    public:

	    static void exportJS(v8::Local<v8::Object> object);

        JSArrayBufferRef();

        ~JSArrayBufferRef();

        void callManagerRemoveArrayBuffer();

        int getID();

        bool getIsSyncToRender();

    public:

        int     m_nID;
        bool    m_bSyncToRender;

    public:
        static void WeakCallback(const v8::WeakCallbackInfo<JSArrayBufferRef>& data);
        v8::Persistent<v8::Object> persistentObject;
        static v8::Persistent<v8::ObjectTemplate> persistentObjectTemplate;
    };
}
//------------------------------------------------------------------------------


#endif //__JSArrayBufferRef_H__

//-----------------------------END FILE--------------------------------