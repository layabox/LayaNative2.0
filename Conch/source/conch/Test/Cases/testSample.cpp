#ifdef _TEST_
#include "../JCTestManager.h"
#include <util/JCCommonMethod.h>

using namespace laya;

/*
 * 这里的namespace并没有特殊要求，因为是全局函数，为了防止冲突，才加上一个namespace，对最终的测试没有影响。
*/
namespace urlTest{
	void test1(){
        int ret = hexStringToInt("ff");
        VERIFYEQ(ret, 0xff, "hexStringToInt 0xff");
        int ret1 = hexStringToInt("0xff");
        VERIFYEQ(ret1, 0, "不允许包含0x");
    }

    //这个是主测试函数，要在这里调用所有的测试函数
	void testMain(){
        test1();
	}
    //添加测试用例，第一个参数是主函数，第二个是测试用例的说明，也可以可以用来执行测试用例的过滤。
	ADDTESTCASE(testMain,"简单测试用例");
}
#endif
