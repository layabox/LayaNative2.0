/**
@file			JCJpegImg.cpp
@brief			
@author			hugao
@version		1.0
@date			2016_5_11
*/

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "JCImageRW.h"
#include "../util/JCCommonMethod.h"
#pragma warning (disable: 4996)
//------------------------------------------------------------------------------

/*
 * Include file for users of JPEG library.
 * You will need to have included system headers that define at least
 * the typedefs FILE and size_t before you can include jpeglib.h.
 * (stdio.h is sufficient on ANSI-conforming systems.)
 * You may also wish to include "jerror.h".
 */
#include "jpeglib.h"
/*
 * <setjmp.h> is used for the optional error recovery mechanism shown in
 * the second part of the example.
 */
#include <setjmp.h>
namespace laya
{
	/******************** JPEG COMPRESSION SAMPLE INTERFACE *******************/

	/* This half of the example shows how to feed data into the JPEG compressor.
	 * We present a minimal version that does not worry about refinements such
	 * as error recovery (the JPEG code will just exit() if it gets an error).
	 */
	/*
	 * ERROR HANDLING:
	 *
	 * The JPEG library's standard error handler (jerror.c) is divided into
	 * several "methods" which you can override individually.  This lets you
	 * adjust the behavior without duplicating a lot of code, which you might
	 * have to update with each future release.
	 *
	 * Our example here shows how to override the "error_exit" method so that
	 * control is returned to the library's caller when a fatal error occurs,
	 * rather than calling exit() as the standard error_exit method does.
	 *
	 * We use C's setjmp/longjmp facility to return control.  This means that the
	 * routine which calls the JPEG library must first execute a setjmp() call to
	 * establish the return point.  We want the replacement error_exit to do a
	 * longjmp().  But we need to make the setjmp buffer accessible to the
	 * error_exit routine.  To do this, we make a private extension of the
	 * standard JPEG error handler object.  (If we were using C++, we'd say we
	 * were making a subclass of the regular error handler.)
	 *
	 * Here's the extended error handler struct:
	 */

	struct my_error_mgr {
	  struct jpeg_error_mgr pub;	/* "public" fields */

	  jmp_buf setjmp_buffer;	/* for return to caller */
	};

	typedef struct my_error_mgr * my_error_ptr;

	/*
	 * Here's the routine that will replace the standard error_exit method:
	 */

	METHODDEF(void)
	my_error_exit (j_common_ptr cinfo)
	{
	  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
	  my_error_ptr myerr = (my_error_ptr) cinfo->err;

	  /* Always display the message. */
	  /* We could postpone this until after returning, if we chose. */
	  (*cinfo->err->output_message) (cinfo);

	  /* Return control to the setjmp point */
	  longjmp(myerr->setjmp_buffer, 1);
	}


	/*
	 * Sample routine for JPEG decompression.  We assume that the source file name
	 * is passed in.  We want to return 1 on success, 0 on error.
	 */
	unsigned char* readFile(const char* filename,int &size)
	{
	  unsigned char *infile;
	  FILE* fInput;
	  if ((fInput = fopen(filename, "rb")) == NULL)
	  {
		  fprintf(stderr, "can't open %s\n", filename);
		  return 0;
	  }
	  fseek(fInput,0L,SEEK_END);
	  size=ftell(fInput);
	  fseek(fInput,0L,SEEK_SET);
	  infile = (unsigned char*)malloc(size);
	  if (!infile) {
		  throw - 3;
	  }
	  else {
		  size = fread(infile, 1, size, fInput);
	  }
	  fclose(fInput);
	  return infile;
	}

