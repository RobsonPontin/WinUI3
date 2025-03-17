#pragma once

#include <queue>
#include <functional>
#include <mutex>

#include "PlaygroundExport.h"
#include "ITask.h"

namespace Playground::Utils
{
	struct TaskRunner
	{
		/// <summary>
		/// Add a task to the Queue to be executed
		/// </summary>
		PLAYGROUND_UTILS_API void AddTask(const std::function<bool()>& task);

		/// <summary>
		/// Add a task to a priority queue, sorted as min-heap.
		/// </summary>
		PLAYGROUND_UTILS_API void AddTask(std::shared_ptr<ITask> task);

		/// <summary>
		/// Run queue in sync on current calling thread.
		/// </summary>
		PLAYGROUND_UTILS_API void Run();

		void Shutdown();

	private:
		std::priority_queue<std::shared_ptr<ITask>, std::vector<std::shared_ptr<ITask>>, ITaskComparator> m_priorityQueueTasks;
		std::queue<std::function<bool()>> m_queueTasks;
		std::mutex m_mutex;
		std::mutex m_mutexPq;
		std::thread m_workerThread;
		std::thread m_workerThreadPriorityQueue;
		bool m_isInit{ false };

		// Used to sync with mutex and block thread from proceeding and then
		// once a thread finishes changes it will notify for next thread to go head
		std::condition_variable m_conditionVariable;
		std::condition_variable m_conditionVariablePq;

		void RunImpl();
		void ProcessPriorityQueue();
	};
}