/**
@file			JCFileTable.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_11
*/

#include "JCFileTable.h"
#include "../fileSystem/JCFileSystem.h"
#include <stdlib.h>

namespace laya
{
	JCFileTable::JCFileTable(){
	}

    JCFileTable::~JCFileTable(){
	}

	void JCFileTable::init(){
	}

	//fileid chksum \r\n
	//数字都是10进制的
	int JCFileTable::initByString( const char* p_pszStr ){
		//split by \r\n
		const char* pCur = p_pszStr;
		const char* pCurData=pCur;
		std::vector<unsigned int> allData;
		allData.reserve(1024);
		while(*pCur!=0){
			while(*pCur==' '||*pCur=='\r'||*pCur=='\n'){
				pCur++;
			}
			if(*pCur==0)
				break;
			const char* pData=pCur;
			while(*pCur!=' '&&*pCur!='\r'&&*pCur!='\n'&&*pCur!=0){
				pCur++;
			}
			int len = (long)pCur-(long)pData-1;
			//之所以用strtoul是为了能得到unsigned int的值，如果用atoi的话，会出错。
			allData.push_back( strtoul(pData,0,16));
		}

		if( allData.size()%2!=0){
			//printf("fileTable::initByString 不合理的参数，应该为偶数个数据");
			//throw -1;
			return 0;
		}
		for( int i=0,sz=(int)allData.size()/2; i<sz; i++){
			_addFile(allData[i*2],allData[i*2+1]);
		}
		return (int)allData.size() / 2;
	}

	void JCFileTable::save(){
	}

	bool JCFileTable::find(unsigned int p_nFileID, unsigned int& p_nChkSum){
		Info* pInfo = getInfo(p_nFileID);
		if(pInfo){ 
			p_nChkSum = pInfo->chksum;
			return true;
		}
        p_nChkSum = 0;
		return false;
	}

	void JCFileTable::_addFile( unsigned int p_nFileID, unsigned int p_nChkSum ){
		unsigned char idx = (p_nFileID>>24)&0xff;
		std::vector<Info>& b = m_AllItem[idx];
		for( int i=0,sz=(int)b.size(); i<sz; i++){
			Info& curI = b[i];
			if(curI.fileid==p_nFileID ){
				return;				
			}
		}
		Info I={p_nFileID, p_nChkSum};
		b.push_back(I);
	}

	void JCFileTable::delFile( unsigned int p_nFileID )
    {
	}
};
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
