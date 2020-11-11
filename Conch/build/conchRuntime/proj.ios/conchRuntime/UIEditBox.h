/**
 @file			UIEditBox.h
 @brief         editBox类 继承 uitextField
 @author		wyw
 @version		1.0
 @date			2012_12_17
 @company       JoyChina
 */
#import "UIKit/UIKit.h"

@interface UIEditBox  : UITextView
{
    bool                    m_bNumberOnly;
    bool                    m_bForbidEdit;
    bool                    m_bIsMultiAble;
    int                     m_nMaxLength;
    NSString*               m_sRegular;
    NSRegularExpression*    m_pRegular;
}
-(UIEditBox*)init;
-(void) SetMaxLength:(int)p_nMaxLength;
-(int) GetMaxLength;
-(void) setNumberOnly:(bool)p_bNumberOnly;
-(void) setForbidEdit:(bool)p_bForbidEdit;
-(bool) getNumberOnly;
-(bool) getForbidEdit;
-(void) setRegular:(NSString*)p_sRegular;
-(BOOL) IsInputValid:(NSString*)p_sBuffer;
-(void)setMultiAble:(bool)p_bMultiAble;
-(BOOL) getMultiAble;

@end
