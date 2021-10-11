/**
@file			JSFileReader.cpp
@brief			
@author			James
@version		1.0
@date			2017_11_28
*/

#include "JSFileReader.h"
#include <resource/JCFileResManager.h>
#include <thread>
#include <util/Log.h>
#include <util/JCZlib.h>
#include <util/JCMemorySurvey.h>
#include "../../JCScriptRuntime.h"
#include "JSGlobalExportCFun.h"
#include <fstream>
#include <ctime>
#include "../../JCSystemConfig.h"
namespace laya 
{
    bool IsTextUTF8(char* str, unsigned long length)
    {
        unsigned long nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
        unsigned char chr;
        bool bAllAscii = true; //如果全部都是ASCII, 说明不是UTF-8
        for (size_t i = 0; i < length; i++)
        {
            chr = *(str + i);
            if ((chr & 0x80) != 0) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
                bAllAscii = false;
            if (nBytes == 0) //如果不是ASCII码,应该是多字节符,计算字节数
            {
                if (chr >= 0x80)
                {
                    if (chr >= 0xFC && chr <= 0xFD)
                        nBytes = 6;
                    else if (chr >= 0xF8)
                        nBytes = 5;
                    else if (chr >= 0xF0)
                        nBytes = 4;
                    else if (chr >= 0xE0)
                        nBytes = 3;
                    else if (chr >= 0xC0)
                        nBytes = 2;
                    else
                    {
                        return false;
                    }
                    nBytes--;
                }
            }
            else //多字节符的非首字节,应为 10xxxxxx
            {
                if ((chr & 0xC0) != 0x80)
                {
                    return false;
                }
                nBytes--;
            }
        }
        if (nBytes > 0) //违返规则
        {
            return false;
        }
        if (bAllAscii) //如果全部都是ASCII, 说明不是UTF-8
        {
            return true;
        }
        return true;
    }
    ADDJSCLSINFO(JsFileReader, JSObjNode);
    JsFileReader::JsFileReader()
    {
        m_nResponseType = 0;
        m_bSync = false;
        m_nBufferSize = 0;
        readyState = EMPTY;
        m_pszError = 0;
        m_iContentType = content_type_string;
        AdjustAmountOfExternalAllocatedMemory(86);
        JCMemorySurvey::GetInstance()->newClass("JsFileReader", 86, this);
        m_CallbackRef.reset(new int(1));
        m_bIgnoreError = false;
        m_pFile = NULL;
    }
    JsFileReader::~JsFileReader()
    {
        JCMemorySurvey::GetInstance()->releaseClass("JsFileReader", this);
    }
    void JsFileReader::readAsArrayBuffer(JSValueAsParam p_pFile)
    {
        m_iContentType = content_type_buffer;
        m_pFile = (JsFile*)__TransferToCpp<JsFile*>::ToCpp(p_pFile);
        m_hFileObject.set(6, this, p_pFile);
        __LoadRemoteFile(m_pFile);
    }
    void JsFileReader::readAsText(JSValueAsParam p_pFile)
    {
        m_iContentType = content_type_string;
        m_pFile = (JsFile*)__TransferToCpp<JsFile*>::ToCpp(p_pFile);
        m_hFileObject.set(6, this, p_pFile);
        __LoadRemoteFile(m_pFile);
    }
    void JsFileReader::readAsDataURL(JSValueAsParam p_pFile)
    {
        m_hFileObject.set(6, this, p_pFile);
        return;
    }
    void JsFileReader::__LoadLocalFile(JsFile *p_pFile)
    {
        OnStart();
        p_pFile->close();
        size_t iFileSize;
        std::time_t tmLastWrite;
        try
        {
            iFileSize = (size_t)fs::file_size(p_pFile->m_FullName);
#ifdef WIN32
            tmLastWrite = std::chrono::system_clock::to_time_t(fs::last_write_time(p_pFile->m_FullName));
#else
            tmLastWrite = fs::last_write_time(p_pFile->m_FullName);
#endif
        }
        catch (fs::filesystem_error &ec)
        {
            printf("read file error :%s\n", ec.what());
            OnFinished(false, JsFileReaderErr_NotFoundError);
            return;
        }
        if (0 == iFileSize)
        {
            OnFinished(false, JsFileReaderErr_NotFoundError);
            return;
        }
        std::ifstream ifile;
        ifile.open(p_pFile->m_FullName.c_str(), std::ios::in | std::ios::binary);
        if (!ifile.is_open())
        {
            OnFinished(false, JsFileReaderErr_SecurityError);
            return;
        }
        if (content_type_string == m_iContentType)
        {
            //判断是否有BOM,如果有，就去掉
            if (iFileSize >= 3) {
                int bom = 0;
                ifile.read((char*)&bom, 3);
                if (ifile.gcount() == 3) {
                    if (bom == 0xbfbbef) {
                        iFileSize -= 3;
                    }
                    else {
                        ifile.seekg(0);
                    }
                }
                else {
                    OnFinished(false, JsFileReaderErr_NotReadableError);
                    ifile.close();
                    return;
                }
            }

            p_pFile->Allocate(iFileSize + 1);
            ifile.read(p_pFile->m_pBuffer, iFileSize);
            if (ifile.gcount() != iFileSize) {
                OnFinished(false, JsFileReaderErr_NotReadableError);
                ifile.close();
                return;
            }
            p_pFile->m_pBuffer[iFileSize] = 0;
            p_pFile->m_i64Size--;
        }
        else
        {
            LOGE("JsFileReader::__LoadLocalFile not implement yet!");
            throw - 1;
            p_pFile->Allocate(iFileSize);
            ifile.read(p_pFile->m_pBuffer, iFileSize);
            if (ifile.gcount() != iFileSize) {
                OnFinished(false, JsFileReaderErr_NotReadableError);
                ifile.close();
                return;
            }
        }
        ifile.close();
        p_pFile->UpdateTime(tmLastWrite);
        OnFinished(true);
    }
    /*
    std::mutex g_kMutex;
    */
    /*
        现在这个函数其实能同时读取本地和http文件。
    */
    void JsFileReader::__LoadRemoteFile(JsFile *p_pFile)
    {
		makeStrong();	//防止被釋放
        OnStart();
        if (m_bSync)
        {
        }
        else
        {
            std::string file = p_pFile->m_FullName;
            if (p_pFile->m_iPos == JsFile::__IsLocal)
            {
                file = std::string("file:///") + file;
            }
            JCFileResManager* pfsMgr = JCScriptRuntime::s_JSRT->m_pFileResMgr;
            JCFileRes* res = pfsMgr->getRes(file,m_nConnTimeout, m_nOptTimeout);
            res->setIgnoreError(m_bIgnoreError);
            std::weak_ptr<int> cbref(m_CallbackRef);
            res->setOnReadyCB(std::bind(&JsFileReader::onDownloadEnd, this, p_pFile, std::placeholders::_1, cbref));
            res->setOnErrorCB(std::bind(&JsFileReader::onDownloadErr, this, std::placeholders::_1, std::placeholders::_2, cbref));
        }
    }
    bool JsFileReader::onDownloadEnd(JsFile *p_pFile, void* p_pRes, std::weak_ptr<int> callbackref)
    {
        if (!callbackref.lock())
            return false;
        JCResStateDispatcher* pRes = (JCResStateDispatcher*)p_pRes;
        JCFileRes* pFileRes = (JCFileRes*)pRes;
        //if (pFileRes->m_pBuffer.get() == NULL || pFileRes->m_nLength == 0)
        //{
        //    OnFinished(false, JsFileReaderErr_NotFoundError);
        //    return false;
       // }
        //else
        {
            unsigned char* pBuff = (unsigned char*)(pFileRes->m_pBuffer.get());
            int nLen = pFileRes->m_nLength;
            //-------------------------------------------------
            //解压
            //-------------------------------------------------
            int nOffset = 0;
            uLong nUncompressSize = getLayaBoxUncompressSize(pBuff, nLen, nOffset);
            if (nUncompressSize > 0)
            {
                double nTime = tmGetCurms();
                unsigned char* sUCBuffer = new unsigned char[nUncompressSize];
                memset(sUCBuffer, 0, nUncompressSize);
                if (uncompress(sUCBuffer, &nUncompressSize, pBuff + nOffset, nLen) != Z_OK)
                {
                    delete[] sUCBuffer;
                    sUCBuffer = NULL;
                    //解压失败
                    OnFinished(false);
                    return false;
                }
                nLen = nUncompressSize;
                pBuff = sUCBuffer;
                int nSpace = (int)(tmGetCurms() - nTime);
                LOGI("LayaUncompress time=%d fileName=%s", nSpace, p_pFile->GetName());
                //-------------------------------------------------
                //-------------------------------------------------
            }
            //临时存储一下为了删除，因为下去去掉bom，指针会被改变
            unsigned char* pTemp = pBuff;
            if (content_type_string == m_iContentType)
            {
#ifdef __APPLE__
                if (IsTextUTF8((char*)pBuff, nLen) == false)
                {
                    char sBuffer[1024] = { 0 };
                    sprintf(sBuffer, "严重错误 iOS does not support non utf8 format files url=%s", p_pFile->GetName());
                    LOGE( sBuffer );
                }
#endif
                //循环去掉BOM
                while (true)
                {
                    if (nLen >= 3 && ((*(int*)pBuff) & 0x00ffffff) == 0xbfbbef)
                    {
                        nLen -= 3;
                        pBuff += 3;
                    }
                    else
                    {
                        break;
                    }
                }
                p_pFile->Allocate(nLen + 1);
                memcpy(p_pFile->m_pBuffer, pBuff, nLen);
                p_pFile->m_pBuffer[nLen] = 0;
                p_pFile->m_i64Size--;
            }
            else
            {
                p_pFile->Allocate(nLen);
                memcpy(p_pFile->m_pBuffer, pBuff, nLen);
            }
            //析构
            if (nUncompressSize > 0)
            {
                delete[] pTemp;
                pTemp = 0;
            }
            OnFinished(true);
        }
        return true;
    }
    void JsFileReader::onDownloadErr(void* p_pRes, int p_nErrCode, std::weak_ptr<int> callbackref)
    {
        if (!callbackref.lock())
            return;
        JCFileRes* pFRes = (JCFileRes*)p_pRes;
        m_strSvIP = pFRes->m_strSvIP;
        m_nErrorCode = pFRes->m_nErrNo;
        m_nHttpResponse = pFRes->m_nLastHttpResponse;
        if (p_nErrCode == 1)
            OnFinished(false, JsFileReaderErr_NotFoundError);
        else
            OnFinished(false, "UnknownError");
    }
    void JsFileReader::OnFinished(bool p_bSuccess, const char *p_pszError) 
    {
        if (!IsMyJsEnv())return;
        if (p_bSuccess)
        {
            m_pszError = 0;
            readyState = DONE;
            if (m_pFile)
                m_pFile->UpdateTime();
            onload.Call();
        }
        else
        {
            if (m_pFile)
                m_pFile->close();
            m_pszError = JsFileReaderErr_NotReadableError;
            readyState = DONE;
            onerror.Call(p_pszError);
        }
        onloadend.Call();
        m_pszError = 0;
        readyState = EMPTY;
        m_hFileObject.Reset();	//完成后，要把对File的引用去掉
        m_pFile = 0;
		makeWeak();
    }
    JsValue JsFileReader::GetResult()
    {
        if (DONE != readyState) {
            return JSP_TO_JS_UNDEFINE;
        }
        //else if (0 == m_pFile || 0 == m_pFile->m_i64Size) {
        //    return JSP_TO_JS_NULL;
        //}
        else if (content_type_buffer == m_iContentType)
        {
            //if (m_pFile->m_i64Size <= 0 || m_pFile->m_pBuffer == NULL) {
            //    return JSP_TO_JS_NULL;
            //}

            if (m_pFile->m_i64Size > 0x7fffffff) {
                LOGE("文件太大，无法返回！%s", (char*)m_pFile->m_FullName.c_str());
                throw - 1;
            }
            return createJSAB(m_pFile->m_pBuffer, (int)m_pFile->m_i64Size);
            //JSArrayBuffer* pAB = JSArrayBuffer::create((int)m_pFile->m_i64Size);
            //memcpy( pAB->getPtr(),m_pFile->m_pBuffer,(int)m_pFile->m_i64Size);
            //return (pAB->toLocal());
        }
        else
        {
            if (m_nResponseType == 1)
            {
                if (m_pFile->m_i64Size > 0x7fffffff) {
                    LOGE("文件太大，无法返回！%s", (char*)m_pFile->m_FullName.c_str());
                    throw - 1;
                }
                return createJSAB(m_pFile->m_pBuffer, (int)m_pFile->m_i64Size);
                //JSArrayBuffer* pAB = JSArrayBuffer::create((int)m_pFile->m_i64Size);
                //memcpy( pAB->getPtr(),m_pFile->m_pBuffer,(int)m_pFile->m_i64Size);
                //return (pAB->toLocal());
                //return JSP_TO_JS_BYTE_ARRAY( (unsigned char *)(m_hFile.m_pFile->m_pBuffer),m_hFile.m_pFile->m_i64Size );
            }
            else
            {
                if (m_pFile->m_pBuffer == NULL)
                {
                    return (JSP_TO_JS_STR(""));
                }
                return (JSP_TO_JS_STR(m_pFile->m_pBuffer));
            }
        }
    }

