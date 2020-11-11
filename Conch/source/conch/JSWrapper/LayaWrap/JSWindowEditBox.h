/**
@file			JSWindowEditBox.h
@brief			
@author			James
@version		1.0
@date			2014_4_28
*/

#ifndef __JSWindowEditBox_H__
#define __JSWindowEditBox_H__

#include <stdio.h>
#include <string>
#include "../JSInterface/JSInterface.h"
#include "EditBox/WinEditBox.h"

/** 
 * @brief 
*/
namespace laya 
{
    class JSWindowEditBox:public JsObjBase, public JSObjNode
    {
    public:
	    static JsObjClassInfo JSCLSINFO;

	    static void exportJS();

	    JSWindowEditBox();

	    ~JSWindowEditBox();

    public:

	    int set_Left( int p_nLeft );

	    int get_Left();

	    int set_Top( int p_nTop );

	    int get_Top();

	    int set_Width( int p_nWidth );

	    int get_Width();

	    int set_Height( int p_nHeight );

	    int get_Height();

	    float set_Opacity( float p_Opacity );

	    float get_Opacity();

	    const char* set_Value( const char* p_sValue );

	    const char* get_Value();

	    const char* set_Style( const char* p_sStyle );

	    const char* get_Style();
	
	    bool set_Visible( bool p_bVisible );
	
	    bool get_Visible();

        void addEventListener(const char* p_sName,JSValueAsParam p_pFunction );
        void setSelectionRange(int start,int end);

        void onInputCallJSFunction(std::weak_ptr<int> callbackref);
        void onKeyDownCallJSFunction(int keyCode, std::weak_ptr<int> callbackref);

    public:
	
	    void setColor( const char* p_sColor );

		void setBgColor(const char* p_sBgColor);
	
	    void setPos( int x,int y );
	
	    void setSize( int w,int h );
	
	    void setCursorPosition( int pos );	
	
	    void setLeft( int p_nLeft );

	    void setTop( int p_nTop );

	    void setWidth( int p_nWidth );

	    void setHeight( int p_nHeight );

	    void setOpacity( float p_Opacity );

	    void setValue( const char* p_sValue );

	    const char* getValue();

	    void setStyle( const char* p_sStyle );
	
	    void setVisible( bool p_bVisible );
	
	    void focus();
	
	    void blur();

    public:

	    void setScale( float p_nSx,float p_nSy );

	    void setMaxLength( int p_nMaxLength );

	    void setType( const char* p_sType );

	    void setRegular( const char* p_sRegular );

	    void setFontSize( int p_nFontSize );

	    void setFont( const char* p_sFont );

	    int measureText( const char* p_sText );
	
	    void setNumberOnly( bool p_bNumberOnly );

        void setMultiAble(bool p_bMultiAble);

        void setForbidEdit(bool bForbidEdit);

        bool getForbidEdit();

    public:
	    void drawEditboxAll();
	    bool handleMouseEvent( int p_nMouseX,int p_nMouseY );
	    void mouseMove( int p_MouseX,int p_MouseY );

	    bool onKeyChar( short p_sBuffer );
	
	    void ResetCaretBlink();
	    void PlaceCaret( int nCP );
	    void drawText();
	    void onDeleteCharB();
	    void onDeleteCharD();
	    void drawSelected();
	    void DeleteSelectionText();
	    void CopyToClipboard();
	    void PasteFromClipboard();
	    void changeCandidate();
	    void compositionWM_IME();
	    char* getLpChar(int index);
	    void caretMoveRight();
	    void caretMoveToEnd();
    public:

	    int							m_nLeft;
	    int							m_nTop;
	    int							m_nWidth;
	    int							m_nHeight;
	    int							m_nMaxLength;
	    std::string					m_sType;
	    bool						m_bVisible;
	    float						m_fOpacity;
	    std::string					m_sStyle;
	    std::vector<short>			m_vValue;
	    std::string					m_sValue;
	    std::string					m_sRegular;
	    std::string					m_sBgColor;
	    std::string					m_sBoarderColor;
	    bool						m_bFocus;
	    std::string					m_sFontColor;
	    int							m_nFontSize;
	    std::string					m_sFont;

    public:
	    int							m_nCaretColor;				//光标的颜色
	    int							m_nLastTextWidth;			//上一次的文字宽度
	    bool						m_bCaretOn;					//光标是否显示
	    int							m_nCaret;					//光标的位置
	    int							m_nCaretPos;				//光标距离editbox左边的长度
	    int							m_nCaretStart;				//从第几个光标位置开始显示。假设editbox内容为str,这个表示editbox左边缘对应str的第几个字，从0开始。
	    int							m_nCaretEnd;				//显示到最后的光标位置
	    int							m_nCaretLast;				//鼠标移动时候记录上一个光标位置
	    int							m_nSelStart;				//鼠标拖动选择的位置
	    bool						m_bMouseDrag;
        bool                        m_bForbidEdit;

    private:
	    enum{oninputid,onkeydownid};
        std::shared_ptr<int>        m_CallbackRef;
        JsObjHandle                 m_pJSFunctionOnInput;       //JS的回调
        JsObjHandle                 m_pJSFunctionOnKeydown;     //JS的回调

		static WinEditBox*     ms_EditBoxInst;
    };
}

#endif //__JSWindowEditBox_H__

//-----------------------------END FILE--------------------------------