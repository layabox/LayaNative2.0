/**
@file			JSInterface.cpp
@brief
@author			wyw
@version		1.0
@date			2014_7_29
*/

#include "JSInterface.h"
#ifdef JS_JSC
    //#include <JavaScriptCore/JSBasePrivate.h>
#endif

namespace laya 
{
    void AdjustAmountOfExternalAllocatedMemory(int p_nMemorySize)
    {
#ifdef JS_V8
        v8::Isolate::GetCurrent()->AdjustAmountOfExternalAllocatedMemory(p_nMemorySize);
#elif JS_JSVM
        int64_t result;
        auto env = ENV;
        JSVM_Status status;
        JSVM_API_CALL(status, env, OH_JSVM_AdjustExternalMemory(env, p_nMemorySize, &result));
#elif JS_JSC
        //JSReportExtraMemoryCost(__TlsData::GetInstance()->GetCurContext(), p_nMemorySize);
#endif
    }
}



//-----------------------------END FILE--------------------------------
