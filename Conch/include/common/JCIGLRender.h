/**
@file			JCIGLRender.h
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#ifndef __JCIGLRender_H__
#define __JCIGLRender_H__

namespace laya
{
    class JCIGLRender
    {
    public:
        virtual void onGLReady() = 0;	                                        //环境准备好了,可以做一些准备工作
        virtual int renderFrame( long nCurrentFrame, bool bStopRender ) = 0;   //渲染
    };
}
//------------------------------------------------------------------------------


#endif //__JCIGLRender_H__

//-----------------------------END FILE--------------------------------