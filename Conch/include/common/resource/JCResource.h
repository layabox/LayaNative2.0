/**
@file			JCResource.h
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#ifndef __JCResource_H__
#define __JCResource_H__

#include <functional>
#include <vector>
#include "../util/ListNode.h"

namespace laya
{
	class JCResManager;
	class JCResStateDispatcher
    {
	public:
		enum ResState
        {
			getting=0,
			ready=1,
			freed=2,
			error=3
		};
		enum ResError
        {
			noError=0,
			downloadError=1,
			decodeError=2
		};
	public:
		typedef std::function<void(JCResStateDispatcher*)>      ResOnReadyCB;
		typedef std::function<void(JCResStateDispatcher*,int)>  ResOnErrCB;

        JCResStateDispatcher();

		ResState getState(){return m_nResState;};

		//可能会触发回调。
		void setState( ResState p_state );

		//资源准备好之后的回调。可以调用多次，添加多个回调
		void setOnReadyCB(ResOnReadyCB cb );

		//资源出错了的回调。
		void setOnErrorCB(ResOnErrCB cb);

		void setOnFreeCB(ResOnReadyCB cb );

		virtual bool restoreRes(){return false;};

    public:

        //资源准备好了的回调。回调函数在执行之后就会被删除
        std::vector<ResOnReadyCB >	    m_allOnReadyCB,m_tmpOnReadyCB;
        std::vector<ResOnErrCB >	    m_allOnErrCB,m_tmpOnErrCB;
        std::vector<ResOnReadyCB >	    m_allOnFreeCB,m_tmpOnFreeCB;
        ResState		                m_nResState;	    //当前的状态。外部禁止修改
        int				                m_nErrNo;	        //错误代码
	};

	class JCResource :public JCResStateDispatcher, public ListNode
    {
		friend class JCResManager;
	public:
        JCResource();
		~JCResource();

		//本资源占用的容量。
		//这个值可能会变。
		//问题：如果直接通过manager.addMemSize则不用这个。但是
		//		用addMemSize的话，
		//			没有地方保存资源占用的大小，在add到资源管理器之前，无法进行加减操作
		//			需要到处进行加减的维护
		//		用 resSize 的话
		//			外部直接调用释放资源，无法通知到resMgr
		//			如果是固定大小，如果是异步资源，无法得到一致的大小，会导致问题。变化的大小没有问题。
		//virtual int     resSize(){return mnResSize;};

		//设置资源大小，如果为0的话，会被从活动列表中删除
		void setResSize(int sz);
		int getResSize(){ return m_nResSize;}

		//清理本资源占用的内存。此对象本身不会被释放。
		//p_bNeedUpdateSize 表示是否需要通知资源管理器自己状态发生变化了。如果是资源管理器调用的FreeRes，显然应该为false
		//					其他情况应该为true。
		//					这个参数不用了，外部调用者不应该关心占用空间的问题。如果关心的话，则自己再调用manager去管理
		//p_bNeedRestore 表示是否还需要再恢复。例如当整个系统析构的时候，就不再需要了。
		//					例如bitmap资源会在释放的时候生成缩略图。如果是系统要关闭了，则没有必要做这个了。
		virtual void freeRes( bool p_bNeedRestore ){};

		//恢复res的内容，这个是给ResMap使用的，
		//当获得成功后，要通知已有的回调
		//如果restore后增加占用内存，可能又会导致自己被释放掉。
		virtual bool restoreRes(){return false;};

		inline bool isActive(){ return !notInChain();}

		virtual void touch( bool p_bRestoreRes=false);

    public:
        int			    m_nTouch;	//上次活动时间
        int			    m_nUsedMem;	//当前在管理器中记录的大小。即上次统计的时候的大小。    
        int			    m_nResSize;	//当前的资源的大小。这个会被随时修改。
        JCResManager*   m_pResManager;
	};
}
//------------------------------------------------------------------------------


#endif //__JCResource_H__

//-----------------------------END FILE--------------------------------