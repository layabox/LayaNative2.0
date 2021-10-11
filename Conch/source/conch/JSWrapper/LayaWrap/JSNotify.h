/**
@file			JSNotify.h
@brief			
@author			James
@version		1.0
@date			2015_9_26
*/

#ifndef __JSNotify_H__
#define __JSNotify_H__

//包含头文件
#include <stdio.h>
#include <string>
#include "../JSInterface/JSInterface.h"

/** 
 * @brief 
*/
namespace laya 
{
    class JSNotify :public JsObjBase, public JSObjNode
    {
    public:
	    static JsObjClassInfo JSCLSINFO;

	    static void exportJS();

        static JSNotify* getInstance();

	    JSNotify();

	    ~JSNotify();
    
    public:
    
        //设置重复的消息
        //type  0是年  1是月  2是日 3是时  4是分  5是秒
        void setRepeatNotify( int p_nID,int p_nStartTime,int p_nRepeatType,const char* p_sTickerText,const char* p_sTitleText,const char* p_sDesc );
    
        //设置只提示一次的消息
        void setOnceNotify( int p_nID,int p_nStartTime,const char* p_sTickerText,const char* p_sTitleText,const char* p_sDesc );
    
        //删除某一个消息和定时器
        void deleteOnceNotify( int p_nID );
    
        //只删除全部消息，但是保留定时器
        void deleteAllNotify();
    
    public:

        static JSNotify* ms_pNotify;
    
    };
}

#endif //__JSNotify_H__

//-----------------------------END FILE--------------------------------
