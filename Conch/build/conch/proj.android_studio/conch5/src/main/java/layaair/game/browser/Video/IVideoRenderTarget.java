package layaair.game.browser.Video;

import android.graphics.Bitmap;
import android.view.Surface;
import android.view.SurfaceHolder;

public interface IVideoRenderTarget {
    void addCallback(IVideoRenderTarget.Callback callback);
    void setX(int val);
    void setY(int val);
    void setWidth(int val);
    void setHeight(int val);

    void dispose();

    boolean isFrameAvailable();
    Bitmap getBitmap();

    Surface getSurface();

    interface Callback
    {
        void onRenderTargetCreated();
    }
}


