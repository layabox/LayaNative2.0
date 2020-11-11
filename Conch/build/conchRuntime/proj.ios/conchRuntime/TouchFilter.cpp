/**
@file			TouchFilter.cpp
@brief			
@author			wyw
@version		1.0
@date			2012_12_26
*/


//包含头文件
#include "TouchFilter.h"
#include "../../../../source/conch/JSWrapper/LayaWrap/JSInput.h"
#include "../../../../source/conch/JCScriptRuntime.h"
#include <util/Log.h>

static const int __iMoveRangeDefault = 10;
//------------------------------------------------------------------------------
JCTouchFilter::JCTouchFilter( void )
{
	for( int i=0; i < TOUCH_EVENT_SIZE; i++ )
	{
		m_vTouches[i] = new MyTouchPt();
		m_vTouches[i]->m_nID=-1;
		m_vTouches[i]->m_nLastMoveX = 0;
		m_vTouches[i]->m_nLastMoveY = 0;
		m_vTouches[i]->m_nTouchState = PTSTATE_NOTOUCH;
	}
    m_nMovRange=__iMoveRangeDefault;
}
//------------------------------------------------------------------------------
JCTouchFilter::~JCTouchFilter( void )
{
	for( int i=0; i < TOUCH_EVENT_SIZE; i++ )
	{
		if ( m_vTouches[i] != NULL )
		{
			delete m_vTouches[i];
			m_vTouches[i] = NULL;
		}
	}
}

