package layaair.game.conch;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.InputStream;
import java.lang.reflect.Method;
import java.util.Vector;

import layaair.game.browser.LayaVideoPlayer;
import layaair.game.network.NetworkReceiver;
import layaair.game.browser.ExportJavaFunction;
import layaair.game.browser.ConchJNI;
import layaair.game.browser.LayaEditBox;
import layaair.game.browser.LayaWebView;
import layaair.game.browser.ConchCanvas;
import layaair.game.config.config;
import layaair.game.device.DevID;
import layaair.game.utility.LayaAudioMusic;
import layaair.game.utility.ProcessInfo;
import layaair.game.utility.Utils;

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.Activity;
import android.app.ActivityManager;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.net.ConnectivityManager;
import android.os.Build;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnGenericMotionListener;
import android.view.View.OnKeyListener;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.AbsoluteLayout;
import android.widget.RelativeLayout;
import android.widget.Toast;

import javax.microedition.khronos.opengles.GL10;

import static android.content.Context.SENSOR_SERVICE;

public class LayaConch5 implements ILayaGameEgine,OnKeyListener {
	// view
	private static final String TAG = "LayaConch5";
	public boolean localizable=false;
	public AbsoluteLayout m_pAbsLayout = null;
	public LayaWebView m_pLayaWebView = null;
	public LayaEditBox m_pEditBox = null;
	private AbsoluteLayout m_pEditBoxLayout = null;
	public ConchCanvas m_pCavans = null;
	public DevID m_pDevID = null;
	private ILayaEventListener m_layaEventListener = null;
	private boolean m_interceptKey = false;
	public AssetManager m_AM = null;
	public Context mCtx = null;
	public boolean m_bIsPlug = true;
	public String m_strUrl = "";
	public String m_strExt = "";
	public boolean m_bHorizontalScreen ; // 是否横屏
	private NetworkReceiver m_pNetWorkReveiver;
	private long m_nBackPressTime = 0;
	protected int m_nDownloadThreadNum = 3;
	protected String m_strCachePath = "";
	protected String m_strExpansionMainPath = "";
	protected String m_strExpansionPatchPath = "";
	static public String m_strSoPath = "";
	static public String m_strJarFile = "";
	static public String m_strSoFile = "/liblayaair.so";
	static Bundle m_marketBundle;
	static public final String MARKET_MARKETNAME = "marketName";
	static public final String MARKET_WAITSCREENBKCOLOR = "waitscreenbkcolor";
	static public final String MARKET_ENTERPLATFORMTYPE = "enterplatformtype";
	static public final String MARKET_EXITSHOWWEBURL = "exitshowweburle";
	static public final String MARKET_SERVERNAME = "servername";
	static public final String MARKET_PAYTYPE = "paytype";
	static public final String MARKET_LOGINTYPE = "logintype";
	static public final String MARKET_CHARGETYPE = "chargetype";
	private SensorManager mSensorManager = null;
	private Sensor mSensor = null;
	private Sensor orientationSensor=null;
	private float x, y, z;
	private boolean  mBIsSensor=false;
	private int canvas_res_w=-1;	//外部设定的canvas的大小。-1表示没有设置。因为设置的时候，canvas可能还没有创建。所以需要保存。
	private int canvas_res_h=-1;
	private boolean m_bEnableOnLayout =false;
	public String m_appVersion = "";
	public String m_appLocalVersion = "";

	public static LayaConch5 ms_layaConche = null;

	private LayaVideoPlayer m_pLayaVideoPlayer;

	private Vector<LayaVideoPlayer> m_videoPlayers = new Vector<LayaVideoPlayer>();

	public String 	m_strAlertTitle = "";
	public String  m_strOnBackPressed = "";
	public static LayaConch5 GetInstance() {
		if (ms_layaConche == null) {
			ms_layaConche = new LayaConch5();
		}
		return ms_layaConche;
	}

	public LayaConch5() {
		ms_layaConche = this;
	}

	public void addVideoPlayer(LayaVideoPlayer player)
	{
		m_videoPlayers.add( player );
	}

	public void removeVideoPlayer(LayaVideoPlayer player)
	{
		m_videoPlayers.remove( player );
	}

	public  void setSensorAble(boolean a)
	{
		if(mBIsSensor!=a)
		{
			mBIsSensor=a;
			if(mBIsSensor){
				registerSensor();
			}
			else {
				unRegisterSensor();
			}
		}
	}

