#import <UIKit/UIKit.h>
//文字相关的函数
//-----------------------------------------------------------------

void CToObjectCLog( const char* szFormat,...)
{
    va_list args;
    va_start(args, szFormat);
    NSString* nsFormat = [NSString stringWithUTF8String:szFormat];
    NSLogv( nsFormat, args);
    va_end(args);
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
void alert(const char* fmt, ...)
{
    char buf[1024];
    char* pBuf = NULL;
    va_list args;
    va_start(args, fmt);
    int len = vsprintf(buf, fmt, args);
    if (len < 0) {
        pBuf = new char[4096];
        len = vsprintf(pBuf, fmt, args);
    }
    va_end(args);
    CToObjectCLog(pBuf ? pBuf : buf);
    if (pBuf)
    {
        delete[] pBuf;
    }
}
