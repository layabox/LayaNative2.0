package layaair.game.Market;

import layaair.game.IMarket.IPlugin;
import layaair.game.IMarket.IPluginRuntimeProxy;
import layaair.game.conch.ILayaEventListener;
import layaair.game.conch.ILayaGameEgine;
import layaair.game.conch.LayaConch5;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import com.layabox.conch6.R;

public class GameEngine implements IPlugin {
	private static final String TAG = "LayaGameEngine";
	public ILayaGameEgine mLayaGameEngine = null;
    private String mGameUrl = "";
    private Context mContext = null;
    private IPluginRuntimeProxy mGameEngineProxy = null;
	public static GameEngine _instance = null;
		
	public GameEngine(Context _ctx){
        mContext = _ctx;
		mLayaGameEngine = new LayaConch5(_ctx);
		_instance = this;
	}

    public static GameEngine getInstance(){
        return _instance;
    }

	@Override
	public Object game_plugin_get_value(String key) {
		return null;
	}
	
	@Override
	public View game_plugin_get_view() {
		Log.e(TAG,"game_plugin_get_view");		
		return mLayaGameEngine.getAbsLayout();
	}

	@Override
	public void game_plugin_init(int nDownloadThreadNum) {
	    Log.d(TAG,"game_plugin_init url ="+mGameUrl);
	    if( mGameUrl == null || mGameUrl.length() <2 ){
	        Log.e("","引擎初始化失败，没有游戏地址 gameUrl = "+mGameUrl);
	        return;
	    }
	    String gameUrl = mGameUrl;
	    mLayaGameEngine.setIsPlugin(false);
	    mLayaGameEngine.setGameUrl(gameUrl);
		//mLayaGameEngine.setResolution(600,200);
	    Log.d(TAG,"url="+gameUrl);
		String _path = (String) mGameEngineProxy.laya_get_value("CacheDirInSdcard");
		if(_path==null)
			_path = (String) mGameEngineProxy.laya_get_value("CacheDir");
		mLayaGameEngine.setAlertTitle(mContext.getString( R.string.alert_dialog_title ));
		mLayaGameEngine.setStringOnBackPressed(mContext.getString( R.string.on_back_pressed ));
		mLayaGameEngine.setDownloadThreadNum(nDownloadThreadNum);
		mLayaGameEngine.setAppCacheDir(_path);
		mLayaGameEngine.setExpansionZipDir( (String)mGameEngineProxy.laya_get_value("ExpansionMainPath"),(String)mGameEngineProxy.laya_get_value("ExpansionPatchPath"));
		AssetManager am = mContext.getAssets();
		mLayaGameEngine.setAssetInfo(am);
		layaGameListener listener=new layaGameListener();
		listener.activity=(Activity)mContext;
		mLayaGameEngine.setLayaEventListener(listener);
		mLayaGameEngine.setInterceptKey(true);
		mLayaGameEngine.onCreate();
		LayaConch5 tmp = (LayaConch5)mLayaGameEngine;
		Log.e(TAG,"game_plugin_init soPath="+tmp.getSoPath()+" jarfile="+tmp.getJarFile()+" appcache="+tmp.getAppCacheDir());
	}

	@Override
	public boolean game_plugin_intercept_key(int keycode) {
		return false;
	}

	@Override
	public Object game_plugin_invoke_method(String method, Bundle param) {
		return null;
	}

	@Override
	public void game_plugin_onPause() {
		mLayaGameEngine.onPause();
	}

	@Override
	public void game_plugin_onResume() {
		mLayaGameEngine.onResume();
	}

	@Override
	public void game_plugin_onStop() {
		mLayaGameEngine.onStop();
	}
	@Override
	public  void game_plugin_onDestory() {
		mLayaGameEngine.onDestroy();
		System.exit(0);
	}

	@Override
	public void game_plugin_set_option(String key, String value) {
		Log.e(TAG,"game_plugin_set_option key="+key+" value="+value);
		if( key.equalsIgnoreCase("gameUrl") )
		    mGameUrl = value;
		else if(key.equalsIgnoreCase("localize")){
			boolean l = value.equalsIgnoreCase("true");
			mLayaGameEngine.setLocalizable(l);
		}
	}

	@Override
	public void game_plugin_configonChanged(Configuration newConfig) {

	}

    @Override
    public void game_plugin_set_runtime_proxy(
            IPluginRuntimeProxy paramIGameEngineRuntimeProxy) {
        mGameEngineProxy = paramIGameEngineRuntimeProxy;
    }
    public IPluginRuntimeProxy getRuntimeProxy(){
        return mGameEngineProxy;
    }
	static class layaGameListener implements ILayaEventListener {
		public Activity activity;

		@Override
		public void ExitGame() {
			Log.i("=======", "======exit");
			activity.finish();
			activity = null;
			//mLayaEngine.onDestroy();
			System.exit(0);
		}
		@Override
		public void destory() {
		}
	}
}
