/**
@file			JSConsole.h
@brief			
@author			James
@version		1.0
@date			2016_5_18
*/

#ifndef __JSConsole_H__
#define __JSConsole_H__

#include <stdio.h>
#include "../JSInterface/JSInterface.h"

/** 
 * @brief 
*/
namespace laya 
{
    class JSConsole :public JsObjBase, public JSObjNode
    {
    public:
        static JsObjClassInfo JSCLSINFO;
        static void exportJS();

        JSConsole();

        ~JSConsole();

        static JSConsole* getInstance();

    public:

        void log(int p_nType, const char* p_sBuffer);

    public:

        static JSConsole*		m_spConsole;

    };
}
//------------------------------------------------------------------------------


#endif //__JSConsole_H__

//-----------------------------END FILE--------------------------------