/**
@file			JCScrpitRuntimeWASM.h
@brief			
@author			James
@version		1.0
@date			2017_9_25
*/

#ifndef __JCScrpitRuntimeWASM_H__
#define __JCScrpitRuntimeWASM_H__


#include <vector>
#include <util/JCCommonMethod.h>
#include <buffer/JCBuffer.h>
#include <FontRender/JCFreeTypeRender.h>
#include <FontRender/JCMeasureTextManager.h>
#include <resource/text/JCFontManager.h>
#include "JCCmdDispatchManager.h"
#include <RenderCmd/CmdBase.h>
#include <Node/JCNode2D.h>
#include <Node/JCNode2DRenderer.h>

namespace laya
{
    class JCScriptRuntime 
    {
    public:
        
        JCScriptRuntime();

        ~JCScriptRuntime();

        void init(JCFreeTypeFontRender* pFreeTypeRender);
        
        void onThInit();
        
        bool onUpdate();
        
        //每次需要渲染的时候触发js
        void onUpdateDraw(double vsyncTime);

        void onThExit();

        void flushSharedCmdBuffer();

        void dispatchScriptCmd();

        void setJSCmdBuffer(char* pBuffer);

        void setRootNode(JCNode2D* pNode);

    private:

        void startScriptOnRenderThread();

    public:

        static JCScriptRuntime*     s_JSRT;
        JCHtml5Context*             m_pRootHtml5Context;
        JCNode2D*                   m_pRootNode;
        std::vector<CmdBase*>       m_vRenderCmds;
        JCCmdDispathManager*        m_pCmdDispathManager;           //
        JCMeasureTextManager*       m_pMeasureTextManager;          //MeasureTextManager
        JCMemClass*					m_pJSCmdBuffer;                 //JS的buffer
        char*                       m_pCmdBufferSharedWithJS;       //第一个int是当前指针。
        int                         m_nCmdBufferSharedWithJSLen;
        int							m_nUpdateCount;                 //update次数
        JCTextManager*              m_pTextManager;                 ///<TextManager
        JCFreeTypeFontRender*       m_pFreeTypeRender;              ///<FreeTypeRender 用于绘制文字
        JCFontManager*              m_pFontManager;                 ///<FontManager
        int                         m_nCountGroup;                  ///<组的个数
        int                         m_nCountVertex;                 ///<顶点的个数
        int                         m_nCountNode;                   ///<node的个数
  
    };
}
//------------------------------------------------------------------------------


#endif //__JCScrpitRuntimeWASM_H__

//-----------------------------END FILE--------------------------------