/**
@file			JCArrayBufferManager.h
@brief			
@author			James
@version		1.0
@date			2017_11_29
*/

#ifndef __JCArrayBufferManager_H__
#define __JCArrayBufferManager_H__

#include "../buffer/JCBuffer.h"
#include <vector>
#include <unordered_map>

namespace laya
{
    class JCArrayBufferManager
    {
    public:
        enum ARRAY_BUFFER_TYPE
        {
            ARRAY_BUFFER_TYPE_DATA,
            ARRAY_BUFFER_TYPE_CMD,
        };

        enum ARRAY_BUFFER_REF_TYPE
        {
            ARRAY_BUFFER_REF_TYPE_REFERENCE,
            ARRAY_BUFFER_REF_TYPE_COPY,
        }; 

        class ArrayBufferContent
        {
        public:
            ArrayBufferContent(char* pBuffer, int nLength, ARRAY_BUFFER_TYPE nType,bool bNeedDelBuffer )
            {
                m_bNeedDelBuffer = bNeedDelBuffer;
                m_nType = nType;
                m_pBuffer = pBuffer;
                m_nLength = nLength;
                m_nNoteSize = 0;
                m_pCommandEncoderBuffer = NULL;
                if (m_nType == ARRAY_BUFFER_TYPE_CMD)
                {
                    m_pCommandEncoderBuffer = new JCCommandEncoderBuffer(0, 0);
                    m_nNoteSize = *(int*)m_pBuffer;
                    if (m_nNoteSize > 0)
                    {
                        m_pCommandEncoderBuffer->setShareBuffer(m_pBuffer + 4, (m_nNoteSize - 1) * 4);
                    }
                }
            }
            ArrayBufferContent(ARRAY_BUFFER_TYPE nType,bool bNeedDelBuffer )
            {
                m_bNeedDelBuffer = bNeedDelBuffer;
                m_nType = nType;
                m_pBuffer = NULL;
                m_nLength = 0;
                m_nNoteSize = 0;
                m_pCommandEncoderBuffer = NULL;
                if (m_nType == ARRAY_BUFFER_TYPE_CMD)
                {
                    m_pCommandEncoderBuffer = new JCCommandEncoderBuffer(0, 0);
                }
            }
            void mallocBuffer(char* pBuffer, int nLength, ARRAY_BUFFER_TYPE nType)
            {
                if (m_pBuffer)
                {
                    delete m_pBuffer;
                }
                m_nLength = nLength;
                m_pBuffer = new char[m_nLength];
                memcpy(m_pBuffer, pBuffer, nLength);
                if (m_nType == ARRAY_BUFFER_TYPE_CMD)
                {
                    m_pCommandEncoderBuffer = new JCCommandEncoderBuffer(0, 0);
                    m_nNoteSize = *(int*)m_pBuffer;
                    if (m_nNoteSize > 0)
                    {
                        m_pCommandEncoderBuffer->setShareBuffer(m_pBuffer + 4, (m_nNoteSize - 1) * 4);
                    }
                }
            }
            ~ArrayBufferContent()
            {
                if (m_bNeedDelBuffer)
                {
                    delete m_pBuffer;
                    m_pBuffer = NULL;
                }
                if (m_pCommandEncoderBuffer)
                {
                    delete m_pCommandEncoderBuffer;
                    m_pCommandEncoderBuffer = NULL;
                }
            }
            void syncContent(ArrayBufferContent* pContent)
            {
                if (m_pBuffer)
                {
                    if (m_nLength == pContent->m_nLength)
                    {
                        memcpy(m_pBuffer, pContent->m_pBuffer, m_nLength);
                    }
                    else
                    {
                        delete m_pBuffer;
                        m_nLength = pContent->m_nLength;
                        m_pBuffer = new char[m_nLength];
                        memcpy(m_pBuffer, pContent->m_pBuffer, m_nLength);
                    }
                }
                else
                {
                    m_nLength = pContent->m_nLength;
                    m_pBuffer = new char[m_nLength];
                    memcpy(m_pBuffer, pContent->m_pBuffer, m_nLength);
                }
                //同步MemClass
                if (pContent->m_pCommandEncoderBuffer)
                {
                    if (m_pCommandEncoderBuffer == NULL)
                    {
                        m_pCommandEncoderBuffer = new JCCommandEncoderBuffer(0, 0);
                    }
                    m_nNoteSize = *(int*)m_pBuffer;
                    if (m_nNoteSize > 0)
                    {
                        m_pCommandEncoderBuffer->setShareBuffer(m_pBuffer + 4, (m_nNoteSize - 1) * 4);
                    }
                }
                else
                {
                    if (m_pCommandEncoderBuffer)
                    {
                        delete m_pCommandEncoderBuffer;
                        m_pCommandEncoderBuffer = NULL;
                    }
                }
            }
            void setBuffer(char* pBuffer, int nLength)
            {
                m_pBuffer = pBuffer;
                m_nLength = nLength;
                if (m_pCommandEncoderBuffer)
                {
                    m_nNoteSize = *(int*)m_pBuffer;
                    if (m_nNoteSize > 0)
                    {
                        m_pCommandEncoderBuffer->setShareBuffer(m_pBuffer + 4, (m_nNoteSize - 1) * 4);
                    }
                }
            }
            JCCommandEncoderBuffer* getCommandEncoderBuffer()
            {
                if (*(int*)m_pBuffer != m_nNoteSize )
                {
                    m_nNoteSize = *(int*)m_pBuffer;
                    if (m_nNoteSize > 0)
                    {
                        m_pCommandEncoderBuffer->setShareBuffer(m_pBuffer + 4, (m_nNoteSize - 1) * 4);
                    }
                }
                return m_pCommandEncoderBuffer;
            }
        public:
            char*                           m_pBuffer;
            int                             m_nLength;
            bool                            m_bNeedDelBuffer;
            ARRAY_BUFFER_TYPE               m_nType;
        public:
            int                             m_nNoteSize;                    //记录buffer中的第0个值，为了快速判断用
            JCCommandEncoderBuffer*         m_pCommandEncoderBuffer;        //为了解流使用
        };
        typedef std::vector<ArrayBufferContent*>     VectorBuffer;

    public:

        JCArrayBufferManager();

        ~JCArrayBufferManager();

        void clearAll();
        
        inline ArrayBufferContent* getArrayBuffer(int nID)
        {
            return nID < m_vBuffers.size() ? m_vBuffers[nID] : NULL;
        }

        int getID();
        
        void removeArrayBuffer(int nID);

        int createArrayBuffer(char* pBuffer, int nLength, ARRAY_BUFFER_TYPE nType, ARRAY_BUFFER_REF_TYPE nRefType);

        bool updateArrayBuffer(int nID, char* pBuffer, int nLength);

        bool syncArrayBufferDataToRuntime(int nID, char* pBuffer, int nLength);

        void prepareRemoveArrayBuffer(int nID);

    public:
        
        int                 m_nGlobalID;
        VectorBuffer        m_vBuffers;
        std::vector<int>    m_vPrepareDelIDs;

    };
}
//------------------------------------------------------------------------------


#endif //__JCArrayBufferManager_H__

//-----------------------------END FILE--------------------------------