	private void registerSensor()
	{
		if(mSensorManager==null) {
			mSensorManager = (SensorManager)mCtx.getSystemService(SENSOR_SERVICE);
			mSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
			orientationSensor=mSensorManager.getDefaultSensor(Sensor.TYPE_ORIENTATION);
		}
		mSensorManager.registerListener(lsn, orientationSensor, SensorManager.SENSOR_DELAY_NORMAL);
		mSensorManager.registerListener(lsn, mSensor, SensorManager.SENSOR_DELAY_NORMAL);
	}

	private void unRegisterSensor()
	{
		if(mSensorManager!=null) {
			mSensorManager.unregisterListener(lsn);
		}
	}

	public LayaConch5(Context ctx) {
		setContext(ctx);
		Configuration configuration=ctx.getResources().getConfiguration();
		try {
			m_bHorizontalScreen = (configuration.screenWidthDp > configuration.screenHeightDp);
		}
		catch (NoSuchFieldError e)
		{
			WindowManager wm = (WindowManager) (mCtx
					.getSystemService(Context.WINDOW_SERVICE));
			DisplayMetrics dm = new DisplayMetrics();
			wm.getDefaultDisplay().getMetrics(dm);
			m_bHorizontalScreen= dm.widthPixels>dm.heightPixels;
		}
		m_pNetWorkReveiver = new NetworkReceiver();
		ms_layaConche = this;
		m_appVersion = getLocalVersionName(ctx);
		m_appLocalVersion = getLocalVersion(ctx);
	}

	public void setContext(Context ctx) {
		if (ctx == null)
			return;
		mCtx = ctx;
		m_pDevID = new DevID(ctx);
	}
	SensorEventListener lsn = new SensorEventListener() {
		@Override
		public void onSensorChanged(SensorEvent event) {
		    int  a=	event.sensor.getType();
			x = event.values[SensorManager.DATA_X];
			y = event.values[SensorManager.DATA_Y];
			z = event.values[SensorManager.DATA_Z];
			if(a==Sensor.TYPE_ORIENTATION)
			{
				ConchJNI.handleDeviceOrientationEvent(x,y,z);
			}
			else {
				ConchJNI.handleDeviceMotionEvent(0f, 0f, 0f, x, y, z, 0f, 0f, 0f, 1f);
			}
		}
		@Override
		public void onAccuracyChanged(Sensor sensor, int accuracy) {
			// TODO Auto-generated method stub
			int a=accuracy;
		}
	};

	public void onCreate() {
		// 监听网络
		Log.e(TAG, ">>>>>>>conchjar android-2.0.8");
		IntentFilter pFilter = new IntentFilter();
		pFilter.addAction(ConnectivityManager.CONNECTIVITY_ACTION);
		try 
		{
			mCtx.registerReceiver(m_pNetWorkReveiver, pFilter);
		} 
		catch (Exception e) 
		{
			Log.i("0","registerReceiver error m_pNetWorkReveiver=" + m_pNetWorkReveiver );
		}
		Log.e(TAG, "plugin-----------------onCreate() ");
		ActivityManager am = (ActivityManager) mCtx.getSystemService(Context.ACTIVITY_SERVICE);
		ProcessInfo.init(am);
		InputStream in = getClass().getResourceAsStream("/assets/config.ini");
		config.GetInstance().init(in);

		boolean initedNative = false;
		if (!m_bIsPlug) {
			if (m_strSoPath.length() > 0) {
				String pluginPath = getSoPath() + m_strSoFile;// "libegret.so";
				initedNative = ConchJNI.initNativeLibrary(pluginPath, true);
				if (!initedNative) {
					throw new RuntimeException("Failed to load native runtime library");
				}
			} else {
				initedNative = ConchJNI.initNativeLibrary("layaair", false);
				if (!initedNative) {
					throw new RuntimeException("Failed to load native runtime library");
				}
			}
		} else {
			String pluginPath = getSoPath() + m_strSoFile;// "libegret.so";
			initedNative = ConchJNI.initNativeLibrary(pluginPath, true);
			if (!initedNative) {
				throw new RuntimeException("Failed to load native runtime library");
			}
		}
		// 先配置一下
		ConchJNI.configSetIsPlug(m_bIsPlug);
		if (m_strUrl.length() > 0) {
			ConchJNI.configSetURL(m_strUrl);
		}
		ExportJavaFunction.m_nState = 0;
		ExportJavaFunction expjava = ExportJavaFunction.GetInstance();
		expjava.m_pEngine = this;
		expjava.Init(mCtx);
		String _marketName = getMarketBundle().getString(MARKET_MARKETNAME);

		PlatformInitOK(0);

	}
	public void PlatformInitOK(int p_nFlag) {
		Log.e("0", "==============Java流程 InitMainCanvas()");
		EngineStart();
	}