    void JsFileReader::setConnTimeout(int tm) {
        m_nConnTimeout = tm;
    }

    void JsFileReader::setOptTimeout(int tm) {
        m_nOptTimeout = tm;
    }

    char* JsFileReader::getSVIP() {
        return (char*)m_strSvIP.c_str();
    }

    void JsFileReader::RegisterToJS()
    {
        JSP_CLASS("FileReader", JsFileReader);
        JSP_ADD_FIXED_PROPERTY(EMPTY, JsFileReader, (int)JsFileReader::EMPTY);
        JSP_ADD_FIXED_PROPERTY(LOADING, JsFileReader, (int)JsFileReader::LOADING);
        JSP_ADD_FIXED_PROPERTY(DONE, JsFileReader, (int)JsFileReader::DONE);
        JSP_ADD_PROPERTY_RO(readyState, JsFileReader, GetReadyState);
        JSP_ADD_PROPERTY_RO(error, JsFileReader, GetError);
        JSP_ADD_PROPERTY_RO(result, JsFileReader, GetResult);
        JSP_ADD_PROPERTY(onloadstart, JsFileReader, Get_onloadstart, Set_onloadstart);
        JSP_ADD_PROPERTY(onprogress, JsFileReader, Get_onprogress, Set_onprogress);
        JSP_ADD_PROPERTY(onload, JsFileReader, Get_onload, Set_onload);
        JSP_ADD_PROPERTY(onabort, JsFileReader, Get_onabort, Set_onabort);
        JSP_ADD_PROPERTY(onerror, JsFileReader, Get_onerror, Set_onerror);
        JSP_ADD_PROPERTY(onloadend, JsFileReader, Get_onloadend, Set_onloadend);
        JSP_ADD_PROPERTY(sync, JsFileReader, GetSync, SetSync);
        JSP_ADD_PROPERTY(responseType, JsFileReader, getResponseType, setResponseType);
        JSP_ADD_METHOD("abort", JsFileReader::abort);
        JSP_ADD_METHOD("readAsArrayBuffer", JsFileReader::readAsArrayBuffer);
        JSP_ADD_METHOD("readAsText", JsFileReader::readAsText);
        JSP_ADD_METHOD("readAsDataURL", JsFileReader::readAsDataURL);
        JSP_ADD_METHOD("setIgnoreError", JsFileReader::setIgnoreError);
        JSP_ADD_METHOD("getErrorCode", JsFileReader::getErrorCode);
        JSP_ADD_METHOD("getHttpCode", JsFileReader::getHttpResponseCode);
        JSP_ADD_METHOD("getSVIP", JsFileReader::getSVIP);
        JSP_ADD_METHOD("setConnTimeout", JsFileReader::setConnTimeout);
        JSP_ADD_METHOD("setOptTimeout", JsFileReader::setOptTimeout);
        JSP_INSTALL_CLASS("FileReader", JsFileReader);
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
