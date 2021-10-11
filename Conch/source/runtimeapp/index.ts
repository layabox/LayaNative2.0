
window._conchInfo = { version: '2.1.3.1' };
var _inline =!conchConfig.localizable; //是否是单机版
console.log('======================================================  ');
console.log('             LAYA CONCH            ');
console.log('     runtimeversion:' + conchConfig.getRuntimeVersion());
console.log('          jsversion:' + window._conchInfo.version);
console.log('             isplug:' + conchConfig.getIsPlug());
console.log('======================================================');

function log(m) {
    console.log(m);
}
if (conchConfig.getOS() == "Conch-ios"){
    require('promise');
}

function loadLib(url:string) {
    var script = document.createElement("script");
    
    script.src = url;
    script.onerror=function(){
        if(window["onLayaInitError"]){
            window["onLayaInitError"]("Load script error");
        }
    }
    document.head.appendChild(script);
    if(url.indexOf("laya.physics3D.js") >= 0 )
    {
        loadLib(url.replace("laya.physics3D.js","laya.physics3D.runtime.js"));
    }
}
window['loadLib']=loadLib;

import asyncs = require('async');

async function initFreeType() {
    var sOS = conchConfig.getOS();
    var bRet = false;

    //优先使用包中的字体
    var sTempFontPath = conch.getCachePath() + "/runtimeFont/";
    if (!fs_exists(sTempFontPath)) {
        fs_mkdir(sTempFontPath);
    }
    sTempFontPath += "layabox.ttf";
    bRet = _conchTextCanvas.initFreeTypeDefaultFontFromFile(sTempFontPath);
    if(bRet == false){
        var assetFontData = conch.readFileFromAsset('font/layabox.ttf','raw');
        if( assetFontData ){
            fs_writeFileSync(sTempFontPath, assetFontData);
            bRet = _conchTextCanvas.initFreeTypeDefaultFontFromFile(sTempFontPath);
        }
    }
    
    if(!bRet){
        if (sOS == "Conch-window") {
            bRet = _conchTextCanvas.initFreeTypeDefaultFontFromFile("C:/Windows/Fonts/simhei.ttf");
        }
        else if (sOS == "Conch-android") {
            var fSystemVersion = navigator.sv;
            if (fSystemVersion >= 2.0 && fSystemVersion < 5.0) {
                //这个步骤为因为HTC部分手机，作出了一个奇葩的决定：将DroidSansFallback架空，引入DFHEIA5A和DFHEIA7A两个字体文件。
                bRet = _conchTextCanvas.initFreeTypeDefaultFontFromFile("/system/fonts/DFHEIA5A.ttf");
                if (bRet == false) {
                    bRet = _conchTextCanvas.initFreeTypeDefaultFontFromFile("/system/fonts/DroidSansFallback.ttf");
                }
            }
            else if (fSystemVersion >= 5.0 && fSystemVersion < 6.0) {
                var vDefaultStrings = [];
                vDefaultStrings.push("/system/fonts/NotoSansHans-Regular.otf");
                vDefaultStrings.push("/system/fonts/Roboto-Regular.ttf");
                bRet = _conchTextCanvas.initFreeTypeDefaultFontFromFile(vDefaultStrings.join('|'));
            }
            else if (fSystemVersion >= 6.0 && fSystemVersion < 7.0) {
                var vDefaultStrings = [];
                vDefaultStrings.push("/system/fonts/NotoSansSC-Regular.otf");
                vDefaultStrings.push("/system/fonts/Roboto-Regular.ttf");
                bRet = _conchTextCanvas.initFreeTypeDefaultFontFromFile(vDefaultStrings.join('|'));
            }
            else if (fSystemVersion >= 7.0 && fSystemVersion < 8.0) {
                bRet = false;
            }
        }
        else if(sOS=="Conch-ios")
        {
            bRet = _conchTextCanvas.initFreeTypeDefaultFontFromFile("");
        }
    }
    if (bRet == false) {
        log('字体初始化失败，从网络下载字体...');
        var data = <ArrayBuffer>(await asyncs.downloadSync(location.fullpath+'/font/simhei.ttf', true, null));
        if(!data){
            data=<ArrayBuffer>(await asyncs.downloadSync('http://runtime.layabox.com/font/simhei.ttf', true, null));
        }
        if (!data) {
            alert('下载字体失败。 ');
            return;
        }
        fs_writeFileSync(sTempFontPath, data);
        bRet = _conchTextCanvas.initFreeTypeDefaultFontFromFile(sTempFontPath);
    }
    if(!bRet){
        log('字体初始化失败。');
    }
}
//initFreeType();

function setOrientation(s: string) {
    var nameToVal = {
        landscape: 0, portrait: 1, user: 2, behind: 3, sensor: 4, nosensor: 5, sensor_landscape: 6,sensorLandscape: 6,
        sensor_portrait: 7,sensorPortrait: 7, reverse_landscape: 8,reverseLandscape: 8, reverse_portrait: 9,reversePortrait: 9, full_sensor: 10,fullSensor: 10,
    }
    var nOri = (function (name: string) {
        try {
            var n = nameToVal[name];
            return n || 0;
        } catch (e) {
            return 0;
        }
    })(s);

    //if (conchConfig.getScreenOrientation() != nOri) {
        conchConfig.setScreenOrientation(nOri);;
    //}
}

