package layaair.game.browser;

/*
 * 维护当前pointer的状态。
 */
public class TouchPoint
{
	public static int PTSTATE_NOTOUCH=0;
	public static int PTSTATE_DOWNSTILL=1;
	public static int PTSTATE_MOVING = 2;
	
	public int m_nID=-1;
    int m_nTouchState=PTSTATE_NOTOUCH;	
    int m_nLastMoveX=0;
    int m_nLastMoveY=0;
}
