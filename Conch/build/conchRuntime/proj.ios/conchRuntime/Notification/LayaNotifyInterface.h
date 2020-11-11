/**
 @file			LayaNotifyInterface
 @brief         本地推送接口
 @author		James
 @version		1.0
 @date			2014_1_16
 @company       JoyChina
 */
#import <UIKit/UIKit.h>

@protocol LayaNotifyInterface

//设置重复的消息
-(void)setRepeatNotify:(int)p_nID startTime:(long)p_nStartTime repeateType:(int)nRepeateType tickerText:(NSString*)p_sTickerText titleText:(NSString*)p_sTitleText desc:(NSString*)p_sDesc;

//设置只提示一次的消息
-(void)setOnceNotify:(int)p_nID startTime:(long)p_nStartTime tickerText:(NSString*)p_sTickerText titleText:(NSString*)p_sTitleText desc:(NSString*)p_sDesc;

//只删除消息，但是保留定时器
//-(void)removeNotify:(int)p_nID;

//删除某一个消息和定时器
-(void)deleteOnceNotify:(int)p_nID;

//只删除全部消息，但是保留定时器
//-(void)removeAllNotify;

//删除消息和定时器
-(void)deleteAllNotify;

//显示推送消息
-(void)showNotification:(UILocalNotification*)notifi needApplication:(UIApplication *)application;

@end
