package layaair.game.browser;

import android.content.Context;
import android.graphics.Bitmap;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.widget.AbsoluteLayout;

import java.io.IOException;
import java.util.ArrayList;

import layaair.game.BuildConfig;
import layaair.game.browser.Video.IVideoRenderTarget;
import layaair.game.browser.Video.SurfaceViewRenderTarget;
import layaair.game.browser.Video.TextureViewRenderTarget;
import layaair.game.browser.Video.VideoPlayerState;
import layaair.game.conch.LayaConch5;

import static android.content.Context.AUDIO_SERVICE;
import static android.media.MediaPlayer.SEEK_CLOSEST;


public class LayaVideoPlayer implements IVideoRenderTarget.Callback{
    public final static String TAG = "LayaVideo";

    enum ReadyState {
        HAVE_NOTHING(0),
        HAVE_METADATA(1),
        HAVE_CURRENT_DATA(2),
        HAVE_FUTURE_DATA(3),
        HAVE_ENOUGH_DATA(4);

        private int m_value;

        ReadyState(int val)
        {
            m_value = val;
        }

        public int getValue()
        {
            return m_value;
        }
    };

    enum MediaPlayerStatus
    {
        NotInit,
        Idle,
        Initalized,
        Preparing,
        Prepared,
        Started,
        Stopped,
        Paused,
        OnPaused,
    };

    enum PendingType
    {
        Null,
        Load,
        Play,
        Pause,
        Stop,
    }

    class PendingData
    {
        public PendingType type = PendingType.Null;
        public String additionData = null;

        public PendingData()
        {
            this(PendingType.Null, null);
        }

        public PendingData(PendingType type)
        {
           this(type, null);
        }

        public PendingData(PendingType type, String additionData)
        {
            this.type = type;
            this.additionData = additionData;
        }
    }

    private IVideoRenderTarget m_renderTarget;
    private MediaPlayer m_mediaPlayer = null;

    private ReadyState m_readyState = ReadyState.HAVE_NOTHING;

    private AbsoluteLayout m_parentLayout;

    private Context m_ctx;

    private MediaPlayerStatus m_curStatus = MediaPlayerStatus.NotInit;


    private boolean m_isSurfaceCreated = false;
    private boolean m_isEmitEnable = true;
    private ArrayList<PendingData> m_pendingDataAr = new ArrayList<PendingData>();

    private long m_appVideoPtr = 0;

    private static Context ms_ctx = null;
    private static AbsoluteLayout ms_mediaPlayerLayout = null;

    private String m_url = null;
    private int m_currentTime = 0;
    // volume
    private float m_volume = 1.0f;

    VideoPlayerState m_playerState;


    private native void emit(long appVideoPtr, String evtName);
    private native void transferBitmap(Bitmap bitmap, long dataPtr);

    public static void SetDefaultVideoContext(Context context, AbsoluteLayout parentLayout)
    {
        ms_ctx = context;
        ms_mediaPlayerLayout = parentLayout;
    }

    public LayaVideoPlayer(long appVideoPtr)
    {
        m_ctx= ms_ctx;
        m_parentLayout = ms_mediaPlayerLayout;
        m_appVideoPtr = appVideoPtr;
        Init();
        LayaConch5.GetInstance().addVideoPlayer( this );
    }

    public LayaVideoPlayer(Context context, AbsoluteLayout parentLayout)
    {
        m_ctx = context;
        m_parentLayout = parentLayout;
        Init();
        LayaConch5.GetInstance().addVideoPlayer( this );
    }

    public void Dispose()
    {
        m_isEmitEnable = false;
        if(m_mediaPlayer != null)
        {
            m_mediaPlayer.release();
            m_mediaPlayer = null;
        }
        m_renderTarget.dispose();

        LayaConch5.GetInstance().removeVideoPlayer( this );
    }

    private void Init()
    {
        if(m_ctx == null)
        {
            m_ctx = ms_ctx;
            m_parentLayout = ms_mediaPlayerLayout;
        }

        m_playerState = new VideoPlayerState(m_ctx, this);

        if(Build.VERSION.SDK_INT <= Build.VERSION_CODES.KITKAT)
        {
            m_renderTarget = new SurfaceViewRenderTarget(m_ctx, m_parentLayout);
        }
        else
        {
            m_renderTarget = new TextureViewRenderTarget(m_ctx, m_parentLayout);
        }
        m_renderTarget.addCallback(this);
    }

