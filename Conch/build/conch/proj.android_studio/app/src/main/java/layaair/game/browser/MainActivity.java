package layaair.game.browser;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.res.Configuration;
import android.net.Uri;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.Window;
import android.view.WindowManager;
import com.google.zxing.integration.android.IntentIntegrator;
import com.google.zxing.integration.android.IntentResult;
import layaair.game.conch.ILayaEventListener;
import layaair.game.conch.LayaConch5;
import layaair.game.floatmenu.FloatPanel;
import layaair.game.wrapper.ILayaLibWrapper;
import layaair.game.wrapper.LayaWrapperFactroy;
import com.layabox.conch6.R;

@SuppressLint("Wakelock")
public class MainActivity extends AppCompatActivity {
	//------------------------------------------------------------------------------
	private long 			m_nBackPressTime=0;
	public ILayaLibWrapper		mLayaEngine = null;
	public static SplashDialog mSplashDialog;
	public static FloatPanel m_FloatPanel = null;;
	// ------------------------------------------------------------------------------
	protected void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		
		JSBridge.mMainActivity = this;
        mSplashDialog = new SplashDialog(this);
        mSplashDialog.showSplash();

		m_FloatPanel = new FloatPanel(this);

		setFullScreen(true);

		Bundle bundle = new Bundle();
		bundle.putString(LayaConch5.MARKET_MARKETNAME, "LayaMarket");
		LayaConch5.setMarketBundle(bundle);

		mLayaEngine = LayaWrapperFactroy.createLayaWrapper();
		mLayaEngine.initEngine(this);
		mLayaEngine.setAlertTitle(this.getString( R.string.alert_dialog_title ));
		mLayaEngine.setStringOnBackPressed(this.getString( R.string.on_back_pressed ));

		layaGameListener listener=new layaGameListener();
		listener.activity=this;
		mLayaEngine.setLayaEventListener(listener);
		mLayaEngine.setInterceptKey(true);
		Intent intent = getIntent();
		String scheme = intent.getScheme();
		Uri uri = intent.getData();
		Log.e("2jni", "scheme:" + scheme);
		mLayaEngine.setLocalizable(false);
		if (uri != null) {
			String host = uri.getHost();
			String dataString = intent.getDataString();
			String id = uri.getQueryParameter("d");
			String path = uri.getPath();
			String path1 = uri.getEncodedPath();
			String queryString = uri.getQuery();
			Log.e("2jni", "host:" + host);
			Log.e("2jni", "dataString:" + dataString);
			Log.e("2jni", "id:" + id);
			Log.e("2jni", "path:" + path);
			Log.e("2jni", "path1:" + path1);
			Log.e("2jni", "queryString:" + queryString);
			if (dataString.length() > 0) {
				String gameUrl = "http" + dataString.substring(7);
				Log.e("2jni", "setUrl from intent:" + gameUrl);
				mLayaEngine.setGameUrl(gameUrl);
			}
		}
		/*else
		{
			mLayaEngine.setGameUrl("http://10.10.30.29:8899/conch-main.html");
		}*/
//		HashMap<String, Object> _option = new HashMap<String, Object>();
//		_option.put("sopath", "/data/data/com.layabox.conch3/pluginslib/");
//		mLayaEngine.setOptions(_option);
//		mLayaEngine.setGameUrl("http://10.10.30.29:8888/starthtml/index4.html");
//		mLayaEngine.setGameUrl("http://192.168.0.106:8888/starthtml/default1.html");

