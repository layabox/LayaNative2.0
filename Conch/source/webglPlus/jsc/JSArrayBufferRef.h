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
#include <JavaScriptCore/JavaScriptCore.h>
/** 
 * @brief 
*/
namespace laya 
{
    class JSArrayBufferRef : public JCListNode
    {
    public:

	    static void exportJS(JSContextRef ctx, JSObjectRef object);

        JSArrayBufferRef();

        ~JSArrayBufferRef();

        void callManagerRemoveArrayBuffer();

        int getID();

        bool getIsSyncToRender();

    public:

        int     m_nID;
        bool    m_bSyncToRender;
        
    };
}
//------------------------------------------------------------------------------


#endif //__JSArrayBufferRef_H__

//-----------------------------END FILE--------------------------------
