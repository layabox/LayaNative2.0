/**
@file			JCLayaThreadPool.h
@brief			
@author			hugao
@version		1.0
@date			2016_5_12
*/

#ifndef __JCLayaThreadPool_H__
#define __JCLayaThreadPool_H__


#include <thread>
#include <vector>
#include <list>
#include <functional>
#include "JCWorkSemaphore.h"
//#include "conchDebugThread.h"
#include "../util/JCCommonMethod.h"
#include <mutex>

#ifdef WIN32
extern void SetNameInternal(unsigned int thread_id, const char* name);
#elif ANDROID
#include <sys/syscall.h>  
#include <unistd.h>
#define gettidv1() syscall(__NR_gettid)  
#define gettidv2() syscall(SYS_gettid)  
#elif __APPLE__
#include <pthread.h>
#endif


namespace laya
{
#ifdef __APPLE__
	extern pthread_key_t s_tls_curDataThread;
#else
    extern thread_local long*	s_tls_curDataThread;
#endif
	void ___datathread_onthreadstart(const char* threadname);
	void ___datathread_onthreadstop(const char* threadname);
	template<class _Tp>
    /** 
      * @brief  管理多个工作线程。所有线程的工作任务类似。
              * 这个是用来代替原来的threadpool的。因为原来的已经看不懂了，不好维护。
              * TODO 临时写的，功能设计有些混乱。以后再整理
    */
	class JCDataThread{
	public:
		typedef _Tp								_DataType;
		typedef std::function<void(void)>		ThreadFunc;
		typedef std::list<_DataType>			__DataQueue;
		typedef typename __DataQueue::iterator	__DataQueueIter;

	public:
        JCDataThread(){
			m_pThread = NULL;
			m_nThreadNo=0;
		}
		~JCDataThread(){
			Stop();
		}

		bool Start( ThreadFunc p_pfnThreadFunc ){
			if(m_pThread){
				Stop();
			}
			m_ThreadFunc = p_pfnThreadFunc;
			//TODO m_Semaphore 的stop标记现在不是原子的。
			//std::interprocess::ipcdetail::atomic_write32( &m_bWantToStop, 0 );
			m_pThread = new std::thread(std::bind(&JCDataThread::__ThreadEntry,this));
			if( 0 == m_pThread )
				return false;
			return true;
		}

        int getNum() {
            int num = 0;
            {
                m_Mutex.lock();
                num = m_Datas.size();
                m_Mutex.unlock();
            }
            return num;
        }

		void SendData( _DataType p_Data ){
			int num =0;
			{
                std::unique_lock<std::recursive_mutex> _lock(m_Mutex);
				m_Datas.push_back(p_Data);
				num=m_Datas.size();
			}
			m_Semaphore.setDataNum(num);
		}

		bool WaitData( _DataType *p_pData ){
			m_Semaphore.waitUntilHasData();
			{
                std::unique_lock<std::recursive_mutex> _lock(m_Mutex);
				if(m_Datas.size()>0){
					*p_pData = m_Datas.front();
					m_Datas.pop_front();
					m_Semaphore.setDataNum(m_Datas.size());
					return true;
				}
			}
			return false;
		}

		void Stop(){
			if(!m_pThread)
				return;
			m_Semaphore.notifyAllWait();
			m_pThread->join();
			m_Semaphore.reset();
			delete m_pThread;
			m_pThread = NULL;
		}

		/**
		* 多次尝试join
		* 返回true表示成功join了
		* timeout 每次尝试的超时时间，单位是毫秒
		* traStopWorker ： 返回true表示希望停止尝试。参数是当前尝试次数和用户定义参数
		*/
		typedef bool (*tryStopWorker)(int,void*);
		bool tryStop(int timeout, tryStopWorker func, void* userdata) {
            return false;
		}

		void ClearTask(){
            std::unique_lock<std::recursive_mutex> _lock(m_Mutex);
			m_Datas.clear();
		}

		//必须在启动线程前设置。
		void setName(const char* p_pszName){
			m_strName = p_pszName;
		}
	protected:
		void __ThreadEntry(){
#ifdef __APPLE__
            pthread_setspecific(s_tls_curDataThread,(void*)this);
#else
			s_tls_curDataThread = (long*)this;
#endif
			___datathread_onthreadstart(m_strName.c_str());
			m_ThreadFunc();
			___datathread_onthreadstop(m_strName.c_str());
		}
		//std::uint32_t m_bWantToStop;
		JCWorkSemaphore	m_Semaphore;
		std::recursive_mutex m_Mutex;
		__DataQueue		m_Datas;
		ThreadFunc		m_ThreadFunc;
		std::thread*	m_pThread;
		std::string		m_strName;
	public:
		int				m_nThreadNo;
	};

