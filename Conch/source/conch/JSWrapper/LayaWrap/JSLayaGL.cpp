/**
@file			JSLayaGL.cpp
@brief			
@author			James
@version		1.0
@date			2018_5_25
*/

#include "JSLayaGL.h"
#include "../../JCScriptRuntime.h"
#include "../../JCConchRender.h"
#include "../../JCConch.h"
#include <math/Matrix32.h>
#include "JSShaderActiveInfo.h"
#include "JSShaderPrecisionFormat.h"
#include "../../JCSystemConfig.h"
#include <LayaGL/JCLayaGLDispatch.h>
#include <set>
#include <webglplus/JCWebGLPlus.h>
#include "JSArrayBufferRef.h"
//#include "JSKeyframeNodeList.h"

extern int g_nInnerWidth;
extern int g_nInnerHeight;
//------------------------------------------------------------------------------
namespace laya 
{
    JSLayaGL*   JSLayaGL::s_pLayaGL = NULL;
    ADDJSCLSINFO(JSLayaGL, JSObjNode);
    JSLayaGL::JSLayaGL()
    {
        AdjustAmountOfExternalAllocatedMemory(8192);
        JCMemorySurvey::GetInstance()->newClass("layagl", 8192, this);
        m_nSyncToRenderABListID = -1;
        m_nFrameAndSyncCountABListID = -1;
        m_nRootCommandEncoderID = -1;
        m_pValueTemplate = new JCGlobalValue();
        m_nProgramParameter = 0;
        m_nAttribLocation = 0;
        m_pShaderActiveInfo = new WebGLActiveInfo();
        m_pShaderPrecisionFormat = new ShaderPrecisionFormat();
        m_pShaderTemplate = NULL;
        m_pGpuProgram = NULL;
        m_nSyncArrayBufferCount = 0;
        m_nFrameCount = 0;
        m_nParameterResult = 0;
        m_pRootCommandEncoder = NULL;
    }
    JSLayaGL::~JSLayaGL()
    {
        if (m_pValueTemplate)
        {
            delete m_pValueTemplate;
            m_pValueTemplate = NULL;
        }
        if (m_pShaderActiveInfo)
        {
            delete m_pShaderActiveInfo;
            m_pShaderActiveInfo = NULL;
        }
        if (m_pShaderTemplate)
        {
            delete m_pShaderTemplate;
            m_pShaderTemplate = NULL;
            m_pGpuProgram = NULL;
        }
        if (m_pShaderPrecisionFormat)
        {
            delete m_pShaderPrecisionFormat;
            m_pShaderPrecisionFormat = NULL;
        }
        JCMemorySurvey::GetInstance()->releaseClass("layagl", this);
        s_pLayaGL = NULL;
    }
    JSLayaGL* JSLayaGL::getInstance()
    {
        if (s_pLayaGL == NULL)
        {
            s_pLayaGL = new JSLayaGL();
        }
        return s_pLayaGL;
    }
    void JSLayaGL::setSyncArrayBufferID(int nSyncArrayBufferID)
    {
        m_nSyncToRenderABListID = nSyncArrayBufferID;
    }
    void JSLayaGL::setFrameAndSyncCountArrayBufferID(int nBufferID)
    {
        m_nFrameAndSyncCountABListID = nBufferID;
    }
    void JSLayaGL::setRootCommandEncoder(int nCommandEncoderID)
    {
        m_nRootCommandEncoderID = nCommandEncoderID;
        m_pRootCommandEncoder = JCScriptRuntime::s_JSRT->m_pArrayBufferManager->getArrayBuffer(m_nRootCommandEncoderID);
    }
    int JSLayaGL::getThreadMode()
    {
        return (int)g_kSystemConfig.m_nThreadMODE;
    }
    int JSLayaGL::getProgramParameterEx(const char* vs, const char* ps, const char* define, int type)
    {
		std::string str_vs = vs;
		std::string str_ps = ps;
		std::string str_define = define;
        JCConch::s_pConchRender->setInterruptFunc(std::bind(&JSLayaGL::_getProgramParameterEx, this, str_vs, str_ps, str_define, type));
        return m_nProgramParameter;
    }
    JsValue JSLayaGL::getActiveAttribEx(const char* vs, const char* ps, const char* define, int nIndex)
    {
		std::string str_vs = vs;
		std::string str_ps = ps;
		std::string str_define = define;
        JCConch::s_pConchRender->setInterruptFunc(std::bind(&JSLayaGL::_getActiveAttribEx, this, str_vs, str_ps, str_define, nIndex));
        JSShaderActiveInfo * pActiveInfo = new JSShaderActiveInfo();
        pActiveInfo->m_sName = m_pShaderActiveInfo->name;
        pActiveInfo->m_nType = m_pShaderActiveInfo->type;
        pActiveInfo->m_nSize = m_pShaderActiveInfo->size;
        return JSP_TO_JS(JSShaderActiveInfo*, pActiveInfo);
    }
    JsValue JSLayaGL::getActiveUniformEx(const char* vs, const char* ps, const char* define, int nIndex)
    {
		std::string str_vs = vs;
		std::string str_ps = ps;
		std::string str_define = define;
        JCConch::s_pConchRender->setInterruptFunc(std::bind(&JSLayaGL::_getActiveUniformEx, this, str_vs, str_ps, str_define, nIndex));
        JSShaderActiveInfo * pActiveInfo = new JSShaderActiveInfo();
        pActiveInfo->m_sName = m_pShaderActiveInfo->name;
        pActiveInfo->m_nType = m_pShaderActiveInfo->type;
        pActiveInfo->m_nSize = m_pShaderActiveInfo->size;
        return JSP_TO_JS(JSShaderActiveInfo*, pActiveInfo);
    }
    JsValue JSLayaGL::getShaderPrecisionFormat(int shaderType, int precisionType)
    {
        JCConch::s_pConchRender->setInterruptFunc(std::bind(&JSLayaGL::_getShaderPrecisionFormat, this, shaderType, precisionType));
        JSShaderPrecisionFormat* pShaderPrecision = new JSShaderPrecisionFormat();
        pShaderPrecision->m_nPrecision = m_pShaderPrecisionFormat->precision[0];
        pShaderPrecision->m_nRangeMin = m_pShaderPrecisionFormat->range[0];
        pShaderPrecision->m_nRangeMax = m_pShaderPrecisionFormat->range[1];
        return JSP_TO_JS(JSShaderPrecisionFormat*, pShaderPrecision);
    }

