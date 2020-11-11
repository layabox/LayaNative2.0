/**
@file			JSHistory.cpp
@brief			
@author			James
@version		1.0
@date			2017_11_28
*/

#include "JSHistory.h"
#include "../../JCConch.h"

namespace laya 
{
    ADDJSCLSINFO(JSHistory, JSObjNode);
    int JSHistory::getLength() 
    {
        if (JCConch::s_pConch) 
        {
            return JCConch::s_pConch->urlHistoryLength();
        }
        return 0;
    }
    void JSHistory::back() 
    {
        if (JCConch::s_pConch) 
        {
            return JCConch::s_pConch->urlBack();
        }
    }
    void JSHistory::forward() 
    {
        if (JCConch::s_pConch) 
        {
            return JCConch::s_pConch->urlBack();
        }
    }
    void JSHistory::go(int step) 
    {
        if (JCConch::s_pConch)
        {
            return JCConch::s_pConch->urlGo(step);
        }
    }
    void JSHistory::push(char* strUrl)
    {
        if (JCConch::s_pConch) 
        {
            return JCConch::s_pConch->urlHistoryPush(strUrl);
        }
    }
    void JSHistory::exportJS()
    {
        JSP_GLOBAL_CLASS("history", JSHistory);
        JSP_ADD_PROPERTY_RO(length, JSHistory, getLength);
        JSP_ADD_METHOD("back", JSHistory::back);
        JSP_ADD_METHOD("forward", JSHistory::forward);
        JSP_ADD_METHOD("go", JSHistory::go);
        JSP_ADD_METHOD("_push", JSHistory::push);
        JSP_INSTALL_GLOBAL_CLASS("history", JSHistory, this);
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------