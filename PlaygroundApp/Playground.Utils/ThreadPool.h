#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "PlaygroundExport.h"

namespace Playground::Utils
{
	struct ThreadPool
	{
		PLAYGROUND_UTILS_API ThreadPool(int size);
		PLAYGROUND_UTILS_API ~ThreadPool();

		PLAYGROUND_UTILS_API void Enqueue(std::function<bool()> job);

	private:
		std::vector<std::jthread> m_threadPool;
		std::condition_variable m_condition_variable;
		std::mutex m_mutex;
		std::queue<std::function<bool()>> m_jobs;
		std::atomic<bool> m_shutdownRequest;

		void Run();
	};

}