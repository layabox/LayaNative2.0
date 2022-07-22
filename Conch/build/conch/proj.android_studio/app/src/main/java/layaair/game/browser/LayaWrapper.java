package layaair.game.browser;

import android.Manifest;
import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.Activity;
import android.app.AlarmManager;
import android.app.AlertDialog;
import android.app.PendingIntent;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.net.ConnectivityManager;
import android.os.Build;
import android.support.v4.content.ContextCompat;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.webkit.ValueCallback;
import android.widget.AbsoluteLayout;
import android.widget.Toast;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.HashMap;
import java.util.Map.Entry;

import layaair.game.conch.ILayaEventListener;
import layaair.game.conch.LayaConch5;
import layaair.game.config.config;
import layaair.game.wrapper.ILayaLibWrapper;

@SuppressLint("Wakelock")
public class LayaWrapper implements ILayaLibWrapper{
	public static final int AR_CHECK_UPDATE = 1;
	public static final int AR_INIT_PLATFORM = 2;
	private static int 		m_nStartActivityType = 0;	//设置网络之后，再次调用到哪个函数的标记
	
	public static final int BACK_TO_MAIN = 0;
	public static final int REFRESH = 1;
	public static final int CLOSE_BIG = 2;
	
	//------------------------------------------------------------------------------
	static 	LayaWrapper	ms_layaEngine = null;
    public boolean 			m_bPopAD = true;
    static public AbsoluteLayout 	m_pAbsEditLayout = null;
    private long 			m_nBackPressTime=0;
    
    //kuo change code here
    public View			m_pExternalLoadingView = null;
    static private Toast mToast;  
    
    //------------------------------------------------------------------------------
	public SensorManager 		m_pSensorManager = null;
	public Sensor				m_pSensor = null;
	public SensorEventListener 	m_pSensorListener = null;
	public  int					SENSOR_OFFSET = 6;
	public double				m_fSensorX = 0;
	public double				m_fSensorY = 0;
	public double				m_fSensorZ = 0;
	public int 					m_nScreenOrientation = ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE;
	private String 				tempSoPath = "";
	private String 				tempSoFile = "";
	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------
	public static Activity 	m_LayaEngineContext = null;
	public static LayaConch5 m_pEngine =  null;
	
	
	//------------------------------------------------------------------------------
    public static LayaWrapper GetInstance()
    {
    	if (ms_layaEngine == null) {
			ms_layaEngine = new LayaWrapper();
		}
    	return ms_layaEngine; 
    }
    //------------------------------------------------------------------------------
    public LayaWrapper()
    {
    	
    }
    public void setLayaEventListener(ILayaEventListener _lis){
    	if(m_pEngine!=null)
    		m_pEngine.setLayaEventListener(_lis);
    }

    //------------------------------------------------------------------------------
    public void initEngine(Activity pContext)
    {
    	ms_layaMainActivity = pContext;
    	ms_layaEngine = this;
    	m_LayaEngineContext = pContext;
		ms_mCtx = m_LayaEngineContext;//.getApplicationContext();
		/*try{
			InputStream is = m_LayaEngineContext.getResources().getAssets().open("config.ini");
			config.GetInstance().init( is );
		}catch(Exception e){
			Log.e("","打开配置文件错误。");
        }*/
		m_pEngine = new LayaConch5(ms_mCtx);
		m_pEngine.game_conch3_SetIsPlug(false);
		if (tempSoPath.length()>0) {
			m_pEngine.setSoPath(tempSoPath);
		}
		if (tempSoFile.length()>0) {
			m_pEngine.setSoPath(tempSoFile);
		}
    }
    //------------------------------------------------------------------------------
    public void setSoPath(String pSoPath)
    {
    	tempSoPath = pSoPath;
    	if (m_pEngine!=null) {
			m_pEngine.setSoPath(pSoPath);
		}
    }
	public void setAlertTitle(String title) {
		if (m_pEngine!=null) {
			m_pEngine.setAlertTitle(title);
		}
	}
	public void  setStringOnBackPressed(String str) {
		if (m_pEngine!=null) {
			m_pEngine.setStringOnBackPressed(str);
		}
	}
  //------------------------------------------------------------------------------
    public void setSoFile(String pSoFile)
    {
    	tempSoFile = pSoFile;
    	if (m_pEngine!=null) {
			m_pEngine.setSoFile(pSoFile);
		}
    }
    //------------------------------------------------------------------------------
    public void setGameUrl(String pUrl)
    {
    	m_pEngine.setGameUrl(pUrl);
    }


