/**
@file			JCMemorySurvey.h
@brief			
@author			James
@version		1.0
@date			2014_5_13
*/

#ifndef __JCMemorySurvey_H__
#define __JCMemorySurvey_H__

#include <stdio.h>
#include <map>
#include <vector>
#include <string>

namespace laya 
{
struct MemorySurveyInfo
{
	long		m_nThis;	//作为类的指针数据,这个值作为key
	int			m_nID;
	std::string m_sClassName;
	int			m_nMemorySize;
};
struct MemoryStatistics
{
	std::string	m_sClassName;
	int			m_nCount;
	int			m_nMemorySize;
};
/**
* @brief 统计对象内存用的
*/
class JCMemorySurvey
{
public:

	typedef std::map<long,MemorySurveyInfo*>	MapMemoryInfo;
	typedef MapMemoryInfo::iterator				MapMemoryInfoIter;

	typedef std::vector<MemoryStatistics*>		VectorMemoryStatistics;
	typedef VectorMemoryStatistics::iterator	VectorMemoryStatisticsIter;

public:

	JCMemorySurvey();

	~JCMemorySurvey();

	static JCMemorySurvey* GetInstance();
	static void DelInstance();

public:

	void newClass( const char* p_sClassName,int p_nMemorySize,void* p_nThis,int p_nID = 0 );

	void releaseClass( const char* p_sClassName,void* p_nThis );

	void printAll( const char* p_sPath);

    void setEnable( bool bEnable );

protected:

	void statisticsMemory();

	void printMemorySurveyInfo( const char* p_sPath );

	void printMemoryStatis(const char* p_sPath);

private:

	MemoryStatistics* getMemoryStatistics( const char* p_sClassName );

	void clearAllMemoryStatistics();

public:

	static JCMemorySurvey*		ms_pMemorySurvey;

	MapMemoryInfo				m_vMapMemoryInfos;

	VectorMemoryStatistics		m_vVectorMemoryStatistics;

    bool                        m_bEnable;

};
}

#endif //__JCMemorySurvey_H__

//-----------------------------END FILE--------------------------------
