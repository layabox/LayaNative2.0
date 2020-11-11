package layaair.game.browser;

import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.view.KeyEvent;
import android.view.View;
import android.widget.TextView;
import com.layabox.conch6.R;


public class SplashDialog extends Dialog {
    private Context mContext;
    private long mStartTime;
    private long mleastShowTime = 2;
    private TextView mTipsView;
    private String[] mTips = {};
    private int mFontColor;
    private int mIndex = 0;
    private int mPercent = 0;
    private View mLayout;
    Handler mSplashHandler = new Handler(Looper.getMainLooper()) {
        @Override
        public void handleMessage(Message message) {
            super.handleMessage(message);
            switch(message.what) {
                case 0:
                    int length = mTips.length;
                    mSplashHandler.removeMessages(0);
                    if (length > 0) {
                        if (mIndex >= length) {
                            mIndex = 0;
                        }
                        mTipsView.setText(mTips[mIndex] + "(" + mPercent + "%)");
                        mIndex++;
                    }
                    mSplashHandler.sendEmptyMessageDelayed(0, 1000);
                    break;
                case 1:
                    mSplashHandler.removeMessages(0);
                    mSplashHandler.removeMessages(1);
                    SplashDialog.this.dismiss();
                    break;
                default:
                    break;
            }
        }
    };

    public SplashDialog(Context context) {
        super(context, R.style.Splash);
        mContext = context;
    }
    public void setTips(String[] tips) {
        mTips = tips;
    }
    public void setPercent(int percent) {
        mPercent = percent;
        if (mPercent > 100) {
            mPercent = 100;
        }
        if (mPercent < 0) {
            mPercent = 0;
        }
        int length = mTips.length;
        if (length > 0) {
            if (mIndex >= mTips.length) {
                mIndex = 0;
            }
            mTipsView.setText(mTips[mIndex] + "(" + mPercent + "%)");
        }
		if (mPercent == 100) {
            dismissSplash();
        }
    }
    public void setFontColor(int color) {
        mTipsView.setTextColor(color);
    }
    public void setBackgroundColor(int color) {
        mLayout.setBackgroundColor(color);
    }
    public void showTextInfo(boolean show) {
        if (show) {
            mTipsView.setVisibility(View.VISIBLE);
        }
        else {
            mTipsView.setVisibility(View.INVISIBLE);
        }
    }
    public void showSplash() {
        this.show();
        mStartTime = System.currentTimeMillis();
        mSplashHandler.sendEmptyMessage(0);
    }
    public void dismissSplash() {
        long showTime = System.currentTimeMillis() - mStartTime;
        if (showTime >= mleastShowTime * 1000) {
            mSplashHandler.sendEmptyMessage(1);
        }
        else {
            mSplashHandler.sendEmptyMessageDelayed(1, (long) (this.mleastShowTime * 1000 - showTime));
        }
    }
    protected void onCreate(Bundle bundle) {
        super.onCreate(bundle);
        setContentView(R.layout.splash_dialog);
        mTipsView = (TextView)findViewById(R.id.tipsView);
        mLayout = findViewById(R.id.layout);
    }
    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        if(event.getKeyCode() == KeyEvent.KEYCODE_BACK){
            return true;
        }else {
            return super.dispatchKeyEvent(event);
        }
    }
}
