/**
@file			JCWaveInfo.cpp
@brief			
@author			James
@version		1.0
@date			2012_11_17
*/

#include "JCWaveInfo.h"
#include "../../util/Log.h"
#pragma warning (disable: 4996)

namespace laya
{
//------------------------------------------------------------------------------
JCWaveInfo::JCWaveInfo( void )
{
    m_nTouchTime = 0;
	m_pExternalMark = NULL;
	m_pData = NULL;
	m_nCurPos = 0;
}
//------------------------------------------------------------------------------
JCWaveInfo::~JCWaveInfo( void )
{
	//赋值为NULL，就可以了，不需要删除
	if( m_pExternalMark != NULL )
	{
		m_pExternalMark = NULL;
	}
	if( m_pData != NULL )
	{
		delete[] m_pData;
		m_pData = NULL;
	}
}
//------------------------------------------------------------------------------
bool JCWaveInfo::LoadData( const char* p_sFileName )
{
	FILE *fp = fopen( p_sFileName,"rb" );
	if( fp == NULL )
	{
		LOGE("JCWaveInfo::LoadData Can't open file %s",p_sFileName );
		return false;
	}
	fseek( fp,0,SEEK_END );
	int nSize=ftell(fp);
	fseek(fp,0L,SEEK_SET);
	unsigned char* sBuffer = new unsigned char[nSize];
	fread( sBuffer,1,nSize,fp );
	fclose(fp);
	m_nCurPos = 0;
	LoadData( sBuffer,nSize ); 
    if (sBuffer)
    {
        delete[] sBuffer;
        sBuffer = NULL;
    }
	return true;
}
//------------------------------------------------------------------------------
bool JCWaveInfo::LoadData( unsigned char* p_sBuffer,int p_nBufferSize )
{
	if( p_sBuffer == NULL )
	{
		LOGE("JCWaveInfo::LoadData buffer == null" );
		return false;
	}

	//读取 RIFF 头
	{
		read( &m_kRiff, sizeof(RIFF_HEADER), p_sBuffer );
		if(	memcmp( m_kRiff.szRiffID, "RIFF", 4 ) != 0 || memcmp( m_kRiff.szRiffFormat, "WAVE", 4 ) != 0 )
		{
			LOGE("JCWaveInfo::LoadData No a vaild wave file!\n");
			return false;
		}
		m_nCurPos += sizeof(RIFF_HEADER);
	}

	//读取格式
	{
		read( &m_kFmtBlock.szFmtID, 4, p_sBuffer );
		m_nCurPos += 4;
		read( &m_kFmtBlock.dwFmtSize, 4, p_sBuffer );
		m_nCurPos += 4;

		if(	memcmp(m_kFmtBlock.szFmtID, "fmt ", 4) !=0 )
		{
			LOGE("JCWaveInfo::LoadData we only support follow format" );
			return false;
		}
		if(	m_kFmtBlock.dwFmtSize != 18 && m_kFmtBlock.dwFmtSize != 16 )
		{
			LOGE("JCWaveInfo::LoadData we only support Format: linear PCM  size = %d",m_kFmtBlock.dwFmtSize  );
			return false;
		}
		if( m_kFmtBlock.dwFmtSize == 16 )
		{
			read(&m_kFmtBlock.wavFormat, 16, p_sBuffer );
			m_nCurPos += 16;
		}
		else if( m_kFmtBlock.dwFmtSize == 18 )
		{
			read(&m_kFmtBlock.wavFormat, 18, p_sBuffer );
			m_nCurPos += 18;
		}
        /*
		if(	m_kFmtBlock.wavFormat.dwSamplesPerSec != 22050 )
		{
			LOGE("we only support Samples Rate:  22050 KHz" );
			return false;
		}
		if(	m_kFmtBlock.wavFormat.wChannels != 0x1 )
		{
			LOGE("we only support one channel" );
			return false;
		}
		if(	m_kFmtBlock.wavFormat.wBitsPerSample != 16 )
		{
			LOGE("we only support BitsPerSample: 16" );
			return false;
		}
		if(	m_kFmtBlock.wavFormat.wFormatTag != 0x1 )
		{
			LOGE("we only support wFormatTag != 0x1" );
			return false;
		}
        */
	}
    /*
	//读取无用信息 改到下面循环查找了
    {
        //试图读取fact
        read(&m_kFact, sizeof(FACT_BLOCK), p_sBuffer);
        if (memcmp(m_kFact.szFactID, "fact", 4) == 0)
        {
            m_nCurPos += sizeof(FACT_BLOCK);
            read(&m_vFackBlockBuffer, m_kFact.dwFactSize, p_sBuffer);
            m_nCurPos += m_kFact.dwFactSize;
        }
        //试图读取JUNK
        read(&m_kJunkBlock, sizeof(_JUNK_BLOCK), p_sBuffer);
        if (memcmp(m_kJunkBlock.szJUNK, "JUNK", 4) == 0)
        {
            m_nCurPos += sizeof(_JUNK_BLOCK) + m_kJunkBlock.dwJunkSize;
        }
        //试图读取FLLR
        read(&m_kJunkBlock, sizeof(_JUNK_BLOCK), p_sBuffer);
        if (memcmp(m_kJunkBlock.szJUNK, "FLLR", 4) == 0)
        {
            m_nCurPos += sizeof(_JUNK_BLOCK) + m_kJunkBlock.dwJunkSize;
        }
    }
    */
    bool bResult = false;
    int nSize = p_nBufferSize - sizeof(DATA_BLOCK);
	while( m_nCurPos < nSize )
	{
		read( &m_kDataBlock, sizeof(DATA_BLOCK), p_sBuffer );
		if ( memcmp( m_kDataBlock.szDataID, "data", 4 ) == 0 )
		{
            m_kDataBlock.dwDataSize /= 2;
            m_nCurPos += sizeof(DATA_BLOCK);
            bResult = true;
            break;
		}
        else
        {
            m_nCurPos += sizeof(DATA_BLOCK) + m_kDataBlock.dwDataSize;
        }
	}
	
    if (bResult == false)
    {
        LOGE("JCWaveInfo::LoadData parse wav error");
        return false;
    }
	//拿出data数据
	m_nRealDataSize = m_kDataBlock.dwDataSize * 2;
	m_pData = new U8[m_nRealDataSize];
	for( unsigned int i =0; i< m_kDataBlock.dwDataSize; i++ )
	{
		U8 n1 = (U8) getC( p_sBuffer, p_nBufferSize);
		m_nCurPos ++;
		U8 n2 = (U8) getC( p_sBuffer, p_nBufferSize);
		m_nCurPos ++;

		m_pData[i*2+0] = n1;
		m_pData[i*2+1] = n2;
	}
	return true;
}
//------------------------------------------------------------------------------
void JCWaveInfo::read( void* pRet, int p_nSize,unsigned char* p_sBuffer )
{
	unsigned char* p = p_sBuffer + m_nCurPos;
	memcpy( pRet,(void*)(p),p_nSize );
}
//------------------------------------------------------------------------------
unsigned char JCWaveInfo::getC( unsigned char* p_sBuffer, int p_nBufferSize)
{
	if (m_nCurPos >= p_nBufferSize) {
		return 0;
	}
	return p_sBuffer[m_nCurPos];
}
//------------------------------------------------------------------------------
}

//-----------------------------END FILE--------------------------------
