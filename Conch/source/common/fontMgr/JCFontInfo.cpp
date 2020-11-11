/**
@file			JCFontInfo.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#include "JCFontInfo.h"
#include <vector>
#include "../util/JCCommonMethod.h"
#include "../util/JCColor.h"
#include "../util/Log.h"

namespace laya
{
JCFontInfo JCFontInfo::__DEFAULT__ ;
int JCFontInfo::ms_nID = 0;
JCFontInfo::JCFontInfo()
{
	m_nFontId = 0;
	m_nStyle = 0;
	m_nWeight = 100;
	m_nFontSize = 12;
	m_nBorderSize = 0;
	m_nBorderColor = 0;
	m_nFamliyLen = 0;
	m_nDecorationLine = 0;
	m_nDecorationColor = 0x00000000;
	strcpy( m_sFamily,"Arial" );
}
JCFontInfo::~JCFontInfo()
{

}
void JCFontInfo::copy(JCFontInfo* p_pOther )
{
	m_nFontId = p_pOther->m_nFontId;
	m_nStyle = p_pOther->m_nStyle;
	m_nWeight = p_pOther->m_nWeight;
	m_nFontSize = p_pOther->m_nFontSize;
	strcpy( m_sFamily,p_pOther->m_sFamily );
	m_nBorderSize = p_pOther->m_nBorderSize;
	m_nBorderColor = p_pOther->m_nBorderColor;
	m_nDecorationLine = p_pOther->m_nDecorationLine;
	m_nDecorationColor = p_pOther->m_nDecorationColor;
}
void JCFontInfo::buildFont( const std::string& p_sText )
{
	m_nFontId = ms_nID++;
	parse( p_sText );
}
/**
 * 顺序是  
 *    font-style：normal|italic|oblique|inherit
 *    font-variant：normal|small-caps|inherit
 *    font-weight：normal|bold|bolder|lighter|100~900|inherit
					400 normal， 700 bold
 *    font-size/line-height：
				 xx-small
				 x-small
				 small
				 medium
				 large
				 x-large
				 xx-large
				 smaller
				 larger
				 length
				 %
				 inherit
		line-height: normal|number|length|%|inherit
*    font-family:用逗号分隔的多个字体，优先使用第一个
*/

