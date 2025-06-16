export enum ContextType {
  APP_LIFECYCLE = 0,
  JSPAGE_LIFECYCLE,
  WORKER_INIT,
  NATIVE_API,
  VIDEOPLAYER_NAPI,
  MOUSE_WHEEL_NAPI,
}

export class Constants {
  static readonly APP_KEY_WORKER_MANAGER = "app_key_worker_manager";
}