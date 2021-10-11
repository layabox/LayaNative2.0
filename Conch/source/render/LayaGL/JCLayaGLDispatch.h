/**
@file			JCLayaGLDispatch.h
@brief			
@author			James
@version		1.0
@date			2018_5_25
*/

#ifndef __JCLayaGLDispatch_H__
#define __JCLayaGLDispatch_H__

#include <stdlib.h>
#include "JCLayaGL.h"
#include <buffer/JCBuffer.h>

//#define DEBUG_PARSE_STREAM 0

namespace laya
{
    enum LAYA_FUNCTION_ID
    {
        LAYA_UNIFORMMATRIX2FVEX = 0,
        LAYA_UNIFORMMATRIX3FVEX,
        LAYA_UNIFORMMATRIX4FVEX,
        LAYA_ADDSHADERUNIFORM,
        LAYA_UPLOADSHADERUNIFORMS,
        LAYA_UPLOADSHADERUNIFORMS_BUFFER,
        LAYA_USECOMMANDENCODER,
        LAYA_LOADDATATOREG,
        LAYA_LOADDATATOREGEX,
        LAYA_IFLESS0,
        LAYA_IFEQUAL0,
        LAYA_IFGREATER0,
        LAYA_IFLEQUAL0,
        LAYA_IFGEQUAL0,
        LAYA_IFGNOTEQUAL0,
        LAYA_OPERATEREG,
        LAYA_STORE,
		LAYA_CREATE_IMAGE_ON_RENDER_THREAD = 64,
		LAYA_DELETE_IMAGE_ON_RENDER_THREAD,
		LAYA_RELEASE_IMAGE_ON_RENDER_THREAD,
		LAYA_SET_IMAGE_RELEASE_SPACE_TIME,
		LAYA_SET_PREMULTIPLY_ALPHA,
		LAYA_PERFADDDATA,
		LAYA_PERFUPDATEDT,
		LAYA_SET_MAIN_CONTEXT_SIZE,

