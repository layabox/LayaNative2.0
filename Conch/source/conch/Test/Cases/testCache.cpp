#ifdef _TEST_
#include "../JCTestManager.h"
#include <util/JCCommonMethod.h>
#include <downloadCache/JCServerFileCache.h>
#include <resource/JCFileResManager.h>
#include <downloadMgr/JCDownloadMgr.h>
#include <downloadMgr/JCHttpHeader.h>
#include <fileSystem/JCFileSystem.h>
#include <chrono>

using namespace std::chrono;
/*
    . 没有缓存的测试
        
    . 资源在dcc表中的测试
        校验正确
        校验错误

    . 第一次读取，没有内容
      更新一个不在dcc的特定扩展名的文件，也保存到缓存目录
      再次读取指定文件，能返回内容，且知道不属于dcc
    
    . 非dcc文件，每次启动都会重新下载。例如sessionid
    . 非dcc文件只有特定文件才要放到缓存中：要根据扩展名+文件内容
    . 一个dcc文件转成非dcc文件
*/

using namespace laya;

/*
 * 这里的namespace并没有特殊要求，因为是全局函数，为了防止冲突，才加上一个namespace，对最终的测试没有影响。
*/
namespace cacheTest{
    JCFileRes* createFileRes(const char* pFile, const char* pData, int len) {
        JCServerFileCache svc;
        svc.setCachePath("d:/temp/cache/");
        svc.switchToApp("http://localhost/app/");
        JCFileRes* pRes = new JCFileRes(nullptr,nullptr);
        pRes->m_nLocalFileID = svc.hashURLFull(pFile);
        pRes->m_nLength = len;
        pRes->m_pBuffer = std::shared_ptr<char>(new char[len], std::default_delete<char[]>());//  std::shared_array<char>(new char[len]);
        return pRes;
    }
    void testSessionID() {
        /*
        JCServerFileCache svc;
        svc.setCachePath("d:/temp/cache/");
        svc.switchToApp("http://localhost/app/");
        svc.rmSessionID();
        svc.switchToApp("http://localhost/app/");
        int id1 = svc.getSessionID();
        VERIFYEQ(id1, 1, "第一次，id为1");
        svc.switchToApp("http://localhost/app/");
        id1 = svc.getSessionID();
        VERIFYEQ(id1, 2, "第二次，id为2");
        */
    } 
    void testLoadCached() {
        JCFileRes res(nullptr,nullptr);
        JCBuffer buff;
        bool bret = res.loadFromCache(buff,false);
        VERIFYEQ(bret, false, "");

    }
	void testSessionFileGet(){
        JCServerFileCache svc;
        svc.setCachePath("d:/temp/cache/");
        svc.switchToApp("http://localhost/app/");
        //没有dcc的情况下，无法获得资源
        unsigned int fid = svc.hashURLFull("http://localhost/app/test.png");
        VERIFYEQ(fid, 0xf7859e1b, "计算url的hash");
        svc.clearAllCachedFile();
        JCSharedBuffer buf;
        unsigned int chksum = 0;
        bool bret = svc.load(fid, chksum, buf,false,true);
        VERIFYEQ(bret, false, "没有dcc，第一次就没有缓存文件");

        JCFileRes *fres = new JCFileRes(nullptr,nullptr);
        fres->m_nLocalFileID = 0xf7859e1b;
        fres->m_pBuffer = std::shared_ptr<char>(new char[100],std::default_delete<char[]>()) ;
        fres->m_nLength = 100;
        chksum = 0;
        //保存临时文件。这里调用 onFileDownloaded 就是直接保存，不判断是否应该保存。
        svc.updateAFile(fres->m_nLocalFileID, fres->m_pBuffer.get(),fres->m_nLength, chksum,false,0,false);
        //再次加载，应该能获得session文件
        JCBuffer bufsession;
        bool b = svc.load(fres->m_nLocalFileID, bufsession);
        VERIFYEQ(b, true, "session文件也要保存");
        //再次进入，还要重新下载
        svc.switchToApp("http://localhost/app/");
        b = svc.load(fres->m_nLocalFileID, bufsession);
        VERIFYEQ(b, false, "上次保存的session文件不能在这次使用。");

        svc.updateAFile(fres->m_nLocalFileID, fres->m_pBuffer.get(), fres->m_nLength, chksum, false, 0, false);
        b = svc.load(fres->m_nLocalFileID, bufsession);
        VERIFYEQ(b, true, "再次更新的可以使用。");
    }

