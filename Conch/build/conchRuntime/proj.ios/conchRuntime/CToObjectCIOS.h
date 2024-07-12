/**
 @file			CToObjectCIOS.h
 @brief         CToObjectCIOS
 @author		wyw
 @version		1.0
 @date			2014_8_26
 @company       JoyChina
 */

//process用的，创建、关闭等都需要调用到主线程，线程传递用到的参数类
//-----------------------------------------------------------------
@interface postCmdToMainThreadParam : NSObject
{
@public
    int cmd;
    int nParam1;
    int nParam2;
}
-(id)init:(int)__cmd _p1:(int)_nParam1 _p2:(int)_nParam2;
@end
//-----------------------------------------------------------------

//editBox的相关函数，因为涉及到ui界面，所以也投递到主线程，线程传递用到的参数类
@interface postCmdToEditBoxParam : NSObject
{
@public
    int cmd;
    int nParam1;
    int nParam2;
    bool bParam2;
    std::string sParam3;
}
-(id)init:(int)__cmd _p1:(int)_nParam1 _p2:(int)_nParam2 _p3:(bool)_bParam2 _p4:(const char*)_sParam3;
@end
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//因为涉及到ui界面，所以要投递到主线程，线程传递用到的参数类
//-----------------------------------------------------------------
@interface postCmdParam : NSObject
{
@public
    bool bParam;
    int  iParam;
    float fParam;
    std::string sParam;
}
-(id)init:(bool)_bParam i:(int)_iParam f:(float)_fParam s:(const char*)_sParam;
@end
//-----------------------------------------------------------------

//iOS反射
//-----------------------------------------------------------------
@interface postCmdToReflectionParam : NSObject
{
@public
    int  iObjID;
    bool bSync;
    std::string sClsName;
    std::string sMethodName;
    std::string sParamStr;
}
-(id)init:(int)objid Sync:(bool)sync Class:(const char*)cls Method:(const char*)method Param:(const char*)param;
@end
//-----------------------------------------------------------------


//stringParam
@interface postCmdStringParam : NSObject
{
@public
    std::string sParam;
}
-(id)init:(const char*)_sParam;
@end


//3 stringParam
@interface postCmd3StringParam : NSObject
{
@public
    std::string sParam0;
    std::string sParam1;
    std::string sParam2;
}
-(id)init:(const char*)_sParam0 _p1:(const char*)_sParam1 _p2:(const char*)_sParam2 ;
@end

//2 float
@interface postCmd2FloatParam : NSObject
{
@public
    float fParam0;
    float fParam1;
}
-(id)init:(float)_fParam0 _p1:(float)_fParam1;
@end

//4 int
@interface postCmd4IntParam : NSObject
{
@public
    int iParam0;
    int iParam1;
    int iParam2;
    int iParam3;
}
-(id)init:(int)_iParam0 _p1:(int)_iParam1 _p2:(int)_iParam2 _p3:(int)_iParam3;
@end

//4 int
@interface postCmdString4IntParam : NSObject
{
@public
    std::string sBuffer;
    int iParam0;
    int iParam1;
    int iParam2;
    int iParam3;
    bool bParam4;
}
-(id)init:(const char*)s _p0:(int)_iParam0 _p1:(int)_iParam1 _p2:(int)_iParam2 _p3:(int)_iParam3 _p4:(bool)_bParam4;
@end

//函数调用ObjectC，好多静态方法都在这
//-----------------------------------------------------------------
@interface CToObjectCIOS : NSObject
{
}
+(void) postCmdToMainThread:(id)param;
+(void) postCmdToEditBox:(id)param;
+(char*)fontDataForCGFont:(CGFontRef)cgFont size:(long&)p_nSize;
+(void) setScreenWakeLock:(id)param;
+(NSString*) callMethod:(id)param;
+(void) captureScreen:(id)param;
//-----------------------------------------------------------------

void reflectionCallback(const std::string& jsonret);
@end

