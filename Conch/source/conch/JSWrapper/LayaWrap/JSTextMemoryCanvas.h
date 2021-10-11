/**
@file			JSTextMemoryCanvas.h
@brief			
@author			James
@version		1.0
@date			2016_5_17
*/

#ifndef __JSTextMemoryCanvas_H__
#define __JSTextMemoryCanvas_H__

#include <vector>
#include "../JSInterface/JSInterface.h"
#include "../JSInterface/JSObjBase.h"
#include <fontMgr/JCFontInfo.h>
#include <fontMgr/JCFontManager.h>
#include <fontMgr/JCFreeTypeRender.h>

namespace laya
{
    class JSTextMemoryCanvas :public JsObjBase, public JSObjNode
	{
    public:

        static JSTextMemoryCanvas* getInstance();

		static JsObjClassInfo JSCLSINFO;

		void exportJS();	//全局的对象，需要生成一个实例，所以不用静态函数

        JSTextMemoryCanvas();

		~JSTextMemoryCanvas();

        void clearAllData();

	public:

        /** @brief 初始化freeType默认字体
        *  @param[in] 字体的路径，多个字体中间用 \4 分割
        *  @return 返回是否成功
        */
        bool initFreeTypeDefaultFontFromFile(const char* sDefaultTTFs);

        /** @brief 初始化freeType默认字体
        *  @param[in] arrayBuffer
        *  @return 返回是否成功
        */
        bool initFreeTypeDefaultFontFromBuffer(JSValueAsParam pArrayBufferArgs);


        /** @brief 设置字体从FileName中读取
        *  @param[in] 字体名字
        *  @param[in] ttf的路径
        *  @return 是否读取成功
        */
        bool setFontFaceFromUrl(const char* sFontFamily, const char* sTTFFileName);

        /** @brief 设置字体从buffer中读取
        *  @param[in] 字体的名字
        *  @param[in] buffer
        *  @param[in] bufferLength
        *  @return 是否读取成功
        */
        bool setFontFaceFromBuffer(const char* sFontFamily, JSValueAsParam pArrayBufferArgs);

        /** @brief 移除字体
        *  @param[in] 字体的名字
        *  @return
        */
        bool removeFont(const char* sFontFamily);

		void setFontInfo(const char* sFontInfo);

		const char* getFontInfo();

		//JsValue measureText(const char* sText);
		
        JsValue getTextBitmapData(const char* sText,int nColor, int nBorderSize, int nBorderColor);

        JsValue measureChar(int unicode);

        void scale(double x, double y);

    public:

        static JSTextMemoryCanvas*  ms_pTextMemoryCanvas;
        JCFontManager*              m_pFontManager;
        JCFreeTypeFontRender*       m_pFreeTypeRender;
        JCFontInfo*                 m_pCurrentFontInfo;
        std::string                 m_sFont;
        BitmapData                  m_kBitmapData;
#ifdef __APPLE__
        class JCIOSFreeType*              m_pIOSFreeType;                 ///<iosfreetype 
#endif
        double m_fScaleX;
        double m_fScaleY;
	};
}
//------------------------------------------------------------------------------


#endif //__JSTextMemoryCanvas_H__

//-----------------------------END FILE--------------------------------
