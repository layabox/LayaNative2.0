/**
 @file			UIEditBox.mm
 @brief         继承textField类
 @author		wyw
 @version		1.0
 @date			2014_8_26
 @company       JoyChina
 */

#import <UIKit/UIKit.h>
#import "../../../../source/conch/CToObjectC.h"
#import "LayaEditBox.h"
#import "CToObjectCIOS.h"
#import <util/JCColor.h>
#import <util/JCCommonMethod.h>
#import "util/Log.h"
#import <mach/mach.h>
#import "conchConfig.h"
#import "LayaToast.h"
#import "Notification/LayaNotifyManager.h"
#import "conchRuntime.h"
#import "../../../../source/conch/JCConch.h"
#import "Audio/JCMp3Player.h"
#import "LayaEditBoxDelegate.h"
#import "LayaEditBox.h"
#import "TouchFilter.h"
#import "Audio/JCMp3Player.h"
#import "LayaAlert.h"
#import "LayaDeviceSensor.h"
#import "LayaScreenShotter.h"
#import "JSInput.h"
#import <sys/utsname.h>
#import "Reflection/refection.h"
#import "LayaVideoPlayer.h"
#import "../../../../source/conch/JCScriptRuntime.h"
#import "../../../../source/conch/JCSystemConfig.h"

//文字相关的函数
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
typedef struct FontHeader
{
    int32_t fVersion;
    uint16_t fNumTables;
    uint16_t fSearchRange;
    uint16_t fEntrySelector;
    uint16_t fRangeShift;
}FontHeader;
typedef struct TableEntry
{
    uint32_t fTag;
    uint32_t fCheckSum;
    uint32_t fOffset;
    uint32_t fLength;
}TableEntry;
static uint32_t CalcTableCheckSum(const uint32_t *table, uint32_t numberOfBytesInTable)
{
    uint32_t sum = 0;
    uint32_t nLongs = (numberOfBytesInTable + 3) / 4;
    while (nLongs-- > 0) {
        sum += CFSwapInt32HostToBig(*table++);
    }
    return sum;
}
static uint32_t CalcTableDataRefCheckSum(CFDataRef dataRef)
{
    const uint32_t *dataBuff = (const uint32_t *)CFDataGetBytePtr(dataRef);
    uint32_t dataLength = (uint32_t)CFDataGetLength(dataRef);
    return CalcTableCheckSum(dataBuff, dataLength);
}
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
@implementation postCmdToMainThreadParam
-(id)init:(int)__cmd _p1:(int)_nParam1 _p2:(int)_nParam2
{
    self = [super init];
    cmd = __cmd;
    nParam1 = _nParam1;
    nParam2 = _nParam2;
    return self;
}
@end
//-----------------------------------------------------------------
@implementation postCmdToEditBoxParam
-(id)init:(int)__cmd _p1:(int)_nParam1 _p2:(int)_nParam2 _p3:(bool)_bParam2 _p4:(const char*)_sParam3;
{
    self = [super init];
    cmd = __cmd;
    nParam1 = _nParam1;
    nParam2 = _nParam2;
    bParam2 = _bParam2;
    if( _sParam3 )
    {
        sParam3 = _sParam3;
    }
    return self;
}
@end
//-----------------------------------------------------------------
@implementation postCmdParam
-(id)init:(bool)_bParam i:(int)_iParam f:(float)_fParam s:(const char*)_sParam;
{
    self = [super init];
    bParam = _bParam;
    iParam = _iParam;
    fParam = _fParam;
    if(_sParam)
    {
        sParam = _sParam;
    }
    return self;
}
@end
//-----------------------------------------------------------------
@implementation postCmdStringParam
-(id)init:(const char*)p_sParam;
{
    self = [super init];
    if( p_sParam )
    {
        sParam = p_sParam;
    }
    return self;
}
@end
//-----------------------------------------------------------------
@implementation postCmdToReflectionParam
-(id)init:(int)objid Class:(const char*)cls Method:(const char*)method Param:(const char*)param
{
    self = [super init];
    iObjID = objid;
    sClsName = cls;
    sMethodName = method;
    sParamStr = param;
    return self;
}
@end
//-----------------------------------------------------------------
@implementation postCmd3StringParam
-(id)init:(const char*)_sParam0 _p1:(const char*)_sParam1 _p2:(const char*)_sParam2
{
    self = [super init];

    sParam0 = _sParam0;
    sParam1 = _sParam1;
    sParam2 = _sParam2;
    
    return self;
}
@end
//-----------------------------------------------------------------
@implementation postCmd2FloatParam
-(id)init:(float)_fParam0 _p1:(float)_fParam1
{
    self = [super init];
    fParam0 = _fParam0;
    fParam1 = _fParam1;
    return self;
}
@end
//-----------------------------------------------------------------
@implementation postCmd4IntParam
-(id)init:(int)_iParam0 _p1:(int)_iParam1 _p2:(int)_iParam2 _p3:(int)_iParam3
{
    self = [super init];
    iParam0 = _iParam0;
    iParam1 = _iParam1;
    iParam2 = _iParam2;
    iParam3 = _iParam3;
    return self;
}
@end
//-----------------------------------------------------------------
@implementation postCmdString4IntParam
    
