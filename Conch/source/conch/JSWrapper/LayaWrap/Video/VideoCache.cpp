#include "VideoCache.h"
#include <downloadMgr/JCDownloadMgr.h>
#include <resource/JCFileResManager.h>
#include "../../../JCScriptRuntime.h"
#include <fileSystem/JCFileSystem.h>
#include <downloadCache/JCServerFileCache.h>


namespace laya
{
	enum UrlProtocol {
		File,
		Http,
		Https,
		Unknown,
	};

	struct UrlPair
	{
		const char* name;
		UrlProtocol protocol;
	};

	static UrlProtocol GetUrlProtocol(const std::string& url)
	{
		static UrlPair urlPairs[] =
		{
			{ "file://", UrlProtocol::File },
			{ "http://", UrlProtocol::Http },
			{ "https://", UrlProtocol::Https },
		};

		static const int urlPairNum = 3;

		for (int i = 0; i < urlPairNum; i++)
		{
			int pos = url.find_first_of(urlPairs[i].name);
			if (pos != std::string::npos)
				return urlPairs->protocol;
		}

		return UrlProtocol::Unknown;
	}

	std::map<std::string, std::string> VideoCache::ms_cachePathMap = std::map<std::string, std::string>();

	VideoCache::VideoCache()
	{
		m_isDownloading = false;
		m_onLastFileDownloaded = nullptr;
		m_callbackRef.reset(new int(1));

	}

	bool VideoCache::IsDownloaded(const std::string& url)
	{
		auto it = ms_cachePathMap.find(url);
		if (it == ms_cachePathMap.end())
			return false;

		const std::string& filePathUrl = it->second;
		auto protocol = GetUrlProtocol(url);
		if (protocol != UrlProtocol::File)
			return true;

		int pos = filePathUrl.find_first_of("://");
		const std::string& filePath = filePathUrl.substr(pos + 3);
		if (!fs::exists(filePath))
		{
			ms_cachePathMap.erase(it);
			return false;
		}

		return true;
	}
	const std::string& VideoCache::GetCacheFilePath(const std::string& url)
	{
		auto it = ms_cachePathMap.find(url);
		if (it == ms_cachePathMap.end())
			return "";
		else
			return it->second;
	}

	void VideoCache::DownloadFile(const std::string& url)
	{
		if (IsDownloaded(url))
			return;

		if (m_isDownloading)
		{
			m_pendingUrls.push(url);
			return;
		}

		m_isDownloading = true;
		m_curDownloadUrl = url;

		laya::JCFileRes* res = JCScriptRuntime::s_JSRT->m_pFileResMgr->getRes(m_curDownloadUrl);
		std::weak_ptr<int> cbref(m_callbackRef);
		res->setOnReadyCB(std::bind(&VideoCache::onDownloaded, this, std::placeholders::_1, cbref));
		res->setOnErrorCB(std::bind(&VideoCache::onDownloadErr, this, std::placeholders::_1, std::placeholders::_2, cbref));

	}

	void VideoCache::CancelDownload()
	{
		m_onLastFileDownloaded = nullptr;
		std::queue<std::string> empty;
		m_pendingUrls.swap(empty);
	}


	void VideoCache::OnLastFileDownloaded(std::function<void()> callback)
	{
		m_onLastFileDownloaded = callback;
	}

	bool VideoCache::onDownloaded(void* p_pRes, std::weak_ptr<int> callbackref)
	{
		if (!callbackref.lock()) return false;
		laya::JCResStateDispatcher* pRes = (laya::JCResStateDispatcher*)p_pRes;
		laya::JCFileRes* pFileRes = (laya::JCFileRes*)pRes;
		if (pFileRes->m_pBuffer.get() == NULL || pFileRes->m_nLength == 0)
		{
			return false;
		}
		JCBuffer p_buf;
		p_buf.m_pPtr = pFileRes->m_pBuffer.get();
		p_buf.m_nLen = pFileRes->m_nLength;

		std::map<std::string, std::string>::iterator iter = ms_cachePathMap.find(m_curDownloadUrl);
		std::string localFilePath;
		std::string localFilePathUrl;
		if (iter != ms_cachePathMap.end())
		{
			localFilePath = iter->second;
		}
		else
		{
			int p1 = m_curDownloadUrl.rfind('/');
			int p2 = m_curDownloadUrl.rfind('\\');
			int pos = std::max<int>(p1, p2);
			std::string mediaFile = m_curDownloadUrl.substr(pos + 1, m_curDownloadUrl.length());
			//去掉?后面的，因为可能增加版本号
			int p3 = mediaFile.rfind('?');
			char* sT = (char*)(mediaFile.c_str());
			if (p3 != -1)
			{
				sT[p3] = 0;
			}
			unsigned int hash = JCCachedFileSys::hashRaw(m_curDownloadUrl.c_str());
			char tmpBuf[32];
			sprintf(tmpBuf, "%x_", hash);
			localFilePath = JCScriptRuntime::s_JSRT->m_pFileResMgr->m_pFileCache->getAppPath() + "/" + tmpBuf + mediaFile;
			writeFileSync(localFilePath.c_str(), p_buf);

			localFilePathUrl = "file://" + localFilePath;
			ms_cachePathMap[m_curDownloadUrl] = localFilePathUrl;
		}

		if (m_pendingUrls.size() > 0)
		{
			const std::string nextUrl = m_pendingUrls.front();
			m_pendingUrls.pop();
			DownloadFile(nextUrl);
		}
		else if(m_onLastFileDownloaded)
		{
			m_onLastFileDownloaded();
			m_onLastFileDownloaded = nullptr;
		}

		return true;
	}

	bool VideoCache::onDownloadErr(void* p_pRes, int p_nErrCode, std::weak_ptr<int> callbackref)
	{
		if (!callbackref.lock())return false;
		return true;
	}

}