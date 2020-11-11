/**
@file			JCPerfHUD.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_18
*/

#include "JCPerfHUD.h"
#include <util/Log.h>
#include <util/JCCommonMethod.h>
#include "../LayaGL/JCLayaGL.h"
#if __APPLE__
#include <OpenGLES/ES3/gl.h>
#else
#include <GLES3/gl3.h>
#endif
namespace laya
{
	//----------------------------------------------------------------------------
	int JCPerfHUD::m_nMaxData=100;
	double JCPerfHUD::m_tmCurRender =0;
    double JCPerfHUD::m_tmDelayTime = 0;
	PerfDataBase* JCPerfHUD::m_vDatas[MAXPERFDATA];
	std::vector<short> JCPerfHUD::m_vValidID;
	double JCPerfHUD::m_tmCurJs;
    double JCPerfHUD::m_tmVSYNC = 0.0;
    unsigned int JCPerfHUD::m_nCurVsyncFrm = 0;
    unsigned int JCPerfHUD::m_nCurDrawFrm = 0;
    float JCPerfHUD::m_fGlobalScale = 1.0;
    JCLayaGL* JCPerfHUD::m_pLayaGL = NULL;
    void PerfData::updateData( float dt) 
    {
        m_DataLock.lock();
        m_vDatas.push_back(dt);
        if (m_vDatas.size()>m_nMaxData) {
            m_vDatas.pop_front();
        }
        m_DataLock.unlock();
    }

    //假设是均匀时间
    void PerfData::drawData(JCPerfDataRender* pRender) {
        m_DataLock.lock();
        JCPerfDataRender::vertex cdata[100];
        JCPerfDataRender::vertex* pCurVert = (JCPerfDataRender::vertex*)cdata;

        int nPanelW = pRender->m_nWidth;
        auto it = m_vDatas.begin();
        int nXOff = m_nMaxData - m_vDatas.size();  //必然>=0
        float dx = ((float)nPanelW)/m_nMaxData;
        float cx = nXOff*dx;
        int num = 0;
        for (; it != m_vDatas.end(); it++) {
            cx += dx;
            pCurVert->x = cx;
            pCurVert->y = (*it)*m_fScale;
            pCurVert++;
            num++;
            if (num > 100) {//超出上面的buffer的最大值了，先画一下。
                pRender->draw2DLines((float*)cdata, num, m_nColor);
                num = 0;
                pCurVert = (JCPerfDataRender::vertex*)cdata;
            }
        }
        if(num>0)
            pRender->draw2DLines((float*)cdata, num, m_nColor);
        m_DataLock.unlock();
    }

    void  perfBarData::addData(double tm, float st, float ed) {
        m_DataLock.lock();
        m_vDatas.push_back({ tm,st,ed });
        if (m_vDatas.size() > m_nMaxData) {
            m_vDatas.pop_front();
        }
        m_DataLock.unlock();
    }

    void perfBarData::drawData(JCPerfDataRender* pRender) {
        static dataType datas[100];
        m_DataLock.lock();
        int datanum = m_vDatas.size();
        auto it = m_vDatas.begin();
        int di = 0;
        for (int i = 0; i < datanum; i++) {
            datas[di++] = *it++;
            if (di > 100) {
                pRender->drawAsBarGraph((float*)datas, di, m_fScale, m_nColor);
                di = 0;
            }
        }
        if (di > 0) {
            pRender->drawAsBarGraph((float*)datas, di, m_fScale, m_nColor);
        }
        m_DataLock.unlock();
    }

