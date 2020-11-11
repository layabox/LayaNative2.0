/**
@file			JSXmlAttr.h
@brief
@author			James
@version		1.0
@date			2015_5_16
*/

#ifndef _JSXmlAttr_H  
#define _JSXmlAttr_H  

#include "../JSInterface/JSInterface.h"
#include "util/rapidxml/rapidxml.hpp"

namespace laya
{
    class  JSXmlAttr : public JsObjBase, public JSObjNode
    {
    public:
        static JsObjClassInfo JSCLSINFO;
        static void exportJS();

        JSXmlAttr();
        ~JSXmlAttr();
        const char *getNodeName();
        const char *getNodeValue();
        const char *getTextContent();

    public:
        std::string  m_nodeName;
        std::string  m_nodeValue;
    };
}
#endif 