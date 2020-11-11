/**
@file			JCCondition.h
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#ifndef __JCCondition_H__
#define __JCCondition_H__

#include <thread>
#include <condition_variable>

class JCCondition
{
	std::recursive_mutex m_Mutex;
	std::condition_variable_any m_Condition;

public:
	void Wait()
	{
		m_Condition.wait( m_Mutex );
	}

	void NotifyOne()
	{
		m_Condition.notify_one();
	}

	void NotifyAll()
	{
		m_Condition.notify_all();
	}

	void Lock()
	{
		m_Mutex.lock();
	}

	void Unlock()
	{
		m_Mutex.unlock();
	}
};
//------------------------------------------------------------------------------


#endif //__JCCondition_H__

//-----------------------------END FILE--------------------------------

