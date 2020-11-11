/**
@file			JCWaveParser.cpp
@brief			
@author			James
@version		1.0
@date			2014_11_26
*/

#include "JCWaveParser.h"
#pragma warning (disable: 4996)

namespace laya
{
//------------------------------------------------------------------------------
JCWaveParser* JCWaveParser::m_sWaveParser = NULL;
//------------------------------------------------------------------------------
JCWaveParser::JCWaveParser()
{

}
//------------------------------------------------------------------------------
JCWaveParser::~JCWaveParser()
{

}
//------------------------------------------------------------------------------
JCWaveParser* JCWaveParser::GetInstance( void )
{
	if( m_sWaveParser == NULL )
	{
		m_sWaveParser = new JCWaveParser();
	}
	return m_sWaveParser;
}
//-----------------------------------------------------------------------------
void JCWaveParser::DelInstance( void )
{

}
//------------------------------------------------------------------------------
JCWaveInfo* JCWaveParser::GetWaveInfoFromFile( const char* p_sFileName )
{
	JCWaveInfo* pWaveInfo = new JCWaveInfo();
	bool bRet = pWaveInfo->LoadData( p_sFileName );
	if( bRet )
	{
		return pWaveInfo;
	}
	return NULL;
}
//------------------------------------------------------------------------------
JCWaveInfo* JCWaveParser::GetWaveInfoFromBuffer( unsigned char* p_sBuffer, int p_nBufferSize )
{
	JCWaveInfo* pWaveInfo = new JCWaveInfo();
	bool bRet = pWaveInfo->LoadData( p_sBuffer,p_nBufferSize );
	if( bRet )
	{
		return pWaveInfo;
	}
	return NULL;
}
//------------------------------------------------------------------------------
void JCWaveParser::TestWave( const char* p_sFileName,const char* p_sOutFileName )
{
	JCWaveInfo* pInfo = GetWaveInfoFromFile( p_sFileName );
	if( pInfo )
	{
		FILE* fp = fopen( p_sOutFileName,"a+" );
		char sBuf[128];
		fputs( "{", fp );

		for( unsigned int i = 0 ; i < pInfo->m_nRealDataSize; i++ )
		{
			memset( sBuf,0,128 );
			if( i == pInfo->m_nRealDataSize -1 )
			{
				sprintf( sBuf,"%d", pInfo->m_pData[i] );
			}
			else
			{
				sprintf( sBuf,"%d,", pInfo->m_pData[i] );
			}
			fputs( sBuf,fp );
		}
		fputs( "};", fp );
		fclose( fp );
	}
}
}
//-----------------------------END FILE--------------------------------
