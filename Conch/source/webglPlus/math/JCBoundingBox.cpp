/**
@file			JCBoundingBox.cpp
@brief			
@author			James
@version		1.0
@date			2016_10_31
*/

#include "JCBoundingBox.h"

namespace laya
{
    //------------------------------------------------------------------------------
    JCBoundingBox::JCBoundingBox(const Vector3& min, const Vector3& max)
    {
        m_kMin = min;
        m_kMax = max;
    }
    void JCBoundingBox::getCorners(Vector3* kOutCorners)
    {
        float minX = m_kMin.x;
        float minY = m_kMin.y;
        float minZ = m_kMin.z;
        float maxX = m_kMax.x;
        float maxY = m_kMax.y;
        float maxZ = m_kMax.z;
        kOutCorners[0] = Vector3(minX, maxY, maxZ);
        kOutCorners[1] = Vector3(maxX, maxY, maxZ);
        kOutCorners[2] = Vector3(maxX, minY, maxZ);
        kOutCorners[3] = Vector3(minX, minY, maxZ);
        kOutCorners[4] = Vector3(minX, maxY, minZ);
        kOutCorners[5] = Vector3(maxX, maxY, minZ);
        kOutCorners[6] = Vector3(maxX, minY, minZ);
        kOutCorners[7] = Vector3(minX, minY, minZ);
    }
    void JCBoundingBox::getDebugLinePoint(Vector3* kOutCorners)
    {
        float minX = m_kMin.x;
        float minY = m_kMin.y;
        float minZ = m_kMin.z;
        float maxX = m_kMax.x;
        float maxY = m_kMax.y;
        float maxZ = m_kMax.z;
        Vector3 v0 =Vector3(minX, maxY, maxZ);
        Vector3 v1 =Vector3(maxX, maxY, maxZ);
        Vector3 v2 =Vector3(maxX, minY, maxZ);
        Vector3 v3 =Vector3(minX, minY, maxZ);
        Vector3 v4 =Vector3(minX, maxY, minZ);
        Vector3 v5 =Vector3(maxX, maxY, minZ);
        Vector3 v6 =Vector3(maxX, minY, minZ);
        Vector3 v7 =Vector3(minX, minY, minZ);
        kOutCorners[0] = v0;
        kOutCorners[1] = v1;
        kOutCorners[2] = v1;
        kOutCorners[3] = v2;
        kOutCorners[4] = v2;
        kOutCorners[5] = v3;
        kOutCorners[6] = v3;
        kOutCorners[7] = v0;
        kOutCorners[8] = v4;
        kOutCorners[9] = v5;
        kOutCorners[10] = v5;
        kOutCorners[11] = v6;
        kOutCorners[12] = v6;
        kOutCorners[13] = v7;
        kOutCorners[14] = v7;
        kOutCorners[15] = v4;
        kOutCorners[16] = v4;
        kOutCorners[17] = v0;
        kOutCorners[18] = v7;
        kOutCorners[19] = v3;
        kOutCorners[20] = v5;
        kOutCorners[21] = v1;
        kOutCorners[22] = v6;
        kOutCorners[23] = v2;
    }
    JCBoundingBox::~JCBoundingBox()
    {

    }
    void JCBoundingBox::setValues(float* value)
    {
        m_kMin.x = value[0];
        m_kMin.y = value[1];
        m_kMin.z = value[2];
        m_kMax.x = value[3];
        m_kMax.y = value[4];
        m_kMax.z = value[5];
    }
    //------------------------------------------------------------------------------
}

//-----------------------------END FILE--------------------------------