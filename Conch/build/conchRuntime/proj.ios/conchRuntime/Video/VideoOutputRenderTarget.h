//
//  VideoOutRenderTarget.h
//  conchRuntime
//
//  Created by LayaBox on 2018/11/13.
//  Copyright © 2018 LayaBox. All rights reserved.
//

#import "IVideoRenderTarget.h"

@interface VTImageContainer : IImageContainer

-(VTImageContainer*) init;
-(void) dealloc;


@property(nonatomic) CIImage* image;
@property(nonatomic) CIContext* context;

@property(nonatomic, readonly) CGColorSpaceRef colorSpace;

-(CGRect)getBounds;

@end

@interface VideoOutputRenderTarget : IVideoRenderTarget
{
    AVPlayerItemVideoOutput* m_videoOutput;
    VTImageContainer* m_imageContainer;
}

-(VideoOutputRenderTarget*)init;

@end