	ScopePerf::ScopePerf(int id)
    {
		m_t0 = tmGetCurms();
		m_nid=id;
	}
	ScopePerf::~ScopePerf()
    {
		float dt = (float)(tmGetCurms()-m_t0);
		JCPerfHUD::updateData(m_nid, dt);
	}
	void JCPerfHUD::init()
	{
		m_nMaxData=100;
		memset(m_vDatas, 0, sizeof(m_vDatas));
		addData(PHUD_FRAME_DELAY,0xffffffff,"PHUD_FRAME_DELAY", 1);
		addData(PHUD_RENDER_DELAY,0xffff0000,"PHUD_RENDER_DELAY", 1);
		addData(PHUD_JS_DELAY,0xff00ff00,"PHUD_JS_DELAY", 1);
        /*
        addData(new perfBarData(PHUD_BAR_JS_ONDRAW, 0x6600ff00, "jsbar",10.0f));
        addData(new perfBarData(PHUD_BAR_RENDER, 0x66ff0000, "renderbar",10.0f));
        addData(new perfBarData(PHUD_BAR_JSWAIT, 0x66003300, "jswait", 10.0f));
        addData(new perfBarData(PHUD_BAR_GLWAIT, 0x66660000, "glwait", 10.0f));
        */
	}
	void JCPerfHUD::deleteInstance()
    {
		int nValidSize = JCPerfHUD::m_vValidID.size();
		for( int i=0; i < nValidSize ; i++ )
		{
			PerfData* pData =(PerfData*)JCPerfHUD::m_vDatas[JCPerfHUD::m_vValidID[i]];
			if( pData == NULL )continue;
			delete pData;
		}
		m_vValidID.clear();
	}

    void JCPerfHUD::addVSyncFrame() {
        m_nCurVsyncFrm++;
    }
    void JCPerfHUD::addOnDrawFrame() {
        m_nCurDrawFrm++;
    }

    void JCPerfHUD::resetFrame() {
        m_nCurVsyncFrm = 0;
        m_nCurDrawFrm = 0;
    }


	PerfData* JCPerfHUD::addData( int p_nId, int p_nColor,float scale ,float p_fAlert)
    {
		return addData(p_nId, p_nColor, "", scale, p_fAlert);
	}
	PerfData* JCPerfHUD::addData( int p_nId, int p_nColor,const char* p_sDesc,float scale, float p_fAlert )
	{
		if( p_nId < 0 || p_nId >= MAXPERFDATA )
        {
			LOGE("CPerfHUD::AddData超出最大数量 max=%d,cur=%d", MAXPERFDATA, p_nId );
			return NULL;
		}
		PerfData* pData = (PerfData*)m_vDatas[p_nId];
		if( pData == NULL )
        {
			pData = new PerfData( p_nId,p_nColor,p_sDesc );
			m_vDatas[p_nId]=pData;
			m_vValidID.push_back( p_nId );
		}
		pData->m_fScale = scale*m_fGlobalScale;
		pData->m_nMaxData=m_nMaxData;
		pData->m_nColor=p_nColor;
		pData->m_fAlert = p_fAlert;
		return pData;
	}

    PerfDataBase* JCPerfHUD::addData(PerfDataBase* pData) {
        if (!pData)
            return nullptr;
        int id = pData->m_nID;
        pData->m_nMaxData = m_nMaxData;
        if (id < 0 || id >= MAXPERFDATA){
            LOGE("CPerfHUD::AddData超出最大数量 max=%d,cur=%d", MAXPERFDATA, id);
            return nullptr;
        }
        PerfDataBase* pCurData = m_vDatas[id];
        if (pCurData == NULL){
            m_vDatas[id] = pData;
            m_vValidID.push_back(id);
        }
        else {
            LOGE("已经存在数据了 %d , %s",((int)pCurData->m_nID), (pCurData->m_strDesc.c_str()));
            return pCurData;
        }
        return pData;
    }

