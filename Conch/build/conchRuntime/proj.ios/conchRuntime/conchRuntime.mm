#import "conchRuntime.h"
#import <util/JCColor.h>
#import "../../../../source/conch/JCScriptRuntime.h"
#import "../../../../source/conch/CToObjectC.h"
#import "conchConfig.h"
#import "LayaToast.h"
#import "Notification/LayaNotifyManager.h"
#import <downloadCache/JCIosFileSource.h>
#import "../../../../source/conch/JCConch.h"
#import "../../../../source/conch/Audio/JCAudioManager.h"
#import "../../../../source/conch/JSWrapper/JSInterface/JSInterface.h"
#import "LayaEditBoxDelegate.h"
#import "LayaEditBox.h"
#import "TouchFilter.h"
#import "Audio/JCMp3Player.h"
#import "LayaWebView.h"
#import "Reachability/Reachability.h"
#import "LayaAlert.h"
#import "CToObjectCIOS.h"
#import "Reflection/refection.h"
#import <AudioToolBox/AudioSession.h>
#import "LayaDeviceSensor.h"
#import <resource/JCFileResManager.h>
#import "JCSystemConfig.h"

#import "LayaVideoPlayer.h"

@implementation FuncObj
-(id)init:(std::function<void(void)>)func
{
    self = [super init];
    if( self != nil)
    {
        m_Fuction = func;
        return self;
    }
    return nil;
}
@end

@implementation conchRuntime


