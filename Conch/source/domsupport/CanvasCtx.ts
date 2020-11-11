///<reference path="./LayaConchRuntime.d.ts" />
class CanvasRenderingContext
{
    private canvas:HTMLCanvasElement;
    public gl:LayaGLContext;
    public _width:number;
    public _height:number;
    constructor(c:HTMLCanvasElement) {
        this.canvas = c;
        this.gl = LayaGLContext.instance;
    }
    setSize(w:number,h:number){
        if (this._width != w || this._height != h) {
            this._width = w;
            this._height = h;
            if(this.canvas._isFirst)
            {
                this.gl.setMainContextSize(this._width,this._height);
            }
        }
    }
    clear():void{
        this.gl.clearColor(0, 0, 0, 0);
        this.gl.clear(LayaGLContext.COLOR_BUFFER_BIT | LayaGLContext.DEPTH_BUFFER_BIT | LayaGLContext.STENCIL_BUFFER_BIT);
    }
    toBase64(type:string, encoderOptions:number, callback:(data:string)=>void):void{
    }
    getImageData(x: number, y: number, w: number, h: number, callBack:(data:ArrayBuffer)=>void):void{
    }
    drawImage(...args):void
    {

    }
    destroy():void {
        this.canvas = null;
        this.gl = null;
    }
    set font(fontName:string) {
        window["_conchTextCanvas"].font = fontName;
    }
    get font():string {
        return window["_conchTextCanvas"].font;
    }
}
window["CanvasRenderingContext"]=CanvasRenderingContext;