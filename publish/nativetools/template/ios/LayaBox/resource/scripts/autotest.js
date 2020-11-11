"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const test = require("./unitTest");
function AllTest() {
    this.test_clone = function (test) {
        try {
            var b = document.createElement('audio');
            var c = b.cloneNode(false);
            c.addEventListener('', null);
        }
        catch (e) {
            test.err(`
            cloneNode之后，应该有 addEventListener 函数。
        `);
        }
    };
    this.testEventDispatch = function (test) {
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
    };
    this.testEvent_phase = function (test) {
        var canv = document.createElement('canvas');
        var result = [];
        document.body.appendChild(canv);
        document.addEventListener('mousedown', (evt) => {
            result.push(1);
        }, true);
        document.body.addEventListener('mousedown', (evt) => {
            result.push(2);
        }, true);
        canv.addEventListener('mousedown', (evt) => {
            result.push(3);
        });
        document.body.addEventListener('mousedown', (evt) => {
            result.push(4);
        });
        document.addEventListener('mousedown', (evt) => {
            result.push(5);
        });
        window.addEventListener('mousedown', (evt) => {
            result.push(6);
        });
        var evt = new MouseEvent('mousedown');
        evt.clientX = 100;
        evt.clientY = 100;
        canv.dispatchEvent(evt);
        document.removeChild(canv);
    };
    this.testRuntimeVersion = function (test) {
        var rv = conchConfig.getRuntimeVersion();
        var b = rv.indexOf('conch');
    };
    this.testABToC = function (test) {
    };
    this.testZip = function (test) {
        var zf = new ZipFile();
        zf.setSrc('');
        zf.forEach((id, name, dir, sz) => {
            var ab = zf.readFile(id);
        });
        zf.close();
    };
    this.testFile = function (test) {
        var abv = new Uint32Array([1, 2, 3]);
        fs_writeFileSync('d:/temp/ddd.d', abv.buffer);
        var ab = fs_readFileSync('d:/temp/ddd.d');
        var v1 = new Uint32Array(ab);
        if (v1.byteLength != 12 || v1[0] != 1 || v1[1] != 2 || v1[2] != 3)
            alert('error:' + arguments.callee.name);
        alert(fs_readdirSync('d:/temp'));
    };
    this.testWebSocket = function (test) {
        var ws = new WebSocket('');
    };
    this.testMD5 = function (test) {
        var result = '2a1dd1e1e59d0a384c26951e316cd7e6';
        test.eq(result, calcmd5(new Uint32Array([1, 2, 3]).buffer), 'md5计算不正确');
    };
    this.testPost = function (test) {
        var ab = new Uint32Array([0, 1, 2]);
        conch._postUrl('http://localhost:8888/testpost', 1, ab.buffer, null, (buf) => {
        }, (e) => {
        }, () => {
            return 0;
        });
    };
    this.testUrlEncode = function () {
    };
    this.testFileReader = function (test) {
        var f = new File('file:///d:/temp/test.sh');
        var fr = new FileReader();
        fr.onload = function () {
            alert('ok ' + fr.result);
        };
        fr.readAsText(f);
    };
}
test.testall(new AllTest(), "");
