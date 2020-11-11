/*
var canv = document.createElement('canvas');
var ctx = canv.getContext('2d');
ctx.save();
ctx.fillStyle = '#ff0000';
var img = new Image();
img.src = 'http://10.10.20.19:7777/a.jpg';
var bload = false;
img.onload = function(e) {
    bload = true;
}
img.addEventListener('load', function() {
})
setInterval(() => {
    if (bload) {
        console.log('kkk');
        ctx.drawImage(img._nativeObj, 0, 0, 100, 100, 0, 0, 100, 100);
    }

}, 100);
*/



//require('websocket.js');

// downloadGetHeader('http://s0.tx.zxyh5.com/api/zxy/register.php',(buf)=>{
//     alert('ok ' +buf);
// },(e,httprec)=>{
//     alert('error');
// },100,100);

// class MyWebSocket{
//     constructor(url:string){
//         ConchWebSocket.call(this,url);
//     }
// }

//var w1 = new MyWebSocket('aaa');
//alert(JSON.stringify(w1));
/*
var ws = new WebSocket('ws://10.10.20.241:7060');

ws.onopen=function(ev){
    alert('kkk')
}

function md(e){
    console.log('--------------');
}
document.body.addEventListener('mousedown',md);
document.body.addEventListener('mousedown',md);
*/

import test = require('./unitTest');
type Test = test.Test;

class AllTest {
    /**
     * 1. cloneNode 能正确clone
     */
    test_clone(test: Test) {
        try {
            var b = document.createElement('audio');
            var c = b.cloneNode(false);
            c.addEventListener('', null);
        } catch (e) {
            alert('111')
            test.err(`
            cloneNode之后，应该有 addEventListener 函数。
        `);
        }
    }

    /**
     * 测试event事件的派发
     */
    testEventDispatch(test: Test) {
        var called = false;
        var canv = document.createElement('canvas');
        document.body.appendChild(canv);
        canv.addEventListener('mmousemove', (evt) => {
            called = true;
        });
        var evt = new MouseEvent('mmousemove');
        evt.clientX = 100;
        evt.clientY = 100;
        canv.dispatchEvent(evt);
        test.eq(called, true, '事件应该能正确发送给canvas');
        document.removeChild(canv);
        test.neq(window.document._topElement, canv, 'removeChild之后，应该维护_topElement');
    }

    testEvent_phase(test: Test) {
        var canv = document.createElement('canvas');
        var result: number[] = [];
        document.body.appendChild(canv);
        document.addEventListener('mousedown', (evt) => {
            result.push(1);
            //alert('document capture event');
        }, true);
        document.body.addEventListener('mousedown', (evt) => {
            //alert('body capture event');
            result.push(2);
        }, true);
        canv.addEventListener('mousedown', (evt) => {
            //alert('on mousedown');
            result.push(3);
        });
        document.body.addEventListener('mousedown', (evt) => {
            //alert('event bubble to body');
            result.push(4);
        });
        document.addEventListener('mousedown', (evt) => {
            //alert('event bubble to document');
            result.push(5);
        });
        window.addEventListener('mousedown', (evt) => {
            //alert('event bubble to window');
            result.push(6);
        });
        var evt = new MouseEvent('mousedown');
        evt.clientX = 100;
        evt.clientY = 100;
        canv.dispatchEvent(evt);
        document.removeChild(canv);
    }

    testRuntimeVersion(test: Test) {
        var rv = conchConfig.getRuntimeVersion();
        var b = rv.indexOf('conch');
    }

    //测试ArrayBuffer的传递和管理
    testABToC(test: Test) {
        /*
        while (true) {
            var ab = new ArrayBuffer(1000);
            var cp = conch.byteCompress(ab);
            var ucp = conch.byteUnCompress(cp);
        }
        */
    }

    //所有接口
    testZip(test: Test) {
        var zf = new ZipFile();
        zf.setSrc('');
        zf.forEach((id, name, dir, sz) => {
            var ab = zf.readFile(id);
        });
        zf.close();
    }

