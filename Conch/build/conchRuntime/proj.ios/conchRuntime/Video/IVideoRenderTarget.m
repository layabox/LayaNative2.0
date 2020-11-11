//
//  IVideoRenderTarget.m
//  conchRuntime
//
//  Created by LayaBox on 2018/11/13.
//  Copyright © 2018 LayaBox. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IVideoRenderTarget.h"

@implementation IImageContainer

-(void)copyToBitmap:(char*)imagePtr {
}

@end

@implementation IVideoRenderTarget

-(void)dealloc {
}

-(CALayer*) getLayer {
    return NULL;
}
-(bool)isFrameUpdate {
    return false;
}

-(IImageContainer*) getImageContainer {
    return NULL;
}

-(AVPlayerItemVideoOutput*) getVideoOutput {
    return NULL;
}

-(RenderTargetType) getType {
    return RT_UNKOWN;
}

-(void)updateImageByTime:(CMTime) time {
    return;
}

-(void)setBounds:(CGFloat)x withY:(CGFloat)y withWidth:(CGFloat) width
      withHeight:(CGFloat)height withRentinaValue:(CGFloat) rentinaValue {
}

@end