    /*
        在什么情况下应该保存session文件
    */
    void testSessionFileUpdate() {
        class TestDownloadMgr :public JCDownloadMgr {
        public:
            virtual void download(const char* p_pszURL, int p_nPriority, 
                    onProgressFunc p_ProgCb, 
                    onEndFunc p_CompleteCb, 
                    int p_nOptTimeout) {
                JCBuffer buf;
                buf.create(100);
                memset(buf.m_pPtr, 1, 100);
                buf.m_pPtr[1] = 2;//校验值0x300
                int aa = JCCachedFileSys::getChkSum(buf.m_pPtr,100);
                p_CompleteCb(buf, "", "",0,200,"");
                int a = 0;
            }
        };

        JCServerFileCache svc;
        svc.setCachePath("d:/temp/cache/");
        svc.switchToApp("http://localhost/app/");
        svc.clearAllCachedFile();
        JCDownloadMgr* pDmgr = new TestDownloadMgr();
        JCFileResManager fmgr(pDmgr);
        /*
            HELP
                Q: 怎么设置需要缓存的文件？
                A: 现在只能通过扩展名来判断文件是否需要缓存。如果文件没有在dcc中管理，又希望能缓存，
                就需要设置m_vExtNeedSave。注意这种缓存是非持久的，每次启动后都会丢失。
        */
        fmgr.m_vExtNeedSave = { ".png",".jpg"};
        fmgr.setFileCache(&svc);

        JCFileRes* pFR = new JCFileRes(pDmgr,&fmgr);
        pFR->load("http://localhost/app/test.png",nullptr);
        VERIFYEQ((pFR->m_nLastAction == JCFileRes::UPDATECACHE), true, "png文件要保存。");
        pFR->load("http://localhost/app/test.jpg",nullptr);
        //注意：这里是测试下载，会立即完成，所以可以立即查询本地文件，实际项目中需要在真正下载完了才能查询。
        VERIFYEQ((pFR->m_nLastAction == JCFileRes::UPDATECACHE), true, "jpg文件要保存。");
        pFR->load("http://localhost/app/test.png",nullptr);
        VERIFYEQ((pFR->m_nLastAction == JCFileRes::LOADFROMCACHE), true, "第二次要从缓存取。");
        pFR->load("http://localhost/app/test.png1",nullptr);
        VERIFYEQ((pFR->m_nLastAction == JCFileRes::UPDATECACHE), true, "png1文件不要保存。");
        //pFR->getlo
        //外部版本号控制
        JCFileRes::s_strExtVersion = "ver";
        pFR->load("http://localhost/app/testev.png?ver=10", nullptr);
        VERIFYEQ((pFR->m_nLastAction == JCFileRes::UPDATECACHE), true, "带版本号的文件第一次要保存。");
        pFR->load("http://localhost/app/testev.png?ver=10", nullptr);
        VERIFYEQ((pFR->m_nLastAction == JCFileRes::LOADFROMCACHE), true, "带版本号的文件第2次要读缓存。");
        pFR->load("http://localhost/app/testev.png?ver=1", nullptr);
        VERIFYEQ((pFR->m_nLastAction == JCFileRes::UPDATECACHE), true, "版本号变了要重新下载。");
        JCBuffer ret;
        pFR->loadFromCache(ret, false);
        VERIFYEQ(ret.m_nLen != 0, true, "忽略校验值要能获得内容");

        int a = 0;

        pFR->m_strURL = "";
    }

