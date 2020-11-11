//
//  VideoOutputRenderTarget.m
//  conchRuntime
//
//  Created by LayaBox on 2018/11/13.
//  Copyright © 2018 LayaBox. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VideoOutputRenderTarget.h"
#import <GLKit/GLKit.h>


@implementation VTImageContainer

-(VTImageContainer*)init {
    self = [super init];
    if(!self)
        return nil;
    
    self->_image = nil;
    self.context = [CIContext contextWithOptions: nil];
    self.width = 0;
    self.height = 0;
    
    self->_colorSpace = CGColorSpaceCreateDeviceRGB();
    return self;
}

-(void)dealloc {
    CGColorSpaceRelease(self.colorSpace);
}

-(CGRect)getBounds {
    return CGRectMake(0, 0, self.width, self.height);
}

-(void)copyToBitmap:(char*)imagePtr {
    
    [self.context render:self.image toBitmap:imagePtr rowBytes:self.width * sizeof(int) bounds: [self getBounds] format:kCIFormatRGBA8 colorSpace:self.colorSpace];
}

@end


@implementation VideoOutputRenderTarget

-(VideoOutputRenderTarget*)init {
    self = [super init];
    if(!self)
        return nil;
    
    self->m_videoOutput = [[AVPlayerItemVideoOutput alloc] init];
    self->m_imageContainer = [[VTImageContainer alloc] init];
    
    return self;
}

-(IImageContainer*) getImageContainer {
    return self->m_imageContainer;
}

-(bool)isFrameUpdate {
    return true;
}

-(void)updateImageByTime:(CMTime) time {
    if(![self->m_videoOutput hasNewPixelBufferForItemTime:time])
        return;
    
    CVPixelBufferRef pixelBuffer = [self->m_videoOutput copyPixelBufferForItemTime:time itemTimeForDisplay:nil];
    
    if(pixelBuffer != nil)
    {
        self->m_imageContainer.image = [[CIImage alloc] initWithCVPixelBuffer:pixelBuffer];
        self->m_imageContainer.width = (int)CVPixelBufferGetWidth(pixelBuffer);
        self->m_imageContainer.height = (int)CVPixelBufferGetHeight(pixelBuffer);
        
        CVBufferRelease(pixelBuffer);
    }
}

-(AVPlayerItemVideoOutput*) getVideoOutput {
    return self->m_videoOutput;
}

-(RenderTargetType) getType {
    return RT_VIDEOOUTPUT;
}

@end