        //-------------------------------
        LAYA_GETCONTEXTATTRIBUTES = 128,
        LAYA_ISCONTEXTLOST,
        LAYA_GETSUPPORTEDEXTENSIONS,
        LAYA_GETEXTENSION,
        LAYA_ACTIVETEXTURE,
        LAYA_ATTACHSHADER,
        LAYA_BINDATTRIBLOCATION,
        LAYA_BINDBUFFER,
        LAYA_BINDFRAMEBUFFER,
        LAYA_BINDRENDERBUFFER,//第10个
        LAYA_BINDTEXTURE,
        LAYA_USETEXTURE,
        LAYA_BLENDCOLOR,
        LAYA_BLENDEQUATION,
        LAYA_BLENDEQUATIONSEPARATE,
        LAYA_BLENDFUNC,
        LAYA_BLENDFUNCSEPARATE,
        LAYA_BUFFERDATA_SIZE,
        LAYA_BUFFERDATA_ARRAYBUFFER,
        LAYA_BUFFERSUBDATA,//第20个
        LAYA_CHECKFRAMEBUFFERSTATUS,
        LAYA_CLEAR,
        LAYA_CLEARCOLOR,
        LAYA_CLEARDEPTH,
        LAYA_CLEARSTENCIL,
        LAYA_COLORMASK,
        LAYA_COMPILESHADER,
        LAYA_COPYTEXIMAGE2D,
        LAYA_COPYTEXSUBIMAGE2D,
        LAYA_CREATEBUFFER,//第30个
        LAYA_CREATEFRAMEBUFFER,
        LAYA_CREATEPROGRAM,
        LAYA_CREATERENDERBUFFER,
        LAYA_CREATESHADER,
        LAYA_CREATETEXTURE,
        LAYA_CULLFACE,
        LAYA_DELETEBUFFER,
        LAYA_DELETEFRAMEBUFFER,
        LAYA_DELETEPROGRAM,
        LAYA_DELETERENDERBUFFER,//第40个
        LAYA_DELETESHADER,
        LAYA_DELETETEXTURE,
        LAYA_DEPTHFUNC,
        LAYA_DEPTHMASK,
        LAYA_DEPTHRANGE,
        LAYA_DETACHSHADER,
        LAYA_DISABLE,
        LAYA_DISABLEVERTEXATTRIBARRAY,
        LAYA_DRAWARRAYS,
        LAYA_DRAWELEMENTS,//第50个
        LAYA_ENABLE,
        LAYA_ENABLEVERTEXATTRIBARRAY,
        LAYA_FINISH,
        LAYA_FLUSH,
        LAYA_FRAMEBUFFERRENDERBUFFER,
        LAYA_FRAMEBUFFERTEXTURE2D,
        LAYA_FRONTFACE,
        LAYA_GENERATEMIPMAP,
        LAYA_GETACTIVEATTRIB,
        LAYA_GETACTIVEUNIFORM,//第60个
        LAYA_GETATTRIBLOCATION,
        LAYA_GETPARAMETER,
        LAYA_GETBUFFERPARAMETER,
        LAYA_GETERROR,
        LAYA_GETFRAMEBUFFERATTACHMENTPARAMETER,
        LAYA_GETPROGRAMPARAMETER,
        LAYA_GETPROGRAMINFOLOG,
        LAYA_GETRENDERBUFFERPARAMETER,
        LAYA_GETSHADERPRECISIONFORMAT,
        LAYA_GETSHADERPARAMETER,//第70个
        LAYA_GETSHADERINFOLOG,
        LAYA_GETSHADERSOURCE,
        LAYA_GETTEXPARAMETER,
        LAYA_GETUNIFORM,
        LAYA_GETUNIFORMLOCATION,
        LAYA_GETVERTEXATTRIB,
        LAYA_GETVERTEXATTRIBOFFSET,
        LAYA_HINT,
        LAYA_ISBUFFER,
        LAYA_ISENABLED,//第80个
        LAYA_ISFRAMEBUFFER,
        LAYA_ISPROGRAM,
        LAYA_ISRENDERBUFFER,
        LAYA_ISSHADER,
        LAYA_ISTEXTURE,
        LAYA_LINEWIDTH,
        LAYA_LINKPROGRAM,
        LAYA_PIXELSTOREI,
        LAYA_POLYGONOFFSET,
        LAYA_READPIXELS,//第90个
        LAYA_RENDERBUFFERSTORAGE,
        LAYA_SAMPLECOVERAGE,
        LAYA_SCISSOR,
        LAYA_SHADERSOURCE,
        LAYA_STENCILFUNC,
        LAYA_STENCILFUNCSEPARATE,
        LAYA_STENCILMASK,
        LAYA_STENCILMASKSEPARATE,
        LAYA_STENCILOP,
        LAYA_STENCILOPSEPARATE,//第100个
        LAYA_TEXIMAGE2D,
        LAYA_TEXPARAMETERF,
        LAYA_TEXPARAMETERI,
        LAYA_TEXSUBIMAGE2D,
        LAYA_UNIFORM1F,
        LAYA_UNIFORM1FV,
        LAYA_UNIFORM1I,
        LAYA_UNIFORM1IV,
        LAYA_UNIFORM2F,
        LAYA_UNIFORM2FV,//第110个
        LAYA_UNIFORM2I,
        LAYA_UNIFORM2IV,
        LAYA_UNIFORM3F,
        LAYA_UNIFORM3FV,
        LAYA_UNIFORM3I,
        LAYA_UNIFORM3IV,
        LAYA_UNIFORM4F,
        LAYA_UNIFORM4FV,
        LAYA_UNIFORM4I,
        LAYA_UNIFORM4IV,//第120个
        LAYA_UNIFORMMATRIX2FV,
        LAYA_UNIFORMMATRIX3FV,
        LAYA_UNIFORMMATRIX4FV,
        LAYA_USEPROGRAM,
        LAYA_VALIDATEPROGRAM,
        LAYA_VERTEXATTRIB1F,
        LAYA_VERTEXATTRIB1FV,
        LAYA_VERTEXATTRIB2F,
        LAYA_VERTEXATTRIB2FV,
        LAYA_VERTEXATTRIB3F,//第130个
        LAYA_VERTEXATTRIB3FV,
        LAYA_VERTEXATTRIB4F,
        LAYA_VERTEXATTRIB4FV,
        LAYA_VERTEXATTRIBPOINTER,
        LAYA_VIEWPORT,
        LAYA_CONFIGUREBACKBUFFER,
        LAYA_COMPRESSEDTEXIMAGE2D,
        LAYA_TEXIMAGE2D_PIXEL,
        LAYA_TEXSUBIMAGE2D_PIXEL,
        LAYA_CREATEVERTEXARRAY,//第140个
        LAYA_BINDVERTEXARRAY,
        LAYA_DELETEVERTEXARRAYS,
        LAYA_READPIXELSASYNC,
        LAYA_COMPRESSEDTEXSUBIMAGE2D,
        LAYA_VERTEXATTRIBDIVISOR,
        LAYA_DRAWARRAYSINSTANCED,
        LAYA_DRAWELEMENTSINSTANCED,
		LAYA_TEXIMAGE2DCANVAS,
		LAYA_TEXSTORAGE2D,
		LAYA_RENDERBUFFERSTORAGEMUILTISAMPLE,
    };
    class JCLayaGLDispatch
    {
    public:

