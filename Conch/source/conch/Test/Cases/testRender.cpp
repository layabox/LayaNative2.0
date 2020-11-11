#ifdef _TEST_
#include "../JCTestManager.h"
#include <3DRS/JCRenderer.h>
#include <3DRS/JCRenderGroupData.h>
#include <3DRS/JCGpuProgram.h>
#include <buffer/JCBuffer.h>
#include <fileSystem/JCFilesystem.h>
#include <imageLib/JCImageRW.h>
#include <3DRS/JCMaterial.h>
#include <3DRS/JCConchMesh.h>
#include <3DRS/JCVertexBuffer.h>
#include "../../WindowsEnv/winWindows.h"
#include "../../WindowsEnv/gles.h"
#include <misc/JCWorkerThread.h>
#include <JCIGLRender.h>
#include <3DRS/JCMeshBuilder.h>
#include <gl/GL.h>
#include <util/Log.h>
#include <Image/JCImage.h>

using namespace laya;
namespace laya {
    extern JCWorkerThread* g_DecThread;
}
/*
    0.基本函数的对象检查
        1.vertexbuffer

    1.rendersys正常
        1.能渲染二维
            1.
            2.多层uv的情况
        2.能渲染三维
            1.自己创建的模型
            2.加载模型文件

    2.资源缩减
    3.能渲染单张图片
        1.原始大小渲染
        2.节省模式渲染
            1.image缩小
            2.贴图缩小
    4.能渲染大图集
*/

namespace renderTest {
    const char* _2dvs = R"(
attribute vec2 position;
attribute vec2 texcoord;
attribute vec4 color;
uniform vec4 g_DevInfo;
uniform mat4 g_OffsetMatrix;
varying vec2 texCoord;
varying vec4 oColor;
void main()
{
    float cx = (position.x*2.0/g_DevInfo.x)-1.0;
	float cy = -(position.y*2.0/g_DevInfo.y)+1.0;
    gl_Position = vec4(cx, cy, 0.5, 1.0);
    texCoord.xy = texcoord;
    oColor = color;
}
)";
    const char* _2dps = R"(
#define PI 3.141593
precision mediump float;

uniform sampler2D g_Tex0;
uniform vec4 u_color;       //rgb,strength
uniform vec4 u_blurInfo;   //blurw,blurh,texw,texh
uniform vec4 u_blur_off;    //blurx,blury,offx,offy
uniform float g_Alpha;
uniform vec4 g_TexRange;   //r,g,b表示rgb的亮度调整，w表示饱和度的调整。w从-1到1（0到1？）.
                            //fillimage的时候是纹理范围
uniform vec4 g_Filter;  //x alpha , y 1-饱和度 z 饱和度/3
                            
varying vec2 texCoord;
varying vec4 oColor;        //顶点输出的颜色

vec2 getFillImgTexcoord(){
   vec2 newTexCoord;
   newTexCoord.x = mod((texCoord.x-g_TexRange.x),g_TexRange.y)+g_TexRange.x;
   newTexCoord.y = mod((texCoord.y-g_TexRange.z),g_TexRange.w)+g_TexRange.z;
   return newTexCoord;
}

float sigma=u_blurInfo.x/3.0;//3σ以外影响很小。即当σ=1的时候，半径为3
float sig2 = sigma*sigma;
float _2sig2 = 2.0*sig2;
float gauss1 = 1.0/(2.0*PI*sig2);

float getGaussian(float x, float y){
    return gauss1*exp(-(x*x+y*y)/_2sig2);
}

vec4 blur(vec2 texcoord){
    const float blurw = 9.0;
    vec4 vec4Color = vec4(0.0,0.0,0.0,0.0);
    vec2 halfsz=vec2(blurw,blurw)/2.0/u_blurInfo.zw;    
    vec2 startpos=texcoord-halfsz-u_blur_off.zw;
    vec2 ctexcoord = startpos;
    vec2 step = 1.0/u_blurInfo.zw;  //每个像素      
    
    for(float y = 0.0;y<=blurw; ++y){
        ctexcoord.x=startpos.x;
        for(float x = 0.0;x<=blurw; ++x){
            //TODO 纹理坐标的固定偏移应该在vs中处理
            vec4Color += texture2D(g_Tex0, ctexcoord)*getGaussian(x-blurw/2.0,y-blurw/2.0);
            ctexcoord.x+=step.x;
        }
        ctexcoord.y+=step.y;
    }
    return vec4Color;
}

vec4 filter(vec4 color){
   float avg = dot(color.xyz, g_Filter.zzz);
   color.rgb = (avg + g_TexRange.xyz*color.rgb*g_Filter.y ); 
   color.a *= g_Filter.x;
   return color;
}

