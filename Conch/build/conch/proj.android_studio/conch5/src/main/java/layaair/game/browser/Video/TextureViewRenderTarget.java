package layaair.game.browser.Video;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Matrix;
import android.graphics.SurfaceTexture;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Surface;
import android.view.TextureView;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.AbsoluteLayout;

import java.util.ArrayList;

import static layaair.game.browser.LayaVideoPlayer.TAG;

public class TextureViewRenderTarget implements IVideoRenderTarget, TextureView.SurfaceTextureListener {
    private ArrayList<Callback> m_callbacks = new ArrayList<Callback>();

    private TextureView m_textureView;
    private SurfaceTexture m_SurfaceTexture = null;
    private Surface m_surface;

    private Context m_ctx;
    private AbsoluteLayout m_parentLayout = null;

    private  boolean m_isFrameAvailable;
    private Bitmap m_bitmap;

    private int m_width;
    private int m_height;

    public TextureViewRenderTarget(Context context, AbsoluteLayout parentLayout)
    {
        m_ctx = context;
        m_parentLayout = parentLayout;

        m_isFrameAvailable = false;

        WindowManager wm = (WindowManager)m_ctx.getSystemService(Context.WINDOW_SERVICE);
        DisplayMetrics dm = new DisplayMetrics();
        wm.getDefaultDisplay().getMetrics(dm);
        m_width = dm.widthPixels;
        m_height = dm.heightPixels;

//        Log.i(TAG, "TextureViewRenderTarget: (" + m_width + ", " + m_height + ")");

        m_bitmap = Bitmap.createBitmap(dm.widthPixels, dm.heightPixels, Bitmap.Config.ARGB_8888);

        final TextureViewRenderTarget thisObj = this;

        ((Activity)m_ctx).runOnUiThread(
            new Runnable() {
                @Override
                public void run() {
                    m_textureView = new TextureView(m_ctx);
                    m_textureView.setSurfaceTextureListener(thisObj);

                    m_parentLayout.addView(m_textureView, new ViewGroup.LayoutParams(
                            ViewGroup.LayoutParams.WRAP_CONTENT,
                            ViewGroup.LayoutParams.WRAP_CONTENT));

                    m_textureView.setLayoutParams(new AbsoluteLayout.LayoutParams(thisObj.m_width, thisObj.m_height, 0, 0 ));
                    //m_textureView.setZ(Long.MIN_VALUE + 1);

                    Matrix m = new Matrix();
                    m.postTranslate(thisObj.m_width, thisObj.m_height);
                    m_textureView.setTransform(m);
                }
            }
        );
    }

    @Override
    public void addCallback(Callback callback)
    {
        m_callbacks.add(callback);
    }

    @Override
    public boolean isFrameAvailable()
    {
        return m_isFrameAvailable;
    }

    @Override
    public void setX(int val) {
    }


    @Override
    public void setY(int val) {

    }

    @Override
    public void setWidth(int val) {
        if(m_width != val)
        {
            m_bitmap = Bitmap.createBitmap(m_width, m_height, Bitmap.Config.ARGB_8888);
            m_width = val;
        }

    }

    @Override
    public void setHeight(int val) {
        if(m_height != val)
        {
            m_bitmap = Bitmap.createBitmap(m_width, m_height, Bitmap.Config.ARGB_8888);
            m_height = val;
        }
    }

    @Override
    public void dispose() {
        Log.i(TAG, "[Debug][Video] dispose");
        ((Activity)m_ctx).runOnUiThread(
                new Runnable() {
                    @Override
                    public void run() {
                        m_parentLayout.removeView(m_textureView);
                        m_textureView = null;
                        m_bitmap = null;
                        m_surface = null;
                    }
                }
        );
    }

    @Override
    public Surface getSurface() {
        return m_surface;
    }

    @Override
    public Bitmap getBitmap() {
        m_isFrameAvailable = false;
        m_textureView.getBitmap(m_bitmap);
        return m_bitmap;
    }

    @Override
    public void onSurfaceTextureAvailable(SurfaceTexture surfaceTexture, int i, int i1) {
        m_SurfaceTexture = surfaceTexture;
        m_surface = new Surface(surfaceTexture);
 //       m_SurfaceTexture.setOnFrameAvailableListener(this);

        m_isFrameAvailable = true;
        for(int ii = 0;  ii < m_callbacks.size(); ii++)
        {
            m_callbacks.get(ii).onRenderTargetCreated();
        }
    }

    @Override
    public void onSurfaceTextureSizeChanged(SurfaceTexture surfaceTexture, int i, int i1) {
    }

    @Override
    public boolean onSurfaceTextureDestroyed(SurfaceTexture surfaceTexture) {
        return false;
    }

    @Override
    public void onSurfaceTextureUpdated(SurfaceTexture surfaceTexture) {
        this.m_isFrameAvailable = true;
    }

}
