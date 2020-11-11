package layaair.game.browser;

import android.annotation.TargetApi;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.graphics.PixelFormat;
import layaair.game.browser.GLSurfaceView;
import android.os.Build;
import android.util.Log;
import android.view.Choreographer;
import android.view.MotionEvent;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

import layaair.game.conch.LayaConch5;


public class ConchCanvas extends GLSurfaceView// implements Choreographer.FrameCallback
{	
    //public 		static	MainCanvas 	ms_pCanvas = null;
    private 	static	String 		TAG = "MainCanvas";
	protected 	boolean 			m_bReleasing = false;
	public 		TouchFilter			m_TouchFilter = new TouchFilter();
    private     Renderer render;
    private     Choreographer       mChoreographer=null;
    //public      boolean             mbUseChoreographer=false;
    public      int                 m_nDisplayWidth=0;
    public      int                 m_nDisplayHeight=0;
    public      int                 m_nFixedWidth=0;
    public      int                 m_nFixedHeight=0;
	//------------------------------------------------------------------------------
    private static class ContextFactory implements GLSurfaceView.EGLContextFactory 
    {
        private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
        public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig)
        {
            checkEglError("Before eglCreateContext", egl);
            int[] attrib_list = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE };
            EGLContext context = egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attrib_list);
            checkEglError("After eglCreateContext", egl);
            return context;
        }
        public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context) 
        {
            egl.eglDestroyContext(display, context);
        }
    }
    //------------------------------------------------------------------------------
    private static void checkEglError(String prompt, EGL10 egl) 
    {
        int error;
        while ((error = egl.eglGetError()) != EGL10.EGL_SUCCESS) 
        {
            Log.e(TAG, String.format("%s: EGL error: 0x%x", prompt, error));
        }
    }
    //------------------------------------------------------------------------------
    public ConchCanvas(Context context)
    {
        super(context);
        init(false, 24, 8);
        //ms_pCanvas=this;
    }

    /*@TargetApi(16)
    public void doFrame(long var1){
        if(mbUseChoreographer) {
            if (mChoreographer != null)
                mChoreographer.postFrameCallback(this);
            ConchJNI.onVSyncCallback(var1);
        }
    }*/

    public  void destory()
    {
        this.m_TouchFilter=null;
    }


    public void captureScreen()
    {
        if(render!=null)
        {
            render.captureScreen();
        }
    }

    //------------------------------------------------------------------------------
    private void init(boolean translucent, int depth, int stencil)
    {
        int androidver = Build.VERSION.SDK_INT;
        //if(androidver>=24){
            //这个用native接口
        //}else if(androidver>=16) {
        /*if(androidver>=16) {    //改成全部都用java的
            mChoreographer = Choreographer.getInstance();
            mChoreographer.postFrameCallback(this);
        }*/

        if (translucent) 
        {
            this.getHolder().setFormat(PixelFormat.TRANSLUCENT);
        }
        setPreserveEGLContextOnPause(true);
        //setEGLContextFactory(new ContextFactory());
        int reportedVersion = getVersionFromActivityManager(getContext());

        if (getMajorVersion(reportedVersion) == 3)
        {
            Log.i(TAG, "Android OpenGL ES Major Version 3");
            setEGLContextClientVersion(3);
        }
        else
        {
            Log.i(TAG, "Android OpenGL ES Major Version 2");
            setEGLContextClientVersion(2);
        }
        setEGLConfigChooser(new ConfigChooser(8, 8, 8, 8, depth, stencil));
        render=new Renderer();
        render.pCanvas = this;
        setRenderer(render);
    }
    private static int getVersionFromActivityManager(Context context) {
        ActivityManager activityManager =
                (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo configInfo = activityManager.getDeviceConfigurationInfo();
        if (configInfo.reqGlEsVersion != ConfigurationInfo.GL_ES_VERSION_UNDEFINED) {
            return configInfo.reqGlEsVersion;
        } else {
            return 1 << 16; // Lack of property means OpenGL ES version 1
        }
    }
    private static int getMajorVersion(int glEsVersion) {
        return ((glEsVersion & 0xffff0000) >> 16);
    }
    public void setFixedSize(int w, int h){
        Log.i("fix","setFixedSize "+w+","+h+",scr:"+m_nDisplayWidth+","+m_nDisplayHeight);
        if(w<=0||h<=0)
            return;
        getHolder().setFixedSize(w,h);
        if(LayaConch5.ms_layaConche.getHorizontalScreen()){
            if(w<h){
                Log.e("LayaBox","setFixedSize error! screen orientation = landscape  w should bigger than h");
            }
            m_TouchFilter.m_fScaleX = ((float)w)/Math.max(m_nDisplayWidth,m_nDisplayHeight);
            m_TouchFilter.m_fScaleY = ((float)h)/Math.min(m_nDisplayWidth,m_nDisplayHeight);
            LayaConch5.ms_layaConche.m_pEditBox.m_fPostScaleX = 1.0f/m_TouchFilter.m_fScaleX;
            LayaConch5.ms_layaConche.m_pEditBox.m_fPostScaleY = 1.0f/m_TouchFilter.m_fScaleY;
        }else{
            if(w>h){
                Log.e("LayaBox","setFixedSize error! screen orientation = portrait  w should less than h");
            }
            m_TouchFilter.m_fScaleX = ((float)w)/Math.min(m_nDisplayWidth,m_nDisplayHeight);
            m_TouchFilter.m_fScaleY = ((float)h)/Math.max(m_nDisplayWidth,m_nDisplayHeight);
            LayaConch5.ms_layaConche.m_pEditBox.m_fPostScaleX = 1.0f/m_TouchFilter.m_fScaleX;
            LayaConch5.ms_layaConche.m_pEditBox.m_fPostScaleY = 1.0f/m_TouchFilter.m_fScaleY;
        }
        m_nFixedWidth=w;
        m_nFixedHeight=h;
    }
    //------------------------------------------------------------------------------
    public boolean onTouchEvent(MotionEvent event)
    {
    	if(m_bReleasing ) return true;
    	super.onTouchEvent(event);
    	if( event!=null)
    	{
	    	m_TouchFilter.onTouchEvent(event);
    	}
    	return true;
    }

    public void onResume() {
        super.onResume();
        /*if (mChoreographer != null)
            mChoreographer.postFrameCallback(this);*/
    }
    public void onPause(){
        super.onPause();
        /*if(mChoreographer!=null){
            mChoreographer.removeFrameCallback(this);
        }*/
    }
    //------------------------------------------------------------------------------
    private static class ConfigChooser implements GLSurfaceView.EGLConfigChooser 
    {
        //------------------------------------------------------------------------------
        public ConfigChooser(int r, int g, int b, int a, int depth, int stencil) 
        {
            mRedSize = r;
            mGreenSize = g;
            mBlueSize = b;
            mAlphaSize = a;
            mDepthSize = depth;
            mStencilSize = stencil;
        }
        //------------------------------------------------------------------------------
        private static int EGL_OPENGL_ES2_BIT = 4;
        //------------------------------------------------------------------------------
        private static int[] s_configAttribs2 =
        {
            EGL10.EGL_RED_SIZE, 4,
            EGL10.EGL_GREEN_SIZE, 4,
            EGL10.EGL_BLUE_SIZE, 4,
            EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL10.EGL_NONE
        };
        //------------------------------------------------------------------------------
        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display)
        {
            int[] num_config = new int[1];
            egl.eglChooseConfig(display, s_configAttribs2, null, 0, num_config);
            int numConfigs = num_config[0];
            if (numConfigs <= 0)
            {
                throw new IllegalArgumentException("No configs match configSpec");
            }
            EGLConfig[] configs = new EGLConfig[numConfigs];
            egl.eglChooseConfig(display, s_configAttribs2, configs, numConfigs, num_config);
            return chooseConfig(egl, display, configs);
        }
        //------------------------------------------------------------------------------
        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display,EGLConfig[] configs)
        {
            for(EGLConfig config : configs) 
            {
                int d = findConfigAttrib(egl, display, config,EGL10.EGL_DEPTH_SIZE, 0);
                int s = findConfigAttrib(egl, display, config,EGL10.EGL_STENCIL_SIZE, 0);
                if (d < mDepthSize || s < mStencilSize)continue;
                int r = findConfigAttrib(egl, display, config,EGL10.EGL_RED_SIZE, 0);
                int g = findConfigAttrib(egl, display, config, EGL10.EGL_GREEN_SIZE, 0);
                int b = findConfigAttrib(egl, display, config,EGL10.EGL_BLUE_SIZE, 0);
                int a = findConfigAttrib(egl, display, config,EGL10.EGL_ALPHA_SIZE, 0);
                if (r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize)return config;
            }
            return null;
        }
        //------------------------------------------------------------------------------
        private int findConfigAttrib(EGL10 egl, EGLDisplay display,EGLConfig config, int attribute, int defaultValue)
        {
            if (egl.eglGetConfigAttrib(display, config, attribute, mValue)) 
            {
                return mValue[0];
            }
            return defaultValue;
        }
        //------------------------------------------------------------------------------
        protected int mRedSize;
        protected int mGreenSize;
        protected int mBlueSize;
        protected int mAlphaSize;
        protected int mDepthSize;
        protected int mStencilSize;
        private int[] mValue = new int[1];
        
    }
    //------------------------------------------------------------------------------
    private static class Renderer implements GLSurfaceView.Renderer 
    {
    	private GL10 m_lastgl=null;
    	private int m_nlastW=0,m_nlastH=0;
        private boolean m_bTakeScreenshot;
        public ConchCanvas pCanvas=null;
        //------------------------------------------------------------------------------
        public void onDrawFrame(GL10 gl) 
        {
        	//Log.e("", "usedMem:"+ProcessInfo.GetUsedMem());
        	//ProcessInfo.GetTotalMem();
        	ConchJNI.onDrawFrame();
            if(m_bTakeScreenshot) {
                LayaConch5.GetInstance().dispatchGL(gl, m_nlastW, m_nlastH);
                m_bTakeScreenshot=false;
            }

        }
        public  void captureScreen()
        {
            m_bTakeScreenshot=true;
        }
        //------------------------------------------------------------------------------
        public void onSurfaceChanged(GL10 gl, int width, int height) 
        {
        	if (LayaConch5.ms_layaConche != null)
        	{
                /*if(pCanvas!=null){
                    pCanvas.doFrame(System.nanoTime());
                }*/

                boolean isHorizontion = LayaConch5.ms_layaConche.getHorizontalScreen();
        		if (isHorizontion&& width<height) 
        		{
					m_nlastW=height;
		        	m_nlastH=width;
		        	m_lastgl=gl;
		        	Log.e("",">>>>>>>>>>>>surfaceChangedhw w="+m_nlastW+",h="+m_nlastH+" gl="+gl);
		    		ConchJNI.OnGLReady(m_nlastW, m_nlastH);
					return;
				}
			}
        	else
        	{
        		Log.e("",">>>>>>>>>>>>>surface not ready");
        		return;
        	}
        	
        	if(width==m_nlastW && height==m_nlastH && gl==m_lastgl )return;
        	m_nlastW=width;
        	m_nlastH=height;
        	m_lastgl=gl;
        	Log.e("",">>>>>>>>>>>>surfaceChanged w="+width+",h="+height+" gl="+gl);
    		ConchJNI.OnGLReady(width, height);
        }
        //------------------------------------------------------------------------------
        public void onSurfaceCreated(GL10 gl, EGLConfig config) 
        {
        }
        //------------------------------------------------------------------------------
    }
    //------------------------------------------------------------------------------
}
