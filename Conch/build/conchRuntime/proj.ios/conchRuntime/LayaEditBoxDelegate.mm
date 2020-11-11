/**
 @file			LayaEditBoxDelegate.mm
 @brief         LayaEditBoxDelegate类
 @author		wyw
 @version		1.0
 @date			2012_12_17
 @company       JoyChina
 */

#import "LayaEditBoxDelegate.h"
#import "UIEditBox.h"

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import "conchRuntime.h"
#import "JCScriptRuntime.h"
#import "LayaEditBox.h"

//-------------------------------------------------------------------------------
@implementation LayaEditBoxDelegate

#define NUMBERS @"0123456789\n"
//-------------------------------------------------------------------------------
-(BOOL)textView:(UITextView *)textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)p_sString
{
    UIEditBox* pEditBox = (UIEditBox*)textView;
    if([pEditBox getForbidEdit] == true)return NO;
    if( [p_sString length] <= 0 )return YES;
    int nMaxLength = [pEditBox GetMaxLength];
    if( nMaxLength != -1 )
    {
        if(![pEditBox getMultiAble]&&[p_sString isEqualToString:@"\n"])
        {
            [pEditBox resignFirstResponder];
            return NO;
        }
        NSString* sLanguage = [[UITextInputMode currentInputMode]primaryLanguage];
        //中文输入
        if( [sLanguage isEqualToString:@"zh-Hans"] )
        {
            //range.length 代表 后选字母，比如 中文情况下的 yyy 。变灰色了
            if( range.length > 0 )
            {
                //如果已经够字数了，直接返回
                if( range.location >= nMaxLength )
                {
                    return NO;
                }
                else
                {
                    //获得为输入前EditBox中的内容
                    NSString* sOldText = [pEditBox.text substringToIndex:range.location];
                    //再加上现在的内容  p_sString 是本次输入的字符内容
                    NSString* sCurrentAndOldText = [ NSString stringWithFormat: @"%@%@", sOldText,p_sString ];
                    //如果超过字数，进行裁掉后面的
                    if( sCurrentAndOldText.length > nMaxLength )
                    {
                        pEditBox.text = [sCurrentAndOldText substringToIndex:nMaxLength];
                    }
                    else
                    {
                        pEditBox.text = sCurrentAndOldText;
                    }
                    return NO;
                }
            }
        }
        else if([sLanguage isEqualToString:@"emoji"]){
            return NO;
        }
        else
        {
            if( range.location >= nMaxLength )
            {
                return NO;
            }
        }
    }
    bool bNumberOnly = [pEditBox getNumberOnly];
    if( bNumberOnly )
    {
        NSCharacterSet* cs = [[NSCharacterSet characterSetWithCharactersInString:NUMBERS] invertedSet];
        NSString* nsFiltered = [[p_sString componentsSeparatedByCharactersInSet:cs] componentsJoinedByString:@""];
        BOOL bBasicTest = [p_sString isEqualToString:nsFiltered];
        if(!bBasicTest)
        {
            return NO;
        }
    }
    return [pEditBox IsInputValid:p_sString];
}
//-------------------------------------------------------------------------------
-(void)textViewDidBeginEditing:(UITextView *)textField
{
    m_nEditBoxPosY = ( textField.frame.origin.y + textField.frame.size.height ) * m_fRetinaValue;
}
//-------------------------------------------------------------------------------
-(void)textViewDidEndEditing:(UITextView *)textField
{
    GLKView* pView = [conchRuntime GetIOSConchRuntime]->m_pGLKView;
    
    if( [conchRuntime GetIOSConchRuntime]->m_nGLViewOffset != 0 )
    {
        float width = pView.frame.size.width;
        float height = pView.frame.size.height;
        CGRect rect = CGRectMake(0.0f, 0, width, height );
        pView.frame = rect;
        [conchRuntime GetIOSConchRuntime]->m_nGLViewOffset = 0;
        m_nEditBoxPosY = 0;
    }
}
- (void)keyboardWasShown:(NSNotification*)aNotification
{
    [conchRuntime GetIOSConchRuntime]->m_bIgnoreCurEvent = true;
}
-(void)keyboardWasShownEnd:(NSNotification*)aNotification
{
    [conchRuntime GetIOSConchRuntime]->m_bIgnoreCurEvent = false;
    UITextView * textField=[conchRuntime GetIOSConchRuntime]->m_pEditBox->m_pEditBox;
    m_nEditBoxPosY = ( textField.frame.origin.y + textField.frame.size.height ) * m_fRetinaValue;
    NSDictionary* info = [aNotification userInfo];
    //kbSize即為鍵盤尺寸 (有width, height)
    CGSize kbSize = [[info objectForKey:UIKeyboardFrameEndUserInfoKey] CGRectValue].size;//得到鍵盤的高度
    NSLog(@"keyboard height:%f",kbSize.height);
    
    GLKView* pView = [conchRuntime GetIOSConchRuntime]->m_pGLKView;
    CGPoint* pResolution = [conchRuntime GetIOSConchRuntime]->m_pResolution;
    float width = pResolution->x;    //  当前宽的点
    float height = pResolution->y;   //  当前高的点
    float realWidth = pView.frame.size.width;
    float realHeight = pView.frame.size.height;
    int nKbHeight = height == realHeight ? kbSize.height:kbSize.width;
    int nOffset = 0;
    if( (height*m_fRetinaValue - m_nEditBoxPosY)< nKbHeight*m_fRetinaValue){
        nOffset = (height - m_nEditBoxPosY/m_fRetinaValue) - nKbHeight-5;
        float fIOSVersion = [conchRuntime GetIOSConchRuntime]->m_fIOSVersion;
        if( (m_orient == UIDeviceOrientationLandscapeLeft||
             m_orient == UIDeviceOrientationPortraitUpsideDown||
             (m_orient == UIDeviceOrientationFaceUp&&(
                                                      m_lastOrient == UIDeviceOrientationLandscapeLeft ||
                                                      m_lastOrient == UIDeviceOrientationPortraitUpsideDown)))&& fIOSVersion<8)
            nOffset*=-1;
        
        [conchRuntime GetIOSConchRuntime]->m_nGLViewOffset = nOffset;
        int xOffset = width == realWidth ? 0 :nOffset;
        int yOffset = height == realHeight ? nOffset: 0;
        CGRect rect = CGRectMake( xOffset,yOffset, realWidth, realHeight );
        pView.frame = rect;
    }
}
-(void)orientChange:(NSNotification *)aNotification
{
    //NSDictionary* ntfDict = [aNotification userInfo];
    m_lastOrient = m_orient;
    m_orient = [UIDevice currentDevice].orientation;
    /*
     UIDeviceOrientationUnknown,
     UIDeviceOrientationPortrait,            // Device oriented vertically, home button on the bottom
     UIDeviceOrientationPortraitUpsideDown,  // Device oriented vertically, home button on the top
     UIDeviceOrientationLandscapeLeft,       // Device oriented horizontally, home button on the right
     UIDeviceOrientationLandscapeRight,      // Device oriented horizontally, home button on the left
     UIDeviceOrientationFaceUp,              // Device oriented flat, face up
     UIDeviceOrientationFaceDown             // Device oriented flat, face down   
     */
    switch (m_orient)
    {
        case UIDeviceOrientationPortrait:
            break;
            
        case UIDeviceOrientationLandscapeLeft:
            break;
            
        case UIDeviceOrientationPortraitUpsideDown:
            break;
            
        case UIDeviceOrientationLandscapeRight:
            break;
        case UIDeviceOrientationFaceUp:
        case UIDeviceOrientationFaceDown:
        case UIDeviceOrientationUnknown:
        default:
            break;
            
    }
}
//-------------------------------------------------------------------------------
-(void) setRetinaValue:(float)fRetina
{
    m_fRetinaValue = fRetina;
}
-(void) postToC
{
    if(JCScriptRuntime::s_JSRT->m_pCurEditBox!=NULL)
    {
        JCScriptRuntime::s_JSRT->m_pCurEditBox->onInput();
    }
}
-(void)textViewDidChange:(UITextView *)textView
{
    [self postToC];
}

@end
