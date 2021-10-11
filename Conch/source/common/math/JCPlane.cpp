/**
@file			JCPlane.cpp
@brief			
@author			James
@version		1.0
@date			2016_10_31
*/

#include "JCPlane.h"
#include "JCBoundingSphere.h"
#include <math.h>

namespace laya
{
    JCPlane::JCPlane()
    {
        m_nDistance = 0;
    }
    JCPlane::JCPlane(const Vector3& normal, float nDistance)
    {
        m_kNormal = normal;
        m_nDistance = nDistance;
    }
    JCPlane::JCPlane(const Vector3& point1, const Vector3& point2, const Vector3& point3)
    {
        float x1 = point2.x - point1.x;
        float y1 = point2.y - point1.y;
        float z1 = point2.z - point1.z;
        float x2 = point3.x - point1.x;
        float y2 = point3.y - point1.y;
        float z2 = point3.z - point1.z;
        float yz = (y1 * z2) - (z1 * y2);
        float xz = (z1 * x2) - (x1 * z2);
        float xy = (x1 * y2) - (y1 * x2);
        float invPyth = 1 / (sqrtf((yz * yz) + (xz * xz) + (xy * xy)));
        float x = yz * invPyth;
        float y = xz * invPyth;
        float z = xy * invPyth;
        m_kNormal.x = x;
        m_kNormal.y = y;
        m_kNormal.z = z;
        m_nDistance = -((x * point1.x) + (y * point1.y) + (z * point1.z));
    }
    JCPlane::~JCPlane()
    {

    }
    void JCPlane::normalize()
    {
        float fMagnitude = 1 / sqrtf(m_kNormal.x * m_kNormal.x + m_kNormal.y * m_kNormal.y + m_kNormal.z * m_kNormal.z);
        m_kNormal.x *= fMagnitude;
        m_kNormal.y *= fMagnitude;
        m_kNormal.z *= fMagnitude;
        m_nDistance *= fMagnitude;
    }
    PLANE_INTERSECTION_TYPE JCPlane::intersectsPoint(const Vector3& point)
    {
        float distance = m_kNormal.dot(point) + m_nDistance;
        if (distance > 0)
            return PIT_FRONT;
        else if (distance < 0)
            return PIT_BACK;
        else
            return PIT_INTERSECTING;
    }
    PLANE_INTERSECTION_TYPE JCPlane::intersectsSphere( const JCBoundingSphere& sphere)
    {
        float sphereR = sphere.m_nRadius;
        float distance = m_kNormal.dot(sphere.m_kCenter) + m_nDistance;
        if (distance > sphereR)
            return PIT_FRONT;
        else if (distance < -sphereR)
            return PIT_BACK;
        else
            return PIT_INTERSECTING;
    }
}

//-----------------------------END FILE--------------------------------
