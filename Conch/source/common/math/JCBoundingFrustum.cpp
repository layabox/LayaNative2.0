/**
@file			JCBoundingFrustum.cpp
@brief			
@author			James
@version		1.0
@date			2016_10_31
*/

#include "JCBoundingFrustum.h"
#include <stddef.h>

namespace laya
{
    JCBoundingFrustum::JCBoundingFrustum()
    {
    }
    JCBoundingFrustum::~JCBoundingFrustum()
    {
    }
    JCPlane* JCBoundingFrustum::getPlane(int index)
    {
        switch (index) {

        case 0:
            return &m_kNear;
        case 1:
            return &m_kFar;
        case 2:
            return &m_kLeft;
        case 3:
            return &m_kRight;
        case 4:
            return &m_kTop;
        case 5:
            return &m_kBottom;
        default:
            return NULL;
        }
        return NULL;
    }
    Vector3 JCBoundingFrustum::get3PlaneInterPoint(JCPlane* p1, JCPlane* p2, JCPlane* p3)
    {
        Vector3& p1Nor = p1->m_kNormal;
        Vector3& p2Nor = p2->m_kNormal;
        Vector3& p3Nor = p3->m_kNormal;
        
        Vector3 temp1 = p2Nor.cross(p3Nor);
        Vector3 temp2 = p3Nor.cross(p1Nor);
        Vector3 temp3 = p1Nor.cross(p2Nor);

        float a = p1Nor.dot(temp1);
        float b = p2Nor.dot(temp2);
        float c = p3Nor.dot(temp3);

        temp1 *= -p1->m_nDistance / a;
        temp2 *= -p2->m_nDistance / b;
        temp3 *= -p3->m_nDistance / c;

        return temp1 + temp2 + temp3;
    }
    void JCBoundingFrustum::getCorners(std::vector<Vector3>& pOutCorners)
    {
        pOutCorners.resize(8);
        pOutCorners[0] = get3PlaneInterPoint(&m_kNear, &m_kBottom, &m_kRight);
        pOutCorners[1] = get3PlaneInterPoint(&m_kNear, &m_kTop, &m_kRight);
        pOutCorners[2] = get3PlaneInterPoint(&m_kNear, &m_kTop, &m_kLeft);
        pOutCorners[3] = get3PlaneInterPoint(&m_kNear, &m_kBottom, &m_kLeft);
        pOutCorners[4] = get3PlaneInterPoint(&m_kFar, &m_kBottom, &m_kRight);
        pOutCorners[5] = get3PlaneInterPoint(&m_kFar, &m_kTop, &m_kRight);
        pOutCorners[6] = get3PlaneInterPoint(&m_kFar, &m_kTop, &m_kLeft);
        pOutCorners[7] = get3PlaneInterPoint(&m_kFar, &m_kBottom, &m_kLeft);
    }
    CONTAINMENT_TYPE JCBoundingFrustum::containsAxisAlignedBouningBox(JCBoundingBox& box)
    {
        static const int NUM_PLANES = 6;
        const JCPlane* pPlanes[NUM_PLANES] = { &m_kNear, &m_kLeft, &m_kRight, &m_kBottom, &m_kTop, &m_kFar };

        Vector3 p, n;
        Vector3 boxMin = box.m_kMin;
        Vector3 boxMax = box.m_kMax;

        for (int i = 0; i < NUM_PLANES; i++)
        {
            JCPlane* pPlane = (JCPlane*)(pPlanes[i]);

            const Vector3& planeNor = pPlane->m_kNormal;

            if (planeNor.x >= 0) {
                p.x = boxMax.x;
                n.x = boxMin.x;
            }
            else {
                p.x = boxMin.x;
                n.x = boxMax.x;
            }
            if (planeNor.y >= 0) {
                p.y = boxMax.y;
                n.y = boxMin.y;
            }
            else {
                p.y = boxMin.y;
                n.y = boxMax.y;
            }
            if (planeNor.z >= 0) {
                p.z = boxMax.z;
                n.z = boxMin.z;
            }
            else {
                p.z = boxMin.z;
                n.z = boxMax.z;
            }

            if (pPlane->intersectsPoint(p) == PIT_BACK)
                return CT_DISJOINT;

            if (pPlane->intersectsPoint(n) == PIT_BACK)
                return CT_INTERSECTS;
        }

        return CT_CONTAINS;
    }
    CONTAINMENT_TYPE JCBoundingFrustum::containsBoundingBox(JCBoundingBox& box)
    {
        static const int NUM_PLANES = 6;
        const JCPlane* pPlanes[NUM_PLANES] = { &m_kNear, &m_kLeft, &m_kRight, &m_kBottom, &m_kTop, &m_kFar };
        Vector3 kBoxCorners[8];
        box.getCorners(kBoxCorners);
        int nTotalIn = 0;
        for (int p = 0; p < NUM_PLANES; p++)
        {
            int nInCount = 8;
            int nPointIn = 1;
            JCPlane* pPlane = (JCPlane*)(pPlanes[p]);
            for (int i = 0; i < 8; ++i) 
            {
                if (pPlane->intersectsPoint(kBoxCorners[i]) == PIT_BACK)
                {
                    nPointIn=0;
                    nInCount--;
                }
            }
            if(nInCount == 0)
                return CT_DISJOINT;
            nTotalIn += nPointIn;
        }
        if (nTotalIn == 6)
            return CT_CONTAINS;
        return CT_INTERSECTS;
    }
    CONTAINMENT_TYPE JCBoundingFrustum::containsBoundingSphere(JCBoundingSphere& sphere)
    {
        PLANE_INTERSECTION_TYPE result = PIT_FRONT;
        PLANE_INTERSECTION_TYPE planeResult = PIT_FRONT;
        for (int i = 0; i < 6; i++) 
        {
            switch (i)
            {
            case 0:
                planeResult = m_kNear.intersectsSphere(sphere);
                break;
            case 1:
                planeResult = m_kFar.intersectsSphere(sphere);
                break;
            case 2:
                planeResult = m_kLeft.intersectsSphere(sphere);
                break;
            case 3:
                planeResult = m_kRight.intersectsSphere(sphere);
                break;
            case 4:
                planeResult = m_kTop.intersectsSphere(sphere);
                break;
            case 5:
                planeResult = m_kBottom.intersectsSphere(sphere);
                break;
            }
            switch (planeResult)
            {
            case PIT_BACK:
                return CT_DISJOINT;
            case PIT_INTERSECTING:
                result = PIT_INTERSECTING;
                break;
            }
        }
        switch (result) {
        case PIT_INTERSECTING:
            return CT_INTERSECTS;
        default:
            return CT_CONTAINS;
        }
    }
    
