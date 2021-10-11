
/**
@file			JCBoundingFrustum.h
@brief			
@author			James
@version		1.0
@date			2016_10_31
*/

#ifndef __JCBoundingFrustum_H__
#define __JCBoundingFrustum_H__

#include "JCBoundingBox.h"
#include "JCBoundingSphere.h"
#include "JCPlane.h"
#include "Vector3.h"

namespace laya
{
    /** 
     * @brief 
    */
    class JCBoundingFrustum
    {
    public:

        /** @brief构造函数
        */
        JCBoundingFrustum();

        /** @brief析构函数
        */
        ~JCBoundingFrustum();

        JCPlane* getPlane(int index);

        Vector3 get3PlaneInterPoint( JCPlane* p1, JCPlane* p2, JCPlane* p3 );

        void getCorners(std::vector<Vector3>& pOutCorners);

        CONTAINMENT_TYPE containsAxisAlignedBouningBox(JCBoundingBox& box);

        CONTAINMENT_TYPE containsBoundingBox(JCBoundingBox& box );

        CONTAINMENT_TYPE containsBoundingSphere(JCBoundingSphere& sphere);

        void getDebugLinePoint(Vector3* kOutPoints);

        void setValues(float* value);

        bool intersects(const JCBoundingBox& box);

    public:

        JCPlane             m_kNear;
        JCPlane             m_kFar;
        JCPlane             m_kLeft;
        JCPlane             m_kRight;
        JCPlane             m_kTop;
        JCPlane             m_kBottom;
        
    };
}

#endif //__JCBoundingFrustum_H__

//-----------------------------END FILE--------------------------------