/**
@file			JCIOSFreeType.cpp
@brief			
@author			James
@version		1.0
@date			2016_6_11
*/

#include "JCIOSFreeType.h"
#ifdef __APPLE__
    #include "../CToObjectC.h"
#endif
#include <util/Log.h>

extern std::string gRedistPath;
extern std::string gResourcePath;
namespace laya
{
    
    JCIOSFreeType::JCIOSFreeType()
    {
        
    }
    
    char* JCIOSFreeType::getFontBuffer(long& buffSize)
    {
        return CToObjectCGetFontBuffer(buffSize);
    }
    
    std::string JCIOSFreeType::writeIOSFontTTF()
    {
        long nBufferSize = 0;
        char* sBuffer = getFontBuffer( nBufferSize );
        LOGE("wirteIOSFontTTF size=%d",(int)nBufferSize);
        std::string sPath = gRedistPath + "ios.ttf";
        long nWriteSize = 0;
        if( sBuffer != NULL && nBufferSize > 0 )
        {
            FILE* fp = fopen(sPath.c_str(),"wb");
            if( fp )
            {
                nWriteSize = (long)( fwrite(sBuffer, 1, nBufferSize, fp ) );
                fclose(fp);
            }
            delete sBuffer;
            if( nBufferSize == nWriteSize )
            {
                return sPath;
            }
            else
            {
                return "";
            }
        }
        return "";
    }
    std::string JCIOSFreeType::getIOSFontTTFPath()
    {
        std::string sPath = gRedistPath + "ios.ttf";
        return sPath;
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------