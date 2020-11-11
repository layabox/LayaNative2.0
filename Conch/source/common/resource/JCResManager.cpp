/**
@file			JCResManager.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#include "JCResManager.h"
#include "JCResource.h"
#include "../util/Log.h"
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>
namespace laya
{
    JCResManager::JCResManager(int p_nFreeSize,bool bUseMap)
    {
        m_nGlobalID = 0;
		m_nCurTick = rand()%SLEEPINGAGE;	//给一个随机值。避免所有的manager同时清理。
		m_nNextCleanTick = m_nCurTick+SLEEPINGAGE;
		m_nCurSize = 0;
		m_nFreeSize = p_nFreeSize;
		m_nMaxSize = 1*m_nFreeSize;//本来是可以有一个阀值的
		m_bUseSetResLock = false;
		m_bReleasing = false;
		m_nID = 0;
		m_pUpdatingSzRes = NULL;
		m_bChkThread = false;
		m_pLastTouchedRes=NULL;
		m_nFreedCount = 0;
        m_bUseMap = bUseMap;
	}

	JCResManager::~JCResManager(){
		m_bReleasing=true;
		destroyAll();
	}

	void JCResManager::setFreeSize( unsigned int p_nFreeSize )
	{
		m_nFreeSize = p_nFreeSize;
		m_nMaxSize = 1 * m_nFreeSize;//本来是可以有一个阀值的
	}

	void JCResManager::setFreeSize( unsigned int p_nFreeSize, unsigned int p_nMaxSize ){
		m_nFreeSize = p_nFreeSize;
		m_nMaxSize = p_nMaxSize;
	}

	//注意，为了效率不要频繁调用
	//如果需要创建，则改名成 getRes
	JCResManager::ResType	JCResManager::find(const std::string& p_key ){
		ResMap::iterator it = m_AllRes.find(p_key);
		if(it==m_AllRes.end()){
			return NULL;
			//TODO 需要在这里创建新的么。
			//TYPE pnew = createNew(p_key);
			//m_AllRes[p_key]=pnew;
			//return pnew;
		}else{
			//如果已经释放了，需要出发恢复流程？
			return (*it).second;
		}
	}
	//这个只有一个res资源，只能处理活动列表的问题，不能根据map来查找。
	void JCResManager::add(JCResManager::ResType pRes){
		if( m_bChkThread){
			if( m_WorkThread!=std::this_thread::get_id()){
				LOGE("JCResManager[%d] have been operated by other threads.", m_nID);
				throw -22;
			}
		}
		//为了省事，用一个lock，由于add不太频繁，所以应该问题不大
        std::unique_lock<std::recursive_mutex> _lock(m_Lock);
		if( pRes->isActive()){
			LOGE("Resources have been added!\n");
			return;
		}
		pRes->m_pResManager = this;
		pRes->m_nTouch=m_nCurTick;
		m_ActiveRes.push_back(pRes);
		m_pLastTouchedRes = pRes;
		updateSz(pRes);
	}

	bool JCResManager::freeRes(JCResManager::ResType pRes, bool bDel ){
		if( m_bChkThread){
			if( m_WorkThread!=std::this_thread::get_id()){
				LOGE("JCResManager[%d]  have been operated by other threads.", m_nID);
				throw -22;
			}
		}
		if( !pRes->isActive())
			return false;
        std::unique_lock<std::recursive_mutex> _lock(m_Lock);
		m_ActiveRes.delNode(pRes);
		m_nCurSize-=pRes->m_nUsedMem;
		pRes->m_nResSize = 0;
		pRes->m_nUsedMem = 0;
		pRes->freeRes(!bDel);
		return true;
	}

	/*
		从map中删除。如果 bDel为true，则同时删除对象。
		返回值：
			true 找到这个资源了
			false 没有这个资源
	*/
	bool JCResManager::delRes( const std::string& p_key, bool bDel ){
		//避免删除某个资源的时候，又调回来了。
		if (m_bReleasing)
			return true;
		if( m_bChkThread){
			if( m_WorkThread!=std::this_thread::get_id()){
				LOGE("JCResManager[%d] have been operated by other threads", m_nID);
				throw -22;
			}
		}
		ResMap::iterator it = m_AllRes.find(p_key);
		if(it!=m_AllRes.end()){
			(*it).second->m_nResSize = 0;
			freeRes( (*it).second, true );
			if(bDel){
				//DelFun df;
				delete ((*it).second);
			}
			m_AllRes.erase(it);
			return true;
		}
		return false;
	}

    bool JCResManager::delRes(int p_key, bool bDel) {
        //避免删除某个资源的时候，又调回来了。
        if (m_bReleasing)
            return true;
        if (m_bChkThread) {
            if (m_WorkThread != std::this_thread::get_id()) {
                LOGE("JCResManager[%d] have been operated by other threads", m_nID);
                throw - 22;
            }
        }
        if ((size_t)p_key < m_AllResVector.size())
        {
            ResType pRes = m_AllResVector[p_key];
            if (pRes == NULL) return false;
            pRes->m_nResSize = 0;
            freeRes(pRes, true);
            if (bDel) {
                //DelFun df;
                delete pRes;
            }
            m_AllResVector[p_key] = NULL;
            return true;
        }
        return false;
    }

	//每帧都会调用的。
	//由于resManager不会太多，所以可以每帧都调用，不用用请求列表的方式。
	void JCResManager::tick(){
		m_nCurTick++;
		if( m_nCurTick>=m_nNextCleanTick){
			if( m_nCurSize>m_nFreeSize )
				freeRes( (m_nCurSize-m_nFreeSize)*2 );
			m_nNextCleanTick+=SLEEPINGAGE;
		}
	}

	//删除所有的，这个不会有回调
	void JCResManager::destroyAll(bool bDelObj){
		m_bReleasing=true;
		freeAll();
        if (m_bUseMap)
        {
            if (bDelObj) {
                ResMap::iterator it = m_AllRes.begin();
                while (it != m_AllRes.end()) {
                    delete (*it).second;
                    it++;
                }
            }
            m_AllRes.clear();
        }
        else
        {
            if (bDelObj) {
                for (size_t i = 0; i < m_AllResVector.size(); i++)
                {
                    ResType pRes = m_AllResVector[i];
                    if (pRes)
                    {
                        delete pRes;
                    }
                }
            }
            m_AllResVector.clear();
        }
		m_nCurSize = 0;
        m_bReleasing=false;
        m_nGlobalID = 0;
	}

	//释放所有的。会有回调
	void JCResManager::freeAll(){
        std::unique_lock<std::recursive_mutex> _lock(m_Lock);
		if( m_ActiveRes.size()<=0)
			return ;
		ListNode* pHead = m_ActiveRes.getHead();
        ListNode* pCurNode = pHead->_Next;
		while( pCurNode!=pHead){//越往后越新
			JCResource* pRes = (JCResource*)pCurNode;
			pRes->m_nResSize=0;	//阻止freeRes里面再调用freeRes
			pRes->freeRes(!m_bReleasing);
			pRes->setState(JCResource::freed);
			ListNode* pNext = pCurNode->_Next;
			m_ActiveRes.delNode(pCurNode);
			pCurNode=pNext;
		}
        m_pLastTouchedRes = NULL;
		m_nCurSize=0;
	}

	//返回实际释放的大小
	int JCResManager::freeRes(int p_nSize ){
		if( m_bChkThread){
			if( m_WorkThread!=std::this_thread::get_id()){
				LOGE("JCResManager[%d] have been operated by other threads.", m_nID);
				throw -22;
			}
		}
		//TODO 这个应该有线程的问题
		m_nFreedCount++;
        std::unique_lock<std::recursive_mutex> _lock(m_Lock);
		if( m_ActiveRes.size()<=0)
			return 0;
		int oldnum = m_ActiveRes.size();
		int nFreeSize=0;
		//int nLastSize = m_nCurSize;
		//int nTargetSz = m_nCurSize-p_nSize;
		ListNode* pHead = m_ActiveRes.getHead();
        ListNode* pCurNode = pHead->_Next;
		//int nid=GetCurrentThreadId();
		while( pCurNode!=pHead){//越往后越新
			JCResource* pRes = (JCResource*)pCurNode;
            //先屏蔽了，因为在ios会出现资源管理恢复导致的黑屏闪烁一下问题
            //if (pRes->mnResSize <= 0)
            //{
            //    pCurNode = pCurNode->_Next;
            //    continue;
            //}
			if( pRes == m_pUpdatingSzRes){
				if(pCurNode->_Next==pHead)
					break;
				else{
					LOGE("被保护资源不在最后一个？");
					throw -1;
				}
			}
			nFreeSize+=pRes->m_nUsedMem;
			if(pRes->m_nUsedMem==0){
				//可能是目前的资源还没有加载完成，例如图片还没与解码完成。
			}
			pRes->m_nUsedMem = 0;
			pRes->m_nResSize = 0;
			pRes->freeRes(true);
			if (pRes == m_pLastTouchedRes)
				m_pLastTouchedRes = NULL;
			pRes->setState(JCResource::freed);

			ListNode* pNext = pCurNode->_Next;
			m_ActiveRes.delNode(pCurNode);
			pCurNode=pNext;
			//if( m_nCurSize<=nTargetSz )
			//	break;
			if(nFreeSize>=p_nSize){
				break;
			}
		}
		m_nCurSize-=nFreeSize;
		LOGI("freeRes(%d):Total:%d,left:%d,clearedMem:%d\n", m_nID, oldnum, m_ActiveRes.size(),nFreeSize);
		return nFreeSize;
		//return (nLastSize-m_nCurSize);
	}

	/*
		加到map中。
		加到活动列表中。
	*/
	void JCResManager::setItem(JCResManager::ResType pRes, const char* name){
		if( m_bUseSetResLock )	m_setResLock.lock();
		if(pRes->m_pResManager==NULL){
			pRes->m_pResManager=this;
		}
		if(name)
			m_AllRes[name]=pRes;
		if( pRes->getResSize()>0){
			m_pUpdatingSzRes = pRes;	//防止被删除。
			touchRes(pRes);
			updateSz(pRes);
			m_pUpdatingSzRes = NULL;
		}
		if( m_bUseSetResLock )	m_setResLock.unlock();
	}

    void JCResManager::setItem(JCResManager::ResType pRes, int nID) {
        if (m_bUseSetResLock)	m_setResLock.lock();
        if (pRes->m_pResManager == NULL) {
            pRes->m_pResManager = this;
        }
        //放入vector
        int nSize = (int)m_AllResVector.size();
        if (nID == nSize)
        {
            m_AllResVector.push_back(pRes);
        }
        else if (nID < nSize)
        {
            if (m_AllResVector[nID] == NULL)
            {
                m_AllResVector[nID] = pRes;
            }
            else
            {
                LOGE("JCResManager::addToAllRes error m_vRes[%d] != NULL", nID);
            }
        }
        else
        {
            int nOldSize = m_AllResVector.size();
            m_AllResVector.resize(nID + 1);
            m_AllResVector[nID] = pRes;
        }
        if (pRes->getResSize() > 0) {
            m_pUpdatingSzRes = pRes;	//防止被删除。
            touchRes(pRes);
            updateSz(pRes);
            m_pUpdatingSzRes = NULL;
        }
        if (m_bUseSetResLock)	m_setResLock.unlock();
    }
	void JCResManager::addToAllRes(JCResManager::ResType pRes, const char* name){
		if( m_bUseSetResLock )	m_setResLock.lock();
		if(name)
			m_AllRes[name]=pRes;
		if( m_bUseSetResLock )	m_setResLock.unlock();
	}
    void JCResManager::addToAllRes(ResType pRes, int nID)
    {
        if (m_bUseSetResLock)	m_setResLock.lock();
        int nSize = (int)m_AllResVector.size();
        if (nID == nSize)
        {
            m_AllResVector.push_back(pRes);
        }
        else if (nID < nSize)
        {
            if (m_AllResVector[nID] == NULL)
            {
                m_AllResVector[nID] = pRes;
            }
            else
            {
                LOGE("JCResManager::addToAllRes error m_vRes[%d] != NULL", nID);
            }
        }
        else
        {
            int nOldSize = m_AllResVector.size();
            m_AllResVector.resize(nID + 1);
            m_AllResVector[nID] = pRes;
        }
        if (m_bUseSetResLock)	m_setResLock.unlock();
    }
	void JCResManager::touchRes( JCResManager::ResType pRes, bool restoreRes ){
		if( m_bChkThread){
			if( m_WorkThread!=std::this_thread::get_id()){
				LOGE("JCResManager[%d] have been operated by other threads.", m_nID);
				throw -22;
			}
		}
		pRes->m_nTouch = m_nCurTick;
		if(m_pLastTouchedRes==pRes){
			if( restoreRes && pRes->getState()==JCResource::freed ){
				pRes->restoreRes();
			}
			return;
		}
		m_pLastTouchedRes = pRes;

		if( m_bUseSetResLock )	m_setResLock.lock();
		m_ActiveRes.delNode(pRes);
		m_ActiveRes.push_back(pRes);
		if (!pRes->isActive())
			add(pRes);
		//问题：如果资源还没准备好怎么办
		if( restoreRes && pRes->getState()==JCResource::freed ){
			pRes->restoreRes();
		}
		if( m_bUseSetResLock )	m_setResLock.unlock();
	}

	void JCResManager::updateRes(JCResManager::ResType pRes ){
		if( m_bUseSetResLock )  m_setResLock.lock();
		if( pRes->m_nResSize==0){
			//freeRes(pRes,m_bReleasing);		这个太容易引起混乱。例如freeRes->setResSize(0)->updateRes()->freeRes()...
		}else{
			m_pUpdatingSzRes = pRes;
			touchRes(pRes);	//先把这个资源放到最后。
			updateSz(pRes);	//然后在清理资源。这样就会满足本资源是保护资源，并且在队列的最后，就能不被删除了。	
			m_pUpdatingSzRes = NULL;
		}
		if( m_bUseSetResLock )  m_setResLock.unlock();
	}

	void JCResManager::updateSz(ResType pRes){
		int size = pRes->m_nResSize;// resSize();
		int ds = size - pRes->m_nUsedMem;
		m_nCurSize += ds;
		pRes->m_nUsedMem = size;
		pRes->m_nTouch = m_nCurTick;	//类似touch，如果是restore了，这个会保证自己不会被轻易释放
		//如果ds<=0表示正在清理，这时候不要再freeRes了
		if( ds>0 && m_nMaxSize>0 && m_nCurSize>m_nMaxSize ){
			freeRes(m_nMaxSize/3);
		}
	}

	void JCResManager::InitWorkThread(){
		m_WorkThread = std::this_thread::get_id();
	}

	bool JCResManager::chkMemSize(){
        std::unique_lock<std::recursive_mutex> _lock(m_Lock);
		if( m_ActiveRes.size()<=0)
			return m_nCurSize==0;
		ListNode* pHead = m_ActiveRes.getHead();
        ListNode* pCurNode = pHead->_Next;
		int nMemSize=0;
		while( pCurNode!=pHead){//越往后越新
			JCResource* pRes = (JCResource*)pCurNode;
			nMemSize += pRes->m_nResSize;
			ListNode* pNext = pCurNode->_Next;
			pCurNode=pNext;
		}
		return m_nCurSize==nMemSize;
	}
    int JCResManager::getGlobalID()
    {
        m_setResLock.lock();
        int nID = m_nGlobalID++;
        m_setResLock.unlock();
        return nID;
    }
    void JCResManager::printfActiveTextureInfo()
    {
        LOGI("JCResManager::printfActiveTextureInfo currentSize=%d", m_nCurSize );
        /*
        listNode* pHead = m_ActiveRes.getHead();
        listNode* pCurNode = pHead->_Next;
        int nMemSize = 0;
        while (pCurNode != pHead) 
        {
            LayaTexture* pRes = (LayaTexture*)pCurNode;
            LOGI("JCResManager printfInfo id=%d gpuID=%d type=%d width=%d height=%d textureWidth=%d textureHeight=%d size=%d", pRes->getTextureID(),pRes->m_pGpuTexture, pRes->m_nType,pRes->getWidth(),pRes->getHeight(), pRes->getTextureWidth(), pRes->getTextureHeight(), pRes->getTextureWidth() * pRes->getTextureHeight() * 4 );
            listNode* pNext = pCurNode->_Next;
            pCurNode = pNext;
        }
        */
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
