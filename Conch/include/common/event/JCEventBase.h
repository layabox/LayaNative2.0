/**
@file			JCEventBase.h
@brief			
@author			hugao
@version		1.0
@date			2016_5_11
*/

#ifndef __JCEventBase_H__
#define __JCEventBase_H__

namespace laya
{
	class JCEventBase
    {
	public:
		enum
        {
			HANDLE_ONCE=0x1,//只处理一次，不会派发给队列中的其他监听者。好像没有为n的需求。
			ISCALLBACK=0x2,	//立即回调。不压队列
			UNIQUE=0x4,		//排他性，在队列中只有一个。
			NEEDDELDATA=0x5,	//m_pData需要释放
		};

    public:
        JCEventBase();
		~JCEventBase();
        void setDataAsPtr(void* pData) { m_pData = pData; m_nFlags |= NEEDDELDATA; };
		void setDataAsInt(int data){ m_pData = (void*)data; m_nFlags&=(~NEEDDELDATA);}
		void* getData(){return m_pData; }
    public:
        int					m_nID;
        char*				m_pName;
        unsigned int		m_nFlags;
        char				m_nPriority;	//优先级
        int					m_nP1, m_nP2;
	protected:
		void*				m_pData;		//外面分配，需要自己释放。

	};
};
//------------------------------------------------------------------------------


#endif //__JCEventBase_H__

//-----------------------------END FILE--------------------------------