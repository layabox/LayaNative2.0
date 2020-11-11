/**
@file			JCIniFile.h
@brief			
@author			James
@version		1.0
@date			2016_6_11
*/

#ifndef __JCIniFile_H__
#define __JCIniFile_H__

#include <stddef.h>
#include <deque>

namespace laya
{
    class JCIniFile
    {
        class __BuffCtrl
        {
        public:
            __BuffCtrl();
            ~__BuffCtrl();
            void Bind(char *p_pBuff, int p_iBuffSize);
            void Reset();
            bool GetLine(char *p_pBuff, int p_iBuffSize);
        public:
            char*       m_pBuff;
            int         m_iBuffSize;
            char*       m_pCur;
            int      m_iLeftSize;
        };
        struct _Pair_t
        {
            _Pair_t()
            {
                m_pszKey = m_pszValue = 0;
            }
            ~_Pair_t()
            {
                if (0 != m_pszKey)
                {
                    delete[] m_pszKey;
                }
                if (0 != m_pszValue)
                {
                    delete[] m_pszValue;
                }
            }
            char *m_pszKey;
            char *m_pszValue;
        };
    public:

        typedef std::deque<_Pair_t *>   _PairQueue;
        typedef _PairQueue::iterator    _PairQueueIter;
        char *_TrimLeft(char *p_pszStr);
        char *_TrimRight(char *p_pszStr);
        void __Parse();
    public:
        JCIniFile(char *p_pFileContent, size_t p_iBuffSize);
        ~JCIniFile();
        bool IsReady();
        const char *GetValue(const char *p_pszKey);
        static JCIniFile* loadFile(const char* p_sFileName);
    public:
        __BuffCtrl      m_BuffCtrl;
        _PairQueue      m_Values;
    };
}

//------------------------------------------------------------------------------


#endif //__JCIniFile_H__

//-----------------------------END FILE--------------------------------