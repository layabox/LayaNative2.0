/**
@file			JCKeyframeNode.h
@brief			
@author			James
@version		1.0
@date			2018_7_12
*/

#ifndef __JCKeyframeNode_H__
#define __JCKeyframeNode_H__

#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include "JCFloatKeyframe.h"
#include "JCFloatArrayKeyframe.h"

namespace laya
{
    class JCKeyframeNode
    {
    public:

        JCKeyframeNode()
        {
            m_nIndexInList = 0;
            m_nType = 0;
            m_nDataType = 0;
            m_pDataFloat = 0;
        }

        virtual ~JCKeyframeNode()
        {
            m_vOwnerPath.clear();
            m_vPropertys.clear();
            m_vKeyFrames.clear();
        }


        int getOwnerPathCount()
        {
            return m_vOwnerPath.size();
        }

        int getPropertyCount()
        {
            return m_vPropertys.size();
        }

        int getKeyFramesCount()
        {
            return m_vKeyFrames.size();
        }

        void _setOwnerPathCount(int value)
        {
            m_vOwnerPath.resize(value);
        }

        void _setOwnerPathByIndex(int index, const char* value)
        {
            m_vOwnerPath[index] = value;
        }

        const char* _joinOwnerPath(const char* sep)
        {
            s_sTempString = "";
            int nSize = m_vOwnerPath.size() - 1;
            for (int i = 0; i < nSize; i++)
            {
                s_sTempString += m_vOwnerPath[i];
                s_sTempString += sep;
            }
            s_sTempString += m_vOwnerPath[nSize];
            return s_sTempString.c_str();
        }

        void _setPropertyCount(int value)
        {
            m_vPropertys.resize(value);
        }

        void _setPropertyByIndex(int index, const char* value)
        {
            m_vPropertys[index] = value;
        }

        const char* _joinProperty(const char* sep)
        {
            s_sTempString = "";
            int nSize = m_vPropertys.size() - 1;
            for (int i = 0; i < nSize; i++)
            {
                s_sTempString += m_vPropertys[i];
                s_sTempString += sep;
            }
            s_sTempString += m_vPropertys[nSize];
            return s_sTempString.c_str();
        }

        void _setKeyframeCount(int value)
        {
            m_vKeyFrames.resize(value);
        }

        const char* getOwnerPathByIndex(int index)
        {
            return m_vOwnerPath[index].c_str();
        }

        const char* getPropertyByIndex(int index)
        {
            return m_vPropertys[index].c_str();
        }

        void setIndexInList(int n)
        {
            m_nIndexInList = n;
        }

        int getIndexInList()
        {
            return m_nIndexInList;
        }

        void setType(int nType)
        {
            m_nType = nType;
        }

        int getType()
        {
            return m_nType;
        }

        void setFullPath(const char* sPath)
        {
            m_sFullPath = sPath;
        }

        const char* getFullPath()
        {
            return m_sFullPath.c_str();
        }

        void setPropertyOwner(const char* sBuffer)
        {
            m_sPropertyOwner = sBuffer;
        }

        const char* getPropertyOwner()
        {
            return m_sPropertyOwner.c_str();
        }

        int getDataType()
        {
            return m_nDataType;
        }

        float getFloatData()
        {
            return m_pDataFloat;
        }

    public:
        static std::string                      s_sTempString;
        std::vector<std::string>                m_vOwnerPath;
        std::vector<std::string>                m_vPropertys;
        std::vector<JCKeyFrame*>                m_vKeyFrames;
        int                                     m_nIndexInList;
        int                                     m_nType;
        std::string                             m_sFullPath;
        std::string                             m_sPropertyOwner;
        short                                   m_nDataType;        //0为float  1为FloatArray
        float                                   m_pDataFloat;
        JCFloatArrayKeyframe::FloatArrayBuffer  m_pDataFloatArray;
    };
}
//------------------------------------------------------------------------------


#endif //__JCKeyframeNode_H__

//-----------------------------END FILE--------------------------------