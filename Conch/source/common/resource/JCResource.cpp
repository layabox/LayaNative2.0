/**
@file			JCResource.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#include "JCResource.h"
#include "JCResManager.h"

namespace laya{

	JCResStateDispatcher::JCResStateDispatcher()
    {
		m_nResState = getting;
		m_nErrNo = noError;
	}
    JCResource::JCResource()
    {
		m_pResManager = NULL;
		m_nUsedMem=0;
		m_nResSize =0;
		m_nTouch = 0;
	}
	JCResource::~JCResource()
    {
		if(m_pResManager && m_nUsedMem )
        {
			m_pResManager->freeRes(this,true);
		}
		m_pResManager = NULL;
	}
	/*
		注意：资源释放后再恢复，如果不做特殊处理，是不会再次出发onload的回调的。因为已经被删掉了。
	*/
	void JCResStateDispatcher::setState( JCResource::ResState p_state )
    {
		m_nResState = p_state;
		switch(p_state)
        {
        case getting:
            break;
		case ready:
        {
            swap(m_allOnReadyCB, m_tmpOnReadyCB);
            m_allOnReadyCB.clear();
            m_allOnErrCB.clear();
			int nNum = m_tmpOnReadyCB.size();
            for (int i = 0; i < nNum; i++){
                m_tmpOnReadyCB[i](this);
            }
            m_tmpOnReadyCB.clear();
        }
        break;
		case error:
        {
            swap(m_allOnErrCB, m_tmpOnErrCB);
            m_allOnErrCB.clear();
            m_allOnReadyCB.clear();
			int nNum = m_tmpOnErrCB.size();
			for( int i=0; i<nNum; i++){
                m_tmpOnErrCB[i](this,m_nErrNo);
			}
            m_tmpOnErrCB.clear();
		}
        break;
		//如果资源被释放了，则onload和onerror都会被清掉，在恢复的时候，不会再调到对应的函数了。
		//如果需要调用，就要在回调中再次添加监听
		case freed:
        {
            swap(m_allOnFreeCB, m_tmpOnFreeCB);
            m_allOnFreeCB.clear();
			int nNum = m_tmpOnFreeCB.size();
			for(int i=0; i<nNum; i++){
                m_tmpOnFreeCB[i](this);
			}
            m_tmpOnFreeCB.clear();
        }
        break;
		}
	}

	//资源准备好之后的回调。可以调用多次，添加多个回调
	void JCResStateDispatcher::setOnReadyCB( JCResource::ResOnReadyCB cb )
    {
		switch( m_nResState )
        {
		case ready:
			cb(this);
			break;
		case getting:
			m_allOnReadyCB.push_back(cb);
			break;
		case freed:
		case error://error的话要再次重试
			m_nResState = getting;
			m_allOnReadyCB.push_back(cb);
			restoreRes();	//完成后需要设置state? 现在如果需要恢复资源的话，调用者自己控制
							//还是这里做吧，要不总是容易忘了
							//文件资源 怎么办
			break;
		}
	}
	void JCResStateDispatcher::setOnFreeCB( JCResource::ResOnReadyCB cb )
    {
		switch( m_nResState ){
		case error:
			m_nResState = getting;
		case ready:
		case getting:
			m_allOnFreeCB.push_back(cb);
			break;
		case freed:
			cb(this);
			break;
		}
	}
	void JCResStateDispatcher::setOnErrorCB(ResOnErrCB cb)
    {
		switch( m_nResState ){
		case ready:
		case freed:
			break;
		case getting:
			m_allOnErrCB.push_back(cb);
			break;
		case error://问题，如果多次调用,例如想再重试一下，那直接调用并不是期望的
			m_nResState = getting;
			m_allOnErrCB.push_back(cb);
			restoreRes();
			//cb(this,mnErrNo );
			break;
		}
	}
	void JCResource::touch(bool p_bRestoreRes)
    {
		if(m_pResManager){
			m_pResManager->touchRes(this,p_bRestoreRes);
		}
	}
	void JCResource::setResSize(int sz)
    { 
		if(sz==m_nResSize)return;
		m_nResSize = sz; 
		if(m_pResManager){
			m_pResManager->updateRes(this);
		}
	}
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
