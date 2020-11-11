package layaair.game.wrapper;

import android.app.Activity;
import android.content.Intent;
import android.view.View;

import java.util.HashMap;

import layaair.game.conch.ILayaEventListener;


 public abstract interface ILayaLibWrapper {
	 void initEngine(Activity context);
	 void setLayaEventListener(ILayaEventListener listener);
	 void setLoadingView(View view);
	 void setGameUrl(String url);
	 void startGame();
	 void setInterceptKey(boolean pInterceptKey);
	 void setOptions(HashMap<String, Object> _option);
	 void setLocalizable(boolean b);
	 void checkApkUpdate();
	 void onPause();
	 void onResume();
	 void onDestroy();
	 //	游戏停止
	 void onStop();
	 void onNewIntent(Intent intent);
	 void onRestart();
	 void onActivityResult(int requestCode, int resultCode,Intent intent);
	 void setResolution(int w, int h);
	 void _enableOnLayout(boolean b);
	 void setAlertTitle(String title);
	 void setStringOnBackPressed(String str);
}
