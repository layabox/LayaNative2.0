/**
 @file			LayaEditBox.mm
 @brief         editBox类
 @author		wyw
 @version		1.0
 @date			2012_12_17
 @company       JoyChina
 */

#import "LayaEditBox.h"
#import "LayaEditBoxDelegate.h"

//-------------------------------------------------------------------------------
@implementation LayaEditBox
//-------------------------------------------------------------------------------
-(id)initWithParentView:(GLKView*)p_pParentView EditBoxDelegate:(LayaEditBoxDelegate*)p_pEditBoxDelegate ScreenRatio:(float)p_fScreenRatio;
{
    self = [super init];
    if( self != nil )
    {
        m_pParentView = p_pParentView;
        m_nPosX = 0;
        m_nPoyY = 0;
        m_nWidth = 0;
        m_nHeight = 0;
        m_nMaxLength = -1;
        m_fRetinaValue = p_fScreenRatio;
        
        m_pEditBox = [ [UIEditBox alloc] init ];
        m_pEditBox.delegate = p_pEditBoxDelegate;
        //m_pEditBox.borderStyle = UITextBorderStyleNone;
        [self setLayoutParams];
        UIColor* nColor = [UIColor colorWithRed:1 green:1 blue:1 alpha:0];
        [m_pEditBox setBackgroundColor:nColor];
        //设置字体
        m_pEditBox.font = [UIFont fontWithName:@"ArialMT" size:16];
        //当EditBox变化的时候监听到 暂时先不用了
        //[m_pEditBox addTarget:self action:@selector(textFieldDidChange:) forControlEvents:UIControlEventEditingChanged];
        m_pEditBox.autocapitalizationType = UITextAutocapitalizationTypeNone;
        [m_pParentView addSubview:m_pEditBox];
        [self setVisible:FALSE];
    }
    return self;
}
////-------------------------------------------------------------------------------
//-(void) textFieldDidChange:(UITextField*)p_pTextFiled
//{
//    if( m_pEditBox == p_pTextFiled )
//    {
//        NSString* pValue = [self getValue];
//        [[AppDelegate GetMainViewController]->m_pWebViewDelegate CallJSSetEditBoxValue:m_nID andDenominator:pValue];
//    }
//}
//-------------------------------------------------------------------------------
-(void)OnTextFieldDidBegin:(id)sender
{
    UIEditBox *pEditBox=(UIEditBox*)sender;
    NSString* val = [pEditBox text];
    m_pOldStr=val;
   
}
//-------------------------------------------------------------------------------
-(void)OnTextFieldDidChange:(id)sender
{
    UIEditBox *pEditBox=(UIEditBox*)sender;
    NSString* val = [pEditBox text];
    if( [m_pOldStr compare:val] !=0 )
    {
        //TODO
    }
    m_pOldStr=val;
}
//-------------------------------------------------------------------------------
-(void)setLayoutParams
{
    if( m_pEditBox != nil )
    {
        [m_pEditBox setBounds:CGRectMake(0,0,m_nWidth,m_nHeight) ];
        [m_pEditBox setCenter:CGPointMake(m_nPosX+m_nWidth/2,m_nPoyY+m_nHeight/2) ];
    }
}
//-------------------------------------------------------------------------------
-(void) setPosX:(int)p_nPosX
{
    m_nPosX = p_nPosX / m_fRetinaValue;
    [self setLayoutParams];
}
//-------------------------------------------------------------------------------
-(void) setPosY:(int)p_nPosY
{
    m_nPoyY = p_nPosY / m_fRetinaValue;
    [self setLayoutParams];
}
//-------------------------------------------------------------------------------
-(void) setWidth:(int)p_nWidth
{
    m_nWidth = p_nWidth / m_fRetinaValue;
    [self setLayoutParams];
}
//-------------------------------------------------------------------------------
-(void) setHeight:(int)p_nHeight
{
    m_nHeight = p_nHeight / m_fRetinaValue;
    [self setLayoutParams];
}
//-------------------------------------------------------------------------------
-(void) setValue:(NSString*)p_sValues
{
    if( m_pEditBox != nil )
    {
        m_pEditBox.text=p_sValues;
    }
}
//-------------------------------------------------------------------------------
-(NSString*) getValue
{
    if( m_pEditBox != nil )
    {
        NSString* value = [m_pEditBox text];
        return [value stringByReplacingOccurrencesOfString:@"\u2006" withString:@""];
    }
    return @"";
}
//-------------------------------------------------------------------------------
-(void) setBackgroundColor:(UIColor*)p_nColor
{
    if( m_pEditBox != nil )
    {
        m_pEditBox.backgroundColor = p_nColor;
    }
}
//-------------------------------------------------------------------------------
-(void) setPasswordType:(BOOL)p_bPassWord
{
    if( m_pEditBox != nil )
    {
        m_pEditBox.secureTextEntry = p_bPassWord;
    }
}
//-------------------------------------------------------------------------------
-(void) setNumberOnly:(BOOL)p_bNumberOnly
{
    if( m_pEditBox != nil )
    {
        [m_pEditBox setNumberOnly:p_bNumberOnly];
    }
}
-(void) setForbidEdit:(BOOL)p_bForbidEdit
{
    if( m_pEditBox != nil )
    {
        [m_pEditBox setForbidEdit:p_bForbidEdit];
    }
}
//-------------------------------------------------------------------------------
-(void)setMultiAble:(BOOL)p_bMultiAble
{
    if(m_pEditBox!=nil)
    {
        [m_pEditBox setMultiAble:p_bMultiAble];
    }
    NSLog(@">>>>>>>>>>>>>>>>>>>>>>>");
}
//-------------------------------------------------------------------------------
-(void) setRegular:(NSString*)p_sRegular
{
    if( m_pEditBox != nil )
    {
        [m_pEditBox setRegular:p_sRegular];
    }
}
//-------------------------------------------------------------------------------
-(void) setTextColor:(UIColor*)p_nTextColor
{
    if( m_pEditBox != nil )
    {
        m_pEditBox.textColor = p_nTextColor;
    }
}
//-------------------------------------------------------------------------------
-(void) setTextSize:(int)p_nTextSize
{
    if( m_pEditBox != nil )
    {
        m_pEditBox.font = [UIFont fontWithName:@"ArialMT" size:(p_nTextSize / m_fRetinaValue) ];
    }
}
//-------------------------------------------------------------------------------
-(void) setVisible:(BOOL)p_bVisible
{
    if ( m_pEditBox != nil )
    {
        [m_pEditBox setHidden:!p_bVisible];
    }
}
//-------------------------------------------------------------------------------
-(void) setFocus:(BOOL)p_bFocus
{
    if( m_pEditBox != nil )
    {
        //NSNumber* nsFocus = [NSNumber numberWithBool:p_bFocus];
        //[self performSelectorOnMainThread:@selector(setFocusMainThread:) withObject:nsFocus waitUntilDone:YES];
        if( p_bFocus == YES )
        {
            [m_pEditBox becomeFirstResponder];
            [m_pEditBox setHidden:NO];
        }
        else
        {
            [m_pEditBox resignFirstResponder];
            [m_pEditBox setHidden:YES];
        }
    }
}
//-------------------------------------------------------------------------------
-(void) setFocusMainThread:(id)param
{
    NSNumber* pParam = (NSNumber*)param;
    if( m_pEditBox != nil )
    {
        if( [pParam boolValue] == YES )
        {
            [m_pEditBox becomeFirstResponder];
        }
        else
        {
            [m_pEditBox resignFirstResponder];
        }
    }
}
//-------------------------------------------------------------------------------
-(void) setMaxLength:(int)p_nMaxLength
{
    m_nMaxLength = p_nMaxLength;
    [m_pEditBox SetMaxLength:m_nMaxLength];
}
//-------------------------------------------------------------------------------
-(int) getMaxLength
{
    return m_nMaxLength;
}
//-------------------------------------------------------------------------------
-(UITextView*) getTextField
{
    return m_pEditBox;
}
//-------------------------------------------------------------------------------
-(void) setLines:(int)p_nLines
{
    //TODO
}
//-------------------------------------------------------------------------------
@end
