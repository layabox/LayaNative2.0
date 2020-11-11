class loadingView
{
    constructor() 
	{
		this.sOS = conchConfig.getOS();
		if (this.sOS == "Conch-ios")
		{
			this.bridge = PlatformClass.createClass("JSBridge");
		}
		else if (this.sOS == "Conch-android")
		{
			this.bridge = PlatformClass.createClass("demo.JSBridge");
		}
    }
    set loadingAutoClose(value) 
	{
        this._loadingAutoClose = value;
    }
	get loadingAutoClose()
	{
		return this._loadingAutoClose;
	}
	set showTextInfo(value) 
	{
        this._showTextInfo = value;
		if(this.bridge)
		{
			if (this.sOS == "Conch-ios")
			{
				this.bridge.call("showTextInfo:",value);
			}
			else if(this.sOS == "Conch-android")
			{
				this.bridge.call("showTextInfo",value);
			}
		}
    }
	get showTextInfo()
	{
		return this._showTextInfo;
	}
	bgColor(value)
	{
		if(this.bridge)
		{
			if (this.sOS == "Conch-ios")
			{
				this.bridge.call("bgColor:",value);
			}
			else if(this.sOS == "Conch-android")
			{
				this.bridge.call("bgColor",value);
			}
		}
	}
	setFontColor(value)
	{
		if(this.bridge)
		{
			if (this.sOS == "Conch-ios")
			{
				this.bridge.call("setFontColor:",value);
			}
			else if(this.sOS == "Conch-android")
			{
				this.bridge.call("setFontColor",value);
			}
		}
	}
	setTips(value)
	{
		if(this.bridge)
		{
			if (this.sOS == "Conch-ios")
			{
				this.bridge.call("setTips:",value);
			}
			else if(this.sOS == "Conch-android")
			{
				this.bridge.call("setTips",value);
			}
		}
	}
	loading(value)
	{
		if(this.bridge)
		{
			if (this.sOS == "Conch-ios")
			{
				this.bridge.call("loading:",value);
			}
			else if(this.sOS == "Conch-android")
			{
				this.bridge.call("loading",value);
			}
		}
	}
	hideLoadingView()
	{
		this.bridge.call("hideSplash");
	}
}
window.loadingView = new loadingView();
if(window.loadingView)
{
    window.loadingView.loadingAutoClose=true;//true代表当动画播放完毕，自动进入游戏。false为开发者手动控制
    window.loadingView.bgColor("#000000");//设置背景颜色
    window.loadingView.setFontColor("#ffffff");//设置字体颜色
    window.loadingView.setTips(["新世界的大门即将打开","敌军还有30秒抵达战场","妈妈说，心急吃不了热豆腐"]);//设置tips数组，会随机出现
}
window.onLayaInitError=function(e)
{
	console.log("onLayaInitError error=" + e);
	alert("加载游戏失败，可能由于您的网络不稳定，请退出重进");
}