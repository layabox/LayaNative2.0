package layaair.game.floatmenu;

import com.layabox.conch6.R;
import android.content.Context;
import android.content.res.Configuration;
import android.graphics.PixelFormat;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.WindowManager;
import android.widget.FrameLayout;

import layaair.game.browser.ConchJNI;
import layaair.game.browser.LayaWrapper;

public class FloatPanel extends FrameLayout {
	private WindowManager mWindowManager;
	private WindowManager.LayoutParams mParams;
	private View mView;
	private Context mContext;
	private boolean mShow = false;
	public FloatPanel(Context context) {
		super(context);
		mContext = context;
		mWindowManager = (WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE);

		LayoutInflater layout = (LayoutInflater)context.getApplicationContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		mView = layout.inflate(R.layout.float_panel , null);

		View panel = mView.findViewById(R.id.float_panel);

		View refresh = panel.findViewById(R.id.refresh_btn);

		View back = panel.findViewById(R.id.back_btn);

		refresh.setOnClickListener(onClick);
		back.setOnClickListener(onClick);
	}
	public void show() {

		mParams = new WindowManager.LayoutParams();
		mParams.flags = WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;
		mParams.gravity = Gravity.LEFT | Gravity.TOP;
		mParams.format = PixelFormat.RGBA_8888;
		mParams.width = WindowManager.LayoutParams.WRAP_CONTENT;
		mParams.height = WindowManager.LayoutParams.WRAP_CONTENT;

		int widthMeasureSpec = View.MeasureSpec.makeMeasureSpec((1<<30)-1, MeasureSpec.AT_MOST);
		int heightMeasureSpec = View.MeasureSpec.makeMeasureSpec((1<<30)-1, MeasureSpec.AT_MOST);
		mView.measure(widthMeasureSpec, heightMeasureSpec);

		int screenWidth = mWindowManager.getDefaultDisplay().getWidth();
		int screenHeight = mWindowManager.getDefaultDisplay().getHeight();
		if (mContext.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE) {
			mParams.x = screenWidth - mView.getMeasuredWidth() - 30;
			mParams.y = 30;
		}
		if (mContext.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT) {
			mParams.x = screenWidth - mView.getMeasuredWidth() - 30;
			mParams.y = 30;
		}

        try {
			mWindowManager.addView(mView, mParams);
			mShow = true;
        } catch (Exception e) {
            Log.d("", ">>>>>>>>>>>>>" + e.toString());
        }
		updatePosition();
	}
	public void hide() {
		if (mShow) {
			try {
				mWindowManager.removeView( mView );
				mShow = false;
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
	public void destory() {
		if (mShow) {
			mWindowManager.removeViewImmediate( mView );
		}
	}
	private static OnClickListener onClick = new OnClickListener() {
		@Override
		public void onClick(View v) {
			switch (v.getId()) {
				case R.id.back_btn:
					String js = "window.conchConfig.JSDebugMode = 1;";
					ConchJNI.RunJS(js);
					LayaWrapper.onPopMenu(LayaWrapper.BACK_TO_MAIN);
					break;
				case R.id.refresh_btn:
					LayaWrapper.onPopMenu( LayaWrapper.REFRESH);
					break;
				default:
					break;
			}
		}
	};
	public void updatePosition() {
		if (mShow) {
			int widthMeasureSpec = View.MeasureSpec.makeMeasureSpec( (1 << 30) - 1, MeasureSpec.AT_MOST );
			int heightMeasureSpec = View.MeasureSpec.makeMeasureSpec( (1 << 30) - 1, MeasureSpec.AT_MOST );
			mView.measure( widthMeasureSpec, heightMeasureSpec );
			int screenWidth = mWindowManager.getDefaultDisplay().getWidth();
			int screenHeight = mWindowManager.getDefaultDisplay().getHeight();
			if (mContext.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE) {
				mParams.x = screenWidth - mView.getMeasuredWidth() - 30;
				mParams.y = 30;
			}
			if (mContext.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT) {
				mParams.x = screenWidth - mView.getMeasuredWidth() - 30;
				mParams.y = 30;
			}
			mWindowManager.updateViewLayout( mView, mParams );
		}
	}
}
