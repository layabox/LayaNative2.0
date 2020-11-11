package layaair.game.browser;

import java.security.PublicKey;
import android.util.Log;

import android.webkit.WebChromeClient;
import android.webkit.WebView;
import android.webkit.JsResult;

public class LayaWebChromeClient extends WebChromeClient
{
    private LayaWebView m_pLayaWebView=null;
    public LayaWebChromeClient(LayaWebView webView )
    {
        m_pLayaWebView = webView;
    }
    public boolean onJsAlert(WebView view, String url, String message, JsResult result) {
        Log.e( "LayaWebView","onJsAlert message=" + message );
        return  false;
    }
}