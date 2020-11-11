/**
@file			JCEmitter.h
@brief			
@author			hugao
@version		1.0
@date			2016_5_11
*/
#ifndef __JCEmitter_H__
#define __JCEmitter_H__

#include "JCEventBase.h"
#include <thread>
#include <vector>
#include <map>
#include <mutex>

namespace laya
{
	class JCWorkerThread;
	/** 
	 * @brief 事件发生器。 事件id是并不是全局事件的唯一id，这个id是可以重复的，只有针对具体的emitter才有意义。
	*/
    class JCEventEmitter
    {
    public:
        enum evtexample {
            evtNumExample = 0
        };
        //因为事件不能传指针（会发生有人发送临时事件对象的情况），所以统一成智能指针
        typedef std::shared_ptr<JCEventBase> evtPtr;
        typedef std::function<void(evtPtr)> EventHandler;
        struct EvtHandlerPack {
            EventHandler func;
            JCWorkerThread* pThread;
            int			nID : 31;
            bool		bDel : 1;		//是否删除了
        };

    protected:
        struct EventData {
            std::vector<int> allListener;			//这个是为了保证正确的执行顺序。
            std::vector<EvtHandlerPack> stableListener;	//不删除就一直存在的。
            std::vector<EvtHandlerPack> onceListener; //一次性的。每次用完都会清理
                                                      //std::vector<int>		runData;//事件的运行时数据。例如是否还缓存在线程队列中。这样可以处理只缓存一个的情况
            int nDelNum;			//监听对象被删除的个数，当超过一定的时候需要清理
            EventData() {
                nDelNum = 0;
            }
        };
        typedef std::vector< EventData >	EventListener;
	
    public:
        JCEventEmitter();

        //例如根据this和函数指针来生成id，这样调用者就不用保存id了
        static int makeID(int, int);
		bool emit(evtPtr p_evt );
		bool emit(int p_nEvent, int p_nP1, int p_nP2 );

		//p_pInThread 表示在哪个线程中执行。如果不设置，则是立即回调的方式
		//id 用来唯一标识一个回调。
		bool on(int p_nEvent, EventHandler func, /*int id,*/ JCWorkerThread* p_pInThread=0);	//p_nEvent

		bool once(int p_nEvent, EventHandler func, JCWorkerThread* p_pInThread=0 );
		
		bool addEventListener( int p_nEvent, EventHandler p_Func, bool p_bOnce, JCWorkerThread* p_pInThread=0, int p_nPos=-1 );

		bool removeEventListener(int p_nEvent, int id );

		//删掉某个事件的所有监听者。
		void removeEventListener(int p_nEvent );

	protected:
		//把事件处理函数包装成线程处理函数
		void rewrapEventFunc(EventHandler& p_Func, evtPtr p_pEvt);
		void cleanDelListener();		//清理队列，去掉已经被删除的。

 //------------------------------------------------------------------------------
    public:
        static const int ONCEMASK = 0x8000;			//这个位为1则表示是一次性的。
        static const int IDXMASK = 0x7fff;			//取出位置的
		EventListener	m_AllListener;
		std::recursive_mutex	m_ListenerLock;	//同线程的不要阻塞
	};
};
//------------------------------------------------------------------------------


#endif //__JCEmitter_H__

//-----------------------------END FILE--------------------------------