	JsValue JSLayaGL::getUniformEx(const char* locationName)
	{
		std::string strLocName = locationName;

		JCConch::s_pConchRender->setInterruptFunc(std::bind(&JSLayaGL::_getUniformEx, this, strLocName));
		return __JsArray<float>::ToJsArray(m_nParameterResultArray);
	}

    int JSLayaGL::getParameter(int pname)
    {
        JCConch::s_pConchRender->setInterruptFunc(std::bind(&JSLayaGL::_getParameter, this, pname));
        return m_nParameterResult;
    }

	bool JSLayaGL::getBooleanv(int pname)
	{
		JCConch::s_pConchRender->setInterruptFunc(std::bind(&JSLayaGL::_getBooleanv, this, pname));
		return m_nParameterResultBool;
	}

	int JSLayaGL::getIntegerv(int pname)
	{
		JCConch::s_pConchRender->setInterruptFunc(std::bind(&JSLayaGL::_getIntegerv, this, pname));
		return m_nParameterResult;
	}

	JsValue JSLayaGL::getIntegerArrayv(int pname)
	{
		JCConch::s_pConchRender->setInterruptFunc(std::bind(&JSLayaGL::_getIntegerArrayv, this, pname));
		return __JsArray<float>::ToJsArray(m_nParameterResultArray);
	}


	float JSLayaGL::getFloatv(int pname)
	{
		JCConch::s_pConchRender->setInterruptFunc(std::bind(&JSLayaGL::_getFloatv, this, pname));
		return m_nParameterResultFloat;
	}

	JsValue JSLayaGL::getFloatArrayv(int pname)
	{
		JCConch::s_pConchRender->setInterruptFunc(std::bind(&JSLayaGL::_getFloatArrayv, this, pname));
		return __JsArray<float>::ToJsArray(m_nParameterResultArray);
	}

	JsValue JSLayaGL::readPixels(int x, int y, int width, int height, int format, int type)
	{
		JCConch::s_pConchRender->setInterruptFunc(std::bind(&JSLayaGL::_readPixels, this, x, y, width, height, format, type));
		return createJSAB((char*)m_nParameterResultByteArray.data(), m_nParameterResultByteArray.size());
	}


    int JSLayaGL::getAttribLocationEx(const char* vs, const char* ps, const char* define, const char* sName)
    {
        std::string str_vs = vs;
        std::string str_ps = ps;
        std::string str_define = define;
        std::string str_name = sName;
        JCConch::s_pConchRender->setInterruptFunc(std::bind(&JSLayaGL::_getAttribLocationEx, this, str_vs, str_ps,str_define,str_name));
        return m_nAttribLocation;
    }

    const char* JSLayaGL::getStringEx(unsigned int name)
    {
        m_sGLString = "";
        JCConch::s_pConchRender->setInterruptFunc(std::bind(&JSLayaGL::_getStringEx, this, name));
        return m_sGLString.c_str();
    }

