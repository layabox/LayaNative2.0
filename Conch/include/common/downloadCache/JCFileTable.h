/**
@file			JCFileTable.h
@brief			
@author			James
@version		1.0
@date			2016_5_11
*/

#ifndef __JCFileTable_H__
#define __JCFileTable_H__

#include <vector>

namespace laya
{
	class JCFileTable
    {
	public:

		struct Info
        {
			unsigned int fileid;
			unsigned int chksum;
		};

	public:

        JCFileTable();

		~JCFileTable();

		void init();

        /** @brief 根据字符串创建dcc表。
         *  @param[in] p_pszStr dcc信息。每一行描述一个文件
         *  @return 返回文件个数。
        */
		int initByString( const char* p_pszStr );

		void save();

        /** @brief 检查是否是需要缓存的文件
         *  @param[in] 文件ID
         *  @param[in]
         *  @return 
        */
		bool find(unsigned int p_nFileID, unsigned int& p_nChkSum);

        /** @brief 下面是对表的操作。建议不要用。可能会由于崩溃等问题损坏表。
        *  @param[in] 文件ID
        *  @param[in]
        *  @return
        */
		void addFile( unsigned int p_nFileID, unsigned int p_nChkSum );

        /** @brief 从表中删除一个文件
        *  @param[in] 文件ID
        */
		void delFile( unsigned int p_nFileID );

	protected:

        /** @brief 只修改表。不保存
         *  @param[in] 文件ID
         *  @param[in]
        */
		void _addFile( unsigned int p_nFileID, unsigned int p_nChkSum );

		//
        /** @brief 这个会返回一个不可靠的指针。所以不允许外部访问
        *  @param[in] 文件ID
        *  @return 返回一个可靠的指针
        */
		inline Info* getInfo(unsigned int p_nFileID)
        {
			unsigned char idx = (p_nFileID>>24)&0xff;
			std::vector<Info>& b = m_AllItem[idx];
			for( int i=0,sz=(int)b.size(); i<sz; i++)
            {
				Info& curI = b[i];
				if(curI.fileid==p_nFileID )
                {
					return &curI;
				}
			}
			return 0;
		}

    protected:

        std::vector<Info>	m_AllItem[256];

	};
}
//------------------------------------------------------------------------------


#endif //__JCFileTable_H__

//-----------------------------END FILE--------------------------------