	static class CacheInfo{
		int id;
		String path;
	}
	//cachePath 需要到 appCache 那一层。
	public static Vector<CacheInfo> getCachedApp(String cachePath){
		Vector<CacheInfo> vecFile = new Vector<CacheInfo>();
		File file = new File(cachePath);
		File[] subFile = file.listFiles();

		for (File aSubFile : subFile) {
			CacheInfo cinfo = new CacheInfo();
			// 判断是否为文件夹
			if (aSubFile.isDirectory()) {
				String filename = aSubFile.getName();
				//vecFile.add(filename);
				cinfo.path = cachePath + "/" + filename;
				if (filename.compareTo("sessionFiles") == 0) {
					//vecFile.add("-1");//临时缓存
					cinfo.id = -1;
				} else {
					//找有没有id
					String appidfile = cachePath + "/" + filename + "/sourceid/appid";
					File appidFile = new File(appidfile);
					if (appidFile.exists()) {
						try {
							BufferedReader br = new BufferedReader(new FileReader(appidfile));
							String data = br.readLine();
							//vecFile.add(data);
							cinfo.id = Integer.parseInt(data);
						} catch (Exception e) {
							//vecFile.add("-2");//无法获得
							cinfo.id = -2;
						}
					} else {
						//vecFile.add("-3");//无法获得
						cinfo.id = -3;
					}
				}
			}
			vecFile.add(cinfo);
		}
        return vecFile;		
	}
	/**
	 * 删除一个目录。
	 * @param file
	 */
	public static void deletePath(File file) {
		Log.e("2jni", "cacheMgr delete dir: " + file.toString());
		if (file.isFile()) {
			file.delete();
			return;
		}

		if (file.isDirectory()) {
			File[] childFiles = file.listFiles();
			if (childFiles == null || childFiles.length == 0) {
				file.delete();
				return;
			}

			for (int i = 0; i < childFiles.length; i++) {
				deletePath(childFiles[i]);
			}
			file.delete();
		}
	}

	//TEST
	public void EngineStart() {
		String strLayaCache = getAppCacheDir() + "/LayaCache";
		File cacheFolder = new File(strLayaCache);
		if (!cacheFolder.exists()) {
			cacheFolder.mkdir();
		}
		Log.e(TAG, "plugin-----------------EngineStart() = " + (strLayaCache + "/localstorage"));
		// 准备localStorage目录
		File localStoragePath = new File(strLayaCache + "/localstorage");
		if (!localStoragePath.exists()) {
			if (!localStoragePath.mkdirs()) {
				Log.e("", "创建localStorage目录失败！");
				ExportJavaFunction.alert("创建游戏目录失败，请清理空间或重启应用再试");
				game_plugin_exitGame();
				return;
			}
		}
		ConchJNI.SetLocalStoragePath(strLayaCache + "/localstorage");

		InitView();
		String cachePath = getAppCacheDir() + "/LayaCache";
		if (m_AM != null) {
			ConchJNI.InitDLib(m_AM,getDownloadThreadNum(), "cache", cachePath, m_strExpansionMainPath == null ? "" : m_strExpansionMainPath,m_strExpansionPatchPath == null ? "" : m_strExpansionPatchPath, config.GetInstance().m_nThreadMode,config.GetInstance().m_nDebugMode,config.GetInstance().m_nDebugPort);
		}
		else {
			ConchJNI.InitDLib(null,getDownloadThreadNum(), getJarFile(), cachePath, m_strExpansionMainPath == null ? "" : m_strExpansionMainPath,m_strExpansionPatchPath == null ? "" : m_strExpansionPatchPath, config.GetInstance().m_nThreadMode,config.GetInstance().m_nDebugMode,config.GetInstance().m_nDebugPort);
		}
	}

	public void  dispatchGL(GL10 gl,int w,int h)
	{
		ConchJNI.captureScreenCallBack(w,h,Utils.screenShot(gl,w,h));
	}

	public  boolean isOpenNetwork()
	{
		if (!config.GetInstance().m_bCheckNetwork)
			return true;
		ConnectivityManager connManager = (ConnectivityManager) mCtx.getSystemService(Context.CONNECTIVITY_SERVICE);
		return connManager.getActiveNetworkInfo() != null && (connManager.getActiveNetworkInfo().isAvailable() && connManager.getActiveNetworkInfo().isConnected());
	}

	// 设备控制相关
	public void setScreenOrientation(int ori) {
		// IGameApp.setScreenOrientation(ori);
	}

