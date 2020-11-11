/**
@file			JCImageRW.h
@brief			
@author			hugao
@version		1.0
@date			2016_5_11
*/

#ifndef __JCImageRW_H__
#define __JCImageRW_H__

#include <memory>
#include <functional>
#define PNG_UINT32 unsigned int
#if __APPLE__
#include <OpenGLES/ES3/gl.h>
#else
#include <GLES3/gl3.h>
#endif

namespace laya
{
	enum ImageType
    {
        ImgType_unknow,
		ImgType_jpeg,
		ImgType_png,
		ImgType_gif,
        ImgType_ETC1,
        ImgType_ETC2,
        ImgType_PVR,
	};

	struct ImageBaseInfo
    {
		int m_nWidth, m_nHeight, m_nBpp;
	};

	class BitmapData:public ImageBaseInfo
    {
	public:
		BitmapData();
		BitmapData(char* p_pData, int p_nWidth, int p_nHeight );
		BitmapData(int p_nWidth, int p_nHeight, bool p_bTranslate, int p_nFillColor );
		void releaseData()
        { 
			if(m_pImageData)
            {
				delete [] m_pImageData;
				m_pImageData=0;
			}
		}
        int         m_nImageID;
		char*	    m_pImageData;
        ImageType   m_nImageType;

		void reconfigure(int width, int height, int bpp, ImageType imageType);
		void copyData(char* dataPtr);
	};

#ifndef WEBASM

	//参数是对象实例而不是引用。因为实际对象可能是临时对象
	typedef std::function<void(BitmapData bmp)> imgDecodeCB;

    /** @brief 在函数内部会给 p_pData 分配空间。注意是一维数组，不是二维的 这里只负责解码图片，没有缓存管理等功能。
     *         返回的是函数内部分配的指针。需要外部释放
     *  @param[in] 内存数据
     *  @param[in] 长度
     *  @param[in] 获得到的bitmap
     *  @return 是否解码成功
    */
	bool loadImageMemSync( const char* p_pMem, int p_nLenth, BitmapData& p_bmp );

    /** @brief
         直接加载一个图片文件。
     *  @param[in] p_pszFile 本地文件的路径，绝对路径。
     *  @return 
            返回一个Bitmap对象，注意需要自己手动删除其中的指针。
    */
    BitmapData loadLocalImageSync( const char* p_pszFile );


	void loadImageMemASync(std::shared_ptr<char> p_pBuff, int p_nLenth, imgDecodeCB p_CB);

	ImageType getImgType( const char* p_pMem, int p_nLength );

    /** @brief 获得图片的基本信息。要求快速，如果不必解码就不要解码 同步函数
     *  @param[in] 内存数据
     *  @param[in] 内存长度
     *  @param[in] 图片基本信息
     *  @return 是否解码成功
    */
	bool getImageBaseInfo( const char* p_pMem, int p_nLength, ImageBaseInfo& p_Info );

	/** @brief 保存png
	 *  @param[in] 图片数据
	 *  @param[in] w
	 *  @param[in] h
	 *  @param[in] 文件名字
	 *  @return 是否保存成功
	*/
	bool saveAsPng(const char* p_pData, int w, int h, const char* p_pszFile );

    /** @brief 保存jpg
    *  @param[in] 图片数据
    *  @param[in] 图片信息
    *  @param[in] 文件名字
    *  @return 是否保存成功
    */
	bool saveAsJpeg(const char* p_pData, ImageBaseInfo& p_Info, const char* p_pszFile );

    /** @brief 保存bmp
    *  @param[in] 图片数据
    *  @param[in] 图片信息
    *  @return 是否保存成功
    */
	bool saveAsBmp(const char* p_pData, ImageBaseInfo& p_Info);

    /** @brief windw是每隔几个采样一个。windh类推。
     *  @param[in] 
     *  @param[in]
     *  @param[in]
     *  @param[out] 
     *  @return  返回的dst是在函数内部分配的
    */
	bool downsampleBmp( BitmapData& src, BitmapData& dst, int windw, int windh, bool rbBorder );
	
    /** @brief pFrcation 是4个int表示的宽高缩放。[宽分子，宽分母，高分子，高分母]
     *  @param[in] 
     *  @param[in]
     *  @param[in]
     *  @param[out] 
     *  @return 
    */
	bool downsampleBmp( BitmapData& src, BitmapData& dst, int* pFrcation, bool rbBorder );

	void createGridBmp( BitmapData& out, int w, int h, int gridw);

    /** @brief
     *      把一个灰度图转换成某种颜色，拷贝到dst中。
     *  @param[in] dst 目标BitmapData。
     *  @param[in] nSx 目标bmp的起点x
     *  @param[in] nSy 目标bmp的起点y
     *  @param[in] pSrc  源图片。是一个灰度图。灰度值表示颜色的深浅。
     *  @param[in] nSrcW  源图片的宽
     *  @param[in] nSrcH  源图片的高
     *  @param[in] nSrcColor  颜色。
     *  @return void
    */
    void copy8BitBmp( BitmapData& dst, int nSx, int nSy, unsigned char* pSrc, int nSrcW, int nSrcH, int nSrcColor );

    void copy32BitBmp(BitmapData& dst, int nSx, int nSy,int nDstW, unsigned char* pSrc, int nSrcW, int nSrcH, int srcPitch);

    std::pair<unsigned char*, unsigned long> convertBitmapToJpeg(const char* p_pData, int nWidth, int nHeight, int nBpp);

    std::pair<unsigned char*, unsigned long> convertBitmapToPng(const char* p_pData, int nWidth, int nHeight, int nBitDepth);

    void convertRGBA8888ToFormat(const unsigned char* data, size_t dataLen, GLenum format, unsigned char** outData, size_t* outDataLen);
#endif
}
//------------------------------------------------------------------------------


#endif //__JCImageRW_H__

//-----------------------------END FILE--------------------------------