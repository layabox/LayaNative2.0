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
#import "conchRuntime.h"
#import "../../../../source/conch/JCSystemConfig.h"
#include <util/JCCommonMethod.h>
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
    m_sGameID=nil;              //appStroe用到的
    m_bCheckNetwork=true;       //是否检查网络
    m_bNotification=false;      //是否打开消息推送
    m_nOrientationType = 30;   //屏幕的方向
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
        const char* sCheckNetwork=pConfigFile->GetValue("checkNetwork");
        const char* sOrientation=pConfigFile->GetValue("orientation");
        const char* sNotification = pConfigFile->GetValue("notification");
        const char* sThreadMode = pConfigFile->GetValue("ThreadMode");
        if( sGameID )
        {
            m_sGameID = [[NSString alloc] initWithUTF8String:sGameID ];
        }
        else
        {
            NSLog(@"读取ini gameID 错误");
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
       
        if( sNotification )
        {
            m_bNotification = atoi(sNotification)>0;
        }
        else
        {
            NSLog(@"读取ini notification 错误");
        }
        if(sThreadMode)
        {
            int nThreadMode = atoi(sThreadMode);
            laya::THREAD_MODE nMode = (laya::THREAD_MODE)nThreadMode;
            if (nMode == laya::THREAD_MODE_SINGLE)
            {
                laya::g_kSystemConfig.m_nThreadMODE = nMode;
                NSLog(@">>>>>>Thread Mode = single");
            }
            else if (nMode == laya::THREAD_MODE_DOUBLE)
            {
                laya::g_kSystemConfig.m_nThreadMODE = nMode;
                NSLog(@">>>>>>Thread Mode = double");
            }
            else
            {
                NSLog(@">>>>>>Thread Mode = %d", laya::g_kSystemConfig.m_nThreadMODE);
            }
        }
        else
        {
            NSLog(@"读取ini ThreadMode错误");
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
