package demo;

import java.io.File;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.webkit.ValueCallback;
import layaair.game.IMarket.IPluginRuntimeProxy;

public class RuntimeProxy implements IPluginRuntimeProxy {

    private String TAG = "RuntimeProxy";
    private Activity mActivity = null;
    public RuntimeProxy(Activity mainActivity) {
        mActivity = mainActivity;
    }

    @Override
    public boolean laya_set_value(String key, Object value) {
        // TODO Auto-generated method stub
        return false;
    }
    public String getCacheDir()
    {
        String sCache = mActivity.getCacheDir().toString();
        String []vString = sCache.split("/");
        String sNewCache="";
        for( int i = 0; i < vString.length-1; i++ )
        {
            sNewCache += vString[i];
            sNewCache += "/";
        }
        return sNewCache;
    }
    public String getExpansionMainPath()
    {
        //TODO CP自行定制
        return "";
    }
    public String getExpansionPatchPath()
    {
        //TODO CP自行定制
        return "";
    }
    @Override
    public Object laya_get_value(String key) {
        Log.d(TAG, "laya_get_value key=" + key);
        String str = null;
        if (key.equalsIgnoreCase("CacheDir")) {
            return getCacheDir();
        }
        else if(key.equalsIgnoreCase("ExpansionMainPath")) {
            return getExpansionMainPath();
        }
        else if(key.equalsIgnoreCase("ExpansionPatchPath")) {
            return getExpansionPatchPath();
        }
        return str;
    }

    @Override
    public void laya_stop_game_engine() {
        // TODO Auto-generated method stub
        Log.d(TAG, "Login laya_stop_game_engine.");
    }

    @Override
    public Object laya_invoke_Method(String method, Bundle param) {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public void Login(JSONObject jsonObj, ValueCallback<JSONObject> callback) {
        // TODO Auto-generated method stub
        Log.d(TAG, "Login info = "+jsonObj.toString());
        try {
            JSONObject result = new JSONObject();
            result.put("status", 0);
            result.put("unionUserID", "1123fff4");
            result.put("nickName", "中南海");
            result.put("photo", "http://www.xxx.com/xxx.jpg");
            result.put("sptoken", "33ffffh54444dddd");
            result.put("msg", "success");
            callback.onReceiveValue(result);
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void Logout(JSONObject jsonObj, ValueCallback<JSONObject> callback) {
        // TODO Auto-generated method stub
        Log.d(TAG, "Logout info = "+jsonObj.toString());
        JSONObject result = new JSONObject();
        try {
            result.put("status", 0);
            result.put("msg", "success");
        } catch (JSONException e) {
            e.printStackTrace();
        }
        callback.onReceiveValue(result);
    }

    @Override
    public void Pay(JSONObject jsonObj, ValueCallback<JSONObject> callback) {
        // TODO Auto-generated method stub
        Log.d(TAG, "Logout Pay = "+jsonObj.toString());
        JSONObject result = new JSONObject();
        try {
            result.put("status", 0);
            result.put("msg", "success");
        } catch (JSONException e) {
            e.printStackTrace();
        }
        callback.onReceiveValue(result);
    }

    @Override
    public void PushIcon(JSONObject jsonObj, ValueCallback<JSONObject> callback) {
        // TODO Auto-generated method stub
        Log.d(TAG, "Logout PushIcon = "+jsonObj.toString());
        JSONObject result = new JSONObject();
        try {
            result.put("status", 0);
            result.put("msg", "success");
        } catch (JSONException e) {
            e.printStackTrace();
        }
        callback.onReceiveValue(result);
    }

    @Override
    public void Share(JSONObject jsonObj, ValueCallback<JSONObject> callback) {
        // TODO Auto-generated method stub
        Log.d(TAG, "Logout Share = "+jsonObj.toString());
        JSONObject result = new JSONObject();
        try {
            result.put("status", 0);
            result.put("msg", "success");
        } catch (JSONException e) {
            e.printStackTrace();
        }
        callback.onReceiveValue(result);
    }

    @Override
    public void OpenBBS(JSONObject jsonObj, ValueCallback<JSONObject> callback) {
        // TODO Auto-generated method stub
        Log.d(TAG, "Logout OpenBBS = "+jsonObj.toString());
        JSONObject result = new JSONObject();
        try {
            result.put("status", 0);
            result.put("msg", "success");
        } catch (JSONException e) {
            e.printStackTrace();
        }
        callback.onReceiveValue(result);
    }

    @Override
    public void GetFriendsList(JSONObject jsonObj,
            ValueCallback<JSONObject> callback) {
        // TODO Auto-generated method stub
        Log.d(TAG, "Logout GetFriendsList = "+jsonObj.toString());
        JSONObject result = new JSONObject();
        JSONArray array = new JSONArray();
        try {
            
            JSONObject p1 = new JSONObject();
            p1.put("userId", "1111111");
            p1.put("nickName", "xiaoming");
            p1.put("photo", "http://xxx.com/xxx.jpg");
            p1.put("sex", "0");
            
            JSONObject p2 = new JSONObject();
            p2.put("userId", "1111111");
            p2.put("nickName", "xiaoming");
            p2.put("photo", "http://xxx.com/xxx.jpg");
            p2.put("sex", "0");
            array.put(p1);
            array.put(p2);
            
            result.put("status", 0);
            result.put("msg", "success");
            result.put("friends", array);
        } catch (JSONException e) {
            e.printStackTrace();
        }
        callback.onReceiveValue(result);
    }

    @Override
    public void SendMessageToPlatform(JSONObject jsonObj,
            ValueCallback<JSONObject> callback) {
        // TODO Auto-generated method stub
        Log.d(TAG, "Logout SendMessageToPlatform = "+jsonObj.toString());
        JSONObject result = new JSONObject();
        try {
            result.put("status", 0);
            result.put("msg", "success");
        } catch (JSONException e) {
            e.printStackTrace();
        }
        callback.onReceiveValue(result);
    }

}
