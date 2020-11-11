#ifdef _TEST_
/*
    渲染流程的测试。
        渲染流程是可拆分的

    用来拆分项目
*/

#include "../JCTestManager.h"
#include <Windows.h>
#include "../../WindowsEnv/winWindows.h"
#include "../../WindowsEnv/gles.h"
#include <misc/JCWorkerThread.h>
#include <JCIGLRender.h>
#include <functional>
#include "MonkVG/include/MonkVG/openvg.h"
#include "MonkVG/include/MonkVG/vgext.h"
#include "MonkVG/include/MonkVG/vgu.h"
#include <util/Log.h>
#include "mkContext.h"
/*
*/

using namespace laya;

namespace renderSVGTest{
    void _1_testSVGRender_fillcolor(){
        class TestSVGRender:public JCIGLRender{
        public:
            float   fR = 0.0f;
            int     nFrmNum = 0;
            VGPaint m_Paint;
            VGPaint m_StrokePaint;
            VGPath  m_Path;
            MkVGContext* vg = nullptr;
            std::function<void()> funcExit;
            TestSVGRender(std::function<void()> f) {
                funcExit = f;
            }

            void init() {
                vg = new MkVGContext(800, 600, VG_RENDERING_BACKEND_TYPE_OPENGLES20,NULL,NULL,NULL);
                m_Paint = vg->CreatePaint();
                vg->SetPaint(m_Paint, VG_FILL_PATH);//填充
                VGfloat color[4] = {1.0f,0.0f,0.0f,1.0f};
                vg->SetParameterfv(m_Paint, VG_PAINT_COLOR, 4, color);

                m_StrokePaint = vg->CreatePaint();
                vg->SetPaint(m_StrokePaint, VG_STROKE_PATH);
                VGfloat colstroke[4] = { 0,1.0f,0,1.0f };
                vg->SetParameterfv(m_StrokePaint, VG_PAINT_COLOR, 4, colstroke);

                m_Path = vg->CreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 0, 0, VG_PATH_CAPABILITY_ALL);
                //vguRoundRect(m_Path, 10, 10, 100, 300,10,10);
                //VGfloat pts[] = { 155, 231, 485, 85, 617, 473, 865, 340,155, 231 }; //简单交叉
                //复杂形状，不交叉
                VGfloat pts[] = {241,150,362,151,356,180,265,187,270,205,349,202,349,230,262,231,267,306,235,309};
                //vguPolygon(m_Path, pts, sizeof(pts)/sizeof(float)/2, VG_TRUE);
                int a = 0;
            }

            virtual void onGLReady(){
                init();
            }

            virtual void renderFrame(long p_nCurrentFrame,bool){
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glClearColor(0.5,.5f,.5f,1.0f);
                //render path
                auto sttm = tmGetCurms();
                for (int i = 0; i < 100000; i++) {
                    vg->Seti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
                    vg->LoadIdentity();
                    VGfloat mat[] = {1,0,0, 0,-1.0f,600, 0,0,1};
                    vg->LoadMatrix(mat);
                    vg->SetPaint(m_Paint, VG_FILL_PATH);
                    vg->SetPaint(m_StrokePaint, VG_STROKE_PATH);
                    vg->Setf(VG_STROKE_LINE_WIDTH, 3.0f);
                    vg->DrawPath(m_Path, VG_FILL_PATH | VG_STROKE_PATH);
                }
                //目前是<1000
                LOGE("---dt=%d",(int)(tmGetCurms()-sttm));
                nFrmNum++;
                if (nFrmNum > 1000) {
                    funcExit();
                }
            }

            virtual void invalidGLRes() {
            }
            virtual void freeGLRes() {
            }
        };

        GLEnvThread gl;
        TestSVGRender render([&gl](){ gl.m_pWindows->close(); });
        gl.init(&render,800,600);
        winWindows::handleMessage(NULL);
    }

    void _1_testSVGRender_fillLinearG() {
        class TestSVGRender :public JCIGLRender {
        public:
            int     nFrmNum = 0;
            VGPaint m_Paint;
            VGPaint m_StrokePaint;
            VGPath  m_Path;
            MkVGContext* vg = nullptr;
            std::function<void()> funcExit;
            TestSVGRender(std::function<void()> f) {
                funcExit = f;
            }

            void init() {
                vg = new MkVGContext(800, 600, VG_RENDERING_BACKEND_TYPE_OPENGLES20,NULL,NULL,NULL);
                m_Paint = vg->CreatePaint();
                vg->SetPaint(m_Paint, VG_FILL_PATH);//填充
                VGfloat pos[4] = { 100,0,200,0};//x0,y0,x1,y1。 如果是放射性的话，就是 cx,cy,fx,fy,r
                const static int NUM_STOPS = 2;
                VGfloat stops[5*NUM_STOPS];
                vg->SetParameterfv(m_Paint, VG_PAINT_LINEAR_GRADIENT, 4, pos);
                vg->SetParameterfv(m_Paint, VG_PAINT_COLOR_RAMP_STOPS, 5 * NUM_STOPS, stops);
                //vgSetParameteri(m_Paint, VG_PAINT_COLOR_RAMP_SPREAD_MODE, VG_SPREAD_MODE_PAD);


                m_StrokePaint = vg->CreatePaint();
                vg->SetPaint(m_StrokePaint, VG_STROKE_PATH);
                VGfloat colstroke[4] = { 0,1.0f,0,1.0f };
                vg->SetParameterfv(m_StrokePaint, VG_PAINT_COLOR, 4, colstroke);

                m_Path = vg->CreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 0, 0, VG_PATH_CAPABILITY_ALL);
                //vguRoundRect(m_Path, 10, 10, 100, 300,10,10);
                //VGfloat pts[] = { 155, 231, 485, 85, 617, 473, 865, 340,155, 231 }; //简单交叉
                //复杂形状，不交叉
                VGfloat pts[] = { 241,150,362,151,356,180,265,187,270,205,349,202,349,230,262,231,267,306,235,309 };
                //vguPolygon(m_Path, pts, sizeof(pts) / sizeof(float) / 2, VG_TRUE);
                int a = 0;
            }

            virtual void onGLReady() {
                init();
            }

            virtual void renderFrame(long p_nCurrentFrame, bool) {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glClearColor(0.5, .5f, .5f, 1.0f);
                //render path
                auto sttm = tmGetCurms();
                for (int i = 0; i < 100000; i++) {
                    vg->Seti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
                    vg->LoadIdentity();
                    VGfloat mat[] = { 1,0,0, 0,-1.0f,600, 0,0,1 };
                    vg->LoadMatrix(mat);
                    vg->SetPaint(m_Paint, VG_FILL_PATH);
                    vg->SetPaint(m_StrokePaint, VG_STROKE_PATH);
                    vg->Setf(VG_STROKE_LINE_WIDTH, 3.0f);
                    vg->DrawPath(m_Path, VG_FILL_PATH | VG_STROKE_PATH);
                }
                //目前是<1000
                LOGE("---dt=%d", (int)(tmGetCurms() - sttm));
                nFrmNum++;
                if (nFrmNum > 1000) {
                    funcExit();
                }
            }

            virtual void invalidGLRes() {
            }
            virtual void freeGLRes() {
            }
        };

        GLEnvThread gl;
        TestSVGRender render([&gl]() { gl.m_pWindows->close(); });
        gl.init(&render, 800, 600);
        winWindows::handleMessage(NULL);
    }
    void testMain(){
        _1_testSVGRender_fillLinearG();
        _1_testSVGRender_fillcolor();
    }

    ADDTESTCASE(testMain,"renderSVGTest");
}
#endif
