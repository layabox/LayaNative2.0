package layaair.game.browser.Video;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.ViewGroup;
import android.widget.AbsoluteLayout;

import java.util.ArrayList;

import static layaair.game.browser.LayaVideoPlayer.TAG;

public class SurfaceViewRenderTarget implements IVideoRenderTarget, SurfaceHolder.Callback {

    private SurfaceView m_surfaceView = null;
    private SurfaceHolder m_surfceHolder = null;

    private AbsoluteLayout m_mediaPlayerLayout = null;
    private AbsoluteLayout m_parentLayout = null;
    private Context m_ctx;

    private ArrayList<Callback> m_callbacks = new ArrayList<Callback>();

    public SurfaceViewRenderTarget(Context ctx, AbsoluteLayout parentLayout)
    {
        m_ctx = ctx;
        m_parentLayout = parentLayout;

        Create();
    }

    @Override
    public void dispose()
    {
        ((Activity)m_ctx).runOnUiThread(new Runnable() {
            @Override
            public void run() {
            m_mediaPlayerLayout.removeAllViews();
            m_parentLayout.removeView(m_mediaPlayerLayout);
            m_mediaPlayerLayout = null;
            m_parentLayout.requestLayout();
            Log.i(TAG, "[Debug][Video]SurfaceViewRenderTarget: dispose");
            }
        });
    }

    private void Create()
    {
        final SurfaceViewRenderTarget thisObj = this;

        ((Activity)m_ctx).runOnUiThread(
            new Runnable() {
                @Override
                public void run() {
                m_surfaceView = new SurfaceView(m_ctx);
                m_surfceHolder = m_surfaceView.getHolder();
                m_surfceHolder.addCallback(thisObj);

                m_mediaPlayerLayout = new AbsoluteLayout(m_ctx);
                m_mediaPlayerLayout.addView(m_surfaceView, new ViewGroup.LayoutParams(
                        ViewGroup.LayoutParams.WRAP_CONTENT,
                        ViewGroup.LayoutParams.WRAP_CONTENT));

                m_parentLayout.addView(m_mediaPlayerLayout, new ViewGroup.LayoutParams(
                        ViewGroup.LayoutParams.WRAP_CONTENT,
                        ViewGroup.LayoutParams.WRAP_CONTENT));

                m_surfaceView.setZOrderMediaOverlay(true);
                m_surfaceView.setLayoutParams(new AbsoluteLayout.LayoutParams(800, 480, 0, 0));
                m_parentLayout.requestLayout();
                }
            }
        );
    }

    @Override
    public void addCallback(Callback callback) {
        m_callbacks.add(callback);
    }

    @Override
    public boolean isFrameAvailable()
    {
        return false;
    }

    @Override
    public Bitmap getBitmap()
    {
        return null;
    }

    @Override
    public Surface getSurface()
    {
        return m_surfceHolder.getSurface();
    }

    @Override
    public void setX(int val) {
        final int newVal = val;
        ((Activity)m_ctx).runOnUiThread(
            new Runnable() {
                @Override
                public void run() {
                Log.i(TAG, "[Debug][Video]run: setX : " + newVal);
                AbsoluteLayout.LayoutParams layoutParams = (AbsoluteLayout.LayoutParams) m_surfaceView.getLayoutParams();
                layoutParams.x = newVal;
                m_surfaceView.setLayoutParams(layoutParams);
                m_parentLayout.requestLayout();
                }
            }
        );
    }

    @Override
    public void setY(int val) {
        final int newVal = val;
        ((Activity)m_ctx).runOnUiThread(
            new Runnable() {
                @Override
                public void run() {
                Log.i(TAG, "[Debug][Video]run: setY : " + newVal);
                AbsoluteLayout.LayoutParams layoutParams = (AbsoluteLayout.LayoutParams) m_surfaceView.getLayoutParams();
                layoutParams.y = newVal;
                m_surfaceView.setLayoutParams(layoutParams);
                m_parentLayout.requestLayout();
                }
            }
        );
    }

    @Override
    public void setWidth(int val) {
        final  int newVal = val;
        ((Activity)m_ctx).runOnUiThread(
            new Runnable() {
                @Override
                public void run() {
                Log.i(TAG, "[Debug][Video]setWidth : " + newVal);
                AbsoluteLayout.LayoutParams layoutParams = (AbsoluteLayout.LayoutParams) m_surfaceView.getLayoutParams();
                layoutParams.width = newVal;
                m_surfaceView.setLayoutParams(layoutParams);
                m_parentLayout.requestLayout();
                }
            }
        );
    }

    @Override
    public void setHeight(int val) {
        final  int newVal = val;
        ((Activity)m_ctx).runOnUiThread(
            new Runnable() {
                @Override
                public void run() {
                Log.i(TAG, "[Debug][Video]setHeight : " + newVal);
                AbsoluteLayout.LayoutParams layoutParams = (AbsoluteLayout.LayoutParams) m_surfaceView.getLayoutParams();
                layoutParams.height = newVal;
                m_surfaceView.setLayoutParams(layoutParams);
                m_parentLayout.requestLayout();
                }
            }
        );
    }

    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {
        for(int i = 0; i < m_callbacks.size(); i++)
        {
            m_callbacks.get(i).onRenderTargetCreated();
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {

    }
}
