function async(gen) {
    "use strict";
    var log = console.log;
    var g = gen();
    let nr = null;
    function cast(value) {
        return value instanceof Promise && value.constructor === Promise ? value : new Promise(function () { });
    }
    function thennext(v) {
        nr = g.next(v);
        var p = nr.value;
        nr.done ? (0) : cast(p).then(thennext, function (e) { console.log('async reject:' + e); thennext(null); }).catch(function (r) { 
            alert(`${r}
                ${r.stack}`); 
        });
    }
    thennext(void 0);
}
exports.async = async;
//延时
function delay(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}
exports.delay = delay;
//下载图片
function loadImage(src) {
    return new Promise((resolve, reject) => {
        var img = new Image();
        img.src = src;
        img.onload = () => { resolve(img); };
        img.onerror = () => {
            //reject('file load err:' + src);
            resolve(null);
        };
    });
}
exports.loadImage = loadImage;
//下载文本文件
function loadText(src) {
    return new Promise((resolve, reject) => {
        var xhr = new XMLHttpRequest();
        xhr.open('GET', src);
        xhr.onload = () => { resolve(xhr.responseText); };
        xhr.onerror = (e) => { 
            //if (reject)
            //    reject('download xhr error:' + src + '. e=' + e); 
            resolve(null);
        };
        xhr.send();
    });
}
exports.loadText = loadText;
function downloadSync(url, bin, onprog) {
    return new Promise(function (resolve, reject) {
        var f = new conch_File(url);
        var fr = new conch_FileReader();
        fr.setIgnoreError(true);
        fr.onload = () => {
            console.log('download end');
            resolve(fr.result);
        };
        fr.onerror = (e) => { console.log('onerror ' + e); 
            //if (reject)
            //    reject(e); 
            resolve(null);
        };
        fr.onprogress = onprog;
        if (bin)
            fr.readAsArrayBuffer(f);
        else
            fr.readAsText(f);
    });
}
exports.downloadSync = downloadSync;
//直到func返回true,tm是间隔
function check(func, tm) {
    return new Promise(function (resolve, reject) {
        function checkFunc() {
            if (func()) {
                clearInterval(chker);
                resolve(1);
            }
        }
        var chker = setInterval(checkFunc, tm);
    });
}
function regGlobal(window) {
}
exports.regGlobal = regGlobal;
window['async'] = async;
window['delay'] = delay;
window['downloadSync'] = downloadSync;
window['asynccheck'] = check;
