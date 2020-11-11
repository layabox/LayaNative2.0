package layaair.game.browser;

import java.util.regex.Pattern;

import layaair.game.conch.LayaConch5;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.graphics.Typeface;
import android.os.Build;
import android.os.Handler;
import android.text.InputFilter;
import android.text.InputType;
import android.text.Selection;
import android.text.Spannable;
import android.util.Log;
import android.util.TypedValue;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnKeyListener;
import android.view.inputmethod.InputMethodManager;
import android.widget.AbsoluteLayout;
import android.widget.EditText;

public class LayaEditBox 
{
	public EditText 					m_pEditBox = null;
	public Context						m_pApplication = null;
	public int							m_nPosX = 0;
	public int							m_nPosY = 0;
	public int							m_nWidth = 0;
	public int							m_nHeight = 0;
	public boolean						m_bForbidEdit=false;
	public AbsoluteLayout.LayoutParams 	m_pLayoutParams = null;
    private static final String  		TAG="LayaBox";
	public float 						m_fPostScaleX=1.0f;
	public float 						m_fPostScaleY=1.0f;
	//---------------------------------------------------------------
	//为了避免线程问题
	public String 						m_sValues;
	private int 						m_nBackGroudColor = 0;
	private boolean						m_nNumberOnly = false;
	private boolean 					m_bPassWord = false;
	private int 						m_nTextColor = 0xffffff;
	private int 						m_nTexSize = 24;
	private boolean 					m_bBold = false;
	private boolean 					m_bShow = false;	
	public boolean 						m_bFocus = false;
	private int 						m_nMaxLength = 0;
	private int 						m_nLines = 1;
	private LayaEditBoxWatcher			m_pEditBoxWatcher = null;
	private String						m_sRegular = null;
	private	String						m_sStyle = null;
	public 	Handler						m_Handler = new Handler();
	//---------------------------------------------------------------
	
	public void destory()
	{
		m_pApplication=null;
		if(m_pEditBoxWatcher!=null)
		{
			m_pEditBoxWatcher.destory();
			m_pEditBoxWatcher=null;
		}
		if(m_pEditBox!=null)
		{
			m_pEditBox.destroyDrawingCache();
			m_pEditBox=null;
		}
	}
	@SuppressLint("NewApi")
	public static void hideSoftKeyBorad()
	{
		if(Build.VERSION.SDK_INT>18)
		{
			int a=((Activity) LayaConch5.GetInstance().mCtx).getWindow().getDecorView().getSystemUiVisibility();
			Log.i(TAG, ">>>>>>>>>>>>>>>>>>>a:"+a);
			if((a&0x1000)==0x1000)
			{
				LayaConch5.GetInstance().m_pAbsLayout.setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
			}
		}
	}
	