-(id)init:(const char*)s _p0:(int)_iParam0 _p1:(int)_iParam1 _p2:(int)_iParam2 _p3:(int)_iParam3 _p4:(bool)_bParam4
{
    self = [super init];
    sBuffer = s;
    iParam0 = _iParam0;
    iParam1 = _iParam1;
    iParam2 = _iParam2;
    iParam3 = _iParam3;
    bParam4 = _bParam4;
    return self;
}
@end
//-----------------------------------------------------------------
@implementation CToObjectCIOS
+ (char *)fontDataForCGFont:(CGFontRef)cgFont size:(long&)p_nSize
{
    if (!cgFont)
    {
        return nil;
    }
    CFRetain(cgFont);
    CFArrayRef tags = CGFontCopyTableTags(cgFont);
    int tableCount = CFArrayGetCount(tags);
    size_t *tableSizes = (size_t *)malloc(sizeof(size_t) * tableCount);
    memset(tableSizes, 0, sizeof(size_t) * tableCount);
    BOOL containsCFFTable = NO;
    size_t totalSize = sizeof(FontHeader) + sizeof(TableEntry) * tableCount;
    for (int index = 0; index < tableCount; ++index)
    {
        //get size
        size_t tableSize = 0;
        uint64_t aTag = (uint64_t)CFArrayGetValueAtIndex(tags, index);
        if (aTag == 'CFF ' && !containsCFFTable)
        {
            containsCFFTable = YES;
        }
        CFDataRef tableDataRef = CGFontCopyTableForTag(cgFont, aTag);
        if (tableDataRef != NULL) {
            tableSize = CFDataGetLength(tableDataRef);
            CFRelease(tableDataRef);
        }
        totalSize += (tableSize + 3) & ~3;
        tableSizes[index] = tableSize;
    }
    unsigned char *stream = (unsigned char *)malloc(totalSize);
    memset(stream, 0, totalSize);
    char* dataStart = (char*)stream;
    char* dataPtr = dataStart;
    // compute font header entries
    uint16_t entrySelector = 0;
    uint16_t searchRange = 1;
    while (searchRange < tableCount >> 1)
    {
        entrySelector++;
        searchRange <<= 1;
    }
    searchRange <<= 4;
    uint16_t rangeShift = (tableCount << 4) - searchRange;
    // write font header (also called sfnt header, offset subtable)
    FontHeader* offsetTable = (FontHeader*)dataPtr;
    //OpenType Font contains CFF Table use 'OTTO' as version, and with .otf extension
    //otherwise 0001 0000
    offsetTable->fVersion = containsCFFTable ? 'OTTO' : CFSwapInt16HostToBig(1);
    offsetTable->fNumTables = CFSwapInt16HostToBig((uint16_t)tableCount);
    offsetTable->fSearchRange = CFSwapInt16HostToBig((uint16_t)searchRange);
    offsetTable->fEntrySelector = CFSwapInt16HostToBig((uint16_t)entrySelector);
    offsetTable->fRangeShift = CFSwapInt16HostToBig((uint16_t)rangeShift);
    dataPtr += sizeof(FontHeader);
    // write tables
    TableEntry* entry = (TableEntry*)dataPtr;
    dataPtr += sizeof(TableEntry) * tableCount;
    for (int index = 0; index < tableCount; ++index)
    {
        uint64_t aTag = (uint64_t)CFArrayGetValueAtIndex(tags, index);
        CFDataRef tableDataRef = CGFontCopyTableForTag(cgFont, aTag);
        size_t tableSize = CFDataGetLength(tableDataRef);
        memcpy(dataPtr, CFDataGetBytePtr(tableDataRef), tableSize);
        entry->fTag = CFSwapInt32HostToBig((uint32_t)aTag);
        entry->fCheckSum = CFSwapInt32HostToBig(CalcTableCheckSum((uint32_t *)dataPtr, tableSize));
        uint32_t offset = dataPtr - dataStart;
        entry->fOffset = CFSwapInt32HostToBig((uint32_t)offset);
        entry->fLength = CFSwapInt32HostToBig((uint32_t)tableSize);
        dataPtr += (tableSize + 3) & ~3;
        ++entry;
        CFRelease(tableDataRef);
    }
    CFRelease(cgFont);
    free(tableSizes);
    /*
    //freeWhenDone 是否不删除fontData数据，如果改为yes，那么在用完之后就会删除该内存块。FT_Face无法工作
    NSData* fontData = [NSData dataWithBytesNoCopy:stream
                                            length:totalSize
                                      freeWhenDone:NO];
    */
    
    p_nSize = totalSize;
    return (char*)stream;
}
+(void) postCmdToMainThread:(id)param
{
    postCmdToMainThreadParam* _param = (postCmdToMainThreadParam*)param;
    laya::JCConch::s_pConch->onRunCmdInMainThread(_param->cmd, _param->nParam1, _param->nParam2);
}
+(void) postCmdToEditBox:(id)param
{
    postCmdToEditBoxParam* _param = (postCmdToEditBoxParam*)param;
    ObjectCOperateEditBox(_param->cmd, _param->nParam1,_param->nParam2,_param->bParam2,_param->sParam3);
}
+(void) setScreenWakeLock:(id)param
{
    postCmdParam* _param = (postCmdParam*)param;
    [[UIApplication sharedApplication] setIdleTimerDisabled:_param->bParam];
}
+(void) postCmdToAlert:(id)param
{
    postCmdStringParam* _param = (postCmdStringParam*)param;
    NSString * strMessage = [NSString stringWithUTF8String:_param->sParam.c_str()];
    [[conchRuntime GetIOSConchRuntime]->m_pLayaAlert alert:_param->sParam];
}
+(void) postCmdToSetScreenOrientation:(id)param
{
    postCmdParam* _param = (postCmdParam*)param;
    [[conchRuntime GetIOSConchRuntime] setScreenOrientation:_param->iParam];
}
+(NSString*) callMethod:(id)param
{
    postCmdToReflectionParam* _param = (postCmdToReflectionParam*)param;
    NSString* className = [NSString stringWithUTF8String:_param->sClsName.c_str()];
    NSString* methodName = [NSString stringWithUTF8String:_param->sMethodName.c_str()];
    NSString* sParam = [NSString stringWithUTF8String:_param->sParamStr.c_str()];
    conchRuntime* pConchRuntime = [conchRuntime GetIOSConchRuntime];
    assert(pConchRuntime != nil && "pConchRuntime should not be nil");
    Reflection* pReflection = pConchRuntime->m_pReflection;
    assert(pReflection != nil && "pReflection should not be nil");
    return [pReflection callMethod:_param->iObjID className:className methodName:methodName param:sParam];
}
+(void) setSensorAble:(id)param
{
    postCmdParam* _param = (postCmdParam*)param;
    if (_param->bParam){
        [[LayaDeviceSensor GetInstance] active:YES];
        [[LayaDeviceSensor GetInstance] start];
    }
    else{
        [[LayaDeviceSensor GetInstance] stop];
        [[LayaDeviceSensor GetInstance] active:NO];
    }
}
+(void) captureScreen:(id)param
{
    size_t w = CGImageGetWidth([conchRuntime GetIOSConchRuntime]->m_pGLKView.snapshot.CGImage);
    size_t h = CGImageGetHeight([conchRuntime GetIOSConchRuntime]->m_pGLKView.snapshot.CGImage);
    int len=0;
    unsigned char* result = [LayaScreenShotter convertUIImageToBitmapRGBA8:[conchRuntime GetIOSConchRuntime]->m_pGLKView.snapshot length:&len];
    JSInput::getInstance()->captureScreenCallBack((char*)result,len,(int)w,(int)h);
}
@end
//以下是C++调用ObjectC的函数
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
std::string CToObjectCGetGUID()
{
    NSString* nsDeviceID = [[UIDevice currentDevice] identifierForVendor].UUIDString;
    return [nsDeviceID cStringUsingEncoding:NSUTF8StringEncoding];
}
char* CToObjectCGetFontBuffer(long& dataSize)
{
    //UIFont* font = [UIFont systemFontOfSize:[UIFont systemFontSize]];
    //CGFontRef cgFont = CGFontCreateWithFontName((CFStringRef)font.fontName);
    CGFontRef cgFont = CGFontCreateWithFontName(CFSTR("STHeitiSC-Medium"));
    char* res = [CToObjectCIOS fontDataForCGFont:cgFont size:dataSize];
    return res;
}
void CToObjectCPostMainThread(int cmd,int param1,int param2)
{
    postCmdToMainThreadParam* pparam = [[postCmdToMainThreadParam alloc] init:cmd _p1:param1 _p2:param2];
    [CToObjectCIOS performSelectorOnMainThread:@selector(postCmdToMainThread:) withObject:pparam waitUntilDone:NO];
}
void CToObjectCPostEditBox(int cmd,int param1,int param2,bool bparam2,const char* sparam3)
{
    postCmdToEditBoxParam* pparam = [[postCmdToEditBoxParam alloc] init:cmd _p1:param1 _p2:param2 _p3:bparam2 _p4:sparam3];
    [CToObjectCIOS performSelectorOnMainThread:@selector(postCmdToEditBox:) withObject:pparam waitUntilDone:NO];
}
void ObjectCOperateEditBox(int cmd,int nparam1,int nparam2,bool bparam2,std::string sparam3)
{
    LayaEditBox* pEditBox = [conchRuntime GetIOSConchRuntime]->m_pEditBox;
    switch (cmd) {
        case IOS_EDITBOX_SETPOSX:[pEditBox setPosX:nparam1]; break;
        case IOS_EDITBOX_SETPOSY:[pEditBox setPosY:nparam1];break;
        case IOS_EDITBOX_SETWIDTH:[pEditBox setWidth:nparam1];break;
        case IOS_EDITBOX_SETHEIGHT:[pEditBox setHeight:nparam1];break;
        case IOS_EDITBOX_SETVALUE:{
            NSString* nsValue = [NSString stringWithUTF8String:sparam3.c_str()];
            [pEditBox setValue:nsValue];
        }
            break;
        case IOS_EDITBOX_SETSTYLE:IOS_SetEditBoxStyle(sparam3.c_str());break;
        case IOS_EDITBOX_SETVISIBLE:[pEditBox setVisible:bparam2];break;
        case IOS_EDITBOX_SETFOCUS:[pEditBox setFocus:true];break;
        case IOS_EDITBOX_SETBLUR:[pEditBox setFocus:false];break;
        case IOS_EDITBOX_SETCOLOR:{
            laya::JCColorFloat kFloatColors;
            laya::JCColor::getColorRGBFloatFromInt( nparam1,kFloatColors );
            UIColor* pUIColor = [UIColor colorWithRed:kFloatColors.r green:kFloatColors.g blue:kFloatColors.b alpha:1.0f];
            [pEditBox setTextColor:pUIColor];
        }
            break;
        case IOS_EDITBOX_SETFONTSIZE:[pEditBox setTextSize:nparam1];break;
        case IOS_EDITBOX_SETFONTPOS:[pEditBox setPosX:nparam1];[pEditBox setPosY:nparam2];break;
        case IOS_EDITBOX_SETFONTSIZE2:[pEditBox setWidth:nparam1];[pEditBox setHeight:nparam2];break;
        case IOS_EDITBOX_SETCURSORPOSITION:[pEditBox setFocus:true];[pEditBox setVisible:true];break;
        case IOS_EDITBOX_SETMAXLENGTH:[pEditBox setMaxLength:nparam1];break;
        case IOS_EDITBOX_SETPASSWORD:[pEditBox setPasswordType:bparam2];break;
        case IOS_EDITBOX_SETNUMBERONLY:[pEditBox setNumberOnly:bparam2];break;
        case IOS_EDITBOX_SETFORBIDEDIT:[pEditBox setForbidEdit:bparam2];break;
        case IOS_EDITBOX_SETMULTIABLE:[pEditBox setMultiAble:bparam2];break;
        case IOS_EDITBOX_SETREGULAR:{
            NSString* nsValue = [NSString stringWithFormat:@"%s",sparam3.c_str()];
            [pEditBox setRegular:nsValue];
        }
            break;
            
        default:
            break;
    }
}
int getAppUsedMem()
{
    return 0;
}
void CToObjectCSetEditBoxX( int p_nX )
{
    CToObjectCPostEditBox(IOS_EDITBOX_SETPOSX,p_nX);
}
void CToObjectCSetEditBoxY( int p_nY )
{
    CToObjectCPostEditBox(IOS_EDITBOX_SETPOSY,p_nY);
}
void CToObjectCSetEditBoxWidth( int p_nWidth )
{
    CToObjectCPostEditBox(IOS_EDITBOX_SETWIDTH,p_nWidth);
}
void CToObjectCSetEditBoxHeight( int p_nHeight )
{
    CToObjectCPostEditBox(IOS_EDITBOX_SETHEIGHT,p_nHeight);
}
void CToObjectCSetEditBoxValue( const char* p_sValue )
{
    CToObjectCPostEditBox(IOS_EDITBOX_SETVALUE,0,0,true,p_sValue);
}
int translateCharToInt( const char* p_sValue )
{
    char* sValue = (char*)p_sValue;
    int nLength = strlen(sValue);
    if( nLength > 2 )
    {
        sValue[ nLength-2 ] = 0;
    }
    return atoi( sValue );
}
void paserStyle( LayaEditBox* pEditBox,const char* p_sKey,const char* p_sValue )
{
    if( p_sKey == NULL || p_sValue == NULL )return;
    if( strcmp( p_sKey, "left") == 0 )
    {
        int n = translateCharToInt( p_sValue );
        [pEditBox setPosX:n];
    }
    else if( strcmp( p_sKey, "top") == 0 )
    {
        int n = translateCharToInt( p_sValue );
        [pEditBox setPosY:n];
    }
    else if( strcmp( p_sKey, "width") == 0 )
    {
        int n = translateCharToInt( p_sValue );
        [pEditBox setWidth:n];
    }
    else if( strcmp( p_sKey, "height") == 0 )
    {
        int n = translateCharToInt( p_sValue );
        [pEditBox setHeight:n];
    }
    else if( strcmp( p_sKey, "font-size") == 0 )
    {
        int n = translateCharToInt( p_sValue );
        [pEditBox setTextSize:n];
    }
    else
    {
        LOGE("editBox paserStyle error");
    }
}
void IOS_SetEditBoxStyle(const char* p_sStyle)
{
    if( p_sStyle == NULL ) return;
    LayaEditBox* pEditBox = [conchRuntime GetIOSConchRuntime]->m_pEditBox;
    std::string sStyle = p_sStyle;
    std::vector<char*> vVectorString;
    laya::splitString( vVectorString,(char*)sStyle.c_str(),';' );
    int nLength = (int)(vVectorString.size());
    if( nLength <= 0 )
    {
        std::vector<char*> vVectorString1;
        laya::splitString( vVectorString1, (char*)p_sStyle, ':' );
        if( vVectorString1.size() < 2 )
        {
            LOGE("objectC setEditBoxStyle error1");
            return;
        }
        paserStyle( pEditBox,vVectorString1[0],vVectorString1[1] );
    }
    else
    {
        for (int i = 0; i < nLength; i++ )
        {
            std::vector<char*> vVectorString1;
            laya::splitString( vVectorString1, vVectorString[i], ':' );
            if( vVectorString1.size() < 2 )
            {
                LOGE("objectC setEditBoxStyle error1");
                return;
            }
            paserStyle( pEditBox,vVectorString1[0],vVectorString1[1] );
        }
    }
}
void CToObjectCSetEditBoxStyle( const char* p_sStyle )
{
    CToObjectCPostEditBox(IOS_EDITBOX_SETSTYLE,0,0,true,p_sStyle);
}
void CToObjectCSetEditBoxVisible( bool p_bVisible )
{
    CToObjectCPostEditBox(IOS_EDITBOX_SETVISIBLE,0,0,p_bVisible);
}
void CToObjectCSetEditBoxFocus()
{
    CToObjectCPostEditBox(IOS_EDITBOX_SETFOCUS,0);
}
void CToObjectCSetEditBoxBlur()
{
    CToObjectCPostEditBox(IOS_EDITBOX_SETBLUR,0);
}
void CToObjectCSetEditBoxColor( int p_nColor )
{
    CToObjectCPostEditBox(IOS_EDITBOX_SETCOLOR,p_nColor);
}
void CToObjectCSetEditBoxFontSize( int p_nFontSize )
{
    CToObjectCPostEditBox(IOS_EDITBOX_SETFONTSIZE,p_nFontSize);
}
void CToObjectCSetEditBoxFontPos( int p_nX,int p_nY )
{
    CToObjectCPostEditBox(IOS_EDITBOX_SETFONTPOS,p_nX,p_nY);
}
void CToObjectCSetEditBoxFontSize( int p_nWidth,int p_nHeight )
{
    CToObjectCPostEditBox(IOS_EDITBOX_SETFONTSIZE2,p_nWidth,p_nHeight);
}
void CToObjectCSetEditBoxCursorPosition( int p_nPos )
{
    CToObjectCPostEditBox(IOS_EDITBOX_SETCURSORPOSITION,p_nPos);
}
void CToObjectCSetEditBoxMaxLength( int p_nMaxLength )
{
    CToObjectCPostEditBox(IOS_EDITBOX_SETMAXLENGTH,p_nMaxLength);
}
void CToObjectCSetEditBoxPassword( bool p_bPassword )
{
    CToObjectCPostEditBox(IOS_EDITBOX_SETPASSWORD,0,0,p_bPassword);
}
void CToObjectCSetEditBoxMultiAble(bool p_bMultiAble)
{
    CToObjectCPostEditBox(IOS_EDITBOX_SETMULTIABLE,0,0,p_bMultiAble);
}
void CToObjectCSetEditBoxRegular( const char* p_sRegular )
{
    CToObjectCPostEditBox(IOS_EDITBOX_SETREGULAR,0,0,true,p_sRegular);
}
void CToObjectCSetEditBoxNumberOnly( bool p_bNumberOnly )
{
    CToObjectCPostEditBox(IOS_EDITBOX_SETNUMBERONLY,0,0,p_bNumberOnly );
}
void CToObjectCSetEditBoxForbidEdit( bool p_bForbidEdit )
{
    CToObjectCPostEditBox(IOS_EDITBOX_SETFORBIDEDIT,0,0,p_bForbidEdit );
}
const char* CToObjectCGetEditBoxValue()
{
    LayaEditBox* pEditBox = [conchRuntime GetIOSConchRuntime]->m_pEditBox;
    const char* sValue = [[pEditBox getValue] cStringUsingEncoding:NSUTF8StringEncoding];
    return sValue;
}
void CToObjectCPlayMp3Audio( const char* p_sUrl,int p_nTimes,float nCurrentTime )
{
    JCMp3Player* pMp3Player = [conchRuntime GetIOSConchRuntime]->m_pMp3Player;
    NSString* nsValue = [NSString stringWithUTF8String:p_sUrl];
    [pMp3Player playMp3:nsValue times:p_nTimes currentTime:nCurrentTime];
}
void CToObjectCSetMp3Volume( float p_nVolume )
{
    JCMp3Player* pMp3Player = [conchRuntime GetIOSConchRuntime]->m_pMp3Player;
    [pMp3Player setVolume:p_nVolume];
}
void CToObjectCStopMp3()
{
    JCMp3Player* pMp3Player = [conchRuntime GetIOSConchRuntime]->m_pMp3Player;
    [pMp3Player stopMp3];
}
void CToObjectCResumeMp3()
{
    JCMp3Player* pMp3Player = [conchRuntime GetIOSConchRuntime]->m_pMp3Player;
    [pMp3Player resumeMp3];
}
void CToObjectCPauseMp3()
{
    JCMp3Player* pMp3Player = [conchRuntime GetIOSConchRuntime]->m_pMp3Player;
    [pMp3Player pauseMp3];
}
/*
    注意啦，CToObjectCRunJSLoop和CToObjectCRunStopJSLoop 这两个函数轻易别动，坑太深
    1、这个是js的线程，添加了一个timer，这个timer一直循环调用 runjsLoop函数
    2、如果不在objectc中用这样的方法的写，比如写while循环，无论是在c++代码中，还是在objectC中js很多根本无法释放。
    3、现在使用的方法是CFRunLoopRun,使用这个方法是因为停止的时候需要配套调用CFRunLoopStop，NSRunLoop run这样的方法都不行
    4、CToObjectCRunStopJSLoop停止函数，必须和开启函数在一个线程中，这个太违背常识了，一般我们停止一个子线程必须要求在主线层中
 */
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void CToObjectCRunJSLoop()
{
    conchRuntime* pConchRuntime = [conchRuntime GetIOSConchRuntime];
    pConchRuntime->m_bStopEngine = false;
    pConchRuntime->m_pNSTimer = [NSTimer scheduledTimerWithTimeInterval:0.016f target:pConchRuntime selector:@selector(runJsLoop) userInfo:nil repeats:YES];
    pConchRuntime->m_pJSRunLoop = [NSRunLoop currentRunLoop];
    CFRunLoopRun();
}
void CToObjectCRunStopJSLoop()
{
    conchRuntime* pConchRuntime = [conchRuntime GetIOSConchRuntime];
    pConchRuntime->m_bStopEngine = true;
}
void CToObjectCPostFunc(std::function<void(void)> func)
{
    conchRuntime* pConchRuntime = [conchRuntime GetIOSConchRuntime];
    FuncObj* funcObj = [[FuncObj alloc]init:func];
    NSMutableDictionary* pDictionary = [[NSMutableDictionary alloc] init];
    [pDictionary setObject:funcObj forKey:@"fun"];
    NSTimer* pTimer = [NSTimer timerWithTimeInterval:0 target:pConchRuntime selector:@selector(runFunc:) userInfo:pDictionary repeats:NO];
    [pConchRuntime->m_pJSRunLoop addTimer:pTimer forMode:NSDefaultRunLoopMode];
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void CToObjectCAlert(const char* message)
{
    postCmdStringParam* param = [[postCmdStringParam alloc] init:message];
    [CToObjectCIOS performSelectorOnMainThread:@selector(postCmdToAlert:) withObject:param waitUntilDone:NO];
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

bool memoryInfo(vm_statistics_data_t *vmStats)
{
    mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
    kern_return_t kernReturn = host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)vmStats, &infoCount);
    return kernReturn == KERN_SUCCESS;
}
long CToObjectCGetTotalMem()
{
    vm_statistics_data_t vmStats;
    if ( memoryInfo(&vmStats))
    {
        NSLog(@">>>>>>>>>>>>>>>>>>>ios info free: %u\nactive: %u\ninactive: %u\nwire: %u\nzero fill: %u\nreactivations: %u\npageins: %u\npageouts: %u\nfaults: %u\ncow_faults: %u\nlookups: %u\nhits: %u",
              vmStats.free_count * vm_page_size,
              vmStats.active_count * vm_page_size,
              vmStats.inactive_count * vm_page_size,
              vmStats.wire_count * vm_page_size,
              vmStats.zero_fill_count * vm_page_size,
              vmStats.reactivations * vm_page_size,
              vmStats.pageins * vm_page_size,
              vmStats.pageouts * vm_page_size,
              vmStats.faults,
              vmStats.cow_faults,
              vmStats.lookups,
              vmStats.hits
              );
    }
    return (long)(NSRealMemoryAvailable() / 1024);
}
long CToObjectCGetUsedMem()
{
    task_basic_info_data_t taskInfo;
    mach_msg_type_number_t infoCount = TASK_BASIC_INFO_COUNT;
    kern_return_t kernReturn = task_info(mach_task_self(),TASK_BASIC_INFO,(task_info_t)&taskInfo,&infoCount);
    if (kernReturn != KERN_SUCCESS)
    {
        return NSNotFound;
    }
    return (long)(taskInfo.resident_size / 1024.0);
}
long CToObjectCGetAvalidMem()
{
    vm_statistics_data_t vmStats;
    mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
    kern_return_t kernReturn = host_statistics(mach_host_self(),HOST_VM_INFO,(host_info_t)&vmStats,&infoCount);
    if (kernReturn != KERN_SUCCESS)
    {
        return NSNotFound;
    }
    return (long)((vm_page_size *vmStats.free_count) / 1024.0);
}
float CToObjectCGetScreenInch()
{
    return 0;
}
void CToObjectCSetScreenOrientation( int p_nType )
{
    postCmdParam* param = [[postCmdParam alloc]init:true i:p_nType f:0 s:""];
    [CToObjectCIOS performSelectorOnMainThread:@selector(postCmdToSetScreenOrientation:) withObject:param waitUntilDone:NO];
}
long CToObjectCGetScreenOrientation()
{
    //TODO
    return 0;
}
int CToObjectCGetNetworkType()
{
    return [[conchRuntime GetIOSConchRuntime]checkNetworkState];
}
std::string CToObjectCGetDeviceModel()
{
    return CToObjectCGetDeviceInfo();
}
std::string CToObjectCGetAppVersion()
{
    return [[conchConfig GetInstance]->m_sAppVersion cStringUsingEncoding:NSUTF8StringEncoding];
}
std::string CToObjectCGetAppLocalVersion()
{
    return [[conchConfig GetInstance]->m_sAppLocalVersion cStringUsingEncoding:NSUTF8StringEncoding];
}
float CToObjectCGetDeviceSystemVersion()
{
    return [[[UIDevice currentDevice] systemVersion] floatValue];
}
std::string CToObjectCGetDeviceInfo()
{
    NSString* nsDeviceID = [[UIDevice currentDevice] identifierForVendor].UUIDString;
    //NSString* nsName =[[UIDevice currentDevice] name];
    //设备名称  5s  6  6s等
    struct utsname pSystemInfo;
    uname(&pSystemInfo);
    NSString* nsDeviceName = [NSString stringWithCString: pSystemInfo.machine encoding:NSASCIIStringEncoding];
    NSString* nsVersionName = [[UIDevice currentDevice] systemVersion];
    NSString* nsDeviceModel =[[UIDevice currentDevice] model];
    NSString* nsLocalizedModel = [[UIDevice currentDevice] localizedModel];
    NSString* nsResolution = [NSString stringWithFormat:@"%d*%d",(int)([conchRuntime GetIOSConchRuntime]->m_pResolution->x),(int)([conchRuntime GetIOSConchRuntime]->m_pResolution->y)];
    float fRetinaValue = [conchRuntime GetIOSConchRuntime]->m_fRetinaValue;
    NSString* nsRetinaValue = [NSString stringWithFormat:@"%f",fRetinaValue];
    
    NSDictionary* pDictionary = [NSDictionary dictionaryWithObjectsAndKeys:
                                 nsResolution,@"resolution",
                                 @"imeixxx",@"imei",
                                 @"imsixxx",@"imsi",
                                 @"ios",@"os",
                                 nsVersionName,@"osversion",
                                 nsDeviceModel,@"phonemodel",
                                 nsDeviceName,@"devicename",
                                 nsLocalizedModel,@"localmodel",
                                 nsDeviceID,@"guid",
                                 nsRetinaValue,@"retinavalue",
                                 nil];
    NSError* pError = nil;
    NSData* pJsonData = [NSJSONSerialization dataWithJSONObject:pDictionary options:NSJSONWritingPrettyPrinted error:&pError];
    if( !pError )
    {
        std::string sBuffer = [[[NSString alloc] initWithData:pJsonData encoding:NSUTF8StringEncoding] cStringUsingEncoding:NSUTF8StringEncoding];
        return sBuffer.c_str();
    }
    return "{  \"guid\" : \"unknow\",  \"os\" : \"ios\",  \"resolution\" : \"unkown\",  \"phonename\" : \"unknow\",  \"osversion\" : \"unknow\",  \"phonemodel\" : \"unknow\",  \"retinavalue\" : \"1.000000\",  \"imei\" : \"imeixxx\",  \"imsi\" : \"imsixxx\",  \"localmodel\" : \"unknow\"}";
}
void CToObjectCSetScreenWakeLock( bool p_bWakeLock )
{
    postCmdParam* pParam = [[postCmdParam alloc] init:p_bWakeLock i:0 f:0 s:""];
    [CToObjectCIOS performSelectorOnMainThread:@selector(setScreenWakeLock:) withObject:pParam waitUntilDone:NO];
}
void CToObjectCShowToast( const char* p_sInfo )
{
    NSString* nsInfo = [NSString stringWithUTF8String:p_sInfo];
    [LayaToast showWithText:nsInfo];
}
void CToObjectCOpenAppStoreUrl( const char* p_sAppUrl )
{
    NSString* nsUrl = [NSString stringWithFormat:@"http://itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?id=%s&pageNumber=0&sortOrdering=2&type=Purple+Software&mt=8",p_sAppUrl];
    [[UIApplication sharedApplication]  openURL:[NSURL URLWithString:nsUrl]];
}
void CToObjectCSetRepeatNotify( int p_nID,long p_nStartTime,int p_nRepeatType,const char* p_sTickerText,const char* p_sTitleText,const char* p_sDesc )
{
    if( [conchConfig GetInstance]->m_bNotification)
    {
        NSString* sTickerText = [NSString stringWithUTF8String:p_sTickerText];
        NSString* sTitileText = [NSString stringWithUTF8String:p_sTitleText];
        NSString* sDesc = [NSString stringWithUTF8String:p_sDesc];
        [[LayaNotifyManager GetInstance]setRepeatNotify:p_nID startTime:p_nStartTime repeateType:p_nRepeatType tickerText:sTickerText titleText:sTitileText desc:sDesc];
    }
}
void CToObjectCSetOnceNotify( int p_nID,long p_nStartTime,const char* p_sTickerText,const char* p_sTitleText,const char* p_sDesc )
{
    if( [conchConfig GetInstance]->m_bNotification)
    {
        NSString* sTickerText = [NSString stringWithUTF8String:p_sTickerText];
        NSString* sTitileText = [NSString stringWithUTF8String:p_sTitleText];
        NSString* sDesc = [NSString stringWithUTF8String:p_sDesc];
        [[LayaNotifyManager GetInstance]setOnceNotify:p_nID startTime:p_nStartTime tickerText:sTickerText titleText:sTitileText desc:sDesc];
    }
}
void CToObjectCDeleteOnceNotify( int p_nID )
{
    if( [conchConfig GetInstance]->m_bNotification)
    {
        [[LayaNotifyManager GetInstance]deleteOnceNotify:p_nID];
    }
}
void CToObjectCDeleteAllNotify()
{
    if( [conchConfig GetInstance]->m_bNotification)
    {
        [[LayaNotifyManager GetInstance]deleteAllNotify];
    }
}

