/**
@file			JCZlib.cpp
@brief			
@author			hugao
@version		1.0
@date			2016_5_13
*/

#include "JCZlib.h"
#include <string>

namespace laya
{
     //------------------------------------------------------------------------------
    uLong getLayaBoxUncompressSize(unsigned char* p_sBuffer, int buflen, int& p_nOffset)
    {
        if (p_sBuffer == NULL) return 0;
        if (buflen<24) return 0;
        //我们特有的头 LayaBoxCompress=true
        char sHead[21];
        memset(sHead, 0, 21);
        memcpy(sHead, p_sBuffer, sizeof(char) * 20);

        if (strcmp(sHead, "LayaBoxCompress=true") == 0)
        {
            uLong nUncompressSize = *(int*)(p_sBuffer + 20);
            p_nOffset = 20 + 4;
            return nUncompressSize;
        }
        return 0;
    }
    //------------------------------------------------------------------------------
    unsigned char* zlibByteArrayCompress(unsigned char* p_sBuffer, int buflen, int &realCount)
    {
        uLong tlen = buflen;
        z_stream zs;
        int err;
        memset(&zs, 0, sizeof(zs));
        if (deflateInit(&zs, 9))
        {
            printf("deflateInit() failed!\n");
            return NULL;
        }
        uLong blen = deflateBound(&zs, tlen);
        unsigned char* sUCBuffer = new unsigned char[blen];
        zs.avail_in = buflen;
        zs.avail_out = blen;
        zs.next_in = p_sBuffer;
        zs.next_out = sUCBuffer;

        while (zs.total_in != buflen&&zs.total_out < blen)
        {
            zs.avail_in = zs.avail_out = buflen;
            err = deflate(&zs, Z_NO_FLUSH);
        }
        int status = deflate(&zs, Z_NO_FLUSH);

        for (;;)
        {
            zs.avail_out = blen;
            err = deflate(&zs, Z_FINISH);
            if (err == Z_STREAM_END)break;
            if (err != Z_OK)
            {
                printf("deflateEnd() failed!\n");
                return NULL;
            }
        }
        if (deflateEnd(&zs) != Z_OK)
        {
            printf("deflateEnd() failed!\n");
            return NULL;
        }
        realCount = zs.total_out;
        return sUCBuffer;


        /*uLong tlen = buflen;
        uLong blen = compressBound(tlen);
        unsigned char* sUCBuffer = new unsigned char[blen];
        if (compress(sUCBuffer, &blen, p_sBuffer, tlen) != Z_OK)
        {
        printf("compress failed!\n");
        return NULL;
        }
        else
        {
        realCount = blen;
        return sUCBuffer;
        }*/
    }
    //------------------------------------------------------------------------------
    unsigned char* zlibByteArrayUnCompress(unsigned char* p_sBuffer, int buflen, int &realCount)
    {
        uLong tlen = buflen;
        z_stream zs;
        memset(&zs, 0, sizeof(zs));
        //uLong blen = deflateBound(&zs, tlen);
        unsigned char* sUCBuffer = new unsigned char[1024];
        unsigned char* pCurBuff = sUCBuffer;
        int curOutBuffSz = 1024;
        unsigned char buf[1024];
        zs.avail_in = buflen;
        zs.avail_out = 1024;
        zs.next_in = p_sBuffer;
        zs.next_out = buf;
        if (inflateInit(&zs))
        {
            printf("inflateInit() failed!\n");
            return NULL;
        }
        while (zs.avail_in > 0)
        {
            int status = inflate(&zs, Z_NO_FLUSH);
            if (zs.avail_in != 0)
            {
                unsigned char* old = sUCBuffer;
                curOutBuffSz += 1024;
                sUCBuffer = new unsigned char[curOutBuffSz];
                memcpy(sUCBuffer, old, zs.total_out - 1024);
                memcpy(sUCBuffer + zs.total_out - 1024, buf, 1024);
                pCurBuff = sUCBuffer + zs.total_out;
                delete[] old;
                zs.next_out = buf;
                zs.avail_out = 1024;
            }
            else {
                memcpy(pCurBuff, buf, 1024 - zs.avail_out);
                break;
            }

        }
        if (inflateEnd(&zs) != Z_OK)
        {
            printf("deflateEnd() failed!\n");
            return NULL;
        }
        realCount = zs.total_out;
        return sUCBuffer;
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------