/**
 @file			conchConfig.h
 @brief         配置用到的，比如版本号 或者描述信息
 @author		wyw
 @version		1.0
 @date			2013_7_5
 @company       JoyChina
 */

#import "conchConfig.h"
#import <util/JCIniFile.h>
#import <string>

//-------------------------------------------------------------------------------
static conchConfig* g_pConchConfig = nil;
//-------------------------------------------------------------------------------
@implementation conchConfig
//-------------------------------------------------------------------------------
+(conchConfig*)GetInstance
{
    if( g_pConchConfig == nil )
    {
        g_pConchConfig = [[conchConfig alloc] init];
    }
    return g_pConchConfig;
}
//-------------------------------------------------------------------------------
-(conchConfig*)init
{
    self = [super init];
    m_sLaya8Url=nil;            //如果是Laya8启动，启动的url
    m_sBackgroundcolor="#FFFFFF";//背景色
    m_sGameID=nil;              //appStroe用到的
    m_bCheckNetwork=true;       //是否检查网络
    m_bNotification=false;      //是否打开消息推送
    m_nOrientationType = 30;   //屏幕的方向
 	m_sUrl=nil;
    m_nHostPort=0;
    m_bShowAssistantTouch = false;
    [self readIni];
    m_sAppVersion=nil;          //版本号
    m_sAppLocalVersion = nil;   //对内版本号
    NSDictionary* infoDictionary =  [[NSBundle mainBundle] infoDictionary];
    // 当前应用软件版本 Bundle versions string, short
    m_sAppVersion = [infoDictionary objectForKey:@"CFBundleShortVersionString"];
    NSLog(@"当前应用软件版本:%@",m_sAppVersion);
    // 当前应用版本号码 Bundle versions
    m_sAppLocalVersion = [infoDictionary objectForKey:@"CFBundleVersion"];
    NSLog(@"当前应用Local版本号码：%@",m_sAppLocalVersion);
    return self;
}
//-------------------------------------------------------------------------------
-(bool) readIni
{
    std::string sIniFileName = [[self getResourcePath] cStringUsingEncoding:NSUTF8StringEncoding];
    sIniFileName += "/config.ini";
    // 初始化 IAP
    laya::JCIniFile *pConfigFile = laya::JCIniFile::loadFile( sIniFileName.c_str() );
    
    if( 0 == pConfigFile )
    {
        return false;
    }
    else
    {
        const char* sGameID=pConfigFile->GetValue("gameID");
        const char* sBackgroundColor=pConfigFile->GetValue("backgroundColor");
        const char* sCheckNetwork=pConfigFile->GetValue("checkNetwork");
        const char* sOrientation=pConfigFile->GetValue("orientation");
 		const char* sHostPort = pConfigFile->GetValue("hostport");
        const char* sUrl = pConfigFile->GetValue("url");
        const char* sNotification = pConfigFile->GetValue("notification");
        const char* sAssistantTouch = pConfigFile->GetValue("assistantTouch");
        if( sGameID )
        {
            m_sGameID = [[NSString alloc] initWithUTF8String:sGameID ];
        }
        else
        {
            NSLog(@"读取ini gameID 错误");
        }
        if( sBackgroundColor )
        {
            m_sBackgroundcolor = sBackgroundColor;
        }
        else
        {
            NSLog(@"读取ini backgroundColor 错误");
        }
        if( sCheckNetwork )
        {
            m_bCheckNetwork = atoi(sCheckNetwork)>0;
        }
        else
        {
            NSLog(@"读取ini checkNetworkd 错误");
        }
        if( sOrientation )
        {
            m_nOrientationType = atoi(sOrientation);
            if( m_nOrientationType < 1 )
            {
                NSLog(@"读取ini orientation 错误");
            }
        }
        else
        {
            NSLog(@"读取ini orientation错误");
        }
        if( sHostPort )
        {
            m_nHostPort = atoi(sHostPort);
        }
        else
        {
            NSLog(@"读取ini hostport错误");
        }
        if( sUrl )
        {
            m_sUrl = [[NSString alloc]initWithUTF8String:sUrl];
        }
        else
        {
            NSLog(@"读取ini url错误");
        }
        if( sNotification )
        {
            m_bNotification = atoi(sNotification)>0;
        }
        else
        {
            NSLog(@"读取ini notification 错误");
        }
        if( sAssistantTouch )
        {
            m_bShowAssistantTouch = atoi(sAssistantTouch)>0;
        }
        else
        {
            NSLog(@"读取ini assistantTouch 错误");
        }
        delete pConfigFile;
        pConfigFile = NULL;
    }
    return true;
}
//------------------------------------------------------------------------------
-(NSString*) getResourcePath
{
    return [[NSBundle mainBundle] resourcePath];
}
@end
