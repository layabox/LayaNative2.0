/**
@file			V8Util.h
@brief			
@author			James
@version		1.0
@date			2017_11_29
*/

#ifndef __V8Util_H__
#define __V8Util_H__

#include <v8.h>

/** 
 * @brief 
*/
namespace laya 
{
    class V8Util
    {
    public:
        
        static bool extractJSAB(v8::Local<v8::Value> jsval, char*& data, int& len);
        static void SET_PROTOTYPE_METHOD(v8::Local<v8::FunctionTemplate> recv, const char* name, v8::FunctionCallback callback);
    };
}
//------------------------------------------------------------------------------


#endif //__V8Util_H__

//-----------------------------END FILE--------------------------------