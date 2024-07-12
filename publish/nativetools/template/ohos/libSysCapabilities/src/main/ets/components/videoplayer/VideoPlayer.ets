import { GlobalContext, GlobalContextConstants } from '../../common/GlobalContext';
import { BusinessError } from '@ohos.base';
import { Logger } from '../../utils/Logger'
import audio from '@ohos.multimedia.audio';

let log: Logger = new Logger(0x0001, "VideoPlayerMsg");

@Observed
export class VideoPlayerInfo {
  // position
  public x: number = 0;
  public y: number = 0;
  // size
  public w: number = 0;
  public h: number = 0;
  // url
  public url: string = ''

  public rawfile?: Resource;
  public isUrl: boolean = false
  // tag
  public viewTag: number = 0

  public isPlay: boolean = false
  public isFullScreen: boolean = false

  // Whether to display
  public visible: boolean = true

  public isLoop: boolean = false
  
  public autoPlay: boolean = false

  public objectFit: ImageFit = ImageFit.Auto

  public controller: VideoController = new VideoController()

  constructor(x: number, y: number, w: number, h: number, viewTag: number) {
    this.x = x;
    this.y = y;
    this.w = w;
    this.h = h;
    this.viewTag = viewTag;
  }
}
//表示视频元素的就绪状态：
// 0 = HAVE_NOTHING - 没有关于音频/视频是否就绪的信息
// 1 = HAVE_METADATA - 关于音频/视频就绪的元数据
// 2 = HAVE_CURRENT_DATA - 关于当前播放位置的数据是可用的，但没有足够的数据来播放下一帧/毫秒
// 3 = HAVE_FUTURE_DATA - 当前及至少下一帧的数据是可用的
// 4 = HAVE_ENOUGH_DATA - 可用数据足以开始播放
export enum VideoEvent {
  HAVE_NOTHING = 0,
  HAVE_METADATA,
  HAVE_CURRENT_DATA,
  HAVE_FUTURE_DATA,
  HAVE_ENOUGH_DATA,
}

export  class VideoPlayer{

  static  createVideoPlayer(viewTag: number) : void {
    let newVideoPlayerInfo = new VideoPlayerInfo(0, 0, 0, 0, viewTag);
    GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_VIDEO_PLAYER_ARRAY).push(newVideoPlayerInfo);
    GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_VIDEO_PLAYER_INDEX_MAP).set(viewTag, newVideoPlayerInfo);
  }
  static removeVideoPlayer(viewTag: number) : void {
    let removeIndex = -1;
    GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_VIDEO_PLAYER_ARRAY).forEach((item:VideoPlayerInfo,index:number) => {
      if (item.viewTag == viewTag) {
        removeIndex = index;
      }
    });
    GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_VIDEO_PLAYER_INDEX_MAP).get(viewTag).controller.requestFullscreen(false); //4.x已修复
    GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_VIDEO_PLAYER_ARRAY).splice(removeIndex, 1);
    GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_VIDEO_PLAYER_INDEX_MAP).delete(viewTag);
  }

  static setURL(viewTag: number, url: string, isUrl: number) : void {
    let videoPlayInfo:VideoPlayerInfo = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_VIDEO_PLAYER_INDEX_MAP).get(viewTag);
    if(isUrl == 0) {
      videoPlayInfo.isUrl = false;
      videoPlayInfo.rawfile = $rawfile(url) ;//$rawfile("video1.mp4");
    } else {
      videoPlayInfo.isUrl = true;
      videoPlayInfo.url = url;
    }
  }
  static  play(viewTag: number) : void {
    let videoPlayInfo :VideoPlayerInfo = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_VIDEO_PLAYER_INDEX_MAP).get(viewTag);
    videoPlayInfo.controller.start();
    videoPlayInfo.isPlay = true;
  }
  static  pause(viewTag: number) : void {
    let videoPlayInfo:VideoPlayerInfo = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_VIDEO_PLAYER_INDEX_MAP).get(viewTag);
    videoPlayInfo.controller.pause();
    videoPlayInfo.isPlay = false;
  }
  static  stop(viewTag: number) : void {
    let videoPlayInfo:VideoPlayerInfo = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_VIDEO_PLAYER_INDEX_MAP).get(viewTag);
    videoPlayInfo.controller.stop();
    videoPlayInfo.isPlay = false;
  }
  static  setLooping(viewTag: number, isLoop: boolean) :void {
    let videoPlayInfo: VideoPlayerInfo = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_VIDEO_PLAYER_INDEX_MAP).get(viewTag);
    videoPlayInfo.isLoop = isLoop;
  }
  static  setAutoplay(viewTag: number, autoPlay: boolean) :void {
    let videoPlayInfo: VideoPlayerInfo = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_VIDEO_PLAYER_INDEX_MAP).get(viewTag);
    videoPlayInfo.autoPlay = autoPlay;

  }
  static  setX(viewTag: number, val: number) :void {
    let videoPlayInfo:VideoPlayerInfo = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_VIDEO_PLAYER_INDEX_MAP).get(viewTag);
    try {
      videoPlayInfo.x = val;
    } catch (error) {
      let e: BusinessError = error as BusinessError;
      log.error('videoPlayerInfo  ErrorCode: %{public}d,  Message: %{public}s', e.code, e.message);
    }
  }
  static  setY(viewTag: number, val: number) :void {
    let videoPlayInfo:VideoPlayerInfo = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_VIDEO_PLAYER_INDEX_MAP).get(viewTag);
    try {
      videoPlayInfo.y = val;
    } catch (error) {
      let e: BusinessError = error as BusinessError;
      log.error('videoPlayerInfo  ErrorCode: %{public}d,  Message: %{public}s', e.code, e.message);
    }
  }
  static  setWidth(viewTag: number, val: number) : void {
    let videoPlayInfo:VideoPlayerInfo = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_VIDEO_PLAYER_INDEX_MAP).get(viewTag);
    try {
      videoPlayInfo.w = val;
    } catch (error) {
      let e: BusinessError = error as BusinessError;
      log.error('videoPlayerInfo  ErrorCode: %{public}d,  Message: %{public}s', e.code, e.message);
    }
  }
  static  setHeight(viewTag: number, val: number) : void {
    let videoPlayInfo:VideoPlayerInfo = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_VIDEO_PLAYER_INDEX_MAP).get(viewTag);
    try {
      videoPlayInfo.h = val;
    } catch (error) {
      let e: BusinessError = error as BusinessError;
      log.error('videoPlayerInfo  ErrorCode: %{public}d,  Message: %{public}s', e.code, e.message);
    }
  }
  static  setCurrentTime(viewTag: number, val: number) : void {
    let videoPlayInfo:VideoPlayerInfo = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_VIDEO_PLAYER_INDEX_MAP).get(viewTag);
    videoPlayInfo.controller.setCurrentTime(val)
  }
  static  getVolume(viewTag: number) : number {
    let audioManager = audio.getAudioManager();
    let audioVolumeManager = audioManager.getVolumeManager();
    audioVolumeManager.on('volumeChange', (volumeEvent: audio.VolumeEvent) => {
      console.info(`VolumeType of stream: ${volumeEvent.volumeType} `);
      console.info(`Volume level: ${volumeEvent.volume} `);
      console.info(`Whether to updateUI: ${volumeEvent.updateUi} `);
      return volumeEvent.volume;
    });
    return 0;
  }
  static  setVolume(viewTag: number, val: number) : void {
  }
}