/**
@file			JCLayaUrl.cpp
@brief			
@author			hugao
@version		1.0
@date			2016_5_12
*/

#include "JCLayaUrl.h"
#include "Log.h"
#include "JCCommonMethod.h"


namespace laya{
	JCUrl::JCUrl(){
		m_pszUrl = NULL;
		m_nProto = unk;
		m_nUrlSize = 0;
		m_bHasDriver=false;
	}
    JCUrl::JCUrl(const char* p_pszUrl) {
		m_pszUrl = NULL;
		m_nProto = unk;
		m_nUrlSize = 0;
		m_bHasDriver = false;
		parse(p_pszUrl);
	}
    JCUrl::~JCUrl(){
		if(m_pszUrl){
			delete [] m_pszUrl;
			m_pszUrl = NULL;
		}
	}

	void JCUrl::trim(const char*& p_pszData){
		char cv = *p_pszData;
		while(cv==' '||cv=='\t'){
			cv=*(++p_pszData);
		}
	}

	//由于js的location是没有 // 所以这个也不要带 //
	const char* JCUrl::getProtocolString(){
		switch (m_nProto)
		{
		case laya::JCUrl::http:
			return "http:";
			break;
		case laya::JCUrl::https:
			return "https:";
			break;
		case laya::JCUrl::ftp:
			return "ftp:";
			break;
		case laya::JCUrl::file:
			return "file:";
			break;
		default:
			return "unk:";
			break;
		}
	}

	//TODO 现在还没有拆分 #
	std::vector<int> JCUrl::split(const char* p_pszData){
		const char* pData = p_pszData;
		char cv = *pData;
		std::vector<int> ret;
		while(cv){
			if(cv=='/'||cv=='\\' ){
				ret.push_back(pData-p_pszData);
			}else if(cv=='?'){
				ret.push_back(pData-p_pszData);
				break;
			}
			cv = *(++pData);
		}
		return ret;
	}

	void JCUrl::parseScheme(const char*& p_pszData){
		const char* pcur = p_pszData;
		if((pcur[0]=='h'||pcur[0]=='H')&&(pcur[1]=='t'||pcur[1]=='T')&&(pcur[2]=='t'||pcur[2]=='T')&&(pcur[3]=='p'||pcur[3]=='P')){
			if(pcur[4]=='s' || pcur[4]=='S'){
				m_nProto = https;
				p_pszData+=5;
			}else{
				m_nProto = http;
				//m_Protocol.st=0; m_Protocol.len=4;
				p_pszData+=4;
			}
		}else if((pcur[0]=='f'||pcur[0]=='F')&&(pcur[1]=='t'||pcur[1]=='T')&&(pcur[2]=='p'||pcur[2]=='P')){
			m_nProto = ftp;
			//m_Protocol.st=0; m_Protocol.len=3;
			p_pszData+=3;
		}else if((pcur[0]=='f'||pcur[0]=='F')&&(pcur[1]=='i'||pcur[1]=='I')&&(pcur[2]=='l'||pcur[2]=='L')&&(pcur[3]=='e'||pcur[0]=='E')){
			//m_Protocol.st=0; m_Protocol.len=4;
			m_nProto = file;
			p_pszData+=4;
		}else{
			throw 11;
		}
		if(*(p_pszData++)==':' && *(p_pszData++)=='/' && *(p_pszData++)=='/' ){}
		else
			throw 12;
	}

	void JCUrl::parseUserPassHostPort(const char* p_pszData){
		const char* pColonPos=NULL;
		const char* pAtPos=NULL;
		const char* pCurData = p_pszData;
		while(true ){
			if(*pCurData==0){
				m_Host=pAtPos?(pAtPos):p_pszData;
				break;
			}
			if(*pCurData==':'){
				pColonPos=pCurData+1;
				if(pAtPos){//已经有@了，表示这是端口了
					m_Host = ""; m_Host.append(pAtPos,pCurData-pAtPos);
					m_Port = pColonPos;
					break;
				}
			}else if(*pCurData=='@'){//有用户信息
				pAtPos = pCurData+1;
				if(pColonPos){
					m_User="";m_User.append(p_pszData,pColonPos-p_pszData-1);
					m_Pass="";m_Pass.append(pColonPos, pCurData-pColonPos);
				}else{
					m_User="";m_User.append(p_pszData, pCurData-p_pszData);
				}
			}
			pCurData++;
		}
	}

