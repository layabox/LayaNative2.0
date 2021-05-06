package layaair.game.browser;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import layaair.game.network.NetworkReceiver;
import layaair.game.conch.LayaConch5;
import layaair.game.device.DevID;
import layaair.game.utility.Utils;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Paint.FontMetricsInt;
import android.graphics.Paint.Style;
import android.graphics.Point;
import android.graphics.Typeface;
import android.os.Handler;
import android.os.Vibrator;
import android.text.Layout;
import android.text.StaticLayout;
import android.text.TextPaint;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.WindowManager;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.HashMap;


public class ExportJavaFunction 
{
	private static	ExportJavaFunction		ms_pExportJavaFunction = null;
	private 		Paint 					m_Paint = new Paint();
	private			boolean					m_bShowLoading = false;
	private			boolean					m_bScreenWakeLock = false;
	private			boolean					m_bShowWating = false;
	public 			Handler					m_Handler = new Handler();
	public LayaConch5 m_pEngine= null;
   static public 	String					m_sHref="";
   static public	int 					m_nState=0;		//是否是第一次创建。0 还没创建 ， 1 已经创建， 2被删除了
	
	//------------------------------------------------------------------------------
	public static ExportJavaFunction GetInstance()
	{
		if(ms_pExportJavaFunction==null && m_nState!=2){//如果已经被删除了，则不允许再创建。
			ms_pExportJavaFunction = new ExportJavaFunction();
			m_nState=1;
		}
		
		return ms_pExportJavaFunction;
	}
	public static void DelInstance(){
		Log.e("","DELETE ExportJavaFunction instance!");
		m_nState=2;
		ms_pExportJavaFunction = null;
		synchronized (obj2idMap) {
			obj2idMap.clear();
			id2objMap.clear();
		}
		//obj2idMap=null;
		//id2objMap=null;
	}