    void JSLayaGL::_getStringEx(unsigned int name)
    {
        m_sGLString = (char*)glGetString(name);
    }

    void JSLayaGL::_getProgramParameterEx(const std::string& vs, const std::string& ps, const std::string& define, int type)
    {
        _createShader(vs, ps, define);
        ::glGetProgramiv(m_pGpuProgram->getGpuProgram(), type, &m_nProgramParameter);
    }
    void JSLayaGL::_getActiveAttribEx(const std::string& vs, const std::string& ps, const std::string& define, int nIndex)
    {
        _createShader(vs, ps, define);
        ::glGetActiveAttrib(m_pGpuProgram->getGpuProgram(), nIndex, m_pShaderActiveInfo->bufsize, &m_pShaderActiveInfo->length, &m_pShaderActiveInfo->size, &m_pShaderActiveInfo->type, m_pShaderActiveInfo->name);
    }
    void JSLayaGL::_getActiveUniformEx(const std::string& vs, const std::string& ps, const std::string& define, int nIndex)
    {
        _createShader(vs, ps, define);
        ::glGetActiveUniform(m_pGpuProgram->getGpuProgram(), nIndex, m_pShaderActiveInfo->bufsize, &m_pShaderActiveInfo->length, &m_pShaderActiveInfo->size, &m_pShaderActiveInfo->type, m_pShaderActiveInfo->name);
    }
    void JSLayaGL::_getAttribLocationEx(const std::string& vs, const std::string& ps, const std::string& define, const std::string& name)
    {
        _createShader(vs, ps, define);
        m_nAttribLocation = ::glGetAttribLocation(m_pGpuProgram->getGpuProgram(), name.c_str());
    }
    void JSLayaGL::_getShaderPrecisionFormat(int shaderType, int precisionType)
    {
        ::glGetShaderPrecisionFormat(shaderType, precisionType, m_pShaderPrecisionFormat->range, m_pShaderPrecisionFormat->precision);
    }

	struct TypeDesc {
		GLenum category;
		int size;
	};

	void JSLayaGL::_getUniformEx(const std::string& locationName)
	{
		static int iRet[16];
		static float fRet[16];

		static std::map<GLenum, TypeDesc> typeMap =
		{
			// float
			{ GL_FLOAT,      { GL_FLOAT, 1 } },
			{ GL_FLOAT_VEC2, { GL_FLOAT, 2 } },
			{ GL_FLOAT_VEC3, { GL_FLOAT, 3 } },
			{ GL_FLOAT_VEC4, { GL_FLOAT, 4 } },
			{ GL_FLOAT_MAT2, { GL_FLOAT, 4 } },
			{ GL_FLOAT_MAT3, { GL_FLOAT, 9 } },
			{ GL_FLOAT_MAT4, { GL_FLOAT, 16 } },

			// int
			{ GL_INT,     { GL_INT, 1 } },
			{ GL_INT_VEC2,{ GL_INT, 2 } },
			{ GL_INT_VEC3,{ GL_INT, 3 } },
			{ GL_INT_VEC4,{ GL_INT, 4 } },

			// sampler
			{ GL_SAMPLER_2D,   {GL_INT, 1 } },
			{ GL_SAMPLER_CUBE, {GL_INT, 1}},

			// bool
			{ GL_BOOL,		{ GL_BOOL, 1 } },
			{ GL_BOOL_VEC2,	{ GL_BOOL, 2 } },
			{ GL_BOOL_VEC3,	{ GL_BOOL, 3 } },
			{ GL_BOOL_VEC4,	{ GL_BOOL, 4 } },
		};

		if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
		{
			m_nParameterResultArray.clear();
			GLint gpuProgram = 0;
			::glGetIntegerv(GL_CURRENT_PROGRAM, &gpuProgram);
			if (!gpuProgram)
			{
				return;
			}
			else
			{
				GLint location = ::glGetUniformLocation(gpuProgram, locationName.c_str());

				GLenum type = 0;
				GLsizei size = 0;

				::glGetActiveUniform(gpuProgram, location, 0, nullptr, &size, &type, nullptr);

				auto it = typeMap.find(type);
				if (it == typeMap.end())
				{
					return;
				}
				const TypeDesc& typeDesc = it->second;
				
				m_nParameterResultArray.push_back((float)(typeDesc.category));

				if (typeDesc.category == GL_FLOAT)
				{
					::glGetUniformfv(gpuProgram, location, fRet);
					for (int i = 0; i < typeDesc.size; i++)
					{
						m_nParameterResultArray.push_back(fRet[i]);
					}
				}
				else
				{
					::glGetUniformiv(gpuProgram, location, iRet);
					for (int i = 0; i < typeDesc.size; i++)
					{
						m_nParameterResultArray.push_back((float)iRet[i]);
					}
				}
			}
		}
		else
		{
			LOGI("getUniformEx is not supported");
		}
	}


