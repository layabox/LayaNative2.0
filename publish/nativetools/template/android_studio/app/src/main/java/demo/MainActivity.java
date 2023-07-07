package demo;
import java.io.InputStream;
import layaair.game.IMarket.IPlugin;
import layaair.game.IMarket.IPluginRuntimeProxy;
import layaair.game.Market.GameEngine;
import layaair.game.config.config;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;


public class MainActivity extends Activity{
    public static final int AR_CHECK_UPDATE = 1;
    private IPlugin mPlugin = null;
    private IPluginRuntimeProxy mProxy = null;
    boolean isLoad=false;
    boolean isExit=false;
    public static SplashDialog mSplashDialog;
    @Override    
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
		if (!isTaskRoot()) {
            finish();
            return;
        }
        getWindow().requestFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        hideSystemUI();
        JSBridge.mMainActivity = this;
        mSplashDialog = new SplashDialog(this);
        mSplashDialog.showSplash();
        initEngine();
    }
    public void initEngine()
    {
        mProxy = new RuntimeProxy(this);
        mPlugin = new GameEngine(this);
        mPlugin.game_plugin_set_runtime_proxy(mProxy);
        mPlugin.game_plugin_set_option("localize","false");
        mPlugin.game_plugin_set_option("gameUrl", "http://nativetest.layabox.com/layaplayer2.0.1/index.js");
        mPlugin.game_plugin_init(3);
        View gameView = mPlugin.game_plugin_get_view();
        this.setContentView(gameView);
        isLoad=true;
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
    public void onActivityResult(int requestCode, int resultCode,Intent intent) {

    }
    protected void onPause()
    {
        super.onPause();
        if(isLoad)mPlugin.game_plugin_onPause();
    }
    //------------------------------------------------------------------------------
    protected void onResume()
    {
        super.onResume();
        if(isLoad)mPlugin.game_plugin_onResume();
        
    }
    
    protected void onDestroy()
    {
        super.onDestroy();
        if(isLoad)mPlugin.game_plugin_onDestory();
    }
    
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event)
    {
        return super.onKeyDown(keyCode, event);
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        hideSystemUI();
    }

    /**
     * 全屏模式
     */
    private void hideSystemUI() {
        // 延伸显示区域到刘海
        WindowManager.LayoutParams lp = getWindow().getAttributes();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            lp.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
        }
       //全面屏处理
        getWindow().getDecorView().setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_IMMERSIVE
                        // Set the content to appear under the system bars so that the
                        // content doesn't resize when the system bars hide and show.
                        | View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        // Hide the nav bar and status bar
                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_FULLSCREEN);

    }


}
