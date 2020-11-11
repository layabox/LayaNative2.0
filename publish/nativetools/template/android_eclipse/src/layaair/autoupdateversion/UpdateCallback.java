package layaair.autoupdateversion;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.util.Log;

public class UpdateCallback implements IUpdateCallback {
	
	static private final String DIALOG_DOWNLOAD_ERROR_TITLE = "下载失败";
	static private final String DIALOG_DOWNLOAD_ERROR_MSG = "下载更新文件失败";
	static private final String DIALOG_DOWNLOAD_BUTTON_TRY = "重试";
	static private final String DIALOG_DOWNLOAD_BUTTON_CANCEL= "取消";
	
	static private final String DIALOG_UPDATE_TITLE = "更新";
	static private final String DIALOG_UPDATE_MSG = "立刻更新[";
	static private final String DIALOG_UPDATE_MSGEND = "]吗?";
	static private final String DIALOG_UPDATE_PROGRESS= "更新进度";
	static private final String DIALOG_UPDATE_BUTTON_TRY = "开始更新";
	static private final String DIALOG_UPDATE_BUTTON_CANCEL= "取消更新";
	
	ProgressDialog updateProgressDialog = null;
	public void downloadProgressChanged(int progress) {
		if (updateProgressDialog != null
				&& updateProgressDialog.isShowing()) {
			updateProgressDialog.setProgress(progress);
		}

	}

	public void downloadCompleted(Boolean sucess, CharSequence errorMsg) {
		if (updateProgressDialog != null
				&& updateProgressDialog.isShowing()) {
			updateProgressDialog.dismiss();
		}
		if (sucess) {
			if (AutoUpdateAPK.getInstance() != null)
			AutoUpdateAPK.getInstance().updateAPK();
		} else {
			if (AutoUpdateAPK.getInstance() == null)
				return ;
			DialogHelper.Confirm(AutoUpdateAPK.getInstance().getContext(),
					DIALOG_DOWNLOAD_ERROR_TITLE,
					DIALOG_DOWNLOAD_ERROR_MSG,
					DIALOG_DOWNLOAD_BUTTON_TRY,
					new DialogInterface.OnClickListener() {

						public void onClick(DialogInterface dialog,
								int which) {
							AutoUpdateAPK.getInstance().downloadAPK();

						}
					}, DIALOG_DOWNLOAD_BUTTON_CANCEL, null);
		}
	}

	public void downloadCanceled() {
		Log.i("", "download canceled");
	}

	public void checkUpdateCompleted(Boolean hasUpdate,	CharSequence updateInfo) {
		if (AutoUpdateAPK.getInstance() == null)
			return ;
		if (hasUpdate) 
		{
			DialogHelper.Confirm(
					AutoUpdateAPK.getInstance().getContext(),
					DIALOG_UPDATE_TITLE,
					DIALOG_UPDATE_MSG + updateInfo+ DIALOG_UPDATE_MSGEND,DIALOG_UPDATE_BUTTON_TRY,
					new DialogInterface.OnClickListener() {

						public void onClick(DialogInterface dialog,	int which) {
							updateProgressDialog = new ProgressDialog(AutoUpdateAPK.getInstance().getContext());
							updateProgressDialog.setMessage(DIALOG_UPDATE_PROGRESS);
							updateProgressDialog.setIndeterminate(false);
							updateProgressDialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
							updateProgressDialog.setMax(100);
							updateProgressDialog.setProgress(0);
							updateProgressDialog.setCancelable(false);
							updateProgressDialog.setCanceledOnTouchOutside(false);
							updateProgressDialog.show();

							AutoUpdateAPK.getInstance().downloadAPK();
						}
					}, DIALOG_UPDATE_BUTTON_CANCEL,
					new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialog,	int which) 
						{
								AutoUpdateAPK.onUpdateEnd(3);
						}
					}
					);
		}else{
			AutoUpdateAPK.onUpdateEnd(2);
		}
	}
}
