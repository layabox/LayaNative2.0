/**
@file			JSFile.cpp
@brief			
@author			James
@version		1.0
@date			2017_11_28
*/

#include "JSFile.h"

namespace laya 
{
    ADDJSCLSINFO(JsBlob, JSObjNode);
    ADDJSCLSINFO(JsFile, JsBlob);
    JsFile::JsFile()
    {
        m_pszName = 0;
        m_iPos = __IsLocal;
        m_bEnableCache = true;
        UpdateTime();
        AdjustAmountOfExternalAllocatedMemory(301);
        JCMemorySurvey::GetInstance()->newClass("JsFile", 301, this);
    }
    JsFile::JsFile(const char *p_pszName)
    {
        m_pszName = 0;
        m_iPos = __IsLocal;
        m_bEnableCache = true;
        UpdateTime();
        SetName(p_pszName);
        AdjustAmountOfExternalAllocatedMemory(301);
        JCMemorySurvey::GetInstance()->newClass("JsFile", 301, this);
    }
    JsFile::JsFile(const char *p_pszName, const char *p_pszType)
    {
        m_pszName = 0;
        m_iPos = __IsLocal;
        m_bEnableCache = true;
        UpdateTime();
        SetName(p_pszName);
        SetType(p_pszType);
        AdjustAmountOfExternalAllocatedMemory(301);
        JCMemorySurvey::GetInstance()->newClass("JsFile", 301, this);
    }
    JsFile::~JsFile()
    {
        if (0 != m_pszName)
        {
            delete[] m_pszName;
            m_pszName = 0;
        }
        JCMemorySurvey::GetInstance()->releaseClass("JsFile", this);
    }
    void JsFile::UpdateTime(time_t p_tm)	// unix timestamp
    {
        if (0 == p_tm)
        {
            p_tm = time(0);
        }
        lastModifiedDate = p_tm;
        lastModifiedDate *= 1000;
    }
    JsValue JsFile::GetlastModifiedDate()
    {
        return (__TransferToJs<int64_t>::ToJsDate(lastModifiedDate));
    }
    const char *JsFile::GetName()
    {
        if (0 == m_pszName)
            return "";
        else
            return m_pszName;
    }

    //给 m_FullName 和 m_pszName 赋值
    void JsFile::SetName(const char *p_pszName)
    {   
        if (0 != m_pszName)
        {
            delete[] m_pszName;
            m_pszName = 0;
        }
        size_t len;
        if (0 == p_pszName || 0 == (len = strlen(p_pszName)))
        {
            return;
        }
        if (len > 7 && (strncasecmp(p_pszName, "http://", 7) == 0 || strncasecmp(p_pszName, "https://", 8) == 0))
        {
            m_iPos = __IsRemote;
            //由于在复杂的url中有?:|等很多符号，容易导致非法。所以需要处理一下
            m_FullName = p_pszName;

            const char* pQpos = strchr(p_pszName, '?');
            int len = strlen(p_pszName);
            if (pQpos) len = pQpos - p_pszName;
            std::string strName = "";
            strName.append(p_pszName, len);
            std::string strname = fs::path(strName).filename().generic_string();
            len = strname.length();
            if (len > 0)
            {
                m_pszName = new char[len + 1];
                memcpy(m_pszName, strname.c_str(), len + 1);
            }
        }
        else
        {
            m_iPos = __IsLocal;
            if (len > 7 && strncasecmp(p_pszName, "file://", 7) == 0)
                p_pszName += 7;

            if (p_pszName[2] == ':')	//有盘符
                p_pszName++;
            const char* pQpos = strchr(p_pszName, '?');
            int len = strlen(p_pszName);
            if (pQpos) len = pQpos - p_pszName;
            std::string strName = "";
            strName.append(p_pszName, len);
            m_FullName = strName.c_str();
            std::string strname = fs::path(m_FullName).filename().generic_string();
            len = strname.length();
            if (len > 0)
            {
                m_pszName = new char[len + 1];
                memcpy(m_pszName, strname.c_str(), len + 1);
            }
        }
    }
    void JsFile::RegisterToJS()
    {
        JSP_CLASS("File", JsFile);
        JSP_ADD_PROPERTY_RO(lastModifiedDate, JsFile, GetlastModifiedDate);
        JSP_ADD_PROPERTY_RO(name, JsFile, GetName);
        JSP_ADD_PROPERTY_RO(size, JsFile, GetSize);
        JSP_ADD_PROPERTY_RO(type, JsFile, GetType);
        JSP_ADD_METHOD("close", JsFile::close);
        JSP_ADD_METHOD("slice", JsFile::slice);
        JSP_ADD_PROPERTY(enableCache, JsFile, getEnableCache, setEnableCache);
        JSP_REG_CONSTRUCTOR(JsFile, const char *);
        JSP_REG_CONSTRUCTOR(JsFile, const char *, const char *);
        JSP_INSTALL_CLASS("File", JsFile);
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------