/**
@file			JCImageRW.cpp
@brief			
@author			hugao
@version		1.0
@date			2016_5_11
*/

#include "JCImageRW.h"
#include "../misc/JCWorkerThread.h"
#include "../buffer/JCBuffer.h"
#include "../fileSystem/JCFileSystem.h"
#include <cmath>

//------------------------------------------------------------------------------
namespace laya
{
    BitmapData::BitmapData() {
        m_nImageID = 0;
        m_nWidth = 0;
        m_nHeight = 0;
        m_nBpp = 32;
        m_pImageData = 0;
        m_nImageType = ImgType_unknow;

    }
    BitmapData::BitmapData(char* p_pData, int p_nWidth, int p_nHeight) {
        m_nWidth = p_nWidth;
        m_nHeight = p_nHeight;
        m_nBpp = 32;
        m_pImageData = p_pData;
        m_nImageType = ImgType_unknow;
    }
    BitmapData::BitmapData(int p_nWidth, int p_nHeight, bool p_bTranslate, int p_nFillColor) {
        m_nWidth = p_nWidth;
        m_nHeight = p_nHeight;
        m_nBpp = 32;
        m_pImageData = (char*) new int[p_nWidth*p_nHeight];
        if (p_nFillColor == 0)
            memset(m_pImageData, 0, p_nWidth*p_nHeight*sizeof(int));
        else
            for (int i = 0; i < m_nWidth*m_nHeight; i++) ((int*)m_pImageData)[i] = p_nFillColor;
        m_nImageType = ImgType_unknow;
    }

	void BitmapData::reconfigure(int width, int height, int bpp, ImageType imageType)
	{
		bool isChanged = false;
        
		if (m_nWidth != width)
		{
			isChanged = true;
			m_nWidth = width;
		}

		if (m_nHeight != height)
		{
			isChanged = true;
			m_nHeight = height;
		}

		if (m_nBpp != bpp)
		{
			isChanged = true;
			m_nBpp = bpp;
		}

        if(m_pImageData == nullptr) {
            isChanged = true;
        }
        
		m_nImageType = imageType;

		if (isChanged)
		{
			releaseData();
			int bufferLength = this->m_nWidth * this->m_nHeight * (this->m_nBpp / 8);
			m_pImageData = new char[bufferLength];
		}
	}

	void BitmapData::copyData(char* dataPtr)
	{
		int bufferLength = this->m_nWidth * this->m_nHeight * (this->m_nBpp / 8);
		memcpy(m_pImageData, dataPtr, bufferLength);
	}

#ifndef WEBASM
	unsigned char* ReadPNGFromMem(unsigned char* data, int dataSize, PNG_UINT32* w, PNG_UINT32* h);
	int LoadJpegFromMem (BitmapData* pBitmapData,unsigned  char * memData, int size );
    int LoadGif(BitmapData* pBitmapData, unsigned  char * memData, int size);
	bool saveJpeg(int width ,int height,int bpp, char *buffer, int quality, const char*filename);
	JCWorkerThread* g_DecThread=NULL;	//全局的解码线程
	
