package layaair.game.browser;

import android.app.Activity;
import android.content.Context;
import android.graphics.Color;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.View;

import com.google.zxing.integration.android.IntentIntegrator;

import org.json.JSONArray;
import org.json.JSONException;

import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;

import layaair.game.config.config;

import static android.content.Context.CONNECTIVITY_SERVICE;


public class JSBridge {
    public static Handler m_Handler = new Handler(Looper.getMainLooper());
    public static Activity mMainActivity = null;

    public static void hideSplash() {
        m_Handler.post(
                new Runnable() {
                    public void run() {
                        MainActivity.mSplashDialog.dismissSplash();
                    }
                });
    }

    public static void setFontColor(final String color) {
        m_Handler.post(
                new Runnable() {
                    public void run() {
                        MainActivity.mSplashDialog.setFontColor(Color.parseColor(color));
                    }
                });
    }

    public static void setTips(final JSONArray tips) {
        m_Handler.post(
                new Runnable() {
                    public void run() {
                        try {
                            String[] tipsArray = new String[tips.length()];
                            for (int i = 0; i < tips.length(); i++) {
                                tipsArray[i] = tips.getString(i);
                            }
                            MainActivity.mSplashDialog.setTips(tipsArray);
                        } catch (JSONException e) {
                            e.printStackTrace();
                        }
                    }
                });
    }

    public static void bgColor(final String color) {
        m_Handler.post(
                new Runnable() {
                    public void run() {
                        MainActivity.mSplashDialog.setBackgroundColor(Color.parseColor(color));
                    }
                });
    }

    public static void loading(final double percent) {
        m_Handler.post(
                new Runnable() {
                    public void run() {
                        MainActivity.mSplashDialog.setPercent((int)percent);
                    }
                });
    }

    public static void showTextInfo(final boolean show) {
        m_Handler.post(
                new Runnable() {
                    public void run() {
                        MainActivity.mSplashDialog.showTextInfo(show);
                    }
                });
    }

    public static void showFloatPanel(final boolean show) {
        m_Handler.post(
                new Runnable() {
                    public void run() {
                        if (MainActivity.m_FloatPanel != null) {
                            if (show) {
                                MainActivity.m_FloatPanel.show();
                            } else {
                                MainActivity.m_FloatPanel.hide();
                            }
                        }
                    }
                });
    }

    public static void showScanner(final boolean show) {
        m_Handler.post(
                new Runnable() {
                    public void run() {
                        if (MainActivity.m_FloatPanel != null) {
                            if (show) {
                                new IntentIntegrator(mMainActivity).setCaptureActivity(ScanActivity.class).initiateScan();
                            } else {
                                if (ScanActivity.m_instance != null) {
                                    ScanActivity.m_instance.finish();
                                }
                            }
                        }
                    }
                });
    }

    public static void getIP() {
        m_Handler.post(
                new Runnable() {
                    public void run() {
                        ExportJavaFunction.CallBackToJS(JSBridge.class,"getIP", getIP(mMainActivity));
                    }
                });
    }

    public static void onOrientationChange() {
        MainActivity.m_FloatPanel.updatePosition();
    }

    public static void onScanResult(String result) {
        Log.d("JSBridge", "url " + result);
        if (ScanActivity.m_instance != null) {
            ScanActivity.m_instance.finish();
        }
        String js = "script.UIController.instance.onScanResult('";
        js += result;
        js += "');";
        ConchJNI.RunJS(js);
    }
    public static String getIP(Context context){
        try {
            for (Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces(); en.hasMoreElements();) {
                NetworkInterface intf = en.nextElement();
                for (Enumeration<InetAddress> enumIpAddr = intf.getInetAddresses(); enumIpAddr.hasMoreElements();) {
                    InetAddress inetAddress = enumIpAddr.nextElement();
                    if (!inetAddress.isLoopbackAddress() && (inetAddress instanceof Inet4Address)) {
                        NetworkInfo activeNetInfo = ((ConnectivityManager)context.getSystemService(CONNECTIVITY_SERVICE)).getActiveNetworkInfo();
                        String str = "";
                        if(activeNetInfo.getType() == ConnectivityManager.TYPE_WIFI) {
                            str += "WifiNetworkIP: ";
                        }
                        else if(activeNetInfo.getType() == ConnectivityManager.TYPE_MOBILE) {
                            str += "MobileNetworkIP: ";
                        }
                        else {
                            str += "UnknowNetworkIP: ";
                        }
                        str += inetAddress.getHostAddress().toString();
                        return str;
                    }
                }
            }
        }
        catch (SocketException ex) {
            ex.printStackTrace();
        }
        return "";
    }
}
