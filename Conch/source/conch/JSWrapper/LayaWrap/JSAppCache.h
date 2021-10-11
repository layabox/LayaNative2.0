/**
@file			JSAppCache.h
@brief			
@author			James
@version		1.0
@date			2016_5_17
*/

#ifndef __JSAppCache_H__
#define __JSAppCache_H__


//包含头文件
//------------------------------------------------------------------------------
#include "../JSInterface/JSInterface.h"
#include <string>
namespace laya
{
	class JCServerFileCache;
	class JsAppCache :public JsObjBase, public JSObjNode
	{
	public:
        enum 
        { 
            transUrlFun 
        };

		static JsObjClassInfo JSCLSINFO;
		explicit JsAppCache();
		explicit JsAppCache(const char* p_pszURL);

		~JsAppCache();

		unsigned int getCacheSize();

		std::string getCachePath();

		bool getEnableCache();

		void setEnableCache(bool b );

		//这个已经不用了
		void update( const char* p_pszDccURL, unsigned int p_ulCheckSum, JSValueAsParam p_pFuncCheckOK, JSValueAsParam p_pFuncCheckError );

        /** @brief
        * 更新一个文件,
         * @param p_nFileID 是文件id,用id的方法可以给外部提供更大的灵活性。
        * @param p_ulChkSum 校验码,如果为零则自己计算
        * @param p_pBuffer是文件数据
        * @param p_bExtVersion true 则表示p_ulChkSum是版本号
        * @return
        *   返回为ture表示更新成功,否则表示与校验码不符。
        */
        bool updateFile(int p_nFileID, unsigned int p_ulChkSum, const char* p_pBuffer, int len, bool p_bExtVersion);

        /** @brief
         * 更新一个文件,
         * @param p_nFileID 是文件id,用id的方法可以给外部提供更大的灵活性。
         * @param p_ulChkSum 校验码,如果为零则自己计算
         * @param p_pBuffer是文件数据
         * @param p_bExtVersion true 则表示p_ulChkSum是版本号
         * @return
         *   返回为ture表示更新成功,否则表示与校验码不符。
        */
        bool updateFileForJs(int p_nFileID, unsigned int p_nCheckSum, JSValueAsParam p_pBuffer, bool p_bExtVersion);

		//把url转换成本地文件路径
		std::string url2Local(const char* p_pszURL );

		//得到某个资源的id
		//通常是对应sourceid目录下的一个文件的内容。可能是表示校验码，或者简单的版本号。
		std::string getResourceID(const char* p_pszResource );

		void setResourceID(const char* p_pszResource, const char* p_pszVal);

		void saveFileTable(const char* p_pszFileContent);

        void setFileTable(const char* p_pszFileContent);

        void setUrlTransTable(const char* p_pszFileContent, int split);

		bool isFileTableValid();

		bool isUrlNeedDownload(const char* p_pszFile);

		//获得所有缓存的app列表。返回的是一个js的数组
		JsValue getAppList();

		//得到某个app的缓存的大小
		int32_t getAppSize(const char* appurl);

		//删除指定app的缓存
		bool delAppCache(const char* appurl);

        void delCurAppCache();

		//删除所有的缓存
		void delAllCache();
		
        int hashString(const char* str);

		//临时
		std::string loadCachedURL( const char* p_pszUrl );

        void setTransUrlToCachedUrl(JSValueAsParam pObj);

        char* getTransedUrl(const char* pUrl);

        static void exportJS();

    public:
        JsObjHandle		    m_funcCheckOK;
        JsObjHandle		    m_funcCheckError;
    protected:
        
        bool			    m_bEnableCache;
        std::string		    m_strURL;
        JCServerFileCache*	m_pSvFileCache;
        JsObjHandle         m_funcTransUrl;//用来转换url的
	};
}
//------------------------------------------------------------------------------


#endif //__JSAppCache_H__

//-----------------------------END FILE--------------------------------