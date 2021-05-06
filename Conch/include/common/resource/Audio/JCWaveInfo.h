/**
@file			JCWaveInfo.h
@brief			
@author			James
@version		1.0
@date			2014_11_26
*/

#ifndef __JCWaveInfo_H__
#define __JCWaveInfo_H__

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <string>

namespace laya
{

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;
typedef unsigned long U64;
#pragma pack(push,1)

typedef struct _RIFF_HEADER
{
	U8		szRiffID[4]; // 'R','I','F','F'
	U32		dwRiffSize;
	U8		szRiffFormat[4]; // 'W','A','V','E'
}RIFF_HEADER;

typedef struct _WAVE_FORMAT
{
	U16		wFormatTag;
	U16		wChannels;
	U32		dwSamplesPerSec;
	U32		dwAvgBytesPerSec;
	U16		wBlockAlign;
	U16		wBitsPerSample;
	U16		pack;		//附加信息
}WAVE_FORMAT;

typedef struct _FMT_BLOCK
{
	U8			szFmtID[4];	// 'f','m','t',' '
	U32			dwFmtSize;
	WAVE_FORMAT wavFormat;
}FMT_BLOCK;

typedef struct _FACT_BLOCK
{
	U8		szFactID[4]; // 'f','a','c','t'
	U32		dwFactSize;
}FACT_BLOCK;

typedef struct _JUNK_BLOCK
{
	U8		szJUNK[4];
	U32		dwJunkSize;
}JUNK_BLOCK;

typedef struct _DATA_BLOCK
{
	U8		szDataID[4]; // 'd','a','t','a'
	U32		dwDataSize;
}DATA_BLOCK;

#pragma pack(pop)
/** 
 * @brief 
*/
class JCWaveInfo
{
public:

	/** @brief构造函数
	*/
	JCWaveInfo( void );

	/** @brief析构函数
	*/
	~JCWaveInfo( void );


	/** @brief 加载数据
	 *  @param[in]  文件名
	 *  @return true 代表读取成功 false代表失败
	*/
	bool LoadData( const char* p_sFileName );


	/** @brief 加载数据
	 *  @param[in]  buffer数据
	 *  @param[in]  长度
	 *  @return true 代表读取成功 false代表失败
	*/
	bool LoadData( unsigned char* p_sBuffer,int p_nBufferSize );

private:

	unsigned char getC( unsigned char* p_sBuffer, int p_nBufferSize);
	void read( void* pRet, int p_nSize, unsigned char* p_sBuffer );

public:

	char					m_vFackBlockBuffer[20];		//FackBlockBuffer

	RIFF_HEADER				m_kRiff;					//数据头

	FMT_BLOCK				m_kFmtBlock;				//fmt

	DATA_BLOCK				m_kDataBlock;				//数据

	U32						m_nRealDataSize;			//数据长度

	U8*						m_pData;					//数据

	int						m_nCurPos;					//当前位置

	void*					m_pExternalMark;			//外部标记，现在目的是为了存储JSAudio的指针，为了能够回调JS，或者想存储你想存储的数据

    std::string             m_sUrl;                     //url

    double                  m_nTouchTime;               //时间戳

    //FACT_BLOCK				m_kFact;					//fact

    //JUNK_BLOCK				m_kJunkBlock;				//JunkBlock

};
}

#endif //__JCWaveInfo_H__

//-----------------------------END FILE--------------------------------