    void JSLayaGL::_getParameter(int pname)
    {
        ::glGetIntegerv(pname, &m_nParameterResult);
    }

	void JSLayaGL::_getBooleanv(int pname)
	{
		if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
		{
			if (pname == GL_BLEND || pname == GL_CULL_FACE ||
				pname == GL_DEPTH_TEST || pname == GL_DEPTH_WRITEMASK ||
				pname == GL_DITHER || pname == GL_SAMPLE_COVERAGE_INVERT ||
				pname == GL_SCISSOR_TEST || pname == GL_STENCIL_TEST)
			{
                JCScriptRuntime::s_JSRT->dispatchLayaGLBuffer(false);
			}

			::glGetBooleanv(pname, &m_nParameterResultBool);
		}
		else
		{
			LOGI("getBooleanv not supported");
			m_nParameterResultBool = GL_FALSE;
		}

	}

	void JSLayaGL::_getIntegerv(int pname)
	{
		if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
		{
			if(pname == GL_ACTIVE_TEXTURE || pname == GL_BLEND_DST_ALPHA ||
				pname == GL_BLEND_DST_RGB || pname == GL_BLEND_EQUATION ||
				pname == GL_BLEND_EQUATION_ALPHA || pname == GL_BLEND_EQUATION_RGB ||
				pname == GL_BLEND_SRC_ALPHA || pname == GL_BLEND_SRC_RGB ||
				pname == GL_CULL_FACE_MODE || pname == GL_DEPTH_FUNC ||
				pname == GL_FRONT_FACE || pname == GL_GENERATE_MIPMAP_HINT ||
				pname == GL_STENCIL_BACK_FAIL || pname == GL_STENCIL_BACK_FUNC ||
				pname == GL_STENCIL_BACK_PASS_DEPTH_FAIL || pname == GL_STENCIL_BACK_PASS_DEPTH_PASS || pname == GL_STENCIL_BACK_REF ||
				pname == GL_STENCIL_BACK_VALUE_MASK || pname == GL_STENCIL_BACK_WRITEMASK || pname == GL_STENCIL_CLEAR_VALUE || pname == GL_STENCIL_FAIL || pname == GL_STENCIL_FUNC || pname == GL_STENCIL_PASS_DEPTH_FAIL || pname == GL_STENCIL_PASS_DEPTH_PASS || pname == GL_STENCIL_REF || pname == GL_STENCIL_VALUE_MASK || pname == GL_STENCIL_WRITEMASK || pname == GL_UNPACK_ALIGNMENT ||
				pname == GL_CURRENT_PROGRAM || pname == GL_ELEMENT_ARRAY_BUFFER_BINDING ||
				pname == GL_RENDERBUFFER_BINDING || pname == GL_TEXTURE_BINDING_2D ||
				pname == GL_TEXTURE_CUBE_MAP || pname == GL_ARRAY_BUFFER_BINDING)
			{
                JCScriptRuntime::s_JSRT->dispatchLayaGLBuffer(false);
			}
			::glGetIntegerv(pname, &m_nParameterResult);
		}
		else
		{
			static set<unsigned int> supportedNameSet = {
				GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, GL_MAX_CUBE_MAP_TEXTURE_SIZE,
				GL_MAX_FRAGMENT_UNIFORM_VECTORS, GL_MAX_RENDERBUFFER_SIZE, GL_MAX_TEXTURE_IMAGE_UNITS,
				GL_MAX_TEXTURE_SIZE, GL_MAX_VARYING_VECTORS, GL_MAX_VERTEX_ATTRIBS, GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
				GL_MAX_VERTEX_UNIFORM_VECTORS, GL_NUM_COMPRESSED_TEXTURE_FORMATS, GL_NUM_SHADER_BINARY_FORMATS
			};

			if (supportedNameSet.find(pname) != supportedNameSet.end())
			{
				::glGetIntegerv(pname, &m_nParameterResult);
			}
			else
			{
				LOGI("getIntegerv not supported this type=%d", pname);
				m_nParameterResult = 0;
			}
		}
	}

