/**
@file			JCFreeTypeRender.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_11
*/

#include "JCFreeTypeRender.h"
#include "../util/Log.h"
#include "../fileSystem/JCFileSystem.h"
#include "../util/JCLayaUrl.h"
#include <thread>
#include "../util/JCCommonMethod.h"

namespace laya
{
    unsigned short JCFreeTypeFontRender::m_pWordBmpBorder[MAX_CHAR_WIDTH * MAX_CHAR_HEIGHT * 4];
    char* JCFreeTypeFontRender::m_pWordBuff = nullptr;
    JCFreeTypeFontRender::JCFreeTypeFontRender()
    {
        m_nFontSizeW = 0;
        m_nFontSizeH = 0;
        m_nDevDPIX = 72;
        m_nDevDPIY = 72;
        m_fItalicsValue = 0;
        m_pIOSFTInterface = NULL;
#ifndef WEBASM
        m_pFTlibrary = NULL;
        m_pCurrentFT = NULL;   
        FT_Init_FreeType(&m_pFTlibrary);
#endif
        m_pWordBuff = new char[MAX_LINE_WIDTH * MAX_CHAR_HEIGHT * 4];
    }
    JCFreeTypeFontRender::~JCFreeTypeFontRender()
    {
        clearCustomFont();
        clearDefaultFont();
#ifndef WEBASM
        FT_Done_FreeType(m_pFTlibrary);
        m_pFTlibrary = NULL;
#endif
        if (m_pWordBuff)
        {
            delete[] m_pWordBuff;
            m_pWordBuff = nullptr;
        }
    }
    void JCFreeTypeFontRender::clearCustomFont()
    {
#ifndef WEBASM
        for (MapFTFace::iterator iter = m_vCustomMadeFT.begin(); iter != m_vCustomMadeFT.end(); iter++)
        {
            delete iter->second;
        }
        m_vCustomMadeFT.clear();
#endif
    }
    void JCFreeTypeFontRender::clearDefaultFont()
    {
#ifndef WEBASM
        for (int i = 0, n = m_vDefaultFT.size(); i < n; i++)
        {
            delete m_vDefaultFT[i];
        }
        m_vDefaultFT.clear();
#endif
    }
    bool JCFreeTypeFontRender::initDefaultFont(const char* sDefaultTTFs)
    {
#ifndef WEBASM
        if (m_vDefaultFT.size() > 0)return true;
        
        if( strlen(sDefaultTTFs) > 0 )
        {
            std::vector<char*> vVector;
            splitString(vVector, (char*)sDefaultTTFs,'|');
            for (int i = 0, n = vVector.size(); i < n; i++)
            {
                FT_Face pFTFace = getFTFaceFromFile(vVector[i]);
                if ( pFTFace == NULL)
                {
                    return false;
                }
                else
                {
                    m_vDefaultFT.push_back(new FTFaceRecord(pFTFace));
                }
            }
        }
        else
        {
        #ifdef __APPLE__
            std::string sPath = getIOSFontTTFPath();
            FT_Face pFTFace = getFTFaceFromFile(sPath.c_str());
            if ( pFTFace == NULL )
            {
                sPath = writeIOSFontTTF();
                pFTFace = getFTFaceFromFile(sPath.c_str());
                if ( pFTFace == NULL )
                {
                    return false;
                }
                else
                {
                    m_vDefaultFT.push_back(new FTFaceRecord(pFTFace));
                }
            }
            else
            {
                m_vDefaultFT.push_back(new FTFaceRecord(pFTFace));
            }
        #endif
        }
#endif
        return true;
    }
    bool JCFreeTypeFontRender::initDefaultFont(char* pBuffer, int nBuferLen)
    {
#ifndef WEBASM
        clearDefaultFont();
        //arrayBuffer will be GC , so copy
        char* copyBuffer = new char[nBuferLen];
        memcpy(copyBuffer, pBuffer, nBuferLen);
        FT_Face pFTFace = getFTFaceFromBuffer(copyBuffer,nBuferLen);
        if (pFTFace == NULL)
        {
            delete[] copyBuffer;
            return false;
        }
        else
        {
            m_vDefaultFT.push_back(new FTFaceRecord(pFTFace, copyBuffer));
        }
#endif
        return true;
    }
#ifndef WEBASM
    FT_Face JCFreeTypeFontRender::getFTFaceFromBuffer(char* pBuffer, int nBuferLen)
    {
        FT_Error nError = 0;
        FT_Face pFTFace = NULL;
        nError = FT_New_Memory_Face(m_pFTlibrary, (const FT_Byte*)pBuffer, nBuferLen, 0, &pFTFace);
        if (nError == FT_Err_Unknown_File_Format) 
        {
            return NULL;
        }
        else if (nError)
        {
            return NULL;
        }
        FT_Select_Charmap(pFTFace, FT_ENCODING_UNICODE);
        return pFTFace;
    }
    FT_Face JCFreeTypeFontRender::getFTFaceFromFile(const char* sFileName)
    {
        FT_Face pFTFace = NULL;
        int nError = FT_New_Face(m_pFTlibrary, sFileName, 0, &pFTFace);
        if (nError)
        {
            pFTFace = NULL;
        }
        FT_Select_Charmap(pFTFace, FT_ENCODING_UNICODE);
        return pFTFace;
    }
#endif
    bool JCFreeTypeFontRender::setFont(const char* sFontName) 
    {
#ifndef WEBASM
        m_pCurrentFT = NULL;
        MapFTFace::iterator iter = m_vCustomMadeFT.find(sFontName);
        if (iter != m_vCustomMadeFT.end())
        {
            m_pCurrentFT = iter->second->face;
            return true;
        }
#endif
        return false;
    }
	void JCFreeTypeFontRender::setItalics(float fValue)
    {
#ifndef WEBASM
        if (m_fItalicsValue == fValue)return;
		m_fItalicsValue = fValue;
        FT_Matrix kMatrix;
        kMatrix.xx = 0x10000L;
        kMatrix.xy = (FT_Fixed)(m_fItalicsValue * 0x10000L);
        kMatrix.yx = 0;
        kMatrix.yy = 0x10000L;
        if (m_pCurrentFT)
        {
            FT_Set_Transform(m_pCurrentFT, &kMatrix, 0);
        }
        for (int i = 0, n = m_vDefaultFT.size(); i < n; i++)
        {
            FT_Set_Transform(m_vDefaultFT[i]->face, &kMatrix, 0);
        }
#endif
	}
    void JCFreeTypeFontRender::setFontSize(int nWidth, int nHeight)
    {
#ifndef WEBASM
        int nW = nWidth << 6;
        int nH = nHeight << 6;
        if (m_nFontSizeW == nW && m_nFontSizeH == nH)
        {
            //避免两种字体，第一种设置了size  切换到第二种字体的时候，没有设置就返回了。
            if (m_pCurrentFT)
            {
                FT_Set_Char_Size(m_pCurrentFT, m_nFontSizeW, m_nFontSizeH, m_nDevDPIX, m_nDevDPIY);
            }
            return;
        }
        m_nFontSizeW = nW;
        m_nFontSizeH = nH;
        if (m_pCurrentFT)
        {
            FT_Set_Char_Size(m_pCurrentFT, m_nFontSizeW, m_nFontSizeH, m_nDevDPIX, m_nDevDPIY);
        }
        for (int i = 0, n = m_vDefaultFT.size(); i < n; i++)
        {
            FT_Set_Char_Size(m_vDefaultFT[i]->face, m_nFontSizeW, m_nFontSizeH, m_nDevDPIX, m_nDevDPIY);
        }
#endif
    }
	void JCFreeTypeFontRender::getMetric( int nUnicode, int& nWidth, int& nHeight )
    {
#ifndef WEBASM
        if (nUnicode == 0x0009)//Tab
        {
            nUnicode = 0x0020;//半角空格
        }
        FT_UInt nFoundIndex = 0;
        FT_Face pFTFace = m_pCurrentFT;
        //先去看看用户设置的字体中是否存在
        if (pFTFace)
        {
            nFoundIndex = FT_Get_Char_Index(pFTFace, nUnicode);
        }
		if(nFoundIndex==0)
        {
			for( int i=0,n= m_vDefaultFT.size(); i<n; i++)
            {
                nFoundIndex = FT_Get_Char_Index(m_vDefaultFT[i]->face, nUnicode);
				if(nFoundIndex>0)
                {
                    pFTFace = m_vDefaultFT[i]->face;
					break;
				}
			}
		}
		if(nFoundIndex ==0)
        {
            LOGW("JCFreeTypeFontRender::getMetric Cannot find the specified character in all fonts:%x", nUnicode);
            if (pFTFace)
            {
                nFoundIndex = FT_Get_Char_Index(pFTFace, 0x003f);
            }
            if (nFoundIndex == 0)
            {
                for (int i = 0, n = m_vDefaultFT.size(); i<n; i++)
                {
                    nFoundIndex = FT_Get_Char_Index(m_vDefaultFT[i]->face, 0x003f);
                    if (nFoundIndex>0)
                    {
                        pFTFace = m_vDefaultFT[i]->face;
                        break;
                    }
                }
            }
            if (nFoundIndex == 0)
            {
                nWidth = nHeight = 0;
                return;
            }
		}
		FT_Error kFTError = FT_Load_Glyph(pFTFace, nFoundIndex, FT_LOAD_RENDER|FT_LOAD_DEFAULT|FT_LOAD_NO_BITMAP);
		if(kFTError ==36)
        {
			LOGE("JCFreeTypeFontRender::getMetric Not set font size yet");
            nWidth = nHeight = 0;
		}
		FT_GlyphSlot pGlyph = pFTFace->glyph;
		FT_Bitmap& kBitmap = pGlyph->bitmap;
        nWidth = pGlyph->advance.x >> 6;
        nHeight = (pFTFace->size->metrics.ascender- pFTFace->size->metrics.descender)>>6;
#endif
	}
	void JCFreeTypeFontRender::getBitmapData(int nUnicode, unsigned char*& pBitmap, int& nWidth, int& nHeight, int& nDataW, int& nDataH, int& nDataLeft, int& nDataTop, int& nUnderlineTop, int& nUnderlineHeight)
    {
#ifndef WEBASM
        if (nUnicode == 0x0009)//Tab
        {
            nUnicode = 0x0020;//半角空格
        }
        FT_UInt nFoundIndex = 0;
        FT_Face pFTFace = m_pCurrentFT;
        if (pFTFace)
        {
            nFoundIndex = FT_Get_Char_Index(pFTFace, nUnicode);
        }
        if (nFoundIndex == 0)
        {
            for (int i = 0, n = m_vDefaultFT.size(); i < n; i++)
            {
                nFoundIndex = FT_Get_Char_Index(m_vDefaultFT[i]->face, nUnicode);
                if (nFoundIndex > 0)
                {
                    pFTFace = m_vDefaultFT[i]->face;
                    break;
                }
            }
        }
		if(nFoundIndex ==0)
        {
			LOGW("JCFreeTypeFontRender::getBitmapData Cannot find the specified character in all fonts:%x",nUnicode);
            if (pFTFace)
            {
                nFoundIndex = FT_Get_Char_Index(pFTFace, 0x003f);
            }
            if (nFoundIndex == 0)
            {
                for (int i = 0, n = m_vDefaultFT.size(); i < n; i++)
                {
                    nFoundIndex = FT_Get_Char_Index(m_vDefaultFT[i]->face, 0x003f);
                    if (nFoundIndex > 0)
                    {
                        pFTFace = m_vDefaultFT[i]->face;
                        break;
                    }
                }
            }
            if (nFoundIndex == 0)
            {
                pBitmap = NULL;
                nWidth = nHeight = nDataW = nDataH = nDataLeft = nDataTop = 0;
                return;
            }
		}

        FT_Error kFTError = FT_Load_Glyph(pFTFace, nFoundIndex, FT_LOAD_RENDER | FT_LOAD_DEFAULT | FT_LOAD_NO_BITMAP);
        if (kFTError == 36)
        {
            LOGE("JCFreeTypeFontRender::getBitmapData Not set font size yet");
            pBitmap = NULL;
            nWidth = nHeight = nDataW = nDataH = nDataLeft = nDataTop = 0;
        }
        FT_GlyphSlot pGlyph = pFTFace->glyph;
		if( pGlyph->format != FT_GLYPH_FORMAT_BITMAP )
        {
            FT_Render_Glyph(pGlyph, FT_RENDER_MODE_NORMAL);	
		}
		FT_Bitmap& kBitmap = pGlyph->bitmap;
		nDataLeft= pGlyph->bitmap_left;
		nDataTop = (pFTFace->size->metrics.ascender>>6) - pGlyph->bitmap_top;
		nWidth = pGlyph->advance.x >> 6;
		nHeight = (pFTFace->size->metrics.ascender- pFTFace->size->metrics.descender)>>6;
		nDataW = kBitmap.width;
		nDataH = kBitmap.rows;
        //pFTFace->size->metrics.ascender;
        //pFTFace->size->metrics.descender;
  
        //下划线
        nUnderlineHeight = static_cast<int16_t>(Convert26Dot6ToRoundedPixel<int32_t>(FT_MulFix(pFTFace->underline_thickness, pFTFace->size->metrics.y_scale)));
        nUnderlineHeight = nUnderlineHeight > 0 ? nUnderlineHeight : 1;
        int16_t OutUnderlinePos = static_cast<int16_t>(Convert26Dot6ToRoundedPixel<int32_t>(FT_MulFix(pFTFace->underline_position, pFTFace->size->metrics.y_scale)));
        nUnderlineTop = static_cast<int16_t>(Convert26Dot6ToRoundedPixel<int32_t>(pFTFace->size->metrics.ascender)) - OutUnderlinePos - (int)(0.5f * nUnderlineTop);

        pBitmap = new unsigned char [kBitmap.rows*kBitmap.width];
		if(kBitmap.width== kBitmap.pitch)
        {
			memcpy(pBitmap, kBitmap.buffer, kBitmap.rows*kBitmap.width);
		}
        else
        {
			for( size_t y=0; y<kBitmap.rows; y++)
            {
				memcpy(&pBitmap[y*kBitmap.width], &kBitmap.buffer[y*kBitmap.pitch], kBitmap.width);
			}
		}
#endif
	}
    struct Info
    {
        int nTextWidth = 0;
        int nTextHeight = 0;
        int dataw = 0;
        int datah = 0;
        int datal = 0;
        int datat = 0;
        int nUnderlineTop = 0;
        int nUnderlineHeight = 0;
        unsigned char* pBmp = 0;
        ~Info()
        {
            if (pBmp)
            {
                delete[] pBmp;
                pBmp = NULL;
            }
        }
    };
    void JCFreeTypeFontRender::getTextLineBitmapData(char* str, BitmapData* bitmapData, int nColor, JCFontInfo* fontInfo, int borderSize, int nBorderColor, double scaleX, double scaleY)
    {
        if (str == nullptr)
        {
            bitmapData->m_nWidth = bitmapData->m_nHeight = 1;
            memset(bitmapData->m_pImageData, 0, bitmapData->m_nWidth * bitmapData->m_nHeight * 4);
            return;
        }

        int nLen =strlen(str);
        if (nLen <= 0)
        {
            bitmapData->m_nWidth = bitmapData->m_nHeight = 1;
            memset(bitmapData->m_pImageData, 0, bitmapData->m_nWidth * bitmapData->m_nHeight * 4);
            return;
        }
        
        static unsigned short ucStr[8];
        std::vector<std::string> vText;
        paserUTF8(str, nLen, vText);
        int width = 0;
        int height = 0;
        int nBorderSize = borderSize > TEXT_SIZE_ALLOWANCE / 2 ? TEXT_SIZE_ALLOWANCE / 2 : borderSize;
        int size = vText.size();
        std::vector<Info> infoVector;
        infoVector.resize(size);

        for (int i = 0; i < size; i++)
        {
            Info& currentInfo = infoVector[i];
            int nResultLen = UTF8StrToUnicodeStr((unsigned char *)vText[i].c_str(), ucStr, 8);
            if (nResultLen <= 0)
            {
                infoVector.clear();
                bitmapData->m_nWidth = bitmapData->m_nHeight = 1;
                memset(bitmapData->m_pImageData, 0, bitmapData->m_nWidth * bitmapData->m_nHeight * 4);
                return;
            }
            int nWord = ucStr[0];

            m_kLoadGlyphLock.lock();
            setFont(fontInfo->m_sFamily);
            setFontSize((scaleX * fontInfo->m_nFontSize), (scaleY * fontInfo->m_nFontSize));
            getBitmapData(nWord, currentInfo.pBmp, currentInfo.nTextWidth, currentInfo.nTextHeight, currentInfo.dataw, currentInfo.datah, currentInfo.datal, currentInfo.datat, currentInfo.nUnderlineTop, currentInfo.nUnderlineHeight);

            m_kLoadGlyphLock.unlock();

            /*currentInfo.datal = currentInfo.datal > 0 ? currentInfo.datal : 0;
            currentInfo.datat = currentInfo.datat > 0 ? currentInfo.datat : 0;
            if (currentInfo.nTextWidth < currentInfo.dataw + currentInfo.datal)
            {
                currentInfo.nTextWidth = currentInfo.dataw + currentInfo.datal;
            }
            if (currentInfo.nTextHeight < currentInfo.datah + currentInfo.datat)
            {
                currentInfo.nTextHeight = currentInfo.datah + currentInfo.datat;
            }*/
            
            //currentInfo.nTextWidth += nBorderSize * 2;
            //currentInfo.nTextHeight += nBorderSize * 2;

            width += currentInfo.nTextWidth;
            if (currentInfo.nTextHeight > height)
            {
                height = currentInfo.nTextHeight;
            }
        }

        bitmapData->m_nWidth = width + nBorderSize * 2;
        bitmapData->m_nHeight = height + nBorderSize * 2;
		
        if (bitmapData->m_nHeight > MAX_CHAR_HEIGHT || bitmapData->m_nWidth > MAX_LINE_WIDTH)
        {
            infoVector.clear();
            LOGW("Font is too big");
            bitmapData->m_nWidth = bitmapData->m_nHeight = 1;
            memset(bitmapData->m_pImageData, 0, bitmapData->m_nWidth * bitmapData->m_nHeight * 4);
            return;
        }

        unsigned int* pBuff = (unsigned int*)bitmapData->m_pImageData;//  new unsigned int [nTextWidth*nTextHeight];
        memset(pBuff, 0, bitmapData->m_nWidth * bitmapData->m_nHeight * 4);//TODO 为了效率这个能去掉么
        int offset = 0;           
        
        for (int i = 0; i < size; i++)
        {
            Info& currentInfo = infoVector[i];
            if (nBorderSize > 0)
            {
                //这里先把描边绘制好
                int nW = currentInfo.dataw + nBorderSize * 2;
                int nH = currentInfo.datah + nBorderSize * 2;
                if (nW > MAX_CHAR_WIDTH || nH > MAX_CHAR_HEIGHT)
                {
                    infoVector.clear();
                    LOGW("Font is too big");
                    bitmapData->m_nWidth = bitmapData->m_nHeight = 1;
                    memset(bitmapData->m_pImageData, 0, bitmapData->m_nWidth * bitmapData->m_nHeight * 4);
                    return;
                }

                unsigned short* pBorderBmp = m_pWordBmpBorder;
                memset(pBorderBmp, 0, nW * nH * sizeof(unsigned short) * 4);
                for (int y = 0; y < currentInfo.datah; y++)
                {
                    for (int x = 0; x < currentInfo.dataw; x++)
                    {
                        unsigned char bmpColor = currentInfo.pBmp[y*currentInfo.dataw + x];
                        //8个方向扩充
                        pBorderBmp[y * nW + x] += bmpColor;//左上
                        pBorderBmp[y * nW + x + nBorderSize] += bmpColor;//上
                        pBorderBmp[y * nW + x + nBorderSize * 2] += bmpColor;//右上
                        pBorderBmp[(y + nBorderSize)*nW + x] += bmpColor;//左中
                        pBorderBmp[(y + nBorderSize)*nW + x + nBorderSize * 2] += bmpColor;//右中
                        pBorderBmp[(y + nBorderSize * 2)*nW + x] += bmpColor;//左下
                        pBorderBmp[(y + nBorderSize * 2)*nW + x + nBorderSize] += bmpColor;//下
                        pBorderBmp[(y + nBorderSize * 2)*nW + x + nBorderSize * 2] += bmpColor;//右下
                    }
                }
                {
                    int nBR = nBorderColor & 0xff;
                    int nBG = (nBorderColor >> 8) & 0xff;
                    int nBB = (nBorderColor >> 16) & 0xff;
                    for (int y = 0; y < nH; y++)
                    {
                        for (int x = 0; x < nW; x++)
                        {
                            uint64_t bmpColor = pBorderBmp[x + y*nW];
                            bmpColor = bmpColor > 255 ? 255 : bmpColor;
                            float fAlpha = bmpColor / 255.0f;
                            int b = (int)(nBB * fAlpha);
                            int g = (int)(nBG * fAlpha);
                            int r = (int)(nBR * fAlpha);
                            int pos = offset + x + currentInfo.datal + (y + currentInfo.datat) * bitmapData->m_nWidth;
                            if (pos < MAX_INT32_INDEX && !pBuff[pos])
                                pBuff[pos] = ((int)bmpColor << 24) + (b << 16) + (g << 8) + r;
                        }
                    }
                }
                //文字本身的颜色
                int nTR = nColor & 255;
                int nTG = (nColor >> 8) & 255;
                int nTB = (nColor >> 16) & 255;
                //文字描边的颜色
                int nBR = nBorderColor & 0xff;
                int nBG = (nBorderColor >> 8) & 0xff;
                int nBB = (nBorderColor >> 16) & 0xff;
                for (int y = 0; y < currentInfo.datah; y++)
                {
                    for (int x = 0; x < currentInfo.dataw; x++)
                    {
                        int alpha = (int)(currentInfo.pBmp[x + y*currentInfo.dataw]);
                        float fAlpha = alpha / 255.0f;
                        int nPos = (y + currentInfo.datat + nBorderSize) * bitmapData->m_nWidth + offset + x + nBorderSize + currentInfo.datal;
                        if (nPos < MAX_INT32_INDEX && alpha > 0)
                        {
                            int r = (int)(nTR * fAlpha + nBR * (1 - fAlpha));
                            int g = (int)(nTG * fAlpha + nBG * (1 - fAlpha));
                            int b = (int)(nTB * fAlpha + nBB * (1 - fAlpha));
                            pBuff[nPos] = (255 << 24) + (b << 16) + (g << 8) + r;
                        }
                    }
                }
                //下划线
                if (fontInfo->m_nDecorationLine == 1)
                {
                    int nLineColor = fontInfo->m_nDecorationColor;
                    unsigned char* pLineColor = (unsigned char*)&nLineColor;
                    std::swap(pLineColor[0], pLineColor[2]);
                    int nLineTop = currentInfo.nUnderlineTop + nBorderSize * 2;
                    for (int y = 0; y < currentInfo.nUnderlineHeight; y++) {
                        for (int x = 0; x < currentInfo.nTextWidth; x++) {
							int index = offset + x + (y + nLineTop) * bitmapData->m_nWidth;
							if (index < MAX_INT32_INDEX) {
								pBuff[index] = *(int*)(pLineColor);
							}
                        }
                    }
                    if (nLineTop + currentInfo.nUnderlineHeight > currentInfo.nTextHeight)
                    {
                        currentInfo.nTextHeight = currentInfo.nUnderlineTop + currentInfo.nUnderlineHeight;
                    }
                }
            }
            else
            {
                int nTR = nColor & 255;
                int nTG = (nColor >> 8) & 255;
                int nTB = (nColor >> 16) & 255;
                for (int y = 0; y < currentInfo.datah; y++) {
                    for (int x = 0; x < currentInfo.dataw; x++) {
                        unsigned char a = currentInfo.pBmp[x + y*currentInfo.dataw];
                        float fAlpha = a / 255.0f;
                        int b = (int)(nTB * fAlpha);
                        int g = (int)(nTG * fAlpha);
                        int r = (int)(nTR * fAlpha);
						int index = offset + x + currentInfo.datal + (y + currentInfo.datat) * bitmapData->m_nWidth;
						if (index < MAX_INT32_INDEX) {
							pBuff[index] = (a << 24) + (b << 16) + (g << 8) + r;
						}
                    }
                }
                //下划线
                if (fontInfo->m_nDecorationLine == 1)
                {
                    int nLineColor = fontInfo->m_nDecorationColor;
                    unsigned char* pLineColor = (unsigned char*)&nLineColor;
                    std::swap(pLineColor[0], pLineColor[2]);
                    for (int y = 0; y < currentInfo.nUnderlineHeight; y++) {
                        for (int x = 0; x < currentInfo.nTextWidth; x++) {
							int index = offset + x + (y + currentInfo.nUnderlineTop) * bitmapData->m_nWidth;
							if (index < MAX_INT32_INDEX) {
								pBuff[index] = *(int*)(pLineColor);
							}
                        }
                    }
                    if (currentInfo.nUnderlineTop + currentInfo.nUnderlineHeight > currentInfo.nTextHeight)
                    {
                        currentInfo.nTextHeight = currentInfo.nUnderlineTop + currentInfo.nUnderlineHeight;
                    }
                }
            }
            offset += currentInfo.nTextWidth;
        }
        infoVector.clear();
    }
    bool JCFreeTypeFontRender::setFontFaceFromUrl(const char* sFontFamily, const char* sTTFFileName)
    {
#ifndef WEBASM
        m_kLoadGlyphLock.lock();
        bool bRet = false;
        FT_Face pFTFace = getFTFaceFromFile(sTTFFileName);
        if (pFTFace)
        {
            MapFTFace::iterator iter = m_vCustomMadeFT.find(sFontFamily);
            if (iter!= m_vCustomMadeFT.end())
            {
                delete iter->second;
            }
            m_vCustomMadeFT[sFontFamily] = new FTFaceRecord(pFTFace);
            bRet = true;
        }
        m_kLoadGlyphLock.unlock();
        return bRet;
#endif
        return true;
    }
    bool JCFreeTypeFontRender::setFontFaceFromBuffer(const char* sFontFamily, char* pBuffer, int nBuferLen)
    {
#ifndef WEBASM
        m_kLoadGlyphLock.lock();
        bool bRet = false;
        //arrayBuffer will be GC , so copy
        char* copyBuffer = new char[nBuferLen];
        memcpy(copyBuffer, pBuffer, nBuferLen);
        FT_Face pFTFace = getFTFaceFromBuffer(copyBuffer,nBuferLen);
        if (pFTFace)
        {
            MapFTFace::iterator iter = m_vCustomMadeFT.find(sFontFamily);
            if (iter != m_vCustomMadeFT.end())
            {
                delete iter->second;
            }
            m_vCustomMadeFT[sFontFamily] = new FTFaceRecord(pFTFace, copyBuffer);
            bRet = true;
        }
        m_kLoadGlyphLock.unlock();
        return bRet;
#endif
        return true;
    }
    bool JCFreeTypeFontRender::removeFont(const char* sFontFamily)
    {
#ifndef WEBASM
        m_kLoadGlyphLock.lock();
        bool bRet = false;
        MapFTFace::iterator iter = m_vCustomMadeFT.find(sFontFamily);
        if( iter != m_vCustomMadeFT.end() )
        {
            delete iter->second;
            iter = m_vCustomMadeFT.erase(iter);
            bRet = true;
        }
        m_kLoadGlyphLock.unlock();
        return bRet;
#endif
        return true;
    }
    void JCFreeTypeFontRender::setIOSFTInterface(JCIOSFTInterface* pIOSFTInterface)
    {
        m_pIOSFTInterface = pIOSFTInterface;
    }
    void JCFreeTypeFontRender::measureText( char* p_sWord, JCFontInfo* p_pFontInfo, int& p_nW, int& p_nH)
    {
        if (!p_sWord)
        {
            p_nW = p_nH = 0;
            return;
        }
        int pA = 0;
        static unsigned short ucStr[8];
        int nResultLen = UTF8StrToUnicodeStr((unsigned char*)p_sWord, ucStr, 8);
        if (nResultLen > 1)
        {
            measureTexts(p_sWord, p_pFontInfo, p_nW, p_nH);
            return;
        }
        if (nResultLen)
        {
            int nWord = ucStr[0];
            m_kLoadGlyphLock.lock();
            setFont(p_pFontInfo->m_sFamily);
            setFontSize(p_pFontInfo->m_nFontSize, 0);
            getMetric(nWord, p_nW, p_nH);
            m_kLoadGlyphLock.unlock();
        }
    }
    void JCFreeTypeFontRender::measureTexts(char* p_sWord, JCFontInfo* p_pFontInfo, int& p_nW, int& p_nH)
    {
        if (!p_sWord)
        {
            p_nW = p_nH = 0;
            return;
        }
        int pA = 0;
        int nWord;
        int wordlen = strlen(p_sWord);
        m_kLoadGlyphLock.lock();
        setFont(p_pFontInfo->m_sFamily);
        setFontSize(p_pFontInfo->m_nFontSize, 0);
        int maxHeight = 0;
        int width = 0;
        while (true)
        {
            int ulen = UTF8ToUnicode((unsigned char*)p_sWord, &nWord);
            wordlen -= ulen;
            getMetric(nWord, p_nW, p_nH);
            pA += p_nW;
            width += p_nW;
            if (p_nH > maxHeight) maxHeight = p_nH;
            if (wordlen <= 0)
                break;
            p_sWord += ulen;
        }
        p_nW = width;
        p_nH = maxHeight;
        m_kLoadGlyphLock.unlock();
    }

