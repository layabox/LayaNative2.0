/**
@file			JCCommonMethed.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "JCCommonMethod.h"
#ifndef WEBASM
    #include "../util/JCLayaUrl.h"
#endif
#include "Log.h"

#if __APPLE__
    #include <sys/time.h>
    #include <time.h>
    #include <mach/mach_time.h>
#elif ANDROID
    #include <time.h>
#elif WIN32
    #include <windows.h>
    #include <psapi.h>
    #include <iostream>
    #pragma comment(lib,"psapi.lib")
#endif

#ifdef WEBASM
    extern "C"
    {
        double DateNow();
    }
#endif

namespace laya
{
    //------------------------------------------------------------------------------
    int mathCeilPowerOfTwo(int x)
    {
        x--;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;
        x++;
        return x;
    }
    //------------------------------------------------------------------------------
	unsigned int hexStringToInt( const char* str )
    {
		int len = strlen(str);
		if(len>8)
			return 0;
		unsigned char* pCur = (unsigned char*)(str+len-1);
		unsigned char c = *pCur;
		int ret=0;
		int off=0;
		while((long)pCur>=(long)str){
			if(c!=' '){
				if(c>='0'&&c<='9') c=c-'0';
				else if(c>='a'&&c<='f') c=c-'a'+10;
				else if(c>='A'&&c<='F') c=c-'A'+10;
				else return 0;
				ret+=(c<<off);
				off+=4;
			}
			c=*(--pCur);
		}
		return ret;
	}
    //------------------------------------------------------------------------------
    void splitString(std::vector<char*>& p_pOutVector, char* p_sBuffer, char p_pSeperator)
    {
        //TODO这样会修改 p_sBuffer 需要重新写
        p_pOutVector.clear();
        if (p_sBuffer == NULL) return;
        char* pCur = p_sBuffer;
        p_pOutVector.push_back(pCur);
        for (; 0 != *pCur;)
        {
            if (p_pSeperator == *pCur)
            {
                *pCur = 0;
                if (0 != *(pCur + 1))
                {
                    p_pOutVector.push_back(pCur + 1);
                }
            }
            pCur++;
        }
    }
    //------------------------------------------------------------------------------
    int stringToInt(const char *p_pszStr)
    {
        if (0 == p_pszStr)
            return 0;

        char *p;
        const char *pValue;
        int i;

        if (p_pszStr[0] == '#')
        {
            pValue = p_pszStr + 1;
            i = 16;
        }
        else if (p_pszStr[0] == '0' && p_pszStr[1] == 'x')
        {
            pValue = p_pszStr;
            i = 16;
        }
        else
        {
            pValue = p_pszStr;
            i = 10;
        }
        return strtol(pValue, &p, i);
    }
    //------------------------------------------------------------------------------
    float stringToFloat(const char *p_pszStr)
    {
        if (0 == p_pszStr)
            return 0.0f;
        char *pendptr;
        return (float)strtod(p_pszStr, &pendptr);
    }
    //------------------------------------------------------------------------------
    const char* getExtName(const char* p_sFileName)
    {
        std::vector<char*> vString;
        splitString(vString, (char*)(p_sFileName), '.');
        int nSize = (int)(vString.size());
        if (nSize <= 0)
        {
            return p_sFileName;
        }
        else
        {
            return vString[nSize - 1];
        }
    }
    //------------------------------------------------------------------------------
    std::string stringReplace(std::string & str, const std::string & strsrc, const std::string &strdst)
    {
        std::string::size_type pos = 0;//位置 
        std::string::size_type srclen = strsrc.size();//要替换的字符串大小 
        std::string::size_type dstlen = strdst.size();//目标字符串大小 
        while ((pos = str.find(strsrc, pos)) != std::string::npos)
        {
            str.replace(pos, srclen, strdst);
            pos += dstlen;
        }
        return str;
    }
    //------------------------------------------------------------------------------
    int UTF8ToUnicode(unsigned char *ch, int *unicode)
    {
        unsigned char *p = NULL;
        int e = 0, n = 0;
        if ((p = ch) && unicode)
        {
            if (*p >= 0xfc)
            {
                e = (p[0] & 0x01) << 30;
                e |= (p[1] & 0x3f) << 24;
                e |= (p[2] & 0x3f) << 18;
                e |= (p[3] & 0x3f) << 12;
                e |= (p[4] & 0x3f) << 6;
                e |= (p[5] & 0x3f);
                n = 6;
            }
            else if (*p >= 0xf8)
            {
                e = (p[0] & 0x03) << 24;
                e |= (p[1] & 0x3f) << 18;
                e |= (p[2] & 0x3f) << 12;
                e |= (p[3] & 0x3f) << 6;
                e |= (p[4] & 0x3f);
                n = 5;
            }
            else if (*p >= 0xf0)
            {
                e = (p[0] & 0x07) << 18;
                e |= (p[1] & 0x3f) << 12;
                e |= (p[2] & 0x3f) << 6;
                e |= (p[3] & 0x3f);
                n = 4;
            }
            else if (*p >= 0xe0)
            {
                e = (p[0] & 0x0f) << 12;
                e |= (p[1] & 0x3f) << 6;
                e |= (p[2] & 0x3f);
                n = 3;
            }
            else if (*p >= 0xc0)
            {
                e = (p[0] & 0x1f) << 6;
                e |= (p[1] & 0x3f);
                n = 2;
            }
            else
            {
                e = p[0];
                n = 1;
            }
            *unicode = e;
        }
        return n;
    }
    //------------------------------------------------------------------------------
    int UTF8StrToUnicodeStr(unsigned char * utf8_str, unsigned short * unicode_str, int unicode_str_size)
    {
        int unicode = 0;
        int n = 0;
        int count = 0;
        unsigned char *s = NULL;
        unsigned short *e = NULL;

        s = utf8_str;
        e = unicode_str;

        if ((utf8_str) && (unicode_str))
        {
            while (*s)
            {
                if ((n = UTF8ToUnicode(s, &unicode)) > 0)
                {
                    if ((count++) >= unicode_str_size)
                    {
                        return count;
                    }
                    else
                    {
                        *e = (unsigned short)unicode;
                        e++;
                        *e = 0;
                        s += n;
                    }
                }
                else
                {
                    return count;
                }
            }
        }
        return count;
    }
    //------------------------------------------------------------------------------
    char * UnicodeToUTF8(unsigned int unicode, char *p)
    {
        char *e = p;
        if (!e)
            return NULL;
        if (unicode < 0x80)
        {
            *e++ = unicode;
        }
        else if (unicode < 0x800)
        {
            /* <11011111> < 000 0000 0000> */
            *e++ = ((unicode >> 6) & 0x1f) | 0xc0;
            *e++ = (unicode & 0x3f) | 0x80;
        }
        else if (unicode < 0x10000)
        {
            /* <11101111> <0000 0000 0000 0000> */
            *e++ = ((unicode >> 12) & 0x0f) | 0xe0;
            *e++ = ((unicode >> 6) & 0x3f) | 0x80;
            *e++ = (unicode & 0x3f) | 0x80;
        }
        else if (unicode < 0x200000)
        {
            /* <11110111> <0 0000 0000 0000 0000 0000> */
            *e++ = ((unicode >> 18) & 0x07) | 0xf0;
            *e++ = ((unicode >> 12) & 0x3f) | 0x80;
            *e++ = ((unicode >> 6) & 0x3f) | 0x80;
            *e++ = (unicode & 0x3f) | 0x80;
        }
        else if (unicode < 0x4000000)
        {
            /* <11111011> <00 0000 0000 0000 0000 0000 0000> */
            *e++ = ((unicode >> 24) & 0x03) | 0xf8;
            *e++ = ((unicode >> 18) & 0x3f) | 0x80;
            *e++ = ((unicode >> 12) & 0x3f) | 0x80;
            *e++ = ((unicode >> 6) & 0x3f) | 0x80;
            *e++ = (unicode & 0x3f) | 0x80;
        }
        else
        {
            /* <11111101> <0000 0000 0000 0000 0000 0000 0000 0000> */
            *e++ = ((unicode >> 30) & 0x01) | 0xfc;
            *e++ = ((unicode >> 24) & 0x3f) | 0x80;
            *e++ = ((unicode >> 18) & 0x3f) | 0x80;
            *e++ = ((unicode >> 12) & 0x3f) | 0x80;
            *e++ = ((unicode >> 6) & 0x3f) | 0x80;
            *e++ = (unicode & 0x3f) | 0x80;
        }
        /* Return One step over the end of the utf-8 character buffer */
        return e;
    }
    //------------------------------------------------------------------------------
    char * UnicodeStrToUTF8Str(short * unicode_str, char * utf8_str, int utf8_str_size, int& transedUni, int& usedBuff)
    {
        int unicode = 0;
        transedUni = 0;
        unsigned short* puni = (unsigned short*)unicode_str;
        char* s = utf8_str;
        while ((unicode = (int)*puni++)) {
            //如果超出了。
            usedBuff = s - utf8_str;
            if (usedBuff > utf8_str_size) {
                return NULL;
            }
            s = UnicodeToUTF8(unicode, s);
            transedUni++;
        }
        usedBuff = s - utf8_str;
        usedBuff += 1;	//最後的0
        if (usedBuff  > utf8_str_size)
            return NULL;
        utf8_str[usedBuff - 1] = 0;
        return utf8_str;
    }
    //------------------------------------------------------------------------------
    std::wstring utf8_unicode(const char* str)
    {
        int nLen = strlen(str);
        static unsigned short ucStr[64];
        if (nLen<64)
        {
            nLen = UTF8StrToUnicodeStr((unsigned char*)str, ucStr, 64);
            std::wstring ret = (wchar_t*)ucStr;
            return ret;
        }
        else
        {
            unsigned short* pout = new unsigned short[nLen + 3];
            nLen = UTF8StrToUnicodeStr((unsigned char*)str, pout, nLen + 3);
            std::wstring ret = (wchar_t*)pout;
            delete[] pout;
            return  ret;
        }
    }
    //------------------------------------------------------------------------------
    inline void advSep(const char*& p_pData) {
        while ((*p_pData == '\\' || *p_pData == '/') && *p_pData != 0) {
            p_pData++;
        }
    }
    //------------------------------------------------------------------------------
    inline void advPath(const char*& p_pData) {
        while (*p_pData != '\\'&&*p_pData != '/'&& *p_pData != 0) {
            p_pData++;
        }
    }
    //------------------------------------------------------------------------------
    bool splitPath(const char* p_pszPath, std::vector<std::string>& out) {
        const char* pCurData = p_pszPath;
        while (*pCurData != 0) {
            advSep(pCurData);
            if (*pCurData == 0)
                break;
            const char* pathst = pCurData;
            advPath(pCurData);
            out.push_back(std::string().append(pathst, pCurData - pathst));
        }
        return out.size() > 0;
    }
    //------------------------------------------------------------------------------
    //TODO 没有处理..的情况
    //应该不是很可靠，没有处理大小写等，临时拼凑的。
    std::string normalizePath(const char* p_pszPath, bool toLowerCase, int& p_nProtocol) 
    {
#ifndef WEBASM
        JCUrl url;
        url.parse(p_pszPath);
        p_nProtocol = (int)url.m_nProto;
        if (url.m_nProto == JCUrl::file) {
            std::string ret = url.m_strPath + "/" + url.m_vPath[url.m_vPath.size() - 1];
            if (toLowerCase)
                UTF8ToLowercase((char*)ret.c_str());
            return ret;
        }
        std::string file = url.m_vPath.size()>0 ? url.m_vPath[url.m_vPath.size() - 1] : "";
        std::string attFile = file.length()>0 ? ("/" + file) : "/";
        std::string& query = url.m_Query;
        std::string attQuery = query.length()>0 ? (query) : "";
        std::string ret = url.m_strPath + attFile;
        if (toLowerCase)
            UTF8ToLowercase((char*)ret.c_str());
        ret += attQuery;
        return ret;

        /*
        char* pPos = strchr((char*)p_pszPath,'?');
        //p_pszPath 需要是utf8编码
        std::string url = p_pszPath;
        std::string query="";
        if(pPos){
        url = url.substr(0,pPos-p_pszPath);
        query = pPos;
        }

        if(toLowerCase)
        UTF8ToLowercase((char*)url.c_str());

        std::string ret;
        std::vector<std::string> paths;
        if(!SplitPath(url.c_str(), paths))
        return ret;

        if( paths[0]=="http:") ret="http:/";
        else if(paths[0]=="https:")ret="https:/";
        else if(paths[0].length()==2&&paths[0][1]==':')ret=paths[0];
        else{ ret="/"+paths[0]; };

        for( int i=1,sz=paths.size(); i<sz; i++){
        ret=ret+"/"+paths[i];
        }
        return ret+query;
        */
#else
        return "";
#endif
    }
    //------------------------------------------------------------------------------
    void paserUTF8(std::string p_sBuffer, long p_nSize, std::vector<std::string>& p_vOut)
    {
        p_vOut.clear();
        unsigned char* start = (unsigned char*)p_sBuffer.c_str();
        unsigned char* end = (unsigned char*)p_sBuffer.c_str() + p_nSize;
        while (start < end)
        {
            if (*start < 0x80)
            {
                std::string s("");
                s.append((char*)start, 1);
                p_vOut.push_back(s);
                start++;
            }
            else if (*start < 0xC0)
            {
                break;
            }
            else if (*start < 0xE0)
            {
                if (start >= end - 1) break;
                if ((start[1] & (0xC0)) != 0x80)
                {
                    break;
                }
                std::string s("");
                s.append((char*)start, 2);
                p_vOut.push_back(s);
                start += 2;
            }
            else if (*start < 0xF0)
            {
                if (start >= end - 2) break;
                if ((start[1] & 0xC0) != 0x80 || (start[2] & 0xC0) != 0x80)
                {
                    break;
                }
                std::string s("");
                s.append((char*)start, 3);
                p_vOut.push_back(s);
                start += 3;
            }
            else if (*start < 0xf8) {
                std::string s("");
                s.append((char*)start, 4);
                p_vOut.push_back(s);
                start += 4;
            }
            else if (*start < 0xfc) {
                std::string s("");
                s.append((char*)start, 5);
                p_vOut.push_back(s);
                start += 5;
            }
            else {
                //这个不知道了，不应该吧
                std::string s("");
                s.append((char*)start, 6);
                p_vOut.push_back(s);
                start += 6;
            }
        }
    }
    //------------------------------------------------------------------------------
    char* LayaStrlwr(char* p_str)
    {
#ifdef WIN32
        return _strlwr(p_str);
#else
        char* orig = p_str;
        for (; *p_str != '\0'; p_str++)
            *p_str = tolower(*p_str);
        return orig;
#endif
    }
    //------------------------------------------------------------------------------
    char* LayaStrupr(char* p_str)
    {
#ifdef WIN32
        return _strupr(p_str);
#else
        char * orign = p_str;
        for (; *p_str != '\0'; p_str++)
            *p_str = toupper(*p_str);
        return orign;
#endif
    }
    //------------------------------------------------------------------------------
    char* UTF8ToLowercase(char* p_pszStr) {
        char* pPtr = p_pszStr;
        while ((*pPtr) != 0) {
            int v = *pPtr;
            if (v >= 0xfc)pPtr += 6;
            else if (v >= 0xf8)pPtr += 5;
            else if (v >= 0xf0)pPtr += 4;
            else if (v >= 0xe0)pPtr += 3;
            else if (v >= 0xc0)pPtr += 2;
            else { //>0x80的不用管
                if (v >= 'A' && v <= 'Z') {
                    *pPtr -= ('A' - 'a');
                }
                pPtr += 1;
            }
        }
        return p_pszStr;
    }
    //------------------------------------------------------------------------------
    std::string getLowercaseExtOfUrl(const char* p_pszUrl) {
        if (!p_pszUrl)return "";
        const char* pQuery = strchr(p_pszUrl, '?');
        char* pStart = (char*)(pQuery ? (pQuery - 1) : (p_pszUrl + strlen(p_pszUrl) - 1));
        char* pDot = pStart;
        while (pDot != p_pszUrl && *pDot != '.') {
            pDot--;
        }
        if (pDot != p_pszUrl) {
            static char tmpbuf[64];
            memset(tmpbuf, 0, sizeof(tmpbuf));
            int extlen = pStart - pDot + 1;//算上点了
            if (extlen > sizeof(tmpbuf)) {
                return "";
            }
            //转成小写
            memcpy(tmpbuf, pDot, extlen);
            for (int ci = 0; ci < extlen; ci++) {
                if (tmpbuf[ci] >= 'A'&&tmpbuf[ci] <= 'Z') {
                    tmpbuf[ci] += ('a' - 'A');
                }
            }
            return std::string(tmpbuf);
        }
        else
            return "";
    }
    //------------------------------------------------------------------------------
    unsigned char _ToHex(unsigned char x) {
        return  x > 9 ? x + 55 : x + 48;
    }
    //------------------------------------------------------------------------------
    unsigned char _FromHex(unsigned char x) {
        unsigned char y;
        if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
        else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
        else if (x >= '0' && x <= '9') y = x - '0';
        else {
            LOGE("_FromHex error:%d", x);
            y = 0;
        }
        return y;
    }
    //------------------------------------------------------------------------------
    std::string UrlEncode(const char* str)
    {
        std::string strTemp = "";
        size_t length = strlen(str);
        for (size_t i = 0; i < length; i++)
        {
            if (isalnum((unsigned char)str[i]) ||
                (str[i] == '-') ||
                (str[i] == '_') ||
                (str[i] == '.') ||
                (str[i] == '~') ||
                (str[i] == '=') ||//新加的
                (str[i] == '&') ||//新加的
                (str[i] == '%') ||
                (str[i] == ':') ||
                (str[i] == '|')
                )
                strTemp += str[i];
            else if (str[i] == ' ')
                strTemp += "+";
            else
            {
                strTemp += '%';
                strTemp += _ToHex((unsigned char)str[i] >> 4);
                strTemp += _ToHex((unsigned char)str[i] % 16);
            }
        }
        return strTemp;
    }
    //------------------------------------------------------------------------------
    std::string UrlDecode(const char* str)
    {
        std::string strTemp = "";
        size_t length = strlen(str);
        for (size_t i = 0; i < length; i++) {
            if (str[i] == '+') strTemp += ' ';
            else if (str[i] == '%') {
                if (i + 2 < length) {
                    unsigned char high = _FromHex((unsigned char)str[++i]);
                    unsigned char low = _FromHex((unsigned char)str[++i]);
                    strTemp += high * 16 + low;
                }
                else {
                    LOGE("UrlDecode length error:%s", str);
                }
            }
            else strTemp += str[i];
        }
        return strTemp;
    }
    //------------------------------------------------------------------------------
    double tmGetCurms()
    {
#ifdef WEBASM
        return DateNow();
#else
    #ifdef __APPLE__

        //下面的方法好像也可以达到精度
        //    struct timeval tv;
        //    gettimeofday(&tv, NULL);
        //    return (tv.tv_sec*1000 + tv.tv_usec/1000);

        //根据cpu频率获得的时间
        static mach_timebase_info_data_t info;

        if (info.denom == 0) {
            if (mach_timebase_info(&info) != KERN_SUCCESS)
                return 0;
        }

        int64_t nanosec = mach_absolute_time()*info.numer / info.denom;
        return (nanosec / 1e6);
#elif ANDROID
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        return now.tv_sec * 1000.0 + now.tv_nsec / 1e6;
#elif WIN32
        static __int64 freq = 0;
        if (freq == 0) {
            LARGE_INTEGER lifreq;
            QueryPerformanceFrequency(&lifreq);
            freq = lifreq.QuadPart;
        }
        LARGE_INTEGER tm;
        double ret;
        QueryPerformanceCounter(&tm);
        ret = tm.QuadPart * 1000.0 / freq;
        return ret;//GetTickCount();
                   //return ret;
#else
        return 0;
    #endif
#endif
    }


    int getAppUsedMem() 
    {
#ifdef WIN32
        HANDLE handle = GetCurrentProcess();
        PROCESS_MEMORY_COUNTERS pmc;
        GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
        //WorkingSetSize 使用的内存
        //PeakWorkingSetSize 峰值内存
        //PagefileUsage 虚拟内存
        //PeakPagefileUsage 峰值虚拟内存
        return pmc.WorkingSetSize / 1024;
#elif ANDROID
        return 0;
#elif __APPLE__
        return 0;
#endif
    }

    int hashstr(const char *s)
    {
        int even, odd;
        int i = 0;
        int mask = 0x1F;
        int ret;
        even = odd = 0;
        while (*s != '\0') {
            if (i & 1) {
                odd ^= *s;
            }
            else {
                even ^= *s;
            }
            ++s;
            ++i;
        }
        ret = even & mask;
        ret <<= 5;
        ret += (odd & mask);
        return ret;
    }
    void mulMatrixArray(float* leftMatrixE, float* rightMatrix, float* outArray, int nOffset)
    {
        int i = 0;
        float ai0 = 0, ai1 = 0, ai2 = 0, ai3 = 0;
        float m11 = rightMatrix[0], m12 = rightMatrix[1], m13 = rightMatrix[2], m14 = rightMatrix[3];
        float m21 = rightMatrix[4], m22 = rightMatrix[5], m23 = rightMatrix[6], m24 = rightMatrix[7];
        float m31 = rightMatrix[8], m32 = rightMatrix[9], m33 = rightMatrix[10], m34 = rightMatrix[11];
        float m41 = rightMatrix[12], m42 = rightMatrix[13], m43 = rightMatrix[14], m44 = rightMatrix[15];
        int ai0OutOffset = nOffset;
        int ai1OutOffset = nOffset + 4;
        int ai2OutOffset = nOffset + 8;
        int ai3OutOffset = nOffset + 12;
        for (i = 0; i < 4; i++) 
        {
            ai0 = leftMatrixE[i];
            ai1 = leftMatrixE[i + 4];
            ai2 = leftMatrixE[i + 8];
            ai3 = leftMatrixE[i + 12];
            outArray[ai0OutOffset + i] = ai0 * m11 + ai1 * m12 + ai2 * m13 + ai3 * m14;
            outArray[ai1OutOffset + i] = ai0 * m21 + ai1 * m22 + ai2 * m23 + ai3 * m24;
            outArray[ai2OutOffset + i] = ai0 * m31 + ai1 * m32 + ai2 * m33 + ai3 * m34;
            outArray[ai3OutOffset + i] = ai0 * m41 + ai1 * m42 + ai2 * m43 + ai3 * m44;
        }
    }
    void flipPixelsY(uint8_t* pixels, int bytesPerRow, int rows)
    {
        if (!pixels) 
        { 
            return; 
        }

        uint32_t middle = rows / 2;
        uint32_t intsPerRow = bytesPerRow / sizeof(uint32_t);
        uint32_t remainingBytes = bytesPerRow - intsPerRow * sizeof(uint32_t);

        for (uint32_t rowTop = 0, rowBottom = rows - 1; rowTop < middle; rowTop++, rowBottom--) {

            // Swap bytes in packs of sizeof(GLuint) bytes
            uint32_t *iTop = (uint32_t *)(pixels + rowTop * bytesPerRow);
            uint32_t *iBottom = (uint32_t *)(pixels + rowBottom * bytesPerRow);

            uint32_t itmp;
            int32_t n = intsPerRow;
            do {
                itmp = *iTop;
                *iTop++ = *iBottom;
                *iBottom++ = itmp;
            } while (--n > 0);

            // Swap the remaining bytes
            uint8_t *bTop = (uint8_t *)iTop;
            uint8_t *bBottom = (uint8_t *)iBottom;

            uint8_t btmp;
            switch (remainingBytes) {
            case 3: btmp = *bTop; *bTop++ = *bBottom; *bBottom++ = btmp;
            case 2: btmp = *bTop; *bTop++ = *bBottom; *bBottom++ = btmp;
            case 1: btmp = *bTop; *bTop = *bBottom; *bBottom = btmp;
            }
        }
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