extern bool g_bGLCanvasSizeChanged;
extern int g_nInnerWidth;
extern int g_nInnerHeight;
extern std::string gRedistPath;
extern std::string gResourcePath;
extern std::string gAssetRootPath;
extern bool gbBackground;
//------------------------------------------------------------------------------
static conchRuntime* g_pIOSConchRuntime = nil;
laya::JCConch* m_pConchEngine = NULL;
+(conchRuntime*)GetIOSConchRuntime
{
    return g_pIOSConchRuntime;
}
-(id)initWithView:(GLKView*)pGLView EAGLContext:(EAGLContext*) pGLContext downloadThreadNum:(int)nDownloadThreadNum URL:(NSString*)pUrl
{
    self = [super init];
    if( self != nil )
    {
        if (pUrl != nil)
        {
            g_kSystemConfig.m_strStartURL = pUrl.UTF8String;
        }
        m_bEngineInited = false;
        m_pGLKView = nil;
        m_pGLContext = nil;
        m_pConchEngine = NULL;
        m_pTouchFilter = NULL;
        for( int i = 0; i < 10; i++ )
        {
            m_vTouch[i] = NULL;
        }
        m_nTouchCount = 0;
        m_nGLViewOffset = 0;
        m_pEditBox = NULL;
        m_pEditBoxDelegate = NULL;
        m_pMp3Player = NULL;
        m_pWebView = NULL;
        m_pNetworkListener = NULL;
        m_fRetinaValue = 1;
        m_nsRootResourcePath = nil;
        m_nsRootCachePath = nil;
        m_pNSTimer = nil;
        m_bIgnoreCurEvent = false;
        m_fIOSVersion = 0;
        g_pIOSConchRuntime = self;
        m_pJSRunLoop = nil;
        m_pLayaAlert=[[LayaAlert alloc]init];
        m_pReflection = [[Reflection alloc] init];
        [self setConchRuntimeParam:pGLView EAGLContext:pGLContext downloadThreadNum:nDownloadThreadNum];
        m_nCurrentOrientation=-1;
        m_bStopEngine=false;
        return self;
    }
    return nil;
}
-(id)initWithView:(GLKView*)pGLView EAGLContext:(EAGLContext*) pGLContext downloadThreadNum:(int)nDownloadThreadNum;
{
    self = [self initWithView:pGLView EAGLContext:pGLContext downloadThreadNum:nDownloadThreadNum URL:nil];
    if( self != nil )
    {
        return self;
    }
    return nil;
}
-(void)setConchRuntimeParam:(GLKView*)pGLView EAGLContext:(EAGLContext*) pGLContext downloadThreadNum:(int)nDownloadThreadNum;
{
    float fIOSVersion = [[[UIDevice currentDevice] systemVersion] floatValue];
    //获得retina屏的缩放值
    float fRetinaValue = [UIScreen mainScreen].scale;
    float fIosVersion = [[[UIDevice currentDevice] systemVersion] floatValue];
    if( fIosVersion >= 8.0 )
        fRetinaValue = [UIScreen mainScreen].nativeScale;

    m_pGLKView = pGLView;
    m_pGLContext = pGLContext;
    m_fRetinaValue = fRetinaValue;
    m_nsRootResourcePath = [self getResourcePath];
    m_nsRootCachePath = [self getRootCachePath];
    NSLog(@"AppVersion=%@",[conchConfig GetInstance]->m_sAppVersion );
    if( [conchConfig GetInstance]->m_bNotification)
    {
        [[LayaNotifyManager GetInstance]deleteAllNotify];
    }
    m_pTouchFilter = new JCTouchFilter();
    m_nTouchCount = 0;
    m_fIOSVersion = fIosVersion;
 
    m_pResolution = new CGPoint();
    
    CGRect kRect = [UIScreen mainScreen].bounds;
    int nOrientationType = [conchConfig GetInstance]->m_nOrientationType;
    
    if( ( nOrientationType & UIInterfaceOrientationMaskLandscapeLeft ) == UIInterfaceOrientationMaskLandscapeLeft ||
            ( nOrientationType & UIInterfaceOrientationMaskLandscapeRight ) ==  UIInterfaceOrientationMaskLandscapeRight )
    {
        m_pResolution->x = MAX( kRect.size.width , kRect.size.height );
        m_pResolution->y = MIN( kRect.size.width , kRect.size.height );
    }
    //竖屏
    else if( ( nOrientationType & UIInterfaceOrientationMaskPortrait ) == UIInterfaceOrientationMaskPortrait ||
       ( nOrientationType & UIInterfaceOrientationMaskPortraitUpsideDown ) ==  UIInterfaceOrientationMaskPortraitUpsideDown )
    {
        m_pResolution->x = MIN( kRect.size.width , kRect.size.height );
        m_pResolution->y = MAX( kRect.size.width , kRect.size.height );
    }
    [self initNetworkListener];
    int nNetworkState = [self checkNetworkState];
    [self onGLReady:m_pResolution->x*m_fRetinaValue  height:m_pResolution->y*m_fRetinaValue downloadThreadNum:nDownloadThreadNum];
    m_nGLViewOffset = 0;
    m_pGLKView.multipleTouchEnabled = true;
    m_pEditBoxDelegate = [[LayaEditBoxDelegate alloc]init];

    [m_pEditBoxDelegate setRetinaValue:m_fRetinaValue];
    m_pEditBox = [[LayaEditBox alloc]initWithParentView:m_pGLKView EditBoxDelegate:m_pEditBoxDelegate ScreenRatio:m_fRetinaValue ];
    m_pMp3Player = [[JCMp3Player alloc] init];
    
    [self initExternalWebview];
    
    // 注册监听键盘弹出的事件
    [[NSNotificationCenter defaultCenter] addObserver:m_pEditBoxDelegate
                                             selector:@selector(keyboardWasShown:)
                                                 name:UIKeyboardWillShowNotification
                                               object:nil];
    
    // 注册监听键盘弹出完成的事件
    [[NSNotificationCenter defaultCenter] addObserver:m_pEditBoxDelegate
                                             selector:@selector(keyboardWasShownEnd:)
                                                 name:UIKeyboardDidShowNotification
                                               object:nil];
    // 注册屏幕方向改变的事件
    [[NSNotificationCenter defaultCenter] addObserver:m_pEditBoxDelegate
                                             selector:@selector(orientChange:)
                                                 name:UIDeviceOrientationDidChangeNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(orientChange:)
                                                 name:UIDeviceOrientationDidChangeNotification
                                               object:nil];
    /*[[NSNotificationCenter defaultCenter] addObserver:m_pEditBoxDelegate
                                             selector:@selector(changeValue:)
                                                 name:@"changeValue" object:nil];*/
    //[m_pEditBox->m_pEditBox addTarget:m_pEditBoxDelegate action:@selector(textFieldDidChange:) forControlEvents:UIControlEventEditingChanged];
    
    if ([[[UIDevice currentDevice] systemVersion] intValue] > 5) {
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(handleInterruption:) name:AVAudioSessionInterruptionNotification object:[AVAudioSession sharedInstance]];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(handleInterruption:) name:UIApplicationDidBecomeActiveNotification object:nil];
    }
    else {
        //AudioSessionInitialize(NULL, NULL, AudioEngineInterruptionListenerCallback, NULL);
    }

    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(willResignActive:) name:UIApplicationWillResignActiveNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(enterBackground:) name:UIApplicationDidEnterBackgroundNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(enterForeground:) name:UIApplicationWillEnterForegroundNotification object:nil];
    
    // initialize the default values of LayaVideoPlayer
    [LayaVideoPlayer setCurParentView: pGLView withRetianValue:m_fRetinaValue];
}

