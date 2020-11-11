/**
@file			ListNode.h
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#ifndef __ListNode_H__
#define __ListNode_H__

#include <stdio.h>

namespace laya
{
	class ListNode
    {
	public:
        ListNode* _Prev;
        ListNode* _Next;
		int id; // for debug
        ListNode(){
			_Prev=_Next=this;
			static int n=0;
			id=n++;
		}
		virtual ~ListNode(){
			//如果在链表上则删除
            ListNode* pPrev = _Prev;
            ListNode* pNext = _Next;
			pPrev->_Next = _Next;
			pNext->_Prev = _Prev;
			_Next = this;
			_Prev = this;
		}
		inline bool notInChain(){
			return _Prev==this && _Next==this;
		}
	};

    /** 
     * @brief 因为stl的list在删除对象的时候不太方便，需要遍历找到iterator，所以先自己写一个
    */
	class simpList
    {
	protected:
        ListNode* pHead;	//pHead只是表示头，不是有效内容
		int		nSize;
	public:
		simpList(){
			pHead = new ListNode();
			nSize = 0;
		}
		~simpList(){
			delete pHead;
			pHead = 0;
			nSize = 0;
		}
		int size(){
			return nSize;
		}
		//这个不要了使用了。因为与stl的标准不一致。
        ListNode* getHead(){
			return pHead;
		}
		//begin是有效值。除非整个list没有内容，那他等于end()
        ListNode* begin(){
			return pHead->_Next;
		}
		//end是无效值。就是指向pHead
        ListNode* end(){
			return pHead;
		}
		void push_back(ListNode* pNode ){
            ListNode* pTail = pHead->_Prev;
			if( pTail==pNode)
				return;
			pTail->_Next = pNode;
			pNode->_Prev = pTail;
			pNode->_Next = pHead;
			pHead->_Prev = pNode;
			nSize++;
		}
        ListNode* pop_back(){
            ListNode* pTail = pHead->_Prev;
            ListNode* pPrev = pTail->_Prev;
			//pnext = phead
			pPrev->_Next = pHead;
			pHead->_Prev = pPrev;
			pTail->_Next= pTail;
			pTail->_Prev= pTail;
			nSize--;
			if(nSize<0)nSize=0;
			return pTail;
		}
		void push_front(ListNode* pNode){
			if( pNode==pHead->_Next)
				return;
			pNode->_Next = pHead->_Next;
			pHead->_Next->_Prev=pNode;
			pHead->_Next=pNode;
			pNode->_Prev = pHead;

			nSize++;
		}
        ListNode* pop_front(){
            ListNode* pDel = pHead->_Next;
			pDel->_Next->_Prev = pHead;
			pHead->_Next = pDel->_Next;
			pDel->_Prev=pDel;
			pDel->_Next=pDel;
			nSize--;
			if(nSize<0)nSize=0;
			return pDel;
		}
		//返回下一个节点
        ListNode* delNode(ListNode* pNode){
			if( pNode->notInChain() )
				return NULL;
            ListNode* pPrev = pNode->_Prev;
            ListNode* pNext = pNode->_Next;
			pPrev->_Next = pNode->_Next;
			pNext->_Prev = pNode->_Prev;
			pNode->_Next = pNode;
			pNode->_Prev = pNode;
			nSize--;
			if(nSize<0)nSize=0;
			return pNext;
		}
		void clear(){
		}
	};
}
//------------------------------------------------------------------------------


#endif //__ListNode_H__

//-----------------------------END FILE--------------------------------
