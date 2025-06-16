import { WorkerManager } from '../workers/WorkerManager'
import worker from '@ohos.worker';
import { VideoPlayerInfo, VideoEvent } from '@ohos/libSysCapabilities';

@Component
export struct LayaVideoPlayer {
  @ObjectLink videoPlayerInfo: VideoPlayerInfo;
  layaWorker: worker.ThreadWorker = WorkerManager.getInstance().getWorker();

  build() {
    Column() {
      Video({
        src: this.videoPlayerInfo.isUrl ? this.videoPlayerInfo.url : this.videoPlayerInfo.rawfile,
        controller: this.videoPlayerInfo.controller
      })
        .position({ x: px2vp(this.videoPlayerInfo.x), y: px2vp(this.videoPlayerInfo.y) })
        .width(px2vp(this.videoPlayerInfo.w))
        .height(px2vp(this.videoPlayerInfo.h))
        .visibility(this.videoPlayerInfo.visible ? Visibility.Visible : Visibility.None)
        .controls(false)
        .loop(this.videoPlayerInfo.isLoop)
        .autoPlay(this.videoPlayerInfo.autoPlay)
        .objectFit(this.videoPlayerInfo.objectFit)
        .onPrepared((e) => {
          this.layaWorker.postMessage({
                type: "loadedmetadata",
                viewTag: this.videoPlayerInfo.viewTag,
          });
          if (this.videoPlayerInfo.isPlay) {
            this.videoPlayerInfo.controller.start()
          }
          this.videoPlayerInfo.controller.requestFullscreen(this.videoPlayerInfo.isFullScreen)
          this.layaWorker.postMessage({
            type: "onVideoCallBack",
            viewTag: this.videoPlayerInfo.viewTag,
            event: VideoEvent.HAVE_ENOUGH_DATA,
          });
          this.layaWorker.postMessage({
            type: "onDurationBack",
            viewTag: this.videoPlayerInfo.viewTag,
            duration: e.duration
          });
        })
        .onStart(() => {
          this.layaWorker.postMessage({
            type: "onVideoCallBack",
            viewTag: this.videoPlayerInfo.viewTag,
            event: VideoEvent.HAVE_ENOUGH_DATA
          });
        })
        .onPause(() => {
          this.layaWorker.postMessage({
            type: "onVideoCallBack",
            viewTag: this.videoPlayerInfo.viewTag,
            event: VideoEvent.HAVE_CURRENT_DATA
          });
        })
        .onUpdate((e) => {
          this.layaWorker.postMessage({
            type: "onCurrentTimeBack",
            viewTag: this.videoPlayerInfo.viewTag,
            currentTime: e.time
          });
        })
        .onFinish(() => {
          this.layaWorker.postMessage({
            type: "onVideoCallBack",
            viewTag: this.videoPlayerInfo.viewTag,
            event: VideoEvent.HAVE_CURRENT_DATA
          });
        })
    }
  }
}