		mLayaEngine.startGame();
		//删除全部推送消息
		//LayaNotifyManager.removeAllNotify();
	}

	@Override
	protected void onStop() {
		super.onStop();
		mLayaEngine.onStop();
	}

	// ---------------------------------------------------------------------------
	protected void onActivityResult(int requestCode, int resultCode, Intent intent) {
		IntentResult result = IntentIntegrator.parseActivityResult(requestCode, resultCode, intent);
		if(result != null) {
			if(result.getContents() == null) {
				Log.d("MainActivity", "Cancelled scan");
			} else {
				Log.d("MainActivity", "Scanned");
				JSBridge.onScanResult(result.getContents());
			}
		}
		super.onActivityResult(requestCode, resultCode, intent);
		((LayaWrapper)mLayaEngine).onActivityResult(requestCode, resultCode, intent);
	}

	@Override
	protected void onRestart() {
		super.onRestart();
		mLayaEngine.onRestart();
	}


	// ------------------------------------------------------------------------------
	public void setFullScreen(boolean p_bFull) {
		if (p_bFull) {
			requestWindowFeature(Window.FEATURE_NO_TITLE);
			getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
					WindowManager.LayoutParams.FLAG_FULLSCREEN);
		} else {
			requestWindowFeature(Window.FEATURE_CONTEXT_MENU);
			getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
					WindowManager.LayoutParams.FLAG_FULLSCREEN);
		}
	}
	// ------------------------------------------------------------------------------
	@Override
	protected void onPause() {
		super.onPause();
		mLayaEngine.onPause();
	}
    //------------------------------------------------------------------------------
    @Override
    protected void onResume() {
    	super.onResume();	
    	mLayaEngine.onResume();
    }

	@Override
    protected void onDestroy(){
    	super.onDestroy();
    	mLayaEngine.onDestroy();
		mLayaEngine=null;
		m_FloatPanel.destory();
		m_FloatPanel = null;
		mSplashDialog.dismiss();
		mSplashDialog = null;
		System.exit(0);
    }

	@Override
	protected void onNewIntent(Intent intent) {
		super.onNewIntent(intent);
		mLayaEngine.onNewIntent(intent);
	}
	// ------------------------------------------------------------------------------
	public boolean onCreateOptionsMenu(Menu menu) {
		// menu.add(0, 0, 0, "refresh");
		menu.add(0, 1, 1, "version");
		menu.add(Menu.NONE, 2, 2, "url");
		menu.add(Menu.NONE, 3, 3, "deviceInfo");
		return super.onCreateOptionsMenu(menu);
	}
	// ------------------------------------------------------------------------------
	public void MyAlert(String title, String msg) {
		Builder pBuilder = new Builder(this);
		pBuilder.setTitle(title);
		pBuilder.setMessage(msg);
		// 退出按钮
		pBuilder.setPositiveButton("OK", new OnClickListener() {
			public void onClick(DialogInterface p_pDialog, int arg1) {
				p_pDialog.cancel();
			}
		});
		AlertDialog alertdlg = pBuilder.create();
		alertdlg.setCanceledOnTouchOutside(false);
		alertdlg.show();
	}

	// ------------------------------------------------------------------------------
	public boolean onOptionsItemSelected(MenuItem menuItem) {
		super.onOptionsItemSelected(menuItem);
		switch (menuItem.getItemId()) {
		case 0:
			break;
		case 1:
			MyAlert("", LayaWrapper.getAppVersionName());
			break;
		case 2:
			MyAlert("", ExportJavaFunction.m_sHref);
			break;
		case 3: {
			// 测试用的
			int nW = LayaWrapper.GetScreenWidth();
			int nH = LayaWrapper.GetScreenHeight();
			int nDpi =1;
			float nInch = LayaWrapper.GetScreenInch();
			int nMoveRange = LayaWrapper.m_pEngine.game_plugin_getTouchMovRange();
			MyAlert("", "w=" + nW + ",h=" + nH + ",dpi=" + nDpi + ",inch="
					+ nInch + ",MovRange=" + nMoveRange);
		}
			break;
		}
		return true;
	}

	// ------------------------------------------------------------------------------
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
		if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE) {
			Log.i("0", "=========onConfigurationChanged ORIENTATION_LANDSCAPE");
			JSBridge.onOrientationChange();
		} else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT) {
			Log.i("0", "=========onConfigurationChanged ORIENTATION_PORTRAIT");
			JSBridge.onOrientationChange();
		}
	}

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        if (requestCode == PermisionUtils.REQUEST_EXTERNAL_STORAGEP_ERMISSION_CODE) {
            if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                mLayaEngine.checkApkUpdate();
            }
            else {
				AlertDialog.Builder dialog = new AlertDialog.Builder(this);
				dialog.setTitle(R.string.waring);
				dialog.setMessage(R.string.permission_waring);
				dialog.setPositiveButton(R.string.ok, new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {
						mLayaEngine.checkApkUpdate();
					}
				});
				dialog.show();
            }
        }
    }
	static class layaGameListener implements ILayaEventListener{
		public  Activity activity;
		@Override
		public void ExitGame() {
			Log.i("=======", "======exit");
			activity.finish();
			activity=null;
			//mLayaEngine.onDestroy();
			System.exit(0);
		}
		@Override
		public void destory() {
		}
	}

}
