/**
@file			JCHttpHeader.h
@brief			
@author			guo
@version		1.0
@date			2016_09_03
*/

#ifndef __JCHttpHeader_H__
#define __JCHttpHeader_H__
#include <stdint.h>
namespace laya {
    /*
    Host: github.com
    ^     ^
    |     |
    |     Value (ValueLen = 10)
    |
    | FieldName, FieldNameLen = 4
    */
    typedef struct _HeaderField {
        const char *FieldName = nullptr;
        int         FieldNameLen = 0;;

        const char *Value = nullptr;
        int  ValueLen = 0;
    }HeaderField;

    typedef struct {
        /**
        * Pointer to start of the request line.
        */
        const char * RequestLineStart = nullptr;

        /**
        * Pointer to the end of the request line
        */
        const char * RequestLineEnd = nullptr;

        /**
        * Pointer to the start of the request method string
        */
        const char * RequestMethod = nullptr;

        int    RequestMethodLen = 0;

        const char * RequestURI = nullptr;
        int RequestURILen = 0;

        const char * HTTPVersion = nullptr;
        int HTTPVersionLen = 0;

        unsigned int HeaderSize = 0;
        enum { MAX_FILED_NUM = 26 };
        HeaderField Fields[MAX_FILED_NUM];

    } RequestHeader;

    class JCHttpHeader :public RequestHeader {
    public:
        enum _ERROR {
            ERR_REQUEST_LINE_PARSE_FAIL = -1,
            ERR_INCOMPLETE_HEADER,
            ERR_UNEXPECTED_CHAR,
        };

        static const char * request_line_parse(RequestHeader *header, const char *body, int bodyLength);
        static int request_header_parse(RequestHeader *header, const char *body, int bodyLength);
        struct CacheContrl {
            bool nocache = false;
            bool nostore = false;
            bool mustrevalidate = false;
            int maxage = 0;
        };
    public:
        JCHttpHeader(const char* pHeader);
        const char* getLastModifyed();//"Last-Modified"
        CacheContrl* getCacheContrl() { return &m_CacheControl; };//"Cache-Control"
        const char* getPragma();//"Pragma"
        const char* getContentType();//"Content-Type"
        const char* getETag();//"ETag"
        int64_t getExpires();//"Expires"      这个不用了

        bool parseCacheControl(char* p_pStr, int p_nlen);
    protected:
        int64_t     m_tmExpires = 0;
        CacheContrl m_CacheControl;
        bool        m_bCacheContrl = false;
    };
}

#endif 
