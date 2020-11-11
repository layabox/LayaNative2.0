/**
@file			JCHttpHeader.cpp
@brief			
@author			guo
@version		1.0
@date			2016_09_03
*/

#include "JCHttpHeader.h"
#include "../util/Log.h"
#include "../util/JCCommonMethod.h"
#include "../util/JCCrypto.h"
#include <stdlib.h>

#define peek(buf, c1) *(buf+1) != '\0' && *(buf+1) == c1
#define peek2(buf, c1, c2) \
    *(buf+1) != '\0' && *(buf+1) == c1 \
    *(buf+2) != '\0' && *(buf+2) == c2


#define iscrlf(p) (*p == '\r' && *(p + 1) == '\n')
#define notcrlf(p) (*p != '\r' && *(p + 1) != '\n')

#define notend(p) (*p != '\0')
#define end(p) (*p == '\0')
#define H3_DEFAULT_HTTP_VERSION "HTTP/1.0"

namespace laya{
    /**
    * This function returns a char pointer, which is the end of the request line.
    *
    * Return NULL if parse failed.
    */
    const char * JCHttpHeader::request_line_parse(RequestHeader *header, const char *body, int bodyLength) {
        // Parse the request-line
        // http://tools.ietf.org/html/rfc2616#section-5.1
        // Request-Line   = Method SP Request-URI SP HTTP-Version CRLF
        const char * p = body;
        header->RequestLineStart = body;

        while (notend(p) && !isspace(*p)) p++;

        if (end(p) || iscrlf(p)) {
            // set error
            return NULL;
        }

        header->RequestMethod = body;
        header->RequestMethodLen = p - body;

        // Skip space
        // parse RequestURI
        while (isspace(*p) && notcrlf(p) && notend(p)) p++;

        header->RequestURI = p;
        while (!isspace(*p) && notcrlf(p) && notend(p)) p++;
        header->RequestURILen = p - header->RequestURI;

        // Skip space and parse HTTP-Version
        if (iscrlf(p) || end(p)) {
            header->HTTPVersion = H3_DEFAULT_HTTP_VERSION;
        }
        else {
            while (isspace(*p) && notcrlf(p)) p++;

            header->HTTPVersion = p;
            while (!isspace(*p) && notcrlf(p)) p++;
            header->HTTPVersionLen = p - header->HTTPVersion;
        }
        return p;
    }



    /**
    * Parse header body
    */
    int JCHttpHeader::request_header_parse(RequestHeader *header, const char *body, int bodyLength) {
        const char *p = request_line_parse(header, body, bodyLength);

        if (p == NULL) {
            return ERR_REQUEST_LINE_PARSE_FAIL;
        }

        // should be ended with CR-LF
        if (end(p)) return -1;

        // skip CR-LF
        iscrlf(p); p += 2;
        if (end(p)) return 0;

        header->HeaderSize = 0;

        // Parse Header Fields Here
        do {
            HeaderField *field = &header->Fields[header->HeaderSize++];
            // HeaderField *field = h3_header_field_new();
            field->FieldName = p; // start of a header field name

            while (notend(p) && *p != ':') p++;
            field->FieldNameLen = p - field->FieldName;
            p++; // skip ':'

            // CRLF is not allowed here
            if (end(p) || iscrlf(p)) return -1;

            while (notend(p) && isspace(*p)) p++; // skip space
                                                  // CRLF is not allowed here
            if (end(p) || iscrlf(p)) return -1;

            field->Value = p;
            while (notend(p) && notcrlf(p)) p++;
            field->ValueLen = p - field->Value + 1;

            iscrlf(p); p += 2;
#ifdef _DEBUG
            printf("==> %.*s ==> %.*s\n", field->FieldNameLen, field->FieldName, field->ValueLen, field->Value);
#endif
            // end of header
            if (iscrlf(p)) return 0;
        } while (notend(p) && header->HeaderSize < MAX_FILED_NUM);

        return 0;
    }