void CToObjectCLogIExt(const char* str)
{
    NSLog(@"%s", str);
}

void CToObjectCLogI( const char* szFormat,...)
{
    va_list args;
    va_start(args, szFormat);
    NSString* nsFormat = [NSString stringWithUTF8String:szFormat];
    NSLogv( nsFormat, args);
    va_end(args);
}
void CToObjectCLogE( const char* szFormat,...)
{
    va_list args;
    va_start(args, szFormat);
    NSString* nsFormat = [NSString stringWithUTF8String:szFormat];
    NSLogv( nsFormat, args);
    va_end(args);
}
void CToObjectCLogW( const char* szFormat,...)
{
    va_list args;
    va_start(args, szFormat);
    NSString* nsFormat = [NSString stringWithUTF8String:szFormat];
    NSLogv( nsFormat, args);
    va_end(args);
}

std::string CToObjectCCallMethod(int objid,bool isSync, const char*clsName, const char* methodName, const char* paramStr)
{
    NSLog(@" %s %s %s",clsName,methodName,paramStr);

    postCmdToReflectionParam* pParam =
    [[postCmdToReflectionParam alloc] init:objid Class:clsName Method:methodName Param:paramStr];
    NSString* ret = [CToObjectCIOS callMethod:pParam];
    assert(ret != nil && "ret should not be nil");
    return std::string([ret cStringUsingEncoding:NSUTF8StringEncoding]);
}

