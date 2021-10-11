/**
@file			JCBuffer.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_11
*/

#include "JCBuffer.h"

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
    //------------------------------------------------------------------------------
	JCBuffer::JCBuffer()
    {
		m_bNeedDel=false;
		m_pPtr = 0;
		m_nLen = 0;
	}
    JCBuffer::JCBuffer(int p_nSize )
    {
		m_nLen=p_nSize;
		m_pPtr = new char [p_nSize];
		m_bNeedDel = true;
	}
    JCBuffer::JCBuffer(void* p_ptr, int p_nLength, bool p_bCpy, bool p_bManage)
    {
		m_bNeedDel = p_bManage;
		if(p_bCpy)
        {
			m_bNeedDel = true;
		}
		m_pPtr = (char*)p_ptr;
		m_nLen = p_nLength;
	}
    JCBuffer::JCBuffer(char* p_pStr, encode p_end)
    {
	}
    JCBuffer::~JCBuffer()
    {
		if(m_bNeedDel)
			delete [] m_pPtr;
		m_pPtr = 0;
	}
	bool JCBuffer::create(int p_nLen)
    {
		m_nLen = p_nLen;
		if( m_pPtr && m_bNeedDel )
        {
			delete [] m_pPtr;
		}
		m_pPtr = new char [m_nLen];
		m_bNeedDel=true;
		return m_pPtr!=0;
	}
	void JCBuffer::free()
    {
		m_nLen = 0;
		if(m_bNeedDel)
        {
			m_bNeedDel=false;
			delete [] m_pPtr;
			m_pPtr = 0;
		}
	}
	bool JCBuffer::toString(encode p_enc)
    {
		return false;
	}
};
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------