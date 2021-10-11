/**
@file			JCLayaGL.cpp
@brief			
@author			James
@version		1.0
@date			2018_5_25
*/

#include "JCLayaGL.h"
#include <util/Log.h>
#include <stdio.h>
#include <stdlib.h>
#include "../JCCmdStruct.h"
#include <math/Matrix32.h>
#include <util/JCCommonMethod.h>
#include "JCLayaGLDispatch.h"
#include <Bridge/JCConchBridge.h>
#include "../Performance/JCPerfHUD.h"
#include <webglplus/JCWebGLPlus.h>
#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#else
#include <GLES3/gl3.h>
#endif
extern int g_nInnerWidth;
extern int g_nInnerHeight;

extern int  s_nGLCaps;
#define DVERIFY(v) if(!v){*(int*)0=1;}
namespace laya
{
    char JCLayaGL::m_pDummyData[TEX_IMAGE2D_DUMMY_MAX_SIZE];
    float JCLayaGL::s_fMainCanvasScaleX = 1.0f;
    float JCLayaGL::s_fMainCanvasScaleY = 1.0f;
    float JCLayaGL::s_fMainCanvasTX = 0.0f;
    float JCLayaGL::s_fMainCanvasTY = 0.0f;
    //#define DEBUG_WEBGL 1
    JCLayaGL::JCLayaGL(int nWidth, int nHeight, JCArrayBufferManager* pArrayBuferManager,JCImageManager* pImageManager,JCIDGenerator* pIDGenerator, JCIDGenerator* pProgramLocationTable, JCRegister* pRegister,JCWebGLPlus* pWebGLPLus)
    {
        m_nMainCanvasWidth = nWidth;
        m_nMainCanvasHeight = nHeight;
        JCPerfHUD::m_pLayaGL = this;
        m_bFlipY = false;
        m_pIDGenerator = pIDGenerator;
        m_pProgramLocationTable = pProgramLocationTable;
        m_pImageManager = pImageManager;
        m_pArrayBufferManager = pArrayBuferManager;
        m_nMainFrameBuffer = 0;
        m_nCurrentBuffer = 0;
        m_nCurrentProgram = 0;
		m_pRegister = pRegister;
        m_pWebGLPlus = pWebGLPLus;
#ifdef __APPLE__
        //glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_nMainFrameBuffer);
        m_nMainFrameBuffer = 1;
#endif
    }
    JCLayaGL::~JCLayaGL()
    {
    }
    bool JCLayaGL::isContextLost()
    {
        return false;
    }
    void* JCLayaGL::getExtension(const std::string& name)
    {
        return NULL;
    }
    void JCLayaGL::activeTexture(GLenum texture)
    {
        ::glActiveTexture(texture);
#if DEBUG_WEBGL
        LOGI("activeTexture texture=%d",texture);
        getError();
#endif
    }
    void JCLayaGL::attachShader(WebGLProgram program, WebGLShader shader)
    {
        GLuint realProgram = m_pIDGenerator->getRealID(program);
        GLuint realShader = m_pIDGenerator->getRealID(shader);
        ::glAttachShader(realProgram, realShader);
#ifdef DEBUG_WEBGL
        LOGI("attachShader realProgram=%d,fakeProgram=%d,realShader=%d,fakeShader=%d",realProgram,program,realShader,shader);
        getError();
#endif
    }
    void JCLayaGL::bindAttribLocation(WebGLProgram program, GLuint index, const std::string& name)
    {
        GLuint realProgram = m_pIDGenerator->getRealID(program);
        ::glBindAttribLocation(realProgram,index,name.c_str());
#ifdef DEBUG_WEBGL
        LOGI("bindAttribLocation realProgram=%d,fakeProgram=%d,index=%d,name=%s", realProgram, program, index, name.c_str());
        getError();
#endif
    }
    void JCLayaGL::bindBuffer(GLenum target, WebGLBuffer buffer)
    {
        int realID = (buffer == 0 ) ? 0 : m_pIDGenerator->getRealID(buffer);
        m_nCurrentBuffer = realID;
        ::glBindBuffer(target, realID);
#ifdef DEBUG_WEBGL
        LOGI("bindBuffer target=%d,realBuffer=%d,fakeBuffer=%d", target, realID, buffer);
        getError();
#endif
    }
    void JCLayaGL::bindFramebuffer(GLenum target, WebGLFramebuffer framebuffer)
    {
        int realID = (framebuffer == 0) ? m_nMainFrameBuffer : m_pIDGenerator->getRealID(framebuffer);
        ::glBindFramebuffer(target, realID);
#ifdef DEBUG_WEBGL
        LOGI("bindFramebuffer target=%d,realBuffer=%d,fakeBuffer=%d", target, realID, framebuffer);
        getError();
#endif
    }
    void JCLayaGL::bindRenderbuffer(GLenum target, WebGLRenderbuffer renderbuffer)
    {
        int realID = (renderbuffer == 0) ? 0 : m_pIDGenerator->getRealID(renderbuffer);
        ::glBindRenderbuffer(target, realID);
#ifdef DEBUG_WEBGL
        LOGI("bindRenderbuffer target=%d,realBuffer=%d,fakeBuffer=%d", target, realID, renderbuffer);
        getError();
#endif
    }
    void JCLayaGL::bindTexture(GLenum target, WebGLTexture texture)
    {
        int realID = (texture == 0) ? 0 : m_pIDGenerator->getRealID(texture);
        ::glBindTexture(target, realID);
#ifdef DEBUG_WEBGL
        LOGI("bindTexture target=%d,realTexture=%d,fakeTexture=%d", target, realID, texture);
        getError();
#endif
    }
    void JCLayaGL::blendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
    {
        ::glBlendColor(red, green, blue, alpha);
#ifdef DEBUG_WEBGL
        LOGI("blendColor red=%f,green=%f,blue=%f,alpha=%f", red, green, blue, alpha);
        getError();
#endif
    }
    void JCLayaGL::blendEquation(GLenum mode)
    {
        ::glBlendEquation(mode);
#ifdef DEBUG_WEBGL
        LOGI("blendEquation mode=%d",mode);
        getError();
#endif
    }
    void JCLayaGL::blendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
    {
        ::glBlendEquationSeparate(modeRGB, modeAlpha);
#ifdef DEBUG_WEBGL
        LOGI("blendEquationSeparate modeRGB=%d,modeAlpha=%d", modeRGB, modeAlpha);
        getError();
#endif
    }
    void JCLayaGL::blendFunc(GLenum sfactor, GLenum dfactor)
    {
        ::glBlendFunc(sfactor, dfactor);
#ifdef DEBUG_WEBGL
        LOGI("blendFunc sfactor=%d,dfactor=%d", sfactor, dfactor);
        getError();
#endif
    }
    void JCLayaGL::blendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
    {
        ::glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
#ifdef DEBUG_WEBGL
        LOGI("blendFuncSeparate srcRGB=%d,dstRGB=%d,srcAlpha=%d,dstAlpha=%d", srcRGB, dstRGB, srcAlpha, dstAlpha);
        getError();
#endif
    }
    void JCLayaGL::bufferData_size(GLenum target, GLsizeiptr size, GLenum usage)
    {
        ::glBufferData(target,size,NULL,usage);
#ifdef DEBUG_WEBGL
        LOGI("bufferData_size target=%d,size=%d,usage=%d", target, size, usage);
        getError();
#endif
    }
    void JCLayaGL::bufferData(GLenum target, int dataSize, char* data, GLenum usage)
    {
        ::glBufferData(target, dataSize,(GLvoid*)data,usage);
#ifdef DEBUG_WEBGL
        LOGI("bufferData target=%d,size=%d,usage=%d", target, dataSize, usage);
        getError();
#endif
    }
    void JCLayaGL::bufferSubData(GLenum target, GLintptr offset, int dataSize,char* data )
    {
        ::glBufferSubData(target,offset,dataSize,data);
#ifdef DEBUG_WEBGL
        LOGI("bufferSubData target=%d,offset=%d,dataSize=%d", target, offset, dataSize);
        getError();
#endif
    }
    GLenum JCLayaGL::checkFramebufferStatus(GLenum target)
    {
        GLenum status = ::glCheckFramebufferStatus(target);
#ifdef DEBUG_WEBGL
        LOGI("checkFramebufferStatus");
        getError();
#endif
        return status;
    }
    void JCLayaGL::clear(GLbitfield mask)
    {
        ::glClear(mask);
#ifdef DEBUG_WEBGL
        LOGI("clear mask=%d", mask);
        getError();
#endif
    }
	void JCLayaGL::clearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat* value)
	{
		::glClearBufferfv(buffer, drawbuffer, value);
#ifdef DEBUG_WEBGL
		LOGI("clearBufferfv buffer=%d drawbuffer=%d value %f %f %f %f", buffer, value[0], value[1], value[2], value[3]);
		getError();
#endif
	}
    void JCLayaGL::clearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
    {
        ::glClearColor(red, green, blue, alpha);
#ifdef DEBUG_WEBGL
        LOGI("clearColor red=%f,green=%f,blue=%f,alpha=%f", red,green,blue,alpha);
        getError();
#endif
    }
    void JCLayaGL::clearDepth(GLclampf depth)
    {
        ::glClearDepthf(depth);
#ifdef DEBUG_WEBGL
        LOGI("clearDepth depth=%f",depth);
        getError();
#endif
    }
    void JCLayaGL::clearStencil(GLint s)
    {
        ::glClearStencil(s);
#ifdef DEBUG_WEBGL
        LOGI("glClearStencil s=%d", s);
        getError();
#endif
    }
    void JCLayaGL::colorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
    {
        ::glColorMask(red, green, blue, alpha);
#ifdef DEBUG_WEBGL
        LOGI("glColorMask red=%d,green=%d,blue=%d,alpha=%d", red, green, blue, alpha);
        getError();
#endif
    }
    void JCLayaGL::compileShader(WebGLShader shader)
    {
        GLuint realID = m_pIDGenerator->getRealID(shader);
        ::glCompileShader(realID);
#ifdef DEBUG_WEBGL
        LOGI("compileShader realID=%d,fakeShader=%d", realID,shader);
        getError();
#endif
    }
    void JCLayaGL::compressedTexImage2D(GLenum target, GLint level, GLenum internalformat,GLsizei width, GLsizei height, GLint border, int dataSize,char* data)
    {
        ::glCompressedTexImage2D(target, level, internalformat,width, height, border, dataSize,(GLvoid*)data);
#ifdef DEBUG_WEBGL
        LOGI("compressedTexImage2D target=%d,level=%d,internalformat=%d,width=%d,height=%d,border=%d,dataSize=%d", target, level, internalformat, width, height, border, dataSize);
        getError();
#endif
    }
    void JCLayaGL::compressedTexSubImage2D(GLenum target, GLint level,GLint xoffset, GLint yoffset,GLsizei width, GLsizei height, GLenum format, int dataSize, char* data)
    {
        ::glCompressedTexSubImage2D(target, level,xoffset, xoffset,width, height, format, dataSize,(GLvoid*)data);
#ifdef DEBUG_WEBGL
        LOGI("compressedTexSubImage2D target=%d,xoffset=%d,xoffset=%d,width=%d,height=%d,format=%d,dataSize=%d", target, level, xoffset, xoffset, width, height, format, dataSize );
        getError();
#endif
    }
    void JCLayaGL::copyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
    {
        ::glCopyTexImage2D(target, level, internalformat, x, y, width, height, border);
#ifdef DEBUG_WEBGL
        LOGI("copyTexImage2D target=%d,level=%d,internalformat=%d,x=%d,y=%d,width=%d,height=%d,border=%d", target, level, internalformat, x, y, width, height, border);
        getError();
#endif
    }
    void JCLayaGL::copyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
    {
        ::glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
#ifdef DEBUG_WEBGL
        LOGI("copyTexSubImage2D target=%d,level=%d,xoffset=%d,yoffset=%d,x=%d,y=%d,width=%d,height=%d", target, level, xoffset, yoffset, x, y, width, height);
        getError();
#endif
    }
    WebGLBuffer JCLayaGL::createBuffer(int fakeID)
    {
        WebGLBuffer buffer;
        ::glGenBuffers(1,&buffer);
        m_vAllBuffer[buffer] = true;
        m_pIDGenerator->setRealID(fakeID, buffer);
#ifdef DEBUG_WEBGL
        LOGI("createBuffer realBuffer=%d,fakeID=%d", buffer, fakeID);
        getError();
#endif
        return buffer;
    }
    WebGLFramebuffer JCLayaGL::createFramebuffer(int fakeID)
    {
        WebGLFramebuffer buffer;
        ::glGenFramebuffers(1,&buffer);
        m_vAllFrameBuffer[buffer] = true;
        m_pIDGenerator->setRealID(fakeID, buffer);
#ifdef DEBUG_WEBGL
        LOGI("createFramebuffer realBuffer=%d,fakeID=%d", buffer, fakeID);
        getError();
#endif
        return buffer;
    }
    WebGLProgram JCLayaGL::createProgram(int fakeID)
    {
        WebGLProgram program = ::glCreateProgram();
        m_pIDGenerator->setRealID(fakeID, program);
        m_vAllProgram[program] = true;
#ifdef DEBUG_WEBGL
        LOGI("createProgram realProgram=%d,fakeID=%d", program, fakeID);
        getError();
#endif
        return program;
    }
    WebGLRenderbuffer JCLayaGL::createRenderbuffer(int fakeID)
    {
        WebGLRenderbuffer buffer;
        ::glGenRenderbuffers(1,&buffer);
        m_pIDGenerator->setRealID(fakeID, buffer);
        m_vAllRenderBuffer[buffer] = true;
#ifdef DEBUG_WEBGL
        LOGI("createRenderbuffer realBuffer=%d,fakeID=%d", buffer, fakeID);
        getError();
#endif
        return buffer;
    }
    WebGLShader JCLayaGL::createShader(int fakeID,GLenum type)
    {
        WebGLShader shader = ::glCreateShader(type);
        m_pIDGenerator->setRealID(fakeID, shader);
        m_vAllShader[shader] = true;
#ifdef DEBUG_WEBGL
        LOGI("createShader realShader=%d,fakeID=%d,type=%d", shader, fakeID,type);
        getError();
#endif
        return shader;
    }
    WebGLTexture JCLayaGL::createTexture(int fakeID)
    {
        GLuint tex;
        glGenTextures(1,&tex);
        m_pIDGenerator->setRealID(fakeID, tex);
        m_vAllTexture[tex] = true;
#ifdef DEBUG_WEBGL
        LOGI("createTexture realTexture=%d,fakeID=%d", tex, fakeID);
        getError();
#endif
        return tex;
    }
    void JCLayaGL::cullFace(GLenum mode)
    {
        ::glCullFace(mode);
#ifdef DEBUG_WEBGL
        LOGI("cullFace mode=%d",mode);
        getError();
#endif
    }
    void JCLayaGL::deleteBuffer(WebGLBuffer buffer)
    {
        GLuint nRealID = m_pIDGenerator->getRealID(buffer);
        m_pIDGenerator->deleteID(buffer);
        removeBufferFromMap(nRealID);
        ::glDeleteBuffers(1,&nRealID);
#ifdef DEBUG_WEBGL
        LOGI("deleteBuffer realID=%d,fakeID=%d", nRealID,buffer);
        getError();
#endif
    }
    void JCLayaGL::deleteFramebuffer(WebGLFramebuffer framebuffer)
    {
        GLuint nRealID = m_pIDGenerator->getRealID(framebuffer);
        m_pIDGenerator->deleteID(framebuffer);
        removeFrameBufferFromMap(nRealID);
        ::glDeleteFramebuffers(1,&nRealID);
#ifdef DEBUG_WEBGL
        LOGI("deleteFramebuffer realID=%d,fakeID=%d", nRealID, framebuffer);
        getError();
#endif
    }
    void JCLayaGL::deleteProgram(WebGLProgram program)
    {
        GLuint nRealID = m_pIDGenerator->getRealID(program);
        m_pIDGenerator->deleteID(program);
        removeProgramFromMap(nRealID);
        ::glDeleteProgram(nRealID);
#ifdef DEBUG_WEBGL
        LOGI("deleteProgram realID=%d,fakeID=%d", nRealID, program);
        getError();
#endif
    }
    void JCLayaGL::deleteRenderbuffer(WebGLRenderbuffer renderbuffer)
    {
        GLuint nRealID = m_pIDGenerator->getRealID(renderbuffer);
        m_pIDGenerator->deleteID(renderbuffer);
        removeRenderBufferFromMap(nRealID);
        ::glDeleteRenderbuffers(1,&nRealID);
#ifdef DEBUG_WEBGL
        LOGI("deleteRenderbuffer realID=%d,fakeID=%d", nRealID, renderbuffer);
        getError();
#endif
    }
    void JCLayaGL::deleteShader(WebGLShader shader)
    {
        GLuint nRealID = m_pIDGenerator->getRealID(shader);
        m_pIDGenerator->deleteID(shader);
        removeShaderFromMap(nRealID);
        ::glDeleteShader(nRealID);
#ifdef DEBUG_WEBGL
        LOGI("deleteShader realID=%d,fakeID=%d", nRealID, shader);
        getError();
#endif
    }
    void JCLayaGL::deleteTexture(WebGLTexture texture)
    {
        GLuint nRealID = m_pIDGenerator->getRealID(texture);
        m_pIDGenerator->deleteID(texture);
        removeTextureFromMap(nRealID);
        ::glDeleteTextures(1,&nRealID);
#ifdef DEBUG_WEBGL
        LOGI("deleteTexture realID=%d,fakeID=%d", nRealID, texture);
        getError();
#endif
    }
    void JCLayaGL::depthFunc(GLenum func)
    {
        ::glDepthFunc(func);
#ifdef DEBUG_WEBGL
        LOGI("depthFunc func=%d", func);
        getError();
#endif
    }
    void JCLayaGL::depthMask(GLboolean flag)
    {
        ::glDepthMask(flag);
#ifdef DEBUG_WEBGL
        LOGI("depthMask flag=%d", flag);
        getError();
#endif
    }
    void JCLayaGL::depthRange(GLclampf zNear, GLclampf zFar)
    {
        ::glDepthRangef(zNear,zFar);
#ifdef DEBUG_WEBGL
        LOGI("depthRange zNear=%f,zFar=%f", zNear,zFar);
        getError();
#endif
    }
    void JCLayaGL::detachShader(WebGLProgram program, WebGLShader shader)
    {
        GLuint realProgram = m_pIDGenerator->getRealID(program);
        GLuint realShader = m_pIDGenerator->getRealID(shader);
        ::glDetachShader(realProgram, realShader);
#ifdef DEBUG_WEBGL
        LOGI("detachShader realProgram=%d,fakeProgram=%d,realShader=%d,fakeShader=%d", realProgram, program, realShader, shader);
        getError();
#endif
    }
    void JCLayaGL::disable(GLenum cap)
    {
        glDisable(cap);
#ifdef DEBUG_WEBGL
        LOGI("disable cap=%d", cap);
        getError();
#endif
    }
    void JCLayaGL::disableVertexAttribArray(GLuint index)
    {
        int realLoc = m_pProgramLocationTable->getRealID(index);
        ::glDisableVertexAttribArray(realLoc);
#ifdef DEBUG_WEBGL
        LOGI("disableVertexAttribArray realLoc=%d index=%d", realLoc,index);
        getError();
#endif
    }
    void JCLayaGL::drawArrays(GLenum mode, GLint first, GLsizei count)
    {
        if (m_nCurrentProgram == 0 || m_nCurrentBuffer == 0)return;
        ::glDrawArrays(mode,first,count);
#ifdef DEBUG_WEBGL
        LOGI("drawArrays mode=%d,first=%d,count=%d", mode,first,count);
        getError();
#endif
    }
    void JCLayaGL::drawElements(GLenum mode, GLsizei count, GLenum type, GLintptr offset)
    {
        if (m_nCurrentProgram == 0 /* || m_nCurrentBuffer == 0 */ )return;
        ::glDrawElements(mode,count,type,(void*)offset);
#ifdef DEBUG_WEBGL
        LOGI("drawElements mode=%d,count=%d,type=%d,offset=%d", mode, count, type,offset);
        getError();
#endif
    }
    void JCLayaGL::enable(GLenum cap)
    {
        ::glEnable(cap);
#ifdef DEBUG_WEBGL
        LOGI("enable cap=%d",cap);
        getError();
#endif
    }
    void JCLayaGL::enableVertexAttribArray(GLuint index)
    {
        ::glEnableVertexAttribArray(index);
#ifdef DEBUG_WEBGL
        LOGI("enableVertexAttribArray index=%d", index);
        getError();
#endif
    }
    void JCLayaGL::finish()
    {
        ::glFinish();
#ifdef DEBUG_WEBGL
        LOGI("finish");
        getError();
#endif
    }
    void JCLayaGL::flush()
    {
        ::glFlush();
#ifdef DEBUG_WEBGL
        LOGI("flush");
        getError();
#endif
    }
    void JCLayaGL::framebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, WebGLRenderbuffer renderbuffer)
    {
        int realID = (renderbuffer == 0) ? 0 : m_pIDGenerator->getRealID(renderbuffer);
        ::glFramebufferRenderbuffer(target,attachment,renderbuffertarget, realID);
#ifdef DEBUG_WEBGL
        LOGI("framebufferRenderbuffer target=%d,attachment=%d,renderbuffertarget=%d,realID=%d,fakeID=%d",target,attachment,renderbuffertarget,realID,renderbuffer);
        getError();
#endif
    }
    void JCLayaGL::framebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, WebGLTexture texture, GLint level)
    {
        int realID = (texture == 0) ? 0 : m_pIDGenerator->getRealID(texture);
        ::glFramebufferTexture2D(target,attachment,textarget, realID,level);
#ifdef DEBUG_WEBGL
        LOGI("framebufferTexture2D target=%d,attachment=%d,textarget=%d,level=%d,realID=%d,fakeID=%d", target, attachment, textarget, level, realID, texture );
        getError();
#endif
    }
    void JCLayaGL::frontFace(GLenum mode)
    {
        ::glFrontFace(mode);
#ifdef DEBUG_WEBGL
        LOGI("frontFace mode=%d",mode);
        getError();
#endif
    }
    void JCLayaGL::generateMipmap(GLenum target)
    {
        ::glGenerateMipmap(target);
#ifdef DEBUG_WEBGL
        LOGI("generateMipmap target=%d", target);
        getError();
#endif
    }
    GLint JCLayaGL::getAttribLocation(WebGLProgram program, const std::string& name)
    {
        GLuint realProgram = m_pIDGenerator->getRealID(program);
        WebGLUniformLocation realLoc = glGetAttribLocation(realProgram, name.c_str());
#ifdef DEBUG_WEBGL
        LOGI("getAttribLocation realProgram=%d,realLoc=%d,name=%s", realProgram, realLoc, name.c_str());
        getError();
#endif
        return realLoc;
    }
    GLint JCLayaGL::getBufferParameter(GLenum target, GLenum pname)
    {
        GLint params;
        ::glGetBufferParameteriv(target,pname,&params);
#ifdef DEBUG_WEBGL
        LOGI("getBufferParameter target=%d,name=%d",target,pname);
        getError();
#endif
        return params;
    }
    GLint JCLayaGL::getParameter(GLenum pname,int fakeID)
    {
#ifdef DEBUG_WEBGL
        LOGI("getParameter name=%d", pname);
        getError();
#endif
        return 0;
    }
    GLenum JCLayaGL::getError()
    {
        GLenum nError = glGetError();
        if (nError != GL_NO_ERROR)
        {
            LOGE("GL_ERROR (0x%x)", nError);
        }
        return nError;
    }
    GLint JCLayaGL::getFramebufferAttachmentParameter(GLenum target, GLenum attachment,GLenum pname)
    {
        GLint params;
        ::glGetFramebufferAttachmentParameteriv(target,attachment,pname,&params);
#ifdef DEBUG_WEBGL
        LOGI("getFramebufferAttachmentParameter target=%d,attachment=%d,name=%d",target,attachment,pname);
        getError();
#endif
        return params;
    }                                    
    GLint JCLayaGL::getProgramParameter(WebGLProgram program, GLenum pname)
    {
        int realProgram = m_pIDGenerator->getRealID(program);
        GLint params;
        ::glGetProgramiv(realProgram,pname,&params);
#ifdef DEBUG_WEBGL
        LOGI("getProgramParameter realProgram=%d,program=%d,name=%d",realProgram,program,pname);
        getError();
#endif
        return params;
    }
    std::string JCLayaGL::getProgramInfoLog(WebGLProgram program)
    {
        int realProgram = m_pIDGenerator->getRealID(program);
        int infoLen = getShaderParameter(realProgram,GL_INFO_LOG_LENGTH);
        char *buffer = new char[infoLen];
        GLsizei length;
        ::glGetProgramInfoLog(realProgram,infoLen,&length,buffer);
        std::string result(buffer); 
        delete [] buffer;
#ifdef DEBUG_WEBGL
        LOGI("getProgramInfoLog");
        getError();
#endif
        return result;
    }
    GLint JCLayaGL::getRenderbufferParameter(GLenum target, GLenum pname)
    {
        GLint params;
        ::glGetRenderbufferParameteriv(target,pname,&params);
#ifdef DEBUG_WEBGL
        LOGI("getRenderbufferParameter target=%d,name=%d", target, pname);
        getError();
#endif
        return params;
    }
    GLint JCLayaGL::getShaderParameter(WebGLShader shader, GLenum pname)
    {
        int realShader = m_pIDGenerator->getRealID(shader);
        GLint params;
        ::glGetShaderiv(realShader,pname,&params);
#ifdef DEBUG_WEBGL
        LOGI("getShaderParameter realShader=%d,fakeShader=%d,name=%d", realShader,shader,pname);
        getError();
#endif
        return params;
    }
    std::string JCLayaGL::getShaderInfoLog(WebGLShader shader)
    {
        int realShader = m_pIDGenerator->getRealID(shader);
        GLint length;
        ::glGetShaderiv((GLuint)realShader, GL_INFO_LOG_LENGTH, &length);
        GLsizei size;
        GLchar* info = (GLchar*) malloc(length);
        if (!info)
        {
            return "";
        }
        ::glGetShaderInfoLog((GLuint)realShader, length, &size, info);
        std::string s(info);
        free(info);
#ifdef DEBUG_WEBGL
        LOGI("getShaderInfoLog");
        getError();
#endif
        return s;
    }
    std::string JCLayaGL::getShaderSource(WebGLShader shader)
    {
        int realShader = m_pIDGenerator->getRealID(shader);
        GLint length;
        ::glGetShaderiv((GLuint)realShader, GL_SHADER_SOURCE_LENGTH, &length);
        GLsizei size;
        GLchar* info = (GLchar*) malloc(length);
        if (!info)
        {
            return "";
        }
        ::glGetShaderSource((GLuint)realShader, length, &size, info);
        std::string s(info);
        free(info);
#ifdef DEBUG_WEBGL
        LOGI("getShaderSource");
        getError();
#endif
        return s;
    }
    GLint JCLayaGL::getTexParameter(GLenum target, GLenum pname)
    {
        GLint params;
        ::glGetTexParameteriv(target,pname,&params);
#ifdef DEBUG_WEBGL
        LOGI("getTexParameter target=%d,name=%d",target, pname);
        getError();
#endif
        return params;
    }
    GLint JCLayaGL::getUniform(WebGLProgram program, WebGLUniformLocation location)
    {
        int realProgram = m_pIDGenerator->getRealID(program);
        GLint value;
        ::glGetUniformiv(realProgram, location, &value);
#ifdef DEBUG_WEBGL
        LOGI("getUniform");
        getError();
#endif
        return value;
    }
    WebGLUniformLocation JCLayaGL::getUniformLocation(WebGLProgram program,int fakeLoc, const std::string& name)
    {
        GLuint realProgram = m_pIDGenerator->getRealID(program);
        WebGLUniformLocation realLoc = glGetUniformLocation(realProgram, name.c_str());
        m_pProgramLocationTable->setRealID(fakeLoc, realLoc);
#ifdef DEBUG_WEBGL
        LOGI("getUniformLocation realProgrm=%d,fakeProgram=%d,realLoc=%d,fakeLoc=%d,name=%s",realProgram,program,realLoc,fakeLoc,name.c_str());
        getError();
#endif
        return realLoc;
    }
    GLint JCLayaGL::getVertexAttrib(GLuint index, GLenum pname)
    {
        GLint value;
        ::glGetVertexAttribiv(index,pname,&value);
#ifdef DEBUG_WEBGL
        LOGI("getVertexAttrib index=%d,name=%d",index,pname);
        getError();
#endif
        return value;
    }
    GLsizeiptr JCLayaGL::getVertexAttribOffset(GLuint index, GLenum pname)
    {
        LOGW("Can't support webgl getVertexAttribOffset(...) function");
        return NULL;
    }
    void JCLayaGL::hint(GLenum target, GLenum mode)
    {
        ::glHint(target, mode);
#ifdef DEBUG_WEBGL
        LOGI("hint target=%d,mode=%d", target,mode);
        getError();
#endif
    }
    GLboolean JCLayaGL::isBuffer(WebGLBuffer buffer)
    {
#ifdef DEBUG_WEBGL
        LOGI("isBuffer");
#endif
        return ::glIsBuffer(m_pIDGenerator->getRealID(buffer));
    }
    GLboolean JCLayaGL::isEnabled(GLenum cap)
    {
#ifdef DEBUG_WEBGL
        LOGI("isEnabled cap=%d",cap);
#endif
        return ::glIsEnabled(cap);
    }
    GLboolean JCLayaGL::isFramebuffer(WebGLFramebuffer framebuffer)
    {
#ifdef DEBUG_WEBGL
        LOGI("isFramebuffer");
#endif
        return ::glIsFramebuffer(m_pIDGenerator->getRealID(framebuffer));
    }
    GLboolean JCLayaGL::isProgram(WebGLProgram program)
    {
#ifdef DEBUG_WEBGL
        LOGI("isProgram");
#endif
        return ::glIsProgram(m_pIDGenerator->getRealID(program));
    }
    GLboolean JCLayaGL::isRenderbuffer(WebGLRenderbuffer renderbuffer)
    {
#ifdef DEBUG_WEBGL
        LOGI("isRenderbuffer");
#endif
        return ::glIsRenderbuffer(m_pIDGenerator->getRealID(renderbuffer));
    }
    GLboolean JCLayaGL::isShader(WebGLShader shader)
    {
#ifdef DEBUG_WEBGL
        LOGI("isShader");
#endif
        return ::glIsShader(m_pIDGenerator->getRealID(shader));
    }
    GLboolean JCLayaGL::isTexture(WebGLTexture texture)
    {
#ifdef DEBUG_WEBGL
        LOGI("isTexture");
#endif
        return ::glIsTexture(m_pIDGenerator->getRealID(texture));
    }
    void JCLayaGL::lineWidth(GLfloat width)
    {
        ::glLineWidth(width);
#ifdef DEBUG_WEBGL
        LOGI("lineWidth width=%f", width);
        getError();
#endif
    }
    void JCLayaGL::linkProgram(WebGLProgram program)
    {
        int realID = m_pIDGenerator->getRealID(program);
        ::glLinkProgram(realID);
#ifdef DEBUG_WEBGL
        LOGI("linkProgram realProgram=%d,program=%d", realID,program);
        getError();
#endif
    }
    void JCLayaGL::pixelStorei(GLenum pname, GLint param)
    {
        switch (pname)
        {
        case GL_UNPACK_ALIGNMENT:
        case GL_PACK_ALIGNMENT:
            return;
            break;
        case 0x9240://UNPACK_FLIP_Y_WEBGL
            m_bFlipY = (param != 0);
            break;
        }
        ::glPixelStorei(pname, param);
#ifdef DEBUG_WEBGL
        LOGI("pixelStorei name=%d,param=%d", pname, param);
        getError();
#endif
    }
    void JCLayaGL::polygonOffset(GLfloat factor, GLfloat units)
    {
        ::glPolygonOffset(factor,units);
#ifdef DEBUG_WEBGL
        LOGI("polygonOffset factor=%f,units=%f", factor,units);
        getError();
#endif
    }
    void JCLayaGL::renderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
    {

        if (internalformat == GL_DEPTH_STENCIL)
        {
            internalformat = GL_DEPTH24_STENCIL8;
        }
        ::glRenderbufferStorage( target, internalformat, width, height);
#ifdef DEBUG_WEBGL
        LOGI("renderbufferStorage target=%d, internalformat=%d, width=%d, height=%d", target, internalformat, width, height);
        getError();
#endif
    }
	void JCLayaGL::renderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
	{

		if (internalformat == GL_DEPTH_STENCIL)
		{
			internalformat = GL_DEPTH24_STENCIL8;
		}
		::glRenderbufferStorageMultisample(target, samples, internalformat, width, height);
#ifdef DEBUG_WEBGL
		LOGI("renderbufferStorageMultisample target=%d, internalformat=%d, width=%d, height=%d", target, internalformat, width, height);
		getError();
#endif
	}
	void JCLayaGL::blitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
	{
		::glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
#ifdef DEBUG_WEBGL
		LOGI("blitFramebuffer srcX0=%d,  srcY0=%d  srcX1=%d  srcY1=%d  dstX0=%d  dstY0=%d  dstX1=%d  dstY1=%d  mask=%d  filter=%d", srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
		getError();
#endif
	}
    void JCLayaGL::sampleCoverage(GLclampf value, GLboolean invert)
    {
        ::glSampleCoverage(value,invert);
#ifdef DEBUG_WEBGL
        LOGI("sampleCoverage value=%f, invert=%d",value,invert);
        getError();
#endif
    }
    void JCLayaGL::scissor(GLint x, GLint y, GLsizei width, GLsizei height)
    {
        GLint nFrameBuffer = 0;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &nFrameBuffer);
        if (nFrameBuffer == m_nMainFrameBuffer)
        {
            int nCanvasHeight = this->m_nMainCanvasHeight * JCLayaGL::s_fMainCanvasScaleY;
            int nX = JCLayaGL::s_fMainCanvasTX + x * s_fMainCanvasScaleX;
            int nY = (g_nInnerHeight - JCLayaGL::s_fMainCanvasTY - nCanvasHeight) + y * s_fMainCanvasScaleY;

            ::glScissor(nX, nY, (int)(width*s_fMainCanvasScaleX), (int)(height*s_fMainCanvasScaleY));
        }
        else
        {
            ::glScissor(x , y , width , height );
        }
#ifdef DEBUG_WEBGL
        LOGI("scissor x=%d,y=%d,width=%d,height=%d",x,y,width,height);
        getError();
#endif
    }
    void JCLayaGL::shaderSource(WebGLShader shader, const std::string& source)
    {
        GLuint realShader = m_pIDGenerator->getRealID(shader);
        const char* sBuffer = source.c_str();
        int length = source.length();
        ::glShaderSource(realShader, 1, &sBuffer, &length);
#ifdef DEBUG_WEBGL
        LOGI("shaderSource realShader=%d,shader=%d,source=%s",realShader,shader, sBuffer);
        getError();
#endif
    }
    void JCLayaGL::stencilFunc(GLenum func, GLint ref, GLuint mask)
    {
        ::glStencilFunc(func, ref, mask);
#ifdef DEBUG_WEBGL
        LOGI("stencilFunc func=%d, ref=%d, mask=%d",func,ref,mask);
        getError();
#endif
    }
    void JCLayaGL::stencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
    {
        ::glStencilFuncSeparate(face, func,ref, mask);
#ifdef DEBUG_WEBGL
        LOGI("stencilFuncSeparate face=%d,func=%d,ref=%d,mask=%d",face,func,ref,mask);
        getError();
#endif
    }
    void JCLayaGL::stencilMask(GLuint mask)
    {
        ::glStencilMask(mask);
#ifdef DEBUG_WEBGL
        LOGI("stencilMask mask=%d",mask);
        getError();
#endif
    }
    void JCLayaGL::stencilMaskSeparate(GLenum face, GLuint mask)
    {
        ::glStencilMaskSeparate(face, mask);
#ifdef DEBUG_WEBGL
        LOGI("stencilMaskSeparate face=%d,mask=%d", face,mask);
        getError();
#endif
    }
    void JCLayaGL::stencilOp(GLenum fail, GLenum zfail, GLenum zpass)
    {
        ::glStencilOp(fail, zfail, zpass);
#ifdef DEBUG_WEBGL
        LOGI("stencilOp fail=%d,zfail=%d,zpass=%d", fail, zfail,zpass);
        getError();
#endif
    }
    void JCLayaGL::stencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass)
    {
        ::glStencilOpSeparate(face, fail, zfail, zpass);
#ifdef DEBUG_WEBGL
        LOGI("stencilOpSeparate face=%d,fail=%d,zfail=%d,zpass=%d", face, fail, zfail, zpass);
        getError();
#endif
    }
    void JCLayaGL::_texImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels)
    {
		if (format != GL_RGBA) {
			::glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		}
        if (pixels == NULL)
        {
            int byte = 1;
            if (type != GL_UNSIGNED_BYTE)//GL_HALF_FLOAT 等
                byte = 2;
            if (width * height * sizeof(int) * byte <= TEX_IMAGE2D_DUMMY_MAX_SIZE)
            {
                memset(m_pDummyData, 0, TEX_IMAGE2D_DUMMY_MAX_SIZE);
                ::glTexImage2D(target, level, internalformat, width, height, border, format, type, m_pDummyData);
            }
            else
            {
                char* pDummyData = new char[width * height * 4 * byte];
                memset(pDummyData, 0, width * height * 4 * byte);
                ::glTexImage2D(target, level, internalformat, width, height, border, format, type, pDummyData);
                delete[] pDummyData;
            }
        }
        else
        {			
            ::glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);			
			
        }
		if (format != GL_RGBA) {
			::glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		}
    }
	void JCLayaGL::flipY(GLenum format, GLsizei width, GLsizei height, void* pixels)
	{
		int len = 0;
		switch (format)
		{
		case GL_RGBA:
			len = width * 4;
			break;
		case GL_RGB:
			len = width * 3;
			break;
		case GL_ALPHA:
			len = width;
			break;
		default:
			break;
		}
		unsigned char* nPixels = (unsigned char*)pixels;		
		unsigned char* temp = new unsigned char[len];
		for (size_t i = 0, n = height / 2; i < n; i++)
		{
			unsigned char* p1 = nPixels + i * len;
			unsigned char* p2 = nPixels + (height - i - 1) * len;
			memcpy(temp, p1, len);
			memcpy(p1, p2, len);
			memcpy(p2, temp, len);
		}
		delete[] temp;
	}
    void JCLayaGL::texImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, void* pixels )
    {
		if (m_bFlipY)
		{
			flipY(format, width, height, pixels);
		}
        _texImage2D(target, level, internalformat, width, height, border, format, type, pixels);
#ifdef DEBUG_WEBGL
        LOGI("texImage2D target=%d,level=%d,internalformat=%d,width=%d,height=%d,border=%d,format=%d,type=%d", target, level, internalformat, width, height, border, format, type);
        getError();
#endif
    }
    void JCLayaGL::texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset,GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels )
    {
		if (m_bFlipY)
		{
			flipY(format, width, height, pixels);
		}
		if (format != GL_RGBA) 
        {
			::glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			::glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
			::glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		}
		else 
        {
			::glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
		}
#ifdef DEBUG_WEBGL
        LOGI("texSubImage2D target=%d,level=%d,xoffset=%d,yoffset=%d,width=%d,height=%d,format=%d,type=%d", target, level, xoffset, yoffset, width, height, format, type);
        getError();
#endif
    }
    void JCLayaGL::texParameterf(GLenum target, GLenum pname, GLfloat param)
    {
        ::glTexParameterf(target, pname, param);
#ifdef DEBUG_WEBGL
        LOGI("texParameterf target=%d,name=%d,param=%f",target,pname,param);
        getError();
#endif
    }
    void JCLayaGL::texParameteri(GLenum target, GLenum pname, GLint param)
    {
        ::glTexParameteri(target, pname, param);
#ifdef DEBUG_WEBGL
        LOGI("texParameteri target=%d,name=%d,param=%d", target, pname, param);
        getError();
#endif
    }
    void JCLayaGL::uniform1f(WebGLUniformLocation location, GLfloat x)
    {
        GLuint realLoc = m_pProgramLocationTable->getRealID(location);
        ::glUniform1f(realLoc,x);
#ifdef DEBUG_WEBGL
        LOGI("uniform1f realLoc=%d,fakeLoc=%d",realLoc,location);
        getError();
#endif
    }
    void JCLayaGL::uniform1fv(WebGLUniformLocation location, int byteSize, const GLfloat* v )
    {
        GLuint realLoc = m_pProgramLocationTable->getRealID(location);
        ::glUniform1fv(realLoc, byteSize,v );
#ifdef DEBUG_WEBGL
        LOGI("glUniform1fv realLoc=%d,fakeLoc=%d", realLoc, location);
        getError();
#endif
    }
    void JCLayaGL::uniform1i(WebGLUniformLocation location, GLint x)
    {
        GLuint realLoc = m_pProgramLocationTable->getRealID(location);
        ::glUniform1i(realLoc, x);
#ifdef DEBUG_WEBGL
        LOGI("uniform1i realLoc=%d,fakeLoc=%d", realLoc, location);
        getError();
#endif
    }
    void JCLayaGL::uniform1iv(WebGLUniformLocation location, int byteSize,const GLint* v )
    {
        GLuint realLoc = m_pProgramLocationTable->getRealID(location);
        ::glUniform1iv(realLoc, byteSize,v );
#ifdef DEBUG_WEBGL
        LOGI("uniform1iv realLoc=%d,fakeLoc=%d", realLoc, location);
        getError();
#endif
    }
    void JCLayaGL::uniform2f(WebGLUniformLocation location, GLfloat x, GLfloat y)
    {
        GLuint realLoc = m_pProgramLocationTable->getRealID(location);
        ::glUniform2f(realLoc, x,y);
#ifdef DEBUG_WEBGL
        LOGI("uniform2f realLoc=%d,fakeLoc=%d", realLoc, location);
        getError();
#endif
    }
    void JCLayaGL::uniform2fv(WebGLUniformLocation location, int byteSize, const GLfloat* v )
    {
        GLuint realLoc = m_pProgramLocationTable->getRealID(location);
        ::glUniform2fv(realLoc, byteSize,v );
#ifdef DEBUG_WEBGL
        LOGI("glUniform2fv realLoc=%d,fakeLoc=%d", realLoc, location);
        getError();
#endif
    }
    void JCLayaGL::uniform2i(WebGLUniformLocation location, GLint x, GLint y)
    {
        GLuint realLoc = m_pProgramLocationTable->getRealID(location);
        ::glUniform2i(realLoc, x ,y);
#ifdef DEBUG_WEBGL
        LOGI("uniform2i realLoc=%d,fakeLoc=%d", realLoc, location);
        getError();
#endif
    }
    void JCLayaGL::uniform2iv(WebGLUniformLocation location, int byteSize, const GLint* v)
    {
        GLuint realLoc = m_pProgramLocationTable->getRealID(location);
        ::glUniform2iv(realLoc, byteSize,v );
#ifdef DEBUG_WEBGL
        LOGI("uniform2iv realLoc=%d,fakeLoc=%d", realLoc, location);
        getError();
#endif
    }
    void JCLayaGL::uniform3f(WebGLUniformLocation location, GLfloat x, GLfloat y, GLfloat z)
    {
        GLuint realLoc = m_pProgramLocationTable->getRealID(location);
        ::glUniform3f(realLoc, x, y, z);
#ifdef DEBUG_WEBGL
        LOGI("uniform3f realLoc=%d,fakeLoc=%d", realLoc, location);
        getError();
#endif
    }
    void JCLayaGL::uniform3fv(WebGLUniformLocation location, int byteSize, const GLfloat* v )
    {
        GLuint realLoc = m_pProgramLocationTable->getRealID(location);
        ::glUniform3fv(realLoc, byteSize,v );
#ifdef DEBUG_WEBGL
        LOGI("uniform3fv realLoc=%d,fakeLoc=%d", realLoc, location);
        getError();
#endif
    }
    void JCLayaGL::uniform3i(WebGLUniformLocation location, GLint x, GLint y, GLint z)
    {
        GLuint realLoc = m_pProgramLocationTable->getRealID(location);
        ::glUniform3i(realLoc,x, y, z);
#ifdef DEBUG_WEBGL
        LOGI("uniform3i realLoc=%d,fakeLoc=%d", realLoc, location);
        getError();
#endif
    }
    void JCLayaGL::uniform3iv(WebGLUniformLocation location, int byteSize, const GLint* v )
    {
        GLuint realLoc = m_pProgramLocationTable->getRealID(location);
        ::glUniform3iv(realLoc, byteSize,v );
#ifdef DEBUG_WEBGL
        LOGI("uniform3iv realLoc=%d,fakeLoc=%d", realLoc, location);
        getError();
#endif
    }
    void JCLayaGL::uniform4f(WebGLUniformLocation location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
    {
        GLuint realLoc = m_pProgramLocationTable->getRealID(location);
        ::glUniform4f(realLoc, x, y, z, w);
#ifdef DEBUG_WEBGL
        LOGI("uniform4f realLoc=%d,fakeLoc=%d", realLoc, location);
        getError();
#endif
    }
    void JCLayaGL::uniform4fv(WebGLUniformLocation location, int byteSize, const GLfloat* v )
    {
        GLuint realLoc = m_pProgramLocationTable->getRealID(location);
        ::glUniform4fv(realLoc, byteSize,v );
#ifdef DEBUG_WEBGL
        LOGI("uniform4fv realLoc=%d,fakeLoc=%d", realLoc, location);
        getError();
#endif
    }
    void JCLayaGL::uniform4i(WebGLUniformLocation location, GLint x, GLint y, GLint z, GLint w)
    {
        GLuint realLoc = m_pProgramLocationTable->getRealID(location);
        ::glUniform4i(realLoc, x, y, z, w);
#ifdef DEBUG_WEBGL
        LOGI("uniform4i realLoc=%d,fakeLoc=%d", realLoc, location);
        getError();
#endif
    }
    void JCLayaGL::uniform4iv(WebGLUniformLocation location, int byteSize, const GLint* v )
    {
        GLuint realLoc = m_pProgramLocationTable->getRealID(location);
        ::glUniform4iv(realLoc, byteSize,v );
#ifdef DEBUG_WEBGL
        LOGI("uniform4iv realLoc=%d,fakeLoc=%d", realLoc, location);
        getError();
#endif
    }
    void JCLayaGL::uniformMatrix2fv(WebGLUniformLocation location, int byteSize,GLboolean transpose,GLfloat* value)
    {
        GLuint realLoc = m_pProgramLocationTable->getRealID(location);
        ::glUniformMatrix2fv(realLoc, byteSize,transpose,value);
#ifdef DEBUG_WEBGL
        LOGI("uniformMatrix2fv realLoc=%d,fakeLoc=%d", realLoc, location);
        getError();
#endif
    }                        
    void JCLayaGL::uniformMatrix3fv(WebGLUniformLocation location, int byteSize,GLboolean transpose,GLfloat* value)
    {
        GLuint realLoc = m_pProgramLocationTable->getRealID(location);
        ::glUniformMatrix3fv(realLoc, byteSize,transpose,value);
#ifdef DEBUG_WEBGL
        LOGI("uniformMatrix3fv realLoc=%d,fakeLoc=%d", realLoc, location);
        getError();
#endif
    }                         
    void JCLayaGL::uniformMatrix4fv(WebGLUniformLocation location, int byteSize,GLboolean transpose,GLfloat* value)
    {
        GLuint realLoc = m_pProgramLocationTable->getRealID(location);
        ::glUniformMatrix4fv(realLoc, byteSize,transpose,value);
#ifdef DEBUG_WEBGL
        LOGI("uniformMatrix4fv realLoc=%d,fakeLoc=%d", realLoc, location);
        getError();
#endif
    }                    
    void JCLayaGL::useProgram(WebGLProgram program) 
    {
        int realID = m_pIDGenerator->getRealID(program);
        m_nCurrentProgram = realID;
        ::glUseProgram(realID);
#ifdef DEBUG_WEBGL
        LOGI("useProgram realProgram=%d,fakeProgram=%d", realID, program);
        getError();
#endif
    }
    void JCLayaGL::validateProgram(WebGLProgram program)
    {
        int realID = m_pIDGenerator->getRealID(program);
        ::glValidateProgram(realID);
#ifdef DEBUG_WEBGL
        LOGI("validateProgram realProgram=%d,fakeProgram=%d", realID, program);
        getError();
#endif
    }
    void JCLayaGL::vertexAttrib1f(GLuint index, GLfloat x)
    {
        GLuint realLoc = m_pProgramLocationTable->getRealID(index);
        ::glVertexAttrib1f(realLoc,x);
#ifdef DEBUG_WEBGL
        LOGI("vertexAttrib1f realLoc=%d,index=%d", realLoc, index);
        getError();
#endif
    }
    void JCLayaGL::vertexAttrib1fv(GLuint index, GLfloat* values)
    {
        GLuint realLoc = m_pProgramLocationTable->getRealID(index);
        ::glVertexAttrib1fv(realLoc,values);
#ifdef DEBUG_WEBGL
        LOGI("vertexAttrib1fv realLoc=%d,index=%d", realLoc, index);
        getError();
#endif
    }
    void JCLayaGL::vertexAttrib2f(GLuint index, GLfloat x, GLfloat y)
    {
        GLuint realLoc = m_pProgramLocationTable->getRealID(index);
        ::glVertexAttrib2f(realLoc,x,y);
#ifdef DEBUG_WEBGL
        LOGI("vertexAttrib2f realLoc=%d,index=%d", realLoc, index);
        getError();
#endif
    }
    void JCLayaGL::vertexAttrib2fv(GLuint index, GLfloat* values)
    {
        GLuint realLoc = m_pProgramLocationTable->getRealID(index);
        glVertexAttrib2fv(realLoc,values);
#ifdef DEBUG_WEBGL
        LOGI("vertexAttrib2fv realLoc=%d,index=%d", realLoc, index);
        getError();
#endif
    }
    void JCLayaGL::vertexAttrib3f(GLuint index, GLfloat x, GLfloat y, GLfloat z)
    {
        GLuint realLoc = m_pProgramLocationTable->getRealID(index);
        ::glVertexAttrib3f(realLoc,x,y,z);
#ifdef DEBUG_WEBGL
        LOGI("vertexAttrib3f realLoc=%d,index=%d", realLoc, index);
        getError();
#endif
    }
    void JCLayaGL::vertexAttrib3fv(GLuint index, GLfloat* values)
    {
        GLuint realLoc = m_pProgramLocationTable->getRealID(index);
        ::glVertexAttrib3fv(realLoc,values);
#ifdef DEBUG_WEBGL
        LOGI("vertexAttrib3fv realLoc=%d,index=%d", realLoc, index);
        getError();
#endif
    }
    void JCLayaGL::vertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
    {
        GLuint realLoc = m_pProgramLocationTable->getRealID(index);
        ::glVertexAttrib4f(realLoc, x, y, z, w);
#ifdef DEBUG_WEBGL
        LOGI("vertexAttrib4f realLoc=%d,index=%d", realLoc, index);
        getError();
#endif
    }
    void JCLayaGL::vertexAttrib4fv(GLuint index, GLfloat *values)
    {
        GLuint realLoc = m_pProgramLocationTable->getRealID(index);
        ::glVertexAttrib4fv(realLoc,values);
#ifdef DEBUG_WEBGL
        LOGI("glVertexAttrib4fv realLoc=%d,index=%d", realLoc, index);
        getError();
#endif
    }
    void JCLayaGL::vertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset)
    {
        ::glVertexAttribPointer(index,size,type,normalized,stride,(void*)offset);
#ifdef DEBUG_WEBGL
        LOGI("vertexAttribPointer index=%d,size=%d,type=%d,normalized=%d,stride=%d", index,size, type, normalized, stride);
        getError();
#endif
    }
    void JCLayaGL::viewport(GLint x, GLint y, GLsizei width, GLsizei height)
    {
        if (width == 0 || height == 0)
        {
            return;
        }
        GLint nFrameBuffer = 0;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &nFrameBuffer);
        if (nFrameBuffer == m_nMainFrameBuffer)
        {
            int nCanvasHeight = this->m_nMainCanvasHeight * JCLayaGL::s_fMainCanvasScaleY;
            int nX = JCLayaGL::s_fMainCanvasTX + x * s_fMainCanvasScaleX;
            int nY = (g_nInnerHeight - JCLayaGL::s_fMainCanvasTY - nCanvasHeight) + y * s_fMainCanvasScaleY;

            ::glViewport(nX, nY, (int)(width*s_fMainCanvasScaleX), (int)(height*s_fMainCanvasScaleY));
        }
        else
        {
            ::glViewport(x, y, width,height);
        }