//只到xxpx，或者xx%,后面的字体部分自己处理
struct FontToken {
	struct TransCondition {
		const char* pData;
		int len;
		int next;
	};
	enum state {
		init = 0,
		error = 1,
		end = 2,
		normal,
		italic,
		oblique,
		inherit,
		small_caps,
		bold,
		bolder,
		light,
		lighter,
		weightNum,
		small,
		smaller,
		medium,
		large,
		larger,
		xxpx,
		xxPercenct,
		//中间状态
		number,
		family,
		StateNum
	};
	state _nState;
	const char* _pData;
	const char* _pCurData;
	TransCondition* _pTransTable;
	int _nTransTbSt;
	int _nTransLen;
	int*	_pStateStartTable;
	FontToken(const char* pData) {
		_nState = init;
		_pCurData = _pData = pData;
		static TransCondition transtb[] = {
			{ 0,0,number }, 			//init.第一个是给数字留的特殊的
			{ "bold "	,5,	bold },
			{ "italic "	,7,	italic },
			{ "normal "	,7,	normal },
			{ " "		,1,	init },
			{ "oblique "	,8,	oblique },
			{ "inherit "	,8,	inherit },
			{ "small_caps "	,11,small_caps },
			{ "bolder "	,7,	bolder },
			{ "light "	,6,	light },
			{ "lighter "	,8,	lighter },
			{ "small "	,6,	small },
			{ "smaller "	,8,	smaller },
			{ "medium "	,7,	medium },
			{ "large "	,6,	large },
			{ "larger "	,7,	larger },
			{ 0,0,number },			//numStart
			{ "px ",3,xxpx },	//TODO 没有处理 pt 
			{ " ",1,weightNum },
			{ "% ",2,xxPercenct },
			{ 0,0,error },			//end
			{ 0,0,end }/*0是任意*/
		};
		_pTransTable = transtb;
		//每个状态对应的开始位置和长度
		static int tb1[] = {
			0,16,//init
			0,0,//error = 1,
			0,0,//end = 2,
			0,16,//normal ,
			0,16,//italic ,
			0,16,//oblique ,
			0,16,//inherit,
			0,16,//small_caps,
			0,16,//bold ,
			0,16,//bolder ,
			0,16,//light ,
			0,16,//lighter ,
			0,16,//weightNum ,
			20,2,//small
			20,2,//smaller ,
			20,2,//medium,
			20,2,//large ,
			20,2,//larger
			20,2,//xxpx ,
			20,2,//xxPercenct,
			16,4,//number,
				 //family,
		};
		_pStateStartTable = tb1;
		_nTransTbSt = _pStateStartTable[_nState * 2];
		_nTransLen = _pStateStartTable[_nState * 2 + 1];
	}
	inline bool isEndToken(state& st) {
		return st >= end && st <= xxPercenct;
	}
	//p_st是符合这个符号的起点
	//p_len是长度
	state getToken(const char*& p_st, int& p_len) {
		p_st = _pCurData;
		while (*_pCurData != 0 && //还有数据。
			_nState != end && //状态没有结束
			_nState != error && //状态不是错误
			_nTransLen>0) {//len>0表示还能转换
			//获得当前状态的转换表。
			TransCondition* stcond = &_pTransTable[_nTransTbSt];
			int nLastState = _nState;
			const char& dt = *_pCurData;
			if (dt >= '0'&&dt <= '9') {
				_nState = (state)stcond[0].next;
				_pCurData++;
			}
			else {
				int i = 1;
				for (i = 1; i < _nTransLen; i++) {
					//字符串比较
					int l = 0;
					const char* pSrc = _pCurData;
					const char* pDst = stcond[i].pData;
					int& len1 = stcond[i].len;
					if (!pDst) {//任意值。这时候不前进了
						_nState = (state)stcond[i].next;
						_pCurData += len1;
						break;
					}
					for (; *pSrc++ == *pDst++&& l < len1; l++) {}
					if (l < len1) continue;
					//发现一个匹配的
					_pCurData += len1;
					_nState = (state)stcond[i].next;
					break;
				}
				if (i == _nTransLen) {
					//没有匹配的
					LOGE("getToken Error1");
					return error;
				}
			}
			//<number的话，都是终结状态，必须通知出去。
			if (_nState != nLastState || _nState<number) {
				_nTransTbSt = _pStateStartTable[_nState * 2];
				_nTransLen = _pStateStartTable[_nState * 2 + 1];
				//终结状态，可以输出。
				if (isEndToken(_nState)) {
					p_len = _pCurData - p_st;
					return (state)_nState;
				}
			}
		}
		return error;
	}
};

