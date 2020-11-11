/**
 @file			JSLog.h
 @brief
 @author		James
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

#ifdef __APPLE__
    void CToObjectCLogI( const char* szFormat,...);
    void CToObjectCLogW(const char* szFormat, ...);
    void CToObjectCLogE(const char* szFormat, ...);
    void CToObjectCLogIExt(const char* str);
    #define LOGIExt(str) {CToObjectCLogIExt(str);}
    #define LOGI(...) {CToObjectCLogI(__VA_ARGS__);}
    #define LOGW(...) {CToObjectCLogW(__VA_ARGS__);}
    #define LOGE(...) {CToObjectCLogE(__VA_ARGS__);}
#define TEST_WEBGLPLUS_LOG(...)
    //#define TEST_WEBGLPLUS_LOG(...) {CToObjectCLogI(__VA_ARGS__);}
#elif ANDROID
    #include <jni.h>
    #include <android/log.h>
    #define LOG_TAG "LayaBox"
    #define LOGI(...) {__android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__);}
    #define LOGW(...) {__android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__);}
    #define LOGE(...) {__android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__);}
    #define TEST_WEBGLPLUS_LOG(...)
    //#define TEST_WEBGLPLUS_LOG(...) {__android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__);}
#elif WIN32
    #define LOGI(...) { printf(__VA_ARGS__);printf("\n");}
    #define LOGW(...) { printf(__VA_ARGS__);printf("\n");}
    #define LOGE(...) { printf(__VA_ARGS__);printf("\n");}
    #define TEST_WEBGLPLUS_LOG(...)
    //#define TEST_WEBGLPLUS_LOG(...) { printf(__VA_ARGS__);printf("\n");}
#endif

#endif //__Log_H__


//-----------------------------END FILE--------------------------------
