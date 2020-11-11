#pragma once

#include <string>
#include <map>
#include <queue>
#include <memory>
#include <functional>

namespace laya
{
	class VideoCache
	{
	public:
		VideoCache();
		~VideoCache() {}

		bool IsDownloaded(const std::string& url);
		const std::string& GetCacheFilePath(const std::string& url);
		void DownloadFile(const std::string& url);
		void OnLastFileDownloaded(std::function<void()> callback);
		void CancelDownload();

	private:
		bool onDownloaded(void* p_pRes, std::weak_ptr<int> callbackref);
		bool onDownloadErr(void* p_pRes, int p_nErrCode, std::weak_ptr<int> callbackref);

	private:
		static std::map<std::string, std::string> ms_cachePathMap;

		std::function<void()> m_onLastFileDownloaded;
		std::shared_ptr<int>	m_callbackRef;
		std::queue<std::string> m_pendingUrls;
		std::string m_curDownloadUrl;
		bool m_isDownloading;
	};
};