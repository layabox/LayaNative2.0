/**
@file			JCWaveParser.h
@brief			
@author			James
@version		1.0
@date			2014_11_26
*/
#ifndef __JCWaveParser_H__
#define __JCWaveParser_H__

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
class JCWaveParser
{
public:

	/** @brief构造函数
	*/
	JCWaveParser( void );

	/** @brief析构函数
	*/
	~JCWaveParser( void );

	/*
	 * getInstace()
	*/
	static JCWaveParser* GetInstance( void );


	static void DelInstance( void );

	/*
	 * 从文件中读取wave数据 
	*/
	JCWaveInfo* GetWaveInfoFromFile( const char* p_sFileName );

	/*
	 * 从数据中读取wave
	*/
	JCWaveInfo* GetWaveInfoFromBuffer( unsigned char* p_sBuffer, int p_nBufferSize );


	/** 
	 * @brief 测试用的
	*/
	void TestWave( const char* p_sFileName,const char* p_sOutFileName );

protected:

	static	JCWaveParser*			m_sWaveParser;			//静态的this指针

};
}

#endif //__JCWaveParser_H__

//-----------------------------END FILE--------------------------------