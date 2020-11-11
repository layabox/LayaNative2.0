/**
@file			JCEventBase.cpp
@brief			
@author			hugao
@version		1.0
@date			2016_5_11
*/

//包含头文件
#include "../event/JCEventBase.h"
//------------------------------------------------------------------------------
namespace laya
{
    JCEventBase::JCEventBase()
    {
		m_nID=0;
		m_pName=0;
		m_nFlags=0; 
		m_nPriority=0;	//优先级
		m_nP1=m_nP2=0;
		m_pData=0;		//外面分配，需要自己释放。
	}
    JCEventBase::~JCEventBase()
    {
        /*
		if( m_pData && (m_nFlags&NEEDDELDATA)){
			delete m_pData;
			m_pData = 0;
		}
        */
	}
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
