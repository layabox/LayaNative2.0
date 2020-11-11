
/**
 * https://webbluetoothcg.github.io/web-bluetooth/
 */
enum VendorIDSource {
  "bluetooth",
  "usb"
};

interface BluetoothDeviceEventHandlers{
    
}
interface CharacteristicEventHandlers{
    
}

interface ServiceEventHandlers{
    
}

interface RequestDeviceOptions{
    
}

interface BluetoothAdvertisingData{
    
}

interface BluetoothRemoteGATTServer{
    
} 

class BluetoothDevice {
  id:string;
  name:string;
  adData:BluetoothAdvertisingData;
  deviceClass:number;
  vendorIDSource:VendorIDSource;
  vendorID:number;
  productID:number;
  productVersion:number;
  paired:boolean;
  gatt:BluetoothRemoteGATTServer;
  uuids:string;
  connectGATT():Promise<any>{
      return null;
  }
}


class Bluetooth extends EventTarget implements BluetoothDeviceEventHandlers, CharacteristicEventHandlers, ServiceEventHandlers {
    requestDevice(options:RequestDeviceOptions):Promise<BluetoothDevice>{
        return null;
    }
}

//applyMixins(Bluetooth,[EventTarget]);