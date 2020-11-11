/**
@file			JCThreadPool.h
@brief			
@author			hugao
@version		1.0
@date			2016_5_12
*/

#ifndef __JCThreadPool_H__
#define __JCThreadPool_H__

#include <thread>
#include <atomic>
#include <vector>
#include <list>
#include "JCCondition.h"

/** 
 *  @brief JCThreadPoolShare
          *包含若干线程
          *一个数据列表 m_ShareData
          *多个线程共同处理这个数据列表。 
*/

template<class _Tp>
class JCThreadPoolShare
{
private:
	JCThreadPoolShare(JCThreadPoolShare const&);
	JCThreadPoolShare& operator=(JCThreadPoolShare const&);     

protected:
	typedef _Tp _DataType;

	typedef std::list<_DataType>			__DataQueue;
	typedef typename __DataQueue::iterator	__DataQueueIter;

	struct __Thread_t
	{
		std::thread *m_pThread;
		JCCondition *m_pCondition;

		__DataQueue *m_pData;
		bool m_bNeedFree;

		explicit __Thread_t( std::thread *p_pThread, JCCondition *p_pCondition, __DataQueue *p_pData, bool p_bNeedFree )
		{
			m_pThread = p_pThread;
			m_pCondition = p_pCondition;
			m_pData = p_pData;
			m_bNeedFree = p_bNeedFree;
		}

		~__Thread_t()
		{
			if( 0 != m_pThread )
			{
				delete m_pThread;
				m_pThread = 0;
			}

			if( m_bNeedFree )
			{
				if( 0 != m_pCondition )
				{
					delete m_pCondition;
					m_pCondition = 0;
				}

				if( 0 != m_pData )
				{
					m_pData->clear();
					delete m_pData;
					m_pData = 0;
				}
			}
		}
	};
	typedef std::vector<__Thread_t *>			__ThreadQueue;
	typedef typename __ThreadQueue::iterator	__ThreadQueueIter;

	__ThreadQueue m_Threads;
	std::recursive_mutex m_Mutex;

	JCCondition m_ShareCondition;
	__DataQueue m_ShareData;

#ifdef WIN32
    std::atomic_uint32_t m_bWantToStop;
#else
    std::atomic_uint m_bWantToStop;
#endif

	__Thread_t *__GetCurrentThreadContext()
	{
		__Thread_t *pCurThread = 0;

		std::thread::id _CurID = std::this_thread::get_id();
        
        std::unique_lock<std::recursive_mutex> _lock(m_Mutex);

		if( m_Threads.size() )
		{
			__Thread_t *p;
			__ThreadQueueIter iter;
			for(iter=m_Threads.begin();iter!=m_Threads.end();iter++)
			{
				p = *iter;
				if( p->m_pThread->get_id() == _CurID )
				{
					pCurThread = p;
					break;;
				}
			}
		}

		return pCurThread;
	}

	void __RemoveCurrentThread()
	{
		__Thread_t *pCurThread = 0;

		std::thread::id _CurID = std::this_thread::get_id();

		std::unique_lock<std::recursive_mutex> _lock(m_Mutex);//kuo

		if( m_Threads.size() )
		{
			__Thread_t *p;
			__ThreadQueueIter iter;
			for(iter=m_Threads.begin();iter!=m_Threads.end();iter++)
			{
				p = *iter;
				if( p->m_pThread->get_id() == _CurID )
				{
					m_Threads.erase( iter );
					p->m_pThread->detach();
					delete p;
					break;
				}
			}
		}
	}

	virtual int __AddThread( std::thread *p_pThread )
	{
		__Thread_t *p = new __Thread_t( p_pThread, &m_ShareCondition, &m_ShareData, false );

		m_Threads.push_back( p );

		return m_Threads.size()-1;
	}

	bool __SendData( _DataType p_Data, JCCondition *p_pCondition, __DataQueue *p_pData )
	{
		if( 0 != m_bWantToStop )
		{
			return false;
		}

		p_pCondition->Lock();
		p_pData->push_back( p_Data );
		p_pCondition->Unlock();

		p_pCondition->NotifyOne();

		return true;
	}

public:
	JCThreadPoolShare()
	{
		m_bWantToStop = false;
	}

	~JCThreadPoolShare()
	{
		Stop();
	}

	size_t GetSize()
	{
		std::unique_lock<std::recursive_mutex> _lock(m_Mutex);

		return m_Threads.size();
	}

	template<typename F>
	int CreateThread( F p_pfnThreadFunc )
	{
		m_bWantToStop = 0;

		std::unique_lock<std::recursive_mutex> _lock(m_Mutex);

		std::thread *pThread = new std::thread(p_pfnThreadFunc);

		if( 0 == pThread )
			return -1;
		else
			return __AddThread( pThread );
	}

	void ThreadStopped()
	{
		if( 0 == m_bWantToStop )
		{
			__RemoveCurrentThread();
		}
	}

