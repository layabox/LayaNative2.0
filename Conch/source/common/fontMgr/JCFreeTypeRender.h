/**
@file			JCFreeTypeRender.h
@brief			
@author			James
@version		1.0
@date			2016_5_11
*/

#ifndef __JCFreeTypeRender_H__
#define __JCFreeTypeRender_H__

#include <vector>
#include<unordered_map>
#include <string>
#include "../resource/JCResource.h"
#include "../buffer/JCBuffer.h"
#include <thread>
#include <mutex>
#include "JCFontInfo.h"
#include "../imageLib/JCImageRW.h"
#include "../JCIOSFTInterface.h"
#include <math.h>
#ifndef WEBASM
    #include <ft2build.h>
    #include FT_FREETYPE_H
#endif
namespace laya
{
    const uint32_t TEXT_SIZE_ALLOWANCE = 8;
    const uint32_t MAX_CHAR_WIDTH = 512;
    const uint32_t MAX_CHAR_HEIGHT = 512;
    const uint32_t MAX_LINE_WIDTH = 2048;
	class JCFreeTypeFontRender
    {
    public:
#ifndef WEBASM
        struct FTFaceRecord
        {
            FT_Face face;
            char* buffer;
            FTFaceRecord(FT_Face face = nullptr, char* buffer = nullptr)
                :face(face), buffer(buffer)
            {}
            ~FTFaceRecord()
            {
                if (face)
                {
                    FT_Done_Face(face);
                    face = NULL;
                }
                if (buffer)
                {
                    delete[] buffer;
                    buffer = nullptr;
                }
            }
        };
        typedef std::unordered_map<std::string, FTFaceRecord*> MapFTFace;
#endif
	public:
        
        JCFreeTypeFontRender();

		~JCFreeTypeFontRender();

        /** @brief 初始化默认字体，传入的参数中间用 \4 分割
         *  @return 返回成功或者失败
        */
		bool initDefaultFont( const char* sDefaultTTFs );


        /** @brief 初始化默认字体buffer 和 size
        *  @return 返回成功或者失败
        */
        bool initDefaultFont(char* pBuffer, int nBuferLen);

        /** @brief 设置IOSFT的Interface 因为是从IOS层传过来的
         *  @param[in] JCIOSFTInterface实例
         *  @return 
        */
        void setIOSFTInterface(JCIOSFTInterface* pIOSFTInterface);

        /** @brief 设置字体从FileName中读取
         *  @param[in] 字体名字
         *  @param[in] ttf的路径
         *  @return 是否读取成功
        */
        bool setFontFaceFromUrl(const char* sFontFamily, const char* sTTFFileName);

        /** @brief 设置字体从buffer中读取
         *  @param[in] 字体的名字
         *  @param[in] buffer
         *  @param[in] bufferLength
         *  @return 是否读取成功
        */
        bool setFontFaceFromBuffer(const char* sFontFamily, char* pBuffer, int nBuferLen );

        /** @brief 移除字体
         *  @param[in] 字体的名字
         *  @return 
        */
        bool removeFont( const char* sFontFamily );


        /** @brief 测量文字
        *  @param[in] FreeTypeFontRender
        *  @param[in] 文字
        *  @param[in] 文字的info
        *  @param[out] 返回宽度
        *  @param[out] 返回高度
        *  @return
        */
        void measureText( char* p_sWord, JCFontInfo* p_pFontInfo, int& p_nW, int& p_nH);

        /** @brief 测量一排文字
        *  @param[in] FreeTypeFontRender
        *  @param[in] 文字
        *  @param[in] 文字的info
        *  @param[out] 返回宽度
        *  @param[out] 返回高度
        *  @return
        */
        void measureTexts(char* p_sWord, JCFontInfo* p_pFontInfo, int& p_nW, int& p_nH);

        /** @brief bitmapData 提供buffer和buffer的大小。 返回的时候，bitmapData中的大小已经被改成了实际的大小。
        *  @param[in] 字符
        *  @param[in] bitmapdata
        *  @param[in] colkor
        *  @param[in] 文字信息
        *  @param[out] 如果buffer大小不足，则返回false
        *  @return
        */
        //bool getTextBitmapData(char* str, BitmapData* bitmapData, int nColor, JCFontInfo* p_pFontInfo, int nBorderSize, int nBorderColor);