void AudioEngineInterruptionListenerCallback(void* user_data, UInt32 interruption_state)
{
    ALCcontext *context = JCAudioManager::GetInstance()->m_pWavPlayer->m_pContext;
    if (kAudioSessionBeginInterruption == interruption_state)
    {
        alcMakeContextCurrent(nullptr);
    }
    else if (kAudioSessionEndInterruption == interruption_state)
    {
        OSStatus result = AudioSessionSetActive(true);
        if (result) NSLog(@"Error setting audio session active! %d\n", result);
        
        alcMakeContextCurrent(context);
    }
}

-(void)handleInterruption:(NSNotification*)notification
{
    static bool resumeOnBecomingActive = false;
    ALCcontext *context = JCAudioManager::GetInstance()->m_pWavPlayer->m_pContext;
    if ([notification.name isEqualToString:AVAudioSessionInterruptionNotification]) {
        NSInteger reason = [[[notification userInfo] objectForKey:AVAudioSessionInterruptionTypeKey] integerValue];
        if (reason == AVAudioSessionInterruptionTypeBegan) {
            alcMakeContextCurrent(NULL);
        }
        
        if (reason == AVAudioSessionInterruptionTypeEnded) {
            if ([UIApplication sharedApplication].applicationState == UIApplicationStateActive) {
                NSError *error = nil;
                [[AVAudioSession sharedInstance] setActive:YES error:&error];
                alcMakeContextCurrent(context);
                laya::JCScriptRuntime::s_JSRT->restoreAudio();
            } else {
                resumeOnBecomingActive = true;
            }
        }
    }
    
    if ([notification.name isEqualToString:UIApplicationDidBecomeActiveNotification] && resumeOnBecomingActive) {
        resumeOnBecomingActive = false;
        NSError *error = nil;
        BOOL success = [[AVAudioSession sharedInstance]
                        setCategory: AVAudioSessionCategoryAmbient
                        error: &error];
        if (!success) {
            printf("set audio session failed.\n");
            return;
        }
        [[AVAudioSession sharedInstance] setActive:YES error:&error];
        alcMakeContextCurrent(context);
        laya::JCScriptRuntime::s_JSRT->restoreAudio();
    }
}
-(void)renderFrame
{
    if( laya::JCConch::s_pConchRender)
        laya::JCConch::s_pConchRender->renderFrame(0,false);
}

