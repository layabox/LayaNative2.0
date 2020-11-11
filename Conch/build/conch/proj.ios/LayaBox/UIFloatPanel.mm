
#import "UIFloatPanel.h"
#import <objc/runtime.h>
#import "conchRuntime.h"
#import "conchConfig.h"
#import <UIKit/UIAlertController.h>
#import "AppDelegate.h"
#import "ScanViewController.h"
#import "NavViewController.h"

@implementation UIFloatPanel

-(id)initWithParentView:(GLKView*)pGLView
{
    m_pGLView = pGLView;
    UIImage* m_pImagePanel = [self imagesNamedFromCustomBundle:@"panel.png"];
    self = [super initWithFrame:CGRectMake(0,0,m_pImagePanel.size.width,m_pImagePanel.size.height)];
    if( self != nil )
    {
        self.userInteractionEnabled = TRUE;
        [self setImage:m_pImagePanel];
        [m_pGLView addSubview:self];
        [self resetPosition];
        
        UIImage* pImage = [self imagesNamedFromCustomBundle:@"btn_refresh_normal.png"];
        
        float offset = m_pImagePanel.size.width * 0.15f;
        float distance = (m_pImagePanel.size.width - 2 * offset) * 0.5f;
        
        m_pRefreshBtn = [[UIButton alloc] initWithFrame:CGRectMake(0, 0, pImage.size.width, pImage.size.height)];
        [m_pRefreshBtn setImage:[self imagesNamedFromCustomBundle:@"btn_refresh_normal.png"] forState:UIControlStateNormal];
        [m_pRefreshBtn setImage:[self imagesNamedFromCustomBundle:@"btn_refresh_focused.png"] forState:UIControlStateHighlighted];
        [m_pRefreshBtn setImage:[self imagesNamedFromCustomBundle:@"btn_refresh_pressed.png"] forState:UIControlStateSelected];
        m_pRefreshBtn.center = CGPointMake(offset + distance * 0.5f,m_pImagePanel.size.height * 0.5f);
        [m_pRefreshBtn addTarget:self action:@selector(buttonClickRefresh:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:m_pRefreshBtn];
        
        pImage = [self imagesNamedFromCustomBundle:@"btn_back_normal.png"];
        m_pBackBtn = [[UIButton alloc] initWithFrame:CGRectMake(0, 0, pImage.size.width, pImage.size.height)];
        [m_pBackBtn setImage:[self imagesNamedFromCustomBundle:@"btn_back_normal.png"] forState:UIControlStateNormal];
        [m_pBackBtn setImage:[self imagesNamedFromCustomBundle:@"btn_back_focused.png"] forState:UIControlStateHighlighted];
        [m_pBackBtn setImage:[self imagesNamedFromCustomBundle:@"btn_back_pressed.png"] forState:UIControlStateSelected];
        [m_pBackBtn addTarget:self action:@selector(buttonClickBack:) forControlEvents:UIControlEventTouchUpInside];
        m_pBackBtn.center = CGPointMake(offset + distance * 1.5f, m_pImagePanel.size.height * 0.5f);
        [self addSubview:m_pBackBtn];
        
        
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(orientChange:)
                                                     name:UIDeviceOrientationDidChangeNotification
                                                   object:nil];
        
    }
    return self;
    
}
-(void)buttonClickBack:(id)sender
{
    [[conchRuntime GetIOSConchRuntime] runJS:@"history.back()"];
}
-(void)buttonClickRefresh:(id)sender
{
    [[conchRuntime GetIOSConchRuntime] runJS:@"reloadJS(true)"];
}
- (UIImage *)imagesNamedFromCustomBundle:(NSString *)imgName
{
    NSString *bundlePath = [[NSBundle mainBundle].resourcePath stringByAppendingPathComponent:@"libconchRuntime.bundle"];
    NSString *img_path = [bundlePath stringByAppendingPathComponent:imgName];
    return [UIImage imageWithContentsOfFile:img_path];
}
-(void)resetPosition
{
   self.center = CGPointMake([[UIScreen mainScreen] bounds].size.width - 0.5f * self.bounds.size.width - 10, 30);
}
-(void)orientChange:(NSNotification *)aNotification
{
    [self resetPosition];
}
- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIDeviceOrientationDidChangeNotification object:nil];
}
@end

 