//#ifdef __APPLE__
//        saveViewPortRect(x,y,width,height);
//#endif
#ifdef DEBUG_WEBGL
        LOGI("viewport x=%d,y=%d,width=%d,height=%d",x,y,width,height);
        getError();
#endif
    }
    int JCLayaGL::createVertexArray(int fakeID)
    {
        GLuint vao = 0;
        _createVertexArray(vao);
        m_vAllVAO[vao] = true;
        m_pIDGenerator->setRealID(fakeID, vao);
#ifdef DEBUG_WEBGL
        LOGI("createVertextArray realBuffer=%d,fakeID=%d", vao, fakeID);
        getError();
#endif
        return vao;
    }
    void JCLayaGL::bindVertexArray(int fakeID)
    {
        int realID = (fakeID == 0) ? 0 : m_pIDGenerator->getRealID(fakeID);
        _bindVertexArray(realID);
#ifdef DEBUG_WEBGL
        LOGI("bindVertextArray realBuffer=%d,fakeBuffer=%d", realID, fakeID);
        getError();
#endif
    }
    void JCLayaGL::deleteVertexArray(int fakeID)
    {
        GLuint realID = (fakeID == 0) ? 0 : m_pIDGenerator->getRealID(fakeID);
        _deleteVertexArray(realID);
        removeVAOFromMap(realID);
#ifdef DEBUG_WEBGL
        LOGI("deleteVertextArray realBuffer=%d,fakeBuffer=%d", realID, fakeID);
        getError();
#endif
    }
	void JCLayaGL::_createVertexArray(GLuint& vao)
	{
        glGenVertexArrays(1, &vao);
	}
	void JCLayaGL::_bindVertexArray(GLuint vao)
	{
        glBindVertexArray(vao);
	}
	void JCLayaGL::_deleteVertexArray(GLuint& vao)
	{
        glDeleteVertexArrays(1, &vao);
	}
    void JCLayaGL::vertexAttribDivisor(GLuint index, GLuint divisor)
    {
        if (!s_nGLCaps&(1 << 6))
            return;
        glVertexAttribDivisor(index, divisor);
    }
    void JCLayaGL::drawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei primcount)
    {
        if (!s_nGLCaps&(1 << 6))
            return;
        glDrawArraysInstanced(mode, first, count, primcount);
    }
    void JCLayaGL::drawElementsInstanced(GLenum mode, GLsizei count, GLenum type, GLintptr indices, GLsizei primcount)
    {
        if (!s_nGLCaps&(1 << 6))
            return;
        glDrawElementsInstanced(mode, count, type, (const void*)indices, primcount);
    }
    void JCLayaGL::readPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels)
    {
        ::glReadPixels(x, y, width, height, format, type, (GLvoid*)pixels); 
#ifdef DEBUG_WEBGL
        LOGI("readPixels x=%d,y=%d,width=%d,height=%d,format=%d,type=%d", x, y, width, height, format, type);
        getError();
#endif
    }
    void JCLayaGL::useCommandEncoder(int nCmdSetID)
    {
        JCArrayBufferManager::ArrayBufferContent* pCmd = m_pArrayBufferManager->getArrayBuffer(nCmdSetID);
		JCLayaGLDispatch::dispatchAllCmds(pCmd->getCommandEncoderBuffer());
    }
    void JCLayaGL::deleteAllGLRes()
    {
        m_bFlipY = false;
        m_nCurrentBuffer = 0;
        m_nCurrentProgram = 0;
        for (auto texture : m_vAllTexture) 
        {
            ::glDeleteTextures(1, &(texture.first));
        }
        m_vAllTexture.clear();

        for (auto frambuffer : m_vAllFrameBuffer) 
        {
            ::glDeleteFramebuffers(1, &(frambuffer.first));
        }
        m_vAllFrameBuffer.clear();

        for (auto program : m_vAllProgram) 
        {
            ::glDeleteProgram(program.first);
        }
        m_vAllProgram.clear();

        for (auto s : m_vAllShader) 
        {
            ::glDeleteShader(s.first);
        }
        m_vAllShader.clear();

        for (auto renderBuffer : m_vAllRenderBuffer) 
        {
            ::glDeleteRenderbuffers(1, &(renderBuffer.first));
        }
        m_vAllRenderBuffer.clear();

        for (auto buffer : m_vAllBuffer) 
        {
            ::glDeleteBuffers(1, &(buffer.first));
        }
        m_vAllBuffer.clear();

        for (auto vao : m_vAllVAO)
        {
            glDeleteVertexArrays(1, &(vao.first));
        }
        m_vAllVAO.clear();
    }
    void JCLayaGL::removeTextureFromMap(GLuint texture)
    {
        std::unordered_map<GLuint, bool>::iterator iter = m_vAllTexture.find(texture);
        if (iter != m_vAllTexture.end())
        {
            m_vAllTexture.erase(iter);
        }
    }
    void JCLayaGL::removeFrameBufferFromMap(GLuint frameBuffer)
    {
        std::unordered_map<GLuint, bool>::iterator iter = m_vAllFrameBuffer.find(frameBuffer);
        if (iter != m_vAllFrameBuffer.end())
        {
            m_vAllFrameBuffer.erase(iter);
        }
    }
    void JCLayaGL::removeProgramFromMap(GLuint program)
    {
        std::unordered_map<GLuint, bool>::iterator iter = m_vAllProgram.find(program);
        if (iter != m_vAllProgram.end())
        {
            m_vAllProgram.erase(iter);
        }
    }
    void JCLayaGL::removeShaderFromMap(GLuint shader)
    {
        std::unordered_map<GLuint, bool>::iterator iter = m_vAllShader.find(shader);
        if (iter != m_vAllShader.end())
        {
            m_vAllShader.erase(iter);
        }
    }
    void JCLayaGL::removeRenderBufferFromMap(GLuint renderBuffer)
    {
        std::unordered_map<GLuint, bool>::iterator iter = m_vAllRenderBuffer.find(renderBuffer);
        if (iter != m_vAllRenderBuffer.end())
        {
            m_vAllRenderBuffer.erase(iter);
        }
    }
    void JCLayaGL::removeBufferFromMap(GLuint buffer)
    {
        std::unordered_map<GLuint, bool>::iterator iter = m_vAllBuffer.find(buffer);
        if (iter != m_vAllBuffer.end())
        {
            m_vAllBuffer.erase(iter);
        }
    }
    void JCLayaGL::removeVAOFromMap(GLuint VAO)
    {
        std::unordered_map<GLuint, bool>::iterator iter = m_vAllVAO.find(VAO);
        if (iter != m_vAllVAO.end())
        {
            m_vAllVAO.erase(iter);
        }
    }
    void JCLayaGL::readPixelsAsync(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, int callbackObjID, int funcID)
    {
        _readPixelsAsync(x, y, width, height, format, type, callbackObjID, funcID);
    }
    void JCLayaGL::_readPixelsAsync(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, int callbackObjID, int funcID)
    {
        if (x < 0 || y < 0 || width <= 0 || height <= 0)
            return;

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

        unsigned char* pPixels = new unsigned char[bytes];
        //JCLayaGL::finish();
        //JCLayaGL::pixelStorei(GL_PACK_ALIGNMENT, 1);
        JCLayaGL::readPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pPixels);
        //JCLayaGL::pixelStorei(GL_UNPACK_ALIGNMENT, 4);

        JCConchBridge::getPixelsRenderToJS(pPixels, bytes, width, height, callbackObjID, funcID);
    }
    void JCLayaGL::texStorage2D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
    {
        ::glTexStorage2D(target, levels, internalformat, width, height);
#ifdef DEBUG_WEBGL
        LOGI("glTexStorage2D");
        getError();
#endif
    }
    void JCLayaGL::texImage2DCanvas(GLsizei width, GLsizei height, int canvas)
    {
        //todo 其他格式 预乘alpha
        GLint last_texture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
#ifdef DEBUG_WEBGL
        getError();
#endif
        static std::vector<unsigned char> cbuffer;
        int pixelSize = width * height * 4;
        cbuffer.resize(pixelSize);

        int src = m_pIDGenerator->getRealID(canvas);


        GLint previousFrameBuffer;
        GLint previousRenderBuffer;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFrameBuffer);
        glGetIntegerv(GL_RENDERBUFFER_BINDING, &previousRenderBuffer);
