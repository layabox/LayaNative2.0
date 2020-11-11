/**
@file			JCOggParser.cpp
@brief			
@author			James
@version		1.0
@date			2014_11_26
*/

//包含头文件
#include "JCOggParser.h"
#include "../../util/Log.h"
#include <vorbis/vorbisfile.h>
#include <ogg/ogg.h>
#include <vector>
#pragma warning (disable: 4996)

namespace laya
{
#define READ_OGG_BUFFER_TEMP_SIZE 44100	//读取ogg的时候，临时缓冲区大小
//------------------------------------------------------------------------------
JCOggParser* JCOggParser::ms_pOggParser = NULL;
//------------------------------------------------------------------------------
JCOggParser::JCOggParser()
{
	m_nBufferSize = READ_OGG_BUFFER_TEMP_SIZE;
	m_nCurBufferPos = 0;
	m_pBuffer = new char[m_nBufferSize];
}
//------------------------------------------------------------------------------
JCOggParser::~JCOggParser()
{
	if( m_pBuffer != NULL )
	{
		delete[] m_pBuffer;
		m_pBuffer = NULL;
	}
}
//------------------------------------------------------------------------------
JCOggParser* JCOggParser::GetInstance( void )
{
	if( ms_pOggParser == NULL )
	{
		ms_pOggParser = new JCOggParser();
	}
	return ms_pOggParser;
}
//-----------------------------------------------------------------------------
void JCOggParser::DelInstance( void )
{

}
//-----------------------------------------------------------------------------
bool JCOggParser::copyBuffer( char* p_pBuffer,int p_nSize )
{
	if( p_pBuffer == NULL || p_nSize <=0 ) return false;
	if( m_nCurBufferPos + p_nSize > m_nBufferSize )
	{
		int nElargeSize = (p_nSize<=READ_OGG_BUFFER_TEMP_SIZE) ? READ_OGG_BUFFER_TEMP_SIZE : p_nSize;
		char* pNewBuffer = new char[m_nBufferSize + nElargeSize];
		memcpy( pNewBuffer,m_pBuffer,m_nBufferSize );
		m_nBufferSize += nElargeSize;
		delete m_pBuffer;
		m_pBuffer = pNewBuffer;
	}
	memcpy( m_pBuffer+m_nCurBufferPos,p_pBuffer,p_nSize );
	m_nCurBufferPos += p_nSize;
	return true;
}
//------------------------------------------------------------------------------
JCWaveInfo* JCOggParser::GetWaveInfo( const char* p_sFileName,unsigned char* p_sBuffer, int p_nBufferSize )
{
	m_nCurBufferPos = 0;
	OggVorbis_File  pOggStream;
	vorbis_info* pVorbisInfo = NULL;
	int nResult = -1;
	if( p_sBuffer != NULL && p_nBufferSize > 0 )
	{
		nResult = ov_open_callbacks( NULL, &pOggStream, (const char*)p_sBuffer, p_nBufferSize, OV_CALLBACKS_DEFAULT );
	}
	else if( p_sFileName != NULL )
	{
		FILE* fp = NULL;
		fp = fopen( p_sFileName, "rb" );
		if( fp )
		{
			nResult = ov_open_callbacks( (void*)fp, &pOggStream, NULL, 0, OV_CALLBACKS_DEFAULT );
		}
		fclose( fp );
	}
	if( nResult >= 0 )
	{
		pVorbisInfo = ov_info( &pOggStream, -1 );
		char pBufferData[ READ_OGG_BUFFER_TEMP_SIZE ];//解码时候的缓冲区
		int nSize = 0;
		int nPortion = 0;
		int nResult1 = 0;
		while( true )
		{
			nResult1 = ov_read( &pOggStream, pBufferData, sizeof(pBufferData), 0, 2, 1, &nPortion );
			if( nResult1 > 0 )
			{
				nSize += nResult1;
				copyBuffer( pBufferData,nResult1 );
			}
			else if( nResult1 == 0 )
			{
				break;
			}
			else
			{
				LOGE("JCOggParser::GetWaveInfo error 1");
				return NULL;
			}
		}
		if( nSize == 0)
		{
			LOGE("JCOggParser::GetWaveInfo error 2");
			return NULL;
		}
		JCWaveInfo* pWaveInfo = new JCWaveInfo();
		pWaveInfo->m_nRealDataSize = nSize;
		pWaveInfo->m_pData = new unsigned char[nSize];
		if( nSize == m_nCurBufferPos )
		{
			memcpy( pWaveInfo->m_pData,m_pBuffer,nSize*sizeof(char));
		}
		else
		{
			LOGE("JCOggParser::GetWaveInfo error 3");
			return NULL;
		}
		pWaveInfo->m_kFmtBlock.wavFormat.dwSamplesPerSec = pVorbisInfo->rate;
        pWaveInfo->m_kFmtBlock.wavFormat.wChannels = pVorbisInfo->channels;
        //TODO 没有找到ogg的这个参数，似乎目前只支持16bit
        pWaveInfo->m_kFmtBlock.wavFormat.wBitsPerSample = 16;
        ov_clear(&pOggStream);
		return pWaveInfo;
	}
	LOGE("JCOggParser::GetWaveInfo error 0");
	return NULL;
}
//------------------------------------------------------------------------------
}
//-----------------------------END FILE--------------------------------
