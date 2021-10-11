/**
@file			JSLayaGL.h
@brief			
@author			James
@version		1.0
@date			2018_5_25
*/

#ifndef __JSLayaGL_H__
#define __JSLayaGL_H__

#include "../JSInterface/JSInterface.h"
#include <RenderEx/JCGlobalValue.h>
#include <RenderEx/JCGpuProgram.h>
#include <manager/JCArrayBufferManager.h>

/** 
 * @brief 
*/
namespace laya 
{
    struct WebGLActiveInfo
    {
        const static GLsizei  bufsize = 255;
        GLsizei  length;
        GLint    size;
        GLenum   type;
        GLchar   name[bufsize];
    };
    struct ShaderPrecisionFormat
    {
        int range[2];
        int precision[1];
    };
    class JSLayaGL:public JsObjBase, public JSObjNode
    {
    public:

	    static JsObjClassInfo JSCLSINFO;

        void exportJS();

        static JSLayaGL* getInstance();

        JSLayaGL();

        ~JSLayaGL();

        void setSyncArrayBufferID(int nSyncArrayBufferID);

        void setFrameAndSyncCountArrayBufferID(int nBufferID);

        void setRootCommandEncoder( int nCommandEncoderID );

    public:
    
        int getAttribLocationEx(const char* vs, const char* ps, const char* define,const char* sName);

        JsValue getShaderPrecisionFormat(int shaderType, int precisionType);

		JsValue getUniformEx(const char* locationName);

        int getParameter(int pname);

		bool getBooleanv(int pname);

		int getIntegerv(int pname);

		JsValue getIntegerArrayv(int pname);

		float getFloatv(int pname);

		JsValue getFloatArrayv(int pname);

		JsValue readPixels(int x, int y, int width, int height, int format, int type);

        void setCurrentContext(int nContextID);

        int getThreadMode();

    public:

        int getProgramParameterEx(const char* vs, const char* ps, const char* define, int type);

        JsValue getActiveAttribEx(const char* vs, const char* ps, const char* define, int nIndex);

        JsValue getActiveUniformEx(const char* vs, const char* ps, const char* define, int nIndex);

        const char* getStringEx(unsigned int name);

        const char* getShaderInfoLogEx(const char* source,int type);

        const char* getProgramInfoLogEx(const char* vs, const char* ps, const char* define);

        //int getBufferParameter(int target, int pname);

		int getFramebufferAttachmentParameter(int target, int attachement, int pname);

		//int getRenderbufferParameter(int target, int pname);

		//int getTexParameter(int target, int pname);

		int getShaderParameter(const char* src, int type,int pname);

        int checkFramebufferStatusEx(int target);

        int getVertexAttribEx(int index, int target);

		JsValue getVertexAttribExfv(int index, int target);

		int getVertexAttribOffset(int index, int target);

        int getBufferParameterEx(int target, int pname);

        int getRenderbufferParameterEx(int target, int pname);

        int getTexParameterEx(int target, int pname);

		int isEnabled(unsigned int cap);

		void flushCommand();

		void setMainContextSize(int width, int height);
    private:

        void _getStringEx(unsigned int name);

        void _getProgramParameterEx(const std::string& vs, const std::string& ps, const std::string& define, int type);

        void _getActiveAttribEx(const std::string& vs, const std::string& ps, const std::string& define, int nIndex);

        void _getActiveUniformEx(const std::string& vs, const std::string& ps, const std::string& define, int nIndex);

        void _getAttribLocationEx(const std::string& vs, const std::string& ps, const std::string& define, const std::string& name);

        void _getShaderPrecisionFormat(int shaderType, int precisionType);

		void _getUniformEx(const std::string& locationName);

        void _getParameter(int pname);

		void _getBooleanv(int pname);

		void _getIntegerv(int pname);

		void _getIntegerArrayv(int pname);

		void _getFloatv(int pname);

		void _getFloatArrayv(int pname);

        void _createShader(const std::string& vs, const std::string& ps, const std::string& define);
        
        void _getShaderInfoLog(const std::string& source,int type);

        void _getProgramInfoLog(const std::string& vs, const std::string& ps, const std::string& define);

        //void _getBufferParameter(int target, int pname);

		void _getFramebufferAttachmentParameter(int target, int attachement, int pname);

		//void _getRenderbufferParameter(int target, int pname);

		//void _getTexParameter(int target, int pname);

		void _getShaderParameter(const std::string& source, int type, int pname);

		void _readPixels(int x, int y, int width, int height, int format, int type);

    public:
        static JSLayaGL*                            s_pLayaGL;
        JCGlobalValue*                              m_pValueTemplate;
        int                                         m_nSyncToRenderABListID;
        int                                         m_nFrameAndSyncCountABListID;
        int                                         m_nRootCommandEncoderID;
        JCArrayBufferManager::ArrayBufferContent*   m_pRootCommandEncoder;
        int                                         m_nFrameCount;
        int                                         m_nSyncArrayBufferCount;
        int                                         m_nParameterResult;
		float							            m_nParameterResultFloat;
		GLboolean						            m_nParameterResultBool;
		std::vector<float>				            m_nParameterResultArray;
		std::vector<uint8_t>				        m_nParameterResultByteArray;
    private:
        int                                         m_nAttribLocation;
        int                                         m_nProgramParameter;
        ShaderPrecisionFormat*                      m_pShaderPrecisionFormat;
        WebGLActiveInfo*                            m_pShaderActiveInfo;
        JCGpuProgramTemplate*                       m_pShaderTemplate;
        JCGpuProgram*                               m_pGpuProgram;
        std::string                                 m_sShaderVS;
        std::string                                 m_sShaderPS;
        std::string                                 m_sShaderDefine;
        std::string                                 m_sErrorInfo;
        std::string                                 m_sGLString;
    };
}
//------------------------------------------------------------------------------


#endif //__JSLayaGL_H__

//-----------------------------END FILE--------------------------------