/**
@file			JSConchConfig.h
@brief			
@author			hugao
@version		1.0
@date			2016_5_18
*/

#ifndef __JSConchConfig_H__
#define __JSConchConfig_H__


//包含头文件
//------------------------------------------------------------------------------
#include <string>
#include "../JSInterface/JSInterface.h"
#include "../../../Conch/source/conch/JCSystemConfig.h"

namespace laya
{
    class JSConchConfig: public JsObjBase, public JSObjNode
    {
    public:
	    static JsObjClassInfo JSCLSINFO;
	    void exportJS();

	    JSConchConfig();
	    ~JSConchConfig();
	    static JSConchConfig* getInstance();
	    bool getUrlIgnoreCase();
	    void setUrlIgnoreCase(bool b);
	    int getNetworkType();

	    void setDownloadTryNum(int,int);
	    void setDownloadPathReplace(const char* p_pstrPath, const char* p_pszReplace);
	    void setDownloadTail(int type, const char* p_strTail);
	    //下载的时候修改文件的扩展名，以防止中间被修改。
	    //扩展名没有.
	    void setDownloadReplaceExt(const char* p_pszOrigin, const char* p_pszNew );
	    //是否转换文本文件的回车换行
	    void setDownloadIgnoreCRLR(bool b);
	    void resetDownloadIgnoreCRLR();

	    //设置不需要严格校验的文件。对于这些文件，如果校验失败了，则直接算作成功，并且保存下来
	    //扩展名有.
	    void addChkIgnoreChksumExt( const char* p_pszExt );
	    void clearChkIgnoreChksumExt();

	    //凡是扩展名为p_pszExt的文件,用key来进行异或操作，操作长度为p_nLen。如果p_nKey=0则去掉
	    //扩展名有.
	    void setDownloadUnmask(const char* p_pszExt, unsigned int p_nKey, int p_nLen );
	    void resetDownloadUnmask();

	    //如果传送过程中没有反应了，就在超时后重试，如果为0则不使用这一规则
	    void setDownloadNoResponseTimeout(int p_nDuration);
	    void resetDownloadNoResponseTimeout();

        /** @brief 设置外部版本控制字符串，例如 index.htm?ver=10 则 p_pszVersion就是 ver
         *  @param p_pszVersion NULL或者 "" 表示不进行外部版本管理
         */
        void setDownloadVersionString(const char* p_pszVersion);

		void setMouseFrame(double thredholdms);
        void setSlowFrame(bool p_bIsSlow);
        void setLimitFPS(int nFpsNum);

        void setScreenScale(float fScaleW, float fScaleH, float ftx, float fty);
        float getScreenScaleW();
        float getScreenScaleH();

        void setCurlProxy(const char* pProxy);
        void setWebsocketProxy(const char* pProxy);

        bool getUseAndroidSystemFont();
        void setUseAndroidSystemFont(bool b);

        void setJSDebugMode(int nMode);
        int getJSDebugMode();

        void setJSDebugPort(int nPort);
        int getJSDebugPort();

    public:
	    const char* getLocalStoragePath();
	    float getTotalMem();
	    int getUsedMem();
	    int getAvalidMem();
	    float getScreenInch();
	    void setTouchMoveRange( float p_fMM );
	    void setScreenOrientation( int p_nOrientation );
	    bool getIsPlug();
        bool getLocalable();
        void setLocalable(bool isLocalPackage);
        /** 
         *  @brief  如果需要自己管理文件更新的話，就通過url后加版本號的方法，這裡可以設置版本號字符串
         *          只有當searchPart只有版本號字符串的時候，才會緩存到同一個文件。這樣是為了避免一些錯誤
         *          例如獲取頭像使用同一個請求地址，但是不同的searchPart
         *          例如，設置的字符串為 v
         *          則 http://host/get?v=1, http://host/get?v=2 佔用同一個緩存文件
         *          http://host/get?v=1&bb=1 就不會佔用相同的緩存文件，
         *          http://host/get?id=1
         *  @param[in] p_strVersion
         */
        //void setUrlVersionStr(const char* p_strVersion);
        //const char* getUrlVersionStr();
        void setDownloadConnTimeout(int tm);
        void setDownloadOptTimeout(int tm);
        int getGLCaps();
    public:

	    const char* getOS();
	    const char* getBrowserInfo();
	    const char* getGuid();
	    //得到一个版本描述字符串
	    const char* getRuntimeVersion();
	    const char* getJsonparamExt();
	    const char* getDeviceInfo();
        const char* getAppVersion();
        const char* getAppLocalVersion();

        void setTouchMode(bool bMode);
        bool getTouchMode();
        void setDebugLevel(int nLevel);
        //设置图片释放的间隔时间
        void setImageReleaseSpaceTime(int nSpaceTime);
    
        //打开内存检测
        void enableMemorySurvey(bool bEnable);

        //打开内部的js和gl条状图
        void showInternalPerfBar(int b, float scale);

        //如果是android的话，使用choreographer
        void useChoreographer(int b);

        /*
            设置主表面的分辨率。
            android可以随时设置。
        */
        void setResolution(int w, int h);
        void testSleep(int tm);

        void setImageMisoperationWarningTime( int nSpaceTime );

        float getCurrentDeviceSystemVersion();

        void printAllMemorySurvey(char* sPath);

        void enableEncodeURI(bool b);

        void setSoundGarbageCollectionTime(int nTime);
    
        int getThreadMode();

    public:
	    static JSConchConfig*	ms_pInstance;
	    std::string				m_sGUID;
	    std::string				m_sDeviceModel;
	    std::string				m_sDeviceInfo;
        std::string             m_sAppVersion;
        std::string             m_sAppLocalVersion;
        //std::string             m_sVersionStr;
        float                   m_fScreenScaleW = 1.0f;
        float                   m_fScreenScaleH = 1.0f;
        float                   m_fScreenTx = 0.0f;
        float                   m_fScreenTy = 0.0f;
    };
}
//------------------------------------------------------------------------------


#endif //__JSConchConfig_H__

//-----------------------------END FILE--------------------------------