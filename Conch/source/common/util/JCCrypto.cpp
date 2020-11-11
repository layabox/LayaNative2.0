/**
@file			JCCrypto.cpp
@brief			
@author			hugao
@version		1.0
@date			2016_5_12
*/

#include "JCCrypto.h"
#include <stdlib.h>
#include <string.h>

namespace laya
{
    unsigned char decodeGetByte(char c);
    char encodeGetChar(unsigned char num);

    unsigned char decodeGetByte(char c)
    {
        if (c == '+')
            return 62;
        else if (c == '/')
            return 63;
        else if (c <= '9')
            return (unsigned char)(c - '0' + 52);
        else if (c == '=')
            return 64;
        else if (c <= 'Z')
            return (unsigned char)(c - 'A');
        else if (c <= 'z')
            return (unsigned char)(c - 'a' + 26);
        return 64;
    }
    unsigned int encodeGetChar(char *pDest, const char *pSrc, unsigned int srclen)
    {
        unsigned char input[4];
        unsigned int i, index = 0;
        for (i = 0; i < srclen; i += 4)
        {
            //byte[0]
            input[0] = decodeGetByte(pSrc[i]);
            input[1] = decodeGetByte(pSrc[i + 1]);
            pDest[index++] = (input[0] << 2) + (input[1] >> 4);

            //byte[1]
            if (pSrc[i + 2] != '=')
            {
                input[2] = decodeGetByte(pSrc[i + 2]);
                pDest[index++] = ((input[1] & 0x0f) << 4) + (input[2] >> 2);
            }

            //byte[2]
            if (pSrc[i + 3] != '=')
            {
                input[3] = decodeGetByte(pSrc[i + 3]);
                pDest[index++] = ((input[2] & 0x03) << 6) + (input[3]);
            }
        }

        //null-terminator
        pDest[index] = 0;
        return index;
    }
    inline char encodeGetChar(unsigned char num)
    {
        return
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789"
            "+/="[num];
    }
    unsigned int base64Encode(char *pDest, const char *pSrc, unsigned int srclen)
    {
        unsigned char input[3], output[4];
        unsigned int i, index_src = 0, index_dest = 0;
        for (i = 0; i < srclen; i += 3)
        {
            //char [0]
            input[0] = pSrc[index_src++];
            output[0] = (unsigned char)(input[0] >> 2);
            pDest[index_dest++] = encodeGetChar(output[0]);

            //char [1]
            if (index_src < srclen)
            {
                input[1] = pSrc[index_src++];
                output[1] = (unsigned char)(((input[0] & 0x03) << 4) + (input[1] >> 4));
                pDest[index_dest++] = encodeGetChar(output[1]);
            }
            else
            {
                output[1] = (unsigned char)((input[0] & 0x03) << 4);
                pDest[index_dest++] = encodeGetChar(output[1]);
                pDest[index_dest++] = '=';
                pDest[index_dest++] = '=';
                break;
            }

            //char [2]
            if (index_src < srclen)
            {
                input[2] = pSrc[index_src++];
                output[2] = (unsigned char)(((input[1] & 0x0f) << 2) + (input[2] >> 6));
                pDest[index_dest++] = encodeGetChar(output[2]);
            }
            else
            {
                output[2] = (unsigned char)((input[1] & 0x0f) << 2);
                pDest[index_dest++] = encodeGetChar(output[2]);
                pDest[index_dest++] = '=';
                break;
            }

            //char [3]
            output[3] = (unsigned char)(input[2] & 0x3f);
            pDest[index_dest++] = encodeGetChar(output[3]);
        }
        //null-terminator
        pDest[index_dest] = 0;
        return index_dest;
    }
    //------------------------------------------------------------------------------

#define GET_UINT32(n,b,i)                                       \
{                                                               \
    (n) = (unsigned int) ((unsigned char *) b)[(i)]                           \
      | (((unsigned int) ((unsigned char *) b)[(i)+1]) <<  8)                 \
      | (((unsigned int) ((unsigned char *) b)[(i)+2]) << 16)                 \
      | (((unsigned int) ((unsigned char *) b)[(i)+3]) << 24);                \
}

#define PUT_UINT32(n,b,i)                                       \
{                                                               \
    (((unsigned char *) b)[(i)]  ) = (unsigned char) (((n)      ) & 0xFF);      \
    (((unsigned char *) b)[(i)+1]) = (unsigned char) (((n) >>  8) & 0xFF);      \
    (((unsigned char *) b)[(i)+2]) = (unsigned char) (((n) >> 16) & 0xFF);      \
    (((unsigned char *) b)[(i)+3]) = (unsigned char) (((n) >> 24) & 0xFF);      \
}
#ifndef WEBASM
    void JCMD5::md5_starts(struct md5_context *ctx)
    {
        ctx->total[0] = 0;
        ctx->total[1] = 0;
        ctx->state[0] = 0x67452301;
        ctx->state[1] = 0xEFCDAB89;
        ctx->state[2] = 0x98BADCFE;
        ctx->state[3] = 0x10325476;
    }

