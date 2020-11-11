/**
@file			JCGpuProgram.h
@brief			
@author			James
@version		1.0
@date			2017_11_28
*/

#ifndef __JCGpuProgram_H__
#define __JCGpuProgram_H__

#if __APPLE__
    #include <OpenGLES/ES3/gl.h>
    #include <OpenGLES/ES3/glext.h>
#else
    #include <GLES3/gl3.h>
#endif
#include <string>
#include <map>
#include <vector>

namespace laya
{
	class JCNamedData;
    class JCGpuProgram;
    class JCGpuProgramTemplate 
    {
    public:

        JCGpuProgramTemplate(const char* sVS, const char* sPS, std::vector<std::string>& vBindAtrrib);

        ~JCGpuProgramTemplate();

        void setSrc(const char* sVS, const char* sPS);
        
        JCGpuProgram* getInstance(const char* sMacro);

        void freeGLResource();

        bool splitKeyValue(const char* sBuffer, std::map<std::string, std::string>& vOut);

    public:
        std::string                             m_sVS;
        std::string                             m_sPS;
        std::map<std::string, JCGpuProgram*>    m_vShaderCache;
        std::vector<std::string>                m_vAttribBind;
    };

    /*
        gpuProgram 是一种资源，为了恢复，需要保留源代码或者对应的文件
    */
    class JCGpuProgram
    {
    public:

        JCGpuProgram();

        ~JCGpuProgram();

        bool checkCompile(int nShader, char* sDesc);
        
        void freeGLResource();

        void useProgram();

        GLuint getGpuProgram();
        
        bool compile();

    public:
        int                     m_nVS;
        int                     m_nPS;
        JCGpuProgramTemplate*   m_pTemplate;
        JCNamedData*            m_pUniformDesc;
        std::string             m_sMacro;
    private:
        GLuint                  m_nProgram;
    };
}

//------------------------------------------------------------------------------


#endif //__JCGpuProgram_H__

//-----------------------------END FILE--------------------------------