    void testDCC() {
        class TestDownloadMgr :public JCDownloadMgr {
        public:
            virtual void download(const char* p_pszURL, int p_nPriority, 
                    onProgressFunc p_ProgCb, 
                    onEndFunc p_CompleteCb, 
                    int p_nOptTimeout) {
                JCBuffer buf;
                buf.create(100);
                memset(buf.m_pPtr, 1, 100);
                buf.m_pPtr[1] = 2;
                //现在的校验值是0x300
                p_CompleteCb(buf, "", "",0,200,"");
                int a = 0;
            }
        };

        JCDownloadMgr* pDmgr = new TestDownloadMgr();
        JCServerFileCache svc;
        svc.setCachePath("d:/temp/cache");
        svc.switchToApp("http://localhost/app/");
        svc.clearAllCachedFile();
        std::string dccf = svc.getDccFile();
        const char* buf = "f7859e1b 300\n"  //
            "2 2\n"
            "3 3\n";
        int buflen = strlen(buf);
        bool bwret = writeFileSync1(dccf.c_str(), (char*)buf,buflen,JCBuffer::utf8);
        VERIFYEQ(bwret, true, "");
        int dccnum = svc.reloadDccFile();
        VERIFYEQ(dccnum, 3, "");
        JCFileResManager fmgr(pDmgr);
        fmgr.setFileCache(&svc);
        JCFileRes* pFR = new JCFileRes(pDmgr, &fmgr);
        pFR->load("http://localhost/app/test.png",nullptr);
        VERIFYEQ((pFR->m_nLastAction == JCFileRes::UPDATECACHE), true, "dcc中的文件必须缓存");
        //第二次要能直接读缓存。
        pFR->load("http://localhost/app/test.png",nullptr);
        VERIFYEQ((pFR->m_nLastAction == JCFileRes::LOADFROMCACHE), true, "dcc第二次要能直接读缓存");
    }

    bool end = false;
    std::string retHeader = "";
    void onEnd1(JCBuffer& buff, const std::string& locaddr, const std::string& svaddr,
        int curlret, int httpret, const std::string& header) {
        end = true;
        retHeader = header;
    }
    void testDownload() {
        JCDownloadMgr* pNL = JCDownloadMgr::getInstance();
        pNL->init(3);
        pNL->download("http://www.baidu.com/index.html",0,
            pNL->defProgressFunc,
            onEnd1,
            0);
        WaitUtil(&end, 10000);
        bool lenok = retHeader.length() > 0;
        VERIFYEQ(lenok, true, "下载必须能得到header。");
        end = false;
        retHeader = "";

        pNL->download("https://www.baidu.com/index.html", 0,
            pNL->defProgressFunc,
            onEnd1,
            0);
        WaitUtil(&end, 10000);
        lenok = retHeader.length() > 0;
        VERIFYEQ(lenok, true, "下载https必须能得到header。");
        end = false;
        retHeader = "";

        pNL->getHeader("http://www.baidu.com/index.html", onEnd1, 0, 0);
        WaitUtil(&end, 10000);
        end = false;
        retHeader = "";
        JCDownloadMgr::delInstance();
    }

    void testPost() {
        JCDownloadMgr* pNL = JCDownloadMgr::getInstance();
        pNL->init(3);
        //while (true) {//测试 内存
            pNL->postData("http://10.10.20.19:9999/payapp", "1234567890", 10, onEnd1);
            WaitUtil(&end, 10000);
            end = false;
            retHeader = "";
        //}
        JCDownloadMgr::delInstance();
    }

    int onProg1(unsigned int total, unsigned int now, float speed) {
        if (speed == 0)return 0;

        printf("%d/%d:%d%% %fK\n",now,total,(int)(now*1.0f/total*100),speed);
        return 0;
    }

    void testDownloadBigfile() {
        JCDownloadMgr* pNL = JCDownloadMgr::getInstance();
        pNL->init(3);
        int st = tmGetCurms();
        pNL->downloadBigFile("http://ldc.layabox.com/download/LayaAir/runtime/download.zip", "d:/temp/download.zip", 
            onProg1, onEnd1, 0, 999999);
        WaitUtil(&end, 100000);
        int dt = tmGetCurms() - st;

        int a = 0;
    }

    void testHeader() {
        char* strHeader = R"(HTTP/1.1 200 OK
Server: bfe/1.0.8.14
Date: Fri, 02 Sep 2016 10:54:06 GMT
Content-Type: text/html
Last-Modified: Mon, 13 Jun 2016 02:50:04 GMT
Connection: Keep-Alive
Cache-Control: max-age=100, private, no-cache, no-store, proxy-revalidate, no-transform
Pragma: no-cache
Content-Encoding: gzip

)";
        RequestHeader Header;
        JCHttpHeader::request_header_parse(&Header, strHeader, strlen(strHeader));

        JCHttpHeader httph(strHeader);
        JCHttpHeader::CacheContrl* pCC = httph.getCacheContrl();
       
