
#ifndef _JSC_ARRAYBUFFER_H_
#define _JSC_ARRAYBUFFER_H_

#include <memory>
#include <JavaScriptCore/JSValueRef.h>

namespace laya{

	JSValueRef  createJSAB(char* pData, int len);
    JSValueRef  createJSABAligned(char* pData, int len);
	bool extractJSAB(JSValueRef  ab, char*& data, int& len);

}

#endif
