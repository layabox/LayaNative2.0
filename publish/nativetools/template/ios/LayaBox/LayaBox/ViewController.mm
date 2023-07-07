#import "ViewController.h"
#import <CoreTelephony/CTCellularData.h>
#import "Reachability/Reachability.h"

@implementation ViewController
{
    CTCellularData *_cellularData;
    LayaReachability *_pNetworkListener;
    bool _isInit;
}
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
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(networkStateChange) name:LayakReachabilityChangedNotification object:nil];
        _pNetworkListener = [LayaReachability reachabilityForInternetConnection];
        [_pNetworkListener startNotifier];
        _isInit = false;
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
    
    
    _cellularData = [[CTCellularData alloc] init];
    //选择仅无线网络时也返回kCTCellularDataRestricted，因此需要Reachability协助判断
    if (_cellularData.restrictedState == kCTCellularDataNotRestricted || _pNetworkListener.currentReachabilityStatus != NotReachable) {
        [self initConch];
    } else {
        __weak ViewController* weakSelf = self;
        [self networkAuthorizationAvalible:^{
            ViewController *strongSelf = weakSelf;
            dispatch_async(dispatch_get_main_queue(), ^{
                [strongSelf initConch];
            });
        }];
    }
}
//------------------------------------------------------------------------------
- (void)dealloc
{
    [self tearDownGL];
    if ( [EAGLContext currentContext] == self->m_pGLContext )
    {
        [EAGLContext setCurrentContext:nil];
    }
    [[NSNotificationCenter defaultCenter] removeObserver:self];
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
- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator
{
    [super viewWillTransitionToSize:size withTransitionCoordinator:coordinator];
    [m_pConchRuntime updateCanvasSize:size];
}
- (void)initConch
{
    if (_isInit)
        return;
    _isInit = true;
    
    //conchRuntime 初始化ConchRuntime引擎
    m_pConchRuntime = [[conchRuntime alloc]initWithView:m_pGLKView EAGLContext:m_pGLContext downloadThreadNum:3];
}
- (void)networkAuthorizationAvalible:(void(^)())changeAvaliable
{
    _cellularData.cellularDataRestrictionDidUpdateNotifier = ^(CTCellularDataRestrictedState state) {
        switch (state) {
            case kCTCellularDataRestricted:
                break;
            case kCTCellularDataNotRestricted:
                changeAvaliable();
                break;
                //未知，第一次请求
            case kCTCellularDataRestrictedStateUnknown:
                break;
            default:
                break;
        };
    };
}
- (void)networkStateChange
{
    LayaNetworkStatus networkStatus = _pNetworkListener.currentReachabilityStatus;
    if (networkStatus != NotReachable) {
        [self initConch];
    }
}
@end
