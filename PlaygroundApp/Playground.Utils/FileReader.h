
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
		PLAYGROUND_UTILS_API void ReadFileNameAsync2(std::wstring_view file_path);
		PLAYGROUND_UTILS_API std::wstring_view ReadFileName(std::wstring_view file_path);
		
		void RegisterOnFileNameReady(std::function<void(std::wstring)> callback)
		{
			std::lock_guard<std::mutex> lock_guard(m_mutex);
			m_callbackOnFileNameReady = callback;
		}

	private:
		std::mutex m_mutex;
		std::mutex m_mutex2;
		std::function<void(std::wstring)> m_callbackOnFileNameReady;

		// Request for promise and future
		std::jthread m_jThread;
		std::jthread m_jthread2;

		std::queue<std::wstring> m_filesQueue;
		std::queue<std::wstring> m_filesQueue2;
		std::condition_variable m_conditionVariable;
		std::condition_variable m_conditionVariable2;
		std::atomic<bool> m_shutdownRequest;

		/// <summary>
		/// It runs the thread loop and process requests with std::async and std::future
		/// </summary>
		void Run();

		/// <summary>
		/// It runs the thread loop and process requests with std::promise and std::future
		/// </summary>
		void Run2();
	};
}