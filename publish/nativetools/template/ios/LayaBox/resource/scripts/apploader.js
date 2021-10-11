'use strict';
require("webglPlus.js");
conch["platCallBack"] = function (ret) {
    var objid, m, rs, c, rJSON;
    if (ret == null)
        return;
    objid = ret.objId;
    m = ret.mName;
    c = ret.cName;
    rs = ret.v;
    var platC;
    if (objid != -1) {
        platC = PlatformObj.objMap[objid];
    }
    else {
        platC = PlatformClass.clsMap[c];
    }
    if (platC) {
        var callback = platC.callBackList[m];
        callback && callback(rs);
    }
};
class PlatformBase {
    call(methodName, p1, p2, p3, p4, p5, p6, p7, p8, p9) {
        var a = Array.prototype.slice.call(arguments);
        a.shift();
        try {
            var rs = JSON.parse(conch.callMethod(this.objid, true, this.className, methodName, JSON.stringify(a)) || "{}");
            return rs.v;
        }
        catch (e) {
            return null;
        }
    }
    callWithBack(callback, methodName, p1, p2, p3, p4, p5, p6, p7, p8, p9) {
        this.callBackList[methodName] = callback;
        var a = Array.prototype.slice.call(arguments);
        a.splice(0, 2);
        try {
            var rs = JSON.parse(conch.callMethod(this.objid, false, this.className, methodName, JSON.stringify(a)) || "{}");
            return rs.v;
        }
        catch (e) {
            return null;
        }
    }
}
class PlatformClass extends PlatformBase {
    constructor(clsName) {
        super();
        this.objid = -1;
        this.className = clsName;
        this.callBackList = {};
        PlatformClass.clsMap[clsName] = this;
    }
    static createClass(clsName) {
        return PlatformClass.clsMap[clsName] || new PlatformClass(clsName);
    }
    newObject(p1, p2, p3, p4, p5, p6, p7, p8, p9) {
        var a = Array.prototype.slice.call(arguments);
        a.splice(0, 0, "<init>");
        var rs = new PlatformObj(this);
        rs.init.apply(rs, a);
        return rs;
    }
}
PlatformClass.clsMap = {};
class PlatformObj extends PlatformBase {
    constructor(cls) {
        super();
        this.cls = cls;
        this.className = cls.className;
        this.callBackList = {};
    }
    init() {
        this.objid = PlatformObj.objNum++;
        this.call.apply(this, arguments) || -1;
        PlatformObj.objMap[this.objid] = this;
    }
}
PlatformObj.objMap = {};
PlatformObj.objNum = 0;
window["PlatformClass"] = PlatformClass;
window["PlatformObj"] = PlatformObj;
function loadRawCache(cache, relUrl, encode) {
    var cpath = cache.getCachePath();
    var relFile = relUrl;
    var id = new Uint32Array([cache.hashstr(relFile)])[0].toString(16);
    var fn = cpath + '/files/' + id.substr(0, 2) + '/' + id.substr(2);
    var ab = fs_readFileSync(fn);
    if (ab) {
        var content = new Uint8Array(ab, 48);
        if (encode === 'utf8') {
            var strCont = String.fromCharCode.apply(null, content);
            return strCont;
        }
        else
            return content.buffer;
    }
    return null;
}
class textBitmap {
    constructor(obj) {
        this._nativeObj = obj;
        this._data = null;
    }
    get width() {
        return this._nativeObj.width;
    }
    get height() {
        return this._nativeObj.height;
    }
    get data() {
        return this._data;
    }
    set data(value) {
        this._data = value;
    }
}
window["textBitmap"] = textBitmap;
class measureText {
    constructor() {
        this.width = 0;
        this.height = 0;
    }
    ;
    ;
}
class conchTextCanvas {
    constructor() {
        this.charSizeMap = new Map();
        this._currentFont = null;
        this._nativeObj = window["_conchTextCanvas"];
    }
    scale(x, y) {
        this._nativeObj.scale(x, y);
    }
    get font() {
        this._currentFont = this._nativeObj.font;
        return this._currentFont;
    }
    set font(value) {
        this._currentFont = value;
        this._nativeObj.font = value;
    }
    setFontInfo(font) {
        this._nativeObj.setFontInfo(font);
    }
    measureText(text) {
        if (!this._currentFont) {
            return null;
        }
        var measure = new measureText();
        var widthCount = 0;
        var maxHeight = 0;
        var charCode = 0;
        var char = "";
        var size = null;
        var dic = null;
        if (!text) {
            return this.measureText('undefined');
        }
        for (var i = 0; i < text.length; i++) {
            char = text.charAt(i);
            charCode = text.charCodeAt(i);
            dic = this.charSizeMap.get(this._currentFont);
            if (!dic) {
                dic = new Map();
                this.charSizeMap.set(this._currentFont.slice(0), dic);
            }
            if (charCode >= 0x4E00 && charCode <= 0x9FFF) {
                size = dic.get("国");
                if (!size) {
                    size = this._nativeObj.measureChar("国".charCodeAt(0));
                    dic.set("国", size);
                }
            }
            else {
                size = dic.get(char);
                if (!size) {
                    size = this._nativeObj.measureChar(charCode);
                    dic.set(char.slice(0), size);
                }
            }
            widthCount += size.width;
            maxHeight = size.height > maxHeight ? size.height : maxHeight;
        }
        measure.width = widthCount;
        measure.height = maxHeight;
        return measure;
    }
    initFreeTypeDefaultFontFromFile(defaultTTFs) {
        return this._nativeObj.initFreeTypeDefaultFontFromFile(defaultTTFs);
    }
    initFreeTypeDefaultFontFromBuffer(ab) {
        return this._nativeObj.initFreeTypeDefaultFontFromBuffer(ab);
    }
    setFontFaceFromUrl(fontFamily, TTFFileName) {
        return this._nativeObj.setFontFaceFromUrl(fontFamily, TTFFileName);
    }
    setFontFaceFromBuffer(fontFamily, ab) {
        return this._nativeObj.setFontFaceFromBuffer(fontFamily, ab);
    }
    removeFont(fontFamily) {
        return this._nativeObj.removeFont(fontFamily);
    }
    getTextBitmapData(sText, nColor, nBorderSize, nBorderColor) {
        var nativeObj = this._nativeObj._getTextBitmapData(sText, nColor, nBorderSize, nBorderColor);
        var pTextBitmap = new textBitmap(nativeObj);
        pTextBitmap.data = nativeObj.data;
        return pTextBitmap;
    }
}
window["conchTextCanvas"] = new conchTextCanvas;
class WindowBase64 {
    constructor() {
        this.atob = function () { return null; };
        this.btoa = function () { return null; };
    }
}
window["WindowBase64"] = WindowBase64;
function _process() {
    this.pid = 0;
    this.cwd = function () {
        return 'd:/temp';
    };
    this.mainModule = 'index.js';
    this.argv = ['conch.exe', 'index.js'];
    this.version = '1.3.1';
    this._require = function (f) { console.log('process require(' + f + ')'); return function nop() { }; };
    this._debugObject = {};
}
window.process = new _process();
class SubtleCrypto {
    decrypt(algorithm, key, data) {
    }
    deriveBits(algorithm, baseKey, length) {
    }
    deriveKey(algorithm, baseKey, derivedKeyType, extractable, keyUsages) {
    }
    digest(algorithm, data) {
    }
    encrypt(algorithm, key, data) {
    }
    exportKey(format, key) {
    }
    generateKey(algorithm, extractable, keyUsages) {
    }
    importKey(format, keyData, algorithm, extractable, keyUsages) {
    }
    sign(algorithm, key, data) {
    }
    unwrapKey(format, wrappedKey, unwrappingKey, unwrapAlgorithm, unwrappedKeyAlgorithm, extractable, keyUsages) {
    }
    verify(algorithm, key, signature, data) {
    }
    wrapKey(format, key, wrappingKey, wrapAlgorithm) {
    }
}
class Crypto {
    constructor() {
        this.subtle = new SubtleCrypto();
    }
    getRandomValues(array) {
        return null;
    }
}
var LogLevel;
(function (LogLevel) {
    LogLevel[LogLevel["Warn"] = 0] = "Warn";
    LogLevel[LogLevel["Error"] = 1] = "Error";
    LogLevel[LogLevel["Debug"] = 2] = "Debug";
    LogLevel[LogLevel["Info"] = 3] = "Info";
    LogLevel[LogLevel["Runtime"] = 4] = "Runtime";
})(LogLevel || (LogLevel = {}));
class Console {
    constructor() {
    }
    assert(test, message) {
        var c = _console;
        if (test) {
            c.log(3, message);
        }
        ;
    }
    clear() { }
    count(countTitle) {
    }
    debug(message) {
        var c = _console;
        c.log(LogLevel.Debug, message);
    }
    dir(value) {
    }
    dirxml(value) {
    }
    error(message) {
        var c = _console;
        c.log(LogLevel.Error, message);
    }
    group(groupTitle) { }
    groupCollapsed(groupTitle) { }
    groupEnd() { }
    info(message) {
        var c = _console;
        c.log(LogLevel.Info, message);
    }
    log(message) {
        var c = _console;
        c.log(LogLevel.Info, message);
    }
    profile(reportName) {
    }
    profileEnd() { }
    select(element) { }
    time(timerName) { }
    timeEnd(timerName) { }
    trace(message) {
    }
    warn(message) {
        var c = _console;
        c.log(LogLevel.Warn, message);
    }
}
class GlobalEventHandlers {
    constructor() {
    }
    onpointercancel(ev) {
        return null;
    }
    onpointerdown(ev) {
        return null;
    }
    onpointerenter(ev) {
        return null;
    }
    onpointerleave(ev) {
        return null;
    }
    onpointermove(ev) {
        return null;
    }
    onpointerout(ev) {
        return null;
    }
    onpointerover(ev) {
        return null;
    }
    onpointerup(ev) {
        return null;
    }
}
class ErrorEvent {
    constructor() {
    }
    initErrorEvent(typeArg, canBubbleArg, cancelableArg, messageArg, filenameArg, linenoArg) {
    }
}
class Event {
    constructor(type, eventInitDict) {
        this._propagationStopped = false;
        this.type = type;
        this.timeStamp = Date.now();
        this.bubbles = false;
        this.cancelable = false;
        this.eventPhase = Event.AT_TARGET;
        if (eventInitDict) {
            this.bubbles = eventInitDict.bubbles;
            this.cancelable = eventInitDict.cancelable;
        }
    }
    initEvent(eventTypeArg, canBubbleArg, cancelableArg) {
        this.type = eventTypeArg;
        this.bubbles = canBubbleArg;
        this.cancelable = cancelableArg;
    }
    preventDefault() {
        if (!this.cancelable)
            return;
        this.defaultPrevented = true;
    }
    stopImmediatePropagation() {
    }
    stopPropagation() {
        this._propagationStopped = true;
    }
}
Event.AT_TARGET = 2;
Event.BUBBLING_PHASE = 3;
Event.CAPTURING_PHASE = 1;
var _lbEvent = window['Event'] = Event;
var _lbMap = Map;
class EventTarget {
    constructor() {
        this._evtMaps = new _lbMap();
        this.removeEventListener = (type, listener, useCapture) => {
            var listeners = this._evtMaps.get(type);
            if (!listeners)
                return;
            var newlisteners = [];
            for (var i = 0, sz = listeners.length; i < sz; i++) {
                if (listeners[i].listener != listener || listeners[i].useCapture != useCapture) {
                    newlisteners.push(listeners[i]);
                }
            }
            this._evtMaps.set(type, newlisteners);
        };
        this.dispatchEvent = this._dispatchEvent.bind(this);
        this.fireEventListeners = this._fireEventListeners.bind(this);
    }
    addEventListener(type, listener, useCapture) {
        var listeners = this._evtMaps.get(type) || [];
        listeners.push({ obj: this, listener: listener, useCapture: useCapture });
        this._evtMaps.set(type, listeners);
    }
    _dispatchEvent(evt) {
        this.fireEventListeners(evt);
        return !evt.defaultPrevented;
    }
    _fireEventListeners(evt) {
        var listeners = this._evtMaps.get(evt.type);
        if (listeners) {
            listeners.forEach(function (listener) {
                switch (evt.eventPhase) {
                    case Event.CAPTURING_PHASE:
                        if (listener.useCapture && listener.listener)
                            listener.listener.call(listener.obj, evt);
                        break;
                    case Event.AT_TARGET:
                        if (listener.listener)
                            listener.listener.call(listener.obj, evt);
                        break;
                    case Event.BUBBLING_PHASE:
                        if (!listener.useCapture && listener.listener)
                            listener.listener.call(listener.obj, evt);
                        break;
                }
            });
        }
    }
}
class UIEvent extends Event {
    constructor(type, eventInitDict) {
        super(type);
        this.bubbles = true;
        if (eventInitDict) {
            this.initUIEvent(type, eventInitDict.bubbles, eventInitDict.cancelable, eventInitDict.view, eventInitDict.detail);
        }
    }
    initUIEvent(typeArg, canBubbleArg, cancelableArg, viewArg, detailArg) {
        this.type = typeArg;
        this.cancelable = canBubbleArg;
        this.cancelable = cancelableArg;
        this.view = viewArg;
        this.detail = detailArg;
    }
}
class PointerEvent {
    constructor(typeArg, eventInitDict) {
    }
}
class MouseEvent extends UIEvent {
    constructor(typeArg, eventInitDict) {
        super(typeArg, eventInitDict);
        if (eventInitDict) {
            for (var v in eventInitDict) {
                this[v] = eventInitDict[v];
            }
        }
    }
    getModifierState(keyArg) {
        return false;
    }
    initMouseEvent(typeArg, canBubbleArg, cancelableArg, viewArg, detailArg, screenXArg, screenYArg, clientXArg, clientYArg, ctrlKeyArg, altKeyArg, shiftKeyArg, metaKeyArg, buttonArg, relatedTargetArg) {
        var args = arguments;
        ['type', 'bubbles', 'cancelable', 'view', 'detail', 'screenX', 'screenY', 'clientX', 'clientY', 'ctrlKey', 'altKey', 'shiftKey', 'metaKey', 'button', 'relatedTarget'].forEach((v, i, a) => {
            this[v] = args[i];
        });
    }
}
var _lbMouseEvent = window['MouseEvent'] = MouseEvent;
class MouseWheelEvent extends MouseEvent {
    constructor() {
        super("mousewheel");
    }
    initMouseWheelEvent(typeArg, canBubbleArg, cancelableArg, viewArg, detailArg, screenXArg, screenYArg, clientXArg, clientYArg, buttonArg, relatedTargetArg, modifiersListArg, wheelDeltaArg) {
    }
}
class WheelEvent extends MouseEvent {
    constructor(typeArg, eventInitDict) {
        super(typeArg, eventInitDict);
        if (eventInitDict) {
            this.deltaMode = eventInitDict.deltaMode;
            this.deltaX = eventInitDict.deltaX;
            this.deltaY = eventInitDict.deltaY;
            this.deltaZ = eventInitDict.deltaZ;
        }
    }
    getCurrentPoint(element) {
    }
    initWheelEvent(typeArg, canBubbleArg, cancelableArg, viewArg, detailArg, screenXArg, screenYArg, clientXArg, clientYArg, buttonArg, relatedTargetArg, modifiersListArg, deltaXArg, deltaYArg, deltaZArg, deltaMode) {
    }
}
WheelEvent.DOM_DELTA_LINE = 1;
WheelEvent.DOM_DELTA_PAGE = 2;
WheelEvent.DOM_DELTA_PIXEL = 0;
class Touch {
    constructor() {
    }
}
class TouchList extends Array {
    item(index) {
        return this[index];
    }
}
class DeviceMotionEvent extends Event {
    constructor() {
        super('devicemotion');
    }
    initDeviceMotionEvent(type, bubbles, cancelable, acceleration, accelerationIncludingGravity, rotationRate, interval) {
    }
}
window["DeviceMotionEvent"] = DeviceMotionEvent;
class DeviceOrientationEvent extends Event {
    constructor() {
        super('deviceorientation');
    }
    initDeviceOrientationEvent(type, bubbles, cancelable, alpha, beta, gamma, absolute) {
    }
}
window["DeviceOrientationEvent"] = DeviceOrientationEvent;
class DocumentEvent {
    constructor() {
    }
    createEvent(eventInterface) {
        return null;
    }
}
class ProgressEvent extends Event {
    constructor(type) {
        super(type);
    }
    initProgressEvent(typeArg, canBubbleArg, cancelableArg, lengthComputableArg, loadedArg, totalArg) {
        this.type = typeArg;
        this.cancelBubble = canBubbleArg;
        this.cancelable = cancelableArg;
        this.lengthComputable = lengthComputableArg;
        this.loaded = loadedArg;
        this.total = totalArg;
    }
}
var _lbProgressEvent = window["ProgressEvent"] = ProgressEvent;
var VendorIDSource;
(function (VendorIDSource) {
    VendorIDSource[VendorIDSource["bluetooth"] = 0] = "bluetooth";
    VendorIDSource[VendorIDSource["usb"] = 1] = "usb";
})(VendorIDSource || (VendorIDSource = {}));
;
class BluetoothDevice {
    connectGATT() {
        return null;
    }
}
class Bluetooth extends EventTarget {
    requestDevice(options) {
        return null;
    }
}
class Gamepad {
}
class GamepadEvent extends Event {
}
class Storage {
    constructor() {
        this.storagePath = conchConfig.getStoragePath();
        this.filename = '';
        this.db = {};
        this._len = 0;
    }
    get length() {
        return this._len;
    }
    getItem(key) {
        return this.db[key] || null;
    }
    key(index) {
        var keys = Object.keys(this.db);
        keys.sort();
        return keys[index] || null;
    }
    removeItem(key) {
        if (this.db[key])
            this._len--;
        delete this.db[key];
        this.savedb();
    }
    _setItem(key, data) {
        if (this.db[key] == null)
            this._len++;
        this.db[key] = data;
        Object.defineProperty(this, key, {
            get: function () {
                return this.db[key];
            },
            enumerable: true,
            configurable: true
        });
    }
    setItem(key, data) {
        this._setItem(key, data);
        this.savedb();
    }
    create(url) {
        url = location.fullpath;
        if (location.protocol == "file:") {
            this.filename = url.substring(8).replace(/:/g, '_').replace(/[\\\/]/g, '__');
            this.fileNamePre = this.storagePath + '/' + this.filename;
        }
        else {
            this.fileNamePre = this.storagePath + '/' + url.split('/')[2].replace(':', '_');
        }
        this.filename = this.fileNamePre + '.txt';
        var strdb = readFileSync(this.filename, 'utf8') || '{}';
        var db = JSON.parse(strdb);
        for (var v in db) {
            this._setItem(v, db[v]);
        }
        return this;
    }
    onChange(changes) {
        if (changes && changes.length) {
        }
    }
    clear() {
        this.db = {};
        this.savedb();
    }
    savedb() {
        writeStrFileSync(this.filename, JSON.stringify(this.db));
    }
}
window["Storage"] = Storage;
class WindowLocalStorage {
    constructor() {
        this.localStorage = new Storage();
    }
}
class WindowSessionStorage {
    getItem(i) {
        return this[i] || null;
    }
    setItem(i, b) {
        this[i] = b;
    }
}
window.loadLocalStorage = function (url) {
    return new Storage().create(url);
};
class _Cookie {
    constructor() {
        this.domain = "";
    }
    static addCookie(s) {
        var result = _Cookie.sreg.exec(s);
        if (result == null) {
            console.warn("设置cookie无效");
            return null;
        }
        else {
            var temp = new _Cookie();
            temp.key = result[1];
            temp.value = result[2];
            if (result[4]) {
                temp.expires = new Date(result[4]);
            }
            return temp;
        }
    }
    static pushCookie(c) {
        if (!c.isValid())
            return false;
        for (var i = 0, len = _Cookie.cookies.length; i < len; i++) {
            var temp = _Cookie.cookies[i];
            if (temp.key == c.key) {
                _Cookie.cookies[i] = c;
                return true;
            }
        }
        _Cookie.cookies.push(c);
        return true;
    }
    static flush() {
        writeStrFileSync(document._cookiePath, _Cookie.getString("in", "\""));
    }
    static getString(type, joinstr) {
        var a = [];
        for (var i = 0, len = _Cookie.cookies.length; i < len; i++) {
            var temp = _Cookie.cookies[i];
            if (temp.isValid()) {
                a.push(temp.toLocalString(type));
            }
            else {
                _Cookie.cookies.slice(i, 1);
                i--;
                len--;
            }
        }
        return a.join(joinstr);
    }
    static toLocalString() {
        return _Cookie.getString("out", "; ");
    }
    isValid() {
        if (this.expires && (this.expires.getTime() < Date.now())) {
            return false;
        }
        return true;
    }
    static init(s) {
        if (s) {
            var temp = s.split("\"");
            for (var i = 0, len = temp.length; i < len; i++) {
                var t = _Cookie.addCookie(temp[i]);
                console.warn(">>>>>>>>>>addCookie" + temp[i]);
                if (t) {
                    t.value = decodeURIComponent(t.value);
                    t.domain = decodeURIComponent(t.domain);
                }
                _Cookie.pushCookie(t);
            }
        }
    }
    toLocalString(type) {
        switch (type) {
            case "in":
                if (this.expires) {
                    return this.key + "=" + encodeURIComponent(this.value) + "; expires=" + this.expires.toGMTString() + "; domain=" + encodeURIComponent(this.domain);
                }
                else {
                    return this.key + "=" + encodeURIComponent(this.value) + "; domain=" + encodeURIComponent(this.domain);
                }
            case "out":
                return this.key + "=" + this.value;
            default:
                return null;
        }
    }
}
_Cookie.cookies = [];
_Cookie.sreg = new RegExp("([^=]*)\\s*=\\s*([^;]*)\\s*;\\s*(expires\\s*=\\s*(.{23,26}GMT)|)");
class KeyboardEvent extends UIEvent {
    constructor(typeArg, eventInitDict) {
        super(typeArg);
        if (eventInitDict) {
            var ini = eventInitDict;
            this.altKey = eventInitDict.altKey;
            this.initKeyboardEvent(typeArg, ini.bubbles, ini.cancelable, ini.view, ini.key, ini.location, null, ini.repeat, null);
        }
    }
    initKeyboardEvent(typeArg, canBubbleArg, cancelableArg, viewArg, keyArg, locationArg, modifiersListArg, repeat, locale) {
        this.type = typeArg;
        this.cancelable = canBubbleArg;
        this.cancelable = cancelableArg;
        this.key = keyArg;
        this.location = locationArg;
        this.locale = locale;
    }
}
KeyboardEvent.DOM_KEY_LOCATION_LEFT = 1;
KeyboardEvent.DOM_KEY_LOCATION_NUMPAD = 3;
KeyboardEvent.DOM_KEY_LOCATION_RIGHT = 2;
KeyboardEvent.DOM_KEY_LOCATION_STANDARD = 0;
var _lbKeyboardEvent = window["KeyboardEvent"] = KeyboardEvent;
(function (layaDoc) {
    'use strict';
    var APP_TOUCH_DOWN = 0;
    var APP_TOUCH_UP = 1;
    var APP_TOUCH_MOV = 2;
    var APP_TOUCH_PT_DOWN = 5;
    var APP_TOUCH_PT_UP = 6;
    var m_vTouchs = [];
    class touchEvt extends UIEvent {
        constructor(type, id, name, x, y) {
            super('');
            this.changedTouches = [];
            this.touches = [];
            this.targetTouches = this.changedTouches;
            var touch = new Touch();
            touch.identifier = id;
            touch.pageX = touch.screenX = touch.clientX = x;
            touch.pageY = touch.screenY = touch.clientY = y;
            this.changedTouches.push(touch);
            switch (type) {
                case APP_TOUCH_DOWN:
                case APP_TOUCH_PT_DOWN:
                    m_vTouchs.push(touch);
                    break;
                case APP_TOUCH_PT_UP:
                    {
                        var epos = 0;
                        var tnum = m_vTouchs.length;
                        while (epos < tnum) {
                            var ce = m_vTouchs[epos];
                            if (ce.identifier == id) {
                                break;
                            }
                            epos++;
                        }
                        if (epos >= tnum) {
                        }
                        else {
                            m_vTouchs.splice(epos, 1);
                        }
                    }
                    break;
                case APP_TOUCH_MOV:
                    {
                        var tnum = m_vTouchs.length;
                        var ti = 0;
                        while (ti < tnum) {
                            var ce = m_vTouchs[ti];
                            if (ce.identifier == id) {
                                m_vTouchs[ti] = touch;
                                break;
                            }
                            ti++;
                        }
                    }
                    break;
                case APP_TOUCH_UP:
                    m_vTouchs = [];
                    break;
            }
            this.touches = m_vTouchs;
            switch (type) {
                case 0:
                case 5:
                    this.type = "touchstart";
                    break;
                case 1:
                case 6:
                    this.type = "touchend";
                    break;
                case 2:
                    this.type = "touchmove";
                    break;
            }
        }
    }
    var joystickEvt = function (TL_xOffset, TL_yOffset, TR_xOffset, TR_yOffset, LT_Offset, RT_Offset) {
        this.THUMBL_xOffset = TL_xOffset;
        this.THUMBL_yOffset = TL_yOffset;
        this.THUMBR_xOffset = TR_xOffset;
        this.THUMBR_yOffset = TR_yOffset;
        this.LT_Offset = LT_Offset;
        this.RT_Offset = RT_Offset;
    };
    var keyEvt = function (name, code, keychar, flag) { this.type = name; this.keyCode = code; this.keyChar = keychar; this.altKey = flag & 0x1; this.shiftKey = (flag & 0x2) != 0; this.ctrlKey = (flag & 0x4) != 0; this.preventDefault = function () { }; };
    conch.setTouchEvtFunction((touchtype, id, etype, x, y) => {
        if (conch.disableMultiTouch && id != 0)
            return;
        var doc = window.document;
        if (!doc) {
            console.log('touch event cant dispatch!');
            return;
        }
        var evt = new touchEvt(touchtype, id, etype, x, y);
        evt.target = doc.pickElement(x, y);
        doc.dispatchEvent(evt);
    });
    function keyEventHandle() {
        var ke = new _lbKeyboardEvent('');
        return function (type, keycode, keychar, AltShiftCtrl) {
            var doc = window.document;
            if (!doc)
                return;
            var keyinit = {};
            var kc = String.fromCharCode(keycode);
            ke.defaultPrevented = false;
            ke._propagationStopped = false;
            ke.type = type;
            ke.key = kc;
            ke.keyCode = keycode;
            ke.altKey = (AltShiftCtrl & 0x4) != 0;
            ke.shiftKey = (AltShiftCtrl & 0x2) != 0;
            ke.ctrlKey = (AltShiftCtrl & 0x1) != 0;
            ke.target = doc._topElement;
            doc.dispatchEvent(ke);
            if (!ke.defaultPrevented) {
                var f = doc['on' + type];
                if (f) {
                    f.call(doc, ke);
                }
            }
        };
    }
    conch.setKeyEvtFunction(keyEventHandle());
    conch.setMouseEvtFunction(function (touchtype, type, x, y, wheel) {
        var doc = window.document;
        if (!doc) {
            console.log('mouse event cant dispatch!');
            return;
        }
        var target = doc.pickElement(x, y);
        if (wheel != 0) {
            var evt1 = new MouseWheelEvent();
            evt1.clientX = evt1.pageX = evt1.screenX = x;
            evt1.clientY = evt1.pageY = evt1.screenY = y;
            evt1.target = target;
            evt1.wheelDelta = wheel;
            doc.dispatchEvent(evt1);
        }
        else {
            var evt;
            switch (touchtype) {
                case 10:
                    evt = new _lbMouseEvent("mousedown");
                    evt.button = 1;
                    break;
                case 11:
                    evt = new _lbMouseEvent("mouseup");
                    evt.button = 1;
                    break;
                default:
                    evt = new _lbMouseEvent(type);
                    evt.button = 0;
                    break;
            }
            evt.clientX = evt.pageX = evt.screenX = x;
            evt.clientY = evt.pageY = evt.screenY = y;
            evt.target = target;
            doc.dispatchEvent(evt);
        }
    });
    conch.otherBuffer = new ArrayBuffer(40);
    conch.otherDataView = new DataView(conch.otherBuffer);
    conch.setBuffer(conch.otherBuffer);
    conch.setDeviceMotionEvtFunction(function (type, ra, rb, rg) {
        if ((typeof ra) != "undefined") {
            var e = new DeviceOrientationEvent();
            e.alpha = ra;
            e.beta = rb;
            e.gamma = rg;
            window.dispatchEvent(e);
        }
        else {
            var d = conch.otherDataView;
            var evt = new DeviceMotionEvent();
            evt.acceleration = { x: d.getFloat32(0, true), y: d.getFloat32(4, true), z: d.getFloat32(8, true) };
            evt.accelerationIncludingGravity = { x: d.getFloat32(12, true), y: d.getFloat32(16, true), z: d.getFloat32(20, true) };
            evt.rotationRate = { alpha: d.getFloat32(24, true), beta: d.getFloat32(28, true), gamma: d.getFloat32(32, true) };
            evt.interval = d.getFloat32(36, true);
            window.dispatchEvent(evt);
        }
    });
    setJoystickEvtFunction(function (type, thumbL_xoff, thumbL_yoff, thumbR_xoff, thumbR_yoff, LT_offset, RT_offset) {
    });
    conch.setNetworkEvtFunction(function (type) {
        var event = new Event("network");
        event["code"] = type;
        document.dispatchEvent(event);
    });
})(window.document);
class Navigator {
    constructor() {
    }
    get appName() { return 'Netscape'; }
    get appVersion() { return this.userAgent; }
    ;
    get platform() { return window.layabox.devinfo.os; }
    ;
    get userAgent() {
        var os = window.layabox.devinfo.os;
        if (os == "ios")
            return "LayaBox(iPhone; CPU iPhone OS Mac OS X)";
        else if (os == "android")
            return "LayaBox Android";
        else
            return 'LayaBox/2.1';
    }
    ;
    get sv() {
        var v = window.layabox.devinfo.osversion;
        var t = parseFloat(v);
        if (isNaN(t)) {
            t = v.charCodeAt(0) - 71;
        }
        return t;
    }
    ;
    get appCodeName() { return 'Mozilla'; }
    get language() { return 'zh-CN'; }
    ;
    get userLanguage() { return 'zh-CN'; }
    getGamepads() {
        return null;
    }
    javaEnabled() { return false; }
    vibrate(pattern) {
        return false;
    }
    addEventListener(type, listener, useCapture) {
    }
}
class Node extends EventTarget {
    constructor() {
        super();
        this._childs = [];
        this._zip = 0;
        this._z = 0;
        this.nodeType = 1;
        this._zip = Node._nodeid++;
    }
    get firstChild() {
        return this._childs ? this._childs[0] : null;
    }
    set firstChild(node) {
    }
    get childNodes() {
        return this._childs;
    }
    set childNodes(nodes) {
    }
    insertBefore(newChild, oldChild) {
        this.appendChild(newChild);
    }
    appendChild(newChild) {
        if (newChild == null)
            return null;
        if (newChild == this)
            return;
        if (this._childs.indexOf(newChild) < 0) {
            this._childs.push(newChild);
        }
        newChild.parentNode = this;
        newChild._z = this._z + 1;
        var doc = newChild.ownerDocument || this.ownerDocument;
        if (doc && newChild._z >= doc._topElement._z && newChild instanceof HTMLCanvasElement && !(window.document._topElement instanceof HTMLCanvasElement)) {
            var ele = newChild;
            if (ele.__visible) {
                window.document._topElement = newChild;
            }
        }
        return newChild;
    }
    cloneNode(deep) {
        if (deep) {
            deep = false;
        }
        function clone(obj) {
            if (typeof (obj) != 'object' || obj == null)
                return obj;
            var newObj = Object.create(obj.__proto__);
            for (var i in obj) {
                if (!deep)
                    newObj[i] = obj[i];
                else
                    newObj[i] = clone(obj[i]);
            }
            return newObj;
        }
        return clone(this);
    }
    removeChild(oldChild) {
        var p = this._childs.indexOf(oldChild);
        if (p >= 0) {
            this._childs.splice(p, 1);
            if (window.document._topElement == oldChild)
                window.document._topElement = oldChild.parentElement;
            oldChild.parentNode = null;
            oldChild.parentElement = null;
            return oldChild;
        }
        else
            return null;
    }
    getAncestorsNode() {
        var ret = new Array();
        var obj = this;
        while (obj.parentNode) {
            ret.push(obj.parentNode);
            obj = obj.parentNode;
        }
        return ret;
    }
}
Node._nodeid = 0;
class NodeSelector {
    querySelector(selectors) {
        return null;
    }
    querySelectorAll(selectors) {
        return [];
    }
}
class ChildNode {
    constructor() { }
    remove() {
    }
}
class ElementTraversal {
    constructor() {
    }
}
class Element extends Node {
    constructor() {
        super();
        this.__visible = true;
    }
    set id(s) {
        this._id = s;
        document.all.push(this);
    }
    get id() {
        return this._id;
    }
    setAttribute(name, value) {
        if (!this._attribs)
            this._attribs = [];
        this._attribs[name] = value;
    }
    getAttribute(name) {
        return this._attribs ? this._attribs[name] : null;
    }
}
applyMixins(Element, [Node, GlobalEventHandlers, ElementTraversal, NodeSelector, ChildNode]);
var ns_Timer;
(function (ns_Timer) {
    class timerobj {
        constructor(curtm, interval, obj, func, args, num) {
            this.del = false;
            this.interval = (interval < 18) ? -1 : interval;
            if (Math.abs(conch.maxInterval - interval) < 1)
                this.interval = -1;
            this.tm = curtm + interval;
            this.obj = obj;
            this.func = func;
            this.args = args;
            this.num = num;
        }
    }
    var timerqueue = function () {
        this.AniFrame = [];
        this.tmq = new Array();
        this.addTimer = function (tm, obj, func, args, num) {
            var tmobj = new timerobj(Date.now(), tm, obj, func, args, num);
            this.tmq.push(tmobj);
            return tmobj;
        };
        this.delTimer = function (obj) {
            for (var i = 0, sz = this.tmq.length; i < sz; i++) {
                if (this.tmq[i] === obj) {
                    this.tmq[i].del = true;
                    break;
                }
            }
        };
        this.update = function () {
            this.curtm = Date.now();
            var i = 0;
            var btmq = [];
            for (var sz = this.tmq.length; i < sz; i++) {
                var ctm = this.tmq[i];
                if (ctm.del)
                    continue;
                var dt = ctm.tm - this.curtm;
                if (dt < 0 || ctm.interval < 0 || Math.abs(conch.maxInterval - ctm.interval) < 1) {
                    if (typeof (ctm.func) === 'function') {
                        ctm.func.apply(null, ctm.obj);
                    }
                    else {
                        eval(ctm.func);
                    }
                    if (ctm.num < 0 || --ctm.num > 0) {
                        if (dt < 0)
                            dt = 0;
                        ctm.tm = dt + this.curtm + ctm.interval;
                        btmq.push(ctm);
                    }
                }
                else {
                    btmq.push(ctm);
                }
            }
            this.tmq.splice(0, sz);
            this.tmq = this.tmq.concat(btmq);
        };
    };
    var gTimerQueue = new timerqueue();
    class _WindowTimersExtension {
        clearImmediate(handle) {
        }
        setImmediate(expression) {
            return 0;
        }
    }
    ns_Timer._WindowTimersExtension = _WindowTimersExtension;
    class _WindowTimers extends _WindowTimersExtension {
        constructor() {
            super();
        }
        clearInterval(handle) {
            gTimerQueue.delTimer(handle);
        }
        clearTimeout(handle) {
            gTimerQueue.delTimer(handle);
        }
        setInterval(handler, timeout, parm) {
            var b = _WindowTimers.ET;
            if (arguments.length > 2) {
                b = Array.prototype.slice.call(arguments);
                b.splice(0, 2);
                handler.arg = b;
            }
            return gTimerQueue.addTimer(timeout, b, handler, null, -1);
        }
        setTimeout(handler, timeout, parm) {
            if (!timeout)
                timeout = 0;
            var b = _WindowTimers.ET;
            if (arguments.length > 2) {
                b = Array.prototype.slice.call(arguments);
                b.splice(0, 2);
                handler.arg = b;
            }
            return gTimerQueue.addTimer(timeout, b, handler, null, 1);
        }
    }
    _WindowTimers.ET = [];
    ns_Timer._WindowTimers = _WindowTimers;
    var animfrm = 0;
    function requestAnimationFrame(callback) {
        var id = animfrm++;
        gTimerQueue.AniFrame.push({ id: id, func: callback });
        if (!gTimerQueue.AniFrameStTm)
            gTimerQueue.AniFrameStTm = Date.now();
        return id;
    }
    ns_Timer.requestAnimationFrame = requestAnimationFrame;
    function cancelAnimationFrame(handle) {
    }
    ns_Timer.cancelAnimationFrame = cancelAnimationFrame;
    var framCount = 0;
    var fpsTimeout = 0;
    conch.setOnFrame(function () {
        if (framCount % 30 == 0) {
            var cur = Date.now();
            var space = (cur - fpsTimeout) / 30;
            fpsTimeout = cur;
            window["conchFps"] && window["conchFps"].render(Math.round(1000 / space));
        }
        framCount++;
        gTimerQueue.update();
        if (document)
            document.onframeend();
    });
    conch.setOnDraw((vsync) => {
        var curAnimFrms = gTimerQueue.AniFrame;
        gTimerQueue.AniFrame = [];
        curAnimFrms.forEach(function (af) {
            if (af.del)
                return;
            af.func(vsync);
        });
    });
})(ns_Timer || (ns_Timer = {}));
var WindowTimers = ns_Timer._WindowTimers;
var requestAnimationFrame = ns_Timer.requestAnimationFrame;
var cancelAnimationFrame = ns_Timer.cancelAnimationFrame;
class Location {
    constructor() {
        this._nativeObj = conch;
        this.setHref = (url) => {
            if (!url || url.length < 8) {
                alert("您的地址不符合要求");
                return;
            }
            var s = new RegExp("(http|file|https)://([^/:]*)(:(\\d+)|)([^?]+)(.*|)", "g");
            var result;
            if ((result = s.exec(url)) != null) {
                this._href = result[0];
                this._hostname = result[2];
                this._host = result[2] + result[3];
                this._pathname = result[5];
                this._port = result[4];
                this._search = result[6];
                this._protocol = result[1] + ":";
                this._origin = this._protocol + "//" + this._host;
                var i = this._pathname ? this._pathname.lastIndexOf("/") : -1;
                if (i != -1) {
                    var temp = this._pathname.substring(0, i);
                    this._fullpath = this._origin + temp;
                }
                else {
                    this._fullpath = this._origin + this._pathname;
                }
            }
            else {
                alert("您的地址不符合要求");
            }
            this.bk_setHref(url);
            window.localStorage.create(this._fullpath + '/');
        };
        this.bk_setHref = this._nativeObj.setHref.bind(this._nativeObj);
    }
    get hostname() {
        return this._hostname;
    }
    get host() {
        return this._host;
    }
    get fullpath() {
        return this._fullpath;
    }
    get pathname() {
        return this._pathname;
    }
    get protocol() {
        return this._protocol;
    }
    get search() {
        return this._search;
    }
    get port() {
        return this._port;
    }
    get origin() {
        return this._origin;
    }
    get href() {
        return this._href;
    }
    set href(url) {
        url = this.normalizeUrl(url);
        var oldhref = this._href;
        if (url != oldhref) {
            this.setHref(url);
            if (this._href != oldhref)
                reloadJS(true);
        }
    }
    setBaseHref(basehref) {
    }
    getBaseHref() {
        return "";
    }
    assign(url) {
    }
    reload(forcedReload) {
        reloadJS(forcedReload);
    }
    replace(url) {
    }
    toString() {
        return this._href;
    }
    normalizeUrl(url) {
        url = url.replace(/\\/g, '/');
        if (url[0] === '/')
            url = 'file://' + url;
        else if (url[1] === ':')
            url = 'file:///' + url;
        return url.trim();
    }
    parseQuery(url) {
        var ret = {};
        var p1 = url.indexOf('?');
        if (p1 < 0)
            return null;
        var q = url.substr(p1 + 1);
        q && q.split('&').forEach((v, i, a) => {
            var kv = v.split('=');
            if (kv.length === 2) {
                ret[kv[0].trim()] = kv[1].trim();
            }
        });
        return ret;
    }
    resolve(fileName) {
        var urlcache = Location.__urlCache__[fileName];
        if (urlcache != null)
            return urlcache;
        if (fileName == null) {
            return "";
        }
        if (fileName.indexOf("//") == 0) {
            return this.protocol + fileName;
        }
        if (fileName.indexOf("file:///") == 0 || fileName.indexOf("http://") == 0 || fileName.indexOf("https://") == 0)
            return fileName;
        if ((fileName.charAt(1) == ':' && fileName.charAt(2) == '/'))
            fileName = "file://" + fileName;
        if (fileName.charAt(0) == "/") {
            return this._origin + fileName;
        }
        var basePath = this._fullpath;
        var urlfull = basePath + "/" + fileName;
        urlcache = Location.__urlCache__[urlfull];
        if (urlcache != null)
            return urlcache;
        if (fileName.indexOf("://") < 0)
            fileName = basePath + "/" + fileName;
        var urls = fileName.split("/");
        urls[1] = "";
        var str, i = 2, size = urls.length;
        while (i < size) {
            str = urls[i];
            if (str == null)
                break;
            if (str == '' || str == '.') {
                urls.splice(i, 1);
                continue;
            }
            if (str == "..") {
                if (i <= 3 && this._protocol != "file:") {
                    urls.splice(i, 1);
                }
                else {
                    urls.splice(i - 1, 2);
                    i -= 1;
                }
                continue;
            }
            i += 1;
        }
        fileName = urls.join("/");
        Location.__urlCache__[fileName] = fileName;
        Location.__urlCache__[urlfull] = fileName;
        return fileName;
    }
}
Location.__urlCache__ = {};
function applyMixins(derivedCtor, baseCtors) {
    baseCtors.forEach(baseCtor => {
        Object.getOwnPropertyNames(baseCtor.prototype).forEach(name => {
            if (name !== 'constructor') {
                derivedCtor.prototype[name] = baseCtor.prototype[name];
            }
        });
    });
}
class IDBEnvironment {
}
class _Window extends EventTarget {
    constructor() {
        super();
        this.timer = new WindowTimers();
        this.clearInterval = this.timer.clearInterval;
        this.clearTimeout = this.timer.clearTimeout;
        this.setInterval = this.timer.setInterval;
        this.setTimeout = this.timer.setTimeout;
        this._removeEventListener = this.removeEventListener;
        this.removeEventListener = this.removeEventListenernew;
    }
    addEventListener(type, listener, useCapture) {
        if (type == "devicemotion" || type == "deviceorientation") {
            conch.setSensorAble(true);
        }
        super.addEventListener(type, listener, useCapture);
    }
    removeEventListenernew(type, listener, useCapture) {
        this._removeEventListener(type, listener, useCapture);
        if (type == "devicemotion" || type == "deviceorientation") {
            var de = this._evtMaps.get("devicemotion");
            var deo = this._evtMaps.get("deviceorientation");
            if ((!de || de.length == 0) && (!deo || deo.length == 0)) {
                conch.setSensorAble(false);
            }
        }
    }
}
class XMLHttpRequest extends EventTarget {
    constructor() {
        super();
        this._hasReqHeader = false;
        this.withCredentials = false;
        this.setResponseHeader = function (name, value) {
            this._head = value;
        };
        this.xhr = new _XMLHttpRequest();
        this._readyState = 0;
        this._responseText = this._response = this._responseType = this._url = "";
        this._responseType = "text";
        this._method = "GET";
        this.xhr._t = this;
        this.xhr.set_onreadystatechange(function (r) {
            var _t = this._t;
            if (r == 1) {
                _t._readyState = 1;
            }
            if (_t._onrchgcb) {
                var e = new _lbEvent("readystatechange");
                e.target = _t;
                _t._onrchgcb(e);
            }
            var ev;
            if (_t._status == 200) {
                ev = new _lbEvent("load");
                ev.target = _t;
                _t.dispatchEvent(ev);
            }
            else if (_t._status == 404) {
                ev = new _lbEvent("error");
                ev.target = _t;
                _t.dispatchEvent(ev);
            }
        });
    }
    setRequestHeader(name, value) {
        this.xhr.setRequestHeader(name, value);
        this._hasReqHeader = true;
    }
    getAllResponseHeaders() {
        return this._head;
    }
    get responseText() {
        return this._responseText;
    }
    get response() {
        return this._response;
    }
    get responseType() {
        return this._responseType;
    }
    set responseType(type) {
        this._responseType = type;
        if (type == 'blob') {
            this.xhr.responseTypeCode = 4;
        }
        else if (type == 'arraybuffer') {
            this.xhr.responseTypeCode = 5;
        }
        else {
            this.xhr.responseTypeCode = 1;
        }
    }
    get url() {
        return this._url;
    }
    get async() {
        return this._async;
    }
    get readyState() {
        return this._readyState;
    }
    get status() {
        return this._status;
    }
    _loadsus() {
        var e = new _lbEvent("load");
        e.target = this;
        this._onloadcb(e);
    }
    set onreadystatechange(listen) {
        this._onrchgcb = listen;
        if (listen == null)
            return;
        if (this._readyState != 0) {
            var e = new _lbEvent("readystatechange");
            e.target = this;
            this._onrchgcb(e);
        }
    }
    get onreadystatechange() {
        return this._onrchgcb;
    }
    set onload(listen) {
        this._onloadcb = listen;
        if (listen == null)
            return;
        if (this._readyState == 4 && this._status == 200) {
            this._loadsus();
        }
    }
    get onload() {
        return this._onloadcb;
    }
    getResponseHeader() {
        return this._head;
    }
    open(type, url, async) {
        console.log('xhr.' + type + ' url=' + url);
        if (!url)
            return;
        type = type.toUpperCase();
        async = true;
        url = location.resolve(url);
        this._method = (type === 'POST' ? 'POST' : 'GET');
        this._url = url;
        this._async = (async == null || async == undefined || async == true) ? true : false;
        this.xhr._open(this._method, this._url, this._async);
    }
    overrideMimeType(mime) {
        if (this._responseType == "text" || this._responseText == "")
            this._responseType = "arraybuffer";
        this.xhr.mimeType = "1";
    }
    send(body) {
        if (body) {
            if (body instanceof ArrayBuffer || ArrayBuffer.isView(body) || body instanceof DataView)
                this._responseType = 'arraybuffer';
            else if (body instanceof Object) {
                body = JSON.stringify(body);
            }
        }
        this.xhr._t = this;
        var onPostLoad = function (buf, strbuf) {
            var _t = this._t;
            if (_t.responseType == 'arraybuffer') {
                _t._response = buf;
                _t._responseText = strbuf;
            }
            else {
                _t._response = _t._responseText = buf;
            }
            _t._readyState = 4;
            _t._status = 200;
            _t.xhr._changeState(4);
            if (_t._onloadcb) {
                _t._loadsus();
            }
            onPostLoad.ref = onPostError.ref = null;
        };
        var onPostError = function (e1, e2) {
            var _t = this._t;
            _t._readyState = 4;
            _t._status = 404;
            _t.xhr._changeState(4);
            if (_t.onerror) {
                var ev = new _lbEvent("error");
                ev.target = _t;
                ev['ecode1'] = e1;
                ev['ecode2'] = e2;
                _t.onerror(ev);
            }
            onPostLoad.ref = onPostError.ref = null;
        };
        if (this._method == 'POST' && body) {
            onPostLoad.ref = onPostError.ref = this.xhr;
            this.xhr.setPostCB(onPostLoad, onPostError);
            this.xhr.postData(this.url, body);
        }
        else if (this._hasReqHeader) {
            onPostLoad.ref = onPostError.ref = this.xhr;
            this.xhr.setPostCB(onPostLoad, onPostError);
            this.xhr.getData(this.url);
        }
        else {
            var file = new conch_File(this.url);
            var fileRead = new FileReader();
            fileRead.sync = !this.async;
            if (this._responseType == "text" || this._responseType == "TEXT") {
                fileRead.responseType = 0;
            }
            else if (this._responseType == "arraybuffer") {
                fileRead.responseType = 1;
            }
            else {
                console.log("XMLhttpRequest 暂不支持的类型 responseType=" + this.responseType);
            }
            fileRead._t = this;
            fileRead.onload = function () {
                var _t = this._t;
                if (_t._responseType == "arraybuffer") {
                    _t._response = this.result;
                }
                else {
                    _t._response = _t._responseText = this.result;
                    if (_t._responseType == "json") {
                        _t._response = JSON.parse(this.result);
                    }
                }
                if (_t.xhr.mimeType) {
                    var u8arr = new Uint8Array(_t._response);
                    var strret = "";
                    u8arr.forEach((v, i, arr) => {
                        if (v >= 0x80) {
                            strret += String.fromCharCode(0xf700 | v);
                        }
                        else if (v == 0) {
                            strret += '\0';
                        }
                        else {
                            strret += String.fromCharCode(v);
                        }
                    });
                    _t._responseText = strret;
                }
                _t._readyState = 4;
                _t._status = 200;
                _t.xhr._changeState(4);
                if (_t._onloadcb) {
                    _t._loadsus();
                }
                fileRead.onload = null;
                fileRead.onerror = null;
            };
            fileRead.onerror = function () {
                var _t = this._t;
                _t._readyState = 4;
                _t._status = 404;
                _t.xhr._changeState(4);
                if (_t.onerror) {
                    var ev = new _lbEvent("error");
                    ev.target = _t;
                    _t.onerror(ev);
                }
                fileRead.onload = null;
                fileRead.onerror = null;
            };
            if (this.onerror) {
                fileRead.setIgnoreError(true);
            }
            if (this.responseType == "arraybuffer")
                fileRead.readAsArrayBuffer(file);
            else
                fileRead.readAsText(file);
        }
    }
}
XMLHttpRequest.UNSENT = 0;
XMLHttpRequest.OPENED = 1;
XMLHttpRequest.HEADERS_RECEIVED = 2;
XMLHttpRequest.LOADING = 3;
XMLHttpRequest.DONE = 4;
window.XMLHttpRequest = XMLHttpRequest;
class CanvasRenderingContext {
    constructor(c) {
        this.canvas = c;
        this.gl = LayaGLContext.instance;
    }
    setSize(w, h) {
        if (this._width != w || this._height != h) {
            this._width = w;
            this._height = h;
            if (this.canvas._isFirst) {
                this.gl.setMainContextSize(this._width, this._height);
            }
        }
    }
    clear() {
        this.gl.clearColor(0, 0, 0, 0);
        this.gl.clear(LayaGLContext.COLOR_BUFFER_BIT | LayaGLContext.DEPTH_BUFFER_BIT | LayaGLContext.STENCIL_BUFFER_BIT);
    }
    toBase64(type, encoderOptions, callback) {
    }
    getImageData(x, y, w, h, callBack) {
    }
    drawImage(...args) {
    }
    destroy() {
        this.canvas = null;
        this.gl = null;
    }
    set font(fontName) {
        window["_conchTextCanvas"].font = fontName;
    }
    get font() {
        return window["_conchTextCanvas"].font;
    }
}
window["CanvasRenderingContext"] = CanvasRenderingContext;
class ColorUtil {
    static getColorNum(str) {
        if (!str)
            return 0;
        if (typeof (str) == "number") {
            if (str >= 0xFFFFFF)
                return str;
            return str | 0xFF000000;
        }
        var len;
        if (str[0] == "#") {
            var r = ColorUtil.getRGBByRGBStr(str);
            if (r) {
                return parseInt(r, 16);
            }
            else
                return NaN;
        }
        else if (str.indexOf("rgb") != -1) {
            return ColorUtil.getColNum(str, true);
        }
        else if (str.indexOf("hsl") != -1) {
            return ColorUtil.getColNum(str, false);
        }
        else {
            var c = ColorUtil._COLOR_MAP[str];
            return c ? ColorUtil.getColorNum(c) : NaN;
        }
    }
    static getColNum(c, isRgb) {
        var temp, i, s, result = [], st;
        var t = isRgb ? ColorUtil.RGBReg.exec(c) : ColorUtil.HSLReg.exec(c);
        if (t) {
            var r = t[2].split(",");
            i = 0;
            s = r.length;
            while (i < s) {
                result[i] = parseFloat(r[i]);
                i++;
            }
            var ta = result;
            if (!isRgb) {
                ta = ColorUtil.hsl2rgb(result[0] / 360, result[1] / 100, result[2] / 100);
            }
            if (i == 3 && !t[1]) {
                return 0xff000000 + (ta[0] << 16) + (ta[1] << 8) + ta[2];
            }
            else if (i == 4 && t[1]) {
                return parseInt(Math.floor(0xff * result[3]).toString(16) + "000000", 16) + (ta[0] << 16) + (ta[1] << 8) + ta[2];
            }
            return NaN;
        }
        else
            return NaN;
    }
    static getRGBByRGBStr(str) {
        str = str.substr(1);
        var i, len = str.length, result;
        switch (len) {
            case 3:
                result = "ff";
                for (i = 0; i < len; i++) {
                    result += str[i] + str[i];
                }
                break;
            case 6:
                result = "ff" + str;
            default:
                break;
        }
        return result;
    }
    static toHexColor(color) {
        if (color < 0 || isNaN(color))
            return null;
        if (color > 0xff000000) {
            color -= 0xff000000;
        }
        else {
            return "rgba(" + (color >> 16 & 0xff) + "," + (color >> 8 & 0xff) + "," + (color & 0xff) + "," + ((color >> 24 & 0xff) / 0xff) + ")";
        }
        var str = color.toString(16);
        while (str.length < 6)
            str = "0" + str;
        return "#" + str;
    }
    static hsl2rgb(h, s, l) {
        var r, g, b;
        if (s == 0) {
            r = g = b = l;
        }
        else {
            function hue2rgb(p, q, t) {
                if (t < 0)
                    t += 1;
                if (t > 1)
                    t -= 1;
                if (t < 1 / 6)
                    return p + (q - p) * 6 * t;
                if (t < 1 / 2)
                    return q;
                if (t < 2 / 3)
                    return p + (q - p) * (2 / 3 - t) * 6;
                return p;
            }
            var q = l < 0.5 ? l * (1 + s) : l + s - l * s;
            var p = 2 * l - q;
            r = hue2rgb(p, q, h + 1 / 3);
            g = hue2rgb(p, q, h);
            b = hue2rgb(p, q, h - 1 / 3);
        }
        return [Math.round(r * 255), Math.round(g * 255), Math.round(b * 255)];
    }
}
ColorUtil._COLOR_MAP = { "white": '#FFFFFF', "red": '#FF0000', "green": '#00FF00', "blue": '#0000FF', "black": '#000000', "yellow": '#FFFF00', 'gray': '#AAAAAA' };
ColorUtil.RGBReg = new RegExp("rgb([a]{0,1})\\s*[(]([\\d,.\\s-]*)?[)]");
ColorUtil.HSLReg = new RegExp("hsl([a]{0,1})\\s*[(]([\\d,.\\s-%]*)?[)]");
class ProgramLocationTable {
    constructor() {
        this._fakeLocationNum = 0;
        this._map = {};
    }
    getFakeLocation(fakeProgramID, name) {
        var key = fakeProgramID + "-" + name;
        var fakeID = this._map[key];
        if (!fakeID) {
            fakeID = this._fakeLocationNum++;
            this._map[key] = fakeID;
        }
        return fakeID;
    }
}
var FUNCTION_ID;
(function (FUNCTION_ID) {
    FUNCTION_ID[FUNCTION_ID["UNIFORMMATRIX2FVEX"] = 0] = "UNIFORMMATRIX2FVEX";
    FUNCTION_ID[FUNCTION_ID["UNIFORMMATRIX3FVEX"] = 1] = "UNIFORMMATRIX3FVEX";
    FUNCTION_ID[FUNCTION_ID["UNIFORMMATRIX4FVEX"] = 2] = "UNIFORMMATRIX4FVEX";
    FUNCTION_ID[FUNCTION_ID["ADDSHADERUNIFORM"] = 3] = "ADDSHADERUNIFORM";
    FUNCTION_ID[FUNCTION_ID["UPLOADSHADERUNIFORMS"] = 4] = "UPLOADSHADERUNIFORMS";
    FUNCTION_ID[FUNCTION_ID["UPLOADSHADERUNIFORMS_BUFFER"] = 5] = "UPLOADSHADERUNIFORMS_BUFFER";
    FUNCTION_ID[FUNCTION_ID["USECOMMANDENCODER"] = 6] = "USECOMMANDENCODER";
    FUNCTION_ID[FUNCTION_ID["LOADDATATOREG"] = 7] = "LOADDATATOREG";
    FUNCTION_ID[FUNCTION_ID["LOADDATATOREGEX"] = 8] = "LOADDATATOREGEX";
    FUNCTION_ID[FUNCTION_ID["IFLESS0"] = 9] = "IFLESS0";
    FUNCTION_ID[FUNCTION_ID["IFEQUAL0"] = 10] = "IFEQUAL0";
    FUNCTION_ID[FUNCTION_ID["IFGREATER0"] = 11] = "IFGREATER0";
    FUNCTION_ID[FUNCTION_ID["IFLEQUAL0"] = 12] = "IFLEQUAL0";
    FUNCTION_ID[FUNCTION_ID["IFGEQUAL0"] = 13] = "IFGEQUAL0";
    FUNCTION_ID[FUNCTION_ID["IFGNOTEQUAL0"] = 14] = "IFGNOTEQUAL0";
    FUNCTION_ID[FUNCTION_ID["OPERATEREG"] = 15] = "OPERATEREG";
    FUNCTION_ID[FUNCTION_ID["STORE"] = 16] = "STORE";
    FUNCTION_ID[FUNCTION_ID["CREATEIMAGEONRENDERTHREAD"] = 64] = "CREATEIMAGEONRENDERTHREAD";
    FUNCTION_ID[FUNCTION_ID["DELETE_IMAGE_ON_RENDER_THREAD"] = 65] = "DELETE_IMAGE_ON_RENDER_THREAD";
    FUNCTION_ID[FUNCTION_ID["RELEASE_IMAGE_ON_RENDER_THREAD"] = 66] = "RELEASE_IMAGE_ON_RENDER_THREAD";
    FUNCTION_ID[FUNCTION_ID["SET_IMAGE_RELEASE_SPACE_TIME"] = 67] = "SET_IMAGE_RELEASE_SPACE_TIME";
    FUNCTION_ID[FUNCTION_ID["SET_PREMULTIPLY_ALPHA"] = 68] = "SET_PREMULTIPLY_ALPHA";
    FUNCTION_ID[FUNCTION_ID["PERFADDDATA"] = 69] = "PERFADDDATA";
    FUNCTION_ID[FUNCTION_ID["PERFUPDATEDT"] = 70] = "PERFUPDATEDT";
    FUNCTION_ID[FUNCTION_ID["SET_MAIN_CONTEXT_SIZE"] = 71] = "SET_MAIN_CONTEXT_SIZE";
    FUNCTION_ID[FUNCTION_ID["GETCONTEXTATTRIBUTES"] = 128] = "GETCONTEXTATTRIBUTES";
    FUNCTION_ID[FUNCTION_ID["ISCONTEXTLOST"] = 129] = "ISCONTEXTLOST";
    FUNCTION_ID[FUNCTION_ID["GETSUPPORTEDEXTENSIONS"] = 130] = "GETSUPPORTEDEXTENSIONS";
    FUNCTION_ID[FUNCTION_ID["GETEXTENSION"] = 131] = "GETEXTENSION";
    FUNCTION_ID[FUNCTION_ID["ACTIVETEXTURE"] = 132] = "ACTIVETEXTURE";
    FUNCTION_ID[FUNCTION_ID["ATTACHSHADER"] = 133] = "ATTACHSHADER";
    FUNCTION_ID[FUNCTION_ID["BINDATTRIBLOCATION"] = 134] = "BINDATTRIBLOCATION";
    FUNCTION_ID[FUNCTION_ID["BINDBUFFER"] = 135] = "BINDBUFFER";
    FUNCTION_ID[FUNCTION_ID["BINDFRAMEBUFFER"] = 136] = "BINDFRAMEBUFFER";
    FUNCTION_ID[FUNCTION_ID["BINDRENDERBUFFER"] = 137] = "BINDRENDERBUFFER";
    FUNCTION_ID[FUNCTION_ID["BINDTEXTURE"] = 138] = "BINDTEXTURE";
    FUNCTION_ID[FUNCTION_ID["USETEXTURE"] = 139] = "USETEXTURE";
    FUNCTION_ID[FUNCTION_ID["BLENDCOLOR"] = 140] = "BLENDCOLOR";
    FUNCTION_ID[FUNCTION_ID["BLENDEQUATION"] = 141] = "BLENDEQUATION";
    FUNCTION_ID[FUNCTION_ID["BLENDEQUATIONSEPARATE"] = 142] = "BLENDEQUATIONSEPARATE";
    FUNCTION_ID[FUNCTION_ID["BLENDFUNC"] = 143] = "BLENDFUNC";
    FUNCTION_ID[FUNCTION_ID["BLENDFUNCSEPARATE"] = 144] = "BLENDFUNCSEPARATE";
    FUNCTION_ID[FUNCTION_ID["BUFFERDATA_SIZE"] = 145] = "BUFFERDATA_SIZE";
    FUNCTION_ID[FUNCTION_ID["BUFFERDATA_ARRAYBUFFER"] = 146] = "BUFFERDATA_ARRAYBUFFER";
    FUNCTION_ID[FUNCTION_ID["BUFFERSUBDATA"] = 147] = "BUFFERSUBDATA";
    FUNCTION_ID[FUNCTION_ID["CHECKFRAMEBUFFERSTATUS"] = 148] = "CHECKFRAMEBUFFERSTATUS";
    FUNCTION_ID[FUNCTION_ID["CLEAR"] = 149] = "CLEAR";
    FUNCTION_ID[FUNCTION_ID["CLEARCOLOR"] = 150] = "CLEARCOLOR";
    FUNCTION_ID[FUNCTION_ID["CLEARDEPTH"] = 151] = "CLEARDEPTH";
    FUNCTION_ID[FUNCTION_ID["CLEARSTENCIL"] = 152] = "CLEARSTENCIL";
    FUNCTION_ID[FUNCTION_ID["COLORMASK"] = 153] = "COLORMASK";
    FUNCTION_ID[FUNCTION_ID["COMPILESHADER"] = 154] = "COMPILESHADER";
    FUNCTION_ID[FUNCTION_ID["COPYTEXIMAGE2D"] = 155] = "COPYTEXIMAGE2D";
    FUNCTION_ID[FUNCTION_ID["COPYTEXSUBIMAGE2D"] = 156] = "COPYTEXSUBIMAGE2D";
    FUNCTION_ID[FUNCTION_ID["CREATEBUFFER"] = 157] = "CREATEBUFFER";
    FUNCTION_ID[FUNCTION_ID["CREATEFRAMEBUFFER"] = 158] = "CREATEFRAMEBUFFER";
    FUNCTION_ID[FUNCTION_ID["CREATEPROGRAM"] = 159] = "CREATEPROGRAM";
    FUNCTION_ID[FUNCTION_ID["CREATERENDERBUFFER"] = 160] = "CREATERENDERBUFFER";
    FUNCTION_ID[FUNCTION_ID["CREATESHADER"] = 161] = "CREATESHADER";
    FUNCTION_ID[FUNCTION_ID["CREATETEXTURE"] = 162] = "CREATETEXTURE";
    FUNCTION_ID[FUNCTION_ID["CULLFACE"] = 163] = "CULLFACE";
    FUNCTION_ID[FUNCTION_ID["DELETEBUFFER"] = 164] = "DELETEBUFFER";
    FUNCTION_ID[FUNCTION_ID["DELETEFRAMEBUFFER"] = 165] = "DELETEFRAMEBUFFER";
    FUNCTION_ID[FUNCTION_ID["DELETEPROGRAM"] = 166] = "DELETEPROGRAM";
    FUNCTION_ID[FUNCTION_ID["DELETERENDERBUFFER"] = 167] = "DELETERENDERBUFFER";
    FUNCTION_ID[FUNCTION_ID["DELETESHADER"] = 168] = "DELETESHADER";
    FUNCTION_ID[FUNCTION_ID["DELETETEXTURE"] = 169] = "DELETETEXTURE";
    FUNCTION_ID[FUNCTION_ID["DEPTHFUNC"] = 170] = "DEPTHFUNC";
    FUNCTION_ID[FUNCTION_ID["DEPTHMASK"] = 171] = "DEPTHMASK";
    FUNCTION_ID[FUNCTION_ID["DEPTHRANGE"] = 172] = "DEPTHRANGE";
    FUNCTION_ID[FUNCTION_ID["DETACHSHADER"] = 173] = "DETACHSHADER";
    FUNCTION_ID[FUNCTION_ID["DISABLE"] = 174] = "DISABLE";
    FUNCTION_ID[FUNCTION_ID["DISABLEVERTEXATTRIBARRAY"] = 175] = "DISABLEVERTEXATTRIBARRAY";
    FUNCTION_ID[FUNCTION_ID["DRAWARRAYS"] = 176] = "DRAWARRAYS";
    FUNCTION_ID[FUNCTION_ID["DRAWELEMENTS"] = 177] = "DRAWELEMENTS";
    FUNCTION_ID[FUNCTION_ID["ENABLE"] = 178] = "ENABLE";
    FUNCTION_ID[FUNCTION_ID["ENABLEVERTEXATTRIBARRAY"] = 179] = "ENABLEVERTEXATTRIBARRAY";
    FUNCTION_ID[FUNCTION_ID["FINISH"] = 180] = "FINISH";
    FUNCTION_ID[FUNCTION_ID["FLUSH"] = 181] = "FLUSH";
    FUNCTION_ID[FUNCTION_ID["FRAMEBUFFERRENDERBUFFER"] = 182] = "FRAMEBUFFERRENDERBUFFER";
    FUNCTION_ID[FUNCTION_ID["FRAMEBUFFERTEXTURE2D"] = 183] = "FRAMEBUFFERTEXTURE2D";
    FUNCTION_ID[FUNCTION_ID["FRONTFACE"] = 184] = "FRONTFACE";
    FUNCTION_ID[FUNCTION_ID["GENERATEMIPMAP"] = 185] = "GENERATEMIPMAP";
    FUNCTION_ID[FUNCTION_ID["GETACTIVEATTRIB"] = 186] = "GETACTIVEATTRIB";
    FUNCTION_ID[FUNCTION_ID["GETACTIVEUNIFORM"] = 187] = "GETACTIVEUNIFORM";
    FUNCTION_ID[FUNCTION_ID["GETATTRIBLOCATION"] = 188] = "GETATTRIBLOCATION";
    FUNCTION_ID[FUNCTION_ID["GETPARAMETER"] = 189] = "GETPARAMETER";
    FUNCTION_ID[FUNCTION_ID["GETBUFFERPARAMETER"] = 190] = "GETBUFFERPARAMETER";
    FUNCTION_ID[FUNCTION_ID["GETERROR"] = 191] = "GETERROR";
    FUNCTION_ID[FUNCTION_ID["GETFRAMEBUFFERATTACHMENTPARAMETER"] = 192] = "GETFRAMEBUFFERATTACHMENTPARAMETER";
    FUNCTION_ID[FUNCTION_ID["GETPROGRAMPARAMETER"] = 193] = "GETPROGRAMPARAMETER";
    FUNCTION_ID[FUNCTION_ID["GETPROGRAMINFOLOG"] = 194] = "GETPROGRAMINFOLOG";
    FUNCTION_ID[FUNCTION_ID["GETRENDERBUFFERPARAMETER"] = 195] = "GETRENDERBUFFERPARAMETER";
    FUNCTION_ID[FUNCTION_ID["GETSHADERPRECISIONFORMAT"] = 196] = "GETSHADERPRECISIONFORMAT";
    FUNCTION_ID[FUNCTION_ID["GETSHADERPARAMETER"] = 197] = "GETSHADERPARAMETER";
    FUNCTION_ID[FUNCTION_ID["GETSHADERINFOLOG"] = 198] = "GETSHADERINFOLOG";
    FUNCTION_ID[FUNCTION_ID["GETSHADERSOURCE"] = 199] = "GETSHADERSOURCE";
    FUNCTION_ID[FUNCTION_ID["GETTEXPARAMETER"] = 200] = "GETTEXPARAMETER";
    FUNCTION_ID[FUNCTION_ID["GETUNIFORM"] = 201] = "GETUNIFORM";
    FUNCTION_ID[FUNCTION_ID["GETUNIFORMLOCATION"] = 202] = "GETUNIFORMLOCATION";
    FUNCTION_ID[FUNCTION_ID["GETVERTEXATTRIB"] = 203] = "GETVERTEXATTRIB";
    FUNCTION_ID[FUNCTION_ID["GETVERTEXATTRIBOFFSET"] = 204] = "GETVERTEXATTRIBOFFSET";
    FUNCTION_ID[FUNCTION_ID["HINT"] = 205] = "HINT";
    FUNCTION_ID[FUNCTION_ID["ISBUFFER"] = 206] = "ISBUFFER";
    FUNCTION_ID[FUNCTION_ID["ISENABLED"] = 207] = "ISENABLED";
    FUNCTION_ID[FUNCTION_ID["ISFRAMEBUFFER"] = 208] = "ISFRAMEBUFFER";
    FUNCTION_ID[FUNCTION_ID["ISPROGRAM"] = 209] = "ISPROGRAM";
    FUNCTION_ID[FUNCTION_ID["ISRENDERBUFFER"] = 210] = "ISRENDERBUFFER";
    FUNCTION_ID[FUNCTION_ID["ISSHADER"] = 211] = "ISSHADER";
    FUNCTION_ID[FUNCTION_ID["ISTEXTURE"] = 212] = "ISTEXTURE";
    FUNCTION_ID[FUNCTION_ID["LINEWIDTH"] = 213] = "LINEWIDTH";
    FUNCTION_ID[FUNCTION_ID["LINKPROGRAM"] = 214] = "LINKPROGRAM";
    FUNCTION_ID[FUNCTION_ID["PIXELSTOREI"] = 215] = "PIXELSTOREI";
    FUNCTION_ID[FUNCTION_ID["POLYGONOFFSET"] = 216] = "POLYGONOFFSET";
    FUNCTION_ID[FUNCTION_ID["READPIXELS"] = 217] = "READPIXELS";
    FUNCTION_ID[FUNCTION_ID["RENDERBUFFERSTORAGE"] = 218] = "RENDERBUFFERSTORAGE";
    FUNCTION_ID[FUNCTION_ID["SAMPLECOVERAGE"] = 219] = "SAMPLECOVERAGE";
    FUNCTION_ID[FUNCTION_ID["SCISSOR"] = 220] = "SCISSOR";
    FUNCTION_ID[FUNCTION_ID["SHADERSOURCE"] = 221] = "SHADERSOURCE";
    FUNCTION_ID[FUNCTION_ID["STENCILFUNC"] = 222] = "STENCILFUNC";
    FUNCTION_ID[FUNCTION_ID["STENCILFUNCSEPARATE"] = 223] = "STENCILFUNCSEPARATE";
    FUNCTION_ID[FUNCTION_ID["STENCILMASK"] = 224] = "STENCILMASK";
    FUNCTION_ID[FUNCTION_ID["STENCILMASKSEPARATE"] = 225] = "STENCILMASKSEPARATE";
    FUNCTION_ID[FUNCTION_ID["STENCILOP"] = 226] = "STENCILOP";
    FUNCTION_ID[FUNCTION_ID["STENCILOPSEPARATE"] = 227] = "STENCILOPSEPARATE";
    FUNCTION_ID[FUNCTION_ID["TEXIMAGE2D"] = 228] = "TEXIMAGE2D";
    FUNCTION_ID[FUNCTION_ID["TEXPARAMETERF"] = 229] = "TEXPARAMETERF";
    FUNCTION_ID[FUNCTION_ID["TEXPARAMETERI"] = 230] = "TEXPARAMETERI";
    FUNCTION_ID[FUNCTION_ID["TEXSUBIMAGE2D"] = 231] = "TEXSUBIMAGE2D";
    FUNCTION_ID[FUNCTION_ID["UNIFORM1F"] = 232] = "UNIFORM1F";
    FUNCTION_ID[FUNCTION_ID["UNIFORM1FV"] = 233] = "UNIFORM1FV";
    FUNCTION_ID[FUNCTION_ID["UNIFORM1I"] = 234] = "UNIFORM1I";
    FUNCTION_ID[FUNCTION_ID["UNIFORM1IV"] = 235] = "UNIFORM1IV";
    FUNCTION_ID[FUNCTION_ID["UNIFORM2F"] = 236] = "UNIFORM2F";
    FUNCTION_ID[FUNCTION_ID["UNIFORM2FV"] = 237] = "UNIFORM2FV";
    FUNCTION_ID[FUNCTION_ID["UNIFORM2I"] = 238] = "UNIFORM2I";
    FUNCTION_ID[FUNCTION_ID["UNIFORM2IV"] = 239] = "UNIFORM2IV";
    FUNCTION_ID[FUNCTION_ID["UNIFORM3F"] = 240] = "UNIFORM3F";
    FUNCTION_ID[FUNCTION_ID["UNIFORM3FV"] = 241] = "UNIFORM3FV";
    FUNCTION_ID[FUNCTION_ID["UNIFORM3I"] = 242] = "UNIFORM3I";
    FUNCTION_ID[FUNCTION_ID["UNIFORM3IV"] = 243] = "UNIFORM3IV";
    FUNCTION_ID[FUNCTION_ID["UNIFORM4F"] = 244] = "UNIFORM4F";
    FUNCTION_ID[FUNCTION_ID["UNIFORM4FV"] = 245] = "UNIFORM4FV";
    FUNCTION_ID[FUNCTION_ID["UNIFORM4I"] = 246] = "UNIFORM4I";
    FUNCTION_ID[FUNCTION_ID["UNIFORM4IV"] = 247] = "UNIFORM4IV";
    FUNCTION_ID[FUNCTION_ID["UNIFORMMATRIX2FV"] = 248] = "UNIFORMMATRIX2FV";
    FUNCTION_ID[FUNCTION_ID["UNIFORMMATRIX3FV"] = 249] = "UNIFORMMATRIX3FV";
    FUNCTION_ID[FUNCTION_ID["UNIFORMMATRIX4FV"] = 250] = "UNIFORMMATRIX4FV";
    FUNCTION_ID[FUNCTION_ID["USEPROGRAM"] = 251] = "USEPROGRAM";
    FUNCTION_ID[FUNCTION_ID["VALIDATEPROGRAM"] = 252] = "VALIDATEPROGRAM";
    FUNCTION_ID[FUNCTION_ID["VERTEXATTRIB1F"] = 253] = "VERTEXATTRIB1F";
    FUNCTION_ID[FUNCTION_ID["VERTEXATTRIB1FV"] = 254] = "VERTEXATTRIB1FV";
    FUNCTION_ID[FUNCTION_ID["VERTEXATTRIB2F"] = 255] = "VERTEXATTRIB2F";
    FUNCTION_ID[FUNCTION_ID["VERTEXATTRIB2FV"] = 256] = "VERTEXATTRIB2FV";
    FUNCTION_ID[FUNCTION_ID["VERTEXATTRIB3F"] = 257] = "VERTEXATTRIB3F";
    FUNCTION_ID[FUNCTION_ID["VERTEXATTRIB3FV"] = 258] = "VERTEXATTRIB3FV";
    FUNCTION_ID[FUNCTION_ID["VERTEXATTRIB4F"] = 259] = "VERTEXATTRIB4F";
    FUNCTION_ID[FUNCTION_ID["VERTEXATTRIB4FV"] = 260] = "VERTEXATTRIB4FV";
    FUNCTION_ID[FUNCTION_ID["VERTEXATTRIBPOINTER"] = 261] = "VERTEXATTRIBPOINTER";
    FUNCTION_ID[FUNCTION_ID["VIEWPORT"] = 262] = "VIEWPORT";
    FUNCTION_ID[FUNCTION_ID["CONFIGUREBACKBUFFER"] = 263] = "CONFIGUREBACKBUFFER";
    FUNCTION_ID[FUNCTION_ID["COMPRESSEDTEXIMAGE2D"] = 264] = "COMPRESSEDTEXIMAGE2D";
    FUNCTION_ID[FUNCTION_ID["TEXIMAGE2D_PIXEL"] = 265] = "TEXIMAGE2D_PIXEL";
    FUNCTION_ID[FUNCTION_ID["TEXSUBIMAGE2D_PIXEL"] = 266] = "TEXSUBIMAGE2D_PIXEL";
    FUNCTION_ID[FUNCTION_ID["CREATEVERTEXARRAY"] = 267] = "CREATEVERTEXARRAY";
    FUNCTION_ID[FUNCTION_ID["BINDVERTEXARRAY"] = 268] = "BINDVERTEXARRAY";
    FUNCTION_ID[FUNCTION_ID["DELETEVERTEXARRAYS"] = 269] = "DELETEVERTEXARRAYS";
    FUNCTION_ID[FUNCTION_ID["READPIXELS_ASYNC"] = 270] = "READPIXELS_ASYNC";
    FUNCTION_ID[FUNCTION_ID["COMPRESSEDTEXSUBIMAGE2D"] = 271] = "COMPRESSEDTEXSUBIMAGE2D";
    FUNCTION_ID[FUNCTION_ID["VERTEXATTRIBDIVISOR"] = 272] = "VERTEXATTRIBDIVISOR";
    FUNCTION_ID[FUNCTION_ID["DRAWARRAYSINSTANCED"] = 273] = "DRAWARRAYSINSTANCED";
    FUNCTION_ID[FUNCTION_ID["DRAWELEMENTSINSTANCED"] = 274] = "DRAWELEMENTSINSTANCED";
    FUNCTION_ID[FUNCTION_ID["TEXIMAGE2DCANVAS"] = 275] = "TEXIMAGE2DCANVAS";
    FUNCTION_ID[FUNCTION_ID["TEXSTORAGE2D"] = 276] = "TEXSTORAGE2D";
    FUNCTION_ID[FUNCTION_ID["RENDERBUFFERSTORAGEMUILTISAMPLE"] = 277] = "RENDERBUFFERSTORAGEMUILTISAMPLE";
    FUNCTION_ID[FUNCTION_ID["CLEARBUFFERFV"] = 278] = "CLEARBUFFERFV";
    FUNCTION_ID[FUNCTION_ID["BLITFRAMEBUFFER"] = 279] = "BLITFRAMEBUFFER";
})(FUNCTION_ID || (FUNCTION_ID = {}));
var UNIFORM_TYPE;
(function (UNIFORM_TYPE) {
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM1F"] = 0] = "INTERIOR_UNIFORM1F";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM1FV"] = 1] = "INTERIOR_UNIFORM1FV";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM1I"] = 2] = "INTERIOR_UNIFORM1I";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM1IV"] = 3] = "INTERIOR_UNIFORM1IV";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM2F"] = 4] = "INTERIOR_UNIFORM2F";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM2FV"] = 5] = "INTERIOR_UNIFORM2FV";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM2I"] = 6] = "INTERIOR_UNIFORM2I";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM2IV"] = 7] = "INTERIOR_UNIFORM2IV";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM3F"] = 8] = "INTERIOR_UNIFORM3F";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM3FV"] = 9] = "INTERIOR_UNIFORM3FV";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM3I"] = 10] = "INTERIOR_UNIFORM3I";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM3IV"] = 11] = "INTERIOR_UNIFORM3IV";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM4F"] = 12] = "INTERIOR_UNIFORM4F";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM4FV"] = 13] = "INTERIOR_UNIFORM4FV";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM4I"] = 14] = "INTERIOR_UNIFORM4I";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORM4IV"] = 15] = "INTERIOR_UNIFORM4IV";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORMMATRIX2FV"] = 16] = "INTERIOR_UNIFORMMATRIX2FV";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORMMATRIX3FV"] = 17] = "INTERIOR_UNIFORMMATRIX3FV";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORMMATRIX4FV"] = 18] = "INTERIOR_UNIFORMMATRIX4FV";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORMSAMPLER_2D"] = 19] = "INTERIOR_UNIFORMSAMPLER_2D";
    UNIFORM_TYPE[UNIFORM_TYPE["INTERIOR_UNIFORMSAMPLER_CUBE"] = 20] = "INTERIOR_UNIFORMSAMPLER_CUBE";
})(UNIFORM_TYPE || (UNIFORM_TYPE = {}));
class BindInfo {
    constructor() {
        this.m_bindMap = {};
    }
    setBind(pname, buffer) {
        this.m_bindMap[pname] = buffer;
    }
    getBind(pname) {
        var buffer = this.m_bindMap[pname];
        if (!buffer)
            return null;
        if (buffer.id == -1) {
            this.m_bindMap[pname] = null;
            return null;
        }
        return buffer;
    }
}
class ANGLEInstancedArrays {
    constructor(encoder) {
        this.encoder = encoder;
    }
    vertexAttribDivisorANGLE(index, divisor) {
        this.encoder.vertexAttribDivisor(index, divisor);
    }
    drawArraysInstancedANGLE(mode, first, count, instanceCount) {
        this.encoder.drawArraysInstanced(mode, first, count, instanceCount);
    }
    drawElementsInstancedANGLE(mode, count, type, offset, instanceCount) {
        this.encoder.drawElementsInstanced(mode, count, type, offset, instanceCount);
    }
}
class OESVertexArrayObject {
    constructor(encoder) {
        this.encoder = encoder;
    }
    createVertexArrayOES() {
        return this.encoder.createVertexArray();
    }
    deleteVertexArrayOES(arrayObject) {
        this.encoder.deleteVertexArray(arrayObject);
    }
    isVertexArrayOES(arrayObject) {
        return this.encoder.isVertexArray(arrayObject);
    }
    bindVertexArrayOES(arrayObject) {
        this.encoder.bindVertexArray(arrayObject);
    }
}
class OESTextureHalfFloatLinear {
}
class EXTColorBufferFloat {
}
class WebGLBuffer {
    constructor(id) {
        this.id = id;
    }
}
class WebGLFrameBuffer {
    constructor(id) {
        this.id = id;
        this.m_bindInfo = new BindInfo();
    }
    setBind(pname, buffer) {
        this.m_bindInfo.setBind(pname, buffer);
    }
    getBind(pname) {
        return this.m_bindInfo.getBind(pname);
    }
}
class WebGLRenderBuffer {
    constructor(id) {
        this.id = id;
    }
}
class WebGLShader {
    constructor(id, type) {
        this.id = id;
        this.type = type;
        this.compileStatus = 0;
        this.src = "";
    }
}
class WebGLProgram {
    constructor(id) {
        this.id = id;
        this.linkStatus = 0;
        this.vsShader = null;
        this.psShader = null;
        this.attachedShaderCount = 0;
        this.m_localMap = {};
    }
    addLocationPair(fakeLoc, name) {
        this.m_localMap[fakeLoc] = name;
    }
    getLocationName(fakeLoc) {
        return this.m_localMap[fakeLoc];
    }
}
class WebGLTexture {
    constructor(id) {
        this.id = id;
    }
}
class WebGLVertextArray {
    constructor(id) {
        this.id = id;
    }
}
class CallbackFuncObj {
    constructor() {
        this._vFunc = new Map();
        this.id = CallbackFuncObj.s_nID++;
        this._nativeObj = new _callbackFuncObj(this.id);
    }
    addCallbackFunc(index, func) {
        this._vFunc[index] = func;
        this._nativeObj.addCallbackFunc(index, func);
    }
}
CallbackFuncObj.s_nID = 0;
class fakeIDObj {
    constructor(id = 0, frame = 0) {
        this.id = id;
        this.frameCount = frame;
    }
}
class GLCommandEncoder {
    constructor(layagl, reserveSize, adjustSize, isSyncToRenderThread) {
        this._adjustSize = 0;
        this._byteLen = 0;
        this._isSyncToRenderThread = false;
        this._readPixelsAsyncCallbackFuncObj = new CallbackFuncObj();
        this._isSyncToRenderThread = isSyncToRenderThread;
        this._layagl = layagl;
        this._byteLen = reserveSize;
        this._adjustSize = adjustSize;
        this._init(isSyncToRenderThread);
    }
    _init(isSyncToRenderThread) {
        this._buffer = new ArrayBuffer(this._byteLen);
        this._idata = new Int32Array(this._buffer);
        this._fdata = new Float32Array(this._buffer);
        this._byteArray = new Uint8Array(this._buffer);
        this._layagl.createArrayBufferRef(this._buffer, LayaGLContext.ARRAY_BUFFER_TYPE_CMD, isSyncToRenderThread);
        this._idata[0] = 1;
    }
    getPtrID() {
        return this._buffer["_ptrID"];
    }
    beginEncoding() {
        this._layagl.beginCommandEncoding(this);
    }
    endEncoding() {
        this._layagl.endCommandEncoding();
    }
    clearEncoding() {
        this._idata[0] = 1;
    }
    getCount() {
        return this._idata[0];
    }
    _need(sz) {
        if ((this._byteLen - (this._idata[0] << 2)) >= sz)
            return;
        this._byteLen += (sz > this._adjustSize) ? sz : this._adjustSize;
        var pre = this._idata;
        var preConchRef = this._buffer["conchRef"];
        var prePtrID = this._buffer["_ptrID"];
        this._buffer = new ArrayBuffer(this._byteLen);
        this._idata = new Int32Array(this._buffer);
        this._fdata = new Float32Array(this._buffer);
        this._byteArray = new Uint8Array(this._buffer);
        this._buffer["conchRef"] = preConchRef;
        this._buffer["_ptrID"] = prePtrID;
        pre && this._idata.set(pre, 0);
        webglPlus.updateArrayBufferRef(this._buffer["_ptrID"], preConchRef.isSyncToRender(), this._buffer);
    }
    add_i(i) {
        this._need(4);
        this._idata[this._idata[0]++] = i;
    }
    add_f(a) {
        this._need(4);
        this._fdata[this._idata[0]++] = a;
    }
    add_ff(a, b) {
        this._need(8);
        var i = this._idata[0];
        this._fdata[i++] = a;
        this._fdata[i++] = b;
        this._idata[0] = i;
    }
    add_fff(a, b, c) {
        this._need(12);
        var i = this._idata[0];
        this._fdata[i++] = a;
        this._fdata[i++] = b;
        this._fdata[i++] = c;
        this._idata[0] = i;
    }
    add_ii(a, b) {
        this._need(8);
        var i = this._idata[0];
        this._idata[i++] = a;
        this._idata[i++] = b;
        this._idata[0] = i;
    }
    add_if(a, b) {
        this._need(8);
        var i = this._idata[0];
        this._idata[i++] = a;
        this._fdata[i++] = b;
        this._idata[0] = i;
    }
    add_iii(a, b, c) {
        this._need(12);
        var idata = this._idata;
        var i = this._idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        this._idata[0] = i;
    }
    add_iif(a, b, c) {
        this._need(12);
        var idata = this._idata;
        var i = this._idata[0];
        idata[i++] = a;
        idata[i++] = b;
        this._fdata[i++] = c;
        this._idata[0] = i;
    }
    add_ifi(a, b, c) {
        this._need(12);
        var idata = this._idata;
        var i = this._idata[0];
        idata[i++] = a;
        this._fdata[i++] = b;
        idata[i++] = c;
        this._idata[0] = i;
    }
    add_iifi(a, b, c, d) {
        this._need(16);
        var idata = this._idata;
        var i = this._idata[0];
        idata[i++] = a;
        idata[i++] = b;
        this._fdata[i++] = c;
        idata[i++] = d;
        this._idata[0] = i;
    }
    add_iiii(a, b, c, d) {
        this._need(16);
        var idata = this._idata;
        var i = this._idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        idata[0] = i;
    }
    add_iiif(a, b, c, d) {
        this._need(16);
        var idata = this._idata;
        var i = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        this._fdata[i++] = d;
        idata[0] = i;
    }
    add_iiiif(a, b, c, d, e) {
        this._need(20);
        var idata = this._idata;
        var i = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        this._fdata[i++] = e;
        idata[0] = i;
    }
    add_iiiff(a, b, c, d, e) {
        this._need(20);
        var idata = this._idata;
        var i = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        this._fdata[i++] = d;
        this._fdata[i++] = e;
        idata[0] = i;
    }
    add_iiiiff(a, b, c, d, e, f) {
        this._need(24);
        var idata = this._idata;
        var i = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        this._fdata[i++] = e;
        this._fdata[i++] = f;
        idata[0] = i;
    }
    add_iiifff(a, b, c, d, e, f) {
        this._need(24);
        var idata = this._idata;
        var i = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        this._fdata[i++] = d;
        this._fdata[i++] = e;
        this._fdata[i++] = f;
        idata[0] = i;
    }
    add_iiiifff(a, b, c, d, e, f, g) {
        this._need(28);
        var idata = this._idata;
        var i = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        this._fdata[i++] = e;
        this._fdata[i++] = f;
        this._fdata[i++] = g;
        idata[0] = i;
    }
    add_iiiffff(a, b, c, d, e, f, g) {
        this._need(28);
        var idata = this._idata;
        var i = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        this._fdata[i++] = d;
        this._fdata[i++] = e;
        this._fdata[i++] = f;
        this._fdata[i++] = g;
        idata[0] = i;
    }
    add_iiiiffff(a, b, c, d, e, f, g, h) {
        this._need(32);
        var idata = this._idata;
        var i = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        this._fdata[i++] = e;
        this._fdata[i++] = f;
        this._fdata[i++] = g;
        this._fdata[i++] = h;
        idata[0] = i;
    }
    add_iiff(a, b, c, d) {
        this._need(16);
        var idata = this._idata;
        var fdata = this._fdata;
        var i = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        fdata[i++] = c;
        fdata[i++] = d;
        idata[0] = i;
    }
    add_iifff(a, b, c, d, e) {
        this._need(20);
        var idata = this._idata;
        var fdata = this._fdata;
        var i = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        fdata[i++] = c;
        fdata[i++] = d;
        fdata[i++] = e;
        idata[0] = i;
    }
    add_iiffff(a, b, c, d, e, f) {
        this._need(24);
        var idata = this._idata;
        var fdata = this._fdata;
        var i = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        fdata[i++] = c;
        fdata[i++] = d;
        fdata[i++] = e;
        fdata[i++] = f;
        idata[0] = i;
    }
    add_iiiii(a, b, c, d, e) {
        this._need(20);
        var idata = this._idata;
        var i = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        idata[i++] = e;
        idata[0] = i;
    }
    add_iiiiii(a, b, c, d, e, f) {
        this._need(24);
        var idata = this._idata;
        var i = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        idata[i++] = e;
        idata[i++] = f;
        idata[0] = i;
    }
    add_iiiiiii(a, b, c, d, e, f, g) {
        this._need(28);
        var idata = this._idata;
        var i = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        idata[i++] = e;
        idata[i++] = f;
        idata[i++] = g;
        idata[0] = i;
    }
    add_iiiiiiiiiiii(a, b, c, d, e, f, g, h, j, k, l, m) {
        this._need(48);
        var idata = this._idata;
        var i = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        idata[i++] = e;
        idata[i++] = f;
        idata[i++] = g;
        idata[i++] = h;
        idata[i++] = j;
        idata[i++] = k;
        idata[i++] = l;
        idata[i++] = m;
        idata[0] = i;
    }
    add_iiiiiiiiiiiii(a, b, c, d, e, f, g, h, j, k, l, m, n) {
        this._need(52);
        var idata = this._idata;
        var i = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        idata[i++] = e;
        idata[i++] = f;
        idata[i++] = g;
        idata[i++] = h;
        idata[i++] = j;
        idata[i++] = k;
        idata[i++] = l;
        idata[i++] = m;
        idata[i++] = n;
        idata[0] = i;
    }
    add_iff(a, b, c) {
        this._need(12);
        var i = this._idata[0];
        this._idata[i++] = a;
        this._fdata[i++] = b;
        this._fdata[i++] = c;
        this._idata[0] = i;
    }
    add_iffi(a, b, c, d) {
        this._need(16);
        var i = this._idata[0];
        this._idata[i++] = a;
        this._fdata[i++] = b;
        this._fdata[i++] = c;
        this._idata[i++] = d;
        this._idata[0] = i;
    }
    add_ifffi(a, b, c, d, e) {
        this._need(20);
        var i = this._idata[0];
        this._idata[i++] = a;
        this._fdata[i++] = b;
        this._fdata[i++] = c;
        this._fdata[i++] = d;
        this._idata[i++] = e;
        this._idata[0] = i;
    }
    add_iffff(a, b, c, d, e) {
        this._need(20);
        var i = this._idata[0];
        var fdata = this._fdata;
        this._idata[i++] = a;
        fdata[i++] = b;
        fdata[i++] = c;
        fdata[i++] = d;
        fdata[i++] = e;
        this._idata[0] = i;
    }
    add_iffffi(a, b, c, d, e, f) {
        this._need(24);
        var i = this._idata[0];
        var fdata = this._fdata;
        this._idata[i++] = a;
        fdata[i++] = b;
        fdata[i++] = c;
        fdata[i++] = d;
        fdata[i++] = e;
        this._idata[i++] = f;
        this._idata[0] = i;
    }
    add_ifffffi(a, b, c, d, e, f, g) {
        this._need(28);
        var i = this._idata[0];
        var fdata = this._fdata;
        this._idata[i++] = a;
        fdata[i++] = b;
        fdata[i++] = c;
        fdata[i++] = d;
        fdata[i++] = e;
        fdata[i++] = f;
        this._idata[i++] = g;
        this._idata[0] = i;
    }
    add_iffffiif(a, b, c, d, e, f, g, h) {
        this._need(28);
        var i = this._idata[0];
        var fdata = this._fdata;
        this._idata[i++] = a;
        fdata[i++] = b;
        fdata[i++] = c;
        fdata[i++] = d;
        fdata[i++] = e;
        this._idata[i++] = f;
        this._idata[i++] = g;
        fdata[i++] = h;
        this._idata[0] = i;
    }
    add_iffffiii(a, b, c, d, e, f, g, h) {
        this._need(32);
        var i = this._idata[0];
        var fdata = this._fdata;
        this._idata[i++] = a;
        fdata[i++] = b;
        fdata[i++] = c;
        fdata[i++] = d;
        fdata[i++] = e;
        this._idata[i++] = f;
        this._idata[i++] = g;
        this._idata[i++] = h;
        this._idata[0] = i;
    }
    add_iiiiiiii(a, b, c, d, e, f, g, h) {
        this._need(32);
        var i = this._idata[0];
        var idata = this._idata;
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        idata[i++] = e;
        idata[i++] = f;
        idata[i++] = g;
        idata[i++] = h;
        idata[0] = i;
    }
    add_iiiiiiiii(a, b, c, d, e, f, g, h, j) {
        this._need(36);
        var idata = this._idata;
        var i = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        idata[i++] = e;
        idata[i++] = f;
        idata[i++] = g;
        idata[i++] = h;
        idata[i++] = j;
        idata[0] = i;
    }
    add_iiiiiiiiii(a, b, c, d, e, f, g, h, j, k) {
        this._need(40);
        var idata = this._idata;
        var i = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        idata[i++] = e;
        idata[i++] = f;
        idata[i++] = g;
        idata[i++] = h;
        idata[i++] = j;
        idata[i++] = k;
        idata[0] = i;
    }
    add_iiiiiiiiiii(a, b, c, d, e, f, g, h, j, k, l) {
        this._need(44);
        var idata = this._idata;
        var i = idata[0];
        idata[i++] = a;
        idata[i++] = b;
        idata[i++] = c;
        idata[i++] = d;
        idata[i++] = e;
        idata[i++] = f;
        idata[i++] = g;
        idata[i++] = h;
        idata[i++] = j;
        idata[i++] = k;
        idata[i++] = l;
        idata[0] = i;
    }
    add_String(str, len) {
        var ab = conch.strTobufer(str);
        this._need(len + 4);
        this._idata[this._idata[0]++] = len;
        if (len == 0)
            return;
        var uint8array = new Uint8Array(ab);
        this._byteArray.set(uint8array, this._idata[0] * 4);
        this._idata[0] += len / 4;
    }
    wab(arraybuffer, length, nAlignLength, offset) {
        offset = offset ? offset : 0;
        this._need(nAlignLength + 4);
        this._idata[this._idata[0]++] = length;
        var uint8array = null;
        if (arraybuffer instanceof Float32Array && offset == 0) {
            this._fdata.set(arraybuffer, this._idata[0]);
        }
        else {
            if (arraybuffer instanceof ArrayBuffer) {
                uint8array = new Uint8Array(arraybuffer, offset, length);
            }
            else if (arraybuffer.buffer) {
                uint8array = new Uint8Array(arraybuffer.buffer, offset + arraybuffer.byteOffset, length);
            }
            else {
                console.log("not arraybuffer/dataview");
                return;
            }
            this._byteArray.set(uint8array, this._idata[0] * 4);
        }
        this._idata[0] += nAlignLength / 4;
    }
    copyBuffer(arraybuffer, length, offset) {
        offset = offset ? offset : 0;
        this._need(length);
        var ab = null;
        if (arraybuffer instanceof ArrayBuffer) {
            ab = arraybuffer;
        }
        else if (arraybuffer.buffer) {
            ab = arraybuffer.buffer;
        }
        else {
            console.log("not arraybuffer/dataview ");
            return;
        }
        var uint8array = new Uint8Array(ab, offset, length);
        this._byteArray.set(uint8array, this._idata[0] * 4);
        this._idata[0] += length / 4;
    }
    getContextAttributes() {
        return { alpha: false, antialias: false, depth: true, failIfMajorPerformanceCaveat: false, premultipliedAlpha: true, preserveDrawingBuffer: false, stencil: false };
    }
    isContextLost() {
        console.log("isContextLost can't support");
        return false;
    }
    getSupportedExtensions() {
        let extentions = this._layagl._nativeObj.getStringEx(0x1F03).split(' ');
        let version = this._layagl._nativeObj.getStringEx(0x1F02);
        function supports(extention) {
            for (let ext of extentions) {
                if (ext === extention) {
                    return true;
                }
            }
            return false;
        }
        let result = [];
        if (supports("GL_EXT_blend_minmax"))
            result.push("EXT_blend_minmax");
        if (supports("GL_EXT_sRGB"))
            result.push("EXT_sRGB");
        if (supports("GL_EXT_frag_depth"))
            result.push("EXT_frag_depth");
        if (supports("GL_OES_texture_float"))
            result.push("OES_texture_float");
        if (supports("GL_OES_texture_float_linear"))
            result.push("OES_texture_float_linear");
        if (supports("GL_OES_texture_half_float"))
            result.push("OES_texture_half_float");
        if (supports("GL_OES_texture_half_float_linear"))
            result.push("OES_texture_half_float_linear");
        if (supports("GL_OES_standard_derivatives"))
            result.push("OES_standard_derivatives");
        if (supports("GL_EXT_shader_texture_lod") || supports("GL_ARB_shader_texture_lod"))
            result.push("EXT_shader_texture_lod");
        if (supports("GL_EXT_texture_filter_anisotropic"))
            result.push("EXT_texture_filter_anisotropic");
        if (supports("GL_OES_vertex_array_object"))
            result.push("OES_vertex_array_object");
        if (supports("GL_OES_element_index_uint"))
            result.push("OES_element_index_uint");
        if (supports("GL_ANGLE_translated_shader_source"))
            result.push("WEBGL_debug_shaders");
        if (supports("GL_AMD_compressed_ATC_texture"))
            result.push("WEBGL_compressed_texture_atc");
        if (supports("GL_IMG_texture_compression_pvrtc"))
            result.push("WEBGL_compressed_texture_pvrtc");
        if (supports("GL_EXT_texture_compression_s3tc") || supports("GL_EXT_texture_compression_dxt1"))
            result.push("WEBGL_compressed_texture_s3tc");
        if (supports("GL_OES_depth_texture") || supports("GL_ARB_depth_texture") || supports("GL_ANGLE_depth_texture"))
            result.push("WEBGL_depth_texture");
        if (version.indexOf("OpenGL ES 3.") != -1) {
            if (conchConfig.getOS() == "Conch-android") {
                result.push("WEBGL_compressed_texture_etc");
            }
        }
        if (supports("ANGLE_instanced_arrays"))
            result.push("ANGLE_instanced_arrays");
        if (supports("GL_EXT_color_buffer_float"))
            result.push("EXT_color_buffer_float");
        if (supports("GL_EXT_color_buffer_half_float"))
            result.push("EXT_color_buffer_half_float");
        if (supports("GL_EXT_disjoint_timer_query"))
            result.push("EXT_disjoint_timer_query");
        if (supports("GL_OES_compressed_ETC1_RGB8_texture"))
            result.push("WEBGL_compressed_texture_etc1");
        if (supports("GL_EXT_texture_compression_s3tc"))
            result.push("WEBGL_compressed_texture_s3tc");
        if (supports("GL_EXT_texture_compression_s3tc_srgb"))
            result.push("WEBGL_compressed_texture_s3tc_srgb");
        if (supports("GL_OES_texture_compression_astc"))
            result.push("WEBGL_compressed_texture_astc");
        result.push("WEBGL_debug_renderer_info");
        result.push("WEBGL_lose_context");
        return result;
    }
    getExtension(name) {
        var extention = this._layagl._nativeObj.getStringEx(0x1F03);
        let version = this._layagl._nativeObj.getStringEx(0x1F02);
        let extentions = extention.split(' ');
        function supports(extention) {
            for (let ext of extentions) {
                if (ext === extention) {
                    return true;
                }
            }
            return false;
        }
        if (name === 'EXT_blend_minmax' && extention.indexOf('GL_EXT_blend_minmax') != -1) {
            return {
                MAX_EXT: 32776,
                MIN_EXT: 32775,
            };
        }
        else if (name === 'EXT_color_buffer_float' && extention.indexOf('GL_EXT_color_buffer_float') != -1) {
            return {};
        }
        else if (name === 'EXT_color_buffer_half_float' && extention.indexOf('GL_EXT_color_buffer_half_float') != -1) {
            return {};
        }
        else if (name === 'EXT_shader_texture_lod' &&
            (extention.indexOf('GL_EXT_shader_texture_lod') != -1 || extention.indexOf('GL_ARB_shader_texture_lod') != -1)) {
            return {};
        }
        else if (name === 'OES_element_index_uint' && (extention.indexOf('GL_OES_element_index_uint') != -1)) {
            return {};
        }
        else if (name === 'EXT_sRGB' && extention.indexOf('GL_EXT_sRGB') != -1) {
            return {
                SRGB_EXT: 0x8C40,
                SRGB_ALPHA_EXT: 0x8C42,
                SRGB8_ALPHA8_EXT: 0x8C43,
                FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING_EXT: 0x8210
            };
        }
        else if (name === 'WEBGL_compressed_texture_pvrtc'
            && extention.indexOf('GL_IMG_texture_compression_pvrtc') != -1) {
            return {
                COMPRESSED_RGBA_PVRTC_2BPPV1_IMG: 35843,
                COMPRESSED_RGBA_PVRTC_4BPPV1_IMG: 35842,
                COMPRESSED_RGB_PVRTC_2BPPV1_IMG: 35841,
                COMPRESSED_RGB_PVRTC_4BPPV1_IMG: 35840,
            };
        }
        else if (name === 'WEBGL_compressed_texture_etc1'
            && extention.indexOf('GL_OES_compressed_ETC1_RGB8_texture') != -1) {
            return { COMPRESSED_RGB_ETC1_WEBGL: 36196 };
        }
        else if (name === 'WEBGL_compressed_texture_s3tc'
            && supports('GL_EXT_texture_compression_s3tc')) {
            return {
                COMPRESSED_RGBA_S3TC_DXT1_EXT: 33777,
                COMPRESSED_RGBA_S3TC_DXT3_EXT: 33778,
                COMPRESSED_RGBA_S3TC_DXT5_EXT: 33779,
                COMPRESSED_RGB_S3TC_DXT1_EXT: 33776,
            };
        }
        else if (name === 'WEBGL_compressed_texture_s3tc_srgb'
            && extention.indexOf('GL_EXT_texture_compression_s3tc_srgb') != -1) {
            return {
                COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT: 35917,
                COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT: 35918,
                COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT: 35919,
                COMPRESSED_SRGB_S3TC_DXT1_EXT: 35916,
            };
        }
        else if (name === 'WEBGL_debug_renderer_info') {
            return {
                UNMASKED_RENDERER_WEBGL: 37446,
                UNMASKED_VENDOR_WEBGL: 37445,
            };
        }
        else if (name === 'EXT_texture_filter_anisotropic'
            && extention.indexOf('GL_EXT_texture_filter_anisotropic') != -1) {
            return {
                MAX_TEXTURE_MAX_ANISOTROPY_EXT: 34047,
                TEXTURE_MAX_ANISOTROPY_EXT: 34046,
            };
        }
        else if (name.indexOf('ANGLE_instanced_arrays') != -1 && ((extention.indexOf('_instanced_arrays') != -1) || version.indexOf("OpenGL ES 3.") != -1)) {
            return new ANGLEInstancedArrays(this);
        }
        else if (name.indexOf('OES_vertex_array_object') != -1 && ((extention.indexOf('GL_OES_vertex_array_object') != -1) || version.indexOf("OpenGL ES 3.") != -1)) {
            return new OESVertexArrayObject(this);
        }
        else if (name.indexOf('OES_texture_float') != -1 && extention.indexOf('GL_OES_texture_float') != -1) {
            return {};
        }
        else if (name.indexOf('OES_texture_half_float') != -1 && (extention.indexOf('GL_OES_texture_half_float') != -1)) {
            return { HALF_FLOAT_OES: 36193 };
        }
        else if (name.indexOf('OES_texture_half_float_linear') != -1 && extention.indexOf('GL_OES_texture_half_float_linear') != -1) {
            return new OESTextureHalfFloatLinear();
        }
        else if (name.indexOf('EXT_color_buffer_float') != -1 && extention.indexOf('GL_EXT_color_buffer_float') != -1) {
            return new EXTColorBufferFloat();
        }
        else if (name.indexOf('WEBGL_depth_texture') != -1 && ((supports("GL_OES_depth_texture") || supports("GL_ARB_depth_texture") || supports("GL_ANGLE_depth_texture")))) {
            return { UNSIGNED_INT_24_8_WEBGL: 34042 };
        }
        else if (name.indexOf('WEBGL_compressed_texture_astc') != -1 && (extention.indexOf('GL_OES_texture_compression_astc') != -1 || extention.indexOf('GL_KHR_texture_compression_astc') != -1 || (conchConfig.getOS() == "Conch-android" && version.indexOf("OpenGL ES 3.") != -1 && version.indexOf("OpenGL ES 3.0") == -1))) {
            return {
                COMPRESSED_RGBA_ASTC_4x4_KHR: 0x93B0,
                COMPRESSED_RGBA_ASTC_5x4_KHR: 0x93B1,
                COMPRESSED_RGBA_ASTC_5x5_KHR: 0x93B2,
                COMPRESSED_RGBA_ASTC_6x5_KHR: 0x93B3,
                COMPRESSED_RGBA_ASTC_6x6_KHR: 0x93B4,
                COMPRESSED_RGBA_ASTC_8x5_KHR: 0x93B5,
                COMPRESSED_RGBA_ASTC_8x6_KHR: 0x93B6,
                COMPRESSED_RGBA_ASTC_8x8_KHR: 0x93B7,
                COMPRESSED_RGBA_ASTC_10x5_KHR: 0x93B8,
                COMPRESSED_RGBA_ASTC_10x6_KHR: 0x93B9,
                COMPRESSED_RGBA_ASTC_10x8_KHR: 0x93BA,
                COMPRESSED_RGBA_ASTC_10x10_KHR: 0x93BB,
                COMPRESSED_RGBA_ASTC_12x10_KHR: 0x93BC,
                COMPRESSED_RGBA_ASTC_12x12_KHR: 0x93BD,
                COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR: 0x93D0,
                COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR: 0x93D1,
                COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR: 0x93D2,
                COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR: 0x93D3,
                COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR: 0x93D4,
                COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR: 0x93D5,
                COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR: 0x93D6,
                COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR: 0x93D7,
                COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR: 0x93D8,
                COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR: 0x93D9,
                COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR: 0x93DA,
                COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR: 0x93DB,
                COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR: 0x93DC,
                COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR: 0x93DD,
            };
        }
        else if (name.indexOf('WEBGL_compressed_texture_etc') != -1 && conchConfig.getOS() == "Conch-android" && version.indexOf("OpenGL ES 3.") != -1) {
            return {
                COMPRESSED_R11_EAC: 0x9270,
                COMPRESSED_SIGNED_R11_EAC: 0x9271,
                COMPRESSED_RG11_EAC: 0x9272,
                COMPRESSED_SIGNED_RG11_EAC: 0x9273,
                COMPRESSED_RGB8_ETC2: 0x9274,
                COMPRESSED_SRGB8_ETC2: 0x9275,
                COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2: 0x9276,
                COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2: 0x9277,
                COMPRESSED_RGBA8_ETC2_EAC: 0x9278,
                COMPRESSED_SRGB8_ALPHA8_ETC2_EAC: 0x9279
            };
        }
        else {
            console.log("getExtension " + name + " 尚未支持");
        }
        return null;
    }
    activeTexture(texture) {
        this.add_ii(FUNCTION_ID.ACTIVETEXTURE, texture);
    }
    attachShader(program, shader) {
        if (shader.type == LayaGLContext.VERTEX_SHADER) {
            if (program.vsShader)
                return;
            program.vsShader = shader;
            program.attachedShaderCount++;
        }
        else if (shader.type == LayaGLContext.FRAGMENT_SHADER) {
            if (program.psShader)
                return;
            program.psShader = shader;
            program.attachedShaderCount++;
        }
        this.add_iii(FUNCTION_ID.ATTACHSHADER, program.id, shader.id);
    }
    bindAttribLocation(program, index, name) {
        var strLen = this.getStrLength(name);
        this.add_iii(FUNCTION_ID.BINDATTRIBLOCATION, program.id, index);
        this.add_String(name, strLen);
    }
    bindBuffer(target, buffer) {
        this.add_iii(FUNCTION_ID.BINDBUFFER, target, buffer ? buffer.id : 0);
    }
    bindFramebuffer(target, framebuffer) {
        this.add_iii(FUNCTION_ID.BINDFRAMEBUFFER, target, framebuffer ? framebuffer.id : 0);
    }
    bindRenderbuffer(target, renderbuffer) {
        this.add_iii(FUNCTION_ID.BINDRENDERBUFFER, target, renderbuffer ? renderbuffer.id : 0);
    }
    clearBufferfv(buffer, drawbuffer, values, srcOffset) {
        let offset = srcOffset ? srcOffset : 0;
        this.add_iiiiiii(FUNCTION_ID.CLEARBUFFERFV, buffer, drawbuffer, values[0 + offset], values[1 + offset], values[2 + offset], values[3 + offset]);
    }
    blitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter) {
        this.add_iiiiiiiiiii(FUNCTION_ID.BLITFRAMEBUFFER, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
    }
    bindTexture(target, texture) {
        this.add_iii(FUNCTION_ID.BINDTEXTURE, target, texture ? texture.id : 0);
    }
    useTexture(texture) {
        this.add_ii(FUNCTION_ID.USETEXTURE, texture ? texture.id : 0);
    }
    blendColor(red, green, blue, alpha) {
        this.add_iffff(FUNCTION_ID.BLENDCOLOR, red, green, blue, alpha);
    }
    blendEquation(mode) {
        this.add_ii(FUNCTION_ID.BLENDEQUATION, mode);
    }
    blendEquationSeparate(modeRGB, modeAlpha) {
        this.add_iii(FUNCTION_ID.BLENDEQUATIONSEPARATE, modeRGB, modeAlpha);
    }
    blendFunc(sfactor, dfactor) {
        this.add_iii(FUNCTION_ID.BLENDFUNC, sfactor, dfactor);
    }
    blendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha) {
        this.add_iiiii(FUNCTION_ID.BLENDFUNCSEPARATE, srcRGB, dstRGB, srcAlpha, dstAlpha);
    }
    bufferData(target, sizeOrArray, usage) {
        if (ArrayBuffer.isView(sizeOrArray) || (sizeOrArray instanceof ArrayBuffer)) {
            var nAlignLength = this.getAlignLength(sizeOrArray);
            this.add_iii(FUNCTION_ID.BUFFERDATA_ARRAYBUFFER, target, usage);
            this.wab(sizeOrArray, sizeOrArray.byteLength, nAlignLength);
        }
        else {
            var size = sizeOrArray;
            this.add_iiii(FUNCTION_ID.BUFFERDATA_SIZE, target, size, usage);
        }
    }
    bufferSubData(target, offset, data) {
        var nAlignLength = this.getAlignLength(data);
        this.add_iii(FUNCTION_ID.BUFFERSUBDATA, target, offset);
        this.wab(data, data.byteLength, nAlignLength);
    }
    checkFramebufferStatus(target) {
        return this._layagl._nativeObj.checkFramebufferStatusEx(target);
    }
    clear(mask) {
        this.add_ii(FUNCTION_ID.CLEAR, mask);
    }
    clearColor(red, green, blue, alpha) {
        this.add_iffff(FUNCTION_ID.CLEARCOLOR, red, green, blue, alpha);
    }
    clearDepth(depth) {
        this.add_if(FUNCTION_ID.CLEARDEPTH, depth);
    }
    clearStencil(s) {
        this.add_ii(FUNCTION_ID.CLEARSTENCIL, s);
    }
    colorMask(red, green, blue, alpha) {
        this.add_iiiii(FUNCTION_ID.COLORMASK, red, green, blue, alpha);
    }
    compileShader(shader) {
        this.add_ii(FUNCTION_ID.COMPILESHADER, shader.id);
        shader.compileStatus = this._layagl._nativeObj.getShaderParameter(shader.src, shader.type, LayaGLContext.COMPILE_STATUS);
    }
    copyTexImage2D(target, level, internalformat, x, y, width, height, border) {
        this.add_iiiiiiiii(FUNCTION_ID.COPYTEXIMAGE2D, target, level, internalformat, x, y, width, height, border);
    }
    copyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height) {
        this.add_iiiiiiiii(FUNCTION_ID.COPYTEXSUBIMAGE2D, target, level, xoffset, yoffset, x, y, width, height);
    }
    createBuffer() {
        var fakeID = this.createFakeID();
        this.add_ii(FUNCTION_ID.CREATEBUFFER, fakeID);
        return new WebGLBuffer(fakeID);
    }
    createFramebuffer() {
        var fakeID = this.createFakeID();
        this.add_ii(FUNCTION_ID.CREATEFRAMEBUFFER, fakeID);
        return new WebGLFrameBuffer(fakeID);
    }
    createProgram() {
        var fakeID = this.createFakeID();
        this.add_ii(FUNCTION_ID.CREATEPROGRAM, fakeID);
        return new WebGLProgram(fakeID);
    }
    createRenderbuffer() {
        var fakeID = this.createFakeID();
        this.add_ii(FUNCTION_ID.CREATERENDERBUFFER, fakeID);
        return new WebGLRenderBuffer(fakeID);
    }
    createShader(type) {
        var fakeID = this.createFakeID();
        this.add_iii(FUNCTION_ID.CREATESHADER, fakeID, type);
        return new WebGLShader(fakeID, type);
    }
    createTexture() {
        var fakeID = this.createFakeID();
        this.add_ii(FUNCTION_ID.CREATETEXTURE, fakeID);
        return new WebGLTexture(fakeID);
    }
    cullFace(mode) {
        this.add_ii(FUNCTION_ID.CULLFACE, mode);
    }
    deleteBuffer(buffer) {
        this.deleteFakeID(buffer.id);
        this.add_ii(FUNCTION_ID.DELETEBUFFER, buffer.id);
    }
    deleteFramebuffer(framebuffer) {
        this.deleteFakeID(framebuffer.id);
        this.add_ii(FUNCTION_ID.DELETEFRAMEBUFFER, framebuffer.id);
    }
    deleteProgram(program) {
        this.deleteFakeID(program.id);
        this.add_ii(FUNCTION_ID.DELETEPROGRAM, program.id);
    }
    deleteRenderbuffer(renderbuffer) {
        this.deleteFakeID(renderbuffer.id);
        this.add_ii(FUNCTION_ID.DELETERENDERBUFFER, renderbuffer.id);
    }
    deleteShader(shader) {
        this.deleteFakeID(shader.id);
        this.add_ii(FUNCTION_ID.DELETESHADER, shader.id);
    }
    deleteTexture(texture) {
        this.deleteFakeID(texture.id);
        this.add_ii(FUNCTION_ID.DELETETEXTURE, texture.id);
    }
    depthFunc(func) {
        this.add_ii(FUNCTION_ID.DEPTHFUNC, func);
    }
    depthMask(flag) {
        this.add_ii(FUNCTION_ID.DEPTHMASK, flag);
    }
    depthRange(zNear, zFar) {
        this.add_iff(FUNCTION_ID.DEPTHRANGE, zNear, zFar);
    }
    detachShader(program, shader) {
        let shaderid = -1;
        if (program.vsShader && program.vsShader.id === shader.id) {
            shaderid = program.vsShader.id;
            program.vsShader = null;
        }
        else if (program.psShader && program.psShader.id === shader.id) {
            shaderid = program.psShader.id;
            program.psShader = null;
        }
        if (shaderid > 0) {
            program.attachedShaderCount--;
            this.add_iii(FUNCTION_ID.DETACHSHADER, program.id, shaderid);
        }
    }
    disable(cap) {
        this.add_ii(FUNCTION_ID.DISABLE, cap);
    }
    disableVertexAttribArray(index) {
        this.add_ii(FUNCTION_ID.DISABLEVERTEXATTRIBARRAY, index);
    }
    drawArrays(mode, first, count) {
        this.add_iiii(FUNCTION_ID.DRAWARRAYS, mode, first, count);
    }
    drawElements(mode, count, type, offset) {
        this.add_iiiii(FUNCTION_ID.DRAWELEMENTS, mode, count, type, offset);
    }
    enable(cap) {
        this.add_ii(FUNCTION_ID.ENABLE, cap);
    }
    enableVertexAttribArray(index) {
        this.add_ii(FUNCTION_ID.ENABLEVERTEXATTRIBARRAY, index);
    }
    finish() {
        this.add_i(FUNCTION_ID.FINISH);
    }
    flush() {
        this.add_i(FUNCTION_ID.FLUSH);
    }
    framebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer) {
        this.add_iiiii(FUNCTION_ID.FRAMEBUFFERRENDERBUFFER, target, attachment, renderbuffertarget, renderbuffer ? renderbuffer.id : 0);
    }
    framebufferTexture2D(target, attachment, textarget, texture, level) {
        this.add_iiiiii(FUNCTION_ID.FRAMEBUFFERTEXTURE2D, target, attachment, textarget, texture ? texture.id : 0, level);
    }
    frontFace(mode) {
        this.add_ii(FUNCTION_ID.FRONTFACE, mode);
    }
    generateMipmap(target) {
        this.add_ii(FUNCTION_ID.GENERATEMIPMAP, target);
    }
    getActiveAttrib(program, index) {
        return this._layagl._nativeObj.getActiveAttribEx(program.linkedVSSource, program.linkedPSSource, "", index);
    }
    getActiveUniform(program, index) {
        return this._layagl._nativeObj.getActiveUniformEx(program.linkedVSSource, program.linkedPSSource, "", index);
    }
    getAttribLocation(program, name) {
        return this._layagl._nativeObj.getAttribLocationEx(program.linkedVSSource, program.linkedPSSource, "", name);
    }
    getParameter(pname) {
        var gl = this._layagl;
        var result = 0;
        switch (pname) {
            case LayaGLContext.ACTIVE_TEXTURE:
            case LayaGLContext.ALPHA_BITS:
            case LayaGLContext.BLEND_DST_ALPHA:
            case LayaGLContext.BLEND_DST_RGB:
            case LayaGLContext.BLEND_EQUATION:
            case LayaGLContext.BLEND_EQUATION_ALPHA:
            case LayaGLContext.BLEND_EQUATION_RGB:
            case LayaGLContext.BLEND_SRC_ALPHA:
            case LayaGLContext.BLEND_SRC_RGB:
            case LayaGLContext.BLUE_BITS:
            case LayaGLContext.CULL_FACE_MODE:
            case LayaGLContext.DEPTH_BITS:
            case LayaGLContext.DEPTH_FUNC:
            case LayaGLContext.FRONT_FACE:
            case LayaGLContext.GENERATE_MIPMAP_HINT:
            case LayaGLContext.GREEN_BITS:
            case LayaGLContext.IMPLEMENTATION_COLOR_READ_FORMAT:
            case LayaGLContext.IMPLEMENTATION_COLOR_READ_TYPE:
            case LayaGLContext.MAX_COMBINED_TEXTURE_IMAGE_UNITS:
            case LayaGLContext.MAX_CUBE_MAP_TEXTURE_SIZE:
            case LayaGLContext.MAX_FRAGMENT_UNIFORM_VECTORS:
            case LayaGLContext.MAX_RENDERBUFFER_SIZE:
            case LayaGLContext.MAX_TEXTURE_IMAGE_UNITS:
            case LayaGLContext.MAX_TEXTURE_SIZE:
            case LayaGLContext.MAX_VARYING_VECTORS:
            case LayaGLContext.MAX_VERTEX_ATTRIBS:
            case LayaGLContext.MAX_VERTEX_TEXTURE_IMAGE_UNITS:
            case LayaGLContext.MAX_VERTEX_UNIFORM_VECTORS:
            case LayaGLContext.SAMPLE_BUFFERS:
            case LayaGLContext.SAMPLES:
            case LayaGLContext.STENCIL_BACK_FAIL:
            case LayaGLContext.STENCIL_BACK_FUNC:
            case LayaGLContext.STENCIL_BACK_PASS_DEPTH_FAIL:
            case LayaGLContext.STENCIL_BACK_PASS_DEPTH_PASS:
            case LayaGLContext.STENCIL_BACK_REF:
            case LayaGLContext.STENCIL_BACK_VALUE_MASK:
            case LayaGLContext.STENCIL_BACK_WRITEMASK:
            case LayaGLContext.STENCIL_BITS:
            case LayaGLContext.STENCIL_CLEAR_VALUE:
            case LayaGLContext.STENCIL_FAIL:
            case LayaGLContext.STENCIL_FUNC:
            case LayaGLContext.STENCIL_PASS_DEPTH_FAIL:
            case LayaGLContext.STENCIL_PASS_DEPTH_PASS:
            case LayaGLContext.STENCIL_REF:
            case LayaGLContext.STENCIL_VALUE_MASK:
            case LayaGLContext.STENCIL_WRITEMASK:
            case LayaGLContext.SUBPIXEL_BITS:
            case LayaGLContext.UNPACK_ALIGNMENT:
                result = gl._nativeObj.getIntegerv(pname);
                break;
            case LayaGLContext.CURRENT_PROGRAM:
                gl._nativeObj.flushCommand();
                result = this._layagl.getBind(pname);
                break;
            case LayaGLContext.ARRAY_BUFFER_BINDING:
                gl._nativeObj.flushCommand();
                result = this._layagl.getBind(LayaGLContext.ARRAY_BUFFER);
                break;
            case LayaGLContext.ELEMENT_ARRAY_BUFFER_BINDING:
                gl._nativeObj.flushCommand();
                result = this._layagl.getBind(LayaGLContext.ELEMENT_ARRAY_BUFFER);
                break;
            case LayaGLContext.RENDERBUFFER_BINDING:
                gl._nativeObj.flushCommand();
                result = this._layagl.getBind(LayaGLContext.RENDERBUFFER);
                break;
            case LayaGLContext.TEXTURE_BINDING_2D:
                gl._nativeObj.flushCommand();
                result = this._layagl.getBind(LayaGLContext.TEXTURE_2D);
                break;
            case LayaGLContext.TEXTURE_BINDING_CUBE_MAP:
                gl._nativeObj.flushCommand();
                result = this._layagl.getBind(LayaGLContext.TEXTURE_CUBE_MAP);
                break;
            case LayaGLContext.BLEND:
            case LayaGLContext.CULL_FACE:
            case LayaGLContext.DEPTH_TEST:
            case LayaGLContext.DEPTH_WRITEMASK:
            case LayaGLContext.DITHER:
            case LayaGLContext.SAMPLE_COVERAGE_INVERT:
            case LayaGLContext.SCISSOR_TEST:
            case LayaGLContext.STENCIL_TEST:
                var ret = gl._nativeObj.getBooleanv(pname);
                if (!ret)
                    result = false;
                else
                    result = true;
                break;
            case LayaGLContext.DEPTH_CLEAR_VALUE:
            case LayaGLContext.LINE_WIDTH:
            case LayaGLContext.SAMPLE_COVERAGE_VALUE:
                result = gl._nativeObj.getFloatv(pname);
                break;
            case LayaGLContext.ALIASED_LINE_WIDTH_RANGE:
            case LayaGLContext.ALIASED_POINT_SIZE_RANGE:
            case LayaGLContext.BLEND_COLOR:
            case LayaGLContext.COLOR_CLEAR_VALUE:
            case LayaGLContext.DEPTH_RANGE:
                result = gl._nativeObj.getFloatArrayv(pname);
                break;
            case LayaGLContext.MAX_VIEWPORT_DIMS:
            case LayaGLContext.SCISSOR_BOX:
            case LayaGLContext.VIEWPORT:
                result = gl._nativeObj.getIntegerArrayv(pname);
                break;
            case LayaGLContext.RENDERER:
            case LayaGLContext.SHADING_LANGUAGE_VERSION:
            case LayaGLContext.VENDOR:
            case LayaGLContext.VERSION:
                result = gl._nativeObj.getStringEx(pname);
                break;
            case LayaGLContext.MAX_TEXTURE_MAX_ANISOTROPY_EXT:
            case LayaGLContext.TEXTURE_MAX_ANISOTROPY_EXT:
                result = gl._nativeObj.getParameter(pname);
                break;
            default:
                alert("getParameter 尚未支持");
                break;
        }
        return result;
    }
    getBufferParameter(target, pname) {
        return this._layagl._nativeObj.getBufferParameterEx(target, pname);
    }
    getError() {
        this.add_i(FUNCTION_ID.GETERROR);
        return 0;
    }
    getFramebufferAttachmentParameter(target, attachment, pname) {
        var gl = this._layagl;
        var result = null;
        switch (pname) {
            case LayaGLContext.FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE:
                var frameBuffer = gl.getBind(target);
                if (frameBuffer) {
                    var attachBuffer = frameBuffer.getBind(attachment);
                    if (!attachBuffer)
                        result = LayaGLContext.NONE;
                    else if (attachBuffer instanceof WebGLTexture)
                        result = LayaGLContext.TEXTURE;
                    else if (attachBuffer instanceof WebGLRenderBuffer)
                        result = LayaGLContext.RENDERBUFFER;
                }
                break;
            case LayaGLContext.FRAMEBUFFER_ATTACHMENT_OBJECT_NAME:
                var frameBuffer = gl.getBind(target);
                if (frameBuffer) {
                    return frameBuffer.getBind(attachment);
                }
                break;
            case LayaGLContext.FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE:
            case LayaGLContext.FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL:
                result = gl._nativeObj.getFramebufferAttachmentParameter(target, attachment, pname);
                break;
            default:
                break;
        }
        return result;
    }
    getProgramParameter(program, pname) {
        let ret;
        var gl = this._layagl;
        if (pname == LayaGLContext.ATTACHED_SHADERS)
            return program.attachedShaderCount;
        if (program.linkStatus == 0 || program.id == -1) {
            if (program.id == -1) {
                if (pname == LayaGLContext.DELETE_STATUS || pname == LayaGLContext.LINK_STATUS ||
                    pname == LayaGLContext.VALIDATE_STATUS) {
                    return null;
                }
            }
            else if (program.linkStatus == 0) {
                if (pname == LayaGLContext.DELETE_STATUS || pname == LayaGLContext.LINK_STATUS ||
                    pname == LayaGLContext.VALIDATE_STATUS) {
                    return false;
                }
            }
            return 0;
        }
        else {
            ret = this._layagl._nativeObj.getProgramParameterEx(program.linkedVSSource, program.linkedPSSource, "", pname);
            if (pname == LayaGLContext.DELETE_STATUS || pname == LayaGLContext.LINK_STATUS ||
                pname == LayaGLContext.VALIDATE_STATUS) {
                return ret == 1 ? true : false;
            }
            else {
                return ret;
            }
        }
    }
    getProgramInfoLog(program) {
        if (program.id == -1)
            return null;
        if (program.linkStatus)
            return "";
        if (program.attachedShaderCount < 2) {
            return "missing shaders" + String.fromCharCode(0);
        }
        var ret = this._layagl._nativeObj.getProgramInfoLogEx(program.vsShader.src, program.psShader.src, "");
        if (ret === "invalid shaders") {
            ret = ret + String.fromCharCode(0);
        }
        return ret;
    }
    getRenderbufferParameter(target, pname) {
        return this._layagl._nativeObj.getRenderbufferParameterEx(target, pname);
    }
    getShaderPrecisionFormat(shaderType, precisionType) {
        return this._layagl._nativeObj.getShaderPrecisionFormat(shaderType, precisionType);
    }
    getShaderParameter(shader, pname) {
        if (shader && shader.id == -1)
            return null;
        if (pname == LayaGLContext.COMPILE_STATUS) {
            return shader.compileStatus === 0 ? false : true;
        }
        return this._layagl._nativeObj.getShaderParameter(shader.src, shader.type, pname);
    }
    getShaderInfoLog(shader) {
        return this._layagl._nativeObj.getShaderInfoLogEx(shader.src, shader.type);
    }
    getShaderSource(shader) {
        if (!(shader instanceof WebGLShader))
            throw new TypeError("parameter 1 is not of type 'WebGLShader'");
        if (shader && shader.id !== -1)
            return shader.src;
        return null;
    }
    getTexParameter(target, pname) {
        return this._layagl._nativeObj.getTexParameterEx(target, pname);
    }
    getUniform(program, location) {
        if (!program.linkStatus || program.id == -1)
            return null;
        var gl = this._layagl;
        var curProgram = gl.getBind(LayaGLContext.CURRENT_PROGRAM);
        var result = null;
        this.useProgram(program);
        gl._nativeObj.flushCommand();
        var locationName = program.getLocationName(location);
        var result = this._layagl._nativeObj.getUniformEx(locationName);
        var type = result[0];
        result.splice(0, 1);
        if (type == LayaGLContext.BOOL) {
            for (var i = 0; i < result.length; i++) {
                if (result[i] === 0)
                    result[i] = false;
                else
                    result[i] = true;
            }
        }
        if (curProgram) {
            this.useProgram(curProgram);
            gl._nativeObj.flushCommand();
        }
        if (result.length == 1) {
            return result[0];
        }
        else {
            if (type == LayaGLContext.BOOL)
                return result;
            else if (type == LayaGLContext.FLOAT)
                return new Float32Array(result);
            else if (type == LayaGLContext.INT)
                return new Int32Array(result);
        }
        return null;
    }
    getUniformMutiThread(program, location) {
        console.log("getUniformMutiThread can't support");
        return null;
    }
    getUniformLocation(program, name) {
        var fakeLoc = GLCommandEncoder._locTable.getFakeLocation(program.id, name);
        var strLen = this.getStrLength(name);
        this.add_iii(FUNCTION_ID.GETUNIFORMLOCATION, program.id, fakeLoc);
        this.add_String(name, strLen);
        program.addLocationPair(fakeLoc, name);
        return fakeLoc;
    }
    getVertexAttrib(index, pname) {
        var gl = this._layagl;
        var result = 0;
        switch (pname) {
            case LayaGLContext.VERTEX_ATTRIB_ARRAY_BUFFER_BINDING:
                gl._nativeObj.flushCommand();
                result = gl.getBind(LayaGLContext.ARRAY_BUFFER);
                break;
            case LayaGLContext.VERTEX_ATTRIB_ARRAY_ENABLED:
            case LayaGLContext.VERTEX_ATTRIB_ARRAY_NORMALIZED:
                var ret = gl._nativeObj.getVertexAttribEx(index, pname);
                if (ret === 0)
                    result = false;
                else
                    result = true;
                break;
            case LayaGLContext.VERTEX_ATTRIB_ARRAY_SIZE:
            case LayaGLContext.VERTEX_ATTRIB_ARRAY_STRIDE:
            case LayaGLContext.VERTEX_ATTRIB_ARRAY_TYPE:
                result = gl._nativeObj.getVertexAttribEx(index, pname);
                break;
            case LayaGLContext.CURRENT_VERTEX_ATTRIB:
                result = gl._nativeObj.getVertexAttribExfv(index, pname);
                break;
            default:
                result = 0;
                break;
        }
        return result;
    }
    getVertexAttribOffset(index, pname) {
        return this._layagl._nativeObj.getVertexAttribOffset(index, pname);
    }
    hint(target, mode) {
        this.add_iii(FUNCTION_ID.HINT, target, mode);
    }
    isBuffer(buffer) {
        return (buffer instanceof WebGLBuffer);
    }
    isEnabled(cap) {
        var ret = this._layagl._nativeObj.isEnabled(cap);
        if (!ret)
            return false;
        return true;
    }
    isFramebuffer(framebuffer) {
        return (framebuffer instanceof WebGLFrameBuffer);
    }
    isProgram(program) {
        return (program instanceof WebGLProgram);
    }
    isRenderbuffer(renderbuffer) {
        return (renderbuffer instanceof WebGLRenderBuffer);
    }
    isShader(shader) {
        return (shader instanceof WebGLShader);
    }
    isTexture(texture) {
        return (texture instanceof WebGLTexture);
    }
    lineWidth(width) {
        this.add_if(FUNCTION_ID.LINEWIDTH, width);
    }
    linkProgram(program) {
        this.add_ii(FUNCTION_ID.LINKPROGRAM, program.id);
        if (program.attachedShaderCount >= 2) {
            var gl = this._layagl;
            program.linkStatus = gl._nativeObj.getProgramParameterEx(program.vsShader.src, program.psShader.src, "", LayaGLContext.LINK_STATUS);
            if (program.linkStatus) {
                program.linkedVSSource = program.vsShader.src;
                program.linkedPSSource = program.psShader.src;
            }
        }
    }
    pixelStorei(pname, param) {
        this.add_iii(FUNCTION_ID.PIXELSTOREI, pname, param);
    }
    polygonOffset(factor, units) {
        this.add_iff(FUNCTION_ID.POLYGONOFFSET, factor, units);
    }
    readPixels(x, y, width, height, format, type, pixels) {
        if (!pixels && !(pixels instanceof Uint8Array) && !(pixels instanceof Uint16Array) &&
            !(pixels instanceof Float32Array))
            return;
        var ret = this._layagl._nativeObj.readPixels(x, y, width, height, format, type);
        pixels.set(new Uint8Array(ret));
    }
    readPixelsAsync(x, y, w, h, format, type, callBack) {
        var fakeID = this.createFakeID();
        this._readPixelsAsyncCallbackFuncObj.addCallbackFunc(fakeID, callBack);
        this.add_iiiiiiiii(FUNCTION_ID.READPIXELS_ASYNC, x, y, w, h, format, type, this._readPixelsAsyncCallbackFuncObj.id, fakeID);
    }
    renderbufferStorage(target, internalformat, width, height) {
        this.add_iiiii(FUNCTION_ID.RENDERBUFFERSTORAGE, target, internalformat, width, height);
    }
    renderbufferStorageMultisample(target, samples, internalformat, width, height) {
        this.add_iiiiii(FUNCTION_ID.RENDERBUFFERSTORAGEMUILTISAMPLE, target, samples, internalformat, width, height);
    }
    sampleCoverage(value, invert) {
        this.add_ifi(FUNCTION_ID.SAMPLECOVERAGE, value, invert);
    }
    scissor(x, y, width, height) {
        this.add_iiiii(FUNCTION_ID.SCISSOR, x, y, width, height);
    }
    shaderSource(shader, source) {
        shader.src = source;
        var strLen = this.getStrLength(source);
        this.add_ii(FUNCTION_ID.SHADERSOURCE, shader.id);
        this.add_String(source, strLen);
    }
    stencilFunc(func, ref, mask) {
        this.add_iiii(FUNCTION_ID.STENCILFUNC, func, ref, mask);
    }
    stencilFuncSeparate(face, func, ref, mask) {
        this.add_iiiii(FUNCTION_ID.STENCILFUNCSEPARATE, face, func, ref, mask);
    }
    stencilMask(mask) {
        this.add_ii(FUNCTION_ID.STENCILMASK, mask);
    }
    stencilMaskSeparate(face, mask) {
        this.add_iii(FUNCTION_ID.STENCILMASKSEPARATE, face, mask);
    }
    stencilOp(fail, zfail, zpass) {
        this.add_iiii(FUNCTION_ID.STENCILOP, fail, zfail, zpass);
    }
    stencilOpSeparate(face, fail, zfail, zpass) {
        this.add_iiiii(FUNCTION_ID.STENCILOPSEPARATE, face, fail, zfail, zpass);
    }
    texImage2D(_args) {
        var args = arguments;
        if (args.length == 6) {
            if (args[5]._nativeObj) {
                this.add_iiiiiii(FUNCTION_ID.TEXIMAGE2D, args[0], args[1], args[2], args[3], args[4], args[5]._nativeObj.conchImgId);
            }
            else {
                this.add_iiiiiiiii(FUNCTION_ID.TEXIMAGE2DCANVAS, args[0], args[1], args[2], args[3], args[4], args[5]._ctx._targets._glTexture.id, args[5].width, args[5].height);
            }
        }
        else if (args.length == 9) {
            if (args[8] == null) {
                this.add_iiiiiiiiii(FUNCTION_ID.TEXIMAGE2D_PIXEL, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], 0);
            }
            else if (args[8] instanceof ArrayBuffer || ArrayBuffer.isView(args[8])) {
                var ab = args[8];
                var nAlignLength = this.getAlignLength(ab);
                this.add_iiiiiiiii(FUNCTION_ID.TEXIMAGE2D_PIXEL, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]);
                this.wab(ab, ab.byteLength, nAlignLength);
            }
        }
    }
    texStorage2D(target, levels, internalformat, width, height) {
        this.add_iiiiii(FUNCTION_ID.TEXSTORAGE2D, target, levels, internalformat, width, height);
    }
    texParameterf(target, pname, param) {
        this.add_iiif(FUNCTION_ID.TEXPARAMETERF, target, pname, param);
    }
    texParameteri(target, pname, param) {
        this.add_iiii(FUNCTION_ID.TEXPARAMETERI, target, pname, param);
    }
    texSubImage2D(_args) {
        var args = arguments;
        if (args.length == 7) {
            if (args[6]._nativeObj) {
                this.add_iiiiiiii(FUNCTION_ID.TEXSUBIMAGE2D, args[0], args[1], args[2], args[3], args[4], args[5], args[6]._nativeObj.conchImgId);
            }
        }
        else if (args.length == 9) {
            if (args[8] == null) {
                this.add_iiiiiiiiii(FUNCTION_ID.TEXSUBIMAGE2D_PIXEL, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], 0);
            }
            else if (args[8] instanceof ArrayBuffer || ArrayBuffer.isView(args[8])) {
                var ab = args[8];
                var nAlignLength = this.getAlignLength(ab);
                this.add_iiiiiiiii(FUNCTION_ID.TEXSUBIMAGE2D_PIXEL, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]);
                this.wab(ab, ab.byteLength, nAlignLength);
            }
        }
    }
    uniform1f(location, x) {
        this.add_iif(FUNCTION_ID.UNIFORM1F, location, x);
    }
    uniform1fv(location, values) {
        values = (values instanceof Array) ? new Float32Array(values) : values;
        this.uniform1fv_laya(location, values);
    }
    uniform1fv_laya(location, values) {
        var nAlignLength = this.getAlignLength(values);
        this.add_ii(FUNCTION_ID.UNIFORM1FV, location);
        this.wab(values, values.byteLength, nAlignLength);
    }
    uniform1i(location, x) {
        this.add_iii(FUNCTION_ID.UNIFORM1I, location, x);
    }
    uniform1iv(location, values) {
        values = (values instanceof Array) ? new Int32Array(values) : values;
        this.uniform1iv_laya(location, values);
    }
    uniform1iv_laya(location, values) {
        var nAlignLength = this.getAlignLength(values);
        this.add_ii(FUNCTION_ID.UNIFORM1IV, location);
        this.wab(values, values.byteLength, nAlignLength);
    }
    uniform2f(location, x, y) {
        this.add_iiff(FUNCTION_ID.UNIFORM2F, location, x, y);
    }
    uniform2fv(location, values) {
        values = (values instanceof Array) ? new Float32Array(values) : values;
        this.uniform2fv_laya(location, values);
    }
    uniform2fv_laya(location, values) {
        var nAlignLength = this.getAlignLength(values);
        this.add_ii(FUNCTION_ID.UNIFORM2FV, location);
        this.wab(values, values.byteLength, nAlignLength);
    }
    uniform2i(location, x, y) {
        this.add_iiii(FUNCTION_ID.UNIFORM2I, location, x, y);
    }
    uniform2iv(location, values) {
        values = (values instanceof Array) ? new Int32Array(values) : values;
        this.uniform2iv_laya(location, values);
    }
    uniform2iv_laya(location, values) {
        var nAlignLength = this.getAlignLength(values);
        this.add_ii(FUNCTION_ID.UNIFORM2IV, location);
        this.wab(values, values.byteLength, nAlignLength);
    }
    uniform3f(location, x, y, z) {
        this.add_iifff(FUNCTION_ID.UNIFORM3F, location, x, y, z);
    }
    uniform3fv(location, values) {
        values = (values instanceof Array) ? new Float32Array(values) : values;
        this.uniform3fv_laya(location, values);
    }
    uniform3fv_laya(location, values) {
        var nAlignLength = this.getAlignLength(values);
        this.add_ii(FUNCTION_ID.UNIFORM3FV, location);
        this.wab(values, values.byteLength, nAlignLength);
    }
    uniform3i(location, x, y, z) {
        this.add_iiiii(FUNCTION_ID.UNIFORM3I, location, x, y, z);
    }
    uniform3iv(location, values) {
        values = (values instanceof Array) ? new Int32Array(values) : values;
        this.uniform3iv_laya(location, values);
    }
    uniform3iv_laya(location, values) {
        var nAlignLength = this.getAlignLength(values);
        this.add_ii(FUNCTION_ID.UNIFORM3IV, location);
        this.wab(values, values.byteLength, nAlignLength);
    }
    uniform4f(location, x, y, z, w) {
        this.add_iiffff(FUNCTION_ID.UNIFORM4F, location, x, y, z, w);
    }
    uniform4fv(location, values) {
        values = (values instanceof Array) ? new Float32Array(values) : values;
        this.uniform4fv_laya(location, values);
    }
    uniform4fv_laya(location, values) {
        var nAlignLength = this.getAlignLength(values);
        this.add_ii(FUNCTION_ID.UNIFORM4FV, location);
        this.wab(values, values.byteLength, nAlignLength);
    }
    uniform4i(location, x, y, z, w) {
        this.add_iiiiii(FUNCTION_ID.UNIFORM4I, location, x, y, z, w);
    }
    uniform4iv(location, values) {
        values = (values instanceof Array) ? new Int32Array(values) : values;
        this.uniform4iv_laya(location, values);
    }
    uniform4iv_laya(location, values) {
        var nAlignLength = this.getAlignLength(values);
        this.add_ii(FUNCTION_ID.UNIFORM4IV, location);
        this.wab(values, values.byteLength, nAlignLength);
    }
    uniformMatrix2fv(location, transpose, values) {
        values = (values instanceof Array) ? new Float32Array(values) : values;
        this.uniformMatrix2fv_laya(location, transpose, values);
    }
    uniformMatrix2fv_laya(location, transpose, value) {
        var nAlignLength = this.getAlignLength(value);
        this.add_iii(FUNCTION_ID.UNIFORMMATRIX2FV, location, transpose);
        this.wab(value, value.byteLength, nAlignLength);
    }
    uniformMatrix3fv(location, transpose, values) {
        values = (values instanceof Array) ? new Float32Array(values) : values;
        this.uniformMatrix3fv_laya(location, transpose, values);
    }
    uniformMatrix3fv_laya(location, transpose, value) {
        var nAlignLength = this.getAlignLength(value);
        this.add_iii(FUNCTION_ID.UNIFORMMATRIX3FV, location, transpose);
        this.wab(value, value.byteLength, nAlignLength);
    }
    uniformMatrix4fv(location, transpose, values) {
        values = (values instanceof Array) ? new Float32Array(values) : values;
        this.uniformMatrix4fv_laya(location, transpose, values);
    }
    uniformMatrix4fv_laya(location, transpose, value) {
        var nAlignLength = this.getAlignLength(value);
        this.add_iii(FUNCTION_ID.UNIFORMMATRIX4FV, location, transpose);
        this.wab(value, value.byteLength, nAlignLength);
    }
    useProgram(program) {
        this.add_ii(FUNCTION_ID.USEPROGRAM, program ? program.id : 0);
    }
    validateProgram(program) {
        this.add_ii(FUNCTION_ID.VALIDATEPROGRAM, program.id);
    }
    vertexAttrib1f(indx, x) {
        this.add_iif(FUNCTION_ID.VERTEXATTRIB1F, indx, x);
    }
    vertexAttrib1fv(indx, values) {
        var nAlignLength = this.getAlignLength(values);
        this.add_ii(FUNCTION_ID.VERTEXATTRIB1FV, indx);
        this.wab(values, values.byteLength, nAlignLength);
    }
    vertexAttrib2f(indx, x, y) {
        this.add_iiff(FUNCTION_ID.VERTEXATTRIB2F, indx, x, y);
    }
    vertexAttrib2fv(indx, values) {
        var nAlignLength = this.getAlignLength(values);
        this.add_ii(FUNCTION_ID.VERTEXATTRIB2FV, indx);
        this.wab(values, values.byteLength, nAlignLength);
    }
    vertexAttrib3f(indx, x, y, z) {
        this.add_iifff(FUNCTION_ID.VERTEXATTRIB3F, indx, x, y, z);
    }
    vertexAttrib3fv(indx, values) {
        var nAlignLength = this.getAlignLength(values);
        this.add_ii(FUNCTION_ID.VERTEXATTRIB3FV, indx);
        this.wab(values, values.byteLength, nAlignLength);
    }
    vertexAttrib4f(indx, x, y, z, w) {
        this.add_iiffff(FUNCTION_ID.VERTEXATTRIB4F, indx, x, y, z, w);
    }
    vertexAttrib4fv(indx, values) {
        var nAlignLength = this.getAlignLength(values);
        this.add_ii(FUNCTION_ID.VERTEXATTRIB4FV, indx);
        this.wab(values, values.byteLength, nAlignLength);
    }
    vertexAttribPointer(indx, size, type, normalized, stride, offset) {
        this.add_iiiiiii(FUNCTION_ID.VERTEXATTRIBPOINTER, indx, size, type, normalized, stride, offset);
    }
    viewport(x, y, width, height) {
        this.add_iiiii(FUNCTION_ID.VIEWPORT, x, y, width, height);
    }
    configureBackBuffer(width, height, antiAlias, enableDepthAndStencil, wantsBestResolution) {
        enableDepthAndStencil = enableDepthAndStencil ? enableDepthAndStencil : true;
        wantsBestResolution = wantsBestResolution ? wantsBestResolution : false;
        console.log("configureBackBuffer can't support");
    }
    compressedTexImage2D(_args) {
        var args = arguments;
        var nAlignLength = this.getAlignLength(args[6]);
        this.add_iiiiiii(FUNCTION_ID.COMPRESSEDTEXIMAGE2D, args[0], args[1], args[2], args[3], args[4], args[5]);
        this.wab(args[6], args[6].byteLength, nAlignLength);
    }
    compressedTexSubImage2D(_args) {
        var args = arguments;
        var nAlignLength = this.getAlignLength(args[7]);
        this.add_iiiiiiii(FUNCTION_ID.COMPRESSEDTEXSUBIMAGE2D, args[0], args[1], args[2], args[3], args[4], args[5], args[6]);
        this.wab(args[7], args[7].byteLength, nAlignLength);
    }
    createVertexArray() {
        var fakeID = this.createFakeID();
        this.add_ii(FUNCTION_ID.CREATEVERTEXARRAY, fakeID);
        return new WebGLVertextArray(fakeID);
    }
    bindVertexArray(vao) {
        this.add_ii(FUNCTION_ID.BINDVERTEXARRAY, vao ? vao.id : 0);
    }
    deleteVertexArray(vao) {
        this.deleteFakeID(vao.id);
        this.add_ii(FUNCTION_ID.DELETEVERTEXARRAYS, vao ? vao.id : 0);
    }
    isVertexArray(vao) {
        return vao.instanceof(WebGLVertextArray);
    }
    vertexAttribDivisor(index, divisor) {
        this.add_iii(FUNCTION_ID.VERTEXATTRIBDIVISOR, index, divisor);
    }
    drawArraysInstanced(mode, first, count, instanceCount) {
        this.add_iiiii(FUNCTION_ID.DRAWARRAYSINSTANCED, mode, first, count, instanceCount);
    }
    drawElementsInstanced(mode, count, type, offset, instanceCount) {
        this.add_iiiiii(FUNCTION_ID.DRAWELEMENTSINSTANCED, mode, count, type, offset, instanceCount);
    }
    uniformMatrix2fvEx(location, transpose, value) {
        if (!value["_ptrID"]) {
            this._layagl.createArrayBufferRef(value, LayaGLContext.ARRAY_BUFFER_TYPE_DATA, true);
        }
        var nID = value["_ptrID"];
        this._layagl.syncBufferToRenderThread(value);
        this.add_iiii(FUNCTION_ID.UNIFORMMATRIX2FVEX, location, transpose, nID);
    }
    uniformMatrix3fvEx(location, transpose, value) {
        if (!value["_ptrID"]) {
            this._layagl.createArrayBufferRef(value, LayaGLContext.ARRAY_BUFFER_TYPE_DATA, true);
        }
        var nID = value["_ptrID"];
        this._layagl.syncBufferToRenderThread(value);
        this.add_iiii(FUNCTION_ID.UNIFORMMATRIX3FVEX, location, transpose, nID);
    }
    uniformMatrix4fvEx(location, transpose, value) {
        if (!value["_ptrID"]) {
            this._layagl.createArrayBufferRef(value, LayaGLContext.ARRAY_BUFFER_TYPE_DATA, true);
        }
        var nID = value["_ptrID"];
        this._layagl.syncBufferToRenderThread(value);
        this.add_iiii(FUNCTION_ID.UNIFORMMATRIX4FVEX, location, transpose, nID);
    }
    addShaderUniform(one) {
        var funID = 0;
        var isArray = one.isArray;
        switch (one.type) {
            case GLCommandEncoder.INT:
                funID = isArray ? UNIFORM_TYPE.INTERIOR_UNIFORM1IV : UNIFORM_TYPE.INTERIOR_UNIFORM1I;
                break;
            case GLCommandEncoder.FLOAT:
                funID = isArray ? UNIFORM_TYPE.INTERIOR_UNIFORM1FV : UNIFORM_TYPE.INTERIOR_UNIFORM1F;
                break;
            case GLCommandEncoder.FLOAT_VEC2:
                funID = isArray ? UNIFORM_TYPE.INTERIOR_UNIFORM2FV : UNIFORM_TYPE.INTERIOR_UNIFORM2F;
                break;
            case GLCommandEncoder.FLOAT_VEC3:
                funID = isArray ? UNIFORM_TYPE.INTERIOR_UNIFORM3FV : UNIFORM_TYPE.INTERIOR_UNIFORM3F;
                break;
            case GLCommandEncoder.FLOAT_VEC4:
                funID = isArray ? UNIFORM_TYPE.INTERIOR_UNIFORM4FV : UNIFORM_TYPE.INTERIOR_UNIFORM4F;
                break;
            case GLCommandEncoder.SAMPLER_2D:
                funID = UNIFORM_TYPE.INTERIOR_UNIFORMSAMPLER_2D;
                break;
            case GLCommandEncoder.SAMPLER_CUBE:
                funID = UNIFORM_TYPE.INTERIOR_UNIFORMSAMPLER_CUBE;
                break;
            case GLCommandEncoder.FLOAT_MAT4:
                funID = UNIFORM_TYPE.INTERIOR_UNIFORMMATRIX4FV;
                break;
            case GLCommandEncoder.BOOL:
                funID = UNIFORM_TYPE.INTERIOR_UNIFORM1I;
                break;
            case GLCommandEncoder.FLOAT_MAT2:
                funID = UNIFORM_TYPE.INTERIOR_UNIFORMMATRIX2FV;
                break;
            case GLCommandEncoder.FLOAT_MAT3:
                funID = UNIFORM_TYPE.INTERIOR_UNIFORMMATRIX3FV;
                break;
            default:
                throw new Error("compile shader err!");
        }
        this._layagl.syncBufferToRenderThread(this._buffer);
        this.add_iiiiii(FUNCTION_ID.ADDSHADERUNIFORM, funID, one.location, one.type, one.dataOffset, one.textureID);
    }
    uploadShaderUniforms(commandEncoder, data, type) {
        if (type == LayaGLContext.UPLOAD_SHADER_UNIFORM_TYPE_ID) {
            this._layagl.syncBufferToRenderThread(data);
            this.add_iii(FUNCTION_ID.UPLOADSHADERUNIFORMS, commandEncoder._buffer["_ptrID"], data._ptrID);
        }
        else {
            var nAlignLength = this.getAlignLength(data);
            this.add_ii(FUNCTION_ID.UPLOADSHADERUNIFORMS_BUFFER, commandEncoder._buffer["_ptrID"]);
            this.wab(data, data.byteLength, nAlignLength);
        }
        return 0;
    }
    createFakeID() {
        var frameCount = this._layagl.getFrameCount();
        var fakeArray = GLCommandEncoder._fakeArray;
        for (var i = 0, n = fakeArray.length; i < n; i++) {
            var fakeObj = fakeArray[i];
            if (fakeObj.id == -1 && fakeObj.frameCount != frameCount) {
                fakeObj.id = i;
                fakeObj.frameCount = frameCount;
                return i;
            }
        }
        var nID = ++GLCommandEncoder._fakeIDCount;
        fakeArray.push(new fakeIDObj(nID, frameCount));
        return nID;
    }
    deleteFakeID(nID) {
        var fakeObj = GLCommandEncoder._fakeArray[nID];
        fakeObj.id = -1;
        fakeObj.frameCount = this._layagl.getFrameCount();
    }
    useCommandEncoder(commandEncoder) {
        if (commandEncoder._isSyncToRenderThread) {
            this._layagl.syncBufferToRenderThread(commandEncoder);
        }
        var loc = this._idata[0];
        this.add_ii(FUNCTION_ID.USECOMMANDENCODER, commandEncoder.getPtrID());
        return loc;
    }
    getProgramParameterEx(vs, ps, define, pname) {
        return this._layagl.getProgramParameterEx(vs, ps, define, pname);
    }
    getActiveAttribEx(vs, ps, define, index) {
        return this._layagl.getActiveAttribEx(vs, ps, define, index);
    }
    getActiveUniformEx(vs, ps, define, index) {
        return this._layagl.getActiveUniformEx(vs, ps, define, index);
    }
    loadDataToReg(regNum, data, offset, size) {
        var nAlignLength = this.getAlignLength(data);
        this.add_iiii(FUNCTION_ID.LOADDATATOREG, regNum, offset, size);
        this.wab(data, data.byteLength, nAlignLength);
    }
    loadDataToRegEx(regNum, dataID, offset, size) {
        this.add_iiiii(FUNCTION_ID.LOADDATATOREGEX, regNum, dataID, offset, size);
    }
    ifLess0(regNum, statementNum) {
        this.add_iii(FUNCTION_ID.IFLESS0, regNum, statementNum);
    }
    ifEqual0(regNum, statementNum) {
        this.add_iii(FUNCTION_ID.IFEQUAL0, regNum, statementNum);
    }
    ifGreater0(regNum, statementNum) {
        this.add_iii(FUNCTION_ID.IFGREATER0, regNum, statementNum);
    }
    ifLEqual0(regNum, statementNum) {
        this.add_iii(FUNCTION_ID.IFLEQUAL0, regNum, statementNum);
    }
    ifGEqual0(regNum, statementNum) {
        this.add_iii(FUNCTION_ID.IFGEQUAL0, regNum, statementNum);
    }
    ifGNotEqual0(regNum, statementNum) {
        this.add_iii(FUNCTION_ID.IFGNOTEQUAL0, regNum, statementNum);
    }
    operateReg(regOut, reg1, reg2, size, operateType, dataType) {
        this.add_iiiiiii(FUNCTION_ID.OPERATEREG, regOut, reg1, reg2, size, operateType, dataType);
    }
    store(dataID, offset, regID, size) {
        this.add_iiiii(FUNCTION_ID.STORE, dataID, offset, regID, size);
    }
    getAlignLength(data) {
        var byteLength = data.byteLength;
        return (byteLength + 3) & 0xfffffffc;
    }
    getStrLength(str) {
        var ab = conch.strTobufer(str);
        return ab.byteLength;
    }
}
GLCommandEncoder.INT = 0x1404;
GLCommandEncoder.FLOAT = 0x1406;
GLCommandEncoder.FLOAT_VEC2 = 0x8B50;
GLCommandEncoder.FLOAT_VEC3 = 0x8B51;
GLCommandEncoder.FLOAT_VEC4 = 0x8B52;
GLCommandEncoder.INT_VEC2 = 0x8B53;
GLCommandEncoder.INT_VEC3 = 0x8B54;
GLCommandEncoder.INT_VEC4 = 0x8B55;
GLCommandEncoder.BOOL = 0x8B56;
GLCommandEncoder.BOOL_VEC2 = 0x8B57;
GLCommandEncoder.BOOL_VEC3 = 0x8B58;
GLCommandEncoder.BOOL_VEC4 = 0x8B59;
GLCommandEncoder.FLOAT_MAT2 = 0x8B5A;
GLCommandEncoder.FLOAT_MAT3 = 0x8B5B;
GLCommandEncoder.FLOAT_MAT4 = 0x8B5C;
GLCommandEncoder.SAMPLER_2D = 0x8B5E;
GLCommandEncoder.SAMPLER_CUBE = 0x8B60;
GLCommandEncoder._fakeIDCount = 0;
GLCommandEncoder._fakeArray = [new fakeIDObj(0, 0)];
GLCommandEncoder._locTable = new ProgramLocationTable();
class LayaGLContext {
    constructor(contextType) {
        this.width = 0;
        this.height = 0;
        this.canvas = null;
        this._currentCmdEncoder = null;
        this._saveCommandEncoder = new Array();
        this._currentContext = null;
        this._threadMode = LayaGLContext.THREAD_MODE_DOUBLE;
        this._curBindInfo = new BindInfo();
        this.DEPTH_BUFFER_BIT = 0x00000100;
        this.STENCIL_BUFFER_BIT = 0x00000400;
        this.COLOR_BUFFER_BIT = 0x00004000;
        this.POINTS = 0x0000;
        this.LINES = 0x0001;
        this.LINE_LOOP = 0x0002;
        this.LINE_STRIP = 0x0003;
        this.TRIANGLES = 0x0004;
        this.TRIANGLE_STRIP = 0x0005;
        this.TRIANGLE_FAN = 0x0006;
        this.ZERO = 0;
        this.ONE = 1;
        this.SRC_COLOR = 0x0300;
        this.ONE_MINUS_SRC_COLOR = 0x0301;
        this.SRC_ALPHA = 0x0302;
        this.ONE_MINUS_SRC_ALPHA = 0x0303;
        this.DST_ALPHA = 0x0304;
        this.ONE_MINUS_DST_ALPHA = 0x0305;
        this.DST_COLOR = 0x0306;
        this.ONE_MINUS_DST_COLOR = 0x0307;
        this.SRC_ALPHA_SATURATE = 0x0308;
        this.FUNC_ADD = 0x8006;
        this.BLEND_EQUATION = 0x8009;
        this.BLEND_EQUATION_RGB = 0x8009;
        this.BLEND_EQUATION_ALPHA = 0x883D;
        this.FUNC_SUBTRACT = 0x800A;
        this.FUNC_REVERSE_SUBTRACT = 0x800B;
        this.BLEND_DST_RGB = 0x80C8;
        this.BLEND_SRC_RGB = 0x80C9;
        this.BLEND_DST_ALPHA = 0x80CA;
        this.BLEND_SRC_ALPHA = 0x80CB;
        this.CONSTANT_COLOR = 0x8001;
        this.ONE_MINUS_CONSTANT_COLOR = 0x8002;
        this.CONSTANT_ALPHA = 0x8003;
        this.ONE_MINUS_CONSTANT_ALPHA = 0x8004;
        this.BLEND_COLOR = 0x8005;
        this.ARRAY_BUFFER = 0x8892;
        this.ELEMENT_ARRAY_BUFFER = 0x8893;
        this.ARRAY_BUFFER_BINDING = 0x8894;
        this.ELEMENT_ARRAY_BUFFER_BINDING = 0x8895;
        this.STREAM_DRAW = 0x88E0;
        this.STATIC_DRAW = 0x88E4;
        this.DYNAMIC_DRAW = 0x88E8;
        this.BUFFER_SIZE = 0x8764;
        this.BUFFER_USAGE = 0x8765;
        this.CURRENT_VERTEX_ATTRIB = 0x8626;
        this.FRONT = 0x0404;
        this.BACK = 0x0405;
        this.FRONT_AND_BACK = 0x0408;
        this.CULL_FACE = 0x0B44;
        this.BLEND = 0x0BE2;
        this.DITHER = 0x0BD0;
        this.STENCIL_TEST = 0x0B90;
        this.DEPTH_TEST = 0x0B71;
        this.SCISSOR_TEST = 0x0C11;
        this.POLYGON_OFFSET_FILL = 0x8037;
        this.SAMPLE_ALPHA_TO_COVERAGE = 0x809E;
        this.SAMPLE_COVERAGE = 0x80A0;
        this.INVALID_ENUM = 0x0500;
        this.INVALID_VALUE = 0x0501;
        this.INVALID_OPERATION = 0x0502;
        this.OUT_OF_MEMORY = 0x0505;
        this.CW = 0x0900;
        this.CCW = 0x0901;
        this.LINE_WIDTH = 0x0B21;
        this.ALIASED_POINT_SIZE_RANGE = 0x846D;
        this.ALIASED_LINE_WIDTH_RANGE = 0x846E;
        this.CULL_FACE_MODE = 0x0B45;
        this.FRONT_FACE = 0x0B46;
        this.DEPTH_RANGE = 0x0B70;
        this.DEPTH_WRITEMASK = 0x0B72;
        this.DEPTH_CLEAR_VALUE = 0x0B73;
        this.DEPTH_FUNC = 0x0B74;
        this.STENCIL_CLEAR_VALUE = 0x0B91;
        this.STENCIL_FUNC = 0x0B92;
        this.STENCIL_FAIL = 0x0B94;
        this.STENCIL_PASS_DEPTH_FAIL = 0x0B95;
        this.STENCIL_PASS_DEPTH_PASS = 0x0B96;
        this.STENCIL_REF = 0x0B97;
        this.STENCIL_VALUE_MASK = 0x0B93;
        this.STENCIL_WRITEMASK = 0x0B98;
        this.STENCIL_BACK_FUNC = 0x8800;
        this.STENCIL_BACK_FAIL = 0x8801;
        this.STENCIL_BACK_PASS_DEPTH_FAIL = 0x8802;
        this.STENCIL_BACK_PASS_DEPTH_PASS = 0x8803;
        this.STENCIL_BACK_REF = 0x8CA3;
        this.STENCIL_BACK_VALUE_MASK = 0x8CA4;
        this.STENCIL_BACK_WRITEMASK = 0x8CA5;
        this.VIEWPORT = 0x0BA2;
        this.SCISSOR_BOX = 0x0C10;
        this.COLOR_CLEAR_VALUE = 0x0C22;
        this.COLOR_WRITEMASK = 0x0C23;
        this.UNPACK_ALIGNMENT = 0x0CF5;
        this.PACK_ALIGNMENT = 0x0D05;
        this.MAX_TEXTURE_SIZE = 0x0D33;
        this.MAX_VIEWPORT_DIMS = 0x0D3A;
        this.SUBPIXEL_BITS = 0x0D50;
        this.RED_BITS = 0x0D52;
        this.GREEN_BITS = 0x0D53;
        this.BLUE_BITS = 0x0D54;
        this.ALPHA_BITS = 0x0D55;
        this.DEPTH_BITS = 0x0D56;
        this.STENCIL_BITS = 0x0D57;
        this.POLYGON_OFFSET_UNITS = 0x2A00;
        this.POLYGON_OFFSET_FACTOR = 0x8038;
        this.TEXTURE_BINDING_2D = 0x8069;
        this.SAMPLE_BUFFERS = 0x80A8;
        this.SAMPLES = 0x80A9;
        this.SAMPLE_COVERAGE_VALUE = 0x80AA;
        this.SAMPLE_COVERAGE_INVERT = 0x80AB;
        this.COMPRESSED_TEXTURE_FORMATS = 0x86A3;
        this.DONT_CARE = 0x1100;
        this.FASTEST = 0x1101;
        this.NICEST = 0x1102;
        this.GENERATE_MIPMAP_HINT = 0x8192;
        this.BYTE = 0x1400;
        this.UNSIGNED_BYTE = 0x1401;
        this.SHORT = 0x1402;
        this.UNSIGNED_SHORT = 0x1403;
        this.INT = 0x1404;
        this.UNSIGNED_INT = 0x1405;
        this.FLOAT = 0x1406;
        this.DEPTH_COMPONENT = 0x1902;
        this.ALPHA = 0x1906;
        this.RGB = 0x1907;
        this.RGBA = 0x1908;
        this.LUMINANCE = 0x1909;
        this.LUMINANCE_ALPHA = 0x190A;
        this.UNSIGNED_SHORT_4_4_4_4 = 0x8033;
        this.UNSIGNED_SHORT_5_5_5_1 = 0x8034;
        this.UNSIGNED_SHORT_5_6_5 = 0x8363;
        this.FRAGMENT_SHADER = 0x8B30;
        this.VERTEX_SHADER = 0x8B31;
        this.MAX_VERTEX_ATTRIBS = 0x8869;
        this.MAX_VERTEX_UNIFORM_VECTORS = 0x8DFB;
        this.MAX_VARYING_VECTORS = 0x8DFC;
        this.MAX_COMBINED_TEXTURE_IMAGE_UNITS = 0x8B4D;
        this.MAX_VERTEX_TEXTURE_IMAGE_UNITS = 0x8B4C;
        this.MAX_TEXTURE_IMAGE_UNITS = 0x8872;
        this.MAX_FRAGMENT_UNIFORM_VECTORS = 0x8DFD;
        this.MAX_TEXTURE_MAX_ANISOTROPY_EXT = 34047;
        this.TEXTURE_MAX_ANISOTROPY_EXT = 34046;
        this.SHADER_TYPE = 0x8B4F;
        this.DELETE_STATUS = 0x8B80;
        this.LINK_STATUS = 0x8B82;
        this.VALIDATE_STATUS = 0x8B83;
        this.ATTACHED_SHADERS = 0x8B85;
        this.ACTIVE_UNIFORMS = 0x8B86;
        this.ACTIVE_ATTRIBUTES = 0x8B89;
        this.SHADING_LANGUAGE_VERSION = 0x8B8C;
        this.CURRENT_PROGRAM = 0x8B8D;
        this.NEVER = 0x0200;
        this.LESS = 0x0201;
        this.EQUAL = 0x0202;
        this.LEQUAL = 0x0203;
        this.GREATER = 0x0204;
        this.NOTEQUAL = 0x0205;
        this.GEQUAL = 0x0206;
        this.ALWAYS = 0x0207;
        this.KEEP = 0x1E00;
        this.REPLACE = 0x1E01;
        this.INCR = 0x1E02;
        this.DECR = 0x1E03;
        this.INVERT = 0x150A;
        this.INCR_WRAP = 0x8507;
        this.DECR_WRAP = 0x8508;
        this.VENDOR = 0x1F00;
        this.RENDERER = 0x1F01;
        this.VERSION = 0x1F02;
        this.NEAREST = 0x2600;
        this.LINEAR = 0x2601;
        this.NEAREST_MIPMAP_NEAREST = 0x2700;
        this.LINEAR_MIPMAP_NEAREST = 0x2701;
        this.NEAREST_MIPMAP_LINEAR = 0x2702;
        this.LINEAR_MIPMAP_LINEAR = 0x2703;
        this.TEXTURE_MAG_FILTER = 0x2800;
        this.TEXTURE_MIN_FILTER = 0x2801;
        this.TEXTURE_WRAP_S = 0x2802;
        this.TEXTURE_WRAP_T = 0x2803;
        this.TEXTURE_2D = 0x0DE1;
        this.TEXTURE = 0x1702;
        this.TEXTURE_CUBE_MAP = 0x8513;
        this.TEXTURE_BINDING_CUBE_MAP = 0x8514;
        this.TEXTURE_CUBE_MAP_POSITIVE_X = 0x8515;
        this.TEXTURE_CUBE_MAP_NEGATIVE_X = 0x8516;
        this.TEXTURE_CUBE_MAP_POSITIVE_Y = 0x8517;
        this.TEXTURE_CUBE_MAP_NEGATIVE_Y = 0x8518;
        this.TEXTURE_CUBE_MAP_POSITIVE_Z = 0x8519;
        this.TEXTURE_CUBE_MAP_NEGATIVE_Z = 0x851A;
        this.MAX_CUBE_MAP_TEXTURE_SIZE = 0x851C;
        this.TEXTURE0 = 0x84C0;
        this.TEXTURE1 = 0x84C1;
        this.TEXTURE2 = 0x84C2;
        this.TEXTURE3 = 0x84C3;
        this.TEXTURE4 = 0x84C4;
        this.TEXTURE5 = 0x84C5;
        this.TEXTURE6 = 0x84C6;
        this.TEXTURE7 = 0x84C7;
        this.TEXTURE8 = 0x84C8;
        this.TEXTURE9 = 0x84C9;
        this.TEXTURE10 = 0x84CA;
        this.TEXTURE11 = 0x84CB;
        this.TEXTURE12 = 0x84CC;
        this.TEXTURE13 = 0x84CD;
        this.TEXTURE14 = 0x84CE;
        this.TEXTURE15 = 0x84CF;
        this.TEXTURE16 = 0x84D0;
        this.TEXTURE17 = 0x84D1;
        this.TEXTURE18 = 0x84D2;
        this.TEXTURE19 = 0x84D3;
        this.TEXTURE20 = 0x84D4;
        this.TEXTURE21 = 0x84D5;
        this.TEXTURE22 = 0x84D6;
        this.TEXTURE23 = 0x84D7;
        this.TEXTURE24 = 0x84D8;
        this.TEXTURE25 = 0x84D9;
        this.TEXTURE26 = 0x84DA;
        this.TEXTURE27 = 0x84DB;
        this.TEXTURE28 = 0x84DC;
        this.TEXTURE29 = 0x84DD;
        this.TEXTURE30 = 0x84DE;
        this.TEXTURE31 = 0x84DF;
        this.ACTIVE_TEXTURE = 0x84E0;
        this.REPEAT = 0x2901;
        this.CLAMP_TO_EDGE = 0x812F;
        this.MIRRORED_REPEAT = 0x8370;
        this.FLOAT_VEC2 = 0x8B50;
        this.FLOAT_VEC3 = 0x8B51;
        this.FLOAT_VEC4 = 0x8B52;
        this.INT_VEC2 = 0x8B53;
        this.INT_VEC3 = 0x8B54;
        this.INT_VEC4 = 0x8B55;
        this.BOOL = 0x8B56;
        this.BOOL_VEC2 = 0x8B57;
        this.BOOL_VEC3 = 0x8B58;
        this.BOOL_VEC4 = 0x8B59;
        this.FLOAT_MAT2 = 0x8B5A;
        this.FLOAT_MAT3 = 0x8B5B;
        this.FLOAT_MAT4 = 0x8B5C;
        this.SAMPLER_2D = 0x8B5E;
        this.SAMPLER_CUBE = 0x8B60;
        this.VERTEX_ATTRIB_ARRAY_ENABLED = 0x8622;
        this.VERTEX_ATTRIB_ARRAY_SIZE = 0x8623;
        this.VERTEX_ATTRIB_ARRAY_STRIDE = 0x8624;
        this.VERTEX_ATTRIB_ARRAY_TYPE = 0x8625;
        this.VERTEX_ATTRIB_ARRAY_NORMALIZED = 0x886A;
        this.VERTEX_ATTRIB_ARRAY_POINTER = 0x8645;
        this.VERTEX_ATTRIB_ARRAY_BUFFER_BINDING = 0x889F;
        this.IMPLEMENTATION_COLOR_READ_TYPE = 0x8B9A;
        this.IMPLEMENTATION_COLOR_READ_FORMAT = 0x8B9B;
        this.COMPILE_STATUS = 0x8B81;
        this.LOW_FLOAT = 0x8DF0;
        this.MEDIUM_FLOAT = 0x8DF1;
        this.HIGH_FLOAT = 0x8DF2;
        this.LOW_INT = 0x8DF3;
        this.MEDIUM_INT = 0x8DF4;
        this.HIGH_INT = 0x8DF5;
        this.FRAMEBUFFER = 0x8D40;
        this.RENDERBUFFER = 0x8D41;
        this.RGBA4 = 0x8056;
        this.RGB5_A1 = 0x8057;
        this.RGB565 = 0x8D62;
        this.DEPTH_COMPONENT16 = 0x81A5;
        this.STENCIL_INDEX = 0x1901;
        this.STENCIL_INDEX8 = 0x8D48;
        this.DEPTH_STENCIL = 0x84F9;
        this.RENDERBUFFER_WIDTH = 0x8D42;
        this.RENDERBUFFER_HEIGHT = 0x8D43;
        this.RENDERBUFFER_INTERNAL_FORMAT = 0x8D44;
        this.RENDERBUFFER_RED_SIZE = 0x8D50;
        this.RENDERBUFFER_GREEN_SIZE = 0x8D51;
        this.RENDERBUFFER_BLUE_SIZE = 0x8D52;
        this.RENDERBUFFER_ALPHA_SIZE = 0x8D53;
        this.RENDERBUFFER_DEPTH_SIZE = 0x8D54;
        this.RENDERBUFFER_STENCIL_SIZE = 0x8D55;
        this.FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE = 0x8CD0;
        this.FRAMEBUFFER_ATTACHMENT_OBJECT_NAME = 0x8CD1;
        this.FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL = 0x8CD2;
        this.FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE = 0x8CD3;
        this.COLOR_ATTACHMENT0 = 0x8CE0;
        this.DEPTH_ATTACHMENT = 0x8D00;
        this.STENCIL_ATTACHMENT = 0x8D20;
        this.DEPTH_STENCIL_ATTACHMENT = 0x821A;
        this.NONE = 0;
        this.FRAMEBUFFER_COMPLETE = 0x8CD5;
        this.FRAMEBUFFER_INCOMPLETE_ATTACHMENT = 0x8CD6;
        this.FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT = 0x8CD7;
        this.FRAMEBUFFER_INCOMPLETE_DIMENSIONS = 0x8CD9;
        this.FRAMEBUFFER_UNSUPPORTED = 0x8CDD;
        this.FRAMEBUFFER_BINDING = 0x8CA6;
        this.RENDERBUFFER_BINDING = 0x8CA7;
        this.MAX_RENDERBUFFER_SIZE = 0x84E8;
        this.INVALID_FRAMEBUFFER_OPERATION = 0x0506;
        this.UNPACK_FLIP_Y_WEBGL = 0x9240;
        this.UNPACK_PREMULTIPLY_ALPHA_WEBGL = 0x9241;
        this.CONTEXT_LOST_WEBGL = 0x9242;
        this.UNPACK_COLORSPACE_CONVERSION_WEBGL = 0x9243;
        this.BROWSER_DEFAULT_WEBGL = 0x9244;
        this.READ_BUFFER = 0x0C02;
        this.UNPACK_ROW_LENGTH = 0x0CF2;
        this.UNPACK_SKIP_ROWS = 0x0CF3;
        this.UNPACK_SKIP_PIXELS = 0x0CF4;
        this.PACK_ROW_LENGTH = 0x0D02;
        this.PACK_SKIP_ROWS = 0x0D03;
        this.PACK_SKIP_PIXELS = 0x0D04;
        this.COLOR = 0x1800;
        this.DEPTH = 0x1801;
        this.STENCIL = 0x1802;
        this.RED = 0x1903;
        this.RGB8 = 0x8051;
        this.RGBA8 = 0x8058;
        this.RGB10_A2 = 0x8059;
        this.TEXTURE_BINDING_3D = 0x806A;
        this.UNPACK_SKIP_IMAGES = 0x806D;
        this.UNPACK_IMAGE_HEIGHT = 0x806E;
        this.TEXTURE_3D = 0x806F;
        this.TEXTURE_WRAP_R = 0x8072;
        this.MAX_3D_TEXTURE_SIZE = 0x8073;
        this.UNSIGNED_INT_2_10_10_10_REV = 0x8368;
        this.MAX_ELEMENTS_VERTICES = 0x80E8;
        this.MAX_ELEMENTS_INDICES = 0x80E9;
        this.TEXTURE_MIN_LOD = 0x813A;
        this.TEXTURE_MAX_LOD = 0x813B;
        this.TEXTURE_BASE_LEVEL = 0x813C;
        this.TEXTURE_MAX_LEVEL = 0x813D;
        this.MIN = 0x8007;
        this.MAX = 0x8008;
        this.DEPTH_COMPONENT24 = 0x81A6;
        this.MAX_TEXTURE_LOD_BIAS = 0x84FD;
        this.TEXTURE_COMPARE_MODE = 0x884C;
        this.TEXTURE_COMPARE_FUNC = 0x884D;
        this.CURRENT_QUERY = 0x8865;
        this.QUERY_RESULT = 0x8866;
        this.QUERY_RESULT_AVAILABLE = 0x8867;
        this.STREAM_READ = 0x88E1;
        this.STREAM_COPY = 0x88E2;
        this.STATIC_READ = 0x88E5;
        this.STATIC_COPY = 0x88E6;
        this.DYNAMIC_READ = 0x88E9;
        this.DYNAMIC_COPY = 0x88EA;
        this.MAX_DRAW_BUFFERS = 0x8824;
        this.DRAW_BUFFER0 = 0x8825;
        this.DRAW_BUFFER1 = 0x8826;
        this.DRAW_BUFFER2 = 0x8827;
        this.DRAW_BUFFER3 = 0x8828;
        this.DRAW_BUFFER4 = 0x8829;
        this.DRAW_BUFFER5 = 0x882A;
        this.DRAW_BUFFER6 = 0x882B;
        this.DRAW_BUFFER7 = 0x882C;
        this.DRAW_BUFFER8 = 0x882D;
        this.DRAW_BUFFER9 = 0x882E;
        this.DRAW_BUFFER10 = 0x882F;
        this.DRAW_BUFFER11 = 0x8830;
        this.DRAW_BUFFER12 = 0x8831;
        this.DRAW_BUFFER13 = 0x8832;
        this.DRAW_BUFFER14 = 0x8833;
        this.DRAW_BUFFER15 = 0x8834;
        this.MAX_FRAGMENT_UNIFORM_COMPONENTS = 0x8B49;
        this.MAX_VERTEX_UNIFORM_COMPONENTS = 0x8B4A;
        this.SAMPLER_3D = 0x8B5F;
        this.SAMPLER_2D_SHADOW = 0x8B62;
        this.FRAGMENT_SHADER_DERIVATIVE_HINT = 0x8B8B;
        this.PIXEL_PACK_BUFFER = 0x88EB;
        this.PIXEL_UNPACK_BUFFER = 0x88EC;
        this.PIXEL_PACK_BUFFER_BINDING = 0x88ED;
        this.PIXEL_UNPACK_BUFFER_BINDING = 0x88EF;
        this.FLOAT_MAT2x3 = 0x8B65;
        this.FLOAT_MAT2x4 = 0x8B66;
        this.FLOAT_MAT3x2 = 0x8B67;
        this.FLOAT_MAT3x4 = 0x8B68;
        this.FLOAT_MAT4x2 = 0x8B69;
        this.FLOAT_MAT4x3 = 0x8B6A;
        this.SRGB = 0x8C40;
        this.SRGB8 = 0x8C41;
        this.SRGB8_ALPHA8 = 0x8C43;
        this.COMPARE_REF_TO_TEXTURE = 0x884E;
        this.RGBA32F = 0x8814;
        this.RGB32F = 0x8815;
        this.RGBA16F = 0x881A;
        this.RGB16F = 0x881B;
        this.VERTEX_ATTRIB_ARRAY_INTEGER = 0x88FD;
        this.MAX_ARRAY_TEXTURE_LAYERS = 0x88FF;
        this.MIN_PROGRAM_TEXEL_OFFSET = 0x8904;
        this.MAX_PROGRAM_TEXEL_OFFSET = 0x8905;
        this.MAX_VARYING_COMPONENTS = 0x8B4B;
        this.TEXTURE_2D_ARRAY = 0x8C1A;
        this.TEXTURE_BINDING_2D_ARRAY = 0x8C1D;
        this.R11F_G11F_B10F = 0x8C3A;
        this.UNSIGNED_INT_10F_11F_11F_REV = 0x8C3B;
        this.RGB9_E5 = 0x8C3D;
        this.UNSIGNED_INT_5_9_9_9_REV = 0x8C3E;
        this.TRANSFORM_FEEDBACK_BUFFER_MODE = 0x8C7F;
        this.MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS = 0x8C80;
        this.TRANSFORM_FEEDBACK_VARYINGS = 0x8C83;
        this.TRANSFORM_FEEDBACK_BUFFER_START = 0x8C84;
        this.TRANSFORM_FEEDBACK_BUFFER_SIZE = 0x8C85;
        this.TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN = 0x8C88;
        this.RASTERIZER_DISCARD = 0x8C89;
        this.MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS = 0x8C8A;
        this.MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS = 0x8C8B;
        this.INTERLEAVED_ATTRIBS = 0x8C8C;
        this.SEPARATE_ATTRIBS = 0x8C8D;
        this.TRANSFORM_FEEDBACK_BUFFER = 0x8C8E;
        this.TRANSFORM_FEEDBACK_BUFFER_BINDING = 0x8C8F;
        this.RGBA32UI = 0x8D70;
        this.RGB32UI = 0x8D71;
        this.RGBA16UI = 0x8D76;
        this.RGB16UI = 0x8D77;
        this.RGBA8UI = 0x8D7C;
        this.RGB8UI = 0x8D7D;
        this.RGBA32I = 0x8D82;
        this.RGB32I = 0x8D83;
        this.RGBA16I = 0x8D88;
        this.RGB16I = 0x8D89;
        this.RGBA8I = 0x8D8E;
        this.RGB8I = 0x8D8F;
        this.RED_INTEGER = 0x8D94;
        this.RGB_INTEGER = 0x8D98;
        this.RGBA_INTEGER = 0x8D99;
        this.SAMPLER_2D_ARRAY = 0x8DC1;
        this.SAMPLER_2D_ARRAY_SHADOW = 0x8DC4;
        this.SAMPLER_CUBE_SHADOW = 0x8DC5;
        this.UNSIGNED_INT_VEC2 = 0x8DC6;
        this.UNSIGNED_INT_VEC3 = 0x8DC7;
        this.UNSIGNED_INT_VEC4 = 0x8DC8;
        this.INT_SAMPLER_2D = 0x8DCA;
        this.INT_SAMPLER_3D = 0x8DCB;
        this.INT_SAMPLER_CUBE = 0x8DCC;
        this.INT_SAMPLER_2D_ARRAY = 0x8DCF;
        this.UNSIGNED_INT_SAMPLER_2D = 0x8DD2;
        this.UNSIGNED_INT_SAMPLER_3D = 0x8DD3;
        this.UNSIGNED_INT_SAMPLER_CUBE = 0x8DD4;
        this.UNSIGNED_INT_SAMPLER_2D_ARRAY = 0x8DD7;
        this.DEPTH_COMPONENT32F = 0x8CAC;
        this.DEPTH32F_STENCIL8 = 0x8CAD;
        this.FLOAT_32_UNSIGNED_INT_24_8_REV = 0x8DAD;
        this.FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING = 0x8210;
        this.FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE = 0x8211;
        this.FRAMEBUFFER_ATTACHMENT_RED_SIZE = 0x8212;
        this.FRAMEBUFFER_ATTACHMENT_GREEN_SIZE = 0x8213;
        this.FRAMEBUFFER_ATTACHMENT_BLUE_SIZE = 0x8214;
        this.FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE = 0x8215;
        this.FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE = 0x8216;
        this.FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE = 0x8217;
        this.FRAMEBUFFER_DEFAULT = 0x8218;
        this.UNSIGNED_INT_24_8 = 0x84FA;
        this.DEPTH24_STENCIL8 = 0x88F0;
        this.UNSIGNED_NORMALIZED = 0x8C17;
        this.DRAW_FRAMEBUFFER_BINDING = 0x8CA6;
        this.READ_FRAMEBUFFER = 0x8CA8;
        this.DRAW_FRAMEBUFFER = 0x8CA9;
        this.READ_FRAMEBUFFER_BINDING = 0x8CAA;
        this.RENDERBUFFER_SAMPLES = 0x8CAB;
        this.FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER = 0x8CD4;
        this.MAX_COLOR_ATTACHMENTS = 0x8CDF;
        this.COLOR_ATTACHMENT1 = 0x8CE1;
        this.COLOR_ATTACHMENT2 = 0x8CE2;
        this.COLOR_ATTACHMENT3 = 0x8CE3;
        this.COLOR_ATTACHMENT4 = 0x8CE4;
        this.COLOR_ATTACHMENT5 = 0x8CE5;
        this.COLOR_ATTACHMENT6 = 0x8CE6;
        this.COLOR_ATTACHMENT7 = 0x8CE7;
        this.COLOR_ATTACHMENT8 = 0x8CE8;
        this.COLOR_ATTACHMENT9 = 0x8CE9;
        this.COLOR_ATTACHMENT10 = 0x8CEA;
        this.COLOR_ATTACHMENT11 = 0x8CEB;
        this.COLOR_ATTACHMENT12 = 0x8CEC;
        this.COLOR_ATTACHMENT13 = 0x8CED;
        this.COLOR_ATTACHMENT14 = 0x8CEE;
        this.COLOR_ATTACHMENT15 = 0x8CEF;
        this.FRAMEBUFFER_INCOMPLETE_MULTISAMPLE = 0x8D56;
        this.MAX_SAMPLES = 0x8D57;
        this.HALF_FLOAT = 0x140B;
        this.RG = 0x8227;
        this.RG_INTEGER = 0x8228;
        this.R8 = 0x8229;
        this.RG8 = 0x822B;
        this.R16F = 0x822D;
        this.R32F = 0x822E;
        this.RG16F = 0x822F;
        this.RG32F = 0x8230;
        this.R8I = 0x8231;
        this.R8UI = 0x8232;
        this.R16I = 0x8233;
        this.R16UI = 0x8234;
        this.R32I = 0x8235;
        this.R32UI = 0x8236;
        this.RG8I = 0x8237;
        this.RG8UI = 0x8238;
        this.RG16I = 0x8239;
        this.RG16UI = 0x823A;
        this.RG32I = 0x823B;
        this.RG32UI = 0x823C;
        this.VERTEX_ARRAY_BINDING = 0x85B5;
        this.R8_SNORM = 0x8F94;
        this.RG8_SNORM = 0x8F95;
        this.RGB8_SNORM = 0x8F96;
        this.RGBA8_SNORM = 0x8F97;
        this.SIGNED_NORMALIZED = 0x8F9C;
        this.COPY_READ_BUFFER = 0x8F36;
        this.COPY_WRITE_BUFFER = 0x8F37;
        this.COPY_READ_BUFFER_BINDING = 0x8F36;
        this.COPY_WRITE_BUFFER_BINDING = 0x8F37;
        this.UNIFORM_BUFFER = 0x8A11;
        this.UNIFORM_BUFFER_BINDING = 0x8A28;
        this.UNIFORM_BUFFER_START = 0x8A29;
        this.UNIFORM_BUFFER_SIZE = 0x8A2A;
        this.MAX_VERTEX_UNIFORM_BLOCKS = 0x8A2B;
        this.MAX_FRAGMENT_UNIFORM_BLOCKS = 0x8A2D;
        this.MAX_COMBINED_UNIFORM_BLOCKS = 0x8A2E;
        this.MAX_UNIFORM_BUFFER_BINDINGS = 0x8A2F;
        this.MAX_UNIFORM_BLOCK_SIZE = 0x8A30;
        this.MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS = 0x8A31;
        this.MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS = 0x8A33;
        this.UNIFORM_BUFFER_OFFSET_ALIGNMENT = 0x8A34;
        this.ACTIVE_UNIFORM_BLOCKS = 0x8A36;
        this.UNIFORM_TYPE = 0x8A37;
        this.UNIFORM_SIZE = 0x8A38;
        this.UNIFORM_BLOCK_INDEX = 0x8A3A;
        this.UNIFORM_OFFSET = 0x8A3B;
        this.UNIFORM_ARRAY_STRIDE = 0x8A3C;
        this.UNIFORM_MATRIX_STRIDE = 0x8A3D;
        this.UNIFORM_IS_ROW_MAJOR = 0x8A3E;
        this.UNIFORM_BLOCK_BINDING = 0x8A3F;
        this.UNIFORM_BLOCK_DATA_SIZE = 0x8A40;
        this.UNIFORM_BLOCK_ACTIVE_UNIFORMS = 0x8A42;
        this.UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES = 0x8A43;
        this.UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER = 0x8A44;
        this.UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER = 0x8A46;
        this.INVALID_INDEX = 0xFFFFFFFF;
        this.MAX_VERTEX_OUTPUT_COMPONENTS = 0x9122;
        this.MAX_FRAGMENT_INPUT_COMPONENTS = 0x9125;
        this.MAX_SERVER_WAIT_TIMEOUT = 0x9111;
        this.OBJECT_TYPE = 0x9112;
        this.SYNC_CONDITION = 0x9113;
        this.SYNC_STATUS = 0x9114;
        this.SYNC_FLAGS = 0x9115;
        this.SYNC_FENCE = 0x9116;
        this.SYNC_GPU_COMMANDS_COMPLETE = 0x9117;
        this.UNSIGNALED = 0x9118;
        this.SIGNALED = 0x9119;
        this.ALREADY_SIGNALED = 0x911A;
        this.TIMEOUT_EXPIRED = 0x911B;
        this.CONDITION_SATISFIED = 0x911C;
        this.WAIT_FAILED = 0x911D;
        this.SYNC_FLUSH_COMMANDS_BIT = 0x00000001;
        this.VERTEX_ATTRIB_ARRAY_DIVISOR = 0x88FE;
        this.ANY_SAMPLES_PASSED = 0x8C2F;
        this.ANY_SAMPLES_PASSED_CONSERVATIVE = 0x8D6A;
        this.SAMPLER_BINDING = 0x8919;
        this.RGB10_A2UI = 0x906F;
        this.INT_2_10_10_10_REV = 0x8D9F;
        this.TRANSFORM_FEEDBACK = 0x8E22;
        this.TRANSFORM_FEEDBACK_PAUSED = 0x8E23;
        this.TRANSFORM_FEEDBACK_ACTIVE = 0x8E24;
        this.TRANSFORM_FEEDBACK_BINDING = 0x8E25;
        this.TEXTURE_IMMUTABLE_FORMAT = 0x912F;
        this.MAX_ELEMENT_INDEX = 0x8D6B;
        this.TEXTURE_IMMUTABLE_LEVELS = 0x82DF;
        this.TIMEOUT_IGNORED = -1;
        this.MAX_CLIENT_WAIT_TIMEOUT_WEBGL = 0x9247;
        if (!LayaGLContext.instance) {
            LayaGLContext.__init__();
            LayaGLContext.instance = this;
        }
        else {
            console.log("LayaGL is already exist");
            return LayaGLContext.instance;
        }
        this._nativeObj = layagl;
        this._threadMode = this._nativeObj.getThreadMode();
        contextType = contextType.toLowerCase();
        if (contextType.indexOf("layagl") >= 0) {
            this._nativeObj.setSyncArrayBufferID(LayaGLContext._syncBufferList["_ptrID"]);
            this._nativeObj.setFrameAndSyncCountArrayBufferID(LayaGLContext._frameAndSyncCountBuffer["_ptrID"]);
            this._defaultEncoder = new GLCommandEncoder(this, 102400, 1280, false);
            this._currentCmdEncoder = this._defaultEncoder;
            this._saveCommandEncoder.push(this._currentCmdEncoder);
            this._nativeObj.setRootCommandEncoder(this._currentCmdEncoder._buffer["_ptrID"]);
        }
        else {
            this._nativeObj.setSyncArrayBufferID(LayaGLContext._syncBufferList["_ptrID"]);
            this._nativeObj.setFrameAndSyncCountArrayBufferID(LayaGLContext._frameAndSyncCountBuffer["_ptrID"]);
            this._defaultEncoder = new GLCommandEncoder(this, 102400, 1280, false);
            this._currentCmdEncoder = this._defaultEncoder;
            this._nativeObj.setRootCommandEncoder(this._defaultEncoder._buffer["_ptrID"]);
        }
    }
    static __init__() {
        LayaGLContext._syncBufferList = new Int32Array(LayaGLContext._syncBufferSize);
        LayaGLContext._syncBufferList["conchRef"] = webglPlus.createArrayBufferRef(LayaGLContext._syncBufferList, LayaGLContext.ARRAY_BUFFER_TYPE_DATA, false, LayaGLContext.ARRAY_BUFFER_REF_REFERENCE);
        LayaGLContext._syncBufferList["_ptrID"] = LayaGLContext._syncBufferList["conchRef"].id;
        LayaGLContext._frameAndSyncCountBuffer = new Int32Array(2);
        LayaGLContext._frameAndSyncCountBuffer["conchRef"] = webglPlus.createArrayBufferRef(LayaGLContext._frameAndSyncCountBuffer, LayaGLContext.ARRAY_BUFFER_TYPE_DATA, false, LayaGLContext.ARRAY_BUFFER_REF_REFERENCE);
        LayaGLContext._frameAndSyncCountBuffer["_ptrID"] = LayaGLContext._frameAndSyncCountBuffer["conchRef"].id;
        LayaGLContext._frameAndSyncCountBuffer[0] = 1;
        LayaGLContext._frameAndSyncCountBuffer[1] = 0;
        if (layagl.getThreadMode() > 1) {
            LayaGLContext.prototype.bindBuffer = LayaGLContext.prototype.bindBufferMutiThread;
            LayaGLContext.prototype.useProgram = LayaGLContext.prototype.useProgramMutiThread;
            GLCommandEncoder.prototype.getUniform = GLCommandEncoder.prototype.getUniformMutiThread;
        }
    }
    static getFrameCount() {
        return LayaGLContext._frameAndSyncCountBuffer[0];
    }
    getFrameCount() {
        return LayaGLContext._frameAndSyncCountBuffer[0];
    }
    syncBufferToRenderThread(value, index = 0) {
        if (LayaGLContext.instance._threadMode == LayaGLContext.THREAD_MODE_SINGLE)
            return;
        var bNeedSync = false;
        if (!value._refArray) {
            if (value.frameCount != LayaGLContext._frameAndSyncCountBuffer[0]) {
                value.frameCount = LayaGLContext._frameAndSyncCountBuffer[0];
                bNeedSync = true;
            }
        }
        else {
            var obj = value._refArray[index];
            if (obj.frameCount != LayaGLContext._frameAndSyncCountBuffer[0]) {
                obj.frameCount = LayaGLContext._frameAndSyncCountBuffer[0];
                bNeedSync = true;
            }
        }
        if (bNeedSync) {
            if ((LayaGLContext._frameAndSyncCountBuffer[1] + 1) > LayaGLContext._syncBufferSize) {
                var pre = LayaGLContext._syncBufferList;
                var preConchRef = LayaGLContext._syncBufferList["conchRef"];
                var prePtrID = LayaGLContext._syncBufferList["_ptrID"];
                LayaGLContext._syncBufferSize += LayaGLContext._SYNC_ARRAYBUFFER_SIZE_;
                LayaGLContext._syncBufferList = new Int32Array(LayaGLContext._syncBufferSize);
                LayaGLContext._syncBufferList["conchRef"] = preConchRef;
                LayaGLContext._syncBufferList["_ptrID"] = prePtrID;
                pre && LayaGLContext._syncBufferList.set(pre, 0);
                webglPlus.updateArrayBufferRef(LayaGLContext._syncBufferList["_ptrID"], false, LayaGLContext._syncBufferList);
            }
            var nID = value.getPtrID ? value.getPtrID(index) : value["_ptrID"];
            if (!nID) {
                alert("syncBufferToRenderThread id error");
                debugger;
            }
            LayaGLContext._syncBufferList[LayaGLContext._frameAndSyncCountBuffer[1]++] = nID;
        }
    }
    getDefaultCommandEncoder() {
        return this._defaultEncoder;
    }
    commit() {
        this._saveCommandEncoder.length = 0;
        this._currentCmdEncoder = this._defaultEncoder;
        this._saveCommandEncoder.push(this._currentCmdEncoder);
    }
    setBind(pname, buffer) {
        this._curBindInfo.setBind(pname, buffer);
    }
    getBind(pname) {
        return this._curBindInfo.getBind(pname);
    }
    addGlobalValueDefine(modifyType, type, size, defaultData) {
        return this._nativeObj.addGlobalValueDefine(modifyType, type, size, defaultData);
    }
    endGlobalValueDefine() {
        this._nativeObj.endGlobalValueDefine();
    }
    defineShaderMacro(macroString, uniformInfo) {
        var nID = LayaGLContext._shader_macro_id_;
        var sUniformInfo = "";
        var len = uniformInfo.length;
        for (var i = 0; i < len; i++) {
            sUniformInfo += uniformInfo[i].uname + "," + uniformInfo[i].id.toString() + ",";
        }
        this._nativeObj.defineShaderMacro(nID, macroString, sUniformInfo);
        LayaGLContext._shader_macro_id_ <<= 1;
        return nID;
    }
    setCanvasType(type) {
    }
    setSize(w, h) {
        if (!isNaN(w) && !isNaN(h)) {
            this.width = w;
            this.height = h;
            this._nativeObj.setSize(w, h);
        }
    }
    createCommandEncoder(reserveSize, adjustSize, isSyncToRenderThread) {
        reserveSize = reserveSize ? reserveSize : 128;
        adjustSize = adjustSize ? adjustSize : 64;
        isSyncToRenderThread = isSyncToRenderThread ? isSyncToRenderThread : false;
        var cmd = new GLCommandEncoder(this, reserveSize, adjustSize, isSyncToRenderThread);
        if (isSyncToRenderThread) {
            this.syncBufferToRenderThread(cmd);
        }
        return cmd;
    }
    beginCommandEncoding(commandEncoder) {
        commandEncoder = commandEncoder ? commandEncoder : this._defaultEncoder;
        this._saveCommandEncoder.push(commandEncoder);
        this._currentCmdEncoder = commandEncoder;
    }
    endCommandEncoding() {
        this._saveCommandEncoder.pop();
        this._currentCmdEncoder = this._saveCommandEncoder[this._saveCommandEncoder.length - 1];
    }
    getContextAttributes() {
        return this._currentCmdEncoder.getContextAttributes();
    }
    get drawingBufferWidth() {
        return this.canvas.width;
    }
    get drawingBufferHeight() {
        return this.canvas.height;
    }
    isContextLost() {
        this._currentCmdEncoder.isContextLost();
    }
    getSupportedExtensions() {
        return this._currentCmdEncoder.getSupportedExtensions();
    }
    getExtension(name) {
        return this._currentCmdEncoder.getExtension(name);
    }
    activeTexture(texture) {
        this._currentCmdEncoder.activeTexture(texture);
    }
    attachShader(program, shader) {
        this._currentCmdEncoder.attachShader(program, shader);
    }
    getAttachedShaders(program) {
        let ret = [];
        if (program.vsShader) {
            ret.push(program.vsShader);
        }
        if (program.psShader) {
            ret.push(program.psShader);
        }
        return ret;
    }
    bindAttribLocation(program, index, name) {
        this._currentCmdEncoder.bindAttribLocation(program, index, name);
    }
    bindBuffer(target, buffer) {
        this.setBind(target, buffer);
        this._currentCmdEncoder.bindBuffer(target, buffer);
    }
    bindBufferMutiThread(target, buffer) {
        this._currentCmdEncoder.bindBuffer(target, buffer);
    }
    bindFramebuffer(target, framebuffer) {
        this._currentCmdEncoder.bindFramebuffer(target, framebuffer);
        this.setBind(target, framebuffer);
    }
    bindRenderbuffer(target, renderbuffer) {
        this._currentCmdEncoder.bindRenderbuffer(target, renderbuffer);
        this.setBind(target, renderbuffer);
    }
    clearBufferfv(buffer, drawbuffer, values, srcOffset) {
        this._currentCmdEncoder.clearBufferfv(buffer, drawbuffer, values, srcOffset);
    }
    blitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter) {
        this._currentCmdEncoder.blitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
    }
    bindTexture(target, texture) {
        this._currentCmdEncoder.bindTexture(target, texture);
        this.setBind(target, texture);
    }
    useTexture(texture) {
        this._currentCmdEncoder.useTexture(texture);
    }
    blendColor(red, green, blue, alpha) {
        this._currentCmdEncoder.blendColor(red, green, blue, alpha);
    }
    blendEquation(mode) {
        this._currentCmdEncoder.blendEquation(mode);
    }
    blendEquationSeparate(modeRGB, modeAlpha) {
        this._currentCmdEncoder.blendEquationSeparate(modeRGB, modeAlpha);
    }
    blendFunc(sfactor, dfactor) {
        this._currentCmdEncoder.blendFunc(sfactor, dfactor);
    }
    blendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha) {
        this._currentCmdEncoder.blendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
    }
    bufferData(target, size, usage) {
        this._currentCmdEncoder.bufferData(target, size, usage);
    }
    bufferSubData(target, offset, data) {
        this._currentCmdEncoder.bufferSubData(target, offset, data);
    }
    checkFramebufferStatus(target) {
        return this._currentCmdEncoder.checkFramebufferStatus(target);
    }
    clear(mask) {
        this._currentCmdEncoder.clear(mask);
    }
    clearColor(red, green, blue, alpha) {
        this._currentCmdEncoder.clearColor(red, green, blue, alpha);
    }
    clearDepth(depth) {
        this._currentCmdEncoder.clearDepth(depth);
    }
    clearStencil(s) {
        this._currentCmdEncoder.clearStencil(s);
    }
    colorMask(red, green, blue, alpha) {
        this._currentCmdEncoder.colorMask(red, green, blue, alpha);
    }
    compileShader(shader) {
        this._currentCmdEncoder.compileShader(shader);
    }
    copyTexImage2D(target, level, internalformat, x, y, width, height, border) {
        this._currentCmdEncoder.copyTexImage2D(target, level, internalformat, x, y, width, height, border);
    }
    copyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height) {
        this._currentCmdEncoder.copyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
    }
    createBuffer() {
        return this._currentCmdEncoder.createBuffer();
    }
    createFramebuffer() {
        return this._currentCmdEncoder.createFramebuffer();
    }
    createProgram() {
        return this._currentCmdEncoder.createProgram();
    }
    createRenderbuffer() {
        return this._currentCmdEncoder.createRenderbuffer();
    }
    createShader(type) {
        return this._currentCmdEncoder.createShader(type);
    }
    createTexture() {
        return this._currentCmdEncoder.createTexture();
    }
    cullFace(mode) {
        this._currentCmdEncoder.cullFace(mode);
    }
    deleteBuffer(buffer) {
        this._currentCmdEncoder.deleteBuffer(buffer);
        buffer.id = -1;
    }
    deleteFramebuffer(framebuffer) {
        this._currentCmdEncoder.deleteFramebuffer(framebuffer);
        framebuffer.id = -1;
    }
    deleteProgram(program) {
        this._currentCmdEncoder.deleteProgram(program);
        program.id = -1;
    }
    deleteRenderbuffer(renderbuffer) {
        this._currentCmdEncoder.deleteRenderbuffer(renderbuffer);
        renderbuffer.id = -1;
    }
    deleteShader(shader) {
        this._currentCmdEncoder.deleteShader(shader);
        shader.id = -1;
        shader.src = null;
    }
    deleteTexture(texture) {
        this._currentCmdEncoder.deleteTexture(texture);
        texture.id = -1;
    }
    depthFunc(func) {
        this._currentCmdEncoder.depthFunc(func);
    }
    depthMask(flag) {
        this._currentCmdEncoder.depthMask(flag);
    }
    depthRange(zNear, zFar) {
        this._currentCmdEncoder.depthRange(zNear, zFar);
    }
    detachShader(program, shader) {
        this._currentCmdEncoder.detachShader(program, shader);
    }
    disable(cap) {
        this._currentCmdEncoder.disable(cap);
    }
    disableVertexAttribArray(index) {
        this._currentCmdEncoder.disableVertexAttribArray(index);
    }
    drawArrays(mode, first, count) {
        this._currentCmdEncoder.drawArrays(mode, first, count);
    }
    drawElements(mode, count, type, offset) {
        this._currentCmdEncoder.drawElements(mode, count, type, offset);
    }
    enable(cap) {
        this._currentCmdEncoder.enable(cap);
    }
    enableVertexAttribArray(index) {
        this._currentCmdEncoder.enableVertexAttribArray(index);
    }
    finish() {
        this._currentCmdEncoder.finish();
    }
    flush() {
        this._currentCmdEncoder.flush();
    }
    framebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer) {
        this._currentCmdEncoder.framebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
        var frameBuffer = this.getBind(target);
        if (frameBuffer) {
            frameBuffer.setBind(attachment, renderbuffer);
        }
    }
    framebufferTexture2D(target, attachment, textarget, texture, level) {
        this._currentCmdEncoder.framebufferTexture2D(target, attachment, textarget, texture, level);
        var frameBuffer = this.getBind(target);
        if (frameBuffer) {
            frameBuffer.setBind(attachment, texture);
        }
    }
    frontFace(mode) {
        return this._currentCmdEncoder.frontFace(mode);
    }
    generateMipmap(target) {
        return this._currentCmdEncoder.generateMipmap(target);
    }
    getActiveAttrib(program, index) {
        return this._currentCmdEncoder.getActiveAttrib(program, index);
    }
    getActiveUniform(program, index) {
        return this._currentCmdEncoder.getActiveUniform(program, index);
    }
    getAttribLocation(program, name) {
        return this._currentCmdEncoder.getAttribLocation(program, name);
    }
    getParameter(pname) {
        return this._currentCmdEncoder.getParameter(pname);
    }
    getBufferParameter(target, pname) {
        return this._currentCmdEncoder.getBufferParameter(target, pname);
    }
    getError() {
        return this._currentCmdEncoder.getError();
    }
    getFramebufferAttachmentParameter(target, attachment, pname) {
        return this._currentCmdEncoder.getFramebufferAttachmentParameter(target, attachment, pname);
    }
    getProgramParameter(program, pname) {
        return this._currentCmdEncoder.getProgramParameter(program, pname);
    }
    getProgramInfoLog(program) {
        return this._currentCmdEncoder.getProgramInfoLog(program);
    }
    getRenderbufferParameter(target, pname) {
        return this._currentCmdEncoder.getRenderbufferParameter(target, pname);
    }
    getShaderPrecisionFormat(shaderType, precisionType) {
        return this._currentCmdEncoder.getShaderPrecisionFormat(shaderType, precisionType);
    }
    getShaderParameter(shader, pname) {
        return this._currentCmdEncoder.getShaderParameter(shader, pname);
    }
    getShaderInfoLog(shader) {
        return this._currentCmdEncoder.getShaderInfoLog(shader);
    }
    getShaderSource(shader) {
        return this._currentCmdEncoder.getShaderSource(shader);
    }
    getTexParameter(target, pname) {
        return this._currentCmdEncoder.getTexParameter(target, pname);
    }
    getUniform(program, location) {
        return this._currentCmdEncoder.getUniform(program, location);
    }
    getUniformLocation(program, name) {
        return this._currentCmdEncoder.getUniformLocation(program, name);
    }
    getVertexAttrib(index, pname) {
        return this._currentCmdEncoder.getVertexAttrib(index, pname);
    }
    getVertexAttribOffset(index, pname) {
        return this._currentCmdEncoder.getVertexAttribOffset(index, pname);
    }
    hnumber(target, mode) {
        this._currentCmdEncoder.hint(target, mode);
    }
    hint(target, mode) {
        this._currentCmdEncoder.hint(target, mode);
    }
    isBuffer(buffer) {
        this._currentCmdEncoder.isBuffer(buffer);
    }
    isEnabled(cap) {
        return this._currentCmdEncoder.isEnabled(cap);
    }
    isFramebuffer(framebuffer) {
        this._currentCmdEncoder.isFramebuffer(framebuffer);
    }
    isProgram(program) {
        this._currentCmdEncoder.isProgram(program);
    }
    isRenderbuffer(renderbuffer) {
        this._currentCmdEncoder.isRenderbuffer(renderbuffer);
    }
    isShader(shader) {
        this._currentCmdEncoder.isShader(shader);
    }
    isTexture(texture) {
        this._currentCmdEncoder.isTexture(texture);
    }
    lineWidth(width) {
        this._currentCmdEncoder.lineWidth(width);
    }
    linkProgram(program) {
        this._currentCmdEncoder.linkProgram(program);
    }
    pixelStorei(pname, param) {
        this._currentCmdEncoder.pixelStorei(pname, param);
    }
    polygonOffset(factor, units) {
        this._currentCmdEncoder.polygonOffset(factor, units);
    }
    readPixels(x, y, width, height, format, type, pixels) {
        this._currentCmdEncoder.readPixels(x, y, width, height, format, type, pixels);
    }
    readPixelsAsync(x, y, w, h, format, type, callBack) {
        this._currentCmdEncoder.readPixelsAsync(x, y, w, h, format, type, callBack);
    }
    renderbufferStorage(target, internalformat, width, height) {
        this._currentCmdEncoder.renderbufferStorage(target, internalformat, width, height);
    }
    renderbufferStorageMultisample(target, samples, internalformat, width, height) {
        this._currentCmdEncoder.renderbufferStorageMultisample(target, samples, internalformat, width, height);
    }
    sampleCoverage(value, invert) {
        this._currentCmdEncoder.sampleCoverage(value, invert);
    }
    scissor(x, y, width, height) {
        this._currentCmdEncoder.scissor(x, y, width, height);
    }
    shaderSource(shader, source) {
        this._currentCmdEncoder.shaderSource(shader, source);
    }
    stencilFunc(func, ref, mask) {
        this._currentCmdEncoder.stencilFunc(func, ref, mask);
    }
    stencilFuncSeparate(face, func, ref, mask) {
        this._currentCmdEncoder.stencilFuncSeparate(face, func, ref, mask);
    }
    stencilMask(mask) {
        this._currentCmdEncoder.stencilMask(mask);
    }
    stencilMaskSeparate(face, mask) {
        this._currentCmdEncoder.stencilMaskSeparate(face, mask);
    }
    stencilOp(fail, zfail, zpass) {
        this._currentCmdEncoder.stencilOp(fail, zfail, zpass);
    }
    stencilOpSeparate(face, fail, zfail, zpass) {
        this._currentCmdEncoder.stencilOpSeparate(face, fail, zfail, zpass);
    }
    texImage2D(_args) {
        var args = arguments;
        this._currentCmdEncoder.texImage2D.apply(this._currentCmdEncoder, args);
    }
    texStorage2D(target, levels, internalformat, width, height) {
        this._currentCmdEncoder.texStorage2D(target, levels, internalformat, width, height);
    }
    texParameterf(target, pname, param) {
        this._currentCmdEncoder.texParameterf(target, pname, param);
    }
    texParameteri(target, pname, param) {
        this._currentCmdEncoder.texParameteri(target, pname, param);
    }
    texSubImage2D(_args) {
        var args = arguments;
        this._currentCmdEncoder.texSubImage2D.apply(this._currentCmdEncoder, args);
    }
    uniform1f(location, x) {
        this._currentCmdEncoder.uniform1f(location, x);
    }
    uniform1fv(location, v) {
        this._currentCmdEncoder.uniform1fv(location, v);
    }
    uniform1i(location, x) {
        this._currentCmdEncoder.uniform1i(location, x);
    }
    uniform1iv(location, v) {
        this._currentCmdEncoder.uniform1iv(location, v);
    }
    uniform2f(location, x, y) {
        this._currentCmdEncoder.uniform2f(location, x, y);
    }
    uniform2fv(location, v) {
        this._currentCmdEncoder.uniform2fv(location, v);
    }
    uniform2i(location, x, y) {
        this._currentCmdEncoder.uniform2i(location, x, y);
    }
    uniform2iv(location, v) {
        this._currentCmdEncoder.uniform2iv(location, v);
    }
    uniform3f(location, x, y, z) {
        this._currentCmdEncoder.uniform3f(location, x, y, z);
    }
    uniform3fv(location, v) {
        this._currentCmdEncoder.uniform3fv(location, v);
    }
    uniform3i(location, x, y, z) {
        this._currentCmdEncoder.uniform3i(location, x, y, z);
    }
    uniform3iv(location, v) {
        this._currentCmdEncoder.uniform3iv(location, v);
    }
    uniform4f(location, x, y, z, w) {
        this._currentCmdEncoder.uniform4f(location, x, y, z, w);
    }
    uniform4fv(location, v) {
        this._currentCmdEncoder.uniform4fv(location, v);
    }
    uniform4i(location, x, y, z, w) {
        this._currentCmdEncoder.uniform4i(location, x, y, z, w);
    }
    uniform4iv(location, v) {
        this._currentCmdEncoder.uniform4iv(location, v);
    }
    uniformMatrix2fv(location, transpose, value) {
        this._currentCmdEncoder.uniformMatrix2fv(location, transpose, value);
    }
    uniformMatrix3fv(location, transpose, value) {
        this._currentCmdEncoder.uniformMatrix3fv(location, transpose, value);
    }
    uniformMatrix4fv(location, transpose, value) {
        this._currentCmdEncoder.uniformMatrix4fv(location, transpose, value);
    }
    useProgram(program) {
        this.setBind(LayaGLContext.CURRENT_PROGRAM, program);
        this._currentCmdEncoder.useProgram(program);
    }
    useProgramMutiThread(program) {
        this._currentCmdEncoder.useProgram(program);
    }
    validateProgram(program) {
        this._currentCmdEncoder.validateProgram(program);
    }
    vertexAttrib1f(indx, x) {
        this._currentCmdEncoder.vertexAttrib1f(indx, x);
    }
    vertexAttrib1fv(indx, values) {
        this._currentCmdEncoder.vertexAttrib1fv(indx, values);
    }
    vertexAttrib2f(indx, x, y) {
        this._currentCmdEncoder.vertexAttrib2f(indx, x, y);
    }
    vertexAttrib2fv(indx, values) {
        this._currentCmdEncoder.vertexAttrib2fv(indx, values);
    }
    vertexAttrib3f(indx, x, y, z) {
        this._currentCmdEncoder.vertexAttrib3f(indx, x, y, z);
    }
    vertexAttrib3fv(indx, values) {
        this._currentCmdEncoder.vertexAttrib3fv(indx, values);
    }
    vertexAttrib4f(indx, x, y, z, w) {
        this._currentCmdEncoder.vertexAttrib4f(indx, x, y, z, w);
    }
    vertexAttrib4fv(indx, values) {
        this._currentCmdEncoder.vertexAttrib4fv(indx, values);
    }
    vertexAttribPointer(indx, size, type, normalized, stride, offset) {
        this._currentCmdEncoder.vertexAttribPointer(indx, size, type, normalized, stride, offset);
    }
    viewport(x, y, width, height) {
        this._currentCmdEncoder.viewport(x, y, width, height);
    }
    configureBackBuffer(width, height, antiAlias, enableDepthAndStencil, wantsBestResolution) {
        enableDepthAndStencil = enableDepthAndStencil ? enableDepthAndStencil : true;
        wantsBestResolution = wantsBestResolution ? wantsBestResolution : false;
        this._currentCmdEncoder.configureBackBuffer(width, height, antiAlias, enableDepthAndStencil, wantsBestResolution);
    }
    compressedTexImage2D(_args) {
        var args = arguments;
        this._currentCmdEncoder.compressedTexImage2D.apply(this._currentCmdEncoder, args);
    }
    compressedTexSubImage2D(_args) {
        var args = arguments;
        this._currentCmdEncoder.compressedTexSubImage2D.apply(this._currentCmdEncoder, args);
    }
    createVertexArray() {
        return this._currentCmdEncoder.createVertexArray();
    }
    bindVertexArray(vao) {
        this._currentCmdEncoder.bindVertexArray(vao);
    }
    deleteVertexArray(vao) {
        this._currentCmdEncoder.deleteVertexArray(vao);
    }
    isVertexArray(vao) {
        return this._currentCmdEncoder.isVertexArray(vao);
    }
    vertexAttribDivisor(index, divisor) {
        this._currentCmdEncoder.vertexAttribDivisor(index, divisor);
    }
    drawArraysInstanced(mode, first, count, instanceCount) {
        this._currentCmdEncoder.drawArraysInstanced(mode, first, count, instanceCount);
    }
    drawElementsInstanced(mode, count, type, offset, instanceCount) {
        this._currentCmdEncoder.drawElementsInstanced(mode, count, type, offset, instanceCount);
    }
    uniformMatrix2fvEx(location, transpose, value) {
        this._currentCmdEncoder.uniformMatrix2fvEx(location, transpose, value);
    }
    uniformMatrix3fvEx(location, transpose, value) {
        this._currentCmdEncoder.uniformMatrix3fvEx(location, transpose, value);
    }
    uniformMatrix4fvEx(location, transpose, value) {
        this._currentCmdEncoder.uniformMatrix4fvEx(location, transpose, value);
    }
    addShaderUniform(one) {
        this._currentCmdEncoder.addShaderUniform(one);
    }
    uploadShaderUniforms(commandEncoder, data, type) {
        this._currentCmdEncoder.uploadShaderUniforms(commandEncoder, data, type);
        return 0;
    }
    useCommandEncoder(commandEncoder) {
        this._currentCmdEncoder.useCommandEncoder(commandEncoder);
    }
    loadDataToReg(regNum, data, offset, size) {
        this._currentCmdEncoder.loadDataToReg(regNum, data, offset, size);
    }
    loadDataToRegEx(regNum, dataID, offset, size) {
        this._currentCmdEncoder.loadDataToRegEx(regNum, dataID, offset, size);
    }
    ifLess0(regNum, statementNum) {
        this._currentCmdEncoder.ifLess0(regNum, statementNum);
    }
    ifEqual0(regNum, statementNum) {
        this._currentCmdEncoder.ifEqual0(regNum, statementNum);
    }
    ifGreater0(regNum, statementNum) {
        this._currentCmdEncoder.ifGreater0(regNum, statementNum);
    }
    ifLEqual0(regNum, statementNum) {
        this._currentCmdEncoder.ifLEqual0(regNum, statementNum);
    }
    ifGEqual0(regNum, statementNum) {
        this._currentCmdEncoder.ifGEqual0(regNum, statementNum);
    }
    ifGNotEqual0(regNum, statementNum) {
        this._currentCmdEncoder.ifGNotEqual0(regNum, statementNum);
    }
    operateReg(regOut, reg1, reg2, size, operateType, dataType) {
        this._currentCmdEncoder.operateReg(regOut, reg1, reg2, size, operateType, dataType);
    }
    store(dataID, offset, regID, size) {
        this._currentCmdEncoder.store(dataID, offset, regID, size);
    }
    setMainContextSize(width, height) {
        this._nativeObj.setMainContextSize(width, height);
    }
    getCurrentContext() {
        return this._currentContext;
    }
    getProgramParameterEx(vs, ps, define, pname) {
        return this._nativeObj.getProgramParameterEx(vs, ps, define, pname);
    }
    getActiveAttribEx(vs, ps, define, index) {
        return this._nativeObj.getActiveAttribEx(vs, ps, define, index);
    }
    getActiveUniformEx(vs, ps, define, index) {
        return this._nativeObj.getActiveUniformEx(vs, ps, define, index);
    }
    static createArrayBufferRef(arrayBuffer, type, syncRender) {
        var bufferConchRef = webglPlus.createArrayBufferRef(arrayBuffer, type, syncRender, LayaGLContext.ARRAY_BUFFER_REF_REFERENCE);
        arrayBuffer["conchRef"] = bufferConchRef;
        arrayBuffer["_ptrID"] = bufferConchRef.id;
        return bufferConchRef;
    }
    static createArrayBufferRefs(arrayBuffer, type, syncRender, refType) {
        if (!arrayBuffer._refArray) {
            arrayBuffer._refArray = [];
            arrayBuffer._refNum = 1;
            arrayBuffer._refArray.length = 1;
            arrayBuffer.getRefNum = function () {
                return this._refNum;
            };
            arrayBuffer.clearRefNum = function () {
                this._refNum = 1;
            };
            arrayBuffer.getRefSize = function () {
                return this._refArray.length;
            };
            arrayBuffer.getPtrID = function (index) {
                index = index ? index : 0;
                return this._refArray[index].ptrID;
            };
        }
        var bufferConchRef = null;
        if (refType == LayaGLContext.ARRAY_BUFFER_REF_REFERENCE) {
            var refArray = arrayBuffer._refArray;
            if (!refArray[0]) {
                bufferConchRef = webglPlus.createArrayBufferRef(arrayBuffer, type, syncRender, refType);
                refArray[0] = { "ref": bufferConchRef, "ptrID": bufferConchRef.id };
            }
        }
        else {
            if (arrayBuffer._refNum < arrayBuffer._refArray.length) {
                bufferConchRef = arrayBuffer._refArray[arrayBuffer._refNum].ref;
                var nPtrID = arrayBuffer.getPtrID(arrayBuffer._refNum);
                webglPlus.syncArrayBufferDataToRuntime(nPtrID, bufferConchRef.isSyncToRender(), arrayBuffer);
            }
            else {
                bufferConchRef = webglPlus.createArrayBufferRef(arrayBuffer, type, syncRender, refType);
                arrayBuffer._refArray.push({ "ref": bufferConchRef, "ptrID": bufferConchRef.id });
            }
            arrayBuffer._refNum++;
        }
        return bufferConchRef;
    }
    static syncBufferToRenderThread(value, index = 0) {
        if (LayaGLContext.instance._threadMode == LayaGLContext.THREAD_MODE_SINGLE)
            return;
        var bNeedSync = false;
        if (!value._refArray) {
            if (value.frameCount != LayaGLContext._frameAndSyncCountBuffer[0]) {
                value.frameCount = LayaGLContext._frameAndSyncCountBuffer[0];
                bNeedSync = true;
            }
        }
        else {
            var obj = value._refArray[index];
            if (obj.frameCount != LayaGLContext._frameAndSyncCountBuffer[0]) {
                obj.frameCount = LayaGLContext._frameAndSyncCountBuffer[0];
                bNeedSync = true;
            }
        }
        if (bNeedSync) {
            if ((LayaGLContext._frameAndSyncCountBuffer[1] + 1) > LayaGLContext._syncBufferSize) {
                var pre = LayaGLContext._syncBufferList;
                var preConchRef = LayaGLContext._syncBufferList["conchRef"];
                var prePtrID = LayaGLContext._syncBufferList["_ptrID"];
                LayaGLContext._syncBufferSize += LayaGLContext._SYNC_ARRAYBUFFER_SIZE_;
                LayaGLContext._syncBufferList = new Int32Array(LayaGLContext._syncBufferSize);
                LayaGLContext._syncBufferList["conchRef"] = preConchRef;
                LayaGLContext._syncBufferList["_ptrID"] = prePtrID;
                pre && LayaGLContext._syncBufferList.set(pre, 0);
                webglPlus.updateArrayBufferRef(LayaGLContext._syncBufferList["_ptrID"], false, LayaGLContext._syncBufferList);
            }
            var nID = value.getPtrID ? value.getPtrID(index) : value["_ptrID"];
            if (!nID) {
                alert("syncBufferToRenderThread id error");
                debugger;
            }
            LayaGLContext._syncBufferList[LayaGLContext._frameAndSyncCountBuffer[1]++] = nID;
        }
    }
}
LayaGLContext._SYNC_ARRAYBUFFER_SIZE_ = 4096;
LayaGLContext._syncBufferSize = LayaGLContext._SYNC_ARRAYBUFFER_SIZE_;
LayaGLContext._tempGLEncoder = null;
LayaGLContext._shader_macro_id_ = 1;
LayaGLContext.THREAD_MODE_SINGLE = 1;
LayaGLContext.THREAD_MODE_DOUBLE = 2;
LayaGLContext.EXECUTE_JS_THREAD_BUFFER = 0;
LayaGLContext.EXECUTE_RENDER_THREAD_BUFFER = 1;
LayaGLContext.EXECUTE_COPY_TO_RENDER = 2;
LayaGLContext.EXECUTE_COPY_TO_RENDER3D = 3;
LayaGLContext.VALUE_OPERATE_ADD = 0;
LayaGLContext.VALUE_OPERATE_SUB = 1;
LayaGLContext.VALUE_OPERATE_MUL = 2;
LayaGLContext.VALUE_OPERATE_DIV = 3;
LayaGLContext.VALUE_OPERATE_M2_MUL = 4;
LayaGLContext.VALUE_OPERATE_M3_MUL = 5;
LayaGLContext.VALUE_OPERATE_M4_MUL = 6;
LayaGLContext.VALUE_OPERATE_M32_MUL = 7;
LayaGLContext.VALUE_OPERATE_SET = 8;
LayaGLContext.VALUE_OPERATE_M32_TRANSLATE = 9;
LayaGLContext.VALUE_OPERATE_M32_SCALE = 10;
LayaGLContext.VALUE_OPERATE_M32_ROTATE = 11;
LayaGLContext.VALUE_OPERATE_M32_SCALE_PIVOT = 12;
LayaGLContext.VALUE_OPERATE_M32_ROTATE_PIVOT = 13;
LayaGLContext.VALUE_OPERATE_M32_TRANSFORM_PIVOT = 14;
LayaGLContext.VALUE_OPERATE_BYTE4_COLOR_MUL = 15;
LayaGLContext.ARRAY_BUFFER_TYPE_DATA = 0;
LayaGLContext.ARRAY_BUFFER_TYPE_CMD = 1;
LayaGLContext.ARRAY_BUFFER_REF_REFERENCE = 0;
LayaGLContext.ARRAY_BUFFER_REF_COPY = 1;
LayaGLContext.UPLOAD_SHADER_UNIFORM_TYPE_ID = 0;
LayaGLContext.UPLOAD_SHADER_UNIFORM_TYPE_DATA = 1;
LayaGLContext.DEPTH_BUFFER_BIT = 0x00000100;
LayaGLContext.STENCIL_BUFFER_BIT = 0x00000400;
LayaGLContext.COLOR_BUFFER_BIT = 0x00004000;
LayaGLContext.POINTS = 0x0000;
LayaGLContext.LINES = 0x0001;
LayaGLContext.LINE_LOOP = 0x0002;
LayaGLContext.LINE_STRIP = 0x0003;
LayaGLContext.TRIANGLES = 0x0004;
LayaGLContext.TRIANGLE_STRIP = 0x0005;
LayaGLContext.TRIANGLE_FAN = 0x0006;
LayaGLContext.ZERO = 0;
LayaGLContext.ONE = 1;
LayaGLContext.SRC_COLOR = 0x0300;
LayaGLContext.ONE_MINUS_SRC_COLOR = 0x0301;
LayaGLContext.SRC_ALPHA = 0x0302;
LayaGLContext.ONE_MINUS_SRC_ALPHA = 0x0303;
LayaGLContext.DST_ALPHA = 0x0304;
LayaGLContext.ONE_MINUS_DST_ALPHA = 0x0305;
LayaGLContext.DST_COLOR = 0x0306;
LayaGLContext.ONE_MINUS_DST_COLOR = 0x0307;
LayaGLContext.SRC_ALPHA_SATURATE = 0x0308;
LayaGLContext.FUNC_ADD = 0x8006;
LayaGLContext.BLEND_EQUATION = 0x8009;
LayaGLContext.BLEND_EQUATION_RGB = 0x8009;
LayaGLContext.BLEND_EQUATION_ALPHA = 0x883D;
LayaGLContext.FUNC_SUBTRACT = 0x800A;
LayaGLContext.FUNC_REVERSE_SUBTRACT = 0x800B;
LayaGLContext.BLEND_DST_RGB = 0x80C8;
LayaGLContext.BLEND_SRC_RGB = 0x80C9;
LayaGLContext.BLEND_DST_ALPHA = 0x80CA;
LayaGLContext.BLEND_SRC_ALPHA = 0x80CB;
LayaGLContext.CONSTANT_COLOR = 0x8001;
LayaGLContext.ONE_MINUS_CONSTANT_COLOR = 0x8002;
LayaGLContext.CONSTANT_ALPHA = 0x8003;
LayaGLContext.ONE_MINUS_CONSTANT_ALPHA = 0x8004;
LayaGLContext.BLEND_COLOR = 0x8005;
LayaGLContext.ARRAY_BUFFER = 0x8892;
LayaGLContext.ELEMENT_ARRAY_BUFFER = 0x8893;
LayaGLContext.ARRAY_BUFFER_BINDING = 0x8894;
LayaGLContext.ELEMENT_ARRAY_BUFFER_BINDING = 0x8895;
LayaGLContext.STREAM_DRAW = 0x88E0;
LayaGLContext.STATIC_DRAW = 0x88E4;
LayaGLContext.DYNAMIC_DRAW = 0x88E8;
LayaGLContext.BUFFER_SIZE = 0x8764;
LayaGLContext.BUFFER_USAGE = 0x8765;
LayaGLContext.CURRENT_VERTEX_ATTRIB = 0x8626;
LayaGLContext.FRONT = 0x0404;
LayaGLContext.BACK = 0x0405;
LayaGLContext.FRONT_AND_BACK = 0x0408;
LayaGLContext.CULL_FACE = 0x0B44;
LayaGLContext.BLEND = 0x0BE2;
LayaGLContext.DITHER = 0x0BD0;
LayaGLContext.STENCIL_TEST = 0x0B90;
LayaGLContext.DEPTH_TEST = 0x0B71;
LayaGLContext.SCISSOR_TEST = 0x0C11;
LayaGLContext.POLYGON_OFFSET_FILL = 0x8037;
LayaGLContext.SAMPLE_ALPHA_TO_COVERAGE = 0x809E;
LayaGLContext.SAMPLE_COVERAGE = 0x80A0;
LayaGLContext.INVALID_ENUM = 0x0500;
LayaGLContext.INVALID_VALUE = 0x0501;
LayaGLContext.INVALID_OPERATION = 0x0502;
LayaGLContext.OUT_OF_MEMORY = 0x0505;
LayaGLContext.CW = 0x0900;
LayaGLContext.CCW = 0x0901;
LayaGLContext.LINE_WIDTH = 0x0B21;
LayaGLContext.ALIASED_POINT_SIZE_RANGE = 0x846D;
LayaGLContext.ALIASED_LINE_WIDTH_RANGE = 0x846E;
LayaGLContext.CULL_FACE_MODE = 0x0B45;
LayaGLContext.FRONT_FACE = 0x0B46;
LayaGLContext.DEPTH_RANGE = 0x0B70;
LayaGLContext.DEPTH_WRITEMASK = 0x0B72;
LayaGLContext.DEPTH_CLEAR_VALUE = 0x0B73;
LayaGLContext.DEPTH_FUNC = 0x0B74;
LayaGLContext.STENCIL_CLEAR_VALUE = 0x0B91;
LayaGLContext.STENCIL_FUNC = 0x0B92;
LayaGLContext.STENCIL_FAIL = 0x0B94;
LayaGLContext.STENCIL_PASS_DEPTH_FAIL = 0x0B95;
LayaGLContext.STENCIL_PASS_DEPTH_PASS = 0x0B96;
LayaGLContext.STENCIL_REF = 0x0B97;
LayaGLContext.STENCIL_VALUE_MASK = 0x0B93;
LayaGLContext.STENCIL_WRITEMASK = 0x0B98;
LayaGLContext.STENCIL_BACK_FUNC = 0x8800;
LayaGLContext.STENCIL_BACK_FAIL = 0x8801;
LayaGLContext.STENCIL_BACK_PASS_DEPTH_FAIL = 0x8802;
LayaGLContext.STENCIL_BACK_PASS_DEPTH_PASS = 0x8803;
LayaGLContext.STENCIL_BACK_REF = 0x8CA3;
LayaGLContext.STENCIL_BACK_VALUE_MASK = 0x8CA4;
LayaGLContext.STENCIL_BACK_WRITEMASK = 0x8CA5;
LayaGLContext.VIEWPORT = 0x0BA2;
LayaGLContext.SCISSOR_BOX = 0x0C10;
LayaGLContext.COLOR_CLEAR_VALUE = 0x0C22;
LayaGLContext.COLOR_WRITEMASK = 0x0C23;
LayaGLContext.UNPACK_ALIGNMENT = 0x0CF5;
LayaGLContext.PACK_ALIGNMENT = 0x0D05;
LayaGLContext.MAX_TEXTURE_SIZE = 0x0D33;
LayaGLContext.MAX_VIEWPORT_DIMS = 0x0D3A;
LayaGLContext.SUBPIXEL_BITS = 0x0D50;
LayaGLContext.RED_BITS = 0x0D52;
LayaGLContext.GREEN_BITS = 0x0D53;
LayaGLContext.BLUE_BITS = 0x0D54;
LayaGLContext.ALPHA_BITS = 0x0D55;
LayaGLContext.DEPTH_BITS = 0x0D56;
LayaGLContext.STENCIL_BITS = 0x0D57;
LayaGLContext.POLYGON_OFFSET_UNITS = 0x2A00;
LayaGLContext.POLYGON_OFFSET_FACTOR = 0x8038;
LayaGLContext.TEXTURE_BINDING_2D = 0x8069;
LayaGLContext.SAMPLE_BUFFERS = 0x80A8;
LayaGLContext.SAMPLES = 0x80A9;
LayaGLContext.SAMPLE_COVERAGE_VALUE = 0x80AA;
LayaGLContext.SAMPLE_COVERAGE_INVERT = 0x80AB;
LayaGLContext.COMPRESSED_TEXTURE_FORMATS = 0x86A3;
LayaGLContext.DONT_CARE = 0x1100;
LayaGLContext.FASTEST = 0x1101;
LayaGLContext.NICEST = 0x1102;
LayaGLContext.GENERATE_MIPMAP_HINT = 0x8192;
LayaGLContext.BYTE = 0x1400;
LayaGLContext.UNSIGNED_BYTE = 0x1401;
LayaGLContext.SHORT = 0x1402;
LayaGLContext.UNSIGNED_SHORT = 0x1403;
LayaGLContext.INT = 0x1404;
LayaGLContext.UNSIGNED_INT = 0x1405;
LayaGLContext.FLOAT = 0x1406;
LayaGLContext.DEPTH_COMPONENT = 0x1902;
LayaGLContext.ALPHA = 0x1906;
LayaGLContext.RGB = 0x1907;
LayaGLContext.RGBA = 0x1908;
LayaGLContext.LUMINANCE = 0x1909;
LayaGLContext.LUMINANCE_ALPHA = 0x190A;
LayaGLContext.UNSIGNED_SHORT_4_4_4_4 = 0x8033;
LayaGLContext.UNSIGNED_SHORT_5_5_5_1 = 0x8034;
LayaGLContext.UNSIGNED_SHORT_5_6_5 = 0x8363;
LayaGLContext.FRAGMENT_SHADER = 0x8B30;
LayaGLContext.VERTEX_SHADER = 0x8B31;
LayaGLContext.MAX_VERTEX_ATTRIBS = 0x8869;
LayaGLContext.MAX_VERTEX_UNIFORM_VECTORS = 0x8DFB;
LayaGLContext.MAX_VARYING_VECTORS = 0x8DFC;
LayaGLContext.MAX_COMBINED_TEXTURE_IMAGE_UNITS = 0x8B4D;
LayaGLContext.MAX_VERTEX_TEXTURE_IMAGE_UNITS = 0x8B4C;
LayaGLContext.MAX_TEXTURE_IMAGE_UNITS = 0x8872;
LayaGLContext.MAX_FRAGMENT_UNIFORM_VECTORS = 0x8DFD;
LayaGLContext.MAX_TEXTURE_MAX_ANISOTROPY_EXT = 34047;
LayaGLContext.TEXTURE_MAX_ANISOTROPY_EXT = 34046;
LayaGLContext.SHADER_TYPE = 0x8B4F;
LayaGLContext.DELETE_STATUS = 0x8B80;
LayaGLContext.LINK_STATUS = 0x8B82;
LayaGLContext.VALIDATE_STATUS = 0x8B83;
LayaGLContext.ATTACHED_SHADERS = 0x8B85;
LayaGLContext.ACTIVE_UNIFORMS = 0x8B86;
LayaGLContext.ACTIVE_ATTRIBUTES = 0x8B89;
LayaGLContext.SHADING_LANGUAGE_VERSION = 0x8B8C;
LayaGLContext.CURRENT_PROGRAM = 0x8B8D;
LayaGLContext.NEVER = 0x0200;
LayaGLContext.LESS = 0x0201;
LayaGLContext.EQUAL = 0x0202;
LayaGLContext.LEQUAL = 0x0203;
LayaGLContext.GREATER = 0x0204;
LayaGLContext.NOTEQUAL = 0x0205;
LayaGLContext.GEQUAL = 0x0206;
LayaGLContext.ALWAYS = 0x0207;
LayaGLContext.KEEP = 0x1E00;
LayaGLContext.REPLACE = 0x1E01;
LayaGLContext.INCR = 0x1E02;
LayaGLContext.DECR = 0x1E03;
LayaGLContext.INVERT = 0x150A;
LayaGLContext.INCR_WRAP = 0x8507;
LayaGLContext.DECR_WRAP = 0x8508;
LayaGLContext.VENDOR = 0x1F00;
LayaGLContext.RENDERER = 0x1F01;
LayaGLContext.VERSION = 0x1F02;
LayaGLContext.NEAREST = 0x2600;
LayaGLContext.LINEAR = 0x2601;
LayaGLContext.NEAREST_MIPMAP_NEAREST = 0x2700;
LayaGLContext.LINEAR_MIPMAP_NEAREST = 0x2701;
LayaGLContext.NEAREST_MIPMAP_LINEAR = 0x2702;
LayaGLContext.LINEAR_MIPMAP_LINEAR = 0x2703;
LayaGLContext.TEXTURE_MAG_FILTER = 0x2800;
LayaGLContext.TEXTURE_MIN_FILTER = 0x2801;
LayaGLContext.TEXTURE_WRAP_S = 0x2802;
LayaGLContext.TEXTURE_WRAP_T = 0x2803;
LayaGLContext.TEXTURE_2D = 0x0DE1;
LayaGLContext.TEXTURE = 0x1702;
LayaGLContext.TEXTURE_CUBE_MAP = 0x8513;
LayaGLContext.TEXTURE_BINDING_CUBE_MAP = 0x8514;
LayaGLContext.TEXTURE_CUBE_MAP_POSITIVE_X = 0x8515;
LayaGLContext.TEXTURE_CUBE_MAP_NEGATIVE_X = 0x8516;
LayaGLContext.TEXTURE_CUBE_MAP_POSITIVE_Y = 0x8517;
LayaGLContext.TEXTURE_CUBE_MAP_NEGATIVE_Y = 0x8518;
LayaGLContext.TEXTURE_CUBE_MAP_POSITIVE_Z = 0x8519;
LayaGLContext.TEXTURE_CUBE_MAP_NEGATIVE_Z = 0x851A;
LayaGLContext.MAX_CUBE_MAP_TEXTURE_SIZE = 0x851C;
LayaGLContext.TEXTURE0 = 0x84C0;
LayaGLContext.TEXTURE1 = 0x84C1;
LayaGLContext.TEXTURE2 = 0x84C2;
LayaGLContext.TEXTURE3 = 0x84C3;
LayaGLContext.TEXTURE4 = 0x84C4;
LayaGLContext.TEXTURE5 = 0x84C5;
LayaGLContext.TEXTURE6 = 0x84C6;
LayaGLContext.TEXTURE7 = 0x84C7;
LayaGLContext.TEXTURE8 = 0x84C8;
LayaGLContext.TEXTURE9 = 0x84C9;
LayaGLContext.TEXTURE10 = 0x84CA;
LayaGLContext.TEXTURE11 = 0x84CB;
LayaGLContext.TEXTURE12 = 0x84CC;
LayaGLContext.TEXTURE13 = 0x84CD;
LayaGLContext.TEXTURE14 = 0x84CE;
LayaGLContext.TEXTURE15 = 0x84CF;
LayaGLContext.TEXTURE16 = 0x84D0;
LayaGLContext.TEXTURE17 = 0x84D1;
LayaGLContext.TEXTURE18 = 0x84D2;
LayaGLContext.TEXTURE19 = 0x84D3;
LayaGLContext.TEXTURE20 = 0x84D4;
LayaGLContext.TEXTURE21 = 0x84D5;
LayaGLContext.TEXTURE22 = 0x84D6;
LayaGLContext.TEXTURE23 = 0x84D7;
LayaGLContext.TEXTURE24 = 0x84D8;
LayaGLContext.TEXTURE25 = 0x84D9;
LayaGLContext.TEXTURE26 = 0x84DA;
LayaGLContext.TEXTURE27 = 0x84DB;
LayaGLContext.TEXTURE28 = 0x84DC;
LayaGLContext.TEXTURE29 = 0x84DD;
LayaGLContext.TEXTURE30 = 0x84DE;
LayaGLContext.TEXTURE31 = 0x84DF;
LayaGLContext.ACTIVE_TEXTURE = 0x84E0;
LayaGLContext.REPEAT = 0x2901;
LayaGLContext.CLAMP_TO_EDGE = 0x812F;
LayaGLContext.MIRRORED_REPEAT = 0x8370;
LayaGLContext.FLOAT_VEC2 = 0x8B50;
LayaGLContext.FLOAT_VEC3 = 0x8B51;
LayaGLContext.FLOAT_VEC4 = 0x8B52;
LayaGLContext.INT_VEC2 = 0x8B53;
LayaGLContext.INT_VEC3 = 0x8B54;
LayaGLContext.INT_VEC4 = 0x8B55;
LayaGLContext.BOOL = 0x8B56;
LayaGLContext.BOOL_VEC2 = 0x8B57;
LayaGLContext.BOOL_VEC3 = 0x8B58;
LayaGLContext.BOOL_VEC4 = 0x8B59;
LayaGLContext.FLOAT_MAT2 = 0x8B5A;
LayaGLContext.FLOAT_MAT3 = 0x8B5B;
LayaGLContext.FLOAT_MAT4 = 0x8B5C;
LayaGLContext.SAMPLER_2D = 0x8B5E;
LayaGLContext.SAMPLER_CUBE = 0x8B60;
LayaGLContext.VERTEX_ATTRIB_ARRAY_ENABLED = 0x8622;
LayaGLContext.VERTEX_ATTRIB_ARRAY_SIZE = 0x8623;
LayaGLContext.VERTEX_ATTRIB_ARRAY_STRIDE = 0x8624;
LayaGLContext.VERTEX_ATTRIB_ARRAY_TYPE = 0x8625;
LayaGLContext.VERTEX_ATTRIB_ARRAY_NORMALIZED = 0x886A;
LayaGLContext.VERTEX_ATTRIB_ARRAY_POINTER = 0x8645;
LayaGLContext.VERTEX_ATTRIB_ARRAY_BUFFER_BINDING = 0x889F;
LayaGLContext.IMPLEMENTATION_COLOR_READ_TYPE = 0x8B9A;
LayaGLContext.IMPLEMENTATION_COLOR_READ_FORMAT = 0x8B9B;
LayaGLContext.COMPILE_STATUS = 0x8B81;
LayaGLContext.LOW_FLOAT = 0x8DF0;
LayaGLContext.MEDIUM_FLOAT = 0x8DF1;
LayaGLContext.HIGH_FLOAT = 0x8DF2;
LayaGLContext.LOW_INT = 0x8DF3;
LayaGLContext.MEDIUM_INT = 0x8DF4;
LayaGLContext.HIGH_INT = 0x8DF5;
LayaGLContext.FRAMEBUFFER = 0x8D40;
LayaGLContext.RENDERBUFFER = 0x8D41;
LayaGLContext.RGBA4 = 0x8056;
LayaGLContext.RGB5_A1 = 0x8057;
LayaGLContext.RGB565 = 0x8D62;
LayaGLContext.DEPTH_COMPONENT16 = 0x81A5;
LayaGLContext.STENCIL_INDEX = 0x1901;
LayaGLContext.STENCIL_INDEX8 = 0x8D48;
LayaGLContext.DEPTH_STENCIL = 0x84F9;
LayaGLContext.RENDERBUFFER_WIDTH = 0x8D42;
LayaGLContext.RENDERBUFFER_HEIGHT = 0x8D43;
LayaGLContext.RENDERBUFFER_INTERNAL_FORMAT = 0x8D44;
LayaGLContext.RENDERBUFFER_RED_SIZE = 0x8D50;
LayaGLContext.RENDERBUFFER_GREEN_SIZE = 0x8D51;
LayaGLContext.RENDERBUFFER_BLUE_SIZE = 0x8D52;
LayaGLContext.RENDERBUFFER_ALPHA_SIZE = 0x8D53;
LayaGLContext.RENDERBUFFER_DEPTH_SIZE = 0x8D54;
LayaGLContext.RENDERBUFFER_STENCIL_SIZE = 0x8D55;
LayaGLContext.FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE = 0x8CD0;
LayaGLContext.FRAMEBUFFER_ATTACHMENT_OBJECT_NAME = 0x8CD1;
LayaGLContext.FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL = 0x8CD2;
LayaGLContext.FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE = 0x8CD3;
LayaGLContext.COLOR_ATTACHMENT0 = 0x8CE0;
LayaGLContext.DEPTH_ATTACHMENT = 0x8D00;
LayaGLContext.STENCIL_ATTACHMENT = 0x8D20;
LayaGLContext.DEPTH_STENCIL_ATTACHMENT = 0x821A;
LayaGLContext.NONE = 0;
LayaGLContext.FRAMEBUFFER_COMPLETE = 0x8CD5;
LayaGLContext.FRAMEBUFFER_INCOMPLETE_ATTACHMENT = 0x8CD6;
LayaGLContext.FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT = 0x8CD7;
LayaGLContext.FRAMEBUFFER_INCOMPLETE_DIMENSIONS = 0x8CD9;
LayaGLContext.FRAMEBUFFER_UNSUPPORTED = 0x8CDD;
LayaGLContext.FRAMEBUFFER_BINDING = 0x8CA6;
LayaGLContext.RENDERBUFFER_BINDING = 0x8CA7;
LayaGLContext.MAX_RENDERBUFFER_SIZE = 0x84E8;
LayaGLContext.INVALID_FRAMEBUFFER_OPERATION = 0x0506;
LayaGLContext.UNPACK_FLIP_Y_WEBGL = 0x9240;
LayaGLContext.UNPACK_PREMULTIPLY_ALPHA_WEBGL = 0x9241;
LayaGLContext.CONTEXT_LOST_WEBGL = 0x9242;
LayaGLContext.UNPACK_COLORSPACE_CONVERSION_WEBGL = 0x9243;
LayaGLContext.BROWSER_DEFAULT_WEBGL = 0x9244;
LayaGLContext.RGBA16F = 0x881A;
LayaGLContext.HALF_FLOAT = 0x140B;
window["WebGLRenderingContext"] = LayaGLContext;
window["WebGL2RenderingContext"] = LayaGLContext;
window["LayaGLContext"] = LayaGLContext;
window["ProgramLocationTable"] = ProgramLocationTable;
window["GLCommandEncoder"] = GLCommandEncoder;
window["CallbackFuncObj"] = CallbackFuncObj;
class CSSStyleDeclaration {
    constructor() {
        this._transform = new Float32Array([1, 0, 0, 1, 0, 0]);
        this._styleNumber = 0;
    }
    get length() {
        return this._styleNumber;
    }
    getPropertyPriority(propertyName) {
        return '';
    }
    getPropertyValue(propertyName) {
        return '';
    }
    item(index) {
        return '';
    }
    removeProperty(propertyName) {
        return '';
    }
    setProperty(propertyName, value, priority) {
    }
    set left(l) {
        var nl = parseInt(l);
        this._htmlEle.clientLeft = nl;
    }
    set top(t) {
        var nt = parseInt(t);
        this._htmlEle.clientTop = nt;
    }
    set height(h) {
        var nh = parseInt(h);
        this._htmlEle.clientHeight = nh;
    }
    set width(w) {
        var nw = parseInt(w);
        this._htmlEle.clientWidth = nw;
    }
    set transform(trans) {
        var s1 = trans.split(/[\(,\)]/);
        if (s1 && s1.length > 2) {
            if (s1[0] === 'matrix') {
                if (s1.length >= 7) {
                    this._transform[0] = 1 * s1[1];
                    this._transform[1] = 1 * s1[2];
                    this._transform[2] = 1 * s1[3];
                    this._transform[3] = 1 * s1[4];
                    this._transform[4] = 1 * s1[5];
                    this._transform[5] = 1 * s1[6];
                    if (this._htmlEle && this._htmlEle.onCSS_Transform) {
                        this._htmlEle.onCSS_Transform(this._transform);
                    }
                }
            }
            else if (s1[0] === 'scale') {
                console.log('css scale not implemented');
            }
        }
    }
}
class HTMLElement extends Element {
    constructor() {
        super();
        this.style = new CSSStyleDeclaration();
        this.style._htmlEle = this;
    }
    onCSS_Transform(mat) { }
    ;
    onerror(ev) {
    }
    onload(ev) {
    }
}
window["HTMLElement"] = HTMLElement;
class HTMLDivElement extends HTMLElement {
    constructor() {
        super();
        this.tagName = "DIV";
    }
}
class HTMLScriptElement extends HTMLElement {
    constructor() {
        super();
        this.tagName = "SCRIPT";
    }
    set src(url) {
        var _t = this;
        this._src = location.resolve(url);
        console.log("HTMLScriptElement set src" + this._src);
        document.uploadScript({ "src": this._src, "obj": this });
    }
    get src() {
        return this._src;
    }
    set text(t) {
        this._text = t;
        document.uploadScript({ "text": this._text, "obj": this });
    }
    get text() {
        return this._text;
    }
}
class HTMLCanvasElement extends HTMLElement {
    constructor() {
        super();
        this._isFirst = false;
        this._tranform = null;
        this._hasTransform = false;
        this._clientRect = { left: 0, top: 0, width: 0, height: 0, right: 0, bottom: 0 };
        this.clientLeft;
        this.tagName = "CANVAS";
        this._width = HTMLCanvasElement.defaultWidth;
        this._height = HTMLCanvasElement.defaultHeight;
    }
    setFirst() {
        this._isFirst = true;
        if (this._context) {
            LayaGLContext.instance.canvas = this;
        }
    }
    getBoundingClientRect() {
        var cr = this._clientRect;
        if (this._hasTransform) {
            cr.width = this._width * this._tranform[0];
            cr.height = this._height * this._tranform[3];
            cr.left = this._tranform[4];
            cr.top = this._tranform[5];
            cr.right = cr.left + cr.width;
            cr.bottom = cr.left + cr.height;
        }
        else {
            cr.right = cr.width = this._width;
            cr.bottom = cr.height = this._height;
        }
        return cr;
    }
    getContext(contextType) {
        console.log("getContext type=" + contextType);
        if (!LayaGLContext.instance) {
            LayaGLContext.instance = new LayaGLContext(contextType);
        }
        if (!this._context) {
            this._context = new CanvasRenderingContext(this);
            if (this._width)
                this._context.setSize(this._width, this._height);
            if (this._isFirst) {
                LayaGLContext.instance.canvas = this;
            }
        }
        let version = LayaGLContext.instance._nativeObj.getStringEx(0x1F02);
        if (contextType.indexOf("webgl2") >= 0) {
            if (version.indexOf("OpenGL ES 3.") >= 0) {
                LayaGLContext.instance.canvas = this;
                return LayaGLContext.instance;
            }
            else {
                return null;
            }
        }
        else if (contextType.indexOf("webgl") >= 0) {
            if (version.indexOf("OpenGL ES 2.") >= 0 || version.indexOf("OpenGL ES 3.") >= 0) {
                LayaGLContext.instance.canvas = this;
                return LayaGLContext.instance;
            }
            else {
                return null;
            }
        }
        else {
            return this._context;
        }
    }
    onCSS_Transform(mat) {
        this._tranform = mat;
        this._hasTransform = true;
        var e = new Event('csstransform');
        this.dispatchEvent(e);
        var sw = mat[0];
        var sh = mat[3];
        conchConfig.setScreenScale(sw, sh, mat[4], mat[5]);
        if (sw == 0 || sh == 0) {
            if (mat[4] > mat[5]) {
                console.warn("设置横竖屏错误! 请在初始html页面meta中设置screenorientation为横屏landscape");
            }
            else {
                console.warn("设置横竖屏错误! 请在初始html页面meta中设置screenorientation为竖屏portrait");
            }
        }
    }
    toDataURL(type) {
        return "";
    }
    setSize(w, h) {
        this._width = w;
        this._height = h;
        this._context && this._context.setSize(w, h);
    }
    set width(w) {
        if (this._isFirst && HTMLCanvasElement.RS) {
            this._w = w;
            return;
        }
        if (this._width != w) {
            this._width = w;
            this._context && this._context.setSize(this._width, this._height);
        }
    }
    get width() {
        return this._width;
    }
    set height(h) {
        if (this._isFirst && HTMLCanvasElement.RS) {
            this._h = h;
            return;
        }
        if (this._height != h) {
            this._height = h;
            this._context && this._context.setSize(this._width, this._height);
        }
    }
    get height() {
        return this._height;
    }
    get clientWidth() {
        return this._width;
    }
    get clientHeight() {
        return this._height;
    }
    set clientWidth(v) {
        this._width = v;
    }
    set clientHeight(v) {
        this._height = v;
    }
    toBase64(type, encoderOptions, callback) {
        if (this._context) {
            this._context.toBase64(type, encoderOptions, callback);
        }
    }
    getImageData(x, y, w, h, callBack) {
        if (this._context) {
            this._context.getImageData(x, y, w, h, callBack);
        }
    }
}
HTMLCanvasElement.defaultWidth = 300;
HTMLCanvasElement.defaultHeight = 150;
window.HTMLCanvasElement = HTMLCanvasElement;
class HTMLImageElement extends HTMLElement {
    constructor() {
        super();
        this._nativeObj = null;
        this.complete = false;
        this._enableMerageInAtlas = true;
        this.tagName = 'IMG';
        this._nativeObj = new conchImage();
        this._nativeObj.srcs = 0;
        this._nativeObj.obj = this;
    }
    set src(url) {
        this._nativeObj.onerror = this._nativeOnError;
        this._nativeObj.onload = this._nativeOnload;
        this._nativeObj.srcs = this._nativeObj.srcs || 0;
        if (this._nativeObj.srcs == 0) {
            this._nativeObj.obj = this;
        }
        this._nativeObj.srcs++;
        if (this.isBase64(url)) {
            let index = url.indexOf(';base64,');
            if (index != -1) {
                this._nativeObj.setBase64(url.substr(index + 8));
            }
        }
        else {
            url = location.resolve(url);
            this._nativeObj.setSrc(url);
        }
    }
    isBase64(url) {
        return url.startsWith('data:image/');
    }
    get src() {
        return this._nativeObj.src;
    }
    _nativeOnload() {
        this.srcs = this.srcs || 1;
        this.srcs--;
        var temp = this.obj;
        var e = new Event('load');
        temp.complete = true;
        e.target = this;
        temp.onload && temp.onload(e);
        temp.dispatchEvent(e);
        if (this.srcs == 0) {
            this.obj = null;
        }
    }
    _nativeOnError(e) {
        this.srcs = this.srcs || 1;
        this.srcs--;
        var temp = this.obj;
        var evt = new Event('error');
        evt.target = this;
        if (typeof (e) === 'object') {
            evt['extCode'] = e.extCode;
        }
        else
            evt['extCode'] = e;
        temp.onerror && temp.onerror(evt);
        temp.dispatchEvent(evt);
        if (this.srcs == 0) {
            this.obj = null;
        }
    }
    get width() {
        if (this._nativeObj)
            return this._nativeObj.width;
        return 0;
    }
    get height() {
        if (this._nativeObj)
            return this._nativeObj.height;
        return 0;
    }
    get imgId() {
        return this._nativeObj.conchImgId;
    }
    putData(data) {
        this._nativeObj.onerror = this._nativeOnError;
        this._nativeObj.onload = this._nativeOnload;
        this._nativeObj.putData(data);
    }
    putImageData(data, w, h) {
        this._nativeObj.onerror = this._nativeOnError;
        this._nativeObj.onload = this._nativeOnload;
        this._nativeObj.putBitmapData(data, w, h);
    }
    setPremultiplyAlpha(b) {
        this._nativeObj.setPremultiplyAlpha(b);
    }
    conchDestroy() {
        this._nativeObj.destroy();
    }
    set enableMerageInAtlas(value) {
        this._enableMerageInAtlas = value;
        this._nativeObj.enableMerageInAtlas(value);
    }
    get enableMerageInAtlas() {
        return this._enableMerageInAtlas;
    }
    releaseTexture() {
        this._nativeObj.releaseTexture();
    }
}
window["HTMLImageElement"] = HTMLImageElement;
class HTMLInputElement extends HTMLElement {
    constructor() {
        super();
        this.tagName = "Input";
        this._nativeObj = new ConchInput();
        this.setLeft = this._nativeObj.setLeft.bind(this._nativeObj);
        this.setTop = this._nativeObj.setTop.bind(this._nativeObj);
        this.setWidth = this._nativeObj.setWidth.bind(this._nativeObj);
        this.setHeight = this._nativeObj.setHeight.bind(this._nativeObj);
        this.setOpacity = this._nativeObj.setOpacity.bind(this._nativeObj);
        this.setValue = this._nativeObj.setValue.bind(this._nativeObj);
        this.getValue = this._nativeObj.getValue.bind(this._nativeObj);
        this.setStyle = this._nativeObj.setStyle.bind(this._nativeObj);
        this.setVisible = this._nativeObj.setVisible.bind(this._nativeObj);
        this.focus = this._nativeObj.focus.bind(this._nativeObj);
        this.blur = this._nativeObj.blur.bind(this._nativeObj);
        this.setColor = this._nativeObj.setColor.bind(this._nativeObj);
        this.setFontSize = this._nativeObj.setFontSize.bind(this._nativeObj);
        this.setPos = this._nativeObj.setPos.bind(this._nativeObj);
        this.setSize = this._nativeObj.setSize.bind(this._nativeObj);
        this.setCursorPosition = this._nativeObj.setCursorPosition.bind(this._nativeObj);
        this.setScale = this._nativeObj.setScale.bind(this._nativeObj);
        this.setMaxLength = this._nativeObj.setMaxLength.bind(this._nativeObj);
        this.setType = this._nativeObj.setType.bind(this._nativeObj);
        this.setNumberOnly = this._nativeObj.setNumberOnly.bind(this._nativeObj);
        this.setRegular = this._nativeObj.setRegular.bind(this._nativeObj);
        this.setFont = this._nativeObj.setFont.bind(this._nativeObj);
        this.setMultiAble = this._nativeObj.setMultiAble.bind(this._nativeObj);
        this.setForbidEdit = this._nativeObj.setForbidEdit.bind(this._nativeObj);
        if (this._nativeObj.setBgColor)
            this.setBgColor = this._nativeObj.setBgColor.bind(this._nativeObj);
        else
            this.setBgColor = function (c) { };
    }
    set maxLength(v) {
        this.setMaxLength(v);
    }
    set left(v) {
        this._nativeObj.left = v;
    }
    get left() {
        return this._nativeObj.left;
    }
    set top(v) {
        this._nativeObj.top = v;
    }
    get top() {
        return this._nativeObj.top;
    }
    set width(v) {
        this._nativeObj.width = v;
    }
    get width() {
        return this._nativeObj.width;
    }
    set height(v) {
        this._nativeObj.height = v;
    }
    get height() {
        return this._nativeObj.height;
    }
    set opacity(v) {
        this._nativeObj.opacity = v;
    }
    get opacity() {
        return this._nativeObj.opacity;
    }
    get clientLeft() {
        return this._nativeObj.left;
    }
    set clientLeft(v) {
        this._nativeObj.left = v;
    }
    get clientTop() {
        return this._nativeObj.top;
    }
    set clientTop(v) {
        this._nativeObj.top = v;
    }
    set clientWidth(v) {
        this._nativeObj.width = v;
    }
    get clientWidth() {
        return this._nativeObj.width;
    }
    set clientHeight(v) {
        this._nativeObj.height = v;
    }
    get clientHeight() {
        return this._nativeObj.height;
    }
    set value(v) {
        this._nativeObj.value = v;
    }
    get value() {
        return this._nativeObj.value;
    }
    set visible(v) {
        this._nativeObj.visible = v;
    }
    get visible() {
        return this._nativeObj.visible;
    }
    addEventListener(type, listener, useCapture) {
        super.addEventListener(type, listener, useCapture);
        var _t = this;
        this._nativeObj.addEventListener(type, function (e) {
            var ev = new _lbEvent(type);
            ev.target = this;
            _t.dispatchEvent(ev);
        });
    }
}
let CanPlayTypeResult = {
    Empty: "",
    Maybe: "maybe",
    Probably: "probably"
};
class HTMLMediaElement extends HTMLElement {
    constructor(NativeMediaCreator) {
        super();
        this._src = "";
        this._nativeObj = new NativeMediaCreator();
    }
    set src(val) {
        this._src = val;
        this._nativeObj.src = window.location.resolve(this._src);
    }
    get src() {
        return this._nativeObj.src;
    }
    get currentSrc() {
        return this._src;
    }
    canPlayType(type) {
        return CanPlayTypeResult.Empty;
    }
    load() {
    }
    set currentTime(v) {
        this._nativeObj.currentTime = v;
    }
    get currentTime() {
        return this._nativeObj.currentTime;
    }
    play() {
        this._nativeObj.play();
    }
    pause() {
        this._nativeObj.pause();
    }
    set autoplay(v) {
        this._nativeObj.autoplay = v;
    }
    get autoplay() {
        return this._nativeObj.autoplay;
    }
    set loop(v) {
        this._nativeObj.loop = v;
    }
    get loop() {
        return this._nativeObj.loop;
    }
    set volume(v) {
        this._nativeObj.volume = v;
    }
    get volume() {
        return this._nativeObj.volume;
    }
    set muted(v) {
        this._nativeObj.muted = v;
    }
    get muted() {
        return this._nativeObj.muted;
    }
}
class HTMLMetaElement extends HTMLElement {
    constructor() {
        super();
        this.tagName = "META";
    }
    get httpEquiv() {
        return this["http-equiv"];
    }
    set name(n) {
        this._name = n;
        document._elements[n] = document._elements[n] || [];
        document._elements[n].push(this);
    }
    get name() {
        return this._name;
    }
}
class HTMLAudioElement extends HTMLMediaElement {
    constructor() {
        super(ConchAudio);
        this.readyState = 0;
        this.tagName = "AUDIO";
    }
    setLoop(loop) {
        this._nativeObj.setLoop(loop);
    }
    stop() {
        this._nativeObj.stop();
    }
    set muted(v) {
        this._nativeObj.muted = v;
    }
    get muted() {
        return this._nativeObj.muted;
    }
    get duration() {
        return this._nativeObj.duration;
    }
    addEventListener(type, listener, useCapture) {
        super.addEventListener(type, listener, useCapture);
        var _t = this;
        this._nativeObj.addEventListener(type, function (e) {
            var ev = new _lbEvent(type);
            ev.target = this;
            if (type == "canplaythrough")
                _t.readyState = 4;
            _t.dispatchEvent(ev);
        });
    }
}
class HTMLVideoElement extends HTMLMediaElement {
    constructor() {
        super(ConchVideo);
        this._src = "";
        this.tagName = "VIDEO";
        this._nativeObj._setDispatchEventFunc(this._dispatchEventByType.bind(this));
        this._removeEventListener = this.removeEventListener;
        this.removeEventListener = this.removeEventListenerNew;
    }
    appendChild(node) {
        if (node.src) {
            this.src = node.src;
        }
        return super.appendChild(node);
    }
    get readyState() {
        return this._nativeObj.readyState;
    }
    canPlayType(val) {
        let result = this._nativeObj.canPlayType(val);
        return result ? CanPlayTypeResult.Maybe : CanPlayTypeResult.Empty;
    }
    get paused() { return this._nativeObj.paused; }
    get duration() { return this._nativeObj.duration; }
    get src() { return this._src; }
    set src(val) { super.src = val; }
    load() {
        this._nativeObj.load();
    }
    set width(val) { this._nativeObj.width = val; }
    get width() { return this._nativeObj.width; }
    set height(val) { this._nativeObj.height = val; }
    get height() { return this._nativeObj.height; }
    set videoWidth(val) { this._nativeObj.videoWidth = val; }
    get videoWidth() { return this._nativeObj.videoWidth; }
    set videoHeight(val) { this._nativeObj.videoHeight = val; }
    get videoHeight() { return this._nativeObj.videoHeight; }
    get clientLeft() {
        return this._nativeObj.x;
    }
    set clientLeft(val) {
        this._nativeObj.x = val;
    }
    get clientTop() {
        return this._nativeObj.y;
    }
    set clientTop(val) {
        this._nativeObj.y = val;
    }
    addEventListener(type, listener, useCapture) {
        super.addEventListener(type, listener, useCapture);
        this._nativeObj.addEvent(type);
    }
    removeEventListenerNew(type, listener, useCapture) {
        this._removeEventListener(type, listener, useCapture);
        this._nativeObj.removeEvent(type);
    }
    _dispatchEventByType(type) {
        var ev = new _lbEvent(type);
        ev.target = this;
        this._dispatchEvent(ev);
    }
    _destroy() {
        this._nativeObj._releaseHandler();
    }
}
window["HTMLVideoElement"] = HTMLVideoElement;
class HTMLBodyElement extends HTMLElement {
    constructor() {
        super();
        this.tagName = "BODY";
    }
    get clientHeight() {
        return window.innerHeight;
    }
    set clientHeight(h) {
    }
    get clientWidth() {
        return window.innerWidth;
    }
    set clientWidth(w) {
    }
    appendChild(newChild) {
        if (newChild instanceof HTMLCanvasElement) {
            newChild.setFirst();
        }
        return super.appendChild(newChild);
    }
}
class NodeList extends Array {
    constructor() {
        super();
    }
    item(index) {
        return this[index];
    }
}
class HTMLHeadElement extends HTMLElement {
    constructor() {
        super();
        this.tagName = 'head';
        this.__visible = false;
    }
}
class HTMLCollection extends Array {
    item(nameOrIndex, optionalIndex) {
        return this[nameOrIndex];
    }
    namedItem(name) {
        return null;
    }
}
class Document extends Node {
    constructor() {
        super();
        this.createMap = new Map();
        this._frameEndEvt = new Event('frameend');
        this.scriptTextList = [];
        this._loading = 0;
        this._evalNum = 0;
        this._eventPathCache = null;
        this.all = new HTMLCollection();
        this._elements = [];
        window.document = this;
        this.defaultView = window;
        var cm = this.createMap;
        cm.set('div', this.create_div);
        cm.set('img', this.create_img);
        cm.set('image', this.create_img);
        cm.set('canvas', this.create_canvas);
        cm.set('audio', this.create_audio);
        cm.set('input', this.create_input);
        cm.set('textarea', this.create_input);
        cm.set('video', this.create_video);
        cm.set('script', this.create_script);
        cm.set('meta', this.create_meta);
        var html = new HTMLElement();
        var ww = getInnerWidth();
        var wh = getInnerHeight();
        html.clientWidth = ww;
        html.clientHeight = wh;
        html.tagName = "HTML";
        html.ownerDocument = this;
        this.documentElement = html;
        this._topElement = html;
        this.appendChild(this.documentElement);
        var body = new HTMLBodyElement();
        body.ownerDocument = this;
        this.body = body;
        this.documentElement.appendChild(this.body);
        this.head = new HTMLHeadElement();
        this.documentElement.appendChild(this.head);
        this.dispatchEvent = this._dispatchEvent.bind(this);
        this._frameEndEvt.bubbles = false;
        this.nodeType = 9;
        this.location = window.location;
        var _t = this;
        var temp;
    }
    setReferrer(s) {
        this.referrer = s;
    }
    uploadScript(d) {
        var _t = this;
        d.i = this._loading;
        this._loading++;
        if (d.src) {
            console.log("_downloadAysn:temp.src" + d.src);
            window.downloadfile(d.src, false, function (data) {
                d._stext = data + "\n//@ sourceURL=" + d.src;
                _t._downloadOk(d);
            }, function () {
                var e = new Event("error");
                e.target = e.currentTarget = d.obj;
                d.obj.onerror && d.obj.onerror(e);
            });
        }
        else {
            d._stext = d.text;
            _t._downloadOk(d);
        }
    }
    _downloadOk(d) {
        this.scriptTextList[d.i] = d;
        for (var i = this._evalNum, len = this.scriptTextList.length; i < len; i++) {
            var t = this.scriptTextList[i];
            if (!t)
                return;
            console.log(">>>>>>>>>>>>>>>eval" + t.src);
            var t1 = Date.now();
            window.evalJS(t._stext);
            console.log(">>>>>>>>>>>>>>>>>eval take time:" + (Date.now() - t1));
            var e = new Event("load");
            e.target = e.currentTarget = t.obj;
            t.obj.onload && t.obj.onload(e);
            this._evalNum++;
        }
        if (this._loading == this._evalNum) {
            this._loading = this._evalNum = 0;
            this.scriptTextList.length = 0;
        }
    }
    pickElement(screenx, screeny) {
        return this._topElement;
    }
    createElement(tagName) {
        tagName = tagName.toLowerCase();
        var f = this.createMap.get(tagName);
        var ret = null;
        if (f) {
            ret = f.call(this);
        }
        else {
            return new div();
        }
        return ret;
    }
    createElementNS(tagName) {
        return this.createElement(tagName);
    }
    create_div() {
        var ret = new HTMLDivElement();
        ret.ownerDocument = this;
        return ret;
    }
    create_img() {
        var ret = new HTMLImageElement();
        ret.ownerDocument = this;
        return ret;
    }
    create_canvas() {
        var ret = new HTMLCanvasElement();
        ret.ownerDocument = this;
        return ret;
    }
    create_audio() {
        var ret = new HTMLAudioElement();
        ret.ownerDocument = this;
        return ret;
    }
    create_input() {
        var rs = new HTMLInputElement();
        rs.ownerDocument = this;
        return rs;
    }
    create_video() {
        var ret = new HTMLVideoElement();
        ret.ownerDocument = this;
        return ret;
    }
    create_script() {
        var ret = new HTMLScriptElement();
        ret.ownerDocument = this;
        return ret;
    }
    create_meta() {
        var ret = new HTMLMetaElement();
        ret.ownerDocument = this;
        return ret;
    }
    createDocumentFragment() {
        return new HTMLDivElement();
    }
    onframeend() {
        this._frameEndEvt.eventPhase = Event.AT_TARGET;
        super._fireEventListeners(this._frameEndEvt);
    }
    _dispatchEvent(evt) {
        var ancestores = null;
        if (evt.target)
            ancestores = evt.target.getAncestorsNode();
        if (ancestores == null || ancestores.length == 0) {
            return super._dispatchEvent(evt);
        }
        var ancLen = ancestores.length;
        evt.eventPhase = Event.CAPTURING_PHASE;
        var stop = (function () {
            window.dispatchEvent(evt);
            if (evt._propagationStopped)
                return true;
            for (var i = ancLen - 1; i >= 0; i--) {
                var cnode = ancestores[i];
                evt.currentTarget = cnode;
                cnode.fireEventListeners(evt);
                if (evt._propagationStopped) {
                    return true;
                }
            }
            return false;
        })();
        if (!stop) {
            evt.eventPhase = Event.AT_TARGET;
            evt.currentTarget = evt.target;
            stop = (function () {
                evt.target.fireEventListeners(evt);
                if (evt._propagationStopped)
                    return true;
                return false;
            })();
        }
        if (!stop && !evt.cancelable) {
            evt.eventPhase = Event.BUBBLING_PHASE;
            for (var i = 0; i < ancLen; i++) {
                var cnode = ancestores[i];
                evt.currentTarget = cnode;
                cnode.fireEventListeners(evt);
                if (evt._propagationStopped) {
                    stop = true;
                    break;
                }
            }
            if (!stop) {
                evt.currentTarget = window;
                window.dispatchEvent(evt);
            }
        }
        if (evt.preventDefault) {
        }
        return true;
    }
    set cookie(v) {
        var t = _Cookie.addCookie(v);
        if (t && _Cookie.pushCookie(t)) {
            _Cookie.flush();
        }
    }
    get cookie() {
        return _Cookie.toLocalString();
    }
    loadCookie() {
        this._cookiePath = window.localStorage.fileNamePre + "_cookie.txt";
        var temp = readFileSync(this._cookiePath, "utf8");
        _Cookie.init(temp);
        return true;
    }
    open(url, name, features, replace) {
        throw 'not implements';
    }
    getElementsByTagName(tagname) {
        var d = new NodeList();
        if ("body" == tagname)
            d.push(this.body);
        else if ("head" == tagname)
            d.push(this.head);
        return d;
    }
    onkeydown(ev) {
    }
    onkeypress(ev) {
    }
    onkeyup(ev) {
    }
    onmousedown(ev) {
    }
    onmousemove(ev) { }
    onmouseout(ev) { }
    onmouseover(ev) { }
    onmouseup(ev) { }
    onmousewheel(ev) { }
    ontouchcancel(ev) { }
    ontouchend(ev) { }
    ontouchmove(ev) { }
    ontouchstart(ev) { }
    getElementById(elementId) {
        for (var i = 0, sz = this.all.length; i < sz; i++) {
            var c = this.all[i];
            if (c.id === elementId)
                return c;
        }
        return null;
    }
    getElementsByClassName(classNames) {
        throw 'not implemented';
    }
    getElementsByName(name) {
        return document._elements[name] || [];
    }
    write(value) {
        console.log("The document don't support write function!!!");
    }
}
applyMixins(Document, [Node, GlobalEventHandlers, NodeSelector, DocumentEvent]);
class CloseEvent extends Event {
    constructor() {
        super('close');
    }
    initCloseEvent(typeArg, canBubbleArg, cancelableArg, wasCleanArg, codeArg, reasonArg) {
    }
}
class MessageEvent extends Event {
    constructor(type, eventInitDict) {
        super(type);
    }
    initMessageEvent(typeArg, canBubbleArg, cancelableArg, dataArg, originArg, lastEventIdArg, sourceArg) {
    }
}
class WebSocket extends EventTarget {
    constructor(url) {
        super();
        this._nativeObj = null;
        this._nativeObj = new ConchWebSocket(url);
        this._nativeObj._onmessage = (data) => {
            var evt = new MessageEvent('message');
            evt.data = data;
            evt.target = evt.currentTarget = this;
            this.onmessage && this.onmessage(evt);
        };
    }
    get binaryType() {
        return this._nativeObj.binaryType;
    }
    set binaryType(b) {
        this._nativeObj.binaryType = b;
    }
    get timegap() {
        return this._nativeObj.timegap;
    }
    set onopen(f) {
        this._nativeObj.onopen = function () {
            var e = new Event("open");
            e.target = e.currentTarget = this;
            f(e);
        };
    }
    set onclose(f) {
        this._nativeObj.onclose = f;
    }
    set onerror(f) {
        this._nativeObj.onerror = f;
    }
    close() {
        this._nativeObj.close();
    }
    send(msg) {
        this._nativeObj.send(msg);
    }
    get readyState() {
        return this._nativeObj.readyState;
    }
}
WebSocket.CLOSED = 3;
WebSocket.CLOSING = 2;
WebSocket.CONNECTING = 0;
WebSocket.OPEN = 1;
window.WebSocket = WebSocket;
class DOMParser {
    constructor() {
        this._parser = new _DOMParser();
    }
    set src(s) {
        this._src = location.resolve(s);
        this._parser.src = this._src;
    }
    get src() {
        return this._src;
    }
    set onload(callback) {
        this._parser._onload = callback;
        this._parser.onload = this.nativeObjOnload;
    }
    nativeObjOnload() {
        this._onload();
    }
    get onload() {
        return this._parser._onload;
    }
    set onerror(callback) {
        this._parser._onerror = callback;
        this._parser.onerror = this.nativeObjOnerror;
    }
    nativeObjOnerror() {
        this._onerror();
    }
    get onerror() {
        return this._parser._onerror;
    }
    static initXMl(xml) {
        var result;
        if (!xml) {
            var temp = new _jsXmlNode();
            temp.nodeName = "parsererror";
            temp.textContent = "parsererror error";
            result = new _jsXmlNode();
            result.childNodes[0] = temp;
            return result;
        }
        result = new _jsXmlNode();
        result.nodeName = xml.nodeName;
        result.nodeValue = xml.nodeValue;
        result.nodeType = 1;
        if (result.nodeName == "#cdata-section") {
            result.nodeType = 3;
            result.nodeName = "#text";
        }
        result.textContent = xml.textContent;
        var attrs = xml.attributes;
        for (var i = 0, sz1 = attrs.length; i < sz1; i++) {
            var attr = attrs[i];
            var key = attr.nodeName;
            var tempAttr = new _jsXmlAttr(key, attr.nodeValue);
            result.attributes[i] = tempAttr;
            result.attributes[key] = tempAttr;
        }
        var childs = xml.childNodes;
        for (var i = 0, sz1 = childs.length; i < sz1; i++) {
            var chd = childs[i];
            var nodeName = chd.nodeName;
            result.childNodes[i] = DOMParser.initXMl(chd);
        }
        return result;
    }
    parseFromString(s, t) {
        var xml = this._parser.parseFromString(s, t);
        var root = new _jsXmlDocument();
        root.childNodes[0] = DOMParser.initXMl(xml.childNodes[0]);
        return root;
    }
    getResult() {
        if (!this._result) {
            this._result = new _jsXmlDocument();
            this._result.childNodes[0] = DOMParser.initXMl(this._parser.getResult().childNodes[0]);
        }
        return this._result;
    }
}
window["DOMParser"] = DOMParser;
class _jsXmlAttr {
    constructor(key, value) {
        this.nodeName = key;
        this.nodeValue = this.textContent = value;
    }
    get value() {
        return this.nodeValue;
    }
}
class _jsXmlNode extends _jsXmlAttr {
    constructor() {
        super("", "");
        this.childNodes = [];
        this.childNodes["item"] = function (i) {
            return this[i];
        };
        this.attributes = [];
    }
    get firstChild() {
        return this.childNodes ? this.childNodes[0] : null;
    }
    getElementsByTagName(name) {
        var result = [];
        if (this.nodeName == name)
            result.push(this);
        else {
            for (var i = 0, n = this.childNodes.length; i < n; i++) {
                var son = this.childNodes[i];
                result = result.concat(son.getElementsByTagName(name));
            }
        }
        return result;
    }
    getAttribute(name) {
        var attr = this.attributes[name];
        return attr ? attr["nodeValue"] : "";
    }
}
class _jsXmlDocument extends _jsXmlNode {
}
var GL_CAPS;
(function (GL_CAPS) {
    GL_CAPS[GL_CAPS["NONE"] = 0] = "NONE";
    GL_CAPS[GL_CAPS["TEXTURE_COMPRESSION_PVR"] = 2] = "TEXTURE_COMPRESSION_PVR";
    GL_CAPS[GL_CAPS["TEXTURE_COMPRESSION_ETC1"] = 4] = "TEXTURE_COMPRESSION_ETC1";
    GL_CAPS[GL_CAPS["TEXTURE_COMPRESSION_ETC2"] = 8] = "TEXTURE_COMPRESSION_ETC2";
    GL_CAPS[GL_CAPS["TEXTURE_TPG"] = 32] = "TEXTURE_TPG";
    GL_CAPS[GL_CAPS["INSTANCEING"] = 64] = "INSTANCEING";
})(GL_CAPS || (GL_CAPS = {}));
var _window = new _Window();
window.getComputedStyle = function (ele, parm) {
    return null;
};
window.pageXOffset = window.pageYOffset = 0;
window.localStorage = new Storage();
window.sessionStorage = new WindowSessionStorage();
var location = window.location = new Location;
window.console = new Console();
window.addEventListener = _window.addEventListener.bind(_window);
window.removeEventListener = _window.removeEventListener.bind(_window);
window.dispatchEvent = _window.dispatchEvent.bind(_window);
window.document = new Document();
window.layaDoc = window.document;
window.crypto = new Crypto();
window.devicePixelRatio = 1.0;
var Image = window.Image = HTMLImageElement;
var Audio = window.Audio = HTMLAudioElement;
window.requestAnimationFrame = requestAnimationFrame;
window.cancelAnimationFrame = cancelAnimationFrame;
var parent = window.parent = window;
var frames = window.frames = null;
var navigator = window.navigator = new Navigator();
window.open = (url, target, features, replace) => {
    createProcess('scripts/index.js', url);
    return window;
};
var onload = window.onload = null;
function printstack() {
    var e = new Error();
    alert(e.stack);
}
var div = HTMLDivElement;
window.layabox = {
    devinfo: (function getDevInfo() {
        var devi = conchConfig.getDeviceInfo();
        window.console.log(devi);
        return JSON.parse(devi);
    })(),
    getDeviceInfo: function () { return this.devinfo; }
};
window.layaHtml5 = { File: File };
var _$innerWidth = getInnerWidth();
var _$innerHeight = getInnerHeight();
var _$devicePixelRatio = getDevicePixelRatio();
Object.defineProperty(window, 'innerWidth', { get: function () { return _$innerWidth; } });
Object.defineProperty(window, 'innerHeight', { get: function () { return _$innerHeight; } });
Object.defineProperty(window, 'outerWidth', { get: function () { return _$innerWidth; } });
Object.defineProperty(window, 'outerHeight', { get: function () { return _$innerHeight; } });
Object.defineProperty(window, 'devicePixelRatio', { get: function () { return _$devicePixelRatio; } });
conch.setOnResize(function (w, h) {
    _$innerWidth = w;
    _$innerHeight = h;
    window.console.log(">>>>>>>>>>>>>>>>innerWidth:" + _$innerWidth + "innerHeight:" + _$innerHeight);
    var evt = new UIEvent('resize');
    evt.view = window;
    document._dispatchEvent(evt);
    window.dispatchEvent(evt);
});
conch.config = conchConfig;
class Screen {
    get width() {
        return _$innerWidth;
    }
    get height() {
        return _$innerHeight;
    }
}
window.screen = new Screen();
window.onresize = function (e) {
};
conch.onerror = function (message, filename, lineno, colno, error) {
    if (window.onerror) {
        var ln = decodeTemp(lineno);
        var cn = decodeTemp(colno);
        var er = decodeTemp(error);
        var mg = decodeTemp(message);
        var fn = decodeTemp(filename);
        var e = {
            message: decodeTemp(message),
            stack: er,
            name: ""
        };
        window.onerror(mg == "undefined" ? undefined : mg, fn == "undefined" ? undefined : fn, ln != "undefined" ? parseInt(ln) : undefined, cn != "undefined" ? parseInt(cn) : undefined, e);
    }
};
Object.defineProperty(window, 'onerror', { set: function (fun) {
        conch.__onerror = fun;
        showAlertOnJsException(false);
    }, get: function () {
        return conch.__onerror;
    } });