	void JSLayaGL::_getIntegerArrayv(int pname)
	{
		int size = 0;
		int* pRet = nullptr;

		if (pname == GL_MAX_VIEWPORT_DIMS)
		{
			size = 2;
			int ret[2] = { 0 };
			pRet = ret;
		}
		else if (pname == GL_SCISSOR_BOX || pname == GL_VIEWPORT)
		{
			size = 4;
			int ret[4] = { 0 };
			pRet = ret;
		}

		if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
		{
			if (pname == GL_SCISSOR_BOX || pname == GL_VIEWPORT)
			{
				JCScriptRuntime::s_JSRT->dispatchLayaGLBuffer(false);
			}

			::glGetIntegerv(pname, pRet);
		}
		else
		{
			if (pname == GL_MAX_VIEWPORT_DIMS)
			{
				::glGetIntegerv(pname, pRet);
			}
			else
			{
				LOGI("getIntegerv not supported");
			}
		}

		m_nParameterResultArray.clear();
		for (int i = 0; i < size; i++)
		{
			m_nParameterResultArray.push_back(pRet[i]);
		}
	}

	void JSLayaGL::_getFloatv(int pname)
	{
		if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
		{
			if (pname == GL_DEPTH_CLEAR_VALUE || pname == GL_SAMPLE_COVERAGE_VALUE)
			{
                JCScriptRuntime::s_JSRT->dispatchLayaGLBuffer(false);
			}
			::glGetFloatv(pname, &m_nParameterResultFloat);
		}
		else
		{
			LOGI("getIntegerv not supported");
			m_nParameterResultFloat = 0;
		}
	}

	void JSLayaGL::_getFloatArrayv(int pname)
	{
		int size = 0;
		float* pRet = nullptr;

		if (pname == GL_ALIASED_LINE_WIDTH_RANGE || pname == GL_ALIASED_POINT_SIZE_RANGE ||
			pname == GL_DEPTH_RANGE)
		{
			size = 2;
			float ret[2] = { 0.0 };
			pRet = ret;
		}
		else if (pname == GL_BLEND_COLOR || pname == GL_COLOR_CLEAR_VALUE)
		{
			size = 4;
			float ret[4] = { 0.0 };
			pRet = ret;
		}

		if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
		{
			if (pname == GL_BLEND_COLOR || pname == GL_COLOR_CLEAR_VALUE ||
				pname == GL_DEPTH_RANGE)
			{
                JCScriptRuntime::s_JSRT->dispatchLayaGLBuffer(false);
			}

			::glGetFloatv(pname, pRet);
		}
		else
		{
			if (pname == GL_ALIASED_LINE_WIDTH_RANGE || pname == GL_ALIASED_POINT_SIZE_RANGE ||
				pname == GL_DEPTH_RANGE)
			{
				::glGetFloatv(pname, pRet);
			}
			else
			{
				LOGI("glGetFloatv not supported");
			}
		}

		m_nParameterResultArray.clear();
		for (int i = 0; i < size; i++)
		{
			m_nParameterResultArray.push_back(pRet[i]);
		}
	}

