/**
@file			JSShaderActiveInfo.h
@brief			
@author			James
@version		1.0
@date			2018_3_27
*/

#ifndef __JSShaderActiveInfo_H__
#define __JSShaderActiveInfo_H__

//包含头文件
#include <stdio.h>
#include <string>
#include <map>
#include "../JSInterface/JSInterface.h"
#include "../JSInterface/JSObjBase.h"

namespace laya
{
    class JSShaderActiveInfo:public JsObjBase, public JSObjNode
    {
    public:
	    static JsObjClassInfo JSCLSINFO;
	    static void exportJS();

        JSShaderActiveInfo();

	    ~JSShaderActiveInfo();

        int getType();

        int getSize();

        const char* getName();

    public:

        int         m_nType;
        int         m_nSize;
        std::string m_sName;

    };
}
//------------------------------------------------------------------------------


#endif //__JSShaderActiveInfo_H__

//-----------------------------END FILE--------------------------------