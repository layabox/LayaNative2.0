

/**
@file			JCPlane.h
@brief			
@author			James
@version		1.0
@date			2016_10_31
*/

#ifndef __JCPlane_H__
#define __JCPlane_H__

#include "JCBoundingSphere.h"
#include "Vector3.h"

namespace laya
{
    enum PLANE_INTERSECTION_TYPE
    {
        PIT_BACK = 0,
        PIT_FRONT,
        PIT_INTERSECTING,
    };
    /** 
     * @brief 
    */
    class JCPlane
    {
    public:

        /** @brief构造函数
        */
        JCPlane();

        /** @brief构造函数
        */
        JCPlane( const Vector3& normal,float nDistance);

        /** @brief构造函数
        */
        JCPlane(const Vector3& point1, const Vector3& point2, const Vector3& point3);

        /** @brief析构函数
        */
        ~JCPlane();


        /** @brief normallize
         *  @return 
        */
        void normalize();

        /** @brief 点和平面的关系
         *  @param[in] 点的信息
         *  @return 
        */
        PLANE_INTERSECTION_TYPE intersectsPoint( const Vector3& point );

        /** @brief 点和包围球的关系
        *  @param[in] 包围球
        *  @return
        */
        PLANE_INTERSECTION_TYPE intersectsSphere(const JCBoundingSphere& sphere);

    public:
        Vector3       m_kNormal;
        float         m_nDistance;

    };
}

#endif //__JCPlane_H__

//-----------------------------END FILE--------------------------------