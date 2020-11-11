/**
@file			JCCrypto.h
@brief
@author			hugao
@version		1.0
@date			2016_5_12
*/

#ifndef __JCCrypto_H__
#define __JCCrypto_H__

#pragma warning(disable:4786)
#define NORMAL_SIZE BUFSIZ
#include <string>
using namespace std;
namespace laya
{
    char *base64_decode(const unsigned char *str, int length, int *ret_length);
    char* base64_encode(const unsigned char *str, int length, int *ret_length);

    unsigned int base64Decode(char *pDest, const char *pSrc, unsigned int srclen);
    unsigned int base64Encode(char *pDest, const char *pSrc, unsigned int srclen);
    class JCBKDRHash {
    public:
        static unsigned int hashStr(const unsigned char* str);

        static unsigned int hashMem(const unsigned char* mem, int len);
    };
#ifndef WEBASM
    struct MD5_DATA
    {
        unsigned long data[4];
        bool operator < (const MD5_DATA& p) const
        {
            return memcmp(data, p.data, 4 * sizeof(long))>0;
        }
    };

    class JCMD5
    {
    private:
        
        struct md5_context
        {
            unsigned int total[2];
            unsigned int state[4];
            unsigned char buffer[64];
        };

        void md5_starts(struct md5_context *ctx);
        void md5_process(struct md5_context *ctx, unsigned char data[64]);
        void md5_update(struct md5_context *ctx, unsigned char *input, size_t length);
        void md5_finish(struct md5_context *ctx, unsigned char digest[16]);

    public:
        //! construct a CMD5 from any buffer
        void GenerateMD5(unsigned char* buffer, size_t bufferlen);

        //! construct a CMD5
        JCMD5();

        //! construct a md5src from char *
        JCMD5(const char * md5src);

        //! construct a CMD5 from a 16 bytes md5
        JCMD5(unsigned long* md5src);

        //! add a other md5
        JCMD5 operator +(JCMD5 adder);

        //! just if equal
        bool operator ==(JCMD5 cmper);

        //! give the value from equer
        // void operator =(CMD5 equer);

        //! to a string
        string ToString();

        unsigned long m_data[4];
    };
#ifndef THIN_COMMON_WITHOUT_DOWNLOAD
    class JCEncrypt
    {
    public:

        /** @brief构造函数
        */
        JCEncrypt();

        /** @brief析构函数
        */
        ~JCEncrypt();

        static bool decrypt(char *buf, int len);

        static std::string getpassCode(std::string url);

    private:
        static std::string s_sPassCode;//生成秘钥的辅助字符串
        static std::string s_sPreCode; //加密文件的前面的标志
        static std::string s_sPassMd5; //秘钥
    public:
        static int         s_nPreLen; //加密前缀的字数
    };
#endif
    uint32_t crc32(const unsigned char *buf, uint32_t size);

#endif
}
//------------------------------------------------------------------------------


#endif //__JCCrypto_H__

//-----------------------------END FILE--------------------------------