/**
@file			JCFontManager.h
@brief			
@author			James
@version		1.0
@date			2016_5_17
*/

#ifndef __JCFontManager_H__
#define __JCFontManager_H__

#include <string>
#include "JCFontInfo.h"
#include <unordered_map>

namespace laya
{
    class JCFontManager
    {
    public:
        typedef std::unordered_map<std::string, JCFontInfo*>	MapFontInfo;
        typedef MapFontInfo::iterator						    MapFontInfoIter;
    public:
        
        JCFontManager();

        ~JCFontManager();

        void clearAllData();
        
        JCFontInfo* getFontInfoFromText(const std::string& sFont);

    public:
        MapFontInfo					m_vMapFontID;
    };
}
//------------------------------------------------------------------------------


#endif //__JCFontManager_H__

//-----------------------------END FILE--------------------------------