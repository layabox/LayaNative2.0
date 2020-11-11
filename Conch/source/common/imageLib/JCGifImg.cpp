/**
@file			JCGifImg.cpp
@brief			
@author			James
@version		1.0
@date			2016_7_13
*/

#include "JCGifImg.h"
#include <string.h>
#include "../util/Log.h"
#include "JCImageRW.h"
#ifdef ANDROID
#include <malloc.h>
#endif

namespace laya
{
    //------------------------------------------------------------------------------
    GifLoader::GifLoader( void )
    {	
	    m_bOpened = false;
	    m_bError = false;
	    m_bInMem = true;
	    m_pBufferIOStream = NULL;
    }
    //------------------------------------------------------------------------------
    GifLoader::GifLoader( const char* p_sFileName,bool p_bInMem )
    {	
	    m_pBufferIOStream = NULL;
	    m_bOpened = false;
	    m_bError = false;
	    if( open( p_sFileName,p_bInMem ) )
	    {
		    m_bOpened = true;
		    m_bError = false;
	    }
	    else
	    {
		    m_bOpened = false;
		    m_bError = true;
	    }
    }
    //------------------------------------------------------------------------------
    GifLoader::GifLoader( unsigned char* p_pBuffer, int p_nBufferSize )
    {
	    m_bInMem = true;
	    m_bOpened = false;
	    m_bError = false;
	    m_pBufferIOStream = new BufferIOStream( (char*)p_pBuffer,p_nBufferSize );
	    open();
    }
    //------------------------------------------------------------------------------
    GifLoader::~GifLoader( void )
    {	
	    close();
	    if( m_pBufferIOStream )
	    {
		    delete m_pBufferIOStream;
		    m_pBufferIOStream = NULL;
	    }
    }
    //------------------------------------------------------------------------------
    bool GifLoader::operator!( void )
    {	
	    return m_bError;
    }
    //------------------------------------------------------------------------------
    bool GifLoader::open( void )
    {	
	    char cc[4];
	    unsigned char be = 0;
	    //bool fileEnd = false;
	    m_vAllFrames = NULL;
	    m_nCurIndex = 0;
	    m_kCurFrame.pColorTable = NULL;
	    m_kCurFrame.dataBuf = NULL;
	    m_kCtrlExt.active = false;

	    m_pBufferIOStream->read(cc,3);
	    if(strncmp(cc,"GIF",3) != 0)
	    {
		    goto m_bError;
	    }
	    m_pBufferIOStream->read(m_sVersion,3);
	    m_sVersion[3] = 0;
	    if(strncmp(m_sVersion,"89a",3) > 0)
	    {
		    goto m_bError;
	    }
	    m_pBufferIOStream->read((char*)&m_kGInfo.scrWidth,2);
	    m_pBufferIOStream->read((char*)&m_kGInfo.scrHeight,2);
	    if(!m_pBufferIOStream->good())
	    {
		    goto m_bError;
	    }
	    m_pBufferIOStream->read((char*)&be,1);
	    if((be&0x80) != 0)
	    {
		    m_kGInfo.gFlag = true;
	    }
	    else
	    {
		    m_kGInfo.gFlag = false;
	    }
	    m_kGInfo.colorRes = ( (be&0x70)>>4 ) + 1;
	    if( m_kGInfo.gFlag )
	    {	
		    if((be&0x08) != 0)
		    {
			    m_kGInfo.gSort = true;
		    }
		    else
		    {
			    m_kGInfo.gSort = false;
		    }
		    m_kGInfo.gSize = 1;
		    m_kGInfo.gSize <<= ((be&0x07)+1);
	    }
	    m_pBufferIOStream->read((char*)&be,1);
	    m_kGInfo.BKColorIdx = be;
	    m_pBufferIOStream->read((char*)&be,1);
	    m_kGInfo.pixelAspectRatio = be;
	    if(!m_pBufferIOStream->good())
	    {
		    goto m_bError;
	    }
	    if(m_kGInfo.gFlag)
	    {	m_pBufferIOStream->read((char*)m_vGColorTable,m_kGInfo.gSize*3);
		    m_kGInfo.gColorTable = m_vGColorTable;
	    }
	    else
	    {
		    m_kGInfo.gColorTable = NULL;
	    }
	    m_kDataStart = m_pBufferIOStream->tellg();
	    if((m_kGInfo.frames = checkFrames(*m_pBufferIOStream)) == 0)
	    {
		    goto m_bError;
	    }
	    if(m_bInMem)
	    {
		    if((m_vAllFrames = new FRAME[m_kGInfo.frames]) == NULL)
		    {
			    goto m_bError;
		    }
		    memset( m_vAllFrames,0,sizeof(FRAME)*m_kGInfo.frames );
		    //ZeroMemory(m_vAllFrames,sizeof(FRAME)*m_kGInfo.frames);
		    if(!getAllFrames( *m_pBufferIOStream ))
		    {
			    delete[] m_vAllFrames;
			    m_vAllFrames = NULL;
			    goto m_bError;
		    }
	    }
	    return true;
    m_bError:
	    m_pBufferIOStream->close();
	    return false;
    }
    //------------------------------------------------------------------------------
    bool GifLoader::open( const char* p_sFileName,bool p_bInMem )
    {
	    char cc[4];
	    unsigned char be;
	    //bool fileEnd = false;
	    m_bInMem = p_bInMem;
	    m_vAllFrames = NULL;
	    m_nCurIndex = 0;
	    m_kCurFrame.pColorTable = NULL;
	    m_kCurFrame.dataBuf = NULL;
	    m_kCtrlExt.active = false;
	    if(m_bOpened)
	    {
		    return false;
	    }
	    m_kIOStream.open( p_sFileName,ios::binary );
	    if( !m_kIOStream )
	    {
		    return false;
	    }
	    m_kIOStream.read(cc,3);
	    if(strncmp(cc,"GIF",3) != 0)
	    {
		    goto m_bError;
	    }
	    m_kIOStream.read(m_sVersion,3);
	    m_sVersion[3] = 0;
	    if(strncmp(m_sVersion,"89a",3) > 0)
	    {
		    goto m_bError;
	    }
	    m_kIOStream.read((char*)&m_kGInfo.scrWidth,2);
	    m_kIOStream.read((char*)&m_kGInfo.scrHeight,2);
	    if(!m_kIOStream.good())
	    {
		    goto m_bError;
	    }
	    m_kIOStream.read((char*)&be,1);
	    if((be&0x80) != 0)
	    {
		    m_kGInfo.gFlag = true;
	    }
	    else
	    {
		    m_kGInfo.gFlag = false;
	    }
	    m_kGInfo.colorRes = ( (be&0x70)>>4 ) + 1;
	    if( m_kGInfo.gFlag )
	    {
		    if((be&0x08) != 0)
		    {
			    m_kGInfo.gSort = true;
		    }
		    else
		    {
			    m_kGInfo.gSort = false;
		    }
		    m_kGInfo.gSize = 1;
		    m_kGInfo.gSize <<= ((be&0x07)+1);
	    }
	    m_kIOStream.read((char*)&be,1);
	    m_kGInfo.BKColorIdx = be;
	    m_kIOStream.read((char*)&be,1);
	    m_kGInfo.pixelAspectRatio = be;
	    if(!m_kIOStream.good())
	    {
		    goto m_bError;
	    }
	    if(m_kGInfo.gFlag)
	    {	m_kIOStream.read((char*)m_vGColorTable,m_kGInfo.gSize*3);
		    m_kGInfo.gColorTable = m_vGColorTable;
	    }
	    else
	    {
		    m_kGInfo.gColorTable = NULL;
	    }
	    m_kDataStart = m_kIOStream.tellg();
	    if((m_kGInfo.frames = checkFrames(m_kIOStream)) == 0)
	    {
		    goto m_bError;
	    }
	    if(m_bInMem)
	    {
		    if((m_vAllFrames = new FRAME[m_kGInfo.frames]) == NULL)
		    {
			    goto m_bError;
		    }
		    memset( m_vAllFrames,0,sizeof(FRAME)*m_kGInfo.frames );
		    //ZeroMemory(m_vAllFrames,sizeof(FRAME)*m_kGInfo.frames);
		    if(!getAllFrames(m_kIOStream))
		    {
			    delete[] m_vAllFrames;
			    m_vAllFrames = NULL;
			    goto m_bError;
		    }
		    m_kIOStream.close();
	    }
	    m_bOpened = true;
	    return true;
    m_bError:
	    m_kIOStream.close();
	    return false;
    }
    //------------------------------------------------------------------------------
    unsigned int GifLoader::checkFrames( ifstream& p_kIOStream )
    {	
	    unsigned char be;
	    bool fileEnd = false;
	    unsigned int frames=0;
	    streampos pos = p_kIOStream.tellg();
	    while(!p_kIOStream.eof() && !fileEnd)
	    {	
		    p_kIOStream.read((char*)&be,1);
		    switch(be)
		    {	
		    case 0x21:
			    p_kIOStream.read((char*)&be,1);
			    switch(be)
			    {	
			    case 0xf9:
			    case 0xfe:
			    case 0x01:
			    case 0xff:
				    while(!p_kIOStream.eof())
				    {	p_kIOStream.read((char*)&be,1);
					    if(be == 0)
						    break;
					    p_kIOStream.seekg(be,ios::cur);
				    }
				    break;
			    default:
				    return 0;
			    }
			    break;
		    case 0x2c:
		    {	
			    unsigned char bp;
			    bool lFlag=false;
			    unsigned int lSize=1;
			    frames++;
			    p_kIOStream.seekg(8,ios::cur);
			    p_kIOStream.read((char*)&bp,1);
			    if((bp&0x80) != 0)
				    lFlag = true;
			    lSize <<= ((bp&0x07)+1);
			    if(lFlag)
				    p_kIOStream.seekg(lSize*3,ios::cur);
			    if(!p_kIOStream.good())
				    return 0;
			    p_kIOStream.read((char*)&be,1);
			    while(!p_kIOStream.eof())
			    {	p_kIOStream.read((char*)&be,1);
				    if(be == 0)
					    break;
				    p_kIOStream.seekg(be,ios::cur);
			    }
			    break;
		    }
		    case 0x3b:
			    fileEnd = true;
			    break;
		    case 0x00:
			    break;
		    default:
			    return 0;
		    }
	    }
	    p_kIOStream.seekg(pos);
	    return frames;
    }
    //------------------------------------------------------------------------------
    unsigned int GifLoader::checkFrames( BufferIOStream& p_kIOStream )
    {
	    unsigned char be = 0;
	    bool fileEnd = false;
	    unsigned int frames=0;
	    streampos pos = p_kIOStream.tellg();
	    while(!p_kIOStream.eof() && !fileEnd)
	    {
		    p_kIOStream.read((char*)&be,1);
		    switch(be)
		    {
		    case 0x21:
			    p_kIOStream.read((char*)&be,1);
			    switch(be)
			    {
			    case 0xf9:
			    case 0xfe:
			    case 0x01:
			    case 0xff:
				    while(!p_kIOStream.eof())
				    {	p_kIOStream.read((char*)&be,1);
					    if(be == 0)
						    break;
					    p_kIOStream.seekg(be,ios::cur);
				    }
				    break;
			    default:
				    return 0;
			    }
			    break;
		    case 0x2c:
		    {
			    unsigned char bp = 0;
			    bool lFlag=false;
			    unsigned int lSize=1;
			    frames++;
			    p_kIOStream.seekg(8,ios::cur);
			    p_kIOStream.read((char*)&bp,1);
			    if((bp&0x80) != 0)
				    lFlag = true;
			    lSize <<= ((bp&0x07)+1);
			    if(lFlag)
				    p_kIOStream.seekg(lSize*3,ios::cur);
			    if(!p_kIOStream.good())
				    return 0;
			    p_kIOStream.read((char*)&be,1);
			    while(!p_kIOStream.eof())
			    {	p_kIOStream.read((char*)&be,1);
				    if(be == 0)
					    break;
				    p_kIOStream.seekg(be,ios::cur);
			    }
			    break;
		    }
		    case 0x3b:
			    fileEnd = true;
			    break;
		    case 0x00:
			    break;
		    default:
			    return 0;
		    }
	    }
	    p_kIOStream.seekg((int)pos);
	    return frames;
    }
    //------------------------------------------------------------------------------
    bool GifLoader::getAllFrames(ifstream& p_kIOStream)
    {	
	    unsigned char be;
	    bool fileEnd = false;
	    FRAME *pf = m_vAllFrames;
	    streampos pos = p_kIOStream.tellg();
	    int i;
	    while(!p_kIOStream.eof() && !fileEnd)
	    {	
		    p_kIOStream.read((char*)&be,1);
		    switch(be)
		    {	case 0x21:
				    p_kIOStream.read((char*)&be,1);
				    switch(be)
				    {	case 0xf9:
						    while(!p_kIOStream.eof())
						    {	p_kIOStream.read((char*)&be,1);
							    if(be == 0)
								    break;
							    if(be == 4)
							    {	m_kCtrlExt.active = true;
								    p_kIOStream.read((char*)&be,1);
								    m_kCtrlExt.disposalMethod = (be&0x1c)>>2;
								    if((be&0x02) != 0)
									    m_kCtrlExt.userInputFlag = true;
								    else
									    m_kCtrlExt.userInputFlag = false;
								    if((be&0x01) != 0)
									    m_kCtrlExt.trsFlag = true;
								    else
									    m_kCtrlExt.trsFlag = false;
								    p_kIOStream.read((char*)&m_kCtrlExt.delayTime,2);
								    p_kIOStream.read((char*)&be,1);
								    m_kCtrlExt.trsColorIndex = be;
							    }
							    else
								    p_kIOStream.seekg(be,ios::cur);
						    }
						    break;
					    case 0xfe:
					    case 0x01:
					    case 0xff:
						    while(!p_kIOStream.eof())
						    {	p_kIOStream.read((char*)&be,1);
							    if(be == 0)
								    break;
							    p_kIOStream.seekg(be,ios::cur);
						    }
						    break;
					    default:
						    goto m_bError;
				    }
				    break;
			    case 0x2c:
			    {	unsigned char bp;
				    p_kIOStream.read((char*)&pf->imageLPos,2);
				    p_kIOStream.read((char*)&pf->imageTPos,2);
				    p_kIOStream.read((char*)&pf->imageWidth,2);
				    p_kIOStream.read((char*)&pf->imageHeight,2);
				    p_kIOStream.read((char*)&bp,1);
				    if((bp&0x80) != 0)
					    pf->lFlag = true;
				    if((bp&0x40) != 0)
					    pf->interlaceFlag = true;
				    if((bp&0x20) != 0)
					    pf->sortFlag = true;
				    pf->lSize = 1;
				    pf->lSize <<= ((bp&0x07)+1);
				    if(pf->lFlag)
				    {	if((pf->pColorTable = new unsigned char[pf->lSize*3]) == NULL)
						    goto m_bError;
					    p_kIOStream.read((char*)pf->pColorTable,pf->lSize*3);
				    }
				    if(!p_kIOStream.good())
					    goto m_bError;
				    if(!extractData(pf,p_kIOStream))
					    goto m_bError;
				    if(m_kCtrlExt.active)
				    {	pf->ctrlExt = m_kCtrlExt;
					    m_kCtrlExt.active = false;
				    }
				    pf++;
				    break;
			    }
			    case 0x3b:
				    fileEnd = true;
				    break;
			    case 0x00:
				    break;
			    default:
				    goto m_bError;
		    }
	    }
	    p_kIOStream.seekg(pos);
	    return true;
    m_bError:
	    pf = m_vAllFrames;
	    for(i=0;i<(int)m_kGInfo.frames;i++)
	    {	if(pf->pColorTable != NULL)
		    {	delete[] pf->pColorTable;
			    pf->pColorTable = NULL;
		    }
		    if(pf->dataBuf != NULL)
		    {	delete[] pf->dataBuf;
			    pf->dataBuf = NULL;
		    }
	    }
	    return false;
    }
    //------------------------------------------------------------------------------
    bool GifLoader::getAllFrames( BufferIOStream& p_kIOStream )
    {
	    unsigned char be = 0;
	    bool fileEnd = false;
	    FRAME *pf = m_vAllFrames;
	    streampos pos = p_kIOStream.tellg();
	    int i;
	    while(!p_kIOStream.eof() && !fileEnd)
	    {
		    p_kIOStream.read((char*)&be,1);
		    switch(be)
		    {	case 0x21:
				    p_kIOStream.read((char*)&be,1);
				    switch(be)
				    {	case 0xf9:
						    while(!p_kIOStream.eof())
						    {	p_kIOStream.read((char*)&be,1);
							    if(be == 0)
								    break;
							    if(be == 4)
							    {	m_kCtrlExt.active = true;
								    p_kIOStream.read((char*)&be,1);
								    m_kCtrlExt.disposalMethod = (be&0x1c)>>2;
								    if((be&0x02) != 0)
									    m_kCtrlExt.userInputFlag = true;
								    else
									    m_kCtrlExt.userInputFlag = false;
								    if((be&0x01) != 0)
									    m_kCtrlExt.trsFlag = true;
								    else
									    m_kCtrlExt.trsFlag = false;
								    p_kIOStream.read((char*)&m_kCtrlExt.delayTime,2);
								    p_kIOStream.read((char*)&be,1);
								    m_kCtrlExt.trsColorIndex = be;
							    }
							    else
								    p_kIOStream.seekg(be,ios::cur);
						    }
						    break;
					    case 0xfe:
					    case 0x01:
					    case 0xff:
						    while(!p_kIOStream.eof())
						    {	p_kIOStream.read((char*)&be,1);
							    if(be == 0)
								    break;
							    p_kIOStream.seekg(be,ios::cur);
						    }
						    break;
					    default:
						    goto m_bError;
				    }
				    break;
			    case 0x2c:
			    {	unsigned char bp = 0;
				    p_kIOStream.read((char*)&pf->imageLPos,2);
				    p_kIOStream.read((char*)&pf->imageTPos,2);
				    p_kIOStream.read((char*)&pf->imageWidth,2);
				    p_kIOStream.read((char*)&pf->imageHeight,2);
				    p_kIOStream.read((char*)&bp,1);
				    if((bp&0x80) != 0)
					    pf->lFlag = true;
				    if((bp&0x40) != 0)
					    pf->interlaceFlag = true;
				    if((bp&0x20) != 0)
					    pf->sortFlag = true;
				    pf->lSize = 1;
				    pf->lSize <<= ((bp&0x07)+1);
				    if(pf->lFlag)
				    {	if((pf->pColorTable = new unsigned char[pf->lSize*3]) == NULL)
						    goto m_bError;
					    p_kIOStream.read((char*)pf->pColorTable,pf->lSize*3);
				    }
				    if(!p_kIOStream.good())
					    goto m_bError;
				    if(!extractData(pf,p_kIOStream))
					    goto m_bError;
				    if(m_kCtrlExt.active)
				    {	pf->ctrlExt = m_kCtrlExt;
					    m_kCtrlExt.active = false;
				    }
				    pf++;
				    break;
			    }
			    case 0x3b:
				    fileEnd = true;
				    break;
			    case 0x00:
				    break;
			    default:
				    goto m_bError;
		    }
	    }
	    p_kIOStream.seekg((int)pos);
	    return true;
    m_bError:
	    pf = m_vAllFrames;
	    for(i=0;i<(int)m_kGInfo.frames;i++)
	    {	if(pf->pColorTable != NULL)
		    {	delete[] pf->pColorTable;
			    pf->pColorTable = NULL;
		    }
		    if(pf->dataBuf != NULL)
		    {	delete[] pf->dataBuf;
			    pf->dataBuf = NULL;
		    }
	    }
	    return false;
    }
    //------------------------------------------------------------------------------
    LPCFRAME GifLoader::getNextFrame()
    {	
	    if(m_bInMem)
	    {	
		    FRAME* p =  m_vAllFrames+m_nCurIndex;
		    m_nCurIndex++;
		    if(m_nCurIndex >= m_kGInfo.frames)
			    m_nCurIndex = 0;
		    return p;
	    }
	    else
	    {	
		    unsigned char be;
		    //bool fileEnd = false;
		    if(m_kCurFrame.pColorTable != NULL)
		    {	delete[] m_kCurFrame.pColorTable;
			    m_kCurFrame.pColorTable = NULL;
		    }
		    if(m_kCurFrame.dataBuf != NULL)
		    {	delete[] m_kCurFrame.dataBuf;
			    m_kCurFrame.dataBuf = NULL;
		    }
		    memset( &m_kCurFrame,0,sizeof(FRAME) );
		    while(true)
		    {	m_kIOStream.read((char*)&be,1);
			    switch(be)
			    {	case 0x21:
					    m_kIOStream.read((char*)&be,1);
					    switch(be)
					    {	case 0xf9:
							    while(!m_kIOStream.eof())
							    {	m_kIOStream.read((char*)&be,1);
								    if(be == 0)
									    break;
								    if(be == 4)
								    {	m_kCtrlExt.active = true;
									    m_kIOStream.read((char*)&be,1);
									    m_kCtrlExt.disposalMethod = (be&0x1c)>>2;
									    if((be&0x02) != 0)
										    m_kCtrlExt.userInputFlag = true;
									    else
										    m_kCtrlExt.userInputFlag = false;
									    if((be&0x01) != 0)
										    m_kCtrlExt.trsFlag = true;
									    else
										    m_kCtrlExt.trsFlag = false;
									    m_kIOStream.read((char*)&m_kCtrlExt.delayTime,2);
									    m_kIOStream.read((char*)&be,1);
									    m_kCtrlExt.trsColorIndex = be;
								    }
								    else
									    m_kIOStream.seekg(be,ios::cur);
							    }
							    break;
						    case 0xfe:
						    case 0x01:
						    case 0xff:
							    while(!m_kIOStream.eof())
							    {	m_kIOStream.read((char*)&be,1);
								    if(be == 0)
									    break;
								    m_kIOStream.seekg(be,ios::cur);
							    }
							    break;
						    default:
							    goto m_bError;
					    }
					    break;
				    case 0x2c:
				    {	unsigned char bp;
					    m_kIOStream.read((char*)&m_kCurFrame.imageLPos,2);
					    m_kIOStream.read((char*)&m_kCurFrame.imageTPos,2);
					    m_kIOStream.read((char*)&m_kCurFrame.imageWidth,2);
					    m_kIOStream.read((char*)&m_kCurFrame.imageHeight,2);
					    m_kIOStream.read((char*)&bp,1);
					    if((bp&0x80) != 0)
						    m_kCurFrame.lFlag = true;
					    if((bp&0x40) != 0)
						    m_kCurFrame.interlaceFlag = true;
					    if((bp&0x20) != 0)
						    m_kCurFrame.sortFlag = true;
					    m_kCurFrame.lSize = 1;
					    m_kCurFrame.lSize <<= ((bp&0x07)+1);
					    if((m_kCurFrame.pColorTable = new unsigned char[m_kCurFrame.lSize*3]) == NULL)
						    goto m_bError;
					    if(m_kCurFrame.lFlag)
						    m_kIOStream.read((char*)m_kCurFrame.pColorTable,m_kCurFrame.lSize*3);
					    if(!m_kIOStream.good())
						    goto m_bError;
					    if(!extractData(&m_kCurFrame,m_kIOStream))
						    goto m_bError;
					    m_kCurFrame.ctrlExt = m_kCtrlExt;
					    if(m_kCtrlExt.active == true)
						    m_kCtrlExt.active = false;
					    return &m_kCurFrame;
				    }
				    case 0x3b:
					    m_kIOStream.seekg(m_kDataStart);
					    break;
				    case 0x00:
					    break;
				    default:
					    goto m_bError;
			    }
		    }	
		    return &m_kCurFrame;
    m_bError:
		    if(m_kCurFrame.pColorTable != NULL)
		    {	delete[] m_kCurFrame.pColorTable;
			    m_kCurFrame.pColorTable = NULL;
		    }
		    if(m_kCurFrame.dataBuf != NULL)
		    {	delete[] m_kCurFrame.dataBuf;
			    m_kCurFrame.dataBuf = NULL;
		    }
		    return NULL;
	    }
    }
    //------------------------------------------------------------------------------
    bool GifLoader::initStrTable(STRING_TABLE_ENTRY* strTable,unsigned int rootSize)
    {	
	    unsigned int i;
	    unsigned char *cc;
	    for(i=0;i<rootSize;i++)
	    {	if((cc = new unsigned char[2]) == NULL)
			    goto m_bError;
		    cc[0] = i,cc[1] = 0;
		    strTable[i].p = cc;
		    strTable[i].len = 1;
	    }
	    return true;
    m_bError:
	    for(i=0;i<rootSize;i++)
		    if(strTable[i].p != NULL)
		    {	delete[] strTable[i].p;
			    strTable[i].p = NULL;
		    }
	    return false;
    }
    //------------------------------------------------------------------------------
    bool GifLoader::addStrTable(STRING_TABLE_ENTRY* strTable,unsigned int addIdx,unsigned int idx,unsigned char c)
    {	
	    unsigned char *cc;
	    unsigned int l = strTable[idx].len;
	    if(addIdx >= 4096)
		    return false;
	    if((cc = new unsigned char[l+2]) == NULL)
		    return false;
	    for(unsigned int i=0;i<l;i++)
		    cc[i] = strTable[idx].p[i];
	    cc[l] = c;
	    cc[l+1] = 0;
	    strTable[addIdx].p = cc;
	    strTable[addIdx].len = strTable[idx].len +1;
	    return true;
    }
    //------------------------------------------------------------------------------
    bool GifLoader::extractData(FRAME* f,ifstream& p_kIOStream)
    {	
	    STRING_TABLE_ENTRY *strTable;
	    unsigned int codeSize,rootSize,tableIndex,codeSizeBK;
	    int remainInBuf = 0,i;
	    unsigned int bufIndex = 0,outIndex = 0;
	    unsigned int bitIndex = 0;
	    unsigned int code,oldCode;
	    unsigned char be,*outP;
	    unsigned char buf[262];
	    bool readOK = false;
	    unsigned int bufLen = f->imageWidth*f->imageHeight;
	    if((strTable = new STRING_TABLE_ENTRY[4096]) == NULL)
		    return false;
	    memset( strTable,0,sizeof(STRING_TABLE_ENTRY)*4096 );
	    outP = f->dataBuf = new unsigned char[bufLen];
	    if(f->dataBuf == NULL)
	    {	delete[] strTable;
		    return false;
	    }
	    p_kIOStream.read((char*)&be,1);
	    codeSizeBK = codeSize = be+1;
	    rootSize = 1;
	    rootSize <<= be; 
	    tableIndex = rootSize+2;
	    if(!initStrTable(strTable,rootSize))
		    goto m_bError;

    begin:
	    if(remainInBuf<=4 && !readOK)
	    {	for(i=0;i<remainInBuf;i++)
			    buf[i] = buf[bufIndex+i];
		    bufIndex = 0;
		    p_kIOStream.read((char*)&be,1);
		    if(be != 0)
		    {	p_kIOStream.read((char*)(buf+remainInBuf),be);
			    remainInBuf += be;
		    }
		    else
			    readOK = true;
		    if(!p_kIOStream.good())
			    goto m_bError;
	    }
	    if(remainInBuf<=4)
		    if(remainInBuf<=0 || codeSize > (remainInBuf*8-bitIndex))
			    goto done;
	    code = *((unsigned int*)(buf+bufIndex));
	    code <<= 32-codeSize-bitIndex;
	    code >>= 32-codeSize;
	    bitIndex += codeSize;
	    bufIndex += bitIndex/8;
	    remainInBuf -= bitIndex/8;
	    bitIndex %= 8;
	    if(code >= rootSize+1)
		    goto m_bError;
	    if(code == rootSize)
		    goto begin;
	    else
	    {	outP[outIndex++] = *strTable[code].p;
		    oldCode = code;
	    }
	    for(;;)
	    {	if(remainInBuf<=4 && !readOK)
		    {	for(i=0;i<remainInBuf;i++)
				    buf[i] = buf[bufIndex+i];
			    bufIndex = 0;
			    p_kIOStream.read((char*)&be,1);
			    if(be != 0)
			    {	p_kIOStream.read((char*)(buf+remainInBuf),be);
				    remainInBuf += be;
			    }
			    else
				    readOK = true;
			    if(!p_kIOStream.good())
				    goto m_bError;
		    }
		    if(remainInBuf<=4)
			    if(remainInBuf<=0 || codeSize > (remainInBuf*8-bitIndex))
				    break;
		    code = *((unsigned int*)(buf+bufIndex));
		    code <<= 32-codeSize-bitIndex;
		    code >>= 32-codeSize;
		    bitIndex += codeSize;
		    bufIndex += bitIndex/8;
		    remainInBuf -= bitIndex/8;
		    bitIndex %= 8;
		    if(code == rootSize)
		    {	codeSize = codeSizeBK;
			    for(i=rootSize;i<4096;i++)
				    if(strTable[i].p != NULL)
				    {	delete[] strTable[i].p;
					    strTable[i].p = NULL;
					    strTable[i].len = 0;
				    }
			    tableIndex = rootSize+2;
			    goto begin;
		    }
		    else if(code == rootSize+1)
			    break;
		    else
		    {	unsigned char *p = strTable[code].p;
			    int l = strTable[code].len;
			    unsigned char c;
			    if(p != NULL)
			    {	c = *p;
				    if(outIndex+l <= bufLen)
					    for(i=0;i<l;i++)
						    outP[outIndex++] = *p++;
				    else
					    goto m_bError;
				    if(!addStrTable(strTable,tableIndex++,oldCode,c))
					    goto m_bError;
				    oldCode = code;
			    }
			    else
			    {	p = strTable[oldCode].p;
				    l = strTable[oldCode].len;
				    c = *p;
				    if(outIndex+l+1 <= bufLen)
				    {	for(i=0;i<l;i++)
						    outP[outIndex++] = *p++;
					    outP[outIndex++] = c;
				    }
				    else
					    goto m_bError;
				    if(!addStrTable(strTable,tableIndex++,oldCode,c))
					    goto m_bError;
				    oldCode = code;
			    }
			    if(tableIndex == (((unsigned int)1)<<codeSize) && codeSize != 12)
				    codeSize++;
		    }
	    }
    done:
	    for(i=0;i<4096;i++)
		    if(strTable[i].p != NULL)
		    {	delete[] strTable[i].p;
			    strTable[i].p = NULL;
		    }
	    delete[] strTable;
	    return true;
    m_bError:
	    for(i=0;i<4096;i++)
		    if(strTable[i].p != NULL)
		    {	delete[] strTable[i].p;
			    strTable[i].p = NULL;
		    }
	    delete[] strTable;
	    delete[] f->dataBuf;
	    f->dataBuf = NULL;
	    return false;
    }
    //------------------------------------------------------------------------------
    bool GifLoader::extractData(FRAME* f,BufferIOStream& p_kIOStream)
    {
	    STRING_TABLE_ENTRY *strTable;
	    unsigned int codeSize,rootSize,tableIndex,codeSizeBK;
	    int remainInBuf = 0,i;
	    unsigned int bufIndex = 0,outIndex = 0;
	    unsigned int bitIndex = 0;
	    unsigned int code,oldCode;
	    unsigned char be,*outP;
	    unsigned char buf[262];
	    bool readOK = false;
	    unsigned int bufLen = f->imageWidth*f->imageHeight;
	    if((strTable = new STRING_TABLE_ENTRY[4096]) == NULL)
		    return false;
	    memset( strTable,0,sizeof(STRING_TABLE_ENTRY)*4096 );
	    outP = f->dataBuf = new unsigned char[bufLen];
	    if(f->dataBuf == NULL)
	    {	delete[] strTable;
		    return false;
	    }
	    p_kIOStream.read((char*)&be,1);
	    codeSizeBK = codeSize = be+1;
	    rootSize = 1;
	    rootSize <<= be;
	    tableIndex = rootSize+2;
	    if(!initStrTable(strTable,rootSize))
		    goto m_bError;

    begin:
	    if(remainInBuf<=4 && !readOK)
	    {	for(i=0;i<remainInBuf;i++)
			    buf[i] = buf[bufIndex+i];
		    bufIndex = 0;
		    p_kIOStream.read((char*)&be,1);
		    if(be != 0)
		    {	p_kIOStream.read((char*)(buf+remainInBuf),be);
			    remainInBuf += be;
		    }
		    else
			    readOK = true;
		    if(!p_kIOStream.good())
			    goto m_bError;
	    }
	    if(remainInBuf<=4)
        {
		    if(remainInBuf<=0 || codeSize > (remainInBuf*8-bitIndex))
            {
			    goto done;
            }
        }
	    code = *((unsigned int*)(buf+bufIndex));
	    code <<= 32-codeSize-bitIndex;
	    code >>= 32-codeSize;
	    bitIndex += codeSize;
	    bufIndex += bitIndex/8;
	    remainInBuf -= bitIndex/8;
	    bitIndex %= 8;
	    if(code >= rootSize+1)
        {
		    goto m_bError;
        }
	    if(code == rootSize)
        {
		    goto begin;
        }
	    else
	    {	outP[outIndex++] = *strTable[code].p;
		    oldCode = code;
	    }
	    for(;;)
	    {	if(remainInBuf<=4 && !readOK)
		    {	for(i=0;i<remainInBuf;i++)
				    buf[i] = buf[bufIndex+i];
			    bufIndex = 0;
			    p_kIOStream.read((char*)&be,1);
			    if(be != 0)
			    {	p_kIOStream.read((char*)(buf+remainInBuf),be);
				    remainInBuf += be;
			    }
			    else
				    readOK = true;
			    if(!p_kIOStream.good())
				    goto m_bError;
		    }
		    if(remainInBuf<=4)
			    if(remainInBuf<=0 || codeSize > (remainInBuf*8-bitIndex))
				    break;
		    code = *((unsigned int*)(buf+bufIndex));
		    code <<= 32-codeSize-bitIndex;
		    code >>= 32-codeSize;
		    bitIndex += codeSize;
		    bufIndex += bitIndex/8;
		    remainInBuf -= bitIndex/8;
		    bitIndex %= 8;
		    if(code == rootSize)
		    {	codeSize = codeSizeBK;
			    for(i=rootSize;i<4096;i++)
				    if(strTable[i].p != NULL)
				    {	delete strTable[i].p;
					    strTable[i].p = NULL;
					    strTable[i].len = 0;
				    }
			    tableIndex = rootSize+2;
			    goto begin;
		    }
		    else if(code == rootSize+1)
			    break;
		    else
		    {	unsigned char *p = strTable[code].p;
			    int l = strTable[code].len;
			    unsigned char c;
			    if(p != NULL)
			    {	c = *p;
				    if(outIndex+l <= bufLen)
					    for(i=0;i<l;i++)
						    outP[outIndex++] = *p++;
				    else
					    goto m_bError;
				    if(!addStrTable(strTable,tableIndex++,oldCode,c))
					    goto m_bError;
				    oldCode = code;
			    }
			    else
			    {	p = strTable[oldCode].p;
				    l = strTable[oldCode].len;
				    c = *p;
				    if(outIndex+l+1 <= bufLen)
				    {	for(i=0;i<l;i++)
						    outP[outIndex++] = *p++;
					    outP[outIndex++] = c;
				    }
				    else
					    goto m_bError;
				    if(!addStrTable(strTable,tableIndex++,oldCode,c))
					    goto m_bError;
				    oldCode = code;
			    }
			    if(tableIndex == (((unsigned int)1)<<codeSize) && codeSize != 12)
				    codeSize++;
		    }
	    }
    done:
	    for(i=0;i<4096;i++)
		    if(strTable[i].p != NULL)
		    {	delete strTable[i].p;
			    strTable[i].p = NULL;
		    }
	    delete[] strTable;
	    return true;
    m_bError:
	    for(i=0;i<4096;i++)
		    if(strTable[i].p != NULL)
		    {	delete strTable[i].p;
			    strTable[i].p = NULL;
		    }
	    delete[] strTable;
	    delete[] f->dataBuf;
	    f->dataBuf = NULL;
	    return false;
    }
    //------------------------------------------------------------------------------
    LPCGLOBAL_INFO GifLoader::getGlobalInfo()
    {	
	    return &m_kGInfo;
    }
    //------------------------------------------------------------------------------
    void GifLoader::close()
    {	
	    if(m_bOpened)
	    {	m_bOpened = false;
		    if(m_bInMem && m_vAllFrames != NULL)
		    {	FRAME* pf = m_vAllFrames;
			    for(unsigned int i=0;i<m_kGInfo.frames;i++)
			    {	if(pf->pColorTable != NULL)
				    {	delete[] pf->pColorTable;
					    pf->pColorTable = NULL;
				    }
				    if(pf->dataBuf != NULL)
				    {	delete[] pf->dataBuf;
					    pf->dataBuf = NULL;
				    }
			    }
			    delete[] m_vAllFrames;
			    m_vAllFrames = NULL;
		    }
		    if(!m_bInMem)
		    {	if(m_kCurFrame.pColorTable != NULL)
			    {	delete[] m_kCurFrame.pColorTable;
				    m_kCurFrame.pColorTable = NULL;
			    }
			    if(m_kCurFrame.dataBuf != NULL)
			    {	delete[] m_kCurFrame.dataBuf;
				    m_kCurFrame.dataBuf = NULL;
			    }
			    m_kIOStream.close();
		    }
	    }
    }
    //------------------------------------------------------------------------------
    char* GifLoader::getVersion( void )
    {	
	    return m_sVersion;
    }
    //------------------------------------------------------------------------------
    bool LoadGifWH( const char* p_sFileName,int& p_nWidth,int& p_nHeight )
    {
	    GifLoader kGif( p_sFileName,true );
	    if( !kGif )
	    {
		    return false;
	    }
	    LPCGLOBAL_INFO kGInfo = kGif.getGlobalInfo();
	    //获得文件宽度和高度和帧数
	    p_nWidth = kGInfo->scrWidth;
	    p_nHeight = kGInfo->scrHeight;
	    return true;
    }
    //------------------------------------------------------------------------------
    int LoadGifInfo(BitmapData* pBitmapData,GifLoader& kGif )
    {
	    if( !kGif ) return 0;
	    LPCGLOBAL_INFO kGInfo = kGif.getGlobalInfo();
	    //获得文件宽度和高度和帧数
	    int nWidth = kGInfo->scrWidth;
	    int nHeight = kGInfo->scrHeight;
	    int nCountFrame = kGInfo->frames;
	    LPCFRAME pFrame = &( kGif.m_vAllFrames[0] );
        if (pFrame == NULL) return 0;
	    int nImgPosX = pFrame->imageLPos;
	    int nImgPosY = pFrame->imageTPos;
	    int nImgWidth = pFrame->imageWidth;
	    int nImgHeight = pFrame->imageHeight;

	    int nGifNum = 4;
	    //分配内存
	    unsigned char* pPixels = (unsigned char*)( new char[ nWidth * nHeight * nGifNum  ] );
	    for( int i = 0 ; i < nWidth * nHeight * nGifNum ; i++ )
	    {
		    pPixels[i] = 0;
	    }
	    int nNum = 0;
	    for( int i = 0; i < nHeight ; i++ )
	    {
		    for( int j = 0; j < nWidth ; j++ )
		    {
			    if( j < nImgPosX || j >= ( nImgPosX + nImgWidth ) || i < nImgPosY || i >= ( nImgPosY + nImgHeight ) )
			    {
				    pPixels[ ( i * nWidth + j ) * nGifNum + 0 ] = 0;
				    pPixels[ ( i * nWidth + j ) * nGifNum + 1 ] = 0;
				    pPixels[ ( i * nWidth + j ) * nGifNum + 2 ] = 0;
				    pPixels[ ( i * nWidth + j ) * nGifNum + 3 ] = 0;
			    }
			    else
			    {
				    pPixels[ ( i * nWidth + j ) * nGifNum  + 0 ] = kGInfo->gColorTable[ pFrame->dataBuf[nNum] * 3  + 0 ];
				    pPixels[ ( i * nWidth + j ) * nGifNum  + 1 ] = kGInfo->gColorTable[ pFrame->dataBuf[nNum] * 3  + 1 ];
				    pPixels[ ( i * nWidth + j ) * nGifNum  + 2 ] = kGInfo->gColorTable[ pFrame->dataBuf[nNum] * 3  + 2 ];
				    //透明信息
				    if( pFrame->ctrlExt.trsFlag == true && pFrame->ctrlExt.trsColorIndex == pFrame->dataBuf[nNum] )
				    {
					    pPixels[ ( i * nWidth + j ) * nGifNum  + 3 ] = 0;
				    }
				    else
				    {
					    pPixels[ ( i * nWidth + j ) * nGifNum  + 3 ] = 255;
				    }
				    nNum++;
			    }
		    }
	    }
	    if( nGifNum == 4 )
	    {
            pBitmapData->m_nBpp = 32;
	    }
        pBitmapData->m_nWidth = nWidth;
        pBitmapData->m_nHeight = nHeight;
        pBitmapData->m_pImageData = (char*)pPixels;
	    return nCountFrame;
    }
    //------------------------------------------------------------------------------
    int LoadGif(BitmapData* pBitmapData,const char* p_sFileName )
    {
	    GifLoader kGif( p_sFileName,true );
	    if( !kGif )
	    {
		    return 0;
	    }
	    return LoadGifInfo(pBitmapData,kGif );
    }
    //------------------------------------------------------------------------------
    int LoadGif(BitmapData* pBitmapData,unsigned char* p_pData,int p_nDataSize )
    {
	    GifLoader kGif( p_pData,p_nDataSize );
	    if( !kGif )
	    {
		    return 0;
	    }
	    return LoadGifInfo(pBitmapData,kGif );
    }
    //------------------------------------------------------------------------------
    //这个是读取没帧动画用的。。但是因为外层还没有想好呢 所以先放着  方法是对的
    //就是没帧都依赖于上一阵的结果
    unsigned char* loadGif( char* p_sFileName,int p_nIndex,int& p_nWidth,int& p_nHeight, int& p_nCountFrame )
    {
	    GifLoader kGif( p_sFileName,true );
	    if( !kGif )
	    {
		    return NULL;
	    }
	    LPCGLOBAL_INFO kGInfo = kGif.getGlobalInfo();

	    //获得文件宽度和高度和帧数
	    int nWidth = kGInfo->scrWidth;
	    int nHeight = kGInfo->scrHeight;
	    int nCountFrame = kGInfo->frames;
	    if( p_nIndex >= nCountFrame )
	    {
		    return NULL;
	    }
	    LPCFRAME pFrame = &( kGif.m_vAllFrames[0] );
	    LPCFRAME pFrame1 = &( kGif.m_vAllFrames[1] );

	    int nImgPosX0 = pFrame->imageLPos;
	    int nImgPosy0 = pFrame->imageTPos;
	    int nImgWidth0 = pFrame->imageWidth;
	    int nImgHeight0 = pFrame->imageHeight;

	    int nImgPosX1 = pFrame1->imageLPos;
	    int nImgPosy1 = pFrame1->imageTPos;
	    int nImgWidth1 = pFrame1->imageWidth;
	    int nImgHeight1 = pFrame1->imageHeight;

	    int nGifNum = 4;

	    //分配内存
	    unsigned char* pPixels = (unsigned char*)( malloc( nWidth * nHeight * nGifNum  ) );
	    for( int i = 0 ; i < nWidth * nHeight * nGifNum ; i++ )
	    {
		    pPixels[i] = 0;
	    }

	    int nNum = 0;
	    for( int i = 0; i < nHeight ; i++ )
	    {
		    for( int j = 0; j < nWidth ; j++ )
		    {
			    if( j < nImgPosX0 || j >= ( nImgPosX0 + nImgWidth0 ) || i < nImgPosy0 || i >= ( nImgPosy0 + nImgHeight0 ) )
			    {
				    pPixels[ ( i * nWidth + j ) * nGifNum + 0 ] = 0;
				    pPixels[ ( i * nWidth + j ) * nGifNum + 1 ] = 0;
				    pPixels[ ( i * nWidth + j ) * nGifNum + 2 ] = 0;
			    }
			    else
			    {
				    pPixels[ ( i * nWidth + j ) * nGifNum  + 2 ] = kGInfo->gColorTable[ pFrame->dataBuf[nNum] * 3  + 0 ];
				    pPixels[ ( i * nWidth + j ) * nGifNum  + 1 ] = kGInfo->gColorTable[ pFrame->dataBuf[nNum] * 3  + 1 ];
				    pPixels[ ( i * nWidth + j ) * nGifNum  + 0 ] = kGInfo->gColorTable[ pFrame->dataBuf[nNum] * 3  + 2 ];
				    nNum++;
			    }
		    }
	    }

	    nNum = 0;
	    for( int i = 0; i < nHeight ; i++ )
	    {
		    for( int j = 0; j < nWidth ; j++ )
		    {
			    if( j < nImgPosX1 || j >= ( nImgPosX1 + nImgWidth1 ) || i < nImgPosy1 || i >= ( nImgPosy1 + nImgHeight1 ) )
			    {
				    //什么都不做
			    }
			    else
			    {
				    if( pFrame1->ctrlExt.trsFlag == true && pFrame1->ctrlExt.trsColorIndex == pFrame1->dataBuf[nNum] )
				    {
					    //什么都不做
					    //这个位置被透明掉了
				    }
				    else
				    {
					    pPixels[ ( i * nWidth + j ) * nGifNum  + 2 ] = kGInfo->gColorTable[ pFrame1->dataBuf[nNum] * 3  + 0 ];
					    pPixels[ ( i * nWidth + j ) * nGifNum  + 1 ] = kGInfo->gColorTable[ pFrame1->dataBuf[nNum] * 3  + 1 ];
					    pPixels[ ( i * nWidth + j ) * nGifNum  + 0 ] = kGInfo->gColorTable[ pFrame1->dataBuf[nNum] * 3  + 2 ];
				    }
				    nNum++;
			    }
		    }
	    }

	    p_nWidth = nWidth;
	    p_nHeight = nHeight;
	    p_nCountFrame = nCountFrame;

	    return pPixels;
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
