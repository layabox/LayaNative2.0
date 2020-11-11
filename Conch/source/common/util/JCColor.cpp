/**
@file			JCColor.cpp
@brief			
@author			James
@version		1.0
@date			2013_12_19
*/

#include <string.h>
#include "JCColor.h"
#pragma warning (disable: 4996)

namespace laya
{
//------------------------------------------------------------------------------
JCColorInt::JCColorInt( unsigned char p_nR,unsigned char p_nG,unsigned char p_nB,unsigned char p_nA )
{
	setARGB( p_nR,p_nG,p_nB,p_nA );	
}
//------------------------------------------------------------------------------
void JCColorInt::setColor( unsigned int p_nColor )
{
	color = p_nColor;
	b=(color&255);
	g=((color>>8)&255);
	r=((color>>16)&255);
	a=((color>>24)&255);
}
//------------------------------------------------------------------------------
void JCColorInt::setARGB( unsigned char p_nR,unsigned char p_nG,unsigned char p_nB,unsigned char p_nA )
{
	r = p_nR;
	g = p_nG;
	b = p_nB;
	a = p_nA;
	color = (a<<24) + (r<<16) + (g<<8) + b;
}
//------------------------------------------------------------------------------
void JCColorInt::setColorFromString( const char* p_sColor )
{
	unsigned int nColor = JCColor::getColorUintFromString( p_sColor );
	setColor( nColor );
}
//------------------------------------------------------------------------------
JCColorFloat::JCColorFloat( float p_nR,float p_nG,float p_nB,float p_nA )
{
	setARGB( p_nR,p_nG,p_nB,p_nA );	
}
//------------------------------------------------------------------------------
void JCColorFloat::setColor( unsigned int p_nColor )
{
	color = p_nColor;
	b=(color&255)/255.0f;
	g=((color>>8)&255)/255.0f;
	r=((color>>16)&255)/255.0f;
	a=((color>>24)&255)/255.0f;
}
//------------------------------------------------------------------------------
void JCColorFloat::setARGB( float p_nR,float p_nG,float p_nB,float p_nA )
{
	r = p_nR;
	g = p_nG;
	b = p_nB;
	a = p_nA;
	color = ( (int)(a*255)<<24 ) + ( (int)(r*255)<<16 ) + ( (int)(g*255)<<8 ) + ( (int)(b*255) );
}
//------------------------------------------------------------------------------
void JCColorFloat::setColorFromString( const char* p_sColor )
{
	unsigned int nColor = JCColor::getColorUintFromString( p_sColor );
	setColor( nColor );
}
//------------------------------------------------------------------------------
unsigned int JCColor::hexstrtoint( const char* str )
{
	char szValue[32]={0};
	szValue[0]='0';
	szValue[1]='x';
	strcpy( &szValue[2],str );
	unsigned int nValude = 0;         
	sscanf(szValue,"%x",&nValude);   
	return nValude;
    /*
    int len = strlen(str);
    if (len > 8)
        return 0;
    unsigned char* pCur = (unsigned char*)(str + len - 1);
    unsigned char c = *pCur;
    int ret = 0;
    int off = 0;
    while ((long)pCur >= (long)str) {
        if (c != ' ') {
            if (c >= '0'&&c <= '9') c = c - '0';
            else if (c >= 'a'&&c <= 'f') c = c - 'a' + 10;
            else if (c >= 'A'&&c <= 'F') c = c - 'A' + 10;
            else return 0;
            ret += (c << off);
            off += 4;
        }
        c = *(--pCur);
    }
    return ret;
    */
}
//------------------------------------------------------------------------------
int JCColor::getColorUintFromString( const char* p_sColor )
{
	const char* pData = strchr( p_sColor,'#' );
	if( pData )
	{
		int nLen = strlen( pData );
		bool bNoAlpha = nLen < 9;
		unsigned int color= hexstrtoint( pData+1 );
		if( bNoAlpha ) color |= 0xff000000;
		return color;
	}
	else
	{
		//TODO
	}
	return 0;
}
//------------------------------------------------------------------------------
void JCColor::getColorRGBIntFromString( const char* p_sColor,JCColorInt& p_pOutColorInit )
{
	unsigned int nColor = getColorUintFromString( p_sColor );
	p_pOutColorInit.setColor( nColor );
}
//------------------------------------------------------------------------------
void JCColor::getColorRGBIntFromInt( int p_nColor,JCColorInt& p_pOutColorInit )
{
	p_pOutColorInit.setColor( p_nColor );
}
//------------------------------------------------------------------------------
void JCColor::getColorRGBFloatFromString( const char* p_sColor,JCColorFloat& p_pOutColorFloat )
{
	unsigned int nColor = getColorUintFromString( p_sColor );
	p_pOutColorFloat.setColor( nColor );
}
//------------------------------------------------------------------------------
void JCColor::getColorRGBFloatFromInt( int p_nColor,JCColorFloat& p_pOutColorFloat )
{
	p_pOutColorFloat.setColor( p_nColor );
}
//------------------------------------------------------------------------------
}
//------------------------------------------------------------------------------

//-----------------------------END FILE--------------------------------
