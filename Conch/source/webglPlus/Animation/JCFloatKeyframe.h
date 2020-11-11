/**
@file			JCFloatKeyframe.h
@brief			
@author			James
@version		1.0
@date			2018_7_12
*/

#ifndef __JCFloatKeyframe_H__
#define __JCFloatKeyframe_H__

#include <stdio.h>
#include <string>
#include <map>

namespace laya
{

    class JCKeyFrame
    {
    public:
        JCKeyFrame()
        {
            m_nTime = 0;
        }
        virtual ~JCKeyFrame()
        {

        }
        float m_nTime;
    };
    class JCFloatKeyframe : public JCKeyFrame
    {
    public:
        JCFloatKeyframe()
        {
            m_nInTangent = 0;
            m_nOutTangent = 0;
            m_nValue = 0;
        }

        virtual ~JCFloatKeyframe() {}

        void setTime(float nTime)
        {
            m_nTime = nTime;
        }

        float getTime()
        {
            return m_nTime;
        }

        void setInTangent(float nInTangent)
        {
            m_nInTangent = nInTangent;
        }

        float getInTangent()
        {
            return m_nInTangent;
        }

        void setOutTangent(float nInTangent)
        {
            m_nOutTangent = nInTangent;
        }

        float getOutTangent()
        {
            return m_nOutTangent;
        }

        void setValue(float nValue)
        {
            m_nValue = nValue;
        }

        float getValue()
        {
            return m_nValue;
        }

        void _cloneTo(JCFloatKeyframe* pDestObj)
        {
            pDestObj->m_nTime = m_nTime;
            pDestObj->m_nInTangent = m_nInTangent;
            pDestObj->m_nOutTangent = m_nOutTangent;
            pDestObj->m_nValue = m_nValue;
        }

    public:

        float   m_nInTangent;
        float   m_nOutTangent;
        float   m_nValue;
    };
}
//------------------------------------------------------------------------------


#endif //__JCFloatKeyframe_H__

//-----------------------------END FILE--------------------------------