void JCTouchFilter::AdjustMoveRange( int p_iScale )
{
    m_nMovRange = __iMoveRangeDefault * p_iScale;
    if( m_nMovRange < __iMoveRangeDefault )
        m_nMovRange = __iMoveRangeDefault;
}
//------------------------------------------------------------------------------
MyTouchPt* JCTouchFilter::GetTouchPt( int p_nID )
{
	for( int i = 0; i < TOUCH_EVENT_SIZE; i++ )
	{
		if( m_vTouches[i]->m_nID<0 )
		{
			continue;
		}
		if( m_vTouches[i]->m_nID==p_nID )
		{
			return m_vTouches[i];
		}
	}
	return NULL;
}
//------------------------------------------------------------------------------
void JCTouchFilter::AddTouchPt(int p_nID, int p_nX, int p_nY )
{
	for( int i = 0; i < TOUCH_EVENT_SIZE; i++ )
	{
		if( m_vTouches[i]->m_nID < 0 )
		{
			m_vTouches[i]->m_nID = p_nID;
			m_vTouches[i]->m_nLastMoveX = p_nX;
			m_vTouches[i]->m_nLastMoveY = p_nY;
			m_vTouches[i]->m_nTouchState = PTSTATE_DOWNSTILL;
			break;
		}
	}
}
//------------------------------------------------------------------------------
void JCTouchFilter::DelTouchPt(int p_nID)
{
	for( int i = 0; i < TOUCH_EVENT_SIZE; i++ )
	{
		if( m_vTouches[i]->m_nID < 0 )
		{
			continue;
		}
		if( m_vTouches[i]->m_nID == p_nID )
		{
			m_vTouches[i]->m_nID = -1;
			m_vTouches[i]->m_nLastMoveX = 0;
			m_vTouches[i]->m_nLastMoveY = 0;
			m_vTouches[i]->m_nTouchState = PTSTATE_NOTOUCH;
		}
	}
}
//------------------------------------------------------------------------------
void JCTouchFilter::ClearTouchPt( void )
{
	for( int i=0; i < TOUCH_EVENT_SIZE; i++ )
	{
		m_vTouches[i]->m_nID = -1;
		m_vTouches[i]->m_nLastMoveX = 0;
		m_vTouches[i]->m_nLastMoveY = 0;
	}
}
//------------------------------------------------------------------------------
int JCTouchFilter::GetPointerNum( void )
{
	int nSum = 0;
	for( int i=0; i<TOUCH_EVENT_SIZE; i++)
	{
		if( m_vTouches[i]->m_nID >=0)
		{
			nSum++;
		}
	}
	return nSum;
}
//------------------------------------------------------------------------------
void JCTouchFilter::onTouchEvent( IOSTouch& p_kTouch, float p_fRatio,int p_nViewOffset )
{
	int pointerCount = p_kTouch.m_nCount;
    for( int i = 0; i < pointerCount; i++ )
    {
        int action = p_kTouch.m_nType;
        int id = p_kTouch.m_nID[i];
        int nx = p_kTouch.m_nX[i];
        nx *= p_fRatio;
        if( nx<0 ) nx = 0;
        int ny = p_kTouch.m_nY[i];
        ny *= p_fRatio;
        if( ny<0 ) ny = 0;
        
        bool bIgnoreCurEvent=false;
        if( action == TOUCH_BEGAIN )
        {
            AddTouchPt(id, nx, ny);
        }
        else if( action == TOUCHE_END )
        {
            ClearTouchPt();
        }
        else if( action == TOUCHE_MOVED )
        {
            MyTouchPt* curTouch = GetTouchPt(id);
            if( curTouch != NULL )
            {
                if( nx == curTouch->m_nLastMoveX && ny == curTouch->m_nLastMoveY )
                {
                    bIgnoreCurEvent = true;
                }
                else
                {
                    if( curTouch->m_nTouchState == PTSTATE_DOWNSTILL )
                    {
                        if(	abs( nx-curTouch->m_nLastMoveX ) > m_nMovRange || abs( ny-curTouch->m_nLastMoveY ) > m_nMovRange )
                        {
                            curTouch->m_nLastMoveX = nx;
                            curTouch->m_nLastMoveY = ny;
                            curTouch->m_nTouchState = PTSTATE_MOVING;
                        }
                        else
                        {
                            bIgnoreCurEvent = true;
                        }
                    }
                    else if( curTouch->m_nTouchState== PTSTATE_MOVING )
                    {
                        curTouch->m_nLastMoveX = nx;
                        curTouch->m_nLastMoveY = ny;
                    }
                }
            }
            else 
            {
                //LOG
            }
        }
        else if( action == ACTION_POINTER_DOWN )
        {
            AddTouchPt(id, nx, ny);
        }
        else if( action == ACTION_POINTER_UP )
        {
            DelTouchPt(id);
        }

        if( !bIgnoreCurEvent )
        {
            handleTouch( action, id>10?0:id, nx, ny + p_nViewOffset );
        }
    }
	
}
void JCTouchFilter::handleTouch( int type,int id,int x,int y )
{
    switch(type)
    {
    case TOUCH_BEGAIN:
        {
            laya::inputEvent e;
            e.nType = laya::E_ONTOUCHSTART;
            e.nTouchType = type;
            e.posX = x;
            e.posY = y;
            e.id = id;
            strncpy(e.type, "touchstart", 256 );
            //LOGE( "=================action_down=%d",id );
            laya::JSInput::getInstance()->activeCall(e);
        }
        break;
    case TOUCHE_END:
        {
            laya::inputEvent e;
            e.nType = laya::E_ONTOUCHEND;
            e.nTouchType = type;
            e.posX = x;
            e.posY = y;
            e.id = id;
            //LOGE( "=================action_UP=%d",id );
            strncpy(e.type, "touchend", 256 );
            laya::JSInput::getInstance()->activeCall(e);
        }
        break;
    case TOUCHE_MOVED:
        {
            laya::inputEvent e;
            e.nType = laya::E_ONTOUCHMOVE;
            e.nTouchType = type;
            e.posX = x;
            e.posY = y;
            e.id = id;
            //LOGE( "=================move id=%d,posX=%d,posY=%d",id,x,y );
            strncpy(e.type, "touchmove", 256 );
            laya::JSInput::getInstance()->activeCall(e);
        }
        break;
    case ACTION_POINTER_DOWN:
        {
            laya::inputEvent e;
            e.nType = laya::E_ONACTION_POINTER_DOWN;
            e.nTouchType = type;
            e.posX = x;
            e.posY = y;
            e.id = id;
            strncpy(e.type, "touchstart", 256 );
            //LOGE( "=================pointer_down=%d",id );
            laya::JSInput::getInstance()->activeCall(e);
        }
        break;
    case ACTION_POINTER_UP:
        {
            laya::inputEvent e;
            e.nType = laya::E_ONACTION_POINTER_UP;
            e.nTouchType = type;
            e.posX = x;
            e.posY = y;
            e.id = id;
            strncpy(e.type, "touchend", 256 );
            //LOGE( "======touchend===========pointer_up=%d",id );
            laya::JSInput::getInstance()->activeCall(e);
        }
        break;
    }
}
//------------------------------------------------------------------------------

//-----------------------------END FILE--------------------------------
