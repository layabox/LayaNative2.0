
class Screen_ extends EventTarget {
    availHeight: number;
    availWidth: number;
    bufferDepth: number;
    colorDepth: number;
    deviceXDPI: number;
    deviceYDPI: number;
    fontSmoothingEnabled: boolean;
    height: number;
    logicalXDPI: number;
    logicalYDPI: number;
    msOrientation: string;
    onmsorientationchange: (ev: Event) => any;
    pixelDepth: number;
    systemXDPI: number;
    systemYDPI: number;
    width: number;
}