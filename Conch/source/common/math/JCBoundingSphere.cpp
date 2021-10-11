/**
@file			JCBoundingSphere.cpp
@brief			
@author			James
@version		1.0
@date			2016_10_31
*/

#include "JCBoundingSphere.h"

namespace laya
{
    JCBoundingSphere::JCBoundingSphere()
    {
        m_nRadius = 0;
    }
    //------------------------------------------------------------------------------
    JCBoundingSphere::JCBoundingSphere(const Vector3& center, float nRadius)
    {
        m_kCenter = center;
        m_nRadius = nRadius;
    }
    void JCBoundingSphere::setValues(float* value)
    {
        m_kCenter.x = value[0];
        m_kCenter.y = value[1];
        m_kCenter.z = value[2];
        m_nRadius = value[3];
    }
    JCBoundingSphere::~JCBoundingSphere()
    {
        
    }
    //------------------------------------------------------------------------------
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
