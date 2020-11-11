/**
@file			JCFontManager.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_17
*/

#include "JCFontManager.h"

namespace laya
{	
    JCFontManager::JCFontManager()
    {
    }
    JCFontManager::~JCFontManager()
    {
        clearAllData();
    }
    void JCFontManager::clearAllData()
    {
        for (MapFontInfoIter iter = m_vMapFontID.begin(); iter != m_vMapFontID.end(); iter++)
        {
            JCFontInfo* pFontInfo = iter->second;
            if (pFontInfo != NULL)
            {
                delete pFontInfo;
                pFontInfo = NULL;
            }
        }
        m_vMapFontID.clear();
    }
    JCFontInfo* JCFontManager::getFontInfoFromText( const std::string& sFontText )
    {
	    MapFontInfoIter iter = m_vMapFontID.find(sFontText);
	    if( iter != m_vMapFontID.end() )
	    {
		    return iter->second;
	    }
	    else
	    {
		    //这样先复制，看着特别傻，但是我也没有办法，因为不知道为什么ANDROID就是不行
            std::string sTempBuffer = sFontText;
		    JCFontInfo* pFontInfo = new JCFontInfo();
		    pFontInfo->buildFont(sFontText);
		    m_vMapFontID[ sTempBuffer ] = pFontInfo;
		    return pFontInfo;
	    }
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
