/**
 @file			UIEditBox.mm
 @brief         继承textField类
 @author		wyw
 @version		1.0
 @date			2012_12_17
 @company       JoyChina
 */

#import "UIEditBox.h"
//-------------------------------------------------------------------------------
@implementation UIEditBox
//-------------------------------------------------------------------------------
-(UIEditBox*)init
{
    self = [super init];
    m_nMaxLength = -1;
    m_sRegular = NULL;
    m_pRegular = NULL;
    m_bNumberOnly = false;
    m_bIsMultiAble=false;
    //[self setRegular:@"\\d+"];
    m_bForbidEdit = false;
    return self;
}
//-------------------------------------------------------------------------------
-(void) setNumberOnly:(bool)p_bNumberOnly
{
    m_bNumberOnly = p_bNumberOnly;
}
-(void) setForbidEdit:(bool)p_bForbidEdit
{
    m_bForbidEdit = p_bForbidEdit;
}
//-------------------------------------------------------------------------------
-(bool) getNumberOnly
{
    return m_bNumberOnly;
}
//-------------------------------------------------------------------------------    
-(bool) getForbidEdit
{
    return m_bForbidEdit;
}
//-------------------------------------------------------------------------------
-(void) SetMaxLength:(int)p_nMaxLength
{
    m_nMaxLength = p_nMaxLength;
}
//-------------------------------------------------------------------------------
-(int) GetMaxLength
{
    return m_nMaxLength;
}
//-------------------------------------------------------------------------------
-(void) setRegular:(NSString*)p_sRegular
{
    m_sRegular = p_sRegular;
    NSError *error = NULL;
    m_pRegular = [ NSRegularExpression regularExpressionWithPattern:m_sRegular options:NSRegularExpressionCaseInsensitive error:&error];
}
//-------------------------------------------------------------------------------
-(BOOL) IsInputValid:(NSString*)p_sBuffer
{
    if( m_pRegular == NULL || m_sRegular == NULL )return true;
    NSArray* vArrays = [m_pRegular matchesInString:p_sBuffer options:NSMatchingCompleted range:NSMakeRange( 0, [p_sBuffer length] )];
    if( vArrays != NULL && [vArrays count] > 0 )
    {
        return true;
    }
    return false;
}
//-------------------------------------------------------------------------------
-(void)setMultiAble:(bool)p_bMultiAble
{
    m_bIsMultiAble=p_bMultiAble;
}
-(BOOL) getMultiAble
{
    return m_bIsMultiAble;
}

@end
