/**
@file			JCWorkSemaphore.cpp
@brief			
@author			hugao
@version		1.0
@date			2016_5_12
*/

#include "JCWorkSemaphore.h"

namespace laya
{
    JCWorkSemaphore::JCWorkSemaphore(){
		setDataNum(0);
		m_bStop=false;
	}

    JCWorkSemaphore::~JCWorkSemaphore(){
	}

	bool JCWorkSemaphore::waitUntilNoData(){
		std::unique_lock<std::mutex> lock(m_SemLock);
		while(m_nDataNum>0 && !m_bStop){
			m_condHasData.wait(lock);
		}
		return m_bStop;
	}
	
	bool JCWorkSemaphore::waitUntilHasData(){
		std::unique_lock<std::mutex> lock(m_SemLock);
		while(m_nDataNum<=0 && !m_bStop){
			m_condHasData.wait(lock);
		}
		return m_bStop;
	}

	void JCWorkSemaphore::setDataNum(int p_n ){
		std::unique_lock<std::mutex> lock(m_SemLock);
		m_nDataNum=p_n;
		m_condHasData.notify_all();
	}
    int JCWorkSemaphore::getDataNum()
    {
        return m_nDataNum;
    }

	void JCWorkSemaphore::notifyAllWait(){
		std::unique_lock<std::mutex> lock(m_SemLock);
		m_bStop=true;
		m_condHasData.notify_all();
	}

	void JCWorkSemaphore::reset(){
		m_bStop=false;
		m_nDataNum=0;
	}
};
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------