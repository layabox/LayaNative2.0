/**
@file			JCJson.h
@brief			
@author			James
@version		1.0
@date			2014_6_17
*/

#ifndef __JCJson_H__
#define __JCJson_H__

#include <stdio.h>
#include <vector>
#include <string.h>
#pragma warning (disable: 4996)

namespace laya
{
enum PASER_JSON_STATE
{
	PJ_STATE_INIT = 0,			//初始状态
	PJ_STATE_MATCHING_KEY,		//匹配key的状态
	PJ_STATE_MATCHING_VALUE,	//匹配value状态
	PJ_STATE_MATCHING_OBJECT,	//匹配Object
	PJ_STATE_MATCHING_ARRAY,	//匹配Array
};
class JsonNode
{
public:
	char*			m_sKey;
	int				m_nKey;
	unsigned char	m_nType;
    JsonNode()
    {
        m_sKey = NULL;
        m_nKey = 0;
        m_nType = -1;
    }
    virtual ~JsonNode()
    {
        m_sKey = NULL;
        m_nKey = 0;
        m_nType = -1;
    }
	void setKey( char* p_sKey)
	{
		m_sKey = p_sKey;
	}
	virtual bool addChild( JsonNode* p_pChild )
	{
		return false;
	}
	virtual bool clearChild()
	{
		return false;
	}
};
struct JsonValue : JsonNode
{
	JsonValue()
	{
		m_sKey = NULL;
		m_nKey = 0;
		m_nType = 0;
		m_sValue = NULL;
	}
    ~JsonValue()
    {
    }
	void setKeyAndValue( char* p_sKey,char* p_sValue )
	{
		m_sKey = p_sKey;
		m_sValue = p_sValue;
	}
	bool addChild( JsonNode* p_pChild )
	{
		return false;
	}
	char* m_sValue;
};
struct JsonObject : JsonNode
{
	JsonObject()
	{
		m_sKey = NULL;
		m_nKey = 0;
		m_nType = 1;
	}
    ~JsonObject()
    {
    }
	bool addChild( JsonNode* p_pChild )
	{
		m_vVector.push_back( p_pChild );
		return true;
	}
	bool clearChild()
	{
		for( int i = 0,nSize = (int)(m_vVector.size()); i < nSize; i++ )
		{
			if( m_vVector[i] != NULL )
			{
				m_vVector[i]->clearChild();
				delete m_vVector[i];
				m_vVector[i] = NULL;
			}
		}
		m_vVector.clear();
		return true;
	}

	JsonNode* getNode(const char* p_pszName) {
		for (int i = 0, sz = (int)m_vVector.size(); i < sz; i++) {
			JsonNode* pN = m_vVector[i];
			if (strcmp(pN->m_sKey, p_pszName) == 0) {
				return pN;
			}
		}
		return NULL;
	}
	std::vector<JsonNode*>	m_vVector;
};

struct JsonArray : JsonNode
{
	JsonArray()
	{
		m_sKey = NULL;
		m_nKey = 0;
		m_nType = 2;
	}
    ~JsonArray()
    {
    }
	bool addChild( JsonNode* p_pChild )
	{
		m_vVector.push_back( p_pChild );
		return true;
	}
	bool clearChild()
	{
		for( int i = 0,nSize = (int)(m_vVector.size()); i < nSize; i++ )
		{
			if( m_vVector[i] != NULL )
			{
				m_vVector[i]->clearChild();
				delete m_vVector[i];
				m_vVector[i] = NULL;
			}
		}
		m_vVector.clear();
		return true;
	}
	std::vector<JsonNode*>	m_vVector;
};

class JCJson
{
public:

	JCJson();

	~JCJson();

	bool cleanUp();

	//注意啦，为了效率，这个buffer没有自己拷贝，外部也先不要删除
	bool paserJson( char* p_sBuffer );

	bool paserJson( char* p_sBuffer, int len);

	JsonNode* getRoot();

private:

	int paserJson( char* p_sBuffer,int p_nBufferSize,int p_nOffset,JsonNode* p_pNode );

private:

	JsonNode*		m_pRoot;

};

}

#endif //__JCJson_H__

//-----------------------------END FILE--------------------------------