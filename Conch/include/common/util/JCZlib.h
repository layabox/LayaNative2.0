/**
@file			JCZlib.h
@brief			
@author			hugao
@version		1.0
@date			2016_5_13
*/

#ifndef __JCZlib_H__
#define __JCZlib_H__

#include <zlib.h>

#define MAX_CHARACTER_SIZE    8 

namespace laya
{
    /** @brief 获得文件解压开后的大小
     *  @param[in] 
     *  @param[in]
     *  @param[in] 
     *  @return 
    */
    uLong getLayaBoxUncompressSize(unsigned char* p_sBuffer, int buflen, int& p_nOffset);
    

    /** @brief 压缩文件
     *  @param[in] 输入的数据流 
     *  @param[in] 数据流大小
     *  @param[out] 返回的大小
     *  @return 返回的数据流
    */
    unsigned char* zlibByteArrayCompress(unsigned char* p_sBuffer, int buflen, int &realCount);

    /** @brief 解压文件
     *  @param[in] 输入的数据流
     *  @param[in] 数据流大小
     *  @param[out] 返回的大小
     *  @return 返回的数据流
    */
    unsigned char* zlibByteArrayUnCompress(unsigned char* p_sBuffer, int buflen, int &realCount);
};
//------------------------------------------------------------------------------


#endif //__JCZlib_H__

//-----------------------------END FILE--------------------------------