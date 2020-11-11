
/**
@file			JCConchBridge.h
@brief
@author			James
@version		1.0
@date			2016_5_27
*/

#ifndef __JCConchBridge_H__
#define __JCConchBridge_H__

namespace laya
{
    /**
    * @brief
    */
    class JCConchBridge
    {
    public:
        static void getPixelsRenderToJS(unsigned char * pPixels, int nSize, int w, int h, int callbackObjID, int funcID);
    };
}
//------------------------------------------------------------------------------


#endif //__JCConchBridge_H__

 //-----------------------------END FILE--------------------------------