
class KeyboardEvent extends UIEvent {
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
    constructor(typeArg: string, eventInitDict?: KeyboardEventInit) {
        super(typeArg);
        if (eventInitDict) {
            var ini = eventInitDict;
            this.altKey = eventInitDict.altKey;
            this.initKeyboardEvent(typeArg, ini.bubbles, ini.cancelable, ini.view, ini.key, ini.location, null, ini.repeat, null);
        }
    }
    initKeyboardEvent(typeArg: string, canBubbleArg: boolean, cancelableArg: boolean, viewArg: Window, keyArg: string, locationArg: number, modifiersListArg: string, repeat: boolean, locale: string): void {
        this.type = typeArg;
        this.cancelable = canBubbleArg;
        this.cancelable = cancelableArg;
        this.key = keyArg;
        this.location = locationArg;
        this.locale = locale;
    }
    static DOM_KEY_LOCATION_JOYSTICK: number;
    static DOM_KEY_LOCATION_LEFT = 1;
    static DOM_KEY_LOCATION_MOBILE: number;
    static DOM_KEY_LOCATION_NUMPAD = 3;
    static DOM_KEY_LOCATION_RIGHT = 2;
    static DOM_KEY_LOCATION_STANDARD = 0;
}
var _lbKeyboardEvent= window["KeyboardEvent"]=KeyboardEvent;