	public void setScreenWakeLock( boolean p_bWakeLock ) 
	{
		try
		{
			Log.i("0",">>>>>>screenWakeLock wake=" + p_bWakeLock );
			Activity activity = (Activity)(mCtx);
			if(activity==null)
			{
				return;
			}
			if( p_bWakeLock )
			{
				Log.i("0",">>>>>>screenWakeLock ok" + p_bWakeLock );
				activity.getWindow().addFlags( WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON );
			}
			else
			{
				Log.i("0",">>>>>>screenWakeLock ok" + p_bWakeLock );
				activity.getWindow().clearFlags( WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON );
			}
		}
		catch( Exception e )
		{
			Log.i("0",">>>>>>screenWakeLock error" );
		}
	}

	@Override
	public boolean onKey(View v, int keyCode, KeyEvent event)
	{
		Log.e("input", ">>>>>>>>>>>>>>>>" + keyCode);
		if(event.getAction() == KeyEvent.ACTION_DOWN)
		{
			ConchJNI.handleKeyEvent(keyCode, KeyEvent.ACTION_DOWN);
		}
		else if(event.getAction() == KeyEvent.ACTION_UP)
		{
			ConchJNI.handleKeyEvent(keyCode, KeyEvent.ACTION_UP);
		}
		ExportJavaFunction exp = ExportJavaFunction.GetInstance();
		if(exp!=null&&exp.m_pEngine.getIsPlug() ){
			//此处方便js接管Back事件，鉴于qq浏览器在没有初始化完成的时候就调用了back造成非法现予以屏蔽
			return false;
		}
		else
		{
			Log.e("","exp is null");
		}
		if (keyCode == KeyEvent.KEYCODE_BACK && event.getAction() == KeyEvent.ACTION_DOWN)
		{
			Log.e("", "onKey = " + keyCode);
			if (!ConchJNI.onBackPressed())
			{
				long curtm = System.currentTimeMillis();
				int MaxDelay = 3500; // Toast.LENGTH_LONG 对应的时间
				if (m_nBackPressTime == 0 || (m_nBackPressTime > 0 && curtm - m_nBackPressTime > MaxDelay))
				{
					m_nBackPressTime = System.currentTimeMillis();
					Activity context=(Activity)mCtx;
					if (isFinishing(context))return true;
					Toast.makeText(mCtx, m_strOnBackPressed, Toast.LENGTH_LONG).show();
				}
				else
				{
					// 退出
					game_plugin_exitGame();
				}
				return false;
			}
		}
		return keyCode == KeyEvent.KEYCODE_BACK && event.getAction() == KeyEvent.ACTION_UP;
	}