    void JSLayaGL::_createShader(const std::string& vs, const std::string& ps, const std::string& define)
    {
        if (m_sShaderVS != vs || m_sShaderPS != ps || m_sShaderDefine != define)
        {
            if (m_pShaderTemplate)
            {
                delete m_pShaderTemplate;
                m_pShaderTemplate = NULL;
                m_pGpuProgram = NULL;
            }
            m_sShaderVS = vs;
            m_sShaderPS = ps;
            m_sShaderDefine = define;

            //重新new新的shader
            std::vector<std::string> vString;
            m_pShaderTemplate = new JCGpuProgramTemplate(vs.c_str(), ps.c_str(),vString);
            m_pGpuProgram = m_pShaderTemplate->getInstance(define.c_str());
        }
    }
    const char* JSLayaGL::getShaderInfoLogEx(const char* source,int type)
    {
        std::string str_source = source;
        m_sErrorInfo = "";
        JCConch::s_pConchRender->setInterruptFunc(std::bind(&JSLayaGL::_getShaderInfoLog, this, str_source,type));
        return m_sErrorInfo.c_str();
    }
    const char* JSLayaGL::getProgramInfoLogEx(const char* vs, const char* ps, const char* define)
    {
        std::string str_vs = vs;
        std::string str_ps = ps;
        std::string str_define = define;
        m_sErrorInfo = "";
        JCConch::s_pConchRender->setInterruptFunc(std::bind(&JSLayaGL::_getProgramInfoLog, this, str_vs, str_ps, str_define));
        return m_sErrorInfo.c_str();
    }
    void JSLayaGL::_getShaderInfoLog(const std::string& source,int type)
    {
        GLuint nShader = glCreateShader(type);
        const char* strShaders[1] = {source.c_str()};
        glShaderSource(nShader, 1, strShaders, NULL);
        glCompileShader(nShader);
        //看错误
        GLint nInfoLen = 0;
        glGetShaderiv(nShader, GL_INFO_LOG_LENGTH, &nInfoLen);
        if (nInfoLen)
        {
            char* buf = new char[nInfoLen];
            glGetShaderInfoLog(nShader, nInfoLen, NULL, buf);
            m_sErrorInfo = "";
            m_sErrorInfo = buf;
            delete buf;
        }
        glDeleteShader(nShader);
    }
    void JSLayaGL::_getProgramInfoLog(const std::string& vs, const std::string& ps, const std::string& define)
    {
        _createShader(vs, ps, define);
        GLint nInfoLen = 0;

		int gpuProgram = m_pGpuProgram->getGpuProgram();
		if (!gpuProgram)
		{
			m_sErrorInfo = "invalid shaders";
		}
		else
		{
			glGetShaderiv(gpuProgram, GL_INFO_LOG_LENGTH, &nInfoLen);
			if (nInfoLen)
			{
				char* buf = new char[nInfoLen];
				glGetProgramInfoLog(m_pGpuProgram->getGpuProgram(), nInfoLen, NULL, buf);
				m_sErrorInfo = "";
				m_sErrorInfo = buf;
				delete buf;
			}
		}
    }
    /*int JSLayaGL::getBufferParameter(int target, int pname)
    {
        JCConch::s_pConchRender->setInterruptFunc(std::bind(&JSLayaGL::_getBufferParameter, this, target,pname));
        return m_nParameterResult;
    }
    void JSLayaGL::_getBufferParameter(int target, int pname)
    {
        ::glGetBufferParameteriv(target, pname, &m_nParameterResult);
    }*/
	int JSLayaGL::getFramebufferAttachmentParameter(int target, int attachment, int pname)
	{
		JCConch::s_pConchRender->setInterruptFunc(std::bind(&JSLayaGL::_getFramebufferAttachmentParameter, this, target, attachment, pname));
		return m_nParameterResult;
	}
	void JSLayaGL::_getFramebufferAttachmentParameter(int target, int attachment, int pname)
	{
		if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
		{
            JCScriptRuntime::s_JSRT->dispatchLayaGLBuffer(false);
			::glGetFramebufferAttachmentParameteriv(target, attachment, pname, &m_nParameterResult);
		}
		else
		{
			LOGI("getFramebufferAttachmentParameter not supported");
		}
	}
	/*int JSLayaGL::getRenderbufferParameter(int target, int pname)
	{
		JCConch::s_pConchRender->setInterruptFunc(std::bind(&JSLayaGL::_getRenderbufferParameter, this, target, pname));
		return m_nParameterResult;
	}
	void JSLayaGL::_getRenderbufferParameter(int target, int pname)
	{
		::glGetRenderbufferParameteriv(target, pname, &m_nParameterResult);
	}
	int JSLayaGL::getTexParameter(int target, int pname)
	{
		JCConch::s_pConchRender->setInterruptFunc(std::bind(&JSLayaGL::_getTexParameter, this, target, pname));
		return m_nParameterResult;
	}
	void JSLayaGL::_getTexParameter(int target, int pname)
	{
		::glGetTexParameteriv(target, pname, &m_nParameterResult);
	}*/
	int JSLayaGL::getShaderParameter(const char* src,int type,int pname)
	{
        std::string str_source = src;
		JCConch::s_pConchRender->setInterruptFunc(std::bind(&JSLayaGL::_getShaderParameter, this, str_source, type, pname));
		return m_nParameterResult;
	}
	void JSLayaGL::_getShaderParameter(const std::string& source,int type, int pname)
	{
        GLuint nShader = glCreateShader(type);
        const char* strShaders[1] = { source.c_str() };
        glShaderSource(nShader, 1, strShaders, NULL);
        glCompileShader(nShader);
		::glGetShaderiv(nShader, pname, &m_nParameterResult);
        glDeleteShader(nShader);
	}

	void JSLayaGL::_readPixels(int x, int y, int width, int height, int format, int type)
	{
		if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
		{
            JCScriptRuntime::s_JSRT->dispatchLayaGLBuffer(false);

            m_nParameterResultByteArray.clear();
            int bytes = 0;
            int bytesPerRow = 0;
            int comp = 0;
            switch (format)
            {
            case GL_ALPHA:
                comp = 1;
                break;
            case GL_RGB:
                comp = 3;
                break;
            case GL_RGBA:
                comp = 4;
                break;
            default:
                LOGE("LayaGL Invalid parameter");
                return;
                break;
            }

            switch (type)
            {
            case GL_UNSIGNED_BYTE:
                bytesPerRow = width * comp;
                bytes = bytesPerRow * height;
                break;
            case GL_UNSIGNED_SHORT_5_6_5:
                if (format != GL_RGB)
                {
                    LOGE("LayaGL Invalid operation");
                    return;
                }
                bytesPerRow = width * 2;
                bytes = bytesPerRow * height;
                break;
            case GL_UNSIGNED_SHORT_4_4_4_4:
            case GL_UNSIGNED_SHORT_5_5_5_1:
                if (format != GL_RGBA)
                {
                    LOGE("LayaGL Invalid operation");
                    return;
                }
                bytesPerRow = width * 2;
                bytes = bytesPerRow * height;
                break;
            case GL_FLOAT:
                bytesPerRow = width * 4 * comp;;
                bytes = bytesPerRow * height;
                break;
            default:
                LOGE("LayaGL Invalid parameter");
                return;
                break;
            }

            m_nParameterResultByteArray.resize(bytes);
			
            ::glReadPixels(x, y, width, height, format, type, (void*)m_nParameterResultByteArray.data());
		}
		else
		{
			LOGI("readPixels not supported");
		}
	}


