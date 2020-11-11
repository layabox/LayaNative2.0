window = this;
window.conch_File=File;
window.conch_FileReader = FileReader;

(function () {
    function file2path(p) {
        if (!p) return null;
        var lastpos = Math.max(p.lastIndexOf('/'),
            p.lastIndexOf('\\'));
        var ret = lastpos < 0 ? p : p.substr(0, lastpos);
        return ret.replace(/\\/g, '/');
    }
    var mcache = {};
    /*起始路径总是这里，如果需要改变的话，就在这里通过require跳转。*/
    window.requireOrig = function (file) {
        function evalreq(fc, fid) {/*return function*/
            if (!fc || fc.length <= 0) return null;
            /*注意：并不是window.eval所以脚本中不能假设当前是在window上下文*/
            try {
                //注意 fc后面要加\n来关掉行注释
                var func = eval('(function(exports,global,require,__dirname,__filename){' + fc + ';\nreturn exports;})\n//@ sourceURL=' + fid);
                mcache[fid] = func;
                return func;
            }
            catch (e) {
                _console.log(1,'require error:' + e);
                return null;
            }
        }
        var mod = { dir: this.dir, file: file };
        if (file.substr(file.length - 3) != '.js')
            file += '.js';
        /*优先读取外部的*/
        var extfile = null;
        if (file.charAt(1) === ':' || file.charAt(0) === '/') { extfile = file; }
        else extfile = this.dir ? (this.dir + '/' + file) : null;
        var extfunc = null;
        _console.log(3,'require(' + extfile + ')');
        var reqresult = mcache[extfile] ||
            (extfunc = evalreq(readFileSync(extfile, 'utf8'), extfile)) ||
            mcache[file] ||
            evalreq(readTextAsset('scripts/' + file), file);
        if (extfunc) {
            mod.dir = file2path(extfile);/*使用window的或者当前模块的*/
            mod.file = extfile;
        }
        if (!reqresult) {
            throw ('require failed：' + file);
            return null;
        }
        try {
            var ret = reqresult({}, window, window.requireOrig.bind(mod), mod.dir, mod.file);
            return ret;
        }
        catch (e) {
            var err = 'eval script error in require:\n ' + file + '\n' + e.stack;
            _console.log(1,err);
            throw err;
        }
        return null;
    }
    var exepath = file2path(getExePath());
    window.require = window.requireOrig.bind({ dir: exepath ? (exepath + '/scripts') : '/sdcard/layabox/scripts', file: null })
})();