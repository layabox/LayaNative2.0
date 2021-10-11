/**
@file			JCWebGLPlus.h
@brief			
@author			James
@version		1.0
@date			2019_8_24
*/

#ifndef __JCWebGLPlus_H__
#define __JCWebGLPlus_H__

#if __APPLE__
    #include <OpenGLES/ES3/gl.h>
#else
    #include <GLES3/gl3.h>
#endif
#include <stdio.h>
#include <vector>
#include <string>
#include <functional>
#include "../manager/JCArrayBufferManager.h"

namespace laya
{
    enum UNIFORM_FUN
    {
        UNIFORM1F = 0,
        UNIFORM1FV,
        UNIFORM1I,
        UNIFORM1IV,
        UNIFORM2F,
        UNIFORM2FV,
        UNIFORM2I,
        UNIFORM2IV,
        UNIFORM3F,
        UNIFORM3FV,
        UNIFORM3I,
        UNIFORM3IV,
        UNIFORM4F,
        UNIFORM4FV,
        UNIFORM4I,
        UNIFORM4IV,
        UNIFORMMATRIX2FV,
        UNIFORMMATRIX3FV,
        UNIFORMMATRIX4FV,
        UNIFORMSAMPLER_2D,
        UNIFORMSAMPLER_CUBE,
    };
    struct WEBGLPLUS_iiiiii
    {
        int b;
        int c;
        int d;
        int e;
        int f;
        int g;
    };
    /** 
     * @brief 
    */
    class JCWebGLPlus
    {
    public:

        /** @brief构造函数
        */
        JCWebGLPlus();

        /** @brief析构函数
        */
        ~JCWebGLPlus();

        static JCWebGLPlus* getInstance();
        
        static void releaseInstance();
        
        void init(int nThreadMode);

        void exportJS(void* ctx, void* object);
        
        void clearAll();
        
    public:

        int culling3D(int cullingCount, char* pFrustumBuffer, int nFrustumLen, char* pCullingBuffer, int nCullingBufferLen,
            char* pCullingBufferIndices, int nCullingBufferIndicesLen, char* pCullingBufferResult, int nCullingBufferResultLen);

        bool updateAnimationNodeWorldMatix3D(char* pLocPosition, int nLocPosLen, char* pLocRotation, int nLocRotLen, char* pLocScaling, int nLocScaLen,
            char* pParentIndices, int nLocParentLen, char* pOutWorldMatrix, int nOutWorldLen);

        bool computeSubSkinnedData3D(char* pWorldMatrixs, int nWorldMatLen, char* pWorldMatrixIndex, int nWorldMatIndeLen,
            char* pInverseBindPoses, int nInverseBindPosesLen, char* pBoneIndices, int nBoneIndicesLen,
            char* pBindPoseIndices, int nBindPoseIndicesLen, char* pResultData, int nResultDataLen);

        void calcMatrix(float* pos, float* scale, float* quat, float* outE);

        void matrix4x4MultiplyFFF(float* a, float* b, float* e);

    public:

        void uniformMatrix2fvEx(GLuint location, GLboolean transpose, int id);
        void uniformMatrix3fvEx(GLuint location, GLboolean transpose, int id);
        void uniformMatrix4fvEx(GLuint location, GLboolean transpose, int id);
        void uploadShaderUniforms(int nCmdID, int nDataID);
        void uploadShaderUniformsBuffer(int nCmdID, char* pData, int nDataSize);
        void _uploadShaderUniforms(JCCommandEncoderBuffer* cmd, char* data, int dataSize);

    public:
        
        static JCWebGLPlus*                 s_pWebGLPlus;
        JCArrayBufferManager*               m_pJSArrayBufferManager;            ///<JS线程的ArrayBufferManager
        JCArrayBufferManager*               m_pJSABManagerSyncToRender;         ///<JS线程里面需要同步给渲染线程的ArrayBufferManager
        JCArrayBufferManager*               m_pRArrayBufferManager;             ///<渲染线程的ArrayBufferManager
        int                                 m_nThreadMODE;

    public:
        std::function<void(GLuint location, GLfloat x)>                                          uniform1f;
        std::function<void(GLuint location, int count, const GLfloat* v)>                        uniform1fv;
        std::function<void(GLuint location, GLint x)>                                            uniform1i;
        std::function<void(GLuint location, int count, const GLint* v)>                          uniform1iv;
        std::function<void(GLuint location, GLfloat x, GLfloat y)>                               uniform2f;
        std::function<void(GLuint location, int count, const GLfloat* v)>                        uniform2fv;
        std::function<void(GLuint location, GLint x, GLint y)>                                   uniform2i;
        std::function<void(GLuint location, int count, const GLint* v)>                          uniform2iv;
        std::function<void(GLuint location, GLfloat x, GLfloat y, GLfloat z)>                    uniform3f;
        std::function<void(GLuint location, int count, const GLfloat* v)>                        uniform3fv;
        std::function<void(GLuint location, GLint x, GLint y, GLint z)>                          uniform3i;
        std::function<void(GLuint location, int count, const GLint* v)>                          uniform3iv;
        std::function<void(GLuint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)>         uniform4f;
        std::function<void(GLuint location, int count, const GLfloat* v)>                        uniform4fv;
        std::function<void(GLuint location, GLint x, GLint y, GLint z, GLint w)>                 uniform4i;
        std::function<void(GLuint location, int count, const GLint* v)>                          uniform4iv;
        std::function<void(GLuint location, int count, GLboolean transpose, GLfloat* value)>     uniformMatrix2fv;
        std::function<void(GLuint location, int count, GLboolean transpose, GLfloat* value)>     uniformMatrix3fv;
        std::function<void(GLuint location, int count, GLboolean transpose, GLfloat* value)>     uniformMatrix4fv;
        std::function<void(GLenum texture)>                                                      activeTexture;
        std::function<void(GLenum target, GLuint texture)>                                       bindTexture;
    };
}

#endif //__JCWebGLPlus_H__

//-----------------------------END FILE--------------------------------
