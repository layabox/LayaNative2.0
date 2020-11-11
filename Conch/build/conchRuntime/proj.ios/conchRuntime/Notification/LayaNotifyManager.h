/**
 @file			LayaNotifyManager.h
 @brief         本地推送
 @author		James
 @version		1.0
 @date			2014_1_16
 @company       JoyChina
 */

#import "LayaNotifyInterface.h"


@interface LayaNotifyManager : NSObject<LayaNotifyInterface>
{
}

+(LayaNotifyManager*)GetInstance;

@end