	template<class _Tp>
	class JCThreadPool{
	public:
		typedef _Tp		            _DataType;
		typedef JCDataThread<_Tp>   dtthread;
        JCThreadPool(){
			m_nThreadNum = 0;
			m_AllThread = 0;
		}

		~JCThreadPool(){
			stop();
		}

		void init(int p_nThread,std::function<void(void)> p_fun){
			stop();
			{
                std::unique_lock<std::recursive_mutex> _lock(m_Mutex);
				m_nThreadNum=p_nThread;
				m_AllThread = new dtthread* [m_nThreadNum];
				for( int i=0; i<m_nThreadNum; i++){
					m_AllThread[i] = new dtthread();
					m_AllThread[i]->m_nThreadNo=i;
					m_AllThread[i]->setName(m_strThreadName.c_str());
					m_AllThread[i]->Start(p_fun);
				}
			}
		}

        //发送到任务最少的线程中
        bool sendToThread(_DataType p_dt) {
            if (m_nThreadNum <= 0)
                return false;
            m_Mutex.lock();
            if (m_nThreadNum == 1) {
                m_AllThread[0]->SendData(p_dt);
            }
            else {
                int minv = -1;
                int mini = -1;
                for (int i = 0; i < m_nThreadNum; i++) {
                    int n = m_AllThread[i]->getNum();
                    if (minv<0 || n < minv) {
                        minv = n;
                        mini = i;
                    }
                }
                if (mini >= 0) {
                    m_AllThread[mini]->SendData(p_dt);
                }
                else {
                    m_Mutex.unlock();
                    return false;
                }
            }
            m_Mutex.unlock();
            return true;
        }

		bool sendToThread(_DataType p_dt, int p_nThread){
            std::unique_lock<std::recursive_mutex> _lock(m_Mutex);
			if(p_nThread>=m_nThreadNum){
				return false;
			}
			m_AllThread[p_nThread]->SendData(p_dt);
			return true;
		}

		void stop(){
            std::unique_lock<std::recursive_mutex> _lock(m_Mutex);
			for( int i=0; i<m_nThreadNum; i++){
				m_AllThread[i]->Stop();
				delete m_AllThread[i];
			}
			delete [] m_AllThread;
			m_AllThread = NULL;
			m_nThreadNum = 0;
		}

		bool waitData(_DataType* pData ){
#ifdef __APPLE__
            dtthread* pThread=(dtthread*)pthread_getspecific(s_tls_curDataThread);
            return pThread->WaitData(pData);
#else
			dtthread* pThread = (dtthread*)s_tls_curDataThread;
			return pThread->WaitData(pData);
#endif
		}

		void* getCurThread(){
#ifdef __APPLE__
            dtthread* pThread=(dtthread*)pthread_getspecific(s_tls_curDataThread);
            return pThread;
#else
			dtthread* pThread = (dtthread*)s_tls_curDataThread;
			return pThread;
#endif
		}

		int getCurThreadNo(void* p_pThread){
			dtthread* pThread=(dtthread*)p_pThread;
			return pThread->m_nThreadNo;
		}

		bool waitData(void* p_pThread, _DataType* pData){
			dtthread* pThread=(dtthread*)p_pThread;
			return pThread->WaitData(pData);
		}

		int getThreadNum(){ return m_nThreadNum;}

		void ClearDataOfThread(int i){
			if(i>=m_nThreadNum)
				return;
			{
                std::unique_lock<std::recursive_mutex> _lock(m_Mutex);
				m_AllThread[i]->ClearTask();
			}
		}

		void setThreadName(const char* p_pszName){
			m_strThreadName = p_pszName;
		}

	protected:
		std::recursive_mutex	m_Mutex;
		dtthread**				m_AllThread;
		int						m_nThreadNum;
		std::string				m_strThreadName;
	};
};
//------------------------------------------------------------------------------


#endif //__JCLayaThreadPool_H__

//-----------------------------END FILE--------------------------------
