/**
@file			JSAndroidEditBox.cpp
@brief			
@author			hugao
@version		1.0
@date			2016_5_19
*/

//包含头文件
#ifdef ANDROID
#include <jni.h>
#endif
#include "JSAndroidEditBox.h"
#include "../JSInterface/JSInterface.h"
#include "../../JCScriptRuntime.h"
#include "util/Log.h"
#ifdef ANDROID
	#include "../../CToJavaBridge.h"
#endif
#include "util/JCColor.h"
#include <util/ColorParser.h>
	
namespace laya 
{
ADDJSCLSINFO(JSAndroidEditBox, JSObjNode );
//------------------------------------------------------------------------------
JSAndroidEditBox::JSAndroidEditBox()
{
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
	AdjustAmountOfExternalAllocatedMemory( 256 );
	JCMemorySurvey::GetInstance()->newClass( "AndroidEditBox",256,this );
}
//------------------------------------------------------------------------------
JSAndroidEditBox::~JSAndroidEditBox()
{
    JCMemorySurvey::GetInstance()->releaseClass( "AndroidEditBox",this );
}
//------------------------------------------------------------------------------
void JSAndroidEditBox::addEventListener(const char* p_sName, JSValueAsParam p_pFunction )
{
    if(strcmp( p_sName,"input" ) == 0)
    {
        m_pJSFunctionOnInput.set(0,this,p_pFunction);
    }
    else if(strcmp( p_sName,"keydown" ) == 0)
    {
        //m_pJSFunctionOnKeydown=p_pFunction;
    }
}
//------------------------------------------------------------------------------
int JSAndroidEditBox::set_Left( int p_nLeft )
{
	m_nLeft = p_nLeft;
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setEditBoxPosX", p_nLeft, kRet);
	return m_nLeft;
}
//------------------------------------------------------------------------------
int JSAndroidEditBox::get_Left()
{
	return m_nLeft;
}
//------------------------------------------------------------------------------
int JSAndroidEditBox::set_Top( int p_nTop )
{
	m_nTop = p_nTop;
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setEditBoxPosY", p_nTop, kRet);
	return m_nTop;
}
//------------------------------------------------------------------------------
int JSAndroidEditBox::get_Top()
{
	return m_nTop;
}
//------------------------------------------------------------------------------
int JSAndroidEditBox::set_Width( int p_nWidth )
{
	m_nWidth = p_nWidth;
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setEditBoxWidth", (int)(p_nWidth * m_nScaleX), kRet);
	return m_nWidth;
}
//------------------------------------------------------------------------------
int JSAndroidEditBox::get_Width()
{
	return m_nWidth;
}
//-------------------------------------------false-----------------------------------
int JSAndroidEditBox::set_Height( int p_nHeight )
{
	m_nHeight = p_nHeight;
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(),"setEditBoxHeight",(int)(p_nHeight*m_nScaleY),kRet);
	return m_nHeight;
}
//------------------------------------------------------------------------------
int JSAndroidEditBox::get_Height()
{
	return m_nHeight;
}
//------------------------------------------------------------------------------
float JSAndroidEditBox::set_Opacity( float p_Opacity )
{
	m_fOpacity = p_Opacity;
	return m_fOpacity;
}
//------------------------------------------------------------------------------
float JSAndroidEditBox::get_Opacity()
{
	return m_fOpacity;
}
//------------------------------------------------------------------------------
const char* JSAndroidEditBox::set_Value( const char* p_sValue )
{
	LOGI("JSAndroidEditBox::set_Value=%s",p_sValue );
	m_sValue = ( p_sValue != NULL ) ? p_sValue : "";
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setEditBoxValue", p_sValue, kRet);
	return m_sValue.c_str();
}
//------------------------------------------------------------------------------
const char* JSAndroidEditBox::get_Value()
{
	std::vector<intptr_t> params;
	CToJavaBridge::JavaRet kRet;
	if(CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getEditBoxValue", kRet))
    {
        m_sValue=CToJavaBridge::GetInstance()->getJavaString( kRet.pJNI,kRet.strRet);
	    LOGI("JSAndroidEditBox::get_Value=%s",m_sValue.c_str() );
    }
	return m_sValue.c_str();
}
//------------------------------------------------------------------------------
void JSAndroidEditBox::set_Style( const char* p_sStyle )
{
	if(p_sStyle==NULL)
		m_sStyle="";
	else
		m_sStyle = p_sStyle;
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setEditBoxStyle", m_sStyle.c_str(),kRet);
}
//------------------------------------------------------------------------------
const char* JSAndroidEditBox::get_Style()
{
	return m_sStyle.c_str();
}
//------------------------------------------------------------------------------
bool JSAndroidEditBox::set_Visible( bool p_bVisible )
{
	m_bVisible = p_bVisible;
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setEditBoxVisible", p_bVisible, kRet);
	return m_bVisible;
}
//------------------------------------------------------------------------------
bool JSAndroidEditBox::get_Visible()
{
	return m_bVisible;
}
//------------------------------------------------------------------------------
void JSAndroidEditBox::setLeft( int p_nLeft )
{
	set_Left( p_nLeft );
}
//------------------------------------------------------------------------------
void JSAndroidEditBox::setTop( int p_nTop )
{
	set_Top( p_nTop );
}
//------------------------------------------------------------------------------
void JSAndroidEditBox::setWidth( int p_nWidth )
{
	set_Width( p_nWidth );
}
//------------------------------------------------------------------------------
void JSAndroidEditBox::setHeight( int p_nHeight )
{
	set_Height( p_nHeight );
}
//------------------------------------------------------------------------------
void JSAndroidEditBox::setOpacity( float p_Opacity )
{
	set_Opacity( p_Opacity );
}
//------------------------------------------------------------------------------
void JSAndroidEditBox::setValue( const char* p_sValue )
{
    if(p_sValue==NULL)
        set_Value("");
    else
	    set_Value( p_sValue );
}
//------------------------------------------------------------------------------
const char* JSAndroidEditBox::getValue()
{
	return get_Value();
}
//------------------------------------------------------------------------------
void JSAndroidEditBox::setStyle( const char* p_sStyle )
{
	set_Style( p_sStyle );
}
//------------------------------------------------------------------------------
void JSAndroidEditBox::setVisible( bool p_bVisible )
{
	set_Visible( p_bVisible );
}
//------------------------------------------------------------------------------
void JSAndroidEditBox::focus()
{
    JCScriptRuntime::s_JSRT->m_pCurEditBox=this;
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod( CToJavaBridge::JavaClass.c_str(), "setEditBoxFocus", kRet);
}
//------------------------------------------------------------------------------
void JSAndroidEditBox::blur()
{
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod( CToJavaBridge::JavaClass.c_str(), "setEditBoxBlur", kRet);
    JCScriptRuntime::s_JSRT->m_pCurEditBox = NULL;
}
//------------------------------------------------------------------------------
void JSAndroidEditBox::setColor( const char* p_sColor )
{
	uint32_t colorR{0};
    uint32_t colorG{0};
    uint32_t colorB{0};
    uint32_t colorA{0};
	parseRGBAFromString(p_sColor, colorR, colorG, colorB, colorA);
	int nColor = colorA << 24 | colorR << 16 | colorG << 8 | colorB;

	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setEditBoxColor", nColor, kRet);
}
//------------------------------------------------------------------------------
void JSAndroidEditBox::setFontSize( int p_nFontSize )
{
	m_nFontSize = p_nFontSize;
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setEditBoxFontSize", (int)(p_nFontSize * m_nScaleY), kRet);
}
//------------------------------------------------------------------------------
void JSAndroidEditBox::setPos( int x,int y )
{
	m_nLeft = x;
	m_nTop = y;
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod( CToJavaBridge::JavaClass.c_str(), "setEditBoxPos", x,y, kRet);
}
//------------------------------------------------------------------------------
void JSAndroidEditBox::setSize( int w,int h )
{
	m_nWidth = w;
	m_nHeight = h;
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod( CToJavaBridge::JavaClass.c_str(), "setEditBoxSize", (int)(w * m_nScaleX), (int)(h * m_nScaleY), kRet);
}
//------------------------------------------------------------------------------
void JSAndroidEditBox::setCursorPosition( int pos )
{
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod( CToJavaBridge::JavaClass.c_str(), "setEditBoxCursorPosition", pos, kRet);
}
//------------------------------------------------------------------------------
void JSAndroidEditBox::setScale( float p_nSx,float p_nSy )
{
	m_nScaleX = p_nSx;
	m_nScaleY = p_nSy;
	setFontSize( m_nFontSize );
	setSize( m_nWidth,m_nHeight );
}
//------------------------------------------------------------------------------
void JSAndroidEditBox::setMaxLength( int p_nMaxLength )
{
	m_nMaxLength = p_nMaxLength;
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setEditBoxMaxLength", p_nMaxLength, kRet);
}
//------------------------------------------------------------------------------
void JSAndroidEditBox::setType( const char* p_sType )
{
	m_sType = p_sType;
	bool bPassword = false;
	if( m_sType == "password" )
	{
		bPassword = true;
	}
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod( CToJavaBridge::JavaClass.c_str(), "setEditBoxPassword", bPassword, kRet);
}
//------------------------------------------------------------------------------
void JSAndroidEditBox::setRegular( const char* p_sRegular )
{
	m_sRegular = p_sRegular;
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod( CToJavaBridge::JavaClass.c_str(), "setEditBoxRegular", p_sRegular, kRet);
}
//------------------------------------------------------------------------------
void JSAndroidEditBox::setFont( const char* p_sFont )
{
}
//------------------------------------------------------------------------------
void JSAndroidEditBox::setNumberOnly( bool p_bNumberOnly )
{
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setEditBoxNumberOnly", p_bNumberOnly, kRet);
}
//------------------------------------------------------------------------------
void  JSAndroidEditBox::onInputCallJSFunction(std::weak_ptr<int> callbackref)
{
    if( !callbackref.lock())
        return;
    m_pJSFunctionOnInput.Call();
}
//------------------------------------------------------------------------------
void JSAndroidEditBox::onInput()
{
    std::weak_ptr<int> cbref(m_CallbackRef);
    std::function<void(void)> pFunction = std::bind(&JSAndroidEditBox::onInputCallJSFunction,this, cbref);
    JCScriptRuntime::s_JSRT->m_pScriptThread->post( pFunction );
}
void JSAndroidEditBox::setMultiAble(bool p_bMultiAble)
{
    CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(),"setMultiAble",p_bMultiAble,kRet);
}
void JSAndroidEditBox::setForbidEdit( bool bForbidEdit )
{
	m_bForbidEdit = bForbidEdit;
	CToJavaBridge::JavaRet kRet;
    CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setForbidEdit", bForbidEdit, kRet);
}
bool JSAndroidEditBox::getForbidEdit()
{
	return m_bForbidEdit;
}
void JSAndroidEditBox::exportJS() 
{
    JSP_CLASS("ConchInput", JSAndroidEditBox);
    JSP_ADD_PROPERTY(left, JSAndroidEditBox, get_Left, set_Left);//2
    JSP_ADD_PROPERTY(top, JSAndroidEditBox, get_Top, set_Top);//2
    JSP_ADD_PROPERTY(width, JSAndroidEditBox, get_Width, set_Width);//2
    JSP_ADD_PROPERTY(height, JSAndroidEditBox, get_Height, set_Height);//2
    JSP_ADD_PROPERTY(opacity, JSAndroidEditBox, get_Opacity, set_Opacity);//2
    JSP_ADD_PROPERTY(style, JSAndroidEditBox, get_Style, set_Style);
    JSP_ADD_PROPERTY(value, JSAndroidEditBox, get_Value, set_Value);//2
    JSP_ADD_PROPERTY(visible, JSAndroidEditBox, get_Visible, set_Visible);//2
    JSP_ADD_METHOD("addEventListener", JSAndroidEditBox::addEventListener);
    JSP_ADD_METHOD("setLeft", JSAndroidEditBox::setLeft);
    JSP_ADD_METHOD("setTop", JSAndroidEditBox::setTop);
    JSP_ADD_METHOD("setWidth", JSAndroidEditBox::setWidth);
    JSP_ADD_METHOD("setHeight", JSAndroidEditBox::setHeight);
    JSP_ADD_METHOD("setOpacity", JSAndroidEditBox::setOpacity);
    JSP_ADD_METHOD("setValue", JSAndroidEditBox::setValue);
    JSP_ADD_METHOD("getValue", JSAndroidEditBox::getValue);
    JSP_ADD_METHOD("setStyle", JSAndroidEditBox::setStyle);
    JSP_ADD_METHOD("setVisible", JSAndroidEditBox::setVisible);
    JSP_ADD_METHOD("focus", JSAndroidEditBox::focus);
    JSP_ADD_METHOD("blur", JSAndroidEditBox::blur);
    JSP_ADD_METHOD("setColor", JSAndroidEditBox::setColor);
    JSP_ADD_METHOD("setFontSize", JSAndroidEditBox::setFontSize);
    JSP_ADD_METHOD("setPos", JSAndroidEditBox::setPos);
    JSP_ADD_METHOD("setSize", JSAndroidEditBox::setSize);
    JSP_ADD_METHOD("setCursorPosition", JSAndroidEditBox::setCursorPosition);
    JSP_ADD_METHOD("setScale", JSAndroidEditBox::setScale);
    JSP_ADD_METHOD("setMaxLength", JSAndroidEditBox::setMaxLength);
    JSP_ADD_METHOD("setType", JSAndroidEditBox::setType);
    JSP_ADD_METHOD("setNumberOnly", JSAndroidEditBox::setNumberOnly);
    JSP_ADD_METHOD("setRegular", JSAndroidEditBox::setRegular);
    JSP_ADD_METHOD("setFont", JSAndroidEditBox::setFont);
    JSP_ADD_METHOD("setMultiAble", JSAndroidEditBox::setMultiAble);
	JSP_ADD_METHOD("setForbidEdit",JSAndroidEditBox::setForbidEdit);
	JSP_ADD_METHOD("getForbidEdit",JSAndroidEditBox::getForbidEdit);
    JSP_INSTALL_CLASS("ConchInput", JSAndroidEditBox);
}
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------