#import "LaunchView.h"

static const NSTimeInterval REFRESH_INTERVAL = 1.0;
static const int LEAST_SHOW_TIME = 2;
@interface LaunchView()
@property (strong, nonatomic) UIViewController* viewController;
@property (assign, nonatomic) NSTimeInterval startTime;
@property (strong, nonatomic) dispatch_source_t timer;
@property (assign, nonatomic) NSInteger index;
@end
@implementation LaunchView
-(id)initWithFrame:(CGRect)frame
{
    self = [super init];
    if( self != nil )
    {
        UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"LaunchScreen" bundle:nil];
        _viewController = [storyboard instantiateViewControllerWithIdentifier:@"LaunchScreen"];
        _view = _viewController.view;
        _view.frame = frame;
        _startTime = [[NSDate date] timeIntervalSince1970];
        _percent = 0;
        _index = 0;
        __weak typeof(self) weakSelf = self;
        dispatch_queue_t queue = dispatch_get_main_queue();
        _timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, queue);
        dispatch_source_set_timer(_timer, dispatch_walltime(NULL, 0), REFRESH_INTERVAL * NSEC_PER_SEC, 0);
        dispatch_source_set_event_handler(_timer, ^{
            NSInteger length = weakSelf.tips != nil ? weakSelf.tips.count : 0;
            if (length > 0) {
                if (weakSelf.index >= length) {
                    weakSelf.index = 0;
                }
                for (UIView* subView in weakSelf.view.subviews) {
                    if ([subView isKindOfClass:[UILabel class]]) {
                        UILabel* label = (UILabel*)subView;
                        label.text = [NSString stringWithFormat:@"%@(%ld%%)", [weakSelf.tips objectAtIndex:weakSelf.index], (long)weakSelf.percent];
                    }
                }
                weakSelf.index++;
            }
        });
        dispatch_resume(_timer);
            
        return self;
    }
    return nil;
}
-(void)hide
{
    NSTimeInterval showTime = [[NSDate date] timeIntervalSince1970] - _startTime;
    if (showTime >= LEAST_SHOW_TIME)
    {
        [_viewController.view removeFromSuperview];
        dispatch_source_cancel(_timer);
    }
    else
    {
        NSTimeInterval timeLeft = LEAST_SHOW_TIME - showTime;
        __weak typeof(self) weakSelf = self;
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(timeLeft * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            [weakSelf.viewController.view removeFromSuperview];
            dispatch_source_cancel(weakSelf.timer);
        });
    }
}
+ (UIColor *)colorWithHexString:(NSString*)color
{
    NSString *cString = [[color stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]] uppercaseString];
    if ([cString length] < 6) {
        return [UIColor clearColor];
    }
    if ([cString hasPrefix:@"0X"])
    cString = [cString substringFromIndex:2];
    if ([cString hasPrefix:@"#"])
    cString = [cString substringFromIndex:1];
    if ([cString length] != 6)
        return [UIColor clearColor];
    
    NSRange range;
    range.location = 0;
    range.length = 2;
    
    NSString *rString = [cString substringWithRange:range];
    
    range.location = 2;
    NSString *gString = [cString substringWithRange:range];
    
    range.location = 4;
    NSString *bString = [cString substringWithRange:range];

    unsigned int r, g, b;
    [[NSScanner scannerWithString:rString] scanHexInt:&r];
    [[NSScanner scannerWithString:gString] scanHexInt:&g];
    [[NSScanner scannerWithString:bString] scanHexInt:&b];
    
    return [UIColor colorWithRed:((float) r / 255.0f) green:((float) g / 255.0f) blue:((float) b / 255.0f) alpha:1.0f];
}
-(void)setFontColor:(NSString*)color
{
    for (UIView* subView in self.view.subviews) {
        if ([subView isKindOfClass:[UILabel class]]) {
            UILabel* label = (UILabel*)subView;
            label.textColor = [LaunchView colorWithHexString:color];
        }
    }
}
-(void)setPercent:(NSInteger)percent
{
    _percent = percent;
    if (_percent > 100) {
        _percent = 100;
    }
    if (_percent < 0) {
        _percent = 0;
    }
    NSInteger length = self.tips != nil ? self.tips.count : 0;
    if (length > 0) {
        if (self.index >= length) {
            self.index = 0;
        }
        for (UIView* subView in self.view.subviews) {
            if ([subView isKindOfClass:[UILabel class]]) {
                UILabel* label = (UILabel*)subView;
                label.text = [NSString stringWithFormat:@"%@(%ld%%)", [self.tips objectAtIndex:self.index], (long)self.percent];
            }
        }
    }
	if (_percent == 100) {
        [self hide];
    }
}
-(void)setBackgroundColor:(NSString*)color
{
    _view.backgroundColor = [LaunchView colorWithHexString:color];
}
-(void)showTextInfo:(Boolean)show
{
    for (UIView* subView in self.view.subviews) {
        if ([subView isKindOfClass:[UILabel class]]) {
            UILabel* label = (UILabel*)subView;
            label.hidden = !show;
        }
    }
}
@end