#ifdef DEBUG_WEBGL
        getError();
#endif

        GLuint viewFrameBuffer = 0;
        glGenFramebuffers(1, &viewFrameBuffer);
#ifdef DEBUG_WEBGL
        getError();
#endif
        glBindFramebuffer(GL_FRAMEBUFFER, viewFrameBuffer);
#ifdef DEBUG_WEBGL
        getError();
#endif
        GLuint viewRenderBuffer = 0;
        glGenRenderbuffers(1, &viewRenderBuffer);
#ifdef DEBUG_WEBGL
        getError();
#endif
        glBindRenderbuffer(GL_RENDERBUFFER, viewRenderBuffer);
#ifdef DEBUG_WEBGL
        getError();
#endif
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, src, 0);
#ifdef DEBUG_WEBGL
        getError();
#endif
        GLint preUnpackAlignment;
        glGetIntegerv(GL_UNPACK_ALIGNMENT, &preUnpackAlignment);
#ifdef DEBUG_WEBGL
        getError();
#endif
        ::glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
#ifdef DEBUG_WEBGL
        getError();
#endif
        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, (void*)cbuffer.data());
#ifdef DEBUG_WEBGL
        getError();
#endif
        ::glPixelStorei(GL_UNPACK_ALIGNMENT, preUnpackAlignment);
#ifdef DEBUG_WEBGL
        getError();
#endif
        //webgl标准依据？
        flipY(GL_RGBA, width, height, cbuffer.data());

        texImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, cbuffer.data());
#ifdef DEBUG_WEBGL
        getError();
#endif
        glDeleteRenderbuffers(1, &viewRenderBuffer);
#ifdef DEBUG_WEBGL
        getError();
#endif
        glDeleteFramebuffers(1, &viewFrameBuffer);
#ifdef DEBUG_WEBGL
        getError();
#endif
        glBindFramebuffer(GL_FRAMEBUFFER, previousFrameBuffer);
#ifdef DEBUG_WEBGL
        getError();
#endif
        glBindRenderbuffer(GL_RENDERBUFFER, previousRenderBuffer);
#ifdef DEBUG_WEBGL
        getError();
#endif
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
