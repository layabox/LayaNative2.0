/**
@file			JCFontInfo.h
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#ifndef __JCFontInfo_H__
#define __JCFontInfo_H__

#pragma warning(disable:4996)
#include <string>

namespace laya
{
	class JCFontInfo
	{
	public:
        JCFontInfo();
		~JCFontInfo();
		void copy(JCFontInfo* p_pOther );
		void buildFont( const std::string& p_sText );
		bool parse( const std::string& p_sText );
	public:
		static JCFontInfo   __DEFAULT__;
		static int			ms_nID;
		int					m_nFontId;					//id,JS传入的
		short				m_nStyle;					//normal	默认值。浏览器显示一个标准的字体样式。 italic	浏览器会显示一个斜体的字体样式。 oblique	浏览器会显示一个倾斜的字体样式
		short				m_nWeight;					//粗体的宽度
		short				m_nFontSize;				//字体大小
		int					m_nBorderSize;				//描边大小
		int					m_nBorderColor;				//描边颜色
		short				m_nDecorationLine;			//下划线 中划线 上划线
		int					m_nDecorationColor;			//线的颜色
		short				m_nFamliyLen;
		char				m_sFamily[64];
	};
}
//------------------------------------------------------------------------------


#endif //__JCFontInfo_H__

//-----------------------------END FILE--------------------------------

