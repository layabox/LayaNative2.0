/**
@file			JSTextBitmapInfo.h
@brief			
@author			James
@version		1.0
@date			2018_3_31
*/

#ifndef __JSTextBitmapInfo_H__
#define __JSTextBitmapInfo_H__


//包含头文件
#include <stdio.h>
#include <string>
#include <map>
#include "../JSInterface/JSInterface.h"
#include "../JSInterface/JSObjBase.h"
#include <fontMgr/JCFontInfo.h>

namespace laya
{
    class JSTextBitmapInfo :public JsObjBase, public JSObjNode
    {
    public:
	    static JsObjClassInfo JSCLSINFO;
	    static void exportJS();

        JSTextBitmapInfo();

	    ~JSTextBitmapInfo();

	    int getWidth();

	    int getHeight();

        JsValue getBitmapData();

        void setInfo(int w, int h, char* data);

    public:

	    int					m_nWidth;
	    int					m_nHeight;
        char*               m_pBitmapData;

    };
}
//------------------------------------------------------------------------------


#endif //__JSTextBitmapInfo_H__

//-----------------------------END FILE--------------------------------