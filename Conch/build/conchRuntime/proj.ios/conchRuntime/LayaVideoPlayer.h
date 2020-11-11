#import <AVFoundation/AVFoundation.h>
#import <GLKit/GLKit.h>
#import <functional>
#import "Video/IVideoRenderTarget.h"

const unsigned short HAVE_NOTHING = 0;
const unsigned short HAVE_METADATA = 1;
const unsigned short HAVE_CURRENT_DATA = 2;
const unsigned short HAVE_FUTURE_DATA = 3;
const unsigned short HAVE_ENOUGH_DATA = 4;

@interface LayaVideoPlayer : NSObject
{
    AVPlayer* m_player;
    AVPlayerItem *m_playerItem;
    GLKView* m_pView;
    
    NSString* m_curUrlStr;
    std::function<void(const char*)> m_emitFunc;
    
    CGSize m_videoSize;
    
    unsigned short m_readyState;
    
    CGFloat m_width;
    CGFloat m_height;
    CGFloat m_x;
    CGFloat m_y;
    
    id m_loopObserver;
    bool m_isLoop;
    
    bool m_isEnded;
    bool m_isMuted;
    
    float m_volume;
    
    IVideoRenderTarget* m_renderTarget;
    RenderTargetType m_renderTargetType;
}

@property(nonatomic, getter=getReadyState, readonly) unsigned short readyState;
@property(nonatomic, getter=getWidth, setter=setWidth:) CGFloat width;
@property(nonatomic, getter=getHeight, setter=setHeight:) CGFloat height;
@property(nonatomic, getter=getVolume, setter=setVolume:) float volume;
@property(nonatomic, getter=getLoop, setter=setLoop:) bool loop;
@property(nonatomic, getter=getMuted, setter=setMuted:) bool muted;
@property(nonatomic) bool autoplay;

+(void)setCurParentView:(GLKView*) view withRetianValue: (float) rentinaValue;
+(void)updateRentinaValue:(float)rentinaValue;

-(id)initDefault;
-(id)initDefaultWithType:(RenderTargetType)renderTargetType;
-(id)initWithParentView:(GLKView*) view withRenderTargetType:(RenderTargetType)renderTargetType;
-(void)initInternal;

-(void)load:(const char*) url;
-(void)play;
-(void)pause;
-(void)setEmit:(std::function<void(const char*)>) emitFunc;

-(float) getVideoWidth;
-(float) getVideoHeight;
-(void) setX:(double)val;
-(void) setY:(double)val;
-(void) setWidth:(double)val;
-(CGFloat) getWidth;
-(void) setHeight:(double)val;
-(CGFloat) getHeight;
-(float) currentTime;
-(void) setCurrentTime:(float) val;
-(float) duration;
-(unsigned short)getReadyState;
-(bool)isPaused;
-(float)getVolume;
-(void)setVolume:(float)val;
-(void)setLoop:(bool)val;
-(bool)getLoop;
-(void)setMuted:(bool) val;
-(bool)getMuted;

-(bool)isFrameUpdate;
-(IImageContainer*) getImage;

-(void)onVideoEnded:(NSNotification*) notification;

-(void) observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary<NSKeyValueChangeKey,id> *)change context:(void *)context;

-(void)dealloc;

-(void)_updateSize;
@end
