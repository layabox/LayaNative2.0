/**
@file			jsobjbase.cpp
@brief			
@author			James
@version		1.0
@date			2019_8_24
*/

#include "JSObjBase.h"

namespace laya
{
    unsigned int gnJSClsID = 1;
    JsObjClassInfo JSObjNode::JSCLSINFO = { "JSObjNode",NULL,0 };
    JCSimpList* JSObjNode::s_pListJSObj = NULL;
    JSObjNode::JSObjNode()
    {
        if (s_pListJSObj)
        {
            s_pListJSObj->push_back(this);
        }
        m_pClsInfo = &JSObjNode::JSCLSINFO;
    }
    JSObjNode::~JSObjNode()
    {
        if (s_pListJSObj)
        {
            s_pListJSObj->delNode(this);
        }
    }
}

//-----------------------------END FILE--------------------------------
