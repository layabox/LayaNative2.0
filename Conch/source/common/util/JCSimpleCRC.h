/**
@file			JCSimpleCRC.h
@brief			
@author			hugao
@version		1.0
@date			2016_5_12
*/

#ifndef __JCSimpleCRC_H__
#define __JCSimpleCRC_H__


class JCSimpleCRC
{
private:
	long long m_i64Key;
public:
    JCSimpleCRC()
	{
		m_i64Key = 0;
	}
	void process_bytes( const void *p_pBuffer, size_t p_iSize )
	{
		long long *pBuf64=(long long *)p_pBuffer;
		for(size_t i=0,isz=p_iSize/8; i<isz; i++,pBuf64++)
		{
			m_i64Key ^= *pBuf64;
		}
		for(size_t i=p_iSize/8*8; i<p_iSize; i++)
		{
			m_i64Key ^= ((const unsigned char *)p_pBuffer)[i];
		}
	}
	void reset()
	{
		m_i64Key = 0;
	}
	unsigned int checksum()
	{
		unsigned int lRet = 0;
		unsigned int *p = (unsigned int *)&m_i64Key;
		lRet ^= *p;
		lRet ^= *(p+1);
		return lRet;
	}
};
//------------------------------------------------------------------------------


#endif //__JCSimpleCRC_H__

//-----------------------------END FILE--------------------------------