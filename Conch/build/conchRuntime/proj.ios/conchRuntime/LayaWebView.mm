/**
 @file			UIEditBox.mm
 @brief         继承textField类
 @author		wyw
 @version		1.0
 @date			2012_12_17
 @company       JoyChina
 */

#import "LayaWebView.h"
#import "conchRuntime.h"
#import "../../../../source/conch/JCScriptRuntime.h"
#import "JCThreadCmdMgr.h"
#import "NSString.h"

@implementation LayaWebView

- (UIImage *)imagesNamedFromCustomBundle:(NSString *)imgName
{
    NSString *bundlePath = [[NSBundle mainBundle].resourcePath stringByAppendingPathComponent:@"libconchRuntime.bundle"];
    NSString *img_path = [bundlePath stringByAppendingPathComponent:imgName];
    return [UIImage imageWithContentsOfFile:img_path];
}
-(LayaWebView*)init
{
    m_nX = 0;
    m_nY = 0;
    m_nWidth = [conchRuntime GetIOSConchRuntime]->m_pResolution->x;
    m_nHeight = [conchRuntime GetIOSConchRuntime]->m_pResolution->y;
    
    WKWebViewConfiguration* config = [[WKWebViewConfiguration alloc]init];
    config.preferences.javaScriptEnabled=true;
    config.preferences.javaScriptCanOpenWindowsAutomatically = true;
    config.userContentController = [[WKUserContentController alloc] init];
    
    m_pWebView = [[WKWebView alloc] initWithFrame:CGRectMake( m_nX, m_nY, m_nWidth, m_nHeight) configuration:config];
    m_pWebView.allowsBackForwardNavigationGestures = YES;
    //m_pWebView.UIDelegate = self;
    //m_pWebView.navigationDelegate = self;
    
    m_pWebView.scrollView.bounces = false;
    m_pWebView.scrollView.bouncesZoom = false;
    
    //closeButton
    m_pCloseButton = [UIButton buttonWithType:UIButtonTypeCustom];
    UIImage* pImage = [self imagesNamedFromCustomBundle:@"close.png"];
    [m_pCloseButton setImage:pImage forState:UIControlStateNormal];
    [m_pCloseButton addTarget:self action:@selector(handleCloseButton:) forControlEvents: UIControlEventTouchUpInside];
    [m_pCloseButton setFrame:CGRectMake( 5, 5, 32, 32)];
    [m_pWebView addSubview:m_pCloseButton];
    
    //排版
    [self composing:m_nX posy:m_nY width:m_nWidth height:m_nHeight];
    m_pWebView.hidden = YES;
    return self;
}
//-------------------------------------------------------------------------------
-(void)showWebView:(NSString*)p_sUrl posx:(int)p_nX posy:(int)p_nY width:(int)p_nWidth height:(int)p_nHeight showCloseButton:(bool)p_bShowCloseButton
{
    float fRetinaValue = [conchRuntime GetIOSConchRuntime]->m_fRetinaValue;
    p_nX /= fRetinaValue;
    p_nY /= fRetinaValue;
    p_nWidth /= fRetinaValue;
    p_nHeight /= fRetinaValue;
    if(p_bShowCloseButton)
    {
        m_pCloseButton.hidden = NO;
    }
    else
    {
        m_pCloseButton.hidden = YES;
    }
    if(p_nWidth==0)p_nWidth = [conchRuntime GetIOSConchRuntime]->m_pResolution->x;
    if(p_nHeight==0)p_nHeight = [conchRuntime GetIOSConchRuntime]->m_pResolution->y;
    if( m_nWidth != p_nWidth || m_nHeight != p_nHeight || m_nX != p_nX || m_nY != p_nY )
    {
        m_nX = p_nX;
        m_nY = p_nY;
        m_nWidth = p_nWidth;
        m_nHeight = p_nHeight;
        [self composing:m_nX posy:m_nY width:m_nWidth height:m_nHeight];
        m_pWebView.hidden = NO;
        [self loadUrl:p_sUrl];
    }
    else
    {
        m_pWebView.hidden = NO;
        [self loadUrl:p_sUrl];
    }
}
//-------------------------------------------------------------------------------
-(void)closeWebView
{
    m_pWebView.hidden = YES;
    [m_pWebView loadHTMLString:@"" baseURL:nil];
}
//-------------------------------------------------------------------------------
-(void)composing:(int)p_nX posy:(int)p_nY width:(int)p_nWidth height:(int)p_nHeight
{
    [m_pWebView setFrame:CGRectMake( p_nX,p_nY,p_nWidth,p_nHeight )];
}
//-------------------------------------------------------------------------------
-(void)loadUrl:(NSString*)p_sUrl
{
    NSURL* pUrl = [NSURL URLWithString:p_sUrl];
    NSURLRequest* pRequest =[NSURLRequest requestWithURL:pUrl];
    [m_pWebView loadRequest:pRequest];
}
//-------------------------------------------------------------------------------
-(void)handleCloseButton:(UIButton *)button
{
    [self closeWebView];
}
//-------------------------------------------------------------------------------
-(NSString*)encodeString:(NSString*)nsParam
{
    
    return [nsParam stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet characterSetWithCharactersInString:@"!*'();:@&=+$,/?%#[]"]];
    /*return (__bridge_transfer NSString *)CFURLCreateStringByAddingPercentEscapes(kCFAllocatorDefault,
                                                                 (__bridge CFStringRef)nsParam,
                                                                 NULL,
                                                                 CFSTR("!*'();:@&=+$,/?%#[]"),
                                                                 kCFStringEncodingUTF8);*/
}
-(NSString*)decodeString:(NSString*)nsParam
{
    return [nsParam stringByRemovingPercentEncoding];
    /*return (__bridge_transfer NSString *)CFURLCreateStringByReplacingPercentEscapesUsingEncoding(kCFAllocatorDefault,
                                                            (__bridge CFStringRef)nsParam,
                                                             CFSTR(""),
                                                             kCFStringEncodingUTF8);*/
}

//-------------------------------------------------------------------------------
-(void)callWebviewJS:(NSString*)p_sMethodName para:(NSString*)p_sJsonPara callback:(NSString*)p_sCallback
{
    NSLog(@"callWebviewJS is not implemented");
}
@end