void reflectionCallback(const std::string& jsonret)
{
    std::string script = "conch.platCallBack(" + jsonret + ")";
    JSP_RUN_SCRIPT(script.c_str(), NULL);
    NSLog(@"%@", [[NSString alloc] initWithUTF8String:jsonret.c_str()]);
}

void CToObjectCSetSensorAble(bool p_bAble)
{
    postCmdParam* pParam = [[postCmdParam alloc] init:p_bAble i:0 f:0 s:""];
    [CToObjectCIOS performSelectorOnMainThread:@selector(setSensorAble:) withObject:pParam waitUntilDone:NO];
}
void CToObjectCOnBlur()
{
    if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
    {
        if(laya::JCScriptRuntime::s_JSRT)
        {
            if (JCScriptRuntime::s_JSRT->m_pJSOnBlurFunction.isValid())
            {
                JCScriptRuntime::s_JSRT->m_pJSOnBlurFunction.Call();
            }
        }
    }
}
void CToObjectCOnFocus()
{
    if (g_kSystemConfig.m_nThreadMODE == THREAD_MODE_SINGLE)
    {
        if(laya::JCScriptRuntime::s_JSRT)
        {
            if (JCScriptRuntime::s_JSRT->m_pJSOnFocusFunction.isValid())
            {
                JCScriptRuntime::s_JSRT->m_pJSOnFocusFunction.Call();
            }
        }
    }
}
void CToObjectCRunJS(const std::string& script)
{
    JSP_RUN_SCRIPT(script.c_str(), NULL);
}
void CToObjectCCaptureScreen()
{
    [CToObjectCIOS performSelectorOnMainThread:@selector(captureScreen:) withObject:nil waitUntilDone:NO];
}

