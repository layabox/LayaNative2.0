#import <UIKit/UIKit.h>
#import <WebKit/WKUIDelegate.h>
#import <WebKit/WKScriptMessageHandler.h>
#import <WebKit/WKNavigationDelegate.h>
@interface ViewController : UIViewController<WKScriptMessageHandler,WKUIDelegate,WKNavigationDelegate>
-(id)init;
@end

