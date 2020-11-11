/**
@file			JCXml.h
@brief			
@author			James
@version		1.0
@date			2014_6_17
*/

#ifndef __JCXml_H__
#define __JCXml_H__

#include <stdio.h>
#include <vector>
#include <string.h>
#pragma warning (disable: 4996)

namespace laya
{

class XmlNode
{
public:

	typedef std::vector<XmlNode*>		VectorXmlNode;
	typedef VectorXmlNode::iterator		VectorXmlNodeIter;

public:

	XmlNode(  XmlNode* p_pParent );

	~XmlNode();

	int getIntForKey( const char* p_sValue );

	char* getStringForKey( const char* p_sValue );

	float getFloatForKey( const char* p_sValue );

	bool getBoolForKey( const char* p_sValue );

	XmlNode* getNodeForKey( const char* p_sValue );

public:

	void setName( char* p_sName );

	void setValue( char* p_sValue );

	void addChild( XmlNode* p_pNode );

	bool clearChild();
	
public:

	char*			m_sNodeName;
	char*			m_sValue;
	XmlNode*		m_pParentNode;
	VectorXmlNode	m_vChildNodes;
};

class JCXml
{
public:

	JCXml();

	~JCXml();

	bool cleanUp();

	bool loadXml( char* p_sBuffer );

	int paserXml( char* p_sBuffer,int p_nBufferSize,int p_nOffset,XmlNode* p_pNode );

	XmlNode* getRoot();

private:

	XmlNode*		m_pRoot;

};
}

#endif //__JCXml_H__

//-----------------------------END FILE--------------------------------