    void JCMD5::md5_process(struct md5_context *ctx, unsigned char data[64])
    {
        unsigned int A, B, C, D, X[16];

        GET_UINT32(X[0], data, 0);
        GET_UINT32(X[1], data, 4);
        GET_UINT32(X[2], data, 8);
        GET_UINT32(X[3], data, 12);
        GET_UINT32(X[4], data, 16);
        GET_UINT32(X[5], data, 20);
        GET_UINT32(X[6], data, 24);
        GET_UINT32(X[7], data, 28);
        GET_UINT32(X[8], data, 32);
        GET_UINT32(X[9], data, 36);
        GET_UINT32(X[10], data, 40);
        GET_UINT32(X[11], data, 44);
        GET_UINT32(X[12], data, 48);
        GET_UINT32(X[13], data, 52);
        GET_UINT32(X[14], data, 56);
        GET_UINT32(X[15], data, 60);

#define S(x,n) ((x << n) | ((x & 0xFFFFFFFF) >> (32 - n)))

#define P(a,b,c,d,k,s,t)                                \
{                                                       \
    a += F(b,c,d) + X[k] + t; a = S(a,s) + b;           \
}

        A = ctx->state[0];
        B = ctx->state[1];
        C = ctx->state[2];
        D = ctx->state[3];

#define F(x,y,z) (z ^ (x & (y ^ z)))

        P(A, B, C, D, 0, 7, 0xD76AA478);
        P(D, A, B, C, 1, 12, 0xE8C7B756);
        P(C, D, A, B, 2, 17, 0x242070DB);
        P(B, C, D, A, 3, 22, 0xC1BDCEEE);
        P(A, B, C, D, 4, 7, 0xF57C0FAF);
        P(D, A, B, C, 5, 12, 0x4787C62A);
        P(C, D, A, B, 6, 17, 0xA8304613);
        P(B, C, D, A, 7, 22, 0xFD469501);
        P(A, B, C, D, 8, 7, 0x698098D8);
        P(D, A, B, C, 9, 12, 0x8B44F7AF);
        P(C, D, A, B, 10, 17, 0xFFFF5BB1);
        P(B, C, D, A, 11, 22, 0x895CD7BE);
        P(A, B, C, D, 12, 7, 0x6B901122);
        P(D, A, B, C, 13, 12, 0xFD987193);
        P(C, D, A, B, 14, 17, 0xA679438E);
        P(B, C, D, A, 15, 22, 0x49B40821);

#undef F

#define F(x,y,z) (y ^ (z & (x ^ y)))

        P(A, B, C, D, 1, 5, 0xF61E2562);
        P(D, A, B, C, 6, 9, 0xC040B340);
        P(C, D, A, B, 11, 14, 0x265E5A51);
        P(B, C, D, A, 0, 20, 0xE9B6C7AA);
        P(A, B, C, D, 5, 5, 0xD62F105D);
        P(D, A, B, C, 10, 9, 0x02441453);
        P(C, D, A, B, 15, 14, 0xD8A1E681);
        P(B, C, D, A, 4, 20, 0xE7D3FBC8);
        P(A, B, C, D, 9, 5, 0x21E1CDE6);
        P(D, A, B, C, 14, 9, 0xC33707D6);
        P(C, D, A, B, 3, 14, 0xF4D50D87);
        P(B, C, D, A, 8, 20, 0x455A14ED);
        P(A, B, C, D, 13, 5, 0xA9E3E905);
        P(D, A, B, C, 2, 9, 0xFCEFA3F8);
        P(C, D, A, B, 7, 14, 0x676F02D9);
        P(B, C, D, A, 12, 20, 0x8D2A4C8A);

#undef F

#define F(x,y,z) (x ^ y ^ z)

        P(A, B, C, D, 5, 4, 0xFFFA3942);
        P(D, A, B, C, 8, 11, 0x8771F681);
        P(C, D, A, B, 11, 16, 0x6D9D6122);
        P(B, C, D, A, 14, 23, 0xFDE5380C);
        P(A, B, C, D, 1, 4, 0xA4BEEA44);
        P(D, A, B, C, 4, 11, 0x4BDECFA9);
        P(C, D, A, B, 7, 16, 0xF6BB4B60);
        P(B, C, D, A, 10, 23, 0xBEBFBC70);
        P(A, B, C, D, 13, 4, 0x289B7EC6);
        P(D, A, B, C, 0, 11, 0xEAA127FA);
        P(C, D, A, B, 3, 16, 0xD4EF3085);
        P(B, C, D, A, 6, 23, 0x04881D05);
        P(A, B, C, D, 9, 4, 0xD9D4D039);
        P(D, A, B, C, 12, 11, 0xE6DB99E5);
        P(C, D, A, B, 15, 16, 0x1FA27CF8);
        P(B, C, D, A, 2, 23, 0xC4AC5665);

#undef F

#define F(x,y,z) (y ^ (x | ~z))

        P(A, B, C, D, 0, 6, 0xF4292244);
        P(D, A, B, C, 7, 10, 0x432AFF97);
        P(C, D, A, B, 14, 15, 0xAB9423A7);
        P(B, C, D, A, 5, 21, 0xFC93A039);
        P(A, B, C, D, 12, 6, 0x655B59C3);
        P(D, A, B, C, 3, 10, 0x8F0CCC92);
        P(C, D, A, B, 10, 15, 0xFFEFF47D);
        P(B, C, D, A, 1, 21, 0x85845DD1);
        P(A, B, C, D, 8, 6, 0x6FA87E4F);
        P(D, A, B, C, 15, 10, 0xFE2CE6E0);
        P(C, D, A, B, 6, 15, 0xA3014314);
        P(B, C, D, A, 13, 21, 0x4E0811A1);
        P(A, B, C, D, 4, 6, 0xF7537E82);
        P(D, A, B, C, 11, 10, 0xBD3AF235);
        P(C, D, A, B, 2, 15, 0x2AD7D2BB);
        P(B, C, D, A, 9, 21, 0xEB86D391);

#undef F

        ctx->state[0] += A;
        ctx->state[1] += B;
        ctx->state[2] += C;
        ctx->state[3] += D;
    }

