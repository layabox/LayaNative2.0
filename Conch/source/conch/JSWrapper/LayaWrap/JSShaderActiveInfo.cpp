/**
@file			JSShaderActiveInfo.cpp
@brief			
@author			James
@version		1.0
@date			2018_3_27
*/

#include "JSShaderActiveInfo.h"

namespace laya
{
	ADDJSCLSINFO(JSShaderActiveInfo, JSObjNode);
    //------------------------------------------------------------------------------
    JSShaderActiveInfo::JSShaderActiveInfo()
    {
        m_nType = m_nSize = 0;
	    AdjustAmountOfExternalAllocatedMemory( 64 );
	    JCMemorySurvey::GetInstance()->newClass( "JSShaderActiveInfo",64,this );
    }
    //------------------------------------------------------------------------------
    JSShaderActiveInfo::~JSShaderActiveInfo()
    {
	    JCMemorySurvey::GetInstance()->releaseClass( "JSShaderActiveInfo",this );
    }
    //------------------------------------------------------------------------------
    int JSShaderActiveInfo::getType()
    {
        return m_nType;
    }
    int JSShaderActiveInfo::getSize()
    {
        return m_nSize;
    }
    const char* JSShaderActiveInfo::getName()
    {
        return m_sName.c_str();
    }
    void JSShaderActiveInfo::exportJS()
    {
        JSP_CLASS("shaderActiveInfo", JSShaderActiveInfo);
        JSP_ADD_PROPERTY_RO(type, JSShaderActiveInfo, getType);
        JSP_ADD_PROPERTY_RO(size, JSShaderActiveInfo, getSize);
        JSP_ADD_PROPERTY_RO(name, JSShaderActiveInfo, getName);
        JSP_INSTALL_CLASS("shaderActiveInfo", JSShaderActiveInfo);
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------