	bool loadImageMemSync( const char* p_pMem, int p_nLenth, BitmapData& p_bmp ){
		ImageType imgType = getImgType(p_pMem, p_nLenth);
        p_bmp.m_nImageType = imgType;
		p_bmp.m_nBpp = 32;
		switch( imgType ){
		case ImgType_jpeg:
			return LoadJpegFromMem(&p_bmp,(unsigned char*)p_pMem,p_nLenth)!=0;
			break;
		case ImgType_png:
			p_bmp.m_pImageData = (char*)ReadPNGFromMem((unsigned char*)p_pMem, p_nLenth, (PNG_UINT32*)&p_bmp.m_nWidth, (PNG_UINT32*)&p_bmp.m_nHeight);
			return p_bmp.m_pImageData!=0;
			break;
        case ImgType_gif:
            return LoadGif(&p_bmp, (unsigned char*)p_pMem, p_nLenth) != 0;
            break;
        case ImgType_unknow:
        default:
            break;
		}
		return false;
	}
    BitmapData loadLocalImageSync( const char* p_pszFile ){
        BitmapData bmp;
        JCBuffer buf;
        readFileSync(p_pszFile,buf);
        bool b = loadImageMemSync(buf.m_pPtr, buf.m_nLen, bmp);
        if(!b){
            bmp.m_pImageData=NULL;
        }
        return bmp;
    }
	void _AsyncLoadImage(std::shared_ptr<char> p_pBuff, int p_nLenth, imgDecodeCB p_CB){
		std::shared_ptr<char> pMem = p_pBuff;
        BitmapData bmp;
		bool b = loadImageMemSync(pMem.get(), p_nLenth, bmp );
		if( b ){
			p_CB(bmp);
		}else{
			if(bmp.m_pImageData ) delete [] bmp.m_pImageData ;
			bmp.m_pImageData = 0;
			p_CB(bmp);
		}
	}
	void loadImageMemASync(std::shared_ptr<char> p_pBuff, int p_nLenth, imgDecodeCB p_CB){
		g_DecThread->post( std::bind(_AsyncLoadImage, p_pBuff, p_nLenth,p_CB ) );
	}
	ImageType getImgType( const char* p_pMem, int p_nLength ){
		static int jpegID = 0x00ffd8ff;	//最高位00那个位置不一定是什么
		static int gifID = 0x38464947;
		static int pngID = 0x474e5089;

		int idval = *(int*)p_pMem;
		if( idval == pngID )return ImgType_png;
		else if( idval==gifID ) return ImgType_gif;
		else if( (idval &0xffffff) == jpegID ) return ImgType_jpeg;
		return ImgType_unknow;
	}
	bool getImageBaseInfo( const char* p_pMem, int p_nLength, ImageBaseInfo& p_Info ){
		return true;
	}
    extern int SavePng(char* png_file_name, char **data, int width, int height, int bit_depth);
	bool saveAsPng(const char* p_pData, int w, int h, const char* p_pszFile ){
        char** pngline = new char*[h];
        for( int i=0; i<h; i++){
            pngline[i]=(char*)(p_pData+w*4*i);
        }
        SavePng((char*)p_pszFile, pngline, w, h, 8);
        delete [] pngline;
		return true;
	}
	bool saveAsJpeg(const char* p_pData, ImageBaseInfo& p_Info, const char* p_pszFile ){
		char* pData = (char*)p_pData;
		int width = p_Info.m_nWidth;
		int height = p_Info.m_nHeight;
		int bpp = p_Info.m_nBpp;

		//32转24
		if( bpp==32){
			pData = new char[width*height*3];
			char* pSrc = (char*)p_pData;
			char* pDst = pData;
			for( int y=0; y<height; y++){
				for(int x=0; x<width; x++){
					pDst[0]=pSrc[0];pDst[1]=pSrc[1];pDst[2]=pSrc[2];
					pSrc+=4;
					pDst+=3;
				}
			}
		}
		saveJpeg(width, height, 24,(char*)pData, 80, p_pszFile);
		if( bpp==32)
			delete [] pData;

		return false;
	}
	bool saveAsBmp(const char* p_pData, ImageBaseInfo& p_Info){
		return false;
	}
	//把一个浮点数转换成分数。注意现在是用遍历的方法，速度有限，且只处理到分母为2048
	void getFraction(float v,int& a, int& b){
		float e=1e-4f;
		for(int i=1; i<2048; i++){
			float f1 = i*v;
			float ff = f1-(int)f1;
			if(f1-(int)f1<e){
				b=i;
				a=(int)(b*v+e);
				break;
			}
		}
	}
	class RGBAColor
    {
	public:
		RGBAColor(){
			fa=fr=fg=fb=0.0f;
		}
		RGBAColor(int color){
			set(color);
		}
		RGBAColor(const RGBAColor& color){
			fa=color.fa;fr=color.fr;fg=color.fg;fb=color.fb;
		}
		void operator += (const RGBAColor& b){
			fa+=b.fa;
			fr+=b.fr;
			fg+=b.fg;
			fb+=b.fb;
		}
		int toInt32(){
			int ret=0;
			unsigned char* pRGBA = (unsigned char*)&ret;
			unsigned char& a = pRGBA[0];
			unsigned char& r = pRGBA[1];
			unsigned char& g = pRGBA[2];
			unsigned char& b = pRGBA[3];

			a=(unsigned char)(fa*255); a=a>255?255:a;
			r=(unsigned char)(fr*255); r=r>255?255:r;
			g=(unsigned char)(fg*255); g=g>255?255:g;
			b=(unsigned char)(fb*255); b=b>255?255:b;
			return ret;
		}
		void set(int color){
			unsigned char* pRGBA = (unsigned char*)&color;
			fa = pRGBA[0]/255.0f;
			fr = pRGBA[1]/255.0f;
			fg = pRGBA[2]/255.0f;
			fb = pRGBA[3]/255.0f;
		}
		void clear(){
			fa=fr=fg=fb=0.0f;
		}
		RGBAColor operator *(float f){
			RGBAColor ret;
			ret.fa=fa*f;
			ret.fr=fr*f;
			ret.fg=fg*f;
			ret.fb=fb*f;
			return ret;
		}
		void operator *=(float f){
			fa*=f;fr*=f;fg*=f;fb*=f;
		}
	private:
		float fa,fr,fg,fb;
	};
	//求一条线的颜色的和
	RGBAColor _getLineColor(int* pData, int colorNum, float stk, float midk, float edk){
		RGBAColor res;
		res+=RGBAColor(*pData++)*stk;
		RGBAColor resmid;
		for(int i=0; i<colorNum-2; i++) resmid+=RGBAColor(*pData++);
		res+=resmid*midk;
		res+=RGBAColor(*pData++)*edk;
		return res;
	}
	//fsx,fsy。这种方法只适用于w>1,h>1的情况，即缩小图片。而且也不是很精确，没有考虑插值。
	RGBAColor _getBmpRectColor(BitmapData& bmp,float fsx, float fsy, float w, float h ){
		if(w<1.0f||h<1.0f){
			throw -1;
		}
		RGBAColor res;
		float dstposx=0.0f;
		float dstposy=0.0f;
		int sx=(int)fsx;
		int sy=(int)fsy;
		int ex=(int)ceil(fsx+w); ex=ex>bmp.m_nWidth?bmp.m_nWidth:ex;
		int ey=(int)ceil(fsy+h); ey=ey>bmp.m_nHeight?bmp.m_nHeight:ey;
		//TODO 对于整幅图像的缩小，下面几个值每个目标像素都要重算一遍，效率低
		float fracfsx = fsx-sx;
		float fracfsy = fsy-sy;
		float sxk = fracfsx==0.0f?1:(1.0f-fracfsx);
		float syk = fracfsy==0.0f?1:(1.0f-fracfsy);
		float exk = (fsx+w)-(ex-1); if(exk<0)exk=1.0f;
		float eyk = (fsy+h)-(ey-1); if(eyk<0)eyk=1.0f;
		int rectw = ex-sx;
		int recth = ey-sy;
		if(recth<=0) return res;
		int y=sy;
		int* pCurData = ((int*)bmp.m_pImageData)+sy*bmp.m_nWidth+sx;
		//第一行
		res += _getLineColor(pCurData,rectw,sxk*syk,syk,exk*syk);
		y++;
		pCurData+=bmp.m_nWidth;
		//中间行
		for( ; y<ey-1; y++){
			res += _getLineColor(pCurData,rectw,sxk,1,exk);
			pCurData+=bmp.m_nWidth;
		}
		//最后一行
		if(y<ey)
			res += _getLineColor(pCurData, rectw, sxk*eyk,eyk,exk*eyk);
		return res;
	}
	bool downsampleBmp( BitmapData& src, BitmapData& dst, int* pFrcation, bool rbBorder ){
		int& wa=pFrcation[0];
		int& wb=pFrcation[1];
		int& ha=pFrcation[2];
		int& hb=pFrcation[3];
		if(wa==wb&&ha==hb){
			//先不处理不变的情况
			throw -1;
			return false;
		}
		if( wa>wb || ha>hb ){
			//不支持放大
			throw -1;
			return false;
		}
		int& srcw = src.m_nWidth;
		int& srch = src.m_nHeight;
		int& dstw = dst.m_nWidth;
		int& dsth = dst.m_nHeight;
		dstw = (int)ceil((float)srcw*wa/wb);
		dsth = (int)ceil((float)srch*ha/hb);
		int borderAdd=0;
		if(rbBorder){
			dstw++;
			dsth++;
			borderAdd=1;
		}
		//实际图片
		dst.m_pImageData = new char[dstw*dsth*4];
#ifdef _DEBUG
		memset(dst.m_pImageData,0x86,dstw*dsth*4);
#endif
		dst.m_nBpp = 32;
		int* pdst = (int*)dst.m_pImageData;
		float fsx=0.0f,fsy=0.0f,fw=(float)wb/wa,fh=(float)hb/ha;
		RGBAColor res;
		for(int y=0; y<dsth; y++){
			fsx=0;
			for(int x=0; x<dstw; x++){
				res = _getBmpRectColor(src,fsx,fsy,fw,fh);
				res *= (((float)wa*ha)/(wb*hb));
				*pdst++=res.toInt32();
				fsx+=fw;
			}
			fsy+=fh;
		}
        return true;
		/*
		int stx = 0;
		//行
		RGBAColor res,cur;
		float dsrc = ((float)wa)/wb;//源每前进一步，对应新的步长
		int* pcurSrc=(int*)src.m_pImageData;
		for( int y=0; y<dsth; y++){
			int num = dstw/srcw;
			for(int x=0; x<num; x++,stx+=wb){
				float fcurpos=0.0f;
				float vk=1.0f;
				for( int cx=stx; cx<stx+wb; cx++){
					//取一个颜色
					cur.set(*pcurSrc++);
					//这个颜色可能全给新的像素了，也可能只给了一部分
					float d1 = 1.0f-fcurpos;
					if(d1>dsrc){//全部
						res+=cur;
					}else{//部分
						vk = d1/dsrc;//一定<1
						res+=(cur*vk);
					}
					fcurpos+=dsrc;
				}
			}
			//剩下的
			int left = dstw%srcw;
		}
		//列
		*/
	}
	bool downsampleBmp( BitmapData& src, BitmapData& dst, int windw, int windh, bool rbBorder ){
		if( src.m_pImageData==NULL)		return false;
		if(windw<1 || windh<1)return false;
		if(windw==1 && windh==1){
			//这个怎么处理.这么用是浪费的
			throw -1;
		}
		int& srcw = src.m_nWidth;
		int& srch = src.m_nHeight;
		int& dstw = dst.m_nWidth;
		int& dsth = dst.m_nHeight;
		//取整。可能会丢掉一部分
		dstw = srcw/windw;	
		dsth = srch/windh;
		//TODO
		//采用先水平扫描的方法，这样可能会快一些
		int borderAdd=0;
		if(rbBorder){
			dstw++;
			dsth++;
			borderAdd=1;
		}
		//实际图片
		dst.m_pImageData = new char[dstw*dsth*4];
		dst.m_nBpp = 32;
		int snum=windw*windh;
		int* psrc = (int*)src.m_pImageData;
		int* pdst = (int*)dst.m_pImageData;
		for( int y=0; y<dsth-borderAdd; y++ ){
			int* pcurdst=pdst;
			for( int x=0; x<dstw-borderAdd; x++ ){
				unsigned int valr = 0,valg=0,valb=0,vala=0;
				unsigned char* pcurc = (unsigned char*)&psrc[x*windw + (y*windh)*srcw];
				//下面开始处理一个块
				for(int sy=0;sy<windh; sy++){
					unsigned char* tmpSrc=pcurc;
					for(int sx=0; sx<windw; sx++){
						valr+=*tmpSrc++; valg+=*tmpSrc++; valb+=*tmpSrc++; vala+=*tmpSrc++;
					}
					//下一行的块开头
					pcurc += srcw*4;
				}
				valr/=snum; valg/=snum; valb/=snum; vala/=snum;
				unsigned char avgval[]={static_cast<unsigned char>(valr),static_cast<unsigned char>(valg),static_cast<unsigned char>(valb),static_cast<unsigned char>(vala)};
				*pcurdst++=*(int*)avgval;
			}
			//水平复制最后一个
			if(borderAdd){
				int lastv = *(pcurdst-1);
				*pcurdst++ = lastv;
			}
			pdst+=dstw;	//这个要实际到下一行，考虑包边
		}
		//复制最后一行
		if(borderAdd){
			int* pcurimg = (int*)dst.m_pImageData;
			memcpy(pcurimg+dstw*(dsth-1),pcurimg+dstw*(dsth-2), dstw*sizeof(int));
		}
		return true;
	}
	void createGridBmp( BitmapData& out, int w, int h, int gridw){
		out.m_nBpp=32;
		out.m_nWidth=w;
		out.m_nHeight=h;
		out.m_pImageData=(char*)new int[w*h];
		int* pData = (int*)out.m_pImageData;
		for(int y=0;y<h;y++){
			for(int x=0; x<w; x++){
				int v = (((x/gridw)%2)^((y/gridw)%2))?(0xffffffff):(0x77777777);
				*pData++=v;
			}
		}
	}
    void copy8BitBmp( BitmapData& dst, int nSx, int nSy, unsigned char* pSrc, int nSrcW, int nSrcH, int nSrcColor ){
        int srcsx=0,srcsy=0;
        int dstsx=nSx,dstsy=nSy;
        int dtw=nSrcW,dth=nSrcH;
        if(dstsx<0){
            srcsx = -dstsx;
            dtw+=dstsx;
            dstsx=0;
        }
        if( dstsy<0){
            srcsy = -dstsy;
            dth+=dstsy;
            dstsy=0;
        }
        int ex = dstsx+dtw;
        int ey = dstsy+dth;
        if(ex>dst.m_nWidth){
            dtw -= (ex - dst.m_nWidth);
        }
        if(ey>dst.m_nHeight){
            dth -= (ey - dst.m_nHeight);
        }
        unsigned int*  pdst = (unsigned int*)(dst.m_pImageData+(dstsy*dst.m_nWidth+dstsx)*4);
        unsigned char* psrc = pSrc+srcsy*nSrcW+srcsx;
        for( int y=0; y<dth; y++){
            unsigned int* pdstl=pdst;
            unsigned char* psrcl=psrc;
            for( int x=0; x<dtw; x++){
                unsigned char alpha = *pSrc++;
                int col = nSrcColor&0x00ffffff;
                col|=(alpha<<24);
                *pdstl++=col;
            }
            pdst+=dst.m_nWidth;
            psrc+=nSrcW;
        }
    }
    void copy32BitBmp(BitmapData& dst, int nSx, int nSy,int nDstW, unsigned char* pSrc, int nSrcW, int nSrcH, int srcPitch )
    {
        int srcsx = 0, srcsy = 0;
        int dstsx = nSx, dstsy = nSy;
        int dtw = nSrcW, dth = nSrcH;
        if (dstsx < 0) {
            srcsx = -dstsx;
            dtw += dstsx;
            dstsx = 0;
        }
        if (dstsy < 0) {
            srcsy = -dstsy;
            dth += dstsy;
            dstsy = 0;
        }
        int ex = dstsx + dtw;
        int ey = dstsy + dth;
        if (ex > nDstW) {
            dtw -= (ex - nDstW);
        }
        if (ey > dst.m_nHeight) {
            dth -= (ey - dst.m_nHeight);
        }
        if (dtw < 0) dtw = 0;
        char*  pdst = dst.m_pImageData + (dstsy*nDstW + dstsx) * 4;
        unsigned char* psrc = pSrc + srcsy*srcPitch + srcsx*4;
        for (int y = 0; y < dth; y++) {
            memcpy(pdst, psrc, dtw * 4);
            pdst += nDstW *4;
            psrc += srcPitch;
        }
    }
#endif
    void convertRGBA8888ToRGB888(const unsigned char* data, size_t dataLen, unsigned char* outData)
    {
        for (size_t i = 0, l = dataLen - 3; i < l; i += 4)
        {
            *outData++ = data[i]; 
            *outData++ = data[i + 1];
            *outData++ = data[i + 2];
        }
    }
    void convertRGBA8888ToA8(const unsigned char* data, size_t dataLen, unsigned char* outData)
    {
        for (size_t i = 0, l = dataLen - 3; i < l; i += 4)
        {
            *outData++ = data[i + 3];
        }
    }
    void convertRGBA8888ToFormat(const unsigned char* data, size_t dataLen, GLenum format, unsigned char** outData, size_t* outDataLen)
    {
        switch (format)
        {
        case GL_RGB:
            *outDataLen = dataLen / 4 * 3;
            *outData = new unsigned char[sizeof(unsigned char) * (*outDataLen)];
            convertRGBA8888ToRGB888(data, dataLen, *outData);
            break;
        case GL_ALPHA:
            *outDataLen = dataLen / 4;
            *outData = new unsigned char[sizeof(unsigned char) * (*outDataLen)];
            convertRGBA8888ToA8(data, dataLen, *outData);
            break;
        default:
            *outData = (unsigned char*)data;
            *outDataLen = dataLen;
        }
    }
}
//------------------------------------------------------------------------------

//-----------------------------END FILE--------------------------------
