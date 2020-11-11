//
//  LayaWKWebview.h
//  LayaWKWebview
//
//  Created by helloworldlv on 2018/3/23.
//  Copyright © 2018年 render. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <WebKit/WebKit.h>
#import <WebKit/WKWebview.h>
#import <WebKit/WKUserContentController.h>
#import <WebKit/WKUserScript.h>

@interface LayaWKWebview : NSObject
@property(nonatomic, strong) WKWebView* webview;
/*接管下载，支持DCC*/
- (instancetype)initWithWebview:(WKWebView*)webview url:(NSString*)url hostPort:(int)port;
/*直接调WKWebview，不支持DCC*/
- (instancetype)initWithWebview:(WKWebView*)webview url:(NSString*)url;
- (void)loadUrl:(NSString*)url;
- (NSString*)callMethod:(int)objid className:(NSString*)cls methodName:(NSString*)method param:(NSString*)param;
-(void)callbackToJSWithClass:(Class)cls methodName:(NSString*)name ret:(NSObject*)retObj;
-(void)callbackToJSWithClassName:(NSString*)cls methodName:(NSString*)name ret:(NSObject*)retObj;
-(void)callbackToJSWithObject:(id)obj methodName:(NSString*)name ret:(NSObject*)retObj;
-(void)runJS:(NSString*)script;
+(LayaWKWebview*)GetLayaWKWebview;
@end