Object.defineProperty(window,'screenOrientation',{
    get:function(){
        return (window as any).___screenOri;
    },
    set:function(v:string){
        (window as any).___screenOri = v;
        setOrientation(v);
    }
});


/**
 * 
 * @param data json字符串
 */
function startApp(data: string) {
    var jsonobj:{scripts:string[],screenorientation:string,screenOrientation:string,}=null;
    try{
        jsonobj = JSON.parse(data);
    }catch(e){
        console.log("Error:start page parse error! \n "+data);
        //TODO 通知全局错误
        return;
    }

    jsonobj.scripts.forEach((v)=>{
        var t = document.createElement("script");
        t["src"]=v;
        t.onerror=function(){
            if(window["onLayaInitError"])
            {
                window["onLayaInitError"]("Load script error");
            }
           
        }
        document.head.appendChild(t);
    });
    if (jsonobj.screenOrientation) setOrientation(jsonobj.screenOrientation);
    else if (jsonobj.screenorientation) setOrientation(jsonobj.screenorientation);
    else setOrientation("sensor_landscape");
    document.createElement("script").text="window.onload&&window.onload()";
}

async function loadApp(url: string) {
    var urllen = url.length;
    if (urllen < 2) return;
    url = url.trim();

    if(url.substring(urllen-1)==='/') url = url +'runtime.json';
    //TODO 用自动把html换成json么。现在的工具会生成html的，所以先替换掉。
    url = url.replace(/.html$/i,'.json');

    if(url.indexOf('http://stand.alone.version')==0)
        _inline=false;
    if( !_inline ){
        url = 'http://stand.alone.version/index.js';
    }
    console.log("loadApp:" + url);

    if (history.length <= 0) {
        history._push(url);
    }
    if (url.length < 2) return;
    location.setHref(url);
    var urlpath = location.fullpath + '/';
    var cache = window.appcache = new AppCache(urlpath);//这时候会加载资源索引，因此如果更新了索引，必须重新创建。
    //为了让启动html也使用缓存，需要在loadUrl刚开始的时候就创建appcache对象。
    document.loadCookie();
    await initFreeType();   //TODO 如果下载这里相当于会卡住。
    try{
        require("config");
    }
    catch(e)
    {
        
    }
    var isDccOk=true;
    async function updateDcc() {
        cache.setResourceID('appurl', urlpath);
        var curassets = cache.getResourceID('netassetsid');
        var assetsidStr =<string>( await asyncs.downloadSync(urlpath + 'update/assetsid.txt?rand=' + Math.random() * Date.now(), false, null));
        console.log("assetsid old:"+curassets+"  new:"+assetsidStr);
        if (!assetsidStr) {
            //对于发布版，这个功能还是不要了把。一旦下载错误，就会导致都重新下载，在网络不好的情况下，恶化了实际表现。
            //如果没有assetsid，则去掉校验功能。
            //cache.saveFileTable('');
            //cache.setResourceID('netassetsid', '');	//去掉，保证下次能正确更新
            //cache = new AppCache(urlpath);
            if(curassets && curassets!="")//如果之前就有dcc，现在下载不到，肯定是网络问题，不能直接进游戏，不然会有问题
            {
                if(window["onLayaInitError"])
                {
                    isDccOk=false;
                    window["onLayaInitError"]("Update DCC get assetsid error");
                }
            }
        } else {
            if (curassets != assetsidStr) {
                log('need update;');
                var txtdcc = '';
                var bindcc = await asyncs.downloadSync(urlpath + 'update/filetable.bin?' + assetsidStr, true, null);
                if (!bindcc || !(bindcc instanceof ArrayBuffer)) {
                    txtdcc = <string>(await asyncs.downloadSync(urlpath + 'update/filetable.txt?' + assetsidStr, false, null));
                } else {
                    if (bindcc.byteLength % 8 != 0) {
                        log('下载的的filetable.bin的长度不对。是不是错了。');
                    } else {
                        var v = new Uint32Array(bindcc);
                        if (v[0] != 0xffeeddcc || v[1] != 1) {//需要校验
                            //onloadfterr();
                            log('dcc.bin file err!');
                        } else {
                            if (v[2] == 0x00ffffff) {
                                //从v[4]开始是32个字节的md5
                                var stp = (4 + 8) / 2;
                                var md5int = v.slice(4, 12);
                                var md5char = new Uint8Array(md5int.buffer);
                                var so = String.fromCharCode.apply(null, md5char);
                                console.log('--------------------------------------------')
                                console.log('so=' + so);
                                console.log('netid=' + assetsidStr);
                                if (so == assetsidStr) {
                                    //一致了
                                    for (var ii = stp, isz = v.length / 2; ii < isz; ii++)
                                        txtdcc += v[ii * 2].toString(16) + ' ' + v[ii * 2 + 1].toString(16) + '\n';
                                }
                            } else {
                                console.log('----------------old format');
                                for (var ii = 1, isz = v.length / 2; ii < isz; ii++)
                                    txtdcc += v[ii * 2].toString(16) + ' ' + v[ii * 2 + 1].toString(16) + '\n';
                            }
                        }
                    }
                }
                if (txtdcc&&txtdcc.length > 0) {
                    cache.saveFileTable(txtdcc);
                    window.appcache = cache = new AppCache(urlpath);
                    //设置新的id。只有写完filetable之后才能写这个
                    cache.setResourceID('netassetsid', assetsidStr);
                } else {
                    //没有得到任何dcc TODO
                    if(window["onLayaInitError"]){
                         isDccOk=false;
                        window["onLayaInitError"]("Update DCC get filetable error");
                    }

                }
            }
        }
    }

    //dcc
    if(_inline){
        await updateDcc();
            if(!isDccOk)
            {
                console.log("init dcc fail");
                return;
            }
    }

    var data: string = await asyncs.loadText(url);
    for(var n=0; n<3 && !data; n++){
        data = await asyncs.loadText(url);
    }
    if(!data){
        if (window["loadingView"]) {
            window["loadingView"].setFontColor("#FF0000");
            window["loadingView"].setTips(['网络异常，请检查您的网络或与开发商联系。']);
        }

        data = cache.loadCachedURL(url);
        if(!data || data.length <= 0)
                 if(window["onLayaInitError"])
                {
                    window["onLayaInitError"]("Load start url error");
                }
            return ;
    }    

    console.log("");//此操作在ios下 能起很大的作用  如果注释掉就会非法 不信你试试 但是最后还是得看看是鸡巴毛原因 TODO
    var qpos = url.indexOf('?');
    if(qpos<0) qpos = url.length;

    if (url.substr(qpos - 3,3) === '.js') {
        window.eval(data + `
        //@ sourceURL=${url}
        `);
        document.createElement("script").text="window.onload&&window.onload()";
    }
    else {//全部当成json处理
        startApp(data);
    }
    if (window["loadingView"] && window["loadingView"].loadingAutoClose) {
        window["loadingView"].hideLoadingView();
    }
}

