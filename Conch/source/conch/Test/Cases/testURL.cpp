#ifdef _TEST_
#include "../JCTestManager.h"
#include <util/JCLayaUrl.h>

/*
标准
resolve 函数
如果第二个参数带?则不能被去掉。
TODO
url要先encode么，例如空格怎么办
*/

using namespace laya;

namespace urlTest1 {
    /*
    */
    /*
    void testURL1(){
    JCUrl url;
    //url.parsePath("abc");
    //VERIFYEQ((int)url.m_vPath.size(),1,"");
    url.parse("http://a.b.c/a b.html");//不要非法
    url.parse("http：//a.b.c/a.html");//不要非法

    url.parsePath("./b/c/../d/a.txt");
    VERIFYEQ(url.m_vPath[0],std::string("b"),".被忽略");
    url.parsePath("../b/c/../d/a.txt");
    VERIFYEQ(url.m_vPath[0],std::string(".."),"..不能被忽略");
    VERIFYEQ(url.m_strPath,std::string("../b/d/a.txt"),"");
    url.parsePath("a/b/c/../d/a.txt");
    VERIFYEQ(url.m_strPath,std::string("a/b/d/a.txt"),"");
    url.parsePath("/a/b/c/../d/a.txt");
    VERIFYEQ(url.m_vPath[0],std::string(""),"从根目录开始的路径，第一个路径为空");
    VERIFYEQ(url.m_strPath,std::string("/a/b/d/a.txt"),"");

    url.parse("  http://guo:zh@laya.com:8888/a/b.c/a.html?xx=xx#s");
    VERIFYEQ(url.m_Pass,std::string("zh"),"");
    VERIFYEQ(url.m_Port,std::string("8888"),"");
    url.parse("  http://guo@laya.com/a/b.c/a.html?xx=xx#s");
    VERIFYEQ(url.m_User,std::string("guo"),"");
    VERIFYEQ(url.m_Port,std::string(""),"");

    url.parse("  http://guo:zh@laya.com:8888/a/b.c/a.html?xx=xx#s&p=a/index.html");
    VERIFYEQ(url.m_Query, std::string("?xx=xx#s&p=a/index.html"),"");
    VERIFYEQ((int)url.m_vPath.size(),3,"");

    url.parse("http://a.b.c/../.././p/q/r.a.txt");
    url.parse("http://a.b.c/a/b/../../p/q/r.a.txt");
    url.parse("  http://guo:zh@laya.com/a/b.c/a.html?xx=xx#s");
    VERIFYEQ(url.m_Query, std::string("?xx=xx#s"),"search的内容需要包含？");
    url.parse("http://guo@k.k.com/a.b.c/c/?");
    VERIFYEQ((int)url.m_Query.size(),1,"?后面没有内容，长度应该为1,即?");
    VERIFYEQ(url.m_vPath[url.m_vPath.size()-1],std::string(""),"如果没有文件，则路径的最后一个为空");
    //VERIFYEQ((int)url.m_Query.len,0,"?后面没有内容，长度应该为0");
    url.parse("https://a.b.c/a.txt");
    VERIFYEQ(url.toString(),std::string("https://a.b.c/a.txt"),"需要支持https");

    url.parse("klksdl");
    url.parse("");
    url.parse(NULL);
    url.parse("ftp://k.com/a.txt");
    VERIFYEQ((int)url.m_vPath[0].length(),5,"a.txt的长度应该为5");
    url.parse("file:///c:/a.txt");
    VERIFYEQ( url.m_vPath[0],std::string("c:"),"");
    VERIFYEQ( url.m_vPath[1],std::string("a.txt"),"");
    url.parse("file:///data/data/v.txt");
    VERIFYEQ( url.m_vPath[0],std::string("data"),"有协议的都是从/开始，不用考虑根目录的问题。");
    VERIFYEQ( url.m_vPath[1],std::string("data"),"");
    VERIFYEQ( url.m_vPath[2],std::string("v.txt"),"");
    url.parse("file://10.10.20.31/share/a.txt");
    VERIFYEQ( url.m_Host, std::string("10.10.20.31"),"");
    VERIFYEQ( url.m_vPath[0], std::string("share"),"");
    url.parse("file://guo@10.10.20.31/share/a.txt");
    VERIFYEQ( url.m_User, std::string("guo"),"");
    VERIFYEQ( url.m_Host, std::string("10.10.20.31"),"");

    url.parse("file:///C:\\a\\b\\c.html");
    VERIFYEQ( url.m_vPath[0],std::string("C:"),"要支持\\");
    VERIFYEQ( url.m_vPath[1],std::string("a"),"要支持\\");
    VERIFYEQ( url.m_vPath[2],std::string("b"),"要支持\\");
    url.parse("file:///C:\\c.html");
    VERIFYEQ( url.m_vPath[0],std::string("C:"),"要支持\\");
    VERIFYEQ( url.m_vPath[1],std::string("c.html"),"要支持\\");
    url.parse("file:///C:\\a\\\\b\\c.html");
    VERIFYEQ( url.m_vPath[0],std::string("C:"),"要支持\\\\");
    VERIFYEQ( url.m_vPath[1],std::string("a"),"要支持\\\\");
    VERIFYEQ( url.m_vPath[2],std::string("b"),"要支持\\\\");


    url.parse("c:/a/b/c.txt");
    VERIFYEQ(url.m_strPath,std::string("file:///c:/a/b"),"需要支持直接写目录没有file://的url");
    url.parse("/a/b/c.txt");
    VERIFYEQ(url.m_strPath,std::string("file:///a/b"),"需要支持直接写目录没有file://的url");
    url.parse("c:/a.txt");
    VERIFYEQ(url.m_strPath,std::string("file:///c:"),"需要支持直接写目录没有file://的url");
    url.parse("/a.txt");
    VERIFYEQ(url.m_strPath,std::string("file:///"),"需要支持直接写目录没有file://的url");
    }

    void testResolve1(){
    std::string ret;
    JCUrl url;
    url.parse("http://laya.com:8888/a/b/c/d.html");
    ret = url.resolve("d/e/f.html");
    VERIFYEQ( ret, std::string("http://laya.com:8888/a/b/c/d/e/f.html"),"");
    ret = url.resolve("/d/e/f.html");
    VERIFYEQ( ret, std::string("http://laya.com:8888/d/e/f.html"),"");

    ret = url.resolve("f.html");
    VERIFYEQ( ret, std::string("http://laya.com:8888/a/b/c/f.html"),"");

    url.parse("http://laya.com:8888/d.html");
    ret = url.resolve("f.html");
    VERIFYEQ( ret, std::string("http://laya.com:8888/f.html"),"");

    url.parse("file:///c:/a/b.html");
    ret = url.resolve("c.html");
    VERIFYEQ( ret, std::string("file:///c:/a/c.html"),"");

    url.parse("file:///c:/b.html");
    ret = url.resolve("c.html");
    VERIFYEQ( ret, std::string("file:///c:/c.html"),"");

    url.parse("file:///c:/b.html");
    ret = url.resolve("d:/c.html");
    VERIFYEQ( ret, std::string("file:///d:/c.html"),"");

    url.parse("file:///c:/a/b.html");
    ret = url.resolve("/c.html");
    VERIFYEQ( ret, std::string("file:///c:/c.html"),"");

    url.parse("file:///c:/a/b.html");
    ret = url.resolve("../c.html");
    VERIFYEQ( ret, std::string("file:///c:/a/../c.html"),"第一级就是..的现在还没做。应该不会有什么影响吧？");

    url.parse("file:///c:/a/b.html");
    ret = url.resolve("file:///d:/c.html");
    VERIFYEQ( ret, std::string("file:///d:/c.html"),"");

    url.parse("c:/a/b.txt");
    ret = url.resolve("c.txt");
    VERIFYEQ( ret, std::string("file:///c:/a/c.txt"),"");

    url.parse("file:///C:\\c.html");
    ret = url.resolve("file:///C:\\a\\\\b\\c.html");
    VERIFYEQ( ret , std::string("file:///C:/a/b/c.html"),"即使是绝对路径，也要整理一下");

    url.parse("http://a.b.c/a/b.html");
    ret = url.resolve("http://a.b/a.html?a=b");
    VERIFYEQ(ret, std::string("http://a.b/a.html?a=b"),"");

    ret = url.resolve("c.html?b=c");
    VERIFYEQ( ret, std::string("http://a.b.c/a/c.html?b=c"),"");
    }
    void testResolve2(){
    }

    void testJSLocation(){
    }

    void testMain(){
    //这个执行的时候，会调用所有的测试函数
    testURL1();
    testResolve1();
    testResolve2();
    testJSLocation();
    }

    ADDTESTCASE(testMain,"URL相关测试");
    */
}
#endif