float CToObjectCGetDevicePixelRatio()
{
    return [conchRuntime GetIOSConchRuntime]->m_fRetinaValue;
}

//-------------------------------
// begin video player

void CToObjectNewVideoPlayer(void** obj, std::function<void(const char*)> emitFunc)
{
    LayaVideoPlayer* videoPlayer = [[LayaVideoPlayer alloc] initDefaultWithType:RT_VIDEOOUTPUT];
    [videoPlayer setEmit: emitFunc];
    *obj = (__bridge_retained void*) videoPlayer;
}

void CToObjectDisposeVideoPlayer(void* obj)
{
    LayaVideoPlayer* pVideoPlayer = (__bridge_transfer LayaVideoPlayer*)obj;
    pVideoPlayer = nullptr;
}

void CToObjectVideoPlayerLoad(void* obj, const char* url)
{
    LayaVideoPlayer* pVideoPlayer = (__bridge LayaVideoPlayer*)obj;
    [pVideoPlayer load: url];
}

void CToObjectVideoPlayerPlay(void* obj)
{
    LayaVideoPlayer* pVideoPlayer = (__bridge LayaVideoPlayer*)obj;
    [pVideoPlayer play];
}

void CToObjectVideoPlayerPause(void* obj)
{
    LayaVideoPlayer* pVideoPlayer = (__bridge LayaVideoPlayer*)obj;
    [pVideoPlayer pause];
}

