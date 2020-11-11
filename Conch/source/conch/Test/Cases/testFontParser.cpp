#ifdef _TEST_
#include "../JCTestManager.h"
#include <util/JCCommonMethod.h>
#include <resource/text/JCFontInfo.h>

using namespace laya;

/*
 * 这里的namespace并没有特殊要求，因为是全局函数，为了防止冲突，才加上一个namespace，对最终的测试没有影响。
*/
namespace fontParserTest{
	void test1(){
        JCFontInfo fi;
        fi.parse("normal 100 32px Arial 1 #00ff00 1 #ff0000");
        bool bfok = (strcmp(fi.m_sFamily, "Arial")==0);
        VERIFYEQ(bfok, true, "");
        VERIFYEQ((int)fi.m_nWeight, 100, "");
        VERIFYEQ(fi.m_nBorderSize, 1, "");

        fi.parse("normal 100 16px Arial b 1  #ff0000 1 #00ff00 ");
        VERIFYEQ((int)fi.m_nWeight, 100, "");
        VERIFYEQ((int)fi.m_nFontSize, 16, "");
        fi.parse(" ");
        fi.parse("");
        fi.parse("normal 1300 28px Microsoft_YaHei 1 #444444 0");
        VERIFYEQ((int)fi.m_nWeight, 400, "weight必须是固定的几种。");
        fi.parse("normal normal 18px 宋体 2 #000000 0 #000000");
        VERIFYEQ(strcmp(fi.m_sFamily, "宋体"), 0, "");
        VERIFYEQ((int)fi.m_nFontSize, 18, "");
    }

    //这个是主测试函数，要在这里调用所有的测试函数
	void testMain(){
        test1();
	}
    //添加测试用例，第一个参数是主函数，第二个是测试用例的说明，也可以可以用来执行测试用例的过滤。
	ADDTESTCASE(testMain,"testFontParser");
}
#endif
