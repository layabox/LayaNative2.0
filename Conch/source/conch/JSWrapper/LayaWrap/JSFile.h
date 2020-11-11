/**
@file			JSFile.h
@brief			
@author			James
@version		1.0
@date			2017_11_28
*/

#ifndef __JSFile_H__
#define __JSFile_H__

#include "../JSInterface/JSInterface.h"
#include <time.h>
#include "JsBlob.h"
#include <util/JCMemorySurvey.h>
#include <fileSystem/JCFileSystem.h>
#ifdef WIN32
    #define strcasecmp _stricmp
    #define strncasecmp _strnicmp
#endif

namespace laya 
{
    class JsFile : public JsBlob
    {
    public:
        JsFile();

        JsFile(const char *p_pszName);

        JsFile(const char *p_pszName, const char *p_pszType);

        ~JsFile();

        void UpdateTime(time_t p_tm = 0);

        JsValue GetlastModifiedDate();

        const char *GetName();

        void SetName(const char *p_pszName);

        bool getEnableCache()
        {
            return m_bEnableCache;
        }

        void setEnableCache(bool b)
        {
            m_bEnableCache = b;
        }

        static void RegisterToJS();

    public:

        static JsObjClassInfo JSCLSINFO;
        friend class JsFileReader;
        enum
        {
            __IsLocal = 0,
            __IsRemote = 1,
        };

    protected:

        long long                   lastModifiedDate;
        /*
        * 如果是本地路径，就取去掉file://和?后的内容。
        * 如果是远程的，就是完整的原始路径。因为要做cache的key，所以连？都要
        */
        std::string                 m_FullName;
        int                         m_iPos;
        char*                       m_pszName;
        bool                        m_bEnableCache;

    };
}
//------------------------------------------------------------------------------


#endif //__JSFile_H__

//-----------------------------END FILE--------------------------------