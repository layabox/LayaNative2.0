
/**
@file			JCCommonMethod.h
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#ifndef __JCCommonMethod_H__
#define __JCCommonMethod_H__

#include <stdio.h>
#include <vector>
#include <string>

#define MAX_CHARACTER_SIZE    8 

namespace laya
{
    enum THREAD_MODE
    {
        THREAD_MODE_SINGLE = 1,
        THREAD_MODE_DOUBLE,
    };

    #define PI          3.1415926535897932f
    #define ARC_TO_RAD  0.0174532925199433f

    /** @brief 转成2的N次方
     *  @param[in] 输入的值
     *  @return 返回2的n次方
    */
    int mathCeilPowerOfTwo(int x);

    /** @brief 把字符串转换成int
     *  @param[in] 字符串 "#ff00ff"
     *  @return 
    */
	unsigned int hexStringToInt( const char* str );

    /** @brief 分割字符串
     *  @param[out] 返回到verctor中 
     *  @param[in] 字符串
     *  @param[in] 哪个字符分割
    */
    void splitString(std::vector<char*>& p_vOutVector, char* p_sBuffer, char p_pSeperator);

    /** @brief string转换成int
     *  @param[in] 字符串
     *  @return 返回int值
    */
    int stringToInt(const char *p_pszStr);

    /** @brief string转换成float
    *  @param[in] 字符串
    *  @return 返回float值
    */
    float stringToFloat(const char *p_pszStr);

    /** @brief 返回文件的扩展名字
     *  @param[in] 文件名字
     *  @return 返回扩展名
    */
    const char* getExtName(const char* p_sFileName);

    /** @brief 替换字符串
    */
    std::string stringReplace(std::string & str, const std::string & strsrc, const std::string &strdst);

    /** @brief UTF8转unicode
     *  @param[in] 
     *  @param[in]
     *  @return 
    */
    int UTF8ToUnicode(unsigned char *ch, int *unicode);

    /** @brief 把utf8的字符串转成unicode的字符串。
     *  @param[in] 
     *  @param[in]
     *  @param[in]
     *  @return 转换后的字符的个数
    */
    int UTF8StrToUnicodeStr(unsigned char * utf8_str, unsigned short * unicode_str, int unicode_str_size);

    /** @brief 把一个unicode的字符拷贝到p中（不管16还是32），会在p中前进，返回当前位置（下一个位置）。
     *  @param[in] 
     *  @param[in]
     *  @return 返回当前位置（下一个位置）。
    */
    char* UnicodeToUTF8(unsigned int unicode, char *p);

    /** @brief utf8_str_size 是提供的buffer的大小。如果空间不足，则返回NULL。 transedNum 为实际转换了多少个unicode字符。
     *                       usedBuff是使用了多少空间，如果转换成功了，最后会有一个0
     *  @param[in] 
     *  @param[in]
     *  @param[in]
     *  @return  如果空间不足，则返回NULL
    */
    char* UnicodeStrToUTF8Str(short * unicode_str, char * utf8_str, int utf8_str_size, int& transedUni, int& usedBuff);

    /** @brief utf8 to unicode
     *  @param[in] 
     *  @param[in]
     *  @param[in]
     *  @param[out] 
     *  @return 
    */
    std::wstring utf8_unicode(const char* str);

    /** @brief 分割路径
     *  @param[in] 文件名字
     *  @param[in] 返回vertor
     *  @return 是否成功
    */
    bool splitPath(const char* p_pszPath, std::vector<std::string>& out);

    /** @brief nomailizePath
     *  @param[in] 
     *  @param[in]
     *  @param[out] 
     *  @return 
    */
    std::string normalizePath(const char* p_pszPath, bool toLowerCase, int& p_nProtocol);

    /** @brief paserUTF8
     *  @param[in] 
     *  @param[in]
     *  @return 
    */
    void paserUTF8(std::string p_sBuffer, long p_nSize, std::vector<std::string>& p_vOut);

    /** @brief 字符串转小写
     *  @param[in] 字符串
     *  @return 转换后的字符串
    */
    char* LayaStrlwr(char* p_str);

    /** @brief 字符串转大写
    *  @param[in] 字符串
    *  @return 转换后的字符串
    */
    char* LayaStrupr(char* p_str);
    
    /** @brief 把一个字符串转为小写。直接修改字符串. 如果是utf8的则没问题。否则不要调用这个。
     *  @param[in] 字符串
     *  @return 转换后的字符串
    */
    char* UTF8ToLowercase(char* p_pszStr);

    /** @brief 得到一个url的转为小写的扩展名
     *  @param[in] 输入的url
     *  @return 返回扩展名
    */
    std::string getLowercaseExtOfUrl(const char* p_pszUrl);
    
    /** @brief UrlEncode
     *  @param[in] 输入的字符串
     *  @return 返回的字符串
    */
    std::string UrlEncode(const char* str);

    /** @brief UrlDecode
    *  @param[in] 输入的字符串
    *  @return 返回的字符串
    */
    std::string UrlDecode(const char* str);


    /** @brief 获得当前时间
     *  @return  返回当前时间
    */
    double tmGetCurms();


    /** @brief 获得app使用的内存
     *  @return 返回内存值
    */
    int getAppUsedMem();

    int hashstr(const char *s);

    void mulMatrixArray(float* leftMatrixE, float* rightMatrix, float* outArray, int nOffset);

    void flipPixelsY(uint8_t* pixels, int bytesPerRow, int rows);

    bool IsTextUTF8(char* str, unsigned long length);
};
//------------------------------------------------------------------------------


#endif //__JCCommonMethod_H__

//-----------------------------END FILE--------------------------------

