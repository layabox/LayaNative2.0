#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface LaunchView : NSObject
-(id)initWithFrame:(CGRect)frame;
-(void)hide;
-(void)setFontColor:(NSString*)color;
-(void)setBackgroundColor:(NSString*)color;
-(void)showTextInfo:(Boolean)show;
@property (strong, nonatomic) UIView* view;
@property (strong, nonatomic) NSArray* tips;
@property (assign, nonatomic) NSInteger percent;
@end