var document = window.document;
var addEventListener = window.addEventListener.bind(this);
var dispatchEvent = window.dispatchEvent.bind(this);
var removeEventListener = window.removeEventListener.bind(this);
var clearInterval = window.clearInterval = _window.clearInterval;
var clearTimeout = window.clearTimeout = _window.clearTimeout;
var setInterval = window.setInterval = _window.setInterval;
var setTimeout = window.setTimeout = _window.setTimeout;
Object.defineProperty(window, 'runtime', { get: function () { return true; } });
window.postMessage = function (data, d) {
    if (typeof (data) == "object")
        data = JSON.stringify(data);
    conch.callWebviewJS("window.__getMessemage", encodeURIComponent(data), "");
};
window.postRuntimeMessage = function (d) {
    if (typeof (d) == "object")
        d = JSON.stringify(d);
    d = decodeURIComponent(d);
    var e = new MessageEvent('message');
    e.data = JSON.parse(d);
    e.target = window;
    var s = new RegExp("(http|file|https)://([^/:]*)(:(\\d+)|)([^?]+)(.*|)");
    var rs = s.exec(document.referrer);
    if (rs) {
        e.origin = rs[1] + "://" + rs[2] + rs[3];
    }
    window.dispatchEvent(e);
};
window.SetupWebglContext = function () { };
window.downloadfile = function (url, force, onok, onerr) {
    if (force) {
        url = (function (url) {
            var ret = url;
            if (url.indexOf('?') < 0) {
                ret = url + '?rnd=' + Math.random();
            }
            else {
                ret = url + '&downloadrnd' + Math.random().toString().substr(2) + '=1';
            }
            return ret;
        })(url);
    }
    var file = new window.layaHtml5.File(url);
    var filereader = new FileReader();
    filereader.onload = function () { onok && onok(filereader.result); };
    filereader.onerror = function () { onerr && onerr(); };
    filereader.readAsText(file);
};
if (window.navigator.platform != "windows")
    window["ontouchstart"] = null;
window['GL_CAPS'] = GL_CAPS;
window.focus = function () { };
var nMem = conchConfig.getTotalMem();
if (nMem <= 524288) {
    conchConfig.atlasNum = 10;
    conchConfig.maxTextureMemSize = 64 * 1024 * 1024;
}
else if (nMem > 524288 && nMem <= 1048576) {
    conchConfig.atlasNum = 16;
    conchConfig.maxTextureMemSize = 84 * 1024 * 1024;
}
else if (nMem > 1048576) {
    conchConfig.atlasNum = 20;
    conchConfig.maxTextureMemSize = 128 * 1024 * 1024;
}
class Performance {
    now() {
        return tmGetCurms();
    }
}
window["Performance"] = Performance;
window.performance = new Performance();
(function () {
    'use strict';
    var gl = LayaGLContext;
    window["extendWebGLPlusToWebGLContext"](gl);
    class AppInfo {
    }
    ;
    var appobj = null;
    try {
        appobj = JSON.parse(conch.readFileFromAsset('app.json', 'utf8'));
        if (appobj) {
            require(appobj.mainjs);
        }
    }
    catch (e) {
        require('index');
    }
})();
