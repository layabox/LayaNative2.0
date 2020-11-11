//
//  LayaPlayer.m
//  conchRuntime
//
//  Created by LayaBox on 2018/10/20.
//  Copyright © 2018 LayaBox. All rights reserved.
//

#import "LayaVideoPlayer.h"
#import "Video/PlayerLayerRenderTarget.h"
#import "Video/VideoOutputRenderTarget.h"

static GLKView* ms_pCurParentView;
static float ms_rentinaValue;

@implementation LayaVideoPlayer

+(void)setCurParentView:(GLKView *)view withRetianValue: (float) rentinaValue {
    ms_pCurParentView = view;
    ms_rentinaValue = rentinaValue;
}

+(void)updateRentinaValue:(float)rentinaValue {
    ms_rentinaValue = rentinaValue;
}

-(id)initDefault
{
    m_pView = ms_pCurParentView;
    m_renderTargetType = RT_LAYER;
    [self initInternal];
    return self;
}

-(id)initDefaultWithType:(RenderTargetType)renderTargetType {
    m_pView = ms_pCurParentView;
    m_renderTargetType = renderTargetType;
    [self initInternal];
    return self;
}


-(id)initWithParentView:(GLKView*) view withRenderTargetType:(RenderTargetType) renderTargetType {
    self->m_pView = view;
    self->m_renderTargetType = renderTargetType;
    [self initInternal];
    
    return self;
}

-(void)initInternal
{
    self->m_loopObserver = nullptr;
    m_isLoop = false;
    
    self->m_playerItem = nullptr;
    self->m_player = [[AVPlayer alloc] init];
    
    self->m_x = 0;
    self->m_y = 0;
    self->m_width  = 0;
    self->m_height = 0;
    self->m_isEnded = false;
    self->m_isMuted = false;
    self->m_volume = [self->m_player volume];
    self->_autoplay = false;
    
    [self performSelectorOnMainThread:@selector(_updateSize) withObject:nil waitUntilDone:NO];
    
    if(self->m_renderTargetType == RT_LAYER) {
        self->m_renderTarget = [[PlayerLayerRenderTarget alloc] initWithPlayer:self->m_player];
        [self->m_pView.layer addSublayer:[self->m_renderTarget getLayer]];
    }
    else if(self->m_renderTargetType == RT_VIDEOOUTPUT) {
        self->m_renderTarget = [[VideoOutputRenderTarget alloc] init];
    }
    
    self->m_videoSize = CGSizeZero;
}

-(void)dealloc {
    [self->m_playerItem removeObserver:self forKeyPath:@"status"];
    [NSNotificationCenter.defaultCenter removeObserver:self name:AVPlayerItemDidPlayToEndTimeNotification object:[self->m_player currentItem]];
    
    if(self->m_loopObserver)
    {
        [self->m_player removeTimeObserver:self->m_loopObserver];
        self->m_loopObserver = nullptr;
    }

    if(m_renderTarget != nullptr)
        m_renderTarget = nullptr;
    
    if(self->m_player)
    {
        self->m_player = nullptr;
    }
}

-(void)load:(const char *)url
{
    [self pause];
    
    NSString *newUrlStr = [NSString stringWithUTF8String:url];
    if(m_playerItem && [m_curUrlStr isEqualToString:newUrlStr])
    {
        return;
    }
    
    self->m_curUrlStr = newUrlStr;
    self->m_readyState = HAVE_NOTHING;
    
    NSURL *newUrl = [NSURL URLWithString:self->m_curUrlStr];
 //   NSURL *newUrl = [NSURL fileURLWithPath:self->m_curUrlStr];
    self->m_playerItem = [AVPlayerItem playerItemWithURL:newUrl];
    [self->m_playerItem addObserver:self forKeyPath:@"status" options:NSKeyValueObservingOptionNew context:nil];
    
    if(self->m_renderTargetType == RT_VIDEOOUTPUT) {
        [self->m_playerItem addOutput:[self->m_renderTarget getVideoOutput]];
    }
    
    [NSNotificationCenter.defaultCenter removeObserver:self name:AVPlayerItemDidPlayToEndTimeNotification object:[self->m_player currentItem]];
    
    [self->m_player replaceCurrentItemWithPlayerItem:self->m_playerItem];
    [NSNotificationCenter.defaultCenter addObserver:self selector:@selector(onVideoEnded:) name:AVPlayerItemDidPlayToEndTimeNotification object:[self->m_player currentItem]];
}

-(void)setEmit:(std::function<void(const char*)>) emitFunc {
    self->m_emitFunc = emitFunc;
}

-(void)play {
    if(self.readyState == HAVE_NOTHING)
    {
        return;
    }
    
    if(m_isEnded)
    {
        self->m_isEnded = false;
        [self setCurrentTime:0];
    }
    
    [self->m_player play];
}