void CToObjectVideoPlayerGetPaused(void* obj, bool* ret)
{
    LayaVideoPlayer* pVideoPlayer = (__bridge LayaVideoPlayer*)obj;
    *ret = [pVideoPlayer isPaused];
}

void CToObjectVideoPlayerGetVideoWidth(void* obj, double* ret)
{
    LayaVideoPlayer* pVideoPlayer = (__bridge LayaVideoPlayer*)obj;
    *ret = [pVideoPlayer getVideoWidth];
}

void CToObjectVideoPlayerGetVideoHeight(void* obj, double* ret)
{
    LayaVideoPlayer* pVideoPlayer = (__bridge LayaVideoPlayer*)obj;
    *ret = [pVideoPlayer getVideoHeight];
}

void CToObjectVideoPlayerSetX(void* obj, double val)
{
    LayaVideoPlayer* pVideoPlayer = (__bridge LayaVideoPlayer*)obj;
    [pVideoPlayer setX: val];
}

void CToObjectVideoPlayerSetY(void* obj, double val)
{
    LayaVideoPlayer* pVideoPlayer = (__bridge LayaVideoPlayer*)obj;
    [pVideoPlayer setY: val];
}

void CToObjectVideoPlayerSetWidth(void* obj, double val)
{
    LayaVideoPlayer* pVideoPlayer = (__bridge LayaVideoPlayer*)obj;
    [pVideoPlayer setWidth: val];
}