	@Override
	public void onKeyEvent(String keyEvent,int keyCode) {
		if( "onKeyUp".equals(keyEvent) ){
			ConchJNI.handleKeyEvent(keyCode, KeyEvent.ACTION_UP);
		}else if("onKeyDown".equals(keyEvent)){
			ConchJNI.handleKeyEvent(keyCode, KeyEvent.ACTION_DOWN);
		}
	}
	@SuppressLint("NewApi") @TargetApi(Build.VERSION_CODES.HONEYCOMB)
	void InitView() {
		m_pCavans = new ConchCanvas(mCtx);
		if (m_pAbsLayout == null) {
			if(m_bEnableOnLayout) {
				m_pAbsLayout = new AbsoluteLayout(this.mCtx) {
					@Override
					protected void onLayout(boolean changed, int l, int t, int r, int b) {
						super.onLayout(changed, l, t, r, b);
						Log.i("fix", "onLayout (" + l + "," + t + "," + r + "," + b + ")");
						if (isHUAWEI() && m_pCavans != null && m_pCavans.m_nFixedWidth > 0 && m_pCavans.m_nFixedHeight > 0) {
							WindowManager wm = (WindowManager) (mCtx.getSystemService(Context.WINDOW_SERVICE));
							DisplayMetrics dm = new DisplayMetrics();
							wm.getDefaultDisplay().getMetrics(dm);
							if (m_bHorizontalScreen && dm.heightPixels > dm.widthPixels
									|| !m_bHorizontalScreen && dm.widthPixels > dm.heightPixels) {
								int tmp = dm.heightPixels;
								dm.heightPixels = dm.widthPixels;
								dm.widthPixels = tmp;
							}
							m_pCavans.m_nDisplayWidth = dm.widthPixels;
							m_pCavans.m_nDisplayHeight = dm.heightPixels;
							m_pCavans.setFixedSize(m_pCavans.m_nFixedWidth, m_pCavans.m_nFixedHeight);
						}
					}
				};
			}
			else{
				m_pAbsLayout = new AbsoluteLayout(this.mCtx);
			}

			m_pAbsLayout.setBackgroundColor(0x00ffffff);
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB){
				m_pAbsLayout.setOnGenericMotionListener(new OnGenericMotionListener() {
					@Override
					public boolean onGenericMotion(View arg0, MotionEvent event) {
						// TODO Auto-generated method stub
						//if( MotionEvent.TOOL_TYPE_UNKNOWN == event.getToolType(0))
						int eventSource = event.getSource();
						if( (eventSource & InputDevice.SOURCE_JOYSTICK) == InputDevice.SOURCE_JOYSTICK
		                && event.getAction() == MotionEvent.ACTION_MOVE)
						{
							/* 获取左摇杆值 */
							float leftX = event.getAxisValue(MotionEvent.AXIS_X);
							float leftY = event.getAxisValue(MotionEvent.AXIS_Y);
							/* 获取右摇杆值 */
							float z = event.getAxisValue(MotionEvent.AXIS_Z);
							float rz = event.getAxisValue(MotionEvent.AXIS_RZ);

							float lt = event.getAxisValue(MotionEvent.AXIS_LTRIGGER);
							float rt = event.getAxisValue(MotionEvent.AXIS_RTRIGGER);
							ConchJNI.handleJoystickEvent(leftX, leftY, z, rz, lt, rt);
						}
						return false;
					}
				});
			}
			m_pAbsLayout.setOnKeyListener(this);
		}
		int uiParm=0;
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
			uiParm=View.SYSTEM_UI_FLAG_FULLSCREEN;
		}
		if(uiParm!=0)m_pAbsLayout.setSystemUiVisibility(uiParm);

		RelativeLayout.LayoutParams lp = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);

		m_pCavans.setLayoutParams(lp);

		WindowManager wm = (WindowManager) (mCtx
				.getSystemService(Context.WINDOW_SERVICE));
		DisplayMetrics dm = new DisplayMetrics();
		wm.getDefaultDisplay().getMetrics(dm);
		if (m_bHorizontalScreen && dm.heightPixels > dm.widthPixels
				|| !m_bHorizontalScreen && dm.widthPixels > dm.heightPixels) {
			int tmp = dm.heightPixels;
			dm.heightPixels = dm.widthPixels;
			dm.widthPixels = tmp;
		}
		m_pCavans.m_nDisplayWidth = dm.widthPixels;
		m_pCavans.m_nDisplayHeight = dm.heightPixels;

		if(canvas_res_w>0 || canvas_res_h>0){
			m_pCavans.setFixedSize(canvas_res_w,canvas_res_h);
			//下面这个用设置么。
			canvas_res_w=-1;
			canvas_res_h=-1;
		}
		m_pAbsLayout.addView(m_pCavans);
		if (m_pLayaWebView == null) {
			try
			{
				m_pLayaWebView = new LayaWebView(mCtx, this);
				if( config.GetInstance().m_sWebviewUrl != null ) {
					m_pLayaWebView.showWebView( config.GetInstance().m_sWebviewUrl, 0, 0, GetScreenWidth(), GetScreenHeight());
				}
			}
			catch(Exception e)
			{
				Log.e("LayaBoxwebView", ">>>>>>>>>>>>>>Exception"+e.toString());
			}
			catch(Throwable e)
			{
				Log.e("LayaBoxwebView", ">>>>>>>>>>>>>>throwable"+e.toString());
			}
		}

		if (m_pEditBox == null) {

			m_pEditBoxLayout = new AbsoluteLayout(mCtx);
			m_pEditBoxLayout.setBackgroundColor(0x00000000);

			m_pEditBox = new LayaEditBox(mCtx);
			m_pEditBox.CreateEditBox();

			m_pEditBoxLayout.addView(m_pEditBox.m_pEditBox,
					new AbsoluteLayout.LayoutParams(GetScreenWidth(), GetScreenHeight(), 0, 0));
			m_pAbsLayout.addView(m_pEditBoxLayout, new ViewGroup.LayoutParams(
					ViewGroup.LayoutParams.WRAP_CONTENT,
					ViewGroup.LayoutParams.WRAP_CONTENT));
		}

		LayaVideoPlayer.SetDefaultVideoContext(mCtx, m_pAbsLayout);