        void getTextLineBitmapData(char* str, BitmapData* bitmapData, int nColor, JCFontInfo* fontInfo, int borderSize, int nBorderColor, double scaleX, double scaleY);

        /** @brief 清空用户自己传入的文字
        *  @return
        */
        void clearCustomFont();

        void measureChar(int unicode, JCFontInfo* p_pFontInfo, int& p_nW, int& p_nH);

    private:

        /** @brief 设置字体
        *  @param[in] 字体的名字
        *  @return 是否设置成功
        */
        bool setFont(const char* sFontName);

        /** @brief 设置斜体的值
        *  @param[in] 值越大越斜
        *  @return
        */
        void setItalics(float nValue);

        /** @brief 设置字体大小
        *  @param[in] 宽度 0 表示等高
        *  @param[in] 高度 0 表示等宽
        */
        void setFontSize(int nWidth, int nHeight);

        /** @brief 获得bitmapData从freetype中
        *  @param[in] unicode文字
        *  @param[out] 获得的bitmap,这个bitmap需要外面删除的
        *  @param[out] 宽度
        *  @param[out] 高度
        *  @param[out] 数据的宽度度
        *  @param[out] 数据的高度
        *  @param[out] 左上角的距离
        *  @param[out] 左上角的距离
        *  @return
        */
        void getBitmapData(int nUnicode, unsigned char*& pBitmap, int& nWidth, int& nHeight, int& nDataW, int& nDataH, int& nDataLeft, int& nDataTop, int& nUnderlineTop, int& nUnderlineHeight);

        /** @brief 测量文字
        *  @param[in] unicode
        *  @param[out] 获得宽度
        *  @param[out] 获得的高度
        *  @return
        */
        void getMetric(int nUnicode, int& nWidth, int& nHeight);

        /** @brief 清空默认的font
        */
        void clearDefaultFont();


        /** @brief 获得IOS写入的font路径
         *  @return 返回路径
        */
        std::string getIOSFontTTFPath();

        /** @brief 把ios的font写入文件
         *  @return 返回路径
        */
        std::string writeIOSFontTTF();

#ifndef WEBASM

        /** @brief 通过buffer获得FT_face
        *  @param[in] buffer
        *  @param[in] bufferSize
        *  @return FT_Face
        */
        FT_Face getFTFaceFromBuffer(char* pBuffer, int nBuferLen);


        /** @brief 通过文件名字获得FT
        *  @param[in] 文件名字
        *  @return FT_Face
        */
        FT_Face getFTFaceFromFile(const char* sFileName);

#endif

        static  int32_t RoundToInt(float F)
        {
            #ifdef WIN32
                // Note: the x2 is to workaround the rounding-to-nearest-even-number issue when the fraction is .5
                return _mm_cvt_ss2si(_mm_set_ss(F + F + 0.5f)) >> 1;
            #else
                return (int32_t)(floorf(F + 0.5f));
            #endif
        }
        template <typename TRetType, typename TParamType>
        TRetType Convert26Dot6ToRoundedPixel(TParamType InValue)
        {
            return static_cast<TRetType>(RoundToInt(InValue / 64.0f));
        }
    public:

        static unsigned short                       m_pWordBmpBorder[MAX_CHAR_WIDTH * MAX_CHAR_HEIGHT * 4];
        static char*                                m_pWordBuff;
        std::recursive_mutex	                    m_kLoadGlyphLock;               ///<锁

    protected:
        
        int				                            m_nFontSizeW;                   ///<字体大小
        int                                         m_nFontSizeH;	                ///<字体大小
        int				                            m_nDevDPIX;                     ///<dpi
        int                                         m_nDevDPIY;                     ///<dpi
        float			                            m_fItalicsValue;                ///<斜体度
        JCIOSFTInterface*                           m_pIOSFTInterface;              ///<为了IOS使用的

#ifndef WEBASM
        FT_Library		                            m_pFTlibrary;                   ///<FreeType需要的
        std::vector<FTFaceRecord*>                  m_vDefaultFT;	                ///<默认的字体库
        FT_Face                                     m_pCurrentFT;                   ///<当前的字体库
        MapFTFace                                   m_vCustomMadeFT;                ///<用户定制的字体
#endif
	};
}
//------------------------------------------------------------------------------


#endif //__JCFreeTypeRender_H__

//-----------------------------END FILE--------------------------------