	void JCUrl::parsePath(const char* pData){
		m_vPath.clear();
		int datalen = strlen(pData);
		std::vector<int> parts = split(pData);
		if( parts.size()>0){
			//第一部分
			std::string cpath="";
			cpath.append(pData,parts[0]);
			if(cpath!=".")
				m_vPath.push_back(cpath);

			bool bHasQuery=false;
			if(pData[parts[parts.size()-1]]=='?'){//query
				bHasQuery=true;
				int nQuerySt =parts[parts.size()-1];
				m_Query="";m_Query.append(pData+nQuerySt,datalen-nQuerySt);
			}



			//中间
			for( int i=0,sz=parts.size()-1; i<sz; i++){
				cpath="";
				int l = (parts[i+1]-parts[i])-1;
				if(l<=0&&i!=0 && i!=sz-1){
					continue;		//空的，通常是 //或者\\或者 /\什么的
				}
				cpath.append((char*)(pData+parts[i]+1),l);
				if(cpath==".."){
					if(m_vPath.size())
						m_vPath.pop_back();
					continue;
				}
				if(cpath!=".")
					m_vPath.push_back(cpath);
			}
			//剩余
			if(!bHasQuery){
				cpath="";
				cpath.append((char*)(pData+parts[parts.size()-1]+1),datalen-parts[parts.size()-1]-1);
				if(cpath==".."){
					if(m_vPath.size())
						m_vPath.pop_back();
				}else if(cpath!=".")
					m_vPath.push_back(cpath);
			}
		}else{
			m_vPath.push_back(pData);
		}
		pathToString(true);
	}

	void JCUrl::pathToString(bool p_bAddFile){
		//计算空间
		m_strPath.clear();
		int nPathSize = 0;
		int nPathNum = m_vPath.size();
		if( !p_bAddFile && nPathNum==1){
			m_strPath="";
			return;
		}
		for( int i=0; i<nPathNum; i++){
			nPathSize+=m_vPath[i].length();
		}
		if( nPathSize>0){
			//if( m_vPath[nPathNum-1].length()==0)	//最后一个是文件
			//TODO url可能最后是一个目录么？如果是的话怎么处理。按理说应该必须是一个文件。实际的目录自动加index.html是服务器端做得？
			m_strPath.append(m_vPath[0].c_str());
			for(int i=1; i<nPathNum-(p_bAddFile?0:1); i++){
				m_strPath.append("/",1);
				m_strPath.append(m_vPath[i].c_str());
			}
		}
	}

	void JCUrl::parseFile(const char*& p_pszData){
	}

	void JCUrl::parseQuery(const char*& p_pszData){
	}

	void JCUrl::parseSegment(const char*& p_pszData){
	}

