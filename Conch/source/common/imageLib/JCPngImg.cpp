/**
@file			JCPngImg.cpp
@brief			
@author			hugao
@version		1.0
@date			2016_5_11
*/

#include <string.h>
#include "png.h"
#include "pngconf.h"
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "../util/JCCommonMethod.h"
#include "JCImageRW.h"

//------------------------------------------------------------------------------
#pragma warning (disable: 4996)
namespace laya
{
	typedef struct  
	{
		unsigned char* data;
		int size;
		int offset;
	}ImageSource;

	//从内存读取PNG图片的回调函数
	static void pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length)
	{
		ImageSource* isource = (ImageSource*)png_get_io_ptr(png_ptr);
		if( (int)(isource->offset + length) <= isource->size)
		{
			memcpy(data, isource->data+isource->offset, length);
			isource->offset += length;
		}
		else
			png_error(png_ptr, "pngReaderCallback failed");
	}
	void my_error_fn(png_structp png_ptr, png_const_charp error_msg)
	{
		
	}
	void my_warning_fn(png_structp png_ptr,png_const_charp warning_msg)
	{
		
	}

    static unsigned int multiply_alpha(unsigned int  alpha, unsigned int  color)
    {
        unsigned int  temp = alpha * color + 0x80;
        return (temp + (temp >> 8)) >> 8;
    }

    static void premultiply_data(png_structp png, png_row_infop row_info, png_bytep data)
    {
        unsigned int  i;

        for (i = 0; i < row_info->rowbytes; i += 4)
        {
            unsigned char*  base = &data[i];
            unsigned int    alpha = base[3];


            if (alpha == 0)
                base[0] = base[1] = base[2] = base[3] = 0;

            else
            {
                unsigned int  red = base[0];
                unsigned int  green = base[1];
                unsigned int  blue = base[2];


                if (alpha != 0xFF)
                {
                    red = multiply_alpha(alpha, red);
                    green = multiply_alpha(alpha, green);
                    blue = multiply_alpha(alpha, blue);
                }

                base[0] = (unsigned char)red;
                base[1] = (unsigned char)green;
                base[2] = (unsigned char)blue;
                base[3] = (unsigned char)alpha;
            }
        }
    }
	unsigned char* ReadPNGFromMem(unsigned char* data, int dataSize, PNG_UINT32* w, PNG_UINT32* h)
	{
		//unsigned char sig[8];
		png_structp png_ptr;  
		png_infop   info_ptr;  
		unsigned char* image_data;  
		int bit_depth;  
		int color_type;  
		unsigned int rowbytes;  
		png_uint_32 i;  
		png_bytepp row_pointers;  

		if ((png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, my_error_fn, my_warning_fn)) == NULL) {  
			return NULL;  
		}  
		if ((info_ptr = png_create_info_struct(png_ptr)) == NULL) {  
			png_destroy_read_struct(&png_ptr, NULL, NULL);  
			return NULL;  
		}  

		if (setjmp(png_jmpbuf(png_ptr))) {  
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);  
			return NULL;  
		}  

		ImageSource imgsource;
		imgsource.data = data;
		imgsource.size = dataSize;
		imgsource.offset = 0;
		png_set_read_fn(png_ptr, &imgsource,pngReadCallback);
		png_read_info(png_ptr, info_ptr); 	
		png_set_sig_bytes(png_ptr, 8);  
		png_get_IHDR(png_ptr, info_ptr, w, h, &bit_depth, &color_type, NULL, NULL, NULL);  

        //预乘alpha
        //png_set_read_user_transform_fn(png_ptr, premultiply_data);

		//if (color_type & PNG_COLOR_MASK_ALPHA)
		//    png_set_strip_alpha(png_ptr);  //去掉alhpa信息了。TODO 以后要加上。
		if (bit_depth >8)
			png_set_strip_16(png_ptr);  //16bit的通道要转换成8bit的
		if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)  
			png_set_gray_to_rgb(png_ptr);  
		if (color_type == PNG_COLOR_TYPE_PALETTE)  
			png_set_palette_to_rgb(png_ptr);  
		if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
			png_set_tRNS_to_alpha(png_ptr);
		// if (color_type == PNG_COLOR_TYPE_RGB)
		png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);//PNG_FILLER_BEFORE);

		png_read_update_info(png_ptr, info_ptr);  

		rowbytes = png_get_rowbytes(png_ptr, info_ptr);  
		if ((image_data =(unsigned char *) malloc(*h * rowbytes)) == NULL) {  
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);  
			return NULL;  
		}  
		if ((row_pointers =(png_bytepp) malloc(*h * sizeof(png_bytep))) == NULL) {  
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);  
			free(image_data);  
			return NULL;  
		}  

		for (i = 0; i < *h; i++)  
			//row_pointers[*h - 1 - i] = image_data + i*rowbytes;
			row_pointers[i] = image_data + i*rowbytes;

		png_read_image(png_ptr, row_pointers);  
		free(row_pointers);  
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);  

		return image_data;  
	}
    /*
	//获得宽高信息，并不最终解码
	bool LoadPngWH( const char* p_sFileName,int& p_nWidth,int& p_nHeight )
	{
		FILE* f;
		unsigned char sig[8];
		png_structp png_ptr;
		png_infop   info_ptr;
		//unsigned char* image_data;
		int bit_depth;
		int color_type;
		//unsigned int rowbytes;
		//png_uint_32 i;
		//png_bytepp row_pointers;

		if ((f = fopen(p_sFileName, "rb")) == NULL)
			return false;
		fread(sig, sizeof(*sig), sizeof(sig), f);
		if (!png_check_sig(sig, sizeof(*sig))) {
			fclose(f);
			return false;
		}
		if ((png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL)) == NULL) {
			fclose(f);
			return false;
		}
		if ((info_ptr = png_create_info_struct(png_ptr)) == NULL) {
			png_destroy_read_struct(&png_ptr, NULL, NULL);
			fclose(f);
			return false;
		}

		if (setjmp(png_jmpbuf(png_ptr))) {
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			fclose(f);
			return false;
		}
		png_ptr->io_ptr = (png_voidp) f;
		png_set_sig_bytes(png_ptr, 8);

		png_read_info(png_ptr, info_ptr);
		png_uint_32 w,h;
		png_get_IHDR(png_ptr, info_ptr, &w, &h, &bit_depth, &color_type, NULL, NULL, NULL);
		p_nWidth = w;
		p_nHeight = h;
		fclose(f);
		return true;
	}
    */
	int SavePng(char* png_file_name, char **data, int width, int height, int bit_depth)  
	{  
		png_structp png_ptr;  
		png_infop info_ptr;  
		FILE *png_file = fopen(png_file_name, "wb");  
		//assert(png_file);  
		png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);  
		if(png_ptr == NULL)  
		{  
			//cerr<<"ERROR:png_create_write_struct/n";
            if (png_file)
                fclose(png_file);
			return 0;  
		}  
		info_ptr = png_create_info_struct(png_ptr);  
		if(info_ptr == NULL)  
		{  
			//cerr<<"ERROR:png_create_info_struct/n";
            if (png_file) {
                fclose(png_file);
            }
			png_destroy_write_struct(&png_ptr, NULL);  
			return 0;  
		}  
		png_init_io(png_ptr, png_file);  
		png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, PNG_COLOR_TYPE_RGB_ALPHA,  
			PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);  
		//png_set_PLTE(png_ptr, info_ptr, palette, palette_len);  
		png_write_info(png_ptr, info_ptr);  

		png_bytepp row_pointers = new png_bytep[height];  
		for (int i=0; i<height; ++i)  
		{  
			row_pointers[i] = (png_bytep)data[i];  
		}  
		png_write_image(png_ptr, row_pointers);  
		delete[] row_pointers;
		png_write_end(png_ptr, info_ptr);  
		//png_free(png_ptr, palette);  
		//palette=NULL;  
		png_destroy_write_struct(&png_ptr, &info_ptr);  
		fclose(png_file);  
		return 0;  
	}

    typedef unsigned char ui8;
    static void PngWriteToMemoryFunc(png_structp  png_ptr, png_bytep data, png_size_t length) {
        std::vector<ui8> *p = (std::vector<ui8>*)png_get_io_ptr(png_ptr);
        p->insert(p->end(), data, data + length);
    }

    std::pair<unsigned char*, unsigned long> convertBitmapToPng(const char* p_pData, int nWidth, int nHeight, int nBitDepth) 
    {
        std::vector<unsigned char> buffer;
        buffer.reserve(nWidth * nHeight * 4);


        std::pair<unsigned char*, unsigned long> res
            = std::make_pair((unsigned char*)NULL, 0);

        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, (png_voidp)NULL, NULL, NULL);
        if (!png_ptr)
            return res;
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
            return res;

        if (setjmp(png_jmpbuf(png_ptr)))
            return res;

        png_set_write_fn(png_ptr, &buffer, PngWriteToMemoryFunc, NULL);


        png_set_IHDR(png_ptr, info_ptr,
            nWidth,
            nHeight,
            8,
            PNG_COLOR_TYPE_RGB_ALPHA,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT);

        png_write_info(png_ptr, info_ptr);
        for (int i = 0; i < nHeight; i++)
            png_write_row(png_ptr, (unsigned char*)p_pData + i * nWidth * 4);

        png_write_end(png_ptr, info_ptr);

        png_destroy_write_struct(&png_ptr, &info_ptr);

        unsigned long nByte = buffer.size();
        unsigned char* pBuffer = new unsigned char[nByte];
        memcpy(pBuffer, &buffer[0], nByte);
        res.first = pBuffer;
        res.second = nByte;
        return res;
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