	//取出当前线程的任务进行处理。
	//根据运行时所在线程。
	bool WaitData( _DataType *p_pData )
	{
		if( 0 == p_pData )
			return false;

		m_bWantToStop = 0;

		__Thread_t *pContext = __GetCurrentThreadContext();
		JCCondition *pCondition;
		__DataQueue *pDataList;

		if( 0 == pContext )
		{
			pCondition = &m_ShareCondition;
			pDataList = &m_ShareData;
		}
		else
		{
			pCondition = pContext->m_pCondition;
			pDataList = pContext->m_pData;
		}

		pCondition->Lock();
		if( pDataList->size() )
		{
			*p_pData = pDataList->front();
			pDataList->pop_front();
			pCondition->Unlock();
			return true;
		}

		for(;0==m_bWantToStop;)
		{
			pCondition->Wait();
			if( 0 != m_bWantToStop )
			{
				pCondition->Unlock();
				return false;
			}

			if( pDataList->size() )
			{
				*p_pData = pDataList->front();
				pDataList->pop_front();
				pCondition->Unlock();
				return true;
			}
		}

		pCondition->Unlock();

		return false;
	}

	virtual bool SendData( _DataType p_Data )
	{
		return __SendData( p_Data, &m_ShareCondition, &m_ShareData );
	}

	void ClearSharedData(){
		m_ShareCondition.Lock();
		m_ShareData.clear();
		m_ShareCondition.Unlock();
	}

	virtual bool SendDataToThread( _DataType p_Data, size_t n )
	{
		return SendData( p_Data );
	}
#if 0
    virtual void Stop()
    {
        std::recursive_mutex::scoped_lock _lock(m_Mutex);

		m_bWantToStop = 1;

		if( m_Threads.size() )
		{
			__ThreadQueueIter iter;
			__Thread_t *pThread;

			for(iter=m_Threads.begin();iter!=m_Threads.end();iter++)
			{
				pThread = *iter;
				//pThread->m_pCondition->Lock();
				//pThread->m_pCondition->NotifyAll();
				//pThread->m_pCondition->Unlock();

                bool finished = false;
                while(!finished)
                {
                    std::this_thread::sleep(std::posix_time::milliseconds(10));
                    pThread->m_pCondition->Lock();
			        pThread->m_pCondition->NotifyAll();
			        pThread->m_pCondition->Unlock();
                    finished = pThread->m_pThread->timed_join( std::posix_time::milliseconds(1*10));
                }
    			//pThread->m_pThread->join();
				delete pThread;
			}

			m_Threads.clear();
		}

		m_ShareCondition.NotifyAll();
    }
#endif
#if 1
	virtual void Stop(){
		m_Mutex.lock();
		//std::recursive_mutex::scoped_lock _lock(m_Mutex);

		m_bWantToStop = 1;

		if( m_Threads.size() )
		{
			__ThreadQueueIter iter;
			__Thread_t *pThread;

			for(iter=m_Threads.begin();iter!=m_Threads.end();iter++)
			{
				pThread = *iter;
				pThread->m_pCondition->Lock();
				pThread->m_pCondition->NotifyAll();
				pThread->m_pCondition->Unlock();
				m_Mutex.unlock();//join需要等待线程结束，而线程的结束可能需要 m_Mutex
				pThread->m_pThread->join();
				m_Mutex.lock();
			}
			//两个线程都停了，再删除。否则waitdata的时候会遍历数组，访问里面的对象
			for(iter=m_Threads.begin();iter!=m_Threads.end();iter++){
				pThread = *iter;
				delete pThread;
			}

			m_Threads.clear();
		}

		m_ShareCondition.NotifyAll();
		m_Mutex.unlock();
	}
#endif
};

template<class _Tp>
class JCThreadPoolSingle : public JCThreadPoolShare<_Tp>
{
    typedef JCThreadPoolShare<_Tp> __BaseClass;
    
protected:
	/*
		single 的话
			每个线程使用独立的 JCCondition 和 数据队列。

	*/
	virtual int __AddThread( std::thread *p_pThread )
	{
		JCCondition *pCondition = new JCCondition;
        typename __BaseClass::__DataQueue *pData = new typename __BaseClass::__DataQueue;

        typename __BaseClass::__Thread_t *p = new typename __BaseClass::__Thread_t( p_pThread, pCondition, pData, true );

		__BaseClass::m_Threads.push_back( p );

		return __BaseClass::m_Threads.size()-1;
	}

public:
	virtual bool SendData( typename __BaseClass::_DataType p_Data )
	{
		throw "JCThreadPoolSingle::SendData error";
		return false;
	}

	virtual bool SendDataToThread( typename __BaseClass::_DataType p_Data, size_t n )
	{
		std::unique_lock<std::recursive_mutex> _lock(__BaseClass::m_Mutex);

		if( n >= __BaseClass::m_Threads.size() )
		{
			return false;
		}

        typename __BaseClass::__Thread_t *p = __BaseClass::m_Threads[n];

		return this->__SendData( p_Data, p->m_pCondition, p->m_pData );
	}

	virtual void ClearDataOfThread(int p_n){
		printf("----ClearDataOfThread %d\n",p_n);
		std::unique_lock<std::recursive_mutex> _lock(__BaseClass::m_Mutex);
		if( p_n >= (int)__BaseClass::m_Threads.size() ){
			return ;
		}
        typename __BaseClass::__Thread_t *p = __BaseClass::m_Threads[p_n];
		p->m_pCondition->Lock();
		printf("    datasize=%d\n", p->m_pData->size());
		p->m_pData->clear();
		p->m_pCondition->Unlock();
	}
};
//------------------------------------------------------------------------------


#endif //__JCThreadPool_H__

//-----------------------------END FILE--------------------------------
