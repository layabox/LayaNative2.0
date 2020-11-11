/**
@file			JSCallbackFuncObj.cpp
@brief			
@author			James
@version		1.0
@date			2018_6_21
*/

#include "JSCallbackFuncObj.h"
#include "../../JCScriptRuntime.h"

namespace laya
{
	ADDJSCLSINFO(JSCallbackFuncObj, JSObjNode);
    //------------------------------------------------------------------------------
    JSCallbackFuncObj::JSCallbackFuncObj()
    {
        m_nID = 0;
    }
    //------------------------------------------------------------------------------
    JSCallbackFuncObj::JSCallbackFuncObj(int nID)
    {
        m_nID = nID;
        JCScriptRuntime::s_JSRT->m_pCallbackFuncManager->setRes(m_nID, this);
    }
    //------------------------------------------------------------------------------
    JSCallbackFuncObj::~JSCallbackFuncObj()
    {
        for (std::vector<JsObjHandle*>::iterator iter = m_vFunc.begin(); iter != m_vFunc.end(); iter++)
        {
            JsObjHandle* pFunc = *iter;
            if (pFunc)
            {
                delete pFunc;
                pFunc = NULL;
            }
        }
        m_vFunc.clear();
        JCScriptRuntime::s_JSRT->m_pCallbackFuncManager->removeRes(m_nID);
    }
    void JSCallbackFuncObj::addCallbackFunc(int nID, JSValueAsParam pFunction)
    {
        JsObjHandle* pFunc = new JsObjHandle();
        pFunc->set(nID, this, pFunction);

        //push到vector中
        int nSize = (int)m_vFunc.size();
        if (nID == nSize)
        {
            m_vFunc.push_back(pFunc);
        }
        else if (nID < nSize)
        {
            if (m_vFunc[nID] == NULL)
            {
                m_vFunc[nID] = pFunc;
            }
            else
            {
                LOGE("JSCallbackFuncObj::addCallbackFunc error m_vFunc[%d] != NULL", nID);
                delete m_vFunc[nID];
                m_vFunc[nID] = pFunc;
            }
        }
        else
        {
            int nOldSize = m_vFunc.size();
            m_vFunc.resize(nID + 1);
            m_vFunc[nID] = pFunc;
        }
    }
    void JSCallbackFuncObj::callJS(int nID)
    {
        if (m_vFunc[nID])
        {
            m_vFunc[nID]->Call();
        }
        else
        {
            LOGI("JSCallbackFuncObj::callJS error id=%d",nID);
        }
    }
    void JSCallbackFuncObj::testCall(int nID)
    {
        if (!m_vFunc[nID]->Empty())
        {
            m_vFunc[nID]->Call();
        }
    }
    //------------------------------------------------------------------------------
    void JSCallbackFuncObj::exportJS()
    {
        JSP_CLASS("_callbackFuncObj", JSCallbackFuncObj);
        JSP_ADD_METHOD("addCallbackFunc", JSCallbackFuncObj::addCallbackFunc);
        JSP_ADD_METHOD("testCall", JSCallbackFuncObj::testCall);
        JSP_REG_CONSTRUCTOR(JSCallbackFuncObj, int);
        JSP_INSTALL_CLASS("_callbackFuncObj", JSCallbackFuncObj);
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
