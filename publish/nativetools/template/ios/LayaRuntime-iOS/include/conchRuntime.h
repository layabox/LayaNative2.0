/**
 @file			conchRuntime.h
 @brief         runtime类
 @author		James
 @version		1.0
 @date			2015_8_28
 @company       LayaBox
 */
#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import "conchConfig.h"
#import <functional>

typedef char* (*HandleFileData)(const char* data, int& len);

class JCTouchFilter;
@class LayaReachability;
@class JCMp3Player;
@class LayaWebView;
@class LayaEditBox;
@class LayaEditBoxDelegate;
@class LayaAlert;
@class Reflection;

@interface FuncObj : NSObject
{
@public
    std::function<void(void)> m_Fuction;
};
-(id)init:(std::function<void(void)>)func;
@end

@interface conchRuntime : NSObject
{
@public
    
    GLKView*                    m_pGLKView;
    
    EAGLContext*                m_pGLContext;
    
    bool                        m_bEngineInited;
    
    //-----------------------------------------------------------------
    //touch用的
    JCTouchFilter*              m_pTouchFilter;
    
    UITouch*                    m_vTouch[10];
    
    int                         m_nTouchCount;
    
    //-----------------------------------------------------------------
    //editBox相关的
    int                         m_nGLViewOffset;        //因为editBox导致得移动
    
    LayaEditBox*                m_pEditBox;             //EditBox
    
    LayaEditBoxDelegate*        m_pEditBoxDelegate;     //eiditBoxDelegate
    
    //-----------------------------------------------------------------
    
    JCMp3Player*                m_pMp3Player;           //MP3播放器
    
    //界面相关的
    //-----------------------------------------------------------------
    
    LayaWebView*                m_pWebView;             //webView
    
    LayaReachability*           m_pNetworkListener;     //网络监听
    
    //-----------------------------------------------------------------
    
    float                       m_fRetinaValue;         //Retina的缩放比例
    
    NSString*                   m_nsRootResourcePath;   //Resoure的路径
    
    NSString*                   m_nsRootCachePath;      //Cache的根路径
    
    CGPoint*                    m_pResolution;          //当前分辨率
    
    float                       m_fIOSVersion;          //ios设备的版本
    
    NSTimer*                    m_pNSTimer;             //定时器
    
    bool                        m_bIgnoreCurEvent;
    
    LayaAlert*                  m_pLayaAlert;

    Reflection*                 m_pReflection;
    
    int                         m_nCurrentOrientation;
    bool                        m_bStopEngine;
    
    NSRunLoop*                  m_pJSRunLoop;
}


//全局的MainView
+(conchRuntime*)GetIOSConchRuntime;
-(id)initWithView:(GLKView*)pGLView EAGLContext:(EAGLContext*) pGLContext downloadThreadNum:(int)nDownloadThreadNum;
-(id)initWithView:(GLKView*)pGLView EAGLContext:(EAGLContext*) pGLContext downloadThreadNum:(int)nDownloadThreadNum URL:(NSString*)pUrl;
-(void)setConchRuntimeParam:(GLKView*)pGLView EAGLContext:(EAGLContext*) pGLContext downloadThreadNum:(int)nDownloadThreadNum;
-(void)didReceiveMemoryWarning;
-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;
-(void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event;
-(void)renderFrame;
-(void)onVsync:(double)vsyncTmMs;
-(int)checkNetworkState;
-(NSString*)getRootCachePath;
-(NSString*)getResourcePath;
-(void) setScreenOrientation:(int)p_nType;
-(void)reset;
-(void)callbackToJSWithClass:(Class)cls methodName:(NSString*)name ret:(NSObject*)retObj;
-(void)callbackToJSWithClassName:(NSString*)cls methodName:(NSString*)name ret:(NSObject*)retObj;
-(void)callbackToJSWithObject:(id)obj methodName:(NSString*)name ret:(NSObject*)retObj;
-(void)runJS:(NSString*)script;
-(void)setFileDataHandler:(HandleFileData)func;
-(void)destory;
-(void)runJsLoop;
-(void)runFunc:(NSTimer*)timer;
@end
