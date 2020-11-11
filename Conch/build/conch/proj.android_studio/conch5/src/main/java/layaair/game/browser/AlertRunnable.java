package layaair.game.browser;

import android.content.Context;

import layaair.game.utility.Utils;

public class AlertRunnable implements Runnable {
    public  Context m_context;
    public  String					m_sAlertMsg = "";
    public	 int					m_nAlertCallbackType = 0;
    public  String					m_sAlertTitle = "";
    public void run() {
        ExportJavaFunction expjava = ExportJavaFunction.GetInstance();
        if (expjava != null) {
            expjava.m_pEngine.alertJS(m_sAlertTitle, m_sAlertMsg, m_nAlertCallbackType);
        }
    }
}
