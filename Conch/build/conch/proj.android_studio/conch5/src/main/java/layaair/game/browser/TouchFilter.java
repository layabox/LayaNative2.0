package layaair.game.browser;

import android.util.Log;
import android.view.MotionEvent;

/*
 *	只处理MotionEvent中的主事件，相当于一次只接收一个点的状态。根据这些信息可以知道
 *	当前所有的点的位置和状态。
 *
 *	限定最多只有10个触摸点。	
 *
 * 	测试：
 * 		单点
 * 			刚down后的小范围移动不产生输出
 * 			一旦move后，就不再限制范围，只要有移动的输入就会有输出
 * 			up后数组为空
 * 
 *		多点
 * 			同时按下n个点后，数组中有n个有效元素
 * 			抬起一个点后，能正确删除
 * 			都抬起后能clear				
 */
public class TouchFilter
{
    int m_nMovRange=10;		//移动检测范围。move后，超过1秒钟没有超出此范围则认为静止了
    /*
     * m_vTouches 只是用来更新pointer的状态的，内部并不保存和更新当前的实际位置。
     * 目前 只有当状态为PTSTATE_MOVING的时候，其位置才代表实际位置
     */
    TouchPoint 	m_vTouches[] = new TouchPoint[10];
	public float m_fScaleX=1.0f;
	public float m_fScaleY=1.0f;
	public TouchFilter()
	{
		for( int i=0; i<10; i++)
		{
			m_vTouches[i] = new TouchPoint();
			m_vTouches[i].m_nID=-1;
			m_vTouches[i].m_nLastMoveX = 0;
			m_vTouches[i].m_nLastMoveY = 0;
			m_vTouches[i].m_nTouchState = TouchPoint.PTSTATE_NOTOUCH;
		}
		setMoveRangeMM( 0.8f ); 
	}

	//外部设置移动的偏差毫米为单位
	public void setMoveRangeMM( float p_fMM )
	{
		//毫米转英寸
		float fInch = (float) (p_fMM/10.0f*0.3937008);
		m_nMovRange = Math.round( fInch );
	}
    
    TouchPoint GetTouchPt(int p_nID)
    {
    	for( int i=0; i<10; i++)
    	{
    		if( m_vTouches[i].m_nID<0)
    			continue;
    		if( m_vTouches[i].m_nID==p_nID)
    			return m_vTouches[i];
    	}
    	return null;
    }
    
    void AddTouchPt(int p_nID, int p_nX, int p_nY )
    {
    	for( int i=0; i<10; i++)
    	{
    		if( m_vTouches[i].m_nID<0)
    		{
    			m_vTouches[i].m_nID = p_nID;
    			m_vTouches[i].m_nLastMoveX = p_nX;
    			m_vTouches[i].m_nLastMoveY = p_nY;
    			m_vTouches[i].m_nTouchState = TouchPoint.PTSTATE_DOWNSTILL;
    			break;
    		}
    	}
    }
    
    void DelTouchPt(int p_nID)
    {
    	for( int i=0; i<10; i++)
    	{
    		if( m_vTouches[i].m_nID<0)
    			continue;
    		if( m_vTouches[i].m_nID==p_nID)
    		{
    			m_vTouches[i].m_nID = -1;
    			m_vTouches[i].m_nLastMoveX = 0;
    			m_vTouches[i].m_nLastMoveY = 0;
    			m_vTouches[i].m_nTouchState = TouchPoint.PTSTATE_NOTOUCH;
    		}
    	}
    }
    
    void ClearTouchPt()
    {
    	for( int i=0; i<10; i++)
    	{
    		m_vTouches[i].m_nID = -1;
    		m_vTouches[i].m_nLastMoveX = 0;
    		m_vTouches[i].m_nLastMoveY = 0;
    	}
    }
    
