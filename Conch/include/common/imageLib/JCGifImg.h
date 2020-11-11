/**
@file			JCGifImg.h
@brief			
@author			James
@version		1.0
@date			2016_7_13
*/

#ifndef __JCGifImg_H__
#define __JCGifImg_H__

#include <fstream>
#include <iostream>
using namespace std;

namespace laya
{
    /*
     * 图像扩展参数
    */
    typedef struct
    {
        bool active;			//本结构中的其它参数是否可用
        unsigned int disposalMethod;	//处理方法（见gif89a.doc，可忽略）
        bool userInputFlag;		//是否期待用户输入
        bool trsFlag;			//是否有透明色
        unsigned short delayTime;			//延时时间（单位1/100秒）
        unsigned int trsColorIndex;		//透明色调色板索引
    }GCTRLEXT;

    /*
     * 一帧图象的参数
    */
    typedef struct
    {
        unsigned short imageLPos;			//图象左边沿到逻辑屏幕的距离（单位像素）
        unsigned short imageTPos;			//图象上边沿到逻辑屏幕的距离（单位像素）
        unsigned short imageWidth;		//图象的宽度（单位像素）
        unsigned short imageHeight;		//图象的高度（单位像素）
        bool lFlag;				//是否有局部调色板（决定其他调色板参数是否有效）
        bool interlaceFlag;		//图象数据是否交错
        bool sortFlag;			//局部调色板数据是否按优先排序
        unsigned int lSize;				//局部调色板大小（有多少个实际入口）
        unsigned char* pColorTable;		//指向局部调色板的指针（256个入口，每个入口三字节）
        unsigned char* dataBuf;			//调色板格式请参看gif89a.doc
        GCTRLEXT ctrlExt;		//图象数据指针
    }FRAME;						//图象扩展参数（与透明背景和动画有关）
    typedef FRAME *LPFRAME;
    typedef const FRAME *LPCFRAME;

    /*
     * GIF文件的全局参数
    */
    typedef struct
    {									//GIF文件的全局参数
        unsigned int frames;					//文件中图象帧数
        unsigned short scrWidth, scrHeight;		//逻辑屏幕的宽度和高度（单位像素）
        bool gFlag;						//是否有全局调色板（决定其他调色板参数是否有效）
        unsigned int colorRes;					//色彩分辨率（不使用）
        bool gSort;						//全局调色板是否按优先排序
        unsigned int gSize;						//全局调色板大小（有多少个实际入口）
        unsigned int BKColorIdx;				//背景色的调色板索引
        unsigned int pixelAspectRatio;			//像素长宽比例
        unsigned char *gColorTable;				//指向全局调色板的指针（256个入口，每个入口三字节）
    }GLOBAL_INFO;						//调色板格式请参看gif89a.doc
    typedef GLOBAL_INFO *LPGLOBAL_INFO;
    typedef const GLOBAL_INFO *LPCGLOBAL_INFO;

    /*
     *
    */
    typedef struct
    {
        unsigned int		len;
        unsigned	char* p;
    }STRING_TABLE_ENTRY;


    class BufferIOStream
    {
    public:
        BufferIOStream(char* p_sBuffer, int p_nBufferSize)
        {
            m_pBuffer = p_sBuffer;
            m_nSize = p_nBufferSize;
            m_nCurPos = 0;
        }
        ~BufferIOStream()
        {
            close();
        }
        bool read(char* p_pRet, int p_nSize)
        {
            if ((m_nCurPos + p_nSize) > m_nSize)
            {
                return false;
            }
            char* pCur = m_pBuffer + m_nCurPos;
            memcpy(p_pRet, pCur, p_nSize);
            m_nCurPos += p_nSize;
            return true;
        }
        bool good()
        {
            return true;
        }
        int tellg()
        {
            return m_nCurPos;
        }
        void close()
        {
            /*直接拿的指针应该在外面释放
            if( m_pBuffer != NULL )
            {
                delete[] m_pBuffer;
                m_pBuffer = NULL;
            }
            */
            m_nSize = 0;
            m_nCurPos = 0;
        }
        bool eof()
        {
            return (m_nCurPos >= m_nSize);
        }
        void seekg(int p_nPos)
        {
            m_nCurPos = p_nPos;
        }
        void seekg(int p_nPos, int p_nMark)
        {
            if (p_nMark == 0)
            {
                m_nCurPos = 0 + p_nPos;
            }
            else if (p_nMark == 1)
            {
                m_nCurPos += p_nPos;
            }
            else if (p_nMark == 2)
            {
                m_nCurPos = m_nSize + p_nPos;
            }
        }
    protected:
        char*				m_pBuffer;
        int					m_nSize;
        int					m_nCurPos;
    };


