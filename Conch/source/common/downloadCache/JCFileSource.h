/**
@file			JCFileSource.h
@brief			
@author			James
@version		1.0
@date			2016_5_11
*/

#ifndef __JCFileSource_H__
#define __JCFileSource_H__

namespace laya
{
	class JCFileSource
	{
	public:
        virtual ~JCFileSource() {};

        virtual bool isFileExistInZipAPKExpansion(const char* sFileName) = 0;

        /** @brief 直接判断包内是否存在这个文件
         *  @param[in] 文件名字
         *  @return 返回true或者false
        */
		virtual bool isFileExist( const char* sFileName )=0;


        /** @brief 获得fileSize
         *  @param[in] 文件名字
         *  @return 返回size
        */
        virtual unsigned int getFileSize(const char* sFileName)=0;

        /** @brief 分配一段空间，函数指针，为了分配内存使用的
         *  @param[in] 
         *  @param[in]
         *  @return 
        */
		typedef unsigned char* (*ALLOCMEM)(int nBufferSize,void* pBuffer );

        /** @brief 读取文件内容
         *  @param[in] 文件名字
         *  @param[in] alloc的函数指针，调用者自己决定分配策略
         *  @param[in] 获得的buffer
         *  @param[out] 返回的buffersize
         *  @return 是否成功
        */
		virtual bool loadFileContent( const char* sFileName, ALLOCMEM pAllocFunc, void* pUserData, int& nBuffSize ) = 0;


        /** @brief 读取文件内容
        *  @param[in] 文件名字
        *  @param[in] 获得的buffer
        *  @param[out] 返回的buffersize
        *  @return 是否成功
        */
        virtual bool loadFileContent(const char* sFileName, char*& pBuffer, int& nBuffSize) = 0;
	};
}
//------------------------------------------------------------------------------


#endif //__JCFileSource_H__

//-----------------------------END FILE--------------------------------
