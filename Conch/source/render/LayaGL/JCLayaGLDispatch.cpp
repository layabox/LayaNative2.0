/**
@file			JCLayaGLDispatch.cpp
@brief			
@author			James
@version		1.0
@date			2018_5_25
*/

#include "JCLayaGLDispatch.h"
#include "../JCCmdStruct.h"
#include <util/Log.h>
#include <manager/JCArrayBufferManager.h>
#include "../render/RenderEx/JCRegister.h"
#include <util/JCCommonMethod.h>
#include "../render/Performance/JCPerfHUD.h"
#include <webglplus/JCWebGLPlus.h>

namespace laya
{
    JCLayaGL* JCLayaGLDispatch::ms_pLayaGL = NULL;
    bool JCLayaGLDispatch::dispatchScriptCmd(JCCommandEncoderBuffer& pRenderCmd)
    {
		static int nLastPos = 0;
		static int nFuncID = 0;
        static const ProcFunction g_svProcFunctions[] =
        {
            &JCLayaGLDispatch::_layaGL_uniformMatrix2fvEx,			//0
            &JCLayaGLDispatch::_layaGL_uniformMatrix3fvEx,			//1
            &JCLayaGLDispatch::_layaGL_uniformMatrix4fvEx,			//2
            &JCLayaGLDispatch::_layaGL_addShaderUniform,			//3
			&JCLayaGLDispatch::_layaGL_uploadShaderUniforms,		//4
            &JCLayaGLDispatch::_layaGL_uploadShaderUniformsData,	//5
			&JCLayaGLDispatch::_layaGL_useCommandEncoder,			//6
			&JCLayaGLDispatch::_layaGL_loadDataToReg,				//7
			&JCLayaGLDispatch::_layaGL_loadDataToRegEx,				//8
			&JCLayaGLDispatch::_layaGL_ifLess0,						//9
			&JCLayaGLDispatch::_layaGL_ifEqual0,					//10
			&JCLayaGLDispatch::_layaGL_ifGreater0,					//11
			&JCLayaGLDispatch::_layaGL_ifLEqual0,					//12
			&JCLayaGLDispatch::_layaGL_ifGEqual0,					//13
			&JCLayaGLDispatch::_layaGL_ifGNotEqual0,				//14
			&JCLayaGLDispatch::_layaGL_operateReg,					//15
			&JCLayaGLDispatch::_layaGL_store,						//16
            &JCLayaGLDispatch::_layaGL_empty,//17
            &JCLayaGLDispatch::_layaGL_empty,//18
            &JCLayaGLDispatch::_layaGL_empty,//19
            &JCLayaGLDispatch::_layaGL_empty,//20
            &JCLayaGLDispatch::_layaGL_empty,//21
            &JCLayaGLDispatch::_layaGL_empty,//22
            &JCLayaGLDispatch::_layaGL_empty,//23
            &JCLayaGLDispatch::_layaGL_empty,//24
            &JCLayaGLDispatch::_layaGL_empty,//25
            &JCLayaGLDispatch::_layaGL_empty,//26
            &JCLayaGLDispatch::_layaGL_empty,//27
            &JCLayaGLDispatch::_layaGL_empty,//28
            &JCLayaGLDispatch::_layaGL_empty,//29
            &JCLayaGLDispatch::_layaGL_empty,//30
            &JCLayaGLDispatch::_layaGL_empty,//31
            &JCLayaGLDispatch::_layaGL_empty,//32
            &JCLayaGLDispatch::_layaGL_empty,//33
            &JCLayaGLDispatch::_layaGL_empty,//34
            &JCLayaGLDispatch::_layaGL_empty,//35
            &JCLayaGLDispatch::_layaGL_empty,//36
            &JCLayaGLDispatch::_layaGL_empty,//37
            &JCLayaGLDispatch::_layaGL_empty,//38
            &JCLayaGLDispatch::_layaGL_empty,//39
            &JCLayaGLDispatch::_layaGL_empty,//40
            &JCLayaGLDispatch::_layaGL_empty,//41
            &JCLayaGLDispatch::_layaGL_empty,//42
            &JCLayaGLDispatch::_layaGL_empty,//43
            &JCLayaGLDispatch::_layaGL_empty,//44
            &JCLayaGLDispatch::_layaGL_empty,//45
            &JCLayaGLDispatch::_layaGL_empty,//46
            &JCLayaGLDispatch::_layaGL_empty,//47
            &JCLayaGLDispatch::_layaGL_empty,//48
            &JCLayaGLDispatch::_layaGL_empty,//49
            &JCLayaGLDispatch::_layaGL_empty,//50
            &JCLayaGLDispatch::_layaGL_empty,//51
            &JCLayaGLDispatch::_layaGL_empty,//52
            &JCLayaGLDispatch::_layaGL_empty,//53
            &JCLayaGLDispatch::_layaGL_empty,//54
            &JCLayaGLDispatch::_layaGL_empty,//55
            &JCLayaGLDispatch::_layaGL_empty,//56
            &JCLayaGLDispatch::_layaGL_empty,//57
            &JCLayaGLDispatch::_layaGL_empty,//58
            &JCLayaGLDispatch::_layaGL_empty,//59
            &JCLayaGLDispatch::_layaGL_empty,//60
            &JCLayaGLDispatch::_layaGL_empty,//61
            &JCLayaGLDispatch::_layaGL_empty,//62
            &JCLayaGLDispatch::_layaGL_empty,//63
            &JCLayaGLDispatch::_layaGL_createImageOnRenderThread,//64
            &JCLayaGLDispatch::_layaGL_deleteImageOnRenderThread,//65
            &JCLayaGLDispatch::_layaGL_releaseImageOnRenderThread,//66
            &JCLayaGLDispatch::_layaGL_setImageReleaseSpaceTime,//67
            &JCLayaGLDispatch::_layaGL_setPremultiplyAlpha,//68
            &JCLayaGLDispatch::_layaGL_PerfAddData,//69
            &JCLayaGLDispatch::_layaGL_PerfUpdateDt,//70
            &JCLayaGLDispatch::_layaGL_setMainContextSize,//71
            &JCLayaGLDispatch::_layaGL_empty,//72
            &JCLayaGLDispatch::_layaGL_empty,//73
            &JCLayaGLDispatch::_layaGL_empty,//74
            &JCLayaGLDispatch::_layaGL_empty,//75
            &JCLayaGLDispatch::_layaGL_empty,//76
            &JCLayaGLDispatch::_layaGL_empty,//77
            &JCLayaGLDispatch::_layaGL_empty,//78
            &JCLayaGLDispatch::_layaGL_empty,//79
            &JCLayaGLDispatch::_layaGL_empty,//80
			&JCLayaGLDispatch::_layaGL_empty,//81
			&JCLayaGLDispatch::_layaGL_empty,//82
			&JCLayaGLDispatch::_layaGL_empty,//83
			&JCLayaGLDispatch::_layaGL_empty,//84
			&JCLayaGLDispatch::_layaGL_empty,//85
			&JCLayaGLDispatch::_layaGL_empty,//86
			&JCLayaGLDispatch::_layaGL_empty,//87
			&JCLayaGLDispatch::_layaGL_empty,//88
			&JCLayaGLDispatch::_layaGL_empty,//89
			&JCLayaGLDispatch::_layaGL_empty,//90
			&JCLayaGLDispatch::_layaGL_empty,//91
			&JCLayaGLDispatch::_layaGL_empty,//92
			&JCLayaGLDispatch::_layaGL_empty,//93
			&JCLayaGLDispatch::_layaGL_empty,//94
			&JCLayaGLDispatch::_layaGL_empty,//95
			&JCLayaGLDispatch::_layaGL_empty,//96
			&JCLayaGLDispatch::_layaGL_empty,//97
			&JCLayaGLDispatch::_layaGL_empty,//98
			&JCLayaGLDispatch::_layaGL_empty,//99
			&JCLayaGLDispatch::_layaGL_empty,//100
			&JCLayaGLDispatch::_layaGL_empty,//101
			&JCLayaGLDispatch::_layaGL_empty,//102
			&JCLayaGLDispatch::_layaGL_empty,//103
			&JCLayaGLDispatch::_layaGL_empty,//104
			&JCLayaGLDispatch::_layaGL_empty,//105
			&JCLayaGLDispatch::_layaGL_empty,//106
			&JCLayaGLDispatch::_layaGL_empty,//107
			&JCLayaGLDispatch::_layaGL_empty,//108
			&JCLayaGLDispatch::_layaGL_empty,//109
			&JCLayaGLDispatch::_layaGL_empty,//110
			&JCLayaGLDispatch::_layaGL_empty,//111
			&JCLayaGLDispatch::_layaGL_empty,//112
			&JCLayaGLDispatch::_layaGL_empty,//113
			&JCLayaGLDispatch::_layaGL_empty,//114
			&JCLayaGLDispatch::_layaGL_empty,//115
			&JCLayaGLDispatch::_layaGL_empty,//116
			&JCLayaGLDispatch::_layaGL_empty,//117
			&JCLayaGLDispatch::_layaGL_empty,//118
			&JCLayaGLDispatch::_layaGL_empty,//119
			&JCLayaGLDispatch::_layaGL_empty,//120
			&JCLayaGLDispatch::_layaGL_empty,//121
			&JCLayaGLDispatch::_layaGL_empty,//122
			&JCLayaGLDispatch::_layaGL_empty,//123
			&JCLayaGLDispatch::_layaGL_empty,//124
			&JCLayaGLDispatch::_layaGL_empty,//125
			&JCLayaGLDispatch::_layaGL_empty,//126
			&JCLayaGLDispatch::_layaGL_empty,//127
            //---------------------------3D的渲染指令从这开始---------------------------
            &JCLayaGLDispatch::_layaGL_getContextAttributes,
            &JCLayaGLDispatch::_layaGL_isContextLost,
            &JCLayaGLDispatch::_layaGL_getSupportedExtensions,
            &JCLayaGLDispatch::_layaGL_getExtension,
            &JCLayaGLDispatch::_layaGL_activeTexture,
            &JCLayaGLDispatch::_layaGL_attachShader,
            &JCLayaGLDispatch::_layaGL_bindAttribLocation,
            &JCLayaGLDispatch::_layaGL_bindBuffer,
            &JCLayaGLDispatch::_layaGL_bindFramebuffer,
            &JCLayaGLDispatch::_layaGL_bindRenderbuffer,//webgl第10个
            &JCLayaGLDispatch::_layaGL_bindTexture,
            &JCLayaGLDispatch::_layaGL_useTexture,
            &JCLayaGLDispatch::_layaGL_blendColor,
            &JCLayaGLDispatch::_layaGL_blendEquation,
            &JCLayaGLDispatch::_layaGL_blendEquationSeparate,
            &JCLayaGLDispatch::_layaGL_blendFunc,
            &JCLayaGLDispatch::_layaGL_blendFuncSeparate,
            &JCLayaGLDispatch::_layaGL_bufferData_size,
            &JCLayaGLDispatch::_layaGL_bufferData_ArrayBuffer,
            &JCLayaGLDispatch::_layaGL_bufferSubData,//webgl第20个
            &JCLayaGLDispatch::_layaGL_checkFramebufferStatus,
            &JCLayaGLDispatch::_layaGL_clear,
            &JCLayaGLDispatch::_layaGL_clearColor,
            &JCLayaGLDispatch::_layaGL_clearDepth,
            &JCLayaGLDispatch::_layaGL_clearStencil,
            &JCLayaGLDispatch::_layaGL_colorMask,
            &JCLayaGLDispatch::_layaGL_compileShader,
            &JCLayaGLDispatch::_layaGL_copyTexImage2D,
            &JCLayaGLDispatch::_layaGL_copyTexSubImage2D,
            &JCLayaGLDispatch::_layaGL_createBuffer,//webgl第30个
            &JCLayaGLDispatch::_layaGL_createFramebuffer,
            &JCLayaGLDispatch::_layaGL_createProgram,
            &JCLayaGLDispatch::_layaGL_createRenderbuffer,
            &JCLayaGLDispatch::_layaGL_createShader,
            &JCLayaGLDispatch::_layaGL_createTexture,
            &JCLayaGLDispatch::_layaGL_cullFace,
            &JCLayaGLDispatch::_layaGL_deleteBuffer,
            &JCLayaGLDispatch::_layaGL_deleteFramebuffer,
            &JCLayaGLDispatch::_layaGL_deleteProgram,
            &JCLayaGLDispatch::_layaGL_deleteRenderbuffer,//webgl第40个
            &JCLayaGLDispatch::_layaGL_deleteShader,
            &JCLayaGLDispatch::_layaGL_deleteTexture,
            &JCLayaGLDispatch::_layaGL_depthFunc,
            &JCLayaGLDispatch::_layaGL_depthMask,
            &JCLayaGLDispatch::_layaGL_depthRange,
            &JCLayaGLDispatch::_layaGL_detachShader,
            &JCLayaGLDispatch::_layaGL_disable,
            &JCLayaGLDispatch::_layaGL_disableVertexAttribArray,
            &JCLayaGLDispatch::_layaGL_drawArrays,
            &JCLayaGLDispatch::_layaGL_drawElements,//webgl第50个
            &JCLayaGLDispatch::_layaGL_enable,
            &JCLayaGLDispatch::_layaGL_enableVertexAttribArray,
            &JCLayaGLDispatch::_layaGL_finish,
            &JCLayaGLDispatch::_layaGL_flush,
            &JCLayaGLDispatch::_layaGL_framebufferRenderbuffer,
            &JCLayaGLDispatch::_layaGL_framebufferTexture2D,
            &JCLayaGLDispatch::_layaGL_frontFace,
            &JCLayaGLDispatch::_layaGL_generateMipmap,
            &JCLayaGLDispatch::_layaGL_getActiveAttrib,
            &JCLayaGLDispatch::_layaGL_getActiveUniform,//webgl第60个
            &JCLayaGLDispatch::_layaGL_getAttribLocation,
            &JCLayaGLDispatch::_layaGL_getParameter,
            &JCLayaGLDispatch::_layaGL_getBufferParameter,
            &JCLayaGLDispatch::_layaGL_getError,
            &JCLayaGLDispatch::_layaGL_getFramebufferAttachmentParameter,
            &JCLayaGLDispatch::_layaGL_getProgramParameter,
            &JCLayaGLDispatch::_layaGL_getProgramInfoLog,
            &JCLayaGLDispatch::_layaGL_getRenderbufferParameter,
            &JCLayaGLDispatch::_layaGL_getShaderPrecisionFormat,
            &JCLayaGLDispatch::_layaGL_getShaderParameter,//webgl第70个
            &JCLayaGLDispatch::_layaGL_getShaderInfoLog,
            &JCLayaGLDispatch::_layaGL_getShaderSource,
            &JCLayaGLDispatch::_layaGL_getTexParameter,
            &JCLayaGLDispatch::_layaGL_getUniform,
            &JCLayaGLDispatch::_layaGL_getUniformLocation,
            &JCLayaGLDispatch::_layaGL_getVertexAttrib,
            &JCLayaGLDispatch::_layaGL_getVertexAttribOffset,
            &JCLayaGLDispatch::_layaGL_hint,
            &JCLayaGLDispatch::_layaGL_isBuffer,
            &JCLayaGLDispatch::_layaGL_isEnabled,//webgl第80个
            &JCLayaGLDispatch::_layaGL_isFramebuffer,
            &JCLayaGLDispatch::_layaGL_isProgram,
            &JCLayaGLDispatch::_layaGL_isRenderbuffer,
            &JCLayaGLDispatch::_layaGL_isShader,
            &JCLayaGLDispatch::_layaGL_isTexture,
            &JCLayaGLDispatch::_layaGL_lineWidth,
            &JCLayaGLDispatch::_layaGL_linkProgram,
            &JCLayaGLDispatch::_layaGL_pixelStorei,
            &JCLayaGLDispatch::_layaGL_polygonOffset,
            &JCLayaGLDispatch::_layaGL_readPixels,//webgl第90个
            &JCLayaGLDispatch::_layaGL_renderbufferStorage,
            &JCLayaGLDispatch::_layaGL_sampleCoverage,
            &JCLayaGLDispatch::_layaGL_scissor,
            &JCLayaGLDispatch::_layaGL_shaderSource,
            &JCLayaGLDispatch::_layaGL_stencilFunc,
            &JCLayaGLDispatch::_layaGL_stencilFuncSeparate,
            &JCLayaGLDispatch::_layaGL_stencilMask,
            &JCLayaGLDispatch::_layaGL_stencilMaskSeparate,
            &JCLayaGLDispatch::_layaGL_stencilOp,
            &JCLayaGLDispatch::_layaGL_stencilOpSeparate,//webgl第100个
            &JCLayaGLDispatch::_layaGL_texImage2D,
            &JCLayaGLDispatch::_layaGL_texParameterf,
            &JCLayaGLDispatch::_layaGL_texParameteri,
            &JCLayaGLDispatch::_layaGL_texSubImage2D,
            &JCLayaGLDispatch::_layaGL_uniform1f,
            &JCLayaGLDispatch::_layaGL_uniform1fv,
            &JCLayaGLDispatch::_layaGL_uniform1i,
            &JCLayaGLDispatch::_layaGL_uniform1iv,
            &JCLayaGLDispatch::_layaGL_uniform2f,
            &JCLayaGLDispatch::_layaGL_uniform2fv,//webgl第110个
            &JCLayaGLDispatch::_layaGL_uniform2i,
            &JCLayaGLDispatch::_layaGL_uniform2iv,
            &JCLayaGLDispatch::_layaGL_uniform3f,
            &JCLayaGLDispatch::_layaGL_uniform3fv,
            &JCLayaGLDispatch::_layaGL_uniform3i,
            &JCLayaGLDispatch::_layaGL_uniform3iv,
            &JCLayaGLDispatch::_layaGL_uniform4f,
            &JCLayaGLDispatch::_layaGL_uniform4fv,
            &JCLayaGLDispatch::_layaGL_uniform4i,
            &JCLayaGLDispatch::_layaGL_uniform4iv,//webgl第120个
            &JCLayaGLDispatch::_layaGL_uniformMatrix2fv,
            &JCLayaGLDispatch::_layaGL_uniformMatrix3fv,
            &JCLayaGLDispatch::_layaGL_uniformMatrix4fv,
            &JCLayaGLDispatch::_layaGL_useProgram,
            &JCLayaGLDispatch::_layaGL_validateProgram,
            &JCLayaGLDispatch::_layaGL_vertexAttrib1f,
            &JCLayaGLDispatch::_layaGL_vertexAttrib1fv,
            &JCLayaGLDispatch::_layaGL_vertexAttrib2f,
            &JCLayaGLDispatch::_layaGL_vertexAttrib2fv,
            &JCLayaGLDispatch::_layaGL_vertexAttrib3f,//webgl第130个
            &JCLayaGLDispatch::_layaGL_vertexAttrib3fv,
            &JCLayaGLDispatch::_layaGL_vertexAttrib4f,
            &JCLayaGLDispatch::_layaGL_vertexAttrib4fv,
            &JCLayaGLDispatch::_layaGL_vertexAttribPointer,
            &JCLayaGLDispatch::_layaGL_viewport,
            &JCLayaGLDispatch::_layaGL_configureBackBuffer,
            &JCLayaGLDispatch::_layaGL_compressedTexImage2D,
            &JCLayaGLDispatch::_layaGL_texImage2D_pixel,
            &JCLayaGLDispatch::_layaGL_texSubImage2D_pixel,
            &JCLayaGLDispatch::_layaGL_createVertexArray,
            &JCLayaGLDispatch::_layaGL_bindVertexArray,
            &JCLayaGLDispatch::_layaGL_deleteVertexArray,
            &JCLayaGLDispatch::_layaGL_readPixelsAsync,
            &JCLayaGLDispatch::_layaGL_compressedTexSubImage2D,
            &JCLayaGLDispatch::_layaGL_vertexAttribDivisor,
            &JCLayaGLDispatch::_layaGL_drawArraysInstanced,
            &JCLayaGLDispatch::_layaGL_drawElementsInstanced,
            &JCLayaGLDispatch::_layaGL_texImage2D_canvas,
            &JCLayaGLDispatch::_layaGL_texStorage2D,
			&JCLayaGLDispatch::_layaGL_renderbufferStorageMultisample,
			&JCLayaGLDispatch::_layaGL_clearBufferfv,
			&JCLayaGLDispatch::_layaGL_blitFramebuffer,
        };
        static const int nFuncs = sizeof(g_svProcFunctions) / sizeof(g_svProcFunctions[0]);
        char* pCmdBuffer = pRenderCmd.getReadPtr();
        if (pCmdBuffer)
        {
#ifdef DEBUG_PARSE_STREAM
			int nCurrentPos = pRenderCmd.getReadPos();
			if (nCurrentPos == 0) {
				nLastPos = 0;
			}
			nLastPos = pRenderCmd.getReadPos();
#endif 
            nFuncID = *(pRenderCmd.popp<int>());
            if (nFuncID >= 0 && nFuncID < nFuncs)
            {
                (g_svProcFunctions[nFuncID])(pRenderCmd);
            }
            else
            {
                LOGE("JCLayaGLDispatch::dispatchScriptCmd Wrong command number!,cmd=%d", nFuncID);
            }
        }
        return true;
    }
	void JCLayaGLDispatch::dispatchAllCmds(JCCommandEncoderBuffer* pTemplateMem)
	{
		//循环模板指令流
		pTemplateMem->setReadPos(0);
		int nLen = pTemplateMem->getDataSize();
		while (pTemplateMem->getReadPos() < (size_t)nLen)
		{
			dispatchScriptCmd(*pTemplateMem);
		}
	}
	void JCLayaGLDispatch::_layaGL_empty(JCCommandEncoderBuffer& layaGLCmd)
    {
        
    }
    void JCLayaGLDispatch::_layaGL_getContextAttributes(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("getContextAttributes can't support");
    }
    void JCLayaGLDispatch::_layaGL_isContextLost(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("isContextLost can't support"); 
    }
    void JCLayaGLDispatch::_layaGL_getSupportedExtensions(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("getSupportedExtensions can't support");
    }
    void JCLayaGLDispatch::_layaGL_getExtension(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("getExtension can't support");
    }
    void JCLayaGLDispatch::_layaGL_activeTexture(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->activeTexture(*(layaGLCmd.popp<int>()));
    }
    void JCLayaGLDispatch::_layaGL_attachShader(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        ms_pLayaGL->attachShader(cmd->i, cmd->j);
    }
    void JCLayaGLDispatch::_layaGL_bindAttribLocation(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iii* cmd = layaGLCmd.popp<CMD_iii>();
        char* sName = (char*)layaGLCmd.readBufferAlign(cmd->k);
        ms_pLayaGL->bindAttribLocation(cmd->i, cmd->j, sName);
    }
    void JCLayaGLDispatch::_layaGL_bindBuffer(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        ms_pLayaGL->bindBuffer(cmd->i, cmd->j);
    }
    void JCLayaGLDispatch::_layaGL_bindFramebuffer(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        ms_pLayaGL->bindFramebuffer(cmd->i, cmd->j);
    }
    void JCLayaGLDispatch::_layaGL_bindRenderbuffer(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        ms_pLayaGL->bindRenderbuffer(cmd->i, cmd->j);
    }
    void JCLayaGLDispatch::_layaGL_bindTexture(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        ms_pLayaGL->bindTexture(cmd->i, cmd->j);
    }
    void JCLayaGLDispatch::_layaGL_useTexture(JCCommandEncoderBuffer& layaGLCmd)
    {
        int texture = *(layaGLCmd.popp<int>());
        LOGE("useTexture can't support");
    }
    void JCLayaGLDispatch::_layaGL_blendColor(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ffff* cmd = layaGLCmd.popp<CMD_ffff>();
        ms_pLayaGL->blendColor(cmd->x, cmd->y, cmd->w, cmd->h);
    }
    void JCLayaGLDispatch::_layaGL_blendEquation(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->blendEquation(*(layaGLCmd.popp<int>()));
    }
    void JCLayaGLDispatch::_layaGL_blendEquationSeparate(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        ms_pLayaGL->blendEquationSeparate(cmd->i, cmd->j);
    }
    void JCLayaGLDispatch::_layaGL_blendFunc(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        ms_pLayaGL->blendFunc(cmd->i, cmd->j);
    }
    void JCLayaGLDispatch::_layaGL_blendFuncSeparate(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iiii* cmd = layaGLCmd.popp<CMD_iiii>();
        ms_pLayaGL->blendFuncSeparate(cmd->i, cmd->j,cmd->k,cmd->l);
    }
    void JCLayaGLDispatch::_layaGL_bufferData_size(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iii* cmd = layaGLCmd.popp<CMD_iii>();
        ms_pLayaGL->bufferData_size(cmd->i, cmd->j, cmd->k);
    }
    void JCLayaGLDispatch::_layaGL_bufferData_ArrayBuffer(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iii* cmd = layaGLCmd.popp<CMD_iii>();
        if (cmd->k > 0)
        {
            char* value = layaGLCmd.readBufferAlign(cmd->k);
            ms_pLayaGL->bufferData(cmd->i, cmd->k, value, cmd->j);
        }
    }
    void JCLayaGLDispatch::_layaGL_bufferSubData(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iii* cmd = layaGLCmd.popp<CMD_iii>();
        if (cmd->k > 0)
        {
            char* value = layaGLCmd.readBufferAlign(cmd->k);
            ms_pLayaGL->bufferSubData(cmd->i, cmd->j,cmd->k, value);
        }
    }
    void JCLayaGLDispatch::_layaGL_checkFramebufferStatus(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("checkFramebufferStatus can't support");
    }
    void JCLayaGLDispatch::_layaGL_clear(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->clear(*layaGLCmd.popp<int>());
    }
	void JCLayaGLDispatch::_layaGL_clearBufferfv(JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_iiiiii* cmd = layaGLCmd.popp<CMD_iiiiii>();
		GLfloat value[4];
		value[0] = cmd->k;
		value[1] = cmd->l;
		value[2] = cmd->m;
		value[3] = cmd->n;
		ms_pLayaGL->clearBufferfv(cmd->i, cmd->j, value);
	}
	void JCLayaGLDispatch::_layaGL_blitFramebuffer(JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_iiiiiiiiii* cmd = layaGLCmd.popp<CMD_iiiiiiiiii>();
		ms_pLayaGL->blitFramebuffer(cmd->a, cmd->b, cmd->c, cmd->d, cmd->e, cmd->f, cmd->g, cmd->h, cmd->i, cmd->j);
	}
    void JCLayaGLDispatch::_layaGL_clearColor(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ffff* cmd = layaGLCmd.popp<CMD_ffff>();
        ms_pLayaGL->clearColor(cmd->x, cmd->y, cmd->w, cmd->h);
    }
    void JCLayaGLDispatch::_layaGL_clearDepth(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->clearDepth(*layaGLCmd.popp<float>());
    }
    void JCLayaGLDispatch::_layaGL_clearStencil(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->clearStencil(*layaGLCmd.popp<int>());
    }
    void JCLayaGLDispatch::_layaGL_colorMask(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iiii* cmd = layaGLCmd.popp<CMD_iiii>();
        ms_pLayaGL->colorMask(cmd->i, cmd->j,cmd->k,cmd->l);
    }
    void JCLayaGLDispatch::_layaGL_compileShader(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->compileShader(*layaGLCmd.popp<int>());
    }
    void JCLayaGLDispatch::_layaGL_copyTexImage2D(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iiiiiiii* cmd = layaGLCmd.popp<CMD_iiiiiiii>();
        ms_pLayaGL->copyTexImage2D(cmd->a,cmd->b,cmd->c,cmd->d,cmd->e,cmd->f,cmd->g,cmd->h);
    }
    void JCLayaGLDispatch::_layaGL_copyTexSubImage2D(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iiiiiiii* cmd = layaGLCmd.popp<CMD_iiiiiiii>();
        ms_pLayaGL->copyTexSubImage2D(cmd->a, cmd->b, cmd->c, cmd->d, cmd->e, cmd->f, cmd->g, cmd->h);
    }
    void JCLayaGLDispatch::_layaGL_createBuffer(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->createBuffer(*layaGLCmd.popp<int>());
    }
    void JCLayaGLDispatch::_layaGL_createFramebuffer(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->createFramebuffer(*layaGLCmd.popp<int>());
    }
    void JCLayaGLDispatch::_layaGL_createProgram(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->createProgram(*layaGLCmd.popp<int>());
    }
    void JCLayaGLDispatch::_layaGL_createRenderbuffer(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->createRenderbuffer(*layaGLCmd.popp<int>());
    }
    void JCLayaGLDispatch::_layaGL_createShader(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        ms_pLayaGL->createShader(cmd->i, cmd->j);
    }
    void JCLayaGLDispatch::_layaGL_createTexture(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->createTexture(*layaGLCmd.popp<int>());
    }
    void JCLayaGLDispatch::_layaGL_cullFace(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->cullFace(*layaGLCmd.popp<int>());
    }
    void JCLayaGLDispatch::_layaGL_deleteBuffer(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->deleteBuffer(*layaGLCmd.popp<int>());
    }
    void JCLayaGLDispatch::_layaGL_deleteFramebuffer(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->deleteFramebuffer(*layaGLCmd.popp<int>());
    }
    void JCLayaGLDispatch::_layaGL_deleteProgram(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->deleteProgram(*layaGLCmd.popp<int>());
    }
    void JCLayaGLDispatch::_layaGL_deleteRenderbuffer(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->deleteRenderbuffer(*layaGLCmd.popp<int>());
    }
    void JCLayaGLDispatch::_layaGL_deleteShader(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->deleteShader(*layaGLCmd.popp<int>());
    }
    void JCLayaGLDispatch::_layaGL_deleteTexture(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->deleteTexture(*layaGLCmd.popp<int>());
    }
    void JCLayaGLDispatch::_layaGL_depthFunc(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->depthFunc(*layaGLCmd.popp<int>());
    }
    void JCLayaGLDispatch::_layaGL_depthMask(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->depthMask(*layaGLCmd.popp<int>());
    }
    void JCLayaGLDispatch::_layaGL_depthRange(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ff* cmd = layaGLCmd.popp<CMD_ff>();
        ms_pLayaGL->depthRange(cmd->x,cmd->y);
    }
    void JCLayaGLDispatch::_layaGL_detachShader(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        ms_pLayaGL->detachShader(cmd->i, cmd->j);
    }
    void JCLayaGLDispatch::_layaGL_disable(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->disable(*layaGLCmd.popp<int>());
    }
    void JCLayaGLDispatch::_layaGL_disableVertexAttribArray(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->disableVertexAttribArray(*layaGLCmd.popp<int>());
    }
    void JCLayaGLDispatch::_layaGL_drawArrays(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iii* cmd = layaGLCmd.popp<CMD_iii>();
        ms_pLayaGL->drawArrays(cmd->i, cmd->j, cmd->k);
    }
    void JCLayaGLDispatch::_layaGL_drawElements(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iiii* cmd = layaGLCmd.popp<CMD_iiii>();
        ms_pLayaGL->drawElements(cmd->i, cmd->j, cmd->k, cmd->l);
    }
    void JCLayaGLDispatch::_layaGL_enable(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->enable(*layaGLCmd.popp<int>());
    }
    void JCLayaGLDispatch::_layaGL_enableVertexAttribArray(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->enableVertexAttribArray(*layaGLCmd.popp<int>());
    }
    void JCLayaGLDispatch::_layaGL_finish(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->finish();
    }
    void JCLayaGLDispatch::_layaGL_flush(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->flush();
    }
    void JCLayaGLDispatch::_layaGL_framebufferRenderbuffer(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iiii* cmd = layaGLCmd.popp<CMD_iiii>();
        ms_pLayaGL->framebufferRenderbuffer(cmd->i, cmd->j, cmd->k, cmd->l);
    }
    void JCLayaGLDispatch::_layaGL_framebufferTexture2D(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iiiii* cmd = layaGLCmd.popp<CMD_iiiii>();
        ms_pLayaGL->framebufferTexture2D(cmd->i, cmd->j, cmd->k, cmd->l,cmd->m);
    }
    void JCLayaGLDispatch::_layaGL_frontFace(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->frontFace(*layaGLCmd.popp<int>());
    }
    void JCLayaGLDispatch::_layaGL_generateMipmap(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->generateMipmap(*layaGLCmd.popp<int>());
    }
    void JCLayaGLDispatch::_layaGL_getActiveAttrib(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("getActiveAttrib can't support");
    }
    void JCLayaGLDispatch::_layaGL_getActiveUniform(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("getActiveUniform can't support");
    }
    void JCLayaGLDispatch::_layaGL_getAttribLocation(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("Serious error, it should not call this function. JCLayaGLDispatch::_layaGL_getAttribLocation");
    }
    void JCLayaGLDispatch::_layaGL_getParameter(JCCommandEncoderBuffer& layaGLCmd)
    {
        //CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        //ms_pLayaGL->getParameter(cmd->i, cmd->j);
    }
    void JCLayaGLDispatch::_layaGL_getBufferParameter(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("getBufferParameter can't support");
    }
    void JCLayaGLDispatch::_layaGL_getError(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->getError();
    }
    void JCLayaGLDispatch::_layaGL_getFramebufferAttachmentParameter(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("getFramebufferAttachmentParameter can't support");
    }
    void JCLayaGLDispatch::_layaGL_getProgramParameter(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("getProgramParameter can't support");
    }
    void JCLayaGLDispatch::_layaGL_getProgramInfoLog(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("getProgramInfoLog can't support");
    }
    void JCLayaGLDispatch::_layaGL_getRenderbufferParameter(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("getRenderbufferParameter can't support");
    }
    void JCLayaGLDispatch::_layaGL_getShaderPrecisionFormat(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("getShaderPrecision can't support");
    }
    void JCLayaGLDispatch::_layaGL_getShaderParameter(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("getShaderParameter can't support");
    }
    void JCLayaGLDispatch::_layaGL_getShaderInfoLog(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("getShaderInfoLog can't support");
    }
    void JCLayaGLDispatch::_layaGL_getShaderSource(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("getShaderSource can't support");
    }
    void JCLayaGLDispatch::_layaGL_getTexParameter(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("getTexParameter can't support");
    }
    void JCLayaGLDispatch::_layaGL_getUniform(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("getUniform can't support");
    }
    void JCLayaGLDispatch::_layaGL_getUniformLocation(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iii* cmd = layaGLCmd.popp<CMD_iii>();
        char* sName = (char*)layaGLCmd.readBufferAlign(cmd->k);
        ms_pLayaGL->getUniformLocation(cmd->i, cmd->j, sName);
    }
    void JCLayaGLDispatch::_layaGL_getVertexAttrib(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("getVertexAttrib can't support");
    }
    void JCLayaGLDispatch::_layaGL_getVertexAttribOffset(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("getVertexAttribOffset can't support");
    }
    void JCLayaGLDispatch::_layaGL_hint(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        ms_pLayaGL->hint(cmd->i,cmd->j);
    }
    void JCLayaGLDispatch::_layaGL_isBuffer(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("isBuffer can't support");
    }
    void JCLayaGLDispatch::_layaGL_isEnabled(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("isEnabled can't support");
    }
    void JCLayaGLDispatch::_layaGL_isFramebuffer(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("isFramebuffer can't support");
    }
    void JCLayaGLDispatch::_layaGL_isProgram(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("isProgram can't support");
    }
    void JCLayaGLDispatch::_layaGL_isRenderbuffer(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("isRenderbuffer can't support");
    }
    void JCLayaGLDispatch::_layaGL_isShader(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("isShader can't support");
    }
    void JCLayaGLDispatch::_layaGL_isTexture(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("isTexture can't support");
    }
    void JCLayaGLDispatch::_layaGL_lineWidth(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->lineWidth(*layaGLCmd.popp<float>());
    }
    void JCLayaGLDispatch::_layaGL_linkProgram(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->linkProgram(*layaGLCmd.popp<int>());
    }
    void JCLayaGLDispatch::_layaGL_pixelStorei(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        ms_pLayaGL->pixelStorei(cmd->i,cmd->j);
    }
    void JCLayaGLDispatch::_layaGL_polygonOffset(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ff* cmd = layaGLCmd.popp<CMD_ff>();
        ms_pLayaGL->polygonOffset(cmd->x,cmd->y);
    }
    void JCLayaGLDispatch::_layaGL_readPixels(JCCommandEncoderBuffer& layaGLCmd)
    {
        LOGE("readPixels can't support");
    }
    void JCLayaGLDispatch::_layaGL_renderbufferStorage(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iiii* cmd = layaGLCmd.popp<CMD_iiii>();
        ms_pLayaGL->renderbufferStorage(cmd->i, cmd->j, cmd->k, cmd->l);
    }
    void JCLayaGLDispatch::_layaGL_sampleCoverage(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_fi* cmd = layaGLCmd.popp<CMD_fi>();
        ms_pLayaGL->sampleCoverage(cmd->x, cmd->y);
    }
    void JCLayaGLDispatch::_layaGL_scissor(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iiii* cmd = layaGLCmd.popp<CMD_iiii>();
        ms_pLayaGL->scissor(cmd->i, cmd->j, cmd->k, cmd->l);
    }
    void JCLayaGLDispatch::_layaGL_shaderSource(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        char* sBuffer = (char*)layaGLCmd.readBufferAlign(cmd->j);
        ms_pLayaGL->shaderSource(cmd->i,sBuffer);
    }
    void JCLayaGLDispatch::_layaGL_stencilFunc(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iii* cmd = layaGLCmd.popp<CMD_iii>();
        ms_pLayaGL->stencilFunc(cmd->i, cmd->j, cmd->k);
    }
    void JCLayaGLDispatch::_layaGL_stencilFuncSeparate(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iiii* cmd = layaGLCmd.popp<CMD_iiii>();
        ms_pLayaGL->stencilFuncSeparate(cmd->i,cmd->j,cmd->k,cmd->l);
    }
    void JCLayaGLDispatch::_layaGL_stencilMask(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->stencilMask(*layaGLCmd.popp<int>());
    }
    void JCLayaGLDispatch::_layaGL_stencilMaskSeparate(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        ms_pLayaGL->stencilMaskSeparate(cmd->i, cmd->j);
    }
    void JCLayaGLDispatch::_layaGL_stencilOp(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iii* cmd = layaGLCmd.popp<CMD_iii>();
        ms_pLayaGL->stencilOp(cmd->i, cmd->j, cmd->k);
    }
    void JCLayaGLDispatch::_layaGL_stencilOpSeparate(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iiii* cmd = layaGLCmd.popp<CMD_iiii>();
        ms_pLayaGL->stencilOpSeparate(cmd->i, cmd->j, cmd->k, cmd->l);
    }
    void JCLayaGLDispatch::_layaGL_texImage2D_canvas(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iiiiiiii* cmd = layaGLCmd.popp<CMD_iiiiiiii>();
        int width = cmd->g;
        int height = cmd->h;
        ms_pLayaGL->texImage2DCanvas(width, height, cmd->f);
    }
    void JCLayaGLDispatch::_layaGL_texStorage2D(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iiiii* cmd = layaGLCmd.popp<CMD_iiiii>();
        ms_pLayaGL->texStorage2D(cmd->i, cmd->j, cmd->k, cmd->l, cmd->m);
    }
    void JCLayaGLDispatch::_layaGL_texImage2D(JCCommandEncoderBuffer& layaGLCmd)
    {
        //TODO 目前只支持Image
        CMD_iiiiii* cmd = layaGLCmd.popp<CMD_iiiiii>();
        JCImage* pImage = ms_pLayaGL->m_pImageManager->getImage(cmd->n);
        if (pImage)
        {
            pImage->enableImage();
			pImage->updateTexImage();
            int width = pImage->getWidth();
            int height = pImage->getHeight();
            char* pBufferData = (char*)pImage->m_kBitmapData.m_pImageData;
            if (pBufferData && width > 0 && height > 0)
            {
                unsigned char* outData = nullptr;
                size_t outDataLen = 0;
                convertRGBA8888ToFormat((unsigned char*)pBufferData, pImage->m_kBitmapData.m_nWidth * pImage->m_kBitmapData.m_nHeight * 4, cmd->k, &outData, &outDataLen);
                ms_pLayaGL->texImage2D(cmd->i,cmd->j,cmd->k,width,height,0,cmd->l,cmd->m, outData);
                if (outData != (unsigned char*)pBufferData)
                {
                   delete[] outData;
                }
            }
            pImage->releaseBitmapData();
        }
        else
        {
            LOGE("JCLayaGLDispatch::_layaGL_texImage2D image error");
        }
    }
    void JCLayaGLDispatch::_layaGL_texParameterf(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iif* cmd = layaGLCmd.popp<CMD_iif>();
        ms_pLayaGL->texParameterf(cmd->i,cmd->j,cmd->x);
    }
    void JCLayaGLDispatch::_layaGL_texParameteri(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iii* cmd = layaGLCmd.popp<CMD_iii>();
        ms_pLayaGL->texParameteri(cmd->i, cmd->j, cmd->k);
    }
    void JCLayaGLDispatch::_layaGL_texSubImage2D(JCCommandEncoderBuffer& layaGLCmd)
    {
        //TODO 目前只支持Image
        CMD_iiiiiii* cmd = layaGLCmd.popp<CMD_iiiiiii>();
        JCImage* pImage = ms_pLayaGL->m_pImageManager->getImage(cmd->g);
        if (pImage)
        {
            pImage->enableImage();
			pImage->updateTexImage();
            int width = pImage->getWidth();
            int height = pImage->getHeight();
            char* pBufferData = (char*)pImage->m_kBitmapData.m_pImageData;
            if (pBufferData && width > 0 && height > 0)
            {
                unsigned char* outData = nullptr;
                size_t outDataLen = 0;
                convertRGBA8888ToFormat((unsigned char*)pBufferData, pImage->m_kBitmapData.m_nWidth * pImage->m_kBitmapData.m_nHeight * 4, cmd->e, &outData, &outDataLen);
                ms_pLayaGL->texSubImage2D(cmd->a, cmd->b, cmd->c, cmd->d, width, height, cmd->e, cmd->f, outData);
                if (outData != (unsigned char*)pBufferData)
                {
                    delete[] outData;
                }
            }
            pImage->releaseBitmapData();
        }
        else
        {
            LOGE("JCLayaGLDispatch::_layaGL_texSubImage2D image error");
        }
    }
    void JCLayaGLDispatch::_layaGL_uniform1f(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_if* cmd = layaGLCmd.popp<CMD_if>();
        ms_pLayaGL->uniform1f(cmd->i,cmd->j);
    }
    void JCLayaGLDispatch::_layaGL_uniform1fv(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        if (cmd->j > 0)
        {
            float* value = (float*)layaGLCmd.readBuffer(cmd->j);
            ms_pLayaGL->uniform1fv(cmd->i, cmd->j/sizeof(float),value );
        }
    }
    void JCLayaGLDispatch::_layaGL_uniform1i(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        ms_pLayaGL->uniform1i(cmd->i, cmd->j);
    }
    void JCLayaGLDispatch::_layaGL_uniform1iv(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        if (cmd->j > 0)
        {
            int* value = (int*)layaGLCmd.readBuffer(cmd->j);
            ms_pLayaGL->uniform1iv(cmd->i, cmd->j / sizeof(int), value);
        }
    }
    void JCLayaGLDispatch::_layaGL_uniform2f(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iff* cmd = layaGLCmd.popp<CMD_iff>();
        ms_pLayaGL->uniform2f(cmd->i, cmd->x, cmd->y);
    }
    void JCLayaGLDispatch::_layaGL_uniform2fv(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        if (cmd->j > 0)
        {
            float* value = (float*)layaGLCmd.readBuffer(cmd->j);
            ms_pLayaGL->uniform2fv(cmd->i, cmd->j / sizeof(float)/2, value);
        }
    }
    void JCLayaGLDispatch::_layaGL_uniform2i(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iii* cmd = layaGLCmd.popp<CMD_iii>();
        ms_pLayaGL->uniform2i(cmd->i, cmd->j, cmd->k);
    }
    void JCLayaGLDispatch::_layaGL_uniform2iv(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        if (cmd->j > 0)
        {
            int* value = (int*)layaGLCmd.readBuffer(cmd->j);
            ms_pLayaGL->uniform2iv(cmd->i, cmd->j/sizeof(int) / 2, value);
        }
    }
    void JCLayaGLDispatch::_layaGL_uniform3f(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ifff* cmd = layaGLCmd.popp<CMD_ifff>();
        ms_pLayaGL->uniform3f(cmd->i, cmd->x, cmd->y, cmd->z);
    }
    void JCLayaGLDispatch::_layaGL_uniform3fv(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        if (cmd->j > 0)
        {
            float* value = (float*)layaGLCmd.readBuffer(cmd->j);
            ms_pLayaGL->uniform3fv(cmd->i, cmd->j / sizeof(float) / 3, value);
        }
    }
    void JCLayaGLDispatch::_layaGL_uniform3i(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iiii* cmd = layaGLCmd.popp<CMD_iiii>();
        ms_pLayaGL->uniform3i(cmd->i, cmd->j, cmd->k, cmd->l);
    }
    void JCLayaGLDispatch::_layaGL_uniform3iv(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        if (cmd->j > 0)
        {
            int* value = (int*)layaGLCmd.readBuffer(cmd->j);
            ms_pLayaGL->uniform3iv(cmd->i, cmd->j / sizeof(int) / 3, value);
        }
    }
    void JCLayaGLDispatch::_layaGL_uniform4f(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iffff* cmd = layaGLCmd.popp<CMD_iffff>();
        ms_pLayaGL->uniform4f(cmd->i, cmd->x, cmd->y, cmd->w,cmd->h);
    }
    void JCLayaGLDispatch::_layaGL_uniform4fv(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        if (cmd->j > 0)
        {
            float* value = (float*)layaGLCmd.readBuffer(cmd->j);
            ms_pLayaGL->uniform4fv(cmd->i, cmd->j / sizeof(float) / 4, value);
        }
    }
    void JCLayaGLDispatch::_layaGL_uniform4i(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iiiii* cmd = layaGLCmd.popp<CMD_iiiii>();
        ms_pLayaGL->uniform4i(cmd->i, cmd->j, cmd->k, cmd->l, cmd->m);
    }
    void JCLayaGLDispatch::_layaGL_uniform4iv(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        if (cmd->j > 0)
        {
            int* value = (int*)layaGLCmd.readBuffer(cmd->j);
            ms_pLayaGL->uniform4iv(cmd->i, cmd->j / sizeof(int) / 4, value);
        }
    }
    void JCLayaGLDispatch::_layaGL_uniformMatrix2fv(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iii* cmd = layaGLCmd.popp<CMD_iii>();
        if (cmd->k > 0)
        {
            float* value = (float*)layaGLCmd.readBuffer(cmd->k);
            ms_pLayaGL->uniformMatrix2fv(cmd->i, cmd->k / sizeof(float) / 4,cmd->j, value );
        }
    }
    void JCLayaGLDispatch::_layaGL_uniformMatrix3fv(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iii* cmd = layaGLCmd.popp<CMD_iii>();
        if (cmd->k > 0)
        {
            float* value = (float*)layaGLCmd.readBuffer(cmd->k);
            ms_pLayaGL->uniformMatrix3fv(cmd->i, cmd->k / sizeof(float) / 9,cmd->j, value );
        }
    }
    void JCLayaGLDispatch::_layaGL_uniformMatrix4fv(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iii* cmd = layaGLCmd.popp<CMD_iii>();
        if (cmd->k > 0)
        {
            float* value = (float*)layaGLCmd.readBuffer(cmd->k);
            ms_pLayaGL->uniformMatrix4fv(cmd->i, cmd->k / sizeof(float) / 16,cmd->j, value );
        }
    }
    void JCLayaGLDispatch::_layaGL_useProgram(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->useProgram(*layaGLCmd.popp<int>());
    }
    void JCLayaGLDispatch::_layaGL_validateProgram(JCCommandEncoderBuffer& layaGLCmd)
    {
        ms_pLayaGL->validateProgram(*layaGLCmd.popp<int>());
    }
    void JCLayaGLDispatch::_layaGL_vertexAttrib1f(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_if* cmd = layaGLCmd.popp<CMD_if>();
        ms_pLayaGL->vertexAttrib1f(cmd->i,cmd->j);
    }
    void JCLayaGLDispatch::_layaGL_vertexAttrib1fv(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        if (cmd->j > 0)
        {
            float* value = (float*)layaGLCmd.readBuffer(cmd->j);
            ms_pLayaGL->vertexAttrib1fv(cmd->i, value);
        }
    }
    void JCLayaGLDispatch::_layaGL_vertexAttrib2f(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iff* cmd = layaGLCmd.popp<CMD_iff>();
        ms_pLayaGL->vertexAttrib2f(cmd->i, cmd->x,cmd->y);
    }
    void JCLayaGLDispatch::_layaGL_vertexAttrib2fv(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        if (cmd->j > 0)
        {
            float* value = (float*)layaGLCmd.readBuffer(cmd->j);
            ms_pLayaGL->vertexAttrib2fv(cmd->i, value);
        }
    }
    void JCLayaGLDispatch::_layaGL_vertexAttrib3f(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ifff* cmd = layaGLCmd.popp<CMD_ifff>();
        ms_pLayaGL->vertexAttrib3f(cmd->i, cmd->x, cmd->y,cmd->z);
    }
    void JCLayaGLDispatch::_layaGL_vertexAttrib3fv(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        if (cmd->j > 0)
        {
            float* value = (float*)layaGLCmd.readBuffer(cmd->j);
            ms_pLayaGL->vertexAttrib3fv(cmd->i, value);
        }
    }
    void JCLayaGLDispatch::_layaGL_vertexAttrib4f(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iffff* cmd = layaGLCmd.popp<CMD_iffff>();
        ms_pLayaGL->vertexAttrib4f(cmd->i, cmd->x, cmd->y, cmd->w,cmd->h);
    }
    void JCLayaGLDispatch::_layaGL_vertexAttrib4fv(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        if (cmd->j > 0)
        {
            float* value = (float*)layaGLCmd.readBuffer(cmd->j);
            ms_pLayaGL->vertexAttrib4fv(cmd->i, value);
        }
    }
    void JCLayaGLDispatch::_layaGL_vertexAttribPointer(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iiiiii* cmd = layaGLCmd.popp<CMD_iiiiii>();
        ms_pLayaGL->vertexAttribPointer(cmd->i,cmd->j,cmd->k,cmd->l,cmd->m,cmd->n);
    }
    void JCLayaGLDispatch::_layaGL_viewport(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iiii* cmd = layaGLCmd.popp<CMD_iiii>();
        ms_pLayaGL->viewport(cmd->i,cmd->j,cmd->k,cmd->l);
    }
    void JCLayaGLDispatch::_layaGL_configureBackBuffer(JCCommandEncoderBuffer& layaGLCmd)
    {
        
    }
    void JCLayaGLDispatch::_layaGL_compressedTexImage2D(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iiiiiii* cmd = layaGLCmd.popp<CMD_iiiiiii>();
        if (cmd->g > 0)
        {
            char* value = (char*)layaGLCmd.readBufferAlign(cmd->g);
            ms_pLayaGL->compressedTexImage2D(cmd->a, cmd->b, cmd->c, cmd->d, cmd->e, cmd->f, cmd->g, value);
        }   
    }
    void JCLayaGLDispatch::_layaGL_compressedTexSubImage2D(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iiiiiiii* cmd = layaGLCmd.popp<CMD_iiiiiiii>();
        if (cmd->h > 0)
        {
            char* value = (char*)layaGLCmd.readBufferAlign(cmd->h);
            ms_pLayaGL->compressedTexSubImage2D(cmd->a, cmd->b, cmd->c, cmd->d, cmd->e, cmd->f, cmd->g, cmd->h, value);
        }
    }
    void JCLayaGLDispatch::_layaGL_texImage2D_pixel(JCCommandEncoderBuffer& layaGLCmd)
    {
		CMD_iiiiiiiii* cmd = layaGLCmd.popp<CMD_iiiiiiiii>();
        char* value = NULL;
		if (cmd->k > 0)value = layaGLCmd.readBufferAlign(cmd->k);
		ms_pLayaGL->texImage2D(cmd->a, cmd->b, cmd->c, cmd->d, cmd->e, cmd->f, cmd->g, cmd->h, value);
    }
    void JCLayaGLDispatch::_layaGL_texSubImage2D_pixel(JCCommandEncoderBuffer& layaGLCmd)
    {
		CMD_iiiiiiiii* cmd = layaGLCmd.popp<CMD_iiiiiiiii>();
		int a = cmd->a, b = cmd->b, c = cmd->c, d = cmd->d, e = cmd->e, f = cmd->f, g = cmd->g, h = cmd->h, k = cmd->k;
        char* value = NULL;
        if (k > 0)value = layaGLCmd.readBufferAlign(k);
        ms_pLayaGL->texSubImage2D(a, b, c, d, e, f, g, h, value);
    }
    void JCLayaGLDispatch::_layaGL_createVertexArray(JCCommandEncoderBuffer& layaGLCmd)
    {
        int vao = *(layaGLCmd.popp<int>());
        ms_pLayaGL->createVertexArray(vao);
    }
    void JCLayaGLDispatch::_layaGL_bindVertexArray(JCCommandEncoderBuffer& layaGLCmd)
    {
        int vao = *(layaGLCmd.popp<int>());
        ms_pLayaGL->bindVertexArray(vao);
    }
    void JCLayaGLDispatch::_layaGL_deleteVertexArray(JCCommandEncoderBuffer& layaGLCmd)
    {
        int vao = *(layaGLCmd.popp<int>());
        ms_pLayaGL->deleteVertexArray(vao);
    }
    void JCLayaGLDispatch::_layaGL_vertexAttribDivisor(JCCommandEncoderBuffer& layaGLCmd)
    {
        int index = *(layaGLCmd.popp<int>());
        int divisor = *(layaGLCmd.popp<int>());
        ms_pLayaGL->vertexAttribDivisor(index, divisor);
    }
    void JCLayaGLDispatch::_layaGL_drawArraysInstanced(JCCommandEncoderBuffer& layaGLCmd)
    {
        int mode = *(layaGLCmd.popp<int>());
        int first = *(layaGLCmd.popp<int>());
        int count = *(layaGLCmd.popp<int>());
        int instanceCount = *(layaGLCmd.popp<int>());
        ms_pLayaGL->drawArraysInstanced(mode, first, count, instanceCount);
    }
    void JCLayaGLDispatch::_layaGL_drawElementsInstanced(JCCommandEncoderBuffer& layaGLCmd)
    {
        int mode = *(layaGLCmd.popp<int>()); 
        int count = *(layaGLCmd.popp<int>()); 
        int type = *(layaGLCmd.popp<int>());
        int indices = *(layaGLCmd.popp<int>()); 
        int primcount = *(layaGLCmd.popp<int>());
        ms_pLayaGL->drawElementsInstanced(mode, count, type, indices, primcount);
    }
    void JCLayaGLDispatch::_layaGL_readPixelsAsync(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iiiiiiii* cmd = layaGLCmd.popp<CMD_iiiiiiii>();
        int x = cmd->a, y = cmd->b, w = cmd->c, h = cmd->d, format = cmd->e, type = cmd->f, callbackObjID = cmd->g, funcID = cmd->h;
        ms_pLayaGL->readPixelsAsync(x, y, w, h, format, type, callbackObjID, funcID);
    }
	void JCLayaGLDispatch::_layaGL_renderbufferStorageMultisample(JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_iiiii* cmd = layaGLCmd.popp<CMD_iiiii>();
		ms_pLayaGL->renderbufferStorageMultisample(cmd->i, cmd->j, cmd->k, cmd->l, cmd->m);
	}
    //------------------------------------------------------------------------------
    //-------------------------webgl扩展函数-----------------------------------------
    //------------------------------------------------------------------------------
    void JCLayaGLDispatch::_layaGL_uniformMatrix2fvEx(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iii* cmd = layaGLCmd.popp<CMD_iii>();
        ms_pLayaGL->m_pWebGLPlus->uniformMatrix2fvEx(cmd->i, cmd->j, cmd->k);
    }
    void JCLayaGLDispatch::_layaGL_uniformMatrix3fvEx(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iii* cmd = layaGLCmd.popp<CMD_iii>();
        ms_pLayaGL->m_pWebGLPlus->uniformMatrix3fvEx(cmd->i, cmd->j, cmd->k);
    }
    void JCLayaGLDispatch::_layaGL_uniformMatrix4fvEx(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iii* cmd = layaGLCmd.popp<CMD_iii>();
        ms_pLayaGL->m_pWebGLPlus->uniformMatrix4fvEx(cmd->i, cmd->j, cmd->k);
    }
    void JCLayaGLDispatch::_layaGL_addShaderUniform(JCCommandEncoderBuffer& layaGLCmd)
    {
		CMD_iiiii* cmd = layaGLCmd.popp<CMD_iiiii>();
		int nFuncID = cmd->i;
		int nLocation = cmd->j;
		int nType = cmd->k;
		int nDataOffset = cmd->l;
		int nTextureID = cmd->m;
        LOGE("JCLayaGLDispatch::_layaGL_addShaderUniform error");
    }
    void JCLayaGLDispatch::_layaGL_uploadShaderUniforms(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        ms_pLayaGL->m_pWebGLPlus->uploadShaderUniforms(cmd->i,cmd->j);
    }
	void JCLayaGLDispatch::_layaGL_uploadShaderUniformsData(JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
		char* pBuffer = (char*)layaGLCmd.readBuffer(cmd->j);
		ms_pLayaGL->m_pWebGLPlus->uploadShaderUniformsBuffer(cmd->i, pBuffer, cmd->j);
	}
	void JCLayaGLDispatch::_layaGL_useCommandEncoder(JCCommandEncoderBuffer& layaGLCmd)
	{
        int id = *(layaGLCmd.popp<int>());
        ms_pLayaGL->useCommandEncoder(id);
	}
    void JCLayaGLDispatch::_layaGL_loadDataToReg(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iii* cmd = layaGLCmd.popp<CMD_iii>();
        char* buffer = (char*)layaGLCmd.readBufferAlign(cmd->k);
        ms_pLayaGL->m_pRegister->loadData(cmd->i, buffer + cmd->j, cmd->k);
    }
    void JCLayaGLDispatch::_layaGL_loadDataToRegEx(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iiii* cmd = layaGLCmd.popp<CMD_iiii>();
        char* buffer = ms_pLayaGL->m_pArrayBufferManager->getArrayBuffer(cmd->j)->m_pBuffer;
        ms_pLayaGL->m_pRegister->loadData(cmd->i, buffer + cmd->k, cmd->l);
    }
    void JCLayaGLDispatch::_layaGL_ifLess0(JCCommandEncoderBuffer& layaGLCmd) 
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        int data = *(int*)(ms_pLayaGL->m_pRegister->getReg(cmd->i));
    }
    void JCLayaGLDispatch::_layaGL_ifEqual0(JCCommandEncoderBuffer& layaGLCmd) 
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        int data = *(int*)(ms_pLayaGL->m_pRegister->getReg(cmd->i));
    }
    void JCLayaGLDispatch::_layaGL_ifGreater0(JCCommandEncoderBuffer& layaGLCmd) 
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        int data = *(int*)(ms_pLayaGL->m_pRegister->getReg(cmd->i));
    }
    void JCLayaGLDispatch::_layaGL_ifLEqual0(JCCommandEncoderBuffer& layaGLCmd) 
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        int data = *(int*)(ms_pLayaGL->m_pRegister->getReg(cmd->i));
    }
    void JCLayaGLDispatch::_layaGL_ifGEqual0(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        int data = *(int*)(ms_pLayaGL->m_pRegister->getReg(cmd->i));
    }
    void JCLayaGLDispatch::_layaGL_ifGNotEqual0(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
        int data = *(int*)(ms_pLayaGL->m_pRegister->getReg(cmd->i));
    }
    void JCLayaGLDispatch::_layaGL_operateReg(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iiiiii* cmd = layaGLCmd.popp<CMD_iiiiii>();
        ms_pLayaGL->m_pRegister->operateReg(cmd->i, cmd->j, cmd->k, cmd->l, (VALUE_OPERATE_TYPE)cmd->m, cmd->n);
    }
    void JCLayaGLDispatch::_layaGL_store(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_iiii* cmd = layaGLCmd.popp<CMD_iiii>();
        int nDataID = cmd->i;
        int nOffset = cmd->j;
        int nRegID = cmd->k;
        int nSize = cmd->l;
        char* buffer = ms_pLayaGL->m_pArrayBufferManager->getArrayBuffer(nDataID)->m_pBuffer;
        ms_pLayaGL->m_pRegister->storeData(nRegID, buffer + nOffset, nSize);
    }
	void JCLayaGLDispatch::_layaGL_createImageOnRenderThread(JCCommandEncoderBuffer& layaGLCmd)
	{
        int nID = *(layaGLCmd.popp<int>());
		JCImage* pImage = (JCImage*)(*(layaGLCmd.popp<long>()));
		ms_pLayaGL->m_pImageManager->setImage(nID, pImage);
	}

	void JCLayaGLDispatch::_layaGL_deleteImageOnRenderThread(JCCommandEncoderBuffer& layaGLCmd)
	{
		int nID = *(layaGLCmd.popp<int>());
		ms_pLayaGL->m_pImageManager->deleteImage(nID);
	}

	void JCLayaGLDispatch::_layaGL_releaseImageOnRenderThread(JCCommandEncoderBuffer& layaGLCmd)
	{
		int nID = *(layaGLCmd.popp<int>());
		JCImage* pImage = ms_pLayaGL->m_pImageManager->getImage(nID);
		if (pImage)
		{
		    pImage->releaseBitmapData();
		}
	}

	void JCLayaGLDispatch::_layaGL_setImageReleaseSpaceTime(JCCommandEncoderBuffer& layaGLCmd)
	{
		int nSpaceTime = *(layaGLCmd.popp<int>());
		ms_pLayaGL->m_pImageManager->setReleaseSpaceTime(nSpaceTime);
	}

	void JCLayaGLDispatch::_layaGL_setPremultiplyAlpha(JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();

		int nID = cmd->i;
		bool bPremultiplyAlpha = (bool)cmd->j;

		JCImage* pImage = ms_pLayaGL->m_pImageManager->getImage(nID);
        if (pImage)
        {
            pImage->setPremultiplyAlpha(bPremultiplyAlpha);
        }
	}

	void JCLayaGLDispatch::_layaGL_PerfAddData(JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_iiff* cmd = layaGLCmd.popp<CMD_iiff>();
		int nID = cmd->i;
		int nColor = cmd->j;
		float fScale = cmd->x;
		float fAlert = cmd->y;

		JCPerfHUD::addData(nID, nColor, fScale, fAlert);
	}

	void JCLayaGLDispatch::_layaGL_PerfUpdateDt(JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_if* cmd = layaGLCmd.popp<CMD_if>();
		int nID = cmd->i;
		float fSpace = cmd->j;

		JCPerfHUD::updateData(nID, fSpace);
	}

	void JCLayaGLDispatch::_layaGL_setMainContextSize(JCCommandEncoderBuffer& layaGLCmd)
	{
		CMD_ii* cmd = layaGLCmd.popp<CMD_ii>();
		ms_pLayaGL->m_nMainCanvasWidth = cmd->i;
		ms_pLayaGL->m_nMainCanvasHeight = cmd->j;
	}

}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