//		if(m_pLayaVideoPlayer == null)
//		{
//			m_pLayaVideoPlayer = new LayaVideoPlayer(mCtx, m_pAbsLayout);
//			m_pLayaVideoPlayer.setX(700);
//			m_pLayaVideoPlayer.setY(700);
//			m_pLayaVideoPlayer.setWidth(300);
//			m_pLayaVideoPlayer.setHeight(500);
//			m_pLayaVideoPlayer.Load("https://cdn3.maxuscloud.com/wgame2/jjcgame/shipin/sp01.mp4");
//			m_pLayaVideoPlayer.Play();
//		}

		if (m_interceptKey) {
			Log.i("layaConch5", "m_interceptKey OK!");
			m_pAbsLayout.setFocusable(true);
			m_pAbsLayout.setFocusableInTouchMode(true);
			m_pAbsLayout.requestFocus();
		}
		/*
        if(isHUAWEI()) {
            //boolean nb = checkDeviceHasNavigationBar(getGameContext());
            getGameContext().getContentResolver().registerContentObserver(Settings.Global.getUriFor
                    ("navigationbar_is_min"), true, mNavigationStatusObserver);
        }
        */
	}

    /**
     * 是否是华为
     */
    public static boolean isHUAWEI() {
        return android.os.Build.MANUFACTURER.equals("HUAWEI");
    }

    //获取是否存在NavigationBar
    public static boolean checkDeviceHasNavigationBar(Context context) {
        boolean hasNavigationBar = false;
        try {
            Resources rs = context.getResources();
            int id = rs.getIdentifier("config_showNavigationBar", "bool", "android");
            if (id > 0) {
                hasNavigationBar = rs.getBoolean(id);
            }
            Class systemPropertiesClass = Class.forName("android.os.SystemProperties");
            Method m = systemPropertiesClass.getMethod("get", String.class);
            String navBarOverride = (String) m.invoke(systemPropertiesClass, "qemu.hw.mainkeys");
            if ("1".equals(navBarOverride)) {
                hasNavigationBar = false;
            } else if ("0".equals(navBarOverride)) {
                hasNavigationBar = true;
            }
        } catch (Exception e) {

        }

        return hasNavigationBar;
    }

    /*
    private ContentObserver mNavigationStatusObserver = new ContentObserver(null) {
        @Override
        public void onChange(boolean selfChange) {
            int navigationBarIsMin = Settings.System.getInt(getGameContext().getContentResolver(),
                    "navigationbar_is_min", 0);
            if (navigationBarIsMin == 1) {
                //导航键隐藏了
            } else {
                //导航键显示了
            }

        }
    };
    */

	/**
	 * 判断activity是否处于可用状态
	 * @param context
	 * @return
	 */
	private boolean isFinishing(Activity activity)
	{
		if(activity==null||activity.isFinishing())
			return true;
		return false;
	}

	public void showMessage(String pMessage)
	{
		Activity context= (Activity) (mCtx);
		if(isFinishing(context))return;
		Toast mToast = Toast.makeText(context, pMessage, Toast.LENGTH_LONG);
		mToast.show();
	}

	public void alertJS(String title, String msg,final int p_nCallbackType)
	{
		//TODO 不知道为什么，插件情况下传入ctx就行，但是直接执行必须用Activity对象。
		Activity context= (Activity) (mCtx);
		if(isFinishing(context))return;
		AlertDialog.Builder pBuilder = new AlertDialog.Builder(context);
		pBuilder.setTitle(title);
		pBuilder.setMessage(msg);
		pBuilder.setCancelable(false);
		// 退出按钮
		pBuilder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface p_pDialog, int arg1) {
				p_pDialog.cancel();
				if (p_nCallbackType == 1) {
					ConchJNI.alertCallback();
				} else if (p_nCallbackType == 2) {
					game_plugin_exitGame();
				}
			}
		});
		AlertDialog alertdlg = pBuilder.create();
		alertdlg.setCanceledOnTouchOutside(false);
		alertdlg.show();
	}

	int m_iScreenWidth = 0;
	int m_iScreenHeight = 0;

	// ------------------------------------------------------------------------------
	public int GetScreenWidth() {
		// if (m_iScreenWidth != 0)
		// return m_iScreenWidth;
		WindowManager wm = (WindowManager) (mCtx
				.getSystemService(Context.WINDOW_SERVICE));
		DisplayMetrics pDm = new DisplayMetrics();
		wm.getDefaultDisplay().getMetrics(pDm);
		m_iScreenWidth = pDm.widthPixels;
		return m_iScreenWidth;
	}

	// ------------------------------------------------------------------------------
	public int GetScreenHeight() {
		// if (m_iScreenHeight != 0)
		// return m_iScreenHeight;
		WindowManager wm = (WindowManager) (mCtx
				.getSystemService(Context.WINDOW_SERVICE));
		DisplayMetrics pDm = new DisplayMetrics();
		wm.getDefaultDisplay().getMetrics(pDm);
		m_iScreenHeight = pDm.heightPixels;
		return m_iScreenHeight;
	}

	// ------------------------------------------------------------------------------

	public void game_conch3_setAssetInfo(AssetManager am) {
		m_AM = am;
	}

	public void game_conch3_SetIsPlug(boolean b) {
		m_bIsPlug = b;
	}

	public void game_conch3_setAppWorkPath(String runpath) {// 在这里保存运行数据。
		m_strCachePath = runpath;
	}

	public AbsoluteLayout getAbsLayout() {
		return m_pAbsLayout;
	}

	public DevID getDevID() {
		return m_pDevID;
	}

	public ConchCanvas getCanvas() {
		return m_pCavans;
	}

	public boolean getIsPlug() {
		return m_bIsPlug;
	}

	public LayaEditBox getEditBox() {
		return m_pEditBox;
	}

	public LayaWebView getWebView() {
		return m_pLayaWebView;
	}

	public Context getGameContext() {
		return mCtx;
	}

	public boolean getHorizontalScreen() {
		return m_bHorizontalScreen;
	}

	public void onPause() 
	{
		for (LayaVideoPlayer video :  m_videoPlayers) {
			video.onPause();
		}
		if(mBIsSensor)unRegisterSensor();
		if(m_pCavans!=null)
		{
			m_pCavans.onPause();
		}
		else
		{
			Log.e("Canvas", ">>>>>onPause m_pCavans is null");
		}
		ConchJNI.OnAppPause();
	}

	public NetworkReceiver getNetworkReceiver() {
		return m_pNetWorkReveiver;
	}

	// ------------------------------------------------------------------------------
	public void onResume() 
	{
		for (LayaVideoPlayer video :  m_videoPlayers) {
			video.onResume();
		}
		if(mBIsSensor)registerSensor();
		if(m_pCavans!=null)
		{
			m_pCavans.onResume();
		}
		else
		{
			Log.e("Canvas", ">>>>>onResume m_pCavans is null");
		}
		ConchJNI.OnAppResume();
	}
	public void onStop() {
	}

	public void onDestroy() {

		ProcessInfo.uninit();
		try{
			mCtx.unregisterReceiver(m_pNetWorkReveiver);
		} 
		catch (Exception e){
			Log.i("0","unregisterReceiver error m_pNetWorkReveiver=" + m_pNetWorkReveiver );
		}
		delInstance();
		ConchJNI.ReleaseDLib();
		if(m_pCavans!=null){
			m_pCavans.onPause();
		}
		else{
			Log.e("Canvas", ">>>>>onDestroy m_pCavans is null");
		}

		LayaAudioMusic.uninit();
		destory();
	}
	@SuppressLint("NewApi") @TargetApi(Build.VERSION_CODES.HONEYCOMB)
	private void destory()
	{
		if(m_pAbsLayout!=null)
			m_pAbsLayout.removeAllViews();
		m_pAbsLayout.setOnKeyListener(null);
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB){
			m_pAbsLayout.setOnGenericMotionListener(null);
		}
		m_pAbsLayout=null;
		if(m_pCavans!=null) {
			m_pCavans.destory();
			m_pCavans = null;
		}
		ms_layaConche = null;
		m_marketBundle=null;

		if(m_pLayaWebView!=null)
		{
			m_pLayaWebView.destory();
			m_pLayaWebView=null;
		}
		if(m_pEditBoxLayout!=null)
		{
			m_pEditBoxLayout.removeAllViews();
		    m_pEditBoxLayout = null;
		}
		if(m_pEditBox!=null)
		{
			m_pEditBox.destory();
			m_pEditBox=null;
		}
		mCtx=null;
		m_layaEventListener=null;
	}

	public void delInstance()
	{
		ExportJavaFunction.DelInstance();
		config.DelInstance();
		if(m_layaEventListener!=null) {
			m_layaEventListener.destory();
		}
	}



	@Override
	public void onNewIntent(Intent intent) {
	}

	@Override
	public void onRestart() {
		for (LayaVideoPlayer video :  m_videoPlayers) {
			video.onRestart();
		}
	}

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent intent) {
	}


	public void game_plugin_onUrlBack() {
		ConchJNI.onRunCmd(0x400 + 3333 + 104, -1, 0);
	}

	public void game_plugin_onUrlRefresh() {
		ConchJNI.onRunCmd(0x400 + 3333 + 102, 0, 0);
	}

	public int game_plugin_getTouchMovRange() {
		return 10;
	}

	public void setLocalizable(boolean b)
	{
		localizable=b;
		//ConchJNI.onSendToDesktop("sfsdfsadfsadf");
	}

	public void setResolution(int w, int h){
		Log.i("fix", "setResolution "+w+","+h);
		if(m_pCavans!=null){
			m_pCavans.setFixedSize(w,h);
		}else{
			canvas_res_w = w;
			canvas_res_h = h;
		}
	}

	public void _enableOnLayout(boolean b){
		m_bEnableOnLayout=b;
	}

	public void setGameUrl(String _param) {
		m_strUrl = _param;
	}
	
	public void setParamExt(String _param){
		m_strExt = _param;
	}
	
	public void setRuntimeExt()
	{
		if (m_strExt.length() > 0) {
			ConchJNI.configSetParamExt(m_strExt);
		}
	}
	
	public void setSoFile(String _param) {
		m_strSoFile = _param;
	}

	public void setSoPath(String _param) {
		m_strSoPath = _param;
	}

	public void setJarFile(String _param) {
		m_strJarFile = _param;
	}

	public void setAppCacheDir(String _param) {
		m_strCachePath = _param;
	}

	public void setDownloadThreadNum(int nNum){ m_nDownloadThreadNum = nNum;}

	public void setExpansionZipDir( final String mainPath,final String patchPath )
	{
		m_strExpansionMainPath = mainPath;
		m_strExpansionPatchPath = patchPath;
	}

	// 获得so文件路径
	public String getSoPath() {
		return m_strSoPath;
	}

	// 获得jar文件
	public String getJarFile() {
		return m_strJarFile;
	}

	// 获得游戏缓存目录
	public String getAppCacheDir() {
		return m_strCachePath;
	}

	public int getDownloadThreadNum()
	{
		return m_nDownloadThreadNum;
	}

	// 游戏主动退出游戏
	public void game_plugin_exitGame() {
		if (m_layaEventListener != null)
			m_layaEventListener.ExitGame();
	}

	// 游戏插件初始化
	public void game_conch3_init() {
		this.onCreate();
	}

	// 获得游戏需要显示的view
	public View game_conch3_get_view() {
		return m_pAbsLayout;
	}

	// 进入后台时调用
	public void game_conch3_onPause() {
		this.onPause();
	}

	// 恢复前台时调用
	public void game_conch3_onResume() {
		this.onResume();
	}

	// 退出游戏时调用
	public void game_conch3_onStop() {
		this.onStop();
	}

	@Override
	public void setLayaEventListener(ILayaEventListener _lis) {
		m_layaEventListener = _lis;

	}

	@Override
	public void setInterceptKey(boolean _intercept) {
		m_interceptKey = _intercept;
		setGameFocus();
	}

	public boolean getInterceptKey() {
		return m_interceptKey;
	}

	public void setGameFocus() {
		if (m_interceptKey) {
			Log.i("layaConch3", "m_interceptKey OK!");
			if (m_pAbsLayout == null) {
				return;
			}
			m_pAbsLayout.setFocusable(true);
			m_pAbsLayout.setFocusableInTouchMode(true);
			m_pAbsLayout.requestFocus();
			m_pAbsLayout.setOnKeyListener(this);
		}
	}

	static public Bundle getMarketBundle(){
		if(m_marketBundle == null){
			m_marketBundle = new Bundle();
			return m_marketBundle;
		}
		return m_marketBundle;
	}
	static public void setMarketBundle( Bundle bundle){
		m_marketBundle = bundle;
	}

    @Override
    public void setAssetInfo(AssetManager am) {
        game_conch3_setAssetInfo(am);        
    }

    @Override
    public void setIsPlugin(boolean isPlugin) {
        game_conch3_SetIsPlug(isPlugin);
    }

	public static String getLocalVersion(Context ctx) {
		int localVersion = 0;
		try {
			PackageInfo packageInfo = ctx.getApplicationContext()
					.getPackageManager()
					.getPackageInfo(ctx.getPackageName(), 0);
			localVersion = packageInfo.versionCode;
		} catch (PackageManager.NameNotFoundException e) {
			e.printStackTrace();
		}
		return Integer.toString(localVersion);
	}
	public static String getLocalVersionName(Context ctx) {
		String localVersion = "";
		try {
			PackageInfo packageInfo = ctx.getApplicationContext()
					.getPackageManager()
					.getPackageInfo(ctx.getPackageName(), 0);
			localVersion = packageInfo.versionName;
		} catch (PackageManager.NameNotFoundException e) {
			e.printStackTrace();
		}
		return localVersion;
	}
	@Override
	public void setAlertTitle(String title){
		m_strAlertTitle = title;
	}
	@Override
	public void  setStringOnBackPressed(String str) {
		m_strOnBackPressed = str;
	}
}
