import media from '@ohos.multimedia.media';
import fs from '@ohos.file.fs';
import laya from "liblaya.so";
import audio from '@ohos.multimedia.audio';
import { BusinessError } from '@ohos.base';

export default class SoundUtils {
    private static avPlayer: media.AVPlayer;
    private static mVolume = 0.5;
    private static mPaused = false;

    static async playBackgroundMusic(p_sFilePath: string, p_nTimes: number, nCurrentTime: number) {
        console.log("enter js playBackgroundMusic, " + p_sFilePath);
        SoundUtils.avPlayer = await media.createAVPlayer();
        let fdPath = 'fd://';
        let file = await fs.open(p_sFilePath);
        fdPath = fdPath + '' + file.fd;
        SoundUtils.avPlayer.url = fdPath;

        let avPlayer = SoundUtils.avPlayer;
        avPlayer.on('stateChange', async (state: string, reason: media.StateChangeReason) => {
            switch (state) {
                case 'idle': // 成功调用reset接口后触发该状态机上报
                    console.info('AVPlayer state idle called.');
                    avPlayer.release(); // 调用release接口销毁实例对象
                    break;
                case 'initialized': // avplayer 设置播放源后触发该状态上报
                    console.info('AVPlayer state initialized called.');
                    let audioRendererInfo : audio.AudioRendererInfo = {
                        usage: audio.StreamUsage.STREAM_USAGE_GAME,
                        rendererFlags: 0
                    }
                    avPlayer.audioRendererInfo = audioRendererInfo;
                    avPlayer.prepare().then(() => {
                        console.info('AVPlayer prepare succeeded.');
                    }, (err: BusinessError) => {
                        console.error(`Invoke prepare failed, code is ${err.code}, message is ${err.message}`);
                    });
                    break;
                case 'prepared': // prepare调用成功后上报该状态机
                    console.info('AVPlayer state prepared called.');
                    let bLoop = (p_nTimes == -1);
                    avPlayer.loop = bLoop;
                    avPlayer.setVolume(SoundUtils.mVolume);
                    avPlayer.play(); // 调用播放接口开始播放
                    break;
                case 'playing': // play成功调用后触发该状态机上报
                    console.info('AVPlayer state playing called.');
                    break;
                case 'paused': // pause成功调用后触发该状态机上报
                    console.info('AVPlayer state paused called.');
                    break;
                case 'completed': // 播放结束后触发该状态机上报
                    console.info('AVPlayer state completed called.');
                    laya.ConchNAPI_audioMusicPlayEnd();
                    break;
                case 'stopped': // stop接口成功调用后触发该状态机上报
                    console.info('AVPlayer state stopped called.');
                    avPlayer.reset(); // 调用reset接口初始化avplayer状态
                    break;
                case 'released':
                    console.info('AVPlayer state released called.');
                    break;
                default:
                    console.info('AVPlayer state unknown called.');
                    break;
            }
        });

        avPlayer.on('audioInterrupt', (interruptEvent: audio.InterruptEvent) => {
            switch (interruptEvent.hintType) {
                case audio.InterruptHint.INTERRUPT_HINT_PAUSE:
                    avPlayer.pause();
                    break;
                case audio.InterruptHint.INTERRUPT_HINT_STOP:
                    avPlayer.stop();
                    break;
                case audio.InterruptHint.INTERRUPT_HINT_RESUME:
                    avPlayer.play();
                    break;
                default:
                    console.info('Invalid interruptEvent', interruptEvent.hintType);
                    break;
            }
        });
    }

    static stopBackgroundMusic() {
        if (SoundUtils.avPlayer != null) {
            SoundUtils.avPlayer.stop();
            SoundUtils.mPaused = true;
        }
    }

    static resumeBackgroundMusic() {
        if (SoundUtils.avPlayer != null) {
            SoundUtils.avPlayer.play();
            SoundUtils.mPaused = false;
        }
    }

    static pauseBackgroundMusic() {
        if (SoundUtils.avPlayer != null) {
            SoundUtils.avPlayer.pause();
            SoundUtils.mPaused = true;
        }
    }

    static setBackgroundMusicVolume(p_fVolume: number) {
        if (p_fVolume < 0.0) {
            p_fVolume = 0.0;
        }

        if (p_fVolume > 1.0) {
            p_fVolume = 1.0;
        }
        SoundUtils.mVolume = p_fVolume;
        if (SoundUtils.avPlayer != null) {
            SoundUtils.avPlayer.setVolume(SoundUtils.mVolume);
        }
    }

    static setCurrentTime(nCurrentTime: number) {
        if (SoundUtils.avPlayer != null) {
            SoundUtils.avPlayer.seek(nCurrentTime * 1000, media.SeekMode.SEEK_PREV_SYNC)
        }
    }

    static getCurrentTime() {
        if (SoundUtils.avPlayer != null) {
            return SoundUtils.avPlayer.currentTime * 0.001;
        }
        return 0.0;
    }

    static getDuration(): number {
        if (SoundUtils.avPlayer != null) {
            return SoundUtils.avPlayer.duration;
        }
        return 0.0;
    }
}