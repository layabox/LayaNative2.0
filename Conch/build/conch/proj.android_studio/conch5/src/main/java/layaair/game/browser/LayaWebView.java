package layaair.game.browser;

import layaair.game.conch.LayaConch5;
import layaair.game.config.config;
import android.annotation.SuppressLint;
import android.content.Context;
import android.os.Build;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.webkit.JavascriptInterface;
import android.webkit.WebView;
import android.widget.AbsoluteLayout;

//------------------------------------------------------------------------------
class ReadyData
{
	public int m_nX = 0;
	public int m_nY = 0;
	public int m_nWidth = 0;
	public int m_nHeight = 0;
	public String m_sUrl = "";
	public Boolean m_bKeyEventCloseView = true;
}
//------------------------------------------------------------------------------
public class LayaWebView extends AbsoluteLayout implements View.OnKeyListener
{
	public ReadyData				m_pReadyData = new ReadyData();
	private LayaWebViewClient 		m_pWebViewClient = null;
	private LayaWebChromeClient		m_pWebChromeClient = null;
	private WebView					m_pWebView = null;
	private boolean				m_bIsAdd = false;
	private int					m_nX = 0;
	private int					m_nY = 0;
	private int					m_nWidth = 0;
	private int					m_nHeight = 0;
	public LayaConch5 				m_pEngine=null;
	//------------------------------------------------------------------------------
	@Override
	public boolean onKey(View v, int keyCode, KeyEvent event)
	{
		if(m_pReadyData.m_bKeyEventCloseView==false) return false;
		if (event.getAction() == KeyEvent.ACTION_DOWN)
		{
			if (!ConchJNI.onBackPressed())
			{
				if (keyCode == KeyEvent.KEYCODE_BACK && m_pWebView.canGoBack())
				{
					//m_pWebView.goBack();//后退
					if(config.GetInstance().m_bBackkeyWebviewHide) {
						hideWebView();
					}
					else
						closeWebView();
					return true;    //已处理
				}
				else if (keyCode == KeyEvent.KEYCODE_BACK && !m_pWebView.canGoBack())
				{
					if(config.GetInstance().m_bBackkeyWebviewHide) {
						hideWebView();
					}
					else
						closeWebView();
					return true;
				}
			}
		}
		return false;
	}
	@SuppressLint("JavascriptInterface")
	public LayaWebView( Context context, LayaConch5 engine )
	{
		super( context );
		m_pEngine = engine;
		m_pWebViewClient = new LayaWebViewClient();
		m_pWebViewClient.m_pLayaWebView=this;
		m_pWebView = new WebView( context );
		m_pWebView.setWebViewClient(m_pWebViewClient);
		m_pWebView.getSettings().setJavaScriptEnabled(true);
		m_pWebView.getSettings().setDefaultTextEncodingName("GBK");
		m_pWebView.getSettings().setDomStorageEnabled(true);
		m_pWebView.getSettings().setAppCacheMaxSize(1024 * 1024 * 8);
		String appCachePath = context.getApplicationContext().getCacheDir().getAbsolutePath()+"/webviewCache";
		m_pWebView.getSettings().setAppCachePath(appCachePath);
		m_pWebView.getSettings().setAllowFileAccess(true);
		m_pWebView.getSettings().setAppCacheEnabled(true);
		m_pWebView.setScrollBarStyle(View.SCROLLBARS_INSIDE_OVERLAY);
		m_pWebView.setBackgroundColor(0);
		this.addView(m_pWebView);
		m_pWebChromeClient = new LayaWebChromeClient( this );
		m_pWebView.setWebChromeClient( m_pWebChromeClient );
		m_pWebView.addJavascriptInterface(this, "runtime");
		m_pWebView.setOnKeyListener(this);
	}
	public void destory()
	{
		this.removeAllViews();
		if(m_pWebViewClient!=null)
		{
			m_pWebViewClient.m_pLayaWebView=null;
			m_pWebViewClient=null;
		}
		m_pEngine=null;
		if(m_pWebView!=null) {
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
				m_pWebView.removeJavascriptInterface("runtime");
			}
			m_pWebView.setWebViewClient(null);
			m_pWebView.setWebChromeClient(null);
			m_pWebView.setOnKeyListener(null);
			m_pWebView = null;
		}
	}
	//------------------------------------------------------------------------------
	public WebView getWebView()
	{
		return m_pWebView;
	}
	//------------------------------------------------------------------------------
	public boolean showWebView() {
		return m_pReadyData != null && showWebView(m_pReadyData.m_sUrl, m_pReadyData.m_nX, m_pReadyData.m_nY, m_pReadyData.m_nWidth, m_pReadyData.m_nHeight);
	}
	public void  setReadyData(int x,int y,int w,int h)
	{
		m_pReadyData.m_nX=x;
		m_pReadyData.m_nY=y;
		m_pReadyData.m_nWidth=w;
		m_pReadyData.m_nHeight=h;
	}
	public void showWebViewEx(){
		m_pWebView.requestFocus();
		setVisibility(VISIBLE);
	}
	public void closeWebView()
	{
		m_pWebView.clearFocus();
		m_pWebView.loadDataWithBaseURL(null, "","text/html", "utf-8",null);
		setVisibility(INVISIBLE);
		ConchJNI.closeExternalWebView();
	}
	public void  hideWebView()
	{
		String sBuffer = "javascript:window.conchBack&&window.conchBack();";
		Log.i("LayaWebView",sBuffer);
		m_pWebView.loadUrl( sBuffer );
		m_pWebView.clearFocus();
	    setVisibility(INVISIBLE);
		//ConchJNI.closeExternalWebView();
	}
	public  void stopWebViewLoading()
	{
		m_pWebView.stopLoading();
	}
	//------------------------------------------------------------------------------
	public boolean showWebView( String p_sUrl,int p_nX,int p_nY,int p_nWidth,int p_nHeight )
	{
		if( m_pEngine==null ) return false;
		m_pWebView.requestFocus();
		//第一次添加
		if(!m_bIsAdd)
		{
			m_nX = p_nX;
			m_nY = p_nY;
			m_nWidth = p_nWidth;
			m_nHeight = p_nHeight;
			m_pEngine.getAbsLayout().addView(this);
			//排版
			this.composing( m_nX,m_nY,m_nWidth,m_nHeight );
			this.LoadUrl( p_sUrl );
			m_bIsAdd = true;
			return true;
		}
		//宽和高度改变
		else if( m_nWidth != p_nWidth || m_nHeight != p_nHeight || m_nX != p_nX || m_nY != p_nY )
		{
			m_nWidth = p_nWidth;
			m_nHeight = p_nHeight;
			//排版
			this.composing( p_nX,p_nY,m_nWidth,m_nHeight );
			this.LoadUrl( p_sUrl );
			this.setVisibility( VISIBLE );
			return true;
		}
		//都没有改变
		else
		{
			this.composing( p_nX,p_nY,m_nWidth,m_nHeight );	//不调用这个会导致这种情况出问题：开web，关，开editbox，关，再开web，web无法显示
			this.LoadUrl( p_sUrl );
			this.setVisibility( VISIBLE );
			return true;
		}
	}
	//------------------------------------------------------------------------------
	private void composing( int p_nX,int p_nY,int p_nWidth,int p_nHeight )
	{
		AbsoluteLayout.LayoutParams pWebViewLP = new AbsoluteLayout.LayoutParams( p_nWidth,p_nHeight, p_nX,p_nY);
		m_pWebView.setLayoutParams( pWebViewLP );
	}
	//------------------------------------------------------------------------------
	private void LoadUrl( String p_sUrl )
	{
		if( m_pWebViewClient != null )
		{
			m_pWebViewClient.shouldOverrideUrlLoading( m_pWebView,p_sUrl );
		}
	}
	@JavascriptInterface
	public void JSLog( String p_sBuf )
	{
		Log.i( "LayaWebView",p_sBuf );
	}
	@JavascriptInterface
	public void callWebViewJSFunction( String sFunctionName,String sJsonParam,String sCallbackFuncton )
	{
		if( m_pWebView != null )
		{
			String sBuffer = "javascript: try{" + sFunctionName+"(\"" + sJsonParam +"\",\"" + sCallbackFuncton + "\"" + ");}catch(e){window.runtime.JSLog('CallJSFunction err ' + e.stack );}";
			Log.i("LayaWebView",sBuffer);
			m_pWebView.loadUrl( sBuffer );
		}
	}
	//------------------------------------------------------------------------------
	@JavascriptInterface
	public void callConchJSFunction(String sFunctionName, String sJsonParam, String sCallbackFunction)
	{
		Log.i("LayaWebView","CallConchJSFuncton functionName=" + sFunctionName + ",sJsonParam=" + sJsonParam + ",callbackFunction=" + sCallbackFunction );
		ConchJNI.callConchJSFunction( sFunctionName,sJsonParam,sCallbackFunction );
	}
}
