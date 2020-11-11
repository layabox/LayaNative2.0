
/*
	假装自己是nodejs
*/

function _process(){
	this.pid=0;
	this.cwd=function(){
		return 'd:/temp';
	}
	this.mainModule = 'index.js';
	this.argv=['conch.exe','index.js'];
	this.version='1.3.1';
	this._require=function(f){console.log('process require('+f+')');return function nop(){};};	//不能真的load
	this._debugObject={};
}

window.process = new _process();