void CToObjectVideoPlayerGetWidth(void* obj, double* val)
{
    LayaVideoPlayer* pVideoPlayer = (__bridge LayaVideoPlayer*)obj;
    *val = pVideoPlayer.width;
}

void CToObjectVideoPlayerSetHeight(void* obj, double val)
{
    LayaVideoPlayer* pVideoPlayer = (__bridge LayaVideoPlayer*)obj;
    [pVideoPlayer setHeight: val];
}

void CToObjectVideoPlayerGetHeight(void* obj, double* val)
{
    LayaVideoPlayer* pVideoPlayer = (__bridge LayaVideoPlayer*)obj;
    *val = pVideoPlayer.height;
}

void CToObjectVideoPlayerGetCurrentTime(void* obj, double* val)
{
    LayaVideoPlayer* pVideoPlayer = (__bridge LayaVideoPlayer*)obj;
    *val = [pVideoPlayer currentTime];
}

void CToObjectVideoPlayerSetCurrentTime(void* obj, double val)
{
    LayaVideoPlayer* pVideoPlayer = (__bridge LayaVideoPlayer*)obj;
    [pVideoPlayer setCurrentTime:val];
}
void CToObjectVideoPlayerGetDuration(void* obj, double* val)
{
    LayaVideoPlayer* pVideoPlayer = (__bridge LayaVideoPlayer*)obj;
    *val = [pVideoPlayer duration];
}

