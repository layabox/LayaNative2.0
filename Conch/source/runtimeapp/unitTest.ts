
var log = console.log;

export class Test {
    desc = '';
    constructor(desc: string) {
        this.desc = desc;
    }

    verifyEq(a, b, desc: string) {
        if (a instanceof Array && b instanceof Array) {
            if (a.length != b.length)
                throw desc + ' cant compare array with diffrent length! should:' + b.length + ' actual:' + a.length;
            for (var i = 0; i < a.length; i++) {
                if (a[i] != b[i])
                    throw desc + ' the ' + i + 'th data in array not equal';
            }
        }
        else {
            if (a == b) { } else {	//用==判断是为了防止NaN
                throw desc + ' should:' + b + ' actual:' + a;
            }
        }
    }
    eq = this.verifyEq;

    verifyNEq(a, b, desc: string) {
        if (a == b) {
            throw desc + ' shouldnot:' + b + ' actual:' + a;
        }
    }
    neq = this.verifyNEq;
    err(desc: string) {
        throw desc;
    }

    verifyEqRange(a, b, err: number, desc: string) {
        if (typeof (a) == 'number') {
            if (Math.abs(a - b) < err) { } else {//用==判断是为了防止NaN
                throw desc + ' should:' + b + ' actual:' + a;
            }
        } else {
            if (a.length != b.length)
                throw desc + ' wrong data length,should:' + b.length + ' actual:' + a.length;
            for (var i = 0; i < a.length; i++) {
                if (Math.abs(a[i] - b[i]) > err)
                    throw desc + ' the ' + i + 'th data not equal';
            }
        }
    }
    eqr = this.verifyEqRange;

    verifyThrow(func, desc) {
    }

    testall(all: Object, flag: string) {
        var errnum = 0;
        var oknum = 0;
        var sum = 0;
        var ps: string[] = [];
        var obj = all;
        /*
        if( typeof(all)==='function'){
            alert(1);
            ps = Object.getOwnPropertyNames((<Function>all).prototype);
            obj = new (<Function>all).prototype.constructor();
        }
        else if( typeof(all)==='object')
        */
        if (typeof (all) === 'object') {
            for (var f in all) {
                //ps =<Object>all;// Object.getOwnPropertyNames((<any>all).__proto__);
                //ps.forEach((f,i,a)=>{
                sum++;
                var bIgnore = false;
                if (flag) {
                    bIgnore = true;
                    if (obj[f][flag])
                        bIgnore = false;
                }

                try {
                    if (!bIgnore)
                        obj[f].call(obj, this);
                    oknum++;
                } catch (e) {
                    errnum++;
                    if (!e.name)	//自定义的
                        log("	ERROR: " + f + " : " + e);
                    else {
                        log(e);
                        //log(e.stack);
                    }
                }

            }
        }
        log(`
        ======================================
        ${this.desc}
        Test Result:
             passed: ${oknum}
              error: ${errnum}
        `);
    }
}

//testpac 是包含所有测试函数的对象
export function testall(testpack: Object, desc: string, flag?: string) {
    new Test(desc).testall(testpack, flag);
}