-(void)onVsync:(double)vsyncTmMs{
    if(laya::JCScriptRuntime::s_JSRT){
	    laya::JCScriptRuntime::s_JSRT->onVSyncEvent(vsyncTmMs);
    }
}
-(void) initExternalWebview
{
    m_pWebView = [[LayaWebView alloc] init];
    [m_pGLKView addSubview:m_pWebView->m_pWebView];
}
-(void) initNetworkListener
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(networkStateChange) name:LayakReachabilityChangedNotification object:nil];
    m_pNetworkListener=[LayaReachability reachabilityForInternetConnection];
    [m_pNetworkListener startNotifier];
}
//------------------------------------------------------------------------------
-(NSString*) getRootCachePath
{
    NSString* sAppDirctory = NSHomeDirectory();
    NSString* sDownloadRootPath = [ NSString stringWithFormat: @"%@/Library/Caches/", sAppDirctory ];
    return sDownloadRootPath;
}
//------------------------------------------------------------------------------
-(NSString*) getResourcePath
{
    return [[NSBundle mainBundle] resourcePath];
}
//------------------------------------------------------------------------------
-(void)destory
{
    if( m_pConchEngine != NULL )
    {
        m_bStopEngine = true;
        m_pConchEngine->onAppDestory();
        delete m_pConchEngine;
        m_pConchEngine = NULL;
    }

}
- (void)dealloc
{
    if( m_pConchEngine != NULL )
    {
        delete m_pConchEngine;
        m_pConchEngine = NULL;
    }
    [[NSNotificationCenter defaultCenter] removeObserver:m_pEditBoxDelegate name:UIKeyboardWillShowNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:m_pEditBoxDelegate name:UIKeyboardDidShowNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:m_pEditBoxDelegate name:UIDeviceOrientationDidChangeNotification object:nil];
    //移除网络监控
    [m_pNetworkListener stopNotifier];
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    
    [[NSNotificationCenter defaultCenter] removeObserver:self name:AVAudioSessionInterruptionNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIApplicationDidBecomeActiveNotification object:nil];
    
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIApplicationWillResignActiveNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIApplicationDidEnterBackgroundNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIApplicationWillEnterForegroundNotification object:nil];
}
//------------------------------------------------------------------------------
- (void)didReceiveMemoryWarning
{
    if(laya::JCScriptRuntime::s_JSRT){
        laya::JCScriptRuntime::s_JSRT->jsGC();
        laya::JCScriptRuntime::s_JSRT->jsGC();
    }
}
//------------------------------------------------------------------------------
-(void) onGLReady:(int)p_nWidth height:(int)p_nHeight downloadThreadNum:(int)nDownloadThreadNum
{
    if( m_bEngineInited == false )
    {
        NSLog(@"=============onGLReady width=%d,height=%d",p_nWidth,p_nHeight );
        g_nInnerWidth = p_nWidth;
        g_nInnerHeight = p_nHeight;
        gRedistPath = [m_nsRootCachePath cStringUsingEncoding:NSUTF8StringEncoding];
        const char* sResourcePath = [m_nsRootResourcePath cStringUsingEncoding:NSUTF8StringEncoding];
        laya::JCIosFileSource* pRedistFileResource = new laya::JCIosFileSource();
        gResourcePath = sResourcePath;
        gResourcePath += "/";
        gAssetRootPath= gResourcePath+"/cache/";
        pRedistFileResource->Init( gResourcePath.c_str() );
        laya::JCConch::s_pAssetsFiles = pRedistFileResource;
        m_pConchEngine = new laya::JCConch(nDownloadThreadNum,laya::JS_DEBUG_MODE_OFF,0);
        m_pConchEngine->onAppStart();
        laya::JCConch::s_pConchRender->onGLReady();
        m_bEngineInited = true;
    }
}
//-------------------------------------------------------------------------------
- (int) AddTouchToArray:(UITouch*)p_pTouch
{
    if( m_nTouchCount < 10 )
    {
        for( int i = 0; i < 10 ;i++ )
        {
            if( m_vTouch[i] == NULL )
            {
                m_vTouch[i] = p_pTouch;
                m_nTouchCount++;
                return i;
            }
        }
    }
    return -1;
}
//-------------------------------------------------------------------------------
- (int) RemoveTouchToArray:(UITouch*)p_pTouch
{
    if( m_nTouchCount > 0 )
    {
        for( int i = 0; i < 10 ;i++ )
        {
            if( m_vTouch[i] == p_pTouch )
            {
                m_vTouch[i] = NULL;
                m_nTouchCount--;
                return i;
            }
        }
        for( int i = 0; i < 10 ;i++ )
        {
            m_vTouch[i] = NULL;
        }
        m_nTouchCount=0;
    }
    return -1;
}
//-------------------------------------------------------------------------------
- (int) FindTouchToArray:(UITouch*)p_pTouch
{
    if( m_nTouchCount > 0 )
    {
        for( int i = 0; i < 10 ;i++ )
        {
            if( m_vTouch[i] == p_pTouch )
            {
                return i;
            }
        }
    }
    return -1;
}
//-------------------------------------------------------------------------------
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSSet* allTouches = [event allTouches];
    int nCount = (int)[allTouches count];
    //如果<=0 就发送down
    if( m_nTouchCount <= 0 )
    {
        //第一个手指
        IOSTouch kIOSTouch;
        kIOSTouch.m_nType = TOUCH_BEGAIN;
        
        //不是第一个手指
        IOSTouch kIOSTouchPD;
        kIOSTouchPD.m_nType = ACTION_POINTER_DOWN;
        
        int nNum = 0;
        for( int i = 0 ; i < nCount ;i++ )
        {
            UITouch* pSysTouch = [[allTouches allObjects] objectAtIndex:i];
            CGPoint kTouchLocation = [pSysTouch locationInView:[pSysTouch view]];
            int nID = [self AddTouchToArray:pSysTouch];
            if( nID != -1 )
            {
                //第一个手指是发送 TOUCH_BEGAIN
                if( i == 0 )
                {
                    kIOSTouch.m_nID[0] = nID;
                    kIOSTouch.m_nX[0] = kTouchLocation.x;
                    kIOSTouch.m_nY[0] = kTouchLocation.y;
                }
                //剩下的发送 POINTER_DOWN
                else
                {
                    if( nNum >= TOUCH_EVENT_SIZE )
                    {
                        NSLog(@"can't support touch num > %d",TOUCH_EVENT_SIZE);
                    }
                    kIOSTouchPD.m_nID[nNum] = nID;
                    kIOSTouchPD.m_nX[nNum] = kTouchLocation.x;
                    kIOSTouchPD.m_nY[nNum] = kTouchLocation.y;
                    nNum++;
                }
            }
        }
        //发送 TOUCH_BEGAIN
        kIOSTouch.m_nCount = 1;
        [self sendTouchEvent:kIOSTouch ratio:m_fRetinaValue offset:m_nGLViewOffset];
        
        //发送 POINTER_DOWN
        if( nNum > 0 )
        {
            kIOSTouchPD.m_nCount = nNum;
            [self sendTouchEvent:kIOSTouchPD ratio:m_fRetinaValue offset:m_nGLViewOffset];
        }
    }
    else
    {
        IOSTouch kIOSTouch;
        kIOSTouch.m_nType = ACTION_POINTER_DOWN;
        int nNum = 0;
        for( int i = 0 ; i < nCount ;i++ )
        {
            UITouch* pSysTouch = [[allTouches allObjects] objectAtIndex:i];
            if( [self FindTouchToArray:pSysTouch ] == -1 )
            {
                CGPoint kTouchLocation = [pSysTouch locationInView:[pSysTouch view]];
                int nID = [self AddTouchToArray:pSysTouch];
                if( nID != -1 )
                {
                    if( nNum >= TOUCH_EVENT_SIZE )
                    {
                        NSLog(@"can't support touch num > %d",TOUCH_EVENT_SIZE);
                    }
                    kIOSTouch.m_nID[nNum] = nID;
                    kIOSTouch.m_nX[nNum] = kTouchLocation.x;
                    kIOSTouch.m_nY[nNum] = kTouchLocation.y;
                    nNum++;
                }
            }
        }
        kIOSTouch.m_nCount = nNum;
        [self sendTouchEvent:kIOSTouch ratio:m_fRetinaValue offset:m_nGLViewOffset];
    }
}
//-------------------------------------------------------------------------------
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSSet* allTouches = [event allTouches];
    int nCount = (int)[allTouches count];
    IOSTouch kIosTouch;
    kIosTouch.m_nCount = nCount;
    kIosTouch.m_nType = TOUCHE_MOVED;
    
    for( int i = 0 ; i < nCount ;i++ )
    {
        UITouch* pSysTouch = [[allTouches allObjects] objectAtIndex:i];
        CGPoint kTouchLocation = [pSysTouch locationInView:[pSysTouch view]];
        kIosTouch.m_nID[i] = [self FindTouchToArray:pSysTouch];
        kIosTouch.m_nX[i] = kTouchLocation.x;
        kIosTouch.m_nY[i] = kTouchLocation.y;
    }
    [self sendTouchEvent:kIosTouch ratio:m_fRetinaValue offset:m_nGLViewOffset];
}
-(void) sendTouchEvent:(IOSTouch&) p_kTouch ratio:(float)p_fRatio offset:(int)p_nViewOffset
{
    if(m_bIgnoreCurEvent)
    {
        return;
    }
    m_pTouchFilter->onTouchEvent( p_kTouch, p_fRatio,p_nViewOffset );
}
//-------------------------------------------------------------------------------
- (void)HandleTouchEnd:(NSSet*)allTouches state:(int)p_nState
{
    int nCount = (int)[allTouches count];
    int nNum = 0;//要发送 pointer_up 的总数
    IOSTouch kIOSTouch;
    kIOSTouch.m_nType = ACTION_POINTER_UP;
    
    IOSTouch kLastTouch;//要发送的end
    kLastTouch.m_nType = TOUCHE_END;
    kLastTouch.m_nCount = 1;
    
    for( int i = 0 ; i < nCount ;i++ )
    {
        UITouch* pSysTouch = [[allTouches allObjects] objectAtIndex:i];
        if( p_nState == -1 || pSysTouch.phase == p_nState )
        {
            CGPoint kTouchLocation = [pSysTouch locationInView:[pSysTouch view]];
            int nID = [self RemoveTouchToArray:pSysTouch];
            if( nID != -1 )
            {
                if( m_nTouchCount <= 0 )
                {
                    kLastTouch.m_nID[0] = nID;
                    kLastTouch.m_nX[0] = kTouchLocation.x;
                    kLastTouch.m_nY[0] = kTouchLocation.y;
                    break;
                }
                else
                {
                    kIOSTouch.m_nID[nNum] = nID;
                    kIOSTouch.m_nX[nNum] = kTouchLocation.x;
                    kIOSTouch.m_nY[nNum] = kTouchLocation.y;
                    nNum++;
                }
            }
            else
            {
                NSLog(@"Can't find this touch pointer！！");
            }
        }
    }
    if(nNum>0)
    {
        kIOSTouch.m_nCount = nNum;
        [self sendTouchEvent:kIOSTouch ratio:m_fRetinaValue offset:m_nGLViewOffset];
    }
    //发送最后一个
    if( m_nTouchCount <= 0 )
    {
        [self sendTouchEvent:kLastTouch ratio:m_fRetinaValue offset:m_nGLViewOffset];
    }
}
//-------------------------------------------------------------------------------
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSSet* allTouches = [event allTouches];
    [self HandleTouchEnd:allTouches state:3];
}
//-------------------------------------------------------------------------------
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSSet* allTouches = [event allTouches];
    [self HandleTouchEnd:allTouches state:-1];
}
//-------------------------------------------------------------------------------
-(void) runJsLoop
{
    laya::JSThreadInterface* pJSThread = laya::JCScriptRuntime::s_JSRT->m_pScriptThread;
    pJSThread->run(NULL,NULL);
    if(m_bStopEngine)
    {
        dispatch_async(dispatch_get_main_queue(), ^{
            [self reset];
        });
        [m_pNSTimer invalidate];
        CFRunLoopStop( [[NSRunLoop currentRunLoop] getCFRunLoop] );
    }
}
-(void)runFunc:(NSTimer*)timer;
{
    FuncObj* pFunObj = [[timer userInfo] objectForKey:@"fun"];
    if( !m_bStopEngine && pFunObj && pFunObj->m_Fuction )
    {
        pFunObj->m_Fuction();
    }
    [timer invalidate];
    timer=nil;
}
//-------------------------------------------------------------------------------
- (void)networkStateChange
{
    int nState = [self checkNetworkState];
    laya::JCScriptRuntime::s_JSRT->onNetworkChanged(nState);
}
-(int)checkNetworkState
{
    // 1.检测手机是否能上网络(WIFI\3G\2.5G\4G)
    LayaReachability *conn = [LayaReachability reachabilityForInternetConnection];
    if ([conn currentReachabilityStatus] == NotReachable)
    {
        NSLog(@">>>>>>>checkNetworkState 没有网络");
        return laya::NET_NO;
    }
    else
    {
        NSLog(@">>>>>>>checkNetworkState 有网络");
        return laya::NET_YES;
    }
}
//-------------------------------------------------------------------------------
-(void) setScreenOrientation:(int)p_nType
{
    switch(p_nType)
    {
        case 0://landscape 16
        {
            [conchConfig GetInstance]->m_nOrientationType = UIInterfaceOrientationMaskLandscapeRight;
            NSNumber *value=[NSNumber numberWithInt:UIInterfaceOrientationLandscapeRight];
            [[UIDevice currentDevice] setValue:value forKey:@"orientation"];
            
        }
            break;
        case 1://portrait 2
        {
            [conchConfig GetInstance]->m_nOrientationType = UIInterfaceOrientationMaskPortrait;
            NSNumber *value=[NSNumber numberWithInt:UIInterfaceOrientationPortrait];
            [[UIDevice currentDevice] setValue:value forKey:@"orientation"];
        }
            break;
        case 8://reverse_landscape 8
        {
            [conchConfig GetInstance]->m_nOrientationType = UIInterfaceOrientationMaskLandscapeLeft;
            NSNumber *value=[NSNumber numberWithInt:UIInterfaceOrientationLandscapeLeft];
            [[UIDevice currentDevice] setValue:value forKey:@"orientation"];
        }
            break;
        case 9://reverse_portrait 4
        {
            [conchConfig GetInstance]->m_nOrientationType = UIInterfaceOrientationMaskPortraitUpsideDown;
            NSNumber *value=[NSNumber numberWithInt:UIInterfaceOrientationPortraitUpsideDown];
            [[UIDevice currentDevice] setValue:value forKey:@"orientation"];
        }
            break;
        case 4://sensor all
        {
            [conchConfig GetInstance]->m_nOrientationType = UIInterfaceOrientationMaskAll;
        }
            break;
        case 10://full sensor all
        {
            [conchConfig GetInstance]->m_nOrientationType = UIInterfaceOrientationMaskAll;
        }
            break;
        case 2://user full
        {
            [conchConfig GetInstance]->m_nOrientationType = UIInterfaceOrientationMaskAll;
        }
            break;
        case 6://sensor_landscape 16+8
        {
            [conchConfig GetInstance]->m_nOrientationType = UIInterfaceOrientationMaskLandscapeRight | UIInterfaceOrientationMaskLandscapeLeft;
            if (!UIDeviceOrientationIsLandscape([UIDevice currentDevice].orientation))
            {
                switch([UIApplication sharedApplication].statusBarOrientation)
                {
                    case UIInterfaceOrientationLandscapeRight:
                    {
                        NSNumber *value=[NSNumber numberWithInt:UIInterfaceOrientationLandscapeRight];
                        [[UIDevice currentDevice] setValue:value forKey:@"orientation"];
                    }
                    break;
                    case UIInterfaceOrientationLandscapeLeft:
                    {
                        NSNumber *value=[NSNumber numberWithInt:UIInterfaceOrientationLandscapeLeft];
                        [[UIDevice currentDevice] setValue:value forKey:@"orientation"];
                    }
                    default:
                    {
                        NSNumber *value=[NSNumber numberWithInt:UIInterfaceOrientationLandscapeLeft];
                        [[UIDevice currentDevice] setValue:value forKey:@"orientation"];
                    }
                    break;
                }
            }
        }
            break;
        case 7://reverse_portrait 2+4
        {
            [conchConfig GetInstance]->m_nOrientationType = UIInterfaceOrientationMaskPortrait | UIInterfaceOrientationMaskPortraitUpsideDown;
            if (!UIDeviceOrientationIsPortrait([UIDevice currentDevice].orientation))
            {
                switch([UIApplication sharedApplication].statusBarOrientation)
                {
                    case UIInterfaceOrientationMaskPortrait:
                    {
                        NSNumber *value=[NSNumber numberWithInt:UIInterfaceOrientationPortrait];
                        [[UIDevice currentDevice] setValue:value forKey:@"orientation"];
                    }
                    break;
                    case UIInterfaceOrientationMaskPortraitUpsideDown:
                    {
                        NSNumber *value=[NSNumber numberWithInt:UIInterfaceOrientationMaskPortraitUpsideDown];
                        [[UIDevice currentDevice] setValue:value forKey:@"orientation"];
                    }
                    break;
                    default:
                    {
                        NSNumber *value=[NSNumber numberWithInt:UIInterfaceOrientationPortrait];
                        [[UIDevice currentDevice] setValue:value forKey:@"orientation"];
                    }
                    break;
                }
            }
        }
        break;
            
        default://behind nosensor
            break;
    }

    [self updateResolution];
}
-(void)updateResolution
{
    CGRect kRect = [UIScreen mainScreen].bounds;
    int nOrientation=[UIDevice currentDevice].orientation;
    //竖屏
    if( nOrientation == UIInterfaceOrientationPortrait ||
       nOrientation ==  UIInterfaceOrientationPortraitUpsideDown )
    {
        m_pResolution->x = MIN( kRect.size.width , kRect.size.height );
        m_pResolution->y = MAX( kRect.size.width , kRect.size.height );
    }
    //横屏
    else if( nOrientation == UIInterfaceOrientationLandscapeLeft ||
            nOrientation ==  UIInterfaceOrientationLandscapeRight )
    {
        m_pResolution->x = MAX( kRect.size.width , kRect.size.height );
        m_pResolution->y = MIN( kRect.size.width , kRect.size.height );
    }

}
-(void)orientChange:(NSNotification *)aNotification
{
    int nOrientation=[UIDevice currentDevice].orientation;
    if (!UIDeviceOrientationIsValidInterfaceOrientation((UIDeviceOrientation)nOrientation))
    {
        return;
    }
    if( m_nCurrentOrientation == nOrientation )
    {
        return;
    }
    if ( ( [conchConfig GetInstance]->m_nOrientationType & nOrientation ) != nOrientation )
    {
        return;
    }
    int nOldOrientation = m_nCurrentOrientation;
    m_nCurrentOrientation = nOrientation;
    switch(nOrientation)
    {
    case UIDeviceOrientationPortrait:            // Device oriented vertically, home button on the bottom
    case UIDeviceOrientationPortraitUpsideDown:  // Device oriented vertically, home button on the top
        {
            if(UIInterfaceOrientationIsLandscape((UIInterfaceOrientation)nOldOrientation))
            {
                int w = MIN(g_nInnerWidth,g_nInnerHeight);
                int h = MAX(g_nInnerWidth,g_nInnerHeight);
                g_nInnerWidth = w;
                g_nInnerHeight = h;
                g_bGLCanvasSizeChanged = true;
                [self updateResolution];
            }
        }
        break;
    case UIDeviceOrientationLandscapeLeft:       // Device oriented horizontally, home button on the right
    case UIDeviceOrientationLandscapeRight:     // Device oriented horizontally, home button on the left
        {
             if(UIInterfaceOrientationIsPortrait((UIInterfaceOrientation)nOldOrientation))
             {
                 int w = MAX(g_nInnerWidth,g_nInnerHeight);
                 int h = MIN(g_nInnerWidth,g_nInnerHeight);
                 g_nInnerWidth = w;
                 g_nInnerHeight = h;
                 g_bGLCanvasSizeChanged = true;
                 [self updateResolution];
             }
        }
        break;
    case UIDeviceOrientationFaceUp:              // Device oriented flat, face up
    case UIDeviceOrientationFaceDown:             // Device oriented flat, face down
        {
        }
        break;
    };
}
-(void)reset
{
    if(m_pLayaAlert)
    {
        [m_pLayaAlert reset];
    }
    if (m_pReflection){
        [m_pReflection clearReflectionObjects];
    }
}

