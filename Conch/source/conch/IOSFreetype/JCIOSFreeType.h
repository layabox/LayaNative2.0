/**
@file			JCIOSFreeType.h
@brief			
@author			James
@version		1.0
@date			2016_6_11
*/

#ifndef __JCIOSFreeType_H__
#define __JCIOSFreeType_H__

#include <JCIOSFTInterface.h>
#include <string>

namespace laya
{
    class JCIOSFreeType : public JCIOSFTInterface
    {
    public:

        JCIOSFreeType();
        
        char* getFontBuffer(long& buffSize);
        
        std::string getIOSFontTTFPath();
        
        std::string writeIOSFontTTF();
        
    };
}

//------------------------------------------------------------------------------


#endif //__JCIOSFreeType_H__

//-----------------------------END FILE--------------------------------