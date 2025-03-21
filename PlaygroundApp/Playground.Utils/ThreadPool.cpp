
#include "pch.h"
#include "ThreadPool.h"

#include <assert.h>

namespace Playground::Utils
{
	ThreadPool::ThreadPool(int size)
	{
		if (size == 0)
			assert(false);

		for (int i = 0; i < size; ++i)
		{
			m_threadPool.push_back(std::jthread(&ThreadPool::Run, this));
		}
	}

	ThreadPool::~ThreadPool()
	{
		// Call for all thread to quit
		m_shutdownRequest = true;
		m_condition_variable.notify_all();
	}

	void ThreadPool::Enqueue(std::function<bool()> job)
	{
		std::lock_guard<std::mutex> lock_guard(m_mutex);
		m_jobs.push(job);
		m_condition_variable.notify_one();
	}

	void ThreadPool::Run()
	{
		while (true)
		{
			std::function<bool()> job;
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				m_condition_variable.wait(lock, [this]() -> bool
					{
						if (m_shutdownRequest)
							return true; // At this point a shutdown has been request, so quit thread to allow join

						return !m_jobs.empty();
					});

				if (m_shutdownRequest)
					break;

				job = m_jobs.front();
				m_jobs.pop();
			}

			job();
		}
	}
}