-(void)callbackToJSWithClass:(Class)cls methodName:(NSString*)name ret:(NSObject*)retObj
{
    [m_pReflection callbackToJSWithClass:cls methodName:name ret:retObj];
}

-(void)callbackToJSWithClassName:(NSString*)cls methodName:(NSString*)name ret:(NSObject*)retObj
{
    [m_pReflection callbackToJSWithClassName:cls methodName:name ret:retObj];
}
-(void)callbackToJSWithObject:(id)obj methodName:(NSString*)name ret:(NSObject*)retObj
{
    [m_pReflection callbackToJSWithObject:obj methodName:name ret:retObj];
}
- (void)enterBackground:(NSNotification *)notification {
    CToObjectCOnBlur();
    [[LayaDeviceSensor GetInstance] stop];
}

- (void)enterForeground:(NSNotification *)notification {
    CToObjectCOnFocus();
    [[LayaDeviceSensor GetInstance] start];
}

- (void)willResignActive:(NSNotification *)notification {
    //CToObjectCOnBlur();
}

-(void)runJS:(NSString*)script{
    
    const char* pScript = [script UTF8String];
    if (pScript){
        auto pFunction = std::bind(&CToObjectCRunJS,std::string(pScript));
    
        if (JCScriptRuntime::s_JSRT && JCScriptRuntime::s_JSRT->m_pPoster){
            JCScriptRuntime::s_JSRT->m_pPoster->postToJS(pFunction);
        }
    }
}
-(void)setFileDataHandler:(HandleFileData)func
{
    laya::setFileDataHandler(func);
}
@end