    void JCMD5::md5_update(struct md5_context *ctx, unsigned char *input, size_t length)
    {
        unsigned int left, fill;

        if (!length) return;

        left = (ctx->total[0] >> 3) & 0x3F;
        fill = 64 - left;

        ctx->total[0] += length << 3;
        ctx->total[1] += length >> 29;

        ctx->total[0] &= 0xFFFFFFFF;
        ctx->total[1] += ctx->total[0] < length << 3;

        if (left && length >= fill)
        {
            memcpy((void *)(ctx->buffer + left), (void *)input, fill);
            md5_process(ctx, ctx->buffer);
            length -= fill;
            input += fill;
            left = 0;
        }

        while (length >= 64)
        {
            md5_process(ctx, input);
            length -= 64;
            input += 64;
        }

        if (length)
        {
            memcpy((void *)(ctx->buffer + left), (void *)input, length);
        }
    }

    static unsigned char md5_padding[64] =
    {
        0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    void JCMD5::md5_finish(struct md5_context *ctx, unsigned char digest[16])
    {
        unsigned int last, padn;
        unsigned char msglen[8];

        PUT_UINT32(ctx->total[0], msglen, 0);
        PUT_UINT32(ctx->total[1], msglen, 4);

        last = (ctx->total[0] >> 3) & 0x3F;
        padn = (last < 56) ? (56 - last) : (120 - last);

        md5_update(ctx, md5_padding, padn);
        md5_update(ctx, msglen, 8);

        PUT_UINT32(ctx->state[0], digest, 0);
        PUT_UINT32(ctx->state[1], digest, 4);
        PUT_UINT32(ctx->state[2], digest, 8);
        PUT_UINT32(ctx->state[3], digest, 12);
    }

    void JCMD5::GenerateMD5(unsigned char* buffer, size_t bufferlen)
    {
        struct md5_context context;
        md5_starts(&context);
        md5_update(&context, buffer, bufferlen);
        md5_finish(&context, (unsigned char*)m_data);
    }

    JCMD5::JCMD5()
    {
        for (int i = 0; i < 4; i++)
            m_data[i] = 0;
    }

    JCMD5::JCMD5(unsigned long* md5src)
    {
        memcpy(m_data, md5src, 16);
    }
    int _httoi(const char *value)
    {
        struct CHexMap
        {
            char chr;
            int value;
        };
        const int HexMapL = 16;
        CHexMap HexMap[HexMapL] =
        {
            { '0', 0 },{ '1', 1 },
            { '2', 2 },{ '3', 3 },
            { '4', 4 },{ '5', 5 },
            { '6', 6 },{ '7', 7 },
            { '8', 8 },{ '9', 9 },
            { 'a', 10 },{ 'b', 11 },
            { 'c', 12 },{ 'd', 13 },
            { 'e', 14 },{ 'f', 15 }
        };
        char *mstr = strdup(value);
        char *s = mstr;
        int result = 0;
        if (*s == '0' && *(s + 1) == 'X') s += 2;
        bool firsttime = true;
        while (*s != '\0')
        {
            bool found = false;
            for (int i = 0; i < HexMapL; i++)
            {
                if (*s == HexMap[i].chr)
                {
                    if (!firsttime) result <<= 4;
                    result |= HexMap[i].value;
                    found = true;
                    break;
                }
            }
            if (!found) break;
            s++;
            firsttime = false;
        }
        free(mstr);
        return result;
    }


    JCMD5::JCMD5(const char* md5src)
    {
        if (strcmp(md5src, "") == 0)
        {
            for (int i = 0; i < 4; i++)
                m_data[i] = 0;
            return;
        }
        for (int j = 0; j < 16; j++)
        {
            char buf[10];
            strncpy(buf, md5src, 2);
            md5src += 2;
            ((unsigned char*)m_data)[j] = _httoi(buf);
        }
    }

    JCMD5 JCMD5::operator +(JCMD5 adder)
    {
        unsigned long m_newdata[4];
        for (int i = 0; i < 4; i++)
            m_newdata[i] = m_data[i] ^ (adder.m_data[i]);
        return JCMD5(m_newdata);
    }

    bool JCMD5::operator ==(JCMD5 cmper)
    {
        return (memcmp(cmper.m_data, m_data, 16) == 0);
    }

    //void CMD5::operator =(CMD5 equer)
    //{
    // memcpy(m_data,equer.m_data ,16);
    //}

    string JCMD5::ToString()
    {
        char output[33];
        for (int j = 0; j < 16; j++)
        {
            sprintf(output + j * 2, "%02x", ((unsigned char*)m_data)[j]);
        }
        return string(output);
    }
#ifndef THIN_COMMON_WITHOUT_DOWNLOAD
    string JCEncrypt::s_sPassCode = "LayaBoxChgTWorld";
    string JCEncrypt::s_sPreCode = "!#$LAYABOX";
    int	   JCEncrypt::s_nPreLen = s_sPreCode.size();
    string JCEncrypt::s_sPassMd5;
    JCEncrypt::JCEncrypt()
    {

    }
    JCEncrypt::~JCEncrypt()
    {

    }
    string JCEncrypt::getpassCode(string url)
    {
        int len = url.size();
        JCMD5 imd5;
        imd5.GenerateMD5((unsigned char *)url.c_str(), len);
        string result = imd5.ToString();
        for (int i = 0, len = result.size(); i < len; i++)
        {
            result[i] ^= s_sPassCode[i % 16];
        }
        s_sPassMd5 = result;
        return result;
    }
    bool JCEncrypt::decrypt(char *buf, int len)
    {
        if (strncmp(buf, s_sPreCode.c_str(), s_nPreLen) == 0)
        {
            for (int i = 0; i < len - s_nPreLen; i++)
            {
                buf[i] = buf[i + s_nPreLen] ^ s_sPassMd5[i % 32];
            }
            return true;
        }
        else
        {
            return false;
        }
    }
#endif
#endif
    unsigned int JCBKDRHash::hashStr(const unsigned char* str){
        unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
        unsigned int hash = 0;
        while (*str) {
            hash = hash * seed + (*str++);
        }
        return (hash & 0x7FFFFFFF);
    }

    unsigned int JCBKDRHash::hashMem(const unsigned char* mem, int len){
        unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
        unsigned int hash = 0;
        for (int i = 0; i < len; i++) {
            hash = hash * seed + (*mem++);
        }
        hash = hash & 0x7FFFFFFF;
        return hash;
    }

    static const uint32_t crc32tab[] = {
        0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL,
        0x076dc419L, 0x706af48fL, 0xe963a535L, 0x9e6495a3L,
        0x0edb8832L, 0x79dcb8a4L, 0xe0d5e91eL, 0x97d2d988L,
        0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L, 0x90bf1d91L,
        0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
        0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L,
        0x136c9856L, 0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL,
        0x14015c4fL, 0x63066cd9L, 0xfa0f3d63L, 0x8d080df5L,
        0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L, 0xa2677172L,
        0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
        0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L,
        0x32d86ce3L, 0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L,
        0x26d930acL, 0x51de003aL, 0xc8d75180L, 0xbfd06116L,
        0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L, 0xb8bda50fL,
        0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
        0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL,
        0x76dc4190L, 0x01db7106L, 0x98d220bcL, 0xefd5102aL,
        0x71b18589L, 0x06b6b51fL, 0x9fbfe4a5L, 0xe8b8d433L,
        0x7807c9a2L, 0x0f00f934L, 0x9609a88eL, 0xe10e9818L,
        0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
        0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL,
        0x6c0695edL, 0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L,
        0x65b0d9c6L, 0x12b7e950L, 0x8bbeb8eaL, 0xfcb9887cL,
        0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L, 0xfbd44c65L,
        0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
        0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL,
        0x4369e96aL, 0x346ed9fcL, 0xad678846L, 0xda60b8d0L,
        0x44042d73L, 0x33031de5L, 0xaa0a4c5fL, 0xdd0d7cc9L,
        0x5005713cL, 0x270241aaL, 0xbe0b1010L, 0xc90c2086L,
        0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
        0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L,
        0x59b33d17L, 0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL,
        0xedb88320L, 0x9abfb3b6L, 0x03b6e20cL, 0x74b1d29aL,
        0xead54739L, 0x9dd277afL, 0x04db2615L, 0x73dc1683L,
        0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
        0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L,
        0xf00f9344L, 0x8708a3d2L, 0x1e01f268L, 0x6906c2feL,
        0xf762575dL, 0x806567cbL, 0x196c3671L, 0x6e6b06e7L,
        0xfed41b76L, 0x89d32be0L, 0x10da7a5aL, 0x67dd4accL,
        0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
        0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L,
        0xd1bb67f1L, 0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL,
        0xd80d2bdaL, 0xaf0a1b4cL, 0x36034af6L, 0x41047a60L,
        0xdf60efc3L, 0xa867df55L, 0x316e8eefL, 0x4669be79L,
        0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
        0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL,
        0xc5ba3bbeL, 0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L,
        0xc2d7ffa7L, 0xb5d0cf31L, 0x2cd99e8bL, 0x5bdeae1dL,
        0x9b64c2b0L, 0xec63f226L, 0x756aa39cL, 0x026d930aL,
        0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
        0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L,
        0x92d28e9bL, 0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L,
        0x86d3d2d4L, 0xf1d4e242L, 0x68ddb3f8L, 0x1fda836eL,
        0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L, 0x18b74777L,
        0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
        0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L,
        0xa00ae278L, 0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L,
        0xa7672661L, 0xd06016f7L, 0x4969474dL, 0x3e6e77dbL,
        0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L, 0x37d83bf0L,
        0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
        0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L,
        0xbad03605L, 0xcdd70693L, 0x54de5729L, 0x23d967bfL,
        0xb3667a2eL, 0xc4614ab8L, 0x5d681b02L, 0x2a6f2b94L,
        0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL, 0x2d02ef8dL
    };

    uint32_t crc32(const unsigned char *buf, uint32_t size){
        uint32_t i, crc;
        crc = 0xFFFFFFFF;
        for (i = 0; i < size; i++)
            crc = crc32tab[(crc ^ buf[i]) & 0xff] ^ (crc >> 8);
        return crc ^ 0xFFFFFFFF;
    }
    static const char base64_table[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/', '\0'
    };

    static const char base64_pad = '=';

    static const short base64_reverse_table[256] = {
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -2, -2, -1, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, 62, -2, -2, -2, 63,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -2, -2, -2, -2, -2, -2,
        -2,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -2, -2, -2, -2, -2,
        -2, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2
    };


    char* base64_encode(const unsigned char *str, int length, int *ret_length) /* {{{ */
    {
        const unsigned char *current = str;
        char *p;
        char *result;

        if ((length + 2) < 0 || ((length + 2) / 3) >= (1 << (sizeof(int) * 8 - 2))) {
            if (ret_length != NULL) {
                *ret_length = 0;
            }
            return NULL;
        }

        //result = (char *)malloc((((length + 2) / 3) * 4)*(sizeof(char))+(1));
        result = new char[(((length + 2) / 3) * 4)*(sizeof(char)) + (1)];
        if (result == NULL) {
            //fprintf(stderr, "out of memory!\n");
            //exit(1);
            return NULL;
        }
        p = result;

        while (length > 2) {
            *p++ = base64_table[current[0] >> 2];
            *p++ = base64_table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
            *p++ = base64_table[((current[1] & 0x0f) << 2) + (current[2] >> 6)];
            *p++ = base64_table[current[2] & 0x3f];

            current += 3;
            length -= 3;
        }

        if (length != 0) {
            *p++ = base64_table[current[0] >> 2];
            if (length > 1) {
                *p++ = base64_table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
                *p++ = base64_table[(current[1] & 0x0f) << 2];
                *p++ = base64_pad;
            }
            else {
                *p++ = base64_table[(current[0] & 0x03) << 4];
                *p++ = base64_pad;
                *p++ = base64_pad;
            }
        }
        if (ret_length != NULL) {
            *ret_length = (int)(p - result);
        }
        *p = '\0';
        return result;
    }


    char *base64_decode(const unsigned char *str, int length, int *ret_length)
    {
        const unsigned char *current = str;
        int ch, i = 0, j = 0, k;
        char *result;

        //result = (char *)malloc(length+1);
        result = new char[length + 1];
        if (result == NULL) {
            //fprintf(stderr, "out of memory!\n");
            //exit(1);
            return NULL;
        }

        while ((ch = *current++) != '\0' && length-- > 0) {
            if (ch == base64_pad) {
                if (*current != '=' && (i % 4) == 1) {
                    //free(result);
                    delete[] result;
                    return NULL;
                }
                continue;
            }

            ch = base64_reverse_table[ch];
            if ((1 && ch < 0) || ch == -1) {
                continue;
            }
            else if (ch == -2) {
                //free(result);
                delete[] result;
                return NULL;
            }

            switch (i % 4) {
            case 0:
                result[j] = ch << 2;
                break;
            case 1:
                result[j++] |= ch >> 4;
                result[j] = (ch & 0x0f) << 4;
                break;
            case 2:
                result[j++] |= ch >> 2;
                result[j] = (ch & 0x03) << 6;
                break;
            case 3:
                result[j++] |= ch;
                break;
            }
            i++;
        }

        k = j;

        if (ch == base64_pad) {
            switch (i % 4) {
            case 1:
                //free(result);
                delete[] result;
                return NULL;
            case 2:
                k++;
            case 3:
                result[k] = 0;
            }
        }
        if (ret_length) {
            *ret_length = j;
        }
        result[j] = '\0';
        return result;
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
