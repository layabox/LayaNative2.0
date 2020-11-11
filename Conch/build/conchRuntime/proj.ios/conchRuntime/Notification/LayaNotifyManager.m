/**
 @file			LayaNotifyManager.m
 @brief         本地推送
 @author		James
 @version		1.0
 @date			2014_1_16
 @company       JoyChina
 */

#import "LayaNotifyManager.h"

static LayaNotifyManager* g_spNotifyManager = nil;

@implementation LayaNotifyManager

- (id)init
{
    self = [super init];
    return self;
}

+(LayaNotifyManager*)GetInstance
{
    if( g_spNotifyManager == nil )
    {
        g_spNotifyManager = [[LayaNotifyManager alloc] init];
    }
    return g_spNotifyManager;
}

-(void)setRepeatNotify:(int)p_nID startTime:(long)p_nStartTime repeateType:(int)nRepeateType tickerText:(NSString*)p_sTickerText titleText:(NSString*)p_sTitleText desc:(NSString*)p_sDesc
{
    [self deleteOnceNotify:p_nID];
    UILocalNotification *notification = [[UILocalNotification alloc] init];
    //设置10秒之后
    NSDate *pushDate = [NSDate dateWithTimeIntervalSinceNow:p_nStartTime/1000];
    if (notification != nil) {
        // 设置推送时间
        notification.fireDate = pushDate;
        // 设置时区
        notification.timeZone = [NSTimeZone defaultTimeZone];
        // 设置重复间隔
        if (nRepeateType == 0) {
            notification.repeatInterval = kCFCalendarUnitYear;
        }else if(nRepeateType == 1){
        notification.repeatInterval = kCFCalendarUnitMonth;
        }else if(nRepeateType == 2){
            notification.repeatInterval = kCFCalendarUnitDay;
        }else if(nRepeateType == 3){
            notification.repeatInterval = kCFCalendarUnitHour;
        }else if(nRepeateType == 4){
            notification.repeatInterval = kCFCalendarUnitMinute;
        }else if(nRepeateType == 5){
            notification.repeatInterval = kCFCalendarUnitSecond;
        }
        // 推送声音
        notification.soundName = UILocalNotificationDefaultSoundName;
        // 推送内容
        notification.alertBody = p_sDesc;
        //显示在icon上的红色圈中的数子
        notification.applicationIconBadgeNumber = 1;
        //设置userinfo 方便在之后需要撤销的时候使用
        //NSDictionary *info = [NSDictionary dictionaryWithObject:[NSString stringWithFormat:@"%d", p_nID] forKey:@"key"];
        NSDictionary *info = [NSDictionary dictionaryWithObjectsAndKeys:[NSString stringWithFormat:@"%d", p_nID],@"key",p_sTickerText,@"ticker",p_sTitleText,@"title",nil];
        notification.userInfo = info;
        //添加推送到UIApplication
        UIApplication *app = [UIApplication sharedApplication];
        [app scheduleLocalNotification:notification];
    }
}
//设置只提示一次的消息
-(void)setOnceNotify:(int)p_nID startTime:(long)p_nStartTime tickerText:(NSString*)p_sTickerText titleText:(NSString*)p_sTitleText desc:(NSString*)p_sDesc
{
    [self deleteOnceNotify:p_nID];
    
    UILocalNotification *notification = [[UILocalNotification alloc] init];
    //设置10秒之后
    NSDate *pushDate = [NSDate dateWithTimeIntervalSinceNow:p_nStartTime/1000];
    //NSDate *now=[NSDate new];
    if (notification != nil) {
        // 设置推送时间
        notification.fireDate = pushDate;
        // 设置时区
        notification.timeZone = [NSTimeZone defaultTimeZone];
        // 设置重复间隔
        //notification.repeatInterval = kCFCalendarUnitMinute;
        notification.repeatInterval = 0;
        // 推送声音
        notification.soundName = UILocalNotificationDefaultSoundName;
        // 推送内容
        notification.alertBody = p_sDesc;
        //显示在icon上的红色圈中的数子
        notification.applicationIconBadgeNumber = 1;
        //设置userinfo 方便在之后需要撤销的时候使用
        //NSDictionary *info = [NSDictionary dictionaryWithObject:@"name"forKey:@"key"];
        //NSDictionary *info = [NSDictionary dictionaryWithObject:[NSString stringWithFormat:@"%d", p_nID] forKey:@"key"];
        NSDictionary *info = [NSDictionary dictionaryWithObjectsAndKeys:[NSString stringWithFormat:@"%d", p_nID],@"key",p_sTickerText,@"ticker",p_sTitleText,@"title",nil];
        notification.userInfo = info;
        //添加推送到UIApplication
        UIApplication *app = [UIApplication sharedApplication];
        [app scheduleLocalNotification:notification];
    }
}
//只删除消息，但是保留定时器
//-(void)removeNotify:(int)p_nID;

//只删除全部消息，但是保留定时器
//-(void)removeAllNotify;

//删除某一个消息和定时器
-(void)deleteOnceNotify:(int)p_nID
{
    UIApplication *app = [UIApplication sharedApplication];
    //获取本地推送数组
    NSArray *localArray = [app scheduledLocalNotifications];
    //声明本地通知对象
    UILocalNotification *localNotification;
    if (localArray) {
        for (UILocalNotification *noti in localArray) {
            NSDictionary *dict = noti.userInfo;
            if (dict) {
                NSString *inKey = [dict objectForKey:@"key"];
                if ([inKey isEqualToString:[NSString stringWithFormat:@"%d", p_nID]]) {
                    if (!localNotification){
                        localNotification = noti;
                    }
                    break;
                }
            }
        }
        //判断是否找到已经存在的相同key的推送
        if (!localNotification) {
            //不存在初始化
            localNotification = [[UILocalNotification alloc] init];
        }
        if (localNotification) {
            //不推送 取消推送
            [app cancelLocalNotification:localNotification];
            return;
        }
    }
}
//删除所有消息和定时器
-(void)deleteAllNotify
{
    //8.0需要设置权限
    float fVersion = [[[UIDevice currentDevice] systemVersion] floatValue];
    if ( fVersion >= 8.0 )
    {
        UIUserNotificationSettings *settings = [UIUserNotificationSettings settingsForTypes:UIUserNotificationTypeBadge categories:nil];
        [[UIApplication sharedApplication] registerUserNotificationSettings:settings];
    }
    
    [[UIApplication sharedApplication] cancelAllLocalNotifications];
    [UIApplication sharedApplication].applicationIconBadgeNumber=0;
}
//显示推送消息
-(void)showNotification:(UILocalNotification*)notifi needApplication:(UIApplication *)application
{
    NSString *ticker;
    NSString *title;
    NSDictionary *dict = notifi.userInfo;
    if (dict) {
        title = [dict objectForKey:@"title"];
        ticker = [dict objectForKey:@"ticker"];
    }
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title message:notifi.alertBody delegate:nil cancelButtonTitle:ticker otherButtonTitles:nil];
    [alert show];
    // notification.applicationIconBadgeNumber -= 1;
    application.applicationIconBadgeNumber -= 1;
}


@end
