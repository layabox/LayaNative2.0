/**
@file			JCLayaGL.h
@brief			
@author			James
@version		1.0
@date			2018_5_25
*/

#ifndef __JCLayaGL_H__
#define __JCLayaGL_H__

#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#if __APPLE__
    #include <OpenGLES/ES3/gl.h>
#else
    #include <GLES3/gl3.h>
#endif
#include "../Image/JCImageManager.h"
#include "JCIDGenerator.h"
#include <manager/JCArrayBufferManager.h>
#include "../RenderEx/JCGlobalValue.h"
#include "../Manager/JCOrderResManager.h"
#include "../RenderEx/JCRegister.h"
#include <buffer/JCBuffer.h>

//1024*1024*4
#define  TEX_IMAGE2D_DUMMY_MAX_SIZE 4194304


namespace laya
{
    typedef GLuint WebGLShader;
    typedef GLuint WebGLProgram;
    typedef GLuint WebGLBuffer;
    typedef GLuint WebGLFramebuffer;
    typedef GLuint WebGLRenderbuffer;
    typedef GLuint WebGLTexture;
    typedef GLuint WebGLUniformLocation;
    class JCWebGLPlus;
    /** 
     * @brief
    */
    class JCLayaGL
    {
    public:
        JCLayaGL(int nWidth,int nHeight,JCArrayBufferManager* pArrayBuferManager, JCImageManager* pImageManager,JCIDGenerator* pIDGenerator,JCIDGenerator* pProgramLocationTable,JCRegister* pRegister,JCWebGLPlus* pWebGLPlus);
        static void _texImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
        static void _createVertexArray(GLuint& vao);
        static void _bindVertexArray(GLuint vao);
        static void _deleteVertexArray(GLuint& vao);
        virtual ~JCLayaGL();
        virtual bool isContextLost();
        virtual void* getExtension(const std::string& name);
        virtual void activeTexture(GLenum texture);
        virtual void attachShader(WebGLProgram program, WebGLShader shader);
        virtual void bindAttribLocation(WebGLProgram program, GLuint index, const std::string& name);
        virtual void bindBuffer(GLenum target, WebGLBuffer buffer);
        virtual void bindFramebuffer(GLenum target, WebGLFramebuffer framebuffer);
        virtual void bindRenderbuffer(GLenum target, WebGLRenderbuffer renderbuffer);
        virtual void bindTexture(GLenum target, WebGLTexture texture);
        virtual void blendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
        virtual void blendEquation(GLenum mode);
        virtual void blendEquationSeparate(GLenum modeRGB, GLenum modeAlpha);
        virtual void blendFunc(GLenum sfactor, GLenum dfactor);
        virtual void blendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
        virtual void bufferData_size(GLenum target, GLsizeiptr size, GLenum usage);
        virtual void bufferData(GLenum target, int dataSize, char* data, GLenum usage);
        virtual void bufferSubData(GLenum target, GLintptr offset,int dataSize, char* data);
        virtual GLenum checkFramebufferStatus(GLenum target);
        virtual void clear(GLbitfield mask);
        virtual void clearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
        virtual void clearDepth(GLclampf depth);
        virtual void clearStencil(GLint s);
        virtual void colorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
        virtual void compileShader(WebGLShader shader);
        virtual void compressedTexImage2D(GLenum target, GLint level, GLenum internalformat,GLsizei width, GLsizei height, GLint border,int dataSize,char* data);
        virtual void compressedTexSubImage2D(GLenum target, GLint level,GLint xoffset, GLint yoffset,GLsizei width, GLsizei height, GLenum format, int dataSize,char* data );
        virtual void copyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
        virtual void copyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
        virtual WebGLBuffer createBuffer(int fakeID);
        virtual WebGLFramebuffer createFramebuffer(int fakeID);
        virtual WebGLProgram createProgram(int fakeID);
        virtual WebGLRenderbuffer createRenderbuffer(int fakeID);
        virtual WebGLShader createShader(int fakeID,GLenum type);
        virtual WebGLTexture createTexture(int fakeID);
        virtual void cullFace(GLenum mode);
        virtual void deleteBuffer(WebGLBuffer buffer);
        virtual void deleteFramebuffer(WebGLFramebuffer framebuffer);
        virtual void deleteProgram(WebGLProgram program);
        virtual void deleteRenderbuffer(WebGLRenderbuffer renderbuffer);
        virtual void deleteShader(WebGLShader shader);
        virtual void deleteTexture(WebGLTexture texture);
        virtual void depthFunc(GLenum func);
        virtual void depthMask(GLboolean flag);
        virtual void depthRange(GLclampf zNear, GLclampf zFar);
        virtual void detachShader(WebGLProgram program, WebGLShader shader);
        virtual void disable(GLenum cap);
        virtual void disableVertexAttribArray(GLuint index);
        virtual void drawArrays(GLenum mode, GLint first, GLsizei count);
        virtual void drawElements(GLenum mode, GLsizei count, GLenum type, GLintptr offset);
        virtual void enable(GLenum cap);
        virtual void enableVertexAttribArray(GLuint index);
        virtual void finish();
        virtual void flush();
        virtual void framebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, WebGLRenderbuffer renderbuffer);
        virtual void framebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, WebGLTexture texture, GLint level);
        virtual void frontFace(GLenum mode);
        virtual void generateMipmap(GLenum target);
        virtual GLint getAttribLocation(WebGLProgram program,const std::string& name);
        virtual GLint getBufferParameter(GLenum target, GLenum pname);
        virtual GLint getParameter(GLenum pname,int fakeID);
        virtual GLenum getError();
        virtual GLint getFramebufferAttachmentParameter(GLenum target, GLenum attachment,GLenum pname);
        virtual GLint getProgramParameter(WebGLProgram program, GLenum pname);
        virtual std::string getProgramInfoLog(WebGLProgram program);
        virtual GLint getRenderbufferParameter(GLenum target, GLenum pname);
        virtual GLint getShaderParameter(WebGLShader shader, GLenum pname);
        virtual std::string getShaderInfoLog(WebGLShader shader);
        virtual std::string getShaderSource(WebGLShader shader);
        virtual GLint getTexParameter(GLenum target, GLenum pname);
        virtual GLint getUniform(WebGLProgram program, WebGLUniformLocation location);
        virtual WebGLUniformLocation getUniformLocation(WebGLProgram program, int fakeLoc,const std::string& name);
        virtual GLint getVertexAttrib(GLuint index, GLenum pname);
        virtual GLsizeiptr getVertexAttribOffset(GLuint index, GLenum pname);
        virtual void hint(GLenum target, GLenum mode);
        virtual GLboolean isBuffer(WebGLBuffer buffer);
        virtual GLboolean isEnabled(GLenum cap);
        virtual GLboolean isFramebuffer(WebGLFramebuffer framebuffer);
        virtual GLboolean isProgram(WebGLProgram program);
        virtual GLboolean isRenderbuffer(WebGLRenderbuffer renderbuffer);
        virtual GLboolean isShader(WebGLShader shader);
        virtual GLboolean isTexture(WebGLTexture texture);
        virtual void lineWidth(GLfloat width);
        virtual void linkProgram(WebGLProgram program);
        virtual void pixelStorei(GLenum pname, GLint param);
        virtual void polygonOffset(GLfloat factor, GLfloat units);
        virtual void readPixelsAsync(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, int callbackObjID, int funcID);
        virtual void renderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
        virtual void sampleCoverage(GLclampf value, GLboolean invert);
        virtual void scissor(GLint x, GLint y, GLsizei width, GLsizei height);
        virtual void shaderSource(WebGLShader shader, const std::string& source);
        virtual void stencilFunc(GLenum func, GLint ref, GLuint mask);
        virtual void stencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask);
        virtual void stencilMask(GLuint mask);
        virtual void stencilMaskSeparate(GLenum face, GLuint mask);
        virtual void stencilOp(GLenum fail, GLenum zfail, GLenum zpass);
        virtual void stencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass);
        virtual void texImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, void* pixels);
        virtual void texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset,GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels);
        virtual void texParameterf(GLenum target, GLenum pname, GLfloat param);
        virtual void texParameteri(GLenum target, GLenum pname, GLint param);
        virtual void uniform1f(WebGLUniformLocation location, GLfloat x);
        virtual void uniform1fv(WebGLUniformLocation location, int byteSize, const GLfloat* v );
        virtual void uniform1i(WebGLUniformLocation location, GLint x);
        virtual void uniform1iv(WebGLUniformLocation location, int byteSize, const GLint* v );
        virtual void uniform2f(WebGLUniformLocation location, GLfloat x, GLfloat y);
        virtual void uniform2fv(WebGLUniformLocation location, int byteSize, const GLfloat* v );
        virtual void uniform2i(WebGLUniformLocation location, GLint x, GLint y);
        virtual void uniform2iv(WebGLUniformLocation location, int byteSize, const GLint* v );
        virtual void uniform3f(WebGLUniformLocation location, GLfloat x, GLfloat y, GLfloat z);
        virtual void uniform3fv(WebGLUniformLocation location, int byteSize, const GLfloat* v);
        virtual void uniform3i(WebGLUniformLocation location, GLint x, GLint y, GLint z);
        virtual void uniform3iv(WebGLUniformLocation location, int byteSize, const GLint* v );
        virtual void uniform4f(WebGLUniformLocation location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
        virtual void uniform4fv(WebGLUniformLocation location, int byteSize, const GLfloat* v );
        virtual void uniform4i(WebGLUniformLocation location, GLint x, GLint y, GLint z, GLint w);
        virtual void uniform4iv(WebGLUniformLocation location, int byteSize, const GLint* v);
        virtual void uniformMatrix2fv(WebGLUniformLocation location, int byteSize,GLboolean transpose,GLfloat* value);
        virtual void uniformMatrix3fv(WebGLUniformLocation location, int byteSize,GLboolean transpose,GLfloat* value);
        virtual void uniformMatrix4fv(WebGLUniformLocation location, int byteSize, GLboolean transpose,GLfloat* value);
        virtual void useProgram(WebGLProgram program);
        virtual void validateProgram(WebGLProgram program);
        virtual void vertexAttrib1f(GLuint index, GLfloat x);
        virtual void vertexAttrib1fv(GLuint index, GLfloat* values);
        virtual void vertexAttrib2f(GLuint index, GLfloat x, GLfloat y);
        virtual void vertexAttrib2fv(GLuint index, GLfloat* values);
        virtual void vertexAttrib3f(GLuint index, GLfloat x, GLfloat y, GLfloat z);
        virtual void vertexAttrib3fv(GLuint index, GLfloat* values);
        virtual void vertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
        virtual void vertexAttrib4fv(GLuint index, GLfloat* values);
        virtual void vertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset);
        virtual void viewport(GLint x, GLint y, GLsizei width, GLsizei height);
        virtual void readPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels);
        virtual int createVertexArray(int fakeID);
        virtual void bindVertexArray(int fakeID);
        virtual void deleteVertexArray(int fakeID);
        virtual void vertexAttribDivisor(GLuint index, GLuint divisor);
        virtual void drawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei primcount);
        virtual void drawElementsInstanced(GLenum mode, GLsizei count, GLenum type, GLintptr indices, GLsizei primcount);
		virtual void texImage2DCanvas(GLsizei width, GLsizei height, int canvas);
        virtual void texStorage2D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
		virtual void renderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
		virtual void clearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat* value);
		virtual void blitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        virtual void useCommandEncoder(int nCmdSetID);
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
    public:
        virtual void deleteAllGLRes();
        virtual void removeTextureFromMap(GLuint texture);
        virtual void removeFrameBufferFromMap(GLuint frameBuffer);
        virtual void removeProgramFromMap(GLuint program);
        virtual void removeShaderFromMap(GLuint shader);
        virtual void removeRenderBufferFromMap(GLuint renderBuffer);
        virtual void removeBufferFromMap(GLuint buffer);
        virtual void removeVAOFromMap(GLuint VAO);
    public:
        void _readPixelsAsync(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, int callbackObjID, int funcID);
    private:
        void flipY(GLenum format, GLsizei width, GLsizei height, void* pixels);
    public:
        static char                             m_pDummyData[TEX_IMAGE2D_DUMMY_MAX_SIZE]; 
    public:
        JCWebGLPlus*                        m_pWebGLPlus;
        JCImageManager*                     m_pImageManager;
        JCArrayBufferManager*               m_pArrayBufferManager;
		JCRegister*							m_pRegister;
    protected:
        GLint                               m_nMainFrameBuffer;
        JCIDGenerator*                      m_pIDGenerator;
        JCIDGenerator*                      m_pProgramLocationTable;
        bool                                m_bFlipY;
    public:
        int                                 m_nCurrentBuffer;
        int                                 m_nCurrentProgram;
        static      float                   s_fMainCanvasScaleX;
        static      float                   s_fMainCanvasScaleY;
        static      float                   s_fMainCanvasTX;
        static      float                   s_fMainCanvasTY;
		int									m_nMainCanvasWidth;
		int									m_nMainCanvasHeight;
    private:
        std::unordered_map<GLuint, bool>    m_vAllTexture;
        std::unordered_map<GLuint, bool>    m_vAllFrameBuffer;;
        std::unordered_map<GLuint, bool>    m_vAllProgram;
        std::unordered_map<GLuint, bool>    m_vAllShader;
        std::unordered_map<GLuint, bool>    m_vAllRenderBuffer;
        std::unordered_map<GLuint, bool>    m_vAllBuffer;
        std::unordered_map<GLuint, bool>    m_vAllVAO;
    };
}
//------------------------------------------------------------------------------


#endif //__JCLayaGL_H__

//-----------------------------END FILE--------------------------------