    void JCBoundingFrustum::getDebugLinePoint(Vector3* kOutPoints)
    {
        std::vector<Vector3> kOutCorners;
        getCorners(kOutCorners);
        kOutPoints[0] = kOutCorners[0];
        kOutPoints[1] = kOutCorners[1];
        kOutPoints[2] = kOutCorners[1];
        kOutPoints[3] = kOutCorners[2];
        kOutPoints[4] = kOutCorners[2];
        kOutPoints[5] = kOutCorners[3];
        kOutPoints[6] = kOutCorners[3];
        kOutPoints[7] = kOutCorners[0];
        kOutPoints[8] = kOutCorners[4];
        kOutPoints[9] = kOutCorners[5];
        kOutPoints[10] = kOutCorners[5];
        kOutPoints[11] = kOutCorners[6];
        kOutPoints[12] = kOutCorners[6];
        kOutPoints[13] = kOutCorners[7];
        kOutPoints[14] = kOutCorners[7];
        kOutPoints[15] = kOutCorners[4];
        kOutPoints[16] = kOutCorners[4];
        kOutPoints[17] = kOutCorners[0];
        kOutPoints[18] = kOutCorners[7];
        kOutPoints[19] = kOutCorners[3];
        kOutPoints[20] = kOutCorners[5];
        kOutPoints[21] = kOutCorners[1];
        kOutPoints[22] = kOutCorners[6];
        kOutPoints[23] = kOutCorners[2];
    }
    void JCBoundingFrustum::setValues(float* value)
    {
        Vector3* pNormal = &m_kNear.m_kNormal;
        pNormal->x = value[0]; pNormal->y = value[1]; pNormal->z = value[2]; m_kNear.m_nDistance = value[3];
        pNormal = &m_kFar.m_kNormal;
        pNormal->x = value[4]; pNormal->y = value[5]; pNormal->z = value[6]; m_kFar.m_nDistance = value[7];
        pNormal = &m_kLeft.m_kNormal;
        pNormal->x = value[8]; pNormal->y = value[9]; pNormal->z = value[10]; m_kLeft.m_nDistance = value[11];
        pNormal = &m_kRight.m_kNormal;
        pNormal->x = value[12]; pNormal->y = value[13]; pNormal->z = value[14]; m_kRight.m_nDistance = value[15];
        pNormal = &m_kTop.m_kNormal;
        pNormal->x = value[16]; pNormal->y = value[17]; pNormal->z = value[18]; m_kTop.m_nDistance = value[19];
        pNormal = &m_kBottom.m_kNormal;
        pNormal->x = value[20]; pNormal->y = value[21]; pNormal->z = value[22]; m_kBottom.m_nDistance = value[23];
    }
    bool JCBoundingFrustum::intersects(const JCBoundingBox& box)
    {
        const Vector3& min = box.m_kMin;
        const Vector3& max = box.m_kMax;
        float minX = min.x;
        float minY = min.y;
        float minZ = min.z;
        float maxX = max.x;
        float maxY = max.y;
        float maxZ = max.z;

        Vector3 nearNormal = this->m_kNear.m_kNormal;
        if (this->m_kNear.m_nDistance + (nearNormal.x * (nearNormal.x < 0 ? minX : maxX)) + (nearNormal.y * (nearNormal.y < 0 ? minY : maxY)) + (nearNormal.z * (nearNormal.z < 0 ? minZ : maxZ)) < 0)
            return false;

        Vector3 leftNormal = this->m_kLeft.m_kNormal;
        if (this->m_kLeft.m_nDistance + (leftNormal.x * (leftNormal.x < 0 ? minX : maxX)) + (leftNormal.y * (leftNormal.y < 0 ? minY : maxY)) + (leftNormal.z * (leftNormal.z < 0 ? minZ : maxZ)) < 0)
            return false;

        Vector3 rightNormal = this->m_kRight.m_kNormal;
        if (this->m_kRight.m_nDistance + (rightNormal.x * (rightNormal.x < 0 ? minX : maxX)) + (rightNormal.y * (rightNormal.y < 0 ? minY : maxY)) + (rightNormal.z * (rightNormal.z < 0 ? minZ : maxZ)) < 0)
            return false;

        Vector3 bottomNormal = this->m_kBottom.m_kNormal;
        if (this->m_kBottom.m_nDistance + (bottomNormal.x * (bottomNormal.x < 0 ? minX : maxX)) + (bottomNormal.y * (bottomNormal.y < 0 ? minY : maxY)) + (bottomNormal.z * (bottomNormal.z < 0 ? minZ : maxZ)) < 0)
            return false;

        Vector3 topNormal = this->m_kTop.m_kNormal;
        if (this->m_kTop.m_nDistance + (topNormal.x * (topNormal.x < 0 ? minX : maxX)) + (topNormal.y * (topNormal.y < 0 ? minY : maxY)) + (topNormal.z * (topNormal.z < 0 ? minZ : maxZ)) < 0)
            return false;

        // Can ignore far plane when distant object culling is handled by another mechanism
        Vector3 farNormal = this->m_kFar.m_kNormal;
        if (this->m_kFar.m_nDistance + (farNormal.x * (farNormal.x < 0 ? minX : maxX)) + (farNormal.y * (farNormal.y < 0 ? minY : maxY)) + (farNormal.z * (farNormal.z < 0 ? minZ : maxZ)) < 0)
            return false;

        return true;
    }
}

//-----------------------------END FILE--------------------------------