window.document.addEventListener('keydown', function (e: KeyboardEvent) {
    switch (e.keyCode) {
        case 116://F5
            reloadJS(true);
            break;
        case 117://F6
            history.back();
            break;
        case 118://F7
            //conch.printAllMemorySurvey("","http://10.10.20.114:8889/upload","layabox");
            break;
        case 119://F8
            break;
        case 120:
            gc();
            break;
    }
});
window.loadConchUrl = loadApp;


/**
 * 更新。
 * @param {string} url  zip下载地址
 * @param {(event:string,downloadPercent:number,curfile:string)=>boolean} onEvent  事件的回调。
 *      事件名称：
 *          'downloading' 下载中，这时候downloadPercent有值
 *          'downloadError' 下载错误
 *          'downloadOK' 下载成功。
 *          'updating'  更新中，这时候 curfile有值，表示正在更新的文件
 *          'updateError' curfile更新错误。因为curfile不在dcc列表，或者文件内容与dcc内容不一致。少量更新错误可以忽略，因为在实际使用的时候还是会下载
 *          'unknownError' 
 * @param {function(localfile:string):void} onEnd 更新完成的回调
 */
window['updateByZip'] =function(url, onEvent, onEnd){
    let cachePath = conch.getCachePath();
    let localfile =  cachePath+url.substr(url.lastIndexOf('/'));
    /**
     * 下载文件，保存到localfile中。
     */
    downloadBigFile(url,localfile, 
        //进度回调
        (total:number,now:number,speed:number)=>{
            onEvent('downloading',Math.floor((now/total)*100),null);
            return false;
        },
        //完成回调
        (curlret:number,httpret:number)=>{
            if(curlret!=0 || httpret<200||httpret>=300){
                onEvent('downloadError');
                //throw 'download error';
            }else{
                onEvent('downloadOK');
                //let md5 = calcmd5(fs_readFileSync(localfile));
                //console.log('md5='+md5);
                let zip = new ZipFile();
                if(zip.setSrc(localfile)){
                    zip.forEach((id,name,dir,sz)=>{
                        if(!dir){
                            let buf = zip.readFile(id);
                            //console.log("update "+name);
                            let fid = window.appcache.hashstr('/'+name);    //TODO 测试
                            if(window.appcache.updateFile(fid,0,buf,false)){
                                //更新成功
                                onEvent('updating',null,name);
                            }else{
                                //更新失败。
                                /**
                                 * 更新失败是指没有对应的dcc信息，或者文件内容与dcc中的不一致。
                                 * 这个可以看做不是致命错误。
                                 */
                                onEvent("updateError", null,name);
                                //console.log("update error :"+name);
                            }
                        }
                    });
                    zip.close();
                    if(onEnd) onEnd(localfile);
                }else{
                    console.log( "set zip src error!");
                    onEvent('unknownError');
                }
            }
        },10,100000000);
}
loadApp(conch.presetUrl || "http://nativetest.layabox.com/layaplayer2.0.1/index.js");