    testFile(test: Test) {
        var abv = new Uint32Array([1, 2, 3]);
        fs_writeFileSync('d:/temp/ddd.d', abv.buffer);
        var ab = fs_readFileSync('d:/temp/ddd.d');
        var v1 = new Uint32Array(ab);
        if (v1.byteLength != 12 || v1[0] != 1 || v1[1] != 2 || v1[2] != 3)
            alert('error:' + arguments.callee.name);
        alert(fs_readdirSync('d:/temp'));
        //var ab = readBinFileSync('');
    }


    testWebSocket(test: Test) {
        var ws = new WebSocket('');
    }

    testMD5(test: Test) {
        var result = '2a1dd1e1e59d0a384c26951e316cd7e6';
        test.eq(result, calcmd5(new Uint32Array([1, 2, 3]).buffer), 'md5计算不正确');
    }

    testPost(test: Test) {
        var ab = new Uint32Array([0, 1, 2]);
        conch._postUrl('http://localhost:8888/testpost', 1, ab.buffer, null, (buf) => {

        }, (e) => {

        }, () => {
            return 0;
        })
    }

    testImgErr(test: Test) {
        var img = new Image();
        img.src = '';
        img.onerror = (e) => {
            alert('onerror');
        }
        img.onload = (e) => {
            alert('onload');
        }
    }
    
    testUrlEncode(){
        //http://115.159.92.56:8107/update/0?4089:21922|%5B%7B%22m%22%3A%22C_GSVR%22%2C%22c%22%3A0%2C%22p%22%3A%5Bnull%2C%22QQ%22%2Cnull%2C%221%2E0.2%22%5D%2C%22st%22%3A1460718545%7D%5D
        //结果不能变
    }
    
    testCanvasInterface(){
        var c = document.createElement('canvas');
        c.width=100;//必须有这个属性，否则夺塔不行
    }
    
    testFontInfo(test:Test){
        /*
        var cn = new ConchNode();
        cn.font('normal 100 16px Arial b 1  #ff0000 1 #00ff00 ');
        test.eq(1,1,'');
        cn.font(' ')
        test.eq(1,1,'');
        cn.font('')
        test.eq(1,1,'');
        cn.font('normal 100 28px Microsoft_YaHei 1 #444444 0');
        
        cn.font('normal normal 18px 宋体 2 #000000 0 #000000');
        test.eq(1,1,'');
        */
    }
    
    testFileReader(test:Test){
        var f = new File('file:///d:/temp/test.sh');
        var fr = new FileReader();
        fr.onload=function(){
            alert('ok '+fr.result);
        }
        fr.readAsText(f);
    }
    
    testFileReaderRemote(test:Test){
        var f = new File('http://www.layabox.com/statics/images/laya/slider.jpg');
        var fr = new FileReader();
        fr.onload=function(){
            alert('ok');
        }
        fr.readAsArrayBuffer(f);
    }
    
    testImgDecode(test:Test){
        var img = new Image();
        img.src='http://www.layabox.com/statics/images/laya/slider.jpg';
        img.onload=function(){
            alert('ok');
        }
    }
    testWebGL1(test:Test){
        var canv = document.createElement('canvas');
        var gl = canv.getContext('webgl');
        var cc=0;
        function render(){
            cc+=0.001;
            gl.clearColor(0,cc,1,1);    
            gl.clear(0x4100);
            gl.flush();
        }
        setInterval(render,10);
    }
}

var xhr = new XMLHttpRequest();
xhr.open('GET','http://dtsg.games.layabox.com/assets/bootstrap/preloading.xml',true);
xhr.onload=function(e){
    alert(e);
}
xhr.onerror=function(e){
    alert('eee:'+e);
}

//test.testall(AllTest, "");

var canv = new HTMLCanvasElement();
document.body.appendChild(canv);
var gl = canv.getContext('webgl');

function r() {
    console.log('kkkkk')
    gl.clear(0);
    gl.flush();
}

setInterval(r,10);


// // /ws.close(-1,'');
// var f = new conch_File('http://raganwald.com/2014/07/09/javascript-constructor-problem.html');
// var fr = new conch_FileReader();
// fr.onload=()=>{
//     alert(fr.result);
// }
// fr.readAsText(f);


