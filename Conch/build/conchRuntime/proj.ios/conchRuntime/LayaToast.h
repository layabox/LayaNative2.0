/**
 @file			LayaToast.h
 @brief         Toast
 @author		James
 @version		1.0
 @date			2015_9_21
 @company       LayaBox
 */

#import <Foundation/Foundation.h>
#import "UIKit/UIKit.h"

#define DEFAULT_DISPLAY_DURATION 2.0f

@interface LayaToast : NSObject
{
    NSString* m_sText;
    UIButton* m_pContentView;
    CGFloat   m_nDuration;
}

- (id)initWithText:(NSString *)text_;
- (void)setDuration:(CGFloat) duration_;

- (void)dismisToast;
- (void)toastTaped:(UIButton *)sender_;

- (void)showAnimation;
- (void)hideAnimation;

- (void)show;

+ (void)showWithText:(NSString *) text_;

@end
