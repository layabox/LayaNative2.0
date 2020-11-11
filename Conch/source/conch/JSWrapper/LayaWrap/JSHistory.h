/**
@file			JSHistory.h
@brief			
@author			James
@version		1.0
@date			2017_11_28
*/

#ifndef __JSHistory_H__
#define __JSHistory_H__

#include <JSObjBase.h>
#include "../JSInterface/JSInterface.h"

namespace laya 
{

    class JSHistory :public JsObjBase, public JSObjNode 
    {
    public:
        static JsObjClassInfo JSCLSINFO;
        void exportJS();

    public:
        int getLength();
        void back();
        void forward();
        void go(int step);
        void push(char* strUrl);
    };

}

//------------------------------------------------------------------------------


#endif //__JSHistory_H__

//-----------------------------END FILE--------------------------------