package layaair.game.browser;

import android.content.Intent;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Build;
import android.util.Log;
import android.webkit.WebResourceResponse;
import android.webkit.WebView;
import android.webkit.WebViewClient;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;

import layaair.game.conch.LayaConch5;
import layaair.game.config.config;

public class LayaWebViewClient extends WebViewClient
{
    public LayaWebView m_pLayaWebView = null;
    //------------------------------------------------------------------------------
    public LayaWebViewClient()
    {
    }
    public WebResourceResponse shouldInterceptRequest(WebView view, String sUrl)
    {
        Log.i("LayaWebView", "shouldInterceptRequest url=" + sUrl);

       if(config.GetInstance().m_sConchGameUrl != null )
        {
            if (sUrl.indexOf(config.GetInstance().m_sConchGameUrl) > -1)
            {
                try
                {
                    m_pLayaWebView.callConchJSFunction("document.setReferrer", config.GetInstance().m_sWebviewUrl, "");
                    m_pLayaWebView.callConchJSFunction("window.loadConchUrl", sUrl, "");
                    ExportJavaFunction.hideExternalLink();
                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
                        return  new WebResourceResponse("html","UTF-8",null);
                    }
                }
                catch ( Exception exception )
                {
                    Log.i( "0","exception" + exception.toString() );
                }
            }
        }
        return  super.shouldInterceptRequest(view, sUrl);
    }

    //------------------------------------------------------------------------------
    public boolean shouldOverrideUrlLoading( WebView view, String sUrl )
    {
        Log.i("LayaWebView", "shouldOverrideUrlLoading url=" + sUrl);
        if(config.GetInstance().m_sConchGameUrl != null )
        {
            if (sUrl.indexOf(config.GetInstance().m_sConchGameUrl) > -1)
            {
                m_pLayaWebView.callConchJSFunction("window.loadConchUrl", sUrl, "");
                ExportJavaFunction.closeExternalLink();
            }
            else
            {
                view.loadUrl(sUrl);
            }
        }
        /*if (sUrl.startsWith("weixin://wap/pay?")||sUrl.startsWith("mqqapi://forward/url?")) {
            Intent intent = new Intent();
            intent.setAction(Intent.ACTION_VIEW);
            intent.setData(Uri.parse(sUrl));
            m_pLayaWebView.m_pEngine.getGameContext().startActivity(intent);
            view.stopLoading();
        }
        else*/
        {
            view.loadUrl(sUrl);
        }
        return true;
    }
    //------------------------------------------------------------------------------
    public void onPageStarted(WebView view, String url, Bitmap favicon) {
        Log.d("LayaWebView","onPageStarted url="+url);
        super.onPageStarted(view, url, favicon);
    }
    //------------------------------------------------------------------------------
    public void onLoadResource(WebView view, String url)
    {
        super.onLoadResource(view, url);
    }
    //------------------------------------------------------------------------------
    public void onPageFinished(android.webkit.WebView view, java.lang.String url)
    {
        Log.i("LayaWebView", "onPageFinished");
        String sBuffer = "javascript:";
        AssetManager am= LayaConch5.GetInstance().m_AM;
        try {
            InputStream in= am.open("scripts/webviewInit.js");
            int ch;
            ByteArrayOutputStream out = new ByteArrayOutputStream(); //实现了一个输出流
            while((ch=in.read())!=-1) {
                out.write(ch); //将指定的字节写入此 byte 数组输出流
            }
            byte[] buff = out.toByteArray();//以 byte 数组的形式返回此输出流的当前内容
            out.close(); //关闭流
            in.close(); //关闭流
            String content = new String(buff, "UTF-8"); //设置字符串编码
            sBuffer+=content;

        } catch (IOException e) {
            e.printStackTrace();
        }
        Log.i("LayaWebView",sBuffer);
        m_pLayaWebView.getWebView().loadUrl(sBuffer );
        super.onPageFinished(view, url);
    }
    //------------------------------------------------------------------------------
}
