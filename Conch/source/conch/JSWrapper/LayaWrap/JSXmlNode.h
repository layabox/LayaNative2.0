/**
@file			JSXmlNode.h
@brief
@author			James
@version		1.0
@date			2015_5_16
*/
#ifndef _JSXmlNode_H  
#define _JSXmlNode_H  
#include <vector>
#include "JSXmlAttr.h"
#include "util/rapidxml/rapidxml_utils.hpp"
#include "util/rapidxml/rapidxml_print.hpp"

namespace laya
{
    class  JSXmlNode : public JsObjBase, public JSObjNode
    {
    public:
        static JsObjClassInfo JSCLSINFO;
        static void exportJS();
        JSXmlNode();
        virtual ~JSXmlNode();
        bool  hasChildNodes();
        JSXmlNode* insertBefore(JSXmlNode* newChild, JSXmlNode* refChild);
        JSXmlNode* replaceChild(JSXmlNode* newChild, JSXmlNode* oldChild);
        JSXmlNode* removeChild(JSXmlNode* oldChild);
        JSXmlNode* appendChild(JSXmlNode* newChild);
        JsValue  getParentNode();
        JsValue  getFirstChild();
        JsValue  getLastChild();
        JsValue  getPreviousSibling();
        JsValue  getNextSibling();
        const char *getNodeName();
        const char *getNodeValue();
        const char *getTextContent();
        virtual JsValue  getChildNodes();
        virtual JsValue  getAttributes();
        void initXmlNode(rapidxml::xml_node<>* node,bool isRoot);
    public:
        std::vector<JSXmlNode*> m_childNodes;
        std::vector<JSXmlAttr*> m_attributes;
        enum { allchildsid, allattribid };
        JsObjHandle m_jsChildNodes; bool m_bCreateChilds;
        JsObjHandle m_jsAttribs; bool m_bCreateAttribs;
        JSXmlNode* m_parentNode;
        std::string  m_nodeName;
        std::string  m_nodeValue;
    };
    class JSXmlDocument :public JSXmlNode
    {
    public:
        static JsObjClassInfo JSCLSINFO;
        static void exportJS();
        JSXmlDocument();
        ~JSXmlDocument();
        void parse(const char* str);
    public:
        class rapidxml::xml_document<>*m_document;
    };
}
#endif 