/**
 @file			LayaWebView.h
 @brief         LayaWebView类 显示外部网页用的
 @author		wyw
 @version		1.0
 @date			2013_8_30
 @company       JoyChina
 */

#import <UIKit/UIKit.h>
#import <WebKit/WebKit.h>

//外部网页类
@interface LayaWebView  : NSObject
{
@public
    WKWebView*              m_pWebView;
    UIButton*               m_pCloseButton;
    int                     m_nX;
    int                     m_nY;
    int                     m_nWidth;
    int                     m_nHeight;
    
}

//初始化
-(LayaWebView*)init;
//显示WebView
-(void)showWebView:(NSString*)p_sUrl posx:(int)p_nX posy:(int)p_nY width:(int)p_nWidth height:(int)p_nHeight showCloseButton:(bool)p_bShowCloseButton;
//关闭WebView
-(void)closeWebView;
//排版
-(void)composing:(int)p_nX posy:(int)p_nY width:(int)p_nWidth height:(int)p_nHeight;
//loadUrl
-(void)loadUrl:(NSString*)p_sUrl;

-(void)callWebviewJS:(NSString*)p_sMethodName para:(NSString*)p_sJsonPara callback:(NSString*)p_sCallback;
@end
