package layaair.game.IMarket;

import android.content.res.Configuration;
import android.os.Bundle;
import android.view.View;

public abstract interface IPlugin {	
	//	获取游戏引擎端的变量
	public abstract Object game_plugin_get_value(String key);
	//	获取要显示的view
	public abstract View game_plugin_get_view();
	//	游戏引擎初始化
	public abstract void game_plugin_init(int nDownloadThreadNum);
	//	拦截keycode
	public abstract boolean game_plugin_intercept_key(int keycode);
	//	调用游戏引擎的方法
	public abstract Object game_plugin_invoke_method(String method, Bundle param);
	//	进入后台时调用
	public abstract void game_plugin_onPause();
	//	恢复前台时调用
	public abstract void game_plugin_onResume();
	//	退出游戏时调用
	public abstract void game_plugin_onStop();
	//	退出游戏时 销毁
	public abstract void game_plugin_onDestory();
	//	向游戏引擎传递参数
	public abstract void game_plugin_set_option(String key, String value);
    //  设置代理对象
    public abstract void game_plugin_set_runtime_proxy(IPluginRuntimeProxy paramIGameEngineRuntimeProxy);
	//	游戏页面发生改变时触发(分辨率，横竖屏...)
	public abstract void game_plugin_configonChanged(Configuration newConfig);
}
