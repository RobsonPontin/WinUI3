#pragma once

#include "RunningProcess.h"
#include <pplawait.h>

namespace Multiprocess
{
	namespace Core 
	{
		struct ProcessManager
		{
			ProcessManager();

			void CreateBackgroundService();

			/// <summary>
			/// Triggered once the process has succefully launched.
			/// </summary>
			/// <param name="shutdownCallback">The process that is now running.</param>
			void SetProcessStartedCallback(std::function<void(std::shared_ptr<RunningProcess> process)> shutdownCallback);

			/// <summary>
			/// It will be called once the process has been shutdown.
			/// </summary>
			/// <param name="shutdownCallback">The process that was terminated.</param>
			void SetProcessShutdownCallback(std::function<void(std::shared_ptr<RunningProcess> process)> shutdownCallback);

			std::shared_ptr<std::vector<std::shared_ptr<RunningProcess>>> RunningProcesses()
			{
				return m_activeProcesses;
			}

		private:
			std::mutex m_mutex;

			std::shared_ptr<std::vector<std::shared_ptr<RunningProcess>>> m_requestedProcesses;
			std::shared_ptr<std::vector<std::shared_ptr<RunningProcess>>> m_activeProcesses;

			std::function<void(std::shared_ptr<RunningProcess> process)> m_processStartedCallback;
			std::function<void(std::shared_ptr<RunningProcess> process)> m_processShutdownCallback;
		};
	}
}