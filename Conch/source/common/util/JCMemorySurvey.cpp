/**
@file			JCMemorySurvey.cpp
@brief			
@author			James
@version		1.0
@date			2014_5_13
*/

//包含头文件
#include "JCMemorySurvey.h"
#include "Log.h"
#include "../fileSystem/JCFileSystem.h"

namespace laya 
{
JCMemorySurvey* JCMemorySurvey::ms_pMemorySurvey = NULL;
//------------------------------------------------------------------------------
JCMemorySurvey::JCMemorySurvey()
{
    m_bEnable = false;
}
//------------------------------------------------------------------------------
JCMemorySurvey::~JCMemorySurvey()
{

}
//------------------------------------------------------------------------------
void JCMemorySurvey::setEnable(bool bEnable)
{
    m_bEnable = bEnable;
}
//------------------------------------------------------------------------------
JCMemorySurvey* JCMemorySurvey::GetInstance()
{
	if( ms_pMemorySurvey == NULL )
	{
		ms_pMemorySurvey =  new JCMemorySurvey();
	}
	return ms_pMemorySurvey;
}
//------------------------------------------------------------------------------
void JCMemorySurvey::DelInstance()
{
	if( ms_pMemorySurvey )
    {
		delete ms_pMemorySurvey;
		ms_pMemorySurvey = NULL;
	}
}
//------------------------------------------------------------------------------
void JCMemorySurvey::newClass( const char* p_sClassName,int p_nMemorySize,void* p_nThis,int p_nID )
{
    if (m_bEnable)
    {
        long nThis = (long)p_nThis;
        MapMemoryInfoIter iter = m_vMapMemoryInfos.find(nThis);
        if (iter != m_vMapMemoryInfos.end())
        {
            LOGI("JCMemorySurvey::newClass error");
            return;
        }
        MemorySurveyInfo* pInfo = new MemorySurveyInfo();
        pInfo->m_nID = p_nID;
        pInfo->m_nMemorySize = p_nMemorySize;
        pInfo->m_sClassName = p_sClassName;
        pInfo->m_nThis = nThis;
        m_vMapMemoryInfos[pInfo->m_nThis] = pInfo;
    }
}
//------------------------------------------------------------------------------
void JCMemorySurvey::releaseClass( const char* p_sClassName,void* p_nThis )
{
    if (m_bEnable)
    {
        long nThis = (long)p_nThis;
        MapMemoryInfoIter iter = m_vMapMemoryInfos.find(nThis);
        if (iter != m_vMapMemoryInfos.end())
        {
            MemorySurveyInfo* pInfo = iter->second;
            if (pInfo != NULL)
            {
                delete pInfo;
                pInfo = NULL;
            }
            iter = m_vMapMemoryInfos.erase(iter);
        }
        else
        {
            LOGI("JCMemorySurvey::releaseClass error className=%s,p_nThis=%d", p_sClassName, p_nThis);
        }
    }
}
//------------------------------------------------------------------------------
MemoryStatistics* JCMemorySurvey::getMemoryStatistics(  const char* p_sClassName )
{
	for( int i = 0 ; i < (int)(m_vVectorMemoryStatistics.size()); i++ )
	{
		if( strcmp( m_vVectorMemoryStatistics[i]->m_sClassName.c_str(),p_sClassName ) == 0 )
		{
			return m_vVectorMemoryStatistics[i];
		}
	}
	return NULL;
}
//------------------------------------------------------------------------------
void JCMemorySurvey::clearAllMemoryStatistics()
{
	for( int i = 0 ; i < (int)(m_vVectorMemoryStatistics.size()); i++ )
	{
		MemoryStatistics* pStatis = m_vVectorMemoryStatistics[i];
		if( pStatis != NULL )
		{
			delete pStatis;
			pStatis = NULL;
		}
	}
	m_vVectorMemoryStatistics.clear();
}
//------------------------------------------------------------------------------
void JCMemorySurvey::statisticsMemory()
{
	clearAllMemoryStatistics();
	for( MapMemoryInfoIter iter = m_vMapMemoryInfos.begin(); iter != m_vMapMemoryInfos.end(); iter++ )
	{
		MemorySurveyInfo* pInfo = iter->second;
		if( pInfo != NULL )
		{
			MemoryStatistics* pStatis = getMemoryStatistics( pInfo->m_sClassName.c_str() );
			if( pStatis != NULL )
			{
				pStatis->m_nCount++;
			}
			else
			{
				pStatis = new MemoryStatistics();
				pStatis->m_nCount = 1;
				pStatis->m_nMemorySize = pInfo->m_nMemorySize;
				pStatis->m_sClassName = pInfo->m_sClassName;
				m_vVectorMemoryStatistics.push_back( pStatis );
			}
		}
	}
}
//------------------------------------------------------------------------------
void JCMemorySurvey::printMemorySurveyInfo( const char* p_sPath )
{
	std::string sBuffer = "";
	for( MapMemoryInfoIter iter = m_vMapMemoryInfos.begin(); iter != m_vMapMemoryInfos.end(); iter++ )
	{
		MemorySurveyInfo* pInfo = iter->second;
		if( pInfo != NULL )
		{
			char sTemp[1024] = {0};
			sprintf( sTemp,"class=%s,id=%d,this=%ld,size=%d\r\n",pInfo->m_sClassName.c_str(),pInfo->m_nID,pInfo->m_nThis,pInfo->m_nMemorySize );
			sBuffer += sTemp;
		}
	}
	std::string sPath = p_sPath;
	sPath += "/memorySurvey.txt";
	JCBuffer buf((char*)sBuffer.c_str(), strlen(sBuffer.c_str()),false,false);
	writeFileSync( sPath.c_str(),buf );
}
//------------------------------------------------------------------------------
void JCMemorySurvey::printMemoryStatis( const char* p_sPath)
{
	statisticsMemory();
	std::string sBuffer = "";
    int nCount = 0;
	for( int i = 0 ; i < (int)(m_vVectorMemoryStatistics.size()); i++ )
	{
		MemoryStatistics* pInfo = m_vVectorMemoryStatistics[i];
		if( pInfo != NULL )
		{
			char sTemp[1024] = {0};
			sprintf( sTemp,"count=%d\t,class=%s\t,size=%d,countSize=%d\r\n",pInfo->m_nCount,pInfo->m_sClassName.c_str(),pInfo->m_nMemorySize,pInfo->m_nCount*pInfo->m_nMemorySize );
			LOGI("printMemoryStatis size=%d,%s",m_vVectorMemoryStatistics.size(),sTemp);
			sBuffer += sTemp;
            nCount += pInfo->m_nCount*pInfo->m_nMemorySize;
		}
	}
    char sTemp1[1024] = { 0 };
    sprintf( sTemp1,"Dynamic Object memory around=%5.2fMB\r\n" ,(float)(nCount/1024.0f/1024.0f) );
    LOGI("%s",sTemp1);
    sBuffer += sTemp1;

	std::string sPath = p_sPath;
	sPath += "/memoryStatis.txt";
	JCBuffer buf((char*)sBuffer.c_str(), strlen(sBuffer.c_str()),false,false);
	writeFileSync( sPath.c_str(),buf );
}
//------------------------------------------------------------------------------
void JCMemorySurvey::printAll(const char* p_sPath)
{
    if (m_bEnable)
    {
        //printMemorySurveyInfo(p_sPath);
        printMemoryStatis(p_sPath);
    }
    else
    {
        LOGE("JCMemorySurvey::printAll error enable=false.");
        LOGE("You can set it by method. 'conch.config.enableMemorySurvey(true);'");
    }
}
//------------------------------------------------------------------------------
}

//-----------------------------END FILE--------------------------------
