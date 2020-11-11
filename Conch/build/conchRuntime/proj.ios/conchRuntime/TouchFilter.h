/**
@file			TouchFilter.h
@brief			
@author			wyw
@version		1.0
@date			2012_12_26
*/

#ifndef __TouchFilter_H__
#define __TouchFilter_H__

#include <stdio.h>
#include <string>

//定义状态的宏
#define PTSTATE_NOTOUCH 0
#define PTSTATE_DOWNSTILL 1
#define PTSTATE_MOVING 2

//touch 的开始  移动 结束
#define TOUCH_BEGAIN 0
#define TOUCHE_END 1
#define TOUCHE_MOVED 2
#define ACTION_POINTER_DOWN 5
#define ACTION_POINTER_UP 6

#define TOUCH_EVENT_SIZE  10

class IOSTouch
{
public:
	int m_nType;
	int m_nCount;
	int m_nID[TOUCH_EVENT_SIZE];
	int m_nX[TOUCH_EVENT_SIZE];
	int m_nY[TOUCH_EVENT_SIZE];
	IOSTouch()
	{
		m_nType = -1;
		m_nCount = 0;
		for( int i = 0 ; i < TOUCH_EVENT_SIZE ; i++ )
		{
			m_nX[i] = -100;
			m_nY[i] = -100;
			m_nID[i] = -1;
		}
	}
};

/** 
 * @brief touch定义
*/
class MyTouchPt
{
public:
	MyTouchPt()
	{
		m_nID = 0;
		m_nTouchState = PTSTATE_NOTOUCH;
		m_nLastMoveX = 0;
		m_nLastMoveY = 0;
	}
public:
	int m_nID;				//手指ID
	int m_nTouchState;		//touch的状态
	int m_nLastMoveX;		//保存信息 过滤用的
	int m_nLastMoveY;		//保存信息 过滤用的
};

/** 
 * @brief 
*/
class JCTouchFilter
{
public:

	/** @brief构造函数
	*/
	JCTouchFilter( void );

	/** @brief析构函数
	*/
	~JCTouchFilter( void );

public:

	MyTouchPt* GetTouchPt( int p_nID );

	void AddTouchPt(int p_nID, int p_nX, int p_nY );

	void DelTouchPt(int p_nID);

	void ClearTouchPt( void );

	int GetPointerNum( void );

	void onTouchEvent( IOSTouch& p_kTouch, float p_fRatio,int p_nViewOffset );

    void AdjustMoveRange( int p_iScale );
    
    void handleTouch( int type,int id,int x,int y );
    
protected:

	int			m_nMovRange;		//移动检测范围。move后，超过1秒钟没有超出此范围则认为静止了
	
	MyTouchPt* 	m_vTouches[TOUCH_EVENT_SIZE];		//数组

};

#endif //__TouchFilter_H__

//-----------------------------END FILE--------------------------------
