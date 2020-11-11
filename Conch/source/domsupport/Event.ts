
interface EventListener {
    (evt: Event): void;
}
interface EventListenerObject {
    handleEvent(evt: Event): void;
}

declare type EventListenerOrEventListenerObject = EventListener | EventListenerObject;

interface EventHandler {
    obj: Object;
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

class GlobalEventHandlers implements GlobalEventHandlers {
    constructor() {

    }
    onpointercancel(ev: PointerEvent): any {
        return null;
    }
    onpointerdown(ev: PointerEvent): any {
        return null;
    }
    onpointerenter(ev: PointerEvent): any {
        return null;
    }
    onpointerleave(ev: PointerEvent): any {
        return null;
    }
    onpointermove(ev: PointerEvent): any {
        return null;
    }
    onpointerout(ev: PointerEvent): any {
        return null;
    }
    onpointerover(ev: PointerEvent): any {
        return null;
    }
    onpointerup(ev: PointerEvent): any {
        return null;
    }
}

class ErrorEvent implements ErrorEvent {
    constructor() {

    }
    initErrorEvent(typeArg: string, canBubbleArg: boolean, cancelableArg: boolean, messageArg: string, filenameArg: string, linenoArg: number): void {
        //TODO
    }
}

class Event {
    //TODO 下面的属性应该是只读的。
    /**
     * 是否允许冒泡
     */
    bubbles: boolean;
    cancelBubble: boolean;  //阻止冒泡
    /**
     * 如果为false，则不允许cancle
     */
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
    _propagationStopped = false;
    constructor(type: string, eventInitDict?: EventInit) {
        this.type = type;
        this.timeStamp = Date.now();
        this.bubbles = false;//默认为false
        this.cancelable = false;
        this.eventPhase = Event.AT_TARGET;
        if (eventInitDict) {
            this.bubbles = eventInitDict.bubbles;
            this.cancelable = eventInitDict.cancelable;
        }
    }
    initEvent(eventTypeArg: string, canBubbleArg: boolean, cancelableArg: boolean): void {
        this.type = eventTypeArg;
        this.bubbles = canBubbleArg;
        this.cancelable = cancelableArg;
    }
    preventDefault(): void {
        if (!this.cancelable)
            return;
        this.defaultPrevented = true;
    }
    stopImmediatePropagation(): void {

    }
    /**
     * 阻止事件的传播。但是不阻止在本节点的传播
     */
    stopPropagation(): void {
        this._propagationStopped = true;
    }