    /*bool JCFreeTypeFontRender::getTextBitmapData(char* str, BitmapData* bitmapData, int nColor, JCFontInfo* fontInfo, int borderSize, int nBorderColor)
    {
        unsigned char firstv = str[0];
        if (firstv == '\r' || firstv == '\n')
        {
            bitmapData->m_nWidth = bitmapData->m_nHeight = 0;
            return true;
        }
        int nInBuffLen = bitmapData->m_nWidth*bitmapData->m_nHeight;
        int nTextWidth = 0;
        int nTextHeight = 0;
        int dataw = 0, datah = 0;
        int datal = 0, datat = 0;
        unsigned char* pBmp = NULL;
        static unsigned short ucStr[8];
        int nUnderlineTop = 0;
        int nUnderlineHeight = 0;
        int nResultLen = UTF8StrToUnicodeStr((unsigned char*)str, ucStr, 8);
        if (nResultLen)
        {
            int nWord = ucStr[0];
            m_kLoadGlyphLock.lock();
            setFont(fontInfo->m_sFamily);
            setFontSize(fontInfo->m_nFontSize, 0);
            getBitmapData(nWord, pBmp, nTextWidth, nTextHeight, dataw, datah, datal, datat, nUnderlineTop, nUnderlineHeight);
            
			if (nTextWidth == 0 && (*str == ' '))
			{
				nTextWidth = (int)floorf((fontInfo->m_nFontSize + 1.0f) / 4);
			}
			
			m_kLoadGlyphLock.unlock();
        }
        datal = datal > 0 ? datal : 0;
        datat = datat > 0 ? datat : 0;
        if (nTextWidth<dataw + datal)
        {
            nTextWidth = dataw + datal;
        }
        if (nTextHeight<datah + datat)
        {
            nTextHeight = datah + datat;
        }
        int nBorderSize = borderSize > TEXT_SIZE_ALLOWANCE / 2 ? TEXT_SIZE_ALLOWANCE / 2 : borderSize;
        nTextWidth += nBorderSize * 2;
        nTextHeight += nBorderSize * 2;

        if (nTextHeight*nTextWidth>nInBuffLen)
        {
            LOGW("JCFreeTypeFontRender::getTextBitmapData the buffersize is insufficient.");
            delete pBmp;
            return false;
        }
        unsigned int* pBuff = (unsigned int*)bitmapData->m_pImageData;//  new unsigned int [nTextWidth*nTextHeight];
        memset(pBuff, 0, nTextWidth*nTextHeight * 4);//TODO 为了效率这个能去掉么
        if (nBorderSize > 0)
        {
            //这里先把描边绘制好
            int nW = dataw + nBorderSize * 2;
            int nH = datah + nBorderSize * 2;
            unsigned short* pBorderBmp = m_pWordBmpBorder;
            memset(pBorderBmp, 0, nW * nH * sizeof(unsigned short) * 4);
            for (int y = 0; y < datah; y++)
            {
                for (int x = 0; x < dataw; x++)
                {
                    unsigned char bmpColor = pBmp[y*dataw + x];
                    //8个方向扩充
                    pBorderBmp[y * nW + x] += bmpColor;//左上
                    pBorderBmp[y * nW + x + nBorderSize] += bmpColor;//上
                    pBorderBmp[y * nW + x + nBorderSize * 2] += bmpColor;//右上
                    pBorderBmp[(y + nBorderSize)*nW + x] += bmpColor;//左中
                    pBorderBmp[(y + nBorderSize)*nW + x + nBorderSize * 2] += bmpColor;//右中
                    pBorderBmp[(y + nBorderSize * 2)*nW + x] += bmpColor;//左下
                    pBorderBmp[(y + nBorderSize * 2)*nW + x + nBorderSize] += bmpColor;//下
                    pBorderBmp[(y + nBorderSize * 2)*nW + x + nBorderSize * 2] += bmpColor;//右下
                }
            }
            {
                int nBR = nBorderColor & 0xff;
                int nBG = (nBorderColor >> 8) & 0xff;
                int nBB = (nBorderColor >> 16) & 0xff;
                for (int y = 0; y < nH; y++)
                {
                    for (int x = 0; x < nW; x++)
                    {
                        uint64_t bmpColor = pBorderBmp[x + y*nW];
                        bmpColor = bmpColor > 255 ? 255 : bmpColor;
                        float fAlpha = bmpColor / 255.0f;
                        int b = (int)(nBB * fAlpha);
                        int g = (int)(nBG * fAlpha);
                        int r = (int)(nBR * fAlpha);
                        pBuff[x + datal + (y + datat)*nTextWidth] = ((int)bmpColor << 24) + (b << 16) + (g << 8) + r;
                    }
                }
            }

            //文字本身的颜色
            int nTR = nColor & 255;
            int nTG = (nColor >> 8) & 255;
            int nTB = (nColor >> 16) & 255;
            //文字描边的颜色
            int nBR = nBorderColor & 0xff;
            int nBG = (nBorderColor >> 8) & 0xff;
            int nBB = (nBorderColor >> 16) & 0xff;
            for (int y = 0; y < datah; y++)
            {
                for (int x = 0; x < dataw; x++)
                {
                    int alpha = (int)(pBmp[x + y*dataw]);
                    float fAlpha = alpha / 255.0f;
                    int nPos = (y + datat + nBorderSize) * nTextWidth + x + nBorderSize + datal;
                    if (alpha > 0)
                    {
                        int r = (int)(nTR * fAlpha + nBR * (1 - fAlpha));
                        int g = (int)(nTG * fAlpha + nBG * (1 - fAlpha));
                        int b = (int)(nTB * fAlpha + nBB * (1 - fAlpha));
                        pBuff[nPos] = (255 << 24) + (b << 16) + (g << 8) + r;
                    }
                }
            }
            //下划线
            if (fontInfo->m_nDecorationLine == 1)
            {
                int nLineColor = fontInfo->m_nDecorationColor;
                unsigned char* pLineColor = (unsigned char*)&nLineColor;
                std::swap(pLineColor[0], pLineColor[2]);
                int nLineTop = nUnderlineTop + nBorderSize*2;
                for (int y = 0; y < nUnderlineHeight; y++) {
                    for (int x = 0; x < nTextWidth; x++) {
                        pBuff[x + (y + nLineTop)*nTextWidth] = *(int*)(pLineColor);
                    }
                }
                if (nLineTop + nUnderlineHeight > nTextHeight)
                {
                    nTextHeight = nUnderlineTop + nUnderlineHeight;
                }
            }
        }
        else
        {
            int nTR = nColor & 255;
            int nTG = (nColor >> 8) & 255;
            int nTB = (nColor >> 16) & 255;
            for (int y = 0; y < datah; y++) {
                for (int x = 0; x < dataw; x++) {
                    unsigned char a = pBmp[x + y*dataw];
                    float fAlpha = a / 255.0f;
                    int b = (int)(nTB * fAlpha);
                    int g = (int)(nTG * fAlpha);
                    int r = (int)(nTR * fAlpha);
                    pBuff[x + datal + (y + datat)*nTextWidth] = (a << 24) + (b << 16) + (g << 8) + r;
                }
            }
            //下划线
            if (fontInfo->m_nDecorationLine == 1)
            {
                int nLineColor = fontInfo->m_nDecorationColor;
                unsigned char* pLineColor = (unsigned char*)&nLineColor;
                std::swap(pLineColor[0], pLineColor[2]);
                for (int y = 0; y < nUnderlineHeight; y++) {
                    for (int x = 0; x < nTextWidth; x++) {
                        pBuff[x + (y + nUnderlineTop)*nTextWidth] = *(int*)(pLineColor);
                    }
                }
                if (nUnderlineTop + nUnderlineHeight > nTextHeight)
                {
                    nTextHeight = nUnderlineTop + nUnderlineHeight;
                }
            }
        }
        bitmapData->m_nWidth = nTextWidth;
        bitmapData->m_nHeight = nTextHeight;
        delete[] pBmp;
        pBmp = NULL;
        return true;
    }
    */
    std::string JCFreeTypeFontRender::getIOSFontTTFPath()
    {
        return m_pIOSFTInterface->getIOSFontTTFPath();
    }
    std::string JCFreeTypeFontRender::writeIOSFontTTF()
    {
        return m_pIOSFTInterface->writeIOSFontTTF();
    }
    void JCFreeTypeFontRender::measureChar(int unicode, JCFontInfo* p_pFontInfo, int& p_nW, int& p_nH)
    {
        m_kLoadGlyphLock.lock();
        setFont(p_pFontInfo->m_sFamily);
        setFontSize(p_pFontInfo->m_nFontSize, 0);
        getMetric(unicode, p_nW, p_nH);
        m_kLoadGlyphLock.unlock();
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
