#import "ViewController.h"
#import "UIFloatPanel.h"

@implementation ViewController

static ViewController* g_pIOSMainViewController = nil;
//------------------------------------------------------------------------------
+(ViewController*)GetIOSViewController
{
    return g_pIOSMainViewController;
}
//------------------------------------------------------------------------------
-(id)init
{
    self = [super init];
    if( self != nil )
    {
        g_pIOSMainViewController = self;
        return self;
    }
    return Nil;
}
//------------------------------------------------------------------------------
- (void)viewDidLoad
{
    [super viewDidLoad];
    //保持屏幕常亮，可以通过脚本设置
    [[UIApplication sharedApplication] setIdleTimerDisabled:YES];
    self->m_pGLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    if (self->m_pGLContext)
    {
        NSLog(@"iOS OpenGL ES 3.0 context created");
    }
    else
    {
        self->m_pGLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        if (self->m_pGLContext)
        {
            NSLog(@"iOS OpenGL ES 2.0 context created");
        }
        else
        {
            NSLog(@"iOS OpenGL ES 2.0 context created failed");
        }
    }
    m_pGLKView = (GLKView *)self.view;
    m_pGLKView.context = self->m_pGLContext;
    m_pGLKView.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    m_pGLKView.drawableStencilFormat = GLKViewDrawableStencilFormat8;
    [EAGLContext setCurrentContext:self->m_pGLContext];
    self.preferredFramesPerSecond = 10000;
    
    //conchRuntime 初始化ConchRuntime引擎
    m_pConchRuntime = [[conchRuntime alloc]initWithView:m_pGLKView EAGLContext:m_pGLContext downloadThreadNum:3];
    [self prefersStatusBarHidden];
    [self setNeedsStatusBarAppearanceUpdate];

    
    //在主线程开这个DisplayLink会导致卡死。因为gl等待会导致事件函数无法调到。
    m_displayLink=nullptr;
    //[self startDisplayLink];
    
    m_floatPanel = [[UIFloatPanel alloc] initWithParentView:m_pGLKView];
    m_floatPanel.hidden = true;
}

- (void)startDisplayLink
{
    m_displayLink = [CADisplayLink displayLinkWithTarget:self
                                                   selector:@selector(handleDisplayLink:)];
    [m_displayLink addToRunLoop:[NSRunLoop currentRunLoop]
                           forMode:NSDefaultRunLoopMode];
}

- (void)handleDisplayLink:(CADisplayLink *)displayLink
{
    if( m_pConchRuntime){
        [m_pConchRuntime onVsync:displayLink.timestamp*1000.0];
    }
}

- (void)stopDisplayLink
{
    if(m_displayLink)
        [m_displayLink invalidate];
    m_displayLink = nil;
}


//------------------------------------------------------------------------------
- (void)dealloc
{
    [m_pConchRuntime destory];
    [self tearDownGL];
    if ( [EAGLContext currentContext] == self->m_pGLContext )
    {
        [EAGLContext setCurrentContext:nil];
    }
    [self stopDisplayLink];
    m_floatPanel = nil;
}
//------------------------------------------------------------------------------
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    
    //conchRuntime 内存警告的时候的处理
    [m_pConchRuntime didReceiveMemoryWarning];
}
//------------------------------------------------------------------------------
- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self->m_pGLContext];
}
//------------------------------------------------------------------------------
- (void)update
{
    
}
//------------------------------------------------------------------------------
- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    //conchRuntime renderFrame
    [m_pConchRuntime renderFrame];
}
//-------------------------------------------------------------------------------
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    //conchRuntime touch
    [m_pConchRuntime touchesBegan:touches withEvent:event];
}
//-------------------------------------------------------------------------------
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    //conchRuntime touch
    [m_pConchRuntime touchesMoved:touches withEvent:event];
}
//-------------------------------------------------------------------------------
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    //conchRuntime touch
    [m_pConchRuntime touchesEnded:touches withEvent:event];
}
//-------------------------------------------------------------------------------
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    //conchRuntime touch
    [m_pConchRuntime touchesCancelled:touches withEvent:event];
}
//-------------------------------------------------------------------------------
-(NSUInteger)supportedInterfaceOrientations
{
    /*
     UIInterfaceOrientationMaskPortrait,             ===2
     UIInterfaceOrientationMaskPortraitUpsideDown,   ===4
     UIInterfaceOrientationMaskLandscapeLeft,        ===8
     UIInterfaceOrientationMaskLandscapeRight,       ===16
     */
    return [conchConfig GetInstance]->m_nOrientationType;
}
//-------------------------------------------------------------------------------
- (BOOL)shouldAutorotate
{
    return YES;//支持转屏
}
- (BOOL)prefersStatusBarHidden
{
    return YES;//隐藏为YES，显示为NO
}
@end