/*
	fontFamilys 所有的字体
	nBorderSize 边的大小
	nBorderColor 边的颜色
	nDecorationLine 下划线位置 (0代表没有 1下划线 2中划线 3上划线)
	nDLColor 下划线的颜色
*/
void parseFontFamilyAndBorder(const char* pData, std::vector<std::string>& fontFamilys, 
		int& nBorderSize, int& nBorderColor, short& nDecorationLine, int& nDLColor) {
	nBorderSize = 0;
	nBorderColor = 0;
	nDecorationLine = 0;
	nDLColor = 0;
	if (!pData)
		return;
	int len = strlen(pData);
	//当前数据，指向字符串的最末尾
	char* pDt =(char*)pData+len;
	char cdt = *(--pDt);

	int nintnum = 0;	//已经拆分出来的个数
	char* sppos[8];		//拆分出来的每个的位置
	int nState = 0;		//0 开始， 1识别出一个 2 遇到除了数字和空格外的其他字符，结束。
	while (pDt!=pData && nState!=2) {
		//只处理空格和数字
		switch (cdt) {
		case ' ':
			if (nState == 0) {
			}
			else {
				nState = 0;
				sppos[nintnum++] = pDt;
			}
			break;
		case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':case '#':
		case 'a':case 'b':case 'c':case 'd':case 'e':case 'f':
		case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':
			if (nState == 0) {
				sppos[nintnum++] = pDt;
				nState = 1;
			}
			else {
			}
			break;
		default:
			nState = 2;
			break;
		}
		if (nintnum >= 8) {
			nState = 2;
			break;
		}
		if (nState == 2)
			break;//不要再--pDt了
		cdt = *(--pDt);
	}
    //if to start of string
    if (pDt == pData) {
        nintnum--;  //
        pDt = sppos[nintnum-1];
        nState = 2;
    }

	if (nState==2) {
		switch (nintnum) {
		case 8: {
			std::string strDLColor(sppos[1] + 1, sppos[0] - sppos[1]);
			nDLColor = JCColor::getColorUintFromString(strDLColor.c_str());
		}
		case 6: {
			//nintnum可能为8或者6，所以不能固定值
			std::string strDL(sppos[nintnum-5] + 1, sppos[nintnum-6] - sppos[nintnum-5]);
			nDecorationLine = stringToInt(strDL.c_str());
		}
		case 4: {
			std::string strBDCorlor(sppos[nintnum-3] + 1, sppos[nintnum-4] - sppos[nintnum-3]);
			nBorderColor = JCColor::getColorUintFromString(strBDCorlor.c_str());
		}
		case 2: {
			std::string strBDSize(sppos[nintnum - 1] + 1, sppos[nintnum - 2] - sppos[nintnum - 1]);
			nBorderSize = stringToInt(strBDSize.c_str());
		}
		case 0:
			break;
		}
        if (pDt[0] != ' ')
        {
            pDt++;
        }
        std::string family(pData, pDt - pData);
		std::vector<char*> families;
		splitString(families, (char*)family.c_str(), ',');
		fontFamilys.resize(families.size());
		for (int i = 0,len= families.size(); i < len; i++)
			fontFamilys[i] = families[i];
	}
}

