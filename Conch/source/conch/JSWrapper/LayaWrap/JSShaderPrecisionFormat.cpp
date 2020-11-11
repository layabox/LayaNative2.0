/**
@file			JSShaderPrecisionFormat.cpp
@brief			
@author			James
@version		1.0
@date			2018_5_9
*/

#include "JSShaderPrecisionFormat.h"

namespace laya
{
	ADDJSCLSINFO(JSShaderPrecisionFormat, JSObjNode);
    //------------------------------------------------------------------------------
    JSShaderPrecisionFormat::JSShaderPrecisionFormat()
    {
        m_nRangeMin = m_nRangeMax = m_nPrecision = 0;
	    AdjustAmountOfExternalAllocatedMemory( 64 );
	    JCMemorySurvey::GetInstance()->newClass( "JSShaderPrecisionFormat",64,this );
    }
    //------------------------------------------------------------------------------
    JSShaderPrecisionFormat::~JSShaderPrecisionFormat()
    {
	    JCMemorySurvey::GetInstance()->releaseClass( "JSShaderPrecisionFormat",this );
    }
    //------------------------------------------------------------------------------
    int JSShaderPrecisionFormat::getRangeMin()
    {
        return m_nRangeMin;
    }
    int JSShaderPrecisionFormat::getRangeMax()
    {
        return m_nRangeMax;
    }
    int JSShaderPrecisionFormat::getPrecison()
    {
        return m_nPrecision;
    }
    void JSShaderPrecisionFormat::exportJS()
    {
        JSP_CLASS("shaderPrecisionFormat", JSShaderPrecisionFormat);
        JSP_ADD_PROPERTY_RO(rangeMin, JSShaderPrecisionFormat, getRangeMin);
        JSP_ADD_PROPERTY_RO(rangeMax, JSShaderPrecisionFormat, getRangeMax);
        JSP_ADD_PROPERTY_RO(precision, JSShaderPrecisionFormat, getPrecison);
        JSP_INSTALL_CLASS("shaderPrecisionFormat", JSShaderPrecisionFormat);
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
