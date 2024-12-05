
#ifndef _JSVM_ARRAYBUFFER_H_
#define _JSVM_ARRAYBUFFER_H_

#include <ark_runtime/jsvm.h>
#include <ark_runtime/jsvm_types.h>
#include <vector>

/*
	如果有c这边的ArrayBufferView引用这个ArrayBuffer的话，需要增加引用计数，不能直接删除
*/

namespace laya {
	JSVM_Value createJSAB(char* pData, int len);
    JSVM_Value createJSABAligned(char* pData, int len);
	bool extractJSAB(JSVM_Value ab, char*& data, int& len);
}

#endif
