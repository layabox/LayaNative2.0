package layaair.autoupdateversion;

public interface IUpdateCallback {  
    void checkUpdateCompleted(Boolean hasUpdate,
            CharSequence updateInfo);  

    void downloadProgressChanged(int progress);
    void downloadCanceled();
    void downloadCompleted(Boolean sucess, CharSequence errorMsg);
}  