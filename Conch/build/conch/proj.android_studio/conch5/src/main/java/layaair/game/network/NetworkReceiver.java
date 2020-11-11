package layaair.game.network;

import layaair.game.browser.ConchJNI;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.telephony.TelephonyManager;
import android.util.Log;

public class NetworkReceiver extends BroadcastReceiver 
{
	/**
	 * 枚举网络状态
	 * NET_NO:没有网络
	 * NET_2G:2g网络
	 * NET_3G:3g网络
	 * NET_4G:4g网络
	 * NET_WIFI:wifi
	 * NET_UNKNOWN:未知网络
	 */
	public final static int  NET_NO = 0;
	public final static int  NET_WIFI = 1;
	public final static int  NET_2G = 2;
	public final static int  NET_3G = 3;
	public final static int  NET_4G = 4;
	public final static int  NET_UNKNOWN = 5;

	@Override
	public void onReceive( Context context, Intent intent )
	{
		String sAction = intent.getAction();
		if ( sAction.equals(ConnectivityManager.CONNECTIVITY_ACTION ) )
        {
        	Log.i("0",">>>>>>>>>>>>>The network has changed");
        	handleConnected( getConnectedType( context ) );
        }
	}
	public int getConnectedType( Context context ) 
	{
	    int nStateCode = NET_NO;
	    ConnectivityManager pConnectManager = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
	    NetworkInfo pNetwordInfo = pConnectManager.getActiveNetworkInfo();
	    if ( pNetwordInfo != null && pNetwordInfo.isConnected())
	    {
	        switch ( pNetwordInfo.getType() ) 
	        {
	            case ConnectivityManager.TYPE_WIFI:
	            	nStateCode = NET_WIFI;
	                break;
	            case ConnectivityManager.TYPE_MOBILE:
	                switch (pNetwordInfo.getSubtype()) 
	                {
	                    case TelephonyManager.NETWORK_TYPE_GPRS: //联通2g
	                    case TelephonyManager.NETWORK_TYPE_CDMA: //电信2g
	                    case TelephonyManager.NETWORK_TYPE_EDGE: //移动2g
	                    case TelephonyManager.NETWORK_TYPE_1xRTT:
	                    case TelephonyManager. NETWORK_TYPE_IDEN:
	                    	nStateCode = NET_2G;
	                        break;
	                    case TelephonyManager.NETWORK_TYPE_EVDO_A: //电信3g
	                    case TelephonyManager.NETWORK_TYPE_UMTS:
	                    case TelephonyManager.NETWORK_TYPE_EVDO_0:
	                    case TelephonyManager.NETWORK_TYPE_HSDPA:
	                    case TelephonyManager.NETWORK_TYPE_HSUPA:
	                    case TelephonyManager.NETWORK_TYPE_HSPA:
	                    case TelephonyManager.NETWORK_TYPE_EVDO_B:
	                    case TelephonyManager.NETWORK_TYPE_EHRPD:
	                    case TelephonyManager.NETWORK_TYPE_HSPAP:
	                    	nStateCode = NET_3G;
	                        break;
	                    case TelephonyManager.NETWORK_TYPE_LTE:
	                    	nStateCode = NET_4G;
	                        break;
	                    default:
	                    	nStateCode = NET_UNKNOWN;
	                }
	                break;
	            default:
	            	nStateCode = NET_UNKNOWN;
	        }
	    }
	    return nStateCode;
	}
	public void handleConnected( int nState )
	{
		Log.i("0", "connected type=" + nState );
		ConchJNI.networkChanged(nState);
	}
}
