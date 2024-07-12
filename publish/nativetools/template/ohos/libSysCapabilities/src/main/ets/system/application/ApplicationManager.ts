import bundleManager from '@ohos.bundle.bundleManager';
import process from '@ohos.process';

export default class ApplicationManager {
  static getApplicationInfo() {
    let bundleFlags = bundleManager.BundleFlag.GET_BUNDLE_INFO_DEFAULT | bundleManager.BundleFlag.GET_BUNDLE_INFO_WITH_APPLICATION | bundleManager.BundleFlag.GET_BUNDLE_INFO_WITH_METADATA | bundleManager.BundleFlag.GET_BUNDLE_INFO_WITH_SIGNATURE_INFO;
    return JSON.stringify(bundleManager.getBundleInfoForSelfSync(bundleFlags));
  }

  static pro = new process.ProcessManager();

  static exit() : void {
      ApplicationManager.pro.exit(0);
  }
  
}