/**
@file			JCColor.h
@brief			
@author			James
@version		1.0
@date			2013_12_19
*/

#ifndef __JCColor_H__
#define __JCColor_H__

#include <stdio.h>
#include <vector>

#define EXTRACT_COLOR_R(color) ((color & 0xff) / 255.0f)
#define EXTRACT_COLOR_G(color) (((color >> 8) & 0xff) / 255.0f)
#define EXTRACT_COLOR_B(color) (((color >> 16) & 0xff) / 255.0f)
#define EXTRACT_COLOR_A(color) (((color >> 24) & 0xff) / 255.0f)

namespace laya
{
//------------------------------------------------------------------------------
/** 
 * @brief  
*/
class JCColorInt
{
public:

	JCColorInt( unsigned char p_nR=0,unsigned char p_nG=0,unsigned char p_nB=0,unsigned char p_nA=0 );

	void setColor( unsigned int p_nColor );

	void setARGB( unsigned char p_nR,unsigned char p_nG,unsigned char p_nB,unsigned char p_nA );

	void setColorFromString( const char* p_sColor );

public:

	unsigned int		color;
	unsigned char		r;
	unsigned char		g;
	unsigned char		b;
	unsigned char		a;
};
//------------------------------------------------------------------------------
/** 
 * @brief 
*/
class JCColorFloat
{
public:

	JCColorFloat( float p_nR=0.0f,float p_nG=0.0f,float p_nB=0.0f,float p_nA=0.0f );

	void setColor( unsigned int p_nColor );

	void setARGB( float p_nR,float p_nG,float p_nB,float p_nA );

	void setColorFromString( const char* p_sColor );

public:

	unsigned int	color;
	float			r;
	float			g;
	float			b;
	float			a;

};
//------------------------------------------------------------------------------
/** 
 * @brief 
*/
class JCColor
{
public:

	//str没有0x，直接是数字，例如"a0"
	static unsigned int hexstrtoint(const char* str);

	static int getColorUintFromString( const char* p_sColor );

	static void getColorRGBIntFromString( const char* p_sColor,JCColorInt& p_pOutColorInit );

	static void getColorRGBIntFromInt( int p_nColor,JCColorInt& p_pOutColorInit );
	
	static void getColorRGBFloatFromString( const char* p_sColor,JCColorFloat& p_pOutColorFloat );

	static void getColorRGBFloatFromInt( int p_nColor,JCColorFloat& p_pOutColorFloat );

};
//------------------------------------------------------------------------------
}

#endif //__JCColor_H__

//-----------------------------END FILE--------------------------------