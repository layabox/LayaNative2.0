package layaair.autoupdateversion;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.ProtocolException;
import java.net.URL;

public class NetHelper {

    public static String httpStringGet(String url) throws Exception {
        return httpStringGet(url, "utf-8");
    }

    public static String httpStringGet(String url, String enc) throws Exception {
        HttpURLConnection connection = null;
        BufferedReader reader = null;
        try {
            URL u = new URL( url );
            connection = (HttpURLConnection) u.openConnection();
            connection.setRequestMethod( "GET" );
            connection.setConnectTimeout( 5000 );
            connection.setReadTimeout( 5000 );
            InputStream in = connection.getInputStream();
            reader = new BufferedReader( new InputStreamReader( in ) );
            StringBuilder result = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) {
                result.append( line );
            }
            return String.valueOf( result );
        } catch (MalformedURLException e) {
            e.printStackTrace();
        } catch (ProtocolException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (reader != null) {
                try {
                    reader.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if (connection != null) {
                connection.disconnect();
            }
        }
        return "";
    }
}
