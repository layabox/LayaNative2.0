/**
 @file			LayaEditBox.h
 @brief         editBox类
 @author		wyw
 @version		1.0
 @date			2012_12_17
 @company       JoyChina
 */

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import "UIEditBox.h"

@class LayaEditBoxDelegate;
@interface LayaEditBox  : NSObject
{
@public
    UIEditBox*              m_pEditBox;
    GLKView*                m_pParentView;
    int                     m_nPosX;
    int                     m_nPoyY;
    int                     m_nWidth;
    int                     m_nHeight;
    int                     m_nMaxLength;
    NSString*               m_pOldStr;
    float                   m_fRetinaValue;
    LayaEditBoxDelegate*    m_pEditBoxDelegate;
}
//初始化
-(id)initWithParentView:(GLKView*)p_pParentView EditBoxDelegate:(LayaEditBoxDelegate*)p_pEditBoxDelegate ScreenRatio:(float)p_fScreenRatio;
//设置大小和为止
-(void) setLayoutParams;
//设置位置和宽高
-(void) setPosX:(int)p_pPosX;
-(void) setPosY:(int)p_pPosY;
-(void) setWidth:(int)p_pWidth;
-(void) setHeight:(int)p_pHeight;
//给editbox设置值
-(void) setValue:(NSString*)p_sValues;
//从editbox获得值
-(NSString*) getValue;
//设置背景颜色
-(void) setBackgroundColor:(UIColor*)p_nColor;
//设置是否为密码模式
-(void) setPasswordType:(BOOL)p_bPassWord;
//设置是否为NumberOnly
-(void) setNumberOnly:(BOOL)p_bNumberOnly;
//设置是否为禁止输入
-(void) setForbidEdit:(BOOL)p_bForbidEdit;
//设置是否允许多行
-(void)setMultiAble:(BOOL)p_bMultiAble;
//设置正则表达式
-(void) setRegular:(NSString*)p_sRegular;
//设置文字颜色
-(void) setTextColor:(UIColor*)p_nTextColor;
//设置文字大小
-(void) setTextSize:(int)p_nTextSize;
//设置是否可见
-(void) setVisible:(BOOL)p_bVisible;
//设置focus
-(void) setFocus:(BOOL)p_bFocus;
//设置最大输入值
-(void) setMaxLength:(int)p_nMaxLength;
//设置行数
-(void) setLines:(int)p_nLines;
//获得最大长度
-(int) getMaxLength;
//获得TextField 指针
-(UITextView*) getTextField;
//监听变化的
//-(void) textFieldDidChange:(UITextField*)p_pTextFiled;

@end