    /**
     * Gif类
    */
    class GifLoader
    {
    public:

        /** @brief
         *  构造函数
        */
        GifLoader(void);

        /** @brief 构造函数
         *  @param[in] 文件名字
         *  @param[in] 是否一次性读取完成
         *  @return
        */
        GifLoader(const char* p_sFileName, bool p_bInMem);


        GifLoader(unsigned char* p_pBuffer, int p_nBufferSize);

        /** @brief
        *  析构函数
        */
        ~GifLoader(void);

        /** @brief
        *  重载错误符号
        */
        bool operator!(void);

        /** @brief 打开图片
        *  @param[in] 图片路径
        *  @param[in] 是否一次性读取完成
        *  @return
        */
        bool open(const char* p_sFileName, bool p_bInMem);

        /** @brief 关闭
         *
        */
        void close(void);

        /** @brief 获得版本
         *
        */
        char* getVersion(void);

        /** @brief 获得下一帧
        *
        */
        LPCFRAME getNextFrame(void);

        /** @brief 获得图片的全局信息
        *
        */
        LPCGLOBAL_INFO getGlobalInfo();

        /*
         *
        */
    public:



        //从内存读取的方法      现在代码比较恶心，回头把ifstream  和 bufferIOStream 封装成一个类
        bool open(void);

        unsigned int checkFrames(BufferIOStream& p_kStrteam);

        bool getAllFrames(BufferIOStream& p_kIOStream);

        bool extractData(FRAME* p_pFrame, BufferIOStream& p_kStrteam);


    private:

        /*
        功能  ：检查文件中图象帧数。
        参数  ：ifs:对文件流的引用。
        返回值：文件中图象帧数。
        */
        unsigned int checkFrames(ifstream& p_kStrteam);

        /*
        功能  ：将所有图象帧数据读入内存。
        参数  ：ifs:对文件流的引用。
        返回值：操作是否成功，为真成功，为假失败。
        */
        bool getAllFrames(ifstream& p_kStrteam);

        /*
        功能  ：解压缩一帧图象数据。
        参数  ：f:指向用于保存图象数据的结构。
        ifs:对文件流的引用。
        返回值：操作是否成功，为真成功，为假失败。
        */
        bool extractData(FRAME* p_pFrame, ifstream& p_kStrteam);

        /*
        功能  ：初始化字符串表。
        参数  ：strTable:指向字符串表的指针。
        rootSize:初始化的入口数。
        返回值：操作是否成功，为真成功，为假失败。
        */
        bool initStrTable(STRING_TABLE_ENTRY* p_pSTable, unsigned int p_nRootSize);

        /*
        功能  ：在字符串表中增加一项。
        参数  ：strTable:指向字符串表的指针。
        addIdx:增加的入口索引。
        idx:用于构造要增加的字符串的入口索引。
        c:用于构造要增加的字符串的字符。
        返回值：操作是否成功，为真成功，为假失败。
        */
        bool addStrTable(STRING_TABLE_ENTRY* p_pSTable, unsigned int p_nAddInedx, unsigned int p_nIndex, unsigned char p_cBuf);

    public:

        FRAME*					m_vAllFrames;			//指向所有图象帧的指针（inMem为真时用）

    private:

        GLOBAL_INFO				m_kGInfo;				//GIF文件的全局参数

        FRAME					m_kCurFrame;			//当前帧的参数（inMem为假时用）

        GCTRLEXT				m_kCtrlExt;				//图象扩展参数（读入数据时临时使用）

    private:

        BufferIOStream*			m_pBufferIOStream;		//用于读取内存的

        ifstream				m_kIOStream;			//用于读文件的文件流

        char					m_sVersion[4];			//版本字符串

        bool					m_bError;				//类实例变量创建时是否出错的标志

        bool					m_bOpened;				//是否处于打开状态

        bool					m_bInMem;				//图象数据是否一次读入内存

        unsigned char					m_vGColorTable[256 * 3];	//全局调色板

        unsigned char					m_vLColorTable[256 * 3];	//局部调色板（inMem为假时用）

        streampos				m_kDataStart;			//保存文件流中图象数据开始的地方

        unsigned int					m_nCurIndex;			//当前帧的索引（inMem为真时用）

    };
}
//------------------------------------------------------------------------------


#endif //__JCGifImg_H__

//-----------------------------END FILE--------------------------------