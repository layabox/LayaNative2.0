/**
 @file			LayaAlert.h
 @brief         LayaAlert类
 @author		James
 @version		1.0
 @date			2016_7_6
 @company       LayaBox
 */

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import <string>
#import <vector>

@interface LayaAlert  : NSObject<UIAlertViewDelegate>
{
@public
    UIAlertView*                m_pAlert;
    std::vector<std::string>    m_vSaveInfo;
    bool                        m_bShow;
}
//初始化
-(id)init;
-(void)alert:(std::string)sInfo;
-(void)showAlert;
-(void)reset;
@end