	public  void setLocalizable(boolean b)
	{
		m_pEngine.setLocalizable(b);
	}
    //------------------------------------------------------------------------------
    public void startGame()
    {
    	EngineStart();
    }
	public String getCacheDir() {
		//return m_LayaEngineContext.getCacheDir().toString();
		String sCache = m_LayaEngineContext.getCacheDir().toString();
		String []vString = sCache.split("/");
		String sNewCache="";
		for( int i = 0; i < vString.length-1; i++ )
		{
			sNewCache += vString[i];
			sNewCache += "/";
		}
		return sNewCache;
	}
    // ------------------------------------------------------------------------------
    public void EngineStart()
    {
		String cachePath = getCacheDir();
        String strLayaCache = cachePath + "/LayaCache";
        File cacheFolder=new File(strLayaCache); 
        if(!cacheFolder.exists()) {
        	cacheFolder.mkdir();
        }
		AssetManager am = LayaWrapper.getLayaApplicationAsset();
		/*if (m_pExternalLoadingView!=null) {
			m_pEngine.setLoadingView(m_pExternalLoadingView);
		}else {
			m_pEngine.setLoadingView(m_pLoadingView);
		}*/
		m_pEngine.game_conch3_setAppWorkPath(cachePath);
		m_pEngine.game_conch3_setAssetInfo(am);
		m_pEngine.game_conch3_init();
		//准备localStorage目录
		File localStoragePath = new File(strLayaCache + "/localstorage");
		if (!localStoragePath.exists()) {
			if (!localStoragePath.mkdirs()) {
				Log.e("", "创建localstorage目录失败！");
			}
		}
		ConchJNI.SetLocalStoragePath(strLayaCache + "/localstorage");

		//环境都准备好了，开始流程了。
		if (PermisionUtils.checkExternalStoragePermission(ms_layaMainActivity)) {
			checkApkUpdate();
		}
    }
	public  boolean isOpenNetwork(Context context)
	{
		if (!config.GetInstance().m_bCheckNetwork)
			return true;
		ConnectivityManager connManager = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
		return connManager.getActiveNetworkInfo() != null && (connManager.getActiveNetworkInfo().isAvailable() && connManager.getActiveNetworkInfo().isConnected());
	}
	public void settingNetwork(final Context context, final int p_nType)
	{
		AlertDialog.Builder pBuilder = new AlertDialog.Builder(context);
		pBuilder.setTitle("连接失败，请检查网络或与开发商联系").setMessage("是否对网络进行设置?");
		// 退出按钮
		pBuilder.setPositiveButton("是", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface p_pDialog, int arg1) {
				Intent intent;
				try {
					String sdkVersion = android.os.Build.VERSION.SDK;
					if (Integer.valueOf(sdkVersion) > 10) {
						intent = new Intent(
								android.provider.Settings.ACTION_WIRELESS_SETTINGS);
					} else {
						intent = new Intent();
						ComponentName comp = new ComponentName(
								"com.android.settings",
								"com.android.settings.WirelessSettings");
						intent.setComponent(comp);
						intent.setAction("android.intent.action.VIEW");
					}
					((Activity)context).startActivityForResult(intent, p_nType);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
		pBuilder.setNegativeButton("否", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int which) {
				dialog.cancel();
				((Activity)context).finish();
			}
		});
		AlertDialog alertdlg = pBuilder.create();
		alertdlg.setCanceledOnTouchOutside(false);
		alertdlg.show();
	}
	public  void checkApkUpdate( Context context,final ValueCallback<Integer> callback)
	{
		if (isOpenNetwork(context)) {
			callback.onReceiveValue(1);
		} else {
			settingNetwork(context,1);
		}
	}
	public void checkApkUpdate() {
		checkApkUpdate(m_LayaEngineContext,new ValueCallback<Integer>() {
			@Override
			public void onReceiveValue(Integer integer) {
				if (integer.intValue() == 1) {
					InitView();
				} else {
					((Activity)m_LayaEngineContext).finish();
				}
			}
		});
	}
    //------------------------------------------------------------------------------
 	@TargetApi(Build.VERSION_CODES.HONEYCOMB)
 	public void InitView() {
 		View view = m_pEngine.game_conch3_get_view();
 		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
 			view.setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN);
 	    }
 		m_LayaEngineContext.setContentView(view);
 	}
 	//------------------------------------------------------------------------------
	//获取当前应用的版本名称
	static public String getAppVersionName() 
	{    
		String versionName = "";    
		int versioncode = 0;
		try {
			// ---get the package info---    
			PackageManager pm = LayaWrapper.ms_mCtx.getPackageManager();    
			PackageInfo pi = pm.getPackageInfo(LayaWrapper.ms_mCtx.getPackageName(), 0);    
			versionName = pi.versionName;    
			versioncode = pi.versionCode;  
			if (versionName == null || versionName.length() <= 0) {
				return "";
			}
		} catch (Exception e) {
			Log.e("VersionInfo", "Exception", e);
		}
		return versionName;
	}
	/*
	 * 从assets目录下拷贝文件 srcFile 只能是资源根目录下的文件
	 */
	public void CopyFileFromAssets(String srcFile, String destPath) {
		File curWorkingPath = new File(destPath);
		if (!curWorkingPath.exists()) {
			Log.i("", "mkdir:" + destPath);
			if (!curWorkingPath.mkdirs()) {
				// TODO 要验证，看文件夹和文件的处理方式
				Log.e("", "copyasserts error： 创建文件夹出错，dir=" + destPath);
			}
		}

		try {
			File outFile = new File(curWorkingPath, srcFile);
			InputStream in = null;

			try {
				in = m_LayaEngineContext.getAssets().open(srcFile);
				Log.i("", "copy file: " + srcFile);
			} catch (IOException e) {
				Log.e("", "open file err:" + srcFile);
				return;
			}

			OutputStream out = new FileOutputStream(outFile);
			// Transfer bytes from in to out
			byte[] buf = new byte[1024];
			int len;
			while ((len = in.read(buf)) > 0) {
				out.write(buf, 0, len);
			}
			in.close();
			out.close();
			Log.e("", "拷贝文件" + srcFile + "成功");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	// ------------------------------------------------------------------------------
	public void InitSensor() {
		m_pSensorManager = (SensorManager) m_LayaEngineContext.getSystemService(Context.SENSOR_SERVICE);
		m_pSensor = m_pSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
		m_pSensorListener = new SensorEventListener() {
			public void onAccuracyChanged(Sensor sensor, int accuracy) {
			}

			public void onSensorChanged(SensorEvent event) {
				double fX = event.values[SensorManager.DATA_X];
				double fY = event.values[SensorManager.DATA_Y];
				double fZ = event.values[SensorManager.DATA_Z];
				if (Math.abs(m_fSensorX - fX) > 0.1
						|| Math.abs(m_fSensorY - fY) > 0.1
						|| Math.abs(m_fSensorZ - fZ) > 0.1) {
					m_fSensorX = fX;
					m_fSensorY = fY;
					m_fSensorZ = fZ;
					if (fX >= (SENSOR_OFFSET - 1) && fX <= (SENSOR_OFFSET + 1)
							&& Math.abs(fY) < 2) {
						ConchJNI.onSensorChanged(-1);
						// Log.e("0", ">>>>>>>arc=-1" + ",x=" + fX + ",y=" + fY
						// + ",z=" + fZ );
					} else {
						double fArc = Math.atan2(fY, fX
								- SENSOR_OFFSET)
								- Math.PI / 2.0f;
						fArc = (fArc < 0) ? (fArc + Math.PI * 2) : fArc;
						ConchJNI.onSensorChanged((float) (fArc));
						// Log.e("0", ">>>>>>>arc=" + fArc + ",x=" + fX + ",y="
						// + fY + ",z=" + fZ );
					}
				}
			}
		};
		m_pSensorManager.registerListener(m_pSensorListener, m_pSensor,
				SensorManager.SENSOR_DELAY_GAME);
	}
	
	public void handleUncaughtException (Thread thread, Throwable e){
		e.printStackTrace(); // not all Android versions will print the stack trace automatically
		Intent intent = new Intent ();
		intent.setAction ("com.dawawa.SEND_LOG"); 
		intent.setFlags (Intent.FLAG_ACTIVITY_NEW_TASK); // required when starting from Application
		m_LayaEngineContext.startActivity (intent);
		System.exit(1); // kill off the crashed app
	  }	
	// ------------------------------------------------------------------------------
    public static void onPopMenu(int id)
    {
    	switch (id) {
		case BACK_TO_MAIN:
			//Toast.makeText(GetInstance(), "BACK_TO_MAIN", Toast.LENGTH_SHORT).show();
			ConchJNI.onRunCmd(0x400 + 3333 + 104, -1, 0);
			break;
		case CLOSE_BIG:
			//Toast.makeText(GetInstance(), "CLOSE_BIG", Toast.LENGTH_SHORT).show();
			break;
		case REFRESH:
			//Toast.makeText(GetInstance(), "REFRESH", Toast.LENGTH_SHORT).show();
			ConchJNI.onRunCmd(0x400 + 3333 + 102, 0, 0);
			break;
		}
    }
	// ------------------------------------------------------------------------------
//	public void initPlatform() {
//		// 初始化平台c
//		m_pEngine.setMarketName( config.GetInstance().m_sMarketPlatformClassname!=null?config.GetInstance().m_sMarketPlatformClassname:"");
//		if ((config.GetInstance().m_sMarketPlatformClassname != null)
//				&& (config.GetInstance().m_sMarketPlatformClassname.length() > 1)) {
//			if (isOpenNetwork()) {
//				Log.e("0", "==============Java流程 第三方平台初始化");
//				m_pPlatform.LP_Init(m_LayaEngineContext);
//			} else {
//				settingNetwork(AR_INIT_PLATFORM);
//			}
//		} else {
//			Log.e("0", "==============Java流程 没有第三方平台直接调用 PlatformInitOK");
//			PlatformInitOK(0);
//		}
//	}

	// ------------------------------------------------------------------------------
	public void PlatformInitOK(int p_nFlag) {
		Log.e("0", "==============Java流程 InitMainCanvas()");
		InitView();
	}

	// ---------------------------------------------------------------------------
	public void onActivityResult(int requestCode, int resultCode,Intent intent) {
		if (requestCode == AR_INIT_PLATFORM) {
			InitView();
		} else if (requestCode == AR_CHECK_UPDATE) {
			checkApkUpdate();
		}
		m_pEngine.onActivityResult(requestCode,resultCode,intent);
		/*
		 * if( m_pPlatform != null ) { m_pPlatform.LP_handleActivityResult(
		 * requestCode, resultCode, intent); }
		 */
	}

	public void setResolution(int w, int h){
		m_pEngine.setResolution(w,h);
	}

	public void _enableOnLayout(boolean b){
		m_pEngine._enableOnLayout(b);
	}

	// ------------------------------------------------------------------------------
	public void onPause() {
		if (m_pSensorManager != null) {
			m_pSensorManager.unregisterListener(m_pSensorListener);
		}
		m_pEngine.game_conch3_onPause();
	}
    //------------------------------------------------------------------------------
    public void onResume() {

        if( m_pSensorManager != null ) {
        	m_pSensorManager.registerListener( m_pSensorListener, m_pSensor, SensorManager.SENSOR_DELAY_GAME );
        }
        m_pEngine.game_conch3_onResume();
    }
    
    public void onDestroy(){
    	m_pEngine.onDestroy();
		DelInstance();
    }

	private static void DelInstance()
	{
		m_pEngine=null;
		m_LayaEngineContext=null;
		ms_layaEngine=null;
		ms_layaMainActivity=null;
		ms_mCtx=null;
	}
	@Override
	public void onStop() {
		m_pEngine.onStop();
	}

	@Override
	public void onNewIntent(Intent intent) {
		m_pEngine.onNewIntent(intent);
	}

	@Override
	public void onRestart() {
		m_pEngine.onRestart();
	}

	public void setInterceptKey(boolean _intercept)
    {
    	m_pEngine.setInterceptKey(_intercept);
    }
    public void setOptions(HashMap<String, Object> _option)
    {
		for (Object o : _option.entrySet()) {
			Entry entry = (Entry) o;
			String key = (String) entry.getKey();
			Log.i("setOption", "setOptions() key=" + key + " value=" + entry.getValue());
			if (key.compareToIgnoreCase("url") == 0) {
				m_pEngine.setGameUrl((String) entry.getValue());
			} else if (key.compareToIgnoreCase("sopath") == 0) {
				setSoPath((String) entry.getValue());
			} else if (key.compareToIgnoreCase("sofile") == 0) {
				setSoFile((String) entry.getValue());
			}
		}
    }

    public static boolean IsFinishing(Activity context)
    {
    	if(context==null||context.isFinishing())
    		return true;
    	return false;
    }
    // ------------------------------------------------------------------------------
    public static void ShowMessage(String pMessage, LayaConch5 _pEngine){
		_pEngine.showMessage(pMessage);
	}
	// ------------------------------------------------------------------------------
	static public int GetScreenWidth() {
		DisplayMetrics pDm = new DisplayMetrics();
		LayaWrapper.getLayaApplicationActivity().getWindowManager().getDefaultDisplay().getMetrics( pDm );
		return pDm.widthPixels;
	}

	// ------------------------------------------------------------------------------
	static public int GetScreenHeight() {
		DisplayMetrics pDm = new DisplayMetrics();
		LayaWrapper.getLayaApplicationActivity().getWindowManager().getDefaultDisplay().getMetrics( pDm );
		return pDm.heightPixels;
	}
	
	// ------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------
	static public float GetScreenInch() {
		DisplayMetrics pDm = new DisplayMetrics();
		LayaWrapper.getLayaApplicationActivity().getWindowManager().getDefaultDisplay().getMetrics( pDm );
		int nWidth = pDm.widthPixels;
		int nHeight = pDm.heightPixels;
		float nDpi = pDm.densityDpi;
		return (float) (Math.sqrt(nWidth * nWidth + nHeight * nHeight) / nDpi);
	}

	// ------------------------------------------------------------------------------
	public Bitmap getResImage(String name) {
		ApplicationInfo pAppInfo = m_LayaEngineContext.getApplicationInfo();
		int nResID = m_LayaEngineContext.getResources().getIdentifier(name, "drawable",pAppInfo.packageName);
		return BitmapFactory.decodeResource(m_LayaEngineContext.getResources(), nResID);
	}

	// ------------------------------------------------------------------------------
	static public void reloadApp(){
		ConchJNI.reloadJS();
	}

	// ------------------------------------------------------------------------------
	// 通过 AlarmManager 来重新启动应用
	public void restartApp() {
		Intent intent = new Intent(m_LayaEngineContext.getApplicationContext(), LayaWrapper.class);
		PendingIntent restartIntent = PendingIntent.getActivity(
				m_LayaEngineContext.getApplicationContext(), 0, intent,Intent.FLAG_ACTIVITY_NEW_TASK);
		// 退出程序
		AlarmManager mgr = (AlarmManager) m_LayaEngineContext.getSystemService(Context.ALARM_SERVICE);
		mgr.set(AlarmManager.RTC, System.currentTimeMillis() + 1000,
				restartIntent); // 1秒钟后重启应用
		m_LayaEngineContext.finishActivity(0);
	}

	// ------------------------------------------------------------------------------
	static public void MyJSAlert(String title, String msg, int p_nCallbackType, final LayaConch5 _gameEngine) {
  		if(_gameEngine!=null)
	 		 _gameEngine.alertJS(title,msg,p_nCallbackType);
	}
	//------------------------------------------------------------------------------
	static public void setScreenWakeLock( boolean p_bWakeLock ) {
		Activity activity = LayaWrapper.getLayaApplicationActivity();
		if(activity==null)
		{
			return;
		}
		if( p_bWakeLock )
		{
			LayaWrapper.getLayaApplicationActivity().getWindow().addFlags( WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON );
		}
		else
		{
			LayaWrapper.getLayaApplicationActivity().getWindow().clearFlags( WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON );
		}
	}
	// ------------------------------------------------------------------------------
	/* 
	 * The follow code for plugin implementation
	 * author:kuo
	 *{{ begin
	 */
	static private Activity ms_layaMainActivity=null;
	static public Context ms_mCtx=null;
	
	static public Context  getLayaApplicationContext()
	{
		return ms_mCtx;
	}
	
	static public Activity getLayaApplicationActivity()
	{
		return ms_layaMainActivity;
	}
		
	static public AssetManager  getLayaApplicationAsset()
	{
		return ms_layaMainActivity.getAssets();
	}
	

	@Override
	public void setLoadingView(View view) {
		m_pExternalLoadingView = view;		
	}
}
