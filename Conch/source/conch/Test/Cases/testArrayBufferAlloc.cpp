#ifdef _TEST_
#include "../JCTestManager.h"
#include "../../JSWrapper/JSInterface/V8/JSArrayBuffer.h"
using namespace laya;

#ifdef JS_V8
/*
 * 这里的namespace并没有特殊要求，因为是全局函数，为了防止冲突，才加上一个namespace，对最终的测试没有影响。
*/
namespace arrayBufferAllocTest{
	void test1(){
        /*
        ArrayBufferAllocator* pAlloc = ArrayBufferAllocator::getInstance();
        void* pPtr = pAlloc->Allocate(100);
        int idx = pAlloc->_testGetID(pPtr);
        VERIFYEQ(idx, 0, "ArrayBuffer分配后，要保存id");
        void* pPtr1 = pAlloc->Allocate(10);
        VERIFYEQ(pAlloc->_testGetID(pPtr1), 1, "arraybuffer的id要正确维护");
        pAlloc->Free(pPtr,100);
        VERIFYEQ(pAlloc->_testGetID(pPtr1), 0, "删除后，需要修改被挪动的buffer的id");
        void* pPtr3 = pAlloc->Allocate(100);
        VERIFYEQ(pAlloc->_testGetID(pPtr3), 1, "有删除后的id分配。");
        pAlloc->FreeAllAlive();
        VERIFYEQ(pAlloc->getAliveBufferNum(), 0, "FreeAllAlive.");
        */
    }

    //这个是主测试函数，要在这里调用所有的测试函数
	void testMain(){
        test1();
	}
    //添加测试用例，第一个参数是主函数，第二个是测试用例的说明，也可以可以用来执行测试用例的过滤。
	ADDTESTCASE(testMain,"ArrayBufferAllocTests");
}
#endif
#endif
