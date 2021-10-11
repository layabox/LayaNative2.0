/**
@file			JsBlob.h
@brief			
@author			James
@version		1.0
@date			2017_11_28
*/

#ifndef __JsBlob_H__
#define __JsBlob_H__

#include "../JSInterface/JSInterface.h"
#include <stdio.h>
#include <string.h>

namespace laya 
{
    class JsBlob : public JsObjBase, public JSObjNode
    {
    public:
        JsBlob()
        {
            m_pBuffer = 0;
            m_i64Size = 0;
            m_pszType = 0;
            SetType("text/plain");
        }
        JsBlob(void *p_pData, size_t p_iSize, const char *p_pszType = 0)
        {
            m_pBuffer = 0;
            m_i64Size = 0;
            m_pszType = 0;
            SetData(p_pData, p_iSize);
            SetType(p_pszType);
            if (0 == m_pszType) SetType("text/plain");
        }
        ~JsBlob()
        {
            if (0 != m_pBuffer)
            {
                delete[] m_pBuffer;
                m_pBuffer = 0;
            }
            if (0 != m_pszType)
            {
                delete[] m_pszType;
                m_pszType = 0;
            }
        }
        uint32_t GetSize()
        {
            return static_cast<uint32_t>(m_i64Size);
        }
        void close()
        {
            SetData(0, 0);
        }
        void Allocate(size_t p_iSize)
        {
            if (0 != m_pBuffer)
            {
                delete[] m_pBuffer;
                m_pBuffer = 0;
            }
            m_i64Size = 0;

            if (p_iSize > 0)
            {
                m_pBuffer = new char[p_iSize];
                m_i64Size = p_iSize;
            }
        }
        void SetData(const void *p_pData, size_t p_iLen)
        {
            if (0 != m_pBuffer)
            {
                delete[] m_pBuffer;
                m_pBuffer = 0;
            }
            m_i64Size = 0;
            if (0 != p_pData && 0 != p_iLen)
            {
                m_pBuffer = new char[p_iLen];
                memcpy(m_pBuffer, p_pData, p_iLen);
                m_i64Size = p_iLen;
            }
        }
        JsValue slice(uint64_t p_iBegin = 0, uint64_t p_iEnd = -1)
        {
            return JSP_TO_JS_NULL;
        }
        const char *GetType()
        {
            return (0 == m_pszType) ? "" : m_pszType;
        }
        void SetType(const char *p_pszType)
        {
            if (0 != m_pszType)
            {
                delete[] m_pszType;
                m_pszType = 0;
            }
            size_t len;
            if (0 != p_pszType && 0 != (len = strlen(p_pszType)))
            {
                m_pszType = new char[len + 1];
                memcpy(m_pszType, p_pszType, len + 1);
                JsBlob::ToLowerString(m_pszType);
            }
        }
    protected:

        static char *ToLowerString(char *p_pszStr)
        {
            static const char __cdis = 'a' - 'A';

            char *pBegin = p_pszStr;
            for (; 0 != *pBegin; pBegin++)
            {
                if (*pBegin >= 'A' && *pBegin <= 'Z')
                    *pBegin += __cdis;
            }
            return p_pszStr;
        }

    public:

        static JsObjClassInfo JSCLSINFO;
        friend class JsFileReader;

    protected:
        char*       m_pBuffer;
        uint64_t  m_i64Size;
        char*       m_pszType;
    };
}
//------------------------------------------------------------------------------


#endif //__JsBlob_H__

//-----------------------------END FILE--------------------------------