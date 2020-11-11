


interface WindowTimers{
    clearInterval(handle: number): void;
    clearTimeout(handle: number): void;
    //为了生成的代码简单，不要后面的可变参数了 ...args: any[]
    setInterval(handler: any, timeout?: any): number;
    setTimeout(handler: any, timeout?: any): number;
}

namespace ns_Timer {
    //TODO怎么避免new
    class timerobj{
        interval:number;
        tm:number;
        obj:{};
        func:any;
        args:any;
        num:number;
		del=false;
        constructor(curtm:number, interval:number, obj:{}, func:Function, args:any, num:number){
            this.interval = (interval < 18) ? -1 : interval;//<18则认为全帧
			if( Math.abs(conch.maxInterval-interval)<1)
				this.interval=-1;
            this.tm = curtm + interval;
            this.obj = obj;
            this.func = func;
            this.args = args;
            this.num = num;
        }
    }
    
	//TODO 优化
	var timerqueue = function(){
		this.AniFrame = [];
		this.tmq=new Array<timerobj>();
		this.addTimer=function(tm:number,obj:{},func,args:any,num:number ){
			//this.curtm = Date.now();
			var tmobj = new timerobj(Date.now(),tm,obj,func,args,num);
			this.tmq.push(tmobj);
			return tmobj;
		}
		
		this.delTimer=function(obj){
			for(var i=0,sz=this.tmq.length; i<sz; i++){
				if(this.tmq[i]===obj){
					//this.tmq.splice(i,1);
					this.tmq[i].del=true;
					break;
				}
			}
		}
		this.update=function(){
			this.curtm = Date.now();
			var i=0;
			var btmq=[];
			for(var sz=this.tmq.length; i<sz; i++){
				var ctm:timerobj = this.tmq[i];
				if(ctm.del)continue;
				//log('func['+i+']='+ctm.func);
				var dt = ctm.tm-this.curtm;
				if(dt<0 || ctm.interval<0 || Math.abs(conch.maxInterval-ctm.interval)<1 ){
					//run
					if(typeof(ctm.func)==='function'){
						ctm.func.apply(null,ctm.obj);
					}else{
						eval(ctm.func);
					}
					//多次
					if(ctm.num<0 || --ctm.num>0 ){
						if(dt<0) dt=0;
						ctm.tm=dt+this.curtm+ctm.interval;
						btmq.push(ctm);
					}
				}else{
					btmq.push(ctm);
				}
			}
			this.tmq.splice(0,sz);//因为在运行的时候也可能添加新的定时器。所以不能直接删掉，只能删掉已经运行的
			this.tmq=this.tmq.concat(btmq);
			
			//this.tmq.splice(0,i-1);
		}
	}

	var gTimerQueue = new timerqueue();
    
    export class _WindowTimersExtension {
        clearImmediate(handle: number): void {

        }
        setImmediate(expression: any/*, ...args: any[]*/): number {
            return 0;
        }
    }

    export class _WindowTimers extends _WindowTimersExtension implements WindowTimers {
        constructor() {
            super();
        }
        clearInterval(handle: number): void { 
            gTimerQueue.delTimer(handle);
        }
        clearTimeout(handle: number): void { 
            gTimerQueue.delTimer(handle);
        }
		static ET:Array<any>=[];
        setInterval(handler: any, timeout?:any,parm?:any): number {
			var b=_WindowTimers.ET;
			if(arguments.length>2)
			{
				b= Array.prototype.slice.call(arguments);
				b.splice(0,2);
				handler.arg=b;
			}
            return gTimerQueue.addTimer(timeout,b,handler,null,-1);
        }
        
        //定时器对象
        //现在的问题是不能在回调中删除自己，否则会导致非法：
        //    tm=null; gc();
        setTimeout(handler: any, timeout?: any,parm?:any): number {
            if(!timeout) timeout=0;
			var b=_WindowTimers.ET;
			if(arguments.length>2)
			{
				b= Array.prototype.slice.call(arguments);
				b.splice(0,2);
				handler.arg=b;
			}
            return gTimerQueue.addTimer(timeout,b,handler,null,1);
        }
    }
    var animfrm=0;
    export function requestAnimationFrame(callback: FrameRequestCallback): number{
		var id = animfrm++;
		gTimerQueue.AniFrame.push({id:id,func:callback});
		if(!gTimerQueue.AniFrameStTm)
			gTimerQueue.AniFrameStTm=Date.now();
		return id;
    }
    export function cancelAnimationFrame(handle: number): void{
        
    }
    var framCount=0;
	var fpsTimeout=0;
	conch.setOnFrame(function(){
		if(framCount%30==0)
		{
			var cur=Date.now();
			var space=(cur-fpsTimeout)/30;
			fpsTimeout=cur;
			window["conchFps"]&&window["conchFps"].render(Math.round(1000/space));
		}
		framCount++;
		gTimerQueue.update();
		if(document)
			document.onframeend();
	});
    
    conch.setOnDraw((vsync)=>{
        //console.log('draw');
        var curAnimFrms = gTimerQueue.AniFrame;
        gTimerQueue.AniFrame=[];
        curAnimFrms.forEach(function(af){
            if(af.del)return;
            af.func(vsync);
        });        
    });
}

var WindowTimers = ns_Timer._WindowTimers;
var requestAnimationFrame = ns_Timer.requestAnimationFrame;
var cancelAnimationFrame = ns_Timer.cancelAnimationFrame;
