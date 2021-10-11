/**
@file			JCBoundingBox.h
@brief			
@author			James
@version		1.0
@date			2016_10_31
*/

#ifndef __JCBoundingBox_H__
#define __JCBoundingBox_H__

#include <vector>
#include "Vector3.h"

namespace laya
{
    enum CONTAINMENT_TYPE
    {
        CT_DISJOINT = 0,    //不相交
        CT_CONTAINS = 1,    //全部包含
        CT_INTERSECTS = 2,  //相交
    };
    /** 
     * @brief 
    */
    class JCBoundingBox
    {
    public:

        /** @brief构造函数
        */
        JCBoundingBox(const Vector3& min = { -0.5f,-0.5f,-0.5f }, const Vector3& max = { 0.5f,0.5f,0.5f });


        /** @brief 返回八个点
         *  @param[out] 返回vector每个点
         *  @return 
        */
        void getCorners(Vector3* kOutCorners);

        
        /** @brief 返回绘制线框的点数 24个点
         *  @param[out] 返回24个点
         *  @return 
        */
        void getDebugLinePoint(Vector3* kOutCorners);

        /** @brief析构函数
        */
        ~JCBoundingBox();

        /** @brief 两个盒子是否相交
         *  @param[in] 其他盒子
         *  @return 
        */
        bool intersectsWithBox(JCBoundingBox& pOther)
        {
            return (m_kMin <= pOther.m_kMax) && (m_kMax >= pOther.m_kMin);   
        }

        /** @brief 是否完全在包含其中
        *  @param[in] 其他盒子
        *  @return
        */
        bool isFullInside(JCBoundingBox& pOther)
        {
            return m_kMin >= pOther.m_kMin && m_kMax <= pOther.m_kMax;
        }

        bool isContain(JCBoundingBox& pOther)
        {
            return pOther.m_kMin >= m_kMin && pOther.m_kMax <= m_kMax;
        }

        Vector3 getCenter() const
        {
            return (m_kMin + m_kMax) * 0.5f;
        }

        void setCenter( const Vector3& kCenter )
        {
           Vector3 kMove = kCenter - getCenter();
            m_kMin += kMove;
            m_kMax += kMove;
        }

        /** @brief 获得size
         *  @return 
        */
        Vector3 getSize()
        {
            return m_kMax - m_kMin;
        }

       Vector3 getHalfSize()
        {
            return (m_kMax - m_kMin)*0.5;
        }

        void setValues(float* value);

    public:
        
        Vector3       m_kMin;
        Vector3       m_kMax;

    };
}

#endif //__JCBoundingBox_H__

//-----------------------------END FILE--------------------------------