bool JCFontInfo::parse( const std::string& p_sText )
{
	//init
	m_nStyle = 0;
	m_nFontSize = 16;
	m_nWeight = 400;
	//
	FontToken ft(p_sText.c_str());
	FontToken::state st = FontToken::init;
	while (st != FontToken::error && st != FontToken::end) {
		const char* pSt = NULL;
		int len = 0;
		st = ft.getToken(pSt, len);
		const char* pCur = ft._pCurData;
		switch (st) {
        case FontToken::init:
            break;
        case FontToken::normal:
            break;
        case FontToken::inherit:
            break;
		case FontToken::italic:
			m_nStyle = 1;
			break;
		case FontToken::oblique:
			m_nStyle = 2;
			break;
		case FontToken::bold:
		case FontToken::bolder:
			m_nWeight = 700;
			break;
		case FontToken::xxpx: {
			int intlen = len - 3;
			int px = 0;
			switch (intlen) {
			case 3:
				px += (*pSt++ - '0') * 100; 
			case 2:
				px += (*pSt++ - '0') * 10; 
			case 1:
				px += (*pSt - '0');				break;
			default://其他的都不支持
				px = 16;				break;
			}
			m_nFontSize = px;
		} break;
		case FontToken::xxPercenct: {
			int intlen = len - 2;
			int pct = 0;
			switch (intlen) {
			case 4:
				pct += (*pSt++ - '0') * 1000; 
			case 3:
				pct += (*pSt++ - '0') * 100; 
			case 2:
				pct += (*pSt++ - '0') * 10; 
			case 1:
				pct += (*pSt - '0');
				break;
			default://其他的都不支持
				pct = 100;				break;
			}
			m_nFontSize = 16;//	TODO 
		}break;
		case FontToken::weightNum: {
			int intlen = len - 1;
			int wt = 0;
			switch (intlen) {
			case 3:
				wt = (pSt[0]-'0') * 100; break;
			default://其他的都不支持
				wt = 400;				break;
			}
			m_nWeight = wt;
		}break;
		case FontToken::small:
			m_nFontSize = 13;
			break;
		case FontToken::smaller:
			m_nFontSize = 16;
			break;
		case FontToken::medium:
			m_nFontSize = 16;
			break;
		case FontToken::large:
			m_nFontSize = 19;
			break;
		case FontToken::larger:
			m_nFontSize = 16;//TODO
			break;
		//xx-small 9
		//x - small 11
		//x - large 23
		//xx-large 28
		case FontToken::end: {
			/*
			const char* pFontFamily = pSt;
			std::vector<char*> vStrings;
			SplitString(vStrings, (char*)pFontFamily, ',');
			m_nFamliyLen = (short)strlen(vStrings[0]);
			strcpy(m_sFamily, vStrings[0]);
			*/
			std::vector<std::string> families;
			parseFontFamilyAndBorder(pSt, families, m_nBorderSize, m_nBorderColor, m_nDecorationLine, m_nDecorationColor);
            if ((int)families.size() > 0) {
                m_nFamliyLen = (short)families[0].length();
                strcpy(m_sFamily, families[0].c_str());
            }
            else {
                m_nFamliyLen = 5;
                strcpy(m_sFamily, (const char*)"Arial");
            }
		}break;
		case FontToken::error:
			break;
		}
	}
	//LOGI("JCFontInfo::parse %s,fontinfo:size:%d,family:%s", p_sText.c_str(), m_nFontSize, m_sFamily);
	/*
	std::vector<char*> vStrings;
	SplitString( vStrings,(char*)p_sText.c_str(),' ' );
	int nVectorSize = (int)(vStrings.size());
	if( vStrings.size() < 4 )
	{
        if(vStrings.size()==3)
        {
            int nStyle = 0;
            char* sStyle = vStrings[0];
            if( strcmp( sStyle,"normal" ) == 0 )
            {
                nStyle = 0;
            }
            else if( strcmp( sStyle,"italic" ) == 0 )
            {
                nStyle = 1;
            }
            else if( strcmp( sStyle,"oblique" ) == 0 )
            {
                nStyle = 2;
            }
            else
            {
                nStyle = StringToInt( vStrings[0] );
            }
            int nWeight = 100;//StringToInt( vStrings[1] );
            int nSize = StringToInt( vStrings[1] );
            //TODO中文的问题
            char* sFamily = vStrings[2];
            m_nFontSize = nSize;
            m_nStyle = nStyle;
            m_nWeight = nWeight;
            m_nFamliyLen=strlen(sFamily);
            strcpy( m_sFamily,sFamily);

        }
       

		printf("setFont into number <4 size = %d\n", (int)(vStrings.size()) );
		return false;
	}
	else
	{
		int nStyle = 0;
		char* sStyle = vStrings[0];
		if( strcmp( sStyle,"normal" ) == 0 )
		{
			 nStyle = 0;
		}
		else if( strcmp( sStyle,"italic" ) == 0 )
		{
			 nStyle = 1;
		}
		else if( strcmp( sStyle,"oblique" ) == 0 )
		{
			 nStyle = 2;
		}
		else
		{
			nStyle = StringToInt( vStrings[0] );
		}
		int nWeight = StringToInt( vStrings[1] );
		int nSize = StringToInt( vStrings[2] );
		//TODO中文的问题
		char* sFamily = vStrings[3];
		m_nFontSize = nSize;
		m_nStyle = nStyle;
		m_nWeight = nWeight;
		m_nFamliyLen=strlen(sFamily);
		strcpy( m_sFamily,sFamily);
		int nBorderSize = 0;
		int nBorderColor = 0xff000000;
		if( vStrings.size() > 4 )
		{
			nBorderSize = StringToInt( vStrings[4] );
		}
		if( vStrings.size() > 5 )
		{
			nBorderColor = Color::getColorUintFromString( vStrings[5] );
		}
		m_nBorderSize = nBorderSize;
		m_nBorderColor = nBorderColor;
		if( vStrings.size() > 7 )
		{
			m_nDecorationLine = StringToInt( vStrings[6] );
			m_nDecorationColor = Color::getColorUintFromString( vStrings[7] );
		}
		return true;
	}
	*/
	return true;
}
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
