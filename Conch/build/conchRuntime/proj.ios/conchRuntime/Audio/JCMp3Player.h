/**
 @file			JCMp3Player.h
 @brief
 @author		wyw
 @version		1.0
 @date			2013_1_1
*/


#import <Foundation/Foundation.h>
#import <AudioToolbox/AudioToolbox.h>
#import <AVFoundation/AVFoundation.h>
#import <AudioToolbox/AudioServices.h>


@interface JCMp3Player : NSObject<AVAudioPlayerDelegate>
{
@public
    AVAudioPlayer*      m_pPlayer;
}

//初始化
-(JCMp3Player*)init;
//播放声音
-(void)playMp3:(NSString*)p_sFilePath times:(int)p_nTimes currentTime:(float)nCurrentTime;
-(void)stopMp3;
-(void)pauseMp3;
-(void)resumeMp3;
//设置音量
-(void)setVolume:(float)p_nVolume;
-(void)setCurrentTime: (double)nCurrentTime;
-(double)getCurrentTime;
-(double)getDuration;
@end