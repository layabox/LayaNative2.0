package layaair.game.browser;

import android.app.Activity;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;
import android.view.Gravity;
import android.view.ViewGroup;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.view.animation.Animation.AnimationListener;
import android.webkit.ValueCallback;
import android.widget.ImageView;
import android.widget.LinearLayout;

import java.io.IOException;
import java.io.InputStream;

import layaair.game.config.config;

public class FlashScreen 
{
    //logo闪屏相关的
    private LinearLayout 	m_pLayoutHead = null;
    private int  			m_curFlashIndex = 1;
    private Activity	m_pActivity = null;
    //------------------------------------------------------------------------------
	public FlashScreen( Activity p_pActivity )
	{
		m_pActivity = p_pActivity;
		m_pLayoutHead = new LinearLayout( m_pActivity );
	}
	//------------------------------------------------------------------------------
	public void start(final ValueCallback<Boolean> callback)
	{
		Log.e("0", "==============Java流程 FlashScreen start");
		String sImageName = "flashscreen"+m_curFlashIndex+".jpg";
		AssetManager pAssetManager = m_pActivity.getAssets();
		InputStream pInputStream = null;
		try 
		{
			pInputStream=pAssetManager.open( sImageName );//name:图片的名称
		} 
		catch (IOException e) 
		{
			e.printStackTrace();
		}
		if( pInputStream == null )
		{
			Log.e("0", "==============Java流程 FlashScreen end 调用checkApkUpdate");
			//LayaWrapper.GetInstance().checkApkUpdate();
			callback.onReceiveValue(true);
			return;
		}
		m_pLayoutHead.removeAllViews();
		ImageView pLogoImage = new ImageView( m_pActivity );
		LinearLayout.LayoutParams param=new LinearLayout.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,ViewGroup.LayoutParams.FILL_PARENT);
		m_pLayoutHead.setGravity(Gravity.CENTER);
		Bitmap pBitMap = BitmapFactory.decodeStream( pInputStream );
		LinearLayout.LayoutParams pLayoutParams =new LinearLayout.LayoutParams(android.view.ViewGroup.LayoutParams.WRAP_CONTENT,android.view.ViewGroup.LayoutParams.WRAP_CONTENT);
		pLogoImage.setImageBitmap( pBitMap );
		m_pLayoutHead.setBackgroundColor( config.GetInstance().m_nMarketWaitScreenBKColor );
		m_pLayoutHead.addView( pLogoImage,pLayoutParams );
		m_pActivity.setContentView(m_pLayoutHead,param);
		AlphaAnimation alphaAnimation = new AlphaAnimation(0.1f, 1.0f);  
        alphaAnimation.setDuration(2000);
        pLogoImage.startAnimation( alphaAnimation );
        alphaAnimation.setAnimationListener(new AnimationListener() 
        {  
            public void onAnimationStart(Animation animation) 
            {
            }  
            public void onAnimationRepeat(Animation animation)
            {  
            }  
            public void onAnimationEnd(Animation animation) 
            {  
            	try 
            	{
					Thread.sleep(1000);
				} 
            	catch (InterruptedException e)
            	{
					e.printStackTrace();
				}
            	m_curFlashIndex++;
            	start(callback);
            }
        });
	}
	//------------------------------------------------------------------------------
}
