import resmgr from '@ohos.resourceManager';
import resourceManager from "@ohos.resourceManager";

export interface CPPFunctions {
  onCreate: () => void;
  onShow: () => void;
  onHide: () => void;
  onBackPress: () => void;
  onDestroy: () => void;
  onPageShow: () => void;
  onPageHide: () => void;
  nativeResourceManagerInit: (resourceManager: resmgr.ResourceManager) => void;
  writablePathInit: (writePath: string) => void;
  workerInit: () => void;
  nativeEngineStart: () => void;
  onVideoCallBack: (viewTag: number, event: number) => void;
  onDurationBack: (viewTag: number, duration: number) => void;
  onCurrentTimeBack: (viewTag: number, currentTime: number) => void;
  loadedmetadata: (viewTag: number) => void;
}

export const getContext: (a: number) => CPPFunctions;

export const ConchNAPI_InitDLib: (rsmgr: resourceManager.ResourceManager, nThreadNum: number, p_strAssetRootPath: string, p_strCachePath: string, threadMode: number, debugMode: number, debugPort: number) => void;
export const ConchNAPI_configSetURL: (p_strUrl: string) => void;
export const ConchNAPI_configSetIsPlug: (v: boolean) => void;
export const ConchNAPI_SetLocalStoragePath:(p_strLocalStorage: string) => void;
export const ConchNAPI_setLocalizable: (p_bIsLocalPackage: boolean) => void;
export const ConchNAPI_OnAppStart: () => void;
export const ConchNAPI_handleDeviceMotionEvent: (ax: number, ay: number, az: number, agx: number, agy: number, agz: number, ra: number, rb: number, rg: number, interval: number) => void;
export const ConchNAPI_handleDeviceOrientationEvent: (ra: number, rb: number, rg: number) => void;