	GLOBAL(int) LoadJpegFromMem (BitmapData * texture,unsigned  char * infile,int size )
	{
	  /* This struct contains the JPEG decompression parameters and pointers to
	   * working space (which is allocated as needed by the JPEG library).
	   */
	  struct jpeg_decompress_struct cinfo;
	  /* We use our private extension JPEG error handler.
	   * Note that this struct must live as long as the main JPEG parameter
	   * struct, to avoid dangling-pointer problems.
	   */
	  struct my_error_mgr jerr;
	  /* More stuff */
	  JSAMPARRAY buffer;		/* Output row buffer */
	  int row_stride;		/* physical row width in output buffer */

	  /* In this example we want to open the input file before doing anything else,
	   * so that the setjmp() error recovery below can assume the file is open.
	   * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
	   * requires it in order to read binary files.
	   */
	  /* Step 1: allocate and initialize JPEG decompression object */

	  /* We set up the normal JPEG error routines, then override error_exit. */
	  cinfo.err = jpeg_std_error(&jerr.pub);
	  jerr.pub.error_exit = my_error_exit;
	  /* Establish the setjmp return context for my_error_exit to use. */
	  if (setjmp(jerr.setjmp_buffer)) {
		/* If we get here, the JPEG code has signaled an error.
		 * We need to clean up the JPEG object, close the input file, and return.
		 */
		jpeg_destroy_decompress(&cinfo);
		//fclose(infile);
		return 0;
	  }
	  /* Now we can initialize the JPEG decompression object. */
	  jpeg_create_decompress(&cinfo);

	  /* Step 2: specify data source (eg, a file) */

	  //jpeg_stdio_src(&cinfo, infile);
	  jpeg_mem_src(&cinfo, infile,size);

	  /* Step 3: read file parameters with jpeg_read_header() */

	  (void) jpeg_read_header(&cinfo, TRUE);
	  /* We can ignore the return value from jpeg_read_header since
	   *   (a) suspension is not possible with the stdio data source, and
	   *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
	   * See libjpeg.txt for more info.
	   */

	  /* Step 4: set parameters for decompression */

	  /* In this example, we don't need to change any of the defaults set by
	   * jpeg_read_header(), so we do nothing here.
	   */

      if (cinfo.jpeg_color_space == JCS_GRAYSCALE)
      {
      }
      else
      {
          cinfo.out_color_space = JCS_RGB;
      }

	  /* Step 5: Start decompressor */

	  (void) jpeg_start_decompress(&cinfo);
	  /* We can ignore the return value since suspension is not possible
	   * with the stdio data source.
	   */

	  texture->m_nWidth  = cinfo.image_width;
	  texture->m_nHeight = cinfo.output_height;
	  texture->m_nBpp = 32;//cinfo.output_components * 8;
	  /*if(cinfo.output_components!=3)
	  {
          (void)jpeg_finish_decompress(&cinfo);
          jpeg_destroy_decompress(&cinfo);
		  return false;
	  }*/
	#if 0
	  if(cinfo.output_components == 3)
	  {
		texture->type	= GL_RGB;
	  }
	  else  // Else if its 32 BPP
	  {
		texture->type	= GL_RGBA;
	  }
		texture->imageData = std::shared_ptr<GLubyte> (new GLubyte[texture->width*texture->height*cinfo.output_components], std::default_delete<char[]>());
	#endif
        
	  //texture->type	= GL_RGBA;
	  texture->m_pImageData = /*std::shared_array<char> */(new char[texture->m_nWidth*texture->m_nHeight*texture->m_nBpp / 8]);
	  if(texture->m_pImageData == NULL)											// If no space was allocated
	  {
		  //fclose(infile);														// Close the file
          (void)jpeg_finish_decompress(&cinfo);
          jpeg_destroy_decompress(&cinfo);
		  return false;														// Return failed
	  }

	  /* We may need to do some setup of our own at this point before reading
	   * the data.  After jpeg_start_decompress() we have the correct scaled
	   * output image dimensions available, as well as the output colormap
	   * if we asked for color quantization.
	   * In this example, we need to make an output work buffer of the right size.
	   */
	  /* JSAMPLEs per row in output buffer */
	  row_stride = cinfo.output_width * cinfo.output_components;
	  /* Make a one-row-high sample array that will go away when done with image */
	  buffer = (*cinfo.mem->alloc_sarray)
			((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	  /* Step 6: while (scan lines remain to be read) */
	  /*           jpeg_read_scanlines(...); */

	  /* Here we use the library's state variable cinfo.output_scanline as the
	   * loop counter, so that we don't have to keep track ourselves.
	   */
	 #if 0
	  int bufOffset = 0;
	  int line =1;
	  while (cinfo.output_scanline < cinfo.output_height) {
		/* jpeg_read_scanlines expects an array of pointers to scanlines.
		 * Here the array is only one element long, but you could ask for
		 * more than one scanline at a time if that's more convenient.
		 */
		(void) jpeg_read_scanlines(&cinfo, buffer, 1);
		/* Assume put_scanline_someplace wants a pointer and sample count. */
		bufOffset = (cinfo.output_height-line)*row_stride;
		memcpy(texture->imageData+bufOffset,buffer[0],row_stride);   //kuo
		line++;
	  }
	#else
		int line =0;
        int pixels = 0;
        if (cinfo.jpeg_color_space == JCS_GRAYSCALE)
        {
            pixels = row_stride;
        }
        else
        {
            pixels = row_stride / 3;
        }
		while (cinfo.output_scanline < cinfo.output_height)
		{
			/* jpeg_read_scanlines expects an array of pointers to scanlines.
			* Here the array is only one element long, but you could ask for
			* more than one scanline at a time if that's more convenient.
			*/
			(void) jpeg_read_scanlines(&cinfo, buffer, 1);
			unsigned char* startBuffer = (unsigned char*)texture->m_pImageData+line*pixels*4;
            if (cinfo.jpeg_color_space == JCS_GRAYSCALE)
            {
                for (int i = 0; i < pixels; ++i)
                {
                    startBuffer[i * 4 + 0] = buffer[0][i];
                    startBuffer[i * 4 + 1] = buffer[0][i];
                    startBuffer[i * 4 + 2] = buffer[0][i];
                    startBuffer[i * 4 + 3] = 255;
                }
            }
            else
            {
                for (int i = 0; i < pixels; ++i)
                {
                    startBuffer[i * 4 + 0] = buffer[0][i * 3 + 0];
                    startBuffer[i * 4 + 1] = buffer[0][i * 3 + 1];
                    startBuffer[i * 4 + 2] = buffer[0][i * 3 + 2];
                    startBuffer[i * 4 + 3] = 255;
                }
            }
			/* Assume put_scanline_someplace wants a pointer and sample count. */
			//memcpy(texture->imageData+line*row_stride,buffer[0],row_stride);   //kuo
			line++;
		}

	#endif
	  /* Step 7: Finish decompression */

	  (void) jpeg_finish_decompress(&cinfo);
	  /* We can ignore the return value since suspension is not possible
	   * with the stdio data source.
	   */

	  /* Step 8: Release JPEG decompression object */

	  /* This is an important step since it will release a good deal of memory. */
	  jpeg_destroy_decompress(&cinfo);

	  /* After finish_decompress, we can close the input file.
	   * Here we postpone it until after no more JPEG errors are possible,
	   * so as to simplify the setjmp error logic above.  (Actually, I don't
	   * think that jpeg_destroy can do an error exit, but why assume anything...)
	   */
	  //fclose(infile);


	  /* At this point you may want to check to see whether any corrupt-data
	   * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
	   */

	  /* And we're done! */
	  return 1;
	}

	//------------------------------------------------------------------------------
	bool LoadJpegWH( const char* p_sFileName,int& p_nWidth,int& p_nHeight )
    {
		return true;
	}
	//保存jpeg图片，要求格式必须为rgb的
	bool saveJpeg(int width ,int height,int bpp, char *buffer, int quality, const char*filename){
		int components = bpp/8;
		if( components!=1 && components!=3 )
			return false;
		jpeg_compress_struct jcs;
		jpeg_error_mgr jem;
		FILE *fp;
		JSAMPROW row_pointer[1];//?一行位图?
		int row_stride; //?每一行的字节数
		jcs.err = jpeg_std_error(&jem);
		jpeg_create_compress(&jcs);
		fp = fopen(filename,"wb");
		if(fp==NULL)
			return false;
		jpeg_stdio_dest(&jcs,fp);
		jcs.image_width = width; //位图的宽和高，单位为像素
		jcs.image_height = height;
		jcs.input_components =components; //?在此为1,表示灰度图，?如果是彩色位图，则为3?
		jcs.in_color_space = components==3?JCS_RGB:JCS_GRAYSCALE;//JCS_GRAYSCALE表示灰度图，JCS_RGB表示彩色图像??
		jpeg_set_defaults(&jcs);
		jpeg_set_quality(&jcs, quality, (boolean)true);
		jpeg_start_compress(&jcs,(boolean)true);
		row_stride = jcs.input_components*width;	/* JSAMPLEs per row in image_buffer */
		while(jcs.next_scanline < jcs.image_height){
			row_pointer[0] = (JSAMPROW)&buffer[jcs.next_scanline * row_stride ];
			jpeg_write_scanlines(&jcs, row_pointer, 1);
		}
		jpeg_finish_compress(&jcs);
		jpeg_destroy_compress(&jcs);
		fclose(fp);
		return true;
	}
    std::pair<unsigned char*, unsigned long> convertBitmapToJpeg(int uWidth, int uHeight, int bpp, unsigned char* pImg, int iQuality)
    {
        std::pair<unsigned char*, unsigned long> mRes = std::make_pair((unsigned char*)NULL, 0);
        int components = bpp / 8;
        if (components != 1 && components != 3)
            return mRes;
        jpeg_compress_struct  jcInfo;
        jpeg_error_mgr      jErr;
        jcInfo.err = jpeg_std_error(&jErr);

        jpeg_create_compress(&jcInfo);

        jpeg_mem_dest(&jcInfo, &mRes.first, &mRes.second);

        jcInfo.image_width = uWidth;
        jcInfo.image_height = uHeight;
        jcInfo.input_components = 3;
        jcInfo.in_color_space = JCS_RGB;


        jpeg_set_defaults(&jcInfo);

        jpeg_set_quality(&jcInfo, iQuality, TRUE);

        jpeg_start_compress(&jcInfo, TRUE);
        int  iRowStride = jcInfo.image_width * jcInfo.input_components;
        while (jcInfo.next_scanline < jcInfo.image_height) {
            JSAMPROW pData = &(pImg[jcInfo.next_scanline * iRowStride]);
            jpeg_write_scanlines(&jcInfo, &pData, 1);
        }

        jpeg_finish_compress(&jcInfo);

        jpeg_destroy_compress(&jcInfo);

        return mRes;
    }
    std::pair<unsigned char*, unsigned long> convertBitmapToJpeg(const char* p_pData, int nWidth, int nHeight, int nBpp)
    {
        char* pData = (char*)p_pData;

        //32转24
        if (nBpp == 32) {
            pData = new char[nWidth*nHeight * 3];
            char* pSrc = (char*)p_pData;
            char* pDst = pData;
            for (int y = 0; y < nHeight; y++) {
                for (int x = 0; x < nWidth; x++) {
                    pDst[0] = pSrc[0]; pDst[1] = pSrc[1]; pDst[2] = pSrc[2];
                    pSrc += 4;
                    pDst += 3;
                }
            }
        }
        std::pair<unsigned char*, unsigned long> ret = convertBitmapToJpeg(nWidth, nHeight, 24, (unsigned char*)pData, 80);
        if (nBpp == 32)
            delete[] pData;

        return ret;
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
