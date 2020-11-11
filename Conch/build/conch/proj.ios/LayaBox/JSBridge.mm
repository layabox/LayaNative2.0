#import "JSBridge.h"
#import "AppDelegate.h"
#import "ViewController.h"
#import "UIFloatPanel.h"
#import "ScanViewController.h"
#import <UIKit/UINavigationController.h>
#import "NavViewController.h"

@implementation JSBridge

+(void)hideSplash
{
    dispatch_async(dispatch_get_main_queue(), ^{
        AppDelegate * appDelegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
        [appDelegate.launchView hide];
    });
}
+(void)setTips:(NSArray*)tips
{
    dispatch_async(dispatch_get_main_queue(), ^{
        AppDelegate * appDelegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
        appDelegate.launchView.tips = tips;
    });
}
+(void)setFontColor:(NSString*)color
{
    dispatch_async(dispatch_get_main_queue(), ^{
        AppDelegate * appDelegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
        [appDelegate.launchView setFontColor:color];
   });
}
+(void)bgColor:(NSString*)color
{
    dispatch_async(dispatch_get_main_queue(), ^{
        AppDelegate * appDelegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
        [appDelegate.launchView setBackgroundColor:color];
    });
}
+(void)loading:(NSNumber*)percent
{
    dispatch_async(dispatch_get_main_queue(), ^{
        AppDelegate * appDelegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
        [appDelegate.launchView setPercent:percent.integerValue];
    });
}
+(void)showTextInfo:(NSNumber*)show
{
    dispatch_async(dispatch_get_main_queue(), ^{
        AppDelegate * appDelegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
        [appDelegate.launchView showTextInfo:show.intValue > 0];
    });
}
+(void)showScanner:(NSNumber*)show
{
    dispatch_async(dispatch_get_main_queue(), ^{
        
        AppDelegate * appDelegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
        if (show.intValue > 0)
        {
            [appDelegate.navigationController presentViewController:[[ScanViewController alloc] init] animated:NO completion:nil];
        }
        else
        {
            
        }
    });
}
+(void)onScanResult:(NSString*)result
{
    NSString* js = [[NSString alloc] initWithFormat:@"script.UIController.instance.onScanResult('%@')", result];
    [[conchRuntime GetIOSConchRuntime] runJS:js];
}
+(void)showFloatPanel:(NSNumber*)show
{
    dispatch_async(dispatch_get_main_queue(), ^{
        if (show.intValue > 0)
        {
            [ViewController GetIOSViewController]->m_floatPanel.hidden = false;
        }
        else
        {
            [ViewController GetIOSViewController]->m_floatPanel.hidden = true;
        }
    });
}
@end