//var layaDoc = <Document>window.layaDoc;
(function(layaDoc: Document) {
    'use strict';
    var APP_TOUCH_DOWN = 0;
    var APP_TOUCH_UP = 1;
    var APP_TOUCH_MOV = 2;
    var APP_TOUCH_PT_DOWN = 5;
    var APP_TOUCH_PT_UP = 6;
    var m_vTouchs: Touch[] = [];

    class touchEvt extends UIEvent {
        changedTouches: Touch[] = [];
        touches: Touch[] = [];
        targetTouches:Touch[];
        constructor(type: number, id: number, name: string, x: number, y: number) {
            super('');
            this.targetTouches = this.changedTouches;
            var touch = new Touch();// touchObj(p_nID, p_sName, p_nX, p_nY);
            //touch.id = p_nID;
            touch.identifier = id;
            touch.pageX = touch.screenX = touch.clientX = x;
            touch.pageY = touch.screenY = touch.clientY = y;
            //touch.type = p_sName;
            //touch.force = 1;
            //touch.radiusX=touch.radiusY=13.8;
            this.changedTouches.push(touch);

            switch (type) {
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

                } break;
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

                } break;
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
        //touches
    }

    var joystickEvt = function(TL_xOffset, TL_yOffset, TR_xOffset, TR_yOffset, LT_Offset, RT_Offset) {
        this.THUMBL_xOffset = TL_xOffset;
        this.THUMBL_yOffset = TL_yOffset;
        this.THUMBR_xOffset = TR_xOffset;
        this.THUMBR_yOffset = TR_yOffset;
        this.LT_Offset = LT_Offset;
        this.RT_Offset = RT_Offset;
    }

    var keyEvt = function(name, code, keychar, flag) { this.type = name; this.keyCode = code; this.keyChar = keychar; this.altKey = flag & 0x1; this.shiftKey = (flag & 0x2) != 0; this.ctrlKey = (flag & 0x4) != 0; this.preventDefault = function() { }; }

    conch.setTouchEvtFunction((touchtype: number, id: number, etype: string, x: number, y: number): void => {
        if (conch.disableMultiTouch && id != 0) return;
        var doc = window.document;
        if (!doc) {
            console.log('touch event cant dispatch!');
            return;
        }
        var evt = new touchEvt(touchtype, id, etype, x, y);
        evt.target = doc.pickElement(x, y);
        doc.dispatchEvent(evt);
    });

/*
    function smpJson(obj): string {
        var ret = '{';
        for (var v in obj) {
            var val = obj[v];
            if (typeof (val) == 'function') val = 'Function'
            ret += '   ' + v + '=' + val + '\n';
        }
        ret += '}';
        return ret;
    }
*/    

    function keyEventHandle() {
        //TODO 问题 用一个ke的话，如果有人保存就会有问题。
        var ke = new _lbKeyboardEvent('');
        return function(type: string, keycode: number, keychar: number, AltShiftCtrl: number) {
            var doc = window.document;
            if (!doc)
                return;
            var keyinit: KeyboardEventInit = {};
            var kc = String.fromCharCode(keycode);
            ke.defaultPrevented=false;
            ke._propagationStopped=false;
            ke.type=type;
            //var evt = new KeyboardEvent(type, { key: kc });
            ke.key = kc;
            ke.keyCode = keycode;
            //console.log('altshifctrl=' + AltShiftCtrl + ',keycod=' + keycode + ',keychar=' + keychar);
            ke.altKey = (AltShiftCtrl & 0x4) != 0;
            ke.shiftKey = (AltShiftCtrl & 0x2) != 0;
            ke.ctrlKey = (AltShiftCtrl & 0x1) != 0;
            ke.target = doc._topElement;
            //TODO 这个是不是不用发事件啊，以提高效率
            doc.dispatchEvent(ke);
            if (!ke.defaultPrevented) {
                //console.log('def evt')
                var f = doc['on' + type];
                if (f) {
                    f.call(doc, ke);
                }
            }
        }
    }
    
    conch.setKeyEvtFunction(keyEventHandle());

    conch.setMouseEvtFunction(function(touchtype: number, type: string, x: number, y: number, wheel: number) {
        var doc = window.document;
        if (!doc) {
            console.log('mouse event cant dispatch!');
            return;
        }
        //console.log('mouseevt:'+type+','+x+','+y+','+wheel)
     
        var target = doc.pickElement(x, y);
        if (wheel != 0) {
            var evt1 = new MouseWheelEvent();
            evt1.clientX = evt1.pageX = evt1.screenX = x;
            evt1.clientY = evt1.pageY = evt1.screenY = y;
            evt1.target=target;
            evt1.wheelDelta = wheel;
            doc.dispatchEvent(evt1);
        }
        else{
            var evt;
            switch(touchtype)
            {
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
            evt.target=target;
            doc.dispatchEvent(evt);
        }
    });
    conch.otherBuffer=new ArrayBuffer(40);
    conch.otherDataView=new DataView(conch.otherBuffer);
    conch.setBuffer(conch.otherBuffer);
    conch.setDeviceMotionEvtFunction(function(type:string,ra:number,rb:number,rg:number){
        if((typeof ra)!="undefined")
        {
            var e=new DeviceOrientationEvent();
            e.alpha=ra;
            e.beta=rb;
            e.gamma=rg;
            window.dispatchEvent(e);
        }
        else
        {
            var d=conch.otherDataView;
            var evt=new DeviceMotionEvent();
            evt.acceleration={x:d.getFloat32(0,true),y:d.getFloat32(4,true),z:d.getFloat32(8,true)};
            evt.accelerationIncludingGravity={x:d.getFloat32(12,true),y:d.getFloat32(16,true),z:d.getFloat32(20,true)};
            evt.rotationRate={alpha:d.getFloat32(24,true),beta:d.getFloat32(28,true),gamma:d.getFloat32(32,true)};
            evt.interval=d.getFloat32(36,true);
            window.dispatchEvent(evt);
        }
    });

    setJoystickEvtFunction(function(type: number, thumbL_xoff: number, thumbL_yoff: number, thumbR_xoff: number, thumbR_yoff: number, LT_offset: number, RT_offset: number) {
        //layaDoc.onEvt( type,new joystickEvt( thumbL_xoff,thumbL_yoff,thumbR_xoff,thumbR_yoff,LT_offset,RT_offset ) );
    });
    //网络监听
    conch.setNetworkEvtFunction( function(type:number){
        var event = new Event("network");
        event["code"] = type;
        document.dispatchEvent( event );
    });

    /*
	Object.defineProperty( layaDoc, "keydown", {   
		get : function () {       
			return null; 
		},    
		set : function ( p_pFunc ) { 
			this.addEventListener( "keydown",p_pFunc );
		} 
	}); 
	Object.defineProperty( layaDoc, "keyup", {   
		get : function () {       
			return null; 
		},
		set : function ( p_pFunc ) { 
			this.addEventListener( "keyup",p_pFunc );
		} 
	});	
	//为了兼容错误的监听 onkeydown  onkeyup
	Object.defineProperty( layaDoc, "onkeydown", {   
		get : function () {       
			return null; 
		},    
		set : function ( p_pFunc ) { 
			this.addEventListener( "onkeydown",p_pFunc );
		} 
	}); 
	Object.defineProperty( layaDoc, "onkeyup", {   
		get : function () {       
			return null; 
		},
		set : function ( p_pFunc ) { 
			this.addEventListener( "onkeyup",p_pFunc );
		} 
	});
    */
})(window.document);
