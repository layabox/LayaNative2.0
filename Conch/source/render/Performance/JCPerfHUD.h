/**
@file			JCPerfHUD.h
@brief			
@author			James
@version		1.0
@date			2016_5_18
*/
#ifndef __JCPerfHUD_H__
#define __JCPerfHUD_H__

#include <list>
#include <string>
#include <vector>
#include <stdint.h>
#include "JCVBOManager.h"
#include <mutex>
#include <util/Log.h>
#define USEPERF

namespace laya
{
    class JCLayaGL;
    class JCPerfDataRender;
    struct  PerfDataBase{
        short 					m_nID=0;
        int 					m_nColor=0xff000000;
        std::string 			m_strDesc;
        unsigned short			m_nMaxData=100;
        float					m_fScale=1.0f;			//渲染时候用
        virtual void drawData(JCPerfDataRender*) {};
    };
    /*
        记录一些变量的一定个数的历史记录。并提供画出来的功能。
        需要效率尽量高，因此，没有足够的封装和保护
        用数组而没用map
        每次updatedata都是累加数据，渲染完了之后，就会清0
     */
    #define MAXPERFDATA 256
    struct PerfData:public PerfDataBase{
	    PerfData(int id,int color, const char* pdesc){
		    m_nID=id;
		    m_strDesc=pdesc;
            m_nColor = color;
	    }
        //添加或者累加数据 如果是新的一帧就添加，如果本帧已经有数据了，就累加
        void updateData( float dt);

        virtual void drawData(JCPerfDataRender* pRender);
    public:
	    std::list<float>		m_vDatas;
	    float					m_fAlert=0.0f;
        std::mutex              m_DataLock;
    };

    class perfBarData :public PerfDataBase {
    public:
        struct dataType{
            double tm;
            float start, duration;
            dataType() {
                tm = start = duration = 0.0f;
            }
            dataType(double t, float st, float ed) {
                tm = t;
                start = st;
                duration = ed - st;
            }
        };

        perfBarData(int id, int color, const char* pdesc,float scale) {
            m_nID = id;
            m_strDesc = pdesc;
            m_nColor = color;
            m_fScale = scale;
        }

        void addData(double tm, float st, float ed);
        virtual void drawData(JCPerfDataRender* pRender);
    private:
        std::list<dataType>		m_vDatas;
        std::mutex              m_DataLock;
    };

    /** @brief RenderGroup组的定义
     *
    */
    class ScopePerf
    {
    public:
	    double m_t0;
	    int m_nid;
	    ScopePerf(int id);
	    ~ScopePerf();
    };

#ifdef USEPERF
#define PERF_INITVAR(var) auto var = tmGetCurms();
#define	PERF_UPDATE_DTIME(dataid,var,dt)	    \
		    { auto curtm = tmGetCurms();        \
		      if( var==0)var = curtm;           \
		      dt = (curtm-var);                 \
		      JCPerfHUD::updateData(dataid,dt); \
		      var = curtm;}
    #define PERF_UPDATE_DATA(dataid,val) {JCPerfHUD::updateData(dataid,val);}
    #define PERF_SCOPE(id) ScopePerf scopeperf(id);
#else
    #define PERF_INITVAR(var)
    #define	PERF_UPDATE_DTIME(dataid,var)
    #define PERF_UPDATE_DATA(dataid,val) 
    #define PERF_SCOPE(id) 
#endif

class JCPerfHUD
{
public:
	enum PERFHUD_TYPE
    {
		PHUD_FRAME_DELAY=0,
		PHUD_RENDER_DELAY,
        PHUD_JS_DELAY,
        PHUD_BAR_JS_ONDRAW,
        PHUD_BAR_RENDER,
        PHUD_BAR_JSWAIT,
        PHUD_BAR_GLWAIT,
        //TODO 现在只是内部使用。导出给js以后，要注意id的冲突的问题。
	};
public:

	static void init();

	static void deleteInstance();

	static PerfData* addData(int id, int color,float scale, float p_fAlert=0);

	static PerfData* addData(int id, int color,const char* pdesc, float scale, float p_fAlert=0);

    static PerfDataBase* addData(PerfDataBase* pPerfData);

	static void delData(int id);

	static PerfDataBase* getData(int id);
    
    static void updateData(int p_nDataID, float p_fData);

    static void addOnDrawFrame();
    static void addVSyncFrame();
    static void resetFrame();
public:    

    static PerfDataBase*			m_vDatas[MAXPERFDATA];
    static std::vector<short>		m_vValidID;         //有效ID，避免遍历m_Datas
	static int 						m_nMaxData;
	static double					m_tmCurRender;
    static double                   m_tmDelayTime;
	static double					m_tmCurJs;
    static double                   m_tmVSYNC;
    static unsigned int             m_nCurVsyncFrm;      //vsync帧数
    static unsigned int             m_nCurDrawFrm;      //实际触发onDraw的帧数
    static float                    m_fGlobalScale;
    static JCLayaGL*                m_pLayaGL;
};

class JCPerfDataRender
{
public:
    struct vertex {
        float x, y;
    };
    JCPerfDataRender();
	~JCPerfDataRender();
	void drawData();
    void draw2DLines(float* p_pVerts, int vertnum, unsigned int p_nColor);
    void draw2DRect(float x, float y, float w, float h, unsigned int p_nColor );
    /*
        data:
            start,height 都是绝对值。与面板的大小同一单位
    */
    void drawAsBarGraph(float* pData, int p_nDataNum, float scale, unsigned int p_nColor);
    void invalidGLRes();

private:

    void validateGPUShader();

    void releaseGPUShader();

private:

    JCVBOManager            m_kVBOManager;
    int 					m_nOffY;
    int 					m_nOffX;
    std::string             m_sVSShader;
    std::string             m_sPSShader;
    GLuint                  m_nVSShader;
    GLuint                  m_nPSShader;
    GLuint                  m_nProgram;
    double                  m_tmDrawTm;         //当前渲染的时候的时间。用来处理跟时间相关的数据
public:
    int 					m_nWidth;           //面板的大小
    int 					m_nHeight;          //面板的大小
};

}
#endif //__JCPerfHUD_H__

//-----------------------------END FILE--------------------------------