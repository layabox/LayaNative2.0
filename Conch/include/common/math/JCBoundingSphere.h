/**
@file			JCBoundingSphere.h
@brief			
@author			James
@version		1.0
@date			2016_10_31
*/

#ifndef __JCBoundingSphere_H__
#define __JCBoundingSphere_H__

#include <vector>
#include "Vector3.h"

namespace laya
{
    /** 
     * @brief 
    */
    class JCBoundingSphere
    {
    public:

        JCBoundingSphere();

        /** @brief构造函数
        */
        JCBoundingSphere( const Vector3& center,float nRadius );

        /** @brief析构函数
        */
        ~JCBoundingSphere();

        void setValues(float* value);

    public:

        Vector3       m_kCenter;
        float           m_nRadius;

    };
}
//------------------------------------------------------------------------------


#endif //__JCBoundingSphere_H__

//-----------------------------END FILE--------------------------------