        typedef void(*ProcFunction)(JCCommandEncoderBuffer& layaGLCmd);
        
        static bool dispatchScriptCmd(JCCommandEncoderBuffer& layaGLCmd);
        
		static void dispatchAllCmds(JCCommandEncoderBuffer* pTemplateMem);

    public:

        static void _layaGL_empty(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_getContextAttributes(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_isContextLost(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_getSupportedExtensions(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_getExtension(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_activeTexture(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_attachShader(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_bindAttribLocation(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_bindBuffer(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_bindFramebuffer(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_bindRenderbuffer(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_bindTexture(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_useTexture(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_blendColor(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_blendEquation(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_blendEquationSeparate(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_blendFunc(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_blendFuncSeparate(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_bufferData_size(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_bufferData_ArrayBuffer(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_bufferSubData(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_checkFramebufferStatus(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_clear(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_clearColor(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_clearDepth(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_clearStencil(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_colorMask(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_compileShader(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_copyTexImage2D(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_copyTexSubImage2D(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_createBuffer(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_createFramebuffer(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_createProgram(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_createRenderbuffer(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_createShader(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_createTexture(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_cullFace(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_deleteBuffer(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_deleteFramebuffer(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_deleteProgram(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_deleteRenderbuffer(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_deleteShader(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_deleteTexture(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_depthFunc(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_depthMask(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_depthRange(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_detachShader(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_disable(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_disableVertexAttribArray(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_drawArrays(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_drawElements(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_enable(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_enableVertexAttribArray(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_finish(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_flush(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_framebufferRenderbuffer(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_framebufferTexture2D(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_frontFace(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_generateMipmap(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_getActiveAttrib(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_getActiveUniform(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_getAttribLocation(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_getParameter(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_getBufferParameter(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_getError(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_getFramebufferAttachmentParameter(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_getProgramParameter(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_getProgramInfoLog(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_getRenderbufferParameter(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_getShaderPrecisionFormat(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_getShaderParameter(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_getShaderInfoLog(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_getShaderSource(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_getTexParameter(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_getUniform(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_getUniformLocation(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_getVertexAttrib(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_getVertexAttribOffset(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_hint(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_isBuffer(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_isEnabled(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_isFramebuffer(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_isProgram(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_isRenderbuffer(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_isShader(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_isTexture(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_lineWidth(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_linkProgram(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_pixelStorei(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_polygonOffset(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_readPixels(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_renderbufferStorage(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_sampleCoverage(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_scissor(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_shaderSource(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_stencilFunc(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_stencilFuncSeparate(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_stencilMask(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_stencilMaskSeparate(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_stencilOp(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_stencilOpSeparate(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_texImage2D(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_texParameterf(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_texParameteri(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_texSubImage2D(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_uniform1f(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_uniform1fv(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_uniform1i(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_uniform1iv(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_uniform2f(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_uniform2fv(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_uniform2i(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_uniform2iv(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_uniform3f(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_uniform3fv(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_uniform3i(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_uniform3iv(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_uniform4f(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_uniform4fv(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_uniform4i(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_uniform4iv(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_uniformMatrix2fv(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_uniformMatrix3fv(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_uniformMatrix4fv(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_useProgram(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_validateProgram(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_vertexAttrib1f(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_vertexAttrib1fv(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_vertexAttrib2f(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_vertexAttrib2fv(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_vertexAttrib3f(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_vertexAttrib3fv(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_vertexAttrib4f(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_vertexAttrib4fv(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_vertexAttribPointer(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_viewport(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_configureBackBuffer(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_compressedTexImage2D(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_texImage2D_pixel(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_texSubImage2D_pixel(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_createVertexArray(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_bindVertexArray(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_deleteVertexArray(JCCommandEncoderBuffer& layaGLCmd);  
        static void _layaGL_readPixelsAsync(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_compressedTexSubImage2D(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_vertexAttribDivisor(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_drawArraysInstanced(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_drawElementsInstanced(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_texImage2D_canvas(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_texStorage2D(JCCommandEncoderBuffer& layaGLCmd);
		static void _layaGL_renderbufferStorageMultisample(JCCommandEncoderBuffer& layaGLCmd);
		static void _layaGL_clearBufferfv(JCCommandEncoderBuffer& layaGLCmd);
		static void _layaGL_blitFramebuffer(JCCommandEncoderBuffer& layaGLCmd);
        //------------------------------------------------------------------------------
        //-------------------------webgl扩展函数-----------------------------------------
        //------------------------------------------------------------------------------
        static void _layaGL_uniformMatrix2fvEx(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_uniformMatrix3fvEx(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_uniformMatrix4fvEx(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_addShaderUniform(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_useCommandEncoder(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_uploadShaderUniforms(JCCommandEncoderBuffer& layaGLCmd);
		static void _layaGL_uploadShaderUniformsData(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_loadDataToReg(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_loadDataToRegEx(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_ifLess0(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_ifEqual0(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_ifGreater0(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_ifLEqual0(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_ifGEqual0(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_ifGNotEqual0(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_operateReg(JCCommandEncoderBuffer& layaGLCmd);
        static void _layaGL_store(JCCommandEncoderBuffer& layaGLCmd);
        //------------------------------------------------------------------------------
        //-------------------------C++函数-----------------------------------------
        //------------------------------------------------------------------------------
		static void _layaGL_createImageOnRenderThread(JCCommandEncoderBuffer& layaGLCmd);
		static void _layaGL_deleteImageOnRenderThread(JCCommandEncoderBuffer& layaGLCmd);
		static void _layaGL_releaseImageOnRenderThread(JCCommandEncoderBuffer& layaGLCmd);
		static void _layaGL_setImageReleaseSpaceTime(JCCommandEncoderBuffer& layaGLCmd);
		static void _layaGL_setPremultiplyAlpha(JCCommandEncoderBuffer& layaGLCmd);
		static void _layaGL_PerfAddData(JCCommandEncoderBuffer& layaGLCmd);
		static void _layaGL_PerfUpdateDt(JCCommandEncoderBuffer& layaGLCmd);
		static void _layaGL_setMainContextSize(JCCommandEncoderBuffer& layaGLCmd);
    public:

        static JCLayaGL*       ms_pLayaGL;      ///<全局的LayaGL
};
}
//------------------------------------------------------------------------------


#endif //__JCLayaGLDispatch_H__

//-----------------------------END FILE--------------------------------