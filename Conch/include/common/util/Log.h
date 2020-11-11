/**
 @file			JSLog.h
 @brief
 @author		wyw
 @version		1.0
 @date			2013_11_12
 */

#ifndef __Log_H__
#define __Log_H__
#include <stdio.h>
#include <stdint.h>
#pragma warning(disable:4996)
#ifdef WIN32
    #pragma execution_character_set("utf-8")
#endif
//通用的日志函数。
enum LogLevel {
    Warn,
    Error,
    Debug,
    Info,
    Runtime,
};
extern void(*gLayaLog)(int level, const char* file, int line,  const char* fmt,...);
//如果知道没有参数就不用做%的转换了。
extern void(*gLayaLogNoParam)(int level, const char* file, int line, const char* msg);
extern void(*gLayaLogBin)(int level, const char* file, int line, void* pData, int len);
extern int g_nDebugLevel;
void alert(const char* fmt, ...);

#ifdef WEBASM
    #define LOG_TAG "LayaBox"
    #define LOGI(...) {printf(__VA_ARGS__);printf("\n");}
    #define LOGW(...) {printf(__VA_ARGS__);printf("\n");}
    #define LOGE(...) {printf(__VA_ARGS__);printf("\n");}
#else
    #ifdef __APPLE__
        void CToObjectCLogI( const char* szFormat,...);
        void CToObjectCLogW(const char* szFormat, ...);
        void CToObjectCLogE(const char* szFormat, ...);
        void CToObjectCLogIExt(const char* str);
        #define LOGIExt(str) {if(g_nDebugLevel >= 3){CToObjectCLogIExt(str);}}
        #define LOGI(...) {if(g_nDebugLevel >= 3){CToObjectCLogI(__VA_ARGS__);}}
        #define LOGW(...) {if(g_nDebugLevel >= 2){CToObjectCLogW(__VA_ARGS__);}if(g_nDebugLevel >= 5){alert(__VA_ARGS__);}}
        #define LOGE(...) {if(g_nDebugLevel >= 1){CToObjectCLogE(__VA_ARGS__);}if(g_nDebugLevel >= 4){alert(__VA_ARGS__);}}
    #elif LINUX
	    #define LOGI(...) { if(g_nDebugLevel >= 3){if(gLayaLog){gLayaLog(Info,__FILE__,__LINE__,__VA_ARGS__);}else{ printf(__VA_ARGS__);printf("\n");}}}
    #define LOGW(...) { if(g_nDebugLevel >= 2){if(gLayaLog){gLayaLog(Warn,__FILE__,__LINE__,__VA_ARGS__);}else{ printf(__VA_ARGS__);printf("\n");}}if(g_nDebugLevel >= 5){alert(__VA_ARGS__);}}
    #define LOGE(...) { if(g_nDebugLevel >= 1){if(gLayaLog){gLayaLog(Error,__FILE__,__LINE__,__VA_ARGS__);}else{ printf(__VA_ARGS__);printf("\n");}}if(g_nDebugLevel >= 4){alert(__VA_ARGS__);}}
    #elif ANDROID
        #include <jni.h>
        #include <android/log.h>
        #define LOG_TAG "LayaBox"
        #define LOGI(...) {if(g_nDebugLevel >= 3){if(gLayaLog){gLayaLog(Info,__FILE__,__LINE__,__VA_ARGS__);}else{__android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__);}}}
        #define LOGW(...) {if(g_nDebugLevel >= 2){if(gLayaLog){gLayaLog(Warn,__FILE__,__LINE__,__VA_ARGS__);}else{__android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__);}}if(g_nDebugLevel >= 5){alert(__VA_ARGS__);}}
        #define LOGE(...) {if(g_nDebugLevel >= 1){if(gLayaLog){gLayaLog(Error,__FILE__,__LINE__,__VA_ARGS__);}else{__android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__);}}if(g_nDebugLevel >= 4){alert(__VA_ARGS__);}}
    #elif WIN32
        #define LOGI(...) { if(g_nDebugLevel >= 3){if(gLayaLog){gLayaLog(Info,__FILE__,__LINE__,__VA_ARGS__);}else{ printf(__VA_ARGS__);printf("\n");}}}
        #define LOGW(...) { if(g_nDebugLevel >= 2){if(gLayaLog){gLayaLog(Warn,__FILE__,__LINE__,__VA_ARGS__);}else{ printf(__VA_ARGS__);printf("\n");}}if(g_nDebugLevel >= 5){alert(__VA_ARGS__);}}
        #define LOGE(...) { if(g_nDebugLevel >= 1){if(gLayaLog){gLayaLog(Error,__FILE__,__LINE__,__VA_ARGS__);}else{ printf(__VA_ARGS__);printf("\n");}}if(g_nDebugLevel >= 4){alert(__VA_ARGS__);}}
    #endif
#endif
#endif //__Log_H__


//-----------------------------END FILE--------------------------------