    int JSLayaGL::checkFramebufferStatusEx(int target)
    {
        if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
        {
            JCScriptRuntime::s_JSRT->dispatchLayaGLBuffer(false);
            return ::glCheckFramebufferStatus(target);
        }
        else
        {
            LOGI("checkFramebufferStatus not supported"); 
            return 0;
        } 
    }
    int JSLayaGL::getVertexAttribEx(int index, int target)
    {
		int result = 0;
        if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
        {
            JCScriptRuntime::s_JSRT->dispatchLayaGLBuffer(false);
			glGetVertexAttribiv(index, target, &result);
        }
        else
        {
            LOGI("getVertexAttrib not supported");
        }
        return result;
    }

	JsValue JSLayaGL::getVertexAttribExfv(int index, int target)
	{
		static float ret[4] = { 0.0 };
		m_nParameterResultArray.clear();
		if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
		{
            JCScriptRuntime::s_JSRT->dispatchLayaGLBuffer(false);
			glGetVertexAttribfv(index, target, ret);
			for (int i = 0; i < 4; i++)
			{
				m_nParameterResultArray.push_back(ret[i]);
			}
		}
		else
		{
			LOGI("getVertexAttrib not supported");
		}
		return __JsArray<float>::ToJsArray(m_nParameterResultArray);
	}

	int JSLayaGL::getVertexAttribOffset(int index, int pname)
	{
		void* result = nullptr;
		if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
		{
            JCScriptRuntime::s_JSRT->dispatchLayaGLBuffer(false);
			::glGetVertexAttribPointerv(index, pname, &result);
		}
		else
		{
			LOGI("getVertexAttribOffset not supported");
		}

		return reinterpret_cast<intptr_t>(result);
	}

    int JSLayaGL::getBufferParameterEx(int target, int pname)
    {
        if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
        {
            JCScriptRuntime::s_JSRT->dispatchLayaGLBuffer(false);
            GLint params;
            ::glGetBufferParameteriv(target, pname, &params);
            return params;
        }
        else
        {
            LOGI("getBufferParameter not supported");
            return 0;
        }
    }
    int JSLayaGL::getRenderbufferParameterEx(int target, int pname)
    {
        if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
        {
            JCScriptRuntime::s_JSRT->dispatchLayaGLBuffer(false);
            GLint params;
            ::glGetRenderbufferParameteriv(target, pname, &params);
            return params;
        }
        else
        {
            LOGI("getRenderbufferParameter not supported");
            return 0;
        }
    }
    int JSLayaGL::getTexParameterEx(int target, int pname)
    {
        if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
        {
            JCScriptRuntime::s_JSRT->dispatchLayaGLBuffer(false);
            GLint params;
            ::glGetTexParameteriv(target, pname, &params);
            return params;
        }
        else
        {
            LOGI("getTexParameter not supported");
            return 0;
        }
    }

	int JSLayaGL::isEnabled(unsigned int cap)
	{
		if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
		{
            JCScriptRuntime::s_JSRT->dispatchLayaGLBuffer(false);
		}

		int ret = (int) ::glIsEnabled(cap);

		return ::glIsEnabled(cap);
	}

