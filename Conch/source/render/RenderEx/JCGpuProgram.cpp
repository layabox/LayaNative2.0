/**
@file			JCGpuProgram.cpp
@brief			
@author			James
@version		1.0
@date			2017_11_28
*/

#include "JCGpuProgram.h"
#include <util/Log.h>
#include <stdio.h>

namespace laya{

    JCGpuProgramTemplate::JCGpuProgramTemplate(const char* sVS, const char* sPS, std::vector<std::string>& vBindAtrrib)
    {
        setSrc(sVS, sPS);
        m_vAttribBind = vBindAtrrib;
    }
    JCGpuProgramTemplate::~JCGpuProgramTemplate() 
    {
        freeGLResource();
        for (auto& v : m_vShaderCache) 
        {
            delete v.second;
        }
        m_vShaderCache.clear();
    }
    void JCGpuProgramTemplate::setSrc(const char* sVS, const char* sPS)
    {
        if (!sVS || !sPS)return;
        m_sVS = sVS;
        m_sPS = sPS;
    }
    JCGpuProgram* JCGpuProgramTemplate::getInstance(const char* sMacro)
    {
        std::map<std::string, JCGpuProgram*>::iterator iter = m_vShaderCache.find(sMacro);
        if (iter != m_vShaderCache.end()) 
        {
            return (*iter).second;
        }
        JCGpuProgram* pRet = new JCGpuProgram();
        pRet->m_sMacro = sMacro ? sMacro : "";
        m_vShaderCache[sMacro] = pRet;
        pRet->m_pTemplate = this;
        return pRet;
    }
    void JCGpuProgramTemplate::freeGLResource() 
    {
        for (auto& v : m_vShaderCache) 
        {
            (v.second)->freeGLResource();
        }
    }
    JCGpuProgram::JCGpuProgram()
    {
        m_nVS = m_nPS = m_nProgram = 0;
        m_pTemplate = NULL;
        m_pUniformDesc = NULL;
    }
    JCGpuProgram::~JCGpuProgram()
    {
        freeGLResource();
        if (m_pUniformDesc)
        {
            delete (void *)m_pUniformDesc;
        }
    }
    bool JCGpuProgram::checkCompile(int nShader, char* sDesc)
    {
        GLint compiled = 0;
        glGetShaderiv(nShader, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
        {
            GLint infoLen = 0;
            glGetShaderiv(nShader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen)
            {
                char* buf = new char[infoLen];
                if (buf)
                {
                    glGetShaderInfoLog(nShader, infoLen, NULL, buf);
                    LOGE("JCGpuProgram::checkCompile Could not compile shader %s:\n%s\n", sDesc ? sDesc : "", buf);
                    delete[] buf;
                }
                glDeleteShader(nShader);
            }
            return false;
        }
        return true;
    }
    void JCGpuProgram::useProgram()
    {
        if( m_nProgram <= 0 )
        {
            compile();
            if(m_nProgram<=0)return;
        }
        glUseProgram( m_nProgram );
    }
    void JCGpuProgram::freeGLResource()
    {
        if( m_nVS>0 )
        {
            glDeleteShader(m_nVS);
            m_nVS = 0;
        }
        if( m_nPS )
        {
            glDeleteShader( m_nPS);
            m_nPS = 0;
        }
        glDeleteProgram( m_nProgram );
        m_nProgram = 0;
    }
    bool JCGpuProgramTemplate::splitKeyValue(const char* pSems, std::map<std::string, std::string>& vOut) 
    {
        char strVarName[64];
        const char* pCur = pSems;
        char* pDes = strVarName;
        std::string* pSemNameInMap = nullptr;
        while (true) 
        {
            if (*pCur != ',' && *pCur != 0) 
            {
                *pDes++ = *pCur;
            }
            else 
            {
                *pDes = 0;
                if (pSemNameInMap == nullptr)
                    pSemNameInMap = &(vOut[strVarName]);
                else 
                {
                    *pSemNameInMap = strVarName;
                    pSemNameInMap = nullptr;
                }
                pDes = strVarName;
                if (*pCur == 0)
                    break;
            }
            pCur++;
        }
        return true;
    }
    bool JCGpuProgram::compile() 
    {
        int nGLVS, nGLPS;
        nGLVS = glCreateShader(GL_VERTEX_SHADER);
        const char* strShaders[2] = { m_sMacro.c_str(), m_pTemplate->m_sVS.c_str() };
        glShaderSource(nGLVS, 2, strShaders, NULL);
        glCompileShader(nGLVS);
        if (!checkCompile(nGLVS, "VS"))return false;
        nGLPS = glCreateShader(GL_FRAGMENT_SHADER);
        strShaders[1] = m_pTemplate->m_sPS.c_str();
        glShaderSource(nGLPS, 2, strShaders, NULL);
        glCompileShader(nGLPS);
        if (!checkCompile(nGLPS, "PS"))return false;
        int nProgram = glCreateProgram();
        glAttachShader(nProgram, nGLVS);
        glAttachShader(nProgram, nGLPS);

        //bind attribute location
        for (size_t i = 0; i < m_pTemplate->m_vAttribBind.size(); i++)
        {
            glBindAttribLocation(nProgram, i, m_pTemplate->m_vAttribBind[i].c_str());
        }
        glLinkProgram(nProgram);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(nProgram, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) 
        {
            GLint bufLength = 0;
            glGetProgramiv(nProgram, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) 
            {
                char* buf = new char[bufLength];
                if (buf) 
                {
                    glGetProgramInfoLog(nProgram, bufLength, NULL, buf);
                    LOGE("JCGpuProgram::compile Could not link program:\n%s\n", buf);
                    delete[] buf;
                }
            }
            glDeleteProgram(nProgram);
            nProgram = 0;
        }
        m_nVS = nGLVS;
        m_nPS = nGLPS;
        m_nProgram = nProgram;
        return true;
    }
    GLuint JCGpuProgram::getGpuProgram() 
    {
        if (m_nProgram > 0)return m_nProgram;
        compile();
        return m_nProgram;
    }
}

//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
