/**
 @file			LayaToast.h
 @brief         Toast
 @author		James
 @version		1.0
 @date			2015_9_21
 @company       LayaBox
 */

#import "LayaToast.h"
#import <QuartzCore/QuartzCore.h>
#import <UIKit/UIStringDrawing.h>
#import "conchRuntime.h"

@implementation LayaToast

- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIDeviceOrientationDidChangeNotification object:[UIDevice currentDevice]];
}
- (id)initWithText:(NSString *)text_
{
    if (self = [super init])
    {
        m_sText = [text_ copy];
        UIFont *font = [UIFont boldSystemFontOfSize:16];
        CGSize textSize = [m_sText sizeWithFont:font constrainedToSize:CGSizeMake(320, MAXFLOAT) ];
        UILabel *textLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, textSize.width + 12, textSize.height + 12)];
        textLabel.backgroundColor = [UIColor clearColor];
        textLabel.textAlignment = NSTextAlignment::NSTextAlignmentCenter;
        textLabel.textColor = [UIColor whiteColor];
        textLabel.font = font;
        textLabel.text = m_sText;
        textLabel.numberOfLines = 0;
        
        m_pContentView = [[UIButton alloc] initWithFrame:CGRectMake(0, 0, textLabel.frame.size.width, textLabel.frame.size.height)];
        m_pContentView.layer.cornerRadius = 5.0f;
        m_pContentView.layer.borderWidth = 1.0f;
        m_pContentView.layer.borderColor = [[UIColor grayColor] colorWithAlphaComponent:0.5].CGColor;
        m_pContentView.backgroundColor = [UIColor colorWithRed:0.2f green:0.2f blue:0.2f alpha:0.5f];
        
        [m_pContentView addSubview:textLabel];
        m_pContentView.autoresizingMask = UIViewAutoresizingFlexibleWidth;
        [m_pContentView addTarget:self action:@selector(toastTaped:) forControlEvents:UIControlEventTouchDown];
        m_pContentView.alpha = 0.0f;
        m_nDuration = DEFAULT_DISPLAY_DURATION;
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(deviceOrientationDidChanged:)
                                                     name:UIDeviceOrientationDidChangeNotification object:[UIDevice currentDevice]];
    }
    return self;
}
- (void)deviceOrientationDidChanged:(NSNotification *)notify_
{
    [self hideAnimation];
}
-(void)dismisToast
{
    [m_pContentView removeFromSuperview];
}
-(void)toastTaped:(UIButton *)sender_
{
    [self hideAnimation];
}
- (void)setDuration:(CGFloat) duration_
{
    m_nDuration = duration_;
}
-(void)showAnimation
{
    [UIView beginAnimations:@"show" context:NULL];
    [UIView setAnimationCurve:UIViewAnimationCurveEaseIn];
    [UIView setAnimationDuration:0.3];
    m_pContentView.alpha = 1.0f;
    [UIView commitAnimations];
}
-(void)hideAnimation
{
    [UIView beginAnimations:@"hide" context:NULL];
    [UIView setAnimationCurve:UIViewAnimationCurveEaseOut];
    [UIView setAnimationDelegate:self];
    [UIView setAnimationDidStopSelector:@selector(dismisToast)];
    [UIView setAnimationDuration:0.3];
    m_pContentView.alpha = 0.0f;
    [UIView commitAnimations];
}
- (void)show
{
    conchRuntime* pConchRuntime = [conchRuntime GetIOSConchRuntime];
    [pConchRuntime->m_pGLKView addSubview:m_pContentView];
    m_pContentView.center = CGPointMake( (pConchRuntime->m_pResolution->x)/2, pConchRuntime->m_pResolution->y - 80 );
    [self showAnimation];
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(m_nDuration * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        [self hideAnimation];
    });
}
+ (void)showWithText:(NSString *)text_
{
    LayaToast* toast = [[LayaToast alloc] initWithText:text_];
    [toast setDuration:DEFAULT_DISPLAY_DURATION];
    [toast show];
}

@end
