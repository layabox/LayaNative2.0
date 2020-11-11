/**
@file			JCJson.cpp
@brief			
@author			James
@version		1.0
@date			2014_6_17
*/

#include "JCXml.h"
#include <stdlib.h>

namespace laya
{
//------------------------------------------------------------------------------
XmlNode::XmlNode( XmlNode* p_pParent )
{
	m_pParentNode = p_pParent;
	m_sNodeName = NULL;
	m_sValue = NULL;
}
//------------------------------------------------------------------------------
XmlNode::~XmlNode()
{
	m_pParentNode = NULL;
	m_sNodeName = NULL;
	m_sValue = NULL;
}
//------------------------------------------------------------------------------
int XmlNode::getIntForKey( const char* p_sValue )
{
	for( int i = 0,nSize=(int)(m_vChildNodes.size());i < nSize; i++ )
	{
		if ( strcmp( m_vChildNodes[i]->m_sNodeName,"key" ) == 0 && 
			strcmp( m_vChildNodes[i]->m_sValue,p_sValue ) == 0 )
		{
			if( m_vChildNodes[i+1] != NULL )
			{
				return atoi( m_vChildNodes[i+1]->m_sValue );
			}
			else
			{
				return 0;
			}
		}
	}
	return 0;
}
//------------------------------------------------------------------------------
char* XmlNode::getStringForKey( const char* p_sValue )
{
	for( int i = 0,nSize=(int)(m_vChildNodes.size());i < nSize; i++ )
	{
		if ( strcmp( m_vChildNodes[i]->m_sNodeName,"key" ) == 0 && 
			strcmp( m_vChildNodes[i]->m_sValue,p_sValue ) == 0 )
		{
			if( m_vChildNodes[i+1] != NULL )
			{
				return m_vChildNodes[i+1]->m_sValue;
			}
			else
			{
				return 0;
			}
		}
	}
	return 0;
}
//------------------------------------------------------------------------------
float XmlNode::getFloatForKey( const char* p_sValue )
{
	for( int i = 0,nSize=(int)(m_vChildNodes.size());i < nSize; i++ )
	{
		if ( strcmp( m_vChildNodes[i]->m_sNodeName,"key" ) == 0 && 
			strcmp( m_vChildNodes[i]->m_sValue,p_sValue ) == 0 )
		{
			if( m_vChildNodes[i+1] != NULL )
			{
				return (float)(atof( m_vChildNodes[i+1]->m_sValue ));
			}
			else
			{
				return 0;
			}
		}
	}
	return 0;
}
//------------------------------------------------------------------------------
bool XmlNode::getBoolForKey( const char* p_sValue )
{
	for( int i = 0,nSize=(int)(m_vChildNodes.size());i < nSize; i++ )
	{
		if ( strcmp( m_vChildNodes[i]->m_sNodeName,"key" ) == 0 && 
			strcmp( m_vChildNodes[i]->m_sValue,p_sValue ) == 0 )
		{
			if( m_vChildNodes[i+1] != NULL )
			{
				if( strstr( m_vChildNodes[i+1]->m_sValue,"true" ) )
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}
	return false;
}
//------------------------------------------------------------------------------
XmlNode* XmlNode::getNodeForKey( const char* p_sValue )
{
	for( int i = 0,nSize=(int)(m_vChildNodes.size());i < nSize; i++ )
	{
		if ( strcmp( m_vChildNodes[i]->m_sNodeName,"key" ) == 0 && 
			strcmp( m_vChildNodes[i]->m_sValue,p_sValue ) == 0 )
		{
			if( m_vChildNodes[i+1] != NULL )
			{
				return m_vChildNodes[i+1];
			}
			else
			{
				return NULL;
			}
		}
	}
	return NULL;
}
//------------------------------------------------------------------------------
void XmlNode::setName( char* p_sName )
{
	m_sNodeName = p_sName;
}
//------------------------------------------------------------------------------
void XmlNode::setValue( char* p_sValue )
{
	m_sValue = p_sValue;
}
//------------------------------------------------------------------------------
void XmlNode::addChild( XmlNode* p_pNode )
{
	m_vChildNodes.push_back( p_pNode );
}
//------------------------------------------------------------------------------
bool XmlNode::clearChild()
{
	for( int i = 0,nSize = (int)(m_vChildNodes.size()); i < nSize; i++ )
	{
		if( m_vChildNodes[i] != NULL )
		{
			m_vChildNodes[i]->clearChild();
			delete m_vChildNodes[i];
			m_vChildNodes[i] = NULL;
		}
	}
	m_vChildNodes.clear();
	return true;
}
//------------------------------------------------------------------------------
JCXml::JCXml()
{
	m_pRoot = NULL;
}
//------------------------------------------------------------------------------
JCXml::~JCXml()
{
	cleanUp();
}
//------------------------------------------------------------------------------
bool JCXml::cleanUp()
{
	if( m_pRoot != NULL )
	{
		m_pRoot->clearChild();
		delete m_pRoot;
		m_pRoot = NULL;
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
XmlNode* JCXml::getRoot()
{
	return m_pRoot;
}
//------------------------------------------------------------------------------
bool JCXml::loadXml( char* p_sBuffer )
{
	if( p_sBuffer == NULL )return false;
	if( m_pRoot == NULL )
	{
		m_pRoot = new XmlNode( NULL );
	}
	int nBufferSize = strlen( p_sBuffer );

	int i = 0;
	while( i < nBufferSize )
	{
		if( p_sBuffer[i] == '<' )
		{
			if( p_sBuffer[i+1] != '?' && p_sBuffer[i+1] != '!' )
			{
				break;
			}
		}
		i++;
	}
	int nOffset = i;
	paserXml( p_sBuffer,nBufferSize,nOffset,m_pRoot );
	return true;
}
//------------------------------------------------------------------------------
int JCXml::paserXml( char* p_sBuffer,int p_nBufferSize,int p_nOffset,XmlNode* p_pNode )
{
	if( p_pNode == NULL ) return p_nOffset;
	if( p_nOffset >= p_nBufferSize ) return p_nOffset;
	int i = p_nOffset;

	int nState = 0;
	char* sName = NULL;			//名字
	char* sMatchingName = NULL; //要匹配的名字
	char* sValue = NULL;		//值

	while( i < p_nBufferSize )
	{
		if( nState == 0 )
		{
			if( p_sBuffer[i] == '<' )
			{
				if( p_sBuffer[i+1] == '/' )
				{
					nState = 3;
					i++;
					sMatchingName = ( p_sBuffer + i + 1 );
				}
				else
				{
					nState = 1;
					sName = ( p_sBuffer + i + 1 );
				}
			}
		}
		else if( nState == 1 )
		{
			if( p_sBuffer[i] == '>' )
			{
				nState = 2;
				p_sBuffer[i] = 0;
				p_pNode->setName( sName );
				sValue = (p_sBuffer + i + 1 );
			}
		}
		else if( nState == 2 )
		{
			if( p_sBuffer[i] == '<' )
			{
				//证明闭合了
				if( p_sBuffer[i+1] == '/' )
				{
					p_sBuffer[i] = 0;
					i++;
					sMatchingName = ( p_sBuffer + i + 1 );
					nState = 3;
				}
				else
				{
					//又找到新的node
					XmlNode* pNewNode = new XmlNode( p_pNode );
					p_pNode->addChild( pNewNode );
					i = paserXml( p_sBuffer,p_nBufferSize,i,pNewNode );
					//恢复数据
					{
						nState = 0;
						sName = NULL;
						sMatchingName = NULL;
						sValue = NULL;
					}
				}
			}
		}
		else if( nState == 3 )
		{
			if( p_sBuffer[i] == '>' )
			{
				p_sBuffer[i] = 0;
				if( strcmp( p_pNode->m_sNodeName,sMatchingName ) == 0 )
				{
					p_pNode->setValue( sValue );
					//恢复数据
					{
						p_pNode = p_pNode->m_pParentNode;
						nState = 2;
						sName = NULL;
						sMatchingName = NULL;
						sValue = NULL;
					}
				}
				else
				{
					if( p_pNode->m_pParentNode != NULL )
					{
						if( strcmp( p_pNode->m_pParentNode->m_sNodeName,sMatchingName ) == 0 )
						{
							return ++i;
						}
					}
				}
			}
		}
		i++;
	}
	return i;
}
//------------------------------------------------------------------------------
}
//------------------------------------------------------------------------------

//-----------------------------END FILE--------------------------------
