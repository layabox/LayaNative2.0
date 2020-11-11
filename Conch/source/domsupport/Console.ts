 enum LogLevel
 {
    Warn,
	Error,
	Debug,
	Info,
	Runtime 
 }
class Console {
    constructor(){
    }
   
    /**
     * 现在的v8不支持...,先去掉吧
     */
    assert(test?: boolean, message?: string/*, ...optionalParams: any[]*/): void{
        var c = _console;
        /*
        var optionalParams = [];
        for (var _i = 2; _i < arguments.length; _i++) {
            optionalParams[_i - 2] = arguments[_i];
        } 
        */       
        if(test){
            c.log(3,message);//+optionalParams.join())
        };
    }
    clear(): void{}
    /**
     * 输出执行到该行的次数，可选参数 label 可以输出在次数之前
     */
    count(countTitle?: string): void{
    }
    
    debug(message?: string/*, ...optionalParams: any[]*/): void{
        var c = _console;   //这样就不怕别的对象执行这个函数的时候遇到this的问题了
        c.log(LogLevel.Debug,message);//+optionalParams.join());
    }
    /**
     * 将传入对象的属性，包括子对象的属性以列表形式输出
     */
    dir(value?: any/*, ...optionalParams: any[]*/): void{
    }
    
    dirxml(value: any): void{
        
    }
    error(message?: any/*, ...optionalParams: any[]*/): void{
        var c = _console;
        c.log(LogLevel.Error,message);//+optionalParams.join());
    }
    group(groupTitle?: string): void{}
    groupCollapsed(groupTitle?: string): void{}
    groupEnd(): void{}
    info(message?: any/*, ...optionalParams: any[]*/): void{
        var c = _console;
        c.log(LogLevel.Info,message);//+optionalParams.join());
    }
    log(message?: any/*, ...optionalParams: any[]*/): void{
        var c = _console;
        c.log(LogLevel.Info, message);//+optionalParams.join());
    }
    //msIsIndependentlyComposed(element: Element): boolean;
    profile(reportName?: string): void{
    }
    profileEnd(): void{}
    select(element: Element): void{}
    time(timerName?: string): void{}
    timeEnd(timerName?: string): void{}
    trace(message?: any/*, ...optionalParams: any[]*/): void{
    }
    warn(message?: any/*, ...optionalParams: any[]*/): void{
        var c = _console;
        c.log(LogLevel.Warn,message);//+optionalParams.join());
    }  
}

