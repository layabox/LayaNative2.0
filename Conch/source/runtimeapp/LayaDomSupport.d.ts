/// <reference path="../domsupport/ES6NoDOM.d.ts" />
/// <reference path="../domsupport/LayaConchRuntime.d.ts" />
interface IPlatForm {
    callBackList: Object;
    call(methodName: any, p1?: any, p2?: any, p3?: any, p4?: any, p5?: any, p6?: any, p7?: any, p8?: any, p9?: any): any;
    callWithBack(callback: any, methodName: any, p1?: any, p2?: any, p3?: any, p4?: any, p5?: any, p6?: any, p7?: any, p8?: any, p9?: any): void;
}
declare class PlatformBase implements IPlatForm {
    callBackList: Object;
    className: string;
    objid: number;
    call(methodName: any, p1?: any, p2?: any, p3?: any, p4?: any, p5?: any, p6?: any, p7?: any, p8?: any, p9?: any): any;
    callWithBack(callback: any, methodName: any, p1?: any, p2?: any, p3?: any, p4?: any, p5?: any, p6?: any, p7?: any, p8?: any, p9?: any): void;
}
declare class PlatformClass extends PlatformBase {
    static clsMap: Object;
    constructor(clsName: string);
    static createClass(clsName: string): PlatformClass;
    newObject(p1?: any, p2?: any, p3?: any, p4?: any, p5?: any, p6?: any, p7?: any, p8?: any, p9?: any): PlatformObj;
}
declare class PlatformObj extends PlatformBase {
    static objMap: Object;
    static objNum: number;
    cls: PlatformClass;
    callBackList: Object;
    constructor(cls: PlatformClass);
    init(): void;
}
declare class WindowBase64 {
    atob: (encodedString: string) => string;
    btoa: (rawString: string) => string;
    constructor();
}
interface IBuffer {
    id: number;
}
declare class _Buffer {
    private ABSIZE;
    private _cmdBuf;
    _byteArray: Uint8Array;
    private _cmdPos;
    private _nativeobj;
    constructor();
    needsz(d: number): void;
    initPos(d: number): void;
    wu8(v: number): void;
    wu32(v: number): void;
    wu16(v: number): void;
    wf32(v: number): void;
    wf32Rect(x: number, y: number, w: number, h: number): void;
    wu64(value: number): void;
    wab(arraybuffer: ArrayBuffer | ArrayBufferView, length: number): void;
    wstring(str: string, is32: boolean): void;
    flush(): void;
    static readString(bf: DataView, pos: number, len: number): string;
    static setMatrix(that: IBuffer, fid: number, m11: number, m12: number, m21: number, m22: number, dx: number, dy: number): void;
    static rect(that: IBuffer, fid: number, x: number, y: number, w: number, h: number): void;
    static noParm(that: IBuffer, fid: number): void;
    static oneNum(that: IBuffer, fid: number, n: number): void;
    static oneInt(that: IBuffer, fid: number, n: number): void;
    static point(that: IBuffer, fid: number, x: number, y: number): void;
}
declare function _process(): void;
interface Algorithm {
    name?: string;
}
interface KeyAlgorithm {
    name?: string;
}
interface CryptoKey {
    algorithm: KeyAlgorithm;
    extractable: boolean;
    type: string;
    usages: string[];
}
declare class SubtleCrypto {
    decrypt(algorithm: string | Algorithm, key: CryptoKey, data: ArrayBufferView): any;
    deriveBits(algorithm: string | Algorithm, baseKey: CryptoKey, length: number): any;
    deriveKey(algorithm: string | Algorithm, baseKey: CryptoKey, derivedKeyType: string | Algorithm, extractable: boolean, keyUsages: string[]): any;
    digest(algorithm: string | Algorithm, data: ArrayBufferView): any;
    encrypt(algorithm: string | Algorithm, key: CryptoKey, data: ArrayBufferView): any;
    exportKey(format: string, key: CryptoKey): any;
    generateKey(algorithm: string | Algorithm, extractable: boolean, keyUsages: string[]): any;
    importKey(format: string, keyData: ArrayBufferView, algorithm: string | Algorithm, extractable: boolean, keyUsages: string[]): any;
    sign(algorithm: string | Algorithm, key: CryptoKey, data: ArrayBufferView): any;
    unwrapKey(format: string, wrappedKey: ArrayBufferView, unwrappingKey: CryptoKey, unwrapAlgorithm: string | Algorithm, unwrappedKeyAlgorithm: string | Algorithm, extractable: boolean, keyUsages: string[]): any;
    verify(algorithm: string | Algorithm, key: CryptoKey, signature: ArrayBufferView, data: ArrayBufferView): any;
    wrapKey(format: string, key: CryptoKey, wrappingKey: CryptoKey, wrapAlgorithm: string | Algorithm): any;
}
declare class Crypto {
    subtle: SubtleCrypto;
    getRandomValues(array: ArrayBufferView): ArrayBufferView;
}
declare enum LogLevel {
    Warn = 0,
    Error = 1,
    Debug = 2,
    Info = 3,
    Runtime = 4,
}
declare class Console {
    constructor();
    assert(test?: boolean, message?: string): void;
    clear(): void;
    count(countTitle?: string): void;
    debug(message?: string): void;
    dir(value?: any): void;
    dirxml(value: any): void;
    error(message?: any): void;
    group(groupTitle?: string): void;
    groupCollapsed(groupTitle?: string): void;
    groupEnd(): void;
    info(message?: any): void;
    log(message?: any): void;
    profile(reportName?: string): void;
    profileEnd(): void;
    select(element: Element): void;
    time(timerName?: string): void;
    timeEnd(timerName?: string): void;
    trace(message?: any): void;
    warn(message?: any): void;
}
interface EventListener {
    (evt: Event): void;
}
interface EventListenerObject {
    handleEvent(evt: Event): void;
}
declare type EventListenerOrEventListenerObject = EventListener | EventListenerObject;
interface EventHandler {
    listener: EventListenerOrEventListenerObject;
    useCapture: boolean;
}
interface EventInit {
    bubbles?: boolean;
    cancelable?: boolean;
}
interface GlobalEventHandlers {
    addEventListener(type: "pointercancel", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointerdown", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointerenter", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointerleave", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointermove", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointerout", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointerover", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointerup", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "wheel", listener: (ev: WheelEvent) => any, useCapture?: boolean): void;
    addEventListener(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void;
}
declare class GlobalEventHandlers implements GlobalEventHandlers {
    constructor();
    onpointercancel(ev: PointerEvent): any;
    onpointerdown(ev: PointerEvent): any;
    onpointerenter(ev: PointerEvent): any;
    onpointerleave(ev: PointerEvent): any;
    onpointermove(ev: PointerEvent): any;
    onpointerout(ev: PointerEvent): any;
    onpointerover(ev: PointerEvent): any;
    onpointerup(ev: PointerEvent): any;
}
declare class ErrorEvent implements ErrorEvent {
    constructor();
}
declare class Event {
    bubbles: boolean;
    cancelBubble: boolean;
    cancelable: boolean;
    currentTarget: EventTarget;
    defaultPrevented: boolean;
    eventPhase: number;
    isTrusted: boolean;
    returnValue: boolean;
    srcElement: Element;
    target: EventTarget;
    timeStamp: number;
    type: string;
    _propagationStopped: boolean;
    constructor(type: string, eventInitDict?: EventInit);
    initEvent(eventTypeArg: string, canBubbleArg: boolean, cancelableArg: boolean): void;
    preventDefault(): void;
    stopImmediatePropagation(): void;
    stopPropagation(): void;
    static AT_TARGET: number;
    static BUBBLING_PHASE: number;
    static CAPTURING_PHASE: number;
}
declare var _lbEvent: typeof Event;
interface _EventTarget {
    addEventListener(type: "mousedown", listener: (ev: MouseEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "mouseenter", listener: (ev: MouseEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "mouseleave", listener: (ev: MouseEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "mousemove", listener: (ev: MouseEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "mouseout", listener: (ev: MouseEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "mouseover", listener: (ev: MouseEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "mouseup", listener: (ev: MouseEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "mousewheel", listener: (ev: MouseWheelEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointercancel", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointerdown", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointerenter", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointerleave", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointermove", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointerout", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointerover", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointerup", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "touchcancel", listener: (ev: TouchEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "touchend", listener: (ev: TouchEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "touchmove", listener: (ev: TouchEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "touchstart", listener: (ev: TouchEvent) => any, useCapture?: boolean): void;
    addEventListener(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void;
    dispatchEvent(evt: Event): boolean;
    removeEventListener(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void;
}
declare var _lbMap: MapConstructor;
declare class EventTarget implements _EventTarget {
    protected _evtMaps: Map<string, EventHandler[]>;
    dispatchEvent: (evt: Event) => boolean;
    addEventListener(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void;
    fireEventListeners: (evt: Event) => void;
    constructor();
    protected _addEventListener(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void;
    protected _dispatchEvent(evt: Event): boolean;
    removeEventListener: (type: string, listener: EventListener | EventListenerObject, useCapture?: boolean) => void;
    protected _fireEventListeners(evt: Event): void;
}
interface UIEventInit extends EventInit {
    view?: any;
    detail?: number;
}
interface SharedKeyboardAndMouseEventInit extends UIEventInit {
    ctrlKey?: boolean;
    shiftKey?: boolean;
    altKey?: boolean;
    metaKey?: boolean;
    keyModifierStateAltGraph?: boolean;
    keyModifierStateCapsLock?: boolean;
    keyModifierStateFn?: boolean;
    keyModifierStateFnLock?: boolean;
    keyModifierStateHyper?: boolean;
    keyModifierStateNumLock?: boolean;
    keyModifierStateOS?: boolean;
    keyModifierStateScrollLock?: boolean;
    keyModifierStateSuper?: boolean;
    keyModifierStateSymbol?: boolean;
    keyModifierStateSymbolLock?: boolean;
}
declare class UIEvent extends Event {
    constructor(type: string, eventInitDict?: UIEventInit);
    detail: number;
    view: Window;
    initUIEvent(typeArg: string, canBubbleArg: boolean, cancelableArg: boolean, viewArg: Window, detailArg: number): void;
}
interface KeyboardEventInit extends SharedKeyboardAndMouseEventInit {
    key?: string;
    location?: number;
    repeat?: boolean;
}
interface MouseEventInit extends SharedKeyboardAndMouseEventInit {
    screenX?: number;
    screenY?: number;
    clientX?: number;
    clientY?: number;
    button?: number;
    buttons?: number;
    relatedTarget?: EventTarget;
}
interface PointerEventInit extends MouseEventInit {
    pointerId?: number;
    width?: number;
    height?: number;
    pressure?: number;
    tiltX?: number;
    tiltY?: number;
    pointerType?: string;
    isPrimary?: boolean;
}
interface WheelEventInit extends MouseEventInit {
    deltaX?: number;
    deltaY?: number;
    deltaZ?: number;
    deltaMode?: number;
}
declare class PointerEvent {
    constructor(typeArg: string, eventInitDict?: PointerEventInit);
}
declare class MouseEvent extends UIEvent {
    altKey: boolean;
    button: number;
    buttons: number;
    clientX: number;
    clientY: number;
    ctrlKey: boolean;
    fromElement: Element;
    layerX: number;
    layerY: number;
    metaKey: boolean;
    movementX: number;
    movementY: number;
    offsetX: number;
    offsetY: number;
    pageX: number;
    pageY: number;
    relatedTarget: EventTarget;
    screenX: number;
    screenY: number;
    shiftKey: boolean;
    toElement: Element;
    which: number;
    x: number;
    y: number;
    constructor(typeArg: string, eventInitDict?: MouseEventInit);
    getModifierState(keyArg: string): boolean;
    initMouseEvent(typeArg: string, canBubbleArg: boolean, cancelableArg: boolean, viewArg: Window, detailArg: number, screenXArg: number, screenYArg: number, clientXArg: number, clientYArg: number, ctrlKeyArg: boolean, altKeyArg: boolean, shiftKeyArg: boolean, metaKeyArg: boolean, buttonArg: number, relatedTargetArg: EventTarget): void;
}
declare var _lbMouseEvent: typeof MouseEvent;
declare class MouseWheelEvent extends MouseEvent {
    wheelDelta: number;
    wheelDeltaX: number;
    wheelDeltaY: number;
    initMouseWheelEvent(typeArg: string, canBubbleArg: boolean, cancelableArg: boolean, viewArg: Window, detailArg: number, screenXArg: number, screenYArg: number, clientXArg: number, clientYArg: number, buttonArg: number, relatedTargetArg: EventTarget, modifiersListArg: string, wheelDeltaArg: number): void;
    constructor();
}
declare class WheelEvent extends MouseEvent {
    deltaMode: number;
    deltaX: number;
    deltaY: number;
    deltaZ: number;
    getCurrentPoint(element: Element): void;
    initWheelEvent(typeArg: string, canBubbleArg: boolean, cancelableArg: boolean, viewArg: Window, detailArg: number, screenXArg: number, screenYArg: number, clientXArg: number, clientYArg: number, buttonArg: number, relatedTargetArg: EventTarget, modifiersListArg: string, deltaXArg: number, deltaYArg: number, deltaZArg: number, deltaMode: number): void;
    constructor(typeArg: string, eventInitDict?: WheelEventInit);
    static DOM_DELTA_LINE: number;
    static DOM_DELTA_PAGE: number;
    static DOM_DELTA_PIXEL: number;
}
declare class Touch {
    clientX: number;
    clientY: number;
    identifier: number;
    pageX: number;
    pageY: number;
    screenX: number;
    screenY: number;
    target: EventTarget;
    constructor();
}
declare class TouchList extends Array {
    item(index: number): Touch;
}
interface TouchEvent extends UIEvent {
    altKey: boolean;
    changedTouches: TouchList;
    ctrlKey: boolean;
    metaKey: boolean;
    shiftKey: boolean;
    targetTouches: TouchList;
    touches: TouchList;
    new (type: number, id: number, name: string, x: number, y: number): TouchEvent;
}
interface DeviceAcceleration {
    x: number;
    y: number;
    z: number;
}
interface DeviceRotationRate {
    alpha: number;
    beta: number;
    gamma: number;
}
interface DeviceAccelerationDict {
    x?: number;
    y?: number;
    z?: number;
}
interface DeviceRotationRateDict {
    alpha?: number;
    beta?: number;
    gamma?: number;
}
declare class DeviceMotionEvent extends Event {
    acceleration: DeviceAcceleration;
    accelerationIncludingGravity: DeviceAcceleration;
    interval: number;
    rotationRate: DeviceRotationRate;
    constructor();
    initDeviceMotionEvent(type: string, bubbles: boolean, cancelable: boolean, acceleration: DeviceAccelerationDict, accelerationIncludingGravity: DeviceAccelerationDict, rotationRate: DeviceRotationRateDict, interval: number): void;
}
declare class DeviceOrientationEvent extends Event {
    absolute: boolean;
    alpha: number;
    beta: number;
    gamma: number;
    constructor();
    initDeviceOrientationEvent(type: string, bubbles: boolean, cancelable: boolean, alpha: number, beta: number, gamma: number, absolute: boolean): void;
}
declare class DocumentEvent {
    constructor();
    createEvent(eventInterface: string): Event;
}
declare class ProgressEvent extends Event {
    lengthComputable: boolean;
    loaded: number;
    total: number;
    constructor(type: string);
    initProgressEvent(typeArg: string, canBubbleArg: boolean, cancelableArg: boolean, lengthComputableArg: boolean, loadedArg: number, totalArg: number): void;
}
declare var _lbProgressEvent: typeof ProgressEvent;
declare enum VendorIDSource {
    "bluetooth" = 0,
    "usb" = 1,
}
interface BluetoothDeviceEventHandlers {
}
interface CharacteristicEventHandlers {
}
interface ServiceEventHandlers {
}
interface RequestDeviceOptions {
}
interface BluetoothAdvertisingData {
}
interface BluetoothRemoteGATTServer {
}
declare class BluetoothDevice {
    id: string;
    name: string;
    adData: BluetoothAdvertisingData;
    deviceClass: number;
    vendorIDSource: VendorIDSource;
    vendorID: number;
    productID: number;
    productVersion: number;
    paired: boolean;
    gatt: BluetoothRemoteGATTServer;
    uuids: string;
    connectGATT(): Promise<any>;
}
declare class Bluetooth extends EventTarget implements BluetoothDeviceEventHandlers, CharacteristicEventHandlers, ServiceEventHandlers {
    requestDevice(options: RequestDeviceOptions): Promise<BluetoothDevice>;
}
interface GamepadButton {
    pressed: boolean;
    value: number;
}
declare class Gamepad {
    axes: number[];
    buttons: GamepadButton[];
    connected: boolean;
    id: string;
    index: number;
    mapping: string;
    timestamp: number;
}
declare class GamepadEvent extends Event {
    gamepad: Gamepad;
}
declare class Storage {
    private storagePath;
    private filename;
    private db;
    private _len;
    fileNamePre: string;
    length: number;
    getItem(key: string): any;
    key(index: number): string;
    removeItem(key: string): void;
    _setItem(key: string, data: string): void;
    setItem(key: string, data: string): void;
    constructor();
    create(url: string): Storage;
    onChange(changes: any): void;
    clear(): void;
    savedb(): void;
}
declare class WindowLocalStorage {
    localStorage: Storage;
}
declare class WindowSessionStorage {
    getItem(i: string): any;
    setItem(i: string, b: Object): void;
}
declare class _Cookie {
    key: string;
    value: string;
    domain: string;
    expires: Date;
    static cookies: Array<_Cookie>;
    static sreg: RegExp;
    constructor();
    static addCookie(s: string): _Cookie;
    static pushCookie(c: _Cookie): boolean;
    static flush(): void;
    static getString(type: string, joinstr: string): string;
    static toLocalString(): string;
    isValid(): boolean;
    static init(s: string): void;
    toLocalString(type: string): string;
}
declare class KeyboardEvent extends UIEvent {
    altKey: boolean;
    char: string;
    charCode: number;
    ctrlKey: boolean;
    key: string;
    keyCode: number;
    locale: string;
    location: number;
    metaKey: boolean;
    repeat: boolean;
    shiftKey: boolean;
    which: number;
    constructor(typeArg: string, eventInitDict?: KeyboardEventInit);
    initKeyboardEvent(typeArg: string, canBubbleArg: boolean, cancelableArg: boolean, viewArg: Window, keyArg: string, locationArg: number, modifiersListArg: string, repeat: boolean, locale: string): void;
    static DOM_KEY_LOCATION_JOYSTICK: number;
    static DOM_KEY_LOCATION_LEFT: number;
    static DOM_KEY_LOCATION_MOBILE: number;
    static DOM_KEY_LOCATION_NUMPAD: number;
    static DOM_KEY_LOCATION_RIGHT: number;
    static DOM_KEY_LOCATION_STANDARD: number;
}
declare var _lbKeyboardEvent: typeof KeyboardEvent;
interface NavigatorID {
    appName: string;
    appVersion: string;
    platform: string;
    product: string;
    productSub: string;
    userAgent: string;
    vendor: string;
    vendorSub: string;
}
interface NavigatorOnLine {
    onLine: boolean;
}
interface PositionError {
    code: number;
    message: string;
    toString(): string;
    PERMISSION_DENIED: number;
    POSITION_UNAVAILABLE: number;
    TIMEOUT: number;
}
interface PositionCallback {
    (position: Position): void;
}
interface PositionErrorCallback {
    (error: PositionError): void;
}
interface Coordinates {
    accuracy: number;
    altitude: number;
    altitudeAccuracy: number;
    heading: number;
    latitude: number;
    longitude: number;
    speed: number;
}
interface Position {
    coords: Coordinates;
    timestamp: number;
}
interface PositionOptions {
    enableHighAccuracy?: boolean;
    timeout?: number;
    maximumAge?: number;
}
interface Geolocation {
    clearWatch(watchId: number): void;
    getCurrentPosition(successCallback: PositionCallback, errorCallback?: PositionErrorCallback, options?: PositionOptions): void;
    watchPosition(successCallback: PositionCallback, errorCallback?: PositionErrorCallback, options?: PositionOptions): number;
}
interface NavigatorGeolocation {
    geolocation: Geolocation;
}
declare class Navigator implements NavigatorID, NavigatorOnLine, NavigatorGeolocation {
    appName: string;
    appVersion: string;
    platform: string;
    product: string;
    productSub: string;
    userAgent: string;
    vendor: string;
    vendorSub: string;
    sv: number;
    onLine: boolean;
    geolocation: Geolocation;
    appCodeName: string;
    appMinorVersion: string;
    browserLanguage: string;
    connectionSpeed: number;
    cookieEnabled: boolean;
    cpuClass: string;
    language: string;
    maxTouchPoints: number;
    msManipulationViewsEnabled: boolean;
    msMaxTouchPoints: number;
    msPointerEnabled: boolean;
    pointerEnabled: boolean;
    systemLanguage: string;
    userLanguage: string;
    webdriver: boolean;
    constructor();
    getGamepads(): Gamepad[];
    javaEnabled(): boolean;
    vibrate(pattern: number | number[]): boolean;
    addEventListener(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void;
}
declare class Node extends EventTarget {
    private _childs;
    private _zip;
    private _z;
    private static _nodeid;
    ownerDocument: Document;
    parentElement: HTMLElement;
    parentNode: Node;
    nodeType: number;
    constructor();
    firstChild: Node;
    childNodes: Node[];
    insertBefore(newChild: Node, oldChild: Node): void;
    appendChild(newChild: Node): Node;
    cloneNode(deep?: boolean): Node;
    removeChild(oldChild: Node): Node;
    protected smpJson(): string;
    getAncestorsNode(): Node[];
}
declare class NodeSelector {
    querySelector(selectors: string): Element;
    querySelectorAll(selectors: string): NodeListOf<Element>;
}
declare class ChildNode {
    constructor();
    remove(): void;
}
declare class ElementTraversal {
    constructor();
}
declare class Element extends Node implements GlobalEventHandlers, ElementTraversal, NodeSelector, ChildNode {
    tagName: string;
    _id: string;
    className: string;
    innerHTML: string;
    clientHeight: number;
    clientLeft: number;
    clientTop: number;
    clientWidth: number;
    __visible: boolean;
    id: string;
    private _attribs;
    querySelector: (selectors: string) => Element;
    querySelectorAll: (selectors: string) => NodeListOf<Element>;
    createEvent: (eventInterface: string) => Event;
    remove: () => void;
    onpointercancel: (ev: PointerEvent) => any;
    onpointerdown: (ev: PointerEvent) => any;
    onpointerenter: (ev: PointerEvent) => any;
    onpointerleave: (ev: PointerEvent) => any;
    onpointermove: (ev: PointerEvent) => any;
    onpointerout: (ev: PointerEvent) => any;
    onpointerover: (ev: PointerEvent) => any;
    onpointerup: (ev: PointerEvent) => any;
    onwheel: (ev: WheelEvent) => any;
    constructor();
    setAttribute(name: string, value: string): void;
    getAttribute(name: string): any;
}
interface WindowTimers {
    clearInterval(handle: number): void;
    clearTimeout(handle: number): void;
    setInterval(handler: any, timeout?: any): number;
    setTimeout(handler: any, timeout?: any): number;
}
declare namespace ns_Timer {
    class _WindowTimersExtension {
        clearImmediate(handle: number): void;
        setImmediate(expression: any): number;
    }
    class _WindowTimers extends _WindowTimersExtension implements WindowTimers {
        constructor();
        clearInterval(handle: number): void;
        clearTimeout(handle: number): void;
        setInterval(handler: any, timeout?: any): number;
        setTimeout(handler: any, timeout?: any): number;
    }
    function requestAnimationFrame(callback: FrameRequestCallback): number;
    function cancelAnimationFrame(handle: number): void;
}
declare var WindowTimers: typeof ns_Timer._WindowTimers;
declare var requestAnimationFrame: typeof ns_Timer.requestAnimationFrame;
declare var cancelAnimationFrame: typeof ns_Timer.cancelAnimationFrame;
declare class Location {
    private _nativeObj;
    private bk_setHref;
    private _host;
    private _fullpath;
    private _pathname;
    private _protocol;
    private _port;
    private _search;
    private _href;
    private _origin;
    static __urlCache__: Object;
    hash: string;
    hostname: string;
    host: string;
    fullpath: string;
    pathname: string;
    protocol: string;
    search: string;
    port: string;
    origin: string;
    href: string;
    constructor();
    setBaseHref(basehref: string): void;
    getBaseHref(): string;
    assign(url: string): void;
    reload(forcedReload?: boolean): void;
    replace(url: string): void;
    toString(): string;
    setHref: (url: string) => void;
    normalizeUrl(url: any): string;
    parseQuery(url: any): Object;
    resolve(fileName: string): string;
}
declare class ConchVirtualBitmap {
}
declare function applyMixins(derivedCtor: any, baseCtors: any[]): void;
declare class IDBEnvironment implements IDBEnvironment {
}
declare class _Window extends EventTarget {
    crypto: Crypto;
    onerror: ErrorEventHandler;
    Audio: HTMLAudioElement;
    private timer;
    clearInterval: (handle: number) => void;
    clearTimeout: (handle: number) => void;
    setInterval: (handler: any, timeout?: any) => number;
    setTimeout: (handler: any, timeout?: any) => number;
    constructor();
    document: Document;
}
declare class XMLHttpRequest extends EventTarget {
    private xhr;
    private _responseText;
    private _response;
    private _responseType;
    private _readyState;
    private _status;
    private _url;
    private _async;
    private _method;
    private _onloadcb;
    private _onrchgcb;
    private _head;
    onerror: (evt: Event) => any;
    withCredentials: boolean;
    constructor();
    setRequestHeader(name: string, value: string): void;
    getAllResponseHeaders(): any;
    responseText: string;
    response: any;
    responseType: string;
    url: string;
    async: boolean;
    readyState: number;
    status: number;
    private _loadsus();
    onreadystatechange: (evt: Event) => any;
    onload: ((evt: Event) => any);
    getResponseHeader(): any;
    setResponseHeader: (name: any, value: any) => void;
    open(type: string, url: string, async: boolean): void;
    overrideMimeType(mime: string): void;
    send(body: any): void;
}
interface ImageData {
}
declare class ColorUtil {
    static _COLOR_MAP: Object;
    static getColorNum(str: any): number;
    static RGBReg: RegExp;
    static HSLReg: RegExp;
    static getColNum(c: string, isRgb: boolean): number;
    static getRGBByRGBStr(str: string): string;
    static toHexColor(color: number): string;
    static hsl2rgb(h: number, s: number, l: number): Array<number>;
}
declare class ContextDataInfo {
    curpos: number;
    buffer: DataView;
    arrayBuffer: Uint8Array;
    constructor();
    private ru16(i);
    private wu16(i, v);
    private ru32(i);
    private wu32(i, v);
    private rf32(i);
    private wf32(i, v);
    fillStyle: number;
    storkeStyle: number;
    lineWidth: number;
    textBaseLineType: number;
    textAlignType: number;
    blendType: number;
    repetition: number;
    shadowColor: number;
    shadowBlur: number;
    shadowOffsetX: number;
    shadowOffsetY: number;
    lineCap: number;
    lineJoin: number;
    miterLimit: number;
    alpha: number;
    font: string;
    fillType: number;
    storkeType: number;
    x: number;
    y: number;
    w: number;
    h: number;
    init(): void;
}
declare class ContextSaveData {
    curData: ContextDataInfo;
    datasize: number;
    sizeofData: number;
    constructor();
    save(): boolean;
    restore(): boolean;
}
declare class CanvasPattern {
    x: number;
    y: number;
    w: number;
    h: number;
    image: number;
    repetition: number;
    constructor(img?: number, r?: number, x?: number, y?: number, w?: number, h?: number);
}
declare class CanvasGradient {
    x0: number;
    y0: number;
    x1: number;
    y1: number;
    addColorStop(fs: string, color: string): void;
}
declare class CanvasRenderingContext2D implements IBuffer {
    private canvas;
    static LINECAP: any;
    static TEXTALIGN: any;
    static TEXTBASELINE: any;
    static LINEJOIN: any;
    static BLEND: any;
    static PATTERN: any;
    private _saveData;
    private _nativeObj;
    id: number;
    constructor();
    setSize(w: number, h: number): void;
    setCanvasType(type: number): void;
    setCanvasClip(b: boolean): void;
    save(): void;
    restore(): void;
    beginPath(): void;
    drawImage(image: HTMLImageElement | HTMLCanvasElement, offsetX: number, offsetY: number, width?: number, height?: number, canvasOffsetX?: number, canvasOffsetY?: number, canvasImageWidth?: number, canvasImageHeight?: number): void;
    rect(x: number, y: number, w: number, h: number): void;
    clip(fillRule?: string): void;
    scale(x: number, y: number): void;
    rotate(angle: number): void;
    translate(x: number, y: number): void;
    transform(m11: number, m12: number, m21: number, m22: number, dx: number, dy: number): void;
    setTransform(m11: number, m12: number, m21: number, m22: number, dx: number, dy: number): void;
    globalAlpha: number;
    clearRect(x: number, y: number, w: number, h: number): void;
    closePath(): void;
    fillStyle: string | CanvasGradient | CanvasPattern;
    fillRect(x: number, y: number, w: number, h: number): void;
    strokeStyle: string | CanvasGradient | CanvasPattern;
    stroke(): void;
    strokeRect(x: number, y: number, w: number, h: number): void;
    private _drawText(fid, text, x, y, maxWidth?);
    strokeText(text: string, x: number, y: number, maxWidth?: number): void;
    fillText(text: string, x: number, y: number, maxWidth?: number): void;
    fill(fillRule?: string): void;
    lineCap: string;
    lineWidth: number;
    lineJoin: string;
    moveTo(x: number, y: number): void;
    lineTo(x: number, y: number): void;
    arc(x: number, y: number, radius: number, startAngle: number, endAngle: number, anticlockwise?: boolean): void;
    arcTo(x1: number, y1: number, x2: number, y2: number, radius: number): void;
    bezierCurveTo(cp1x: number, cp1y: number, cp2x: number, cp2y: number, x: number, y: number): void;
    quadraticCurveTo(cpx: number, cpy: number, x: number, y: number): void;
    font: string;
    globalCompositeOperation: string;
    textBaseline: string;
    textAlign: string;
    miterLimit: number;
    shadowOffsetX: number;
    shadowOffsetY: number;
    shadowColor: string;
    shadowBlur: number;
    setFilter(r: number, g: number, b: number, gray: number): void;
    fillBorderText(text: string, x: number, y: number, font: string, fillColor: string, borderColor: string, lineWidth: number, textAlign: string): void;
    clipRect(x: number, y: number, w: number, h: number): void;
    createLinearGradient(x0: number, y0: number, x1: number, y1: number): CanvasGradient;
    clear(): void;
    _setIBVB(x: number, y: number, ib: any, vb: any, numElement: number, mat: {
        a: number;
        b: number;
        c: number;
        d: number;
        tx: number;
        ty: number;
    }, shader: any, shaderValues: any, startIndex: number, offset: number): void;
    setFilterMatrix(mat: Float32Array, alpha: Float32Array): void;
    isPointInPath(x: number, y: number, fillRule?: string): boolean;
    measureText(text: string): TextMetrics;
    disableClip(b: boolean): void;
    createPattern(image: HTMLImageElement, repetition: string, x?: number, y?: number, w?: number, h?: number): CanvasPattern;
    lineDashOffset: number;
    createImageData(imageDataOrSw: number | ImageData, sh?: number): ImageData;
    createRadialGradient(x0: number, y0: number, r0: number, x1: number, y1: number, r1: number): CanvasGradient;
    getImageData(sx: number, sy: number, sw: number, sh: number): ImageData;
    getLineDash(): number[];
    putImageData(imagedata: ImageData, dx: number, dy: number, dirtyX?: number, dirtyY?: number, dirtyWidth?: number, dirtyHeight?: number): void;
    setLineDash(segments: number[]): void;
}
declare function _init(v: Array<string>): void;
interface WebGLContextAttributes {
    alpha?: boolean;
    depth?: boolean;
    stencil?: boolean;
    antialias?: boolean;
    premultipliedAlpha?: boolean;
    preserveDrawingBuffer?: boolean;
}
declare class ProgramLocationTable {
    FADELOC_BASE: number;
    _obj: Object;
    getFadeLocation(fadeProgramID: number, name: string): number;
}
declare class WebGLRenderingContext {
    private _nativeObj;
    private id;
    drawingBufferWidth: number;
    drawingBufferHeight: number;
    currentTextureID: number;
    private viewportx;
    private viewporty;
    private viewportw;
    private viewporth;
    private _i;
    __priv_canvas: HTMLCanvasElement;
    constructor();
    private createFadeID();
    getContextAttributes(): WebGLContextAttributes;
    onCssTransform(): void;
    width: number;
    height: number;
    _locTable: ProgramLocationTable;
    isContextLost(): boolean;
    getShaderPrecisionFormat(a: number, b: number): any;
    getExtension(name: string): void;
    createBuffer(): number;
    bindBuffer(target: number, buffer: number): void;
    flush(): void;
    bufferData(target: number, sizeOrArray: number | ArrayBuffer, usage: number): void;
    createShader(type: any): number;
    shaderSource(shader: any, source: any): void;
    compileShader(shader: any): void;
    getShaderParameter(shader: any, pname: any): boolean;
    createProgram(): number;
    attachShader(program: any, shader: any): void;
    linkProgram(program: any): void;
    getProgramParameter(program: any, pname: any): boolean;
    useProgram(program: any): void;
    getUniformLocation(program: any, name: any): number;
    getAttribLocation(program: any, name: any): number;
    disable(cap: any): void;
    enable(cap: any): void;
    colorMask(red: any, green: any, blue: any, alpha: any): void;
    viewport(x: number, y: number, width: number, height: number): void;
    clear(mask: number): void;
    clearColor(red: number, green: number, blue: number, alpha: number): void;
    createTexture(invalidCallback: any, restoreCallback: any): number;
    bindTexture(target: any, texture: any): void;
    pixelStorei(pname: any, param: any): void;
    texImage2D(target: any, level: any, internalformat: any, width: any, height: any, border: any, format: any, type: any, pixels: any): void;
    texSubImage2D(target: any, level: any, xoffset: any, yoffset: any, format: any, type: any, source: any): void;
    texParameterf(target: any, pname: any, param: any): void;
    texParameteri(target: any, pname: any, param: any): void;
    activeTexture(texture: any): void;
    getShaderInfoLog(shader: any): string;
    uniform1f(location: any, x: any): void;
    uniform1i(location: any, x: any): void;
    uniform2f(location: any, x: any, y: any): void;
    uniform2i(location: any, x: any, y: any): void;
    uniform3f(location: any, x: any, y: any, z: any): void;
    uniform3i(location: any, x: any, y: any, z: any): void;
    uniform4f(location: any, x: any, y: any, z: any, w: any): void;
    uniform4i(location: any, x: any, y: any, z: any, w: any): void;
    uniform1fv(location: any, v: any): void;
    uniform1iv(location: any, v: any): void;
    uniform2fv(location: any, v: any): void;
    uniform2iv(location: any, v: any): void;
    uniform3fv(location: any, v: any): void;
    uniform3iv(location: any, v: any): void;
    uniform4fv(location: any, v: any): void;
    uniform4iv(location: any, v: any): void;
    uniformxv(cmd: any, location: any, v: any): void;
    _uniformMatrixfv_x(cmd: any, location: any, transpose: any, values: any, number: any): void;
    uniformMatrix2fv(location: any, transpose: any, values: any, number: any): void;
    uniformMatrix3fv(location: any, transpose: any, values: any, number: any): void;
    uniformMatrix4fv(location: any, transpose: any, values: any, number: any): void;
    vertexAttribPointer(index: any, size: any, type: any, normalized: any, stride: any, offset: any): void;
    enableVertexAttribArray(index: any): void;
    blendFunc(sfactor: any, dfactor: any): void;
    bufferSubData(target: number, offset: number, data: ArrayBufferView): void;
    drawElements(mode: any, count: any, type: any, offset: any): void;
    deleteTexture(texture: any): void;
    drawArrays(mode: any, first: any, count: any): void;
    bindAttribLocation(program: any, index: any, name: any): void;
    cullFace(mode: any): void;
    createFramebuffer(): number;
    getParameter(pname: any): number;
    bindFramebuffer(target: any, framebuffer: any): void;
    depthFunc(func: any): void;
    depthMask(flag: any): void;
    depthRange(zNear: any, zFar: any): void;
    stencilFunc(func: any, ref: any, mask: any): void;
    stencilMask(mask: any): void;
    stencilOp(fail: any, zfail: any, zpass: any): void;
    deleteBuffer(buffer: any): void;
    deleteFramebuffer(framebuffer: any): void;
    deleteProgram(program: any): void;
    deleteRenderbuffer(renderbuffer: any): void;
    deleteShader(shader: any): void;
    isBuffer(buffer: any): boolean;
    disableVertexAttribArray(index: any): void;
    scissor(x: any, y: any, width: any, height: any): void;
    generateMipmap(target: any): void;
    framebufferTexture2D(target: any, attachment: any, textarget: any, texture: any, level: any): void;
    checkFramebufferStatus(target: any): number;
    createRenderbuffer(): number;
    bindRenderbuffer(target: number, buffer: number): void;
    renderbufferStorage(target: number, internalformat: number, width: number, height: number): void;
    framebufferRenderbuffer(target: any, attachment: any, renderbuffertarget: any, buffer: any): void;
    blendFuncSeparate(srcRGB: any, dstRGB: any, srcAlpha: any, dstAlpha: any): void;
    DEPTH_BUFFER_BIT: number;
    STENCIL_BUFFER_BIT: number;
    COLOR_BUFFER_BIT: number;
    POINTS: number;
    LINES: number;
    LINE_LOOP: number;
    LINE_STRIP: number;
    TRIANGLES: number;
    TRIANGLE_STRIP: number;
    TRIANGLE_FAN: number;
    ZERO: number;
    ONE: number;
    SRC_COLOR: number;
    ONE_MINUS_SRC_COLOR: number;
    SRC_ALPHA: number;
    ONE_MINUS_SRC_ALPHA: number;
    DST_ALPHA: number;
    ONE_MINUS_DST_ALPHA: number;
    DST_COLOR: number;
    ONE_MINUS_DST_COLOR: number;
    SRC_ALPHA_SATURATE: number;
    FUNC_ADD: number;
    BLEND_EQUATION: number;
    BLEND_EQUATION_RGB: number;
    BLEND_EQUATION_ALPHA: number;
    FUNC_SUBTRACT: number;
    FUNC_REVERSE_SUBTRACT: number;
    BLEND_DST_RGB: number;
    BLEND_SRC_RGB: number;
    BLEND_DST_ALPHA: number;
    BLEND_SRC_ALPHA: number;
    CONSTANT_COLOR: number;
    ONE_MINUS_CONSTANT_COLOR: number;
    CONSTANT_ALPHA: number;
    ONE_MINUS_CONSTANT_ALPHA: number;
    BLEND_COLOR: number;
    ARRAY_BUFFER: number;
    ELEMENT_ARRAY_BUFFER: number;
    ARRAY_BUFFER_BINDING: number;
    ELEMENT_ARRAY_BUFFER_BINDING: number;
    STREAM_DRAW: number;
    STATIC_DRAW: number;
    DYNAMIC_DRAW: number;
    BUFFER_SIZE: number;
    BUFFER_USAGE: number;
    CURRENT_VERTEX_ATTRIB: number;
    FRONT: number;
    BACK: number;
    FRONT_AND_BACK: number;
    CULL_FACE: number;
    BLEND: number;
    DITHER: number;
    STENCIL_TEST: number;
    DEPTH_TEST: number;
    SCISSOR_TEST: number;
    POLYGON_OFFSET_FILL: number;
    SAMPLE_ALPHA_TO_COVERAGE: number;
    SAMPLE_COVERAGE: number;
    INVALID_ENUM: number;
    INVALID_VALUE: number;
    INVALID_OPERATION: number;
    OUT_OF_MEMORY: number;
    CW: number;
    CCW: number;
    LINE_WIDTH: number;
    ALIASED_POINT_SIZE_RANGE: number;
    ALIASED_LINE_WIDTH_RANGE: number;
    CULL_FACE_MODE: number;
    FRONT_FACE: number;
    DEPTH_RANGE: number;
    DEPTH_WRITEMASK: number;
    DEPTH_CLEAR_VALUE: number;
    DEPTH_FUNC: number;
    STENCIL_CLEAR_VALUE: number;
    STENCIL_FUNC: number;
    STENCIL_FAIL: number;
    STENCIL_PASS_DEPTH_FAIL: number;
    STENCIL_PASS_DEPTH_PASS: number;
    STENCIL_REF: number;
    STENCIL_VALUE_MASK: number;
    STENCIL_WRITEMASK: number;
    STENCIL_BACK_FUNC: number;
    STENCIL_BACK_FAIL: number;
    STENCIL_BACK_PASS_DEPTH_FAIL: number;
    STENCIL_BACK_PASS_DEPTH_PASS: number;
    STENCIL_BACK_REF: number;
    STENCIL_BACK_VALUE_MASK: number;
    STENCIL_BACK_WRITEMASK: number;
    VIEWPORT: number;
    SCISSOR_BOX: number;
    COLOR_CLEAR_VALUE: number;
    COLOR_WRITEMASK: number;
    UNPACK_ALIGNMENT: number;
    PACK_ALIGNMENT: number;
    MAX_TEXTURE_SIZE: number;
    MAX_VIEWPORT_DIMS: number;
    SUBPIXEL_BITS: number;
    RED_BITS: number;
    GREEN_BITS: number;
    BLUE_BITS: number;
    ALPHA_BITS: number;
    DEPTH_BITS: number;
    STENCIL_BITS: number;
    POLYGON_OFFSET_UNITS: number;
    POLYGON_OFFSET_FACTOR: number;
    TEXTURE_BINDING_2D: number;
    SAMPLE_BUFFERS: number;
    SAMPLES: number;
    SAMPLE_COVERAGE_VALUE: number;
    SAMPLE_COVERAGE_INVERT: number;
    COMPRESSED_TEXTURE_FORMATS: number;
    DONT_CARE: number;
    FASTEST: number;
    NICEST: number;
    GENERATE_MIPMAP_HINT: number;
    BYTE: number;
    UNSIGNED_BYTE: number;
    SHORT: number;
    UNSIGNED_SHORT: number;
    INT: number;
    UNSIGNED_INT: number;
    FLOAT: number;
    DEPTH_COMPONENT: number;
    ALPHA: number;
    RGB: number;
    RGBA: number;
    LUMINANCE: number;
    LUMINANCE_ALPHA: number;
    UNSIGNED_SHORT_4_4_4_4: number;
    UNSIGNED_SHORT_5_5_5_1: number;
    UNSIGNED_SHORT_5_6_5: number;
    FRAGMENT_SHADER: number;
    VERTEX_SHADER: number;
    MAX_VERTEX_ATTRIBS: number;
    MAX_VERTEX_UNIFORM_VECTORS: number;
    MAX_VARYING_VECTORS: number;
    MAX_COMBINED_TEXTURE_IMAGE_UNITS: number;
    MAX_VERTEX_TEXTURE_IMAGE_UNITS: number;
    MAX_TEXTURE_IMAGE_UNITS: number;
    MAX_FRAGMENT_UNIFORM_VECTORS: number;
    SHADER_TYPE: number;
    DELETE_STATUS: number;
    LINK_STATUS: number;
    VALIDATE_STATUS: number;
    ATTACHED_SHADERS: number;
    ACTIVE_UNIFORMS: number;
    ACTIVE_ATTRIBUTES: number;
    SHADING_LANGUAGE_VERSION: number;
    CURRENT_PROGRAM: number;
    NEVER: number;
    LESS: number;
    EQUAL: number;
    LEQUAL: number;
    GREATER: number;
    NOTEQUAL: number;
    GEQUAL: number;
    ALWAYS: number;
    KEEP: number;
    REPLACE: number;
    INCR: number;
    DECR: number;
    INVERT: number;
    INCR_WRAP: number;
    DECR_WRAP: number;
    VENDOR: number;
    RENDERER: number;
    VERSION: number;
    NEAREST: number;
    LINEAR: number;
    NEAREST_MIPMAP_NEAREST: number;
    LINEAR_MIPMAP_NEAREST: number;
    NEAREST_MIPMAP_LINEAR: number;
    LINEAR_MIPMAP_LINEAR: number;
    TEXTURE_MAG_FILTER: number;
    TEXTURE_MIN_FILTER: number;
    TEXTURE_WRAP_S: number;
    TEXTURE_WRAP_T: number;
    TEXTURE_2D: number;
    TEXTURE: number;
    TEXTURE_CUBE_MAP: number;
    TEXTURE_BINDING_CUBE_MAP: number;
    TEXTURE_CUBE_MAP_POSITIVE_X: number;
    TEXTURE_CUBE_MAP_NEGATIVE_X: number;
    TEXTURE_CUBE_MAP_POSITIVE_Y: number;
    TEXTURE_CUBE_MAP_NEGATIVE_Y: number;
    TEXTURE_CUBE_MAP_POSITIVE_Z: number;
    TEXTURE_CUBE_MAP_NEGATIVE_Z: number;
    MAX_CUBE_MAP_TEXTURE_SIZE: number;
    TEXTURE0: number;
    TEXTURE1: number;
    TEXTURE2: number;
    TEXTURE3: number;
    TEXTURE4: number;
    TEXTURE5: number;
    TEXTURE6: number;
    TEXTURE7: number;
    TEXTURE8: number;
    TEXTURE9: number;
    TEXTURE10: number;
    TEXTURE11: number;
    TEXTURE12: number;
    TEXTURE13: number;
    TEXTURE14: number;
    TEXTURE15: number;
    TEXTURE16: number;
    TEXTURE17: number;
    TEXTURE18: number;
    TEXTURE19: number;
    TEXTURE20: number;
    TEXTURE21: number;
    TEXTURE22: number;
    TEXTURE23: number;
    TEXTURE24: number;
    TEXTURE25: number;
    TEXTURE26: number;
    TEXTURE27: number;
    TEXTURE28: number;
    TEXTURE29: number;
    TEXTURE30: number;
    TEXTURE31: number;
    ACTIVE_TEXTURE: number;
    REPEAT: number;
    CLAMP_TO_EDGE: number;
    MIRRORED_REPEAT: number;
    FLOAT_VEC2: number;
    FLOAT_VEC3: number;
    FLOAT_VEC4: number;
    INT_VEC2: number;
    INT_VEC3: number;
    INT_VEC4: number;
    BOOL: number;
    BOOL_VEC2: number;
    BOOL_VEC3: number;
    BOOL_VEC4: number;
    FLOAT_MAT2: number;
    FLOAT_MAT3: number;
    FLOAT_MAT4: number;
    SAMPLER_2D: number;
    SAMPLER_CUBE: number;
    VERTEX_ATTRIB_ARRAY_ENABLED: number;
    VERTEX_ATTRIB_ARRAY_SIZE: number;
    VERTEX_ATTRIB_ARRAY_STRIDE: number;
    VERTEX_ATTRIB_ARRAY_TYPE: number;
    VERTEX_ATTRIB_ARRAY_NORMALIZED: number;
    VERTEX_ATTRIB_ARRAY_POINTER: number;
    VERTEX_ATTRIB_ARRAY_BUFFER_BINDING: number;
    IMPLEMENTATION_COLOR_READ_TYPE: number;
    IMPLEMENTATION_COLOR_READ_FORMAT: number;
    COMPILE_STATUS: number;
    LOW_FLOAT: number;
    MEDIUM_FLOAT: number;
    HIGH_FLOAT: number;
    LOW_INT: number;
    MEDIUM_INT: number;
    HIGH_INT: number;
    FRAMEBUFFER: number;
    RENDERBUFFER: number;
    RGBA4: number;
    RGB5_A1: number;
    RGB565: number;
    DEPTH_COMPONENT16: number;
    STENCIL_INDEX: number;
    STENCIL_INDEX8: number;
    DEPTH_STENCIL: number;
    RENDERBUFFER_WIDTH: number;
    RENDERBUFFER_HEIGHT: number;
    RENDERBUFFER_INTERNAL_FORMAT: number;
    RENDERBUFFER_RED_SIZE: number;
    RENDERBUFFER_GREEN_SIZE: number;
    RENDERBUFFER_BLUE_SIZE: number;
    RENDERBUFFER_ALPHA_SIZE: number;
    RENDERBUFFER_DEPTH_SIZE: number;
    RENDERBUFFER_STENCIL_SIZE: number;
    FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE: number;
    FRAMEBUFFER_ATTACHMENT_OBJECT_NAME: number;
    FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL: number;
    FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE: number;
    COLOR_ATTACHMENT0: number;
    DEPTH_ATTACHMENT: number;
    STENCIL_ATTACHMENT: number;
    DEPTH_STENCIL_ATTACHMENT: number;
    NONE: number;
    FRAMEBUFFER_COMPLETE: number;
    FRAMEBUFFER_INCOMPLETE_ATTACHMENT: number;
    FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: number;
    FRAMEBUFFER_INCOMPLETE_DIMENSIONS: number;
    FRAMEBUFFER_UNSUPPORTED: number;
    FRAMEBUFFER_BINDING: number;
    RENDERBUFFER_BINDING: number;
    MAX_RENDERBUFFER_SIZE: number;
    INVALID_FRAMEBUFFER_OPERATION: number;
    UNPACK_FLIP_Y_WEBGL: number;
    UNPACK_PREMULTIPLY_ALPHA_WEBGL: number;
    CONTEXT_LOST_WEBGL: number;
    UNPACK_COLORSPACE_CONVERSION_WEBGL: number;
    BROWSER_DEFAULT_WEBGL: number;
}
declare class CSSStyleDeclaration {
    _transform: Float32Array;
    _styleNumber: number;
    _htmlEle: HTMLElement;
    constructor();
    length: number;
    getPropertyPriority(propertyName: string): string;
    getPropertyValue(propertyName: string): string;
    item(index: number): string;
    removeProperty(propertyName: string): string;
    setProperty(propertyName: string, value: string, priority?: string): void;
    left: string;
    top: string;
    height: string;
    width: string;
    transform: string;
}
declare class HTMLElement extends Element {
    style: CSSStyleDeclaration;
    onCSS_Transform(mat: Float32Array): void;
    constructor();
    onerror(ev: Event): any;
    onload(ev: Event): any;
}
interface NativeObjWrapper {
    _nativeObj: any;
}
declare class HTMLDivElement extends HTMLElement {
    align: string;
    noWrap: boolean;
    constructor();
}
declare class HTMLScriptElement extends HTMLElement {
    private _src;
    private _text;
    constructor();
    src: string;
    text: string;
}
interface HTMLCanvasElement {
    getContext(contextId: '2d'): CanvasRenderingContext2D;
    getContext(contextId: 'webgl'): WebGLRenderingContext;
    getContext(contextId: string): CanvasRenderingContext2D | WebGLRenderingContext;
}
declare class HTMLCanvasElement extends HTMLElement {
    static _isFirstCanv: boolean;
    _isFirst: boolean;
    _tranform: Float32Array;
    _hasTransform: boolean;
    static RS: boolean;
    static rootContext: CanvasRenderingContext2D;
    _ctx2d: CanvasRenderingContext2D;
    _w: number;
    _h: number;
    private _width;
    private _height;
    private _clientRect;
    constructor();
    getBoundingClientRect(): {
        left: number;
        top: number;
        width: number;
        height: number;
        right: number;
        bottom: number;
    };
    onCSS_Transform(mat: Float32Array): void;
    toDataURL(type?: string): string;
    setSize(w: number, h: number): void;
    width: number;
    height: number;
    clientWidth: number;
    clientHeight: number;
}
declare class HTMLImageElement extends HTMLElement implements NativeObjWrapper {
    _nativeObj: any;
    complete: boolean;
    private srcs;
    private obj;
    constructor();
    src: string;
    private _nativeOnload();
    private _nativeOnError(e);
    width: number;
    height: number;
    imgId: number;
}
declare class HTMLInputElement extends HTMLElement implements NativeObjWrapper {
    _nativeObj: ConchInput;
    setLeft: (l: number) => void;
    setTop: (t: number) => void;
    setWidth: (w: number) => void;
    setHeight: (h: number) => void;
    setOpacity: (o: number) => void;
    setValue: (v: string) => void;
    getValue: () => string;
    setStyle: (s: string) => void;
    setVisible: (b: boolean) => void;
    focus: () => void;
    blur: () => void;
    setColor: (c: string) => void;
    setFontSize: (s: number) => void;
    setPos: (x: number, y: number) => void;
    setSize: (w: number, h: number) => void;
    setCursorPosition: (p: number) => void;
    setScale: (w: number, h: number) => void;
    setMaxLength: (l: number) => void;
    setType: (t: string) => void;
    setNumberOnly: (b: boolean) => void;
    setRegular: (r: string) => void;
    setFont: (f: string) => void;
    setMultiAble: (b: boolean) => void;
    constructor();
    maxLength: number;
    left: number;
    top: number;
    width: number;
    height: number;
    opacity: number;
    clientLeft: number;
    clientTop: number;
    clientWidth: number;
    clientHeight: number;
    value: string;
    visible: boolean;
    _addEventListener(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void;
}
declare class HTMLMediaElement extends HTMLElement {
}
declare class HTMLMetaElement extends HTMLElement {
    private _name;
    charset: string;
    content: string;
    httpEquiv: string;
    name: string;
    scheme: string;
    url: string;
    constructor();
}
declare class HTMLAudioElement extends HTMLMediaElement implements NativeObjWrapper {
    _nativeObj: ConchAudio;
    constructor();
    readyState: number;
    load(): void;
    setLoop(loop: any): void;
    play(): void;
    pause(): void;
    stop(): void;
    autoplay: boolean;
    loop: boolean;
    muted: boolean;
    src: string;
    volume: number;
    _addEventListener(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void;
}
declare class HTMLBodyElement extends HTMLElement {
    constructor();
    clientHeight: number;
    clientWidth: number;
}
declare class NodeList extends Array {
    constructor();
    item(index: number): Node;
}
interface NodeListOf<TNode extends Node> extends NodeList {
    length: number;
    item(index: number): TNode;
    [index: number]: TNode;
}
interface Document {
    createElement(tagName: "div"): HTMLDivElement;
    createElement(tagName: "img"): HTMLImageElement;
    createElement(tagName: "virtualBitmap"): any;
    createElement(tagName: "audio"): HTMLAudioElement;
    createElement(tagName: "input"): HTMLInputElement;
    createElement(tagName: "textarea"): HTMLInputElement;
    createElement(tagName: "canvas"): HTMLCanvasElement;
    createElement(tagName: "script"): HTMLScriptElement;
    createElement(tagName: "meta"): HTMLMetaElement;
    createElement(tagName: string): HTMLElement | any;
}
declare class HTMLHeadElement extends HTMLElement {
    profile: string;
    constructor();
}
declare class HTMLCollection extends Array {
    length: number;
    item(nameOrIndex?: any, optionalIndex?: any): Element;
    namedItem(name: string): Element;
}
declare class HTMLVideoElement extends HTMLDivElement {
    constructor();
    canPlayType(): boolean;
}
declare class Document extends Node implements GlobalEventHandlers, NodeSelector, DocumentEvent {
    private createMap;
    private _frameEndEvt;
    _cookiePath: string;
    _topElement: HTMLElement;
    body: HTMLElement;
    documentElement: HTMLElement;
    head: HTMLHeadElement;
    location: Location;
    defaultView: Window;
    scriptTextList: Array<any>;
    private _loading;
    private _evalNum;
    private _eventPathCache;
    all: HTMLCollection;
    _elements: Array<any>;
    querySelector: (selectors: string) => Element;
    querySelectorAll: (selectors: string) => NodeListOf<Element>;
    createEvent: (eventInterface: string) => Event;
    onpointercancel: (ev: PointerEvent) => any;
    onpointerdown: (ev: PointerEvent) => any;
    onpointerenter: (ev: PointerEvent) => any;
    onpointerleave: (ev: PointerEvent) => any;
    onpointermove: (ev: PointerEvent) => any;
    onpointerout: (ev: PointerEvent) => any;
    onpointerover: (ev: PointerEvent) => any;
    onpointerup: (ev: PointerEvent) => any;
    constructor();
    uploadScript(d: any): void;
    private _downloadOk(d);
    pickElement(screenx: number, screeny: number): HTMLElement;
    createElementNS(tagName: string): HTMLElement;
    private create_div();
    private create_img();
    private create_canvas();
    private create_vbitmap();
    private create_audio();
    private create_input();
    private create_video();
    private create_script();
    private create_meta();
    createDocumentFragment(): HTMLDivElement;
    onframeend(): void;
    _dispatchEvent(evt: Event): boolean;
    cookie: string;
    loadCookie(): boolean;
    open(url?: string, name?: string, features?: string, replace?: boolean): Document;
    getElementsByTagName(tagname: string): NodeListOf<Element>;
    onkeydown(ev: KeyboardEvent): any;
    onkeypress(ev: KeyboardEvent): any;
    onkeyup(ev: KeyboardEvent): any;
    onmousedown(ev: MouseEvent): any;
    onmousemove(ev: MouseEvent): any;
    onmouseout(ev: MouseEvent): any;
    onmouseover(ev: MouseEvent): any;
    onmouseup(ev: MouseEvent): any;
    onmousewheel(ev: MouseWheelEvent): any;
    ontouchcancel(ev: TouchEvent): any;
    ontouchend(ev: TouchEvent): any;
    ontouchmove(ev: TouchEvent): any;
    ontouchstart(ev: TouchEvent): any;
    getElementById(elementId: string): HTMLElement;
    getElementsByClassName(classNames: string): NodeListOf<Element>;
    getElementsByName(name: string): Array<any>;
}
declare class CloseEvent extends Event {
    code: number;
    reason: string;
    wasClean: boolean;
    constructor();
    initCloseEvent(typeArg: string, canBubbleArg: boolean, cancelableArg: boolean, wasCleanArg: boolean, codeArg: number, reasonArg: string): void;
}
interface ErrorEvent extends Event {
    colno: number;
    error: any;
    filename: string;
    lineno: number;
    message: string;
    initErrorEvent(typeArg: string, canBubbleArg: boolean, cancelableArg: boolean, messageArg: string, filenameArg: string, linenoArg: number): void;
}
interface MessagePort extends EventTarget {
    onmessage: (ev: MessageEvent) => any;
    close(): void;
    postMessage(message?: any, ports?: any): void;
    start(): void;
    addEventListener(type: "message", listener: (ev: MessageEvent) => any, useCapture?: boolean): void;
    addEventListener(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void;
}
interface MessageEventInit extends EventInit {
    data?: any;
    origin?: string;
    lastEventId?: string;
    channel?: string;
    source?: any;
    ports?: MessagePort[];
}
declare class MessageEvent extends Event {
    data: any;
    origin: string;
    ports: any;
    source: Window;
    constructor(type: string, eventInitDict?: MessageEventInit);
    initMessageEvent(typeArg: string, canBubbleArg: boolean, cancelableArg: boolean, dataArg: any, originArg: string, lastEventIdArg: string, sourceArg: Window): void;
}
interface WebSocket {
}
declare class WebSocket extends EventTarget {
    private _nativeObj;
    onmessage: (data: MessageEvent) => void;
    constructor(url: string);
    binaryType: string;
    timegap: number;
    onopen: (e) => void;
    onclose: () => void;
    onerror: () => void;
    close(): void;
    send(msg: string | ArrayBuffer): void;
    readyState: number;
    static CLOSED: number;
    static CLOSING: number;
    static CONNECTING: number;
    static OPEN: number;
}
interface Socket {
    addEventListener(type: "close", listener: (ev: CloseEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "error", listener: (ev: ErrorEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "message", listener: (ev: MessageEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "open", listener: (ev: Event) => any, useCapture?: boolean): void;
    addEventListener(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void;
}
declare class Socket extends EventTarget {
    private _nativeObj;
    onmessage: (data: MessageEvent) => void;
    constructor(url: string);
    binaryType: string;
    timegap: number;
    onopen: (e) => void;
    onclose: () => void;
    onerror: () => void;
    close(): void;
    send(msg: string | ArrayBuffer): void;
    readyState: number;
    static CLOSED: number;
    static CLOSING: number;
    static CONNECTING: number;
    static OPEN: number;
}
declare class DOMParser {
    private _parser;
    private _src;
    private _onload;
    private _onerror;
    private _result;
    constructor();
    src: string;
    onload: any;
    private nativeObjOnload();
    onerror: any;
    private nativeObjOnerror();
    private static initXMl(xml);
    parseFromString(s: string, t: string): _jsXmlDocument;
    getResult(): _jsXmlDocument;
}
declare class _jsXmlAttr {
    nodeName: string;
    nodeValue: string;
    textContent: string;
    constructor(key: string, value: string);
    value: string;
}
declare class _jsXmlNode extends _jsXmlAttr {
    childNodes: Array<_jsXmlNode>;
    attributes: Array<_jsXmlAttr>;
    nodeType: number;
    firstChild: _jsXmlNode;
    constructor();
    getElementsByTagName(name: string): Array<_jsXmlNode>;
    getAttribute(name: string): string;
}
declare class _jsXmlDocument extends _jsXmlNode {
}
interface ErrorEventHandler {
    (message: string, filename?: string, lineno?: number, colno?: number, error?: Error): void;
}
interface LayaBox {
    devinfo: {
        resolution: string;
        mac: string;
        imei: string;
        imsi: string;
        os: string;
        osversion: string;
        phonemodel: string;
    };
    getDeviceInfo(): any;
}
interface Window extends EventTarget, WindowTimers {
    layaHtml5: any;
    screen: Screen;
    SetupWebglContext(): any;
    downloadfile(url: string, force: boolean, onok: (data: string) => void, onerr: () => void): any;
    loadLocalStorage(url: string): Storage;
    XMLHttpRequest: typeof XMLHttpRequest;
    innerHeight: number;
    innerWidth: number;
    localStorage: Storage;
    sessionStorage: WindowSessionStorage;
    location: Location;
    document: Document;
    Audio: typeof HTMLAudioElement;
    crypto: Crypto;
    console: Console;
    pageXOffset: number;
    pageYOffset: number;
    Image: typeof HTMLImageElement;
    layabox: LayaBox;
    layaDoc: any;
    ConchVirtualBitmap: any;
    alert: (message?: any) => void;
    onerror: (message: string, filename?: string, lineno?: number, colno?: number, error?: Error) => void;
    onload: (ev: Event) => any;
    open(url?: string, target?: string, features?: string, replace?: boolean): Window;
    onresize: (ev: UIEvent) => any;
    requestAnimationFrame(callback: FrameRequestCallback): number;
    cancelAnimationFrame(handle: number): void;
    process: any;
    Socket: typeof Socket;
    WebSocket: typeof WebSocket;
    parent: Window;
    frames: Window;
    eval(s: any): any;
    focus(): void;
    navigator: Navigator;
    devicePixelRatio: number;
    getComputedStyle(ele: Element, parm: any): any;
    addEventListener(type: "devicemotion", listener: (ev: DeviceMotionEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "deviceorientation", listener: (ev: DeviceOrientationEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "message", listener: (ev: MessageEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "mousedown", listener: (ev: MouseEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "mouseenter", listener: (ev: MouseEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "mouseleave", listener: (ev: MouseEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "mousemove", listener: (ev: MouseEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "mouseout", listener: (ev: MouseEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "mouseover", listener: (ev: MouseEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "mouseup", listener: (ev: MouseEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "mousewheel", listener: (ev: MouseWheelEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "offline", listener: (ev: Event) => any, useCapture?: boolean): void;
    addEventListener(type: "online", listener: (ev: Event) => any, useCapture?: boolean): void;
    addEventListener(type: "orientationchange", listener: (ev: Event) => any, useCapture?: boolean): void;
    addEventListener(type: "pause", listener: (ev: Event) => any, useCapture?: boolean): void;
    addEventListener(type: "pointercancel", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointerdown", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointerenter", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointerleave", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointermove", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointerout", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointerover", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "pointerup", listener: (ev: PointerEvent) => any, useCapture?: boolean): void;
    addEventListener(type: "load", listener: (ev: Event) => any, useCapture?: boolean): void;
    addEventListener(type: "resize", listener: (ev: UIEvent) => any, useCapture?: boolean): void;
    addEventListener(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void;
    _conchInfo: {
        version: string;
    };
    appcache: AppCache;
    loadConchUrl:any;
}
declare var window: Window;
declare var _window: _Window;
declare var location: Location;
//declare var console: Console;
declare var Image: typeof HTMLImageElement;
declare var Audio: typeof HTMLAudioElement;
declare var parent: Window;
declare var frames: any;
declare var navigator: Navigator;
declare var onload: any;
declare function printstack(): void;
declare var div: typeof HTMLDivElement;
declare var _$innerWidth: number;
declare var _$innerHeight: number;
declare class Screen {
    width: number;
    height: number;
}
declare var document: Document;
interface FrameRequestCallback {
    (time: number): void;
}
declare var addEventListener: (type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean) => void;
declare var dispatchEvent: (evt: Event) => boolean;
declare var removeEventListener: (type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean) => void;
declare var clearInterval: (handle: number) => void;
declare var clearTimeout: (handle: number) => void;
declare var setInterval: (handler: any, timeout?: any) => number;
declare var setTimeout: (handler: any, timeout?: any) => number;
declare var nMem: number;
