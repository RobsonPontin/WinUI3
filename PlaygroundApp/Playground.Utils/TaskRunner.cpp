#include "pch.h"
#include "TaskRunner.h"

#include <future>
#include <unordered_set>

#include "ITask.h"

namespace Playground::Utils
{
	void TaskRunner::AddTask(const std::function<bool()>& task)
	{
		std::lock_guard<std::mutex> lock_guard(m_mutex);
		m_queueTasks.push(task);
		m_conditionVariable.notify_one();
	}

	void TaskRunner::AddTask(std::shared_ptr<ITask> task)
	{
		std::lock_guard<std::mutex> lock_guard(m_mutexPq);
		m_priorityQueueTasks.push(task);
		m_conditionVariablePq.notify_one();
	}

	void TaskRunner::RunImpl()
	{
		while (m_isInit)
		{
			std::function<bool()> task;
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				m_conditionVariable.wait(lock, [this]
					{
						return !m_queueTasks.empty();
					});

				task = m_queueTasks.front();
				m_queueTasks.pop();
			}

			std::future<bool> result = std::async(std::launch::async, task);
			result.get();
		}
	}

	void TaskRunner::ProcessPriorityQueue()
	{
		while (m_isInit)
		{
			std::unique_lock<std::mutex> lock(m_mutexPq);
			m_conditionVariablePq.wait(lock, [this]
				{
					return !m_priorityQueueTasks.empty();
				});

			auto task = m_priorityQueueTasks.top();
			m_priorityQueueTasks.pop();

			// NOTE: this is blocking
			task->Execute();
		}
	}

	void TaskRunner::Run()
	{
		if (!m_isInit)
		{
			m_isInit = true;

			m_workerThread = std::thread(&TaskRunner::RunImpl, this);
			m_workerThread.detach();

			m_workerThreadPriorityQueue = std::thread(&TaskRunner::ProcessPriorityQueue, this);
			m_workerThreadPriorityQueue.detach();
		}
	}

	void TaskRunner::Shutdown()
	{
		m_isInit = false;
	}
}