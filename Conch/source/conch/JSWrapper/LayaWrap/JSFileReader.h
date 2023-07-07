/**
@file			JSFileReader.h
@brief			
@author			James
@version		1.0
@date			2017_11_28
*/

#ifndef __JSFileReader_H__
#define __JSFileReader_H__


//------------------------------------------------------------------------------


#include "../JSInterface/JSInterface.h"
#include "JSFile.h"
#include <util/JCMemorySurvey.h>

namespace laya 
{
    #define __Js_FileReader_Property_Func(pfn,n)  \
            JsValue Get_##pfn() \
            {return (pfn.getJsObj());} \
            void Set_##pfn( JSValueAsParam p_pfn)  \
            {   \
                pfn.set(n,this,p_pfn);    \
            }

    class JsFileReader :public JsObjBase, public JSObjNode
    {
    public:

        JsFileReader();

        ~JsFileReader();

        //以二进制格式读取文件内容
        void readAsArrayBuffer(JSValueAsParam p_pFile);

        //以文本(及字符串)格式读取文件内容，并且可以强制选择文件编码
        void readAsText(JSValueAsParam p_pFile);

        // 以DataURL格式读取文件内容，主要为了直接嵌入网页
        void readAsDataURL(JSValueAsParam p_pFile);

        void __LoadLocalFile(JsFile *p_pFile);

        void __LoadRemoteFile(JsFile *p_pFile);

        void onDownloadEnd(JsFile *p_pFile, void* p_pRes, std::weak_ptr<int> callbackref);

        void onDownloadErr(void* p_pRes, int p_nErrCode, std::weak_ptr<int> callbackref);

        void OnFinished(bool p_bSuccess, const char *p_pszError = 0);

        JsValue GetResult();

        static void RegisterToJS();

        //如果需要下载的话，设置超时参数。非标准
        void setConnTimeout(int tm);
        void setOptTimeout(int tm);

        char* getSVIP();
    public:

        void SetContenttype(int p_iType)
        {
            switch (p_iType)
            {
            case content_type_string:
            case content_type_buffer:
                m_iContentType = p_iType;
                break;

            default:
                break;
            }
        }
        unsigned int GetReadyState()
        {
            return readyState;
        }
        const char *GetError()
        {
            return m_pszError;
        }
        void abort()
        {
            JSP_THROW("abort not impl");
        }
        void OnFinishedSafe(bool p_bSuccess, const char *p_pszError, std::weak_ptr<int> callbackref)
        {
            if (!callbackref.lock())
                return;
            OnFinished(p_bSuccess, p_pszError);
        }
        void OnStart()
        {
            readyState = LOADING;
            onloadstart.Call();
        }
        void OnProgress(size_t p_iSaved, size_t p_iTotal)
        {
        }
        bool GetSync()
        {
            return m_bSync;
        }
        void setIgnoreError(bool b)
        {
            m_bIgnoreError = b;
        }
        void SetSync(bool p_bSync)
        {
            m_bSync = p_bSync;
        }
        void setResponseType(int p_nResponseType)
        {
            m_nResponseType = p_nResponseType;
        }
        int getResponseType()
        {
            return m_nResponseType;
        }

        int getErrorCode() { return m_nErrorCode; }
        int getHttpResponseCode() { return m_nHttpResponse; }
    public:

        static JsObjClassInfo JSCLSINFO;
        enum
        {
            EMPTY = 0,
            LOADING = 1,
            DONE = 2,

            NotFoundError = 0,
            SecurityError = 1,
            NotReadableError = 2,

            content_type_string = 0,
            content_type_buffer = 1,
        };
    protected:
        #define JsFileReaderErr_NotFoundError "NotFoundError"
        #define JsFileReaderErr_SecurityError "SecurityError"
        #define JsFileReaderErr_NotReadableError "NotReadableError"
        JsFile*                 m_pFile;
        JsObjHandle             m_hFileObject;
        JsObjHandle             onloadstart;	// 在读取开始时触发
        JsObjHandle             onprogress;	// 在读取进行中定时触发
        JsObjHandle             onload;		// 在读取成功结束后触发
        JsObjHandle             onabort;		// 在读取中断时触发
        JsObjHandle             onerror;		// 在读取错误时触发
        JsObjHandle             onloadend;		// 在读取结束后，无论成功或者失败都会触发
        const char*             m_pszError;
        unsigned int            readyState;
        static const char*      s_ErrorStr[];
        int                     m_iContentType;
        std::shared_ptr<int>	m_CallbackRef;
        bool                    m_bSync;				//是否为同步加载
        int                     m_nBufferSize;			//buffersize
        int                     m_nResponseType;
        bool	                m_bIgnoreError;
        int                     m_nErrorCode = 0;
        int                     m_nHttpResponse = 0;
        std::string             m_strSvIP;              //调试用。
        int                     m_nConnTimeout = 0;
        int                     m_nOptTimeout = 0;
    public:

        __Js_FileReader_Property_Func(onloadstart, 0);
        __Js_FileReader_Property_Func(onprogress, 1);
        __Js_FileReader_Property_Func(onabort, 2);
        __Js_FileReader_Property_Func(onloadend, 3);
        __Js_FileReader_Property_Func(onload, 4);
        __Js_FileReader_Property_Func(onerror, 5);

    };
}
//------------------------------------------------------------------------------


#endif //__JSFileReader_H__

//-----------------------------END FILE--------------------------------
