package layaair.autoupdateversion.data;

public class VersionData {
	private int m_iVersionCode;
	private String m_szName;
	private String m_szVersion;
	private String m_szDownloasURL;

	public int getVersionCode() {
		return m_iVersionCode;
	}

	public void setVersionCode(int p_iVersionCode) {
		this.m_iVersionCode = p_iVersionCode;
	}

	public String getName() {
		return m_szName;
	}

	public void setName(String p_szName) {
		this.m_szName = p_szName;
	}

	public String getDownloasURL() {
		return m_szDownloasURL;
	}

	public void setDownloasURL(String p_szDownloasURL) {
		this.m_szDownloasURL = p_szDownloasURL;
	}

	public String getVersion() {
		return m_szVersion;
	}

	public void setVersion(String m_szVersion) {
		this.m_szVersion = m_szVersion;
	}

}
