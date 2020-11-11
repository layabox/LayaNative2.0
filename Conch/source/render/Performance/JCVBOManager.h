/**
@file			JCVBOMgr.h
@brief			
@author			James
@version		1.0
@date			2016_5_18
*/

#ifndef __JCVBOMgr_H__
#define __JCVBOMgr_H__

#include <stdio.h>
#include <vector>
#if __APPLE__
    #include <OpenGLES/ES3/gl.h>
    #include <OpenGLES/ES3/glext.h>
#else
    #include <GLES3/gl3.h>
#endif

namespace laya
{
    class JCVBOManager
    {
    public:

        JCVBOManager();
    
        int GetVBO();

        void ReleaseVBO();

	    unsigned int createVertexBuffer( int totalSize );

	    void freeGLResource();
    public:

        std::vector<int>    m_vVBO;			//VBO
        int                 m_nVBOSize;		//VBO的大小
        int                 m_nCurVBOId;	//当前VBO的ID
        bool                m_bInited;		//是否初始化

    };
};
//------------------------------------------------------------------------------


#endif //__JCVBOMgr_H__

//-----------------------------END FILE--------------------------------