void CToObjectVideoPlayerSetVolume(void* obj, double val)
{
    LayaVideoPlayer* pVideoPlayer = (__bridge LayaVideoPlayer*)obj;
    pVideoPlayer.volume = val;
}

void CToObjectVideoPlayerGetVolume(void* obj, double* val)
{
    LayaVideoPlayer* pVideoPlayer = (__bridge LayaVideoPlayer*)obj;
    *val = pVideoPlayer.volume;
}

void CToObjectVideoPlayerSetLoop(void* obj, bool val)
{
    LayaVideoPlayer* pVideoPlayer = (__bridge LayaVideoPlayer*)obj;
    pVideoPlayer.loop = val;
}

void CToObjectVideoPlayerGetLoop(void* obj, bool* val)
{
    LayaVideoPlayer* pVideoPlayer = (__bridge LayaVideoPlayer*)obj;
    *val = pVideoPlayer.loop;
}

void CToObjectVideoPlayerGetAutoplay(void* obj, bool* val)
{
    LayaVideoPlayer* pVideoPlayer = (__bridge LayaVideoPlayer*)obj;
    *val = pVideoPlayer.autoplay;
}

void CToObjectVideoPlayerSetAutoplay(void* obj, bool val)
{
    LayaVideoPlayer* pVideoPlayer = (__bridge LayaVideoPlayer*)obj;
    pVideoPlayer.autoplay = val;
}

void CToObjectVideoPlayerGetReadyState(void* obj, int* val)
{
    LayaVideoPlayer* pVideoPlayer = (__bridge LayaVideoPlayer*)obj;
    *val = pVideoPlayer.readyState;
}

void CToObjectVideoPlayerGetBitmap(void* obj, laya::BitmapData* bitmap)
{
    LayaVideoPlayer* pVideoPlayer = (__bridge LayaVideoPlayer*)obj;
    bool isPaused = [pVideoPlayer isPaused];
    if(isPaused)
        return;
    
    IImageContainer* imageContainer = [pVideoPlayer getImage];
    
    bitmap->reconfigure(imageContainer.width, imageContainer.height, sizeof(int) * 8, ImgType_unknow);
    
    [imageContainer copyToBitmap: bitmap->m_pImageData];
};

void CToObjectVideoPlayerIsFrameUpdate(void* obj, bool* val) {
    LayaVideoPlayer* pVideoPlayer = (__bridge LayaVideoPlayer*)obj;
    *val = [pVideoPlayer isFrameUpdate];
}

void CToObjectCSetCurrentTime(double nCurrentTime)
{
    JCMp3Player* pMp3Player = [conchRuntime GetIOSConchRuntime]->m_pMp3Player;
    [pMp3Player setCurrentTime: nCurrentTime];
}

double CToObjectCGetCurrentTime()
{
    JCMp3Player* pMp3Player = [conchRuntime GetIOSConchRuntime]->m_pMp3Player;
    return [pMp3Player getCurrentTime];
}

double CToObjectCGetDuration()
{
    JCMp3Player* pMp3Player = [conchRuntime GetIOSConchRuntime]->m_pMp3Player;
    return [pMp3Player getDuration];
}
// end video player
//-------------------------------
