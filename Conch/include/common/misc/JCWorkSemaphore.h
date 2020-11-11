/**
@file			JCWorkSemaphore.h
@brief			
@author			hugao
@version		1.0
@date			2016_5_12
*/

#ifndef __JCWorkSemaphore_H__
#define __JCWorkSemaphore_H__

#include <mutex>
#include <condition_variable>

namespace laya
{
    /** 
     * @brief 定制的信号灯。主要用于流水线的同步。
    */
	class JCWorkSemaphore
    {
	public:
        JCWorkSemaphore();
		~JCWorkSemaphore();

		//返回false表示希望停止了
		bool waitUntilNoData();
		//返回false表示希望停止了
		bool waitUntilHasData();
		void setDataNum(int p_nNum);
        //这个get函数没有锁，调用请慎重考虑
        int getDataNum();
		void notifyAllWait();			//并不设置数据，只是让等待的对象解锁，以便继续下去，例如需要关闭线程的时候。
		//变量初始化为构造时候的状态。
		void reset();
		void acquire();
		int availablePermits();
		int release();
    protected:
        std::condition_variable	m_condHasData;
        int							m_nDataNum;
        bool						m_bStop;
        std::mutex				m_SemLock;
	};
}
//------------------------------------------------------------------------------

#endif //__JCWorkSemaphore_H__

//-----------------------------END FILE--------------------------------