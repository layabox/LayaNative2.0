package layaair.game.config;

import android.util.Log;
import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;

public class config 
{
	private static config ms_config = null;
	public boolean m_bCheckNetwork=false;
	public String	m_sWebviewUrl = null;
	public String	m_sConchGameUrl = null;
	public boolean  m_bBackkeyWebviewHide=false;
	public int m_nThreadMode = 2;
	public int m_nDebugMode = 0;
	public int m_nDebugPort = 5959;
	private Properties m_pProperties = null;
	//------------------------------------------------------------------------------
	//市场相关的
	public int 		m_nMarketWaitScreenBKColor=0xffffffff;		//市场等待的背景颜色

	public static config GetInstance()
	{
		if( ms_config == null )
		{
			ms_config = new config();
		}
		return ms_config;
	}

	public static void  DelInstance()
	{
		ms_config=null;
	}
	public config()
	{
		
	}
	private int getColor(String color)
	{
		if(color!=null) {
			boolean hasAlpha = color.length() <= 6;
			int nColor = (int) (Long.parseLong(color, 16));
			if (hasAlpha) {
				nColor |= 0xff000000;
			}
			return nColor;
		}
		return 0;
	}
	public String getProperty( final String name,final String defaultValue )
	{
		if( m_pProperties != null)
		{
			return m_pProperties.getProperty(name,defaultValue);
		}
		else
		{
			Log.e("LayaBox", "getProperty: error m_pProperties==null name=" + name );
		}
		return null;
	}
	public String getProperty( final String name)
	{
		if( m_pProperties != null)
		{
			return m_pProperties.getProperty(name);
		}
		else
		{
			Log.e("LayaBox", "getProperty: error m_pProperties==null name=" + name );
		}
		return null;
	}
	public boolean init( InputStream pInputStream )
	{
		if(m_pProperties == null) {
			m_pProperties = new Properties();
		}
		try {
			if(pInputStream==null)
				return false;
			m_pProperties.load(pInputStream);
			m_bCheckNetwork=!"0".equals( m_pProperties.getProperty("CheckNetwork","0"));
			m_sWebviewUrl=m_pProperties.getProperty("WebviewUrl");
			m_sConchGameUrl=m_pProperties.getProperty("ConchGameUrl");
			m_bBackkeyWebviewHide=!"0".equals( m_pProperties.getProperty("BackKeyWebviewHide","0"));
			int threadMode  = Integer.parseInt(m_pProperties.getProperty("ThreadMode","2"));
			if (threadMode == 1 || threadMode == 2) {
				m_nThreadMode = threadMode;
			}

			int debugMode  = Integer.parseInt(m_pProperties.getProperty("JSDebugMode","0"));
			if (debugMode == 0 || debugMode == 1 || debugMode == 2) {
				m_nDebugMode = debugMode;
			}
			m_nDebugPort = Integer.parseInt(m_pProperties.getProperty("JSDebugPort","5919"));
			return true;
		} catch (IOException e) {
			m_pProperties=null;
			e.printStackTrace();
			return false;
		}
	}
}