    //测试用，获得当前有效数据的个数
/*    int GetPointerNum()
    {
    	int nSum=0;
    	for( int i=0; i<10; i++)
    	{
    		if( m_vTouches[i].m_nID >=0)
    			nSum++;
    	}
    	return nSum;
    }*/
    public Boolean onTouchEvent(MotionEvent event)
    {
    	if( event==null ) {
			return false;
		}
    	
    	int nPointerCount = event.getPointerCount();
		if( nPointerCount > 0 )
		{
			int nRawAction = event.getAction(); 
			int nAction = nRawAction&0xff;
			int nActionidx = ( nRawAction&MotionEvent.ACTION_POINTER_INDEX_MASK)>>MotionEvent.ACTION_POINTER_INDEX_SHIFT;
			int id = event.getPointerId( nActionidx );
			int nx = (int)event.getX( nActionidx );
			if(nx<0)nx=0;
			int ny = (int)event.getY( nActionidx );
			if(ny<0)ny=0;
			//加上缩放。即如果用了fixedsize，需要把touch缩放一下
			nx*=m_fScaleX;
			ny*=m_fScaleY;
			switch( nAction )
			{
			case MotionEvent.ACTION_DOWN:
				AddTouchPt(id, nx, ny);
				//Log.i("0",">>>>input java onTouchEvent ACTION_DOWN" );
				ConchJNI.handleTouch(nAction, (id < 10) ? id : 0, nx, ny);
				break;
			case MotionEvent.ACTION_UP:
				ClearTouchPt();
				ConchJNI.handleTouch(nAction, (id < 10) ? id : 0, nx, ny);
				break;
			case MotionEvent.ACTION_POINTER_DOWN:
				AddTouchPt(id, nx, ny);
				ConchJNI.handleTouch(nAction, (id < 10) ? id : 0, nx, ny);
				break;
			case MotionEvent.ACTION_POINTER_UP:
				DelTouchPt(id);
				ConchJNI.handleTouch(nAction, (id < 10) ? id : 0, nx, ny);
				break;
			case MotionEvent.ACTION_MOVE:
				{
					for( int i = 0; i < nPointerCount; i++ )
					{
						int nID = event.getPointerId( i );
						int nx1 = (int)event.getX( i );
						if(nx1<0)nx1=0;
						int ny1 = (int)event.getY( i );
						if(ny1<0)ny1=0;
						//加上缩放。即如果用了fixedsize，需要把touch缩放一下
						nx1*=m_fScaleX;
						ny1*=m_fScaleY;
						boolean bIgnoreCurEvent = false;
						TouchPoint pCurTouch = GetTouchPt( nID );
						if( pCurTouch != null )
						{
							if(nx1==pCurTouch.m_nLastMoveX && ny1==pCurTouch.m_nLastMoveY ){
								bIgnoreCurEvent=true;
							}else{
			    				if( pCurTouch.m_nTouchState == TouchPoint.PTSTATE_DOWNSTILL )
			    				{
			    					if(	Math.abs( nx1-pCurTouch.m_nLastMoveX)>m_nMovRange ||
			    						Math.abs( ny1-pCurTouch.m_nLastMoveY)>m_nMovRange )
			    					{
			    						pCurTouch.m_nLastMoveX = nx1;
			    						pCurTouch.m_nLastMoveY = ny1;
			    						pCurTouch.m_nTouchState = TouchPoint.PTSTATE_MOVING;
			    					}
			    					else
			    					{
			    						bIgnoreCurEvent = true;
			    					}
			    				}
			    				else if( pCurTouch.m_nTouchState== TouchPoint.PTSTATE_MOVING )
			    				{
			    					pCurTouch.m_nLastMoveX = nx1;
			    					pCurTouch.m_nLastMoveY = ny1;
			    				}
							}
						}
						else
						{
							Log.e("","touch事件错误，根据id没有找到当前touch对象,id="+nID );
						}
						if( !bIgnoreCurEvent )
						{
							ConchJNI.handleTouch(nAction, (nID < 10) ? nID : 0, nx1, ny1);
						}
					}
				}
				break;
			}
		}
    	return true;
    }
}