        VERIFYEQ(pCC->maxage, 100, "max-age");
        int a = 10;
    }

    void testTime() {
        auto t1 = steady_clock::now();
        int a = 10;
    }

    void testImgCache() {
        class TestDownloadMgr :public JCDownloadMgr {
        public:
            virtual void download(const char* p_pszURL, int p_nPriority,
                onProgressFunc p_ProgCb,
                onEndFunc p_CompleteCb,
                int p_nOptTimeout) {
                JCBuffer buf;
                buf.create(100);
                char* pBuf = buf.m_pPtr;
                pBuf[0] = 0x89; pBuf[1] = 0x50; pBuf[2] = 0x4e; pBuf[3] = 0x47;
                pBuf[4] = 0x0d; pBuf[5] = 0x0a; pBuf[6] = 0x1a; pBuf[7] = 0x0a;
                p_CompleteCb(buf, "", "", 0, 200, "");
                int a = 0;
            }
        };

        JCServerFileCache svc;
        svc.setCachePath("d:/temp/cache/");
        svc.switchToApp("http://localhost/app/");
        svc.clearAllCachedFile();

        JCDownloadMgr* pDmgr = new TestDownloadMgr();
        JCFileResManager fmgr(pDmgr);
        fmgr.setFileCache(&svc);
        JCServerFileCache::s_bSessionCacheType = JCServerFileCache::CT_AllwaysReload;
        JCFileRes* pFR = new JCFileRes(pDmgr, &fmgr);
        pFR->load("http://localhost/app/test", nullptr);
        VERIFYEQ((pFR->m_nLastAction == JCFileRes::UPDATECACHE), true, "png类型的图片文件要保存。");
        pFR->load("http://localhost/app/test", nullptr);
        VERIFYEQ((pFR->m_nLastAction == JCFileRes::UPDATECACHE), true, "临时图片文件再次请求还要下载。");
        JCServerFileCache::s_bSessionCacheType = JCServerFileCache::CT_ValidInCurProcess;
    }

    void testreplaceurl() {
        JCDownloadMgr* pNL = JCDownloadMgr::getInstance();
        pNL->init(1);
        std::string  strf = pNL->getFinalUrl("http://203.107.1.1/145720/d?host=httpdns.cs.nxagame.com");
        VERIFYEQ(strf, std::string("http://203.107.1.1/145720/d?host=httpdns.cs.nxagame.com"), "简单情况下不变");

        strf = pNL->getFinalUrl("http://203.107.1.1/145720/d.png?host=httpdns.cs.nxagame.com");
        VERIFYEQ(strf, std::string("http://203.107.1.1/145720/d.png?host=httpdns.cs.nxagame.com"), "有扩展名的简单情况下不变");

        pNL->setFinalReplacePath("vel", "6");
        strf = pNL->getFinalUrl("http://203.107.1.1/145720/d?host=httpdns.cs.nxagame.com");
        VERIFYEQ(strf, std::string("http://203.107.1.1/145720/d?host=httpdns.cs.nxagame.com"), "设置替换的情况下不变");

        strf = pNL->getFinalUrl("http://203.107.1.1/145720/d.png?host=httpdns.cs.nxagame.com");
        VERIFYEQ(strf, std::string("http://203.107.1.1/145720/d.png?host=httpdns.cs.nxagame.com"), "设置替换且有扩展名的情况下不变");

        pNL->setFinalReplacePath("vel", "6");
        strf = pNL->getFinalUrl("http://203.107.1.1/145720/vel/d?host=httpdns.cs.nxagame.com");
        VERIFYEQ(strf, std::string("http://203.107.1.1/145720/6/d?host=httpdns.cs.nxagame.com"), "设置替换的情况下不变");

        strf = pNL->getFinalUrl("http://203.107.1.1/145720/vel/d.png?host=httpdns.cs.nxagame.com");
        VERIFYEQ(strf, std::string("http://203.107.1.1/145720/6/d.png?host=httpdns.cs.nxagame.com"), "");

        JCDownloadMgr::delInstance();
    }

    //这个是主测试函数，要在这里调用所有的测试函数
	void testMain(){
        testreplaceurl();
        testHeader();
        testDownload();
        testPost();
        testDownloadBigfile();

        testDCC();
        testLoadCached();
        testSessionID();
        testSessionFileUpdate();
        testSessionFileUpdate();
        testImgCache();
	}
    //添加测试用例，第一个参数是主函数，第二个是测试用例的说明，也可以可以用来执行测试用例的过滤。
	ADDTESTCASE(testMain,"cache测试");
}
#endif
