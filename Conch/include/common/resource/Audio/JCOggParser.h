/**
@file			JCOggParser.h
@brief			
@author			James
@version		1.0
@date			2014_11_26
*/

#ifndef __JCOggParser_H__
#define __JCOggParser_H__

#include "JCWaveInfo.h"


/** 
 * @brief 读取wav文件  目前只支持
 * 单声道
 * 16位
 * 采样级别 8000
 * 音频格式 PCM
*/
namespace laya
{
class JCOggParser
{
public:

	/** @brief构造函数
	*/
	JCOggParser( void );

	/** @brief析构函数
	*/
	~JCOggParser( void );

	/*
	 * getInstace()
	*/
	static JCOggParser* GetInstance( void );


	static void DelInstance( void );

	/*
	 * 从文件中读取wave数据 
	*/
	JCWaveInfo* GetWaveInfo( const char* p_sFileName,unsigned char* p_sBuffer, int p_nBufferSize );

	//拷贝buffer
	bool copyBuffer( char* p_sBuffer,int p_nSize );

protected:

	static	JCOggParser*			ms_pOggParser;		//静态的this指针

	char*							m_pBuffer;			//临时的buffer

	int								m_nBufferSize;		//buffer大小

	int								m_nCurBufferPos;	//当前buffer的位置

};
}

#endif //__JCOggParser_H__

//-----------------------------END FILE--------------------------------