	bool JCUrl::parse(const char* p_pszUrl){
		if(p_pszUrl==NULL)
			return false;
		int len = strlen(p_pszUrl);
		try{
			m_vPath.clear();
			m_Host="";
			m_Port="";
			m_User="";
			m_Pass="";
			m_Query="";
			m_strProtHost="";
			m_bHasDriver=false;
			const char* pData = p_pszUrl;
			trim(pData);
			std::string tmpString;
			//如果是直接写路径的。缺省认为使用的是file
			if( pData[0]=='/'){
				tmpString = "file://";
				tmpString += pData;
				pData = tmpString.c_str();
			}
			else if( pData[1]==':'){
				tmpString = "file:///";
				tmpString += pData;
				pData = tmpString.c_str();
			}
			const char* pStart = pData;
			parseScheme(pData);
			m_strProtHost.append(pStart,pData-pStart);
			int datalen = strlen(pData);
			std::vector<int> parts = split(pData);
            if (parts.size() > 0) {
                //host
                //m_Host.st=0; m_Host.len=0;
                //m_Query.st=0; m_Query.len=0;
                if (parts[0] > 0) {//user,pass,host,port
                    std::string uphp;
                    uphp.append(pData, parts[0]);
                    m_strProtHost.append(pData, parts[0]);
                    parseUserPassHostPort(uphp.c_str());
                }

                bool bHasQuery = false;
                int lastPos = parts[parts.size() - 1];
                if (pData[lastPos] == '?') {//query
                    bHasQuery = true;
                    //m_Query.st=nQuerySt;
                    //m_Query.len=datalen-nQuerySt;
                    //m_Path.st = parts[0];//包含 /
                    //m_Path.len = parts[parts.size()-1]-m_Path.st;
                    m_Query = ""; m_Query.append(pData + lastPos, datalen - lastPos);

                }
                else {
                    //m_Path.st = parts[0];
                    //m_Path.len = datalen-m_Path.st;
                }

                for (int i = 0, sz = parts.size() - 1; i < sz; i++) {
                    std::string cpath = "";
                    int l = (parts[i + 1] - parts[i]) - 1;
                    if (l <= 0 && i != 0 && i != sz - 1) {
                        continue;		//空的，通常是 //或者\\或者 /\什么的
                    }
                    cpath.append((char*)(pData + parts[i] + 1), l);
                    if (cpath == "..") {
                        if (m_vPath.size())
                            m_vPath.pop_back();
                        continue;
                    }
                    if (cpath != ".")
                        m_vPath.push_back(cpath);
                }

                if (!bHasQuery) {
                    std::string cpath = "";
                    cpath.append((char*)(pData + parts[parts.size() - 1] + 1), datalen - parts[parts.size() - 1] - 1);
                    if (cpath == "..") {
                        if (m_vPath.size())
                            m_vPath.pop_back();
                    }
                    else if (cpath != ".")
                        m_vPath.push_back(cpath);
                }
                if (m_vPath.size() > 0 && m_vPath[0].length() > 1 && m_vPath[0][1] == ':') {
                    m_bHasDriver = true;
                }
            }
            else {
                m_strProtHost = p_pszUrl;
                parseUserPassHostPort(pData);
            }
			//合并path
			pathToString(false);
			if(m_strPath.length()>0){
				if(m_strPath[0]=='/'){
					m_strPath = m_strProtHost+m_strPath;
				}else{
					m_strPath = m_strProtHost+"/"+m_strPath;
				}
			}else{
				if(m_nProto==file)
					m_strPath = "file:///";
				else
					m_strPath = m_strProtHost;
			}
		}catch(int e){
			LOGE("JCUrl::parse error [%d]:%s",e, p_pszUrl );
			return false;
		}
		return true;
	}

	std::string JCUrl::resolve(const char* p_pszPath){
		if(strstr(p_pszPath,"://")){
            JCUrl pathurl;
			pathurl.parse(p_pszPath);

			if(pathurl.m_vPath.size()==1&&pathurl.m_Host.length()<=0)
				return pathurl.m_strPath+pathurl.m_vPath[pathurl.m_vPath.size()-1]+pathurl.m_Query;	//file:///a.txt
			return pathurl.m_strPath+"/"+ (pathurl.m_vPath.size() > 0 ? pathurl.m_vPath[pathurl.m_vPath.size()-1] : "")+pathurl.m_Query;	//file:///a/a.txt
		}
        JCUrl pathurl;
		pathurl.parsePath(p_pszPath);
		if( pathurl.m_vPath.size()>0){
			pathurl.pathToString(true);
			if(pathurl.m_vPath[0]=="" ){ //绝对路径
				if(m_bHasDriver){
					return m_strProtHost+"/"+m_vPath[0]+pathurl.m_strPath+pathurl.m_Query;
				}else
					return m_strProtHost+pathurl.m_strPath+pathurl.m_Query;
			}else if(pathurl.m_vPath[0].length()>1&&pathurl.m_vPath[0].at(1)==':'){//带盘符的
				return m_strProtHost+"/"+pathurl.m_strPath+pathurl.m_Query;
			}else{
				return m_strPath+"/"+pathurl.m_strPath+pathurl.m_Query;
			}
		}
		return m_strPath;
	}

	std::string JCUrl::toString(){
		std::string ret = m_strPath  +"/"+ (m_vPath.size()>0?m_vPath[m_vPath.size()-1]:"")+m_Query;
		return ret;
	}

	std::string JCUrl::encode2() {
		if (m_nProto == file || m_vPath.size()<=0) {
			return toString();
		}
		std::string ret = m_strPath + "/" + UrlEncode(m_vPath[m_vPath.size() - 1].c_str());
		if (m_Query.length() > 0) {
			ret += "?";
			ret += UrlEncode(m_Query.substr(1, m_Query.length() - 1).c_str());
		}
		return ret;
	}
};
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------