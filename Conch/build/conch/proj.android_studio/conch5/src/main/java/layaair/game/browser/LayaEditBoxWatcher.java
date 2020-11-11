package layaair.game.browser;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.KeyEvent;
import android.widget.TextView;

public class LayaEditBoxWatcher implements TextWatcher,TextView.OnEditorActionListener
{
	private CharSequence m_pTemp = null;
	private int m_nEditStart = 0;
	private int m_nEditEnd = 0;
	public LayaEditBox m_pEditBox = null;
	public String m_sOldBuffer = ""; 
	//-------------------------------------------------------------------
	public LayaEditBoxWatcher( LayaEditBox p_pEditBox )
	{
		m_pEditBox = p_pEditBox;
	}
	
	public void destory()
	{
		m_pEditBox=null;
	}
	//-------------------------------------------------------------------
    public void afterTextChanged( Editable s ) 
    {    
    	if( m_pEditBox.m_pEditBox == null ) return;
    	m_nEditStart = m_pEditBox.m_pEditBox.getSelectionStart();
    	m_nEditEnd = m_pEditBox.m_pEditBox.getSelectionEnd();
    	int nLength = m_pTemp.length();
      	if ( nLength > 0 )
        {
			if( m_pEditBox.m_bForbidEdit )
			{
				if( s.toString().equals(m_pEditBox.m_sValues) == false )
				{
					m_pEditBox.m_pEditBox.setText(m_pEditBox.m_sValues);
				}
			}
        	else if(m_pEditBox.GetRegular()!=null&&m_nEditStart>0)
        	{
        		String sBuffer = "" + m_pTemp.charAt(m_nEditStart-1);
                //判断是否合法
               	if(!m_pEditBox.IsInputValid(sBuffer))
			   	{
        	         s.delete(m_nEditStart-1, m_nEditStart);
        	         m_pEditBox.m_pEditBox.setText(s);
               	}
               	m_pEditBox.m_pEditBox.setSelection(m_nEditStart);
				/*
				m_pEditBox.m_pEditBox.setText(s);
  	         	m_pEditBox.m_pEditBox.setSelection(m_nEditStart);
  	         	*/
        	}
        }
		if( m_pEditBox.m_bForbidEdit == false) {
			if (m_sOldBuffer.compareTo(s.toString()) != 0)
				m_pEditBox.onChanged(s.toString());
		}
      }
    //-------------------------------------------------------------------
    public void beforeTextChanged(CharSequence s, int start, int count, int after) 
    {
    	m_sOldBuffer = m_pEditBox.m_pEditBox.getText().toString();
    	m_pTemp = s;
    }
    //-------------------------------------------------------------------
    public void onTextChanged( CharSequence s, int start, int before, int count) 
    {
    }
    public boolean onEditorAction(TextView paramTextView, int paramInt, KeyEvent paramKeyEvent)
    {
    	Log.e("input", ">>>>onEditor"+paramInt);
    	LayaEditBox.hideSoftKeyBorad();
    	if(paramInt==KeyEvent.KEYCODE_ENDCALL)
    	{
    		ConchJNI.inputChange(paramInt);
    	}
    	return false;
    }
    //-------------------------------------------------------------------
}