-(void)pause {
    if(self.readyState == HAVE_NOTHING)
    {
        return;
    }
    
    [self->m_player pause];
}

-(float)getVideoWidth
{
    return self->m_videoSize.width;
}

-(float)getVideoHeight
{
    return self->m_videoSize.height;
}

-(void)setX:(double)val {
    self->m_x = val;
    if(self.readyState == HAVE_NOTHING)
        return;
    [self performSelectorOnMainThread:@selector(_updateSize) withObject:nil waitUntilDone:NO];
}

-(void)setY:(double)val {
    self->m_y = val;
    if(self.readyState == HAVE_NOTHING)
        return;
    [self performSelectorOnMainThread:@selector(_updateSize) withObject:nil waitUntilDone:NO];
}

-(void)setWidth:(double)val {
    self->m_width = val;
    if(self.readyState == HAVE_NOTHING)
        return;
    [self performSelectorOnMainThread:@selector(_updateSize) withObject:nil waitUntilDone:NO];
}

-(CGFloat)getWidth {
    return self->m_width;
}

-(void)setHeight:(double)val {
    self->m_height = val;
    if(self.readyState == HAVE_NOTHING)
        return;
    [self performSelectorOnMainThread:@selector(_updateSize) withObject:nil waitUntilDone:NO];
}

-(CGFloat)getHeight {
    return self->m_height;
}

-(bool)isFrameUpdate {
    return [self->m_renderTarget isFrameUpdate];
}

-(IImageContainer*) getImage {
    CMTime curTime = [self->m_player currentTime];
    [self->m_renderTarget updateImageByTime:curTime];
    return [self->m_renderTarget getImageContainer];
}

-(void)_updateSize {
 //   self->m_playerLayer.frame = CGRectMake(self->m_x, self->m_y, self->m_width, self->m_height);
    if(self->m_width == 0 || self->m_height == 0)
        return;
    [self->m_renderTarget setBounds:self->m_x withY:self->m_y withWidth:self->m_width withHeight:self->m_height withRentinaValue: ms_rentinaValue];
}

-(float)currentTime
{
    CMTime currentTime = [self->m_player currentTime];
    return CMTimeGetSeconds(currentTime);
}

-(void)setCurrentTime:(float)val {
    CMTime time = CMTimeMakeWithSeconds(val, 1 * NSEC_PER_SEC);
    [self->m_player seekToTime:time];
}

-(float)duration {
    CMTime duration = self->m_player.currentItem.asset.duration;
    return CMTimeGetSeconds(duration);
}

-(unsigned short)getReadyState {
    return self->m_readyState;
}

-(bool)isPaused {
    float rate = [self->m_player rate];
    return rate == 0;
}

-(float)getVolume {
    return [self->m_player volume];
}

-(void)setVolume:(float)val {
    m_volume = (val < 0.0 ? 0.0 : (val > 1.0 ? 1.0 : val));
    if(m_isMuted)
        return;
    
    self->m_player.volume = val;
}

-(void)setLoop:(bool)val {
    m_isLoop = val;
}

-(bool)getLoop {
    return m_isLoop;
}

-(void)setMuted:(bool) val {
   if(m_isMuted == val)
       return;
    
    m_isMuted = val;
    float volume = m_isMuted ? 0.0 : m_volume;
    [self setVolume:volume];
}

-(bool)getMuted {
    return m_isMuted;
}

-(void)onVideoEnded:(NSNotification*) notification {
    if(m_isEnded)
        return;
    if(m_emitFunc)
        m_emitFunc("ended");
    m_isEnded = true;
    if(m_isLoop)
    {
        [self play];
    }
}


-(void) observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary<NSKeyValueChangeKey,id> *)change context:(void *)context
{
    if(object == self->m_playerItem)
    {
        if(self->m_readyState == HAVE_ENOUGH_DATA)
            return;
        
        if([keyPath isEqualToString:@"status"])
        {
            if(self->m_playerItem.status == AVPlayerItemStatusReadyToPlay)
            {
                for(AVPlayerItemTrack* track in self->m_playerItem.tracks)
                {
                    if([track.assetTrack.mediaType isEqualToString: AVMediaTypeVideo])
                    {
                        self->m_videoSize = track.assetTrack.naturalSize;
                        break;
                    }
                }
                self->m_readyState = HAVE_ENOUGH_DATA;
                
                if(m_emitFunc)
                {
                    m_emitFunc("loadedmetadata");
                    m_emitFunc("canplay");
                }
                
                if(self->_autoplay)
                    [self play];
                
            }
            else if(self->m_playerItem.status == AVPlayerItemStatusFailed)
            {
                NSLog(@"PlayerItem has error : %@", self->m_playerItem.error);
                m_emitFunc("error");
            }
        }
    }
    else
    {
        [super observeValueForKeyPath:keyPath ofObject:object change:change context:context];
    }
}

@end
