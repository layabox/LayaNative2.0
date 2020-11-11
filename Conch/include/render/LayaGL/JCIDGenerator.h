/**
@file			JCIDGenerator.h
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#ifndef __JCIDGenerator_H__
#define __JCIDGenerator_H__

#include <string>
#include "util/Log.h"
#include <vector>
#if __APPLE__
    #include <OpenGLES/ES3/gl.h>
#else
    #include <GLES3/gl3.h>
#endif

namespace laya
{
    class JCIDGenerator
    {
    public:

        JCIDGenerator();

        ~JCIDGenerator();
        
        bool deleteID(GLuint nFakeID);

        bool setRealID(GLuint fakeID,GLuint realID);

        GLuint getRealID(GLuint fakeID)
        {
            return fakeID < m_vIDTable.size() ? m_vIDTable[fakeID] : 0;
        }

        void reset();

    protected:

        std::vector<GLuint> m_vIDTable;

    };
}
//------------------------------------------------------------------------------


#endif //__JCIDGenerator_H__

//-----------------------------END FILE--------------------------------