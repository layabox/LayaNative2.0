class Node extends EventTarget {
    private _childs: Node[] = [];
    private _zip = 0;
    private _z = 0;       //第几层。
    private static _nodeid = 0;
    ownerDocument: Document;
    parentElement: HTMLElement; //父节点如果是Element，这个就有值，并且等于parentNode
    parentNode: Node;
    nodeType:number;//http://www.w3school.com.cn/jsref/prop_node_nodetype.asp
    constructor() {
        super();
        this.nodeType=1;
        this._zip = Node._nodeid++;
    }
    
    get firstChild():Node
    {
        return this._childs?this._childs[0]:null;
    }
    
    set firstChild(node:Node)
    {
        
    }
    
    get childNodes():Node[]
    {
        return this._childs;
    }
    set childNodes(nodes:Node[])
    {
        
    }
    insertBefore(newChild:Node,oldChild:Node)
    {
        //暂时直接append
        this.appendChild(newChild);
    }
    
    appendChild(newChild: Node): Node {
        if(newChild==null)return null;
        if (newChild == this)
            return;
        if (this._childs.indexOf(newChild) < 0) {
            this._childs.push(newChild);
        }

        newChild.parentNode = this;
        newChild._z = this._z + 1;
        var doc = newChild.ownerDocument || this.ownerDocument;
        // alert(`append: child is:
        //     ${newChild.smpJson()}, 
        //     i am:
        //     ${this.smpJson()}`);
        //更新最上层对象.
        //现在是 > 对于庄园来说，第一个是主画布，append到body上，第二个是一个统计信息，也append到body上
        
        
       
        if (doc && newChild._z >=doc._topElement._z && newChild instanceof HTMLCanvasElement&&!(window.document._topElement instanceof HTMLCanvasElement)) {
            var ele = <HTMLElement>newChild;
            if (ele.__visible) {
                window.document._topElement =newChild;
            }
        }
        return newChild;
    }

    /**
     * TODO 现在还没有做深拷贝
     */
    cloneNode(deep?: boolean): Node {
        if(deep){
            //console.error('现在不支持深度clone');
            deep=false;
        }
        function clone(obj) {
            if (typeof (obj) != 'object' || obj == null) return obj;
            var newObj = Object.create(obj.__proto__); //由于不会遍历父类。先只做浅拷贝，直接给prototype赋值 必须用create
            for (var i in obj) {
                if (!deep)
                    newObj[i] = obj[i];
                else//注意：如果是c导出的对象，deep clone会出错，因为new的是Object，而不是c对象了。
                    newObj[i] = clone(obj[i]);
            }
            return newObj;
        }
        return clone(this);
    }
    removeChild(oldChild: Node): Node {
        var p = this._childs.indexOf(oldChild);
        if (p >= 0) {
            this._childs.splice(p, 1);
            if(window.document._topElement==oldChild)
                window.document._topElement=oldChild.parentElement;
            oldChild.parentNode=null;
            oldChild.parentElement=null;
            return oldChild;
        } else
            return null;
    }
    // protected smpJson(): string {
    //     var ret = '{';
    //     var me = this;
    //     for (var v in me) {
    //         var val = me[v].toString();
    //         //if (typeof (val) == 'function') val = 'Function'
    //         ret += '   ' + v + '=' + val + '\n';
    //     }
    //     ret += '}';
    //     return ret;
    // }

    /**
     * 返回父节点。不包括自己。  
     */
    getAncestorsNode(): Node[] {
        var ret = new Array<Node>();
        var obj: Node = this;
        while (obj.parentNode) {
            ret.push(obj.parentNode);
            obj = obj.parentNode;
        }
        return ret;
    }
}

class NodeSelector {
    querySelector(selectors: string): Element {
        return null;
    }
    querySelectorAll(selectors: string): NodeListOf<Element> {
        return <NodeListOf<Element>>[];
    }
}

class ChildNode {
    constructor() { }
    remove(): void {

    }
}

class ElementTraversal {
    constructor() {
    }
}


class Element extends Node implements GlobalEventHandlers, ElementTraversal, NodeSelector, ChildNode {
    tagName: string;
    _id: string;
    className: string;
    innerHTML: string;
    clientHeight: number;
    clientLeft:number;//=0;inputHTMLelement赋值的时候conchinput还么初始化
    clientTop:number;//=0;
    clientWidth: number;
    __visible = true;
    
    set id(s:string){
        this._id=s;
        document.all.push(this);
    }
    get id():string{
        return this._id;
    }

    private _attribs: {};
    querySelector: (selectors: string) => Element; //接口的空函数
    querySelectorAll: (selectors: string) => NodeListOf<Element>;//接口的空函数
    createEvent: (eventInterface: string) => Event;//为了 DocumentEvent 接口的空函数tes
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
    constructor(){
        super();
    }
    //addEventListener:(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean)=>void;
    setAttribute(name: string, value: string): void {
        if(!this._attribs)this._attribs=[];
        this._attribs[name] = value;
    }
    
    getAttribute(name:string):any{
        return this._attribs?this._attribs[name]:null;
    }
}

applyMixins(Element, [Node, GlobalEventHandlers, ElementTraversal, NodeSelector, ChildNode])