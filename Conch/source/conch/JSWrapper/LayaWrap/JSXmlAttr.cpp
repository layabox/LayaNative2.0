/**
@file			JSXmlAttr.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_18
*/

#include "JSXmlAttr.h"
namespace laya
{
    ADDJSCLSINFO(JSXmlAttr, JSObjNode);
    JSXmlAttr::JSXmlAttr()
    {
        AdjustAmountOfExternalAllocatedMemory(64000);
        JCMemorySurvey::GetInstance()->newClass("XmlAttr", 64000, this);
    }
    JSXmlAttr::~JSXmlAttr()
    {
        JCMemorySurvey::GetInstance()->releaseClass("XmlAttr", this);
    }
    const char * JSXmlAttr::getNodeName()
    {
        return m_nodeName.c_str();
    }
    const char * JSXmlAttr::getNodeValue()
    {
        return m_nodeValue.c_str();
    }
    const char *JSXmlAttr::getTextContent()
    {
        return m_nodeValue.c_str();
    }
    void JSXmlAttr::exportJS()
    {
        JSP_CLASS("_XmlAttr", JSXmlAttr);
        JSP_ADD_PROPERTY_RO(nodeValue, JSXmlAttr, getNodeValue);
        JSP_ADD_PROPERTY_RO(nodeName, JSXmlAttr, getNodeName);
        JSP_ADD_PROPERTY_RO(textContent, JSXmlAttr, getTextContent);
        JSP_INSTALL_CLASS("_XmlAttr", JSXmlAttr);
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------