	void JSLayaGL::flushCommand()
	{
		if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
		{
            JCScriptRuntime::s_JSRT->dispatchLayaGLBuffer(false);
		}
		else
		{
			LOGI("flushCommand is not supported");
		}
	}
	void JSLayaGL::setMainContextSize(int width,int height)
	{
		if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
		{
			JCConch::s_pConchRender->m_pLayaGL->m_nMainCanvasWidth = width;
			JCConch::s_pConchRender->m_pLayaGL->m_nMainCanvasHeight = height;
		}
		else
		{
			JCScriptRuntime::s_JSRT->flushSharedCmdBuffer();
			JCCommandEncoderBuffer* pCmd = JCScriptRuntime::s_JSRT->m_pRenderCmd;
			pCmd->append(LAYA_SET_MAIN_CONTEXT_SIZE);
			pCmd->append(width);
			pCmd->append(height);
		}
	}
    void JSLayaGL::exportJS()
    {
        JSP_GLOBAL_CLASS("layagl", JSLayaGL, this);
		JSP_GLOBAL_ADD_METHOD("setFrameAndSyncCountArrayBufferID", JSLayaGL::setFrameAndSyncCountArrayBufferID);
		JSP_GLOBAL_ADD_METHOD("setSyncArrayBufferID", JSLayaGL::setSyncArrayBufferID);
		JSP_GLOBAL_ADD_METHOD("setRootCommandEncoder", JSLayaGL::setRootCommandEncoder);
		JSP_GLOBAL_ADD_METHOD("getProgramParameterEx", JSLayaGL::getProgramParameterEx);
		JSP_GLOBAL_ADD_METHOD("getStringEx", JSLayaGL::getStringEx);
		JSP_GLOBAL_ADD_METHOD("getActiveAttribEx", JSLayaGL::getActiveAttribEx);
		JSP_GLOBAL_ADD_METHOD("getActiveUniformEx", JSLayaGL::getActiveUniformEx);
		JSP_GLOBAL_ADD_METHOD("getAttribLocationEx", JSLayaGL::getAttribLocationEx);
		JSP_GLOBAL_ADD_METHOD("getShaderInfoLogEx", JSLayaGL::getShaderInfoLogEx);
		JSP_GLOBAL_ADD_METHOD("getProgramInfoLogEx", JSLayaGL::getProgramInfoLogEx);
		JSP_GLOBAL_ADD_METHOD("getShaderPrecisionFormat", JSLayaGL::getShaderPrecisionFormat);
		JSP_GLOBAL_ADD_METHOD("getUniformEx", JSLayaGL::getUniformEx);
		JSP_GLOBAL_ADD_METHOD("getParameter", JSLayaGL::getParameter);
		JSP_GLOBAL_ADD_METHOD("getBooleanv", JSLayaGL::getBooleanv);
		JSP_GLOBAL_ADD_METHOD("getIntegerv", JSLayaGL::getIntegerv);
		JSP_GLOBAL_ADD_METHOD("getIntegerArrayv", JSLayaGL::getIntegerArrayv);
		JSP_GLOBAL_ADD_METHOD("getFloatv", JSLayaGL::getFloatv);
		JSP_GLOBAL_ADD_METHOD("getFloatArrayv", JSLayaGL::getFloatArrayv);
        //JSP_ADD_METHOD("getBufferParameter", JSLayaGL::getBufferParameter);
		JSP_GLOBAL_ADD_METHOD("getFramebufferAttachmentParameter", JSLayaGL::getFramebufferAttachmentParameter);
		//JSP_ADD_METHOD("getRenderbufferParameter", JSLayaGL::getRenderbufferParameter);
		//JSP_ADD_METHOD("getTexParameter", JSLayaGL::getTexParameter);
		JSP_GLOBAL_ADD_METHOD("getShaderParameter", JSLayaGL::getShaderParameter);
		JSP_GLOBAL_ADD_METHOD("getThreadMode", JSLayaGL::getThreadMode);
		JSP_GLOBAL_ADD_METHOD("checkFramebufferStatusEx", JSLayaGL::checkFramebufferStatusEx);
		JSP_GLOBAL_ADD_METHOD("getBufferParameterEx", JSLayaGL::getBufferParameterEx);
		JSP_GLOBAL_ADD_METHOD("getRenderbufferParameterEx", JSLayaGL::getRenderbufferParameterEx);
		JSP_GLOBAL_ADD_METHOD("getTexParameterEx", JSLayaGL::getTexParameterEx);
		JSP_GLOBAL_ADD_METHOD("isEnabled", JSLayaGL::isEnabled);
		JSP_GLOBAL_ADD_METHOD("getVertexAttribEx", JSLayaGL::getVertexAttribEx);
		JSP_GLOBAL_ADD_METHOD("getVertexAttribExfv", JSLayaGL::getVertexAttribExfv);
		JSP_GLOBAL_ADD_METHOD("getVertexAttribOffset", JSLayaGL::getVertexAttribOffset);
		JSP_GLOBAL_ADD_METHOD("flushCommand", JSLayaGL::flushCommand);
		JSP_GLOBAL_ADD_METHOD("readPixels", JSLayaGL::readPixels);
		JSP_GLOBAL_ADD_METHOD("setMainContextSize", JSLayaGL::setMainContextSize);
        JSP_INSTALL_GLOBAL_CLASS("layagl", JSLayaGL,this);
    }
}

//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
