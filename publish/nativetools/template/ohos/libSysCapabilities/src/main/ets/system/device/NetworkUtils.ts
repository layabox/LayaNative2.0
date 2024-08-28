import wifiManager from '@ohos.wifiManager';
import connection from '@ohos.net.connection';

export class NetworkUtils {

    static isWifiConnected() : boolean {
        return wifiManager.isConnected();
    }

    /**
     * 0-没有网络
     * 1-WIFI网络
     * 4-蜂窝网络
     */
    static getNetworkType() : number {
        let netHandle = connection.getDefaultNetSync();
        let networkType = 0;
        if(netHandle && netHandle.netId != 0) {
            let result = connection.getNetCapabilitiesSync(netHandle);
            if (result && result.bearerTypes) {
                networkType = result.bearerTypes[0]==0 ? 4 : 1;
            }
        }
        return networkType;
    }
}