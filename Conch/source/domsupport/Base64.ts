class WindowBase64{
    atob:(encodedString: string)=>string;
    btoa:(rawString: string)=> string;
    constructor(){
        this.atob=function(){return null;};
        this.btoa=function(){return null;};
    }
}
window["WindowBase64"]=WindowBase64;