	//------------------------------------------------------------------------------
	public ExportJavaFunction()
	{
	}
	//------------------------------------------------------------------------------
	public void Init( Context p_pContext )
	{
		ConchJNI.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"));
	}
	//------------------------------------------------------------------------------
	public static int [] getTextBuffer(String p_sChar, String p_sFamily, int p_nSize, int p_nTextColor, int p_nWeight,boolean p_bItalic,int p_nBorderSize,int p_nBorderColor )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null )
			return exjf.getTextPixelBuffer(p_sChar, p_sFamily, p_nSize, p_nTextColor, p_nWeight, p_bItalic,p_nBorderSize,p_nBorderColor );
		return null;
	}




	//------------------------------------------------------------------------------	
    public Bitmap drawTextToBitmap(  String p_sChar, int p_nFontSize, int p_nTexColor, Point pt,int p_nBorderSize,int p_nBorderColor )
	{
    	//目前描边，只支持一个像素的
    	int nBorderSize = 0;
    	if( p_nBorderSize > 0 )
    	{
    		nBorderSize = 1;
    	}
    	int nBmpSize = (p_nFontSize) + nBorderSize * 2;
		m_Paint.setStyle(Style.FILL);
		m_Paint.setTextSize( p_nFontSize );
		m_Paint.setAntiAlias(true);
    	int w = (int) m_Paint.measureText(p_sChar ) +  nBorderSize * 2;
		Bitmap mybitmap = Bitmap.createBitmap( w, nBmpSize, Bitmap.Config.ARGB_8888 );
		Canvas canvas = new Canvas( mybitmap );       
		canvas.drawColor( 0 );
		
		FontMetricsInt fontMetrics = m_Paint.getFontMetricsInt();  // 转载请注明出处：http://blog.csdn.net/hursing  
	    int baseline = 0 + (p_nFontSize - fontMetrics.bottom + fontMetrics.top) / 2 - fontMetrics.top;
		
		//如果有描边，多花4遍
		if( nBorderSize > 0 )
		{
			m_Paint.setColor(p_nBorderColor);
			canvas.drawText( p_sChar, nBorderSize-nBorderSize,baseline-nBorderSize, m_Paint );
			canvas.drawText( p_sChar, nBorderSize+nBorderSize,baseline-nBorderSize, m_Paint );
			canvas.drawText( p_sChar, nBorderSize-nBorderSize,baseline+nBorderSize, m_Paint );
			canvas.drawText( p_sChar, nBorderSize+nBorderSize,baseline+nBorderSize, m_Paint );
		}
		m_Paint.setColor(p_nTexColor);
	    canvas.drawText(p_sChar, nBorderSize, baseline, m_Paint);  
		//canvas.drawText( p_sChar, nBorderSize,p_nFontSize-2, m_Paint );
		pt.x = w;
		pt.y = nBmpSize;
		return mybitmap;
    }
    //------------------------------------------------------------------------------
    public int[] getTextPixelBuffer( String p_sWord, String p_sFamily, int p_nSize, int p_nTextColor,int p_nWeight,boolean p_bItalic,int p_nBorderSize,int p_nBorderColor )
    {
    	int nSize = p_nSize;
    	Point pt=new Point();
 		Bitmap pBitmap = this.drawTextToBitmap(p_sWord, nSize, p_nTextColor, pt, p_nBorderSize, p_nBorderColor);
 		int len = pt.x*pt.y+2;//2 width height
		int[] pDataBuffer = new int[ len ];	
		int nNum = 0;
		
		for( int i = 0; i < pt.y; i++ )
		{
			for( int j = 0; j < pt.x; j++ )
			{
				pDataBuffer[ nNum ] = pBitmap.getPixel(j, i);
				nNum++;
			}
		}
		pDataBuffer[len-2]=pDataBuffer[0];
		pDataBuffer[len-1]=pDataBuffer[1];
		pDataBuffer[0]=pt.x;
		pDataBuffer[1]=pt.y;
		return pDataBuffer;
    }
	public static HashMap<Integer,Object> id2objMap=new HashMap<Integer, Object>();
	public static HashMap<Object,Integer> obj2idMap=new HashMap<Object, Integer>();
	//conch["platCallBack"]=function(c,objid,m,rs)

	public static  void CallBackToJS(Object objOrClass,String methodName,Object ret) {
		JSONObject rsJson = new JSONObject();
		int callerid = -1;
		String cls = null;
		if (objOrClass instanceof Class) {
			cls = ((Class) objOrClass).getName();
		} else if (objOrClass instanceof String) {
			cls = (String) objOrClass;
		} else {
			Integer id;
			synchronized (obj2idMap) {
				id = obj2idMap.get( objOrClass );
			}
			if (id == null) {
				Log.e("LayaBox", "该对象不是通过脚本生成的");
				return;
			}
			callerid = id;
		}
		try {
			rsJson.put("objId", callerid);
			rsJson.put("cName", cls);
			rsJson.put("mName", methodName);
			rsJson.put("v", ret);
		} catch (JSONException e) {
			e.printStackTrace();
		}
		ConchJNI.RunJS("conch.platCallBack(" + rsJson.toString() + ")");
	}

	private static void newObj(Class[] signs, Object[] params,int callerid,Class cls)
	{
		Constructor con= null;
		try {
			con = cls.getConstructor(signs);
			try {
				Object obj= null;
				try {
					obj = con.newInstance(params);
					synchronized (obj2idMap) {
						id2objMap.put( callerid, obj );
						obj2idMap.put( obj, callerid );
					}
				} catch (IllegalAccessException e) {
					e.printStackTrace();
				} catch (InvocationTargetException e) {
					e.printStackTrace();
				}
			} catch (InstantiationException e) {
				e.printStackTrace();
			}
		} catch (NoSuchMethodException e) {
			e.printStackTrace();
		}
	}

	private static Object _callMethod(Class[] signs, Object[] params,String methodName,Object caller,Class cls)
	{
		Object result=null;
		if(cls==null)
		{
			Log.e("",">>>>>>>>>>>>>>>>>>>>null");
			return "";
		}
		try {
			Method method = cls.getMethod(methodName, signs);
			try {
				result= method.invoke(caller, params);
			}
			catch (Exception e)
			{
				return e.toString() + " " + e.getMessage();
			}
		} catch (NoSuchMethodException e) {
			return e.toString();
		}
		return result;
	}
    public static Class[] strClass={String.class};
	public static String callMethod(String className,String methodName,String paramStr)
	{
		Object result=null;
		try {
			className=className.replaceAll("/", ".");
			Class cls=Class.forName(className);
			try {
				Method method=null ;
				if(paramStr!=null)
					method=cls.getMethod(methodName,strClass);
				else
					method=cls.getMethod(methodName);
				try {
					if(paramStr!=null)
						result=method.invoke(null,paramStr);
					else
						result=method.invoke(null);
				} catch (IllegalAccessException e) {
					e.printStackTrace();
				} catch (InvocationTargetException e) {
					e.printStackTrace();
				}
			} catch (NoSuchMethodException e) {
				e.printStackTrace();
			}
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
		//String d=result;
		return result!=null?result.toString():null;
	}
	public static String callMethod(int callerid,boolean isSyn,String className,String methodName,String paramStr)
	{
		Object []param;
		className=className.replaceAll("/",".");
		if(paramStr==null)
			param=new Object[0];
		else {
			JSONArray ja = null;
			try {
				ja = new JSONArray(paramStr);
			} catch (JSONException e) {
				e.printStackTrace();
			}
			int paramLen= ja != null ? ja.length() : 0;
			param=new Object[paramLen];
			for(int i=0;i<paramLen;i++)
			{
				try {
					param[i]=ja.get(i);
				} catch (JSONException e) {
					e.printStackTrace();
				}
			}
		}
		Class [] temp=new Class[param.length];
		Object returnValue=null;
		int i=0;
		for (Object t:param) {
			Class c=t.getClass();
			if(c==Integer.class){
				c=int.class;
			}
			else if(c==Float.class){
				c=float.class;
			}
			else if(c==Double.class) {
				c=float.class;
				param[i]=Float.parseFloat(param[i].toString());
			}
			else if(c==Boolean.class) {
				c=boolean.class;
			}
			temp[i]=c;
			i++;
		}
		Object caller = id2objMap.get( callerid );
		Class cls= null;
		try {
			cls = caller!=null?caller.getClass():Class.forName(className);
			if("<init>".equals(methodName))
			{
				newObj(temp,param,callerid,cls);
			}
			else {
				returnValue = _callMethod(temp, param, methodName,caller,cls);
			}
		} catch (ClassNotFoundException e) {
			returnValue=e.toString();
		}
		JSONObject result=new JSONObject();
		try {
			result.put("v",returnValue);
		} catch (JSONException e) {
			e.printStackTrace();
		}
		return  result.toString();
	}
	public static String callMethodRefection(int callerid,boolean isSyn,String className,String methodName,String paramStr)
	{
		Object []param;
		className=className.replaceAll("/",".");
		if(paramStr==null)
			param=new Object[0];
		else {
			JSONArray ja = null;
			try {
				ja = new JSONArray(paramStr);
			} catch (JSONException e) {
				e.printStackTrace();
			}
			int paramLen= ja != null ? ja.length() : 0;
			param=new Object[paramLen];
			for(int i=0;i<paramLen;i++)
			{
				try {
					param[i]=ja.get(i);
				} catch (JSONException e) {
					e.printStackTrace();
				}
			}
		}
		Class [] temp=new Class[param.length];
		Object returnValue=null;
		int i=0;
		for (Object t:param) {
			Class c=t.getClass();
			if(c==Integer.class){
				c=double.class;//c=int.class;
			}
			else if(c==Float.class){
				c=double.class;//c=float.class;
			}
			else if(c==Double.class) {
				c=double.class;//c=float.class;
				//param[i]=Float.parseFloat(param[i].toString());
			}
			else if(c==Boolean.class) {
				c=boolean.class;
			}
			temp[i]=c;
			i++;
		}
		Object caller = id2objMap.get( callerid );
		Class cls= null;
		try {
			cls = caller!=null?caller.getClass():Class.forName(className);
			if("<init>".equals(methodName))
			{
				newObj(temp,param,callerid,cls);
			}
			else {
				returnValue = _callMethod(temp, param, methodName,caller,cls);
			}
		} catch (ClassNotFoundException e) {
			returnValue=e.toString();
		}
		JSONObject result=new JSONObject();
		try {
			result.put("v",returnValue);
		} catch (JSONException e) {
			e.printStackTrace();
		}
		return  result.toString();
	}
    //------------------------------------------------------------------------------
    public static int getTextWidth(String p_sChar, String p_sFamily, int p_nSize, int p_nWeight,boolean p_bItalic )
    {
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null )
			return exjf._getTextWidth(p_sChar, p_sFamily, p_nSize, p_nWeight, p_bItalic );
		return 1;
    }
    //------------------------------------------------------------------------------
	public int _getTextWidth(String p_sChar, String p_sFamily, int p_nSize, int p_nWeight,boolean p_bItalic )
	{
		m_Paint.setStyle(Style.FILL);
		m_Paint.setTextSize(p_nSize);
		m_Paint.setAntiAlias(true);
		int w = (int) m_Paint.measureText(p_sChar );
		return w;
	}
	//------------------------------------------------------------------------------
	public static void setExternalLink( String p_sUrl,int x,int y,int w,int h,int bKeyCloseView )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null )
			exjf.showExternalWeb(p_sUrl,x,y,w,h,bKeyCloseView);
	}
	//------------------------------------------------------------------------------
	public static void closeExternalLink()
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null )
			exjf.closeWebView();
	}

	public static  void hideExternalLink()
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null )
			exjf.hideWebView();
	}

	public static  void showExternalLink()
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null )
			exjf.showWebView();
	}
	//------------------------------------------------------------------------------
	public static void exit()
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null )
			exjf.doExit();
	}
	//------------------------------------------------------------------------------
	private void showExternalWeb( final String p_sUrl,final int x,final int y,final int w,final int h,final int bKeyCloseWebview )
	{
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					ReadyData readyData=m_pEngine.getWebView().m_pReadyData;
					readyData.m_sUrl = p_sUrl;
					readyData.m_nX = x;
					readyData.m_nY = y;
					readyData.m_nWidth = w;
					readyData.m_nHeight = h;
					readyData.m_bKeyEventCloseView = (bKeyCloseWebview!=0);
					//TODO 插件中怎么获得宽高
					WindowManager wm = (WindowManager) (m_pEngine.getGameContext().getSystemService(Context.WINDOW_SERVICE));
					DisplayMetrics dm = new DisplayMetrics();
					wm.getDefaultDisplay().getMetrics(dm);
					if(readyData.m_nWidth==0)
					{
						readyData.m_nWidth = dm.widthPixels;
					}
					if(readyData.m_nHeight==0)
					{
						readyData.m_nHeight = dm.heightPixels;
					}
					m_pEngine.getWebView().showWebView();
				}
			}
		);
	}
	//------------------------------------------------------------------------------
	private void hideWebView(){
		m_Handler.post
		(
				new Runnable()
				{
					public void run()
					{
						m_pEngine.getWebView().hideWebView();
					}
				}
		);
	}

	//------------------------------------------------------------------------------
	private void closeWebView() {
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					m_pEngine.getWebView().closeWebView();
				}
			}
		);
	}
	private void showWebView(){
		m_Handler.post
				(
						new Runnable()
						{
							public void run()
							{
								m_pEngine.getWebView().showWebViewEx();
							}
						}
				);
	}
	//------------------------------------------------------------------------------
	private void doExit(){
		m_Handler.post
		(
				new Runnable()
				{
					public void run()
					{
						m_pEngine.game_plugin_exitGame();
					}
				}
		);
	}

	//------------------------------------------------------------------------------
	public static void callWebViewJS( String sFunctionName,String sJsonParam,String sCallbackFuncton )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null )
		{
			exjf.callWebViewJSFunction(sFunctionName, sJsonParam,sCallbackFuncton);
		}
	}
	private void callWebViewJSFunction( final String sFunctionName,final String sJsonParam,final String sCallbackFuncton )
	{
		m_Handler.post
			(
					new Runnable() {
						public void run()
					{
						JSONObject obj=new JSONObject();
						try {
							obj.put("type","runJS");
							obj.put("fName",sFunctionName);
							obj.put("value",sJsonParam);
						} catch (JSONException e) {
							e.printStackTrace();
						}
						//if(m_pEngine.m_pPlatform!=null)
						//m_pEngine.m_pPlatform.LP_sendMessageToPlatform(obj.toString());
						m_pEngine.getWebView().callWebViewJSFunction(sFunctionName,sJsonParam,sCallbackFuncton);
					}
				}
			);
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxPosX( int p_nLeft )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setPosX(p_nLeft);
			}
		}
	}

	public static  void captureScreen()
	{
		LayaConch5.GetInstance().getCanvas().captureScreen();
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxPosY( int p_nTop )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setPosY(p_nTop);
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxWidth( int p_nWidth )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
		
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setWidth(p_nWidth);
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxHeight( int p_nHeight )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setHeight(p_nHeight);
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxStyle( String p_sStyle )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setStyle(p_sStyle);
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxFocus()
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				//pEditBox.setFocus( true );
				int len=pEditBox.m_pEditBox.getText().length();
				pEditBox.setCursorPosition(len);
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxBlur()
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null) {
				//这个地方必须要隐藏Editbox否则恢复的时候会弹出软件盘
				pEditBox.m_bFocus = false;
				pEditBox.setVisible(false);
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxVisible( boolean p_bVisible )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){		
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setVisible(p_bVisible);
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxValue( String p_sValue )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){		
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setValue(p_sValue);
			}
		}
	}
	//------------------------------------------------------------------------------
	public static String getEditBoxValue()
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				return pEditBox.getValue();
			}
		}
		return "";
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxColor( int p_nColor )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setTextColor(p_nColor);
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxFontSize( int p_nFontSize )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setTextSize(p_nFontSize);
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxPos( int p_nX,int p_nY )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setPos(p_nX, p_nY);
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxSize( int p_nW,int p_nH )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setSize(p_nW, p_nH);
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxCursorPosition( int p_nPos )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setCursorPosition(p_nPos);
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxMaxLength( int p_nMaxLength )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setMaxLength(p_nMaxLength);
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxPassword( boolean p_bPassword )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setPassWord(p_bPassword);
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxNumberOnly( boolean p_bNumberOnly )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
		LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
		if( pEditBox != null )
		{
			pEditBox.setNumberOnly(p_bNumberOnly);
		}
		}
	}
	
	public static void setMultiAble( boolean p_bMultiAble )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
		LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
		if( pEditBox != null )
		{
			pEditBox.setMultiAble(p_bMultiAble);
		}
		}
	}

	public static void setForbidEdit( boolean p_bForbidEdit )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setForbidEdit(p_bForbidEdit);
			}
		}
	}
	
	//------------------------------------------------------------------------------
	public static void setEditBoxRegular( String p_sRegular )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.SetRegular(p_sRegular);
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void alert( String msg )
	{
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		if(pExport!=null){
			AlertRunnable runnable = new AlertRunnable();
			runnable.m_sAlertMsg = msg;
			runnable.m_nAlertCallbackType = 0;
			runnable.m_sAlertTitle = LayaConch5.GetInstance().m_strAlertTitle;
			pExport.JSAlert(runnable);
		}
	}
	//------------------------------------------------------------------------------
	public static void alertCallback( String msg )
	{
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		if(pExport!=null){
			AlertRunnable runnable = new AlertRunnable();
			runnable.m_sAlertMsg = msg;
			runnable.m_nAlertCallbackType = 1;
			runnable.m_sAlertTitle = LayaConch5.GetInstance().m_strAlertTitle;
			pExport.JSAlert(runnable);
		}
	}
	//------------------------------------------------------------------------------
	public static void alertExit( String msg )
	{
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		if(pExport!=null){
			AlertRunnable runnable = new AlertRunnable();
			runnable.m_sAlertMsg = msg;
			runnable.m_nAlertCallbackType = 2;
			runnable.m_sAlertTitle = LayaConch5.GetInstance().m_strAlertTitle;
			pExport.JSAlert(runnable);
		}
	}
	//------------------------------------------------------------------------------
	public static void setHrefToJava(String href)
	{
		m_sHref = href;
	}
	//------------------------------------------------------------------------------
	public void JSAlert(AlertRunnable runnable)
	{	runnable.m_context = m_pEngine.getGameContext();
		m_Handler.post(runnable);
	}
	//屏幕一直亮着的 设定
	//------------------------------------------------------------------------------
	public static void setScreenWakeLock( boolean p_bWakeLock )
	{
		LayaConch5.GetInstance().setRuntimeExt();//set runtime ext
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		if(pExport!=null){
			pExport.m_bScreenWakeLock = p_bWakeLock;
			pExport.setScreenWakeLock();
		}
	}
	//------------------------------------------------------------------------------
	public void setScreenWakeLock()
	{
		m_Handler.post
				(
						new Runnable() {
							public void run() {
/*					if( m_pEngine.getIsPlug() )
					{*/
								LayaConch5.GetInstance().setScreenWakeLock(m_bScreenWakeLock);
//					}
					/*else
					{
						LayaWrapper.setScreenWakeLock( m_bScreenWakeLock );
					}*/
							}
						}
				);
	}
	//------------------------------------------------------------------------------
	public static void showLoadingView( boolean p_bShow )
	{
		Log.i("LayaBox", "showLoadingview" + p_bShow);
	}
	
	public static void reloadApp(){
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		if(pExport!=null)
			pExport._reloadApp();
	}
	public void _reloadApp(){
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					ConchJNI.reloadJS();
				}
			}
		);
	}
	
	public static void postCmdToMain(int nCmd, int nParam1, int nParam2){
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		if(pExport!=null)
			pExport._postCmdToMain(nCmd, nParam1, nParam2);
	}
	
	public void _postCmdToMain(int nCmd, int nParam1, int nParam2){
		class _Runnable implements Runnable{ 
			int m_nCmd = 0;
			int m_nP1 = 0;
			int m_nP2 = 0;

			public _Runnable(int nCmd, int nParam1, int nParam2){
				m_nCmd = nCmd; m_nP1 = nParam1; m_nP2 = nParam2;
			}
			public void run(){
				ConchJNI.onRunCmd(m_nCmd, m_nP1, m_nP2);
			}
		};
		
		m_Handler.post(new _Runnable(nCmd, nParam1, nParam2));
	}
	//------------------------------------------------------------------------------
	public static String getWifiMac()
	{
		ExportJavaFunction exp = ExportJavaFunction.GetInstance();
		if(exp==null )
			return "";
		/*DevID pDevID = exp.m_pEngine.getDevID();
		if( pDevID != null )
		{
			String wifi=pDevID.GetWifiMac();
			return (wifi==null)?"":wifi;
		}*/
		return "";
	}
	//------------------------------------------------------------------------------
	public static String getAndroidModel()
	{
		ExportJavaFunction exp = ExportJavaFunction.GetInstance();
		if(exp==null )
			return "";
		DevID pDevID = exp.m_pEngine.getDevID();
		if( pDevID != null )
		{
			return pDevID.GetPhoneModelAndSDK();
		}
		return "";
	}
	//------------------------------------------------------------------------------
	public static float getScreenInch()
	{
		return 4;
		//TODO return MainActivity.GetScreenInch();
	}
	//------------------------------------------------------------------------------
	public static void setTouchMoveRange( float p_fMM )
	{
		ExportJavaFunction exp = ExportJavaFunction.GetInstance();
		if(exp!=null)
			exp.m_pEngine.getCanvas().m_TouchFilter.setMoveRangeMM( p_fMM );
	}
	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------
	public static void setScreenOrientation( int p_nOrientation  )
	{
		ExportJavaFunction exp = ExportJavaFunction.GetInstance();
		if(exp==null) return;
		if( !exp.m_pEngine.getIsPlug() )
		{
//			LayaWrapper.GetInstance().m_LayaEngineContext.setRequestedOrientation(p_nOrientation);
			((Activity)exp.m_pEngine.mCtx).setRequestedOrientation(p_nOrientation);
		}
		Log.i("0", ">>>>>>ExportJavaFunction setScreenOrientation=" + p_nOrientation );
		boolean bLandscape = false;
		switch( p_nOrientation )
		{
		case ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE:
			bLandscape= true;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_PORTRAIT:
			bLandscape = false;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_USER:
			bLandscape = false;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_BEHIND:
			bLandscape = false;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_SENSOR:
			bLandscape = false;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_NOSENSOR:
			bLandscape = false;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE:
			bLandscape = true;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT:
			bLandscape = false;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE:
			bLandscape = true;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT:
			bLandscape = false;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_FULL_SENSOR:
			bLandscape = false;
			break;
		}
		exp.m_pEngine.m_bHorizontalScreen = bLandscape;
	}

	public static  void setSensorAble(boolean a)
	{
		ExportJavaFunction exp=ExportJavaFunction.GetInstance();
		if(exp==null)
			return;
		exp.m_pEngine.setSensorAble(a);
	}

	public static int getScreenOrientation( )
	{
		ExportJavaFunction exp = ExportJavaFunction.GetInstance();
		if(exp==null)
			return 0;
		if( !exp.m_pEngine.getIsPlug() ){
		    return ((Activity)exp.m_pEngine.mCtx).getRequestedOrientation();
			//return LayaWrapper.GetInstance().m_LayaEngineContext.getRequestedOrientation();
		}
		return 0;
	}
	// ------------------------------------------------------------------------------
	public static void ShowMessage(String pMessage){
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		if(pExport!=null)
			pExport._showMessage(pMessage);
	}
	public void _showMessage(final String pMessage)
	{
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					ExportJavaFunction expjava = ExportJavaFunction.GetInstance();
					if(expjava!=null)
						expjava.m_pEngine.showMessage(pMessage);
				}
			}
		);
	}
	//------------------------------------------------------------------------------
	public static String GetDeviceInfo(){
		ExportJavaFunction exp = ExportJavaFunction.GetInstance();
		if(exp==null )
			return "";
		DevID pDevID = exp.m_pEngine.getDevID();
		if( pDevID == null )
			return "";
		JSONObject obj = new JSONObject();
		JSONArray imeiArr = new JSONArray();
		//imeiArr.put(pDevID.GetIMEI());
		imeiArr.put("unknown");
		JSONArray imsiArr = new JSONArray();
		//imsiArr.put(pDevID.GetIMSI());
		imsiArr.put("unknown");
		try {
			int temp[]=pDevID.GetResolutionArray();
			obj.put("dpi", temp[2]);
			obj.put("resolution",temp[1]+"*"+temp[0]);
			//obj.put("guid",pDevID.GetWifiMac());
			obj.put("guid","unknown");
			obj.put("imei", imeiArr);
			obj.put("imsi", imsiArr);
			obj.put("os", "android");
			obj.put("osversion", pDevID.GetOSVersion());
			obj.put("phonemodel", pDevID.GetPhoneModelAndSDK());
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return obj.toString();
	}
	//------------------------------------------------------------------------------
	public static void vibrate(){
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		if(pExport==null)
			return;
		pExport._vibrate();
	}
	public void _vibrate(){
		Vibrator vibrator = (Vibrator)(m_pEngine.getGameContext().getSystemService(Context.VIBRATOR_SERVICE));
        /* 
         * 想设置震动大小可以通过改变pattern来设定，如果开启时间太短，震动效果可能感觉不到 
         * */  
      long [] pattern = {10,100,100,200};   // 停止 开启 停止 开启   
      vibrator.vibrate(pattern,-1);           //重复两次上面的pattern 如果只想震动一次，index设为-1
		//vibrator.vibrate(10000);
	}
	public static int getContextedType(){
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		if(pExport==null)
			return NetworkReceiver.NET_UNKNOWN;
		NetworkReceiver rec = pExport.m_pEngine.getNetworkReceiver();		
		return rec.getConnectedType(pExport.m_pEngine.getGameContext());
	}
	public static String getAppVersion(){
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		if(pExport!=null){
			return pExport.m_pEngine.m_appVersion;
		}
		return "";
	}
	public static String getAppLocalVersion(){
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		if(pExport!=null){
            return pExport.m_pEngine.m_appLocalVersion;
		}
		return "";
	}

	public void _useChoreographer(boolean b){
		/*m_pEngine.m_pCavans.mbUseChoreographer=b;
		if(b) {
			m_Handler.post(
					new Runnable() {
						public void run() {
							ExportJavaFunction pExportJavaFun = ExportJavaFunction.GetInstance();
							pExportJavaFun.m_pEngine.m_pCavans.mbUseChoreographer=true;
							pExportJavaFun.m_pEngine.m_pCavans.doFrame(System.nanoTime());
						}
					});
		}else{

		}*/
	}

	public static void setResolution(int w,int h){
		Log.i("fix","js setRes "+w+","+h);
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		if(pExport!=null){
			pExport._setResolution(w,h);
		}
	}

	public void _setResolution(final int w, final int h){
		m_Handler.post(
			new Runnable() {
				public void run() {
					m_pEngine.setResolution(w,h);
			}
		});
	}
	public static Object[] createTextBitmap(String text, String fontName, int fontSize, int fontColor, int strokeColor, float strokeSize) {
		TextPaint paint = new TextPaint();
		paint.setTextSize(fontSize);
		paint.setAntiAlias(true);
		paint.setTypeface(Typeface.create(fontName, Typeface.NORMAL));

		if (strokeSize > 0) {
			paint.setStyle(TextPaint.Style.STROKE);
			paint.setStrokeWidth(strokeSize);
		}

		int maxWidth = (int)Math.ceil( StaticLayout.getDesiredWidth(text, paint));

		Layout layout  = new StaticLayout(text, paint, maxWidth , Layout.Alignment.ALIGN_CENTER,1.0f,0.0f,false);;

		int layoutWidth = layout.getWidth();
		int layoutHeight = layout.getLineTop(layout.getLineCount());

		int bitmapWidth = layoutWidth, width;
		int bitmapHeight = layoutHeight;


		//Rect rect = new Rect();
		//paint.getTextBounds(string,0,string.length(), rect);
		//Log.d("fix", ""+bitmapWidth +" " +rect.width() +" "+ bitmapHeight +" "+ rect.height());

		Bitmap bitmap = Bitmap.createBitmap(bitmapWidth, bitmapHeight, Bitmap.Config.ARGB_8888);
		Canvas canvas = new Canvas(bitmap);

		if (strokeSize > 0)
		{
			paint.setARGB((strokeColor >> 24) & 255, (strokeColor >> 16) & 255, (strokeColor >> 8) & 255, strokeColor & 255);
			layout.draw(canvas);
		}
		paint.setStyle(TextPaint.Style.FILL);

		paint.setARGB((fontColor >> 24) & 255, (fontColor >> 16) & 255, (fontColor >> 8) & 255, fontColor & 255);
		layout.draw(canvas);

		final byte[] pixels = new byte[bitmap.getWidth()
				* bitmap.getHeight() * 4];
		final ByteBuffer buf = ByteBuffer.wrap(pixels);
		buf.order(ByteOrder.nativeOrder());
		bitmap.copyPixelsToBuffer(buf);

		Object[] arr = new Object[3];
		arr[0] = new Integer(bitmap.getWidth());
		arr[1] = new Integer(bitmap.getHeight());
		arr[2] = pixels;
		return arr;
	}
	public static Object[] measureText(String text, String fontName, int fontSize)
	{
		TextPaint paint = new TextPaint();
		paint.setTextSize(fontSize);
		paint.setAntiAlias(true);
		paint.setTypeface(Typeface.create(fontName, Typeface.NORMAL));

		int maxWidth = (int)Math.ceil( StaticLayout.getDesiredWidth(text, paint));

		Layout layout  = new StaticLayout(text, paint, maxWidth , Layout.Alignment.ALIGN_CENTER,1.0f,0.0f,false);;

		int layoutWidth = layout.getWidth();
		int layoutHeight = layout.getLineTop(layout.getLineCount());

		int bitmapWidth = layoutWidth, width;
		int bitmapHeight = layoutHeight;

		Object[] arr = new Object[2];
		arr[0] = new Integer(bitmapWidth);
		arr[1] = new Integer(bitmapHeight);
		return arr;
	}
}
