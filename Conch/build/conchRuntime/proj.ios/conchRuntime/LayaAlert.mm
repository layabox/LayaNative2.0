/**
 @file			LayaAlert.mm
 @brief
 @author		James
 @version		1.0
 @date			2016_7_6
 @company       LayaBox
 */

#import <UIKit/UIKit.h>
#import "LayaAlert.h"

//-----------------------------------------------------------------
@implementation LayaAlert

-(id)init
{
    self = [super init];
    if( self != nil )
    {
        m_pAlert = nil;
        m_bShow = false;
    }
    return self;
}
-(void)reset
{
    m_bShow = false;
    m_vSaveInfo.clear();
}
-(void)alert:(std::string)sInfo
{
    m_vSaveInfo.push_back(sInfo);
    if( m_bShow == false )
    {
        [self showAlert];
    }
    if( m_vSaveInfo.size() >= 50 )
    {
        NSLog(@">>>>Too much alert");
    }
}
-(void)showAlert
{
    if( m_pAlert == nil)
    {
        m_pAlert = [[UIAlertView alloc] initWithTitle:@"alert" message:nil delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil];
    }
    if( m_vSaveInfo.size() >0 )
    {
        //取出第0个，显示然后删除
        std::string sInfo = m_vSaveInfo[0];
        std::vector<std::string>::iterator iter = m_vSaveInfo.begin();
        iter = m_vSaveInfo.erase(iter);
        
        NSString* nsMessage = [NSString stringWithUTF8String:sInfo.c_str()];
        m_pAlert.message = nsMessage;
        [m_pAlert show];
    }
}
//根据被点击按钮的索引处理点击事件
-(void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    
}
//AlertView已经消失时执行的事件
-(void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
    m_bShow = false;
    [self showAlert];
}
//ALertView即将消失时的事件
-(void)alertView:(UIAlertView *)alertView willDismissWithButtonIndex:(NSInteger)buttonIndex
{
    
}
//AlertView的取消按钮的事件
-(void)alertViewCancel:(UIAlertView *)alertView
{
    
}
//AlertView已经显示时的事件
-(void)didPresentAlertView:(UIAlertView *)alertView
{
    
}
//AlertView即将显示时
-(void)willPresentAlertView:(UIAlertView *)alertView
{
    m_bShow = true;
}
@end