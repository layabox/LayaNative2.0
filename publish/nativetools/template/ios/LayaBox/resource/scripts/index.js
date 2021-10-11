"use strict";
var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
Object.defineProperty(exports, "__esModule", { value: true });
window._conchInfo = { version: '2.1.3.1' };
var _inline = !conchConfig.localizable;
console.log('======================================================  ');
console.log('             LAYA CONCH            ');
console.log('     runtimeversion:' + conchConfig.getRuntimeVersion());
console.log('          jsversion:' + window._conchInfo.version);
console.log('             isplug:' + conchConfig.getIsPlug());
console.log('======================================================');
function log(m) {
    console.log(m);
}
if (conchConfig.getOS() == "Conch-ios") {
    require('promise');
}
function loadLib(url) {
    var script = document.createElement("script");
    script.src = url;
    script.onerror = function () {
        if (window["onLayaInitError"]) {
            window["onLayaInitError"]("Load script error");
        }
    };
    document.head.appendChild(script);
    if (url.indexOf("laya.physics3D.js") >= 0) {
        loadLib(url.replace("laya.physics3D.js", "laya.physics3D.runtime.js"));
    }
}
window['loadLib'] = loadLib;
const asyncs = require("async");
function initFreeType() {
    return __awaiter(this, void 0, void 0, function* () {
        var sOS = conchConfig.getOS();
        var bRet = false;
        var sTempFontPath = conch.getCachePath() + "/runtimeFont/";
        if (!fs_exists(sTempFontPath)) {
            fs_mkdir(sTempFontPath);
        }
        sTempFontPath += "layabox.ttf";
        bRet = _conchTextCanvas.initFreeTypeDefaultFontFromFile(sTempFontPath);
        if (bRet == false) {
            var assetFontData = conch.readFileFromAsset('font/layabox.ttf', 'raw');
            if (assetFontData) {
                fs_writeFileSync(sTempFontPath, assetFontData);
                bRet = _conchTextCanvas.initFreeTypeDefaultFontFromFile(sTempFontPath);
            }
        }
        if (!bRet) {
            if (sOS == "Conch-window") {
                bRet = _conchTextCanvas.initFreeTypeDefaultFontFromFile("C:/Windows/Fonts/simhei.ttf");
            }
            else if (sOS == "Conch-android") {
                var fSystemVersion = navigator.sv;
                if (fSystemVersion >= 2.0 && fSystemVersion < 5.0) {
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
            else if (sOS == "Conch-ios") {
                bRet = _conchTextCanvas.initFreeTypeDefaultFontFromFile("");
            }
        }
        if (bRet == false) {
            log('字体初始化失败，从网络下载字体...');
            var data = (yield asyncs.downloadSync(location.fullpath + '/font/simhei.ttf', true, null));
            if (!data) {
                data = (yield asyncs.downloadSync('http://runtime.layabox.com/font/simhei.ttf', true, null));
            }
            if (!data) {
                alert('下载字体失败。 ');
                return;
            }
            fs_writeFileSync(sTempFontPath, data);
            bRet = _conchTextCanvas.initFreeTypeDefaultFontFromFile(sTempFontPath);
        }
        if (!bRet) {
            log('字体初始化失败。');
        }
    });
}
function setOrientation(s) {
    var nameToVal = {
        landscape: 0, portrait: 1, user: 2, behind: 3, sensor: 4, nosensor: 5, sensor_landscape: 6, sensorLandscape: 6,
        sensor_portrait: 7, sensorPortrait: 7, reverse_landscape: 8, reverseLandscape: 8, reverse_portrait: 9, reversePortrait: 9, full_sensor: 10, fullSensor: 10,
    };
    var nOri = (function (name) {
        try {
            var n = nameToVal[name];
            return n || 0;
        }
        catch (e) {
            return 0;
        }
    })(s);
    conchConfig.setScreenOrientation(nOri);
    ;
}
Object.defineProperty(window, 'screenOrientation', {
    get: function () {
        return window.___screenOri;
    },
    set: function (v) {
        window.___screenOri = v;
        setOrientation(v);
    }
});
function startApp(data) {
    var jsonobj = null;
    try {
        jsonobj = JSON.parse(data);
    }
    catch (e) {
        console.log("Error:start page parse error! \n " + data);
        return;
    }
    jsonobj.scripts.forEach((v) => {
        var t = document.createElement("script");
        t["src"] = v;
        t.onerror = function () {
            if (window["onLayaInitError"]) {
                window["onLayaInitError"]("Load script error");
            }
        };
        document.head.appendChild(t);
    });
    if (jsonobj.screenOrientation)
        setOrientation(jsonobj.screenOrientation);
    else if (jsonobj.screenorientation)
        setOrientation(jsonobj.screenorientation);
    else
        setOrientation("sensor_landscape");
    document.createElement("script").text = "window.onload&&window.onload()";
}
function loadApp(url) {
    return __awaiter(this, void 0, void 0, function* () {
        var urllen = url.length;
        if (urllen < 2)
            return;
        url = url.trim();
        if (url.substring(urllen - 1) === '/')
            url = url + 'runtime.json';
        url = url.replace(/.html$/i, '.json');
        if (url.indexOf('http://stand.alone.version') == 0)
            _inline = false;
        if (!_inline) {
            url = 'http://stand.alone.version/index.js';
        }
        console.log("loadApp:" + url);
        if (history.length <= 0) {
            history._push(url);
        }
        if (url.length < 2)
            return;
        location.setHref(url);
        var urlpath = location.fullpath + '/';
        var cache = window.appcache = new AppCache(urlpath);
        document.loadCookie();
        yield initFreeType();
        try {
            require("config");
        }
        catch (e) {
        }
        var isDccOk = true;
        function updateDcc() {
            return __awaiter(this, void 0, void 0, function* () {
                cache.setResourceID('appurl', urlpath);
                var curassets = cache.getResourceID('netassetsid');
                var assetsidStr = (yield asyncs.downloadSync(urlpath + 'update/assetsid.txt?rand=' + Math.random() * Date.now(), false, null));
                console.log("assetsid old:" + curassets + "  new:" + assetsidStr);
                if (!assetsidStr) {
                    if (curassets && curassets != "") {
                        if (window["onLayaInitError"]) {
                            isDccOk = false;
                            window["onLayaInitError"]("Update DCC get assetsid error");
                        }
                    }
                }
                else {
                    if (curassets != assetsidStr) {
                        log('need update;');
                        var txtdcc = '';
                        var bindcc = yield asyncs.downloadSync(urlpath + 'update/filetable.bin?' + assetsidStr, true, null);
                        if (!bindcc || !(bindcc instanceof ArrayBuffer)) {
                            txtdcc = (yield asyncs.downloadSync(urlpath + 'update/filetable.txt?' + assetsidStr, false, null));
                        }
                        else {
                            if (bindcc.byteLength % 8 != 0) {
                                log('下载的的filetable.bin的长度不对。是不是错了。');
                            }
                            else {
                                var v = new Uint32Array(bindcc);
                                if (v[0] != 0xffeeddcc || v[1] != 1) {
                                    log('dcc.bin file err!');
                                }
                                else {
                                    if (v[2] == 0x00ffffff) {
                                        var stp = (4 + 8) / 2;
                                        var md5int = v.slice(4, 12);
                                        var md5char = new Uint8Array(md5int.buffer);
                                        var so = String.fromCharCode.apply(null, md5char);
                                        console.log('--------------------------------------------');
                                        console.log('so=' + so);
                                        console.log('netid=' + assetsidStr);
                                        if (so == assetsidStr) {
                                            for (var ii = stp, isz = v.length / 2; ii < isz; ii++)
                                                txtdcc += v[ii * 2].toString(16) + ' ' + v[ii * 2 + 1].toString(16) + '\n';
                                        }
                                    }
                                    else {
                                        console.log('----------------old format');
                                        for (var ii = 1, isz = v.length / 2; ii < isz; ii++)
                                            txtdcc += v[ii * 2].toString(16) + ' ' + v[ii * 2 + 1].toString(16) + '\n';
                                    }
                                }
                            }
                        }
                        if (txtdcc && txtdcc.length > 0) {
                            cache.saveFileTable(txtdcc);
                            window.appcache = cache = new AppCache(urlpath);
                            cache.setResourceID('netassetsid', assetsidStr);
                        }
                        else {
                            if (window["onLayaInitError"]) {
                                isDccOk = false;
                                window["onLayaInitError"]("Update DCC get filetable error");
                            }
                        }
                    }
                }
            });
        }
        if (_inline) {
            yield updateDcc();
            if (!isDccOk) {
                console.log("init dcc fail");
                return;
            }
        }
        var data = yield asyncs.loadText(url);
        for (var n = 0; n < 3 && !data; n++) {
            data = yield asyncs.loadText(url);
        }
        if (!data) {
            if (window["loadingView"]) {
                window["loadingView"].setFontColor("#FF0000");
                window["loadingView"].setTips(['网络异常，请检查您的网络或与开发商联系。']);
            }
            data = cache.loadCachedURL(url);
            if (!data || data.length <= 0)
                if (window["onLayaInitError"]) {
                    window["onLayaInitError"]("Load start url error");
                }
            return;
        }
        console.log("");
        var qpos = url.indexOf('?');
        if (qpos < 0)
            qpos = url.length;
        if (url.substr(qpos - 3, 3) === '.js') {
            window.eval(data + `
        //@ sourceURL=${url}
        `);
            document.createElement("script").text = "window.onload&&window.onload()";
        }
        else {
            startApp(data);
        }
        if (window["loadingView"] && window["loadingView"].loadingAutoClose) {
            window["loadingView"].hideLoadingView();
        }
    });
}
window.document.addEventListener('keydown', function (e) {
    switch (e.keyCode) {
        case 116:
            reloadJS(true);
            break;
        case 117:
            history.back();
            break;
        case 118:
            break;
        case 119:
            break;
        case 120:
            gc();
            break;
    }
});
window.loadConchUrl = loadApp;
window['updateByZip'] = function (url, onEvent, onEnd) {
    let cachePath = conch.getCachePath();
    let localfile = cachePath + url.substr(url.lastIndexOf('/'));
    downloadBigFile(url, localfile, (total, now, speed) => {
        onEvent('downloading', Math.floor((now / total) * 100), null);
        return false;
    }, (curlret, httpret) => {
        if (curlret != 0 || httpret < 200 || httpret >= 300) {
            onEvent('downloadError');
        }
        else {
            onEvent('downloadOK');
            let zip = new ZipFile();
            if (zip.setSrc(localfile)) {
                zip.forEach((id, name, dir, sz) => {
                    if (!dir) {
                        let buf = zip.readFile(id);
                        let fid = window.appcache.hashstr('/' + name);
                        if (window.appcache.updateFile(fid, 0, buf, false)) {
                            onEvent('updating', null, name);
                        }
                        else {
                            onEvent("updateError", null, name);
                        }
                    }
                });
                zip.close();
                if (onEnd)
                    onEnd(localfile);
            }
            else {
                console.log("set zip src error!");
                onEvent('unknownError');
            }
        }
    }, 10, 100000000);
};
loadApp(conch.presetUrl || "http://nativetest.layabox.com/layaplayer2.0.1/index.js");
