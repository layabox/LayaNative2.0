/**
 @file			conchConfig.h
 @brief         配置用到的，比如版本号 或者描述信息
 @author		James
 @version		1.0
 @date			2013_7_5
 @company       LayaBox
 */
#import "UIKit/UIKit.h"
#import <GLKit/GLKit.h>
#import <string>

@interface conchConfig : NSObject
{
@public
    NSString*       m_sAppVersion;          //对外版本号
    NSString*       m_sAppLocalVersion;     //对内版本号
    NSString*       m_sGameID;              //appStroe用到的
    bool            m_bCheckNetwork;        //是否检查网络
    bool            m_bNotification;        //是否打开推送
    /*
     UIInterfaceOrientationMaskPortrait,             ===2
     UIInterfaceOrientationMaskPortraitUpsideDown,   ===4
     UIInterfaceOrientationMaskLandscapeLeft,        ===8
     UIInterfaceOrientationMaskLandscapeRight,       ===16
     */
    int             m_nOrientationType;     //游戏的方向
}
+(conchConfig*)GetInstance;
-(bool)readIni;
-(conchConfig*)init;
@end