    public  MediaPlayer getMediaPlayer()
    {
        return m_mediaPlayer;
    }

    public void Load(String url)
    {
        m_url = url;
        if(!m_isSurfaceCreated)
        {
            m_pendingDataAr.clear();
            m_pendingDataAr.add(new PendingData(PendingType.Load, m_url));
            return;
        }

        Log.i(TAG, "[Debug][Video] Load: Begin Load Video " + url);

        Stop();

        try {
            Uri videoUri = Uri.parse(m_url);

            m_readyState = ReadyState.HAVE_NOTHING;
            emit("loadstart");
            m_mediaPlayer.setDataSource(m_ctx, videoUri);
            m_curStatus = MediaPlayerStatus.Initalized;

            m_mediaPlayer.prepareAsync();
            m_curStatus = MediaPlayerStatus.Preparing;
            m_mediaPlayer.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
                @Override
                public void onPrepared(MediaPlayer mediaPlayer) {
                    m_curStatus = MediaPlayerStatus.Prepared;
                    m_readyState = ReadyState.HAVE_ENOUGH_DATA;
                    //setCurrentTime(m_currentTime);
                    emit("loadedmetadata");
                    emit("canplay");
                    Log.i(TAG, "[Debug][Video]Load: Video is Prepared.");
                    m_playerState.applyAutoplay();
                }
            });
        }
        catch (IOException e)
        {
            Log.e(TAG, "[Debug][Video]Load: Have errors on load video");
            emit("error");
            e.printStackTrace();
        }
    }

    public void Play()
    {
        if(!m_isSurfaceCreated)
        {
            m_pendingDataAr.add(new PendingData(PendingType.Play));
            return;
        }

        if(m_readyState != ReadyState.HAVE_ENOUGH_DATA || m_curStatus == MediaPlayerStatus.Initalized || m_curStatus == MediaPlayerStatus.Preparing)
        {
            return;
        }
        else
        {
            Log.i(TAG, "[Debug][Video]Play: Start to Play!");
            setCurrentTime(m_currentTime);
            m_mediaPlayer.start();
            m_curStatus = MediaPlayerStatus.Started;
        }
    }

    public void onPause()
    {
        if (m_curStatus == MediaPlayerStatus.Started) {
            m_currentTime = getCurrentTime();
            m_curStatus = MediaPlayerStatus.OnPaused;
            m_mediaPlayer.pause();
        }
    }

    public void onRestart() {
        if (!m_isSurfaceCreated && m_curStatus == MediaPlayerStatus.OnPaused) {
            m_pendingDataAr.clear();
            m_pendingDataAr.add( new PendingData( PendingType.Load, m_url ) );
            return;
        }
    }

    public void onResume() {
        Play();
    }

    public void Pause()
    {

        if(!m_isSurfaceCreated)
        {
            m_pendingDataAr.add(new PendingData(PendingType.Pause));
            return;
        }

        if(m_curStatus == MediaPlayerStatus.Started)
        {
            m_curStatus = MediaPlayerStatus.Paused;
            m_mediaPlayer.pause();

            Log.i(TAG, "[Debug][Video]Pause: Start to Pause");
        }
    }

    public void Stop()
    {
        if(!m_isSurfaceCreated)
        {
            m_pendingDataAr.add(new PendingData(PendingType.Stop));
            return;
        }

        if(m_curStatus == MediaPlayerStatus.Started ||
                m_curStatus == MediaPlayerStatus.Paused) {
           m_mediaPlayer.stop();
           m_curStatus = MediaPlayerStatus.Stopped;
        }
    }

    public boolean IsPaused()
    {
        if(!m_isSurfaceCreated)
        {
            Log.e(TAG, "[Debug][Video]IsPaused:  The surface is not created");
            return true;
        }

        Log.i(TAG, "[Debug][Video]IsPaused: " +  m_mediaPlayer.isPlaying());
        return !m_mediaPlayer.isPlaying();
    }

    public boolean IsLoop()
    {
        return m_playerState.isLoop;
    }

    public void SetLoop(boolean value)
    {
        m_playerState.isLoop = value;
        m_playerState.applyLoop();
    }

    public boolean IsAutoplay()
    {
        return m_playerState.isAutoplay;
    }

    public void SetAutoplay(boolean value)
    {
        Log.i(TAG, "[Debug][Video]SetAutoplay: SetAutoPlay");
        m_playerState.isAutoplay = value;
    }

    public void setX(int x)
    {
        m_renderTarget.setX(x);
    }

    public void setY(int y)
    {
        m_renderTarget.setY(y);
    }

    public void setWidth(int width)
    {
        m_renderTarget.setWidth(width);
    }

    public void setHeight(int height)
    {
        m_renderTarget.setHeight(height);
    }

    public int getVideoWidth()
    {
 //       Log.i(TAG, "[Debug][Video]getVideoWidth: " + m_mediaPlayer.getVideoWidth());
        return m_mediaPlayer.getVideoWidth();
    }

    public int getVideoHeight()
    {
 //       Log.i(TAG, "[Debug][Video]getVideoHeight: " + m_mediaPlayer.getVideoHeight());
        return m_mediaPlayer.getVideoHeight();
    }

    public int GetReadyState() { return m_readyState.getValue(); }

    public int getDuration()
    {
        return m_mediaPlayer.getDuration();
    }

    public int getCurrentTime()
    {
        return m_mediaPlayer.getCurrentPosition();
    }
    public void setCurrentTime(int pos)
    {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            m_mediaPlayer.seekTo( pos, SEEK_CLOSEST );
        }
        else {
            m_mediaPlayer.seekTo( pos );
        }
    }

    public double getVolume()
    {
        return m_volume;
    }

    public void setVolume(double percentage)
    {
 //       Log.i(TAG, "[Debug][Video]setVolume: " + percentage);
        m_volume = (float) (percentage < 0.0 ? 0.0 : (percentage > 1.0 ? 1.0 : percentage));
        AudioManager am = (AudioManager) m_ctx.getSystemService(AUDIO_SERVICE);
        int sysVolume= am.getStreamVolume(AudioManager.STREAM_MUSIC);

        if(m_playerState.isMuted)
            return;
        
        int value = (int)(sysVolume * m_volume);
        Log.i(TAG, "[Debug][Video]setVolume: " + value);
        m_mediaPlayer.setVolume(value, value);
    }

    public boolean getMuted()
    {
        return m_playerState.isMuted;
    }

    public void setMuted(boolean value)
    {
        if(m_playerState.isMuted == value)
            return;

        m_playerState.isMuted = value;
        m_playerState.applyMute();
    }

    private void emit(String evtName)
    {
        if(m_isEmitEnable)
            emit(m_appVideoPtr, evtName);
    }

    public boolean isFrameAvailable()
    {
        if(IsPaused())
        {
            return false;
        }
        return m_renderTarget.isFrameAvailable();
    }

    public void updateBitmap(long dataPtr)
    {
        Bitmap bitmap = m_renderTarget.getBitmap();
  //      Log.i(TAG, "[Debug][Video]updateBitmap: (" + bitmap.getWidth() + ", " + bitmap.getHeight() + ")" + "pointer : " + dataPtr);
        transferBitmap(bitmap, dataPtr);
    }

    @Override
    public void onRenderTargetCreated() {
        Log.i(TAG, "[Debug][Video]surfaceCreated: call me");
        m_mediaPlayer = new MediaPlayer();
        m_mediaPlayer.setSurface(m_renderTarget.getSurface());
        m_mediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);

        m_curStatus = MediaPlayerStatus.Idle;

        m_mediaPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
            @Override
            public void onCompletion(MediaPlayer mediaPlayer) {
                Log.i(TAG, "[Debug][Video]surfaceCreated: emit ended");
                m_currentTime = 0;
                emit("ended");
            }
        });

        m_isSurfaceCreated = true;

        m_playerState.applyLoop();
        m_playerState.applyMute();

        for (PendingData p : m_pendingDataAr) {
            if (p.type == PendingType.Load)
                Load(p.additionData);
            else if (p.type == PendingType.Play)
                Play();
            else if (p.type == PendingType.Pause)
                Pause();
            else if (p.type == PendingType.Stop)
                Stop();
        }

        m_pendingDataAr.clear();
    }
}
