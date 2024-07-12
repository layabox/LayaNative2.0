import wifiManager from '@ohos.wifiManager';
import connection from '@ohos.net.connection';

export class NetworkUtils {

    static isWifiConnected() : boolean {
        return wifiManager.isConnected();
    }

    static getNetworkType() : number {
        let netHandle = connection.getDefaultNetSync();
        if(netHandle && netHandle.netId != 0) {
            let result = connection.getNetCapabilitiesSync(netHandle);
            if (result && result.bearerTypes) {
                return result.bearerTypes[0];
            }
        }
        return 1;
    }
}