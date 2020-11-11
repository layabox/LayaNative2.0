//
//  PlayerLayerRenderTarget.m
//  conchRuntime
//
//  Created by LayaBox on 2018/11/13.
//  Copyright © 2018 LayaBox. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "PlayerLayerRenderTarget.h"

@implementation PlayerLayerRenderTarget

-(void)dealloc {
    [self->m_playerLayer removeFromSuperlayer];
    self->m_playerLayer = nil;
}

-(PlayerLayerRenderTarget*)initWithPlayer:(AVPlayer*)player {
    self = [super init];
    if(!self)
        return nil;
    
    m_playerLayer = [AVPlayerLayer playerLayerWithPlayer:player];
    
    return self;
}

-(CALayer*) getLayer {
    return m_playerLayer;
}


-(RenderTargetType) getType {
    return RT_LAYER;
}

-(void)setBounds:(CGFloat)x withY:(CGFloat)y withWidth:(CGFloat) width
      withHeight:(CGFloat)height withRentinaValue:(CGFloat) rentinaValue {
    CGRect frame = self->m_playerLayer.frame;
    frame.origin.x    = x / rentinaValue;
    frame.origin.y    = y / rentinaValue;
    frame.size.width  = width / rentinaValue;
    frame.size.height = height / rentinaValue;
    self->m_playerLayer.frame = frame;

}

@end
