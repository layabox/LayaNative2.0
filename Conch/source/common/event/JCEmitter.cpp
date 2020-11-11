/**
@file			JCEmitter.cpp
@brief			
@author			hugao
@version		1.0
@date			2016_5_11
*/

#include "../event/JCEmitter.h"
#include "../misc/JCWorkerThread.h"

namespace laya
{	
    JCEventEmitter::JCEventEmitter(){
		//继承类要在构造中分配 m_AllListener
		//例如 m_AllListener.resize(MyCls::evtNum);
	}

	//为了发给线程，需要这样一个函数。TODO 能不用这个转换么
	void JCEventEmitter::rewrapEventFunc(EventHandler& p_Func, evtPtr p_pEvt){
		p_Func(p_pEvt);
	}

	void JCEventEmitter::cleanDelListener(){
		m_ListenerLock.lock();
		int nSize = m_AllListener.size();
		//TODO 还没做呢
		m_ListenerLock.unlock();
	}

	//TODO 如果发送到进程，没有处理只缓存一个事件的情况
	bool JCEventEmitter::emit(evtPtr p_evt ){
		int nID = p_evt->m_nID;
		m_ListenerLock.lock();
		int nNum = (int)m_AllListener.size();
		m_ListenerLock.unlock();
		if( nID>nNum )
			throw -1;

		m_ListenerLock.lock();
		int& delNum = m_AllListener[nID].nDelNum;
		delNum = 0;
		std::vector<int>& allID = m_AllListener[nID].allListener;
		int curListenerNum = (int)allID.size();
		if( curListenerNum<=0){
			m_ListenerLock.unlock();
			return false;
		}

		std::vector<EvtHandlerPack>& curStableListeners = m_AllListener[nID].stableListener;
		int nStableNum = curStableListeners.size();
		//拷贝内容，以避免长时间阻塞
		std::vector<int> idxVector;
		idxVector.resize(nStableNum);
		for( int i=0; i<nStableNum; i++)
			idxVector[i]=i;
		swap(allID,idxVector);	//交换一下，新的idx vector只包含多次监听者的索引。

		std::vector<EvtHandlerPack> workStableListeners;
		workStableListeners.resize(nStableNum);
		for(int i=0; i<nStableNum; i++ )
			workStableListeners[i]=curStableListeners[i];	//这样可能比memcpy更可靠。能执行构造函数。
		//memcpy(&workStableListeners[0],&m_AllListener[nID].stableListener[0],nStableNum*sizeof(EvtHandlerPack));

		std::vector<EvtHandlerPack> workOnceListener;
		swap(workOnceListener,m_AllListener[nID].onceListener);
		m_ListenerLock.unlock();

		for( int i=0; i<curListenerNum; i++ ){
			int idx = allID[i];
			bool bOnce = (idx&ONCEMASK)!=0;
			idx &= IDXMASK;
			EvtHandlerPack& curHandler = bOnce?workOnceListener[idx]:workStableListeners[idx];
			if(curHandler.bDel){
				if(!bOnce) 
					delNum ++;
				continue;
			}
			if( curHandler.pThread ){//如果需要发送到另外的线程执行
				curHandler.pThread->post( std::bind(&JCEventEmitter::rewrapEventFunc,this, curHandler.func, p_evt));
			}else{
					curHandler.func(p_evt);
			}
			//workOnceListener.clear(); 不需要，本来就是临时的
		}

		if( delNum>20 )
			cleanDelListener();

		return curListenerNum>0;
	}

	bool JCEventEmitter::emit(int p_nEvent, int p_nP1, int p_nP2 ){
		throw -1;
		return false;
	}

	bool JCEventEmitter::on(int p_nEvent, EventHandler func, JCWorkerThread* p_pInThread){
		return addEventListener(p_nEvent, func, false, p_pInThread );
	}

	bool JCEventEmitter::once(int p_nEvent, EventHandler func, JCWorkerThread* p_pInThread ){
		return addEventListener( p_nEvent, func, true, p_pInThread );
	}
		
	bool JCEventEmitter::addEventListener( int p_nEvent, EventHandler p_Func, bool p_bOnce, JCWorkerThread* p_pInThread, int p_nPos ){
		m_ListenerLock.lock();
		int nSize = m_AllListener.size();
		m_ListenerLock.unlock();
		if( p_nEvent>nSize){
			printf("监听错误的对象\n");
			throw -1;
		}

		EvtHandlerPack evtPack;
		evtPack.pThread = p_pInThread;
		evtPack.func = p_Func;
		evtPack.bDel = false;
		//TODO p_nPos 还没有用

		m_ListenerLock.lock();
		if( p_bOnce ){
			int nPos = m_AllListener[p_nEvent].onceListener.size();
			m_AllListener[p_nEvent].allListener.push_back(nPos|ONCEMASK);
			m_AllListener[p_nEvent].onceListener.push_back(evtPack);
		}else{
			int nPos = m_AllListener[p_nEvent].stableListener.size();
			m_AllListener[p_nEvent].allListener.push_back(nPos);
			m_AllListener[p_nEvent].stableListener.push_back(evtPack);
		}
		m_ListenerLock.unlock();
		return false;
	}

	bool JCEventEmitter::removeEventListener( int p_nEvent, int id){// EventHandler p_Func ){
		m_ListenerLock.lock();
		int nSize = m_AllListener.size();
		m_ListenerLock.unlock();

		if( p_nEvent>=nSize)
			return false;

		m_ListenerLock.lock();
		std::vector<EvtHandlerPack>& curStableListener = m_AllListener[p_nEvent].stableListener;
		std::vector<EvtHandlerPack>& curOnceListener = m_AllListener[p_nEvent].onceListener;
		int& delNum = m_AllListener[p_nEvent].nDelNum;
		//
		for(int i=0,sz=curStableListener.size(); i<sz; i++){
			EvtHandlerPack& curHandler = curStableListener[i];
			if( id == curHandler.nID ){
				curHandler.bDel = true;
				delNum++;
			}
		}
		for(int i=0,sz=curOnceListener.size(); i<sz; i++){
			EvtHandlerPack& curHandler = curOnceListener[i];
			if( id == curHandler.nID ){
				curHandler.bDel = true;
				delNum++;
			}
		}

		m_ListenerLock.unlock();

		//清理
		if(delNum>20)
			cleanDelListener();

		return false;
	}

	void JCEventEmitter::removeEventListener(int p_nEvent ){
		m_ListenerLock.lock();
		int nSize = m_AllListener.size();
		m_ListenerLock.unlock();

		if( p_nEvent>=nSize)
			return ;
		m_ListenerLock.lock();
		m_AllListener[p_nEvent].allListener.clear();
		m_AllListener[p_nEvent].stableListener.clear();
		m_AllListener[p_nEvent].onceListener.clear();
		m_ListenerLock.unlock();
	}

};

//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
