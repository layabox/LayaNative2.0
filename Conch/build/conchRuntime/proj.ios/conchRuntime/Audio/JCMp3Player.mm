/**
 @file			JCMp3Player.mm
 @brief
 @author		wyw
 @version		1.0
 @date			2013_1_1
*/

#import "JCMp3PLayer.h"
#import "../../../../../source/conch/Audio/JCAudioManager.h"

//------------------------------------------------------------------------------
@implementation JCMp3Player
//------------------------------------------------------------------------------
-(JCMp3Player*)init
{
    self = [super init];
    m_pPlayer = nil;
    return self;
}
//------------------------------------------------------------------------------
-(void)playMp3:(NSString*)p_sFilePath times:(int)p_nTimes currentTime:(float)nCurrentTime
{
    //先停止
    if( m_pPlayer != nil )
    {
        if( m_pPlayer.playing )
        {
            [m_pPlayer stop];
        }
    }
    //播放mp3
    NSURL* sUrl = [NSURL fileURLWithPath:p_sFilePath];
    m_pPlayer = [ [AVAudioPlayer alloc] initWithContentsOfURL:sUrl error:nil];
    m_pPlayer.delegate = self;
    m_pPlayer.numberOfLoops = p_nTimes;
    m_pPlayer.currentTime = nCurrentTime;
    [m_pPlayer prepareToPlay];
    [m_pPlayer play];
}
//------------------------------------------------------------------------------
-(void)stopMp3
{
    if( m_pPlayer != nil )
    {
        [m_pPlayer stop];
    }
}
//------------------------------------------------------------------------------
-(void)setCurrentTime: (double)nCurrentTime
{
    m_pPlayer.currentTime = nCurrentTime;
}
//------------------------------------------------------------------------------
-(double)getCurrentTime
{
    return m_pPlayer.currentTime;
}
//------------------------------------------------------------------------------
-(double)getDuration
{
    return m_pPlayer.duration;
}
-(void)pauseMp3
{
    if( m_pPlayer != nil )
    {
        [m_pPlayer pause];
    }
}
//------------------------------------------------------------------------------
-(void)resumeMp3
{
    if( m_pPlayer != nil )
    {
        [m_pPlayer play];
    }
}
//------------------------------------------------------------------------------
-(void)setVolume:(float)p_nVolume
{
    [m_pPlayer setVolume:p_nVolume];
}
//------------------------------------------------------------------------------
-(void)dealloc
{
    if( m_pPlayer.playing )
    {
        [m_pPlayer stop];
    }
}
//------------------------------------------------------------------------------
- (void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag
{
    laya::JCAudioManager::GetInstance()->m_pMp3Player->onPlayEnd();
}
//------------------------------------------------------------------------------
- (void)audioPlayerDecodeErrorDidOccur:(AVAudioPlayer *)player error:(NSError *)error
{
    NSLog(@">>>>>ios mp3 player audioPlayerDecodeErrorDidOccur error=%@",error );
}
//------------------------------------------------------------------------------
@end