//TODO 测试这样写效率是不是降低了
void main(){
    vec2 texc = texCoord;
    #ifdef FILL_IMG_ADJ_TEXC
    texc = getFillImgTexcoord();
    #endif
    
    #ifdef TYPE_FILLCOLOR
    vec4 color = oColor;
    #elif  defined(TYPE_TEXTURE)
    vec4 color =texture2D(g_Tex0, texc);
    #elif  defined(TYPE_GLOW)
    vec4 color = blur(texc);
    color = vec4(u_color.rgb,color.a*u_color.w);
    #elif  defined(TYPE_BLUR)
    vec4 color = blur(texc);
    #endif
    
    #ifdef USE_FILTER
    color = filter(color);
    #endif
    
    gl_FragColor=color;
    #ifdef USE_G_ALPHA
    gl_FragColor.w*=g_Alpha;
    #endif
}
)";

    /*
    class CRender1{
    public:
        JCGpuProgram*	m_pTestShader;
        mesh*	m_pTestMesh;
        Material*	m_pTestMaterial;
        RenderGroupData	m_RenderGroupData;
        ShaderLinkInfo m_ShaderInfo;
        float m_fV;

        void setup(){
            m_fV = 0;
            m_pTestShader = new gpuProgram();

            buffer bufvs;
            char* pVSSrc="attribute vec3 g_Position;\
                attribute vec2 g_TexCoord0;\
                varying vec2 texCoord;\
                void main(){\
                    gl_Position = vec4(g_Position.x, g_Position.y, g_Position.z, 1.0);\
                    texCoord=g_TexCoord0;\
                }";
            char* pPSSrc="precision mediump float;"
                "uniform sampler2D g_Tex0;"
                "varying vec2 texCoord;"
                "varying vec4 vertColor;"
                "void main(){"
                "   gl_FragColor=texture2D(g_Tex0, texCoord);\n\
                }";
            m_pTestShader->setSrc(pVSSrc, pPSSrc);
            int nProgram = m_pTestShader->compile();

            //load jpeg
            buffer buf;
            readFileSync("d:/temp/gameicon.png",buf);
            BitmapData bmp;
            bool b = loadImageMemSync(buf.m_pPtr, buf.m_nLen, bmp);

            LayaTexture* pLayaTexture = new LayaTexture(NULL,NULL, LayaTexture::TEXTURE_TYPE_NORMAL);
            pLayaTexture->setTextureSize(bmp.m_nWidth, bmp.m_nHeight);
            pLayaTexture->copyImageToTexture(&bmp,0,1,1);

            m_pTestMaterial = new Material();
            m_pTestMaterial->m_fAlpha = 1.0f;
            m_pTestMaterial->setShaderProgram((Shader*)m_pTestShader);
            m_pTestMaterial->m_nBlendType = Material::BLEND_TYPE_NORMAL;
            m_pTestMaterial->m_fColor[0]=1;
            m_pTestMaterial->m_fColor[1]=1;
            m_pTestMaterial->m_fColor[2]=0;
            m_pTestMaterial->m_fColor[3]=0;

            m_pTestMaterial->m_pTexture[0] = pLayaTexture;

            m_pTestMesh = new mesh();

            //VB
            m_pTestMesh->createVB(20,4);
            JCMemClass* pMemBuffer = m_pTestMesh->getVB()->getMemBuffer(0);
            pMemBuffer->clear();

            vertex v[4]={{-0.5f,0.5f,0.5f,0,0},	{0.5f,0.5f,0.5f,1,0},
                {0.5f,-0.5f,0.5f,1,1},{-0.5f,-0.5f,0.5f,0,1}};
            pMemBuffer->append(v,sizeof(v));

            //IB
            m_pTestMesh->createIB(GL_UNSIGNED_SHORT, 6);
            pMemBuffer = m_pTestMesh->getIB()->getMemBuffer(0);
            pMemBuffer->clear();
            unsigned short idx[6]={0,1,2,2,0,3};
            pMemBuffer->append(idx,sizeof(idx));

            VertexDesc* pVertDesc = m_pTestMesh->m_pVertDesc = new VertexDesc();
            pVertDesc->m_VertDescs.push_back(VertexDesc::Desc(gpuProgram::shader_AtrribPostion, GL_FLOAT_VEC3,0,0));
            pVertDesc->m_VertDescs.push_back(VertexDesc::Desc(gpuProgram::shader_AtrribTexCoord0,GL_FLOAT_VEC2,3*4,0));

            namedData* pDefMaterialDesc = getDefaultMaterialNamedData();
            renderer::bindShaderFetch(m_ShaderInfo,pVertDesc, nProgram, &pDefMaterialDesc,1);
            m_RenderGroupData.m_pShaderInfo = &m_ShaderInfo;
            m_RenderGroupData.m_pMaterial = m_pTestMaterial;
            m_RenderGroupData.m_nBegin=0;
            m_RenderGroupData.m_nEnd=6;
            //m_pTestMesh->m_pShaderInfo = &sinfo;
        }
        */
        /*
        void runLoop(){
            glClearColor(0,0.2f,0,1);
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

            //TEST
            JCMemClass* pMemBuffer = m_pTestMesh->getVB()->getMemBuffer(0);
            float* pVertex = (float*)pMemBuffer->getBuf();
            pVertex[0] = sinf(m_fV);
            m_fV+=0.1f;
            pMemBuffer->setChangedFlag(true);

            m_pTestMaterial->m_fColor[2]=pVertex[0];

            renderer::renderMesh(m_pTestMesh, &m_RenderGroupData, (char**)&m_pTestMaterial, 1);
        }
    };

    void _1_1_1_test(){
        class TestRender:public IGLRender{
        public:
            virtual void onGLReady(){
                r.setup();
            }

            virtual void renderFrame(long,bool){
                r.runLoop();
                GLvoid* pix=new char[100];
                glReadPixels(0,0,1,1,GL_RGBA,GL_UNSIGNED_BYTE,pix);
                int a=0;
                delete [] pix;
            }

            virtual void onGLDestory(){
                int a=0;
            }
            virtual void invalidGLRes(){}
            virtual void freeGLRes() {}
            void setProcess(ProcessInterface*) {}
            CRender1 r;
        };

        GLEnvThread	gl;
        TestRender render;
        gl.init(&render,800,600);
        winWindows::handleMessage(NULL);
    }

    void _1_1_2_test(){
        class TestRender:public IGLRender{
        public:
            virtual void onGLReady(){
                //编译shader
                m_pTestShader = new gpuProgram();
                char* pVSSrc="attribute vec3 g_Position;\
                    attribute vec2 g_TexCoord0;\
                    attribute vec2 g_TexCoord1;\
                    varying vec2 tex0;\
                    varying vec2 tex1;\
                    void main(){\
                        gl_Position = vec4(g_Position.x, g_Position.y, g_Position.z, 1.0);\
                        tex0=g_TexCoord0;\
                        tex1=g_TexCoord0;\
                    }";
                char* pPSSrc="precision mediump float;"
                    "uniform sampler2D g_Tex0;"
                    "uniform sampler2D g_Tex1;"
                    "varying vec2 tex0,tex1;"
                    "varying vec4 vertColor;"
                    "void main(){\
                        vec4 col1 = texture2D(g_Tex1,tex0);\n\
                        gl_FragColor=col1+texture2D(g_Tex0, tex0);\n\
                    }";
                m_pTestShader->setSrc(pVSSrc, pPSSrc);
                int nProgram = m_pTestShader->compile();
                //创建mesh
                m_pTestMesh = new mesh();
                //VB
                m_pTestMesh->createVB(20,4);
                JCMemClass* pMemBuffer = m_pTestMesh->getVB()->getMemBuffer(0);
                pMemBuffer->clear();

                vertex v[4]={{-0.5f,0.5f,0.5f,0,0},	{0.5f,0.5f,0.5f,1,0},
                    {0.5f,-0.5f,0.5f,1,1},{-0.5f,-0.5f,0.5f,0,1}};
                pMemBuffer->append(v,sizeof(v));

                //IB
                m_pTestMesh->createIB(GL_UNSIGNED_SHORT, 6);
                pMemBuffer = m_pTestMesh->getIB()->getMemBuffer(0);
                pMemBuffer->clear();
                unsigned short idx[6]={0,1,2,2,0,3};
                pMemBuffer->append(idx,sizeof(idx));

                //Texture
                buffer buf;
                BitmapData bmp;
                readFileSync("d:/temp/gameicon.png",buf);
                bool b = loadImageMemSync(buf.m_pPtr, buf.m_nLen, bmp);
                LayaTexture* pTex1=new LayaTexture(NULL,NULL,100,100,0);
                pTex1->copyImageToTexture(&bmp,0,1,1);

                readFileSync("d:/temp/test.jpg",buf);
                b = loadImageMemSync(buf.m_pPtr, buf.m_nLen, bmp);
                //createGridBmp(bmp,800,800,20);
                LayaTexture* pTex2=new LayaTexture(NULL,NULL,100,100,0);
                pTex2->copyImageToTexture(&bmp,0,1,1);

                //创建材质
                m_pTestMaterial = new Material();
                m_pTestMaterial->m_fAlpha = 1.0f;
                m_pTestMaterial->setShaderProgram((Shader*)m_pTestShader);
                m_pTestMaterial->m_nBlendType = Material::BLEND_TYPE_NORMAL;
                m_pTestMaterial->m_fColor[0]=1;

                m_pTestMaterial->m_pTexture[0] = pTex1;
                m_pTestMaterial->m_pTexture[1] = pTex2;

                VertexDesc* pVertDesc = m_pTestMesh->m_pVertDesc = new VertexDesc();
                pVertDesc->m_VertDescs.push_back(VertexDesc::Desc(gpuProgram::shader_AtrribPostion, GL_FLOAT_VEC3,0,0));
                pVertDesc->m_VertDescs.push_back(VertexDesc::Desc(gpuProgram::shader_AtrribTexCoord0,GL_FLOAT_VEC2,3*4,0));

                namedData* pDefMaterialDesc = getDefaultMaterialNamedData();
                renderer::bindShaderFetch(m_ShaderInfo,pVertDesc, nProgram, &pDefMaterialDesc,1);
                m_RenderGroupData.m_pShaderInfo = &m_ShaderInfo;
                m_RenderGroupData.m_pMaterial = m_pTestMaterial;
                m_RenderGroupData.m_nBegin=0;
                m_RenderGroupData.m_nEnd=6;
            }

            virtual void renderFrame(long,bool){
                glClearColor(0.4f,0.2f,0.2f,1);
                glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
                renderer::renderMesh(m_pTestMesh, &m_RenderGroupData, (char**)&m_pTestMaterial, 1);
            }

            virtual void invalidGLRes(){}
            virtual void freeGLRes() {}
            virtual void setProcess(ProcessInterface* p_pCurProcess) {}

        protected:
            gpuProgram*	m_pTestShader;
            mesh*	m_pTestMesh;
            ShaderLinkInfo m_ShaderInfo;
            RenderGroupData	m_RenderGroupData;
            Material*	m_pTestMaterial;
        };
        TestRender render;
        GLEnvThread gl;
        gl.init(&render,600,600);
        winWindows::handleMessage(NULL);
    }
    */
    /*
    void _1_2_1_test3d(){
        struct vertex{
            float x,y,z;
            float u,v;
            float nx,ny,nz;
            //unsigned int color;
        };
        struct rundata{
            Matrix44    matworld;
            Matrix44    matpers;
        };

        class render3d_1:public IGLRender{
        public:
            void onGLReady(){
                //shader
                m_pShader = new gpuProgram();
                char* pVSSrc="precision mediump float; \
                             attribute vec3 g_Position;\
                             attribute vec2 g_TexCoord0;\
                             attribute vec3 g_Normal; \
                             attribute float g_alpha; \
                             uniform mat4 g_worldmatrix; \
                             uniform mat4 g_persmat; \
                             varying vec2 tex0;\
                             varying vec3 norm; \
                             varying float alpha; \
                             void main(){\
                             vec4 vpos = vec4(g_Position.xyz,1.0); \
                             gl_Position = g_persmat*g_worldmatrix*vpos; \
                             norm = g_Normal; \
                             tex0=g_TexCoord0;\
                             alpha = g_alpha; \
                             }";
                char* pPSSrc="precision mediump float;"
                    "uniform sampler2D g_Tex0;"
                    "uniform sampler2D g_Tex1;"
                    "varying vec2 tex0,tex1;"
                    "varying vec3 norm;"
                    "varying vec4 vertColor;"
                    "varying float alpha;"
                    "void main(){\
                    vec4 col1 = texture2D(g_Tex1,tex0);\n\
                    gl_FragColor=vec4(col1.xyz, 1.0);\
                    }";
                m_pShader->setSrc(pVSSrc, pPSSrc);
                int nProgram = m_pShader->compile();
                int nVertNum=24;
                //TODO 需要测试直接创建多流的情况
                JCMeshBuilder::Geom geo;
                JCMeshBuilder::BuildBoxGeo(true,.8f, .8f, .8f,geo);
                VertexDesc* pVertDesc = m_mesh.m_pVertDesc = new VertexDesc();
                //0
                pVertDesc->m_VertDescs.push_back(VertexDesc::Desc(gpuProgram::shader_AtrribPostion, GL_FLOAT_VEC3,0,0));
                pVertDesc->m_VertDescs.push_back(VertexDesc::Desc(gpuProgram::shader_AtrribTexCoord0, GL_FLOAT_VEC2,3*4,0));
                pVertDesc->m_VertDescs.push_back(VertexDesc::Desc("g_Normal", GL_FLOAT_VEC3,5*4,0));
                //1
                pVertDesc->m_VertDescs.push_back(VertexDesc::Desc("g_alpha", GL_FLOAT,sizeof(vertex)*nVertNum,1));

                int idxnum=36;
                m_mesh.createVB(sizeof(vertex),nVertNum);
                m_mesh.createIB(GL_UNSIGNED_SHORT,idxnum);
                m_mesh.addStream(sizeof(float));	//alpha
                JCMemClass* pgeo = m_mesh.getVBMem(0);
                JCMemClass* palpha = m_mesh.getVBMem(1);
                JCMemClass* pidx = m_mesh.getIB()->getMemBuffer(0);

                //通知一下实际大小。
                pgeo->appendEmpty(sizeof(vertex)*nVertNum);
                palpha->appendEmpty(nVertNum*sizeof(float));
                vertex* pS1 = (vertex*)pgeo->getBuf();

                for( int i=0; i<nVertNum; i++){
                    pS1->x = *(geo.pVertex)++;
                    pS1->y = *geo.pVertex++;
                    pS1->z = *geo.pVertex++;

                    pS1->u = *geo.pUV++;
                    pS1->v = *geo.pUV++;

                    pS1->nx = *geo.pNorm++;
                    pS1->ny = *geo.pNorm++;
                    pS1->nz = *geo.pNorm++;
                    pS1++;
                }
                //memcpy(pS1,v,sizeof(v));

                char* pS2 = palpha->getBuf();
                //memset(pS2,0,palpha->size());
                float* pA =(float*)pS2;
                for(int i=0; i<nVertNum; i++)
                    pA[i]=1.0f;
                pA[1]=0.5f;

                //index
                pidx->appendEmpty(geo.nIdxNum*sizeof(unsigned short));
                char* pidxdt = pidx->getBuf();
                memcpy(pidxdt, geo.pIdx,geo.nIdxNum*sizeof(unsigned short));

                //Texture
                BitmapData bmp = loadLocalImageSync("d:/temp/test.jpg");
                LayaTexture* pTex1=new LayaTexture(NULL,NULL,200,200,0);
                pTex1->copyImageToTexture(&bmp,0,1,1);
                delete [] bmp.m_pImageData;

                //创建材质
                m_pMaterial = new Material();
                m_pMaterial->m_fAlpha = 1.0f;
                m_pMaterial->setShaderProgram((Shader*)m_pShader);
                m_pMaterial->m_nBlendType = Material::BLEND_TYPE_NORMAL;
                m_pMaterial->m_fColor[0]=1;
                m_pMaterial->m_pTexture[0] = pTex1;
                m_pMaterial->m_bEnableZ=true;

                namedData* pDefMaterialDesc = getDefaultMaterialNamedData();
                namedData* pRundata = new namedData();
                pRundata->add("g_worldmatrix",0,namedData::tp_mat4,1);
                pRundata->add("g_persmat",(int)(&((rundata*)0)->matpers), namedData::tp_mat4,1);

                namedData* datadesc[2]={pDefMaterialDesc,pRundata};
                renderer::bindShaderFetch(m_ShaderInfo,pVertDesc, nProgram, datadesc,2);
                m_RenderGroupData.m_pShaderInfo = &m_ShaderInfo;
                m_RenderGroupData.m_pMaterial = m_pMaterial;
                m_RenderGroupData.m_nBegin=0;
                m_RenderGroupData.m_nEnd=idxnum;

                //TODO
                glDisable(GL_CULL_FACE);
                glEnable(GL_DEPTH_TEST);
                glEnable(GL_ALPHA_TEST);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glEnable(GL_BLEND);
                int a=0;
            }
            void renderFrame(long,bool){
                glClearColor(0.4f,0.2f,0.2f,1);
                glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
                Matrix44 matw;
                static float f=(Pi/4.0f);
                f+=0.01f;
                matw = Matrix44::CreateFromAxisAngle(float3(1,1.0f,0),f);
                Matrix44 matper = Matrix44::CreatePerspective(800,600,1,10);
                Matrix44& mat = m_RunData.matworld;
                matw.M43 = -2;
                mat = matw;
                //mat.M41=sinf(f);
                //mat.M43 = 0.6f;
                Matrix44& persmat = m_RunData.matpers;
                persmat =Matrix44::CreatePerspectiveFieldOfView(80.0f*Pi/180.0f,1.0f,0.1f,10.0f);
                char* pdata[2];
                pdata[0]=(char*)m_pMaterial;
                pdata[1]=(char*)&m_RunData;
                if(0)
                    renderer::renderMesh(&m_mesh, &m_RenderGroupData, (char**)pdata, 2);
                else{
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    glEnable(GL_DEPTH_TEST);
                    GLuint texs[2]={m_pMaterial->m_pTexture[0]->m_pGpuTexture,m_pMaterial->m_pTexture[0]->m_pGpuTexture};
                    renderer::renderMesh1(&m_mesh,m_RenderGroupData.m_nBegin, m_RenderGroupData.m_nEnd, m_pMaterial->m_pShaderProgram->m_nProgram,
                        (ShaderLinkInfo*)m_RenderGroupData.m_pShaderInfo, (int*)texs,2,(int*)texs,2,pdata,2);
                }
                mat.M41=0.2f;
                //renderer::renderMesh(&m_mesh, &m_RenderGroupData, (char**)pdata, 2);
            }
            virtual void invalidGLRes() {}
            virtual void freeGLRes() {}
            virtual void setProcess(ProcessInterface* p_pCurProcess) {}

            mesh m_mesh;
            gpuProgram*	m_pShader;
            Material*   m_pMaterial;
            rundata     m_RunData;
            ShaderLinkInfo m_ShaderInfo;
            RenderGroupData	m_RenderGroupData;
        };

        render3d_1 renderor;
        GLEnvThread gl;
        gl.init(&renderor,800,600);
        winWindows::handleMessage(NULL);
    }

    void _1_2_1_test3dmodel(){
        class Renderer:public IGLRender{
        };
    }

    void _2_1_test(){
        Process proc(NULL);
        LayaTexture* pTex = new LayaTexture(NULL,NULL,2048,2048,0);
        pTex->setScale( 2,3,2,3);
        BitmapData bmpdata;
        createGridBmp(bmpdata,20,20,4);
        //
        BitmapData downd;
        int frac[]={1,2,1,2};
        downsampleBmp(bmpdata,downd,frac,false);
        ImageBaseInfo ifo={downd.m_nWidth,downd.m_nHeight,32};
        saveAsJpeg(downd.m_pImageData, ifo,"d:/temp/ss.jpg");
        int borderFlag = ResBase::BOARDER_TYPE_RIGHT|ResBase::BOARDER_TYPE_BOTTOM;
        //这个函数会缩小贴图
        //pTex->copyImageToTexture(&bmpdata,borderFlag,1,1);
        //pTex->mergeTexture(&bmpdata,borderFlag,0,0,1,1);
        int rssz = pTex->getResSize();
        int width = pTex->getWidth();
    }

    void _3_1_testdrawImageCB(resStateDispatcher*, bool* bEnd){
        *bEnd=true;
    }
    void _3_1_testOnUpdate(void* pParam){
        if(pParam){
            int* pPars=(int*)pParam;
            RenderImg* pRImg = (RenderImg*)pPars[0];
            LayaContext* pCtx = (LayaContext*)pPars[1];
            //p_nPaintKey为0则不会引起重新更新mesh
            //pRImg->render(pCtx,0,NULL,0,0,pRImg->m_pImage->getWidth(),pRImg->m_pImage->getHeight());
            pRImg->render(pCtx,1,NULL,0.0f,0.0f,(float)pRImg->m_pImage->getWidth(),(float)pRImg->m_pImage->getHeight());
        }
    }
    void _3_1_testdrawImage(){
        ConchRender* pRender = new ConchRender();
        Process* pProc = new Process(NULL);
        pProc->m_onprocessupdate = _3_1_testOnUpdate;
        pRender->setProcess(pProc);
        pProc->setRunParam("not.exist.js","");
        pProc->active();
        Sleep(100);
        //Node* n1 = new Node(pProc,0,NULL);
        Image img("");
        bool bLoadEnd=false;
        img.setSrc(pProc,"d:/temp/test.jpg");// gameicon.png");
        img.setOnReadyCB(boost::bind(_3_1_testdrawImageCB,_1,&bLoadEnd));
        //img.rect(0,0,100,100);
        {int i=0; while(!bLoadEnd && i<1000){i++;Sleep(10);}}
        //LayaContext ctx(NULL, pProc);
        //TODO 问题 process中会自动创建一个 rootcontext， 是不是有点不灵活
        LayaContext* pRootCtx = pProc->getRootContext();
        pRootCtx->transform(1,0,0,1,0,0);
        pRootCtx->disableClip(true);
        //下面的正常是在js线程中
        RenderImg ri;
        ri.setImg(&img);
        ri.render(pRootCtx,0,NULL,0,0,100,100);
        void* param[2];
        param[0]=&ri;
        param[1]=pRootCtx;
        pProc->m_pOnProcessUpdateParam = param;

        GLEnvThread	gl;
        gl.init(pRender,800,600);
        winWindows::handleMessage(NULL);
        //Sleep(10000);
        //delete n1;
    }

    void init_3(){
    }

    void uninit_3(){
    }

    void _0_1_test(){
    }
    */

    void _1_testRender_GLTread_simp() {
        class TestRender :public JCIGLRender {
        public:
            struct Vertex {
                float pos[3];
                int color;
                Vertex(float x, float y, float z, int c) {
                    pos[0] = x; pos[1] = y; pos[2] = z;
                    color = c;
                }
            };

            int             nFrmNum = 0;
            std::function<void()> funcExit;
            JCConchMesh*         m_pMesh = nullptr;
            JCGpuProgramTemplate* m_pShaderTemp = nullptr;
            JCGpuProgram*   m_pProg = nullptr;
            JCMaterial*     m_pMaterial = nullptr;
            JCShaderLink_Vertex m_LinkVertex;
            JCShaderLink_Uniform m_LinkUniform;
            JCRenderGroupData   m_RenderGroup;
            TestRender(std::function<void()> f) {
                funcExit = f;
            }

            virtual void onGLReady() {
                m_pMesh = new JCConchMesh();
                m_pMesh->regVertexDesc({ { "g_Pos",GL_FLOAT_VEC3 },{ "color",GL_INT } }, 1);

                m_pShaderTemp = new JCGpuProgramTemplate(
                    R"(
                    attribute vec3 g_Pos;
				    void main(){
					    gl_Position = vec4(g_Pos.x, g_Pos.y, g_Pos.z, 1.0);
				    }
                )",
                    R"(
                    precision mediump float;
                    void main(){
                       gl_FragColor=vec4(1,1,0,1);
				    }
                    )");
                m_pProg = m_pShaderTemp->getInst("", 0);
                int nProg = m_pProg->getGpuProgram();
                m_pMaterial = new JCMaterial();
                m_pMaterial->setBlendType(JCMaterial::BLEND_TYPE_SOURCE_OVER);
                m_pMaterial->setAlpha(1.0f);
                m_pMaterial->setShaderProgram(m_pProg);
                JCNamedData* pMaterialDesc = m_pMaterial->getNamedData();
                JCVertexDesc& VDesc = m_pMesh->getVertexDesc(1);
                JCRenderer::link_shader_vertex_uniformdata(m_LinkVertex, m_LinkUniform, &VDesc, m_pProg, &pMaterialDesc, 1,nullptr);

                JCRenderGroupData* pGroup = m_pMesh->pushElements(1, m_pMaterial, 4, {
                        -0.05f,0.05f,0.5f,0 ,  
                        0.05f,0.05f,0.5f,0 ,
                        0.05f,-0.05f,0.5f,0 ,
                        -0.05f,-0.05f,0.5f,0  
                },{
                    0,1,2,2,3,0
                });
                pGroup->m_pShaderVertexInfo = &m_LinkVertex;
                pGroup->m_pShaderUniformInfo = &m_LinkUniform;
            }

            virtual void renderFrame(long p_nCurrentFrame, bool) {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glClearColor(0.7f, 0.7f,0.7f, 1.0f);
                nFrmNum++;
                if (nFrmNum > 100) {
                    funcExit();
                }

                auto sttm = tmGetCurms();
                for (int i = 0; i < 100000; i++) {
                    JCRenderer::renderMesh(m_pMesh, &m_pMesh->m_vRenderGroupData[0], nullptr, 0);
                }
                int dt = tmGetCurms() - sttm;
                LOGE("+++++ dt=%d",dt);
            }

            virtual void invalidGLRes() {
            }
            virtual void freeGLRes() {
            }
        };

        GLEnvThread gl;
        TestRender render([&gl]() { gl.m_pWindows->close(); });
        gl.init(&render, 800, 600);
        winWindows::handleMessage(NULL);
    }

    void _1_testRender_GLTread_multiGroups() {
        //不同材质
        //不同面类型
        //不同顶点格式
        class TestRender :public JCIGLRender {
        public:
            struct Vertex {
                float pos[3];
                int color;
                Vertex(float x, float y, float z, int c) {
                    pos[0] = x; pos[1] = y; pos[2] = z;
                    color = c;
                }
            };

            int             nFrmNum = 0;
            std::function<void()> funcExit;
            JCConchMesh*         m_pMesh = nullptr;
            JCGpuProgramTemplate* m_pTemp1= nullptr;
            JCGpuProgramTemplate* m_pTemp2 = nullptr;
            JCGpuProgramTemplate* m_pTemp3 = nullptr;
            JCGpuProgram*   m_pProg = nullptr;
            JCGpuProgram*   m_pProg1 = nullptr;
            JCGpuProgram*   m_pProg2 = nullptr;
            JCMaterial*     m_pMtl = nullptr;
            JCMaterial*     m_pMtl1 = nullptr;
            JCMaterial*     m_pMtl2 = nullptr;
            JCShaderLink_Vertex    m_LinkV,m_LinkV1,m_LinkV2;
            JCShaderLink_Uniform   m_LinkU, m_LinkU1, m_LinkU2;
            JCRenderGroupData   m_RenderGroup;
            TestRender(std::function<void()> f) {
                funcExit = f;
            }

            virtual void onGLReady() {
                m_pMesh = new JCConchMesh();
                m_pMesh->regVertexDesc({ { "g_Pos",GL_FLOAT_VEC3 },{ "color",GL_INT } }, 1);
                m_pMesh->regVertexDesc({ {"g_Pos",GL_FLOAT_VEC2} }, 2);

                //mtl
                m_pProg = (new JCGpuProgramTemplate(
                R"(
                    attribute vec3 g_Pos;
				    void main(){
					    gl_Position = vec4(g_Pos.x, g_Pos.y, g_Pos.z, 1.0);
				    }
                )", R"(
                    precision mediump float;
                    void main(){
                       gl_FragColor=vec4(1,1,0,1);
				    }
                    )"))->getInst("",0);

                m_pMtl = new JCMaterial();
                m_pMtl->setBlendType(JCMaterial::BLEND_TYPE_SOURCE_OVER);
                m_pMtl->setAlpha(1.0f);
                m_pMtl->setShaderProgram(m_pProg);

                //mtl1
                m_pProg1 = (new JCGpuProgramTemplate(R"(
                    attribute vec3 g_Pos;
				    void main(){
					    gl_Position = vec4(g_Pos.x, g_Pos.y, g_Pos.z, 1.0);
				    }
                )", R"(
                    precision mediump float;
                    void main(){
                       gl_FragColor=vec4(1,0,0,1);
				    }
                    )"))->getInst("",0);
                m_pMtl1 = new JCMaterial();
                m_pMtl1->setBlendType(JCMaterial::BLEND_TYPE_SOURCE_OVER);
                m_pMtl1->setAlpha(1.0f);
                m_pMtl1->setShaderProgram(m_pProg1);

                //mtl2
                m_pProg2 = (new JCGpuProgramTemplate(R"(
                    attribute vec2 g_Pos;
				    void main(){
					    gl_Position = vec4(g_Pos.x, g_Pos.y, 0.5, 1.0);
				    }
                )", R"(
                    precision mediump float;
                    void main(){
                       gl_FragColor=vec4(0,1.0,0,1.0);
				    }
                    )"))->getInst("",0);
                m_pMtl2 = new JCMaterial();
                m_pMtl2->setBlendType(JCMaterial::BLEND_TYPE_SOURCE_OVER);
                m_pMtl2->setAlpha(1.0f);
                m_pMtl2->setShaderProgram(m_pProg1);

                //link
                JCNamedData* pMaterialDesc = m_pMtl->getNamedData();
                JCVertexDesc& VDesc = m_pMesh->getVertexDesc(1);
                JCVertexDesc& VDesc1 = m_pMesh->getVertexDesc(2);
                JCRenderer::link_shader_vertex_uniformdata(
                    m_LinkV, m_LinkU, &VDesc, m_pProg, &pMaterialDesc, 1,
                    nullptr
                    );
                JCRenderer::link_shader_vertex_uniformdata(
                    m_LinkV1,m_LinkU1, &VDesc, m_pProg1, &pMaterialDesc, 1,nullptr);
                JCRenderer::link_shader_vertex_uniformdata(
                    m_LinkV2, m_LinkU2, &VDesc1, m_pProg2, &pMaterialDesc, 1,nullptr);

                float xoff = -0.7f, yoff = 0.0f;
                JCRenderGroupData* pGroup = m_pMesh->pushElements(1, m_pMtl, 4, {
                    -0.05f+xoff,0.05f+yoff,0.5f,0 ,
                    0.05f+xoff,0.05f+yoff,0.5f,0 ,
                    0.05f+xoff,-0.05f+yoff,0.5f,0 ,
                    -0.05f+xoff,-0.05f+yoff,0.5f,0
                }, {
                    0,1,2,2,3,0
                });
                pGroup->m_pShaderVertexInfo = &m_LinkV;
                pGroup->m_pShaderUniformInfo = &m_LinkU;

                xoff = -0.59f;
                m_pMesh->pushElements(1, m_pMtl, 4, {
                    -0.05f + xoff,0.05f + yoff,0.5f,0 ,
                    0.05f + xoff,0.05f + yoff,0.5f,0 ,
                    0.05f + xoff,-0.05f + yoff,0.5f,0 ,
                    -0.05f + xoff,-0.05f + yoff,0.5f,0
                }, {
                    0,1,2,2,3,0
                });
                pGroup->m_pShaderVertexInfo = &m_LinkV1;
                pGroup->m_pShaderUniformInfo = &m_LinkU1;

                xoff = -0.48f;
                pGroup= m_pMesh->pushElements(1, m_pMtl1, 4, {
                    -0.05f + xoff,0.05f + yoff,0.5f,0 ,
                    0.05f + xoff,0.05f + yoff,0.5f,0 ,
                    0.05f + xoff,-0.05f + yoff,0.5f,0 ,
                    -0.05f + xoff,-0.05f + yoff,0.5f,0
                }, {
                    0,1,2,2,3,0
                });
                pGroup->m_pShaderVertexInfo = &m_LinkV1;
                pGroup->m_pShaderUniformInfo = &m_LinkU1;

                //画线
                float w = 800;
                float h = 600;
                pGroup = m_pMesh->pushVertex(1, GL_LINE_STRIP, m_pMtl, 3, {
                    375.0f/w,117.0f/h,0.5f,0,
                    636.0f/w,405.0f/h,0.5f,0,
                    207.0f/w,405.0f/h,0.5f,0
                });
                pGroup->m_pShaderVertexInfo = &m_LinkV;
                pGroup->m_pShaderUniformInfo = &m_LinkU;


                //不同的顶点格式
                pGroup = m_pMesh->pushVertex(2, GL_TRIANGLE_FAN, m_pMtl2, 7, {
                    325.0f / w,329.0f / h,
                    320.0f / w,155.0f / h,
                    486.0f / w,240.0f / h,
                    488.0f / w,410.0f / h,
                    319.0f / w,493.0f / h,
                    155.0f / w,412.0f / h,
                    152.0f / w,241.0f / h
                });
                pGroup->m_pShaderVertexInfo = &m_LinkV2;
                pGroup->m_pShaderUniformInfo = &m_LinkU2;

            }

            virtual void renderFrame(long p_nCurrentFrame, bool) {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
                nFrmNum++;
                if (nFrmNum > 10000) {
                    funcExit();
                }

                for (auto& g : m_pMesh->m_vRenderGroupData) {
                    JCRenderer::renderMesh(m_pMesh, &g, nullptr, 0);
                }
            }

            virtual void invalidGLRes() {
            }
            virtual void freeGLRes() {
            }
        };

        GLEnvThread gl;
        TestRender render([&gl]() { gl.m_pWindows->close(); });
        gl.init(&render, 800, 600);
        winWindows::handleMessage(NULL);
    }

    void testCanvas_1() {

    }

    void _1_testRender_GLTread_mask() {
        class TestRender :public JCIGLRender {
        public:
            struct Vertex {
                float _x,_y,_u1,_v1,_u2,_v2;
                int color;
                Vertex(float x, float y, float u1, float v1, float u2, float v2, int c) {
                    _x = x; _y = y; _u1 = u1; _v1 = v1; _u2 = u2; _v2 = v2;
                    color = c;
                }
            };

            int             nFrmNum = 0;
            std::function<void()> funcExit;
            JCConchMesh*         m_pMesh = nullptr;
            JCGpuProgram*   m_pProg = nullptr;
            JCMaterial*     m_pMaterial = nullptr;
            JCShaderLink_Vertex m_LinkInfoV;
            JCShaderLink_Uniform m_LinkInfoU;
            JCRenderGroupData   m_RenderGroup;
            //JCImage             m_imgBk, m_imgMask;
            JCTexture           *m_pTex1, *m_pTex2;
            TestRender(std::function<void()> f) {
                funcExit = f;
            }

            virtual void onGLReady() {
                m_pMesh = new JCConchMesh();
                m_pMesh->regVertexDesc({ { "POSUV",GL_FLOAT_VEC4 },{ "uv2",GL_FLOAT_VEC2 } }, 1);

                m_pProg = (new JCGpuProgramTemplate(R"(
attribute vec4 g_posuv;
attribute vec2 uv2;   
varying vec2 texCoord1;
varying vec2 texCoord2;
void main(){
	gl_Position = vec4(g_posuv.x, g_posuv.y, 0.5, 1.0);
    texCoord1 = g_posuv.zw;
    texCoord2 = uv2;
}
                )",
                    R"(
precision mediump float;
varying vec2 texCoord1;
varying vec2 texCoord2;
uniform sampler2D g_Tex0;
uniform sampler2D g_Tex1;
void main(){
    vec4 color1 = texture2D(g_Tex0, texCoord1.xy);
    vec4 color2 = texture2D(g_Tex1, texCoord2.xy);//mask
    gl_FragColor=vec4(color1.xyz,color1.w*color2.r);
}
                    )"))->getInst("",0);
                int nProg = m_pProg->getGpuProgram();
                m_pProg->m_pTemp->setSemantics("abc,ddd,g_posuv,POSUV");
                m_pMaterial = new JCMaterial();
                m_pMaterial->setBlendType(JCMaterial::BLEND_TYPE_SOURCE_OVER);
                m_pMaterial->setAlpha(1.0f);
                m_pMaterial->setShaderProgram(m_pProg);
                JCNamedData* pMaterialDesc = m_pMaterial->getNamedData();
                JCVertexDesc& VDesc = m_pMesh->getVertexDesc(1);
                JCRenderer::link_shader_vertex_uniformdata(
                    m_LinkInfoV,m_LinkInfoU, &VDesc, m_pProg, &pMaterialDesc, 1,nullptr);

                JCRenderGroupData* pGroup = m_pMesh->pushElements(1, m_pMaterial, 4, {
                    -0.5f,0.5f,0,0,0,0,
                    0.5f,0.5f,0.5,0,1,0,
                    0.5f,-0.5f,0.5,0.5,1,1,
                    -0.5f,-0.5f,0,0.5,0,1
                }, {
                    0,1,2,2,3,0
                });
                pGroup->m_pShaderUniformInfo = &m_LinkInfoU;
                pGroup->m_pShaderVertexInfo = &m_LinkInfoV;
                //两张贴图，模拟遮罩
                JCBuffer buf;
                BitmapData bmp;
                readFileSync("F:/work/conch5gitsvn/ScriptTest/images/bg2.png", buf);
                bool b = loadImageMemSync(buf.m_pPtr, buf.m_nLen, bmp);
                m_pTex1 = new JCTexture(nullptr, TEXTURE_TYPE_NORMAL, bmp.m_nWidth,
                    bmp.m_nHeight);
                m_pTex1->mergeBitmapDataToTexture(&bmp, (BOARDER_TYPE)0, 0, 0);
                m_pMaterial->setTexture(0, m_pTex1);

                readFileSync("F:/work/conch5gitsvn/ScriptTest/images/mask.png", buf);
                b = loadImageMemSync(buf.m_pPtr, buf.m_nLen, bmp);
                m_pTex2 = new JCTexture(nullptr, TEXTURE_TYPE_NORMAL, bmp.m_nWidth,
                    bmp.m_nHeight);
                m_pTex2->mergeBitmapDataToTexture(&bmp, (BOARDER_TYPE)0, 0, 0);
                m_pMaterial->setTexture(1, m_pTex2);

            }

            virtual void renderFrame(long p_nCurrentFrame, bool) {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
                glEnable(GL_BLEND);
                nFrmNum++;
                if (nFrmNum > 10000) {
                    funcExit();
                }

                JCRenderer::renderMesh(m_pMesh, &m_pMesh->m_vRenderGroupData[0], nullptr, 0);
            }

            virtual void invalidGLRes() {
            }
            virtual void freeGLRes() {
            }
        };

        GLEnvThread gl;
        TestRender render([&gl]() { gl.m_pWindows->close(); });
        gl.init(&render, 800, 600);
        winWindows::handleMessage(NULL);
    }

    void testMain() {
        //_1_testRender_GLTread_simp();
        //_1_testRender_GLTread_multiGroups();
        _1_testRender_GLTread_mask();
        /*
        //TODO 下面这个不太方便
        g_DecThread = new workerThread(true);
        g_DecThread->setThreadName("image decode");
        _1_2_1_test3d();
        _1_2_1_test3dmodel();
        _1_1_2_test();
        //_2_1_test();
        init_3();
        _3_1_testdrawImage();
        uninit_3();
        _1_1_1_test();
        //testRender3D_1();
        delete g_DecThread;
        */
    }
    ADDTESTCASE(testMain, "renderTest");
}
#endif