	void JCPerfHUD::delData(int p_nId)
    {
		if( p_nId < 0 || p_nId >= MAXPERFDATA )
        {
			return ;
		}
		PerfDataBase* pData = m_vDatas[p_nId];
		if( pData != NULL )
        {
			delete m_vDatas[p_nId];
			m_vDatas[p_nId]=NULL;
		}
		std::vector<short>::iterator it = m_vValidID.begin();
		while( it!=m_vValidID.end() )
        {
			if(*it==p_nId)
            {
				it=m_vValidID.erase(it);
			}
            else
            {
                it++;
            }
        }
	}
	//----------------------------------------------------------------------------
    PerfDataBase* JCPerfHUD::getData( int id )
	{
		if( id<0 || id>=MAXPERFDATA){
			LOGE("JCPerfHUD::getData超出最大数量 max=%d,cur=%d", MAXPERFDATA, id );
			return NULL;
		}
		return m_vDatas[id];
	}
	//----------------------------------------------------------------------------
	void JCPerfHUD::updateData( int p_nDataID, float p_fData )
	{
		if( p_nDataID<0 || p_nDataID>=MAXPERFDATA)
		{
			LOGE("JCPerfHUD::updateData超出最大数量 max=%d,cur=%d", MAXPERFDATA, p_nDataID );
			return ;
		}
		if(m_vDatas[p_nDataID])
		{
            ((PerfData*)m_vDatas[p_nDataID])->updateData(p_fData);
		}
	}
	//----------------------------------------------------------------------------
	JCPerfDataRender::JCPerfDataRender()
    {
        m_nOffY = 100;
        m_nOffX = 100;
        m_nWidth = 400;
        m_nHeight = 400;
        m_nProgram = 0;
        m_nVSShader = 0;
        m_nPSShader = 0;
        m_sVSShader = R"(
attribute vec3 g_Position;
void main(){
    gl_Position = vec4(g_Position.x, g_Position.y, g_Position.z, 1.0);
})";
        m_sPSShader = R"(
precision mediump float;
uniform vec4 color;
void main(){
    gl_FragColor = color;
})";

	}
    JCPerfDataRender::~JCPerfDataRender()
    {
        m_kVBOManager.ReleaseVBO();
        releaseGPUShader();
	}
    void JCPerfDataRender::validateGPUShader()
    {
        if (m_nProgram <= 0)
        {
            m_nVSShader = glCreateShader(GL_VERTEX_SHADER);
            const GLchar* sVSSource = m_sVSShader.c_str();
            glShaderSource(m_nVSShader, 1, &sVSSource, NULL);
            glCompileShader(m_nVSShader);
            m_nPSShader = glCreateShader(GL_FRAGMENT_SHADER);
            const GLchar* sPSSource = m_sPSShader.c_str();
            glShaderSource(m_nPSShader, 1, &sPSSource, NULL);
            glCompileShader(m_nPSShader);
            m_nProgram = glCreateProgram();
            glAttachShader(m_nProgram, m_nVSShader);
            glAttachShader(m_nProgram, m_nPSShader);
            glLinkProgram(m_nProgram);
        }
    }
    void JCPerfDataRender::releaseGPUShader()
    {
        if (m_nProgram)glDeleteProgram(m_nProgram);
        if (m_nProgram)glDeleteShader(m_nVSShader);
        if (m_nProgram)glDeleteShader(m_nPSShader);
        m_nProgram = 0;
        m_nVSShader = 0;
        m_nPSShader = 0;
    }
	void JCPerfDataRender::drawData()
	{
        m_tmDrawTm = tmGetCurms();

        GLint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
        GLint last_array_buffer; glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
        GLint last_element_array_buffer; glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
        GLint last_vertex_array; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
        //last_polygon_mode[2]; glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
        GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
        GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
        GLenum last_blend_src_rgb; glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);
        GLenum last_blend_dst_rgb; glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);
        GLenum last_blend_src_alpha; glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha);
        GLenum last_blend_dst_alpha; glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);
        GLenum last_blend_equation_rgb; glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_equation_rgb);
        GLenum last_blend_equation_alpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_equation_alpha);
        GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
        GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
        GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
        GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);
        GLboolean last_color_write_mask[4];
        glGetBooleanv(GL_COLOR_WRITEMASK, last_color_write_mask);

        GLint vb0enabled = 0;
        GLint vb0size = 0;
        GLint vb0type = 0;
        GLint vb0normalized = 0;
        GLint vb0stride = 0;
        GLint vb0bufferbinding = 0;
        void *vb0pointer = 0;
        glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &vb0enabled);
        glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_SIZE, &vb0size);
        glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_TYPE, &vb0type);
        glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &vb0normalized);
        glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &vb0stride);
        glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &vb0bufferbinding);
        glGetVertexAttribPointerv(0, GL_VERTEX_ATTRIB_ARRAY_POINTER, &vb0pointer);

        glColorMask(true, true, true, true);
        JCLayaGL::_bindVertexArray(0);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_SCISSOR_TEST);
        glViewport(0, 0, JCPerfHUD::m_pLayaGL->m_nMainCanvasWidth, JCPerfHUD::m_pLayaGL->m_nMainCanvasHeight);

        validateGPUShader();
        glUseProgram(m_nProgram);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        draw2DRect(0,0,(float)m_nWidth,(float)m_nHeight,0x66000000 );
		//刻度
		float maxx= (float)m_nWidth;
		float k[4];

        k[0] = 0;
        k[1] = 0;
		k[2]=maxx;
        k[3] = 0;
		draw2DLines( k,2,0xff336633);
        k[0] = 0;
        k[1] = 10.0f*JCPerfHUD::m_fGlobalScale;
		k[2]=maxx;
        k[3] = 10.0f*JCPerfHUD::m_fGlobalScale;
		draw2DLines(k,2, 0x55ffff00);
    
        k[0] = 0;
        k[1] = 16.0f*JCPerfHUD::m_fGlobalScale;
		k[2]=maxx;
        k[3] = 16.0f*JCPerfHUD::m_fGlobalScale;
		draw2DLines(k,2,0x55ff0000);

        k[0] = 0;
        k[1] = 33.0f*JCPerfHUD::m_fGlobalScale;
        k[2] = maxx;
        k[3] = 33.0f*JCPerfHUD::m_fGlobalScale;
        draw2DLines(k, 2, 0x55ffff00);

        k[0] = 0;
        k[1] = 50.0f*JCPerfHUD::m_fGlobalScale;
		k[2]=maxx;
        k[3] = 50.0f*JCPerfHUD::m_fGlobalScale;
		draw2DLines(k,2, 0x550000ff);

        k[0] = 0;
        k[1] = 100.0f*JCPerfHUD::m_fGlobalScale;
		k[2]=maxx;
        k[3] = 100.0f*JCPerfHUD::m_fGlobalScale;
	    draw2DLines(k,2, 0x550000ff);

        k[0] = 0;
        k[1] = 167.0f*JCPerfHUD::m_fGlobalScale;
        k[2] = maxx;
        k[3] = 167.0f*JCPerfHUD::m_fGlobalScale;
        draw2DLines(k, 2, 0x550000ff);

		//数据
		int nValidSize = JCPerfHUD::m_vValidID.size();
		for( int i=0; i < nValidSize ; i++ ){
			PerfDataBase* pData =JCPerfHUD::m_vDatas[JCPerfHUD::m_vValidID[i]];
			if( pData == NULL )continue;
            pData->drawData(this);
		}
        // Restore
        glUseProgram(last_program);
        JCLayaGL::_bindVertexArray(last_vertex_array);

        if (vb0enabled)
        {
            glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
            glVertexAttribPointer(0, vb0size, vb0type, vb0normalized, vb0stride, vb0pointer);
        }
        else
        {
            glDisableVertexAttribArray(0);
        }
        glColorMask(last_color_write_mask[0], last_color_write_mask[1], last_color_write_mask[2], last_color_write_mask[3]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
        glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
        glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);
        if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
        if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
        if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
        if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
        glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
        //glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);

	}
    void JCPerfDataRender::invalidGLRes()
    {
        m_kVBOManager.freeGLResource();
        releaseGPUShader();
    }
    
    /*
        数据的x轴单位是帧
    */
    void JCPerfDataRender::draw2DLines( float* p_pVerts, int vertnum, unsigned int p_nColor)
    {
        if (vertnum < 2)
            return;
        int nVBO = m_kVBOManager.GetVBO();
        if (nVBO <= 0)
        {
            LOGE("JCPerfDataRender::draw2DLines error, createvbo error!");
            return;
        }
        static vertex tmpvertex[1000];
        float color[4];
        color[3] = ((p_nColor & 0xff000000) >> 24) / 255.0f;;
        color[0] = ((p_nColor & 0x00ff0000) >> 16) / 255.0f; //r
        color[1] = ((p_nColor & 0x0000ff00) >> 8) / 255.0f; //g
        color[2] = (p_nColor & 0x000000ff) / 255.0f; //b
        int nVertNum = vertnum > 1000 ? 1000 : vertnum;
        for (int i = 0; i < nVertNum; i++)
        {
            float cx = *p_pVerts++;
            float cy = *p_pVerts++;
            cx = m_nOffX+cx;
            cy = m_nOffY + m_nHeight - cy;
            cx = (float)((cx*2.0 / JCPerfHUD::m_pLayaGL->m_nMainCanvasWidth) - 1.0);
            cy = (float)(-(cy*2.0 / JCPerfHUD::m_pLayaGL->m_nMainCanvasHeight) + 1.0);
            tmpvertex[i].x = cx;
            tmpvertex[i].y = cy;
        }
		glBindBuffer(GL_ARRAY_BUFFER, nVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, nVertNum*sizeof(vertex), tmpvertex);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glUniform4fv(0, 1, color);
        glDrawArrays(GL_LINE_STRIP, 0, vertnum);
    }
    //------------------------------------------------------------------------------
    void JCPerfDataRender::draw2DRect(float x, float y, float w, float h, unsigned int p_nColor)
    {
        int nVBO = m_kVBOManager.GetVBO();
        if (nVBO <= 0)
        {
            LOGE("JCPerfDataRender::draw2DRect error, createvbo error!");
            return;
        }
        float color[4];
        color[3] = ((p_nColor & 0xff000000)>>24)/ 255.0f;
        color[0] = ((p_nColor & 0x00ff0000) >> 16) / 255.0f; //r
        color[1] = ((p_nColor & 0x0000ff00) >> 8) / 255.0f; //g
        color[2] = (p_nColor & 0x000000ff) / 255.0f; //b

        static vertex tmpvertex[4];
        int nVertNum = 4;

        //下面的是错误的。可能会导致严重的丢失精度
        tmpvertex[0].x = x;
        tmpvertex[0].y = y;

        tmpvertex[1].x = (x + w);
        tmpvertex[1].y = y;

        tmpvertex[2].x = (x + w);
        tmpvertex[2].y = (y + h);

        tmpvertex[3].x = x;
        tmpvertex[3].y = (y + h);

        for (int i = 0; i < 4; i++)
        {
            float cx = tmpvertex[i].x+m_nOffX;
            float cy = tmpvertex[i].y+m_nOffY;

            cx = (float)((cx*2.0f / JCPerfHUD::m_pLayaGL->m_nMainCanvasWidth) - 1.0f);
            cy = (float)(-(cy*2.0f / JCPerfHUD::m_pLayaGL->m_nMainCanvasHeight) + 1.0f);

            tmpvertex[i].x = cx;
            tmpvertex[i].y = cy;
        }

        glBindBuffer(GL_ARRAY_BUFFER, nVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, nVertNum*sizeof(vertex), tmpvertex);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glUniform4fv(0, 1, color);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    /*
       数据的x轴单位是时间，ms。
    */
    void JCPerfDataRender::drawAsBarGraph(float* pData, int p_nDataNum, float scale, unsigned int p_nColor) {
        if (p_nDataNum <= 1)
            return;
        int nVBO = m_kVBOManager.GetVBO();
        if (nVBO <= 0){
            LOGE("JCPerfDataRender::draw2DLines error, createvbo error!");
            return;
        }
        static vertex tmpvertex[1000];
        vertex* pCurVert = tmpvertex;
        float color[4];
        color[3] = ((p_nColor & 0xff000000) >> 24) / 255.0f;
        color[0] = ((p_nColor & 0x00ff0000) >> 16) / 255.0f; //r
        color[1] = ((p_nColor & 0x0000ff00) >> 8) / 255.0f; //g
        color[2] = (p_nColor & 0x000000ff) / 255.0f; //b
        perfBarData::dataType* pBarData = (perfBarData::dataType*)pData;
        //从当前时间倒退displayDuration ms，整个图形的，左边是m_tmDrawTm - displayDuration， 右边是 m_tmDrawTm 
        static double displayDuration = 4000.0;
        double sttm = m_tmDrawTm - displayDuration;// pBarData[0].tm;
        //float edtm = pBarData[p_nDataNum - 1].tm;
        float dataduration = (float)displayDuration;//固定4秒钟。 edtm - sttm + 16.667f;    //左右各加半个
        float barwidth = m_nWidth/(dataduration/16.6667f);
        float foffx = (float)m_nOffX;
        float foffy = (float)m_nOffY;
        int nVertNum = 0;
        for (int i = 0; i < p_nDataNum; i++, pBarData++) {
            if (pBarData->tm < sttm)
                continue;
            float cx = foffx + ((float)(pBarData->tm - sttm))*m_nWidth/ dataduration;
            float ltx = cx;
            float lty = foffy + m_nHeight - (pBarData->start + pBarData->duration)*scale;
            float rbx = cx + barwidth;
            float rby = foffy + m_nHeight - pBarData->start*scale;
            if (rby - lty < 1.0f)//大小不要为0，否则看不到了
                rby = lty + 1.0f;

            ltx = ((ltx*2.0f / JCPerfHUD::m_pLayaGL->m_nMainCanvasWidth) - 1.0f);
            lty = (-(lty*2.0f / JCPerfHUD::m_pLayaGL->m_nMainCanvasHeight) + 1.0f);
            rbx = ((rbx*2.0f / JCPerfHUD::m_pLayaGL->m_nMainCanvasWidth) - 1.0f);
            rby = (-(rby*2.0f / JCPerfHUD::m_pLayaGL->m_nMainCanvasHeight) + 1.0f);

            pCurVert->x = ltx; pCurVert->y = lty; pCurVert++;
            pCurVert->x = rbx; pCurVert->y = lty; pCurVert++;
            pCurVert->x = rbx; pCurVert->y = rby; pCurVert++;
            pCurVert->x = ltx; pCurVert->y = lty; pCurVert++;
            pCurVert->x = rbx; pCurVert->y = rby; pCurVert++;
            pCurVert->x = ltx; pCurVert->y = rby; pCurVert++;
            nVertNum += 6;
            if (nVertNum >= 1000 - 6) {
                glBindBuffer(GL_ARRAY_BUFFER, nVBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, nVertNum*sizeof(vertex), tmpvertex);
                glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
                glUniform4fv(0, 1, color);
                glDrawArrays(GL_TRIANGLES, 0, nVertNum);
                nVertNum = 0;
                pCurVert = tmpvertex;
            }
            //再把下一段画到上面
            if (true) {
                perfBarData::dataType* pBarDataN =(i<p_nDataNum-1)?(pBarData + 1):pBarData;
                float cx = foffx + ((float)(pBarData->tm - sttm))*m_nWidth / dataduration;//这个时间要用当前的
                float ltx = cx;
                float lty = foffy + m_nHeight - (pBarDataN->start + pBarDataN->duration+16.6667f)*scale;
                float rbx = cx + barwidth;
                float rby = foffy + m_nHeight - (pBarDataN->start+(float)(pBarDataN->tm-pBarData->tm))*scale;
                if (rby - lty < 1.0f)//大小不要为0，否则看不到了
                    rby = lty + 1.0f;

                ltx = ((ltx*2.0f / JCPerfHUD::m_pLayaGL->m_nMainCanvasWidth) - 1.0f);
                lty = (-(lty*2.0f / JCPerfHUD::m_pLayaGL->m_nMainCanvasHeight) + 1.0f);
                rbx = ((rbx*2.0f / JCPerfHUD::m_pLayaGL->m_nMainCanvasWidth) - 1.0f);
                rby = (-(rby*2.0f / JCPerfHUD::m_pLayaGL->m_nMainCanvasHeight) + 1.0f);

                pCurVert->x = ltx; pCurVert->y = lty; pCurVert++;
                pCurVert->x = rbx; pCurVert->y = lty; pCurVert++;
                pCurVert->x = rbx; pCurVert->y = rby; pCurVert++;
                pCurVert->x = ltx; pCurVert->y = lty; pCurVert++;
                pCurVert->x = rbx; pCurVert->y = rby; pCurVert++;
                pCurVert->x = ltx; pCurVert->y = rby; pCurVert++;
                nVertNum += 6;
                if (nVertNum >= 1000 - 6) {
                    glBindBuffer(GL_ARRAY_BUFFER, nVBO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, nVertNum*sizeof(vertex), tmpvertex);
                    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
                    glUniform4fv(0, 1, color);
                    glDrawArrays(GL_TRIANGLES, 0, nVertNum);
                    nVertNum = 0;
                    pCurVert = tmpvertex;
                }
            }
        }
        if (nVertNum > 0) {
            glBindBuffer(GL_ARRAY_BUFFER, nVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, nVertNum*sizeof(vertex), tmpvertex);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glUniform4fv(0, 1, color);
            glDrawArrays(GL_TRIANGLES, 0, nVertNum);
        }
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
