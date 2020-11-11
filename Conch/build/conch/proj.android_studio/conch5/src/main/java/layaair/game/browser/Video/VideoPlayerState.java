package layaair.game.browser.Video;

import android.content.Context;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.util.Log;

import layaair.game.browser.LayaVideoPlayer;

import static android.content.Context.AUDIO_SERVICE;
import static layaair.game.browser.LayaVideoPlayer.TAG;

public class VideoPlayerState {
    public boolean isMuted = false;
    public boolean isAutoplay = false;
    public boolean isLoop = false;

    private LayaVideoPlayer m_layaVideoPlayer;
    private Context m_ctx;

    public VideoPlayerState(Context context, LayaVideoPlayer layaVideoPlayer)
    {
        m_layaVideoPlayer = layaVideoPlayer;
        m_ctx = context;
    }

    public boolean applyMute()
    {
        MediaPlayer mediaPlayer = m_layaVideoPlayer.getMediaPlayer();
        if(mediaPlayer == null)
            return false;

        double volume = isMuted ? 0 : m_layaVideoPlayer.getVolume();
        AudioManager am = (AudioManager) m_ctx.getSystemService(AUDIO_SERVICE);
        int sysVolume= am.getStreamVolume(AudioManager.STREAM_MUSIC);
        int value = (int)(sysVolume * volume);
        mediaPlayer.setVolume(value, value);

        return true;
    }

    public boolean applyAutoplay()
    {
        if(m_layaVideoPlayer.getMediaPlayer() == null)
            return false;

        if(isAutoplay) {
            Log.i(TAG, "[Debug][Video]applyAutoplay: call me");
            m_layaVideoPlayer.Play();
        }
        return true;
    }

    public boolean applyLoop()
    {
        MediaPlayer mediaPlayer = m_layaVideoPlayer.getMediaPlayer();

        if(mediaPlayer == null)
            return false;

        mediaPlayer.setLooping(isLoop);
        return true;
    }
}
