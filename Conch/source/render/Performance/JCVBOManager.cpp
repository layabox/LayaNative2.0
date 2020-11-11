/**
@file			JCVBOMgr.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_18
*/

#include "JCVBOManager.h"

namespace laya
{
    JCVBOManager::JCVBOManager()
    {
        m_nVBOSize = 10240;
        m_nCurVBOId = 0;
        m_vVBO.resize(20);
        m_bInited = false;
    }
    int JCVBOManager::GetVBO()
    {
        if (!m_bInited)
        {
            int nSize = m_vVBO.size();
            for (int i = 0; i < nSize; i++)
            {
                m_vVBO[i] = createVertexBuffer(m_nVBOSize);
            }
            m_bInited = true;
        }
        int vbo = m_vVBO[m_nCurVBOId];
        m_nCurVBOId++;
        if (m_vVBO.size())
        {
            m_nCurVBOId %= m_vVBO.size();
        }
        else
        {
            m_nCurVBOId = 0;
        }
        return vbo;
    }
    void JCVBOManager::freeGLResource()
    {
        int nSize = m_vVBO.size();
        glDeleteBuffers(nSize, (const GLuint*)&m_vVBO[0]);
        m_bInited = false;
    }
    void JCVBOManager::ReleaseVBO()
    {
        m_bInited = false;
        m_nCurVBOId = 0;
    }
    unsigned int JCVBOManager::createVertexBuffer( int totalSize )
    {
	    unsigned int vbo = 0;
	    glGenBuffers(1, &vbo);
	    glBindBuffer(GL_ARRAY_BUFFER, vbo);
	    glBufferData(GL_ARRAY_BUFFER,totalSize,0,GL_DYNAMIC_DRAW);
	    return vbo;
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------