/**
@file			JSCallbackFuncObj.h
@brief			
@author			James
@version		1.0
@date			2018_6_21
*/

#ifndef __JSCallbackFuncObj_H__
#define __JSCallbackFuncObj_H__

#include <stdio.h>
#include <string>
#include <vector>
#include "../JSInterface/JSInterface.h"
#include "../JSInterface/JSObjBase.h"

namespace laya
{
    class JSCallbackFuncObj :public JsObjBase, public JSObjNode
    {
    public:
        static JsObjClassInfo JSCLSINFO;
        static void exportJS();

        JSCallbackFuncObj();

        JSCallbackFuncObj(int nID);

        ~JSCallbackFuncObj();

        void addCallbackFunc(int nFuncID, JSValueAsParam pFunction);

        void callJS(int nID);

        template<typename P1>
        void callJS(int nID, P1 p1)
        {
            {
                if (m_vFunc[nID])
                {
                    m_vFunc[nID]->Call(p1);
                }
                else
                {
                    LOGI("JSCallbackFuncObj::callJS error id=%d", nID);
                }
            }
        }
    public:

        void testCall(int nID);

    public:

        int                                     m_nID;
        std::vector<JsObjHandle*>               m_vFunc;
    };
}
//------------------------------------------------------------------------------


#endif //__JSCallbackFuncObj_H__

//-----------------------------END FILE--------------------------------