/**
@file			JCJson.cpp
@brief			
@author			wyw
@version		1.0
@date			2014_6_17
*/

#include "JCJson.h"
#include <ctype.h>

namespace laya
{
//------------------------------------------------------------------------------
JCJson::JCJson()
{
	m_pRoot = NULL;
}
//------------------------------------------------------------------------------
JCJson::~JCJson()
{
	cleanUp();
}
//------------------------------------------------------------------------------
bool JCJson::cleanUp()
{
	if( m_pRoot != NULL )
	{
		m_pRoot->clearChild();
		delete m_pRoot;
		m_pRoot = NULL;
	}
	return true;
}
//------------------------------------------------------------------------------
JsonNode* JCJson::getRoot()
{
	return m_pRoot;
}
//------------------------------------------------------------------------------
bool JCJson::paserJson( char* p_sBuffer )
{
	return paserJson(p_sBuffer, strlen(p_sBuffer));
}
//------------------------------------------------------------------------------
bool JCJson::paserJson( char* p_sBuffer, int len )
{
	if( p_sBuffer == NULL )return false;
	if( m_pRoot == NULL )
	{
		m_pRoot = new JsonObject();
	}
	int nBufferSize = len; 

	//找到第一个 “{” 这样做是为了避免 文件开头有其他文字
	int i = 0;
	while( i < nBufferSize )
	{
		if( p_sBuffer[i] != '{' )
		{
			i++;
		}
		else
		{
			break;
		}
	}
	int nOffset = i + 1;
	paserJson( p_sBuffer,nBufferSize,nOffset,m_pRoot );
	return true;
}
//------------------------------------------------------------------------------
int JCJson::paserJson( char* p_sBuffer,int p_nBufferSize,int p_nOffset,JsonNode* p_pNode )
{
	if( p_nOffset >= p_nBufferSize ) return p_nOffset;
	int i = p_nOffset;

	bool bValueString = false;
	char* sKey = NULL;
	char* sValue = NULL;
	PASER_JSON_STATE nState = PJ_STATE_INIT;

	while( i < p_nBufferSize )
	{
		//初始状态
		if( nState == PJ_STATE_INIT )
		{
			//如果找到的是 " 就开始进入匹配KEY的状态
			if( p_sBuffer[i] == '"' )
			{
				nState = PJ_STATE_MATCHING_KEY;
				sKey = (p_sBuffer + i + 1);
			}
			//如果是 { 证明又是一个对象,重新递归读取对象
			else if( p_sBuffer[i] == '{' )
			{
				JsonObject* pObject = new JsonObject();
				p_pNode->addChild( pObject );
				i = paserJson( p_sBuffer,p_nBufferSize,++i,pObject );
				//数据恢复
				{
					nState = PJ_STATE_INIT;
					sKey = NULL;
					sValue = NULL;
				}
			}
			else if( p_sBuffer[i] == '}' )
			{
				return i;
			}
			else if( p_sBuffer[i] == ']' )
			{
				return i;
			}
			//这个有点凑了，，因为在文件中会出现			"mat": [1.3,0,0,1.3,0.294,2.893]  
			//这样的现象，所以要处理
			else if( isgraph( p_sBuffer[i] ) && ( p_sBuffer[i] != ',' ) )
			{
				sKey = (char*)"";
				sValue = p_sBuffer + i;
				nState = PJ_STATE_MATCHING_VALUE;
			}
		}
		//如果是匹配key,知道匹配上 " key的值就有了，并进入匹配value阶段
		else if( nState == PJ_STATE_MATCHING_KEY )
		{
			if( p_sBuffer[i] == '"' )
			{
				p_sBuffer[i] = 0;
			}
			else if( p_sBuffer[i] == ':' )
			{
				bValueString = false;
				sValue = p_sBuffer + i + 1;
				nState = PJ_STATE_MATCHING_VALUE;
			}
			// == ','  和 下面的 } ] 这两个是为了，怕碰到这种情况 "config_file_path": ["daji0.plist","daji1.plist"]
			else if( p_sBuffer[i] == ',' )
			{
				JsonValue* pJsonValue = new JsonValue();
				p_sBuffer[i] = 0;
				pJsonValue->setKeyAndValue( (char*)"",sKey );
				p_pNode->addChild( pJsonValue );
				//数据恢复
				{
					nState = PJ_STATE_INIT;
					sKey = NULL;
					sValue = NULL;
				}
			}
			else if( p_sBuffer[i] == '}' || p_sBuffer[i] == ']' )
			{
				JsonValue* pJsonValue = new JsonValue();
				p_sBuffer[i] = 0;
				pJsonValue->setKeyAndValue( (char*)"",sKey );
				p_pNode->addChild( pJsonValue );
				return i;
			}
		}
		//开始匹配value
		else if( nState == PJ_STATE_MATCHING_VALUE )
		{
			if( bValueString == false )
			{
				if( p_sBuffer[i] == '"')
				{
					sValue = p_sBuffer + i + 1;
					bValueString = true;
				}
				//如果是 [ 是数组，再次递归 
				else if( p_sBuffer[i] == '[' )
				{
					nState = PJ_STATE_MATCHING_ARRAY;
					JsonArray* pArray = new JsonArray();
					pArray->setKey( sKey );
					p_pNode->addChild( pArray );
					i = paserJson( p_sBuffer,p_nBufferSize,++i,pArray );
					//数据恢复
					{
						nState = PJ_STATE_INIT;
						sKey = NULL;
						sValue = NULL;
					}
				}
				//如果是 { 是object，再次递归
				else if( p_sBuffer[i] == '{' )
				{
					nState = PJ_STATE_MATCHING_OBJECT;
					JsonObject* pObject = new JsonObject();
					pObject->setKey( sKey );
					p_pNode->addChild( pObject );
					i = paserJson( p_sBuffer,p_nBufferSize,++i,pObject );
					//数据恢复
					{
						nState = PJ_STATE_INIT;
						sKey = NULL;
						sValue = NULL;
					}
				}
				else if( p_sBuffer[i] == ',' )
				{
					JsonValue* pJsonValue = new JsonValue();
					p_sBuffer[i] = 0;
					pJsonValue->setKeyAndValue( sKey,sValue );
					p_pNode->addChild( pJsonValue );
					//数据恢复
					{
						nState = PJ_STATE_INIT;
						sKey = NULL;
						sValue = NULL;
					}
				}
				else if( p_sBuffer[i] == '}' || p_sBuffer[i] == ']' )
				{
					JsonValue* pJsonValue = new JsonValue();
					p_sBuffer[i] = 0;
					pJsonValue->setKeyAndValue( sKey,sValue );
					p_pNode->addChild( pJsonValue );
					return i;
				}
			}
			else
			{
				if( p_sBuffer[i] == '"' )
				{
					JsonValue* pJsonValue = new JsonValue();
					p_sBuffer[i] = 0;
					pJsonValue->setKeyAndValue( sKey,sValue );
					p_pNode->addChild( pJsonValue );
					bValueString = false;
					//数据恢复
					{
						nState = PJ_STATE_INIT;
						sKey = NULL;
						sValue = NULL;
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
