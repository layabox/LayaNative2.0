package layaair.game.utility;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.util.Log;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.ShortBuffer;

import javax.microedition.khronos.opengles.GL10;

/**
 * Created by hugao on 2016/11/5.
 */

public class Utils {
    public static byte[] screenShot(GL10 gl, int width, int height) {
        long begin=System.currentTimeMillis();
        int screenshotSize = width * height*4;
        ByteBuffer bb = ByteBuffer.allocateDirect(screenshotSize);
        bb.order(ByteOrder.nativeOrder());
        gl.glReadPixels(0, 0, width, height, GL10.GL_RGBA, GL10.GL_UNSIGNED_BYTE, bb);
        /*int pixelsBuffer[] = new int[screenshotSize];
        bb.asIntBuffer().get(pixelsBuffer);
        bb = null;
        Bitmap bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        bitmap.setPixels(pixelsBuffer, screenshotSize - width, -width, 0, 0, width, height);
        byte sBuffer[] = new byte[screenshotSize*4];
        ByteBuffer sb = ByteBuffer.wrap(sBuffer);
        bitmap.copyPixelsToBuffer(sb);
        int size=screenshotSize*4;
        byte temp;
        for(int i=0;i<size;i+=4)
        {
            temp=sBuffer[i];
            sBuffer[i]=sBuffer[i+2];
            sBuffer[i+2]=temp;
        }
        long useTime=System.currentTimeMillis()-begin;
        Log.e("temp",">>>>>>>>>>>>>>>>>>>>>>>useTimenew"+  useTime);
        return sb.array();*/
        byte [] a;
        try {
            a = bb.array();
        }
        catch (UnsupportedOperationException e)
        {
            a=new byte[screenshotSize];
            bb.get(a);
        }

        verticalMirror(a,width,height);
        long useTime=System.currentTimeMillis()-begin;
        Log.e("temp",">>>>>>>>>>>>>>>>>>>>>>>useTimenew"+  useTime);
        return a;
    }

    public static byte[] verticalMirror(byte[]a,int w,int h) {
        byte tR,tG,tB,tA;
        int  tempF,tempT;
        for (int i = 0; i < w; i++)//n列
        {
            for (int j = 0; j < h / 2; j++) //每一列转换（n/2）次
            {
                //将上下两个对称的元素进行交
                tempF=(w * j + i)*4;
                tempT=((h - j - 1) * w + i)*4;
                tR=a[tempF];
                tG=a[tempF+1];
                tB=a[tempF+2];
                tA=a[tempF+3];
                a[tempF]=a[tempT];
                a[tempF+1]=a[tempT+1];
                a[tempF+2]=a[tempT+2];
                a[tempF+3]=a[tempT+3];
                a[tempT]=tR;
                a[tempT+1]=tG;
                a[tempT+2]=tB;
                a[tempT+3]=tA;
            }
        }
        return  a;
    }
    public static int getResIdByName(Context context, String className, String resName) {
        String packageName = context.getPackageName();
        int id = 0;
        try {
            Class r = Class.forName(packageName + ".R");
            Class[] classes = r.getClasses();
            Class desireClass = null;
            for (Class cls : classes) {
                if (cls.getName().split("\\$")[1].equals(className)) {
                    desireClass = cls;
                    break;
                }
            }
            if (desireClass != null) {
                id = desireClass.getField(resName).getInt(desireClass);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return id;
    }
}
