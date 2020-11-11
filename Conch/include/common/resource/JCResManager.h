/**
@file			JCResManager.h
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#ifndef __JCResManager_H__
#define __JCResManager_H__

#include <vector>
#include <map>
#include <list>
#include <thread>
#include <mutex>
#include "JCResource.h"

#define SLEEPINGAGE		300

namespace laya
{
    /** 
     * @brief
        资源管理的基类。
        laya的资源管理方法介绍：
        manager主要管理两个东西，
        1 根据key（目前是url）来查找资源，这个随着资源的增加会一直增加，除非显式调用删除函数
        2 一个活动资源列表。所谓活动资源就是正常状态的资源。与之相对的是非活动资源，即主要内存已经释放，只保留壳的资源。
        这个列表是不完整的，不能用来查找资源
        几个典型问题：
        1.资源的创建
        1) 外部自己创建，通过addRes来添加到资源管理中
        2) 以通过manager的 getRes(url,true)来创建,同时会调用增加资源。
        为了避免使用模板（否则不知道new什么），先不用这种方法
        2.内存清理
        当add或者restore后的总内存超出上限了，就需要进行内存清理。为了效率，这里的内存清理不会实时进行。
        有的资源不希望删除，需要加一个bool canFree()的函数么？
        如果不希望删除，则不要添加到资源管理中。
        问题：
        map的key都是string么？可能是起来类型么。需要用模板来表示key么
        原来的mesh2d 是 url+textureGroup
        如果一个资源就超出了上限怎么办
        某个res增加内存的时候，不能把自己释放掉
        测试
        函数的多次调用
        可能会出的问题
        资源活动列表有多个相同的资源
    */
	class JCResManager
    {
    public:

        typedef JCResource*	                    ResType;
        typedef std::map<std::string, ResType>  ResMap;
        typedef std::vector<ResType>            ResVector;

	public:

        JCResManager(int p_nFreeSize,bool bUseMap);

		~JCResManager();

		//设置定期清理的大小
		void setFreeSize( unsigned int p_nFreeSize );

		void setFreeSize( unsigned int p_nFreeSize, unsigned int p_nMaxSize );

		//设置强制清理的大小
		void setMaxSize(int p_nSize)
        {
			m_nMaxSize = p_nSize;
		}

		unsigned int getFreeSize() { return m_nFreeSize; }

		unsigned int getMaxSize() { return m_nMaxSize; }

		//返回执行了多少次free的操作，用来做效率检测
		unsigned int getFreedCount() { return m_nFreedCount; }

		void resetFreeCount() { m_nFreedCount = 0; }

		/*
			查找一个资源。
			注意，为了效率不要频繁调用
			如果需要创建，则改名成 getRes
		*/
		ResType	find(const std::string& p_key );

        ResType find(int nKey)
        {
            return m_AllResVector[nKey];
        }

		//这个只有一个res资源，只能处理活动列表的问题，不能根据map来查找。
		//多次调用不会导致内存统计错误
		void add(ResType pRes);

		/*
			从活动列表中删除某个资源。
			并不影响map
			bDel true 表示不需要资源为恢复做些工作。即这个资源以后不会用到了
		*/
		bool freeRes(ResType pRes, bool bDel );

		/*
			从map中删除。如果 bDel为true，则同时删除对象。
			返回值：
				true 找到这个资源了
				false 没有这个资源
		*/
		bool delRes( const std::string& p_key, bool bDel );

        bool delRes(int p_key, bool bDel);

		//每帧都会调用的。
		//由于resManager不会太多，所以可以每帧都调用，不用用请求列表的方式。
		void tick();

		//删除所有的，这个不会有回调
		void destroyAll(bool bDelObj=false);

		//释放所有的。会有回调
		void freeAll();

		//返回实际释放的大小
		int freeRes(int p_nSize );

		/*
			pRes的大小变化了，需要重新统计。这个会引起touch
			这个操作不会导致pRes被释放。
		*/
		void updateRes(ResType pRes );

		/*
			加到map中。
			加到活动列表中。
		*/
		void setItem(ResType pRes, const char* name);

        void setItem(ResType pRes, int nID);

		/*
			只是加到map中
		*/
		void addToAllRes(ResType pRes, const char* name);

        void addToAllRes(ResType pRes, int nID);
		
		void touchRes( ResType pRes, bool restoreRes=false );

		void useSetResLock(bool b)
        {
			m_bUseSetResLock = b;
		}

		void chkThread(bool b)
        {
			m_bChkThread = b;
		}

		/*
			统计活动列表所有对象实际占用的资源大小，与资源管理器统计的资源大小是否一致。
			这个是测试用的，如果不一致，则会产生bug。
		*/
		bool chkMemSize();

		unsigned int count()
        {
			return m_bUseMap?m_AllRes.size():m_AllResVector.size();
		}

		/*
			设置当前线程为期望共工作线程。
			如果设置了chkThread(true)则当关键函数在另外的线程中工作的时候，会抛异常。
		*/
		void InitWorkThread();

        void printfActiveTextureInfo();

        /*
        当pRes恢复或者释放的时候都会调用到这个来更新占用内存
        */
        void updateSz(ResType pRes);

        int getGlobalID();

    public:
        bool                                    m_bUseMap;
        ResMap		                            m_AllRes;
        int                                     m_nGlobalID;
        ResVector                               m_AllResVector;
        typedef simpList					    ResList;
        ResList				                    m_ActiveRes;		//活动状态的资源列表。
        unsigned int		                    m_nCurTick;			//需要一个心跳。
        unsigned int		                    m_nNextCleanTick;	//下次清理的时间
        std::recursive_mutex                    m_Lock;			
        unsigned int                            m_nCurSize;         //当前占用的大小
        unsigned int		                    m_nFreeSize;        //触发间隔清理的阀值
        unsigned int                            m_nMaxSize;         //触发强制清理的阀值
        bool	                                m_bReleasing;
        int				                        m_nID;
    protected:

        std::recursive_mutex		            m_setResLock;
        bool						            m_bUseSetResLock;	//是否使用m_setResLock. 例如如果都在一个线程，则没必要使用lock，以免影响效率。

	protected:
		ResType		                            m_pUpdatingSzRes;	//请求进入的资源，清理资源的时候，不会去清理这个资源。
		bool		                            m_bChkThread;
		std::thread::id	                        m_WorkThread;
		ResType		                            m_pLastTouchedRes;	//记录上次touch的对象。目的是为了提高效率。例如使用大图集的人，会有很多次相邻的touch完全相同
		unsigned int			                m_nFreedCount;
	};

}
//------------------------------------------------------------------------------


#endif //__JCResManager_H__

//-----------------------------END FILE--------------------------------