    constexpr int _First_Len(char v1, int len) {
        return (((int)v1) << 8) | len;
    }
    JCHttpHeader::JCHttpHeader(const char* pHeader) {
        m_bCacheContrl = false;
        request_header_parse(this, pHeader, strlen(pHeader));

        for (int i = 0; i < (int)HeaderSize; i++) {
            HeaderField& cfield = Fields[i];
            int id = (cfield.FieldName[0] << 8)|(cfield.FieldNameLen & 0xff);
            switch (id) {
            case _First_Len('C',12)://Content-Type
                if (memcmp(cfield.FieldName + 1, "ontent-Type",11) == 0) {
                    int a = 0;
                }
                break;
            case _First_Len('C',13)://Cache-Control
                if (memcmp(cfield.FieldName + 1, "ache-Control",12) == 0) {
                    parseCacheControl((char*)cfield.Value, cfield.ValueLen);
                }
                break;
            case _First_Len('C',14)://Content-Length
                if (memcmp(cfield.FieldName + 1, "ontent-Length", 13) == 0) {
                    int a = 0;
                }
                break;
            case _First_Len('E',4):
                break;
            case _First_Len('E',7):
                break;
            }
        }
   }

    int _getTokeHash(const char*& pDt) {
        const char* pst = pDt;
        while (*pDt != ',' && *pDt != ' '&& *pDt!='=' && *pDt != 0) {
            *pDt++;
        }
        return JCBKDRHash::hashMem((const unsigned char*)pst, pDt-pst);
    }
    void _getSplit(const char*& pDt) {
        while (*pDt == ',' || *pDt == ' ' ||*pDt=='=')
            pDt++;
    }
    int _getNumber(const char*& pDt) {
        const char* pSt = pDt;
        char number[16] = { 0 };
        while (*pDt >= '0' && *pDt <= '9') {
            pDt++;
        }
        if (pDt - pSt > sizeof(number)) {
            LOGE("parseCacheControl error");
            *(int*)0 = 1;//报错
        }
        memcpy(number, pSt, pDt - pSt);
        return atoi(number);
    }

    bool JCHttpHeader::parseCacheControl(char* p_pStr, int p_len) {
        char* cp = new char[p_len + 1];
        memcpy(cp, p_pStr, p_len);
        cp[p_len] = 0; //最后用0比较容易处理。
        const char* pDt = cp;
        while (*pDt != 0) {//
            int hash = _getTokeHash(pDt);
            switch (hash){
            case 0x7dbba7b2://max-age
                pDt++;
                m_CacheControl.maxage = _getNumber(pDt);
                break;
            case 0x4fbcb749://no-store
                m_CacheControl.nostore = true;
                _getSplit(pDt);
                break;
            case 0x3452662e://no-cache
                m_CacheControl.nocache = true;
                _getSplit(pDt);
                break;
            case 0x19fb0881://must-revalidate
                _getSplit(pDt);
                break;
            default:
                _getSplit(pDt);
                break;
            }
        }
        delete[] cp;
        return true;
    }

    const char* JCHttpHeader::getLastModifyed() {//"Last-Modified"
        return "";
    }

    const char* JCHttpHeader::getPragma() {//"Pragma"
        return "";
    }
    const char* JCHttpHeader::getContentType() {//"Content-Type"
        return "";
    }

    const char* JCHttpHeader::getETag() {//"ETag"
        return "";
    }

    int64_t JCHttpHeader::getExpires() {
        static const char* strExpires = "Expires";
        if (m_tmExpires != 0)
            return m_tmExpires;
        for (int i = 0; i < (int)HeaderSize; i++) {
            HeaderField& cfield = Fields[i];
            if (memcmp(cfield.FieldName, strExpires, strlen(strExpires)) == 0) {
                //return 
            }
        }
        return 0;
    }
}
