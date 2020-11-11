
(function () {
    'use strict';
    var gl = LayaGLContext;
    window["extendWebGLPlusToWebGLContext"](gl);
    class AppInfo {
        name: string;
        version: string;
        devUpdateUrl: string;
        updateUrl: string;
        updateDelay: number;//更新延迟
        DomJS: string;
        mainjs: string;
    };
    var appobj:AppInfo = null;
    try {
        appobj = <AppInfo>JSON.parse(<string>conch.readFileFromAsset('app.json', 'utf8'));
        if(appobj){
            require(appobj.mainjs);
        }
    } catch (e) {
        require('index');
    }
})();