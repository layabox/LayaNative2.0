#import "ViewController.h"
#import "conchConfig.h"
#import "LayaWKWebview.h"

@implementation ViewController
{
     LayaWKWebview*          _layaWKWebview;
     NSString* _scriptsPath;
     BOOL _addScriptMessageHandler;
}
-(id)init
{
    self = [super init];
    if( self != nil )
    {
        _addScriptMessageHandler = NO;
        return self;
    }
    return nil;
}
- (void)viewDidLoad
{
    [super viewDidLoad];
	[[UIApplication sharedApplication] setIdleTimerDisabled:YES];
    [conchConfig GetInstance];
    WKWebViewConfiguration* config = [[WKWebViewConfiguration alloc]init];
    config.preferences.javaScriptEnabled=true;
    config.preferences.javaScriptCanOpenWindowsAutomatically = true;
    config.userContentController = [[WKUserContentController alloc] init];
    
    NSString* scriptsPath = [[[NSBundle mainBundle] resourcePath] stringByAppendingString:@"/scripts/init.js"];
    NSString* iniScript = [NSString stringWithContentsOfFile:scriptsPath encoding:NSUTF8StringEncoding error:nil];
    WKUserScript *script = [[WKUserScript alloc] initWithSource:iniScript injectionTime:WKUserScriptInjectionTimeAtDocumentStart forMainFrameOnly:YES];
    [config.userContentController addUserScript:script];
    
    WKWebView* webview = [[WKWebView alloc] initWithFrame:self.view.bounds configuration:config];
    webview.allowsBackForwardNavigationGestures = YES;
    webview.UIDelegate = self;
    webview.navigationDelegate = self;
    
    webview.scrollView.bounces = false;
    webview.scrollView.bouncesZoom = false;
    
	if (@available(iOS 11.0, *)) 
	{
		webview.scrollView.contentInsetAdjustmentBehavior = UIScrollViewContentInsetAdjustmentNever;
    }
		
    [self setNeedsStatusBarAppearanceUpdate];
    [self prefersStatusBarHidden];
    
    _layaWKWebview = [[LayaWKWebview alloc] initWithWebview:webview url:[conchConfig GetInstance]->m_sUrl hostPort:[conchConfig GetInstance]->m_nHostPort];
    [self.view addSubview:_layaWKWebview.webview];
}
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
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
- (BOOL)prefersStatusBarHidden
{
    return YES;
}
- (BOOL)shouldAutorotate
{
    return YES;//支持转屏
}
- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    if (!_addScriptMessageHandler)
    {
        [_layaWKWebview.webview.configuration.userContentController addScriptMessageHandler:self name:@"reflection"];
        [_layaWKWebview.webview.configuration.userContentController addScriptMessageHandler:self name:@"log"];
        _addScriptMessageHandler = YES;
    }
}
- (void)viewWillDisppear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    if (_addScriptMessageHandler)
    {
        [_layaWKWebview.webview.configuration.userContentController removeScriptMessageHandlerForName:@"reflection"];
        [_layaWKWebview.webview.configuration.userContentController removeScriptMessageHandlerForName:@"log"];
        _addScriptMessageHandler = NO;
    }
}
- (void)userContentController:(WKUserContentController *)userContentController didReceiveScriptMessage:(WKScriptMessage *)message
{
    if (![message.body isKindOfClass:NSString.class])
    {
        return;
    }
    NSString* data = message.body;
    if (data == nil)
    {
        return;
    }
    
    //NSLog(@"JS-->OC  %@",message.body);
    
    if ([message.name isEqualToString:@"log"])
    {
           NSLog(@"JSLog:%@", data);
    }
    else if ([message.name isEqualToString:@"reflection"])
    {
        NSError* error = nil;
        NSData* jsonData = [message.body dataUsingEncoding:NSUTF8StringEncoding];
        NSDictionary* dict = [NSJSONSerialization JSONObjectWithData:jsonData options:NSJSONReadingMutableContainers error:&error];
        NSNumber* ID = [dict objectForKey:@"id"];
        NSString* className = [dict objectForKey:@"className"];
        NSString* methodName = [dict objectForKey:@"methodName"];
        NSString* param = [dict objectForKey:@"param"];
        [_layaWKWebview callMethod:ID.intValue className:className methodName:methodName param:param];
    }
}
-(void) callbackToJS:(NSString*)name data:(NSString*)jsonParam
{
    NSString* js = [NSString stringWithFormat:@"window.wkbridge.callback('%@','%@');",name, jsonParam];
    [_layaWKWebview.webview evaluateJavaScript:js completionHandler:^(id _Nullable response, NSError * _Nullable error) {
        if (response || error)
        {
            NSLog(@"value: %@ error: %@", response, error);
        }
    }];
}
/*- (void)webView:(WKWebView *)webView decidePolicyForNavigationAction:(WKNavigationAction *)navigationAction decisionHandler:(void (^)(WKNavigationActionPolicy))decisionHandler
{
    NSDictionary* pDictionary =
    [NSDictionary dictionaryWithObjectsAndKeys:
    [conchConfig GetInstance]->m_sPlatformClassName,@"market_name",
    [NSNumber numberWithInt:[conchConfig GetInstance]->m_nChargeType],@"charge_type",
    [NSNumber numberWithInt:[conchConfig GetInstance]->m_nPayType],@"pay_type",
    [NSNumber numberWithInt:[conchConfig GetInstance]->m_nEnterPlatformType],@"enter_platform_type",
    [NSNumber numberWithInt:[conchConfig GetInstance]->m_nLoginType],@"login_type",nil];
    NSError* pError = nil;
    NSString* pJsonString = nil;
    NSData* pJsonData = [NSJSONSerialization dataWithJSONObject:pDictionary options:0 error:&pError];
    if( !pError )
    {
        pJsonString = [[NSString alloc] initWithData:pJsonData encoding:NSUTF8StringEncoding];
    }

    NSString* js = [NSString stringWithFormat:@"if(!window.conchMarketData){window.conchMarketData=JSON.parse('%@')}",pJsonString];
    [_layaWKWebview.webview evaluateJavaScript:js completionHandler:^(id _Nullable response, NSError * _Nullable error) {
        if (response || error)
        {
            NSLog(@"value: %@ error: %@;", response, error);
        }
    }];
    
    decisionHandler(WKNavigationActionPolicyAllow);
}*/
- (void)webView:(WKWebView *)webView runJavaScriptAlertPanelWithMessage:(NSString *)message initiatedByFrame:(WKFrameInfo *)frame completionHandler:(void (^)(void))completionHandler {
    // 确定按钮
    UIAlertAction *alertAction = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleCancel handler:^(UIAlertAction * _Nonnull action) {
        completionHandler();
    }];
    // alert弹出框
    UIAlertController *alertController = [UIAlertController alertControllerWithTitle:message message:nil preferredStyle:UIAlertControllerStyleAlert];
    [alertController addAction:alertAction];
    [self presentViewController:alertController animated:YES completion:nil];
}
@end
