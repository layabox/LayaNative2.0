//
//  IVideoRenderTarget.h
//  conchRuntime
//
//  Created by LayaBox on 2018/11/13.
//  Copyright © 2018 LayaBox. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef enum  {
    RT_UNKOWN,
    RT_LAYER,
    RT_VIDEOOUTPUT,
} RenderTargetType;

@interface IImageContainer : NSObject

@property(nonatomic) int width;
@property(nonatomic) int height;

-(void)copyToBitmap:(char*)imagePtr;

@end


@interface IVideoRenderTarget : NSObject

-(void)dealloc;

-(CALayer*) getLayer;
-(AVPlayerItemVideoOutput*) getVideoOutput;
-(bool)isFrameUpdate;
-(void)updateImageByTime:(CMTime) time;
-(IImageContainer*) getImageContainer;
-(RenderTargetType) getType;
-(void)setBounds:(CGFloat)x withY:(CGFloat)y
       withWidth:(CGFloat) width withHeight:(CGFloat)height
       withRentinaValue:(CGFloat) rentinaValue;

@end

NS_ASSUME_NONNULL_END
