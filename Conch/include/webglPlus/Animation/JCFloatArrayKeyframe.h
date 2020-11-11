/**
@file			JCFloatArrayKeyframe.h
@brief			
@author			James
@version		1.0
@date			2018_7_12
*/

#ifndef __JCFloatArrayKeyframe_H__
#define __JCFloatArrayKeyframe_H__

#include <stdio.h>
#include <string>
#include <map>
#include "JCFloatKeyframe.h"

namespace laya
{
    class JCFloatArrayKeyframe : public JCKeyFrame
    {
    public:

        struct FloatArrayBuffer
        {
            char*   data;
            int     byteSize;
            FloatArrayBuffer()
            {
                data = NULL;
                byteSize = 0;
            }
        };

    public:

        JCFloatArrayKeyframe() {}

        virtual ~JCFloatArrayKeyframe() {}

        void setTime(float nTime)
        {
            m_nTime = nTime;
        }

        float getTime()
        {
            return m_nTime;
        }

    public:

        FloatArrayBuffer    m_nInTangent;
        FloatArrayBuffer    m_nOutTangent;
        FloatArrayBuffer    m_nValue;
        FloatArrayBuffer    m_pData;
    };
}
//------------------------------------------------------------------------------


#endif //__JCFloatArrayKeyframe_H__

//-----------------------------END FILE--------------------------------