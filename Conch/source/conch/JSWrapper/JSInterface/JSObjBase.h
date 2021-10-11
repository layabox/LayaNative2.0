
#ifndef __JSOBJECT_BASE_NODE_H___
#define __JSOBJECT_BASE_NODE_H___

#include <vector>
#include <string>
#include <functional>

namespace laya 
{

    class JCListNode
    {
    public:
        JCListNode* _Prev;
        JCListNode* _Next;
        int id; // for debug
        JCListNode() {
            _Prev = _Next = this;
            static int n = 0;
            id = n++;
        }
        virtual ~JCListNode() {
            //如果在链表上则删除
            JCListNode* pPrev = _Prev;
            JCListNode* pNext = _Next;
            pPrev->_Next = _Next;
            pNext->_Prev = _Prev;
            _Next = this;
            _Prev = this;
        }
        inline bool notInChain() {
            return _Prev == this && _Next == this;
        }
    };

    /**
    * @brief 因为stl的list在删除对象的时候不太方便，需要遍历找到iterator，所以先自己写一个
    */
    class JCSimpList
    {
    protected:
        JCListNode* pHead;	//pHead只是表示头，不是有效内容
        int		nSize;
    public:
        JCSimpList() {
            pHead = new JCListNode();
            nSize = 0;
        }
        ~JCSimpList() {
            delete pHead;
            pHead = 0;
            nSize = 0;
        }
        int size() {
            return nSize;
        }
        //这个不要了使用了。因为与stl的标准不一致。
        JCListNode* getHead() {
            return pHead;
        }
        //begin是有效值。除非整个list没有内容，那他等于end()
        JCListNode* begin() {
            return pHead->_Next;
        }
        //end是无效值。就是指向pHead
        JCListNode* end() {
            return pHead;
        }
        void push_back(JCListNode* pNode) {
            JCListNode* pTail = pHead->_Prev;
            if (pTail == pNode)
                return;
            pTail->_Next = pNode;
            pNode->_Prev = pTail;
            pNode->_Next = pHead;
            pHead->_Prev = pNode;
            nSize++;
        }
        JCListNode* pop_back() {
            JCListNode* pTail = pHead->_Prev;
            JCListNode* pPrev = pTail->_Prev;
            //pnext = phead
            pPrev->_Next = pHead;
            pHead->_Prev = pPrev;
            pTail->_Next = pTail;
            pTail->_Prev = pTail;
            nSize--;
            if (nSize<0)nSize = 0;
            return pTail;
        }
        void push_front(JCListNode* pNode) {
            if (pNode == pHead->_Next)
                return;
            pNode->_Next = pHead->_Next;
            pHead->_Next->_Prev = pNode;
            pHead->_Next = pNode;
            pNode->_Prev = pHead;

            nSize++;
        }
        JCListNode* pop_front() {
            JCListNode* pDel = pHead->_Next;
            pDel->_Next->_Prev = pHead;
            pHead->_Next = pDel->_Next;
            pDel->_Prev = pDel;
            pDel->_Next = pDel;
            nSize--;
            if (nSize<0)nSize = 0;
            return pDel;
        }
        //返回下一个节点
        JCListNode* delNode(JCListNode* pNode) {
            if (pNode->notInChain())
                return NULL;
            JCListNode* pPrev = pNode->_Prev;
            JCListNode* pNext = pNode->_Next;
            pPrev->_Next = pNode->_Next;
            pNext->_Prev = pNode->_Prev;
            pNode->_Next = pNode;
            pNode->_Prev = pNode;
            nSize--;
            if (nSize<0)nSize = 0;
            return pNext;
        }
        void clear() {
        }
    };

    struct JsObjClassInfo {
        const char*				ClassName;
        const JsObjClassInfo*	ParentClass;
        const unsigned int		ClsID;
    };
    /*
        js对象对应的c端对象。
        所有导出给js的对象都要从这个继承
        并且在创建和删除的时候进行管理
    */
    class JSObjNode :public JCListNode {
    public:
        static JsObjClassInfo   JSCLSINFO;
        static JCSimpList*      s_pListJSObj;			//属于本线程的所有的js对象

        JSObjNode();
        virtual ~JSObjNode();				//这个会引入虚函数表，不知道有没有问题
        const JsObjClassInfo*		m_pClsInfo;// const char* m_pClassName;
    };
    extern unsigned int gnJSClsID;
    #define ADDJSCLSINFO(cls,par)	JsObjClassInfo cls::JSCLSINFO = { #cls,&par::JSCLSINFO,gnJSClsID++};
}
#endif

//-----------------------------END FILE--------------------------------