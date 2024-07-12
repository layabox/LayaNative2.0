#ifndef _TOUCHESNAPI_H_
#define _TOUCHESNAPI_H_

#include "conch/JCScriptRuntime.h"

using namespace laya;

extern "C"
{
    void nativeHandleTouchDown(int id, int x, int y);
    void nativeHandleTouchUp(int id, int x, int y);
    void nativeHandleTouchMove(int id, int x, int y);
    void nativeHandleTouchCancel(int id, int x, int y);
}

#endif