	@SuppressLint("NewApi")
	public static void hideSystemKeyBoard( Context mcontext,View v )
	{
		InputMethodManager imm = (InputMethodManager) (mcontext.getSystemService(Context.INPUT_METHOD_SERVICE));
		imm.hideSoftInputFromWindow(v.getWindowToken(), 0);
		hideSoftKeyBorad();
	}
	public LayaEditBox( Context p_pContext ) 
	{
		m_pApplication = p_pContext;
		m_pEditBox = null;
	}
	public void SetRegular( String  p_sRegular )
	{
		m_sRegular = p_sRegular;
	}
	public String GetRegular()
	{
		return m_sRegular;
	}
	public boolean IsInputValid( String p_sBuffer )
	{
		if( m_sRegular == null||m_sRegular.equals("null")) return true;
		return Pattern.matches( m_sRegular, p_sBuffer );
	}
	public void CreateEditBox()
	{
		/*
		m_pEditBox = new EditText( m_pApplication );
		m_pEditBox.setBackgroundColor( 0xffffffff );
		m_pEditBoxWatcher = new LayaEditBoxWatcher( this );
		m_pEditBox.addTextChangedListener( m_pEditBoxWatcher );
		setLayoutParams();
		m_pEditBox.setVisibility( m_pEditBox.VISIBLE );
		*/
		
		m_pEditBox = new EditText( m_pApplication );
		m_pEditBox.setBackgroundColor( 0xffffffff );
		m_pEditBox.setBackgroundDrawable(null);
		m_pEditBox.setVisibility(  m_pEditBox.INVISIBLE );
		//为了兼容好多板子，比如三星的板子 editbox最小只能是50，所有小于50的，就不再小了，把所有文字靠左上显示
		m_pEditBox.setGravity( Gravity.START | Gravity.TOP );
		m_pEditBox.setSingleLine(true);
		m_pEditBox.setGravity(Gravity.CENTER_VERTICAL);
		m_pEditBoxWatcher = new LayaEditBoxWatcher( this );
		m_pEditBox.addTextChangedListener( m_pEditBoxWatcher );
		m_pEditBox.setPadding(0, 0, 0, 0);
		m_pEditBox.setOnEditorActionListener(m_pEditBoxWatcher);
		//m_pEditBox.setIncludeFontPadding(false);
		//m_pEditBox.setMaxLines(1);
		//这个在2.3的系统下没有
		//m_pEditBox.setTextAlignment(View.TEXT_ALIGNMENT_GRAVITY);
		/*
		//不知到为什么这个不起作用，好像只能接收硬件键盘的消息.只能接受控制字符，可见字符的输入都是走输入法的
		m_pEditBox.setOnKeyListener(new OnKeyListener() 
		{
            public boolean onKey(View v, int keyCode, KeyEvent event) 
            {
                if (event.getAction()==KeyEvent.ACTION_DOWN )
                {
                	//TODO
                    return true;
                }
                return false;
            }
        });
        */		
	}
	public boolean setLayoutParams()
	{
		if( m_pEditBox != null )
		{
			if( m_pLayoutParams == null )
			{
				m_pEditBox.setWidth( m_nWidth );
				m_pEditBox.setHeight( m_nHeight );
				m_pLayoutParams = new AbsoluteLayout.LayoutParams( m_nWidth,m_nHeight,m_nPosX,m_nPosY );
			}
			else
			{
				m_pLayoutParams.width = m_nWidth;
				m_pLayoutParams.height = m_nHeight;
				m_pLayoutParams.x = m_nPosX;
				m_pLayoutParams.y = m_nPosY;
				m_pEditBox.setWidth( m_nWidth );
				m_pEditBox.setHeight( m_nHeight );
			}
			m_pEditBox.setLayoutParams( m_pLayoutParams );
			//密码的设置都失效了，再来一遍
			if (m_bPassWord)
			{
				setRealPassWord();
				int len=m_pEditBox.getText().length();
				m_pEditBox.setSelection(len);
			}
			return true;
		}
		return false;
	}
	public String getValue()
	{
		if( m_pEditBox != null )
		{
			return m_pEditBox.getText().toString();
		}
		return "";
	}
	public void setValue( String p_sValues )
	{
		if(p_sValues==null)p_sValues="";
		m_sValues = p_sValues;
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					setRealValue();
					int len=m_pEditBox.getText().length();
				    m_pEditBox.setSelection(len);
				}
			}
		);
	}
	public boolean setRealValue()
	{
		if( m_pEditBox != null )
		{
			m_pEditBox.setText( m_sValues );
			return true;
		}
		return false;
	}
	public void setBackGroud( int p_nColor )
	{
		m_nBackGroudColor = p_nColor;
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					setRealBackGroud();
				}
			}
		);
	}
	public boolean setRealBackGroud()
	{
		if( m_pEditBox != null )
		{
			m_pEditBox.setBackgroundColor( m_nBackGroudColor );
			return true;
		}
		return false;
	}
	
	public void setMultiAble(final boolean p_bMultiAble)
	{
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					m_pEditBox.setSingleLine(!p_bMultiAble);
					if(p_bMultiAble)
					{
						m_pEditBox.setGravity(Gravity.TOP);
					}
					else
					{
						m_pEditBox.setGravity(Gravity.CENTER_VERTICAL);
					}
				}
			}
		);
	}
	public void setForbidEdit(final boolean p_bForbidEdit)
	{
		m_Handler.post
		(
			new Runnable()
			{
				public void run()
				{
					m_bForbidEdit = p_bForbidEdit;
				}
			}
		);
	}

	public void setPos( int p_nX,int p_nY )
	{
		m_nPosX = (int)(p_nX*m_fPostScaleX);
		m_nPosY = (int)(p_nY*m_fPostScaleY);
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					setLayoutParams();
				}
			}
		);
	}
	public void setPosX( int p_nX )
	{
		m_nPosX = (int)(p_nX*m_fPostScaleX);
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					setLayoutParams();
				}
			}
		);
	}
	public void setPosY( int p_nY )
	{
		m_nPosY = (int)(p_nY*m_fPostScaleY);
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					setLayoutParams();
				}
			}
		);
	}
	public void setSize( int p_nW,int p_nH )
	{
		m_nWidth = (int)(p_nW*m_fPostScaleX);
		m_nHeight = (int)(p_nH*m_fPostScaleY);
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					setLayoutParams();
				}
			}
		);
	}
	public void setWidth( int p_nWidth )
	{
		m_nWidth = (int)(p_nWidth*m_fPostScaleX);
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					setLayoutParams();
				}
			}
		);
	}
	public void setHeight( int p_nHeight )
	{
		m_nHeight = (int)(p_nHeight*m_fPostScaleY);
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					setLayoutParams();
				}
			}
		);
	}
	public void setNumberOnly( boolean p_bNumberOnly )
	{
		m_nNumberOnly = p_bNumberOnly;
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					setRealNumberOnly();
				}
			}
		);
	}
	public boolean setRealNumberOnly()
	{
		if( m_pEditBox != null )
		{
			if( m_nNumberOnly )
			{
				m_pEditBox.setInputType( InputType.TYPE_CLASS_NUMBER );
			}
			else
			{
				m_pEditBox.setInputType( InputType.TYPE_CLASS_TEXT );
			}
			return true;
		}
		return false;
	}
	public void setPassWord( boolean p_bPassWord )
	{
		m_bPassWord = p_bPassWord;
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
						setRealPassWord();
				}
			}
		);
	}
	public boolean setRealPassWord()
	{
		if( m_pEditBox != null )
		{
			if( m_bPassWord )
			{
				//下面两种方式，有的机器用第一种是好用的。。有的用第二种是好用的。
				//但是如果是用第2种方式，明文和密码切换有问题。。明文也变成密码了。。这种方式太影响用户体验了
				//所以用第一种方式，但是这种方式，有的时候切换密码不太好用，因为在老谢那边也会处理，显示变成了*。。所以不太影响
				m_pEditBox.setInputType( InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_PASSWORD);
			}
			else
			{
				m_pEditBox.setInputType(m_pEditBox.getInputType() & ~InputType.TYPE_TEXT_VARIATION_PASSWORD);
			}
			return true;
		}
		return false;
	}
	public void setTextColor( int p_nColor )
	{
		m_nTextColor = p_nColor;
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					setRealTextColor();
				}
			}
		);
	}
	public boolean setRealTextColor()
	{
		if( m_pEditBox != null )
		{
			m_pEditBox.setTextColor( m_nTextColor );
			return true;
		}
		return false;
	}
	public void setTextSize( int p_nTextSize )
	{
		m_nTexSize = (int)(p_nTextSize*m_fPostScaleX);
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					setRealTextSize();
				}
			}
		);
	}
	public boolean setRealTextSize()
	{
		if( m_pEditBox != null )
		{
			m_pEditBox.setTextSize( TypedValue.COMPLEX_UNIT_PX, m_nTexSize );
			return true;
		}
		return false;
	}
	public void setBold( boolean p_bBold )
	{
		m_bBold = p_bBold;
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					setRealBold();
				}
			}
		);
	}
	public boolean setRealBold()
	{
		if( m_pEditBox != null )
		{
			if( m_bBold )
			{
				m_pEditBox.setTypeface( Typeface.DEFAULT_BOLD );
			}
			return true;
		}
		return false;
	}
	public void setVisible( boolean p_bVisble )
	{
		m_bShow= p_bVisble;
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					setRealVisible();
				}
			}
		);
	}
	public boolean getVisible(  )
	{
		return m_bShow ;
	}
	public boolean setRealVisible()
	{
		if( m_pEditBox != null )
		{
			if( m_bShow == true )
			{
				m_pEditBox.setVisibility( View.VISIBLE );
			}
			else
			{
				m_pEditBox.setVisibility( View.INVISIBLE );
				m_pEditBox.clearFocus();
				hideSystemKeyBoard( m_pApplication,m_pEditBox );
				ExportJavaFunction mTemp = ExportJavaFunction.GetInstance();
				if (mTemp!=null && mTemp.m_pEngine!=null) {
					mTemp.m_pEngine.setGameFocus();
				}
			}
			return true;
		}
		return false;
	}
	public void setFocus( boolean p_bFocus )
	{
		m_bFocus = p_bFocus;
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					setRealFocus();
				}
			}
		);
	}
	public boolean getFocus(  )
	{
		return m_bFocus ;
	}
	public boolean setRealFocus()
	{
		if( m_pEditBox != null )
		{
			m_pEditBox.setFocusable( true );
			m_pEditBox.setFocusableInTouchMode(true);
			if(m_bFocus)
			{
				boolean isIntercepter = false;
				ExportJavaFunction mTemp = ExportJavaFunction.GetInstance();
				if (mTemp!=null && mTemp.m_pEngine!=null) {
					isIntercepter = mTemp.m_pEngine.getInterceptKey();
				}
				if (isIntercepter) {
					m_pEditBox.setOnKeyListener(new OnKeyListener() {
						@Override
						public boolean onKey(View v, int keyCode, KeyEvent event) {
							if( keyCode == KeyEvent.KEYCODE_BACK && event.getAction() == KeyEvent.ACTION_DOWN ){
								Log.i(TAG,"onKey = "+keyCode);
								setFocus(false);
								return true;
							}
							return false;
						}
					});
				}
				m_pEditBox.requestFocus();
				InputMethodManager imm = (InputMethodManager)m_pApplication.getSystemService( m_pApplication.INPUT_METHOD_SERVICE );   
				imm.showSoftInput(m_pEditBox, 0);
				m_pEditBox.invalidate();
				CharSequence text = m_pEditBox.getText();
				if (text != null)
				{
					Spannable spanText = (Spannable)text;
					Selection.setSelection(spanText, text.length());
				}
			}
			else
			{
				m_pEditBox.clearFocus();
				hideSystemKeyBoard( m_pApplication,m_pEditBox );
				ExportJavaFunction mTemp = ExportJavaFunction.GetInstance();
				if (mTemp!=null && mTemp.m_pEngine!=null) {
					mTemp.m_pEngine.setGameFocus();
				}
			}
			return true;
		}
		return false;
	}
	public void setMaxLength( int p_nMaxLength )
	{
		m_nMaxLength = p_nMaxLength;
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					setRealMaxLength();
				}
			}
		);
	}
	public boolean setRealMaxLength()
	{
		if( m_pEditBox != null )
		{
			m_pEditBox.setFilters( new InputFilter[]{ new InputFilter.LengthFilter( m_nMaxLength ) } );
			return true;
		}
		return false;
	}
	public void setLines( int p_nLines )
	{
		m_nLines = p_nLines;
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					setRealLines();
				}
			}
		);
	}
	public boolean setRealLines()
	{
		if( m_pEditBox != null )
		{
			m_pEditBox.setMaxLines(m_nLines);
			return true;
		}
		return false;
	}
	public void setStyle( String p_sStyle )
	{
		m_sStyle = p_sStyle;
		setRealStyle();
	}
	public void setRealStyle()
	{
		if ( m_sStyle.length() <= 0 ) return ;
		String[] vString = m_sStyle.split( ";" );
		int nLength = vString.length;
		if ( nLength <= 0 )
		{
			String[] vString1 = m_sStyle.split(":");
			if ( vString1.length < 2 )
			{
				Log.e(TAG,"java setRealStyle error");
				return;
			}
			paserStyle( vString1[0], vString1[1] );
		}
		else
		{
			for (String aVString : vString) {
				String[] vString2 = aVString.split(":");
				if (vString2.length < 2) {
					Log.e(TAG, "java setRealStyle error2");
					continue;
				}
				paserStyle(vString2[0], vString2[1]);
			}
		}
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					setLayoutParams();
					setRealTextSize();
				}
			}
		);
	}
	public void paserStyle( String styleKey, String styleValue )
	{
		try 
		{
			if (styleKey.equals("left")) 
			{
				if (styleValue.contains("px"))
				{
					styleValue = styleValue.substring(0, styleValue.length() - 2);
					
				} 
				m_nPosX = Integer.parseInt( styleValue );
			} 
			else if (styleKey.equals("top"))
			{
				if (styleValue.contains("px"))
				{
					styleValue = styleValue.substring(0, styleValue.length() - 2);
				} 
				m_nPosY = Integer.parseInt( styleValue );
			} 
			else if (styleKey.equals("width")) 
			{
				if (styleValue.contains("px"))
				{
					styleValue = styleValue.substring(0, styleValue.length() - 2);
				}
				m_nWidth = Integer.parseInt( styleValue );
			}
			else if (styleKey.equals("height")) 
			{
				if (styleValue.contains("px"))
				{
					styleValue = styleValue.substring(0, styleValue.length() - 2);
				}
				m_nHeight = Integer.parseInt( styleValue );
			} 
			else if (styleKey.equals("font-size")) 
			{
				if (styleValue.contains("px"))
				{
					styleValue = styleValue.substring(0, styleValue.length() - 2);
				} 
				m_nTexSize = Integer.parseInt( styleValue );
			}
		} 
		catch (Exception e) 
		{
			Log.e("",e.toString());
		}
	}
	public void onChanged(String s)
	{
		ConchJNI.inputChange(0);
		//m_pWebView.runScript("window.inputobjs["+m_nID+"].onInnerChanged('"+s+"');");
	}
	public void setCursorPosition( final int p_nPos )
	{
		m_bShow = true;
		m_bFocus = true;
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					setRealVisible();
					setRealFocus();
					int len=m_pEditBox.getText().length();
					if(p_nPos<0)
					{
						m_pEditBox.setSelection(0);
					}
					else if(p_nPos> len )
					{
						m_pEditBox.setSelection(len);
					}
					else
					{
						m_pEditBox.setSelection(p_nPos);
					}
				}
			}
		);
	}
}
