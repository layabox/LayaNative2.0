//
//  PlayerLayerRenderTarget.h
//  conchRuntime
//
//  Created by LayaBox on 2018/11/13.
//  Copyright © 2018 LayaBox. All rights reserved.
//

#import "IVideoRenderTarget.h"

@interface PlayerLayerRenderTarget : IVideoRenderTarget
{
    AVPlayerLayer* m_playerLayer;
}

-(void)dealloc;

-(PlayerLayerRenderTarget*)initWithPlayer:(AVPlayer*)player;

@end
