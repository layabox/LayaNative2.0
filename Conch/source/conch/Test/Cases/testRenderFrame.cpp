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

/*
    1.能在主线程创建一个gles环境，进行原始的渲染
    2.能在一个新开的线程中打开一个gles渲染
    3.能渲染3D场景
    4.能获得渲染结果
*/

using namespace laya;

namespace renderFrameTest{
    void _1_testRender_GLTread(){
        class TestRender:public JCIGLRender{
        public:
            float   fR = 0.0f;
            int     nFrmNum = 0;
            std::function<void()> funcExit;
            TestRender(std::function<void()> f) {
                funcExit = f;
            }
            virtual void onGLReady(){
            }

            virtual int renderFrame(long p_nCurrentFrame,bool){
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                fR += 0.1f;
                if (fR > 1.0f)fR = 0.0f;
                glClearColor(fR,1.0f,1.0f,1.0f);
                nFrmNum++;
                if (nFrmNum > 100) {
                    funcExit();
                }
                return 0;
            }
            virtual void freeGLRes() {
            }
        };

        GLEnvThread gl;
        TestRender render([&gl](){ gl.m_pWindows->close(); });
        gl.init(&render,800,600);
        winWindows::handleMessage(NULL);
    }
    void testMain(){
        _1_testRender_GLTread();
    }

    ADDTESTCASE(testMain,"renderFrameTest");
}
#endif
