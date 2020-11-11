/**
@file			JCCommandEncoderBuffer.cpp
@brief			
@author			James
@version		1.0
@date			2019_8_24
*/

#include "JCCommandEncoderBuffer.h"

namespace laya
{
	JCCommandEncoderBuffer::JCCommandEncoderBuffer( unsigned int nSize, unsigned int nAdjustSize)
	{
        m_bNeedDelBuffer = true;
        m_pBuffer=NULL;
		m_nBufSize=0;
		m_nDataSize=0;
		m_nAdjustSize= nAdjustSize;
		m_nReadPos=0;
		m_bChanged=true;
        if (nSize > 0)
        {
            expand(nSize);
        }
	}
	JCCommandEncoderBuffer::~JCCommandEncoderBuffer()
	{
        if ( m_bNeedDelBuffer && m_pBuffer )
        {
            delete[] m_pBuffer;
            m_pBuffer = 0;
        }
	}
	void JCCommandEncoderBuffer::setBufferExpandStep(int nAdjustSize)
	{
        m_nAdjustSize = nAdjustSize;
	}
    void JCCommandEncoderBuffer::setAlign(bool bAlign)
    {
        m_bAlign = bAlign;
    }
    void JCCommandEncoderBuffer::setBufferSize(int nBufferSize)
    {
        if (nBufferSize < 0)
        {
            return;
        }
        if (nBufferSize >= (int)m_nBufSize)
        {
            expand(nBufferSize - m_nBufSize);
        }
    }
	void JCCommandEncoderBuffer::expand(int nExpandSize)
	{
		if (nExpandSize<1 ) return;
		unsigned long nSize= nExpandSize+m_nBufSize+m_nAdjustSize;	//需要按4对齐么
		char* pBuf=new char[nSize];
		if (!pBuf)
        {
			return;
		}
		if (m_pBuffer)
        {
			memcpy(pBuf, m_pBuffer,m_nBufSize);
			delete [] m_pBuffer;
		}
		m_nBufSize=nSize;
        m_pBuffer=pBuf;
		m_bChanged=true;
	}
};
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------