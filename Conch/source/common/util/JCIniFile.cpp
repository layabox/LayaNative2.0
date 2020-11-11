#include "JCIniFile.h"
#include <string.h>
#include <stdio.h>
#pragma warning(disable:4996)
#pragma warning(disable:4018)
#pragma warning(disable:4101)
namespace laya
{
    JCIniFile::__BuffCtrl::__BuffCtrl()
    {
        m_pBuff = m_pCur = 0;
        m_iBuffSize = m_iLeftSize = 0;
    }
    JCIniFile::__BuffCtrl::~__BuffCtrl()
    {
        if (0 != m_pBuff)
        {
            delete[] m_pBuff;
            m_pBuff = 0;
        }
    }
    void JCIniFile::__BuffCtrl::Reset()
    {
        m_pCur = m_pBuff;
        m_iLeftSize = m_iBuffSize;
    }
    void JCIniFile::__BuffCtrl::Bind(char *p_pBuff, int p_iBuffSize)
    {
        m_pBuff = p_pBuff;
        m_iBuffSize = p_iBuffSize;
        Reset();
    }
    bool JCIniFile::__BuffCtrl::GetLine(char *p_pBuff, int p_iBuffSize)
    {
        if (0 == m_iLeftSize || 0 == m_pCur)
        {
            return false;
        }
        char *pBegin = m_pCur;
        char *pCur = pBegin;
        int iLeft = m_iLeftSize;
        for (; iLeft > 0;)
        {
            if ('\r' == *pCur || '\n' == *pCur)
            {
                if (p_iBuffSize < pCur - pBegin)
                    return false;
                memcpy(p_pBuff, pBegin, pCur - pBegin);
                p_pBuff[pCur - pBegin] = 0;
                for (; iLeft>0;)
                {
                    if ('\r' != *pCur && '\n' != *pCur)
                        break;
                    else
                    {
                        pCur++;
                        iLeft--;
                    }
                }
                if (0 == iLeft)
                {
                    m_iLeftSize = 0;
                    m_pCur = 0;
                }
                else
                {
                    m_pCur = pCur;
                    m_iLeftSize = iLeft;
                }
                return true;
            }
            else
            {
                pCur++;
                iLeft--;
            }
        }

        if (pCur - pBegin > 0)
        {
            if (p_iBuffSize < pCur - pBegin)
                return false;
            memcpy(p_pBuff, pBegin, pCur - pBegin);
            p_pBuff[pCur - pBegin] = 0;
            m_iLeftSize = 0;
            m_pCur = 0;
            return true;
        }
        else
        {
            return false;
        }
    }
    JCIniFile::JCIniFile(char *p_pFileContent, size_t p_iBuffSize)
    {
        m_BuffCtrl.Bind(p_pFileContent, p_iBuffSize);
        __Parse();
    }

    JCIniFile::~JCIniFile()
    {
        if (m_Values.size())
        {
            _PairQueueIter iter;
            for (iter = m_Values.begin(); iter != m_Values.end(); iter++)
            {
                delete *iter;
            }
            m_Values.clear();
        }
    }
    char *JCIniFile::_TrimLeft(char *p_pszStr)
    {
        char *p = p_pszStr;
        for (;;)
        {
            if (!*p)
                break;
            if (*p > 0x20)
                break;
            p++;
        }
        return p;
    }
    char *JCIniFile::_TrimRight(char *p_pszStr)
    {
        char *p = p_pszStr + strlen(p_pszStr);

        for (; p >= p_pszStr;)
        {
            if (*p <= 0x20)
                *p = 0;
            else
                break;
            p--;
        }
        return p_pszStr;
    }
    void JCIniFile::__Parse()
    {
        char szLine[1024];
        char *ln;
        int n;
        char *l, *r;
        _Pair_t *pPair;

        for (; m_BuffCtrl.GetLine(szLine, 1024);)
        {
            ln = _TrimLeft(szLine);
            if (!*ln)
                continue;
            n = strlen(ln);
            while (n && (ln[n - 1] == '\r' || ln[n - 1] == '\n'))
            {
                ln[n - 1] = '\0';
                n--;
            }
            if (!n)
                continue;

            l = strtok(ln, "=");
            r = strtok(0, "\0");

            l = _TrimRight(l);
            if (r)
            {
                r = _TrimRight(r);
                r = _TrimLeft(r);
            }

            pPair = new _Pair_t;

            pPair->m_pszKey = new char[strlen(l) + 1];
            strcpy(pPair->m_pszKey, l);
            if (r && strlen(r))
            {
                pPair->m_pszValue = new char[strlen(r) + 1];
                strcpy(pPair->m_pszValue, r);
            }
            m_Values.push_back(pPair);
        }
    }
    bool JCIniFile::IsReady()
    {
        return (0 != m_Values.size());
    }
    const char *JCIniFile::GetValue(const char *p_pszKey)
    {
        if (0 == p_pszKey)
            return 0;

        const char *pRet = 0;

        if (m_Values.size())
        {
            _PairQueueIter iter;
            for (iter = m_Values.begin(); iter != m_Values.end(); iter++)
            {
                if (strcmp(p_pszKey, (*iter)->m_pszKey) == 0)
                {
                    pRet = (*iter)->m_pszValue;
                    break;
                }
            }
        }
        return pRet;
    }

    JCIniFile* JCIniFile::loadFile(const char* p_sFileName)
    {
        if (p_sFileName == NULL)return NULL;
        FILE* fp = fopen(p_sFileName, "r");
        if (!fp) return NULL;
        fseek(fp, 0, SEEK_END);
        int nBuffSize = (int)ftell(fp);
        fseek(fp, 0, SEEK_SET);
        char *pBuff = new char[nBuffSize+1];
        int nReadSize = fread((char*)pBuff, 1, nBuffSize, fp);
        if (nReadSize != nBuffSize)
        {
            delete pBuff;
            return NULL;
        }
        fclose(fp);
        pBuff[nBuffSize] = 0;
        return new JCIniFile(pBuff, nBuffSize);
    }
}