    static AT_TARGET = 2;         //处于目标阶段
    static BUBBLING_PHASE = 3;    //冒泡阶段
    static CAPTURING_PHASE = 1;   //捕获阶段
}
var _lbEvent= window['Event'] = Event;

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
var _lbMap=Map;
class EventTarget implements _EventTarget {
    protected _evtMaps = new _lbMap<string, EventHandler[]>();
    dispatchEvent:(evt: Event)=>boolean ;
    fireEventListeners:(evt: Event)=>void;    
    constructor(){
        this.dispatchEvent = this._dispatchEvent.bind(this);
        //this.addEventListener=this._addEventListener.bind(this);
        this.fireEventListeners = this._fireEventListeners.bind(this);	
    }
    /**
     * TODO 
     * 没有考虑 useCapture
     */
    addEventListener (type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void {
        //console.log('addevtlistener:' + type);
        var listeners = this._evtMaps.get(type) || [];
        listeners.push({ obj: this, listener: listener, useCapture: useCapture });
        this._evtMaps.set(type, listeners);
    }
    /*
    addEventListener (type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void{
        
    } 
    */
    /**
     * @param
     * @return boolean fase 则不允许触发缺省处理，表示有人曾经调用过 preventDefault.  
     */
    protected _dispatchEvent(evt: Event): boolean {
        this.fireEventListeners(evt);
        return !evt.defaultPrevented;
    }
    /**
     * 要考虑一个listener被add了多次的情况
     */
    removeEventListener = (type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void => {
        var listeners = this._evtMaps.get(type);
        if(!listeners)
            return;
        var newlisteners = [];
        for (var i = 0, sz = listeners.length; i < sz; i++) {
            if (listeners[i].listener != listener || listeners[i].useCapture != useCapture) {
                newlisteners.push(listeners[i]);
            }
        }
        this._evtMaps.set(type, newlisteners);
    }

    /**
     * 需要绑定this
     */
    protected _fireEventListeners (evt: Event):void {
        var listeners: EventHandler[] = this._evtMaps.get(evt.type);
        if (listeners) {
            listeners.forEach(function(listener: EventHandler) {
                switch (evt.eventPhase) {
                    case Event.CAPTURING_PHASE:
                        if (listener.useCapture && listener.listener)
                            (<any>listener.listener).call(listener.obj, evt);
                        break;
                    case Event.AT_TARGET:
                        if (listener.listener)
                            (<any>listener.listener).call(listener.obj, evt);
                        break;
                    case Event.BUBBLING_PHASE:
                        if (!listener.useCapture && listener.listener)
                            (<any>listener.listener).call(listener.obj, evt);
                        break;
                }
            });
        }
    }
}

interface UIEventInit extends EventInit {
    view?: any;     //window
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

class UIEvent extends Event {
    constructor(type: string, eventInitDict?: UIEventInit) {
        super(type);
        this.bubbles = true;  //UIEVent的默认为true
        if (eventInitDict) {
            this.initUIEvent(type, eventInitDict.bubbles, eventInitDict.cancelable, eventInitDict.view, eventInitDict.detail);
        }
    }
    detail: number;
    view: Window;
    initUIEvent(typeArg: string, canBubbleArg: boolean, cancelableArg: boolean, viewArg: Window, detailArg: number): void {
        this.type = typeArg;
        this.cancelable = canBubbleArg;
        this.cancelable = cancelableArg;
        this.view = viewArg;
        this.detail = detailArg;
    }
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

class PointerEvent {
    constructor(typeArg: string, eventInitDict?: PointerEventInit) {

    }
}

class MouseEvent extends UIEvent {
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
    constructor(typeArg: string, eventInitDict?: MouseEventInit) {
        super(typeArg, eventInitDict);
        if (eventInitDict) {
            for (var v in eventInitDict) {
                this[v] = eventInitDict[v];
            }
        }
    }
    getModifierState(keyArg: string): boolean {
        //not implements
        return false;
    }

    initMouseEvent(typeArg: string, canBubbleArg: boolean, cancelableArg: boolean, viewArg: Window, detailArg: number, screenXArg: number, screenYArg: number,
        clientXArg: number, clientYArg: number, ctrlKeyArg: boolean, altKeyArg: boolean, shiftKeyArg: boolean, metaKeyArg: boolean,
        buttonArg: number, relatedTargetArg: EventTarget): void {
        var args = arguments;
        ['type', 'bubbles', 'cancelable', 'view', 'detail', 'screenX', 'screenY', 'clientX', 'clientY', 'ctrlKey', 'altKey', 'shiftKey', 'metaKey', 'button', 'relatedTarget'].forEach((v, i, a) => {
            this[v] = args[i];
        });
    }
}
var _lbMouseEvent= window['MouseEvent'] = MouseEvent;

class MouseWheelEvent extends MouseEvent {
    wheelDelta: number;
    wheelDeltaX: number;
    wheelDeltaY: number;
    initMouseWheelEvent(typeArg: string, canBubbleArg: boolean, cancelableArg: boolean, viewArg: Window, detailArg: number, screenXArg: number, screenYArg: number, clientXArg: number, clientYArg: number, buttonArg: number, relatedTargetArg: EventTarget, modifiersListArg: string, wheelDeltaArg: number): void {

    }
    constructor() {
        super("mousewheel");
    }
}

class WheelEvent extends MouseEvent {
    deltaMode: number;
    deltaX: number;
    deltaY: number;
    deltaZ: number;
    getCurrentPoint(element: Element): void {

    }
    initWheelEvent(typeArg: string, canBubbleArg: boolean, cancelableArg: boolean, viewArg: Window, detailArg: number, screenXArg: number, screenYArg: number, clientXArg: number, clientYArg: number, buttonArg: number, relatedTargetArg: EventTarget, modifiersListArg: string, deltaXArg: number, deltaYArg: number, deltaZArg: number, deltaMode: number): void {

    }

    constructor(typeArg: string, eventInitDict?: WheelEventInit) {
        super(typeArg, eventInitDict);
        if (eventInitDict) {
            this.deltaMode = eventInitDict.deltaMode;
            this.deltaX = eventInitDict.deltaX;
            this.deltaY = eventInitDict.deltaY;
            this.deltaZ = eventInitDict.deltaZ;
        }
    }
    static DOM_DELTA_LINE = 1;
    static DOM_DELTA_PAGE = 2;
    static DOM_DELTA_PIXEL = 0;
}

class Touch {
    clientX: number;
    clientY: number;
    identifier: number;
    pageX: number;
    pageY: number;
    screenX: number;
    screenY: number;
    target: EventTarget;
    constructor(){
        
    }
}

class TouchList extends Array {
    item(index: number): Touch {
        return this[index];
    }
}

interface TouchEvent extends UIEvent {
    altKey: boolean;
    changedTouches: TouchList;
    ctrlKey: boolean;
    metaKey: boolean;
    shiftKey: boolean;
    targetTouches: TouchList;
    touches: TouchList;
    new(type: number, id: number, name: string, x: number, y: number):TouchEvent;
}
/*
var exp={exp:null};
(function(exp:{exp:any}) {
    'use strict';
    //这个需要闭包
    var APP_TOUCH_DOWN = 0;
    var APP_TOUCH_UP = 1;
    var APP_TOUCH_MOV = 2;
    var APP_TOUCH_PT_DOWN = 5;
    var APP_TOUCH_PT_UP = 6;
    var m_vTouchs: Touch[] = []

    exp.exp = class TouchEvent extends UIEvent {
        altKey = false;
        changedTouches: Touch[] = [];// TouchList;
        ctrlKey = false;
        metaKey = false;
        shiftKey = false;
        targetTouches: Touch[] = [];// TouchList;
        touches: Touch[] = [];
        constructor(type: number, id: number, name: string, x: number, y: number) {
            super('');
            var touch = new Touch();
            touch.identifier = id;
            touch.pageX = touch.screenX = touch.clientX = x;
            touch.pageY = touch.screenY = touch.clientY = y;
            this.touches.push(touch);
            switch(type) {
                case APP_TOUCH_DOWN:
                case APP_TOUCH_PT_DOWN:
                    m_vTouchs.push(touch);
                    break;
                case APP_TOUCH_PT_UP: {
                    //DEL. 已知事件都是一个一个发送的。
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
                        //error not find
                    } else {
                        m_vTouchs.splice(epos, 1);
                    }
                }
                    break;
                case APP_TOUCH_MOV: {
                    //修改. 已知事件都是一个一个发送的。
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
})(exp);

var TouchEvent:TouchEvent = <TouchEvent>((<any>exp).exp);
*/
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


class DeviceMotionEvent extends Event {
    acceleration: DeviceAcceleration;
    accelerationIncludingGravity: DeviceAcceleration;
    interval: number;
    rotationRate: DeviceRotationRate;
    constructor() {
        super('devicemotion');
    }
    initDeviceMotionEvent(type: string, bubbles: boolean, cancelable: boolean, acceleration: DeviceAccelerationDict, accelerationIncludingGravity: DeviceAccelerationDict, rotationRate: DeviceRotationRateDict, interval: number): void {

    }
}
window["DeviceMotionEvent"]=DeviceMotionEvent;

class DeviceOrientationEvent extends Event {
    absolute: boolean;
    alpha: number;
    beta: number;
    gamma: number;
    constructor() {
        super('deviceorientation');
    }
    initDeviceOrientationEvent(type: string, bubbles: boolean, cancelable: boolean, alpha: number, beta: number, gamma: number, absolute: boolean): void {

    }
}
window["DeviceOrientationEvent"]=DeviceOrientationEvent;

class DocumentEvent {
    constructor() {

    }
    createEvent(eventInterface: string): Event {
        return null;
    }
}

class ProgressEvent extends Event {
    lengthComputable: boolean;
    loaded: number;
    total: number;
    constructor(type:string) {
        super(type);
    }
    initProgressEvent(typeArg: string, canBubbleArg: boolean, cancelableArg: boolean, lengthComputableArg: boolean, loadedArg: number, totalArg: number): void {
        //TODO
        this.type=typeArg;
        this.cancelBubble=canBubbleArg;
        this.cancelable=cancelableArg;
        this.lengthComputable=lengthComputableArg;
        this.loaded=loadedArg;
        this.total=totalArg;
    }
}

var _lbProgressEvent=window["ProgressEvent"]=ProgressEvent;