/**
@file			JSShaderPrecisionFormat.h
@brief			
@author			James
@version		1.0
@date			2018_5_9
*/

#ifndef __JSShaderPrecisionFormat_H__
#define __JSShaderPrecisionFormat_H__

#include <stdio.h>
#include <string>
#include <map>
#include "../JSInterface/JSInterface.h"
#include "../JSInterface/JSObjBase.h"

namespace laya
{
    class JSShaderPrecisionFormat :public JsObjBase, public JSObjNode
    {
    public:
	    static JsObjClassInfo JSCLSINFO;
	    static void exportJS();

        JSShaderPrecisionFormat();

	    ~JSShaderPrecisionFormat();

        int getRangeMin();

        int getRangeMax();

        int getPrecison();

    public:

        int m_nRangeMin;
        int m_nRangeMax;
        int m_nPrecision;

    };
}
//------------------------------------------------------------------------------


#endif //__JSShaderPrecisionFormat_H__

//-----------------------------END FILE--------------------------------