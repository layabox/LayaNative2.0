/**
@file			JSRuntime.h
@brief			
@author			James
@version		1.0
@date			2016_5_13
*/

#ifndef __JSRuntime_H__
#define __JSRuntime_H__

#include <stdio.h>
#include "../JSInterface/JSInterface.h"
#include "../../JCScriptRuntime.h"

/** 
 * @brief 这个获得的是桌面分辨率
*/
namespace laya 
{
    class JSRuntime : public JsObjBase, public JSObjNode
    {
    public:
        enum 
        {
            onframeid, ondrawid, onresizeid, ontouchevtid,ondevicemotionevtid, onkeyevtid, onmouseevtid, oninvalidglid,onotherevtid,onnetworkevt,onbackpressed,onblurid,onfocusid, bulletsetid, bulletgetid
        };

	    static JsObjClassInfo JSCLSINFO;
	    
	    void exportJS();
	    
        JSRuntime();

	    ~JSRuntime();

    public:

        void setOnFrameFunction( JSValueAsParam p_pFunction );
        void setOnDrawFunction(JSValueAsParam p_pFunction);

        void setOnResizeFunction(JSValueAsParam p_onresize);

        void setOnBlurFunction(JSValueAsParam p_pFunction);
        
        void setOnFocusFunction(JSValueAsParam p_pFunction);
        
        void setHref(JSValueAsParam p_sHref);

        void setMouseEvtFunction(JSValueAsParam p_pFunction);

        void setTouchEvtFunction(JSValueAsParam p_pFunction);

        void setDeviceMotionEvtFunction(JSValueAsParam p_pFunction);

        void captureScreen(JSValueAsParam p_pFunction);

        void setKeyEvtFunction(JSValueAsParam p_pFunction);

        void setNetworkEvtFunction(JSValueAsParam p_pFunction);

		void setOnBackPressedFunction(JSValueAsParam p_pFunction);

        void setScreenWakeLock(bool p_bWakeLock);

        void setSensorAble(bool p_bSensorAble);

        void setBuffer(JSValueAsParam pArrayBuffer);

        bool saveAsPng(JSValueAsParam pArrayBufferArgs, int w, int h, const char* p_pszFile);

        bool saveAsJpeg(JSValueAsParam pArrayBufferArgs, int w, int h, const char* p_pszFile);
        
        JsValue convertBitmapToPng(JSValueAsParam pArrayBufferArgs, int w, int h);
        
        JsValue convertBitmapToJpeg(JSValueAsParam pArrayBufferArgs, int w, int h);

        void setGetWorldTransformFunction(JSValueAsParam p_pFunction);

        void setSetWorldTransformFunction(JSValueAsParam p_pFunction);

    public:
		static JSRuntime* getInstance();

        JsValue readFileFromAsset(const char* file, const char* encode);

        JsValue strTobufer(const char* s);

        const char* callMethod(int objid,bool isSyn,const char*clsName, const char* methodName, const char* paramStr);

        const char* getCachePath();

        const char* getPresetUrl();

        void printCorpseImages();

        void setExternalLink(const char* sUrl);

        void setExternalLinkEx( const char* sUrl,int x,int y,int w,int h,bool bCloseWebview );

        void closeExternalLink();

        void callWebviewJS( const char* sFunctionName,const char* sJsonParam,const char* sCallbackFunction );

        void hideWebview();

        void showWebView();
		
	    void exit();

        //bool updateArrayBufferRef(int nID, bool bSyncToRender, JSValueAsParam pArrayBuffer);

    public:

        JCScriptRuntime*        m_pScrpitRuntime;

        std::string m_strReturn;

	private:

		static JSRuntime* ms_pRuntime;
			
    };
}
//------------------------------------------------------------------------------


#endif //__JSRuntime_H__

//-----------------------------END FILE--------------------------------
