
#pragma once

#include <functional>
#include <future>
#include <queue>
#include <mutex>
#include <string>
#include <thread>

#include "PlaygroundExport.h"

namespace Playground::Utils
{
	struct FileReader
	{
		PLAYGROUND_UTILS_API FileReader();

		PLAYGROUND_UTILS_API ~FileReader();

		PLAYGROUND_UTILS_API void ReadFileNameAsync(std::wstring_view file_path);
		PLAYGROUND_UTILS_API std::wstring_view ReadFileName(std::wstring_view file_path);
		
		void RegisterOnFileNameReady(std::function<void(std::wstring)> callback)
		{
			std::lock_guard<std::mutex> lock_guard(m_mutex);
			m_callbackOnFileNameReady = callback;
		}

	private:
		std::mutex m_mutex;
		std::function<void(std::wstring)> m_callbackOnFileNameReady;

		// Request for promise and future
		std::jthread m_jThread;

		std::wstring m_filePathRequest;
		std::condition_variable m_conditionVariable;
		std::atomic<bool> m_shutdownRequest;

		void Run();
	};
}