/**
@file			JSIOSEditBox.cpp
@brief			
@author			wyw
@version		1.0
@date			2014_8_22
*/

//包含头文件
#include "JSIOSEditBox.h"
#include "../JSInterface/JSInterface.h"
#include <util/Log.h>
#include <util/JCColor.h>
#include "../../CToObjectC.h"
#include "../../JCScriptRuntime.h"
#include <util/ColorParser.h>

namespace laya
{
ADDJSCLSINFO(JSIOSEditBox, JSObjNode);
//------------------------------------------------------------------------------
JSIOSEditBox::JSIOSEditBox()
{
    //大概估算内部变量 11个int  4个字符串
    AdjustAmountOfExternalAllocatedMemory( 208 );
	JCMemorySurvey::GetInstance()->newClass( "iOSEditBox",208,this );
	m_nLeft = 0;
	m_nTop = 0;
	m_nWidth = 0;
	m_nHeight = 0;
	m_fOpacity = 1;
	m_sStyle = "";
	m_sValue = "";
	m_sType = "type";
	m_nFontSize = 12;
	m_nScaleX = 1;
	m_nScaleY = 1;
	m_bForbidEdit = false;
    m_CallbackRef.reset(new int(1));
}
//------------------------------------------------------------------------------
JSIOSEditBox::~JSIOSEditBox()
{
	JCMemorySurvey::GetInstance()->releaseClass( "iOSEditBox",this );
}
//------------------------------------------------------------------------------
int JSIOSEditBox::set_Left( int p_nLeft )
{
	m_nLeft = p_nLeft;
	CToObjectCSetEditBoxX( p_nLeft );
	return m_nLeft;
}
//------------------------------------------------------------------------------
int JSIOSEditBox::get_Left()
{
	return m_nLeft;
}
//------------------------------------------------------------------------------
int JSIOSEditBox::set_Top( int p_nTop )
{
	m_nTop = p_nTop;
	CToObjectCSetEditBoxY( p_nTop );
	return m_nTop;
}
//------------------------------------------------------------------------------
int JSIOSEditBox::get_Top()
{
	return m_nTop;
}
//------------------------------------------------------------------------------
int JSIOSEditBox::set_Width( int p_nWidth )
{
	m_nWidth = p_nWidth;
	CToObjectCSetEditBoxWidth( p_nWidth*m_nScaleX );
	return m_nWidth;
}
//------------------------------------------------------------------------------
int JSIOSEditBox::get_Width()
{
	return m_nWidth;
}
//------------------------------------------------------------------------------
int JSIOSEditBox::set_Height( int p_nHeight )
{
	m_nHeight = p_nHeight;
	CToObjectCSetEditBoxHeight( p_nHeight*m_nScaleY );
	return m_nHeight;
}
//------------------------------------------------------------------------------
int JSIOSEditBox::get_Height()
{
	return m_nHeight;
}
//------------------------------------------------------------------------------
float JSIOSEditBox::set_Opacity( float p_Opacity )
{
	m_fOpacity = p_Opacity;

	return m_fOpacity;
}
//------------------------------------------------------------------------------
float JSIOSEditBox::get_Opacity()
{
	return m_fOpacity;
}
//------------------------------------------------------------------------------
const char* JSIOSEditBox::set_Value( const char* p_sValue )
{
	m_sValue = ( p_sValue != NULL ) ? p_sValue : "";
	CToObjectCSetEditBoxValue( m_sValue.c_str() );
    m_nSetValueUpdateCount = JCScriptRuntime::s_JSRT->m_nUpdateCount;
	return m_sValue.c_str();
}
//------------------------------------------------------------------------------
const char* JSIOSEditBox::get_Value()
{
    int curUpdateCount = JCScriptRuntime::s_JSRT->m_nUpdateCount;
    if(m_nSetValueUpdateCount != 0 && m_nSetValueUpdateCount == curUpdateCount)
    {
        m_nSetValueUpdateCount = 0;
        return m_sValue.c_str();
    }
    m_nSetValueUpdateCount = 0;
	const char* sValue = CToObjectCGetEditBoxValue();
	m_sValue = sValue==NULL?"":sValue;
	return m_sValue.c_str();
}
//------------------------------------------------------------------------------
void JSIOSEditBox::set_Style( const char* p_sStyle )
{
	m_sStyle = p_sStyle;
	CToObjectCSetEditBoxStyle( p_sStyle );
}
//------------------------------------------------------------------------------
const char* JSIOSEditBox::get_Style()
{
	return m_sStyle.c_str();
}
//------------------------------------------------------------------------------
bool JSIOSEditBox::set_Visible( bool p_bVisible )
{
	m_bVisible = p_bVisible;
	CToObjectCSetEditBoxVisible( m_bVisible );
	return m_bVisible;
}
//------------------------------------------------------------------------------
bool JSIOSEditBox::get_Visible()
{
	return m_bVisible;
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setLeft( int p_nLeft )
{
	set_Left( p_nLeft );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setTop( int p_nTop )
{
	set_Top( p_nTop );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setWidth( int p_nWidth )
{
	set_Width( p_nWidth );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setHeight( int p_nHeight )
{
	set_Height( p_nHeight );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setOpacity( float p_Opacity )
{
	set_Opacity( p_Opacity );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setValue( const char* p_sValue )
{
	set_Value( p_sValue );
}
//------------------------------------------------------------------------------
const char* JSIOSEditBox::getValue()
{
	return get_Value();
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setStyle( const char* p_sStyle )
{
	set_Style( p_sStyle );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setVisible( bool p_bVisible )
{
	set_Visible( p_bVisible );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::focus()
{
    JCScriptRuntime::s_JSRT->m_pCurEditBox=this;
	CToObjectCSetEditBoxFocus();
}
//------------------------------------------------------------------------------
void JSIOSEditBox::blur()
{
	CToObjectCSetEditBoxBlur();
    JCScriptRuntime::s_JSRT->m_pCurEditBox=NULL;
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setColor( const char* p_sColor )
{
    uint32_t colorR{0};
    uint32_t colorG{0};
    uint32_t colorB{0};
    uint32_t colorA{0};
    parseRGBAFromString(p_sColor, colorR, colorG, colorB, colorA);
    int nColor = colorA << 24 | colorR << 16 | colorG << 8 | colorB;
	CToObjectCSetEditBoxColor( nColor );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setFontSize( int p_nFontSize )
{
	m_nFontSize = p_nFontSize;
	CToObjectCSetEditBoxFontSize( m_nFontSize*m_nScaleX );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setPos( int x,int y )
{
	m_nLeft = x;
	m_nTop = y;
	CToObjectCSetEditBoxFontPos( m_nLeft,m_nTop );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setSize( int w,int h )
{
	m_nWidth = w;
	m_nHeight = h;
	CToObjectCSetEditBoxFontSize( m_nWidth*m_nScaleX,m_nHeight*m_nScaleY );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setCursorPosition( int pos )
{
	CToObjectCSetEditBoxCursorPosition( pos );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setScale( float p_nSx,float p_nSy )
{
	m_nScaleX = p_nSx;
	m_nScaleY = p_nSy;
	setFontSize( m_nFontSize );
	setSize( m_nWidth,m_nHeight );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setMaxLength( int p_nMaxLength )
{
	m_nMaxLength = p_nMaxLength;
	CToObjectCSetEditBoxMaxLength( p_nMaxLength );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setType( const char* p_sType )
{
	m_sType = p_sType;
	bool bPassword = false;
	if( m_sType == "password" )
	{
		bPassword = true;
	}
	CToObjectCSetEditBoxPassword( bPassword );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setRegular( const char* p_sRegular )
{
	m_sRegular = p_sRegular;
	CToObjectCSetEditBoxRegular( p_sRegular );
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setFont( const char* p_sFont )
{
}
//------------------------------------------------------------------------------
void JSIOSEditBox::setNumberOnly( bool p_bNumberOnly )
{
	CToObjectCSetEditBoxNumberOnly( p_bNumberOnly );
}
void JSIOSEditBox::addEventListener(const char* p_sName, JSValueAsParam p_pFunction)
{
    if(strcmp(p_sName,"input")==0)
    {
        m_pJSFunctionOnInput.set(0,this,p_pFunction);
    }
}
void JSIOSEditBox::onInput()
{
    std::weak_ptr<int> cbref(m_CallbackRef);
    std::function<void(void)>pFunction =std::bind(&JSIOSEditBox::onInputCallJSFunction,this,cbref);
    JCScriptRuntime::s_JSRT->m_pScriptThread->post(pFunction);
}  
void JSIOSEditBox::onInputCallJSFunction(std::weak_ptr<int> callbackref)
{
    if(!callbackref.lock())
        return;
    m_pJSFunctionOnInput.Call();
}
void JSIOSEditBox::setMultiAble(bool p_bMultiAble)
{
    CToObjectCSetEditBoxMultiAble(p_bMultiAble);
}
void JSIOSEditBox::setForbidEdit(bool bForbidEdit)
{
	m_bForbidEdit = bForbidEdit;
    CToObjectCSetEditBoxForbidEdit(bForbidEdit);
}
bool JSIOSEditBox::getForbidEdit()
{
	return m_bForbidEdit;
}
void JSIOSEditBox::exportJS()
{
    JSP_CLASS("ConchInput", JSIOSEditBox);
    JSP_ADD_PROPERTY(left, JSIOSEditBox, get_Left, set_Left);
    JSP_ADD_PROPERTY(top, JSIOSEditBox, get_Top, set_Top);
    JSP_ADD_PROPERTY(width, JSIOSEditBox, get_Width, set_Width);
    JSP_ADD_PROPERTY(height, JSIOSEditBox, get_Height, set_Height);
    JSP_ADD_PROPERTY(opacity, JSIOSEditBox, get_Opacity, set_Opacity);
    JSP_ADD_PROPERTY(style, JSIOSEditBox, get_Style, set_Style);
    JSP_ADD_PROPERTY(value, JSIOSEditBox, get_Value, set_Value);
    JSP_ADD_PROPERTY(visible, JSIOSEditBox, get_Visible, set_Visible);
    JSP_ADD_METHOD("addEventListener", JSIOSEditBox::addEventListener);
    JSP_ADD_METHOD("setLeft", JSIOSEditBox::setLeft);
    JSP_ADD_METHOD("setTop", JSIOSEditBox::setTop);
    JSP_ADD_METHOD("setWidth", JSIOSEditBox::setWidth);
    JSP_ADD_METHOD("setHeight", JSIOSEditBox::setHeight);
    JSP_ADD_METHOD("setOpacity", JSIOSEditBox::setOpacity);
    JSP_ADD_METHOD("setValue", JSIOSEditBox::setValue);
    JSP_ADD_METHOD("getValue", JSIOSEditBox::getValue);
    JSP_ADD_METHOD("setStyle", JSIOSEditBox::setStyle);
    JSP_ADD_METHOD("setVisible", JSIOSEditBox::setVisible);
    JSP_ADD_METHOD("focus", JSIOSEditBox::focus);
    JSP_ADD_METHOD("blur", JSIOSEditBox::blur);
    JSP_ADD_METHOD("setColor", JSIOSEditBox::setColor);
    JSP_ADD_METHOD("setFontSize", JSIOSEditBox::setFontSize);
    JSP_ADD_METHOD("setPos", JSIOSEditBox::setPos);
    JSP_ADD_METHOD("setSize", JSIOSEditBox::setSize);
    JSP_ADD_METHOD("setCursorPosition", JSIOSEditBox::setCursorPosition);
    JSP_ADD_METHOD("setScale", JSIOSEditBox::setScale);
    JSP_ADD_METHOD("setMaxLength", JSIOSEditBox::setMaxLength);
    JSP_ADD_METHOD("setType", JSIOSEditBox::setType);
    JSP_ADD_METHOD("setNumberOnly", JSIOSEditBox::setNumberOnly);
    JSP_ADD_METHOD("setRegular", JSIOSEditBox::setRegular);
    JSP_ADD_METHOD("setFont", JSIOSEditBox::setFont);
    JSP_ADD_METHOD("setMultiAble", JSIOSEditBox::setMultiAble);
	JSP_ADD_METHOD("setForbidEdit", JSIOSEditBox::setForbidEdit);
	JSP_ADD_METHOD("getForbidEdit", JSIOSEditBox::getForbidEdit);
    JSP_INSTALL_CLASS("ConchInput", JSIOSEditBox);
}
//------------------------------------------------------------------------------
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//-----------------------------END FILE--------------------------------
