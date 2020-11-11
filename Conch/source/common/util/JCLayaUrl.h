/**
@file			JCLayaUrl.h
@brief			
@author			hugao
@version		1.0
@date			2016_5_12
*/

#ifndef __JCLayaUrl_H__
#define __JCLayaUrl_H__

/*
	解析url。
	可以用第三方库：例如 google url, cpp-netlib等。不过使用太麻烦，这里简单写一个，注意并不完全符合标准。
	标准为：
	http://www.ietf.org/rfc/rfc1738.txt
*/

#include <string>
#include <vector>

namespace laya{
	class JCUrl{
	public:
		//struct part{short st,len;};
		enum protocol{
			http,	// http://
			https,
			ftp,	// ftp://
			file,	// file://
			//barefile,//c:/ 没有file的形式
			unk,
		};

    public:
        JCUrl();
        JCUrl(const char* p_pszUrl);
		~JCUrl();

		bool parse(const char* p_pszUrl);

		std::string resolve(const char* p_pszPath);
		std::string toString();

		void parsePath(const char* p_pszData);
		void pathToString(bool p_bAddFile);
		const char* getProtocolString();
		std::string encode2();
	protected:
		void trim(const char*& p_pszData);
		void parseScheme(const char*& p_pszData);
		void parseFile(const char*& p_pszData);
		void parseQuery(const char*& p_pszData);
		void parseSegment(const char*& p_pszData);
		void parseUserPassHostPort(const char* p_pszData);
		//用几个关键符号拆分。/?
		//从host位置开始。
		std::vector<int> split(const char* p_pszData);
	public:
		char*			m_pszUrl;
		int				m_nUrlSize;
		protocol		m_nProto;
		/*
		part			m_Protocol;
		part			m_User;
		part			m_Host;
		part			m_Port;
		part			m_Path;
		part			m_File;
		part			m_Query;
		part			m_Segmet;//TODO 还没做
		*/
		std::string		m_User,m_Pass,m_Host,m_Port,m_Query;
		/*
			对于有协议的，肯定是从根路径开始。因此m_vPath[0]要么是驱动器，要么是第一层目录，不需要是 /
			m_vPath[0] 总是驱动器，或者第一层目录
			m_vPath[最后] 可能是文件也可能是目录，如果最后是 /?xx=xx 则是空字符""
		*/
		std::vector<std::string> m_vPath;
		std::string		m_strPath;//保存一个完整的path，用来快速合并。
		std::string		m_strProtHost;
		bool			m_bHasDriver;
		//Path			m_Path;
	};
};
//------------------------------------------------------------------------------


#endif //__JCLayaUrl_H__

//-----------------------------END FILE--------------------------------